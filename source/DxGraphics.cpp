// ----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�`��v���O����
// 
// 				Ver 3.16d
// 
// ----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "DxGraphics.h"

#ifndef DX_NON_GRAPHICS

// �C���N���[�h----------------------------------------------------------------
#include "DxLib.h"
#include "DxStatic.h"
#include "DxFile.h"
#include "DxSound.h"
#include "DxGraphicsFilter.h"
#include "DxMask.h"
#include "DxMemImg.h"
#include "DxMath.h"
#include "DxFont.h"
#include "DxLog.h"
#include "DxASyncLoad.h"
#include "DxUseCLib.h"
#include "DxSoftImage.h"

#include "DxModel.h"

#include "DxBaseImage.h"
#include "DxMovie.h"
#include "DxInput.h"
#include "DxBaseFunc.h"
#include "DxSystem.h"
#include "DxMemory.h"

#ifdef __WINDOWS__
#include "Windows/DxGraphicsWin.h"
#endif // __WINDOWS__

#ifdef __psp2__
#include "PSVita/DxGraphicsPSVita.h"
#endif // __psp2__

#ifdef __ORBIS__
#include "PS4/DxGraphicsPS4.h"
#endif // __ORBIS__

#ifdef __ANDROID__
#include "Android/DxGraphicsAndroid.h"
#endif // __ANDROID__


#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------------------------------------

// �N���b�s���O�t���O
#define CLIP_XP							(0x01)				// X+�����ɃN���b�v
#define CLIP_XM							(0x02)				// X-�����ɃN���b�v
#define CLIP_YP							(0x04)				// Y+�����ɃN���b�v
#define CLIP_YM							(0x08)				// Y-�����ɃN���b�v
#define CLIP_ZP							(0x10)				// Z+�����ɃN���b�v
#define CLIP_ZM							(0x20)				// Z-�����ɃN���b�v

// �V���h�E�}�b�v�̏����[�x�␳�l
#define DEFAULT_SHADOWMAP_ADJUST_DEPTH		( 0.002f )
#define DEFAULT_SHADOWMAP_GRADATION_RANGE	( 0.0007f )
#define DEFAULT_SHADOWMAP_BLUR_PARAM		( 0 )

// �e�N�X�`���s�N�Z���t�H�[�}�b�g�̃r�b�g�f�v�X�C���f�b�N�X��`
#define TEX_BITDEPTH_16				(0)
#define TEX_BITDEPTH_32 			(1)
#define PIXELBYTE( x )				( x == TEX_BITDEPTH_16 ? 2 : 4 )

// �e�N�X�`���[�̕����L�����̍ő�T�C�Y�ƍŏ��T�C�Y
#define MIN_TEXTURE_SIZE			(1)

// �A���`�G�C���A�X�t���̐��`��p�̃e�N�X�`���̃T�C�Y
#define LINE_TEX_SIZE				(8)

// �A���`�G�C���A�X�t���̐���`�`��p�̃e�N�X�`���̃T�C�Y
#define LINEBOX_TEX_SIZE			(16)

// �~���`��������W�̍ő吔
#define CIRCLE_MAX_POSNUM			(256)

// �R�Q�r�b�g�F�J���[�l�̍쐬
#define SETUP32BITCOLOR( Color, FColor )		\
{\
	int r, g, b ;\
	DWORD NoneRevMask ;\
\
	GetColor_Macro( 0, 0, 0, NoneRevMask ) ;\
	NoneRevMask = ~NoneRevMask ;\
	GetColor2_Macro( Color, r,  g,  b  ) ;\
	if( ( GSYS.DrawSetting.bDrawBright & 0x00ffffff ) != 0x00ffffff )\
	{\
		RGBCOLOR *bright ;\
\
		bright = &GSYS.DrawSetting.DrawBright ;	\
		r  = bright->Red   * r / 255 ;\
		g  = bright->Green * g / 255 ;\
		b  = bright->Blue  * b / 255 ;\
		GetColor_Macro( r, g, b, Color ) ;\
	}\
	Color  &= NoneRevMask ;\
	FColor = ( 0xff000000 ) | ( r << 16 ) | ( g << 8 ) | ( b ) ;\
}

// �p���b�g������摜�̗L�����`�F�b�N
#define PALETTEIMAGECHK( HAND, MEMIMG )														\
{																							\
	IMAGEDATA *Image ;																	\
																							\
	if( GRAPHCHK( ( HAND ), Image ) )														\
		return -1 ;																			\
	if( Image->Orig->FormatDesc.TextureFlag ) return -1 ;									\
	MEMIMG = &Image->Soft.MemImg;															\
	if( MEMIMG->Base->UsePalette == 0 ) return -1;											\
}

// ���C�g�n���h���̗L�����`�F�b�N
#define LIGHTCHK( HAND, LPOINT, ERR )										\
	if( ( ( (HAND) & DX_HANDLEERROR_MASK ) != 0 ) ||											\
		( ( (HAND) & DX_HANDLETYPE_MASK ) != DX_HANDLETYPE_MASK_LIGHT ) ||							\
		( ( (HAND) & DX_HANDLEINDEX_MASK ) >= MAX_LIGHT_NUM ) ||								\
		( ( LPOINT = GSYS.Light.Data[ (HAND) & DX_HANDLEINDEX_MASK ] ) == NULL ) ||				\
		( (int)( (LPOINT)->ID << DX_HANDLECHECK_ADDRESS ) != ( (HAND) & DX_HANDLECHECK_MASK ) ) )\
	{																							\
		return (ERR) ;																			\
	}

// �ǂݍ��ݎ��̓��ߐF�ύX����
#define LUTRANS_START( Img )	\
	int _TransColor = GSYS.CreateImage.TransColor ; \
	if( GSYS.CreateImage.LeftUpColorIsTransColorFlag == TRUE )\
	{\
		GSYS.CreateImage.TransColor = NS_GetGraphImageFullColorCode( Img, 0, 0 ) & 0xffffff ;\
		BASEIM.TransColor = GSYS.CreateImage.TransColor ;\
	}

// �ǂݍ��ݎ��̓��ߐF�ύX����
#define LUTRANS_GPARAM( GParam, Img )	\
	if( GParam->LeftUpColorIsTransColorFlag == TRUE ) GParam->InitHandleGParam.TransColor = NS_GetGraphImageFullColorCode( Img, 0, 0 ) & 0xffffff ;

#define LUTRANS_RE_START( Img, GrH )		\
	LUTRANS_START( Img ) \

#define LUTRANS_RE_GPARAM( GParam, Img, GrH )		\
	LUTRANS_GPARAM( GParam, Img ) \

#define LUTRANS_RE_DIV_START( Img, GrH, Num )	\
	LUTRANS_START( Img ) \

#define LUTRANS_RE_DIV_GPARAM( GParam, Img, GrH, Num )	\
	LUTRANS_GPARAM( GParam, Img ) \

#define LUTRANS_END		\
	GSYS.CreateImage.TransColor = _TransColor ;	\
	BASEIM.TransColor = _TransColor ;

// ��Z�ς݃A���t�@�̃u�����h���[�h���ǂ����̎擾
#define IS_PMA_DRAWBLENDMODE			\
	( (	GSYS.DrawSetting.BlendMode == DX_BLENDMODE_PMA_ALPHA	||	\
		GSYS.DrawSetting.BlendMode == DX_BLENDMODE_PMA_ADD		||	\
		GSYS.DrawSetting.BlendMode == DX_BLENDMODE_PMA_SUB		||	\
		GSYS.DrawSetting.BlendMode == DX_BLENDMODE_PMA_INVSRC	||	\
		GSYS.DrawSetting.BlendMode == DX_BLENDMODE_PMA_ALPHA_X4	||	\
		GSYS.DrawSetting.BlendMode == DX_BLENDMODE_PMA_ADD_X4	) ? TRUE : FALSE )

// �\���̐錾 -----------------------------------------------------------------

// �摜�ǂݍ��ݏ����ɕK�v�ȕϐ���Z�߂�����
struct CREATEGRAPH_LOADBASEIMAGE_PARAM
{
	int						UseTempBaseImage ;
	int						LoadHr ;
	BASEIMAGE				TempRgbBaseImage ;
	BASEIMAGE				TempAlphaBaseImage ;
	BASEIMAGE *				UseRgbBaseImage ;
	BASEIMAGE *				UseAlphaBaseImage ;
} ;

// �l�̑傫�����W�l���g�����`��ɂ��v�Z���x�̒ቺ��}���鏈���ōs���s��ݒ�ŕK�v�ȏ��
struct LARGE3DPOSITIONSUPPORT_DRAWINFO
{
	MATRIX_D				BackupWorldMatrix ;
	MATRIX_D				BackupViewMatrix ;
} ;

// �f�[�^��` -----------------------------------------------------------------

// 8x8 �� tga�e�N�X�`��
static BYTE WhiteTga8x8TextureFileImage[ 84 ] = 
{
	0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00,
	0x20, 0x08, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF,
	0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0xFF,
	0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x54, 0x52, 0x55, 0x45, 0x56, 0x49, 0x53, 0x49, 0x4F, 0x4E, 0x2D, 0x58, 0x46, 0x49,
	0x4C, 0x45, 0x2E, 0x00
} ;

// 8x8 �� �A���`�G�C���A�X�t���̐��`��p�̉摜���
static BYTE LineTexTga8x8TextureImage[ 64 ] = 
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
} ;

// 16x16 �� �A���`�G�C���A�X�t���̐���`�`��p�̉摜���
static BYTE LineBoxTexTga16x16TextureImage[ 16 * 16 ] = 
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
} ;

// 8x8 �� �A���`�G�C���A�X�t���̐��`��p�̉摜���
static BYTE LineOneThicknessTexTga8x8TextureImage[ 64 ] = 
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
} ;

// 16x16 �� �A���`�G�C���A�X�t���̐���`�`��p�̉摜���
static BYTE LineBoxOneThicknessTexTga16x16TextureImage[ 16 * 16 ] = 
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0x00,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0x00,0x00,
	0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
} ;

// �`�����̊�{�I�ȏ��
GRAPHICSSYSTEMDATA GraphicsSysData ;

// �֐��錾 -------------------------------------------------------------------

// �摜�֌W�֐�
__inline static int		Graphics_Image_CheckBlendGraphSize( IMAGEDATA *GraphData ) ;		// �u�����h�摜�Ƃ̑傫���`�F�b�N
		 static void	Graphics_Image_DefaultRestoreGraphFunction( void ) ;				// �f�t�H���g�̃O���t�B�b�N�����֐�
		 static void	Graphics_Image_CreateGraph_LoadBaseImage(      LOADGRAPH_PARAM *Param, CREATEGRAPH_LOADBASEIMAGE_PARAM *LParam ) ;		// CreateGraph �� CreateDivGraph �̋��ʂ��� BASEIMAGE �\�z�������֐�����������
		 static void	Graphics_Image_CreateGraph_TerminateBaseImage( LOADGRAPH_PARAM *Param, CREATEGRAPH_LOADBASEIMAGE_PARAM *LParam ) ;		// CreateGraph �� CreateDivGraph �̋��ʂ��� BASEIMAGE ��n���������֐�����������







// ��ʊ֌W�֐�
static int Graphics_Screen_CheckDisplaySetting( int ScreenSizeX, int ScreenSizeY, int ColorBitDepth ) ;		// �w��̉𑜓x���Ή����Ă��邩�ǂ����𒲂ׂ�֐�







// �`��֌W�֐�
__inline static void	Graphics_Draw_VectorTransformToBillboard( VECTOR *Vector, VECTOR *Position ) ;										// �r���{�[�h�p�̍s��v�Z
		 static int		Graphics_Draw_ModiGraphBase( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int GrHandle, int TransFlag, bool SimpleDrawFlag ) ;
		 static int		Graphics_Draw_ModiGraphFBase( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int GrHandle, int TransFlag, bool SimpleDrawFlag ) ;
		 static void	Graphics_Draw_BeginLarge3DPositionSupportDraw( LARGE3DPOSITIONSUPPORT_DRAWINFO *DrawInfo, VECTOR_D BasePosition ) ;	// �l�̑傫�����W�l���g�����`��ɂ��v�Z���x�̒ቺ��}����ׂ̏������s��
		 static void	Graphics_Draw_EndLarge3DPositionSupportDraw( const LARGE3DPOSITIONSUPPORT_DRAWINFO *DrawInfo ) ;					// �l�̑傫�����W�l���g�����`��ɂ��v�Z���x�̒ቺ��}����ׂ̏����ōs�����ύX�����ɖ߂�








// �n�[�h�E�F�A�A�N�Z�����[�^�g�p�����_�����O�֌W�֐�
static int	Graphics_Hardware_DrawChipMap( int Sx, int Sy, int XNum, int YNum, const int *MapData, int MapDataPitch, int ChipTypeNum, const int *ChipGrHandle, int TransFlag ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawChipMap
static int	Graphics_Hardware_DrawTile( int x1, int y1, int x2, int y2, int Tx, int Ty, double ExtRate, double Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag ) ;	// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTile








// �\�t�g�E�G�A�����_�����O�֐�
static int	Graphics_Software_Initialize( void ) ;																										// �\�t�g�E�G�A�����_�����O�̏��������s��
static int	Graphics_Software_Terminate( void ) ;																										// �\�t�g�E�G�A�����_�����O�̌�n�����s��
static int  Graphics_Software_DrawGraph(            int x,  int y,                                                                  IMAGEDATA *Image, int TransFlag ) ;						// �\�t�g�E�G�A�����_�����O�� DrawGraph
static int  Graphics_Software_DrawExtendGraph(      int x1, int y1, int x2, int y2,                                                 IMAGEDATA *Image, int TransFlag ) ;						// �\�t�g�E�G�A�����_�����O�� DrawExtendGraph
static int  Graphics_Software_DrawRotaGraph(        float x, float y, double ExRate, double Angle,                                  IMAGEDATA *Image, int TransFlag, int TurnFlag ) ;		// �\�t�g�E�G�A�����_�����O�� DrawRotaGraph
static int  Graphics_Software_DrawRotaGraphFast(    float x, float y, float  ExRate, float  Angle,                                  IMAGEDATA *Image, int TransFlag, int TurnFlag ) ;		// �\�t�g�E�G�A�����_�����O�� DrawRotaGraphFast
static int  Graphics_Software_DrawPoly(             DX_POINTDATA *p,                                                                IMAGEDATA *Image, int TransFlag, unsigned int Color ) ;
static int  Graphics_Software_DrawModiGraph(        int   x1, int   y1, int   x2, int   y2, int   x3, int   y3, int   x4, int   y4, IMAGEDATA *Image, int TransFlag ) ;						// �\�t�g�E�G�A�����_�����O�� DrawModiGraph
static int  Graphics_Software_DrawModiGraphF(       float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, int TransFlag ) ;						// �\�t�g�E�G�A�����_�����O�� DrawModiGraphF
static int  Graphics_Software_DrawChipMap(          int Sx, int Sy, int XNum, int YNum, const int *MapData, int MapDataPitch, int ChipTypeNum, const int *ChipGrHandle, int TransFlag ) ;	// �\�t�g�E�G�A�����_�����O�� DrawChipMap
static int  Graphics_Software_DrawTile(             int x1, int y1, int x2, int y2, int Tx, int Ty, double ExtRate, double Angle,   IMAGEDATA *Image, int TransFlag ) ;						// �\�t�g�E�G�A�����_�����O�� DrawTile
static int  Graphics_Software_DrawFillBox(          int x1, int y1, int x2, int y2,                                 unsigned int Color ) ;						// �\�t�g�E�G�A�����_�����O�� DrawFillBox
static int  Graphics_Software_DrawLineBox(          int x1, int y1, int x2, int y2,                                 unsigned int Color ) ;						// �\�t�g�E�G�A�����_�����O�� DrawLineBox
static int  Graphics_Software_DrawLine(             int x1, int y1, int x2, int y2,                                 unsigned int Color ) ;						// �\�t�g�E�G�A�����_�����O�� DrawLine
static int  Graphics_Software_DrawCircle_Thickness( int x, int y, int r,                                            unsigned int Color, int Thickness ) ;		// �\�t�g�E�G�A�����_�����O�� DrawCircle( �����w�肠�� )
static int  Graphics_Software_DrawOval_Thickness(   int x, int y, int rx, int ry,                                   unsigned int Color, int Thickness ) ;		// �\�t�g�E�G�A�����_�����O�� DrawOval( �����w�肠�� )
static int  Graphics_Software_DrawCircle(           int x, int y, int r,                                            unsigned int Color, int FillFlag ) ;		// �\�t�g�E�G�A�����_�����O�� DrawCircle
static int  Graphics_Software_DrawOval(             int x, int y, int rx, int ry,                                   unsigned int Color, int FillFlag ) ;		// �\�t�g�E�G�A�����_�����O�� DrawOval
static int  Graphics_Software_DrawTriangle(         int x1, int y1, int x2, int y2, int x3, int y3,                 unsigned int Color, int FillFlag ) ;		// �\�t�g�E�G�A�����_�����O�� DrawTriangle
static int  Graphics_Software_DrawQuadrangle(       int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FillFlag ) ;		// �\�t�g�E�G�A�����_�����O�� DrawQuadrangle
static int  Graphics_Software_DrawPixel(            int x, int y,                                                   unsigned int Color ) ;						// �\�t�g�E�G�A�����_�����O�� DrawPixel
static int  Graphics_Software_DrawPixelSet(         const POINTDATA *PointData, int Num ) ;																// �\�t�g�E�G�A�����_�����O�� DrawPixelSet
static int  Graphics_Software_DrawLineSet(          const LINEDATA  *LineData,  int Num ) ;																// �\�t�g�E�G�A�����_�����O�� DrawLineSet



__inline	static DWORD GetDiffuseColor( void )															// ���݂̃f�B�t���[�Y�J���[�𓾂�
			{
				return  ( ( DWORD )GSYS.DrawSetting.DrawBright.Red   << 16 ) |
						( ( DWORD )GSYS.DrawSetting.DrawBright.Green << 8  ) |
						( ( DWORD )GSYS.DrawSetting.DrawBright.Blue        ) |
						( ( ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND || GSYS.DrawSetting.UseNoBlendModeParam ) ? ( DWORD )GSYS.DrawSetting.BlendParam : 255 ) << 24 ) ;
			}

// �v���O���� -----------------------------------------------------------------

// �摜�֌W�֐�

// �u�����h�摜�Ƃ̑傫���`�F�b�N
__inline static int Graphics_Image_CheckBlendGraphSize( IMAGEDATA *GraphData )
{
	IMAGEDATA *BlendData ;

	// �n�[�h�E�G�A���g�p����ꍇ�͑傫���̐�������
	if( GSYS.Setting.ValidHardware )
		return 0 ;

	// �\�t�g�E�G�A�̏ꍇ�̓u�����h�摜���W���[�h�� DX_BLENDGRAPH_POSMODE_DRAWGRAPH �̂ݑΉ�
	if( GSYS.DrawSetting.BlendGraphPosMode != DX_BLENDGRAPH_POSMODE_DRAWGRAPH )
		return -1 ;

	if( GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendData ) )
		return -1 ;

	// �u�����h�摜�����傫��������A�E�g
	return ( ( BlendData->WidthI  - GraphData->WidthI  ) |
		     ( BlendData->HeightI - GraphData->HeightI ) ) & 0x80000000 ? -1 : 0 ;
}

// �f�t�H���g�̃O���t�B�b�N�����֐�
static void Graphics_Image_DefaultRestoreGraphFunction( void )
{
	IMAGEDATA *Image ;
	IMAGEDATA *FileBackImage ;
	IMAGEDATA *MemBackImage ;
	int         i ;
	int         ImageDataArea ;
	BASEIMAGE   FileRgbImage ;
	BASEIMAGE   FileAlphaImage ;
	int         FileRgbLoad ;
	int         FileAlphaLoad ;
	BASEIMAGE   MemRgbImage ;
	BASEIMAGE   MemAlphaImage ;
	int         MemRgbLoad ;
	int         MemAlphaLoad ;
	int         LoadHr ;
	LOADBASEIMAGE_GPARAM LoadBaseImageGParam ;

	DXST_ERRORLOG_ADDUTF16LE( "\xb0\x30\xe9\x30\xd5\x30\xa3\x30\xc3\x30\xaf\x30\x92\x30\xa9\x5f\x30\x5e\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�O���t�B�b�N�𕜋A���܂�\n" @*/ ) ;

	// ���ׂẴO���t�B�b�N������
	ImageDataArea  = HandleManageArray[ DX_HANDLETYPE_GRAPH ].AreaMax ;
	FileBackImage  = NULL ;
	FileRgbLoad    = FALSE ;
	FileAlphaLoad  = FALSE ;
	MemBackImage   = NULL ;
	MemRgbLoad     = FALSE ;
	MemAlphaLoad   = FALSE ;
	if( HandleManageArray[ DX_HANDLETYPE_GRAPH ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_GRAPH ].AreaMin ; i <= ImageDataArea ; i ++ )
		{
			Image = ( IMAGEDATA * )HandleManageArray[ DX_HANDLETYPE_GRAPH ].Handle[ i ] ;
			if( Image == NULL || Image->ReadBase == NULL ) continue ;

			// �t�@�C������ǂݍ��񂾏ꍇ�̓t�@�C������摜�𕜌�����
			if( Image->ReadBase->FileName )
			{
				// ��O�̃O���t�B�b�N�ƃp�X�ƃt�@�C���������ꍇ�͓ǂݍ��݂����Ȃ�
				if( FileBackImage == NULL ||
					FileRgbLoad == FALSE ||
					FileBackImage->ReadBase->FileName == NULL ||
					_WCSCMP( Image->ReadBase->FileName, FileBackImage->ReadBase->FileName ) != 0 ||
					Image->ReadBase->ReverseFlag != FileBackImage->ReadBase->ReverseFlag ||
					Image->ReadBase->ConvertPremultipliedAlpha != FileBackImage->ReadBase->ConvertPremultipliedAlpha )
				{
					// BASEIMAGE �̌�n��
					if( FileRgbLoad == TRUE )
					{
						NS_ReleaseGraphImage( &FileRgbImage ) ;
						FileRgbLoad = FALSE ;
					}
					if( FileAlphaLoad == TRUE )
					{
						NS_ReleaseGraphImage( &FileAlphaImage ) ;
						FileAlphaLoad = FALSE ;
					}

					// �摜���t�@�C�����烍�[�h�����ꍇ�͉摜�����[�h����
		//			DXST_ERRORLOGFMT_ADDW(( L"i:%d �t���p�X:%s �����[�h�ł��܂���ł���", i, Image->FilePath )) ;
		//			DXST_ERRORLOGFMT_ADDW(( L"�t�@�C��:%s �����[�h���܂�", Image->FilePath->String )) ;
		//			DXST_ERRORLOGFMT_ADDW(( L"���]�t���O:%d  �t�@�C��:%s �����[�h���܂�", Image->ReadBase->ReverseFlag, Image->ReadBase->FileName )) ;
					InitLoadBaseImageGParam( &LoadBaseImageGParam, FALSE ) ;
					LoadBaseImageGParam.ConvertPremultipliedAlpha = Image->ReadBase->ConvertPremultipliedAlpha ;
					LoadHr = CreateGraphImage_plus_Alpha_UseGParam(
								&LoadBaseImageGParam,
								Image->ReadBase->FileName,
								NULL,
								0,
								LOADIMAGE_TYPE_FILE,
								NULL,
								0,
								LOADIMAGE_TYPE_FILE,
								&FileRgbImage,
								&FileAlphaImage,
								Image->ReadBase->ReverseFlag ) ;
					if( LoadHr == -1 )
					{
						DXST_ERRORLOGFMT_ADDW(( L"Image File Load Error : %s", Image->ReadBase->FileName )) ;
						goto R1 ;
					}

					FileRgbLoad = TRUE ;
					if( LoadHr == 0 ) FileAlphaLoad = TRUE ;
				}

				// �O���t�B�b�N�̓]��
		//		DXST_ERRORLOGFMT_ADDW(( L"UseFileX:%d UseFileY:%d", Image->UseFileX, Image->UseFileY )) ;
				Graphics_Image_BltBmpOrGraphImageToGraphBase(
					&FileRgbImage,
					FileAlphaLoad == TRUE ? &FileAlphaImage : NULL,
					Image->UseBaseXI,
					Image->UseBaseYI,
					Image->HandleInfo.Handle,
					GSYS.CreateImage.NotUseTransColor ? FALSE : TRUE
				) ;
		R1 :
				FileBackImage = Image ;
			}
			else
			// ����������ǂݍ��񂾏ꍇ�̓���������摜�𕜌�����
			if( Image->ReadBase->MemImage )
			{
				// ��O�̃O���t�B�b�N�ƃ������A�h���X�������ꍇ�͓ǂݍ��݂����Ȃ�
				if( MemBackImage == NULL ||
					MemRgbLoad == FALSE ||
					Image->ReadBase->MemImage != MemBackImage->ReadBase->MemImage ||
					Image->ReadBase->AlphaMemImage != MemBackImage->ReadBase->AlphaMemImage ||
					Image->ReadBase->ReverseFlag != FileBackImage->ReadBase->ReverseFlag ||
					Image->ReadBase->ConvertPremultipliedAlpha != FileBackImage->ReadBase->ConvertPremultipliedAlpha )
				{
					// BASEIMAGE �̌�n��
					if( MemRgbLoad == TRUE )
					{
						NS_ReleaseGraphImage( &MemRgbImage ) ;
						MemRgbLoad = FALSE ;
					}
					if( MemAlphaLoad == TRUE )
					{
						NS_ReleaseGraphImage( &MemAlphaImage ) ;
						MemAlphaLoad = FALSE ;
					}

					// �摜�����������烍�[�h�����ꍇ�͉摜�����[�h����
					InitLoadBaseImageGParam( &LoadBaseImageGParam, FALSE ) ;
					LoadBaseImageGParam.ConvertPremultipliedAlpha = Image->ReadBase->ConvertPremultipliedAlpha ;
					LoadHr = CreateGraphImage_plus_Alpha_UseGParam(
								&LoadBaseImageGParam,
								NULL,
								Image->ReadBase->MemImage,
								Image->ReadBase->MemImageSize,
								LOADIMAGE_TYPE_MEM,
								Image->ReadBase->AlphaMemImage,
								Image->ReadBase->AlphaMemImageSize,
								LOADIMAGE_TYPE_MEM,
								&MemRgbImage,
								&MemAlphaImage,
								Image->ReadBase->ReverseFlag ) ;
					if( LoadHr == -1 )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\xe1\x30\xe2\x30\xea\x30\x4b\x30\x89\x30\x6e\x30\x3b\x75\xcf\x50\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xed\x30\xfc\x30\x4c\x30\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x00"/*@ L"����������̉摜�f�[�^�̃��[���ł��܂���ł���" @*/ )) ;
						goto R2 ;
					}

					MemRgbLoad = TRUE ;
					if( LoadHr == 0 ) MemAlphaLoad = TRUE ;
				}

				// �O���t�B�b�N�̓]��
				Graphics_Image_BltBmpOrGraphImageToGraphBase( 
					&MemRgbImage,
					MemAlphaLoad == TRUE ? &MemAlphaImage : NULL,
					Image->UseBaseXI,
					Image->UseBaseYI,
					Image->HandleInfo.Handle,
					GSYS.CreateImage.NotUseTransColor ? FALSE : TRUE
				) ;
		R2 :
				MemBackImage = Image ;
			}
			else
			// BaseImage ����ǂݍ��񂾏ꍇ�� BaseImage ����摜�𕜌�����
			if( Image->ReadBase->BaseImage )
			{
				// �O���t�B�b�N�̓]��
				Graphics_Image_BltBmpOrGraphImageToGraphBase( 
					Image->ReadBase->BaseImage,
					Image->ReadBase->AlphaBaseImage,
					Image->UseBaseXI,
					Image->UseBaseYI,
					Image->HandleInfo.Handle,
					GSYS.CreateImage.NotUseTransColor ? FALSE : TRUE
				) ;
			}
		}
	}

	// GraphImage �̌�n��
	if( FileRgbLoad == TRUE )
	{
		NS_ReleaseGraphImage( &FileRgbImage ) ;
		FileRgbLoad = FALSE ;
	}
	if( FileAlphaLoad == TRUE )
	{
		NS_ReleaseGraphImage( &FileAlphaImage ) ;
		FileAlphaLoad = FALSE ;
	}
	if( MemRgbLoad == TRUE )
	{
		NS_ReleaseGraphImage( &MemRgbImage ) ;
		MemRgbLoad = FALSE ;
	}
	if( MemAlphaLoad == TRUE )
	{
		NS_ReleaseGraphImage( &MemAlphaImage ) ;
		MemAlphaLoad = FALSE ;
	}

	DXST_ERRORLOG_ADDUTF16LE( "\xb0\x30\xe9\x30\xd5\x30\xa3\x30\xc3\x30\xaf\x30\x6e\x30\xa9\x5f\x30\x5e\x4c\x30\x8c\x5b\x86\x4e\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�O���t�B�b�N�̕��A���������܂���\n" @*/ ) ;
}

// CreateGraph �� CreateDivGraph �̋��ʂ��� BASEIMAGE �\�z�������֐�����������
static void Graphics_Image_CreateGraph_LoadBaseImage(
	 LOADGRAPH_PARAM *Param,
	 CREATEGRAPH_LOADBASEIMAGE_PARAM *LParam
 )
{
	LParam->UseTempBaseImage             = FALSE ;
	LParam->LoadHr                       = -1 ;
	LParam->UseRgbBaseImage              = NULL ;
	LParam->UseAlphaBaseImage            = NULL ;
	LParam->TempRgbBaseImage.GraphData   = NULL ;
	LParam->TempAlphaBaseImage.GraphData = NULL ;

	if( Param->FileName != NULL )
	{
		// �t�@�C������ǂݍ��ޏꍇ
		LParam->LoadHr = CreateGraphImage_plus_Alpha_UseGParam(
					&Param->GParam.LoadBaseImageGParam,
					Param->FileName,
					NULL,
					0,
					LOADIMAGE_TYPE_FILE,
					NULL,
					0,
					LOADIMAGE_TYPE_FILE,
					&LParam->TempRgbBaseImage,
					Param->GParam.CreateGraphGParam.InitHandleGParam.NotUseAlphaImageLoadFlag ? NULL : &LParam->TempAlphaBaseImage,
					FALSE
				) ;
		LParam->UseRgbBaseImage = &LParam->TempRgbBaseImage ;
		if( LParam->LoadHr == 0 )
		{
			LParam->UseAlphaBaseImage = &LParam->TempAlphaBaseImage ;
		}
		LParam->UseTempBaseImage = TRUE ;
	}
	else
	if( Param->RgbMemImage != NULL )
	{
		// ��������̃t�@�C���C���[�W����ǂݍ��ޏꍇ
		LParam->LoadHr = CreateGraphImage_plus_Alpha_UseGParam(
					&Param->GParam.LoadBaseImageGParam,
					NULL,
					Param->RgbMemImage,
					Param->RgbMemImageSize,
					LOADIMAGE_TYPE_MEM,
					Param->AlphaMemImage,
					Param->AlphaMemImageSize,
					LOADIMAGE_TYPE_MEM,
					&LParam->TempRgbBaseImage,
					&LParam->TempAlphaBaseImage,
					FALSE
				) ;
		LParam->UseRgbBaseImage = &LParam->TempRgbBaseImage ;
		if( LParam->LoadHr == 0 )
		{
			LParam->UseAlphaBaseImage = &LParam->TempAlphaBaseImage ;
		}
		LParam->UseTempBaseImage = TRUE ;
	}
#ifdef __WINDOWS__
	else
	if( Param->RgbBmpInfo != NULL )
	{
		int RgbCopyHr ;
		int AlphaCopyHr ;

		// BMP ����ǂݍ��ޏꍇ
		RgbCopyHr = NS_ConvBitmapToGraphImage( Param->RgbBmpInfo, Param->RgbBmpImage, &LParam->TempRgbBaseImage, TRUE ) ;
		if( RgbCopyHr == -1 )
			return ;

		if( Param->AlphaBmpInfo != NULL && Param->AlphaBmpImage != NULL )
		{
			AlphaCopyHr = NS_ConvBitmapToGraphImage( Param->AlphaBmpInfo, Param->AlphaBmpImage, &LParam->TempAlphaBaseImage, TRUE ) ;
			if( AlphaCopyHr == -1 )
			{
				if( RgbCopyHr == 1 ) NS_ReleaseGraphImage( &LParam->TempRgbBaseImage ) ;
				return ;
			}
			LParam->UseAlphaBaseImage = &LParam->TempAlphaBaseImage ;
		}
		else
		{
			_MEMSET( &LParam->TempAlphaBaseImage, 0, sizeof( LParam->TempAlphaBaseImage ) ) ;
		}
		LParam->UseRgbBaseImage = &LParam->TempRgbBaseImage ;
		LParam->UseTempBaseImage = TRUE ;

		LParam->LoadHr = 1 ;
	}
#endif // __WINDOWS__
	else
	if( Param->RgbBaseImage != NULL )
	{
		// ��{�C���[�W����ǂݍ��ޏꍇ
		LParam->UseRgbBaseImage   = Param->RgbBaseImage ;
		LParam->UseAlphaBaseImage = Param->AlphaBaseImage ;

		LParam->LoadHr = 1 ;
	}

	if( LParam->LoadHr >= 0 )
	{
		// ���]�t���O�������Ă�����C���[�W�𔽓]����
		if( Param->ReverseFlag == TRUE )
		{
			NS_ReverseGraphImage( LParam->UseRgbBaseImage ) ;
			if( LParam->UseAlphaBaseImage != NULL ) NS_ReverseGraphImage( LParam->UseAlphaBaseImage ) ;
		}
	}
}

// CreateGraph �� CreateDivGraph �̋��ʂ��� BASEIMAGE ��n���������֐�����������
static void Graphics_Image_CreateGraph_TerminateBaseImage(
	LOADGRAPH_PARAM *Param,
	CREATEGRAPH_LOADBASEIMAGE_PARAM *LParam
)
{
	// �ǂݍ��񂾃O���t�B�b�N�̌�n��������
	if( LParam->UseTempBaseImage )
	{
		NS_ReleaseGraphImage( &LParam->TempRgbBaseImage ) ;
		NS_ReleaseGraphImage( &LParam->TempAlphaBaseImage ) ;
	}
	else
	{
		// ���]�t���O�������Ă�����C���[�W�����ɖ߂�
		if( Param->ReverseFlag == TRUE )
		{
			NS_ReverseGraphImage( LParam->UseRgbBaseImage ) ;
			if( LParam->UseAlphaBaseImage != NULL ) NS_ReverseGraphImage( LParam->UseAlphaBaseImage ) ;
		}
	}
}





















// ��ʊ֌W�֐�

// �w��̉𑜓x���Ή����Ă��邩�ǂ����𒲂ׂ�֐�
static int Graphics_Screen_CheckDisplaySetting( int ScreenSizeX, int ScreenSizeY, int ColorBitDepth )
{
	int Num, i ;
	DISPLAYMODEDATA Mode ;

	Num = NS_GetDisplayModeNum() ;
	for( i = 0 ; i < Num ; i ++ )
	{
		Mode = NS_GetDisplayMode( i ) ;
		if( Mode.Width         == ScreenSizeX &&
			Mode.Height        == ScreenSizeY &&
			Mode.ColorBitDepth == ColorBitDepth )
		{
			return TRUE ;
		}
	}

	return FALSE ;
}























// �`��֌W�֐�

// �r���{�[�h�p�̍s��v�Z
__inline static void Graphics_Draw_VectorTransformToBillboard( VECTOR *Vector, VECTOR *Position )
{
	VECTOR invec = *Vector ;
	Vector->x = invec.x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + invec.y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Position->x ;
	Vector->y = invec.x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + invec.y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Position->y ;
	Vector->z = invec.x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + invec.y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Position->z ;
}

#define SETDRAWRECTCODE\
	int x[ 4 ], y[ 4 ], MaxX, MaxY, MinX, MinY ;\
	int i, ix, iy ;\
\
	x[ 0 ] = x1 ; x[ 1 ] = x2 ; x[ 2 ] = x3 ; x[ 3 ] = x4 ;\
	y[ 0 ] = y1 ; y[ 1 ] = y2 ; y[ 2 ] = y3 ; y[ 3 ] = y4 ;\
\
	MaxX = 0 ; MaxY = 0 ;\
	MinX = 0xffffff ; MinY = 0xffffff ;\
\
	for( i = 0 ; i < 4 ; i ++ )\
	{\
		ix = x[ i ] ; iy = y[ i ] ;\
		if( ix > MaxX ) MaxX = ix ; if( iy > MaxY ) MaxY = iy ;\
		if( ix < MinX ) MinX = ix ; if( iy < MinY ) MinY = iy ;\
	}\
\
	SETRECT( DrawRect, MinX, MinY, MaxX, MaxY ) ;\
	DRAWRECT_DRAWAREA_CLIP

static int Graphics_Draw_ModiGraphBase( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int GrHandle, int TransFlag, bool SimpleDrawFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret = -1 ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) ) return -1 ;
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawModiGraph_PF( x1, y1, x2, y2, x4, y4, x3, y3, Image, BlendImage, TransFlag, SimpleDrawFlag ),
		Graphics_Software_DrawModiGraph(    x1, y1, x2, y2, x4, y4, x3, y3, Image,             TransFlag                 ),
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

#undef SETDRAWRECTCODE

#define SETDRAWRECTCODE\
	int x[4], y[4], MaxX, MaxY, MinX, MinY ;\
	int i, ix, iy ;\
\
	x[ 0 ] = _FTOL( x1 ) ; x[ 1 ] = _FTOL( x2 ) ; x[ 2 ] = _FTOL( x3 ) ; x[ 3 ] = _FTOL( x4 ) ;\
	y[ 0 ] = _FTOL( y1 ) ; y[ 1 ] = _FTOL( y2 ) ; y[ 2 ] = _FTOL( y3 ) ; y[ 3 ] = _FTOL( y4 ) ;\
\
	MaxX = 0 ; MaxY = 0 ;\
	MinX = 0xfffffff ; MinY = 0xfffffff ;\
\
	for( i = 0 ; i < 4 ; i ++ )\
	{\
		ix = x[ i ] ; iy = y[ i ] ;\
		if( ix > MaxX ) MaxX = ix ; if( iy > MaxY ) MaxY = iy ;\
		if( ix < MinX ) MinX = ix ; if( iy < MinY ) MinY = iy ;\
	}\
\
	SETRECT( DrawRect, MinX, MinY, MaxX, MaxY ) ;

static int Graphics_Draw_ModiGraphFBase( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int GrHandle, int TransFlag, bool SimpleDrawFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret = -1 ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) ) return -1 ;
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawModiGraphF_PF( x1, y1, x2, y2, x4, y4, x3, y3, Image, BlendImage, TransFlag, SimpleDrawFlag ),
		Graphics_Software_DrawModiGraphF(    x1, y1, x2, y2, x4, y4, x3, y3, Image,             TransFlag                 ),
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

#undef SETDRAWRECTCODE



// �l�̑傫�����W�l���g�����`��ɂ��v�Z���x�̒ቺ��}����ׂ̏������s��
static void	Graphics_Draw_BeginLarge3DPositionSupportDraw( LARGE3DPOSITIONSUPPORT_DRAWINFO *DrawInfo, VECTOR_D BasePosition )
{
	MATRIX_D TranslateMatrix ;
	MATRIX_D TempWorldMatrix ;
	MATRIX_D IdentityMatrix ;

	DrawInfo->BackupWorldMatrix = GSYS.DrawSetting.WorldMatrix ;
	DrawInfo->BackupViewMatrix  = GSYS.DrawSetting.ViewMatrix ;
	CreateTranslationMatrixD( &TranslateMatrix, BasePosition.x, BasePosition.y, BasePosition.z ) ;
	CreateIdentityMatrixD( &IdentityMatrix ) ;
	CreateMultiplyMatrixD( &TempWorldMatrix, &TranslateMatrix, &GSYS.DrawSetting.WorldViewMatrix ) ;
	NS_SetTransformToWorldD( &TempWorldMatrix ) ;
	NS_SetTransformToViewD( &IdentityMatrix ) ;
}

// �l�̑傫�����W�l���g�����`��ɂ��v�Z���x�̒ቺ��}����ׂ̏����ōs�����ύX�����ɖ߂�
static void	Graphics_Draw_EndLarge3DPositionSupportDraw( const LARGE3DPOSITIONSUPPORT_DRAWINFO *DrawInfo )
{
	NS_SetTransformToWorldD( &DrawInfo->BackupWorldMatrix ) ;
	NS_SetTransformToViewD( &DrawInfo->BackupViewMatrix ) ;
}






















// �n�[�h�E�F�A�A�N�Z�����[�^�g�p�����_�����O�֌W�֐�

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawChipMap
static int	Graphics_Hardware_DrawChipMap( int Sx, int Sy, int XNum, int YNum, const int *MapData, int MapDataPitch, int /*ChipTypeNum*/, const int *ChipGrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int i, j ;
	int xI, yI, WidthI, HeightI ;
	float xF, yF, WidthF, HeightF ;
	const int *MP ;

	if( Graphics_Hardware_CheckValid_PF() == 0 ) return -1 ;

	// �`��̏���
	if( GRAPHCHK( ChipGrHandle[ 0 ], Image ) )
		return -1 ;
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;
	}

	WidthI = Image->WidthI ;
	WidthF = Image->WidthF ;
	HeightI = Image->HeightI ;
	HeightF = Image->HeightF ;

	// ���ʂ� for ���Ń��[�v�`��
	yI = Sy ;
	yF = ( float )Sy ;
	MP = MapData ;
	for( i = 0 ; i < YNum ; i ++, yI += HeightI, yF += HeightF )
	{
		xI = Sx ;
		xF = ( float )Sx ;
		for( j = 0 ; j < XNum ; j ++, xI += WidthI, xF += WidthF, MP ++ )
		{
			if( *MP == -1 ) continue ;

			if( GRAPHCHK( ChipGrHandle[ *MP ], Image ) )
				return -1 ;

			Graphics_Hardware_DrawGraph_PF( xI, yI, ( float )xF, ( float )yF, Image, BlendImage, TransFlag, TRUE ) ;
		}
		MP += MapDataPitch - XNum ;
	}

	// �I��
	return 0 ;
}

/*
x1,y1,x2,y2�@��ʒ��A�^�C���\�肵�����̈�
Tx,Ty�@�@�@�@�ψʁ@�@�@�@�@�@�@�@�@�@�@�@���摜���́A�ǂ��̍��W����^�C���\����J�n���邩�B
ExtRate�@�@�@�摜�̊g�嗦�@�@�@�@�@�@�@�@������u1�v�ɂ���ƁA���摜�̂܂܂̑傫���Ń^�C���\�肷��B
Angle�@�@�@�@�摜�̉�]�p�x�@�@�@�@�@�@�@������u0�v�ɂ���ƁA��]�Ȃ��Ń^�C���\�肷��B
GrHandle�@�@ �摜�̃n���h��
TransFlag�@�@���߃t���O
*/

// �n�[�h�E�G�A�A�N�Z�����[�^�g�p�� DrawTile
static int	Graphics_Hardware_DrawTile( int x1, int y1, int x2, int y2, int Tx, int Ty, double ExtRate, double Angle, IMAGEDATA *Image, IMAGEDATA *BlendImage, int TransFlag )
{
	double MaxLength ;
	float Width, Height, f, Sin, Cos, Sin2, Cos2 ;
	float CenX, CenY, x, y, BaseX, BaseY ;
	float xAddX, xAddY, yAddX, yAddY ;
	int temp, Num, i, j ;
	RECT DrawRect ;
	float FExtRate, FAngle ;

	FExtRate = (float)ExtRate ;
	FAngle = (float)Angle ;

	if( ExtRate <= 0.0 ) ExtRate = 0.0001 ;
	if( x1 > x2 ){ temp = x2 ; x2 = x1 ; x1 = temp ; }
	if( y1 > y2 ){ temp = y2 ; y2 = y1 ; y1 = temp ; }
	Tx %= Image->WidthI ;
	Ty %= Image->HeightI ;

	// �i�ޒl���v�Z
	_SINCOS_PLATFORM( FAngle, &Sin, &Cos ) ;
	xAddX = Image->WidthI  * Cos * FExtRate ;
	xAddY = Image->HeightI * Sin * FExtRate ;

	_SINCOS_PLATFORM( FAngle + DX_PI_F / 2.0F, &Sin2, &Cos2 ) ;
	yAddX = Image->WidthI  * Cos2 * FExtRate ;
	yAddY = Image->HeightI * Sin2 * FExtRate ;

	// �`�敝�𓾂�
	Width = (float)( x2 - x1 ) ;
	Height = (float)( y2 - y1 ) ;

	// �ő咷�𓾂�
	MaxLength = _SQRT( Width * Width + Height * Height ) ;

	// �`����𓾂�
	{
		int k ;

		k = Image->WidthI > Image->HeightI ? Image->HeightI : Image->WidthI ;
		Num = _DTOL( MaxLength / ( k * ExtRate ) + 2 ) ;
		if( Num % 2 == 0 ) Num ++ ;
	}

	// �`�撆�S�_�𓾂�
	CenX = x1 + Width / 2 ;
	CenY = y1 + Height / 2 ;

	// �`��\�͈͂�ύX����
	DrawRect = GSYS.DrawSetting.DrawArea ;
	NS_SetDrawArea( x1, y1, x2, y2 ) ;

	// �`��J�n���W�𓾂�
	BaseX = ( -Tx - ( Num * Image->WidthI  ) / 2 ) * FExtRate ;
	BaseY = ( -Ty - ( Num * Image->HeightI ) / 2 ) * FExtRate ;

	f = BaseX * Cos - BaseY * Sin ;
	BaseY = BaseX * Sin + BaseY * Cos + CenY ;
	BaseX = f + CenX ;

	// �`��J�n
	for( i = 0 ; i < Num ; i ++, BaseX += yAddX, BaseY += yAddY )
	{
		x = BaseX ;
		y = BaseY ;
		for( j = 0 ; j < Num ; j ++, x += xAddX, y += xAddY )
		{
			Graphics_Hardware_DrawModiGraphF_PF(
				x, y,
				x + xAddX, y + xAddY,
				x + yAddX, y + yAddY,
				x + xAddX + yAddX, y + xAddY + yAddY,
				Image, BlendImage, TransFlag, true ) ;
		}
	}

	// �`��\�͈͂����ɖ߂�
	NS_SetDrawArea( DrawRect.left, DrawRect.top, DrawRect.right, DrawRect.bottom ) ;

	return 0 ;
}

























// �\�t�g�E�G�A�����_�����O�֐�

// �\�t�g�E�G�A�����_�����O�̏��������s��
static int Graphics_Software_Initialize( void )
{
	// ���C����ʗp MEMIMG �̍쐬
	if( MakeMemImgScreen(
		&GSYS.SoftRender.MainBufferMemImg,
		GSYS.Screen.MainScreenSizeX,
		GSYS.Screen.MainScreenSizeY,
		GSYS.Screen.MainScreenColorBitDepth == 16 ? 0 : 1 ) < 0 )
	{
		return DxLib_ErrorUTF16LE( "\xee\x4e\x3b\x75\x62\x97\x28\x75\x6e\x30\x20\x00\x4d\x00\x45\x00\x4d\x00\x49\x00\x4d\x00\x47\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"����ʗp�� MEMIMG �̍쐬�Ɏ��s���܂���\n" @*/ ) ;
	}

	// �⏕��ʗp MEMIMG �̍쐬
	if( MakeMemImgScreen(
		&GSYS.SoftRender.SubBufferMemImg,
		GSYS.Screen.MainScreenSizeX,
		GSYS.Screen.MainScreenSizeY,
		GSYS.Screen.MainScreenColorBitDepth == 16 ? 0 : 1 ) < 0 )
	{
		return DxLib_ErrorUTF16LE( "\xdc\x88\xa9\x52\x28\x75\x6e\x30\x20\x00\x4d\x00\x45\x00\x4d\x00\x49\x00\x4d\x00\x47\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�⏕�p�� MEMIMG �̍쐬�Ɏ��s���܂���\n" @*/ ) ;
	}

	// �t�H���g�̔������`��p�A���t�@���摜�̍쐬
	_MEMSET( &GSYS.SoftRender.FontScreenMemImgAlpha, 0, sizeof( MEMIMG ) ) ;
	_MEMSET( &GSYS.SoftRender.FontScreenMemImgNormal, 0, sizeof( MEMIMG ) ) ;
	InitializeMemImg(
		&GSYS.SoftRender.FontScreenMemImgNormal,
		GSYS.Screen.MainScreenSizeX,
		GSYS.Screen.MainScreenSizeY,
		-1,
		0,
		GSYS.Screen.MainScreenColorBitDepth == 16 ? 0 : 1,
		FALSE, FALSE, FALSE, NULL ) ;
	InitializeMemImg(
		&GSYS.SoftRender.FontScreenMemImgAlpha,
		GSYS.Screen.MainScreenSizeX,
		GSYS.Screen.MainScreenSizeY,
		-1,
		0,
		GSYS.Screen.MainScreenColorBitDepth == 16 ? 0 : 1,
		FALSE, TRUE, FALSE, NULL ) ;

	// �o�b�t�@�̃N���A
	ClearMemImg( &GSYS.SoftRender.MainBufferMemImg, NULL, NS_GetColor3( GSYS.SoftRender.MainBufferMemImg.Base->ColorDataP, GSYS.Screen.BackgroundRed, GSYS.Screen.BackgroundGreen, GSYS.Screen.BackgroundBlue, 0 ) ) ;

	// �`��Ώۂ̃Z�b�g
	GSYS.SoftRender.TargetMemImg = &GSYS.SoftRender.MainBufferMemImg ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�̌�n�����s��
static int	Graphics_Software_Terminate( void )
{
	// ���C���o�b�t�@�̉��
	TerminateMemImg( &GSYS.SoftRender.MainBufferMemImg ) ;

	// �T�u�o�b�t�@�̉��
	TerminateMemImg( &GSYS.SoftRender.SubBufferMemImg ) ;

	// �t�H���g�̔������`��p�A���t�@���摜�̉��
	TerminateMemImg( &GSYS.SoftRender.FontScreenMemImgNormal ) ;
	TerminateMemImg( &GSYS.SoftRender.FontScreenMemImgAlpha ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawGraph
static int  Graphics_Software_DrawGraph( int x, int y, IMAGEDATA *Image, int TransFlag )
{
	DrawMemImg( GSYS.SoftRender.TargetMemImg, &Image->Soft.MemImg, x, y, TransFlag, GSYS.SoftRender.BlendMemImg ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawExtendGraph
static int  Graphics_Software_DrawExtendGraph( int x1, int y1, int x2, int y2, IMAGEDATA *Image, int TransFlag )
{
	RECT Rect ;

	Rect.left   = x1 ;
	Rect.top    = y1 ;
	Rect.right  = x2 ;
	Rect.bottom = y2 ;

	DrawEnlargeMemImg( GSYS.SoftRender.TargetMemImg, &Image->Soft.MemImg, &Rect, TransFlag, GSYS.SoftRender.BlendMemImg ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawRotaGraph
static int  Graphics_Software_DrawRotaGraph( float x, float y, double ExRate, double Angle, IMAGEDATA *Image, int TransFlag, int TurnFlag )
{
	DrawRotationMemImg( GSYS.SoftRender.TargetMemImg, &Image->Soft.MemImg,
		_FTOL( x ), _FTOL( y ), ( float )Angle, ( float )ExRate, TransFlag, TurnFlag, GSYS.SoftRender.BlendMemImg ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawRotaGraphFast
static int  Graphics_Software_DrawRotaGraphFast( float x, float y, float ExRate, float Angle, IMAGEDATA *Image, int TransFlag, int TurnFlag )
{
	DrawRotationMemImg( GSYS.SoftRender.TargetMemImg, &Image->Soft.MemImg,
		_FTOL( x ), _FTOL( y ), Angle, ExRate, TransFlag, TurnFlag, GSYS.SoftRender.BlendMemImg ) ;

	// �I��
	return 0 ;
}

static int	Graphics_Software_DrawPoly( DX_POINTDATA *p, IMAGEDATA *Image, int TransFlag, unsigned int Color )
{
	DrawBasicPolygonMemImg( GSYS.SoftRender.TargetMemImg, Image ? &Image->Soft.MemImg : NULL, 
							p, TransFlag, GSYS.SoftRender.BlendMemImg, Color ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawModiGraph
static int  Graphics_Software_DrawModiGraph( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, IMAGEDATA *Image, int TransFlag )
{
	DX_POINTDATA p[4] ;

	p[0].x = x1 ;		p[0].y = y1 ;		p[0].u = 0 ;				p[0].v = 0 ;
	p[1].x = x2 ;		p[1].y = y2 ;		p[1].u = Image->WidthI ;	p[1].v = 0 ;
	p[2].x = x3 ;		p[2].y = y3 ;		p[2].u = 0 ;				p[2].v = Image->HeightI ;
	p[3].x = x4 ;		p[3].y = y4 ;		p[3].u = Image->WidthI ;	p[3].v = Image->HeightI ;

	Graphics_Software_DrawPoly( &p[0], Image, TransFlag, 0 ) ;
	Graphics_Software_DrawPoly( &p[1], Image, TransFlag, 0 ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawModiGraphF
static int  Graphics_Software_DrawModiGraphF( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, IMAGEDATA *Image, int TransFlag )
{
	return Graphics_Software_DrawModiGraph( _FTOL( x1 ), _FTOL( y1 ), _FTOL( x2 ), _FTOL( y2 ), _FTOL( x3 ), _FTOL( y3 ), _FTOL( x4 ), _FTOL( y4 ), Image, TransFlag ) ;
}

// �\�t�g�E�G�A�����_�����O�� DrawChipMap
static int	Graphics_Software_DrawChipMap( int Sx, int Sy, int XNum, int YNum, const int *MapData, int MapDataPitch, int /*ChipTypeNum*/, const int *ChipGrHandle, int TransFlag )
{
	int x, y, i, j, Width, Height, Pitch2 ;
	const int *MP ;
	IMAGEDATA *Image ;

	if( GRAPHCHK( ChipGrHandle[ 0 ], Image ) )
		return -1 ;

	Width  = Image->WidthI ;
	Height = Image->HeightI ;

	Pitch2 = MapDataPitch - XNum ;

	// ���ʂ� for ���Ń��[�v�`��
	y = Sy ;
	MP = MapData ;
	for( i = 0 ; i < YNum ; i ++, y += Height )
	{
		x = Sx ;
		for( j = 0 ; j < XNum ; j ++, x += Width, MP ++ )
		{
			if( *MP == -1 ) continue ;

			if( GRAPHCHK( ChipGrHandle[ *MP ], Image ) )
				return -1 ;

			Graphics_Software_DrawGraph( x, y, Image, TransFlag ) ;
		}
		MP += Pitch2 ;
	}

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawTile
static int	Graphics_Software_DrawTile( int x1, int y1, int x2, int y2, int Tx, int Ty, double ExtRate, double Angle, IMAGEDATA *Image, int TransFlag )
{
	double MaxLength ;
	float Width, Height, f, Sin, Cos ;
	float CenX, CenY, x, y, BaseX, BaseY ;
	float xAddX, xAddY, yAddX, yAddY ;
	int temp, Num, i, j ;
	RECT DrawRect ;
	float FAngle, FExtRate, Sin2, Cos2 ;

	FAngle = ( float )Angle ;
	FExtRate = ( float )ExtRate ;

	if( ExtRate <= 0.0 ) ExtRate = 0.0001 ;
	if( x1 > x2 ){ temp = x2 ; x2 = x1 ; x1 = temp ; }
	if( y1 > y2 ){ temp = y2 ; y2 = y1 ; y1 = temp ; }
	Tx %= Image->WidthI ;
	Ty %= Image->HeightI ;

	_SINCOS_PLATFORM( FAngle, &Sin, &Cos ) ;
//	Sin = (float)sin( Angle ) ;
//	Cos = (float)cos( Angle ) ;

	// �i�ޒl���v�Z
	xAddX = Image->WidthI * Cos * FExtRate ;
	xAddY = Image->WidthI * Sin * FExtRate ;

	_SINCOS_PLATFORM( FAngle + DX_PI_F / 2.0F, &Sin2, &Cos2 ) ;
	yAddX = Image->HeightI * Cos2 * FExtRate ;
	yAddY = Image->HeightI * Sin2 * FExtRate ;

	// �`�敝�𓾂�
	Width = (float)( x2 - x1 ) ;
	Height = (float)( y2 - y1 ) ;

	// �ő咷�𓾂�
	MaxLength = _SQRT( Width * Width + Height * Height ) ;

	// �`����𓾂�
	{
		int k ;

		k = Image->WidthI > Image->HeightI ? Image->HeightI : Image->WidthI ;
		Num = _DTOL( MaxLength / ( k * ExtRate ) + 2 ) ;
		if( Num % 2 == 0 ) Num ++ ;
	}

	// �`�撆�S�_�𓾂�
	CenX = x1 + Width / 2 ;
	CenY = y1 + Height / 2 ;

	// �`��\�͈͂�ύX����
	DrawRect = GSYS.DrawSetting.DrawArea ;
	NS_SetDrawArea( x1, y1, x2, y2 ) ;

	// �`��J�n���W�𓾂�
	BaseX = ( -Tx - ( Num * Image->WidthI  ) / 2 ) * FExtRate ;
	BaseY = ( -Ty - ( Num * Image->HeightI ) / 2 ) * FExtRate ;

	f = BaseX * Cos - BaseY * Sin ;
	BaseY = BaseX * Sin + BaseY * Cos + CenY ;
	BaseX = f + CenX ;

	// �`��J�n
	for( i = 0 ; i < Num ; i ++, BaseX += yAddX, BaseY += yAddY )
	{
		x = BaseX ;
		y = BaseY ;
		for( j = 0 ; j < Num ; j ++, x += xAddX, y += xAddY )
		{
			if( Angle == 0.0 )
			{
				if( ExtRate == 0.0 )
				{
					Graphics_Software_DrawGraph( _FTOL( x ), _FTOL( y ), Image, TransFlag ) ;
				}
				else
				{
					Graphics_Software_DrawExtendGraph( _FTOL( x ), _FTOL( y ),
											 _FTOL( x + xAddX ),
											 _FTOL( y + yAddY ), Image, TransFlag ) ;
				}
			}
			else
			{
				Graphics_Software_DrawModiGraph( _FTOL( x ), _FTOL( y ),
								_FTOL( x + xAddX ), _FTOL( y + xAddY ),
								_FTOL( x + yAddX ), _FTOL( y + yAddY ),
								_FTOL( x + xAddX + yAddX ), _FTOL( y + xAddY + yAddY ),
								Image, TransFlag ) ;
			}
		}
	}

	// �`��\�͈͂����ɖ߂�
	NS_SetDrawArea( DrawRect.left, DrawRect.top, DrawRect.right, DrawRect.bottom ) ;

	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawFillBox
static int  Graphics_Software_DrawFillBox( int x1, int y1, int x2, int y2, unsigned int Color )
{
	RECT DestRect ;

	DestRect.left   = x1 ;
	DestRect.top    = y1 ;
	DestRect.right  = x2 ;
	DestRect.bottom = y2 ;

	DrawFillBoxMemImg( GSYS.SoftRender.TargetMemImg, &DestRect, Color ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawLineBox
static int  Graphics_Software_DrawLineBox( int x1, int y1, int x2, int y2, unsigned int Color )
{
	LINEDATA Line[4] ;

	Line[0].x1 = x1 ;		Line[0].y1 = y1 ;		Line[0].x2 = x2 - 1 ;	Line[0].y2 = y1 ;
	Line[1].x1 = x2 - 1 ;	Line[1].y1 = y1 ;		Line[1].x2 = x2 - 1 ;	Line[1].y2 = y2 - 1 ;
	Line[2].x1 = x2 - 1 ;	Line[2].y1 = y2 - 1 ;	Line[2].x2 = x1 ;		Line[2].y2 = y2 - 1 ;
	Line[3].x1 = x1 ;		Line[3].y1 = y2 - 1 ;	Line[3].x2 = x1 ;		Line[3].y2 = y1 ;
	Line[3].color = Line[2].color = Line[1].color = Line[0].color = Color ;
	Line[3].pal   = Line[2].pal   = Line[1].pal   = Line[0].pal   = GSYS.DrawSetting.BlendParam ;

	return Graphics_Software_DrawLineSet( Line, 4 ) ;
}

// �\�t�g�E�G�A�����_�����O�� DrawLine
static int	Graphics_Software_DrawLine( int x1, int y1, int x2, int y2, unsigned int Color )
{
	DrawLineMemImg( GSYS.SoftRender.TargetMemImg, x1, y1, x2, y2, Color ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawCircle( �����w�肠�� )
static int	Graphics_Software_DrawCircle_Thickness( int x, int y, int r, unsigned int Color, int Thickness )
{
	int i ;
	RECT DrawRect ;
	short ( *CirclePos )[ 5 ] ;
	int x1, x2, y1 ;

	DrawRect = GSYS.DrawSetting.DrawArea ;

	// �~�̍��W��������o�b�t�@���擾
	if( Graphics_Other_AllocCommonBuffer( 1, sizeof( short ) * 5 * DrawRect.bottom ) < 0 )
	{
		return -1 ;
	}
	CirclePos = ( short (*)[ 5 ] )GSYS.Resource.CommonBuffer[ 1 ] ;
	_MEMSET( CirclePos, 0, sizeof( short ) * 5 * DrawRect.bottom ) ;

	// �~�̍��W���X�g�쐬
	Graphics_Draw_GetCircle_ThicknessDrawPosition( x, y, r, Thickness, CirclePos ) ;

	// �`�揈��
	for( i = DrawRect.top ; i < DrawRect.bottom ; i ++ )
	{
		if( CirclePos[ i ][ 4 ] == 0 )
		{
			continue ;
		}

		y1 = i ;

		if( ( CirclePos[ i ][ 4 ] & ( 4 | 8 ) ) == 0 || CirclePos[ i ][ 2 ] >= CirclePos[ i ][ 3 ] )
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DrawLineMemImg( GSYS.SoftRender.TargetMemImg, x1, y1, x2 + 1, y1, Color ) ;
		}
		else
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 2 ] ;
			DrawLineMemImg( GSYS.SoftRender.TargetMemImg, x1, y1, x2 + 1, y1, Color ) ;

			x1 = CirclePos[ i ][ 3 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DrawLineMemImg( GSYS.SoftRender.TargetMemImg, x1, y1, x2 + 1, y1, Color ) ;
		}
	}

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawOval( �����w�肠�� )
static int	Graphics_Software_DrawOval_Thickness( int x, int y, int rx, int ry, unsigned int Color, int Thickness )
{
	int i ;
	RECT DrawRect ;
	short ( *CirclePos )[ 5 ] ;
	int x1, x2, y1 ;

	DrawRect = GSYS.DrawSetting.DrawArea ;

	// �~�̍��W��������o�b�t�@���擾
	if( Graphics_Other_AllocCommonBuffer( 1, sizeof( short ) * 5 * DrawRect.bottom ) < 0 )
	{
		return -1 ;
	}
	CirclePos = ( short (*)[ 5 ] )GSYS.Resource.CommonBuffer[ 1 ] ;
	_MEMSET( CirclePos, 0, sizeof( short ) * 5 * DrawRect.bottom ) ;

	// �~�̍��W���X�g�쐬
	Graphics_Draw_GetOval_ThicknessDrawPosition( x, y, rx, ry, Thickness, CirclePos ) ;

	// �`�揈��
	for( i = DrawRect.top ; i < DrawRect.bottom ; i ++ )
	{
		if( CirclePos[ i ][ 4 ] == 0 )
		{
			continue ;
		}

		y1 = i ;

		if( ( CirclePos[ i ][ 4 ] & ( 4 | 8 ) ) == 0 || CirclePos[ i ][ 2 ] >= CirclePos[ i ][ 3 ] )
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DrawLineMemImg( GSYS.SoftRender.TargetMemImg, x1, y1, x2 + 1, y1, Color ) ;
		}
		else
		{
			x1 = CirclePos[ i ][ 0 ] ;
			x2 = CirclePos[ i ][ 2 ] ;
			DrawLineMemImg( GSYS.SoftRender.TargetMemImg, x1, y1, x2 + 1, y1, Color ) ;

			x1 = CirclePos[ i ][ 3 ] ;
			x2 = CirclePos[ i ][ 1 ] ;
			DrawLineMemImg( GSYS.SoftRender.TargetMemImg, x1, y1, x2 + 1, y1, Color ) ;
		}
	}

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawCircle
static int	Graphics_Software_DrawCircle( int x, int y, int r, unsigned int Color, int FillFlag )
{
	DrawCircleMemImg( GSYS.SoftRender.TargetMemImg, x, y, r, Color, FillFlag ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawOval
static int	Graphics_Software_DrawOval( int x, int y, int rx, int ry, unsigned int Color, int FillFlag )
{
	DrawOvalMemImg( GSYS.SoftRender.TargetMemImg, x, y, rx, ry, Color, FillFlag ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawTriangle
static int	Graphics_Software_DrawTriangle( int x1, int y1, int x2, int y2, int x3, int y3, unsigned int Color, int FillFlag )
{
	// �h��Ԃ��������łȂ����ŏ����𕪊�
	if( FillFlag == TRUE )
	{
		// �h��Ԃ��̏ꍇ
		DX_POINTDATA p[3] ;

		p[0].x = x1 ;		p[0].y = y1 ;
		p[1].x = x2 ;		p[1].y = y2 ;
		p[2].x = x3 ;		p[2].y = y3 ;
	
		return Graphics_Software_DrawPoly( p, NULL, FALSE, Color ) ;
	}
	else
	{
		LINEDATA Line[3] ;
		
		Line[0].x1 = x1 ;	Line[0].y1 = y1 ;	Line[0].x2 = x2 ;	Line[0].y2 = y2 ;
		Line[1].x1 = x2 ;	Line[1].y1 = y2 ;	Line[1].x2 = x3 ;	Line[1].y2 = y3 ;
		Line[2].x1 = x3 ;	Line[2].y1 = y3 ;	Line[2].x2 = x1 ;	Line[2].y2 = y1 ;
		Line[2].color = Line[1].color = Line[0].color = Color ;
		Line[2].pal   = Line[1].pal   = Line[0].pal   = GSYS.DrawSetting.BlendParam ;
		return Graphics_Software_DrawLineSet( Line, 3 ) ;
	}
}

// �\�t�g�E�G�A�����_�����O�� DrawQuadrangle
static int	Graphics_Software_DrawQuadrangle( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FillFlag )
{
	// �h��Ԃ��������łȂ����ŏ����𕪊�
	if( FillFlag == TRUE )
	{
		// �h��Ԃ��̏ꍇ

		DX_POINTDATA p[3] ;

		p[0].x = x1 ;		p[0].y = y1 ;
		p[1].x = x2 ;		p[1].y = y2 ;
		p[2].x = x4 ;		p[2].y = y4 ;
		Graphics_Software_DrawPoly( p, NULL, FALSE, Color ) ;

		p[0].x = x3 ;		p[0].y = y3 ;
		p[1].x = x4 ;		p[1].y = y4 ;
		p[2].x = x2 ;		p[2].y = y2 ;
		return Graphics_Software_DrawPoly( p, NULL, FALSE, Color ) ;
	}
	else
	{
		LINEDATA Line[4] ;
		
		Line[0].x1 = x1 ;	Line[0].y1 = y1 ;	Line[0].x2 = x2 ;	Line[0].y2 = y2 ;
		Line[1].x1 = x2 ;	Line[1].y1 = y2 ;	Line[1].x2 = x3 ;	Line[1].y2 = y3 ;
		Line[2].x1 = x3 ;	Line[2].y1 = y3 ;	Line[2].x2 = x4 ;	Line[2].y2 = y4 ;
		Line[3].x1 = x4 ;	Line[3].y1 = y4 ;	Line[3].x2 = x1 ;	Line[3].y2 = y1 ;
		Line[3].color = Line[2].color = Line[1].color = Line[0].color = Color ;
		Line[3].pal   = Line[2].pal   = Line[1].pal   = Line[0].pal   = GSYS.DrawSetting.BlendParam ;
		return Graphics_Software_DrawLineSet( Line, 4 ) ;
	}
}


// �\�t�g�E�G�A�����_�����O�� DrawPixel
static int	Graphics_Software_DrawPixel( int x, int y, unsigned int Color )
{
	DrawPixelMemImg( GSYS.SoftRender.TargetMemImg, x, y, Color ) ;

	// �I��
	return 0 ;
}


// �\�t�g�E�G�A�����_�����O�� DrawPixelSet
static int	Graphics_Software_DrawPixelSet( const POINTDATA *PointData, int Num )
{
	DrawPixelSetMemImg( GSYS.SoftRender.TargetMemImg, PointData, Num ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�����_�����O�� DrawLineSet
static int	Graphics_Software_DrawLineSet( const LINEDATA *LineData, int Num )
{
	DrawLineSetMemImg( GSYS.SoftRender.TargetMemImg, LineData, Num ) ;

	// �I��
	return 0 ;
}




















// �O���t�B�b�N�n���h���쐬�֌W�֐�

// ��̃O���t�B�b�N���쐬
extern	int NS_MakeGraph( int SizeX, int SizeY, int NotUse3DFlag )
{
	SETUP_GRAPHHANDLE_GPARAM GParam ;

	Graphics_Image_InitSetupGraphHandleGParam( &GParam ) ;

	return Graphics_Image_MakeGraph_UseGParam( &GParam, SizeX, SizeY, NotUse3DFlag, FALSE, 0, GetASyncLoadFlag() ) ;
}

// �`��\�ȉ�ʂ��쐬
extern int NS_MakeScreen( int SizeX, int SizeY, int UseAlphaChannel )
{
	SETUP_GRAPHHANDLE_GPARAM GParam ;

	// �\�t�g�E�G�A�����_�����O���[�h�ł̓A���t�@�`�����l���t���̕`��\�摜�͍��Ȃ�
	if( UseAlphaChannel && NS_GetScreenMemToSystemMemFlag() == TRUE )
		return -1 ;

	Graphics_Image_InitSetupGraphHandleGParam( &GParam ) ;

	GParam.DrawValidImageCreateFlag = TRUE ;
	GParam.DrawValidAlphaImageCreateFlag = UseAlphaChannel ;
	return Graphics_Image_MakeGraph_UseGParam( &GParam, SizeX, SizeY, FALSE, FALSE, 0, GetASyncLoadFlag() ) ;
}

// �w��̃O���t�B�b�N�̎w�蕔�������𔲂��o���ĐV���ȃO���t�B�b�N�n���h�����쐬����
extern int NS_DerivationGraph( int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle )
{
	return Graphics_Image_DerivationGraph_UseGParam( FALSE, SrcX, ( float )SrcX, SrcY, ( float )SrcY, Width, ( float )Width, Height, ( float )Height, SrcGraphHandle, FALSE ) ;
}

// �w��̃O���t�B�b�N�n���h���̎w�蕔�������𔲂��o���ĐV���ȃO���t�B�b�N�n���h�����쐬����( float�� )
extern int DerivationGraphF( float SrcX, float SrcY, float Width, float Height, int SrcGraphHandle )
{
	return Graphics_Image_DerivationGraph_UseGParam( TRUE, _FTOL( SrcX ), SrcX, _FTOL( SrcY ), SrcY, _FTOL( Width ), Width, _FTOL( Height ), Height, SrcGraphHandle, FALSE ) ;
}

// �w��̃O���t�B�b�N�f�[�^���폜����
extern	int NS_DeleteGraph( int GrHandle, int )
{
	return SubHandle( GrHandle ) ;
}

// �w��̃O���t�B�b�N�n���h���ƁA�����O���t�B�b�N�f�[�^����h�����Ă���O���t�B�b�N�n���h��( DerivationGraph �Ŕh�������n���h���ALoadDivGraph �ǂݍ���ō쐬���ꂽ�����̃n���h�� )����x�ɍ폜����
extern	int NS_DeleteSharingGraph( int GrHandle )
{
	IMAGEDATA_ORIG *Orig ;
	IMAGEDATA *Image, **TmpImage ;
	bool BreakFlag ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

	// �����O���t�B�b�N�f�[�^����h�����Ă���n���h����S�č폜����
	Orig = Image->Orig ;
	BreakFlag = false ;
	for( TmpImage = ( IMAGEDATA ** )&HandleManageArray[ DX_HANDLETYPE_GRAPH ].Handle[ HandleManageArray[ DX_HANDLETYPE_GRAPH ].AreaMin ] ; ; TmpImage ++ )
	{
		if( *TmpImage == NULL ) continue ;

		if( ( *TmpImage )->Orig == Orig )
		{
			BreakFlag = Orig->RefCount == 1 ;
			NS_DeleteGraph( ( *TmpImage )->HandleInfo.Handle ) ;
			if( BreakFlag ) break ;
		}
	}

	// �I��
	return 0 ;
}

// �L���ȃO���t�B�b�N�̐����擾����
extern	int NS_GetGraphNum( void )
{
	return HandleManageArray[ DX_HANDLETYPE_GRAPH ].Num ;
}

// �O���t�B�b�N�����̐F�œh��Ԃ�
extern	int NS_FillGraph( int GrHandle, int Red, int Green, int Blue, int Alpha )
{
	return Graphics_Image_FillGraph_UseGParam( GrHandle, Red, Green, Blue, Alpha, FALSE ) ;
}

// ������ɗ��Ă�t���O�̃|�C���^���Z�b�g����
extern	int NS_SetGraphLostFlag( int GrHandle, int *LostFlag )
{
	IMAGEDATA *Image ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

	Image->LostFlag = LostFlag ;
	if( LostFlag != NULL ) *LostFlag = FALSE ;

	// �I��
	return 0 ;
}

// InitGraph �ŃO���t�B�b�N�n���h�����폜���邩�ǂ������`�F�b�N����֐�
static int DeleteCancelCheckInitGraphFunction( HANDLEINFO *HandleInfo )
{
	IMAGEDATA *Image = ( IMAGEDATA * )HandleInfo ;

	return Image->NotInitGraphDelete ;
}

// �摜�f�[�^�̏�����
extern	int NS_InitGraph( int )
{
	int Result ;

	// �O���t�B�b�N�n���h����S�č폜
	Result = AllHandleSub( DX_HANDLETYPE_GRAPH, DeleteCancelCheckInitGraphFunction ) ;

	// �V���h�E�}�b�v�n���h�����S�č폜
	Result = AllHandleSub( DX_HANDLETYPE_SHADOWMAP ) ;

	// ���ˑ�����
	Graphics_Hardware_InitGraph_PF() ;

	// �I��
	return Result ;
}

// �t�@�C������ǂݍ��񂾉摜�����ēx�ǂݍ���
extern	int NS_ReloadFileGraphAll( void )
{
	Graphics_Image_DefaultRestoreGraphFunction() ;

	// �I��
	return 0 ;
}






















// �V���h�E�}�b�v�n���h���֌W�֐�

// �V���h�E�}�b�v�n���h�����쐬����
extern int NS_MakeShadowMap( int SizeX, int SizeY )
{
	SETUP_SHADOWMAPHANDLE_GPARAM GParam ;

	// �n�[�h�E�G�A�A�N�Z�����[�^��������������G���[
	if( Graphics_Hardware_CheckValid_PF() == 0 )
		return -1 ;

	GParam.Dummy = 0 ;

	return Graphics_ShadowMap_MakeShadowMap_UseGParam( &GParam, SizeX, SizeY, GetASyncLoadFlag() ) ;
}

// �V���h�E�}�b�v�n���h�����폜����
extern int NS_DeleteShadowMap( int SmHandle )
{
	return SubHandle( SmHandle ) ;
}

// �V���h�E�}�b�v���z�肷�郉�C�g�̕�����ݒ肷��
extern int NS_SetShadowMapLightDirection( int SmHandle, VECTOR Direction )
{
	VECTOR xvec ;
	VECTOR yvec ;
	VECTOR zvec ;
	SHADOWMAPDATA *ShadowMap ;

	// �A�h���X�̎擾
	if( SHADOWMAPCHK( SmHandle, ShadowMap ) )
		return -1 ;

	// ���C�g�x�N�g�������ɉ�]���邽�߂̍s����쐬
	zvec = VNorm( Direction ) ;
	yvec = VGet( 0.0f, 1.0f, 0.0f ) ;
	xvec = VCross( yvec, zvec ) ;
	if( VSquareSize( xvec ) <= 0.0000001f )
	{
		yvec = VGet( 1.0f, 0.0f, 0.0f ) ;
		xvec = VCross( yvec, zvec ) ;
	}
	yvec = VNorm( VCross( zvec, xvec ) ) ;
	xvec = VNorm( xvec ) ;
	ShadowMap->ShadowMapViewMatrix.m[ 0 ][ 0 ] = ShadowMap->LightMatrix.m[ 0 ][ 0 ] = xvec.x ;
	ShadowMap->ShadowMapViewMatrix.m[ 1 ][ 0 ] = ShadowMap->LightMatrix.m[ 1 ][ 0 ] = xvec.y ;
	ShadowMap->ShadowMapViewMatrix.m[ 2 ][ 0 ] = ShadowMap->LightMatrix.m[ 2 ][ 0 ] = xvec.z ;

	ShadowMap->ShadowMapViewMatrix.m[ 0 ][ 1 ] = ShadowMap->LightMatrix.m[ 0 ][ 1 ] = yvec.x ;
	ShadowMap->ShadowMapViewMatrix.m[ 1 ][ 1 ] = ShadowMap->LightMatrix.m[ 1 ][ 1 ] = yvec.y ;
	ShadowMap->ShadowMapViewMatrix.m[ 2 ][ 1 ] = ShadowMap->LightMatrix.m[ 2 ][ 1 ] = yvec.z ;

	ShadowMap->ShadowMapViewMatrix.m[ 0 ][ 2 ] = ShadowMap->LightMatrix.m[ 0 ][ 2 ] = zvec.x ;
	ShadowMap->ShadowMapViewMatrix.m[ 1 ][ 2 ] = ShadowMap->LightMatrix.m[ 1 ][ 2 ] = zvec.y ;
	ShadowMap->ShadowMapViewMatrix.m[ 2 ][ 2 ] = ShadowMap->LightMatrix.m[ 2 ][ 2 ] = zvec.z ;

	// ���C�g�̕�����ۑ�
	ShadowMap->LightDirection = Direction ;

	// �����V���h�E�}�b�v�ւ̕`�揀���ς݂̏ꍇ�̓V���h�E�}�b�v�ւ̕`��ɕK�v�ȍs����X�V����
	if( ShadowMap->DrawSetupFlag )
	{
		// �s��̍X�V
		Graphics_ShadowMap_RefreshMatrix( ShadowMap ) ;
	}

	// �I��
	return 0 ;
}

// �V���h�E�}�b�v�ւ̕`��̏������s��
extern int NS_ShadowMap_DrawSetup( int SmHandle )
{
	int i ;
	SHADOWMAPDATA *ShadowMap ;

	// �A�h���X�̎擾
	if( SHADOWMAPCHK( SmHandle, ShadowMap ) )
		return -1 ;

	// �`��Ŏg�p����V���h�E�}�b�v�ɐݒ肳��Ă��������
	for( i = 0 ; i < MAX_USE_SHADOWMAP_NUM ; i ++ )
	{
		if( GSYS.DrawSetting.ShadowMap[ i ] == SmHandle )
		{
			NS_SetUseShadowMap( i, -1 ) ;
		}
	}

	// ���ɕ`�揀�����������Ă���ꍇ�͉������Ȃ�
	if( ShadowMap->DrawSetupFlag == TRUE )
		return -1 ;

	// �ʂ̃V���h�E�}�b�v�ɑ΂���`�揀�����������Ă�����A������̕`�抮����Ԃ���������
	if( GSYS.DrawSetting.ShadowMapDraw == TRUE )
	{
		NS_ShadowMap_DrawEnd() ;
	}

	// ���X�̕`��Ώۂ�ۑ�
	for( i = 0 ; i < DX_RENDERTARGET_COUNT ; i ++ )
	{
		ShadowMap->RenderTargetScreen[ i ]        = GSYS.DrawSetting.TargetScreen[ i ] ;
		ShadowMap->RenderTargetScreenSurface[ i ] = GSYS.DrawSetting.TargetScreenSurface[ i ] ;
	}

	// ���݂̕`����W�v�Z�p�̏���ۑ�
#ifndef DX_NON_MASK
	ShadowMap->RenderMaskUseFlag          = MASKD.MaskUseFlag ;
#endif

	ShadowMap->RenderCameraPosition       = GSYS.Camera.Position ;
	ShadowMap->RenderCameraTarget         = GSYS.Camera.Target ;
	ShadowMap->RenderCameraUp             = GSYS.Camera.Up ;
	ShadowMap->RenderCameraHRotate        = GSYS.Camera.HRotate ;
	ShadowMap->RenderCameraVRotate        = GSYS.Camera.VRotate ;
	ShadowMap->RenderCameraTRotate        = GSYS.Camera.TRotate ;
	ShadowMap->RenderCameraMatrix         = GSYS.Camera.Matrix ;
	ShadowMap->RenderCameraScreenCenterX  = GSYS.Camera.ScreenCenterX ;
	ShadowMap->RenderCameraScreenCenterY  = GSYS.Camera.ScreenCenterY ;

	ShadowMap->RenderProjectionMatrixMode = GSYS.DrawSetting.ProjectionMatrixMode ;
	ShadowMap->RenderProjNear             = GSYS.DrawSetting.ProjNear ;
	ShadowMap->RenderProjFar              = GSYS.DrawSetting.ProjFar ;
	ShadowMap->RenderProjDotAspect        = GSYS.DrawSetting.ProjDotAspect ;
	ShadowMap->RenderProjFov              = GSYS.DrawSetting.ProjFov ;
	ShadowMap->RenderProjSize             = GSYS.DrawSetting.ProjSize ;
	ShadowMap->RenderProjMatrix           = GSYS.DrawSetting.ProjMatrix ;

	// �`��͈͂��ݒ肳��Ă��Ȃ��ꍇ�Ɏg�p����`��͈͂��Z�o����
	{
		VECTOR AddXVecP ;
		VECTOR AddXVecM ;
		VECTOR AddYVecP ;
		VECTOR AddYVecM ;
		VECTOR AddZVecP ;
		VECTOR AddZVecM ;

		// �`��͈͂ɓ����̈�͎�����̌���Q�{�A���E�E�㉺�P�D�T�{�A�ɂ���
		AddXVecP = VSub( NS_ConvScreenPosToWorldPos( VGet( GSYS.DrawSetting.DrawAreaF.right,                           0.0f, 1.0f ) ), NS_ConvScreenPosToWorldPos( VGet( GSYS.DrawSetting.DrawAreaF.left,                              0.0f, 1.0f ) ) ) ;
		AddYVecP = VSub( NS_ConvScreenPosToWorldPos( VGet(                             0.0f, GSYS.DrawSetting.DrawAreaF.top, 1.0f ) ), NS_ConvScreenPosToWorldPos( VGet(                            0.0f, GSYS.DrawSetting.DrawAreaF.bottom, 1.0f ) ) ) ;
		AddZVecP = VSub( NS_ConvScreenPosToWorldPos( VGet(                             0.0f,                           0.0f, 1.0f ) ), NS_ConvScreenPosToWorldPos( VGet(                            0.0f,                              0.0f, 0.0f ) ) ) ;
		AddXVecM = VScale( AddXVecP, -1.0f ) ;
		AddYVecM = VScale( AddYVecP, -1.0f ) ;
		AddZVecM = VScale( AddZVecP, -1.0f ) ;

		AddXVecP = VScale( AddXVecP, 0.5f ) ;
		AddXVecM = VScale( AddXVecM, 0.5f ) ;
		AddYVecP = VScale( AddYVecP, 0.5f ) ;
		AddYVecM = VScale( AddYVecM, 0.5f ) ;
	//	AddZVecP = VScale( AddZVecP, 0.5f ) ;
	//	AddZVecM = VScale( AddZVecM, 0.5f ) ;

		// ���C�g�x�N�g�������Ƀr���[�N���b�v���W����]���āA�ő�l�ƍŏ��l���Z�o
		for( i = 0 ; i < 8 ; i ++ )
		{
			ShadowMap->DefaultViewClipPos[ i ] = VConvDtoF( ( ( VECTOR_D * )GSYS.DrawSetting.ViewClipPos )[ i ] ) ;
		}
		
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 0 ], &ShadowMap->DefaultViewClipPos[ 0 ], &AddZVecM ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 1 ], &ShadowMap->DefaultViewClipPos[ 1 ], &AddZVecM ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 2 ], &ShadowMap->DefaultViewClipPos[ 2 ], &AddZVecM ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 3 ], &ShadowMap->DefaultViewClipPos[ 3 ], &AddZVecM ) ;
		
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 0 ], &ShadowMap->DefaultViewClipPos[ 0 ], &AddXVecM ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 2 ], &ShadowMap->DefaultViewClipPos[ 2 ], &AddXVecM ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 4 ], &ShadowMap->DefaultViewClipPos[ 4 ], &AddXVecM ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 6 ], &ShadowMap->DefaultViewClipPos[ 6 ], &AddXVecM ) ;

		VectorAdd( &ShadowMap->DefaultViewClipPos[ 1 ], &ShadowMap->DefaultViewClipPos[ 1 ], &AddXVecP ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 3 ], &ShadowMap->DefaultViewClipPos[ 3 ], &AddXVecP ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 5 ], &ShadowMap->DefaultViewClipPos[ 5 ], &AddXVecP ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 7 ], &ShadowMap->DefaultViewClipPos[ 7 ], &AddXVecP ) ;
		
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 0 ], &ShadowMap->DefaultViewClipPos[ 0 ], &AddYVecP ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 1 ], &ShadowMap->DefaultViewClipPos[ 1 ], &AddYVecP ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 4 ], &ShadowMap->DefaultViewClipPos[ 4 ], &AddYVecP ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 5 ], &ShadowMap->DefaultViewClipPos[ 5 ], &AddYVecP ) ;

		VectorAdd( &ShadowMap->DefaultViewClipPos[ 2 ], &ShadowMap->DefaultViewClipPos[ 2 ], &AddYVecM ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 3 ], &ShadowMap->DefaultViewClipPos[ 3 ], &AddYVecM ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 6 ], &ShadowMap->DefaultViewClipPos[ 6 ], &AddYVecM ) ;
		VectorAdd( &ShadowMap->DefaultViewClipPos[ 7 ], &ShadowMap->DefaultViewClipPos[ 7 ], &AddYVecM ) ;
	}

#ifndef DX_NON_MASK
	// �}�X�N��ʂ��g�p���Ȃ��ݒ�ɕύX
	NS_SetUseMaskScreenFlag( FALSE ) ;
#endif

	// �`��Ώۂ��V���h�E�}�b�v�ɕύX
	GSYS.DrawSetting.ShadowMapDrawSetupRequest = TRUE ;
	NS_SetDrawScreen( SmHandle ) ;
	for( i = 1 ; i < DX_RENDERTARGET_COUNT ; i ++ )
	{
		NS_SetRenderTargetToShader( i, -1 ) ;
	}

	// �V���h�E�}�b�v�ւ̃����_�����O�ɕK�v�ȍs��̃Z�b�g�A�b�v
	Graphics_ShadowMap_RefreshMatrix( ShadowMap ) ;

	// �`�揀�������Ă���t���O�𗧂Ă�
	ShadowMap->DrawSetupFlag = TRUE ;

	// ���ˑ�����
	Graphics_Hardware_ShadowMap_DrawSetup_PF( ShadowMap ) ;

	// ��ʂ��Ő[�x�ŃN���A
	{
		int Red, Green, Blue, Enable ;

		Red   = GSYS.Screen.BackgroundRed ;
		Green = GSYS.Screen.BackgroundGreen ;
		Blue  = GSYS.Screen.BackgroundBlue ;
		Enable = GSYS.Screen.EnableBackgroundColor ;

		NS_SetBackgroundColor( 255,255,255 ) ;
		NS_ClearDrawScreen() ;

		GSYS.Screen.BackgroundRed = Red ;
		GSYS.Screen.BackgroundGreen = Green ;
		GSYS.Screen.BackgroundBlue = Blue ;
		GSYS.Screen.EnableBackgroundColor = Enable ;
	}

	// �I��
	return 0 ;
}

// �V���h�E�}�b�v�ւ̕`����I������
extern int NS_ShadowMap_DrawEnd( void )
{
	SHADOWMAPDATA *ShadowMap ;
	int i ;

	// �`�揀�������Ă��Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.DrawSetting.ShadowMapDraw == FALSE )
		return -1 ;

	// �A�h���X�̎擾
	if( SHADOWMAPCHK( GSYS.DrawSetting.ShadowMapDrawHandle, ShadowMap ) )
	{
		GSYS.DrawSetting.ShadowMapDraw = FALSE ;
		return -1 ;
	}

	// �`�揀�������Ă��Ȃ��ꍇ�͉������Ȃ�
	if( ShadowMap->DrawSetupFlag == FALSE )
		return -1 ;

	// �`�揀�������ς݃t���O��|��
	ShadowMap->DrawSetupFlag = FALSE ;

	// �V���h�E�}�b�v�ɑ΂���`��ł��邩�ǂ����̃t���O��|��
	GSYS.DrawSetting.ShadowMapDraw = FALSE ;
	GSYS.DrawSetting.ShadowMapDrawHandle = 0 ;

	// �`�������ɖ߂�
	for( i = 0 ; i < DX_RENDERTARGET_COUNT ; i ++ )
	{
		NS_SetRenderTargetToShader( i, ShadowMap->RenderTargetScreen[ i ], ShadowMap->RenderTargetScreenSurface[ i ] ) ; 
	}

#ifndef DX_NON_MASK
	// �}�X�N��ʂ��g�p����ݒ�����ɖ߂�
	NS_SetUseMaskScreenFlag( ShadowMap->RenderMaskUseFlag ) ;
#endif

	// �J�����ݒ�����ɖ߂�
	GSYS.Camera.Position      = ShadowMap->RenderCameraPosition ;
	GSYS.Camera.Target        = ShadowMap->RenderCameraTarget ;
	GSYS.Camera.Up            = ShadowMap->RenderCameraUp ;
	GSYS.Camera.HRotate       = ShadowMap->RenderCameraHRotate ;
	GSYS.Camera.VRotate       = ShadowMap->RenderCameraVRotate ;
	GSYS.Camera.TRotate       = ShadowMap->RenderCameraTRotate ;
	GSYS.Camera.Matrix        = ShadowMap->RenderCameraMatrix ;
	NS_SetTransformToViewD( &ShadowMap->RenderCameraMatrix ) ;
	NS_SetCameraScreenCenterD( ShadowMap->RenderCameraScreenCenterX, ShadowMap->RenderCameraScreenCenterY ) ;

	// �ˉe�p�����[�^�����Z�b�g
	GSYS.DrawSetting.ProjectionMatrixMode = ShadowMap->RenderProjectionMatrixMode ;
	GSYS.DrawSetting.ProjNear             = ShadowMap->RenderProjNear ;
	GSYS.DrawSetting.ProjFar              = ShadowMap->RenderProjFar ;
	GSYS.DrawSetting.ProjDotAspect        = ShadowMap->RenderProjDotAspect ;
	GSYS.DrawSetting.ProjFov              = ShadowMap->RenderProjFov ;
	GSYS.DrawSetting.ProjSize             = ShadowMap->RenderProjSize ;
	GSYS.DrawSetting.ProjMatrix           = ShadowMap->RenderProjMatrix ;
	Graphics_DrawSetting_SetTransformToProjection_Direct( &GSYS.DrawSetting.ProjMatrix ) ;

	// ���ˑ�����
	Graphics_Hardware_ShadowMap_DrawEnd_PF( ShadowMap ) ;

#ifndef DX_NON_FILTER

	// �V���h�E�}�b�v�Ƀu���[���|����
	if( ShadowMap->BlurParam > 0 )
	{
		NS_GraphFilter( ShadowMap->HandleInfo.Handle, DX_GRAPH_FILTER_GAUSS, 8, ShadowMap->BlurParam ) ;
	}

#endif // DX_NON_FILTER

	// �I��
	return 0 ;
}

// �`��Ŏg�p����V���h�E�}�b�v���w�肷��A�X���b�g�͂O���P�����w��\�ASmHandle �� -1 ��n���Ǝw��̃X���b�g�̃V���h�E�}�b�v������
extern int NS_SetUseShadowMap( int SlotIndex, int SmHandle )
{
	SHADOWMAPDATA *ShadowMap = NULL ;

	// �X���b�g�̃C���f�b�N�X���s���ȏꍇ�̓G���[
	if( SlotIndex < 0 || SlotIndex >= MAX_USE_SHADOWMAP_NUM )
		return -1 ;

	// �n���h���l���}�C�i�X�̒l�̏ꍇ�͉���
	if( SmHandle < 0 )
	{
		// �L���ȃV���h�E�}�b�v��ݒ肵�Ă��鐔���X�V
		if( GSYS.DrawSetting.ShadowMap[ SlotIndex ] != 0 )
		{
			GSYS.DrawSetting.UseShadowMapNum -- ;
		}

		// �n���h���l�����Z�b�g
		GSYS.DrawSetting.ShadowMap[ SlotIndex ] = 0 ;
	}
	else
	{
		// �A�h���X�̎擾
		if( SHADOWMAPCHK( SmHandle, ShadowMap ) )
			return -1 ;

		// �L���ȃV���h�E�}�b�v��ݒ肵�Ă��鐔���X�V
		if( GSYS.DrawSetting.ShadowMap[ SlotIndex ] == 0 )
		{
			GSYS.DrawSetting.UseShadowMapNum ++ ;
		}

		// �n���h���l�̕ۑ�
		GSYS.DrawSetting.ShadowMap[ SlotIndex ] = SmHandle ;
	}

	// ���ˑ�����
	Graphics_Hardware_ShadowMap_SetUse_PF( SlotIndex, ShadowMap ) ;

	// �V�F�[�_�[�̃p�����[�^�ɔ��f
	Graphics_ShadowMap_RefreshVSParam() ;
	Graphics_ShadowMap_RefreshPSParam() ;

	// �I��
	return 0 ;
}

// �V���h�E�}�b�v�ɕ`�悷��ۂ͈̔͂�ݒ肷��( ���̊֐��ŕ`��͈͂�ݒ肵�Ȃ��ꍇ�͎�������g�債���͈͂��`��͈͂ƂȂ� )
extern int NS_SetShadowMapDrawArea( int SmHandle, VECTOR MinPosition, VECTOR MaxPosition )
{
	SHADOWMAPDATA *ShadowMap ;

	// �G���[����
	if( SHADOWMAPCHK( SmHandle, ShadowMap ) )
		return -1 ;

	// ���W��ۑ�
	if( MinPosition.x < MaxPosition.x )
	{
		ShadowMap->DrawAreaMinPosition.x = MinPosition.x ;
		ShadowMap->DrawAreaMaxPosition.x = MaxPosition.x ;
	}
	else
	{
		ShadowMap->DrawAreaMinPosition.x = MaxPosition.x ;
		ShadowMap->DrawAreaMaxPosition.x = MinPosition.x ;
	}

	if( MinPosition.y < MaxPosition.y )
	{
		ShadowMap->DrawAreaMinPosition.y = MinPosition.y ;
		ShadowMap->DrawAreaMaxPosition.y = MaxPosition.y ;
	}
	else
	{
		ShadowMap->DrawAreaMinPosition.y = MaxPosition.y ;
		ShadowMap->DrawAreaMaxPosition.y = MinPosition.y ;
	}

	if( MinPosition.z < MaxPosition.z )
	{
		ShadowMap->DrawAreaMinPosition.z = MinPosition.z ;
		ShadowMap->DrawAreaMaxPosition.z = MaxPosition.z ;
	}
	else
	{
		ShadowMap->DrawAreaMinPosition.z = MaxPosition.z ;
		ShadowMap->DrawAreaMaxPosition.z = MinPosition.z ;
	}

	// ���������ꍇ�͖���
	if( ShadowMap->DrawAreaMaxPosition.x - ShadowMap->DrawAreaMinPosition.x < 0.00000001f ||
		ShadowMap->DrawAreaMaxPosition.y - ShadowMap->DrawAreaMinPosition.y < 0.00000001f ||
		ShadowMap->DrawAreaMaxPosition.z - ShadowMap->DrawAreaMinPosition.z < 0.00000001f )
	{
		return -1 ;
	}

	// �t���O��L���ɐݒ�
	ShadowMap->EnableDrawArea = TRUE ;

	// �V���h�E�}�b�v�ɕ`�悷��͈͂̍��W���X�V
	ShadowMap->DrawAreaViewClipPos[ 0 ].x = ShadowMap->DrawAreaMinPosition.x ;
	ShadowMap->DrawAreaViewClipPos[ 0 ].y = ShadowMap->DrawAreaMinPosition.y ;
	ShadowMap->DrawAreaViewClipPos[ 0 ].z = ShadowMap->DrawAreaMinPosition.z ;

	ShadowMap->DrawAreaViewClipPos[ 1 ].x = ShadowMap->DrawAreaMaxPosition.x ;
	ShadowMap->DrawAreaViewClipPos[ 1 ].y = ShadowMap->DrawAreaMinPosition.y ;
	ShadowMap->DrawAreaViewClipPos[ 1 ].z = ShadowMap->DrawAreaMinPosition.z ;

	ShadowMap->DrawAreaViewClipPos[ 2 ].x = ShadowMap->DrawAreaMinPosition.x ;
	ShadowMap->DrawAreaViewClipPos[ 2 ].y = ShadowMap->DrawAreaMaxPosition.y ;
	ShadowMap->DrawAreaViewClipPos[ 2 ].z = ShadowMap->DrawAreaMinPosition.z ;

	ShadowMap->DrawAreaViewClipPos[ 3 ].x = ShadowMap->DrawAreaMaxPosition.x ;
	ShadowMap->DrawAreaViewClipPos[ 3 ].y = ShadowMap->DrawAreaMaxPosition.y ;
	ShadowMap->DrawAreaViewClipPos[ 3 ].z = ShadowMap->DrawAreaMinPosition.z ;

	ShadowMap->DrawAreaViewClipPos[ 4 ].x = ShadowMap->DrawAreaMinPosition.x ;
	ShadowMap->DrawAreaViewClipPos[ 4 ].y = ShadowMap->DrawAreaMinPosition.y ;
	ShadowMap->DrawAreaViewClipPos[ 4 ].z = ShadowMap->DrawAreaMaxPosition.z ;

	ShadowMap->DrawAreaViewClipPos[ 5 ].x = ShadowMap->DrawAreaMaxPosition.x ;
	ShadowMap->DrawAreaViewClipPos[ 5 ].y = ShadowMap->DrawAreaMinPosition.y ;
	ShadowMap->DrawAreaViewClipPos[ 5 ].z = ShadowMap->DrawAreaMaxPosition.z ;

	ShadowMap->DrawAreaViewClipPos[ 6 ].x = ShadowMap->DrawAreaMinPosition.x ;
	ShadowMap->DrawAreaViewClipPos[ 6 ].y = ShadowMap->DrawAreaMaxPosition.y ;
	ShadowMap->DrawAreaViewClipPos[ 6 ].z = ShadowMap->DrawAreaMaxPosition.z ;

	ShadowMap->DrawAreaViewClipPos[ 7 ].x = ShadowMap->DrawAreaMaxPosition.x ;
	ShadowMap->DrawAreaViewClipPos[ 7 ].y = ShadowMap->DrawAreaMaxPosition.y ;
	ShadowMap->DrawAreaViewClipPos[ 7 ].z = ShadowMap->DrawAreaMaxPosition.z ;

	// �����V���h�E�}�b�v�ւ̕`�揀���ς݂̏ꍇ�̓V���h�E�}�b�v�ւ̕`��ɕK�v�ȍs����X�V����
	if( ShadowMap->DrawSetupFlag )
	{
		// �s��̍X�V
		Graphics_ShadowMap_RefreshMatrix( ShadowMap ) ;
	}

	// �I��
	return 0 ;
}

// SetShadowMapDrawArea �̐ݒ����������
extern int NS_ResetShadowMapDrawArea( int SmHandle )
{
	SHADOWMAPDATA *ShadowMap ;

	// �G���[����
	if( SHADOWMAPCHK( SmHandle, ShadowMap ) )
		return -1 ;

	// ���ɐݒ肪��������Ă����牽�����Ȃ�
	if( ShadowMap->EnableDrawArea == FALSE )
	{
		return 0 ;
	}

	// �t���O��|��
	ShadowMap->EnableDrawArea = FALSE ;

	// �����V���h�E�}�b�v�ւ̕`�揀���ς݂̏ꍇ�̓V���h�E�}�b�v�ւ̕`��ɕK�v�ȍs����X�V����
	if( ShadowMap->DrawSetupFlag )
	{
		// �s��̍X�V
		Graphics_ShadowMap_RefreshMatrix( ShadowMap ) ;
	}

	// �I��
	return 0 ;
}

// �V���h�E�}�b�v���g�p�����`�掞�̕␳�[�x��ݒ肷��
extern int NS_SetShadowMapAdjustDepth( int SmHandle, float Depth )
{
	SHADOWMAPDATA *ShadowMap ;
	int i ;

	// �G���[����
	if( SHADOWMAPCHK( SmHandle, ShadowMap ) )
		return -1 ;

	// �␳�l��ۑ�����
	ShadowMap->AdjustDepth = Depth ;

	// ���ɃV���h�E�}�b�v���g�p���̏ꍇ�͕␳�l��ύX����
	for( i = 0 ; i < MAX_USE_SHADOWMAP_NUM ; i ++ )
	{
		if( GSYS.DrawSetting.ShadowMap[ i ] == SmHandle )
		{
			Graphics_ShadowMap_RefreshPSParam() ;
			break ;
		}
	}

	// �I��
	return 0 ;
}

// �V���h�E�}�b�v�ɓK�p����ڂ����x������ݒ肷��
extern int NS_SetShadowMapBlurParam( int SmHandle, int Param )
{
	SHADOWMAPDATA *ShadowMap ;

	// �G���[����
	if( SHADOWMAPCHK( SmHandle, ShadowMap ) )
		return -1 ;

	// �␳�l��ۑ�����
	ShadowMap->BlurParam = Param ;

	// �I��
	return 0 ;
}

// �V���h�E�}�b�v���g�p�����`�掞�̉e���O���f�[�V����������͈͂�ݒ肷��
extern int NS_SetShadowMapGradationParam( int SmHandle, float Param )
{
	SHADOWMAPDATA *ShadowMap ;
	int i ;

	// �G���[����
	if( SHADOWMAPCHK( SmHandle, ShadowMap ) )
		return -1 ;

	// �O���f�[�V����������͈͂�ۑ�����
	ShadowMap->GradationParam = Param ;

	// ���ɃV���h�E�}�b�v���g�p���̏ꍇ�͕␳�l��ύX����
	for( i = 0 ; i < MAX_USE_SHADOWMAP_NUM ; i ++ )
	{
		if( GSYS.DrawSetting.ShadowMap[ i ] == SmHandle )
		{
			Graphics_ShadowMap_RefreshPSParam() ;
			break ;
		}
	}

	// �I��
	return 0 ;
}

// �V���h�E�}�b�v����ʂɃe�X�g�`�悷��
#define SETDRAWRECTCODE_DRAWSHADOWMAP\
	if( x1 < x2 ){ DrawRect.left = x1 ; DrawRect.right = x2; }\
	if( y1 < y2 ){ DrawRect.top = y1 ; DrawRect.bottom = y2; }\
\
	SETRECT( DrawRect, x1, y1, x2, y2 ) ;\
	DRAWRECT_DRAWAREA_CLIP

extern int NS_TestDrawShadowMap( int SmHandle, int x1, int y1, int x2, int y2 )
{
	SHADOWMAPDATA *ShadowMap ;
	VERTEX_2D DrawVert[ 6 ] ;
	int Ret, Flag ;
	float AdjustPos ;

	CheckActiveState() ;

	// �A�h���X�̎擾
	if( SHADOWMAPCHK( SmHandle, ShadowMap ) )
		return -1 ;

	Ret = -1 ;
	Flag = GSYS.Setting.ValidHardware ;

	AdjustPos = GSYS.HardInfo.DrawFloatCoordType == DX_DRAWFLOATCOORDTYPE_DIRECT3D9 ? -0.5f : 0.0f ;

	DrawVert[ 0 ].rhw = 1.0f ;
	DrawVert[ 0 ].pos.z = GSYS.DrawSetting.DrawZ ;
	DrawVert[ 0 ].color = 0xffffffff ;

	DrawVert[ 1 ] = DrawVert[ 0 ] ;
	DrawVert[ 2 ] = DrawVert[ 0 ] ;
	DrawVert[ 3 ] = DrawVert[ 0 ] ;

	DrawVert[ 0 ].pos.x = ( float )x1 + AdjustPos ;
	DrawVert[ 0 ].pos.y = ( float )y1 + AdjustPos ;
	DrawVert[ 0 ].u = 0.0f ;
	DrawVert[ 0 ].v = 0.0f ;

	DrawVert[ 1 ].pos.x = ( float )x2 + AdjustPos ;
	DrawVert[ 1 ].pos.y = ( float )y1 + AdjustPos ;
	DrawVert[ 1 ].u = 1.0f ;
	DrawVert[ 1 ].v = 0.0f ;

	DrawVert[ 2 ].pos.x = ( float )x1 + AdjustPos ;
	DrawVert[ 2 ].pos.y = ( float )y2 + AdjustPos ;
	DrawVert[ 2 ].u = 0.0f ;
	DrawVert[ 2 ].v = 1.0f ;

	DrawVert[ 3 ].pos.x = ( float )x2 + AdjustPos ;
	DrawVert[ 3 ].pos.y = ( float )y2 + AdjustPos ;
	DrawVert[ 3 ].u = 1.0f ;
	DrawVert[ 3 ].v = 1.0f ;

	DrawVert[ 4 ] = DrawVert[ 2 ] ;
	DrawVert[ 5 ] = DrawVert[ 1 ] ;

	DRAW_DEF(
		Graphics_Hardware_DrawPrimitive2D_PF( DrawVert, 6, DX_PRIMTYPE_TRIANGLELIST, ( IMAGEDATA * )SmHandle, FALSE, FALSE, FALSE, FALSE, 0, TRUE ),
		0,
		SETDRAWRECTCODE_DRAWSHADOWMAP,
		Ret,
		Flag
	)

	// �I��
	return Ret ;
}


























// �O���t�B�b�N�n���h���ւ̉摜�]���֐�

// �摜�f�[�^�̓]��
extern	int NS_BltBmpToGraph( const COLORDATA *BmpColorData, HBITMAP RgbBmp, HBITMAP AlphaBmp, int CopyPointX, int CopyPointY, int GrHandle )
{
	return NS_BltBmpOrGraphImageToGraph( BmpColorData, RgbBmp, AlphaBmp,
											TRUE, NULL, NULL,
											CopyPointX, CopyPointY, GrHandle ) ;
}

// �����摜�ւ̂a�l�o�̓]��
extern	int NS_BltBmpToDivGraph( const COLORDATA *BmpColorData, HBITMAP RgbBmp, HBITMAP AlphaBmp, int AllNum, int XNum, int YNum, int Width, int Height, const int *GrHandle, int ReverseFlag )
{
	return NS_BltBmpOrGraphImageToDivGraph( BmpColorData, RgbBmp, AlphaBmp,
												TRUE, NULL, NULL,
												AllNum, XNum, YNum, Width, Height, GrHandle, ReverseFlag ) ;
}

// �a�l�o �� GraphImage ���摜�ɓ]��
extern int NS_BltBmpOrGraphImageToGraph(
	const COLORDATA	*BmpColorData,
	      HBITMAP	RgbBmp,
	      HBITMAP	AlphaBmp,
	      int		BmpFlag,
	const BASEIMAGE	*RgbBaseImage,
	const BASEIMAGE	*AlphaBaseImage,
	      int		CopyPointX,
	      int		CopyPointY,
	      int		GrHandle
)
{
	int			Result ;
#ifdef __WINDOWS__
	BITMAP		Bmp ;
	BASEIMAGE	TempBaseRGB ;
	BASEIMAGE	TempBaseAlpha = { 0 } ;
#endif // __WINDOWS__

	if( GSYS.InitializeFlag == FALSE ) return -1 ;

	// Bitmap �̏ꍇ�́ABASEIMAGE �̏����쐬����
	if( BmpFlag )
	{
#ifdef __WINDOWS__
		GetObject( RgbBmp, sizeof( BITMAP ), ( void * )&Bmp ) ;
		TempBaseRGB.GraphData      = Bmp.bmBits ;
		TempBaseRGB.Width          = Bmp.bmWidth ;
		TempBaseRGB.Height         = Bmp.bmHeight ;
		TempBaseRGB.Pitch          = ( Bmp.bmWidthBytes + 3 ) / 4 * 4 ;
		TempBaseRGB.ColorData      = *BmpColorData ;
		TempBaseRGB.MipMapCount    = 0 ;
		TempBaseRGB.GraphDataCount = 0 ;

		RgbBaseImage = &TempBaseRGB ;

		if( AlphaBmp )
		{
			GetObject( AlphaBmp, sizeof( BITMAP ), ( void * )&Bmp ) ;
			TempBaseAlpha.GraphData      = Bmp.bmBits ;
			TempBaseAlpha.Width          = Bmp.bmWidth ;
			TempBaseAlpha.Height         = Bmp.bmHeight ;
			TempBaseAlpha.Pitch          = ( Bmp.bmWidthBytes + 3 ) / 4 * 4 ;
			TempBaseAlpha.ColorData      = *BmpColorData ;
			TempBaseAlpha.MipMapCount    = 0 ;
			TempBaseAlpha.GraphDataCount = 0 ;

			AlphaBaseImage = &TempBaseAlpha ;
		}
		else
		{
			AlphaBaseImage = NULL ;
		}
#else // __WINDOWS__
		return -1 ;
#endif // __WINDOWS__
	}

	Result = Graphics_Image_BltBmpOrGraphImageToGraphBase( 
				RgbBaseImage,
				AlphaBaseImage,
				CopyPointX,
				CopyPointY,
				GrHandle,
				GSYS.CreateImage.NotUseTransColor ? FALSE : TRUE
			) ;

	if( Result == 0 )
	{
		if( BmpFlag )
		{
			Graphics_Image_SetGraphBaseInfo( GrHandle, NULL, BmpColorData, RgbBmp, AlphaBmp, NULL, 0, NULL, 0, NULL, NULL, FALSE, FALSE, -1, FALSE ) ;
		}
		else
		{
			Graphics_Image_SetGraphBaseInfo( GrHandle, NULL, NULL, NULL, NULL, NULL, 0, NULL, 0, RgbBaseImage, AlphaBaseImage, FALSE, FALSE, -1, FALSE ) ;
		}
	}

	return Result ;
}

// �a�l�o �� GraphImage ���摜�ɓ]��
extern	int NS_BltBmpOrGraphImageToGraph2(
	const COLORDATA	*BmpColorData,
	      HBITMAP	RgbBmp,
	      HBITMAP	AlphaBmp,
	      int		BmpFlag,
	const BASEIMAGE	*RgbBaseImage,
	const BASEIMAGE	*AlphaBaseImage,
	const RECT		*SrcRect,
	      int		DestX,
	      int		DestY,
	      int		GrHandle
)
{
#ifdef __WINDOWS__
	BITMAP			Bmp ;
	BASEIMAGE		TempBaseRGB ;
	BASEIMAGE		TempBaseAlpha = { 0 } ;
#endif // __WINDOWS__

	// Bitmap �̏ꍇ�́ABASEIMAGE �̏����쐬����
	if( BmpFlag )
	{
#ifdef __WINDOWS__
		GetObject( RgbBmp, sizeof( BITMAP ), ( void * )&Bmp ) ;
		TempBaseRGB.GraphData      = Bmp.bmBits ;
		TempBaseRGB.Width          = Bmp.bmWidth ;
		TempBaseRGB.Height         = Bmp.bmHeight ;
		TempBaseRGB.Pitch          = ( Bmp.bmWidthBytes + 3 ) / 4 * 4 ;
		TempBaseRGB.ColorData      = *BmpColorData ;
		TempBaseRGB.MipMapCount    = 0 ;
		TempBaseRGB.GraphDataCount = 0 ;

		RgbBaseImage = &TempBaseRGB ;

		if( AlphaBmp )
		{
			GetObject( AlphaBmp, sizeof( BITMAP ), ( void * )&Bmp ) ;
			TempBaseAlpha.GraphData      = Bmp.bmBits ;
			TempBaseAlpha.Width          = Bmp.bmWidth ;
			TempBaseAlpha.Height         = Bmp.bmHeight ;
			TempBaseAlpha.Pitch          = ( Bmp.bmWidthBytes + 3 ) / 4 * 4 ;
			TempBaseAlpha.ColorData      = *BmpColorData ;
			TempBaseAlpha.MipMapCount    = 0 ;
			TempBaseAlpha.GraphDataCount = 0 ;

			AlphaBaseImage = &TempBaseAlpha ;
		}
		else
		{
			AlphaBaseImage = NULL ;
		}
#else // __WINDOWS__
		return -1 ;
#endif // __WINDOWS__
	}

	// �摜�̓]��
	return Graphics_Image_BltBmpOrGraphImageToGraph2Base(
				RgbBaseImage,
				AlphaBaseImage,
				SrcRect,
				DestX,
				DestY,
				GrHandle,
				GSYS.CreateImage.NotUseTransColor ? FALSE : TRUE ) ;
}

// �����摜�ւ� �a�l�o �� GraphImage �̓]��
extern	int NS_BltBmpOrGraphImageToDivGraph(
	const COLORDATA	*BmpColorData,
	      HBITMAP	RgbBmp,
	      HBITMAP	AlphaBmp,
	      int		BmpFlag,
	const BASEIMAGE	*RgbBaseImage,
	const BASEIMAGE	*AlphaBaseImage,
	      int		AllNum,
	      int		XNum,
	      int		YNum,
	      int		Width,
	      int		Height,
	const int		*GrHandle,
	      int		ReverseFlag
)
{
	int Result ;
	int i ;
#ifdef __WINDOWS__
	BITMAP		Bmp ;
	BASEIMAGE	TempBaseRGB ;
	BASEIMAGE	TempBaseAlpha = { 0 } ;
#endif // __WINDOWS__

	if( GSYS.InitializeFlag == FALSE ) return -1 ;

	// Bitmap �̏ꍇ�́ABASEIMAGE �̏����쐬����
	if( BmpFlag )
	{
#ifdef __WINDOWS__
		GetObject( RgbBmp, sizeof( BITMAP ), ( void * )&Bmp ) ;
		TempBaseRGB.GraphData      = Bmp.bmBits ;
		TempBaseRGB.Width          = Bmp.bmWidth ;
		TempBaseRGB.Height         = Bmp.bmHeight ;
		TempBaseRGB.Pitch          = ( Bmp.bmWidthBytes + 3 ) / 4 * 4 ;
		TempBaseRGB.ColorData      = *BmpColorData ;
		TempBaseRGB.MipMapCount    = 0 ;
		TempBaseRGB.GraphDataCount = 0 ;

		RgbBaseImage = &TempBaseRGB ;

		if( AlphaBmp )
		{
			GetObject( AlphaBmp, sizeof( BITMAP ), ( void * )&Bmp ) ;
			TempBaseAlpha.GraphData      = Bmp.bmBits ;
			TempBaseAlpha.Width          = Bmp.bmWidth ;
			TempBaseAlpha.Height         = Bmp.bmHeight ;
			TempBaseAlpha.Pitch          = ( Bmp.bmWidthBytes + 3 ) / 4 * 4 ;
			TempBaseAlpha.ColorData      = *BmpColorData ;
			TempBaseAlpha.MipMapCount    = 0 ;
			TempBaseAlpha.GraphDataCount = 0 ;

			AlphaBaseImage = &TempBaseAlpha ;
		}
		else
		{
			AlphaBaseImage = NULL ;
		}
#else // __WINDOWS__
		return -1 ;
#endif // __WINDOWS__
	}

	// �]������
	Result = Graphics_Image_BltBmpOrGraphImageToDivGraphBase(
		RgbBaseImage,
		AlphaBaseImage,
		AllNum,
		XNum,
		YNum,
		FALSE,
		Width, ( float )Width,
		Height, ( float )Height,
		GrHandle,
		ReverseFlag,
		GSYS.CreateImage.NotUseTransColor ? FALSE : TRUE,
		FALSE
	) ;

	// �ŏ��̉摜�Ƀt�@�C�������Z�b�g
	if( BmpFlag )
	{
		Graphics_Image_SetGraphBaseInfo( GrHandle[ 0 ], NULL, BmpColorData, RgbBmp, AlphaBmp, NULL, 0, NULL, 0, NULL, NULL, FALSE, FALSE, -1, FALSE ) ;
	}
	else
	{
		Graphics_Image_SetGraphBaseInfo( GrHandle[ 0 ], NULL, NULL, NULL, NULL, NULL, 0, NULL, 0, RgbBaseImage, AlphaBaseImage, FALSE, FALSE, -1, FALSE ) ;
	}
	for( i = 1 ; i < AllNum ; i ++ )
	{
		Graphics_Image_SetGraphBaseInfo( GrHandle[ i ], NULL, NULL, NULL, NULL, NULL, 0, NULL, 0, NULL, NULL, FALSE, FALSE, GrHandle[ 0 ], FALSE ) ;
	}

	// �I��
	return Result ;
}

// �����摜�ւ� �a�l�o �� GraphImage �̓]��
extern	int NS_BltBmpOrGraphImageToDivGraphF(
	const COLORDATA	*BmpColorData,
	      HBITMAP	RgbBmp,
	      HBITMAP	AlphaBmp,
	      int		BmpFlag,
	const BASEIMAGE	*RgbBaseImage,
	const BASEIMAGE	*AlphaBaseImage,
	      int		AllNum,
	      int		XNum,
	      int		YNum,
	      float		Width,
	      float		Height,
	const int		*GrHandle,
	      int		ReverseFlag
)
{
	int Result ;
	int i ;
#ifdef __WINDOWS__
	BITMAP		Bmp ;
	BASEIMAGE	TempBaseRGB ;
	BASEIMAGE	TempBaseAlpha = { 0 } ;
#endif // __WINDOWS__

	if( GSYS.InitializeFlag == FALSE ) return -1 ;

	// Bitmap �̏ꍇ�́ABASEIMAGE �̏����쐬����
	if( BmpFlag )
	{
#ifdef __WINDOWS__
		GetObject( RgbBmp, sizeof( BITMAP ), ( void * )&Bmp ) ;
		TempBaseRGB.GraphData      = Bmp.bmBits ;
		TempBaseRGB.Width          = Bmp.bmWidth ;
		TempBaseRGB.Height         = Bmp.bmHeight ;
		TempBaseRGB.Pitch          = ( Bmp.bmWidthBytes + 3 ) / 4 * 4 ;
		TempBaseRGB.ColorData      = *BmpColorData ;
		TempBaseRGB.MipMapCount    = 0 ;
		TempBaseRGB.GraphDataCount = 0 ;

		RgbBaseImage = &TempBaseRGB ;

		if( AlphaBmp )
		{
			GetObject( AlphaBmp, sizeof( BITMAP ), ( void * )&Bmp ) ;
			TempBaseAlpha.GraphData      = Bmp.bmBits ;
			TempBaseAlpha.Width          = Bmp.bmWidth ;
			TempBaseAlpha.Height         = Bmp.bmHeight ;
			TempBaseAlpha.Pitch          = ( Bmp.bmWidthBytes + 3 ) / 4 * 4 ;
			TempBaseAlpha.ColorData      = *BmpColorData ;
			TempBaseAlpha.MipMapCount    = 0 ;
			TempBaseAlpha.GraphDataCount = 0 ;

			AlphaBaseImage = &TempBaseAlpha ;
		}
		else
		{
			AlphaBaseImage = NULL ;
		}
#else // __WINDOWS__
		return -1 ;
#endif // __WINDOWS__
	}

	// �]������
	Result = Graphics_Image_BltBmpOrGraphImageToDivGraphBase(
		RgbBaseImage,
		AlphaBaseImage,
		AllNum,
		XNum,
		YNum,
		TRUE,
		_FTOL( Width ),  Width,
		_FTOL( Height ), Height,
		GrHandle,
		ReverseFlag,
		GSYS.CreateImage.NotUseTransColor ? FALSE : TRUE,
		FALSE
	) ;

	// �ŏ��̉摜�Ƀt�@�C�������Z�b�g
	if( BmpFlag )
	{
		Graphics_Image_SetGraphBaseInfo( GrHandle[ 0 ], NULL, BmpColorData, RgbBmp, AlphaBmp, NULL, 0, NULL, 0, NULL, NULL, FALSE, FALSE, -1, FALSE ) ;
	}
	else
	{
		Graphics_Image_SetGraphBaseInfo( GrHandle[ 0 ], NULL, NULL, NULL, NULL, NULL, 0, NULL, 0, RgbBaseImage, AlphaBaseImage, FALSE, FALSE, -1, FALSE ) ;
	}
	for( i = 1 ; i < AllNum ; i ++ )
	{
		Graphics_Image_SetGraphBaseInfo( GrHandle[ i ], NULL, NULL, NULL, NULL, NULL, 0, NULL, 0, NULL, NULL, FALSE, FALSE, GrHandle[ 0 ], FALSE ) ;
	}

	// �I��
	return Result ;
}



















// �摜����O���t�B�b�N�n���h�����쐬����֐�

// �摜��ǂ݂���
extern int NS_LoadBmpToGraph( const TCHAR *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode )
{
#ifdef UNICODE
	return LoadBmpToGraph_WCHAR_T(
		FileName, TextureFlag, ReverseFlag, SurfaceMode
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = LoadBmpToGraph_WCHAR_T(
		UseFileNameBuffer, TextureFlag, ReverseFlag, SurfaceMode
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �摜��ǂ݂���
extern int LoadBmpToGraph_WCHAR_T( const wchar_t *FileName, int TextureFlag, int ReverseFlag, int SurfaceMode )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_LoadBmpToGraph_UseGParam( &GParam, FALSE, -1, FileName, TextureFlag, ReverseFlag, SurfaceMode, GetASyncLoadFlag() ) ;
}

// �a�l�o�t�@�C���̃������ւ̓ǂ݂��݁i�t���O��)
extern int NS_LoadGraph( const TCHAR *GraphName, int NotUse3DFlag )
{
	return NS_LoadBmpToGraph( GraphName, !NotUse3DFlag, FALSE ) ;
}

// �a�l�o�t�@�C���̃������ւ̓ǂ݂��݁i�t���O��)
extern int LoadGraph_WCHAR_T( const wchar_t *GraphName, int NotUse3DFlag )
{
	return LoadBmpToGraph_WCHAR_T( GraphName, !NotUse3DFlag, FALSE ) ;
}

// �a�l�o�t�@�C���̃������ւ̔��]�ǂ݂��݁i�t���O��)
extern int NS_LoadReverseGraph( const TCHAR *GraphName, int NotUse3DFlag )
{
	return NS_LoadBmpToGraph( GraphName, !NotUse3DFlag, TRUE ) ;
}

// �a�l�o�t�@�C���̃������ւ̔��]�ǂ݂��݁i�t���O��)
extern int LoadReverseGraph_WCHAR_T( const wchar_t *GraphName, int NotUse3DFlag )
{
	return LoadBmpToGraph_WCHAR_T( GraphName, !NotUse3DFlag, TRUE ) ;
}

// �a�l�o�̕����ǂ݂��݁i�t���O���j
extern int NS_LoadDivGraph( const TCHAR *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf, int NotUse3DFlag )
{
	return NS_LoadDivBmpToGraph( FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf, !NotUse3DFlag, FALSE ) ;
}

// �a�l�o�̕����ǂ݂��݁i�t���O���j
extern int NS_LoadDivGraphF( const TCHAR *FileName, int AllNum, int XNum, int YNum, float XSize, float YSize, int *HandleBuf, int NotUse3DFlag )
{
	return NS_LoadDivBmpToGraphF( FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf, !NotUse3DFlag, FALSE ) ;
}

// �a�l�o�̕����ǂ݂���
extern int NS_LoadDivBmpToGraph( const TCHAR *FileName, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, int TextureFlag, int ReverseFlag )
{
#ifdef UNICODE
	return LoadDivBmpToGraph_WCHAR_T(
		FileName, AllNum, XNum, YNum, FALSE, SizeX, ( float )SizeX, SizeY, ( float )SizeY, HandleBuf, TextureFlag, ReverseFlag
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = LoadDivBmpToGraph_WCHAR_T(
		UseFileNameBuffer, AllNum, XNum, YNum, FALSE, SizeX, ( float )SizeX, SizeY, ( float )SizeY, HandleBuf, TextureFlag, ReverseFlag
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �a�l�o�̕����ǂ݂���
extern int NS_LoadDivBmpToGraphF( const TCHAR *FileName, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleBuf, int TextureFlag, int ReverseFlag )
{
#ifdef UNICODE
	return LoadDivBmpToGraph_WCHAR_T(
		FileName, AllNum, XNum, YNum, TRUE, _FTOL( SizeX ), SizeX, _FTOL( SizeY ), SizeY, HandleBuf, TextureFlag, ReverseFlag
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = LoadDivBmpToGraph_WCHAR_T(
		UseFileNameBuffer, AllNum, XNum, YNum, TRUE, _FTOL( SizeX ), SizeX, _FTOL( SizeY ), SizeY, HandleBuf, TextureFlag, ReverseFlag
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �a�l�o�̕����ǂ݂���
extern int LoadDivBmpToGraph_WCHAR_T( const wchar_t *FileName, int AllNum, int XNum, int YNum, int IsFloat, int SizeXI, float SizeXF, int SizeYI, float SizeYF, int *HandleBuf, int TextureFlag, int ReverseFlag )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_LoadDivBmpToGraph_UseGParam( &GParam, FALSE, FileName, AllNum, XNum, YNum, IsFloat, SizeXI, SizeXF, SizeYI, SizeYF, HandleBuf, TextureFlag, ReverseFlag, GetASyncLoadFlag(), FALSE ) ;
}

// �a�l�o�̔��]�����ǂ݂��݁i�t���O���j
extern int NS_LoadReverseDivGraph( const TCHAR *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf, int NotUse3DFlag )
{
	return NS_LoadDivBmpToGraph( FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf, !NotUse3DFlag, TRUE ) ;
}

// �a�l�o�̔��]�����ǂ݂��݁i�t���O���j
extern int NS_LoadReverseDivGraphF( const TCHAR *FileName, int AllNum, int XNum, int YNum, float XSize, float YSize, int *HandleBuf, int NotUse3DFlag )
{
	return NS_LoadDivBmpToGraphF( FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf, !NotUse3DFlag, TRUE ) ;
}

// �摜�t�@�C������u�����h�p�摜��ǂݍ���
extern int NS_LoadBlendGraph( const TCHAR *FileName )
{
#ifdef UNICODE
	return LoadBlendGraph_WCHAR_T(
		FileName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = LoadBlendGraph_WCHAR_T(
		UseFileNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �摜�t�@�C������u�����h�p�摜��ǂݍ���
extern int LoadBlendGraph_WCHAR_T( const wchar_t *FileName )
{
	int Result, UseBlendGraphFlag ;

	UseBlendGraphFlag = NS_GetUseBlendGraphCreateFlag() ;
	NS_SetUseBlendGraphCreateFlag( TRUE ) ;
	
	Result = LoadGraph_WCHAR_T( FileName ) ;
	
	NS_SetUseBlendGraphCreateFlag( UseBlendGraphFlag ) ;
	
	return Result ;
}

#ifdef __WINDOWS__

// ���\�[�X����a�l�o�t�@�C����ǂݍ���
extern int NS_LoadGraphToResource( int ResourceID )
{
	BITMAPINFO *BmpInfo ;
	void *GraphData ;
	int NewHandle ;

	// ���\�[�X����摜�n���h���쐬�ɕK�v�ȃf�[�^���擾����
	if( GetBmpImageToResource( ResourceID, &BmpInfo, &GraphData ) == -1 )
		return -1 ;

	// �O���t�B�b�N�̍쐬
	NewHandle = NS_CreateGraphFromBmp( BmpInfo, GraphData ) ;

	// �������̉��
	DXFREE( BmpInfo ) ;
	DXFREE( GraphData ) ;

	// �n���h����Ԃ�
	return NewHandle ;
}

// ���\�[�X����摜�f�[�^�𕪊��ǂݍ��݂���
extern int NS_LoadDivGraphToResource( int ResourceID, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf )
{
	BITMAPINFO *BmpInfo ;
	void *GraphData ;

	// ���\�[�X����摜�n���h���쐬�ɕK�v�ȃf�[�^���擾����
	if( GetBmpImageToResource( ResourceID, &BmpInfo, &GraphData ) == -1 )
		return -1 ;

	// �O���t�B�b�N�̍쐬
	NS_CreateDivGraphFromBmp( BmpInfo, GraphData, AllNum, XNum, YNum, XSize, YSize, HandleBuf ) ;

	// �������̉��
	DXFREE( BmpInfo ) ;
	DXFREE( GraphData ) ;

	// ����
	return 0 ;
}

// ���\�[�X����摜�f�[�^�𕪊��ǂݍ��݂���
extern int NS_LoadDivGraphFToResource( int ResourceID, int AllNum, int XNum, int YNum, float XSize, float YSize, int *HandleBuf )
{
	BITMAPINFO *BmpInfo ;
	void *GraphData ;

	// ���\�[�X����摜�n���h���쐬�ɕK�v�ȃf�[�^���擾����
	if( GetBmpImageToResource( ResourceID, &BmpInfo, &GraphData ) == -1 )
		return -1 ;

	// �O���t�B�b�N�̍쐬
	NS_CreateDivGraphFFromBmp( BmpInfo, GraphData, AllNum, XNum, YNum, XSize, YSize, HandleBuf ) ;

	// �������̉��
	DXFREE( BmpInfo ) ;
	DXFREE( GraphData ) ;

	// ����
	return 0 ;
}

// ���\�[�X����O���t�B�b�N�f�[�^��ǂݍ���
extern int NS_LoadGraphToResource( const TCHAR *ResourceName, const TCHAR *ResourceType )
{
#ifdef UNICODE
	return LoadGraphToResource_WCHAR_T(
		ResourceName, ResourceType
	) ;
#else
	int Result = -1 ;
	int IntResourceName ;
	int IntResourceType ;
	TCHAR_TO_WCHAR_T_STRING_BEGIN( ResourceName )
	TCHAR_TO_WCHAR_T_STRING_BEGIN( ResourceType )

	IntResourceName = ( ( ( DWORD_PTR )ResourceName ) >> 16 ) == 0 ? TRUE : FALSE ;
	IntResourceType = ( ( ( DWORD_PTR )ResourceType ) >> 16 ) == 0 ? TRUE : FALSE ;

	if( IntResourceName )
	{
		UseResourceNameBuffer = ( wchar_t * )ResourceName ;
	}
	else
	{
		TCHAR_TO_WCHAR_T_STRING_SETUP( ResourceName, goto ERR )
	}

	if( IntResourceType )
	{
		UseResourceTypeBuffer = ( wchar_t * )ResourceType ;
	}
	else
	{
		TCHAR_TO_WCHAR_T_STRING_SETUP( ResourceType, goto ERR )
	}

	Result = LoadGraphToResource_WCHAR_T(
		UseResourceNameBuffer, UseResourceTypeBuffer
	) ;

ERR :

	if( IntResourceName == FALSE )
	{
		TCHAR_TO_WCHAR_T_STRING_END( ResourceName )
	}

	if( IntResourceType == FALSE )
	{
		TCHAR_TO_WCHAR_T_STRING_END( ResourceType )
	}

	return Result ;
#endif
}

// ���\�[�X����O���t�B�b�N�f�[�^��ǂݍ���
extern int LoadGraphToResource_WCHAR_T( const wchar_t *ResourceName, const wchar_t *ResourceType )
{
	void *Image ;
	int ImageSize ;
	int Result ;

	// ���\�[�X�̏����擾
	if( GetResourceInfo_WCHAR_T( ResourceName, ResourceType, &Image, &ImageSize ) < 0 )
	{
		return -1 ;
	}

	// �n���h���̍쐬
	Result = NS_CreateGraphFromMem( Image, ImageSize ) ;

	return Result ;
}

// ���\�[�X����O���t�B�b�N�f�[�^�𕪊��ǂݍ��݂���
extern int NS_LoadDivGraphToResource( const TCHAR *ResourceName, const TCHAR *ResourceType, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf )
{
#ifdef UNICODE
	return LoadDivGraphToResource_WCHAR_T(
		ResourceName, ResourceType, AllNum, XNum, YNum, XSize, YSize, HandleBuf
	) ;
#else
	int Result = -1 ;

	int IntResourceName ;
	int IntResourceType ;
	TCHAR_TO_WCHAR_T_STRING_BEGIN( ResourceName )
	TCHAR_TO_WCHAR_T_STRING_BEGIN( ResourceType )

	IntResourceName = ( ( ( DWORD_PTR )ResourceName ) >> 16 ) == 0 ? TRUE : FALSE ;
	IntResourceType = ( ( ( DWORD_PTR )ResourceType ) >> 16 ) == 0 ? TRUE : FALSE ;

	if( IntResourceName )
	{
		UseResourceNameBuffer = ( wchar_t * )ResourceName ;
	}
	else
	{
		TCHAR_TO_WCHAR_T_STRING_SETUP( ResourceName, goto ERR )
	}

	if( IntResourceType )
	{
		UseResourceTypeBuffer = ( wchar_t * )ResourceType ;
	}
	else
	{
		TCHAR_TO_WCHAR_T_STRING_SETUP( ResourceType, goto ERR )
	}

	Result = LoadDivGraphToResource_WCHAR_T(
		UseResourceNameBuffer, UseResourceTypeBuffer, AllNum, XNum, YNum, XSize, YSize, HandleBuf
	) ;

ERR :

	if( IntResourceName == FALSE )
	{
		TCHAR_TO_WCHAR_T_STRING_END( ResourceName )
	}

	if( IntResourceType == FALSE )
	{
		TCHAR_TO_WCHAR_T_STRING_END( ResourceType )
	}

	return Result ;
#endif
}

// ���\�[�X����O���t�B�b�N�f�[�^�𕪊��ǂݍ��݂���
extern int NS_LoadDivGraphFToResource( const TCHAR *ResourceName, const TCHAR *ResourceType, int AllNum, int XNum, int YNum, float XSize, float YSize, int *HandleBuf )
{
#ifdef UNICODE
	return LoadDivGraphFToResource_WCHAR_T(
		ResourceName, ResourceType, AllNum, XNum, YNum, XSize, YSize, HandleBuf
	) ;
#else
	int Result = -1 ;

	int IntResourceName ;
	int IntResourceType ;
	TCHAR_TO_WCHAR_T_STRING_BEGIN( ResourceName )
	TCHAR_TO_WCHAR_T_STRING_BEGIN( ResourceType )

	IntResourceName = ( ( ( DWORD_PTR )ResourceName ) >> 16 ) == 0 ? TRUE : FALSE ;
	IntResourceType = ( ( ( DWORD_PTR )ResourceType ) >> 16 ) == 0 ? TRUE : FALSE ;

	if( IntResourceName )
	{
		UseResourceNameBuffer = ( wchar_t * )ResourceName ;
	}
	else
	{
		TCHAR_TO_WCHAR_T_STRING_SETUP( ResourceName, goto ERR )
	}

	if( IntResourceType )
	{
		UseResourceTypeBuffer = ( wchar_t * )ResourceType ;
	}
	else
	{
		TCHAR_TO_WCHAR_T_STRING_SETUP( ResourceType, goto ERR )
	}

	Result = LoadDivGraphFToResource_WCHAR_T(
		UseResourceNameBuffer, UseResourceTypeBuffer, AllNum, XNum, YNum, XSize, YSize, HandleBuf
	) ;

ERR :

	if( IntResourceName == FALSE )
	{
		TCHAR_TO_WCHAR_T_STRING_END( ResourceName )
	}

	if( IntResourceType == FALSE )
	{
		TCHAR_TO_WCHAR_T_STRING_END( ResourceType )
	}

	return Result ;
#endif
}

// ���\�[�X����O���t�B�b�N�f�[�^�𕪊��ǂݍ��݂���
extern int LoadDivGraphToResource_WCHAR_T( const wchar_t *ResourceName, const wchar_t *ResourceType, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf )
{
	void *Image ;
	int ImageSize ;
	int Result ;

	// ���\�[�X�̏����擾
	if( GetResourceInfo_WCHAR_T( ResourceName, ResourceType, &Image, &ImageSize ) < 0 ) return -1 ;

	// �n���h���̍쐬
	Result = NS_CreateDivGraphFromMem( Image, ImageSize, AllNum, XNum, YNum, XSize, YSize, HandleBuf ) ;

	return Result ;
}

// ���\�[�X����O���t�B�b�N�f�[�^�𕪊��ǂݍ��݂���
extern int LoadDivGraphFToResource_WCHAR_T( const wchar_t *ResourceName, const wchar_t *ResourceType, int AllNum, int XNum, int YNum, float XSize, float YSize, int *HandleBuf )
{
	void *Image ;
	int ImageSize ;
	int Result ;

	// ���\�[�X�̏����擾
	if( GetResourceInfo_WCHAR_T( ResourceName, ResourceType, &Image, &ImageSize ) < 0 ) return -1 ;

	// �n���h���̍쐬
	Result = NS_CreateDivGraphFFromMem( Image, ImageSize, AllNum, XNum, YNum, XSize, YSize, HandleBuf ) ;

	return Result ;
}

#endif // __WINDOWS__

// ��������̃O���t�B�b�N�C���[�W����O���t�B�b�N�n���h�����쐬����
extern int NS_CreateGraphFromMem( const void *MemImage, int MemImageSize, const void *AlphaImage, int AlphaImageSize, int TextureFlag, int ReverseFlag )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateGraphFromMem_UseGParam( &GParam, FALSE, -1, MemImage, MemImageSize, AlphaImage, AlphaImageSize, TextureFlag, ReverseFlag, GetASyncLoadFlag() ) ;
}

// ��������̃O���t�B�b�N�C���[�W��������̃O���t�B�b�N�n���h���Ƀf�[�^��]������
extern int NS_ReCreateGraphFromMem( const void *MemImage, int MemImageSize, int GrHandle, const void *AlphaImage, int AlphaImageSize, int TextureFlag, int ReverseFlag )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateGraphFromMem_UseGParam( &GParam, TRUE, GrHandle, MemImage, MemImageSize, AlphaImage, AlphaImageSize, TextureFlag, ReverseFlag, GetASyncLoadFlag() ) ;
}

// ��������̃O���t�B�b�N�C���[�W���番���O���t�B�b�N�n���h�����쐬����
extern int NS_CreateDivGraphFromMem( const void *MemImage, int MemImageSize, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf,int TextureFlag, int ReverseFlag, const void *AlphaImage, int AlphaImageSize )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateDivGraphFromMem_UseGParam( &GParam, FALSE, MemImage, MemImageSize, AllNum, XNum, YNum, FALSE, SizeX, ( float )SizeX, SizeY, ( float )SizeY, HandleBuf, TextureFlag, ReverseFlag, AlphaImage, AlphaImageSize, GetASyncLoadFlag(), FALSE ) ;
}

// ��������̃O���t�B�b�N�C���[�W���番���O���t�B�b�N�n���h�����쐬����
extern int NS_CreateDivGraphFFromMem( const void *MemImage, int MemImageSize, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleBuf,int TextureFlag, int ReverseFlag, const void *AlphaImage, int AlphaImageSize )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateDivGraphFromMem_UseGParam( &GParam, FALSE, MemImage, MemImageSize, AllNum, XNum, YNum, TRUE, _FTOL( SizeX ), SizeX, _FTOL( SizeY ), SizeY, HandleBuf, TextureFlag, ReverseFlag, AlphaImage, AlphaImageSize, GetASyncLoadFlag(), FALSE ) ;
}

// ��������̃O���t�B�b�N�C���[�W��������̕����O���t�B�b�N�n���h���Ƀf�[�^��]������
extern int NS_ReCreateDivGraphFromMem( const void *MemImage, int MemImageSize, int AllNum, int XNum, int YNum, int SizeX, int SizeY, const int *HandleBuf,int TextureFlag, int ReverseFlag, const void *AlphaImage, int AlphaImageSize )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateDivGraphFromMem_UseGParam( &GParam, TRUE, MemImage, MemImageSize, AllNum, XNum, YNum, FALSE, SizeX, ( float )SizeX, SizeY, ( float )SizeY, ( int * )HandleBuf, TextureFlag, ReverseFlag, AlphaImage, AlphaImageSize, GetASyncLoadFlag(), FALSE ) ;
}

// ��������̃O���t�B�b�N�C���[�W��������̕����O���t�B�b�N�n���h���Ƀf�[�^��]������
extern int NS_ReCreateDivGraphFFromMem( const void *MemImage, int MemImageSize, int AllNum, int XNum, int YNum, float SizeX, float SizeY, const int *HandleBuf,int TextureFlag, int ReverseFlag, const void *AlphaImage, int AlphaImageSize )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateDivGraphFromMem_UseGParam( &GParam, TRUE, MemImage, MemImageSize, AllNum, XNum, YNum, TRUE, _FTOL( SizeX ), SizeX, _FTOL( SizeY ), SizeY, ( int * )HandleBuf, TextureFlag, ReverseFlag, AlphaImage, AlphaImageSize, GetASyncLoadFlag(), FALSE ) ;
}

// �r�b�g�}�b�v�f�[�^����O���t�B�b�N�n���h�����쐬����
extern int NS_CreateGraphFromBmp( const BITMAPINFO *BmpInfo, const void *GraphData, const BITMAPINFO *AlphaInfo, const void *AlphaData, int TextureFlag, int ReverseFlag )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateGraphFromBmp_UseGParam( &GParam, FALSE, -1, BmpInfo, GraphData, AlphaInfo, AlphaData, TextureFlag, ReverseFlag, GetASyncLoadFlag() ) ;
}

// �r�b�g�}�b�v�f�[�^��������̃O���t�B�b�N�n���h���Ƀf�[�^��]������
extern int NS_ReCreateGraphFromBmp( const BITMAPINFO *BmpInfo, const void *GraphData, int GrHandle, const BITMAPINFO *AlphaInfo, const void *AlphaData, int TextureFlag, int ReverseFlag )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateGraphFromBmp_UseGParam( &GParam, TRUE, GrHandle, BmpInfo, GraphData, AlphaInfo, AlphaData, TextureFlag, ReverseFlag, GetASyncLoadFlag() ) ;
}

// �r�b�g�}�b�v�f�[�^���番���O���t�B�b�N�n���h�����쐬����
extern int NS_CreateDivGraphFromBmp( const BITMAPINFO *BmpInfo, const void *GraphData, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, int TextureFlag, int ReverseFlag, const BITMAPINFO *AlphaInfo, const void *AlphaData )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateDivGraphFromBmp_UseGParam( &GParam, FALSE, BmpInfo, GraphData, AllNum, XNum, YNum, FALSE, SizeX, ( float )SizeX, SizeY, ( float )SizeY, HandleBuf, TextureFlag, ReverseFlag, AlphaInfo, AlphaData, GetASyncLoadFlag() ) ;
}

// �r�b�g�}�b�v�f�[�^���番���O���t�B�b�N�n���h�����쐬����
extern int NS_CreateDivGraphFFromBmp( const BITMAPINFO *BmpInfo, const void *GraphData, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleBuf, int TextureFlag, int ReverseFlag, const BITMAPINFO *AlphaInfo, const void *AlphaData )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateDivGraphFromBmp_UseGParam( &GParam, FALSE, BmpInfo, GraphData, AllNum, XNum, YNum, TRUE, _FTOL( SizeX ), SizeX, _FTOL( SizeY ), SizeY, HandleBuf, TextureFlag, ReverseFlag, AlphaInfo, AlphaData, GetASyncLoadFlag() ) ;
}

// �r�b�g�}�b�v�f�[�^��������̕����O���t�B�b�N�n���h���Ƀf�[�^��]������
extern int NS_ReCreateDivGraphFromBmp( const BITMAPINFO *BmpInfo, const void *GraphData, int AllNum, int XNum, int YNum, int SizeX, int SizeY, const int *HandleBuf,int TextureFlag, int ReverseFlag, const BITMAPINFO *AlphaInfo, const void *AlphaData )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateDivGraphFromBmp_UseGParam( &GParam, TRUE, BmpInfo, GraphData, AllNum, XNum, YNum, FALSE, SizeX, ( float )SizeX, SizeY, ( float )SizeY, ( int * )HandleBuf, TextureFlag, ReverseFlag, AlphaInfo, AlphaData, GetASyncLoadFlag() ) ;
}

// �r�b�g�}�b�v�f�[�^��������̕����O���t�B�b�N�n���h���Ƀf�[�^��]������
extern int NS_ReCreateDivGraphFFromBmp( const BITMAPINFO *BmpInfo, const void *GraphData, int AllNum, int XNum, int YNum, float SizeX, float SizeY, const int *HandleBuf,int TextureFlag, int ReverseFlag, const BITMAPINFO *AlphaInfo, const void *AlphaData )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateDivGraphFromBmp_UseGParam( &GParam, TRUE, BmpInfo, GraphData, AllNum, XNum, YNum, TRUE, _FTOL( SizeX ), SizeX, _FTOL( SizeY ), SizeY, ( int * )HandleBuf, TextureFlag, ReverseFlag, AlphaInfo, AlphaData, GetASyncLoadFlag() ) ;
}

// GraphImage �f�[�^����T�C�Y������o���A����ɍ������O���t�B�b�N�n���h�����쐬����
extern int NS_CreateDXGraph( const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, int TextureFlag )
{
	SETUP_GRAPHHANDLE_GPARAM GParam ;
	int NewGraphHandle ;
	int Result ;

	CheckActiveState() ;

	// �n���h���̍쐬
	NewGraphHandle = Graphics_Image_AddHandle( FALSE ) ;
	if( NewGraphHandle == -1 )
	{
		return -1 ;
	}

	Graphics_Image_InitSetupGraphHandleGParam( &GParam ) ;

	Result = Graphics_Image_CreateDXGraph_UseGParam( &GParam, NewGraphHandle, RgbBaseImage, AlphaBaseImage, TextureFlag ) ;
	if( Result < 0 )
	{
		NS_DeleteGraph( NewGraphHandle ) ;
		return -1 ;
	}

	return NewGraphHandle ;
}

// GraphImage�f�[�^����O���t�B�b�N�n���h�����쐬����
extern int NS_CreateGraphFromGraphImage( const BASEIMAGE *BaseImage, int TextureFlag, int ReverseFlag )
{
	return NS_CreateGraphFromGraphImage( BaseImage, NULL, TextureFlag, ReverseFlag ) ;
}

// GraphImage�f�[�^����O���t�B�b�N�n���h�����쐬����
extern int NS_CreateGraphFromGraphImage( const BASEIMAGE *BaseImage, const BASEIMAGE *AlphaBaseImage, int TextureFlag, int ReverseFlag )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, FALSE, -1, BaseImage, AlphaBaseImage, TextureFlag, ReverseFlag, GetASyncLoadFlag() ) ; 
}

// GraphImage�f�[�^��������̃O���t�B�b�N�n���h���Ƀf�[�^��]������
extern int NS_ReCreateGraphFromGraphImage( const BASEIMAGE *BaseImage, int GrHandle, int TextureFlag, int ReverseFlag )
{
	return NS_ReCreateGraphFromGraphImage( BaseImage, NULL, GrHandle, TextureFlag, ReverseFlag ) ;
}

// GraphImage�f�[�^��������̃O���t�B�b�N�n���h���Ƀf�[�^��]������
extern int NS_ReCreateGraphFromGraphImage( const BASEIMAGE *BaseImage, const BASEIMAGE *AlphaBaseImage, int GrHandle, int TextureFlag, int ReverseFlag )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, TRUE, GrHandle, BaseImage, AlphaBaseImage, TextureFlag, ReverseFlag, GetASyncLoadFlag() ) ;
}

// GraphImage�f�[�^���番���O���t�B�b�N�n���h�����쐬����
extern int NS_CreateDivGraphFromGraphImage( BASEIMAGE *BaseImage, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf,int TextureFlag, int ReverseFlag )
{
	return NS_CreateDivGraphFromGraphImage( BaseImage, NULL, AllNum, XNum, YNum, SizeX, SizeY, HandleBuf, TextureFlag, ReverseFlag ) ;
}

// GraphImage�f�[�^���番���O���t�B�b�N�n���h�����쐬����
extern int NS_CreateDivGraphFFromGraphImage( BASEIMAGE *BaseImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleBuf,int TextureFlag, int ReverseFlag )
{
	return NS_CreateDivGraphFFromGraphImage( BaseImage, NULL, AllNum, XNum, YNum, SizeX, SizeY, HandleBuf, TextureFlag, ReverseFlag ) ;
}

// GraphImage�f�[�^���番���O���t�B�b�N�n���h�����쐬����
extern int NS_CreateDivGraphFromGraphImage( BASEIMAGE *BaseImage, const BASEIMAGE *AlphaBaseImage, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf,int TextureFlag, int ReverseFlag )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateDivGraphFromGraphImage_UseGParam( &GParam, FALSE, BaseImage, AlphaBaseImage, AllNum, XNum, YNum, FALSE, SizeX, ( float )SizeX, SizeY, ( float )SizeY, HandleBuf, TextureFlag, ReverseFlag, GetASyncLoadFlag() ) ;
}

// GraphImage�f�[�^���番���O���t�B�b�N�n���h�����쐬����
extern int NS_CreateDivGraphFFromGraphImage( BASEIMAGE *BaseImage, const BASEIMAGE *AlphaBaseImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleBuf,int TextureFlag, int ReverseFlag )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateDivGraphFromGraphImage_UseGParam( &GParam, FALSE, BaseImage, AlphaBaseImage, AllNum, XNum, YNum, TRUE, _FTOL( SizeX ), SizeX, _FTOL( SizeY ), SizeY, HandleBuf, TextureFlag, ReverseFlag, GetASyncLoadFlag() ) ;
}

// GraphImage�f�[�^��������̕����O���t�B�b�N�n���h���Ƀf�[�^��]������
extern int NS_ReCreateDivGraphFromGraphImage( BASEIMAGE *BaseImage, int AllNum, int XNum, int YNum, int SizeX, int SizeY, const int *HandleBuf,int TextureFlag, int ReverseFlag )
{
	return NS_ReCreateDivGraphFromGraphImage( BaseImage, NULL, AllNum, XNum, YNum, SizeX, SizeY, HandleBuf, TextureFlag, ReverseFlag ) ;
}

// GraphImage�f�[�^��������̕����O���t�B�b�N�n���h���Ƀf�[�^��]������
extern int NS_ReCreateDivGraphFFromGraphImage( BASEIMAGE *BaseImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, const int *HandleBuf,int TextureFlag, int ReverseFlag )
{
	return NS_ReCreateDivGraphFFromGraphImage( BaseImage, NULL, AllNum, XNum, YNum, SizeX, SizeY, HandleBuf, TextureFlag, ReverseFlag ) ;
}

// GraphImage�f�[�^��������̕����O���t�B�b�N�n���h���Ƀf�[�^��]������
extern int NS_ReCreateDivGraphFromGraphImage( BASEIMAGE *BaseImage, const BASEIMAGE *AlphaBaseImage, int AllNum, int XNum, int YNum, int SizeX, int SizeY, const int *HandleBuf,int TextureFlag, int ReverseFlag )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateDivGraphFromGraphImage_UseGParam( &GParam, TRUE, BaseImage, AlphaBaseImage, AllNum, XNum, YNum, FALSE, SizeX, ( float )SizeX, SizeY, ( float )SizeY, ( int * )HandleBuf, TextureFlag, ReverseFlag, GetASyncLoadFlag() ) ;
}

// GraphImage�f�[�^��������̕����O���t�B�b�N�n���h���Ƀf�[�^��]������
extern int NS_ReCreateDivGraphFFromGraphImage( BASEIMAGE *BaseImage, const BASEIMAGE *AlphaBaseImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, const int *HandleBuf,int TextureFlag, int ReverseFlag )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_CreateDivGraphFromGraphImage_UseGParam( &GParam, TRUE, BaseImage, AlphaBaseImage, AllNum, XNum, YNum, TRUE, _FTOL( SizeX ), SizeX, _FTOL( SizeY ), SizeY, ( int * )HandleBuf, TextureFlag, ReverseFlag, GetASyncLoadFlag() ) ;
}

// ��������̃O���t�B�b�N�f�[�^����O���t�B�b�N�n���h�����쐬����
extern int NS_CreateGraph( int Width, int Height, int Pitch, const void *GraphData, const void *AlphaData, int GrHandle )
{
	BITMAPINFO	BmpInfo ;
	BYTE *DData, *AData, *SData, *DestData ;
	int i ;
	int DPitch, SPitch ;
	int NewGraph ;

	// �s�b�`�쐬
	{
		int Byte ; 
		
		Byte = Width * 3 ;

		SPitch = ( Pitch == 0 ) ? Width * 3 : Pitch ;
		DPitch = ( ( Byte + 3 ) / 4 ) * 4 ;
	}

	// �����s�b�`�������Ȃ�O���t�B�b�N�f�[�^���R�s�[����K�v�Ȃ�
	AData = NULL ;
	if( SPitch != DPitch )
	{
		// RGB�f�[�^�̍쐬
		{
			// �L���X�g
			SData = ( BYTE * )GraphData ;

			// �O���t�B�b�N�f�[�^�𕡐�
			if( ( DestData = DData = ( BYTE *)DXCALLOC( ( size_t )( Height * DPitch ) ) ) == NULL ) return -1 ;
			for( i = 0 ; i < Height ; i ++, SData += SPitch, DData += DPitch )
				_MEMCPY( DData, SData, ( size_t )SPitch ) ;
			DData = DestData ;
		}

		// �A���t�@�f�[�^�̍쐬
		if( AlphaData != NULL )
		{
			// �L���X�g
			SData = ( BYTE * )AlphaData ;

			// �O���t�B�b�N�f�[�^�𕡐�
			if( ( DestData = AData = ( BYTE *)DXCALLOC( ( size_t )( Height * DPitch ) ) ) == NULL ) return -1 ;
			for( i = 0 ; i < Height ; i ++, SData += SPitch, AData += DPitch )
				_MEMCPY( AData, SData, ( size_t )SPitch ) ;
			AData = DestData ;
		}
	}
	else
	{
		DData = ( BYTE * )GraphData ;
		AData = ( BYTE * )AlphaData ;
	}

	// �V�O���t�B�b�N���쐬
	_MEMSET( &BmpInfo, 0, sizeof( BmpInfo ) ) ;
	BmpInfo.bmiHeader.biSize	= sizeof( BITMAPINFOHEADER ) ;
	BmpInfo.bmiHeader.biWidth	= Width ;
	BmpInfo.bmiHeader.biHeight	= -Height ;
	BmpInfo.bmiHeader.biPlanes	= 1 ;
	BmpInfo.bmiHeader.biBitCount= 24 ;

	if( GrHandle != -1 )
	{
		NS_ReCreateGraphFromBmp( &BmpInfo, DData, GrHandle, AData != NULL ? &BmpInfo : NULL, AData ) ;
		NewGraph = 0 ;
	}
	else
	{
		NewGraph = NS_CreateGraphFromBmp( &BmpInfo, DData, AData != NULL ? &BmpInfo : NULL, AData ) ;
	}

	// �����������
	if( SPitch != DPitch )
	{
		DXFREE( DData ) ;
		DXFREE( AData ) ;
	}

	// �I��
	return NewGraph ;
}

// ��������̃O���t�B�b�N�f�[�^���番���O���t�B�b�N�n���h�����쐬����
extern int NS_CreateDivGraph( int Width, int Height, int Pitch, const void *GraphData, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf, const void *AlphaData )
{
	BITMAPINFO	BmpInfo ;
	BYTE *DData, *AData, *SData, *DestData ;
	int i, Result ;
	int DPitch, SPitch ;

	// �s�b�`�쐬
	{
		int Byte ; 
		
		Byte = Width * 3 ;

		SPitch = ( Pitch == 0 ) ? Width * 3 : Pitch ;
		DPitch = ( ( Byte + 3 ) / 4 ) * 4 ;
	}

	// �����s�b�`�������Ȃ�O���t�B�b�N�f�[�^���R�s�[����K�v�Ȃ�
	AData = NULL ;
	if( SPitch != DPitch )
	{
		// RGB�f�[�^�̍쐬
		{
			// �L���X�g
			SData = ( BYTE * )GraphData ;

			// �O���t�B�b�N�f�[�^�𕡐�
			if( ( DestData = DData = ( BYTE *)DXCALLOC( ( size_t )( Height * DPitch ) ) ) == NULL ) return -1 ;
			for( i = 0 ; i < Height ; i ++, SData += SPitch, DData += DPitch )
				_MEMCPY( DData, SData, ( size_t )SPitch ) ;
			DData = DestData ;
		}

		// �A���t�@�f�[�^�̍쐬
		if( AlphaData != NULL )
		{
			// �L���X�g
			SData = ( BYTE * )AlphaData ;

			// �O���t�B�b�N�f�[�^�𕡐�
			if( ( DestData = AData = ( BYTE *)DXCALLOC( ( size_t )( Height * DPitch ) ) ) == NULL ) return -1 ;
			for( i = 0 ; i < Height ; i ++, SData += SPitch, AData += DPitch )
				_MEMCPY( AData, SData, ( size_t )SPitch ) ;
			AData = DestData ;
		}
	}
	else
	{
		DData = ( BYTE * )GraphData ;
		AData = ( BYTE * )AlphaData ;
	}

	// �V�O���t�B�b�N���쐬
	_MEMSET( &BmpInfo, 0, sizeof( BmpInfo ) ) ;
	BmpInfo.bmiHeader.biSize	= sizeof( BITMAPINFOHEADER ) ;
	BmpInfo.bmiHeader.biWidth	= Width ;
	BmpInfo.bmiHeader.biHeight	= -Height ;
	BmpInfo.bmiHeader.biPlanes	= 1 ;
	BmpInfo.bmiHeader.biBitCount= 24 ;

	Result = NS_CreateDivGraphFromBmp( &BmpInfo, DData, AllNum, XNum, YNum, SizeX, SizeY, HandleBuf, TRUE, FALSE, &BmpInfo, AData ) ;

	// �����������
	if( SPitch != DPitch )
	{
		DXFREE( DData ) ;
		DXFREE( AData ) ;
	}

	// �I��
	return Result ;
}

// ��������̃O���t�B�b�N�f�[�^���番���O���t�B�b�N�n���h�����쐬����
extern int NS_CreateDivGraphF( int Width, int Height, int Pitch, const void *GraphData, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleBuf, const void *AlphaData )
{
	BITMAPINFO	BmpInfo ;
	BYTE *DData, *AData, *SData, *DestData ;
	int i, Result ;
	int DPitch, SPitch ;

	// �s�b�`�쐬
	{
		int Byte ; 
		
		Byte = Width * 3 ;

		SPitch = ( Pitch == 0 ) ? Width * 3 : Pitch ;
		DPitch = ( ( Byte + 3 ) / 4 ) * 4 ;
	}

	// �����s�b�`�������Ȃ�O���t�B�b�N�f�[�^���R�s�[����K�v�Ȃ�
	AData = NULL ;
	if( SPitch != DPitch )
	{
		// RGB�f�[�^�̍쐬
		{
			// �L���X�g
			SData = ( BYTE * )GraphData ;

			// �O���t�B�b�N�f�[�^�𕡐�
			if( ( DestData = DData = ( BYTE *)DXCALLOC( ( size_t )( Height * DPitch ) ) ) == NULL ) return -1 ;
			for( i = 0 ; i < Height ; i ++, SData += SPitch, DData += DPitch )
				_MEMCPY( DData, SData, ( size_t )SPitch ) ;
			DData = DestData ;
		}

		// �A���t�@�f�[�^�̍쐬
		if( AlphaData != NULL )
		{
			// �L���X�g
			SData = ( BYTE * )AlphaData ;

			// �O���t�B�b�N�f�[�^�𕡐�
			if( ( DestData = AData = ( BYTE *)DXCALLOC( ( size_t )( Height * DPitch ) ) ) == NULL ) return -1 ;
			for( i = 0 ; i < Height ; i ++, SData += SPitch, AData += DPitch )
				_MEMCPY( AData, SData, ( size_t )SPitch ) ;
			AData = DestData ;
		}
	}
	else
	{
		DData = ( BYTE * )GraphData ;
		AData = ( BYTE * )AlphaData ;
	}

	// �V�O���t�B�b�N���쐬
	_MEMSET( &BmpInfo, 0, sizeof( BmpInfo ) ) ;
	BmpInfo.bmiHeader.biSize	= sizeof( BITMAPINFOHEADER ) ;
	BmpInfo.bmiHeader.biWidth	= Width ;
	BmpInfo.bmiHeader.biHeight	= -Height ;
	BmpInfo.bmiHeader.biPlanes	= 1 ;
	BmpInfo.bmiHeader.biBitCount= 24 ;

	Result = NS_CreateDivGraphFFromBmp( &BmpInfo, DData, AllNum, XNum, YNum, SizeX, SizeY, HandleBuf, TRUE, FALSE, &BmpInfo, AData ) ;

	// �����������
	if( SPitch != DPitch )
	{
		DXFREE( DData ) ;
		DXFREE( AData ) ;
	}

	// �I��
	return Result ;
}

// ��������̃O���t�B�b�N�f�[�^����O���t�B�b�N�n���h�����č쐬����
extern int NS_ReCreateGraph( int Width, int Height, int Pitch, const void *GraphData, int GrHandle, const void *AlphaData )
{
	return NS_CreateGraph( Width, Height, Pitch, GraphData, AlphaData, GrHandle ) ;
}

#ifndef DX_NON_SOFTIMAGE

// �\�t�g�E�G�A�ň����C���[�W����u�����h�p�摜�O���t�B�b�N�n���h�����쐬����
extern	int		NS_CreateBlendGraphFromSoftImage( int SIHandle )
{
	SOFTIMAGE *SoftImg ;
	LOADGRAPH_GPARAM GParam ;

	// �A�h���X�̎擾
	if( SFTIMGCHK( SIHandle, SoftImg ) )
		return -1 ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;
	GParam.CreateGraphGParam.InitHandleGParam.BlendImageCreateFlag = TRUE ;

	return Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, FALSE, -1, &SoftImg->BaseImage, NULL, TRUE, FALSE, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň����C���[�W����O���t�B�b�N�n���h�����쐬����
extern	int		NS_CreateGraphFromSoftImage( int SIHandle )
{
	SOFTIMAGE *SoftImg ;

	// �A�h���X�̎擾
	if( SFTIMGCHK( SIHandle, SoftImg ) )
		return -1 ;

	return NS_CreateGraphFromBaseImage( &SoftImg->BaseImage ) ;
}

// �\�t�g�E�G�A�ň����C���[�W�̎w��̗̈���g���ăO���t�B�b�N�n���h�����쐬����( -1:�G���[  -1�ȊO:�O���t�B�b�N�n���h�� )
extern	int		NS_CreateGraphFromRectSoftImage( int SIHandle, int x, int y, int SizeX, int SizeY )
{
	SOFTIMAGE *SoftImg ;

	// �A�h���X�̎擾
	if( SFTIMGCHK( SIHandle, SoftImg ) )
		return -1 ;

	return NS_CreateGraphFromRectBaseImage( &SoftImg->BaseImage, x, y, SizeX, SizeY ) ;
}

// �\�t�g�E�G�A�ň����C���[�W��������̃O���t�B�b�N�n���h���ɉ摜�f�[�^��]������
extern	int		NS_ReCreateGraphFromSoftImage( int SIHandle, int GrHandle )
{
	SOFTIMAGE *SoftImg ;

	// �A�h���X�̎擾
	if( SFTIMGCHK( SIHandle, SoftImg ) )
		return -1 ;

	return NS_ReCreateGraphFromBaseImage( &SoftImg->BaseImage, GrHandle ) ;
}

// �\�t�g�E�G�A�ň����C���[�W��������̃O���t�B�b�N�n���h���ɉ摜�f�[�^��]������
extern	int		NS_ReCreateGraphFromRectSoftImage( int SIHandle, int x, int y, int SizeX, int SizeY, int GrHandle )
{
	SOFTIMAGE *SoftImg ;

	// �A�h���X�̎擾
	if( SFTIMGCHK( SIHandle, SoftImg ) )
		return -1 ;

	return NS_ReCreateGraphFromRectBaseImage( &SoftImg->BaseImage, x, y, SizeX, SizeY, GrHandle ) ;
}

// �\�t�g�E�G�A�ň����C���[�W���番���O���t�B�b�N�n���h�����쐬����
extern	int		NS_CreateDivGraphFromSoftImage( int SIHandle, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf )
{
	SOFTIMAGE *SoftImg ;

	// �A�h���X�̎擾
	if( SFTIMGCHK( SIHandle, SoftImg ) )
		return -1 ;

	return NS_CreateDivGraphFromBaseImage( &SoftImg->BaseImage, AllNum, XNum, YNum, SizeX, SizeY, HandleBuf ) ;
}

// �\�t�g�E�G�A�ň����C���[�W���番���O���t�B�b�N�n���h�����쐬����
extern	int		NS_CreateDivGraphFFromSoftImage( int SIHandle, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleBuf )
{
	SOFTIMAGE *SoftImg ;

	// �A�h���X�̎擾
	if( SFTIMGCHK( SIHandle, SoftImg ) )
		return -1 ;

	return NS_CreateDivGraphFFromBaseImage( &SoftImg->BaseImage, AllNum, XNum, YNum, SizeX, SizeY, HandleBuf ) ;
}

#endif // DX_NON_SOFTIMAGE

// ��{�C���[�W�f�[�^����O���t�B�b�N�n���h�����쐬����
extern	int		NS_CreateGraphFromBaseImage( const BASEIMAGE *BaseImage )
{
	return NS_CreateGraphFromGraphImage( BaseImage ) ;
}

// ��{�C���[�W�̎w��̗̈���g���ăO���t�B�b�N�n���h�����쐬����
extern	int		NS_CreateGraphFromRectBaseImage( const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY )
{
	BASEIMAGE DummyImage ;

	// �ʒu��T�C�Y���s����������G���[
	if( SizeX <= 0 || SizeY <= 0 ||
		x < 0 || x + SizeX >  BaseImage->Width ||
		y < 0 || y + SizeY >  BaseImage->Height ) 
		return -1 ;

	// �_�~�[�̊�{�C���[�W���ł����グ�ăO���t�B�b�N�n���h�����쐬����
	DummyImage.ColorData = BaseImage->ColorData ;
	DummyImage.Width = SizeX ;
	DummyImage.Height = SizeY ;
	DummyImage.Pitch = BaseImage->Pitch ;
	DummyImage.MipMapCount = 0 ;
	DummyImage.GraphDataCount = 0 ;
	DummyImage.GraphData = ( BYTE * )BaseImage->GraphData + x * BaseImage->ColorData.PixelByte + y * BaseImage->Pitch ;
	return NS_CreateGraphFromBaseImage( &DummyImage ) ;
}

// ��{�C���[�W�f�[�^��������̃O���t�B�b�N�n���h���ɉ摜�f�[�^��]������
extern	int		NS_ReCreateGraphFromBaseImage( const BASEIMAGE *BaseImage, int GrHandle )
{
	return NS_ReCreateGraphFromGraphImage( BaseImage, GrHandle ) ;
}

// ��{�C���[�W�̎w��̗̈���g���Ċ����̃O���t�B�b�N�n���h���ɉ摜�f�[�^��]������
extern	int		NS_ReCreateGraphFromRectBaseImage( const BASEIMAGE *BaseImage, int x, int y, int SizeX, int SizeY, int GrHandle )
{
	BASEIMAGE DummyImage ;

	// �ʒu��T�C�Y���s����������G���[
	if( SizeX <= 0 || SizeY <= 0 ||
		x < 0 || x + SizeX >  BaseImage->Width ||
		y < 0 || y + SizeY >  BaseImage->Height ) 
		return -1 ;

	// �_�~�[�̊�{�C���[�W���ł����グ�ăO���t�B�b�N�n���h�����쐬����
	DummyImage.ColorData = BaseImage->ColorData ;
	DummyImage.Width = SizeX ;
	DummyImage.Height = SizeY ;
	DummyImage.Pitch = BaseImage->Pitch ;
	DummyImage.MipMapCount = 0 ;
	DummyImage.GraphDataCount = 0 ;
	DummyImage.GraphData = ( BYTE * )BaseImage->GraphData + x * BaseImage->ColorData.PixelByte + y * BaseImage->Pitch ;
	return NS_ReCreateGraphFromGraphImage( &DummyImage, GrHandle ) ;
}

// ��{�C���[�W�f�[�^���番���O���t�B�b�N�n���h�����쐬����
extern int NS_CreateDivGraphFromBaseImage( BASEIMAGE *BaseImage, int AllNum, int XNum, int YNum, int SizeX, int SizeY, int *HandleBuf )
{
	return NS_CreateDivGraphFromGraphImage( BaseImage,  AllNum,  XNum,  YNum,  SizeX,  SizeY, HandleBuf ) ;
}

// ��{�C���[�W�f�[�^���番���O���t�B�b�N�n���h�����쐬����
extern int NS_CreateDivGraphFFromBaseImage( BASEIMAGE *BaseImage, int AllNum, int XNum, int YNum, float SizeX, float SizeY, int *HandleBuf )
{
	return NS_CreateDivGraphFFromGraphImage( BaseImage,  AllNum,  XNum,  YNum,  SizeX,  SizeY, HandleBuf ) ;
}

// �O���t�B�b�N�f�[�^�ւ̃O���t�B�b�N�̓ǂݍ���
extern int NS_ReloadGraph( const TCHAR *FileName, int GrHandle, int ReverseFlag )
{
#ifdef UNICODE
	return ReloadGraph_WCHAR_T(
		FileName, GrHandle, ReverseFlag
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = ReloadGraph_WCHAR_T(
		UseFileNameBuffer, GrHandle, ReverseFlag
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �O���t�B�b�N�f�[�^�ւ̃O���t�B�b�N�̓ǂݍ���
extern int ReloadGraph_WCHAR_T( const wchar_t *FileName, int GrHandle, int ReverseFlag )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_LoadBmpToGraph_UseGParam( &GParam, TRUE, GrHandle, FileName, TRUE, ReverseFlag, DX_MOVIESURFACE_NORMAL, GetASyncLoadFlag() ) ;
}

// �O���t�B�b�N�f�[�^�ւ̃O���t�B�b�N�̕����ǂݍ���
extern int NS_ReloadDivGraph( const TCHAR *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, const int *HandleBuf, int ReverseFlag )
{
#ifdef UNICODE
	return ReloadDivGraph_WCHAR_T(
		FileName, AllNum, XNum, YNum, FALSE, XSize, ( float )XSize, YSize, ( float )YSize, HandleBuf, ReverseFlag
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = ReloadDivGraph_WCHAR_T(
		UseFileNameBuffer, AllNum, XNum, YNum, FALSE, XSize, ( float )XSize, YSize, ( float )YSize, HandleBuf, ReverseFlag
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �O���t�B�b�N�f�[�^�ւ̃O���t�B�b�N�̕����ǂݍ���
extern int NS_ReloadDivGraphF( const TCHAR *FileName, int AllNum, int XNum, int YNum, float XSize, float YSize, const int *HandleBuf, int ReverseFlag )
{
#ifdef UNICODE
	return ReloadDivGraph_WCHAR_T(
		FileName, AllNum, XNum, YNum, TRUE, _FTOL( XSize ), XSize, _FTOL( YSize ), YSize, HandleBuf, ReverseFlag
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = ReloadDivGraph_WCHAR_T(
		UseFileNameBuffer, AllNum, XNum, YNum, TRUE, _FTOL( XSize ), XSize, _FTOL( YSize ), YSize, HandleBuf, ReverseFlag
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �O���t�B�b�N�f�[�^�ւ̃O���t�B�b�N�̕����ǂݍ���
extern int ReloadDivGraph_WCHAR_T( const wchar_t *FileName, int AllNum, int XNum, int YNum, int IsFloat, int XSizeI, float XSizeF, int YSizeI, float YSizeF, const int *HandleBuf, int ReverseFlag )
{
	LOADGRAPH_GPARAM GParam ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	return Graphics_Image_LoadDivBmpToGraph_UseGParam( &GParam, TRUE, FileName, AllNum, XNum, YNum, IsFloat, XSizeI, XSizeF, YSizeI, YSizeF, ( int * )HandleBuf, TRUE, ReverseFlag, GetASyncLoadFlag(), FALSE ) ;
}

// �O���t�B�b�N�f�[�^�ւ̃O���t�B�b�N�̔��]�ǂݍ���
extern int NS_ReloadReverseGraph( const TCHAR *FileName, int GrHandle )
{
	return NS_ReloadGraph( FileName, GrHandle, TRUE ) ;
}

// �O���t�B�b�N�f�[�^�ւ̃O���t�B�b�N�̔��]�ǂݍ���
extern int ReloadReverseGraph_WCHAR_T( const wchar_t *FileName, int GrHandle )
{
	return ReloadGraph_WCHAR_T( FileName, GrHandle, TRUE ) ;
}

// �O���t�B�b�N�f�[�^�ւ̃O���t�B�b�N�̔��]�����ǂݍ���
extern int NS_ReloadReverseDivGraph( const TCHAR *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, const int *HandleBuf )
{
	return NS_ReloadDivGraph( FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf, TRUE ) ;
}

// �O���t�B�b�N�f�[�^�ւ̃O���t�B�b�N�̔��]�����ǂݍ���
extern int NS_ReloadReverseDivGraphF( const TCHAR *FileName, int AllNum, int XNum, int YNum, float XSize, float YSize, const int *HandleBuf )
{
	return NS_ReloadDivGraphF( FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf, TRUE ) ;
}
























// �O���t�B�b�N�n���h���쐬���ݒ�W�֐�

// SetCreateGraphColorBitDepth �̋�����
extern int NS_SetGraphColorBitDepth( int ColorBitDepth )
{
	return NS_SetCreateGraphColorBitDepth( ColorBitDepth ) ;
}

// GetCreateGraphColorBitDepth �̋�����
extern int NS_GetGraphColorBitDepth( void )
{
	return NS_GetCreateGraphColorBitDepth() ;
}

// �쐬����O���t�B�b�N�̐F�[�x��ݒ肷��
extern int NS_SetCreateGraphColorBitDepth( int BitDepth )
{
	// �l��ۑ�����
	GSYS.CreateImage.ColorBitDepth = BitDepth ;
	
	// �I��
	return 0 ;
}

// �쐬����O���t�B�b�N�̐F�[�x���擾����
extern int NS_GetCreateGraphColorBitDepth( void )
{
	return GSYS.CreateImage.ColorBitDepth ;
}

// �쐬����O���t�B�b�N�̂P�`�����l���ӂ�̃r�b�g�[�x��ݒ肷��
extern int NS_SetCreateGraphChannelBitDepth( int BitDepth )
{
	// �l��ۑ�����
	GSYS.CreateImage.ChannelBitDepth = BitDepth ;
	
	// �I��
	return 0 ;
}

// �쐬����O���t�B�b�N�̂P�`�����l���ӂ�̃r�b�g�[�x���擾����
extern int NS_GetCreateGraphChannelBitDepth( void )
{
	return GSYS.CreateImage.ChannelBitDepth ;
}

// �`��\�ȃO���t�B�b�N���쐬���邩�ǂ����̃t���O���Z�b�g����( TRUE:�쐬����  FALSE:�쐬���Ȃ� )
extern int NS_SetDrawValidGraphCreateFlag( int Flag )
{
	// �t���O��ۑ�����
	GSYS.CreateImage.DrawValidFlag = Flag ;
	
	// �I��
	return 0 ;
}

// �`��\�ȃO���t�B�b�N���쐬���邩�ǂ����̃t���O���擾����
extern int NS_GetDrawValidGraphCreateFlag( void )
{
	return GSYS.CreateImage.DrawValidFlag ;
}

// SetDrawValidGraphCreateFlag �̋�����
extern int NS_SetDrawValidFlagOf3DGraph( int Flag )
{
	return NS_SetDrawValidGraphCreateFlag( Flag ) ;
}

// �摜����̐F�𓧉ߐF�ɂ��邩�ǂ����̃t���O���Z�b�g����
extern int NS_SetLeftUpColorIsTransColorFlag( int Flag )
{
	// �t���O���Z�b�g����
	GSYS.CreateImage.LeftUpColorIsTransColorFlag = Flag ;

	// �I��
	return 0 ;
}

// �u�����h�����p�摜���쐬���邩�ǂ����̃t���O���Z�b�g����
extern int NS_SetUseBlendGraphCreateFlag( int Flag )
{
	// �t���O��ۑ�����
	GSYS.CreateImage.BlendImageFlag = Flag ;
	
	// �I��
	return 0 ;
}

// �u�����h�����p�摜���쐬���邩�ǂ����̃t���O���擾����
extern int NS_GetUseBlendGraphCreateFlag( void )
{
	return GSYS.CreateImage.BlendImageFlag ;
}

// �A���t�@�e�X�g���g�p����O���t�B�b�N���쐬���邩�ǂ����̃t���O���Z�b�g����
extern int NS_SetUseAlphaTestGraphCreateFlag( int Flag )
{
	// �t���O���Z�b�g����
	GSYS.CreateImage.AlphaTestFlag = Flag ;
	BASEIM.AlphaTestImageCreateFlag = Flag ;
	
	// �I��
	return 0 ;
}

// �A���t�@�e�X�g���g�p����O���t�B�b�N���쐬���邩�ǂ����̃t���O���擾����
extern int NS_GetUseAlphaTestGraphCreateFlag( void )
{
	return GSYS.CreateImage.AlphaTestFlag ;
}

// SetUseAlphaTestGraphCreateFlag �̋�����
extern int NS_SetUseAlphaTestFlag( int Flag )
{
	return NS_SetUseAlphaTestGraphCreateFlag( Flag ) ;
}

// GetUseAlphaTestGraphCreateFlag �̋�����
extern int NS_GetUseAlphaTestFlag( void )
{
	return NS_GetUseAlphaTestGraphCreateFlag() ;
}

// �L���[�u�}�b�v�e�N�X�`�����쐬���邩�ǂ����̃t���O��ݒ肷��
extern int NS_SetCubeMapTextureCreateFlag( int Flag )
{
	GSYS.CreateImage.CubeMapFlag = Flag == FALSE ? FALSE : TRUE ;

	// ����I��
	return 0 ;
}

// �L���[�u�}�b�v�e�N�X�`�����쐬���邩�ǂ����̃t���O���擾����
extern int NS_GetCubeMapTextureCreateFlag( void )
{
	return 	GSYS.CreateImage.CubeMapFlag ;
}

// SetDrawBlendMode �֐��̑������� DX_BLENDMODE_NOBLEND ���������ۂɁA�f�t�H���g�ł͑������͓����łQ�T�T���w�肵�����ƂɂȂ邪�A���̎����Q�T�T�������Ȃ����ǂ�����ݒ肷��( TRUE:���Ȃ�(�������̒l���g�p�����)   FALSE:����(�������̒l�͖�������� 255 ����Ɏg�p�����)(�f�t�H���g) )���`�����l���t���摜�ɑ΂��ĕ`����s���ꍇ�݈̂Ӗ�������֐�
extern int NS_SetUseNoBlendModeParam( int Flag )
{
	GSYS.DrawSetting.UseNoBlendModeParam = Flag ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �`��\�ȃ��`�����l���t���摜���쐬���邩�ǂ����̃t���O���Z�b�g����,SetDrawValidGraphCreateFlag �֐��ŕ`��\�摜���쐬����悤�ɐݒ肳��Ă��Ȃ��ƌ��ʂ��Ȃ�( TRUE:���`�����l���t�� FALSE:���`�����l���Ȃ� )
extern int NS_SetDrawValidAlphaChannelGraphCreateFlag( int Flag )
{
	// �t���O��ۑ�����
	GSYS.CreateImage.DrawValidAlphaFlag = Flag ;
	
	// �I��
	return 0 ;
}

// �`��\�ȃ��`�����l���t���摜���쐬���邩�ǂ����̃t���O���擾����
extern int NS_GetDrawValidAlphaChannelGraphCreateFlag( void )
{
	// �t���O��Ԃ�
	return GSYS.CreateImage.DrawValidAlphaFlag ;
}

// �`��\�ȕ��������_�^�̉摜���쐬���邩�ǂ����̃t���O���Z�b�g����,SetDrawValidGraphCreateFlag �֐��ŕ`��\�摜���쐬����悤�ɐݒ肳��Ă��Ȃ��ƌ��ʂ��Ȃ��A�f�o�t�� Float�^�̉摜�ɑΉ����Ă��Ȃ��ꍇ�͎��s����( TRUE:Float�^ FALSE:Int�^ )
extern int NS_SetDrawValidFloatTypeGraphCreateFlag( int Flag )
{
	// �t���O��ۑ�����
	GSYS.CreateImage.DrawValidFloatTypeFlag = Flag ;

	// �I��
	return 0 ;
}

// �`��\�ȕ��������_�^�̉摜���쐬���邩�ǂ����̃t���O���擾����
extern int NS_GetDrawValidFloatTypeGraphCreateFlag( void )
{
	// �t���O��Ԃ�
	return GSYS.CreateImage.DrawValidFloatTypeFlag ;
}

// �`��\�摜���쐬����ۂɐ�p�̂y�o�b�t�@���쐬���邩�ǂ�����ݒ肷��( �f�t�H���g�ł� TRUE( �쐬���� ) )
extern int NS_SetDrawValidGraphCreateZBufferFlag( int Flag )
{
	// �t���O��ۑ�����
	GSYS.CreateImage.NotDrawValidCreateZBufferFlag = Flag ? FALSE : TRUE ;

	// �I��
	return 0 ;
}

// �`��\�摜���쐬����ۂɐ�p�̂y�o�b�t�@���쐬���邩�ǂ������擾����
extern int NS_GetDrawValidGraphCreateZBufferFlag( void )
{
	// �t���O��Ԃ�
	return GSYS.CreateImage.NotDrawValidCreateZBufferFlag ? FALSE : TRUE ;
}

// SetDrawScreen �̈����Ƃ��ēn����( �`��ΏۂƂ��Ďg�p�ł��� )�O���t�B�b�N�n���h���ɓK�p����y�o�b�t�@�̃r�b�g�[�x��ݒ肷��( BitDepth:�r�b�g�[�x( �w��\�Ȓl�� 16, 24, 32 �̉��ꂩ( SetDrawValidGraphCreateFlag �֐��ŕ`��ΏۂƂ��Ďg�p�ł���O���t�B�b�N�n���h�����쐬����悤�ɐݒ肳��Ă��Ȃ��ƌ��ʂ���܂��� )
extern int NS_SetCreateDrawValidGraphZBufferBitDepth( int BitDepth )
{
	// �r�b�g�[�x��ۑ�����
	GSYS.CreateImage.DrawValidZBufferBitDepth = BitDepth ;

	// �I��
	return 0 ;
}

// SetDrawScreen �̈����Ƃ��ēn����( �`��ΏۂƂ��Ďg�p�ł��� )�O���t�B�b�N�n���h���ɓK�p����y�o�b�t�@�̃r�b�g�[�x���擾����
extern int NS_GetCreateDrawValidGraphZBufferBitDepth( void )
{
	// �r�b�g�[�x��Ԃ�
	return GSYS.CreateImage.DrawValidZBufferBitDepth == 0 ? 16 : GSYS.CreateImage.DrawValidZBufferBitDepth ;
}

// �쐬����`��\�ȉ摜�̃`�����l������ݒ肷��,SetDrawValidGraphCreateFlag �֐��ŕ`��\�摜���쐬����悤�ɐݒ肳��Ă��Ȃ��ƌ��ʂ��Ȃ�
extern int NS_SetCreateDrawValidGraphChannelNum( int ChannelNum )
{
	// �`�����l������ۑ�����
	GSYS.CreateImage.DrawValidChannelNum = ChannelNum ;

	// �I��
	return 0 ;
}

// �쐬����`��\�ȉ摜�̃`�����l�������擾����
extern int NS_GetCreateDrawValidGraphChannelNum( void )
{
	// �`�����l������Ԃ�
	return GSYS.CreateImage.DrawValidChannelNum ;
}

// SetDrawScreen �̈����Ƃ��ēn����( �`��ΏۂƂ��Ďg�p�ł��� )�O���t�B�b�N�n���h���ɓK�p����}���`�T���v�����O( �A���`�G�C���A�V���O )�ݒ���s��( Samples:�}���`�T���v�������Ɏg�p����h�b�g��( �����قǏd���Ȃ�܂� )  Quality:�}���`�T���v�������̕i�� )
extern int NS_SetCreateDrawValidGraphMultiSample( int Samples, int Quality )
{
	if( Samples > 16 )
		Samples = 16 ;

	// �p�����[�^��ۑ�����
	GSYS.CreateImage.DrawValidMSSamples = Samples ;
	GSYS.CreateImage.DrawValidMSQuality = Quality ;

	// �I��
	return 0 ;
}

// �`��\�ȉ摜�̃}���`�T���v�����O�ݒ���s��
extern int NS_SetDrawValidMultiSample( int Samples, int Quality )
{
	return NS_SetCreateDrawValidGraphMultiSample( Samples, Quality ) ;
}

// �w��̃}���`�T���v�����Ŏg�p�ł���ő�N�I���e�B�l���擾����
extern int NS_GetMultiSampleQuality( int Samples )
{
	return Graphics_Hardware_GetMultiSampleQuality_PF( Samples ) ;
}

// ���ߐF�@�\���g�p���邩�ǂ�����ݒ肷��
extern int NS_SetUseTransColor( int Flag )
{
	GSYS.CreateImage.NotUseTransColor = !Flag;

	// �I��
	return 0;
}

// ���ߐF�@�\���g�p���邱�Ƃ�O��Ƃ����摜�̓ǂݍ��ݏ������s�����ǂ�����ݒ肷��( TRUE �ɂ���� SetDrawMode( DX_DRAWMODE_BILINEAR ); ��������Ԃ� DrawGraphF ���̕��������_�^���W���󂯎��֐��ŏ����_�ȉ��̒l���w�肵���ꍇ�ɔ�������`�挋�ʂ̕s���R���ɘa������ʂ����� ( �f�t�H���g�� FALSE ) )
extern int NS_SetUseTransColorGraphCreateFlag( int Flag )
{
	GSYS.CreateImage.UseTransColorFlag = Flag;

	return 0;
}

// SetUseAlphaChannelGraphCreateFlag �̋�����
extern int NS_SetUseGraphAlphaChannel( int Flag )
{
	return NS_SetUseAlphaChannelGraphCreateFlag( Flag ) ;
}

// GetUseAlphaChannelGraphCreateFlag �̋�����
extern int NS_GetUseGraphAlphaChannel( void )
{
	return NS_GetUseAlphaChannelGraphCreateFlag() ;
}

// ���`�����l���t���O���t�B�b�N���쐬���邩�ǂ����̃t���O���Z�b�g����( TRUE:���`�����l���t��   FALSE:���`�����l������ )
extern int NS_SetUseAlphaChannelGraphCreateFlag( int Flag )
{
	// �t���O��ۑ�����
	GSYS.CreateImage.AlphaChannelFlag = Flag ;
	
	// �I��
	return 0 ;
}

// ���`�����l���t���O���t�B�b�N���쐬���邩�ǂ����̃t���O���擾����( TRUE:���`�����l���t��   FALSE:���`�����l������ )
extern int NS_GetUseAlphaChannelGraphCreateFlag( void )
{
	return GSYS.CreateImage.AlphaChannelFlag ;
}

// ��Ǘ��e�N�X�`�����g�p���邩�A�̃t���O���Z�b�g( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
extern int NS_SetUseNotManageTextureFlag( int Flag )
{
	GSYS.CreateImage.NotUseManagedTextureFlag = Flag ;

	// �I��
	return 0 ;
}

// ��Ǘ��e�N�X�`�����g�p���邩�A�̃t���O���擾����( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
extern int NS_GetUseNotManageTextureFlag( void )
{
	return GSYS.CreateImage.NotUseManagedTextureFlag ;
}

// �쐬����O���t�B�b�N�n���h���Ŏg�p������ˑ��̃e�N�X�`���t�H�[�}�b�g���w�肷��( Direct3D9���Ȃ� DX_TEXTUREFORMAT_DIRECT3D9_R8G8B8 �ȂǁA0 ��n���Ɖ��� )
extern int NS_SetUsePlatformTextureFormat( int PlatformTextureFormat )
{
	GSYS.CreateImage.PlatformTextureFormat = PlatformTextureFormat ;

	// ���ˑ����������s����
	Graphics_Hardware_SetUsePlatformTextureFormat_PF( PlatformTextureFormat ) ;

	// �I��
	return 0 ;
}

// �쐬����O���t�B�b�N�n���h���Ŏg�p������ˑ��̃e�N�X�`���t�H�[�}�b�g���擾����
extern int NS_GetUsePlatformTextureFormat( void )
{
	return GSYS.CreateImage.PlatformTextureFormat ;
}

// �O���t�B�b�N�ɐݒ肷�铧�ߐF���Z�b�g����
extern int NS_SetTransColor( int Red, int Green, int Blue )
{
	if( NS_GetColorBitDepth() == 8 ) return 0 ;
	GSYS.CreateImage.TransColor = ( DWORD)( ( ( ( BYTE )Red ) << 16 ) | ( ( ( BYTE )Green ) << 8 ) | ( ( BYTE )Blue ) ) ;
	BASEIM.TransColor = GSYS.CreateImage.TransColor ;

	// �I��
	return 0 ;
}

// ���ߐF�𓾂�
extern int NS_GetTransColor( int *Red, int *Green, int *Blue )
{
	DWORD TransColor ;

	if( NS_GetColorBitDepth() == 8 ) return -1 ;

	TransColor = GSYS.CreateImage.TransColor ;

	*Red   = ( int )( ( TransColor >> 16 ) & 0xff ) ;
	*Green = ( int )( ( TransColor >> 8  ) & 0xff ) ;
	*Blue  = ( int )(   TransColor         & 0xff ) ;
 
	// �I��
	return 0 ;
}

// �K�v�Ȃ�O���t�B�b�N�̕������s�����A�t���O�̃Z�b�g
extern	int		NS_SetUseDivGraphFlag( int Flag ) 
{
	GSYS.CreateImage.NotUseDivFlag = !Flag ;

	// �I��
	return 0 ;
}

// LoadGraph �Ȃǂ̍ۂɃt�@�C�����̖����� _a ���t�����A���t�@�`�����l���p�̉摜�t�@�C����ǉ��œǂݍ��ޏ������s�����ǂ�����ݒ肷��( TRUE:�s��( �f�t�H���g )  FALSE:�s��Ȃ� )
extern	int		NS_SetUseAlphaImageLoadFlag( int Flag )
{
	GSYS.CreateImage.NotUseAlphaImageLoadFlag = Flag ? FALSE : TRUE ;

	// �I��
	return 0 ;
}

// �g�p����e�N�X�`���[�̍ő�T�C�Y���Z�b�g(0�Ńf�t�H���g)
extern	int		NS_SetUseMaxTextureSize( int Size )
{
	int i ;

	if( Size == 0 )
	{
		GSYS.CreateImage.UserMaxTextureSize = 0 ;
		return 0 ;
	}
	
	// �w��̃T�C�Y�����܂�ŏ��̂Q�̂���̃T�C�Y�����߂�
	for( i = MIN_TEXTURE_SIZE ; i < Size ; i <<= 1 ){}

	// �ő�T�C�Y���傫��������␳
	if( Size > GSYS.HardInfo.MaxTextureSize ) Size = GSYS.HardInfo.MaxTextureSize ; 
	
	// �l���Z�b�g
	GSYS.CreateImage.UserMaxTextureSize = i ;
	
	// �I��
	return 0 ;
}

// �摜���쐬����ۂɎg�p�����摜�f�[�^�̃o�b�N�A�b�v�����ăf�o�C�X���X�g���Ɏg�p���邩�ǂ����̃t���O���Z�b�g����( TRUE:�o�b�N�A�b�v������( �f�t�H���g )  FALSE:�o�b�N�A�b�v�����Ȃ� )
extern int NS_SetUseGraphBaseDataBackup( int Flag )
{
	GSYS.CreateImage.NotGraphBaseDataBackupFlag = Flag ? FALSE : TRUE ;

	// �I��
	return 0 ;
}

// �摜���쐬����ۂɎg�p�����摜�f�[�^�̃o�b�N�A�b�v�����ăf�o�C�X���X�g���Ɏg�p���邩�ǂ����̃t���O���擾����
extern int NS_GetUseGraphBaseDataBackup( void )
{
	return GSYS.CreateImage.NotGraphBaseDataBackupFlag ? FALSE : TRUE ;
}

// �V�X�e����������ɃO���t�B�b�N���쐬���邩�ǂ����̃t���O���Z�b�g����( TRUE:�V�X�e����������ɍ쐬  FALSE:�u�q�`�l��ɍ쐬 )
extern	int NS_SetUseSystemMemGraphCreateFlag( int /* Flag */ )
{
	// �t���O��ۑ�����
//	GSYS.SystemMemImageCreateFlag = Flag ;

	// �I��
	return 0 ;
}

// �V�X�e����������ɃO���t�B�b�N���쐬���邩�ǂ����̃t���O���擾����( TRUE:�V�X�e����������ɍ쐬  FALSE:�u�q�`�l��ɍ쐬 )
extern int NS_GetUseSystemMemGraphCreateFlag( void )
{
//	return GSYS.SystemMemImageCreateFlag ;
	return FALSE ;
}


























// �摜���֌W�֐�

// �w��̉摜�̂`�q�f�a�W�̃t���J���[�C���[�W���擾����
extern const unsigned int *NS_GetFullColorImage( int GrHandle )
{
	IMAGEDATA *Image ;

//	if( GSYS.NotDrawFlag ) return 0 ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return NULL ;

	// �t���J���[�C���[�W���i�[���郁�����̈���܂��m�ۂ��Ă��Ȃ��ꍇ�͊m�ۂ���
	if( Image->FullColorImage == NULL )
	{
		Image->FullColorImage = ( DWORD * )DXALLOC( ( size_t )( Image->WidthI * 4 * Image->HeightI ) ) ;
		if( Image->FullColorImage == NULL )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xeb\x30\xab\x30\xe9\x30\xfc\x30\x6e\x30\x3b\x75\xcf\x50\xa4\x30\xe1\x30\xfc\x30\xb8\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x92\x30\xba\x78\xdd\x4f\xfa\x51\x65\x67\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x02\x30\x0a\x00\x00"/*@ L"�t���J���[�̉摜�C���[�W�f�[�^���i�[���郁�����̈���m�ۏo���܂���ł����B\n" @*/ ) ;
			return NULL;
		}
	}

#ifndef DX_NON_MOVIE

	// ���[�r�[�̏ꍇ�͏����𕪊�
	if( Image->MovieHandle >= 0 )
	{
		MOVIEGRAPH * Movie ;
		Movie = GetMovieData( Image->MovieHandle ) ;
		if( Movie == NULL ) return NULL ;
		static BASEIMAGE Dest ;

		// �T�[�t�F�X���[�h���I�[�o�[���C�̏ꍇ�͖���
		if( Movie->SurfaceMode == DX_MOVIESURFACE_OVERLAY ) return NULL ;

		// ����ȊO�̏ꍇ�̓��[�r�[���璼�ڃf�[�^���擾����

		// ���[�r�[�̃t���[�����X�V
		if( NS_GetMovieStateToGraph( GrHandle ) == FALSE )
		{
			int Time ;

			Time = NS_TellMovieToGraph( GrHandle ) ;
			NS_PlayMovieToGraph( GrHandle ) ;
			UpdateMovie( Image->MovieHandle, TRUE ) ;
			NS_PauseMovieToGraph( GrHandle ) ;
			NS_SeekMovieToGraph( GrHandle, Time ) ;
		}
		else
		{
//			UpdateMovieToGraph( GrHandle ) ;
		}

		// �]�����{�C���[�W�f�[�^�̏�����������
		CreateARGB8ColorData( &Dest.ColorData ) ;
		Dest.Width     = Image->WidthI ;
		Dest.Height    = Image->HeightI ;
		Dest.GraphData = Image->FullColorImage ;
		Dest.Pitch     = Image->WidthI * 4 ;

		// �]��
		BltBaseImage( 0, 0, Image->WidthI, Image->HeightI, 0, 0, &Movie->NowImage, &Dest ) ;
	}
	else
#endif
	{
	}

	// �C���[�W�̃|�C���^��Ԃ�
	return (unsigned int *)Image->FullColorImage ;
}

// �O���t�B�b�N�������̈�̃��b�N
extern int NS_GraphLock( int GrHandle, int *PitchBuf, void **DataPointBuf, COLORDATA **ColorDataPP, int WriteOnly )
{
	// �摜���ǂ����ŏ����𕪊�
	if( GrHandle == DX_SCREEN_BACK || GrHandle == DX_SCREEN_FRONT )
	{
		// �n�[�h�E�G�A���g�p���Ă��邩�ǂ����ŏ����𕪊�
		if( GSYS.Setting.ValidHardware )
		{
			// �n�[�h�E�G�A�ł̃��b�N�͑Ή����Ă��Ȃ�
			return -1 ;
		}
		else
		{
			// �\�t�g�E�G�A�̏ꍇ�͉�ʃC���[�W�̏���Ԃ�
			if( ColorDataPP  ) *ColorDataPP  =        GSYS.SoftRender.MainBufferMemImg.Base->ColorDataP ;
			if( PitchBuf     ) *PitchBuf     = ( int )GSYS.SoftRender.MainBufferMemImg.Base->Pitch ;
			if( DataPointBuf ) *DataPointBuf =        GSYS.SoftRender.MainBufferMemImg.UseImage ;
		}
	}
	else
	{
		IMAGEDATA *Image ;

		// �G���[����
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;

		// ���Ƀ��b�N���Ă�������G���[
		if( Image->LockFlag == TRUE ) return -1 ;

		// ���b�N�t���O�𗧂Ă�
		Image->LockFlag = TRUE ;

		// �e�N�X�`�����ǂ����ŏ����𕪊�
		if( Image->Orig->FormatDesc.TextureFlag )
		{
			COLORDATA *ColorData ;

			// �e�N�X�`���̏ꍇ
			if( Graphics_Hardware_GraphLock_PF( Image, &ColorData, WriteOnly ) < 0 )
			{
				return -1 ;
			}

			// ���̊i�[
			if( PitchBuf     ) *PitchBuf     = ( int )Image->LockImagePitch ;
			if( DataPointBuf ) *DataPointBuf = ( void * )Image->LockImage ;
			if( ColorDataPP  ) *ColorDataPP  = ColorData ;
		}
		else
		{
			// �e�N�X�`���ł͂Ȃ��ꍇ
			if( PitchBuf     ) *PitchBuf     = ( int )Image->Soft.MemImg.Base->Pitch ;
			if( DataPointBuf ) *DataPointBuf =        Image->Soft.MemImg.UseImage ;
			if( ColorDataPP  ) *ColorDataPP  =        Image->Soft.MemImg.Base->ColorDataP ;
		}
	}

	// �I��
	return 0 ;
}

// �O���t�B�b�N�������̈�̃��b�N����
extern	int NS_GraphUnLock( int GrHandle )
{
	IMAGEDATA *Image ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

	// ���b�N���ĂȂ�������G���[
	if( Image->LockFlag == FALSE ) return -1 ;

	// ���b�N�t���O��|��
	Image->LockFlag = FALSE ;

	// �e�N�X�`�����ǂ����ŏ����𕪊�
	if( Image->Orig->FormatDesc.TextureFlag )
	{
		// �e�N�X�`���̏ꍇ
		if( Graphics_Hardware_GraphUnlock_PF( Image ) < 0 )
		{
			return -1 ;
		}
	}
	else
	{
		// �e�N�X�`���ł͂Ȃ��ꍇ
	}

	// �I��
	return 0 ;
}

// �O���t�B�b�N�ɂy�o�b�t�@���g�p���邩�ǂ�����ݒ肷��
extern	int NS_SetUseGraphZBuffer( int GrHandle, int UseFlag, int BitDepth )
{
	IMAGEDATA *Image ;
	int DepthIndex ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

	// �e�N�X�`���ł͂Ȃ��ꍇ�͉������Ȃ�
	if( Image->Orig->FormatDesc.TextureFlag == FALSE )
		return -1 ;

	// �`��\�ł͂Ȃ��ꍇ���������Ȃ�
	if( Image->Orig->FormatDesc.DrawValidFlag == FALSE )
		return -1 ;

	// �r�b�g�[�x�̃Z�b�g
	if( Image->Orig->ZBufferFlag == TRUE && BitDepth < 0 )
	{
		DepthIndex = Image->Orig->ZBufferBitDepthIndex ;
	}
	else
	{
		switch( BitDepth )
		{
		default :
		case 16 : DepthIndex = ZBUFFER_FORMAT_16BIT ; break ;
		case 24 : DepthIndex = ZBUFFER_FORMAT_24BIT ; break ;
		case 32 : DepthIndex = ZBUFFER_FORMAT_32BIT ; break ;
		}
	}

	// �y�o�b�t�@���g���t���O�ƃr�b�g�[�x�������������牽�����Ȃ�
	if( Image->Orig->ZBufferFlag == UseFlag && Image->Orig->ZBufferBitDepthIndex == DepthIndex )
		return 0 ;

	// �t���O��ۑ�
	Image->Orig->ZBufferFlag = UseFlag ;

	// �r�b�g�[�x�C���f�b�N�X��ۑ�
	Image->Orig->ZBufferBitDepthIndex = DepthIndex ;

	// �e�N�X�`���̍�蒼��
	Graphics_Hardware_CreateOrigTexture_PF( Image->Orig ) ;

	// �I��
	return 0 ;
}

// �O���t�B�b�N�̂y�o�b�t�@�̏�Ԃ�ʂ̃O���t�B�b�N�̂y�o�b�t�@�ɃR�s�[����( DestGrHandle �� SrcGrHandle ���y�o�b�t�@�������Ă���`��\�摜�ŁA���A���`�G�C���A�X�摜�ł͂Ȃ����Ƃ����� )
extern int NS_CopyGraphZBufferImage( int DestGrHandle, int SrcGrHandle )
{
	IMAGEDATA *SrcImage ;
	IMAGEDATA *DestImage ;

	// �G���[����
	if( GRAPHCHK( SrcGrHandle, SrcImage ) )
		return -1 ;

	if( GRAPHCHK( DestGrHandle, DestImage ) )
		return -1 ;

	// �e�N�X�`���ł͂Ȃ��ꍇ�͉������Ȃ�
	if( SrcImage->Orig->FormatDesc.TextureFlag == FALSE ||
		DestImage->Orig->FormatDesc.TextureFlag == FALSE )
		return -1 ;

	// ���ˑ�����
	if( Graphics_Hardware_CopyGraphZBufferImage_PF( DestImage, SrcImage ) < 0 )
	{
		return -1 ;
	}

	// ����I��
	return 0 ;
}

// �O���t�B�b�N�X�f�o�C�X�̃f�o�C�X���X�g�������Ɏw��̃O���t�B�b�N�n���h�����폜���邩�ǂ�����ݒ肷��( TRUE:�f�o�C�X���X�g���ɍ폜����  FALSE:�f�o�C�X���X�g���������Ă��폜���Ȃ� )
extern int NS_SetDeviceLostDeleteGraphFlag(	int GrHandle, int DeleteFlag )
{
	IMAGEDATA *Image ;

	if( GRAPHCHK_ASYNC( GrHandle, Image ) )
		return -1 ;

	Image->DeviceLostDeleteFlag = DeleteFlag ? TRUE : FALSE ;

	return 0 ;
}

// �O���t�B�b�N�̃T�C�Y�𓾂�
extern	int NS_GetGraphSize( int GrHandle, int *SizeXBuf, int *SizeYBuf )
{
	IMAGEDATA *Image ;
	SHADOWMAPDATA *ShadowMap ;

	switch( ( DWORD )GrHandle )
	{
	case DX_SCREEN_BACK	 :
	case DX_SCREEN_FRONT :
		if( SizeXBuf ) *SizeXBuf = GSYS.Screen.MainScreenSizeX ;
		if( SizeYBuf ) *SizeYBuf = GSYS.Screen.MainScreenSizeY ;
		break ;

	default :
		// �G���[����
		if( !GRAPHCHK_ASYNC( GrHandle, Image ) )
		{
			if( SizeXBuf ) *SizeXBuf = Image->WidthI ;
			if( SizeYBuf ) *SizeYBuf = Image->HeightI ;
		}
		else
		if( !SHADOWMAPCHK( GrHandle, ShadowMap ) )
		{
			if( SizeXBuf ) *SizeXBuf = ShadowMap->BaseSizeX ;
			if( SizeYBuf ) *SizeYBuf = ShadowMap->BaseSizeY ;
		}
		else
		{
			return -1 ;
		}
		break ;
	}

	return 0 ;
}

// �O���t�B�b�N�̃T�C�Y�𓾂�( float�^ )
extern	int NS_GetGraphSizeF( int GrHandle, float *SizeXBuf, float *SizeYBuf )
{
	IMAGEDATA *Image ;
	SHADOWMAPDATA *ShadowMap ;

	switch( ( DWORD )GrHandle )
	{
	case DX_SCREEN_BACK	 :
	case DX_SCREEN_FRONT :
		if( SizeXBuf ) *SizeXBuf = ( float )GSYS.Screen.MainScreenSizeX ;
		if( SizeYBuf ) *SizeYBuf = ( float )GSYS.Screen.MainScreenSizeY ;
		break ;

	default :
		// �G���[����
		if( !GRAPHCHK_ASYNC( GrHandle, Image ) )
		{
			if( SizeXBuf ) *SizeXBuf = Image->WidthF ;
			if( SizeYBuf ) *SizeYBuf = Image->HeightF ;
		}
		else
		if( !SHADOWMAPCHK( GrHandle, ShadowMap ) )
		{
			if( SizeXBuf ) *SizeXBuf = ( float )ShadowMap->BaseSizeX ;
			if( SizeYBuf ) *SizeYBuf = ( float )ShadowMap->BaseSizeY ;
		}
		else
		{
			return -1 ;
		}
		break ;
	}

	return 0 ;
}

// �O���t�B�b�N�Ɏg�p����Ă���e�N�X�`���[�̃T�C�Y�𓾂�
extern int NS_GetGraphTextureSize( int GrHandle, int *SizeXBuf, int *SizeYBuf )
{
	IMAGEDATA *Image = NULL ;
	SHADOWMAPDATA *ShadowMap = NULL ;

	// �G���[����
	if( GRAPHCHK(     GrHandle, Image    ) &&
		SHADOWMAPCHK( GrHandle, ShadowMap ) )
		return -1 ;

	// �V���h�E�}�b�v���ǂ����ŏ����𕪊�
	if( ShadowMap != NULL )
	{
		if( SizeXBuf ) *SizeXBuf = ShadowMap->BaseSizeX ;
		if( SizeYBuf ) *SizeYBuf = ShadowMap->BaseSizeY ;
	}
	else
	{
		// �e�N�X�`�����ǂ����ŏ����𕪊�
		if( Image->Orig->FormatDesc.TextureFlag )
		{
			if( SizeXBuf ) *SizeXBuf = Image->Orig->Hard.Tex[ 0 ].TexWidth ;
			if( SizeYBuf ) *SizeYBuf = Image->Orig->Hard.Tex[ 0 ].TexHeight ;
		}
		else
		{
			if( SizeXBuf ) *SizeXBuf = Image->WidthI ;
			if( SizeYBuf ) *SizeYBuf = Image->HeightI ;
		}
	}

	// �I��
	return 0 ;
}

// �O���t�B�b�N�����~�b�v�}�b�v���x���̐����擾����
extern int NS_GetGraphMipmapCount( int GrHandle )
{
	IMAGEDATA *Image ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

	// �n�[�h�E�F�A���g�p���Ă��Ȃ��ꍇ�͂P
	if( Image->Orig->FormatDesc.TextureFlag == FALSE )
		return 1 ;

	// �~�b�v�}�b�v�̐���Ԃ�
	return Image->Orig->Hard.MipMapCount ;
}

// �O���t�B�b�N�n���h�����摜�t�@�C������ǂݍ��܂�Ă����ꍇ�A���̉摜�̃t�@�C���p�X���擾����
extern int NS_GetGraphFilePath( int GrHandle, TCHAR *FilePathBuffer )
{
#ifdef UNICODE
	return GetGraphFilePath_WCHAR_T( GrHandle, FilePathBuffer ) ;
#else
	IMAGEDATA *Image ;
	TCHAR TempBuffer[ 2048 ] ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
	{
		return -1 ;
	}

	// �摜�t�@�C���p�X�������ꍇ�̓G���[
	if( Image->ReadBase->FileName == NULL )
	{
		return -1 ;
	}

	// �摜�t�@�C���p�X���R�s�[
	ConvString( ( const char * )Image->ReadBase->FileName, WCHAR_T_CHARCODEFORMAT, TempBuffer, sizeof( TempBuffer ), _TCHARCODEFORMAT ) ;
	if( FilePathBuffer != NULL )
	{
		_TSTRCPY( FilePathBuffer, TempBuffer ) ;
	}

	// ����I���̏ꍇ�͕�����T�C�Y��Ԃ�
	return ( int )_TSTRLEN( TempBuffer ) ;
#endif
}

// �O���t�B�b�N�n���h�����摜�t�@�C������ǂݍ��܂�Ă����ꍇ�A���̉摜�̃t�@�C���p�X���擾����
extern int GetGraphFilePath_WCHAR_T( int GrHandle, wchar_t *FilePathBuffer )
{
	IMAGEDATA *Image ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

	// �摜�t�@�C���p�X�������ꍇ�̓G���[
	if( Image->ReadBase->FileName == NULL )
		return -1 ;

	// �摜�t�@�C���p�X���R�s�[
	if( FilePathBuffer != NULL )
	{
		_WCSCPY( FilePathBuffer, Image->ReadBase->FileName ) ;
	}

	// ����I���̏ꍇ�͕�����T�C�Y��Ԃ�
	return ( int )_WCSLEN( Image->ReadBase->FileName ) ;
}

// �J���[�f�[�^�𓾂�
extern const COLORDATA * NS_GetTexColorData( int AlphaCh, int AlphaTest, int ColorBitDepth, int DrawValid )
{
	IMAGEFORMATDESC Format ;
	
	Format.DrawValidFlag   = ( unsigned char )DrawValid ;
	Format.AlphaChFlag     = ( unsigned char )AlphaCh ;
	Format.AlphaTestFlag   = ( unsigned char )AlphaTest ;
	Format.ColorBitDepth   = ( BYTE )( ColorBitDepth == TEX_BITDEPTH_16 ? 16 : 32 ) ;
	Format.ChannelNum      = 0 ;
	Format.ChannelBitDepth = 0 ;
	Format.FloatTypeFlag   = FALSE ;
	Format.BaseFormat      = DX_BASEIMAGE_FORMAT_NORMAL ;
	return NS_GetTexColorData( &Format ) ;
}

// �t�H�[�}�b�g�Ɋ�Â����J���[�f�[�^�𓾂�
extern const COLORDATA * NS_GetTexColorData( const IMAGEFORMATDESC *Format )
{
	const COLORDATA * Result ;

	Result = NS_GetTexColorData( NS_GetTexFormatIndex( Format ) ) ;

	return Result ;
}

// �w��̃t�H�[�}�b�g�C���f�b�N�X�̃J���[�f�[�^�𓾂�
extern const COLORDATA * NS_GetTexColorData( int FormatIndex )
{
	static int Initialize = FALSE ;
	static COLORDATA ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_NUM ] ;

	// �l���͈͊O��������G���[
	if( FormatIndex >= DX_GRAPHICSIMAGE_FORMAT_3D_NUM ) return NULL ;

	// �ŏ��ɗ����Ƃ��ɏ�����
	if( Initialize == FALSE )
	{
		Initialize = TRUE ;

		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_RGB16                 ],  16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_RGB32                 ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB16           ],  16, 0x00000f00, 0x000000f0, 0x0000000f, 0x0000f000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32           ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16       ],  16, 0x00007c00, 0x000003e0, 0x0000001f, 0x00008000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB32       ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT1                  ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT2                  ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT3                  ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT4                  ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT5                  ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM0             ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM1             ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM2             ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM3             ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_I16              ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 4, 16, FALSE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F16              ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 4, 16, TRUE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32              ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 4, 32, TRUE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8                ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1,  8, FALSE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16               ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1, 16, FALSE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16               ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1, 16, TRUE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32               ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1, 32, TRUE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I8                ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 2,  8, FALSE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I16               ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 2, 16, FALSE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16               ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 2, 16, TRUE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32               ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 2, 32, TRUE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16       ],  16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32       ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ALPHA_RGB32 ],  32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_I16    ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 4, 16, FALSE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F16    ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 4, 16, TRUE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32    ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 4, 32, TRUE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8      ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1,  8, FALSE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16     ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1, 16, FALSE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16     ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1, 16, TRUE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32     ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 1, 32, TRUE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I8      ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 2,  8, FALSE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16     ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 2, 16, FALSE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16     ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 2, 16, TRUE ) ;
		NS_CreateColorData( &ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32     ],   0, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 2, 32, TRUE ) ;

		ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT1 ].Format = DX_BASEIMAGE_FORMAT_DXT1 ; ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT1 ].ColorBitDepth = 4 ;
		ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT2 ].Format = DX_BASEIMAGE_FORMAT_DXT2 ; ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT2 ].ColorBitDepth = 8 ;
		ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT3 ].Format = DX_BASEIMAGE_FORMAT_DXT3 ; ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT3 ].ColorBitDepth = 8 ;
		ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT4 ].Format = DX_BASEIMAGE_FORMAT_DXT4 ; ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT4 ].ColorBitDepth = 8 ;
		ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT5 ].Format = DX_BASEIMAGE_FORMAT_DXT5 ; ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_DXT5 ].ColorBitDepth = 8 ;
		ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM0 ].Format = DX_BASEIMAGE_FORMAT_PLATFORM0 ; ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM0 ].ColorBitDepth = 8 ;
		ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM1 ].Format = DX_BASEIMAGE_FORMAT_PLATFORM1 ; ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM1 ].ColorBitDepth = 8 ;
		ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM2 ].Format = DX_BASEIMAGE_FORMAT_PLATFORM2 ; ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM2 ].ColorBitDepth = 8 ;
		ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM3 ].Format = DX_BASEIMAGE_FORMAT_PLATFORM3 ; ColorData[ DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM3 ].ColorBitDepth = 8 ;
	}

	// �J���[�t�H�[�}�b�g��Ԃ�
	return &ColorData[ FormatIndex ] ;
}

// �O���t�B�b�N�Ɏg�p�����ő�e�N�X�`���T�C�Y���擾����
extern int NS_GetMaxGraphTextureSize( int *SizeX, int *SizeY )
{
	if( SizeX != NULL ) *SizeX = GSYS.HardInfo.MaxTextureWidth ;
	if( SizeY != NULL ) *SizeY = GSYS.HardInfo.MaxTextureHeight ;

	// �I��
	return 0 ;
}

// �O���t�B�b�N�����֐��̗L�����擾	
extern	int NS_GetValidRestoreShredPoint( void )
{
	return GSYS.Setting.GraphRestoreShred != NULL ;
}

// ���ꂩ��V���ɃO���t�B�b�N���쐬����ꍇ�Ɏg�p����J���[�����擾����
extern	int NS_GetCreateGraphColorData( COLORDATA * /*ColorData*/, IMAGEFORMATDESC * /*Format*/ )
{
	// �I��
	return 0 ;
}























// �摜�p���b�g����֌W�֐�

// ��������ɓǂݍ��񂾉摜�̃p���b�g���擾����(�t���J���[�摜�̏ꍇ�͖���)
extern	int		NS_GetGraphPalette( int GrHandle, int ColorIndex, int *Red, int *Green, int *Blue )
{
	MEMIMG *MemImg ;
	unsigned int Color;

	// �G���[�`�F�b�N
	if( ColorIndex < 0 || ColorIndex > 255 ) return -1 ;
	PALETTEIMAGECHK( GrHandle, MemImg );
	
	// �p���b�g���擾����
	switch( MemImg->Base->ColorType )
	{
	case 0: Color = ((WORD  *)MemImg->Base->Palette)[ ColorIndex ]; break;
	case 1: Color = ((DWORD *)MemImg->Base->Palette)[ ColorIndex ]; break;
	default : return -1 ;
	}
	NS_GetColor5( MemImg->Base->ColorDataP, Color, Red, Green, Blue ) ;
	
	// �I��
	return 0 ;
}

// ��������ɓǂݍ��񂾉摜�� SetGraphPalette �ŕύX����O�̃p���b�g���擾����
extern  int		NS_GetGraphOriginalPalette( int GrHandle, int ColorIndex, int *Red, int *Green, int *Blue )
{
	MEMIMG *MemImg ;
	unsigned int Color ;
	
	// �G���[�`�F�b�N
	if( ColorIndex < 0 || ColorIndex > 255 ) return -1 ;
	PALETTEIMAGECHK( GrHandle, MemImg );

	// �p���b�g���擾����
	switch( MemImg->Base->ColorType )
	{
	case 0: Color = ((WORD  *)MemImg->Base->OriginalPalette)[ColorIndex]; break;
	case 1: Color = ((DWORD *)MemImg->Base->OriginalPalette)[ColorIndex]; break;
	default : return -1 ;
	}
	NS_GetColor5( MemImg->Base->ColorDataP, Color, Red, Green, Blue ) ;
	
	// �I��
	return 0 ;
}

// ��������ɓǂݍ��񂾉摜�̃p���b�g��ύX����(�t���J���[�摜�̏ꍇ�͖���)
extern	int		NS_SetGraphPalette( int GrHandle, int ColorIndex, unsigned int Color )
{
	MEMIMG *MemImg ;
	int Red, Green, Blue ;
	COLORDATA *ColorData ;

	// �G���[�`�F�b�N
	if( ColorIndex < 0 || ColorIndex > 255 ) return -1 ;
	PALETTEIMAGECHK( GrHandle, MemImg );

	// �F�R�[�h���Z�b�g����
	NS_GetColor2( Color, &Red, &Green, &Blue ) ;
	ColorData = MemImg->Base->ColorDataP ;
	Color = ColorData->NoneMask +
				( ( ( BYTE )Red   >> ( 8 - ColorData->RedWidth   ) ) << ColorData->RedLoc   ) +
				( ( ( BYTE )Green >> ( 8 - ColorData->GreenWidth ) ) << ColorData->GreenLoc ) +
				( ( ( BYTE )Blue  >> ( 8 - ColorData->BlueWidth  ) ) << ColorData->BlueLoc  ) ;
	switch( MemImg->Base->ColorType )
	{
	case 0: ((WORD  *)MemImg->Base->Palette)[ColorIndex] = (WORD)Color;  break;
	case 1: ((DWORD *)MemImg->Base->Palette)[ColorIndex] = (DWORD)Color; break;
	}
	
	// �I��
	return 0 ;
}

// SetGraphPalette �ŕύX�����p���b�g��S�Č��ɖ߂�
extern int NS_ResetGraphPalette( int GrHandle )
{
	MEMIMG *MemImg ;
	int Size;
	
	// �G���[�`�F�b�N
	PALETTEIMAGECHK( GrHandle, MemImg );

	// ���̃p���b�g���R�s�[����
	switch( MemImg->Base->ColorType )
	{
	case 0 : Size = 1 ; break ;
	case 1 : Size = 2 ; break ;
	default : return -1 ;
	}
	_MEMCPY( MemImg->Base->Palette, MemImg->Base->OriginalPalette, ( size_t )( MemImg->Base->ColorNum << Size ) ) ;
	
	// �I��
	return 0 ;
}

























// �}�`�`��֐�

// �����w��t�����̕`��
static int DrawLine_Thickness( int x1, int y1, int x2, int y2, unsigned int Color, int Thickness )
{
	VERTEX vert[4];
	VECTOR v, p1, p2;
	int r, g, b;
	float f;

	if( Thickness <= 0 || Thickness == 1 )
	{
		return NS_DrawLine( x1, y1, x2, y2, Color, 1 ) ;
	}
	else
	{
		if( x1 == x2 && Thickness % 2 == 1 )
		{
			int x ;

			x = x1 - Thickness / 2 ;
			return NS_DrawBox( x, y1, x + Thickness, y2, Color, TRUE ) ;
		}
		else
		if( y1 == y2 && Thickness % 2 == 1 )
		{
			int y ;

			y = y1 - Thickness / 2 ;
			return NS_DrawBox( x1, y, x2, y + Thickness, Color, TRUE ) ;
		}
	}

	if( GSYS.Setting.ValidHardware )
	{
		p1.x = (float)x1;
		p1.y = (float)y1;
		p2.x = (float)x2;
		p2.y = (float)y2;

		v.x = (    p2.y - p1.y   );
		v.y = ( -( p2.x - p1.x ) );
		f = ( 1.0f / _SQRT( v.x * v.x + v.y * v.y ) ) * ( (float)Thickness / 2.0f );
		v.x *= f;
		v.y *= f;

		NS_GetColor2( Color, &r, &g, &b );
		vert[0].u = 0.0f;
		vert[0].v = 0.0f;
		vert[0].r = (unsigned char)r;
		vert[0].g = (unsigned char)g;
		vert[0].b = (unsigned char)b;
		vert[0].a = (unsigned char)255;
		vert[1] =
		vert[2] =
		vert[3] = vert[0];

		vert[0].x = p1.x + v.x;
		vert[0].y = p1.y + v.y;
		vert[1].x = p2.x + v.x;
		vert[1].y = p2.y + v.y;
		vert[2].x = p1.x - v.x;
		vert[2].y = p1.y - v.y;
		vert[3].x = p2.x - v.x;
		vert[3].y = p2.y - v.y;

		NS_DrawPolygonBase( vert, 4, DX_PRIMTYPE_TRIANGLESTRIP, DX_NONE_GRAPH, FALSE );
	}
	else
	{
		v.x = ( float )(    y2 - y1   );
		v.y = ( float )( -( x2 - x1 ) );
		f = ( 1.0f / _SQRT( v.x * v.x + v.y * v.y ) ) * ( (float)Thickness / 2.0f ) ;
		v.x *= f ;
		v.y *= f ;

		Graphics_Software_DrawQuadrangle( 
			_FTOL( x1 + v.x ),
			_FTOL( y1 + v.y ),
			_FTOL( x2 + v.x ),
			_FTOL( y2 + v.y ),
			_FTOL( x2 - v.x ),
			_FTOL( y2 - v.y ),
			_FTOL( x1 - v.x ),
			_FTOL( y1 - v.y ), Color, TRUE ) ;
	}

	// �I��
	return 0;
}

// ����`��
#define SETDRAWRECTCODE\
	SETRECT( DrawRect, x1, y1, x2, y2 ) ;\
	if( x1 > x2 ){ DrawRect.left = x2 ; DrawRect.right  = x1; }\
	if( y1 > y2 ){ DrawRect.top  = y2 ; DrawRect.bottom = y1; }\
	DrawRect.left   -= Thickness ;\
	DrawRect.top    -= Thickness ;\
	DrawRect.right  += 1 + Thickness ;\
	DrawRect.bottom += 1 + Thickness ;\
	DRAWRECT_DRAWAREA_CLIP

extern	int NS_DrawLine( int x1, int y1, int x2, int y2, unsigned int Color, int Thickness )
{
	int Ret = -1 ;
	int Flag ;

	// �������P�ȏ�̏ꍇ�͑����w��t�����`����s��
	if( Thickness > 1 )
		return DrawLine_Thickness( x1, y1, x2, y2, Color, Thickness ) ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	Flag = GSYS.Setting.ValidHardware ;

	CheckActiveState() ;

	DRAW_DEF(
		Graphics_Hardware_DrawLine_PF( x1, y1, x2, y2, Color ),
		Graphics_Software_DrawLine(    x1, y1, x2, y2, Color ),
		SETDRAWRECTCODE,
		Ret,
		Flag
	)

	// �I��
	return Ret ;
}

#undef SETDRAWRECTCODE

// ����`�悷��( �A���`�G�C���A�X�t�� )
extern int NS_DrawLineAA( float x1, float y1, float x2, float y2, unsigned int Color, float Thickness )
{
	int OldDrawMode = -1 ;
	int GrHandle ;
	GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM Param ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX ParamVertex[ 9 * 4 ] ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *Vertex ;
	VECTOR HVec ;
	VECTOR DVec ;
	float Distance ;
	float LinePosX[ 4 ][ 2 ] ;
	float LinePosY[ 4 ][ 2 ] ;
	float tmp ;
	float tmp1 ;
	float tmp2 ;
	float tmp3 ;
	float tmp4 ;
	int i ;
	int j ;
	DWORD FColor ;
	DWORD OrigColor ;
	static float LineUV[ 4 ] = { 0.0f, 3.0f / LINE_TEX_SIZE, 5.0f / LINE_TEX_SIZE, 1.0f } ;

	if( NS_GetDrawFloatCoordType() == DX_DRAWFLOATCOORDTYPE_DIRECT3D9 )
	{
		x1 -= 0.5f ;
		y1 -= 0.5f ;
		x2 -= 0.5f ;
		y2 -= 0.5f ;
	}

	DVec.x = x2 - x1 ;
	DVec.y = y2 - y1 ;
	DVec.z = 0.0f ;
	Distance = DVec.x * DVec.x + DVec.y * DVec.y ;
	if( Distance < 4.0f )
	{
		return 0 ;
	}
	Distance = _SQRT( Distance ) ;

	DVec.x /= Distance ;
	DVec.y /= Distance ;

	HVec.x =  DVec.y ;
	HVec.y = -DVec.x ;

	tmp = - 2.0f ; LinePosY[ 0 ][ 0 ] = DVec.x * tmp ; LinePosY[ 0 ][ 1 ] = DVec.y * tmp ;
	tmp =   1.0f ; LinePosY[ 1 ][ 0 ] = DVec.x * tmp ; LinePosY[ 1 ][ 1 ] = DVec.y * tmp ;
	tmp = - 1.0f ; LinePosY[ 2 ][ 0 ] = DVec.x * tmp ; LinePosY[ 2 ][ 1 ] = DVec.y * tmp ;
	tmp =   2.0f ; LinePosY[ 3 ][ 0 ] = DVec.x * tmp ; LinePosY[ 3 ][ 1 ] = DVec.y * tmp ;

	SETUP32BITCOLOR( Color, FColor ) ;
	OrigColor = GSYS.DrawSetting.bDrawBright ;
	Graphics_DrawSetting_SetDrawBrightToOneParam( FColor ) ;

	if( GSYS.DrawSetting.DrawMode != DX_DRAWMODE_BILINEAR )
	{
		OldDrawMode = GSYS.DrawSetting.DrawMode ;
		NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	}

	Param.Vertex      = ParamVertex ;
	Param.TransFlag   = TRUE ;
	if( Thickness <= 1.0f )
	{
		static float LineUV_2[ 2 ] = { 0.0f, 4.0f / LINE_TEX_SIZE } ;

		Param.QuadrangleNum = 3 ;

		GrHandle = Graphics_Image_GetLineOneThicknessTexHandle( IS_PMA_DRAWBLENDMODE ) ;

		Param.GraphHandle = GrHandle ;

		Thickness /= 2.0f;
		tmp1 = - Thickness - 1.5f;
		tmp2 = - Thickness + 2.5f;

		LinePosX[ 0 ][ 0 ] = HVec.x * tmp1 ; LinePosX[ 0 ][ 1 ] = HVec.y * tmp1 ;
		LinePosX[ 1 ][ 0 ] = HVec.x * tmp2 ; LinePosX[ 1 ][ 1 ] = HVec.y * tmp2 ;

		Vertex = ParamVertex ;
		for( i = 0; i < 3 ; i++ )
		{
			Vertex[ 0 ].u = LineUV_2[ 0 ] ;	Vertex[ 0 ].v = LineUV[ i + 0 ] ;
			Vertex[ 1 ].u = LineUV_2[ 1 ] ;	Vertex[ 1 ].v = LineUV[ i + 0 ] ;
			Vertex[ 2 ].u = LineUV_2[ 0 ] ;	Vertex[ 2 ].v = LineUV[ i + 1 ] ;
			Vertex[ 3 ].u = LineUV_2[ 1 ] ;	Vertex[ 3 ].v = LineUV[ i + 1 ] ;

			Vertex += 4 ;
		}

		Vertex = ParamVertex ;
		i = 0 ;
		Vertex[ 0 ].x = x1 + LinePosX[ 0 ][ 0 ] + LinePosY[ i + 0 ][ 0 ] ;	Vertex[ 0 ].y = y1 + LinePosY[ i + 0 ][ 1 ] + LinePosX[ 0 ][ 1 ] ;
		Vertex[ 1 ].x = x1 + LinePosX[ 1 ][ 0 ] + LinePosY[ i + 0 ][ 0 ] ;	Vertex[ 1 ].y = y1 + LinePosY[ i + 0 ][ 1 ] + LinePosX[ 1 ][ 1 ] ;
		Vertex[ 2 ].x = x1 + LinePosX[ 0 ][ 0 ] + LinePosY[ i + 1 ][ 0 ] ;	Vertex[ 2 ].y = y1 + LinePosY[ i + 1 ][ 1 ] + LinePosX[ 0 ][ 1 ] ;
		Vertex[ 3 ].x = x1 + LinePosX[ 1 ][ 0 ] + LinePosY[ i + 1 ][ 0 ] ;	Vertex[ 3 ].y = y1 + LinePosY[ i + 1 ][ 1 ] + LinePosX[ 1 ][ 1 ] ;

		Vertex += 4 ;

		i = 1;
		Vertex[ 0 ].x = x1 + LinePosX[ 0 ][ 0 ] + LinePosY[ i + 0 ][ 0 ] ;	Vertex[ 0 ].y = y1 + LinePosY[ i + 0 ][ 1 ] + LinePosX[ 0 ][ 1 ] ;
		Vertex[ 1 ].x = x1 + LinePosX[ 1 ][ 0 ] + LinePosY[ i + 0 ][ 0 ] ;	Vertex[ 1 ].y = y1 + LinePosY[ i + 0 ][ 1 ] + LinePosX[ 1 ][ 1 ] ;
		Vertex[ 2 ].x = x2 + LinePosX[ 0 ][ 0 ] + LinePosY[ i + 1 ][ 0 ] ;	Vertex[ 2 ].y = y2 + LinePosY[ i + 1 ][ 1 ] + LinePosX[ 0 ][ 1 ] ;
		Vertex[ 3 ].x = x2 + LinePosX[ 1 ][ 0 ] + LinePosY[ i + 1 ][ 0 ] ;	Vertex[ 3 ].y = y2 + LinePosY[ i + 1 ][ 1 ] + LinePosX[ 1 ][ 1 ] ;

		Vertex += 4 ;

		i = 2 ;
		Vertex[ 0 ].x = x2 + LinePosX[ 0 ][ 0 ] + LinePosY[ i + 0 ][ 0 ] ;	Vertex[ 0 ].y = y2 + LinePosY[ i + 0 ][ 1 ] + LinePosX[ 0 ][ 1 ] ;
		Vertex[ 1 ].x = x2 + LinePosX[ 1 ][ 0 ] + LinePosY[ i + 0 ][ 0 ] ;	Vertex[ 1 ].y = y2 + LinePosY[ i + 0 ][ 1 ] + LinePosX[ 1 ][ 1 ] ;
		Vertex[ 2 ].x = x2 + LinePosX[ 0 ][ 0 ] + LinePosY[ i + 1 ][ 0 ] ;	Vertex[ 2 ].y = y2 + LinePosY[ i + 1 ][ 1 ] + LinePosX[ 0 ][ 1 ] ;
		Vertex[ 3 ].x = x2 + LinePosX[ 1 ][ 0 ] + LinePosY[ i + 1 ][ 0 ] ;	Vertex[ 3 ].y = y2 + LinePosY[ i + 1 ][ 1 ] + LinePosX[ 1 ][ 1 ] ;

		Vertex += 4 ;
	}
	else
	{
		Param.QuadrangleNum = 9 ;

		GrHandle = Graphics_Image_GetLineTexHandle( IS_PMA_DRAWBLENDMODE ) ;

		Param.GraphHandle = GrHandle ;

		Thickness /= 2.0f ;
		tmp1 = - Thickness - 1.5f ;
		tmp2 = - Thickness + 1.5f ;
		tmp3 =   Thickness - 0.5f ;
		tmp4 =   Thickness + 2.5f ;

		LinePosX[ 0 ][ 0 ] = HVec.x * tmp1 ; LinePosX[ 0 ][ 1 ] = HVec.y * tmp1 ;
		LinePosX[ 1 ][ 0 ] = HVec.x * tmp2 ; LinePosX[ 1 ][ 1 ] = HVec.y * tmp2 ;
		LinePosX[ 2 ][ 0 ] = HVec.x * tmp3 ; LinePosX[ 2 ][ 1 ] = HVec.y * tmp3 ;
		LinePosX[ 3 ][ 0 ] = HVec.x * tmp4 ; LinePosX[ 3 ][ 1 ] = HVec.y * tmp4 ;

		Vertex = ParamVertex ;
		for( i = 0; i < 3 ; i++ )
		{
			for( j = 0; j < 3 ; j++ )
			{
				Vertex[ 0 ].u = LineUV[ j + 0 ] ;	Vertex[ 0 ].v = LineUV[ i + 0 ] ;
				Vertex[ 1 ].u = LineUV[ j + 1 ] ;	Vertex[ 1 ].v = LineUV[ i + 0 ] ;
				Vertex[ 2 ].u = LineUV[ j + 0 ] ;	Vertex[ 2 ].v = LineUV[ i + 1 ] ;
				Vertex[ 3 ].u = LineUV[ j + 1 ] ;	Vertex[ 3 ].v = LineUV[ i + 1 ] ;

				Vertex += 4 ;
			}
		}

		Vertex = ParamVertex ;
		i = 0 ;
		for( j = 0; j < 3; j++ )
		{
			Vertex[ 0 ].x = x1 + LinePosX[ j + 0 ][ 0 ] + LinePosY[ i + 0 ][ 0 ] ;	Vertex[ 0 ].y = y1 + LinePosY[ i + 0 ][ 1 ] + LinePosX[ j + 0 ][ 1 ] ;
			Vertex[ 1 ].x = x1 + LinePosX[ j + 1 ][ 0 ] + LinePosY[ i + 0 ][ 0 ] ;	Vertex[ 1 ].y = y1 + LinePosY[ i + 0 ][ 1 ] + LinePosX[ j + 1 ][ 1 ] ;
			Vertex[ 2 ].x = x1 + LinePosX[ j + 0 ][ 0 ] + LinePosY[ i + 1 ][ 0 ] ;	Vertex[ 2 ].y = y1 + LinePosY[ i + 1 ][ 1 ] + LinePosX[ j + 0 ][ 1 ] ;
			Vertex[ 3 ].x = x1 + LinePosX[ j + 1 ][ 0 ] + LinePosY[ i + 1 ][ 0 ] ;	Vertex[ 3 ].y = y1 + LinePosY[ i + 1 ][ 1 ] + LinePosX[ j + 1 ][ 1 ] ;

			Vertex += 4 ;
		}

		i = 1;
		for( j = 0; j < 3; j++ )
		{
			Vertex[ 0 ].x = x1 + LinePosX[ j + 0 ][ 0 ] + LinePosY[ i + 0 ][ 0 ] ;	Vertex[ 0 ].y = y1 + LinePosY[ i + 0 ][ 1 ] + LinePosX[ j + 0 ][ 1 ] ;
			Vertex[ 1 ].x = x1 + LinePosX[ j + 1 ][ 0 ] + LinePosY[ i + 0 ][ 0 ] ;	Vertex[ 1 ].y = y1 + LinePosY[ i + 0 ][ 1 ] + LinePosX[ j + 1 ][ 1 ] ;
			Vertex[ 2 ].x = x2 + LinePosX[ j + 0 ][ 0 ] + LinePosY[ i + 1 ][ 0 ] ;	Vertex[ 2 ].y = y2 + LinePosY[ i + 1 ][ 1 ] + LinePosX[ j + 0 ][ 1 ] ;
			Vertex[ 3 ].x = x2 + LinePosX[ j + 1 ][ 0 ] + LinePosY[ i + 1 ][ 0 ] ;	Vertex[ 3 ].y = y2 + LinePosY[ i + 1 ][ 1 ] + LinePosX[ j + 1 ][ 1 ] ;

			Vertex += 4 ;
		}

		i = 2 ;
		for( j = 0; j < 3; j++ )
		{
			Vertex[ 0 ].x = x2 + LinePosX[ j + 0 ][ 0 ] + LinePosY[ i + 0 ][ 0 ] ;	Vertex[ 0 ].y = y2 + LinePosY[ i + 0 ][ 1 ] + LinePosX[ j + 0 ][ 1 ] ;
			Vertex[ 1 ].x = x2 + LinePosX[ j + 1 ][ 0 ] + LinePosY[ i + 0 ][ 0 ] ;	Vertex[ 1 ].y = y2 + LinePosY[ i + 0 ][ 1 ] + LinePosX[ j + 1 ][ 1 ] ;
			Vertex[ 2 ].x = x2 + LinePosX[ j + 0 ][ 0 ] + LinePosY[ i + 1 ][ 0 ] ;	Vertex[ 2 ].y = y2 + LinePosY[ i + 1 ][ 1 ] + LinePosX[ j + 0 ][ 1 ] ;
			Vertex[ 3 ].x = x2 + LinePosX[ j + 1 ][ 0 ] + LinePosY[ i + 1 ][ 0 ] ;	Vertex[ 3 ].y = y2 + LinePosY[ i + 1 ][ 1 ] + LinePosX[ j + 1 ][ 1 ] ;

			Vertex += 4 ;
		}
	}

	Graphics_Draw_DrawSimpleQuadrangleGraphF( &Param ) ;

	if( OldDrawMode != -1 )
	{
		NS_SetDrawMode( OldDrawMode ) ;
	}

	Graphics_DrawSetting_SetDrawBrightToOneParam( OrigColor ) ;

	// �I��
	return 0 ;
}


// �l�p�`�̕`��
extern	int NS_DrawBox( int x1, int y1, int x2, int y2, unsigned int Color, int FillFlag )
{
	int Ret ;
	
	if( FillFlag )	Ret = NS_DrawFillBox( x1, y1, x2, y2, Color ) ;
	else			Ret = NS_DrawLineBox( x1, y1, x2, y2, Color ) ;

	// �I��
	return Ret ;
}

// �l�p�`�̕`�悷��( �A���`�G�C���A�X�t�� )
static int DrawBoxLineAA( float x1, float y1, float x2, float y2, unsigned int Color, float LineThickness )
{
	int OldDrawMode = -1 ;
	int GrHandle ;
	GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM Param ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX ParamVertex[ ( 8 + 4 + 8 ) * 4 ] ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *Vertex ;
	float LinePos_Out_X[ 4 ];
	float LinePos_Out_Y[ 4 ];
	float LinePos_In_X[ 4 ];
	float LinePos_In_Y[ 4 ];
	int i ;
	int j ;
	DWORD FColor ;
	DWORD OrigColor ;
	static float Line_Out_UV[ 4 ] = { 0.0f,                    4.0f / LINEBOX_TEX_SIZE, 12.0f / LINEBOX_TEX_SIZE, 1.0f } ;
	static float Line_In_UV[ 4 ]  = { 5.0f / LINEBOX_TEX_SIZE, 8.0f / LINEBOX_TEX_SIZE, 8.0f  / LINEBOX_TEX_SIZE, 11.0f / LINEBOX_TEX_SIZE } ;

	if( x1 == x2 || y1 == y2 || x1 > x2 || y1 > y2 )
	{
		return 0 ;
	}

	x2 -= 1.0f ;
	y2 -= 1.0f ;
	if( NS_GetDrawFloatCoordType() == DX_DRAWFLOATCOORDTYPE_DIRECT3D9 )
	{
		x1 -= 0.5f ;
		y1 -= 0.5f ;
		x2 -= 0.5f ;
		y2 -= 0.5f ;
	}

	SETUP32BITCOLOR( Color, FColor ) ;
	OrigColor = GSYS.DrawSetting.bDrawBright ;
	Graphics_DrawSetting_SetDrawBrightToOneParam( FColor ) ;

	if( GSYS.DrawSetting.DrawMode != DX_DRAWMODE_BILINEAR )
	{
		OldDrawMode = GSYS.DrawSetting.DrawMode ;
		NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	}

	Param.Vertex        = ParamVertex ;
	Param.TransFlag     = TRUE ;
	if( LineThickness <= 1.0f )
	{
		Param.QuadrangleNum = 8 ;

		GrHandle = Graphics_Image_GetLineBoxOneThicknessTexHandle( IS_PMA_DRAWBLENDMODE ) ;

		Param.GraphHandle = GrHandle ;

		LineThickness /= 2.0f;
		LinePos_Out_X[ 0 ] = x1 - LineThickness - 1.5f ;
		LinePos_Out_X[ 1 ] = x1 - LineThickness + 2.5f ;
		LinePos_Out_X[ 2 ] = x2 + LineThickness - 1.5f ;
		LinePos_Out_X[ 3 ] = x2 + LineThickness + 2.5f ;

		LinePos_Out_Y[ 0 ] = y1 - LineThickness - 1.5f ;
		LinePos_Out_Y[ 1 ] = y1 - LineThickness + 2.5f ;
		LinePos_Out_Y[ 2 ] = y2 + LineThickness - 1.5f ;
		LinePos_Out_Y[ 3 ] = y2 + LineThickness + 2.5f ;

		Vertex = ParamVertex ;
		for( i = 0; i < 3 ; i++ )
		{
			for( j = 0; j < 3 ; j++ )
			{
				if( j == 1 && i == 1 )
				{
					continue ;
				}

				Vertex[ 0 ].x = LinePos_Out_X[ j + 1 ] ;	Vertex[ 0 ].y = LinePos_Out_Y[ i + 0 ] ;
				Vertex[ 1 ].x = LinePos_Out_X[ j + 0 ] ;	Vertex[ 1 ].y = LinePos_Out_Y[ i + 0 ] ;
				Vertex[ 2 ].x = LinePos_Out_X[ j + 1 ] ;	Vertex[ 2 ].y = LinePos_Out_Y[ i + 1 ] ;
				Vertex[ 3 ].x = LinePos_Out_X[ j + 0 ] ;	Vertex[ 3 ].y = LinePos_Out_Y[ i + 1 ] ;

				Vertex[ 0 ].u = Line_Out_UV[ j + 1 ] ;		Vertex[ 0 ].v = Line_Out_UV[ i + 0 ] ;
				Vertex[ 1 ].u = Line_Out_UV[ j + 0 ] ;		Vertex[ 1 ].v = Line_Out_UV[ i + 0 ] ;
				Vertex[ 2 ].u = Line_Out_UV[ j + 1 ] ;		Vertex[ 2 ].v = Line_Out_UV[ i + 1 ] ;
				Vertex[ 3 ].u = Line_Out_UV[ j + 0 ] ;		Vertex[ 3 ].v = Line_Out_UV[ i + 1 ] ;

				Vertex += 4 ;
			}
		}
	}
	else
	{
		Param.QuadrangleNum = 8 + 4 + 8 ;

		GrHandle = Graphics_Image_GetLineBoxTexHandle( IS_PMA_DRAWBLENDMODE ) ;

		Param.GraphHandle = GrHandle ;

		LineThickness /= 2.0f;
		LinePos_Out_X[ 0 ] = x1 - LineThickness - 1.5f ;
		LinePos_Out_X[ 1 ] = x1 - LineThickness + 2.5f ;
		LinePos_Out_X[ 2 ] = x2 + LineThickness - 1.5f ;
		LinePos_Out_X[ 3 ] = x2 + LineThickness + 2.5f ;

		LinePos_Out_Y[ 0 ] = y1 - LineThickness - 1.5f ;
		LinePos_Out_Y[ 1 ] = y1 - LineThickness + 2.5f ;
		LinePos_Out_Y[ 2 ] = y2 + LineThickness - 1.5f ;
		LinePos_Out_Y[ 3 ] = y2 + LineThickness + 2.5f ;

		LinePos_In_X[ 0 ] = x1 + LineThickness - 0.5f ;
		LinePos_In_X[ 1 ] = x1 + LineThickness + 2.5f ;
		LinePos_In_X[ 2 ] = x2 - LineThickness - 1.5f ;
		LinePos_In_X[ 3 ] = x2 - LineThickness + 1.5f ;

		LinePos_In_Y[ 0 ] = y1 + LineThickness - 0.5f ;
		LinePos_In_Y[ 1 ] = y1 + LineThickness + 2.5f ;
		LinePos_In_Y[ 2 ] = y2 - LineThickness - 1.5f ;
		LinePos_In_Y[ 3 ] = y2 - LineThickness + 1.5f ;

		Vertex = ParamVertex ;
		for( i = 0; i < 3 ; i++ )
		{
			for( j = 0; j < 3 ; j++ )
			{
				if( j == 1 && i == 1 )
				{
					continue ;
				}

				Vertex[ 0 ].x = LinePos_Out_X[ j + 1 ] ;	Vertex[ 0 ].y = LinePos_Out_Y[ i + 0 ] ;
				Vertex[ 1 ].x = LinePos_Out_X[ j + 0 ] ;	Vertex[ 1 ].y = LinePos_Out_Y[ i + 0 ] ;
				Vertex[ 2 ].x = LinePos_Out_X[ j + 1 ] ;	Vertex[ 2 ].y = LinePos_Out_Y[ i + 1 ] ;
				Vertex[ 3 ].x = LinePos_Out_X[ j + 0 ] ;	Vertex[ 3 ].y = LinePos_Out_Y[ i + 1 ] ;

				Vertex[ 0 ].u = Line_Out_UV[ j + 1 ] ;		Vertex[ 0 ].v = Line_Out_UV[ i + 0 ] ;
				Vertex[ 1 ].u = Line_Out_UV[ j + 0 ] ;		Vertex[ 1 ].v = Line_Out_UV[ i + 0 ] ;
				Vertex[ 2 ].u = Line_Out_UV[ j + 1 ] ;		Vertex[ 2 ].v = Line_Out_UV[ i + 1 ] ;
				Vertex[ 3 ].u = Line_Out_UV[ j + 0 ] ;		Vertex[ 3 ].v = Line_Out_UV[ i + 1 ] ;

				Vertex += 4 ;

				Vertex[ 0 ].x = LinePos_In_X[ j + 1 ] ;		Vertex[ 0 ].y = LinePos_In_Y[ i + 0 ] ;
				Vertex[ 1 ].x = LinePos_In_X[ j + 0 ] ;		Vertex[ 1 ].y = LinePos_In_Y[ i + 0 ] ;
				Vertex[ 2 ].x = LinePos_In_X[ j + 1 ] ;		Vertex[ 2 ].y = LinePos_In_Y[ i + 1 ] ;
				Vertex[ 3 ].x = LinePos_In_X[ j + 0 ] ;		Vertex[ 3 ].y = LinePos_In_Y[ i + 1 ] ;

				Vertex[ 0 ].u = Line_In_UV[ j + 1 ] ;		Vertex[ 0 ].v = Line_In_UV[ i + 0 ] ;
				Vertex[ 1 ].u = Line_In_UV[ j + 0 ] ;		Vertex[ 1 ].v = Line_In_UV[ i + 0 ] ;
				Vertex[ 2 ].u = Line_In_UV[ j + 1 ] ;		Vertex[ 2 ].v = Line_In_UV[ i + 1 ] ;
				Vertex[ 3 ].u = Line_In_UV[ j + 0 ] ;		Vertex[ 3 ].v = Line_In_UV[ i + 1 ] ;

				Vertex += 4 ;
			}
		}

		Vertex[ 0 ].x = LinePos_Out_X[ 1 ] ;		Vertex[ 0 ].y = LinePos_Out_Y[ 1 ] ;
		Vertex[ 1 ].x = LinePos_Out_X[ 2 ] ;		Vertex[ 1 ].y = LinePos_Out_Y[ 1 ] ;
		Vertex[ 2 ].x = LinePos_Out_X[ 1 ] ;		Vertex[ 2 ].y = LinePos_In_Y[  0 ] ;
		Vertex[ 3 ].x = LinePos_Out_X[ 2 ] ;		Vertex[ 3 ].y = LinePos_In_Y[  0 ] ;

		Vertex[ 0 ].u = Line_In_UV[ 0 ] ;			Vertex[ 0 ].v = Line_In_UV[ 0 ] ;
		Vertex[ 1 ].u = Line_In_UV[ 0 ] ;			Vertex[ 1 ].v = Line_In_UV[ 0 ] ;
		Vertex[ 2 ].u = Line_In_UV[ 0 ] ;			Vertex[ 2 ].v = Line_In_UV[ 0 ] ;
		Vertex[ 3 ].u = Line_In_UV[ 0 ] ;			Vertex[ 3 ].v = Line_In_UV[ 0 ] ;

		Vertex += 4 ;

		Vertex[ 0 ].x = LinePos_Out_X[ 1 ] ;		Vertex[ 0 ].y = LinePos_In_Y[  3 ] ;
		Vertex[ 1 ].x = LinePos_Out_X[ 2 ] ;		Vertex[ 1 ].y = LinePos_In_Y[  3 ] ;
		Vertex[ 2 ].x = LinePos_Out_X[ 1 ] ;		Vertex[ 2 ].y = LinePos_Out_Y[ 2 ] ;
		Vertex[ 3 ].x = LinePos_Out_X[ 2 ] ;		Vertex[ 3 ].y = LinePos_Out_Y[ 2 ] ;

		Vertex[ 0 ].u = Line_In_UV[ 0 ] ;			Vertex[ 0 ].v = Line_In_UV[ 0 ] ;
		Vertex[ 1 ].u = Line_In_UV[ 0 ] ;			Vertex[ 1 ].v = Line_In_UV[ 0 ] ;
		Vertex[ 2 ].u = Line_In_UV[ 0 ] ;			Vertex[ 2 ].v = Line_In_UV[ 0 ] ;
		Vertex[ 3 ].u = Line_In_UV[ 0 ] ;			Vertex[ 3 ].v = Line_In_UV[ 0 ] ;

		Vertex += 4 ;

		Vertex[ 0 ].x = LinePos_In_X[  3 ] ;		Vertex[ 0 ].y = LinePos_In_Y[ 0 ] ;
		Vertex[ 1 ].x = LinePos_Out_X[ 2 ] ;		Vertex[ 1 ].y = LinePos_In_Y[ 0 ] ;
		Vertex[ 2 ].x = LinePos_In_X[  3 ] ;		Vertex[ 2 ].y = LinePos_In_Y[ 3 ] ;
		Vertex[ 3 ].x = LinePos_Out_X[ 2 ] ;		Vertex[ 3 ].y = LinePos_In_Y[ 3 ] ;

		Vertex[ 0 ].u = Line_In_UV[ 0 ] ;			Vertex[ 0 ].v = Line_In_UV[ 0 ] ;
		Vertex[ 1 ].u = Line_In_UV[ 0 ] ;			Vertex[ 1 ].v = Line_In_UV[ 0 ] ;
		Vertex[ 2 ].u = Line_In_UV[ 0 ] ;			Vertex[ 2 ].v = Line_In_UV[ 0 ] ;
		Vertex[ 3 ].u = Line_In_UV[ 0 ] ;			Vertex[ 3 ].v = Line_In_UV[ 0 ] ;

		Vertex += 4 ;

		Vertex[ 0 ].x = LinePos_Out_X[ 1 ] ;		Vertex[ 0 ].y = LinePos_In_Y[ 0 ] ;
		Vertex[ 1 ].x = LinePos_In_X[  0 ] ;		Vertex[ 1 ].y = LinePos_In_Y[ 0 ] ;
		Vertex[ 2 ].x = LinePos_Out_X[ 1 ] ;		Vertex[ 2 ].y = LinePos_In_Y[ 3 ] ;
		Vertex[ 3 ].x = LinePos_In_X[  0 ] ;		Vertex[ 3 ].y = LinePos_In_Y[ 3 ] ;

		Vertex[ 0 ].u = Line_In_UV[ 0 ] ;			Vertex[ 0 ].v = Line_In_UV[ 0 ] ;
		Vertex[ 1 ].u = Line_In_UV[ 0 ] ;			Vertex[ 1 ].v = Line_In_UV[ 0 ] ;
		Vertex[ 2 ].u = Line_In_UV[ 0 ] ;			Vertex[ 2 ].v = Line_In_UV[ 0 ] ;
		Vertex[ 3 ].u = Line_In_UV[ 0 ] ;			Vertex[ 3 ].v = Line_In_UV[ 0 ] ;

		Vertex += 4 ;
	}

	Graphics_Draw_DrawSimpleQuadrangleGraphF( &Param ) ;

	if( OldDrawMode != -1 )
	{
		NS_SetDrawMode( OldDrawMode ) ;
	}

	Graphics_DrawSetting_SetDrawBrightToOneParam( OrigColor ) ;

	// �I��
	return 0 ;
}

// �l�p�`�̕`�悷��( �A���`�G�C���A�X�t�� )
static int DrawBoxFillAA( float x1, float y1, float x2, float y2, unsigned int Color )
{
	int OldDrawMode = -1 ;
	int GrHandle ;
	GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM Param ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX ParamVertex[ 9 * 4 ] ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *Vertex ;
	float LinePosX[ 4 ];
	float LinePosY[ 4 ];
	int i ;
	int j ;
	DWORD FColor ;
	DWORD OrigColor ;
	static float LineUV[ 4 ] = { 0.0f, 3.0f / LINE_TEX_SIZE, 5.0f / LINE_TEX_SIZE, 1.0f };

	if( x1 == x2 || y1 == y2 || x1 > x2 || y1 > y2 )
	{
		return 0 ;
	}

	if( NS_GetDrawFloatCoordType() == DX_DRAWFLOATCOORDTYPE_DIRECT3D9 )
	{
		x1 -= 0.5f ;
		y1 -= 0.5f ;
		x2 -= 0.5f ;
		y2 -= 0.5f ;
	}

	SETUP32BITCOLOR( Color, FColor ) ;
	OrigColor = GSYS.DrawSetting.bDrawBright ;
	Graphics_DrawSetting_SetDrawBrightToOneParam( FColor ) ;

	if( GSYS.DrawSetting.DrawMode != DX_DRAWMODE_BILINEAR )
	{
		OldDrawMode = GSYS.DrawSetting.DrawMode ;
		NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	}

	Param.Vertex        = ParamVertex ;
	Param.TransFlag     = TRUE ;
	Param.QuadrangleNum = 9 ;

	GrHandle = Graphics_Image_GetLineTexHandle( IS_PMA_DRAWBLENDMODE ) ;

	Param.GraphHandle = GrHandle ;

	LinePosX[ 0 ] = x1 - 2.0f ;
	LinePosX[ 1 ] = x1 + 1.0f ;
	LinePosX[ 2 ] = x2 - 1.0f ;
	LinePosX[ 3 ] = x2 + 2.0f ;

	LinePosY[ 0 ] = y1 - 2.0f ;
	LinePosY[ 1 ] = y1 + 1.0f ;
	LinePosY[ 2 ] = y2 - 1.0f ;
	LinePosY[ 3 ] = y2 + 2.0f ;

	Vertex = ParamVertex ;
	for( i = 0; i < 3 ; i++ )
	{
		for( j = 0; j < 3 ; j++ )
		{
			Vertex[ 0 ].x = LinePosX[ j + 1 ] ;	Vertex[ 0 ].y = LinePosY[ i + 0 ] ;
			Vertex[ 1 ].x = LinePosX[ j + 0 ] ;	Vertex[ 1 ].y = LinePosY[ i + 0 ] ;
			Vertex[ 2 ].x = LinePosX[ j + 1 ] ;	Vertex[ 2 ].y = LinePosY[ i + 1 ] ;
			Vertex[ 3 ].x = LinePosX[ j + 0 ] ;	Vertex[ 3 ].y = LinePosY[ i + 1 ] ;

			Vertex[ 0 ].u = LineUV[ j + 1 ] ;	Vertex[ 0 ].v = LineUV[ i + 0 ] ;
			Vertex[ 1 ].u = LineUV[ j + 0 ] ;	Vertex[ 1 ].v = LineUV[ i + 0 ] ;
			Vertex[ 2 ].u = LineUV[ j + 1 ] ;	Vertex[ 2 ].v = LineUV[ i + 1 ] ;
			Vertex[ 3 ].u = LineUV[ j + 0 ] ;	Vertex[ 3 ].v = LineUV[ i + 1 ] ;

			Vertex += 4 ;
		}
	}

	Graphics_Draw_DrawSimpleQuadrangleGraphF( &Param ) ;

	if( OldDrawMode != -1 )
	{
		NS_SetDrawMode( OldDrawMode ) ;
	}

	Graphics_DrawSetting_SetDrawBrightToOneParam( OrigColor ) ;

	// �I��
	return 0 ;
}

// �l�p�`�̕`�悷��( �A���`�G�C���A�X�t�� )
extern int NS_DrawBoxAA( float x1, float y1, float x2, float y2, unsigned int Color, int FillFlag, float LineThickness )
{
	if( FillFlag == FALSE )
	{
		return DrawBoxLineAA( x1, y1, x2, y2, Color, LineThickness ) ;
	}
	else
	{
		return DrawBoxFillAA( x1, y1, x2, y2, Color ) ;
	}
}

// ���g�̂���l�p��`��
#define SETDRAWRECTCODE\
	if( x1 < x2 ){ DrawRect.left = x1 ; DrawRect.right = x2; }\
	if( y1 < y2 ){ DrawRect.top = y1 ; DrawRect.bottom = y2; }\
\
	SETRECT( DrawRect, x1, y1, x2, y2 ) ;\
	DRAWRECT_DRAWAREA_CLIP

extern	int NS_DrawFillBox( int x1, int y1, int x2, int y2, unsigned int Color )
{
	int Ret = -1 ;
	int Flag ;
	
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;
	
	Flag = GSYS.Setting.ValidHardware ;

	CheckActiveState() ;

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawFillBox_PF( x1, y1, x2, y2, Color ),
		Graphics_Software_DrawFillBox(    x1, y1, x2, y2, Color ),
		SETDRAWRECTCODE,
		Ret,
		Flag
	)

	// �I��
	return Ret ;
}

// �l�p�`�̕`�� 
extern	int NS_DrawLineBox( int x1, int y1, int x2, int y2, unsigned int Color )
{
	int Ret = -1 ;
	int Flag ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;
	
	Flag = GSYS.Setting.ValidHardware ;

	CheckActiveState() ;

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawLineBox_PF( x1, y1, x2, y2, Color ),
		Graphics_Software_DrawLineBox(    x1, y1, x2, y2, Color ),
		SETDRAWRECTCODE,
		Ret,
		Flag
	)

	// �I��
	return Ret ;
}

#undef SETDRAWRECTCODE

// �~��`��
#define SETDRAWRECTCODE\
	SETRECT( DrawRect, x - r - 1, y - r - 1, x + r + 1, y + r + 1 )\
	DRAWRECT_DRAWAREA_CLIP
extern	int NS_DrawCircle( int x, int y, int r, unsigned int Color, int FillFlag, int Thickness )
{
	int Ret = -1 ;
	int Flag ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;
	
	Flag = GSYS.Setting.ValidHardware ;

	CheckActiveState() ;

	if( r < 0 ) return 0 ;

	// �`��
	if( Thickness == 1 )
	{
		DRAW_DEF(
			Graphics_Hardware_DrawCircle_PF( x, y, r, Color, FillFlag ),
			Graphics_Software_DrawCircle(    x, y, r, Color, FillFlag ),
			SETDRAWRECTCODE,
			Ret,
			Flag
		)
	}
	else
	{
		DRAW_DEF(
			Graphics_Hardware_DrawCircle_Thickness_PF( x, y, r, Color, Thickness ),
			Graphics_Software_DrawCircle_Thickness(    x, y, r, Color, Thickness ),
			SETDRAWRECTCODE,
			Ret,
			Flag
		)
	}

	// �I��
	return Ret ;
}
#undef SETDRAWRECTCODE

// �~��`�悷��( �A���`�G�C���A�X�t�� )
static int DrawCircleLineAA( float x, float y, float r, int posnum, unsigned int Color, float LineThickness )
{
	int OldDrawMode = -1 ;
	int GrHandle ;
	GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM Param ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX ParamVertex[ CIRCLE_MAX_POSNUM * 3 * 4 ] ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *Vertex ;
	float LinePos[ 4 ] ;
	int i ;
	int j ;
	DWORD FColor ;
	DWORD OrigColor ;
	float Sin, Cos ;
	static float LineUV[ 4 ] = { 0.0f, 3.0f / LINE_TEX_SIZE, 5.0f / LINE_TEX_SIZE, 1.0f };
	float PosX[ CIRCLE_MAX_POSNUM ][ 4 ];
	float PosY[ CIRCLE_MAX_POSNUM ][ 4 ];

	if( posnum > CIRCLE_MAX_POSNUM )
	{
		posnum = CIRCLE_MAX_POSNUM ;
	}

	if( NS_GetDrawFloatCoordType() == DX_DRAWFLOATCOORDTYPE_DIRECT3D9 )
	{
		x -= 0.5f ;
		y -= 0.5f ;
	}

	SETUP32BITCOLOR( Color, FColor ) ;
	OrigColor = GSYS.DrawSetting.bDrawBright ;
	Graphics_DrawSetting_SetDrawBrightToOneParam( FColor ) ;

	if( GSYS.DrawSetting.DrawMode != DX_DRAWMODE_BILINEAR )
	{
		OldDrawMode = GSYS.DrawSetting.DrawMode ;
		NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	}

	Param.Vertex      = ParamVertex ;
	Param.TransFlag   = TRUE ;
	if( LineThickness <= 1.0f )
	{
		static float LineUV_2[ 2 ] = { 0.0f, 4.0f / LINE_TEX_SIZE } ;

		Param.QuadrangleNum = posnum ;

		GrHandle = Graphics_Image_GetLineOneThicknessTexHandle( IS_PMA_DRAWBLENDMODE ) ;

		Param.GraphHandle = GrHandle ;

		LineThickness /= 2.0f ;
		LinePos[ 0 ] = r + LineThickness + 1.5f ;
		LinePos[ 1 ] = r - LineThickness - 2.5f ;

		for( i = 0; i < posnum + 1; i++ )
		{
			_TABLE_SINCOS( DX_TWO_PI_F * i / posnum, &Sin, &Cos ) ;

			PosX[ i ][ 0 ] = LinePos[ 0 ] * Cos + x ;
			PosX[ i ][ 1 ] = LinePos[ 1 ] * Cos + x ;

			PosY[ i ][ 0 ] = LinePos[ 0 ] * Sin + y ;
			PosY[ i ][ 1 ] = LinePos[ 1 ] * Sin + y ;
		}

		Vertex = ParamVertex ;
		for( i = 0; i < posnum ; i++ )
		{
			Vertex[ 0 ].x = PosX[ i + 1 ][ 0 ] ;	Vertex[ 0 ].y = PosY[ i + 1 ][ 0 ] ;
			Vertex[ 1 ].x = PosX[ i + 0 ][ 0 ] ;	Vertex[ 1 ].y = PosY[ i + 0 ][ 0 ] ;
			Vertex[ 2 ].x = PosX[ i + 1 ][ 1 ] ;	Vertex[ 2 ].y = PosY[ i + 1 ][ 1 ] ;
			Vertex[ 3 ].x = PosX[ i + 0 ][ 1 ] ;	Vertex[ 3 ].y = PosY[ i + 0 ][ 1 ] ;

			Vertex[ 0 ].u = LineUV_2[ 0 ] ;	Vertex[ 0 ].v = LineUV_2[ 1 ] ;
			Vertex[ 1 ].u = LineUV_2[ 0 ] ;	Vertex[ 1 ].v = LineUV_2[ 1 ] ;
			Vertex[ 2 ].u = LineUV_2[ 1 ] ;	Vertex[ 2 ].v = LineUV_2[ 1 ] ;
			Vertex[ 3 ].u = LineUV_2[ 1 ] ;	Vertex[ 3 ].v = LineUV_2[ 1 ] ;

			Vertex += 4 ;
		}
	}
	else
	{
		Param.QuadrangleNum = posnum * 3 ;

		GrHandle = Graphics_Image_GetLineTexHandle( IS_PMA_DRAWBLENDMODE ) ;

		Param.GraphHandle = GrHandle ;

		LineThickness /= 2.0f ;
		LinePos[ 0 ] = r + LineThickness + 1.5f ;
		LinePos[ 1 ] = r + LineThickness - 1.5f ;
		LinePos[ 2 ] = r - LineThickness + 0.5f ;
		LinePos[ 3 ] = r - LineThickness - 2.5f ;

		for( i = 0; i < posnum + 1; i++ )
		{
			_TABLE_SINCOS( DX_TWO_PI_F * i / posnum, &Sin, &Cos ) ;

			PosX[ i ][ 0 ] = LinePos[ 0 ] * Cos + x ;
			PosX[ i ][ 1 ] = LinePos[ 1 ] * Cos + x ;
			PosX[ i ][ 2 ] = LinePos[ 2 ] * Cos + x ;
			PosX[ i ][ 3 ] = LinePos[ 3 ] * Cos + x ;

			PosY[ i ][ 0 ] = LinePos[ 0 ] * Sin + y ;
			PosY[ i ][ 1 ] = LinePos[ 1 ] * Sin + y ;
			PosY[ i ][ 2 ] = LinePos[ 2 ] * Sin + y ;
			PosY[ i ][ 3 ] = LinePos[ 3 ] * Sin + y ;
		}

		Vertex = ParamVertex ;
		for( i = 0; i < posnum ; i++ )
		{
			for( j = 0; j < 3 ; j++ )
			{
				Vertex[ 0 ].x = PosX[ i + 1 ][ j + 0 ] ;	Vertex[ 0 ].y = PosY[ i + 1 ][ j + 0 ] ;
				Vertex[ 1 ].x = PosX[ i + 0 ][ j + 0 ] ;	Vertex[ 1 ].y = PosY[ i + 0 ][ j + 0 ] ;
				Vertex[ 2 ].x = PosX[ i + 1 ][ j + 1 ] ;	Vertex[ 2 ].y = PosY[ i + 1 ][ j + 1 ] ;
				Vertex[ 3 ].x = PosX[ i + 0 ][ j + 1 ] ;	Vertex[ 3 ].y = PosY[ i + 0 ][ j + 1 ] ;

				Vertex[ 0 ].u = LineUV[ 1 ] ;	Vertex[ 0 ].v = LineUV[ j + 0 ] ;
				Vertex[ 1 ].u = LineUV[ 1 ] ;	Vertex[ 1 ].v = LineUV[ j + 0 ] ;
				Vertex[ 2 ].u = LineUV[ 1 ] ;	Vertex[ 2 ].v = LineUV[ j + 1 ] ;
				Vertex[ 3 ].u = LineUV[ 1 ] ;	Vertex[ 3 ].v = LineUV[ j + 1 ] ;

				Vertex += 4 ;
			}
		}
	}

	Graphics_Draw_DrawSimpleQuadrangleGraphF( &Param ) ;

	if( OldDrawMode != -1 )
	{
		NS_SetDrawMode( OldDrawMode ) ;
	}

	Graphics_DrawSetting_SetDrawBrightToOneParam( OrigColor ) ;

	// �I��
	return 0 ;
}

// �~��`�悷��( �A���`�G�C���A�X�t�� )
static int DrawCircleFillAA( float x, float y, float r, int posnum, unsigned int Color )
{
	int OldDrawMode = -1 ;
	int GrHandle ;
	GRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM Param ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX ParamVertex[ CIRCLE_MAX_POSNUM * ( 6 + 3 ) ] ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *Vertex ;
	float LinePos[ 2 ] ;
	int i ;
	DWORD FColor ;
	DWORD OrigColor ;
	float Sin, Cos ;
	static float LineUV[ 4 ] = { 0.0f, 3.0f / LINE_TEX_SIZE, 5.0f / LINE_TEX_SIZE, 1.0f };
	float PosX[ CIRCLE_MAX_POSNUM ][ 4 ];
	float PosY[ CIRCLE_MAX_POSNUM ][ 4 ];

	if( posnum > CIRCLE_MAX_POSNUM )
	{
		posnum = CIRCLE_MAX_POSNUM ;
	}

	if( NS_GetDrawFloatCoordType() == DX_DRAWFLOATCOORDTYPE_DIRECT3D9 )
	{
		x -= 0.5f ;
		y -= 0.5f ;
	}

	SETUP32BITCOLOR( Color, FColor ) ;
	OrigColor = GSYS.DrawSetting.bDrawBright ;
	Graphics_DrawSetting_SetDrawBrightToOneParam( FColor ) ;

	if( GSYS.DrawSetting.DrawMode != DX_DRAWMODE_BILINEAR )
	{
		OldDrawMode = GSYS.DrawSetting.DrawMode ;
		NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	}

	Param.Vertex      = ParamVertex ;
	Param.TransFlag   = TRUE ;
	Param.TriangleNum = posnum * 3 ;

	GrHandle = Graphics_Image_GetLineTexHandle( IS_PMA_DRAWBLENDMODE ) ;

	Param.GraphHandle = GrHandle ;

	LinePos[ 0 ] = r + 2.0f ;
	LinePos[ 1 ] = r - 1.0f ;

	for( i = 0; i < posnum + 1; i++ )
	{
		_TABLE_SINCOS( DX_TWO_PI_F * i / posnum, &Sin, &Cos ) ;

		PosX[ i ][ 0 ] = LinePos[ 0 ] * Cos + x ;
		PosX[ i ][ 1 ] = LinePos[ 1 ] * Cos + x ;

		PosY[ i ][ 0 ] = LinePos[ 0 ] * Sin + y ;
		PosY[ i ][ 1 ] = LinePos[ 1 ] * Sin + y ;
	}

	Vertex = ParamVertex ;
	for( i = 0; i < posnum ; i++ )
	{
		Vertex[ 0 ].x = PosX[ i + 1 ][ 0 ] ;	Vertex[ 0 ].y = PosY[ i + 1 ][ 0 ] ;
		Vertex[ 1 ].x = PosX[ i + 0 ][ 0 ] ;	Vertex[ 1 ].y = PosY[ i + 0 ][ 0 ] ;
		Vertex[ 2 ].x = PosX[ i + 1 ][ 1 ] ;	Vertex[ 2 ].y = PosY[ i + 1 ][ 1 ] ;
		Vertex[ 3 ].x = PosX[ i + 0 ][ 1 ] ;	Vertex[ 3 ].y = PosY[ i + 0 ][ 1 ] ;
		Vertex[ 4 ].x = PosX[ i + 1 ][ 1 ] ;	Vertex[ 4 ].y = PosY[ i + 1 ][ 1 ] ;
		Vertex[ 5 ].x = PosX[ i + 0 ][ 0 ] ;	Vertex[ 5 ].y = PosY[ i + 0 ][ 0 ] ;

		Vertex[ 0 ].u = LineUV[ 1 ] ;	Vertex[ 0 ].v = LineUV[ 0 ] ;
		Vertex[ 1 ].u = LineUV[ 1 ] ;	Vertex[ 1 ].v = LineUV[ 0 ] ;
		Vertex[ 2 ].u = LineUV[ 1 ] ;	Vertex[ 2 ].v = LineUV[ 1 ] ;
		Vertex[ 3 ].u = LineUV[ 1 ] ;	Vertex[ 3 ].v = LineUV[ 1 ] ;
		Vertex[ 4 ].u = LineUV[ 1 ] ;	Vertex[ 4 ].v = LineUV[ 1 ] ;
		Vertex[ 5 ].u = LineUV[ 1 ] ;	Vertex[ 5 ].v = LineUV[ 0 ] ;

		Vertex += 6 ;


		Vertex[ 0 ].x = PosX[ i + 1 ][ 1 ] ;	Vertex[ 0 ].y = PosY[ i + 1 ][ 1 ] ;
		Vertex[ 1 ].x = PosX[ i + 0 ][ 1 ] ;	Vertex[ 1 ].y = PosY[ i + 0 ][ 1 ] ;
		Vertex[ 2 ].x = x ;						Vertex[ 2 ].y = y ;

		Vertex[ 0 ].u = LineUV[ 1 ] ;	Vertex[ 0 ].v = LineUV[ 1 ] ;
		Vertex[ 1 ].u = LineUV[ 1 ] ;	Vertex[ 1 ].v = LineUV[ 1 ] ;
		Vertex[ 2 ].u = LineUV[ 1 ] ;	Vertex[ 2 ].v = LineUV[ 1 ] ;

		Vertex += 3 ;
	}

	Graphics_Draw_DrawSimpleTriangleGraphF( &Param ) ;

	if( OldDrawMode != -1 )
	{
		NS_SetDrawMode( OldDrawMode ) ;
	}

	Graphics_DrawSetting_SetDrawBrightToOneParam( OrigColor ) ;

	// �I��
	return 0 ;
}

// �~��`�悷��( �A���`�G�C���A�X�t�� )
extern int NS_DrawCircleAA( float x, float y, float r, int posnum, unsigned int Color, int FillFlag, float LineThickness )
{
	if( FillFlag == FALSE )
	{
		return DrawCircleLineAA( x, y, r, posnum, Color, LineThickness ) ;
	}
	else
	{
		return DrawCircleFillAA( x, y, r, posnum, Color ) ;
	}
}

// �ȉ~��`��
#define SETDRAWRECTCODE\
	SETRECT( DrawRect, x - rx - 1, y - ry - 1, x + rx + 1, y + ry + 1 )\
	DRAWRECT_DRAWAREA_CLIP
extern	int NS_DrawOval( int x, int y, int rx, int ry, unsigned int Color, int FillFlag, int Thickness )
{
	int Ret = -1 ;
	int Flag ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	Flag = GSYS.Setting.ValidHardware ;

	CheckActiveState() ;

	if( rx < 0 || ry < 0 ) return 0 ;

	// �`��
	if( Thickness == 1 )
	{
		DRAW_DEF(
			Graphics_Hardware_DrawOval_PF( x, y, rx, ry, Color, FillFlag ),
			Graphics_Software_DrawOval(    x, y, rx, ry, Color, FillFlag ),
			SETDRAWRECTCODE,
			Ret,
			Flag
		)
	}
	else
	{
		DRAW_DEF(
			Graphics_Hardware_DrawOval_Thickness_PF( x, y, rx, ry, Color, Thickness ),
			Graphics_Software_DrawOval_Thickness(    x, y, rx, ry, Color, Thickness ),
			SETDRAWRECTCODE,
			Ret,
			Flag
		)
	}

	// �I��
	return Ret ;
}
#undef SETDRAWRECTCODE

// �ȉ~��`�悷��( �A���`�G�C���A�X�t�� )
static int DrawOvalLineAA( float x, float y, float rx, float ry, int posnum, unsigned int Color, float LineThickness )
{
	int OldDrawMode = -1 ;
	int GrHandle ;
	GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM Param ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX ParamVertex[ CIRCLE_MAX_POSNUM * 3 * 4 ] ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *Vertex ;
	float LinePosX[ 4 ] ;
	float LinePosY[ 4 ] ;
	int i ;
	int j ;
	DWORD FColor ;
	DWORD OrigColor ;
	float Sin, Cos ;
	static float LineUV[ 4 ] = { 0.0f, 3.0f / LINE_TEX_SIZE, 5.0f / LINE_TEX_SIZE, 1.0f };
	float PosX[ CIRCLE_MAX_POSNUM ][ 4 ];
	float PosY[ CIRCLE_MAX_POSNUM ][ 4 ];

	if( posnum > CIRCLE_MAX_POSNUM )
	{
		posnum = CIRCLE_MAX_POSNUM ;
	}

	if( NS_GetDrawFloatCoordType() == DX_DRAWFLOATCOORDTYPE_DIRECT3D9 )
	{
		x -= 0.5f ;
		y -= 0.5f ;
	}

	SETUP32BITCOLOR( Color, FColor ) ;
	OrigColor = GSYS.DrawSetting.bDrawBright ;
	Graphics_DrawSetting_SetDrawBrightToOneParam( FColor ) ;

	if( GSYS.DrawSetting.DrawMode != DX_DRAWMODE_BILINEAR )
	{
		OldDrawMode = GSYS.DrawSetting.DrawMode ;
		NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	}

	Param.Vertex      = ParamVertex ;
	Param.TransFlag   = TRUE ;
	if( LineThickness <= 1.0f )
	{
		static float LineUV_2[ 2 ] = { 0.0f, 4.0f / LINE_TEX_SIZE } ;

		Param.QuadrangleNum = posnum ;

		GrHandle = Graphics_Image_GetLineOneThicknessTexHandle( IS_PMA_DRAWBLENDMODE ) ;

		Param.GraphHandle = GrHandle ;

		LineThickness /= 2.0f ;
		LinePosX[ 0 ] = rx + LineThickness + 1.5f ;
		LinePosX[ 1 ] = rx - LineThickness - 2.5f ;
		LinePosY[ 0 ] = ry + LineThickness + 1.5f ;
		LinePosY[ 1 ] = ry - LineThickness - 2.5f ;

		for( i = 0; i < posnum + 1; i++ )
		{
			_TABLE_SINCOS( DX_TWO_PI_F * i / posnum, &Sin, &Cos ) ;

			PosX[ i ][ 0 ] = LinePosX[ 0 ] * Cos + x ;
			PosX[ i ][ 1 ] = LinePosX[ 1 ] * Cos + x ;

			PosY[ i ][ 0 ] = LinePosY[ 0 ] * Sin + y ;
			PosY[ i ][ 1 ] = LinePosY[ 1 ] * Sin + y ;
		}

		Vertex = ParamVertex ;
		for( i = 0; i < posnum ; i++ )
		{
			Vertex[ 0 ].x = PosX[ i + 1 ][ 0 ] ;	Vertex[ 0 ].y = PosY[ i + 1 ][ 0 ] ;
			Vertex[ 1 ].x = PosX[ i + 0 ][ 0 ] ;	Vertex[ 1 ].y = PosY[ i + 0 ][ 0 ] ;
			Vertex[ 2 ].x = PosX[ i + 1 ][ 1 ] ;	Vertex[ 2 ].y = PosY[ i + 1 ][ 1 ] ;
			Vertex[ 3 ].x = PosX[ i + 0 ][ 1 ] ;	Vertex[ 3 ].y = PosY[ i + 0 ][ 1 ] ;

			Vertex[ 0 ].u = LineUV_2[ 0 ] ;	Vertex[ 0 ].v = LineUV_2[ 1 ] ;
			Vertex[ 1 ].u = LineUV_2[ 0 ] ;	Vertex[ 1 ].v = LineUV_2[ 1 ] ;
			Vertex[ 2 ].u = LineUV_2[ 1 ] ;	Vertex[ 2 ].v = LineUV_2[ 1 ] ;
			Vertex[ 3 ].u = LineUV_2[ 1 ] ;	Vertex[ 3 ].v = LineUV_2[ 1 ] ;

			Vertex += 4 ;
		}
	}
	else
	{
		Param.QuadrangleNum = posnum * 3 ;

		GrHandle = Graphics_Image_GetLineTexHandle( IS_PMA_DRAWBLENDMODE ) ;

		Param.GraphHandle = GrHandle ;

		LineThickness /= 2.0f ;
		LinePosX[ 0 ] = rx + LineThickness + 1.5f ;
		LinePosX[ 1 ] = rx + LineThickness - 1.5f ;
		LinePosX[ 2 ] = rx - LineThickness + 0.5f ;
		LinePosX[ 3 ] = rx - LineThickness - 2.5f ;
		LinePosY[ 0 ] = ry + LineThickness + 1.5f ;
		LinePosY[ 1 ] = ry + LineThickness - 1.5f ;
		LinePosY[ 2 ] = ry - LineThickness + 0.5f ;
		LinePosY[ 3 ] = ry - LineThickness - 2.5f ;

		for( i = 0; i < posnum + 1; i++ )
		{
			_TABLE_SINCOS( DX_TWO_PI_F * i / posnum, &Sin, &Cos ) ;

			PosX[ i ][ 0 ] = LinePosX[ 0 ] * Cos + x ;
			PosX[ i ][ 1 ] = LinePosX[ 1 ] * Cos + x ;
			PosX[ i ][ 2 ] = LinePosX[ 2 ] * Cos + x ;
			PosX[ i ][ 3 ] = LinePosX[ 3 ] * Cos + x ;

			PosY[ i ][ 0 ] = LinePosY[ 0 ] * Sin + y ;
			PosY[ i ][ 1 ] = LinePosY[ 1 ] * Sin + y ;
			PosY[ i ][ 2 ] = LinePosY[ 2 ] * Sin + y ;
			PosY[ i ][ 3 ] = LinePosY[ 3 ] * Sin + y ;
		}

		Vertex = ParamVertex ;
		for( i = 0; i < posnum ; i++ )
		{
			for( j = 0; j < 3 ; j++ )
			{
				Vertex[ 0 ].x = PosX[ i + 1 ][ j + 0 ] ;	Vertex[ 0 ].y = PosY[ i + 1 ][ j + 0 ] ;
				Vertex[ 1 ].x = PosX[ i + 0 ][ j + 0 ] ;	Vertex[ 1 ].y = PosY[ i + 0 ][ j + 0 ] ;
				Vertex[ 2 ].x = PosX[ i + 1 ][ j + 1 ] ;	Vertex[ 2 ].y = PosY[ i + 1 ][ j + 1 ] ;
				Vertex[ 3 ].x = PosX[ i + 0 ][ j + 1 ] ;	Vertex[ 3 ].y = PosY[ i + 0 ][ j + 1 ] ;

				Vertex[ 0 ].u = LineUV[ 1 ] ;	Vertex[ 0 ].v = LineUV[ j + 0 ] ;
				Vertex[ 1 ].u = LineUV[ 1 ] ;	Vertex[ 1 ].v = LineUV[ j + 0 ] ;
				Vertex[ 2 ].u = LineUV[ 1 ] ;	Vertex[ 2 ].v = LineUV[ j + 1 ] ;
				Vertex[ 3 ].u = LineUV[ 1 ] ;	Vertex[ 3 ].v = LineUV[ j + 1 ] ;

				Vertex += 4 ;
			}
		}
	}

	Graphics_Draw_DrawSimpleQuadrangleGraphF( &Param ) ;

	if( OldDrawMode != -1 )
	{
		NS_SetDrawMode( OldDrawMode ) ;
	}

	Graphics_DrawSetting_SetDrawBrightToOneParam( OrigColor ) ;

	// �I��
	return 0 ;
}

// �ȉ~��`�悷��( �A���`�G�C���A�X�t�� )
static int DrawOvalFillAA( float x, float y, float rx, float ry, int posnum, unsigned int Color )
{
	int OldDrawMode = -1 ;
	int GrHandle ;
	GRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM Param ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX ParamVertex[ CIRCLE_MAX_POSNUM * ( 6 + 3 ) ] ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *Vertex ;
	float LinePosX[ 2 ] ;
	float LinePosY[ 2 ] ;
	int i ;
	DWORD FColor ;
	DWORD OrigColor ;
	float Sin, Cos ;
	static float LineUV[ 4 ] = { 0.0f, 3.0f / LINE_TEX_SIZE, 5.0f / LINE_TEX_SIZE, 1.0f };
	float PosX[ CIRCLE_MAX_POSNUM ][ 4 ];
	float PosY[ CIRCLE_MAX_POSNUM ][ 4 ];

	if( posnum > CIRCLE_MAX_POSNUM )
	{
		posnum = CIRCLE_MAX_POSNUM ;
	}

	if( NS_GetDrawFloatCoordType() == DX_DRAWFLOATCOORDTYPE_DIRECT3D9 )
	{
		x -= 0.5f ;
		y -= 0.5f ;
	}

	SETUP32BITCOLOR( Color, FColor ) ;
	OrigColor = GSYS.DrawSetting.bDrawBright ;
	Graphics_DrawSetting_SetDrawBrightToOneParam( FColor ) ;

	if( GSYS.DrawSetting.DrawMode != DX_DRAWMODE_BILINEAR )
	{
		OldDrawMode = GSYS.DrawSetting.DrawMode ;
		NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	}

	Param.Vertex      = ParamVertex ;
	Param.TransFlag   = TRUE ;
	Param.TriangleNum = posnum * 3 ;

	GrHandle = Graphics_Image_GetLineTexHandle( IS_PMA_DRAWBLENDMODE ) ;

	Param.GraphHandle = GrHandle ;

	LinePosX[ 0 ] = rx + 2.0f ;
	LinePosX[ 1 ] = rx - 1.0f ;
	LinePosY[ 0 ] = ry + 2.0f ;
	LinePosY[ 1 ] = ry - 1.0f ;

	for( i = 0; i < posnum + 1; i++ )
	{
		_TABLE_SINCOS( DX_TWO_PI_F * i / posnum, &Sin, &Cos ) ;

		PosX[ i ][ 0 ] = LinePosX[ 0 ] * Cos + x ;
		PosX[ i ][ 1 ] = LinePosX[ 1 ] * Cos + x ;

		PosY[ i ][ 0 ] = LinePosY[ 0 ] * Sin + y ;
		PosY[ i ][ 1 ] = LinePosY[ 1 ] * Sin + y ;
	}

	Vertex = ParamVertex ;
	for( i = 0; i < posnum ; i++ )
	{
		Vertex[ 0 ].x = PosX[ i + 1 ][ 0 ] ;	Vertex[ 0 ].y = PosY[ i + 1 ][ 0 ] ;
		Vertex[ 1 ].x = PosX[ i + 0 ][ 0 ] ;	Vertex[ 1 ].y = PosY[ i + 0 ][ 0 ] ;
		Vertex[ 2 ].x = PosX[ i + 1 ][ 1 ] ;	Vertex[ 2 ].y = PosY[ i + 1 ][ 1 ] ;
		Vertex[ 3 ].x = PosX[ i + 0 ][ 1 ] ;	Vertex[ 3 ].y = PosY[ i + 0 ][ 1 ] ;
		Vertex[ 4 ].x = PosX[ i + 1 ][ 1 ] ;	Vertex[ 4 ].y = PosY[ i + 1 ][ 1 ] ;
		Vertex[ 5 ].x = PosX[ i + 0 ][ 0 ] ;	Vertex[ 5 ].y = PosY[ i + 0 ][ 0 ] ;

		Vertex[ 0 ].u = LineUV[ 1 ] ;	Vertex[ 0 ].v = LineUV[ 0 ] ;
		Vertex[ 1 ].u = LineUV[ 1 ] ;	Vertex[ 1 ].v = LineUV[ 0 ] ;
		Vertex[ 2 ].u = LineUV[ 1 ] ;	Vertex[ 2 ].v = LineUV[ 1 ] ;
		Vertex[ 3 ].u = LineUV[ 1 ] ;	Vertex[ 3 ].v = LineUV[ 1 ] ;
		Vertex[ 4 ].u = LineUV[ 1 ] ;	Vertex[ 4 ].v = LineUV[ 1 ] ;
		Vertex[ 5 ].u = LineUV[ 1 ] ;	Vertex[ 5 ].v = LineUV[ 0 ] ;

		Vertex += 6 ;


		Vertex[ 0 ].x = PosX[ i + 1 ][ 1 ] ;	Vertex[ 0 ].y = PosY[ i + 1 ][ 1 ] ;
		Vertex[ 1 ].x = PosX[ i + 0 ][ 1 ] ;	Vertex[ 1 ].y = PosY[ i + 0 ][ 1 ] ;
		Vertex[ 2 ].x = x ;						Vertex[ 2 ].y = y ;

		Vertex[ 0 ].u = LineUV[ 1 ] ;	Vertex[ 0 ].v = LineUV[ 1 ] ;
		Vertex[ 1 ].u = LineUV[ 1 ] ;	Vertex[ 1 ].v = LineUV[ 1 ] ;
		Vertex[ 2 ].u = LineUV[ 1 ] ;	Vertex[ 2 ].v = LineUV[ 1 ] ;

		Vertex += 3 ;
	}

	Graphics_Draw_DrawSimpleTriangleGraphF( &Param ) ;

	if( OldDrawMode != -1 )
	{
		NS_SetDrawMode( OldDrawMode ) ;
	}

	Graphics_DrawSetting_SetDrawBrightToOneParam( OrigColor ) ;

	// �I��
	return 0 ;
}

// �ȉ~��`�悷��( �A���`�G�C���A�X�t�� )
extern int NS_DrawOvalAA( float x, float y, float rx, float ry, int posnum, unsigned int Color, int FillFlag, float LineThickness )
{
	if( FillFlag == FALSE )
	{
		return DrawOvalLineAA( x, y, rx, ry, posnum, Color, LineThickness ) ;
	}
	else
	{
		return DrawOvalFillAA( x, y, rx, ry, posnum, Color ) ;
	}
}

// �O�p�`�̕`��
#define SETDRAWRECTCODE\
	int x[ 3 ], y[ 3 ], MaxX, MaxY, MinX, MinY ;\
	int i, ix, iy ;\
\
	x[ 0 ] = x1 ; x[ 1 ] = x2 ; x[ 2 ] = x3 ;\
	y[ 0 ] = y1 ; y[ 1 ] = y2 ; y[ 2 ] = y3 ;\
\
	MaxX = 0 ; MaxY = 0 ;\
	MinX = 0xffffff ; MinY = 0xffffff ;\
\
	for( i = 0 ; i < 3 ; i ++ )\
	{\
		ix = x[ i ] ; iy = y[ i ] ;\
		if( ix > MaxX ) MaxX = ix ; if( iy > MaxY ) MaxY = iy ;\
		if( ix < MinX ) MinX = ix ; if( iy < MinY ) MinY = iy ;\
	}\
\
	SETRECT( DrawRect, MinX, MinY, MaxX, MaxY ) ;\
	DRAWRECT_DRAWAREA_CLIP

extern	int NS_DrawTriangle( int x1, int y1, int x2, int y2, int x3, int y3, unsigned int Color, int FillFlag )
{
	int Ret = -1 ;
	int Flag ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;
	
	Flag = GSYS.Setting.ValidHardware ;

	CheckActiveState() ;

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawTriangle_PF( x1, y1, x2, y2, x3, y3, Color, FillFlag ),
		Graphics_Software_DrawTriangle(    x1, y1, x2, y2, x3, y3, Color, FillFlag ),
		SETDRAWRECTCODE,
		Ret,
		Flag
	)

	// �I��
	return Ret ;
}

#undef SETDRAWRECTCODE

// �O�p�`�̕`�悷��( �A���`�G�C���A�X�t�� )
static int DrawTriangleLineAA( float x1, float y1, float x2, float y2, float x3, float y3, unsigned int Color, float LineThickness )
{
	int OldDrawMode = -1 ;
	int GrHandle ;
	GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM Param ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX ParamVertex[ ( 3 * 12 ) * 4 ] ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *Vertex ;
	VECTOR V1Dir, V1Pos, V1Norm, V1Line[ 4 ] ;
	VECTOR V2Dir, V2Pos, V2Norm, V2Line[ 4 ] ;
	VECTOR V3Dir, V3Pos, V3Norm, V3Line[ 4 ] ;
	VECTOR V12Dir ;
	VECTOR V13Dir ;
	VECTOR VPosD[ 3 ][ 4 ][ 4 ] ;
	float Distance ;
	VECTOR TempPos ;
	float P1P2X ;
	float P1P2Y ;
	float P1P3X ;
	float P1P3Y ;
	float TempF ;
	float tmp1 ;
	float tmp2 ;
	float tmp3 ;
	float tmp4 ;
	int i ;
	int n ;
	DWORD FColor ;
	DWORD OrigColor ;
	int NormalDraw = FALSE ;
	float SizeSq ;
	static float Line_Out_UV[ 4 ] = { 0.0f,                    4.0f / LINEBOX_TEX_SIZE, 12.0f / LINEBOX_TEX_SIZE, 1.0f } ;
	static float Line_In_UV[ 4 ]  = { 5.0f / LINEBOX_TEX_SIZE, 8.0f / LINEBOX_TEX_SIZE, 8.0f  / LINEBOX_TEX_SIZE, 11.0f / LINEBOX_TEX_SIZE } ;

	if( NS_GetDrawFloatCoordType() == DX_DRAWFLOATCOORDTYPE_DIRECT3D9 )
	{
		x1 -= 0.5f ;
		y1 -= 0.5f ;
		x2 -= 0.5f ;
		y2 -= 0.5f ;
		x3 -= 0.5f ;
		y3 -= 0.5f ;
	}

	SETUP32BITCOLOR( Color, FColor ) ;
	OrigColor = GSYS.DrawSetting.bDrawBright ;
	Graphics_DrawSetting_SetDrawBrightToOneParam( FColor ) ;

	if( GSYS.DrawSetting.DrawMode != DX_DRAWMODE_BILINEAR )
	{
		OldDrawMode = GSYS.DrawSetting.DrawMode ;
		NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	}

	Param.Vertex      = ParamVertex ;
	Param.TransFlag   = TRUE ;
	if( LineThickness <= 1.0f )
	{
		Param.QuadrangleNum = 6 ;

		GrHandle = Graphics_Image_GetLineBoxOneThicknessTexHandle( IS_PMA_DRAWBLENDMODE ) ;

		Param.GraphHandle = GrHandle ;

		LineThickness /= 2.0f ;
		tmp1 = LineThickness + 1.5f ;
		tmp2 = LineThickness - 2.5f ;

		P1P2X = x2 - x1 ;
		P1P2Y = y2 - y1 ;

		P1P3X = x3 - x1 ;
		P1P3Y = y3 - y1 ;

		if( P1P2X * P1P3Y - P1P2Y * P1P3X < 0.0f )
		{
			TempF = x2 ; x2 = x3 ; x3 = TempF ;
			TempF = y2 ; y2 = y3 ; y3 = TempF ;
		}

		V1Pos.x  = x1 ;			V1Pos.y  = y1 ;			V1Pos.z  = 0.0f ;
		V1Dir.x  = x2 - x1 ;	V1Dir.y  = y2 - y1 ;	V1Dir.z  = 0.0f ;
		V12Dir = V1Dir;
		Distance = V1Dir.x * V1Dir.x + V1Dir.y * V1Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V1Dir.x /= Distance ;
		V1Dir.y /= Distance ;
		V1Norm.x =  V1Dir.y ;
		V1Norm.y = -V1Dir.x ;
		V1Norm.z =  0.0f ;
		TempPos = VAdd( V1Pos, VScale( V1Norm, tmp1 ) ) ;
		V1Line[ 0 ].x =  V1Dir.y ;
		V1Line[ 0 ].y = -V1Dir.x ;
		V1Line[ 0 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;
		TempPos = VAdd( V1Pos, VScale( V1Norm, tmp2 ) ) ;
		V1Line[ 1 ].x =  V1Dir.y ;
		V1Line[ 1 ].y = -V1Dir.x ;
		V1Line[ 1 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;

		V2Pos.x  = x2 ;			V2Pos.y  = y2 ;			V2Pos.z  = 0.0f ;
		V2Dir.x  = x3 - x2 ;	V2Dir.y  = y3 - y2 ;	V2Dir.z  = 0.0f ;
		Distance = V2Dir.x * V2Dir.x + V2Dir.y * V2Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V2Dir.x /= Distance ;
		V2Dir.y /= Distance ;
		V2Norm.x =  V2Dir.y ;
		V2Norm.y = -V2Dir.x ;
		V2Norm.z =  0.0f ;
		TempPos = VAdd( V2Pos, VScale( V2Norm, tmp1 ) ) ;
		V2Line[ 0 ].x =  V2Dir.y ;
		V2Line[ 0 ].y = -V2Dir.x ;
		V2Line[ 0 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;
		TempPos = VAdd( V2Pos, VScale( V2Norm, tmp2 ) ) ;
		V2Line[ 1 ].x =  V2Dir.y ;
		V2Line[ 1 ].y = -V2Dir.x ;
		V2Line[ 1 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;

		V13Dir.x = x3 - x1 ;    V13Dir.y = y3 - y1 ;	V13Dir.z = 0.0f ;
		SizeSq = VSquareSize( VCross( V12Dir, V13Dir ) ) ;
		if( SizeSq / 2.0f < 7500.0f )
		{
			NormalDraw = TRUE ;
			goto END ;
		}

		V3Pos.x  = x3 ;			V3Pos.y  = y3 ;			V3Pos.z  = 0.0f ;
		V3Dir.x  = x1 - x3 ;	V3Dir.y  = y1 - y3 ;	V3Dir.z  = 0.0f ;
		Distance = V3Dir.x * V3Dir.x + V3Dir.y * V3Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V3Dir.x /= Distance ;
		V3Dir.y /= Distance ;
		V3Norm.x =  V3Dir.y ;
		V3Norm.y = -V3Dir.x ;
		V3Norm.z =  0.0f ;
		TempPos = VAdd( V3Pos, VScale( V3Norm, tmp1 ) ) ;
		V3Line[ 0 ].x =  V3Dir.y ;
		V3Line[ 0 ].y = -V3Dir.x ;
		V3Line[ 0 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;
		TempPos = VAdd( V3Pos, VScale( V3Norm, tmp2 ) ) ;
		V3Line[ 1 ].x =  V3Dir.y ;
		V3Line[ 1 ].y = -V3Dir.x ;
		V3Line[ 1 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;


		VPosD[ 0 ][ 0 ][ 0 ].x = ( V3Line[ 0 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V3Line[ 0 ].y ) / ( V1Line[ 0 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 0 ].y = ( V1Line[ 0 ].x * V3Line[ 0 ].z - V1Line[ 0 ].z * V3Line[ 0 ].x ) / ( V1Line[ 0 ].y * V3Line[ 0 ].x - V1Line[ 0 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 1 ].x = ( V3Line[ 1 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V3Line[ 1 ].y ) / ( V1Line[ 0 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 1 ].y = ( V1Line[ 0 ].x * V3Line[ 1 ].z - V1Line[ 0 ].z * V3Line[ 1 ].x ) / ( V1Line[ 0 ].y * V3Line[ 1 ].x - V1Line[ 0 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 0 ].x = ( V3Line[ 0 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V3Line[ 0 ].y ) / ( V1Line[ 1 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 0 ].y = ( V1Line[ 1 ].x * V3Line[ 0 ].z - V1Line[ 1 ].z * V3Line[ 0 ].x ) / ( V1Line[ 1 ].y * V3Line[ 0 ].x - V1Line[ 1 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 0 ][ 1 ][ 1 ].x = ( V3Line[ 1 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V3Line[ 1 ].y ) / ( V1Line[ 1 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 1 ].y = ( V1Line[ 1 ].x * V3Line[ 1 ].z - V1Line[ 1 ].z * V3Line[ 1 ].x ) / ( V1Line[ 1 ].y * V3Line[ 1 ].x - V1Line[ 1 ].x * V3Line[ 1 ].y ) ;

		VPosD[ 1 ][ 0 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 0 ].y ) / ( V2Line[ 0 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 0 ].y = ( V2Line[ 0 ].x * V1Line[ 0 ].z - V2Line[ 0 ].z * V1Line[ 0 ].x ) / ( V2Line[ 0 ].y * V1Line[ 0 ].x - V2Line[ 0 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 1 ].y ) / ( V2Line[ 0 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 1 ].y = ( V2Line[ 0 ].x * V1Line[ 1 ].z - V2Line[ 0 ].z * V1Line[ 1 ].x ) / ( V2Line[ 0 ].y * V1Line[ 1 ].x - V2Line[ 0 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 0 ].y ) / ( V2Line[ 1 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 0 ].y = ( V2Line[ 1 ].x * V1Line[ 0 ].z - V2Line[ 1 ].z * V1Line[ 0 ].x ) / ( V2Line[ 1 ].y * V1Line[ 0 ].x - V2Line[ 1 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 1 ][ 1 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 1 ].y ) / ( V2Line[ 1 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 1 ].y = ( V2Line[ 1 ].x * V1Line[ 1 ].z - V2Line[ 1 ].z * V1Line[ 1 ].x ) / ( V2Line[ 1 ].y * V1Line[ 1 ].x - V2Line[ 1 ].x * V1Line[ 1 ].y ) ;

		VPosD[ 2 ][ 0 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 0 ].y ) / ( V3Line[ 0 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 0 ].y = ( V3Line[ 0 ].x * V2Line[ 0 ].z - V3Line[ 0 ].z * V2Line[ 0 ].x ) / ( V3Line[ 0 ].y * V2Line[ 0 ].x - V3Line[ 0 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 1 ].y ) / ( V3Line[ 0 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 1 ].y = ( V3Line[ 0 ].x * V2Line[ 1 ].z - V3Line[ 0 ].z * V2Line[ 1 ].x ) / ( V3Line[ 0 ].y * V2Line[ 1 ].x - V3Line[ 0 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 0 ].y ) / ( V3Line[ 1 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 0 ].y = ( V3Line[ 1 ].x * V2Line[ 0 ].z - V3Line[ 1 ].z * V2Line[ 0 ].x ) / ( V3Line[ 1 ].y * V2Line[ 0 ].x - V3Line[ 1 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 2 ][ 1 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 1 ].y ) / ( V3Line[ 1 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 1 ].y = ( V3Line[ 1 ].x * V2Line[ 1 ].z - V3Line[ 1 ].z * V2Line[ 1 ].x ) / ( V3Line[ 1 ].y * V2Line[ 1 ].x - V3Line[ 1 ].x * V2Line[ 1 ].y ) ;

		Vertex = ParamVertex ;
		n = 1 ;
		for( i = 0 ; i < 3 ; i++ )
		{
			Vertex[ 0 ].x = VPosD[ i ][ 0 ][ 1 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 0 ][ 1 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 0 ][ 0 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 0 ][ 0 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 1 ][ 1 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 1 ][ 1 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 1 ][ 0 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 1 ][ 0 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;



			Vertex[ 0 ].x = VPosD[ n ][ 1 ][ 0 ].x ;
			Vertex[ 0 ].y = VPosD[ n ][ 1 ][ 0 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 0 ][ 1 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 0 ][ 1 ].y ;
			Vertex[ 2 ].x = VPosD[ n ][ 1 ][ 1 ].x ;
			Vertex[ 2 ].y = VPosD[ n ][ 1 ][ 1 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 1 ][ 1 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 1 ][ 1 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;


			n = n == 2 ? 0 : n + 1 ;
		}
	}
	else
	{
		Param.QuadrangleNum = 3 * 12 ;

		GrHandle = Graphics_Image_GetLineBoxTexHandle( IS_PMA_DRAWBLENDMODE ) ;

		Param.GraphHandle = GrHandle ;

		LineThickness /= 2.0f ;
		tmp1 =  LineThickness + 1.5f ;
		tmp2 =  LineThickness - 2.5f ;
		tmp3 = -LineThickness + 1.5f ;
		tmp4 = -LineThickness - 2.5f ;

		P1P2X = x2 - x1 ;
		P1P2Y = y2 - y1 ;

		P1P3X = x3 - x1 ;
		P1P3Y = y3 - y1 ;

		if( P1P2X * P1P3Y - P1P2Y * P1P3X < 0.0f )
		{
			TempF = x2 ; x2 = x3 ; x3 = TempF ;
			TempF = y2 ; y2 = y3 ; y3 = TempF ;
		}

		V1Pos.x  = x1 ;			V1Pos.y  = y1 ;			V1Pos.z  = 0.0f ;
		V1Dir.x  = x2 - x1 ;	V1Dir.y  = y2 - y1 ;	V1Dir.z  = 0.0f ;
		V12Dir = V1Dir;
		Distance = V1Dir.x * V1Dir.x + V1Dir.y * V1Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V1Dir.x /= Distance ;
		V1Dir.y /= Distance ;
		V1Norm.x =  V1Dir.y ;
		V1Norm.y = -V1Dir.x ;
		V1Norm.z =  0.0f ;
		TempPos = VAdd( V1Pos, VScale( V1Norm, tmp1 ) ) ;
		V1Line[ 0 ].x =  V1Dir.y ;
		V1Line[ 0 ].y = -V1Dir.x ;
		V1Line[ 0 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;
		TempPos = VAdd( V1Pos, VScale( V1Norm, tmp2 ) ) ;
		V1Line[ 1 ].x =  V1Dir.y ;
		V1Line[ 1 ].y = -V1Dir.x ;
		V1Line[ 1 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;
		TempPos = VAdd( V1Pos, VScale( V1Norm, tmp3 ) ) ;
		V1Line[ 2 ].x =  V1Dir.y ;
		V1Line[ 2 ].y = -V1Dir.x ;
		V1Line[ 2 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;
		TempPos = VAdd( V1Pos, VScale( V1Norm, tmp4 ) ) ;
		V1Line[ 3 ].x =  V1Dir.y ;
		V1Line[ 3 ].y = -V1Dir.x ;
		V1Line[ 3 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;

		V2Pos.x  = x2 ;			V2Pos.y  = y2 ;			V2Pos.z  = 0.0f ;
		V2Dir.x  = x3 - x2 ;	V2Dir.y  = y3 - y2 ;	V2Dir.z  = 0.0f ;
		Distance = V2Dir.x * V2Dir.x + V2Dir.y * V2Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V2Dir.x /= Distance ;
		V2Dir.y /= Distance ;
		V2Norm.x =  V2Dir.y ;
		V2Norm.y = -V2Dir.x ;
		V2Norm.z =  0.0f ;
		TempPos = VAdd( V2Pos, VScale( V2Norm, tmp1 ) ) ;
		V2Line[ 0 ].x =  V2Dir.y ;
		V2Line[ 0 ].y = -V2Dir.x ;
		V2Line[ 0 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;
		TempPos = VAdd( V2Pos, VScale( V2Norm, tmp2 ) ) ;
		V2Line[ 1 ].x =  V2Dir.y ;
		V2Line[ 1 ].y = -V2Dir.x ;
		V2Line[ 1 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;
		TempPos = VAdd( V2Pos, VScale( V2Norm, tmp3 ) ) ;
		V2Line[ 2 ].x =  V2Dir.y ;
		V2Line[ 2 ].y = -V2Dir.x ;
		V2Line[ 2 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;
		TempPos = VAdd( V2Pos, VScale( V2Norm, tmp4 ) ) ;
		V2Line[ 3 ].x =  V2Dir.y ;
		V2Line[ 3 ].y = -V2Dir.x ;
		V2Line[ 3 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;

		V13Dir.x = x3 - x1 ;    V13Dir.y = y3 - y1 ;	V13Dir.z = 0.0f ;
		SizeSq = VSquareSize( VCross( V12Dir, V13Dir ) ) ;
		if( SizeSq / 2.0f < 7500.0f )
		{
			NormalDraw = TRUE ;
			goto END ;
		}

		V3Pos.x  = x3 ;			V3Pos.y  = y3 ;			V3Pos.z  = 0.0f ;
		V3Dir.x  = x1 - x3 ;	V3Dir.y  = y1 - y3 ;	V3Dir.z  = 0.0f ;
		Distance = V3Dir.x * V3Dir.x + V3Dir.y * V3Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V3Dir.x /= Distance ;
		V3Dir.y /= Distance ;
		V3Norm.x =  V3Dir.y ;
		V3Norm.y = -V3Dir.x ;
		V3Norm.z =  0.0f ;
		TempPos = VAdd( V3Pos, VScale( V3Norm, tmp1 ) ) ;
		V3Line[ 0 ].x =  V3Dir.y ;
		V3Line[ 0 ].y = -V3Dir.x ;
		V3Line[ 0 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;
		TempPos = VAdd( V3Pos, VScale( V3Norm, tmp2 ) ) ;
		V3Line[ 1 ].x =  V3Dir.y ;
		V3Line[ 1 ].y = -V3Dir.x ;
		V3Line[ 1 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;
		TempPos = VAdd( V3Pos, VScale( V3Norm, tmp3 ) ) ;
		V3Line[ 2 ].x =  V3Dir.y ;
		V3Line[ 2 ].y = -V3Dir.x ;
		V3Line[ 2 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;
		TempPos = VAdd( V3Pos, VScale( V3Norm, tmp4 ) ) ;
		V3Line[ 3 ].x =  V3Dir.y ;
		V3Line[ 3 ].y = -V3Dir.x ;
		V3Line[ 3 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;


		VPosD[ 0 ][ 0 ][ 0 ].x = ( V3Line[ 0 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V3Line[ 0 ].y ) / ( V1Line[ 0 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 0 ].y = ( V1Line[ 0 ].x * V3Line[ 0 ].z - V1Line[ 0 ].z * V3Line[ 0 ].x ) / ( V1Line[ 0 ].y * V3Line[ 0 ].x - V1Line[ 0 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 1 ].x = ( V3Line[ 1 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V3Line[ 1 ].y ) / ( V1Line[ 0 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 1 ].y = ( V1Line[ 0 ].x * V3Line[ 1 ].z - V1Line[ 0 ].z * V3Line[ 1 ].x ) / ( V1Line[ 0 ].y * V3Line[ 1 ].x - V1Line[ 0 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 0 ][ 0 ][ 2 ].x = ( V3Line[ 2 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V3Line[ 2 ].y ) / ( V1Line[ 0 ].x * V3Line[ 2 ].y - V3Line[ 2 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 2 ].y = ( V1Line[ 0 ].x * V3Line[ 2 ].z - V1Line[ 0 ].z * V3Line[ 2 ].x ) / ( V1Line[ 0 ].y * V3Line[ 2 ].x - V1Line[ 0 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 0 ][ 0 ][ 3 ].x = ( V3Line[ 3 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V3Line[ 3 ].y ) / ( V1Line[ 0 ].x * V3Line[ 3 ].y - V3Line[ 3 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 3 ].y = ( V1Line[ 0 ].x * V3Line[ 3 ].z - V1Line[ 0 ].z * V3Line[ 3 ].x ) / ( V1Line[ 0 ].y * V3Line[ 3 ].x - V1Line[ 0 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 0 ][ 1 ][ 0 ].x = ( V3Line[ 0 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V3Line[ 0 ].y ) / ( V1Line[ 1 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 0 ].y = ( V1Line[ 1 ].x * V3Line[ 0 ].z - V1Line[ 1 ].z * V3Line[ 0 ].x ) / ( V1Line[ 1 ].y * V3Line[ 0 ].x - V1Line[ 1 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 0 ][ 1 ][ 1 ].x = ( V3Line[ 1 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V3Line[ 1 ].y ) / ( V1Line[ 1 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 1 ].y = ( V1Line[ 1 ].x * V3Line[ 1 ].z - V1Line[ 1 ].z * V3Line[ 1 ].x ) / ( V1Line[ 1 ].y * V3Line[ 1 ].x - V1Line[ 1 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 2 ].x = ( V3Line[ 2 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V3Line[ 2 ].y ) / ( V1Line[ 1 ].x * V3Line[ 2 ].y - V3Line[ 2 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 2 ].y = ( V1Line[ 1 ].x * V3Line[ 2 ].z - V1Line[ 1 ].z * V3Line[ 2 ].x ) / ( V1Line[ 1 ].y * V3Line[ 2 ].x - V1Line[ 1 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 0 ][ 1 ][ 3 ].x = ( V3Line[ 3 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V3Line[ 3 ].y ) / ( V1Line[ 1 ].x * V3Line[ 3 ].y - V3Line[ 3 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 3 ].y = ( V1Line[ 1 ].x * V3Line[ 3 ].z - V1Line[ 1 ].z * V3Line[ 3 ].x ) / ( V1Line[ 1 ].y * V3Line[ 3 ].x - V1Line[ 1 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 0 ][ 2 ][ 0 ].x = ( V3Line[ 0 ].z * V1Line[ 2 ].y - V1Line[ 2 ].z * V3Line[ 0 ].y ) / ( V1Line[ 2 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 0 ][ 2 ][ 0 ].y = ( V1Line[ 2 ].x * V3Line[ 0 ].z - V1Line[ 2 ].z * V3Line[ 0 ].x ) / ( V1Line[ 2 ].y * V3Line[ 0 ].x - V1Line[ 2 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 0 ][ 2 ][ 1 ].x = ( V3Line[ 1 ].z * V1Line[ 2 ].y - V1Line[ 2 ].z * V3Line[ 1 ].y ) / ( V1Line[ 2 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 0 ][ 2 ][ 1 ].y = ( V1Line[ 2 ].x * V3Line[ 1 ].z - V1Line[ 2 ].z * V3Line[ 1 ].x ) / ( V1Line[ 2 ].y * V3Line[ 1 ].x - V1Line[ 2 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 0 ][ 2 ][ 2 ].x = ( V3Line[ 2 ].z * V1Line[ 2 ].y - V1Line[ 2 ].z * V3Line[ 2 ].y ) / ( V1Line[ 2 ].x * V3Line[ 2 ].y - V3Line[ 2 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 0 ][ 2 ][ 2 ].y = ( V1Line[ 2 ].x * V3Line[ 2 ].z - V1Line[ 2 ].z * V3Line[ 2 ].x ) / ( V1Line[ 2 ].y * V3Line[ 2 ].x - V1Line[ 2 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 0 ][ 2 ][ 3 ].x = ( V3Line[ 3 ].z * V1Line[ 2 ].y - V1Line[ 2 ].z * V3Line[ 3 ].y ) / ( V1Line[ 2 ].x * V3Line[ 3 ].y - V3Line[ 3 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 0 ][ 2 ][ 3 ].y = ( V1Line[ 2 ].x * V3Line[ 3 ].z - V1Line[ 2 ].z * V3Line[ 3 ].x ) / ( V1Line[ 2 ].y * V3Line[ 3 ].x - V1Line[ 2 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 0 ][ 3 ][ 0 ].x = ( V3Line[ 0 ].z * V1Line[ 3 ].y - V1Line[ 3 ].z * V3Line[ 0 ].y ) / ( V1Line[ 3 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 0 ][ 3 ][ 0 ].y = ( V1Line[ 3 ].x * V3Line[ 0 ].z - V1Line[ 3 ].z * V3Line[ 0 ].x ) / ( V1Line[ 3 ].y * V3Line[ 0 ].x - V1Line[ 3 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 0 ][ 3 ][ 1 ].x = ( V3Line[ 1 ].z * V1Line[ 3 ].y - V1Line[ 3 ].z * V3Line[ 1 ].y ) / ( V1Line[ 3 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 0 ][ 3 ][ 1 ].y = ( V1Line[ 3 ].x * V3Line[ 1 ].z - V1Line[ 3 ].z * V3Line[ 1 ].x ) / ( V1Line[ 3 ].y * V3Line[ 1 ].x - V1Line[ 3 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 0 ][ 3 ][ 2 ].x = ( V3Line[ 2 ].z * V1Line[ 3 ].y - V1Line[ 3 ].z * V3Line[ 2 ].y ) / ( V1Line[ 3 ].x * V3Line[ 2 ].y - V3Line[ 2 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 0 ][ 3 ][ 2 ].y = ( V1Line[ 3 ].x * V3Line[ 2 ].z - V1Line[ 3 ].z * V3Line[ 2 ].x ) / ( V1Line[ 3 ].y * V3Line[ 2 ].x - V1Line[ 3 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 0 ][ 3 ][ 3 ].x = ( V3Line[ 3 ].z * V1Line[ 3 ].y - V1Line[ 3 ].z * V3Line[ 3 ].y ) / ( V1Line[ 3 ].x * V3Line[ 3 ].y - V3Line[ 3 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 0 ][ 3 ][ 3 ].y = ( V1Line[ 3 ].x * V3Line[ 3 ].z - V1Line[ 3 ].z * V3Line[ 3 ].x ) / ( V1Line[ 3 ].y * V3Line[ 3 ].x - V1Line[ 3 ].x * V3Line[ 3 ].y ) ;

		VPosD[ 1 ][ 0 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 0 ].y ) / ( V2Line[ 0 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 0 ].y = ( V2Line[ 0 ].x * V1Line[ 0 ].z - V2Line[ 0 ].z * V1Line[ 0 ].x ) / ( V2Line[ 0 ].y * V1Line[ 0 ].x - V2Line[ 0 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 1 ].y ) / ( V2Line[ 0 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 1 ].y = ( V2Line[ 0 ].x * V1Line[ 1 ].z - V2Line[ 0 ].z * V1Line[ 1 ].x ) / ( V2Line[ 0 ].y * V1Line[ 1 ].x - V2Line[ 0 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 1 ][ 0 ][ 2 ].x = ( V1Line[ 2 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 2 ].y ) / ( V2Line[ 0 ].x * V1Line[ 2 ].y - V1Line[ 2 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 2 ].y = ( V2Line[ 0 ].x * V1Line[ 2 ].z - V2Line[ 0 ].z * V1Line[ 2 ].x ) / ( V2Line[ 0 ].y * V1Line[ 2 ].x - V2Line[ 0 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 1 ][ 0 ][ 3 ].x = ( V1Line[ 3 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 3 ].y ) / ( V2Line[ 0 ].x * V1Line[ 3 ].y - V1Line[ 3 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 3 ].y = ( V2Line[ 0 ].x * V1Line[ 3 ].z - V2Line[ 0 ].z * V1Line[ 3 ].x ) / ( V2Line[ 0 ].y * V1Line[ 3 ].x - V2Line[ 0 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 1 ][ 1 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 0 ].y ) / ( V2Line[ 1 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 0 ].y = ( V2Line[ 1 ].x * V1Line[ 0 ].z - V2Line[ 1 ].z * V1Line[ 0 ].x ) / ( V2Line[ 1 ].y * V1Line[ 0 ].x - V2Line[ 1 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 1 ][ 1 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 1 ].y ) / ( V2Line[ 1 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 1 ].y = ( V2Line[ 1 ].x * V1Line[ 1 ].z - V2Line[ 1 ].z * V1Line[ 1 ].x ) / ( V2Line[ 1 ].y * V1Line[ 1 ].x - V2Line[ 1 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 2 ].x = ( V1Line[ 2 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 2 ].y ) / ( V2Line[ 1 ].x * V1Line[ 2 ].y - V1Line[ 2 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 2 ].y = ( V2Line[ 1 ].x * V1Line[ 2 ].z - V2Line[ 1 ].z * V1Line[ 2 ].x ) / ( V2Line[ 1 ].y * V1Line[ 2 ].x - V2Line[ 1 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 1 ][ 1 ][ 3 ].x = ( V1Line[ 3 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 3 ].y ) / ( V2Line[ 1 ].x * V1Line[ 3 ].y - V1Line[ 3 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 3 ].y = ( V2Line[ 1 ].x * V1Line[ 3 ].z - V2Line[ 1 ].z * V1Line[ 3 ].x ) / ( V2Line[ 1 ].y * V1Line[ 3 ].x - V2Line[ 1 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 1 ][ 2 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 2 ].y - V2Line[ 2 ].z * V1Line[ 0 ].y ) / ( V2Line[ 2 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 1 ][ 2 ][ 0 ].y = ( V2Line[ 2 ].x * V1Line[ 0 ].z - V2Line[ 2 ].z * V1Line[ 0 ].x ) / ( V2Line[ 2 ].y * V1Line[ 0 ].x - V2Line[ 2 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 1 ][ 2 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 2 ].y - V2Line[ 2 ].z * V1Line[ 1 ].y ) / ( V2Line[ 2 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 1 ][ 2 ][ 1 ].y = ( V2Line[ 2 ].x * V1Line[ 1 ].z - V2Line[ 2 ].z * V1Line[ 1 ].x ) / ( V2Line[ 2 ].y * V1Line[ 1 ].x - V2Line[ 2 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 1 ][ 2 ][ 2 ].x = ( V1Line[ 2 ].z * V2Line[ 2 ].y - V2Line[ 2 ].z * V1Line[ 2 ].y ) / ( V2Line[ 2 ].x * V1Line[ 2 ].y - V1Line[ 2 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 1 ][ 2 ][ 2 ].y = ( V2Line[ 2 ].x * V1Line[ 2 ].z - V2Line[ 2 ].z * V1Line[ 2 ].x ) / ( V2Line[ 2 ].y * V1Line[ 2 ].x - V2Line[ 2 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 1 ][ 2 ][ 3 ].x = ( V1Line[ 3 ].z * V2Line[ 2 ].y - V2Line[ 2 ].z * V1Line[ 3 ].y ) / ( V2Line[ 2 ].x * V1Line[ 3 ].y - V1Line[ 3 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 1 ][ 2 ][ 3 ].y = ( V2Line[ 2 ].x * V1Line[ 3 ].z - V2Line[ 2 ].z * V1Line[ 3 ].x ) / ( V2Line[ 2 ].y * V1Line[ 3 ].x - V2Line[ 2 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 1 ][ 3 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 3 ].y - V2Line[ 3 ].z * V1Line[ 0 ].y ) / ( V2Line[ 3 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 1 ][ 3 ][ 0 ].y = ( V2Line[ 3 ].x * V1Line[ 0 ].z - V2Line[ 3 ].z * V1Line[ 0 ].x ) / ( V2Line[ 3 ].y * V1Line[ 0 ].x - V2Line[ 3 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 1 ][ 3 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 3 ].y - V2Line[ 3 ].z * V1Line[ 1 ].y ) / ( V2Line[ 3 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 1 ][ 3 ][ 1 ].y = ( V2Line[ 3 ].x * V1Line[ 1 ].z - V2Line[ 3 ].z * V1Line[ 1 ].x ) / ( V2Line[ 3 ].y * V1Line[ 1 ].x - V2Line[ 3 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 1 ][ 3 ][ 2 ].x = ( V1Line[ 2 ].z * V2Line[ 3 ].y - V2Line[ 3 ].z * V1Line[ 2 ].y ) / ( V2Line[ 3 ].x * V1Line[ 2 ].y - V1Line[ 2 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 1 ][ 3 ][ 2 ].y = ( V2Line[ 3 ].x * V1Line[ 2 ].z - V2Line[ 3 ].z * V1Line[ 2 ].x ) / ( V2Line[ 3 ].y * V1Line[ 2 ].x - V2Line[ 3 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 1 ][ 3 ][ 3 ].x = ( V1Line[ 3 ].z * V2Line[ 3 ].y - V2Line[ 3 ].z * V1Line[ 3 ].y ) / ( V2Line[ 3 ].x * V1Line[ 3 ].y - V1Line[ 3 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 1 ][ 3 ][ 3 ].y = ( V2Line[ 3 ].x * V1Line[ 3 ].z - V2Line[ 3 ].z * V1Line[ 3 ].x ) / ( V2Line[ 3 ].y * V1Line[ 3 ].x - V2Line[ 3 ].x * V1Line[ 3 ].y ) ;

		VPosD[ 2 ][ 0 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 0 ].y ) / ( V3Line[ 0 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 0 ].y = ( V3Line[ 0 ].x * V2Line[ 0 ].z - V3Line[ 0 ].z * V2Line[ 0 ].x ) / ( V3Line[ 0 ].y * V2Line[ 0 ].x - V3Line[ 0 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 1 ].y ) / ( V3Line[ 0 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 1 ].y = ( V3Line[ 0 ].x * V2Line[ 1 ].z - V3Line[ 0 ].z * V2Line[ 1 ].x ) / ( V3Line[ 0 ].y * V2Line[ 1 ].x - V3Line[ 0 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 2 ][ 0 ][ 2 ].x = ( V2Line[ 2 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 2 ].y ) / ( V3Line[ 0 ].x * V2Line[ 2 ].y - V2Line[ 2 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 2 ].y = ( V3Line[ 0 ].x * V2Line[ 2 ].z - V3Line[ 0 ].z * V2Line[ 2 ].x ) / ( V3Line[ 0 ].y * V2Line[ 2 ].x - V3Line[ 0 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 2 ][ 0 ][ 3 ].x = ( V2Line[ 3 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 3 ].y ) / ( V3Line[ 0 ].x * V2Line[ 3 ].y - V2Line[ 3 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 3 ].y = ( V3Line[ 0 ].x * V2Line[ 3 ].z - V3Line[ 0 ].z * V2Line[ 3 ].x ) / ( V3Line[ 0 ].y * V2Line[ 3 ].x - V3Line[ 0 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 2 ][ 1 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 0 ].y ) / ( V3Line[ 1 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 0 ].y = ( V3Line[ 1 ].x * V2Line[ 0 ].z - V3Line[ 1 ].z * V2Line[ 0 ].x ) / ( V3Line[ 1 ].y * V2Line[ 0 ].x - V3Line[ 1 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 2 ][ 1 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 1 ].y ) / ( V3Line[ 1 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 1 ].y = ( V3Line[ 1 ].x * V2Line[ 1 ].z - V3Line[ 1 ].z * V2Line[ 1 ].x ) / ( V3Line[ 1 ].y * V2Line[ 1 ].x - V3Line[ 1 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 2 ].x = ( V2Line[ 2 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 2 ].y ) / ( V3Line[ 1 ].x * V2Line[ 2 ].y - V2Line[ 2 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 2 ].y = ( V3Line[ 1 ].x * V2Line[ 2 ].z - V3Line[ 1 ].z * V2Line[ 2 ].x ) / ( V3Line[ 1 ].y * V2Line[ 2 ].x - V3Line[ 1 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 2 ][ 1 ][ 3 ].x = ( V2Line[ 3 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 3 ].y ) / ( V3Line[ 1 ].x * V2Line[ 3 ].y - V2Line[ 3 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 3 ].y = ( V3Line[ 1 ].x * V2Line[ 3 ].z - V3Line[ 1 ].z * V2Line[ 3 ].x ) / ( V3Line[ 1 ].y * V2Line[ 3 ].x - V3Line[ 1 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 2 ][ 2 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 2 ].y - V3Line[ 2 ].z * V2Line[ 0 ].y ) / ( V3Line[ 2 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 2 ][ 2 ][ 0 ].y = ( V3Line[ 2 ].x * V2Line[ 0 ].z - V3Line[ 2 ].z * V2Line[ 0 ].x ) / ( V3Line[ 2 ].y * V2Line[ 0 ].x - V3Line[ 2 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 2 ][ 2 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 2 ].y - V3Line[ 2 ].z * V2Line[ 1 ].y ) / ( V3Line[ 2 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 2 ][ 2 ][ 1 ].y = ( V3Line[ 2 ].x * V2Line[ 1 ].z - V3Line[ 2 ].z * V2Line[ 1 ].x ) / ( V3Line[ 2 ].y * V2Line[ 1 ].x - V3Line[ 2 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 2 ][ 2 ][ 2 ].x = ( V2Line[ 2 ].z * V3Line[ 2 ].y - V3Line[ 2 ].z * V2Line[ 2 ].y ) / ( V3Line[ 2 ].x * V2Line[ 2 ].y - V2Line[ 2 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 2 ][ 2 ][ 2 ].y = ( V3Line[ 2 ].x * V2Line[ 2 ].z - V3Line[ 2 ].z * V2Line[ 2 ].x ) / ( V3Line[ 2 ].y * V2Line[ 2 ].x - V3Line[ 2 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 2 ][ 2 ][ 3 ].x = ( V2Line[ 3 ].z * V3Line[ 2 ].y - V3Line[ 2 ].z * V2Line[ 3 ].y ) / ( V3Line[ 2 ].x * V2Line[ 3 ].y - V2Line[ 3 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 2 ][ 2 ][ 3 ].y = ( V3Line[ 2 ].x * V2Line[ 3 ].z - V3Line[ 2 ].z * V2Line[ 3 ].x ) / ( V3Line[ 2 ].y * V2Line[ 3 ].x - V3Line[ 2 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 2 ][ 3 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 3 ].y - V3Line[ 3 ].z * V2Line[ 0 ].y ) / ( V3Line[ 3 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 2 ][ 3 ][ 0 ].y = ( V3Line[ 3 ].x * V2Line[ 0 ].z - V3Line[ 3 ].z * V2Line[ 0 ].x ) / ( V3Line[ 3 ].y * V2Line[ 0 ].x - V3Line[ 3 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 2 ][ 3 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 3 ].y - V3Line[ 3 ].z * V2Line[ 1 ].y ) / ( V3Line[ 3 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 2 ][ 3 ][ 1 ].y = ( V3Line[ 3 ].x * V2Line[ 1 ].z - V3Line[ 3 ].z * V2Line[ 1 ].x ) / ( V3Line[ 3 ].y * V2Line[ 1 ].x - V3Line[ 3 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 2 ][ 3 ][ 2 ].x = ( V2Line[ 2 ].z * V3Line[ 3 ].y - V3Line[ 3 ].z * V2Line[ 2 ].y ) / ( V3Line[ 3 ].x * V2Line[ 2 ].y - V2Line[ 2 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 2 ][ 3 ][ 2 ].y = ( V3Line[ 3 ].x * V2Line[ 2 ].z - V3Line[ 3 ].z * V2Line[ 2 ].x ) / ( V3Line[ 3 ].y * V2Line[ 2 ].x - V3Line[ 3 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 2 ][ 3 ][ 3 ].x = ( V2Line[ 3 ].z * V3Line[ 3 ].y - V3Line[ 3 ].z * V2Line[ 3 ].y ) / ( V3Line[ 3 ].x * V2Line[ 3 ].y - V2Line[ 3 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 2 ][ 3 ][ 3 ].y = ( V3Line[ 3 ].x * V2Line[ 3 ].z - V3Line[ 3 ].z * V2Line[ 3 ].x ) / ( V3Line[ 3 ].y * V2Line[ 3 ].x - V3Line[ 3 ].x * V2Line[ 3 ].y ) ;

		Vertex = ParamVertex ;
		n = 1 ;
		for( i = 0 ; i < 3 ; i++ )
		{
			Vertex[ 0 ].x = VPosD[ i ][ 0 ][ 1 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 0 ][ 1 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 0 ][ 0 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 0 ][ 0 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 1 ][ 1 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 1 ][ 1 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 1 ][ 0 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 1 ][ 0 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;

			Vertex[ 0 ].x = VPosD[ i ][ 0 ][ 2 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 0 ][ 2 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 0 ][ 1 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 0 ][ 1 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 1 ][ 2 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 1 ][ 2 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 1 ][ 1 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 1 ][ 1 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;

			Vertex[ 0 ].x = VPosD[ i ][ 0 ][ 3 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 0 ][ 3 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 0 ][ 2 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 0 ][ 2 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 1 ][ 3 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 1 ][ 3 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 1 ][ 2 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 1 ][ 2 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;



			Vertex[ 0 ].x = VPosD[ i ][ 1 ][ 1 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 1 ][ 1 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 1 ][ 0 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 1 ][ 0 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 2 ][ 1 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 2 ][ 1 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 2 ][ 0 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 2 ][ 0 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;

			Vertex[ 0 ].x = VPosD[ i ][ 1 ][ 2 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 1 ][ 2 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 1 ][ 1 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 1 ][ 1 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 2 ][ 2 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 2 ][ 2 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 2 ][ 1 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 2 ][ 1 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;

			Vertex[ 0 ].x = VPosD[ i ][ 1 ][ 3 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 1 ][ 3 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 1 ][ 2 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 1 ][ 2 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 2 ][ 3 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 2 ][ 3 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 2 ][ 2 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 2 ][ 2 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;




			Vertex[ 0 ].x = VPosD[ i ][ 2 ][ 1 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 2 ][ 1 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 2 ][ 0 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 2 ][ 0 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 3 ][ 1 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 3 ][ 1 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 3 ][ 0 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 3 ][ 0 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;

			Vertex[ 0 ].x = VPosD[ i ][ 2 ][ 2 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 2 ][ 2 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 2 ][ 1 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 2 ][ 1 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 3 ][ 2 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 3 ][ 2 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 3 ][ 1 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 3 ][ 1 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;

			Vertex[ 0 ].x = VPosD[ i ][ 2 ][ 3 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 2 ][ 3 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 2 ][ 2 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 2 ][ 2 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 3 ][ 3 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 3 ][ 3 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 3 ][ 2 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 3 ][ 2 ].y ;

			Vertex[ 0 ].u = Line_In_UV[ 1 ] ;	Vertex[ 0 ].v = Line_In_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_In_UV[ 0 ] ;	Vertex[ 1 ].v = Line_In_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_In_UV[ 1 ] ;	Vertex[ 2 ].v = Line_In_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_In_UV[ 0 ] ;	Vertex[ 3 ].v = Line_In_UV[ 1 ] ;

			Vertex += 4 ;




			Vertex[ 0 ].x = VPosD[ n ][ 3 ][ 0 ].x ;
			Vertex[ 0 ].y = VPosD[ n ][ 3 ][ 0 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 0 ][ 3 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 0 ][ 3 ].y ;
			Vertex[ 2 ].x = VPosD[ n ][ 3 ][ 1 ].x ;
			Vertex[ 2 ].y = VPosD[ n ][ 3 ][ 1 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 1 ][ 3 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 1 ][ 3 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;


			Vertex[ 0 ].x = VPosD[ n ][ 3 ][ 1 ].x ;
			Vertex[ 0 ].y = VPosD[ n ][ 3 ][ 1 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 1 ][ 3 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 1 ][ 3 ].y ;
			Vertex[ 2 ].x = VPosD[ n ][ 3 ][ 2 ].x ;
			Vertex[ 2 ].y = VPosD[ n ][ 3 ][ 2 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 2 ][ 3 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 2 ][ 3 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;


			Vertex[ 0 ].x = VPosD[ n ][ 3 ][ 2 ].x ;
			Vertex[ 0 ].y = VPosD[ n ][ 3 ][ 2 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 2 ][ 3 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 2 ][ 3 ].y ;
			Vertex[ 2 ].x = VPosD[ n ][ 3 ][ 3 ].x ;
			Vertex[ 2 ].y = VPosD[ n ][ 3 ][ 3 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 3 ][ 3 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 3 ][ 3 ].y ;

			Vertex[ 0 ].u = Line_In_UV[ 1 ] ;	Vertex[ 0 ].v = Line_In_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_In_UV[ 1 ] ;	Vertex[ 1 ].v = Line_In_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_In_UV[ 1 ] ;	Vertex[ 2 ].v = Line_In_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_In_UV[ 1 ] ;	Vertex[ 3 ].v = Line_In_UV[ 1 ] ;

			Vertex += 4 ;


			n = n == 2 ? 0 : n + 1 ;
		}
	}

	Graphics_Draw_DrawSimpleQuadrangleGraphF( &Param ) ;

END :

	if( OldDrawMode != -1 )
	{
		NS_SetDrawMode( OldDrawMode ) ;
	}

	Graphics_DrawSetting_SetDrawBrightToOneParam( OrigColor ) ;

	if( NormalDraw )
	{
		return NS_DrawTriangle( _FTOL( x1 ), _FTOL( y1 ), _FTOL( x2 ), _FTOL( y2 ), _FTOL( x3 ), _FTOL( y3 ), Color, TRUE ) ;
	}

	// �I��
	return 0 ;
}

// �O�p�`�̕`�悷��( �A���`�G�C���A�X�t�� )
static int DrawTriangleFillAA( float x1, float y1, float x2, float y2, float x3, float y3, unsigned int Color )
{
	int OldDrawMode = -1 ;
	int GrHandle ;
	GRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM Param ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX ParamVertex[ 6 * 6 + 3 ] ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *Vertex ;
	VECTOR V1Dir, V1Pos, V1Norm, V1Line[ 2 ] ;
	VECTOR V2Dir, V2Pos, V2Norm, V2Line[ 2 ] ;
	VECTOR V3Dir, V3Pos, V3Norm, V3Line[ 2 ] ;
	VECTOR V12Dir ;
	VECTOR V13Dir ;
	VECTOR VPosD[ 3 ][ 4 ] ;
	float Distance ;
	VECTOR TempPos ;
	float P1P2X ;
	float P1P2Y ;
	float P1P3X ;
	float P1P3Y ;
	float TempF ;
	int i ;
	int n ;
	DWORD FColor ;
	DWORD OrigColor ;
	int NormalDraw = FALSE ;
	static float LineUV[ 4 ] = { 0.0f, 3.0f / LINE_TEX_SIZE, 5.0f / LINE_TEX_SIZE, 1.0f };
	float SizeSq ;

	if( NS_GetDrawFloatCoordType() == DX_DRAWFLOATCOORDTYPE_DIRECT3D9 )
	{
		x1 -= 0.5f ;
		y1 -= 0.5f ;
		x2 -= 0.5f ;
		y2 -= 0.5f ;
		x3 -= 0.5f ;
		y3 -= 0.5f ;
	}

	SETUP32BITCOLOR( Color, FColor ) ;
	OrigColor = GSYS.DrawSetting.bDrawBright ;
	Graphics_DrawSetting_SetDrawBrightToOneParam( FColor ) ;

	if( GSYS.DrawSetting.DrawMode != DX_DRAWMODE_BILINEAR )
	{
		OldDrawMode = GSYS.DrawSetting.DrawMode ;
		NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	}

	Param.Vertex      = ParamVertex ;
	Param.TransFlag   = TRUE ;
	Param.TriangleNum = 13 ;

	GrHandle = Graphics_Image_GetLineTexHandle( IS_PMA_DRAWBLENDMODE ) ;

	Param.GraphHandle = GrHandle ;

	P1P2X = x2 - x1 ;
	P1P2Y = y2 - y1 ;

	P1P3X = x3 - x1 ;
	P1P3Y = y3 - y1 ;

	if( P1P2X * P1P3Y - P1P2Y * P1P3X < 0.0f )
	{
		TempF = x2 ; x2 = x3 ; x3 = TempF ;
		TempF = y2 ; y2 = y3 ; y3 = TempF ;
	}

	V1Pos.x  = x1 ;			V1Pos.y  = y1 ;			V1Pos.z  = 0.0f ;
	V1Dir.x  = x2 - x1 ;	V1Dir.y  = y2 - y1 ;	V1Dir.z  = 0.0f ;
	V12Dir = V1Dir;
	Distance = V1Dir.x * V1Dir.x + V1Dir.y * V1Dir.y ;
	if( Distance < 0.000001f )
	{
		goto END ;
	}
	Distance = _SQRT( Distance ) ;
	V1Dir.x /= Distance ;
	V1Dir.y /= Distance ;
	V1Norm.x = V1Dir.y ;	V1Norm.y = -V1Dir.x ;	V1Norm.z = 0.0f ;
	TempPos = VAdd( V1Pos, VScale( V1Norm,  2.0f ) ) ;
	V1Line[ 0 ].x =  V1Dir.y ;
	V1Line[ 0 ].y = -V1Dir.x ;
	V1Line[ 0 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;
	TempPos = VAdd( V1Pos, VScale( V1Norm, -1.0f ) ) ;
	V1Line[ 1 ].x =  V1Dir.y ;
	V1Line[ 1 ].y = -V1Dir.x ;
	V1Line[ 1 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;

	V2Pos.x  = x2 ;			V2Pos.y  = y2 ;			V2Pos.z  = 0.0f ;
	V2Dir.x  = x3 - x2 ;	V2Dir.y  = y3 - y2 ;	V2Dir.z  = 0.0f ;
	Distance = V2Dir.x * V2Dir.x + V2Dir.y * V2Dir.y ;
	if( Distance < 0.000001f )
	{
		goto END ;
	}
	Distance = _SQRT( Distance ) ;
	V2Dir.x /= Distance ;
	V2Dir.y /= Distance ;
	V2Norm.x = V2Dir.y ;	V2Norm.y = -V2Dir.x ;	V2Norm.z = 0.0f ;
	TempPos = VAdd( V2Pos, VScale( V2Norm,  2.0f ) ) ;
	V2Line[ 0 ].x =  V2Dir.y ;
	V2Line[ 0 ].y = -V2Dir.x ;
	V2Line[ 0 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;
	TempPos = VAdd( V2Pos, VScale( V2Norm, -1.0f ) ) ;
	V2Line[ 1 ].x =  V2Dir.y ;
	V2Line[ 1 ].y = -V2Dir.x ;
	V2Line[ 1 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;

	V13Dir.x = x3 - x1 ;    V13Dir.y = y3 - y1 ;	V13Dir.z = 0.0f ;
	SizeSq = VSquareSize( VCross( V12Dir, V13Dir ) ) ;
	if( SizeSq / 2.0f < 7500.0f )
	{
		NormalDraw = TRUE ;
		goto END ;
	}

	V3Pos.x  = x3 ;			V3Pos.y  = y3 ;			V3Pos.z  = 0.0f ;
	V3Dir.x  = x1 - x3 ;	V3Dir.y  = y1 - y3 ;	V3Dir.z  = 0.0f ;
	Distance = V3Dir.x * V3Dir.x + V3Dir.y * V3Dir.y ;
	if( Distance < 0.000001f )
	{
		goto END ;
	}
	Distance = _SQRT( Distance ) ;
	V3Dir.x /= Distance ;
	V3Dir.y /= Distance ;
	V3Norm.x = V3Dir.y ;	V3Norm.y = -V3Dir.x ;	V3Norm.z = 0.0f ;
	TempPos = VAdd( V3Pos, VScale( V3Norm,  2.0f ) ) ;
	V3Line[ 0 ].x =  V3Dir.y ;
	V3Line[ 0 ].y = -V3Dir.x ;
	V3Line[ 0 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;
	TempPos = VAdd( V3Pos, VScale( V3Norm, -1.0f ) ) ;
	V3Line[ 1 ].x =  V3Dir.y ;
	V3Line[ 1 ].y = -V3Dir.x ;
	V3Line[ 1 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;


	VPosD[ 0 ][ 0 ].x = ( V3Line[ 0 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V3Line[ 0 ].y ) / ( V1Line[ 0 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V1Line[ 0 ].y ) ;
	VPosD[ 0 ][ 0 ].y = ( V1Line[ 0 ].x * V3Line[ 0 ].z - V1Line[ 0 ].z * V3Line[ 0 ].x ) / ( V1Line[ 0 ].y * V3Line[ 0 ].x - V1Line[ 0 ].x * V3Line[ 0 ].y ) ;

	VPosD[ 0 ][ 1 ].x = ( V3Line[ 1 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V3Line[ 1 ].y ) / ( V1Line[ 0 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V1Line[ 0 ].y ) ;
	VPosD[ 0 ][ 1 ].y = ( V1Line[ 0 ].x * V3Line[ 1 ].z - V1Line[ 0 ].z * V3Line[ 1 ].x ) / ( V1Line[ 0 ].y * V3Line[ 1 ].x - V1Line[ 0 ].x * V3Line[ 1 ].y ) ;

	VPosD[ 0 ][ 2 ].x = ( V3Line[ 0 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V3Line[ 0 ].y ) / ( V1Line[ 1 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V1Line[ 1 ].y ) ;
	VPosD[ 0 ][ 2 ].y = ( V1Line[ 1 ].x * V3Line[ 0 ].z - V1Line[ 1 ].z * V3Line[ 0 ].x ) / ( V1Line[ 1 ].y * V3Line[ 0 ].x - V1Line[ 1 ].x * V3Line[ 0 ].y ) ;

	VPosD[ 0 ][ 3 ].x = ( V3Line[ 1 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V3Line[ 1 ].y ) / ( V1Line[ 1 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V1Line[ 1 ].y ) ;
	VPosD[ 0 ][ 3 ].y = ( V1Line[ 1 ].x * V3Line[ 1 ].z - V1Line[ 1 ].z * V3Line[ 1 ].x ) / ( V1Line[ 1 ].y * V3Line[ 1 ].x - V1Line[ 1 ].x * V3Line[ 1 ].y ) ;


	VPosD[ 1 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 0 ].y ) / ( V2Line[ 0 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 0 ].y ) ;
	VPosD[ 1 ][ 0 ].y = ( V2Line[ 0 ].x * V1Line[ 0 ].z - V2Line[ 0 ].z * V1Line[ 0 ].x ) / ( V2Line[ 0 ].y * V1Line[ 0 ].x - V2Line[ 0 ].x * V1Line[ 0 ].y ) ;

	VPosD[ 1 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 1 ].y ) / ( V2Line[ 0 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 0 ].y ) ;
	VPosD[ 1 ][ 1 ].y = ( V2Line[ 0 ].x * V1Line[ 1 ].z - V2Line[ 0 ].z * V1Line[ 1 ].x ) / ( V2Line[ 0 ].y * V1Line[ 1 ].x - V2Line[ 0 ].x * V1Line[ 1 ].y ) ;

	VPosD[ 1 ][ 2 ].x = ( V1Line[ 0 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 0 ].y ) / ( V2Line[ 1 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 1 ].y ) ;
	VPosD[ 1 ][ 2 ].y = ( V2Line[ 1 ].x * V1Line[ 0 ].z - V2Line[ 1 ].z * V1Line[ 0 ].x ) / ( V2Line[ 1 ].y * V1Line[ 0 ].x - V2Line[ 1 ].x * V1Line[ 0 ].y ) ;

	VPosD[ 1 ][ 3 ].x = ( V1Line[ 1 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 1 ].y ) / ( V2Line[ 1 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 1 ].y ) ;
	VPosD[ 1 ][ 3 ].y = ( V2Line[ 1 ].x * V1Line[ 1 ].z - V2Line[ 1 ].z * V1Line[ 1 ].x ) / ( V2Line[ 1 ].y * V1Line[ 1 ].x - V2Line[ 1 ].x * V1Line[ 1 ].y ) ;


	VPosD[ 2 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 0 ].y ) / ( V3Line[ 0 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 0 ].y ) ;
	VPosD[ 2 ][ 0 ].y = ( V3Line[ 0 ].x * V2Line[ 0 ].z - V3Line[ 0 ].z * V2Line[ 0 ].x ) / ( V3Line[ 0 ].y * V2Line[ 0 ].x - V3Line[ 0 ].x * V2Line[ 0 ].y ) ;

	VPosD[ 2 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 1 ].y ) / ( V3Line[ 0 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 0 ].y ) ;
	VPosD[ 2 ][ 1 ].y = ( V3Line[ 0 ].x * V2Line[ 1 ].z - V3Line[ 0 ].z * V2Line[ 1 ].x ) / ( V3Line[ 0 ].y * V2Line[ 1 ].x - V3Line[ 0 ].x * V2Line[ 1 ].y ) ;

	VPosD[ 2 ][ 2 ].x = ( V2Line[ 0 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 0 ].y ) / ( V3Line[ 1 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 1 ].y ) ;
	VPosD[ 2 ][ 2 ].y = ( V3Line[ 1 ].x * V2Line[ 0 ].z - V3Line[ 1 ].z * V2Line[ 0 ].x ) / ( V3Line[ 1 ].y * V2Line[ 0 ].x - V3Line[ 1 ].x * V2Line[ 0 ].y ) ;

	VPosD[ 2 ][ 3 ].x = ( V2Line[ 1 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 1 ].y ) / ( V3Line[ 1 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 1 ].y ) ;
	VPosD[ 2 ][ 3 ].y = ( V3Line[ 1 ].x * V2Line[ 1 ].z - V3Line[ 1 ].z * V2Line[ 1 ].x ) / ( V3Line[ 1 ].y * V2Line[ 1 ].x - V3Line[ 1 ].x * V2Line[ 1 ].y ) ;

	Vertex = ParamVertex ;
	n = 1 ;
	for( i = 0 ; i < 3 ; i++ )
	{
		Vertex[ 0 ].x = VPosD[ i ][ 1 ].x ;
		Vertex[ 0 ].y = VPosD[ i ][ 1 ].y ;
		Vertex[ 1 ].x = VPosD[ i ][ 0 ].x ;
		Vertex[ 1 ].y = VPosD[ i ][ 0 ].y ;
		Vertex[ 2 ].x = VPosD[ i ][ 3 ].x ;
		Vertex[ 2 ].y = VPosD[ i ][ 3 ].y ;
		Vertex[ 3 ].x = VPosD[ i ][ 2 ].x ;
		Vertex[ 3 ].y = VPosD[ i ][ 2 ].y ;
		Vertex[ 4 ].x = VPosD[ i ][ 3 ].x ;
		Vertex[ 4 ].y = VPosD[ i ][ 3 ].y ;
		Vertex[ 5 ].x = VPosD[ i ][ 0 ].x ;
		Vertex[ 5 ].y = VPosD[ i ][ 0 ].y ;

		Vertex[ 0 ].u = LineUV[ 1 ] ;	Vertex[ 0 ].v = LineUV[ 0 ] ;
		Vertex[ 1 ].u = LineUV[ 0 ] ;	Vertex[ 1 ].v = LineUV[ 0 ] ;
		Vertex[ 2 ].u = LineUV[ 1 ] ;	Vertex[ 2 ].v = LineUV[ 1 ] ;
		Vertex[ 3 ].u = LineUV[ 0 ] ;	Vertex[ 3 ].v = LineUV[ 1 ] ;
		Vertex[ 4 ].u = LineUV[ 1 ] ;	Vertex[ 4 ].v = LineUV[ 1 ] ;
		Vertex[ 5 ].u = LineUV[ 0 ] ;	Vertex[ 5 ].v = LineUV[ 0 ] ;

		Vertex += 6 ;


		Vertex[ 0 ].x = VPosD[ n ][ 2 ].x ;
		Vertex[ 0 ].y = VPosD[ n ][ 2 ].y ;
		Vertex[ 1 ].x = VPosD[ i ][ 1 ].x ;
		Vertex[ 1 ].y = VPosD[ i ][ 1 ].y ;
		Vertex[ 2 ].x = VPosD[ n ][ 3 ].x ;
		Vertex[ 2 ].y = VPosD[ n ][ 3 ].y ;
		Vertex[ 3 ].x = VPosD[ i ][ 3 ].x ;
		Vertex[ 3 ].y = VPosD[ i ][ 3 ].y ;
		Vertex[ 4 ].x = VPosD[ n ][ 3 ].x ;
		Vertex[ 4 ].y = VPosD[ n ][ 3 ].y ;
		Vertex[ 5 ].x = VPosD[ i ][ 1 ].x ;
		Vertex[ 5 ].y = VPosD[ i ][ 1 ].y ;

		Vertex[ 0 ].u = LineUV[ 1 ] ;	Vertex[ 0 ].v = LineUV[ 0 ] ;
		Vertex[ 1 ].u = LineUV[ 1 ] ;	Vertex[ 1 ].v = LineUV[ 0 ] ;
		Vertex[ 2 ].u = LineUV[ 1 ] ;	Vertex[ 2 ].v = LineUV[ 1 ] ;
		Vertex[ 3 ].u = LineUV[ 1 ] ;	Vertex[ 3 ].v = LineUV[ 1 ] ;
		Vertex[ 4 ].u = LineUV[ 1 ] ;	Vertex[ 4 ].v = LineUV[ 1 ] ;
		Vertex[ 5 ].u = LineUV[ 1 ] ;	Vertex[ 5 ].v = LineUV[ 0 ] ;

		Vertex += 6 ;

		n = n == 2 ? 0 : n + 1 ;
	}

	Vertex[ 0 ].x = VPosD[ 2 ][ 3 ].x ;
	Vertex[ 0 ].y = VPosD[ 2 ][ 3 ].y ;
	Vertex[ 1 ].x = VPosD[ 1 ][ 3 ].x ;
	Vertex[ 1 ].y = VPosD[ 1 ][ 3 ].y ;
	Vertex[ 2 ].x = VPosD[ 0 ][ 3 ].x ;
	Vertex[ 2 ].y = VPosD[ 0 ][ 3 ].y ;

	Vertex[ 0 ].u = LineUV[ 1 ] ;	Vertex[ 0 ].v = LineUV[ 1 ] ;
	Vertex[ 1 ].u = LineUV[ 1 ] ;	Vertex[ 1 ].v = LineUV[ 1 ] ;
	Vertex[ 2 ].u = LineUV[ 1 ] ;	Vertex[ 2 ].v = LineUV[ 1 ] ;

	Graphics_Draw_DrawSimpleTriangleGraphF( &Param ) ;

END :

	if( OldDrawMode != -1 )
	{
		NS_SetDrawMode( OldDrawMode ) ;
	}

	Graphics_DrawSetting_SetDrawBrightToOneParam( OrigColor ) ;

	if( NormalDraw )
	{
		return NS_DrawTriangle( _FTOL( x1 ), _FTOL( y1 ), _FTOL( x2 ), _FTOL( y2 ), _FTOL( x3 ), _FTOL( y3 ), Color, TRUE ) ;
	}

	// �I��
	return 0 ;
}

// �O�p�`��`�悷��( �A���`�G�C���A�X�t�� )
extern int NS_DrawTriangleAA( float x1, float y1, float x2, float y2, float x3, float y3, unsigned int Color, int FillFlag, float LineThickness )
{
	if( FillFlag == FALSE )
	{
		return DrawTriangleLineAA( x1, y1, x2, y2, x3, y3, Color, LineThickness ) ;
	}
	else
	{
		return DrawTriangleFillAA( x1, y1, x2, y2, x3, y3, Color ) ;
	}
}

// �l�p�`�̕`��
#define SETDRAWRECTCODE\
	int x[ 4 ], y[ 4 ], MaxX, MaxY, MinX, MinY ;\
	int i, ix, iy ;\
\
	x[ 0 ] = x1 ; x[ 1 ] = x2 ; x[ 2 ] = x3 ; x[ 3 ] = x4 ;\
	y[ 0 ] = y1 ; y[ 1 ] = y2 ; y[ 2 ] = y3 ; y[ 3 ] = y4 ;\
\
	MaxX = 0 ; MaxY = 0 ;\
	MinX = 0xffffff ; MinY = 0xffffff ;\
\
	for( i = 0 ; i < 4 ; i ++ )\
	{\
		ix = x[ i ] ; iy = y[ i ] ;\
		if( ix > MaxX ) MaxX = ix ; if( iy > MaxY ) MaxY = iy ;\
		if( ix < MinX ) MinX = ix ; if( iy < MinY ) MinY = iy ;\
	}\
\
	SETRECT( DrawRect, MinX, MinY, MaxX, MaxY ) ;\
	DRAWRECT_DRAWAREA_CLIP

extern	int NS_DrawQuadrangle( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, unsigned int Color, int FillFlag )
{
	int Ret = -1 ;
	int Flag ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;
	
	Flag = GSYS.Setting.ValidHardware ;

	CheckActiveState() ;

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawQuadrangle_PF( x1, y1, x2, y2, x3, y3, x4, y4, Color, FillFlag ),
		Graphics_Software_DrawQuadrangle(    x1, y1, x2, y2, x3, y3, x4, y4, Color, FillFlag ),
		SETDRAWRECTCODE,
		Ret,
		Flag
	)

	// �I��
	return Ret ;
}

#undef SETDRAWRECTCODE

// �l�p�`��`�悷��( �A���`�G�C���A�X�t�� )
static int DrawQuadrangleLineAA( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int Color, float LineThickness )
{
	int OldDrawMode = -1 ;
	int GrHandle ;
	GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM Param ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX ParamVertex[ ( 4 * 12 ) * 4 ] ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *Vertex ;
	VECTOR V1Dir, V1Pos, V1Norm, V1Line[ 4 ] ;
	VECTOR V2Dir, V2Pos, V2Norm, V2Line[ 4 ] ;
	VECTOR V3Dir, V3Pos, V3Norm, V3Line[ 4 ] ;
	VECTOR V4Dir, V4Pos, V4Norm, V4Line[ 4 ] ;
//	VECTOR V12Dir ;
//	VECTOR V13Dir ;
//	VECTOR V14Dir ;
	VECTOR VPosD[ 4 ][ 4 ][ 4 ] ;
	float Distance ;
	VECTOR TempPos ;
//	float P1P2X ;
//	float P1P2Y ;
//	float P1P3X ;
//	float P1P3Y ;
//	float P1P4X ;
//	float P1P4Y ;
//	float TempF ;
	float tmp1 ;
	float tmp2 ;
	float tmp3 ;
	float tmp4 ;
	int i ;
	int n ;
	DWORD FColor ;
	DWORD OrigColor ;
	static float Line_Out_UV[ 4 ] = { 0.0f,                    4.0f / LINEBOX_TEX_SIZE, 12.0f / LINEBOX_TEX_SIZE, 1.0f } ;
	static float Line_In_UV[ 4 ]  = { 5.0f / LINEBOX_TEX_SIZE, 8.0f / LINEBOX_TEX_SIZE, 8.0f  / LINEBOX_TEX_SIZE, 11.0f / LINEBOX_TEX_SIZE } ;

	if( NS_GetDrawFloatCoordType() == DX_DRAWFLOATCOORDTYPE_DIRECT3D9 )
	{
		x1 -= 0.5f ;
		y1 -= 0.5f ;
		x2 -= 0.5f ;
		y2 -= 0.5f ;
		x3 -= 0.5f ;
		y3 -= 0.5f ;
		x4 -= 0.5f ;
		y4 -= 0.5f ;
	}

	SETUP32BITCOLOR( Color, FColor ) ;
	OrigColor = GSYS.DrawSetting.bDrawBright ;
	Graphics_DrawSetting_SetDrawBrightToOneParam( FColor ) ;

	if( GSYS.DrawSetting.DrawMode != DX_DRAWMODE_BILINEAR )
	{
		OldDrawMode = GSYS.DrawSetting.DrawMode ;
		NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	}

	Param.Vertex      = ParamVertex ;
	Param.TransFlag   = TRUE ;
	if( LineThickness <= 1.0f )
	{
		Param.QuadrangleNum = 8 ;

		GrHandle = Graphics_Image_GetLineBoxOneThicknessTexHandle( IS_PMA_DRAWBLENDMODE ) ;

		Param.GraphHandle = GrHandle ;

		LineThickness /= 2.0f ;
		tmp1 = LineThickness + 1.5f ;
		tmp2 = LineThickness - 2.5f ;

//		P1P2X = x2 - x1 ;
//		P1P2Y = y2 - y1 ;
//
//		P1P3X = x3 - x1 ;
//		P1P3Y = y3 - y1 ;
//
//		P1P4X = x3 - x1 ;
//		P1P4Y = y3 - y1 ;
//
//		if( P1P2X * P1P3Y - P1P2Y * P1P3X < 0.0f )
//		{
//			TempF = x2 ; x2 = x3 ; x3 = TempF ;
//			TempF = y2 ; y2 = y3 ; y3 = TempF ;
//		}

		V1Pos.x  = x1 ;			V1Pos.y  = y1 ;			V1Pos.z  = 0.0f ;
		V1Dir.x  = x2 - x1 ;	V1Dir.y  = y2 - y1 ;	V1Dir.z  = 0.0f ;
//		V12Dir = V1Dir;
		Distance = V1Dir.x * V1Dir.x + V1Dir.y * V1Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V1Dir.x /= Distance ;
		V1Dir.y /= Distance ;
		V1Norm.x =  V1Dir.y ;
		V1Norm.y = -V1Dir.x ;
		V1Norm.z =  0.0f ;
		TempPos = VAdd( V1Pos, VScale( V1Norm, tmp1 ) ) ;
		V1Line[ 0 ].x =  V1Dir.y ;
		V1Line[ 0 ].y = -V1Dir.x ;
		V1Line[ 0 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;
		TempPos = VAdd( V1Pos, VScale( V1Norm, tmp2 ) ) ;
		V1Line[ 1 ].x =  V1Dir.y ;
		V1Line[ 1 ].y = -V1Dir.x ;
		V1Line[ 1 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;

		V2Pos.x  = x2 ;			V2Pos.y  = y2 ;			V2Pos.z  = 0.0f ;
		V2Dir.x  = x3 - x2 ;	V2Dir.y  = y3 - y2 ;	V2Dir.z  = 0.0f ;
		Distance = V2Dir.x * V2Dir.x + V2Dir.y * V2Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V2Dir.x /= Distance ;
		V2Dir.y /= Distance ;
		V2Norm.x =  V2Dir.y ;
		V2Norm.y = -V2Dir.x ;
		V2Norm.z =  0.0f ;
		TempPos = VAdd( V2Pos, VScale( V2Norm, tmp1 ) ) ;
		V2Line[ 0 ].x =  V2Dir.y ;
		V2Line[ 0 ].y = -V2Dir.x ;
		V2Line[ 0 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;
		TempPos = VAdd( V2Pos, VScale( V2Norm, tmp2 ) ) ;
		V2Line[ 1 ].x =  V2Dir.y ;
		V2Line[ 1 ].y = -V2Dir.x ;
		V2Line[ 1 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;

//		V13Dir.x = x3 - x1 ;    V13Dir.y = y3 - y1 ;	V13Dir.z = 0.0f ;
//		float SizeSq = VSquareSize( VCross( V12Dir, V13Dir ) ) ;
//		if( SizeSq / 2.0f < 7500.0f )
//		{
//			NS_DrawTriangle( _FTOL( x1 ), _FTOL( y1 ), _FTOL( x2 ), _FTOL( y2 ), _FTOL( x3 ), _FTOL( y3 ), Color, TRUE ) ;
//			goto END ;
//		}

		V3Pos.x  = x3 ;			V3Pos.y  = y3 ;			V3Pos.z  = 0.0f ;
		V3Dir.x  = x4 - x3 ;	V3Dir.y  = y4 - y3 ;	V3Dir.z  = 0.0f ;
		Distance = V3Dir.x * V3Dir.x + V3Dir.y * V3Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V3Dir.x /= Distance ;
		V3Dir.y /= Distance ;
		V3Norm.x =  V3Dir.y ;
		V3Norm.y = -V3Dir.x ;
		V3Norm.z =  0.0f ;
		TempPos = VAdd( V3Pos, VScale( V3Norm, tmp1 ) ) ;
		V3Line[ 0 ].x =  V3Dir.y ;
		V3Line[ 0 ].y = -V3Dir.x ;
		V3Line[ 0 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;
		TempPos = VAdd( V3Pos, VScale( V3Norm, tmp2 ) ) ;
		V3Line[ 1 ].x =  V3Dir.y ;
		V3Line[ 1 ].y = -V3Dir.x ;
		V3Line[ 1 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;

		V4Pos.x  = x4 ;			V4Pos.y  = y4 ;			V4Pos.z  = 0.0f ;
		V4Dir.x  = x1 - x4 ;	V4Dir.y  = y1 - y4 ;	V4Dir.z  = 0.0f ;
		Distance = V4Dir.x * V4Dir.x + V4Dir.y * V4Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V4Dir.x /= Distance ;
		V4Dir.y /= Distance ;
		V4Norm.x =  V4Dir.y ;
		V4Norm.y = -V4Dir.x ;
		V4Norm.z =  0.0f ;
		TempPos = VAdd( V4Pos, VScale( V4Norm, tmp1 ) ) ;
		V4Line[ 0 ].x =  V4Dir.y ;
		V4Line[ 0 ].y = -V4Dir.x ;
		V4Line[ 0 ].z = ( TempPos.x + V4Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V4Dir.y ) ;
		TempPos = VAdd( V4Pos, VScale( V4Norm, tmp2 ) ) ;
		V4Line[ 1 ].x =  V4Dir.y ;
		V4Line[ 1 ].y = -V4Dir.x ;
		V4Line[ 1 ].z = ( TempPos.x + V4Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V4Dir.y ) ;


		VPosD[ 0 ][ 0 ][ 0 ].x = ( V4Line[ 0 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V4Line[ 0 ].y ) / ( V1Line[ 0 ].x * V4Line[ 0 ].y - V4Line[ 0 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 0 ].y = ( V1Line[ 0 ].x * V4Line[ 0 ].z - V1Line[ 0 ].z * V4Line[ 0 ].x ) / ( V1Line[ 0 ].y * V4Line[ 0 ].x - V1Line[ 0 ].x * V4Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 1 ].x = ( V4Line[ 1 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V4Line[ 1 ].y ) / ( V1Line[ 0 ].x * V4Line[ 1 ].y - V4Line[ 1 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 1 ].y = ( V1Line[ 0 ].x * V4Line[ 1 ].z - V1Line[ 0 ].z * V4Line[ 1 ].x ) / ( V1Line[ 0 ].y * V4Line[ 1 ].x - V1Line[ 0 ].x * V4Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 0 ].x = ( V4Line[ 0 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V4Line[ 0 ].y ) / ( V1Line[ 1 ].x * V4Line[ 0 ].y - V4Line[ 0 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 0 ].y = ( V1Line[ 1 ].x * V4Line[ 0 ].z - V1Line[ 1 ].z * V4Line[ 0 ].x ) / ( V1Line[ 1 ].y * V4Line[ 0 ].x - V1Line[ 1 ].x * V4Line[ 0 ].y ) ;
		VPosD[ 0 ][ 1 ][ 1 ].x = ( V4Line[ 1 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V4Line[ 1 ].y ) / ( V1Line[ 1 ].x * V4Line[ 1 ].y - V4Line[ 1 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 1 ].y = ( V1Line[ 1 ].x * V4Line[ 1 ].z - V1Line[ 1 ].z * V4Line[ 1 ].x ) / ( V1Line[ 1 ].y * V4Line[ 1 ].x - V1Line[ 1 ].x * V4Line[ 1 ].y ) ;

		VPosD[ 1 ][ 0 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 0 ].y ) / ( V2Line[ 0 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 0 ].y = ( V2Line[ 0 ].x * V1Line[ 0 ].z - V2Line[ 0 ].z * V1Line[ 0 ].x ) / ( V2Line[ 0 ].y * V1Line[ 0 ].x - V2Line[ 0 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 1 ].y ) / ( V2Line[ 0 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 1 ].y = ( V2Line[ 0 ].x * V1Line[ 1 ].z - V2Line[ 0 ].z * V1Line[ 1 ].x ) / ( V2Line[ 0 ].y * V1Line[ 1 ].x - V2Line[ 0 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 0 ].y ) / ( V2Line[ 1 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 0 ].y = ( V2Line[ 1 ].x * V1Line[ 0 ].z - V2Line[ 1 ].z * V1Line[ 0 ].x ) / ( V2Line[ 1 ].y * V1Line[ 0 ].x - V2Line[ 1 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 1 ][ 1 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 1 ].y ) / ( V2Line[ 1 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 1 ].y = ( V2Line[ 1 ].x * V1Line[ 1 ].z - V2Line[ 1 ].z * V1Line[ 1 ].x ) / ( V2Line[ 1 ].y * V1Line[ 1 ].x - V2Line[ 1 ].x * V1Line[ 1 ].y ) ;

		VPosD[ 2 ][ 0 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 0 ].y ) / ( V3Line[ 0 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 0 ].y = ( V3Line[ 0 ].x * V2Line[ 0 ].z - V3Line[ 0 ].z * V2Line[ 0 ].x ) / ( V3Line[ 0 ].y * V2Line[ 0 ].x - V3Line[ 0 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 1 ].y ) / ( V3Line[ 0 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 1 ].y = ( V3Line[ 0 ].x * V2Line[ 1 ].z - V3Line[ 0 ].z * V2Line[ 1 ].x ) / ( V3Line[ 0 ].y * V2Line[ 1 ].x - V3Line[ 0 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 0 ].y ) / ( V3Line[ 1 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 0 ].y = ( V3Line[ 1 ].x * V2Line[ 0 ].z - V3Line[ 1 ].z * V2Line[ 0 ].x ) / ( V3Line[ 1 ].y * V2Line[ 0 ].x - V3Line[ 1 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 2 ][ 1 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 1 ].y ) / ( V3Line[ 1 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 1 ].y = ( V3Line[ 1 ].x * V2Line[ 1 ].z - V3Line[ 1 ].z * V2Line[ 1 ].x ) / ( V3Line[ 1 ].y * V2Line[ 1 ].x - V3Line[ 1 ].x * V2Line[ 1 ].y ) ;

		VPosD[ 3 ][ 0 ][ 0 ].x = ( V3Line[ 0 ].z * V4Line[ 0 ].y - V4Line[ 0 ].z * V3Line[ 0 ].y ) / ( V4Line[ 0 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V4Line[ 0 ].y ) ;
		VPosD[ 3 ][ 0 ][ 0 ].y = ( V4Line[ 0 ].x * V3Line[ 0 ].z - V4Line[ 0 ].z * V3Line[ 0 ].x ) / ( V4Line[ 0 ].y * V3Line[ 0 ].x - V4Line[ 0 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 3 ][ 0 ][ 1 ].x = ( V3Line[ 1 ].z * V4Line[ 0 ].y - V4Line[ 0 ].z * V3Line[ 1 ].y ) / ( V4Line[ 0 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V4Line[ 0 ].y ) ;
		VPosD[ 3 ][ 0 ][ 1 ].y = ( V4Line[ 0 ].x * V3Line[ 1 ].z - V4Line[ 0 ].z * V3Line[ 1 ].x ) / ( V4Line[ 0 ].y * V3Line[ 1 ].x - V4Line[ 0 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 3 ][ 1 ][ 0 ].x = ( V3Line[ 0 ].z * V4Line[ 1 ].y - V4Line[ 1 ].z * V3Line[ 0 ].y ) / ( V4Line[ 1 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V4Line[ 1 ].y ) ;
		VPosD[ 3 ][ 1 ][ 0 ].y = ( V4Line[ 1 ].x * V3Line[ 0 ].z - V4Line[ 1 ].z * V3Line[ 0 ].x ) / ( V4Line[ 1 ].y * V3Line[ 0 ].x - V4Line[ 1 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 3 ][ 1 ][ 1 ].x = ( V3Line[ 1 ].z * V4Line[ 1 ].y - V4Line[ 1 ].z * V3Line[ 1 ].y ) / ( V4Line[ 1 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V4Line[ 1 ].y ) ;
		VPosD[ 3 ][ 1 ][ 1 ].y = ( V4Line[ 1 ].x * V3Line[ 1 ].z - V4Line[ 1 ].z * V3Line[ 1 ].x ) / ( V4Line[ 1 ].y * V3Line[ 1 ].x - V4Line[ 1 ].x * V3Line[ 1 ].y ) ;

		Vertex = ParamVertex ;
		n = 1 ;
		for( i = 0 ; i < 4 ; i++ )
		{
			Vertex[ 0 ].x = VPosD[ i ][ 0 ][ 1 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 0 ][ 1 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 0 ][ 0 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 0 ][ 0 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 1 ][ 1 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 1 ][ 1 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 1 ][ 0 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 1 ][ 0 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;



			Vertex[ 0 ].x = VPosD[ n ][ 1 ][ 0 ].x ;
			Vertex[ 0 ].y = VPosD[ n ][ 1 ][ 0 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 0 ][ 1 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 0 ][ 1 ].y ;
			Vertex[ 2 ].x = VPosD[ n ][ 1 ][ 1 ].x ;
			Vertex[ 2 ].y = VPosD[ n ][ 1 ][ 1 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 1 ][ 1 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 1 ][ 1 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;


			n = n == 3 ? 0 : n + 1 ;
		}
	}
	else
	{
		Param.QuadrangleNum = 4 * 12 ;

		GrHandle = Graphics_Image_GetLineBoxTexHandle( IS_PMA_DRAWBLENDMODE ) ;

		Param.GraphHandle = GrHandle ;

		LineThickness /= 2.0f ;
		tmp1 =  LineThickness + 1.5f ;
		tmp2 =  LineThickness - 2.5f ;
		tmp3 = -LineThickness + 1.5f ;
		tmp4 = -LineThickness - 2.5f ;

//		P1P2X = x2 - x1 ;
//		P1P2Y = y2 - y1 ;
//
//		P1P3X = x3 - x1 ;
//		P1P3Y = y3 - y1 ;
//
//		P1P4X = x4 - x1 ;
//		P1P4Y = y4 - y1 ;
//
//		if( P1P2X * P1P3Y - P1P2Y * P1P3X < 0.0f )
//		{
//			TempF = x2 ; x2 = x3 ; x3 = TempF ;
//			TempF = y2 ; y2 = y3 ; y3 = TempF ;
//		}

		V1Pos.x  = x1 ;			V1Pos.y  = y1 ;			V1Pos.z  = 0.0f ;
		V1Dir.x  = x2 - x1 ;	V1Dir.y  = y2 - y1 ;	V1Dir.z  = 0.0f ;
//		V12Dir = V1Dir;
		Distance = V1Dir.x * V1Dir.x + V1Dir.y * V1Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V1Dir.x /= Distance ;
		V1Dir.y /= Distance ;
		V1Norm.x =  V1Dir.y ;
		V1Norm.y = -V1Dir.x ;
		V1Norm.z =  0.0f ;
		TempPos = VAdd( V1Pos, VScale( V1Norm, tmp1 ) ) ;
		V1Line[ 0 ].x =  V1Dir.y ;
		V1Line[ 0 ].y = -V1Dir.x ;
		V1Line[ 0 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;
		TempPos = VAdd( V1Pos, VScale( V1Norm, tmp2 ) ) ;
		V1Line[ 1 ].x =  V1Dir.y ;
		V1Line[ 1 ].y = -V1Dir.x ;
		V1Line[ 1 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;
		TempPos = VAdd( V1Pos, VScale( V1Norm, tmp3 ) ) ;
		V1Line[ 2 ].x =  V1Dir.y ;
		V1Line[ 2 ].y = -V1Dir.x ;
		V1Line[ 2 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;
		TempPos = VAdd( V1Pos, VScale( V1Norm, tmp4 ) ) ;
		V1Line[ 3 ].x =  V1Dir.y ;
		V1Line[ 3 ].y = -V1Dir.x ;
		V1Line[ 3 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;

		V2Pos.x  = x2 ;			V2Pos.y  = y2 ;			V2Pos.z  = 0.0f ;
		V2Dir.x  = x3 - x2 ;	V2Dir.y  = y3 - y2 ;	V2Dir.z  = 0.0f ;
		Distance = V2Dir.x * V2Dir.x + V2Dir.y * V2Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V2Dir.x /= Distance ;
		V2Dir.y /= Distance ;
		V2Norm.x =  V2Dir.y ;
		V2Norm.y = -V2Dir.x ;
		V2Norm.z =  0.0f ;
		TempPos = VAdd( V2Pos, VScale( V2Norm, tmp1 ) ) ;
		V2Line[ 0 ].x =  V2Dir.y ;
		V2Line[ 0 ].y = -V2Dir.x ;
		V2Line[ 0 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;
		TempPos = VAdd( V2Pos, VScale( V2Norm, tmp2 ) ) ;
		V2Line[ 1 ].x =  V2Dir.y ;
		V2Line[ 1 ].y = -V2Dir.x ;
		V2Line[ 1 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;
		TempPos = VAdd( V2Pos, VScale( V2Norm, tmp3 ) ) ;
		V2Line[ 2 ].x =  V2Dir.y ;
		V2Line[ 2 ].y = -V2Dir.x ;
		V2Line[ 2 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;
		TempPos = VAdd( V2Pos, VScale( V2Norm, tmp4 ) ) ;
		V2Line[ 3 ].x =  V2Dir.y ;
		V2Line[ 3 ].y = -V2Dir.x ;
		V2Line[ 3 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;

//		V13Dir.x = x3 - x1 ;    V13Dir.y = y3 - y1 ;	V13Dir.z = 0.0f ;
//		float SizeSq = VSquareSize( VCross( V12Dir, V13Dir ) ) ;
//		if( SizeSq / 2.0f < 7500.0f )
//		{
//			NS_DrawTriangle( _FTOL( x1 ), _FTOL( y1 ), _FTOL( x2 ), _FTOL( y2 ), _FTOL( x3 ), _FTOL( y3 ), Color, TRUE ) ;
//			goto END ;
//		}

		V3Pos.x  = x3 ;			V3Pos.y  = y3 ;			V3Pos.z  = 0.0f ;
		V3Dir.x  = x4 - x3 ;	V3Dir.y  = y4 - y3 ;	V3Dir.z  = 0.0f ;
		Distance = V3Dir.x * V3Dir.x + V3Dir.y * V3Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V3Dir.x /= Distance ;
		V3Dir.y /= Distance ;
		V3Norm.x =  V3Dir.y ;
		V3Norm.y = -V3Dir.x ;
		V3Norm.z =  0.0f ;
		TempPos = VAdd( V3Pos, VScale( V3Norm, tmp1 ) ) ;
		V3Line[ 0 ].x =  V3Dir.y ;
		V3Line[ 0 ].y = -V3Dir.x ;
		V3Line[ 0 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;
		TempPos = VAdd( V3Pos, VScale( V3Norm, tmp2 ) ) ;
		V3Line[ 1 ].x =  V3Dir.y ;
		V3Line[ 1 ].y = -V3Dir.x ;
		V3Line[ 1 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;
		TempPos = VAdd( V3Pos, VScale( V3Norm, tmp3 ) ) ;
		V3Line[ 2 ].x =  V3Dir.y ;
		V3Line[ 2 ].y = -V3Dir.x ;
		V3Line[ 2 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;
		TempPos = VAdd( V3Pos, VScale( V3Norm, tmp4 ) ) ;
		V3Line[ 3 ].x =  V3Dir.y ;
		V3Line[ 3 ].y = -V3Dir.x ;
		V3Line[ 3 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;

		V4Pos.x  = x4 ;			V4Pos.y  = y4 ;			V4Pos.z  = 0.0f ;
		V4Dir.x  = x1 - x4 ;	V4Dir.y  = y1 - y4 ;	V4Dir.z  = 0.0f ;
		Distance = V4Dir.x * V4Dir.x + V4Dir.y * V4Dir.y ;
		if( Distance < 0.000001f )
		{
			goto END ;
		}
		Distance = _SQRT( Distance ) ;
		V4Dir.x /= Distance ;
		V4Dir.y /= Distance ;
		V4Norm.x =  V4Dir.y ;
		V4Norm.y = -V4Dir.x ;
		V4Norm.z =  0.0f ;
		TempPos = VAdd( V4Pos, VScale( V4Norm, tmp1 ) ) ;
		V4Line[ 0 ].x =  V4Dir.y ;
		V4Line[ 0 ].y = -V4Dir.x ;
		V4Line[ 0 ].z = ( TempPos.x + V4Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V4Dir.y ) ;
		TempPos = VAdd( V4Pos, VScale( V4Norm, tmp2 ) ) ;
		V4Line[ 1 ].x =  V4Dir.y ;
		V4Line[ 1 ].y = -V4Dir.x ;
		V4Line[ 1 ].z = ( TempPos.x + V4Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V4Dir.y ) ;
		TempPos = VAdd( V4Pos, VScale( V4Norm, tmp3 ) ) ;
		V4Line[ 2 ].x =  V4Dir.y ;
		V4Line[ 2 ].y = -V4Dir.x ;
		V4Line[ 2 ].z = ( TempPos.x + V4Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V4Dir.y ) ;
		TempPos = VAdd( V4Pos, VScale( V4Norm, tmp4 ) ) ;
		V4Line[ 3 ].x =  V4Dir.y ;
		V4Line[ 3 ].y = -V4Dir.x ;
		V4Line[ 3 ].z = ( TempPos.x + V4Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V4Dir.y ) ;


		VPosD[ 0 ][ 0 ][ 0 ].x = ( V4Line[ 0 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V4Line[ 0 ].y ) / ( V1Line[ 0 ].x * V4Line[ 0 ].y - V4Line[ 0 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 0 ].y = ( V1Line[ 0 ].x * V4Line[ 0 ].z - V1Line[ 0 ].z * V4Line[ 0 ].x ) / ( V1Line[ 0 ].y * V4Line[ 0 ].x - V1Line[ 0 ].x * V4Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 1 ].x = ( V4Line[ 1 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V4Line[ 1 ].y ) / ( V1Line[ 0 ].x * V4Line[ 1 ].y - V4Line[ 1 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 1 ].y = ( V1Line[ 0 ].x * V4Line[ 1 ].z - V1Line[ 0 ].z * V4Line[ 1 ].x ) / ( V1Line[ 0 ].y * V4Line[ 1 ].x - V1Line[ 0 ].x * V4Line[ 1 ].y ) ;
		VPosD[ 0 ][ 0 ][ 2 ].x = ( V4Line[ 2 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V4Line[ 2 ].y ) / ( V1Line[ 0 ].x * V4Line[ 2 ].y - V4Line[ 2 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 2 ].y = ( V1Line[ 0 ].x * V4Line[ 2 ].z - V1Line[ 0 ].z * V4Line[ 2 ].x ) / ( V1Line[ 0 ].y * V4Line[ 2 ].x - V1Line[ 0 ].x * V4Line[ 2 ].y ) ;
		VPosD[ 0 ][ 0 ][ 3 ].x = ( V4Line[ 3 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V4Line[ 3 ].y ) / ( V1Line[ 0 ].x * V4Line[ 3 ].y - V4Line[ 3 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 0 ][ 0 ][ 3 ].y = ( V1Line[ 0 ].x * V4Line[ 3 ].z - V1Line[ 0 ].z * V4Line[ 3 ].x ) / ( V1Line[ 0 ].y * V4Line[ 3 ].x - V1Line[ 0 ].x * V4Line[ 3 ].y ) ;
		VPosD[ 0 ][ 1 ][ 0 ].x = ( V4Line[ 0 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V4Line[ 0 ].y ) / ( V1Line[ 1 ].x * V4Line[ 0 ].y - V4Line[ 0 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 0 ].y = ( V1Line[ 1 ].x * V4Line[ 0 ].z - V1Line[ 1 ].z * V4Line[ 0 ].x ) / ( V1Line[ 1 ].y * V4Line[ 0 ].x - V1Line[ 1 ].x * V4Line[ 0 ].y ) ;
		VPosD[ 0 ][ 1 ][ 1 ].x = ( V4Line[ 1 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V4Line[ 1 ].y ) / ( V1Line[ 1 ].x * V4Line[ 1 ].y - V4Line[ 1 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 1 ].y = ( V1Line[ 1 ].x * V4Line[ 1 ].z - V1Line[ 1 ].z * V4Line[ 1 ].x ) / ( V1Line[ 1 ].y * V4Line[ 1 ].x - V1Line[ 1 ].x * V4Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 2 ].x = ( V4Line[ 2 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V4Line[ 2 ].y ) / ( V1Line[ 1 ].x * V4Line[ 2 ].y - V4Line[ 2 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 2 ].y = ( V1Line[ 1 ].x * V4Line[ 2 ].z - V1Line[ 1 ].z * V4Line[ 2 ].x ) / ( V1Line[ 1 ].y * V4Line[ 2 ].x - V1Line[ 1 ].x * V4Line[ 2 ].y ) ;
		VPosD[ 0 ][ 1 ][ 3 ].x = ( V4Line[ 3 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V4Line[ 3 ].y ) / ( V1Line[ 1 ].x * V4Line[ 3 ].y - V4Line[ 3 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 0 ][ 1 ][ 3 ].y = ( V1Line[ 1 ].x * V4Line[ 3 ].z - V1Line[ 1 ].z * V4Line[ 3 ].x ) / ( V1Line[ 1 ].y * V4Line[ 3 ].x - V1Line[ 1 ].x * V4Line[ 3 ].y ) ;
		VPosD[ 0 ][ 2 ][ 0 ].x = ( V4Line[ 0 ].z * V1Line[ 2 ].y - V1Line[ 2 ].z * V4Line[ 0 ].y ) / ( V1Line[ 2 ].x * V4Line[ 0 ].y - V4Line[ 0 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 0 ][ 2 ][ 0 ].y = ( V1Line[ 2 ].x * V4Line[ 0 ].z - V1Line[ 2 ].z * V4Line[ 0 ].x ) / ( V1Line[ 2 ].y * V4Line[ 0 ].x - V1Line[ 2 ].x * V4Line[ 0 ].y ) ;
		VPosD[ 0 ][ 2 ][ 1 ].x = ( V4Line[ 1 ].z * V1Line[ 2 ].y - V1Line[ 2 ].z * V4Line[ 1 ].y ) / ( V1Line[ 2 ].x * V4Line[ 1 ].y - V4Line[ 1 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 0 ][ 2 ][ 1 ].y = ( V1Line[ 2 ].x * V4Line[ 1 ].z - V1Line[ 2 ].z * V4Line[ 1 ].x ) / ( V1Line[ 2 ].y * V4Line[ 1 ].x - V1Line[ 2 ].x * V4Line[ 1 ].y ) ;
		VPosD[ 0 ][ 2 ][ 2 ].x = ( V4Line[ 2 ].z * V1Line[ 2 ].y - V1Line[ 2 ].z * V4Line[ 2 ].y ) / ( V1Line[ 2 ].x * V4Line[ 2 ].y - V4Line[ 2 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 0 ][ 2 ][ 2 ].y = ( V1Line[ 2 ].x * V4Line[ 2 ].z - V1Line[ 2 ].z * V4Line[ 2 ].x ) / ( V1Line[ 2 ].y * V4Line[ 2 ].x - V1Line[ 2 ].x * V4Line[ 2 ].y ) ;
		VPosD[ 0 ][ 2 ][ 3 ].x = ( V4Line[ 3 ].z * V1Line[ 2 ].y - V1Line[ 2 ].z * V4Line[ 3 ].y ) / ( V1Line[ 2 ].x * V4Line[ 3 ].y - V4Line[ 3 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 0 ][ 2 ][ 3 ].y = ( V1Line[ 2 ].x * V4Line[ 3 ].z - V1Line[ 2 ].z * V4Line[ 3 ].x ) / ( V1Line[ 2 ].y * V4Line[ 3 ].x - V1Line[ 2 ].x * V4Line[ 3 ].y ) ;
		VPosD[ 0 ][ 3 ][ 0 ].x = ( V4Line[ 0 ].z * V1Line[ 3 ].y - V1Line[ 3 ].z * V4Line[ 0 ].y ) / ( V1Line[ 3 ].x * V4Line[ 0 ].y - V4Line[ 0 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 0 ][ 3 ][ 0 ].y = ( V1Line[ 3 ].x * V4Line[ 0 ].z - V1Line[ 3 ].z * V4Line[ 0 ].x ) / ( V1Line[ 3 ].y * V4Line[ 0 ].x - V1Line[ 3 ].x * V4Line[ 0 ].y ) ;
		VPosD[ 0 ][ 3 ][ 1 ].x = ( V4Line[ 1 ].z * V1Line[ 3 ].y - V1Line[ 3 ].z * V4Line[ 1 ].y ) / ( V1Line[ 3 ].x * V4Line[ 1 ].y - V4Line[ 1 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 0 ][ 3 ][ 1 ].y = ( V1Line[ 3 ].x * V4Line[ 1 ].z - V1Line[ 3 ].z * V4Line[ 1 ].x ) / ( V1Line[ 3 ].y * V4Line[ 1 ].x - V1Line[ 3 ].x * V4Line[ 1 ].y ) ;
		VPosD[ 0 ][ 3 ][ 2 ].x = ( V4Line[ 2 ].z * V1Line[ 3 ].y - V1Line[ 3 ].z * V4Line[ 2 ].y ) / ( V1Line[ 3 ].x * V4Line[ 2 ].y - V4Line[ 2 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 0 ][ 3 ][ 2 ].y = ( V1Line[ 3 ].x * V4Line[ 2 ].z - V1Line[ 3 ].z * V4Line[ 2 ].x ) / ( V1Line[ 3 ].y * V4Line[ 2 ].x - V1Line[ 3 ].x * V4Line[ 2 ].y ) ;
		VPosD[ 0 ][ 3 ][ 3 ].x = ( V4Line[ 3 ].z * V1Line[ 3 ].y - V1Line[ 3 ].z * V4Line[ 3 ].y ) / ( V1Line[ 3 ].x * V4Line[ 3 ].y - V4Line[ 3 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 0 ][ 3 ][ 3 ].y = ( V1Line[ 3 ].x * V4Line[ 3 ].z - V1Line[ 3 ].z * V4Line[ 3 ].x ) / ( V1Line[ 3 ].y * V4Line[ 3 ].x - V1Line[ 3 ].x * V4Line[ 3 ].y ) ;

		VPosD[ 1 ][ 0 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 0 ].y ) / ( V2Line[ 0 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 0 ].y = ( V2Line[ 0 ].x * V1Line[ 0 ].z - V2Line[ 0 ].z * V1Line[ 0 ].x ) / ( V2Line[ 0 ].y * V1Line[ 0 ].x - V2Line[ 0 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 1 ].y ) / ( V2Line[ 0 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 1 ].y = ( V2Line[ 0 ].x * V1Line[ 1 ].z - V2Line[ 0 ].z * V1Line[ 1 ].x ) / ( V2Line[ 0 ].y * V1Line[ 1 ].x - V2Line[ 0 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 1 ][ 0 ][ 2 ].x = ( V1Line[ 2 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 2 ].y ) / ( V2Line[ 0 ].x * V1Line[ 2 ].y - V1Line[ 2 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 2 ].y = ( V2Line[ 0 ].x * V1Line[ 2 ].z - V2Line[ 0 ].z * V1Line[ 2 ].x ) / ( V2Line[ 0 ].y * V1Line[ 2 ].x - V2Line[ 0 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 1 ][ 0 ][ 3 ].x = ( V1Line[ 3 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 3 ].y ) / ( V2Line[ 0 ].x * V1Line[ 3 ].y - V1Line[ 3 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 1 ][ 0 ][ 3 ].y = ( V2Line[ 0 ].x * V1Line[ 3 ].z - V2Line[ 0 ].z * V1Line[ 3 ].x ) / ( V2Line[ 0 ].y * V1Line[ 3 ].x - V2Line[ 0 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 1 ][ 1 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 0 ].y ) / ( V2Line[ 1 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 0 ].y = ( V2Line[ 1 ].x * V1Line[ 0 ].z - V2Line[ 1 ].z * V1Line[ 0 ].x ) / ( V2Line[ 1 ].y * V1Line[ 0 ].x - V2Line[ 1 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 1 ][ 1 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 1 ].y ) / ( V2Line[ 1 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 1 ].y = ( V2Line[ 1 ].x * V1Line[ 1 ].z - V2Line[ 1 ].z * V1Line[ 1 ].x ) / ( V2Line[ 1 ].y * V1Line[ 1 ].x - V2Line[ 1 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 2 ].x = ( V1Line[ 2 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 2 ].y ) / ( V2Line[ 1 ].x * V1Line[ 2 ].y - V1Line[ 2 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 2 ].y = ( V2Line[ 1 ].x * V1Line[ 2 ].z - V2Line[ 1 ].z * V1Line[ 2 ].x ) / ( V2Line[ 1 ].y * V1Line[ 2 ].x - V2Line[ 1 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 1 ][ 1 ][ 3 ].x = ( V1Line[ 3 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 3 ].y ) / ( V2Line[ 1 ].x * V1Line[ 3 ].y - V1Line[ 3 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 1 ][ 1 ][ 3 ].y = ( V2Line[ 1 ].x * V1Line[ 3 ].z - V2Line[ 1 ].z * V1Line[ 3 ].x ) / ( V2Line[ 1 ].y * V1Line[ 3 ].x - V2Line[ 1 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 1 ][ 2 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 2 ].y - V2Line[ 2 ].z * V1Line[ 0 ].y ) / ( V2Line[ 2 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 1 ][ 2 ][ 0 ].y = ( V2Line[ 2 ].x * V1Line[ 0 ].z - V2Line[ 2 ].z * V1Line[ 0 ].x ) / ( V2Line[ 2 ].y * V1Line[ 0 ].x - V2Line[ 2 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 1 ][ 2 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 2 ].y - V2Line[ 2 ].z * V1Line[ 1 ].y ) / ( V2Line[ 2 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 1 ][ 2 ][ 1 ].y = ( V2Line[ 2 ].x * V1Line[ 1 ].z - V2Line[ 2 ].z * V1Line[ 1 ].x ) / ( V2Line[ 2 ].y * V1Line[ 1 ].x - V2Line[ 2 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 1 ][ 2 ][ 2 ].x = ( V1Line[ 2 ].z * V2Line[ 2 ].y - V2Line[ 2 ].z * V1Line[ 2 ].y ) / ( V2Line[ 2 ].x * V1Line[ 2 ].y - V1Line[ 2 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 1 ][ 2 ][ 2 ].y = ( V2Line[ 2 ].x * V1Line[ 2 ].z - V2Line[ 2 ].z * V1Line[ 2 ].x ) / ( V2Line[ 2 ].y * V1Line[ 2 ].x - V2Line[ 2 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 1 ][ 2 ][ 3 ].x = ( V1Line[ 3 ].z * V2Line[ 2 ].y - V2Line[ 2 ].z * V1Line[ 3 ].y ) / ( V2Line[ 2 ].x * V1Line[ 3 ].y - V1Line[ 3 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 1 ][ 2 ][ 3 ].y = ( V2Line[ 2 ].x * V1Line[ 3 ].z - V2Line[ 2 ].z * V1Line[ 3 ].x ) / ( V2Line[ 2 ].y * V1Line[ 3 ].x - V2Line[ 2 ].x * V1Line[ 3 ].y ) ;
		VPosD[ 1 ][ 3 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 3 ].y - V2Line[ 3 ].z * V1Line[ 0 ].y ) / ( V2Line[ 3 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 1 ][ 3 ][ 0 ].y = ( V2Line[ 3 ].x * V1Line[ 0 ].z - V2Line[ 3 ].z * V1Line[ 0 ].x ) / ( V2Line[ 3 ].y * V1Line[ 0 ].x - V2Line[ 3 ].x * V1Line[ 0 ].y ) ;
		VPosD[ 1 ][ 3 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 3 ].y - V2Line[ 3 ].z * V1Line[ 1 ].y ) / ( V2Line[ 3 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 1 ][ 3 ][ 1 ].y = ( V2Line[ 3 ].x * V1Line[ 1 ].z - V2Line[ 3 ].z * V1Line[ 1 ].x ) / ( V2Line[ 3 ].y * V1Line[ 1 ].x - V2Line[ 3 ].x * V1Line[ 1 ].y ) ;
		VPosD[ 1 ][ 3 ][ 2 ].x = ( V1Line[ 2 ].z * V2Line[ 3 ].y - V2Line[ 3 ].z * V1Line[ 2 ].y ) / ( V2Line[ 3 ].x * V1Line[ 2 ].y - V1Line[ 2 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 1 ][ 3 ][ 2 ].y = ( V2Line[ 3 ].x * V1Line[ 2 ].z - V2Line[ 3 ].z * V1Line[ 2 ].x ) / ( V2Line[ 3 ].y * V1Line[ 2 ].x - V2Line[ 3 ].x * V1Line[ 2 ].y ) ;
		VPosD[ 1 ][ 3 ][ 3 ].x = ( V1Line[ 3 ].z * V2Line[ 3 ].y - V2Line[ 3 ].z * V1Line[ 3 ].y ) / ( V2Line[ 3 ].x * V1Line[ 3 ].y - V1Line[ 3 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 1 ][ 3 ][ 3 ].y = ( V2Line[ 3 ].x * V1Line[ 3 ].z - V2Line[ 3 ].z * V1Line[ 3 ].x ) / ( V2Line[ 3 ].y * V1Line[ 3 ].x - V2Line[ 3 ].x * V1Line[ 3 ].y ) ;

		VPosD[ 2 ][ 0 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 0 ].y ) / ( V3Line[ 0 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 0 ].y = ( V3Line[ 0 ].x * V2Line[ 0 ].z - V3Line[ 0 ].z * V2Line[ 0 ].x ) / ( V3Line[ 0 ].y * V2Line[ 0 ].x - V3Line[ 0 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 1 ].y ) / ( V3Line[ 0 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 1 ].y = ( V3Line[ 0 ].x * V2Line[ 1 ].z - V3Line[ 0 ].z * V2Line[ 1 ].x ) / ( V3Line[ 0 ].y * V2Line[ 1 ].x - V3Line[ 0 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 2 ][ 0 ][ 2 ].x = ( V2Line[ 2 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 2 ].y ) / ( V3Line[ 0 ].x * V2Line[ 2 ].y - V2Line[ 2 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 2 ].y = ( V3Line[ 0 ].x * V2Line[ 2 ].z - V3Line[ 0 ].z * V2Line[ 2 ].x ) / ( V3Line[ 0 ].y * V2Line[ 2 ].x - V3Line[ 0 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 2 ][ 0 ][ 3 ].x = ( V2Line[ 3 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 3 ].y ) / ( V3Line[ 0 ].x * V2Line[ 3 ].y - V2Line[ 3 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 2 ][ 0 ][ 3 ].y = ( V3Line[ 0 ].x * V2Line[ 3 ].z - V3Line[ 0 ].z * V2Line[ 3 ].x ) / ( V3Line[ 0 ].y * V2Line[ 3 ].x - V3Line[ 0 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 2 ][ 1 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 0 ].y ) / ( V3Line[ 1 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 0 ].y = ( V3Line[ 1 ].x * V2Line[ 0 ].z - V3Line[ 1 ].z * V2Line[ 0 ].x ) / ( V3Line[ 1 ].y * V2Line[ 0 ].x - V3Line[ 1 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 2 ][ 1 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 1 ].y ) / ( V3Line[ 1 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 1 ].y = ( V3Line[ 1 ].x * V2Line[ 1 ].z - V3Line[ 1 ].z * V2Line[ 1 ].x ) / ( V3Line[ 1 ].y * V2Line[ 1 ].x - V3Line[ 1 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 2 ].x = ( V2Line[ 2 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 2 ].y ) / ( V3Line[ 1 ].x * V2Line[ 2 ].y - V2Line[ 2 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 2 ].y = ( V3Line[ 1 ].x * V2Line[ 2 ].z - V3Line[ 1 ].z * V2Line[ 2 ].x ) / ( V3Line[ 1 ].y * V2Line[ 2 ].x - V3Line[ 1 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 2 ][ 1 ][ 3 ].x = ( V2Line[ 3 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 3 ].y ) / ( V3Line[ 1 ].x * V2Line[ 3 ].y - V2Line[ 3 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 2 ][ 1 ][ 3 ].y = ( V3Line[ 1 ].x * V2Line[ 3 ].z - V3Line[ 1 ].z * V2Line[ 3 ].x ) / ( V3Line[ 1 ].y * V2Line[ 3 ].x - V3Line[ 1 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 2 ][ 2 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 2 ].y - V3Line[ 2 ].z * V2Line[ 0 ].y ) / ( V3Line[ 2 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 2 ][ 2 ][ 0 ].y = ( V3Line[ 2 ].x * V2Line[ 0 ].z - V3Line[ 2 ].z * V2Line[ 0 ].x ) / ( V3Line[ 2 ].y * V2Line[ 0 ].x - V3Line[ 2 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 2 ][ 2 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 2 ].y - V3Line[ 2 ].z * V2Line[ 1 ].y ) / ( V3Line[ 2 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 2 ][ 2 ][ 1 ].y = ( V3Line[ 2 ].x * V2Line[ 1 ].z - V3Line[ 2 ].z * V2Line[ 1 ].x ) / ( V3Line[ 2 ].y * V2Line[ 1 ].x - V3Line[ 2 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 2 ][ 2 ][ 2 ].x = ( V2Line[ 2 ].z * V3Line[ 2 ].y - V3Line[ 2 ].z * V2Line[ 2 ].y ) / ( V3Line[ 2 ].x * V2Line[ 2 ].y - V2Line[ 2 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 2 ][ 2 ][ 2 ].y = ( V3Line[ 2 ].x * V2Line[ 2 ].z - V3Line[ 2 ].z * V2Line[ 2 ].x ) / ( V3Line[ 2 ].y * V2Line[ 2 ].x - V3Line[ 2 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 2 ][ 2 ][ 3 ].x = ( V2Line[ 3 ].z * V3Line[ 2 ].y - V3Line[ 2 ].z * V2Line[ 3 ].y ) / ( V3Line[ 2 ].x * V2Line[ 3 ].y - V2Line[ 3 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 2 ][ 2 ][ 3 ].y = ( V3Line[ 2 ].x * V2Line[ 3 ].z - V3Line[ 2 ].z * V2Line[ 3 ].x ) / ( V3Line[ 2 ].y * V2Line[ 3 ].x - V3Line[ 2 ].x * V2Line[ 3 ].y ) ;
		VPosD[ 2 ][ 3 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 3 ].y - V3Line[ 3 ].z * V2Line[ 0 ].y ) / ( V3Line[ 3 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 2 ][ 3 ][ 0 ].y = ( V3Line[ 3 ].x * V2Line[ 0 ].z - V3Line[ 3 ].z * V2Line[ 0 ].x ) / ( V3Line[ 3 ].y * V2Line[ 0 ].x - V3Line[ 3 ].x * V2Line[ 0 ].y ) ;
		VPosD[ 2 ][ 3 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 3 ].y - V3Line[ 3 ].z * V2Line[ 1 ].y ) / ( V3Line[ 3 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 2 ][ 3 ][ 1 ].y = ( V3Line[ 3 ].x * V2Line[ 1 ].z - V3Line[ 3 ].z * V2Line[ 1 ].x ) / ( V3Line[ 3 ].y * V2Line[ 1 ].x - V3Line[ 3 ].x * V2Line[ 1 ].y ) ;
		VPosD[ 2 ][ 3 ][ 2 ].x = ( V2Line[ 2 ].z * V3Line[ 3 ].y - V3Line[ 3 ].z * V2Line[ 2 ].y ) / ( V3Line[ 3 ].x * V2Line[ 2 ].y - V2Line[ 2 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 2 ][ 3 ][ 2 ].y = ( V3Line[ 3 ].x * V2Line[ 2 ].z - V3Line[ 3 ].z * V2Line[ 2 ].x ) / ( V3Line[ 3 ].y * V2Line[ 2 ].x - V3Line[ 3 ].x * V2Line[ 2 ].y ) ;
		VPosD[ 2 ][ 3 ][ 3 ].x = ( V2Line[ 3 ].z * V3Line[ 3 ].y - V3Line[ 3 ].z * V2Line[ 3 ].y ) / ( V3Line[ 3 ].x * V2Line[ 3 ].y - V2Line[ 3 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 2 ][ 3 ][ 3 ].y = ( V3Line[ 3 ].x * V2Line[ 3 ].z - V3Line[ 3 ].z * V2Line[ 3 ].x ) / ( V3Line[ 3 ].y * V2Line[ 3 ].x - V3Line[ 3 ].x * V2Line[ 3 ].y ) ;

		VPosD[ 3 ][ 0 ][ 0 ].x = ( V3Line[ 0 ].z * V4Line[ 0 ].y - V4Line[ 0 ].z * V3Line[ 0 ].y ) / ( V4Line[ 0 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V4Line[ 0 ].y ) ;
		VPosD[ 3 ][ 0 ][ 0 ].y = ( V4Line[ 0 ].x * V3Line[ 0 ].z - V4Line[ 0 ].z * V3Line[ 0 ].x ) / ( V4Line[ 0 ].y * V3Line[ 0 ].x - V4Line[ 0 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 3 ][ 0 ][ 1 ].x = ( V3Line[ 1 ].z * V4Line[ 0 ].y - V4Line[ 0 ].z * V3Line[ 1 ].y ) / ( V4Line[ 0 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V4Line[ 0 ].y ) ;
		VPosD[ 3 ][ 0 ][ 1 ].y = ( V4Line[ 0 ].x * V3Line[ 1 ].z - V4Line[ 0 ].z * V3Line[ 1 ].x ) / ( V4Line[ 0 ].y * V3Line[ 1 ].x - V4Line[ 0 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 3 ][ 0 ][ 2 ].x = ( V3Line[ 2 ].z * V4Line[ 0 ].y - V4Line[ 0 ].z * V3Line[ 2 ].y ) / ( V4Line[ 0 ].x * V3Line[ 2 ].y - V3Line[ 2 ].x * V4Line[ 0 ].y ) ;
		VPosD[ 3 ][ 0 ][ 2 ].y = ( V4Line[ 0 ].x * V3Line[ 2 ].z - V4Line[ 0 ].z * V3Line[ 2 ].x ) / ( V4Line[ 0 ].y * V3Line[ 2 ].x - V4Line[ 0 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 3 ][ 0 ][ 3 ].x = ( V3Line[ 3 ].z * V4Line[ 0 ].y - V4Line[ 0 ].z * V3Line[ 3 ].y ) / ( V4Line[ 0 ].x * V3Line[ 3 ].y - V3Line[ 3 ].x * V4Line[ 0 ].y ) ;
		VPosD[ 3 ][ 0 ][ 3 ].y = ( V4Line[ 0 ].x * V3Line[ 3 ].z - V4Line[ 0 ].z * V3Line[ 3 ].x ) / ( V4Line[ 0 ].y * V3Line[ 3 ].x - V4Line[ 0 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 3 ][ 1 ][ 0 ].x = ( V3Line[ 0 ].z * V4Line[ 1 ].y - V4Line[ 1 ].z * V3Line[ 0 ].y ) / ( V4Line[ 1 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V4Line[ 1 ].y ) ;
		VPosD[ 3 ][ 1 ][ 0 ].y = ( V4Line[ 1 ].x * V3Line[ 0 ].z - V4Line[ 1 ].z * V3Line[ 0 ].x ) / ( V4Line[ 1 ].y * V3Line[ 0 ].x - V4Line[ 1 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 3 ][ 1 ][ 1 ].x = ( V3Line[ 1 ].z * V4Line[ 1 ].y - V4Line[ 1 ].z * V3Line[ 1 ].y ) / ( V4Line[ 1 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V4Line[ 1 ].y ) ;
		VPosD[ 3 ][ 1 ][ 1 ].y = ( V4Line[ 1 ].x * V3Line[ 1 ].z - V4Line[ 1 ].z * V3Line[ 1 ].x ) / ( V4Line[ 1 ].y * V3Line[ 1 ].x - V4Line[ 1 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 3 ][ 1 ][ 2 ].x = ( V3Line[ 2 ].z * V4Line[ 1 ].y - V4Line[ 1 ].z * V3Line[ 2 ].y ) / ( V4Line[ 1 ].x * V3Line[ 2 ].y - V3Line[ 2 ].x * V4Line[ 1 ].y ) ;
		VPosD[ 3 ][ 1 ][ 2 ].y = ( V4Line[ 1 ].x * V3Line[ 2 ].z - V4Line[ 1 ].z * V3Line[ 2 ].x ) / ( V4Line[ 1 ].y * V3Line[ 2 ].x - V4Line[ 1 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 3 ][ 1 ][ 3 ].x = ( V3Line[ 3 ].z * V4Line[ 1 ].y - V4Line[ 1 ].z * V3Line[ 3 ].y ) / ( V4Line[ 1 ].x * V3Line[ 3 ].y - V3Line[ 3 ].x * V4Line[ 1 ].y ) ;
		VPosD[ 3 ][ 1 ][ 3 ].y = ( V4Line[ 1 ].x * V3Line[ 3 ].z - V4Line[ 1 ].z * V3Line[ 3 ].x ) / ( V4Line[ 1 ].y * V3Line[ 3 ].x - V4Line[ 1 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 3 ][ 2 ][ 0 ].x = ( V3Line[ 0 ].z * V4Line[ 2 ].y - V4Line[ 2 ].z * V3Line[ 0 ].y ) / ( V4Line[ 2 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V4Line[ 2 ].y ) ;
		VPosD[ 3 ][ 2 ][ 0 ].y = ( V4Line[ 2 ].x * V3Line[ 0 ].z - V4Line[ 2 ].z * V3Line[ 0 ].x ) / ( V4Line[ 2 ].y * V3Line[ 0 ].x - V4Line[ 2 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 3 ][ 2 ][ 1 ].x = ( V3Line[ 1 ].z * V4Line[ 2 ].y - V4Line[ 2 ].z * V3Line[ 1 ].y ) / ( V4Line[ 2 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V4Line[ 2 ].y ) ;
		VPosD[ 3 ][ 2 ][ 1 ].y = ( V4Line[ 2 ].x * V3Line[ 1 ].z - V4Line[ 2 ].z * V3Line[ 1 ].x ) / ( V4Line[ 2 ].y * V3Line[ 1 ].x - V4Line[ 2 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 3 ][ 2 ][ 2 ].x = ( V3Line[ 2 ].z * V4Line[ 2 ].y - V4Line[ 2 ].z * V3Line[ 2 ].y ) / ( V4Line[ 2 ].x * V3Line[ 2 ].y - V3Line[ 2 ].x * V4Line[ 2 ].y ) ;
		VPosD[ 3 ][ 2 ][ 2 ].y = ( V4Line[ 2 ].x * V3Line[ 2 ].z - V4Line[ 2 ].z * V3Line[ 2 ].x ) / ( V4Line[ 2 ].y * V3Line[ 2 ].x - V4Line[ 2 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 3 ][ 2 ][ 3 ].x = ( V3Line[ 3 ].z * V4Line[ 2 ].y - V4Line[ 2 ].z * V3Line[ 3 ].y ) / ( V4Line[ 2 ].x * V3Line[ 3 ].y - V3Line[ 3 ].x * V4Line[ 2 ].y ) ;
		VPosD[ 3 ][ 2 ][ 3 ].y = ( V4Line[ 2 ].x * V3Line[ 3 ].z - V4Line[ 2 ].z * V3Line[ 3 ].x ) / ( V4Line[ 2 ].y * V3Line[ 3 ].x - V4Line[ 2 ].x * V3Line[ 3 ].y ) ;
		VPosD[ 3 ][ 3 ][ 0 ].x = ( V3Line[ 0 ].z * V4Line[ 3 ].y - V4Line[ 3 ].z * V3Line[ 0 ].y ) / ( V4Line[ 3 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V4Line[ 3 ].y ) ;
		VPosD[ 3 ][ 3 ][ 0 ].y = ( V4Line[ 3 ].x * V3Line[ 0 ].z - V4Line[ 3 ].z * V3Line[ 0 ].x ) / ( V4Line[ 3 ].y * V3Line[ 0 ].x - V4Line[ 3 ].x * V3Line[ 0 ].y ) ;
		VPosD[ 3 ][ 3 ][ 1 ].x = ( V3Line[ 1 ].z * V4Line[ 3 ].y - V4Line[ 3 ].z * V3Line[ 1 ].y ) / ( V4Line[ 3 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V4Line[ 3 ].y ) ;
		VPosD[ 3 ][ 3 ][ 1 ].y = ( V4Line[ 3 ].x * V3Line[ 1 ].z - V4Line[ 3 ].z * V3Line[ 1 ].x ) / ( V4Line[ 3 ].y * V3Line[ 1 ].x - V4Line[ 3 ].x * V3Line[ 1 ].y ) ;
		VPosD[ 3 ][ 3 ][ 2 ].x = ( V3Line[ 2 ].z * V4Line[ 3 ].y - V4Line[ 3 ].z * V3Line[ 2 ].y ) / ( V4Line[ 3 ].x * V3Line[ 2 ].y - V3Line[ 2 ].x * V4Line[ 3 ].y ) ;
		VPosD[ 3 ][ 3 ][ 2 ].y = ( V4Line[ 3 ].x * V3Line[ 2 ].z - V4Line[ 3 ].z * V3Line[ 2 ].x ) / ( V4Line[ 3 ].y * V3Line[ 2 ].x - V4Line[ 3 ].x * V3Line[ 2 ].y ) ;
		VPosD[ 3 ][ 3 ][ 3 ].x = ( V3Line[ 3 ].z * V4Line[ 3 ].y - V4Line[ 3 ].z * V3Line[ 3 ].y ) / ( V4Line[ 3 ].x * V3Line[ 3 ].y - V3Line[ 3 ].x * V4Line[ 3 ].y ) ;
		VPosD[ 3 ][ 3 ][ 3 ].y = ( V4Line[ 3 ].x * V3Line[ 3 ].z - V4Line[ 3 ].z * V3Line[ 3 ].x ) / ( V4Line[ 3 ].y * V3Line[ 3 ].x - V4Line[ 3 ].x * V3Line[ 3 ].y ) ;

		Vertex = ParamVertex ;
		n = 1 ;
		for( i = 0 ; i < 4 ; i++ )
		{
			Vertex[ 0 ].x = VPosD[ i ][ 0 ][ 1 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 0 ][ 1 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 0 ][ 0 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 0 ][ 0 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 1 ][ 1 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 1 ][ 1 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 1 ][ 0 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 1 ][ 0 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;

			Vertex[ 0 ].x = VPosD[ i ][ 0 ][ 2 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 0 ][ 2 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 0 ][ 1 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 0 ][ 1 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 1 ][ 2 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 1 ][ 2 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 1 ][ 1 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 1 ][ 1 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;

			Vertex[ 0 ].x = VPosD[ i ][ 0 ][ 3 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 0 ][ 3 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 0 ][ 2 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 0 ][ 2 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 1 ][ 3 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 1 ][ 3 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 1 ][ 2 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 1 ][ 2 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;



			Vertex[ 0 ].x = VPosD[ i ][ 1 ][ 1 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 1 ][ 1 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 1 ][ 0 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 1 ][ 0 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 2 ][ 1 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 2 ][ 1 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 2 ][ 0 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 2 ][ 0 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;

			Vertex[ 0 ].x = VPosD[ i ][ 1 ][ 2 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 1 ][ 2 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 1 ][ 1 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 1 ][ 1 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 2 ][ 2 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 2 ][ 2 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 2 ][ 1 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 2 ][ 1 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;

			Vertex[ 0 ].x = VPosD[ i ][ 1 ][ 3 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 1 ][ 3 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 1 ][ 2 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 1 ][ 2 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 2 ][ 3 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 2 ][ 3 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 2 ][ 2 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 2 ][ 2 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;




			Vertex[ 0 ].x = VPosD[ i ][ 2 ][ 1 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 2 ][ 1 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 2 ][ 0 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 2 ][ 0 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 3 ][ 1 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 3 ][ 1 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 3 ][ 0 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 3 ][ 0 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 0 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;

			Vertex[ 0 ].x = VPosD[ i ][ 2 ][ 2 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 2 ][ 2 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 2 ][ 1 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 2 ][ 1 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 3 ][ 2 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 3 ][ 2 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 3 ][ 1 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 3 ][ 1 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;

			Vertex[ 0 ].x = VPosD[ i ][ 2 ][ 3 ].x ;
			Vertex[ 0 ].y = VPosD[ i ][ 2 ][ 3 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 2 ][ 2 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 2 ][ 2 ].y ;
			Vertex[ 2 ].x = VPosD[ i ][ 3 ][ 3 ].x ;
			Vertex[ 2 ].y = VPosD[ i ][ 3 ][ 3 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 3 ][ 2 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 3 ][ 2 ].y ;

			Vertex[ 0 ].u = Line_In_UV[ 1 ] ;	Vertex[ 0 ].v = Line_In_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_In_UV[ 0 ] ;	Vertex[ 1 ].v = Line_In_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_In_UV[ 1 ] ;	Vertex[ 2 ].v = Line_In_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_In_UV[ 0 ] ;	Vertex[ 3 ].v = Line_In_UV[ 1 ] ;

			Vertex += 4 ;




			Vertex[ 0 ].x = VPosD[ n ][ 3 ][ 0 ].x ;
			Vertex[ 0 ].y = VPosD[ n ][ 3 ][ 0 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 0 ][ 3 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 0 ][ 3 ].y ;
			Vertex[ 2 ].x = VPosD[ n ][ 3 ][ 1 ].x ;
			Vertex[ 2 ].y = VPosD[ n ][ 3 ][ 1 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 1 ][ 3 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 1 ][ 3 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;


			Vertex[ 0 ].x = VPosD[ n ][ 3 ][ 1 ].x ;
			Vertex[ 0 ].y = VPosD[ n ][ 3 ][ 1 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 1 ][ 3 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 1 ][ 3 ].y ;
			Vertex[ 2 ].x = VPosD[ n ][ 3 ][ 2 ].x ;
			Vertex[ 2 ].y = VPosD[ n ][ 3 ][ 2 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 2 ][ 3 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 2 ][ 3 ].y ;

			Vertex[ 0 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 0 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 1 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 1 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 2 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 2 ].v = Line_Out_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_Out_UV[ 1 ] ;	Vertex[ 3 ].v = Line_Out_UV[ 1 ] ;

			Vertex += 4 ;


			Vertex[ 0 ].x = VPosD[ n ][ 3 ][ 2 ].x ;
			Vertex[ 0 ].y = VPosD[ n ][ 3 ][ 2 ].y ;
			Vertex[ 1 ].x = VPosD[ i ][ 2 ][ 3 ].x ;
			Vertex[ 1 ].y = VPosD[ i ][ 2 ][ 3 ].y ;
			Vertex[ 2 ].x = VPosD[ n ][ 3 ][ 3 ].x ;
			Vertex[ 2 ].y = VPosD[ n ][ 3 ][ 3 ].y ;
			Vertex[ 3 ].x = VPosD[ i ][ 3 ][ 3 ].x ;
			Vertex[ 3 ].y = VPosD[ i ][ 3 ][ 3 ].y ;

			Vertex[ 0 ].u = Line_In_UV[ 1 ] ;	Vertex[ 0 ].v = Line_In_UV[ 0 ] ;
			Vertex[ 1 ].u = Line_In_UV[ 1 ] ;	Vertex[ 1 ].v = Line_In_UV[ 0 ] ;
			Vertex[ 2 ].u = Line_In_UV[ 1 ] ;	Vertex[ 2 ].v = Line_In_UV[ 1 ] ;
			Vertex[ 3 ].u = Line_In_UV[ 1 ] ;	Vertex[ 3 ].v = Line_In_UV[ 1 ] ;

			Vertex += 4 ;


			n = n == 3 ? 0 : n + 1 ;
		}
	}

	Graphics_Draw_DrawSimpleQuadrangleGraphF( &Param ) ;

END :

	if( OldDrawMode != -1 )
	{
		NS_SetDrawMode( OldDrawMode ) ;
	}

	Graphics_DrawSetting_SetDrawBrightToOneParam( OrigColor ) ;

	// �I��
	return 0 ;
}

// �l�p�`��`�悷��( �A���`�G�C���A�X�t�� )
static int DrawQuadrangleFillAA( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int Color )
{
	int OldDrawMode = -1 ;
	int GrHandle ;
	GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM Param ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX ParamVertex[ 4 * 2 * 4 + 4 ] ;
	GRAPHICS_DRAW_DRAWSIMPLEANGLEGRAPHF_VERTEX *Vertex ;
	VECTOR V1Dir, V1Pos, V1Norm, V1Line[ 2 ] ;
	VECTOR V2Dir, V2Pos, V2Norm, V2Line[ 2 ] ;
	VECTOR V3Dir, V3Pos, V3Norm, V3Line[ 2 ] ;
	VECTOR V4Dir, V4Pos, V4Norm, V4Line[ 2 ] ;
//	VECTOR V12Dir ;
//	VECTOR V13Dir ;
//	VECTOR V14Dir ;
	VECTOR VPosD[ 4 ][ 4 ] ;
	float Distance ;
	VECTOR TempPos ;
//	float P1P2X ;
//	float P1P2Y ;
//	float P1P3X ;
//	float P1P3Y ;
//	float P1P4X ;
//	float P1P4Y ;
//	float TempF ;
	int i ;
	int n ;
	DWORD FColor ;
	DWORD OrigColor ;
	static float LineUV[ 4 ] = { 0.0f, 3.0f / LINE_TEX_SIZE, 5.0f / LINE_TEX_SIZE, 1.0f };

	if( NS_GetDrawFloatCoordType() == DX_DRAWFLOATCOORDTYPE_DIRECT3D9 )
	{
		x1 -= 0.5f ;
		y1 -= 0.5f ;
		x2 -= 0.5f ;
		y2 -= 0.5f ;
		x3 -= 0.5f ;
		y3 -= 0.5f ;
		x4 -= 0.5f ;
		y4 -= 0.5f ;
	}

	SETUP32BITCOLOR( Color, FColor ) ;
	OrigColor = GSYS.DrawSetting.bDrawBright ;
	Graphics_DrawSetting_SetDrawBrightToOneParam( FColor ) ;

	if( GSYS.DrawSetting.DrawMode != DX_DRAWMODE_BILINEAR )
	{
		OldDrawMode = GSYS.DrawSetting.DrawMode ;
		NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	}

	Param.Vertex        = ParamVertex ;
	Param.TransFlag     = TRUE ;
	Param.QuadrangleNum = 4 * 2 + 1 ;

	GrHandle = Graphics_Image_GetLineTexHandle( IS_PMA_DRAWBLENDMODE ) ;

	Param.GraphHandle = GrHandle ;

//	P1P2X = x2 - x1 ;
//	P1P2Y = y2 - y1 ;
//
//	P1P3X = x3 - x1 ;
//	P1P3Y = y3 - y1 ;
//
//	P1P4X = x4 - x1 ;
//	P1P4Y = y4 - y1 ;
//
//	if( P1P2X * P1P3Y - P1P2Y * P1P3X < 0.0f )
//	{
//		TempF = x2 ; x2 = x3 ; x3 = TempF ;
//		TempF = y2 ; y2 = y3 ; y3 = TempF ;
//	}

	V1Pos.x  = x1 ;			V1Pos.y  = y1 ;			V1Pos.z  = 0.0f ;
	V1Dir.x  = x2 - x1 ;	V1Dir.y  = y2 - y1 ;	V1Dir.z  = 0.0f ;
//	V12Dir = V1Dir;
	Distance = V1Dir.x * V1Dir.x + V1Dir.y * V1Dir.y ;
	if( Distance < 0.000001f )
	{
		goto END ;
	}
	Distance = _SQRT( Distance ) ;
	V1Dir.x /= Distance ;
	V1Dir.y /= Distance ;
	V1Norm.x = V1Dir.y ;	V1Norm.y = -V1Dir.x ;	V1Norm.z = 0.0f ;
	TempPos = VAdd( V1Pos, VScale( V1Norm,  2.0f ) ) ;
	V1Line[ 0 ].x =  V1Dir.y ;
	V1Line[ 0 ].y = -V1Dir.x ;
	V1Line[ 0 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;
	TempPos = VAdd( V1Pos, VScale( V1Norm, -1.0f ) ) ;
	V1Line[ 1 ].x =  V1Dir.y ;
	V1Line[ 1 ].y = -V1Dir.x ;
	V1Line[ 1 ].z = ( TempPos.x + V1Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V1Dir.y ) ;

	V2Pos.x  = x2 ;			V2Pos.y  = y2 ;			V2Pos.z  = 0.0f ;
	V2Dir.x  = x3 - x2 ;	V2Dir.y  = y3 - y2 ;	V2Dir.z  = 0.0f ;
	Distance = V2Dir.x * V2Dir.x + V2Dir.y * V2Dir.y ;
	if( Distance < 0.000001f )
	{
		goto END ;
	}
	Distance = _SQRT( Distance ) ;
	V2Dir.x /= Distance ;
	V2Dir.y /= Distance ;
	V2Norm.x = V2Dir.y ;	V2Norm.y = -V2Dir.x ;	V2Norm.z = 0.0f ;
	TempPos = VAdd( V2Pos, VScale( V2Norm,  2.0f ) ) ;
	V2Line[ 0 ].x =  V2Dir.y ;
	V2Line[ 0 ].y = -V2Dir.x ;
	V2Line[ 0 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;
	TempPos = VAdd( V2Pos, VScale( V2Norm, -1.0f ) ) ;
	V2Line[ 1 ].x =  V2Dir.y ;
	V2Line[ 1 ].y = -V2Dir.x ;
	V2Line[ 1 ].z = ( TempPos.x + V2Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V2Dir.y ) ;

	V3Pos.x  = x3 ;			V3Pos.y  = y3 ;			V3Pos.z  = 0.0f ;
	V3Dir.x  = x4 - x3 ;	V3Dir.y  = y4 - y3 ;	V3Dir.z  = 0.0f ;
	Distance = V3Dir.x * V3Dir.x + V3Dir.y * V3Dir.y ;
	if( Distance < 0.000001f )
	{
		goto END ;
	}
	Distance = _SQRT( Distance ) ;
	V3Dir.x /= Distance ;
	V3Dir.y /= Distance ;
	V3Norm.x = V3Dir.y ;	V3Norm.y = -V3Dir.x ;	V3Norm.z = 0.0f ;
	TempPos = VAdd( V3Pos, VScale( V3Norm,  2.0f ) ) ;
	V3Line[ 0 ].x =  V3Dir.y ;
	V3Line[ 0 ].y = -V3Dir.x ;
	V3Line[ 0 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;
	TempPos = VAdd( V3Pos, VScale( V3Norm, -1.0f ) ) ;
	V3Line[ 1 ].x =  V3Dir.y ;
	V3Line[ 1 ].y = -V3Dir.x ;
	V3Line[ 1 ].z = ( TempPos.x + V3Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V3Dir.y ) ;

	V4Pos.x  = x4 ;			V4Pos.y  = y4 ;			V4Pos.z  = 0.0f ;
	V4Dir.x  = x1 - x4 ;	V4Dir.y  = y1 - y4 ;	V4Dir.z  = 0.0f ;
	Distance = V4Dir.x * V4Dir.x + V4Dir.y * V4Dir.y ;
	if( Distance < 0.000001f )
	{
		goto END ;
	}
	Distance = _SQRT( Distance ) ;
	V4Dir.x /= Distance ;
	V4Dir.y /= Distance ;
	V4Norm.x = V4Dir.y ;	V4Norm.y = -V4Dir.x ;	V4Norm.z = 0.0f ;
	TempPos = VAdd( V4Pos, VScale( V4Norm,  2.0f ) ) ;
	V4Line[ 0 ].x =  V4Dir.y ;
	V4Line[ 0 ].y = -V4Dir.x ;
	V4Line[ 0 ].z = ( TempPos.x + V4Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V4Dir.y ) ;
	TempPos = VAdd( V4Pos, VScale( V4Norm, -1.0f ) ) ;
	V4Line[ 1 ].x =  V4Dir.y ;
	V4Line[ 1 ].y = -V4Dir.x ;
	V4Line[ 1 ].z = ( TempPos.x + V4Dir.x ) * TempPos.y - TempPos.x * ( TempPos.y + V4Dir.y ) ;

//	V13Dir.x = x3 - x1 ;    V13Dir.y = y3 - y1 ;	V13Dir.z = 0.0f ;
//	V14Dir.x = x4 - x1 ;    V14Dir.y = y4 - y1 ;	V14Dir.z = 0.0f ;
//	float SizeSq = VSquareSize( VCross( V12Dir, V13Dir ) ) ;
//	if( SizeSq / 2.0f < 7500.0f )
//	{
//		NS_DrawTriangle( _FTOL( x1 ), _FTOL( y1 ), _FTOL( x2 ), _FTOL( y2 ), _FTOL( x3 ), _FTOL( y3 ), Color, TRUE ) ;
//		goto END ;
//	}


	VPosD[ 0 ][ 0 ].x = ( V4Line[ 0 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V4Line[ 0 ].y ) / ( V1Line[ 0 ].x * V4Line[ 0 ].y - V4Line[ 0 ].x * V1Line[ 0 ].y ) ;
	VPosD[ 0 ][ 0 ].y = ( V1Line[ 0 ].x * V4Line[ 0 ].z - V1Line[ 0 ].z * V4Line[ 0 ].x ) / ( V1Line[ 0 ].y * V4Line[ 0 ].x - V1Line[ 0 ].x * V4Line[ 0 ].y ) ;

	VPosD[ 0 ][ 1 ].x = ( V4Line[ 1 ].z * V1Line[ 0 ].y - V1Line[ 0 ].z * V4Line[ 1 ].y ) / ( V1Line[ 0 ].x * V4Line[ 1 ].y - V4Line[ 1 ].x * V1Line[ 0 ].y ) ;
	VPosD[ 0 ][ 1 ].y = ( V1Line[ 0 ].x * V4Line[ 1 ].z - V1Line[ 0 ].z * V4Line[ 1 ].x ) / ( V1Line[ 0 ].y * V4Line[ 1 ].x - V1Line[ 0 ].x * V4Line[ 1 ].y ) ;

	VPosD[ 0 ][ 2 ].x = ( V4Line[ 0 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V4Line[ 0 ].y ) / ( V1Line[ 1 ].x * V4Line[ 0 ].y - V4Line[ 0 ].x * V1Line[ 1 ].y ) ;
	VPosD[ 0 ][ 2 ].y = ( V1Line[ 1 ].x * V4Line[ 0 ].z - V1Line[ 1 ].z * V4Line[ 0 ].x ) / ( V1Line[ 1 ].y * V4Line[ 0 ].x - V1Line[ 1 ].x * V4Line[ 0 ].y ) ;

	VPosD[ 0 ][ 3 ].x = ( V4Line[ 1 ].z * V1Line[ 1 ].y - V1Line[ 1 ].z * V4Line[ 1 ].y ) / ( V1Line[ 1 ].x * V4Line[ 1 ].y - V4Line[ 1 ].x * V1Line[ 1 ].y ) ;
	VPosD[ 0 ][ 3 ].y = ( V1Line[ 1 ].x * V4Line[ 1 ].z - V1Line[ 1 ].z * V4Line[ 1 ].x ) / ( V1Line[ 1 ].y * V4Line[ 1 ].x - V1Line[ 1 ].x * V4Line[ 1 ].y ) ;


	VPosD[ 1 ][ 0 ].x = ( V1Line[ 0 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 0 ].y ) / ( V2Line[ 0 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 0 ].y ) ;
	VPosD[ 1 ][ 0 ].y = ( V2Line[ 0 ].x * V1Line[ 0 ].z - V2Line[ 0 ].z * V1Line[ 0 ].x ) / ( V2Line[ 0 ].y * V1Line[ 0 ].x - V2Line[ 0 ].x * V1Line[ 0 ].y ) ;

	VPosD[ 1 ][ 1 ].x = ( V1Line[ 1 ].z * V2Line[ 0 ].y - V2Line[ 0 ].z * V1Line[ 1 ].y ) / ( V2Line[ 0 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 0 ].y ) ;
	VPosD[ 1 ][ 1 ].y = ( V2Line[ 0 ].x * V1Line[ 1 ].z - V2Line[ 0 ].z * V1Line[ 1 ].x ) / ( V2Line[ 0 ].y * V1Line[ 1 ].x - V2Line[ 0 ].x * V1Line[ 1 ].y ) ;

	VPosD[ 1 ][ 2 ].x = ( V1Line[ 0 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 0 ].y ) / ( V2Line[ 1 ].x * V1Line[ 0 ].y - V1Line[ 0 ].x * V2Line[ 1 ].y ) ;
	VPosD[ 1 ][ 2 ].y = ( V2Line[ 1 ].x * V1Line[ 0 ].z - V2Line[ 1 ].z * V1Line[ 0 ].x ) / ( V2Line[ 1 ].y * V1Line[ 0 ].x - V2Line[ 1 ].x * V1Line[ 0 ].y ) ;

	VPosD[ 1 ][ 3 ].x = ( V1Line[ 1 ].z * V2Line[ 1 ].y - V2Line[ 1 ].z * V1Line[ 1 ].y ) / ( V2Line[ 1 ].x * V1Line[ 1 ].y - V1Line[ 1 ].x * V2Line[ 1 ].y ) ;
	VPosD[ 1 ][ 3 ].y = ( V2Line[ 1 ].x * V1Line[ 1 ].z - V2Line[ 1 ].z * V1Line[ 1 ].x ) / ( V2Line[ 1 ].y * V1Line[ 1 ].x - V2Line[ 1 ].x * V1Line[ 1 ].y ) ;


	VPosD[ 2 ][ 0 ].x = ( V2Line[ 0 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 0 ].y ) / ( V3Line[ 0 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 0 ].y ) ;
	VPosD[ 2 ][ 0 ].y = ( V3Line[ 0 ].x * V2Line[ 0 ].z - V3Line[ 0 ].z * V2Line[ 0 ].x ) / ( V3Line[ 0 ].y * V2Line[ 0 ].x - V3Line[ 0 ].x * V2Line[ 0 ].y ) ;

	VPosD[ 2 ][ 1 ].x = ( V2Line[ 1 ].z * V3Line[ 0 ].y - V3Line[ 0 ].z * V2Line[ 1 ].y ) / ( V3Line[ 0 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 0 ].y ) ;
	VPosD[ 2 ][ 1 ].y = ( V3Line[ 0 ].x * V2Line[ 1 ].z - V3Line[ 0 ].z * V2Line[ 1 ].x ) / ( V3Line[ 0 ].y * V2Line[ 1 ].x - V3Line[ 0 ].x * V2Line[ 1 ].y ) ;

	VPosD[ 2 ][ 2 ].x = ( V2Line[ 0 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 0 ].y ) / ( V3Line[ 1 ].x * V2Line[ 0 ].y - V2Line[ 0 ].x * V3Line[ 1 ].y ) ;
	VPosD[ 2 ][ 2 ].y = ( V3Line[ 1 ].x * V2Line[ 0 ].z - V3Line[ 1 ].z * V2Line[ 0 ].x ) / ( V3Line[ 1 ].y * V2Line[ 0 ].x - V3Line[ 1 ].x * V2Line[ 0 ].y ) ;

	VPosD[ 2 ][ 3 ].x = ( V2Line[ 1 ].z * V3Line[ 1 ].y - V3Line[ 1 ].z * V2Line[ 1 ].y ) / ( V3Line[ 1 ].x * V2Line[ 1 ].y - V2Line[ 1 ].x * V3Line[ 1 ].y ) ;
	VPosD[ 2 ][ 3 ].y = ( V3Line[ 1 ].x * V2Line[ 1 ].z - V3Line[ 1 ].z * V2Line[ 1 ].x ) / ( V3Line[ 1 ].y * V2Line[ 1 ].x - V3Line[ 1 ].x * V2Line[ 1 ].y ) ;


	VPosD[ 3 ][ 0 ].x = ( V3Line[ 0 ].z * V4Line[ 0 ].y - V4Line[ 0 ].z * V3Line[ 0 ].y ) / ( V4Line[ 0 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V4Line[ 0 ].y ) ;
	VPosD[ 3 ][ 0 ].y = ( V4Line[ 0 ].x * V3Line[ 0 ].z - V4Line[ 0 ].z * V3Line[ 0 ].x ) / ( V4Line[ 0 ].y * V3Line[ 0 ].x - V4Line[ 0 ].x * V3Line[ 0 ].y ) ;

	VPosD[ 3 ][ 1 ].x = ( V3Line[ 1 ].z * V4Line[ 0 ].y - V4Line[ 0 ].z * V3Line[ 1 ].y ) / ( V4Line[ 0 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V4Line[ 0 ].y ) ;
	VPosD[ 3 ][ 1 ].y = ( V4Line[ 0 ].x * V3Line[ 1 ].z - V4Line[ 0 ].z * V3Line[ 1 ].x ) / ( V4Line[ 0 ].y * V3Line[ 1 ].x - V4Line[ 0 ].x * V3Line[ 1 ].y ) ;

	VPosD[ 3 ][ 2 ].x = ( V3Line[ 0 ].z * V4Line[ 1 ].y - V4Line[ 1 ].z * V3Line[ 0 ].y ) / ( V4Line[ 1 ].x * V3Line[ 0 ].y - V3Line[ 0 ].x * V4Line[ 1 ].y ) ;
	VPosD[ 3 ][ 2 ].y = ( V4Line[ 1 ].x * V3Line[ 0 ].z - V4Line[ 1 ].z * V3Line[ 0 ].x ) / ( V4Line[ 1 ].y * V3Line[ 0 ].x - V4Line[ 1 ].x * V3Line[ 0 ].y ) ;

	VPosD[ 3 ][ 3 ].x = ( V3Line[ 1 ].z * V4Line[ 1 ].y - V4Line[ 1 ].z * V3Line[ 1 ].y ) / ( V4Line[ 1 ].x * V3Line[ 1 ].y - V3Line[ 1 ].x * V4Line[ 1 ].y ) ;
	VPosD[ 3 ][ 3 ].y = ( V4Line[ 1 ].x * V3Line[ 1 ].z - V4Line[ 1 ].z * V3Line[ 1 ].x ) / ( V4Line[ 1 ].y * V3Line[ 1 ].x - V4Line[ 1 ].x * V3Line[ 1 ].y ) ;

	Vertex = ParamVertex ;
	n = 1 ;
	for( i = 0 ; i < 4 ; i++ )
	{
		Vertex[ 0 ].x = VPosD[ i ][ 1 ].x ;
		Vertex[ 0 ].y = VPosD[ i ][ 1 ].y ;
		Vertex[ 1 ].x = VPosD[ i ][ 0 ].x ;
		Vertex[ 1 ].y = VPosD[ i ][ 0 ].y ;
		Vertex[ 2 ].x = VPosD[ i ][ 3 ].x ;
		Vertex[ 2 ].y = VPosD[ i ][ 3 ].y ;
		Vertex[ 3 ].x = VPosD[ i ][ 2 ].x ;
		Vertex[ 3 ].y = VPosD[ i ][ 2 ].y ;

		Vertex[ 0 ].u = LineUV[ 1 ] ;	Vertex[ 0 ].v = LineUV[ 0 ] ;
		Vertex[ 1 ].u = LineUV[ 0 ] ;	Vertex[ 1 ].v = LineUV[ 0 ] ;
		Vertex[ 2 ].u = LineUV[ 1 ] ;	Vertex[ 2 ].v = LineUV[ 1 ] ;
		Vertex[ 3 ].u = LineUV[ 0 ] ;	Vertex[ 3 ].v = LineUV[ 1 ] ;

		Vertex += 4 ;


		Vertex[ 0 ].x = VPosD[ n ][ 2 ].x ;
		Vertex[ 0 ].y = VPosD[ n ][ 2 ].y ;
		Vertex[ 1 ].x = VPosD[ i ][ 1 ].x ;
		Vertex[ 1 ].y = VPosD[ i ][ 1 ].y ;
		Vertex[ 2 ].x = VPosD[ n ][ 3 ].x ;
		Vertex[ 2 ].y = VPosD[ n ][ 3 ].y ;
		Vertex[ 3 ].x = VPosD[ i ][ 3 ].x ;
		Vertex[ 3 ].y = VPosD[ i ][ 3 ].y ;

		Vertex[ 0 ].u = LineUV[ 1 ] ;	Vertex[ 0 ].v = LineUV[ 0 ] ;
		Vertex[ 1 ].u = LineUV[ 1 ] ;	Vertex[ 1 ].v = LineUV[ 0 ] ;
		Vertex[ 2 ].u = LineUV[ 1 ] ;	Vertex[ 2 ].v = LineUV[ 1 ] ;
		Vertex[ 3 ].u = LineUV[ 1 ] ;	Vertex[ 3 ].v = LineUV[ 1 ] ;

		Vertex += 4 ;

		n = n == 3 ? 0 : n + 1 ;
	}

	Vertex[ 0 ].x = VPosD[ 2 ][ 3 ].x ;
	Vertex[ 0 ].y = VPosD[ 2 ][ 3 ].y ;
	Vertex[ 1 ].x = VPosD[ 3 ][ 3 ].x ;
	Vertex[ 1 ].y = VPosD[ 3 ][ 3 ].y ;
	Vertex[ 2 ].x = VPosD[ 1 ][ 3 ].x ;
	Vertex[ 2 ].y = VPosD[ 1 ][ 3 ].y ;
	Vertex[ 3 ].x = VPosD[ 0 ][ 3 ].x ;
	Vertex[ 3 ].y = VPosD[ 0 ][ 3 ].y ;

	Vertex[ 0 ].u = LineUV[ 1 ] ;	Vertex[ 0 ].v = LineUV[ 1 ] ;
	Vertex[ 1 ].u = LineUV[ 1 ] ;	Vertex[ 1 ].v = LineUV[ 1 ] ;
	Vertex[ 2 ].u = LineUV[ 1 ] ;	Vertex[ 2 ].v = LineUV[ 1 ] ;
	Vertex[ 3 ].u = LineUV[ 1 ] ;	Vertex[ 3 ].v = LineUV[ 1 ] ;

	Graphics_Draw_DrawSimpleQuadrangleGraphF( &Param ) ;

END :

	if( OldDrawMode != -1 )
	{
		NS_SetDrawMode( OldDrawMode ) ;
	}

	Graphics_DrawSetting_SetDrawBrightToOneParam( OrigColor ) ;

	// �I��
	return 0 ;
}

// �l�p�`��`�悷��( �A���`�G�C���A�X�t�� )
extern int NS_DrawQuadrangleAA( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, unsigned int Color, int FillFlag, float LineThickness )
{
	if( FillFlag == FALSE )
	{
		return DrawQuadrangleLineAA( x1, y1, x2, y2, x3, y3, x4, y4, Color, LineThickness ) ;
	}
	else
	{
		return DrawQuadrangleFillAA( x1, y1, x2, y2, x3, y3, x4, y4, Color ) ;
	}
}

// �p�̊ۂ��l�p�`��`�悷��
extern int NS_DrawRoundRect( int x1, int y1, int x2, int y2, int rx, int ry, unsigned int Color, int FillFlag )
{
	RECT  BackupDrawRect ;
	RECTF BackupDrawRectF ;
	int   DrawSizeX ;
	int   DrawSizeY ;
	int   Temp ;

	// �`���`��ۑ�
	BackupDrawRect  = GSYS.DrawSetting.DrawArea ;
	BackupDrawRectF = GSYS.DrawSetting.DrawAreaF ;

	if( rx <= 0 )
	{
		rx = 1 ;
	}

	if( ry <= 0 )
	{
		ry = 1 ;
	}

	if( x2 < x1 )
	{
		Temp = x1 ;
		x1 = x2 ;
		x2 = Temp ;
	}

	if( y2 < y1 )
	{
		Temp = y1 ;
		y1 = y2 ;
		y2 = Temp ;
	}

	DrawSizeX = x2 - x1 ;
	DrawSizeY = y2 - y1 ;

	if( rx * 2 > DrawSizeX )
	{
		rx = DrawSizeX / 2 ;
	}

	if( ry * 2 > DrawSizeY )
	{
		ry = DrawSizeY / 2 ;
	}

	if( FillFlag )
	{
		NS_DrawBox( x1 + rx, y1,      x2 - rx, y2,      Color, FillFlag ) ;
		NS_DrawBox( x1,      y1 + ry, x1 + rx, y2 - ry, Color, FillFlag ) ;
		NS_DrawBox( x2 - rx, y1 + ry, x2,      y2 - ry, Color, FillFlag ) ;
	}
	else
	{
		NS_SetDrawArea( x1 + rx, y1, x2 - rx, y2 ) ;
		NS_DrawBox( x1, y1, x2, y2, Color, FillFlag ) ;

		NS_SetDrawArea( x1, y1 + ry, x1 + rx, y2 - ry ) ;
		NS_DrawBox( x1, y1, x2, y2, Color, FillFlag ) ;

		NS_SetDrawArea( x2 - rx, y1 + ry, x2, y2 - ry ) ;
		NS_DrawBox( x1, y1, x2, y2, Color, FillFlag ) ;

		NS_SetDrawArea( BackupDrawRect.left, BackupDrawRect.top, BackupDrawRect.right, BackupDrawRect.bottom ) ; 
	}

	SETRECT( GSYS.DrawSetting.DrawArea, x1, y1, x1 + rx, y1 + ry ) ;
	RectClipping_Inline( &GSYS.DrawSetting.DrawArea, &BackupDrawRect ) ;
	SetMemImgDrawArea( &GSYS.DrawSetting.DrawArea ) ;
	GSYS.DrawSetting.DrawAreaF.left   = ( float )GSYS.DrawSetting.DrawArea.left ;
	GSYS.DrawSetting.DrawAreaF.top    = ( float )GSYS.DrawSetting.DrawArea.top ;
	GSYS.DrawSetting.DrawAreaF.right  = ( float )GSYS.DrawSetting.DrawArea.right ;
	GSYS.DrawSetting.DrawAreaF.bottom = ( float )GSYS.DrawSetting.DrawArea.bottom ;
	NS_DrawOval( x1 + rx, y1 + ry, rx, ry, Color, FillFlag ) ;

	SETRECT( GSYS.DrawSetting.DrawArea, x2 - rx, y1, x2, y1 + ry ) ;
	RectClipping_Inline( &GSYS.DrawSetting.DrawArea, &BackupDrawRect ) ;
	SetMemImgDrawArea( &GSYS.DrawSetting.DrawArea ) ;
	GSYS.DrawSetting.DrawAreaF.left   = ( float )GSYS.DrawSetting.DrawArea.left ;
	GSYS.DrawSetting.DrawAreaF.top    = ( float )GSYS.DrawSetting.DrawArea.top ;
	GSYS.DrawSetting.DrawAreaF.right  = ( float )GSYS.DrawSetting.DrawArea.right ;
	GSYS.DrawSetting.DrawAreaF.bottom = ( float )GSYS.DrawSetting.DrawArea.bottom ;
	NS_DrawOval( x2 - rx - 1, y1 + ry, rx, ry, Color, FillFlag ) ;

	SETRECT( GSYS.DrawSetting.DrawArea, x1, y2 - ry, x1 + rx, y2 ) ;
	RectClipping_Inline( &GSYS.DrawSetting.DrawArea, &BackupDrawRect ) ;
	SetMemImgDrawArea( &GSYS.DrawSetting.DrawArea ) ;
	GSYS.DrawSetting.DrawAreaF.left   = ( float )GSYS.DrawSetting.DrawArea.left ;
	GSYS.DrawSetting.DrawAreaF.top    = ( float )GSYS.DrawSetting.DrawArea.top ;
	GSYS.DrawSetting.DrawAreaF.right  = ( float )GSYS.DrawSetting.DrawArea.right ;
	GSYS.DrawSetting.DrawAreaF.bottom = ( float )GSYS.DrawSetting.DrawArea.bottom ;
	NS_DrawOval( x1 + rx, y2 - ry - 1, rx, ry, Color, FillFlag ) ;

	SETRECT( GSYS.DrawSetting.DrawArea, x2 - rx, y2 - ry, x2, y2 ) ;
	RectClipping_Inline( &GSYS.DrawSetting.DrawArea, &BackupDrawRect ) ;
	SetMemImgDrawArea( &GSYS.DrawSetting.DrawArea ) ;
	GSYS.DrawSetting.DrawAreaF.left   = ( float )GSYS.DrawSetting.DrawArea.left ;
	GSYS.DrawSetting.DrawAreaF.top    = ( float )GSYS.DrawSetting.DrawArea.top ;
	GSYS.DrawSetting.DrawAreaF.right  = ( float )GSYS.DrawSetting.DrawArea.right ;
	GSYS.DrawSetting.DrawAreaF.bottom = ( float )GSYS.DrawSetting.DrawArea.bottom ;
	NS_DrawOval( x2 - rx - 1, y2 - ry - 1, rx, ry, Color, FillFlag ) ;

	// �`���`�����ɖ߂�
	GSYS.DrawSetting.DrawArea = BackupDrawRect ;
	SetMemImgDrawArea( &BackupDrawRect ) ;
	GSYS.DrawSetting.DrawAreaF = BackupDrawRectF ;

	// �I��
	return 0 ;
}

// �p�̊ۂ��l�p�`��`�悷��( �A���`�G�C���A�X�t�� )
extern int NS_DrawRoundRectAA( float x1, float y1, float x2, float y2, float rx, float ry, unsigned int Color, int FillFlag, float LineThickness )
{
	int OldDrawMode = -1 ;

	if( GSYS.DrawSetting.DrawMode != DX_DRAWMODE_BILINEAR )
	{
		OldDrawMode = GSYS.DrawSetting.DrawMode ;
		NS_SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	}

	if( OldDrawMode != -1 )
	{
		NS_SetDrawMode( OldDrawMode ) ;
	}

	// �I��
	return 0 ;
}

// �_��`�悷��
#define SETDRAWRECTCODE\
	SETRECT( DrawRect, x, y, x + 1, y + 1 )\
	DRAWRECT_DRAWAREA_CLIP
extern 	int NS_DrawPixel( int x, int y, unsigned int Color )
{
	int Ret = -1 ;
	int Flag ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;
	
	Flag = GSYS.Setting.ValidHardware ;

	CheckActiveState() ;

	// �`��
//	if( GSYS.DrawSetting.NotUseBasicGraphDraw3DDeviceMethodFlag ) Flag = FALSE ;
	DRAW_DEF(
		Graphics_Hardware_DrawPixel_PF( x, y, Color ),
		Graphics_Software_DrawPixel(    x, y, Color ),
		SETDRAWRECTCODE,
		Ret,
		Flag
	)

	// �I��
	return Ret ;
}
#undef SETDRAWRECTCODE

// �w��_���狫�E�F������Ƃ���܂œh��Ԃ�
extern int NS_Paint( int x, int y, unsigned int FillColor, ULONGLONG BoundaryColor )
{
	// �n�[�h�E�F�A���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( GSYS.Setting.ValidHardware )
	{
		// �n�[�h�E�G�A�̋@�\���g�p���Ă���ꍇ
		if( Graphics_Hardware_Paint_PF( x, y, FillColor, BoundaryColor ) == 2 )
		{
			// �Q���Ԃ��Ă�����f�t�H���g�̏������s��

			MEMIMG          MemImg ;
			BASEIMAGE       ScreenImage ;
			int             DrawMode ;
			int             DrawBlendMode ;
			int             DrawBlendParam ;
			RGBCOLOR        DrawBright ;
			RECT            DrawArea ;
			const COLORDATA *MemImageColorData ;
			int				Red, Green, Blue ;
//			const COLORDATA *HardwareMainColorData ;

			// ��ʂ̃C���[�W���擾
			NS_CreateXRGB8ColorBaseImage( GSYS.DrawSetting.DrawSizeX, GSYS.DrawSetting.DrawSizeY, &ScreenImage ) ;
			NS_GetDrawScreenBaseImage( 0, 0, GSYS.DrawSetting.DrawSizeX, GSYS.DrawSetting.DrawSizeY, &ScreenImage ) ;

			// Paint �����p�� MEMIMG ���Z�b�g�A�b�v����
			_MEMSET( &MemImg, 0, sizeof( MEMIMG ) ) ;
			InitializeMemImg(
				&MemImg,
				GSYS.DrawSetting.DrawSizeX,
				GSYS.DrawSetting.DrawSizeY,
				ScreenImage.Pitch,
				0,
				1,
				FALSE, FALSE, FALSE, ScreenImage.GraphData ) ;

			// Paint �������s��
			MemImageColorData     = GetMemImgColorData( 1, FALSE, FALSE ) ;
//			HardwareMainColorData = Graphics_Hardware_GetMainColorData_PF() ;
//			FillColor             = NS_GetColor4( MemImageColorData, HardwareMainColorData, FillColor ) ;
			NS_GetColor2( FillColor, &Red, &Green, &Blue ) ; 
			FillColor             = NS_GetColor3( MemImageColorData, Red, Green, Blue ) ;
#if defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
			if( BoundaryColor != 0xffffffffffffffffULL )
#else // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
			if( BoundaryColor != 0xffffffffffffffff )
#endif // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
			{
//				BoundaryColor = ( ULONGLONG )NS_GetColor4( MemImageColorData, HardwareMainColorData, ( unsigned int )BoundaryColor ) ;
				NS_GetColor2( ( unsigned int )BoundaryColor, &Red, &Green, &Blue ) ; 
				BoundaryColor = NS_GetColor3( MemImageColorData, Red, Green, Blue ) ;
			}
			PaintMemImg( &MemImg, x, y, FillColor, BoundaryColor ) ;

			// MEMIMG �̌�n��
			TerminateMemImg( &MemImg ) ;

			// ��ʂɃy�C���g��̉摜��`��
			DrawBlendMode  = GSYS.DrawSetting.BlendMode ;
			DrawBlendParam = GSYS.DrawSetting.BlendParam ;
			DrawMode       = GSYS.DrawSetting.DrawMode ;
			DrawBright     = GSYS.DrawSetting.DrawBright ;
			DrawArea       = GSYS.DrawSetting.DrawArea ;

			NS_SetDrawBlendMode( DX_BLENDMODE_SRCCOLOR, 255 ) ;
			NS_SetDrawMode( DX_DRAWMODE_NEAREST ) ;
			NS_SetDrawBright( 255, 255, 255 ) ;
			NS_SetDrawArea( 0, 0, GSYS.DrawSetting.DrawSizeX, GSYS.DrawSetting.DrawSizeY ) ;

			NS_DrawBaseImage( 0, 0, &ScreenImage ) ;

			NS_SetDrawBlendMode( DrawBlendMode, DrawBlendParam ) ;
			NS_SetDrawMode( DrawMode ) ;
			NS_SetDrawBright( DrawBright.Red, DrawBright.Green, DrawBright.Blue ) ;
			NS_SetDrawArea( DrawArea.left, DrawArea.top, DrawArea.right, DrawArea.bottom ) ;

			NS_ReleaseBaseImage( &ScreenImage ) ;

			// ����I��
			return 0 ;
		}
	}
	else
	{
		MEMIMG *Img ;
		IMAGEDATA *Image ;

		// �n�[�h�E�G�A�̋@�\���g�p���Ă��Ȃ��ꍇ

		// �G���[����
		if( GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], Image ) )
			Image = NULL ;

		if( Image )
		{
			Img = &Image->Soft.MemImg ;
		}
		else
		{
			Img = &GSYS.SoftRender.MainBufferMemImg ;
		}

		PaintMemImg( Img, x, y, FillColor, BoundaryColor ) ;
	}
	
	// �I��
	return 0 ;
}

// �_�̏W����`��
#define SETDRAWRECTCODE\
	SETRECT( DrawRect, 0, 0, GSYS.DrawSetting.DrawSizeX, GSYS.DrawSetting.DrawSizeY )\
	DRAWRECT_DRAWAREA_CLIP
extern 	int NS_DrawPixelSet( const POINTDATA *PointData, int Num )
{
	int Ret = -1 ;
	int Flag ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	Flag = GSYS.Setting.ValidHardware ;

	CheckActiveState() ;

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawPixelSet_PF( PointData, Num  ),
		Graphics_Software_DrawPixelSet(    PointData, Num  ),
		SETDRAWRECTCODE,
		Ret,
		Flag
	)

	// �I��
	return Ret ;
}
#undef SETDRAWRECTCODE

// ���̏W����`��
#define SETDRAWRECTCODE\
	SETRECT( DrawRect, 0, 0, GSYS.DrawSetting.DrawSizeX, GSYS.DrawSetting.DrawSizeY )\
	DRAWRECT_DRAWAREA_CLIP
extern	int NS_DrawLineSet( const LINEDATA *LineData, int Num )
{
	int Ret = -1 ;
	int Flag ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;
	
	Flag = GSYS.Setting.ValidHardware ;

	CheckActiveState() ;

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawLineSet_PF( LineData, Num ),
		Graphics_Software_DrawLineSet(    LineData, Num ),
		SETDRAWRECTCODE,
		Ret,
		Flag
	)

	// �I��
	return Ret ;
}
#undef SETDRAWRECTCODE

// �R�c�̓_��`�悷��
extern int NS_DrawPixel3D( VECTOR Pos, unsigned int Color )
{
	LARGE3DPOSITIONSUPPORT_DRAWINFO Large3DPosDrawInfo ;
	int Ret ;
	int Flag ;

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE ) return -1 ;

	// ��`��ݒ肪����Ă���ꍇ�͉��������I��
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �u�����h�e�N�X�`���͎g�p�ł��Ȃ�
	if( GSYS.DrawSetting.BlendGraph > 0 )
		return -1 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_BeginLarge3DPositionSupportDraw( &Large3DPosDrawInfo, VConvFtoD( Pos ) ) ; 

		Pos = VGet( 0.0f, 0.0f, 0.0f ) ;
	}

	Flag = GSYS.Setting.ValidHardware ;

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawPixel3D_PF( Pos, Color, TRUE, NULL ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_EndLarge3DPositionSupportDraw( &Large3DPosDrawInfo ) ;
	}

	// �I��
	return Ret ;
}

// �R�c�̓_��`�悷��
extern int NS_DrawPixel3DD( VECTOR_D Pos, unsigned int Color )
{
	LARGE3DPOSITIONSUPPORT_DRAWINFO Large3DPosDrawInfo ;
	VECTOR PosF ;
	int Ret ;
	int Flag ;

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE ) return -1 ;

	// ��`��ݒ肪����Ă���ꍇ�͉��������I��
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �u�����h�e�N�X�`���͎g�p�ł��Ȃ�
	if( GSYS.DrawSetting.BlendGraph > 0 )
		return -1 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_BeginLarge3DPositionSupportDraw( &Large3DPosDrawInfo, Pos ) ; 

		Pos = VGetD( 0.0, 0.0, 0.0 ) ;
	}

	PosF = VConvDtoF( Pos ) ;

	Flag = GSYS.Setting.ValidHardware ;

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawPixel3D_PF( PosF, Color, TRUE, NULL ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_EndLarge3DPositionSupportDraw( &Large3DPosDrawInfo ) ;
	}

	// �I��
	return Ret ;
}

// �R�c�̐�����`�悷��
extern int NS_DrawLine3D( VECTOR Pos1, VECTOR Pos2, unsigned int Color )
{
	LARGE3DPOSITIONSUPPORT_DRAWINFO Large3DPosDrawInfo ;
	int Ret ;
	int Flag ;

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE ) return -1 ;

	// ��`��ݒ肪����Ă���ꍇ�͉��������I��
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �u�����h�e�N�X�`���͎g�p�ł��Ȃ�
	if( GSYS.DrawSetting.BlendGraph > 0 )
		return -1 ;

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_BeginLarge3DPositionSupportDraw( &Large3DPosDrawInfo, VConvFtoD( Pos1 ) ) ; 

		Pos2 = VSub( Pos2, Pos1 ) ;
		Pos1 = VGet( 0.0f, 0.0f, 0.0f ) ;
	}

	Flag = GSYS.Setting.ValidHardware ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawLine3D_PF( Pos1, Pos2, Color, TRUE, NULL ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_EndLarge3DPositionSupportDraw( &Large3DPosDrawInfo ) ;
	}

	// �I��
	return Ret ;
}

// �R�c�̐�����`�悷��
extern int NS_DrawLine3DD( VECTOR_D Pos1, VECTOR_D Pos2, unsigned int Color )
{
	LARGE3DPOSITIONSUPPORT_DRAWINFO Large3DPosDrawInfo ;
	VECTOR Pos1F, Pos2F ;
	int Ret ;
	int Flag ;

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE ) return -1 ;

	// ��`��ݒ肪����Ă���ꍇ�͉��������I��
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �u�����h�e�N�X�`���͎g�p�ł��Ȃ�
	if( GSYS.DrawSetting.BlendGraph > 0 )
		return -1 ;

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_BeginLarge3DPositionSupportDraw( &Large3DPosDrawInfo, Pos1 ) ; 

		Pos2 = VSubD( Pos2, Pos1 ) ;
		Pos1 = VGetD( 0.0, 0.0, 0.0 ) ;
	}

	Pos1F = VConvDtoF( Pos1 ) ;
	Pos2F = VConvDtoF( Pos2 ) ;

	Flag = GSYS.Setting.ValidHardware ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawLine3D_PF( Pos1F, Pos2F, Color, TRUE, NULL ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_EndLarge3DPositionSupportDraw( &Large3DPosDrawInfo ) ;
	}

	// �I��
	return Ret ;
}

// �R�c�̎O�p�`��`�悷��
extern int NS_DrawTriangle3D( VECTOR Pos1, VECTOR Pos2, VECTOR Pos3, unsigned int Color, int FillFlag )
{
	LARGE3DPOSITIONSUPPORT_DRAWINFO Large3DPosDrawInfo ;
	int Ret ;
	int Flag ;

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE ) return -1 ;

	// ��`��ݒ肪����Ă���ꍇ�͉��������I��
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �u�����h�e�N�X�`���͎g�p�ł��Ȃ�
	if( GSYS.DrawSetting.BlendGraph > 0 )
		return -1 ;

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_BeginLarge3DPositionSupportDraw( &Large3DPosDrawInfo, VConvFtoD( Pos1 ) ) ; 

		Pos2 = VSub( Pos2, Pos1 ) ;
		Pos3 = VSub( Pos3, Pos1 ) ;
		Pos1 = VGet( 0.0f, 0.0f, 0.0f ) ;
	}

	Flag = GSYS.Setting.ValidHardware ;

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawTriangle3D_PF( Pos1, Pos2, Pos3, Color, FillFlag, TRUE, NULL ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_EndLarge3DPositionSupportDraw( &Large3DPosDrawInfo ) ;
	}

	// �I��
	return Ret ;
}

// �R�c�̎O�p�`��`�悷��
extern int NS_DrawTriangle3DD( VECTOR_D Pos1, VECTOR_D Pos2, VECTOR_D Pos3, unsigned int Color, int FillFlag )
{
	LARGE3DPOSITIONSUPPORT_DRAWINFO Large3DPosDrawInfo ;
	int Ret ;
	int Flag ;
	VECTOR Pos1F, Pos2F, Pos3F ;

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE ) return -1 ;

	// ��`��ݒ肪����Ă���ꍇ�͉��������I��
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �u�����h�e�N�X�`���͎g�p�ł��Ȃ�
	if( GSYS.DrawSetting.BlendGraph > 0 )
		return -1 ;

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_BeginLarge3DPositionSupportDraw( &Large3DPosDrawInfo, Pos1 ) ; 

		Pos2 = VSubD( Pos2, Pos1 ) ;
		Pos3 = VSubD( Pos3, Pos1 ) ;
		Pos1 = VGetD( 0.0, 0.0, 0.0 ) ;
	}

	Pos1F = VConvDtoF( Pos1 ) ;
	Pos2F = VConvDtoF( Pos2 ) ;
	Pos3F = VConvDtoF( Pos3 ) ;

	Flag = GSYS.Setting.ValidHardware ;

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawTriangle3D_PF( Pos1F, Pos2F, Pos3F, Color, FillFlag, TRUE, NULL ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_EndLarge3DPositionSupportDraw( &Large3DPosDrawInfo ) ;
	}

	// �I��
	return Ret ;
}

// �R�c�̗����̂�`�悷��
extern int NS_DrawCube3D( VECTOR Pos1, VECTOR Pos2, unsigned int DifColor, unsigned int SpcColor, int FillFlag )
{
	return NS_DrawCube3DD( VConvFtoD( Pos1 ), VConvFtoD( Pos2 ), DifColor, SpcColor, FillFlag ) ;
}

// �R�c�̗����̂�`�悷��
extern int NS_DrawCube3DD( VECTOR_D Pos1, VECTOR_D Pos2, unsigned int DifColor, unsigned int SpcColor, int FillFlag )
{
	LARGE3DPOSITIONSUPPORT_DRAWINFO Large3DPosDrawInfo ;
	VECTOR Pos1F, Pos2F ;

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_BeginLarge3DPositionSupportDraw( &Large3DPosDrawInfo, Pos1 ) ; 

		Pos2 = VSubD( Pos2, Pos1 ) ;
		Pos1 = VGetD( 0.0, 0.0, 0.0 ) ;
	}

	Pos1F = VConvDtoF( Pos1 ) ;
	Pos2F = VConvDtoF( Pos2 ) ;

	if( GSYS.Light.ProcessDisable == FALSE && FillFlag == TRUE )
	{
		static WORD FillIndex[ 36 ] =
		{
			 0,  1,  2,   3,  2,  1,
			 4,  5,  6,   7,  6,  5,
			 8,  9, 10,  11, 10,  9,
			12, 13, 14,  15, 14, 13,
			16, 17, 18,  19, 18, 17,
			20, 21, 22,  23, 22, 21,
		} ;
		VERTEX3D Vertex[ 6 ][ 4 ] ;
		int i, dr, dg, db, a, sr, sg, sb ;

		// ���_�f�[�^�̍쐬
		NS_GetColor2( DifColor, &dr, &dg, &db ) ;
		NS_GetColor2( SpcColor, &sr, &sg, &sb ) ;
		a = GSYS.DrawSetting.BlendMode == DX_BLENDMODE_NOBLEND ? 255 : GSYS.DrawSetting.BlendParam ;
		Vertex[ 0 ][ 0 ].pos.x = Pos1F.x ; Vertex[ 0 ][ 0 ].pos.y = Pos2F.y ; Vertex[ 0 ][ 0 ].pos.z = Pos1F.z ;
		Vertex[ 0 ][ 1 ].pos.x = Pos2F.x ; Vertex[ 0 ][ 1 ].pos.y = Pos2F.y ; Vertex[ 0 ][ 1 ].pos.z = Pos1F.z ;
		Vertex[ 0 ][ 2 ].pos.x = Pos1F.x ; Vertex[ 0 ][ 2 ].pos.y = Pos1F.y ; Vertex[ 0 ][ 2 ].pos.z = Pos1F.z ;
		Vertex[ 0 ][ 3 ].pos.x = Pos2F.x ; Vertex[ 0 ][ 3 ].pos.y = Pos1F.y ; Vertex[ 0 ][ 3 ].pos.z = Pos1F.z ;
		Vertex[ 0 ][ 0 ].norm.x = 0.0f ;
		Vertex[ 0 ][ 0 ].norm.y = 0.0f ;
		Vertex[ 0 ][ 0 ].norm.z = -1.0f ;
		Vertex[ 0 ][ 1 ].norm = Vertex[ 0 ][ 0 ].norm ;
		Vertex[ 0 ][ 2 ].norm = Vertex[ 0 ][ 0 ].norm ;
		Vertex[ 0 ][ 3 ].norm = Vertex[ 0 ][ 0 ].norm ;

		Vertex[ 1 ][ 0 ].pos.x = Pos2F.x ; Vertex[ 1 ][ 0 ].pos.y = Pos2F.y ; Vertex[ 1 ][ 0 ].pos.z = Pos2F.z ;
		Vertex[ 1 ][ 1 ].pos.x = Pos1F.x ; Vertex[ 1 ][ 1 ].pos.y = Pos2F.y ; Vertex[ 1 ][ 1 ].pos.z = Pos2F.z ;
		Vertex[ 1 ][ 2 ].pos.x = Pos2F.x ; Vertex[ 1 ][ 2 ].pos.y = Pos1F.y ; Vertex[ 1 ][ 2 ].pos.z = Pos2F.z ;
		Vertex[ 1 ][ 3 ].pos.x = Pos1F.x ; Vertex[ 1 ][ 3 ].pos.y = Pos1F.y ; Vertex[ 1 ][ 3 ].pos.z = Pos2F.z ;
		Vertex[ 1 ][ 0 ].norm.x = 0.0f ;
		Vertex[ 1 ][ 0 ].norm.y = 0.0f ;
		Vertex[ 1 ][ 0 ].norm.z = 1.0f ;
		Vertex[ 1 ][ 1 ].norm = Vertex[ 1 ][ 0 ].norm ;
		Vertex[ 1 ][ 2 ].norm = Vertex[ 1 ][ 0 ].norm ;
		Vertex[ 1 ][ 3 ].norm = Vertex[ 1 ][ 0 ].norm ;

		Vertex[ 2 ][ 0 ].pos.x = Pos1F.x ; Vertex[ 2 ][ 0 ].pos.y = Pos2F.y ; Vertex[ 2 ][ 0 ].pos.z = Pos2F.z ;
		Vertex[ 2 ][ 1 ].pos.x = Pos1F.x ; Vertex[ 2 ][ 1 ].pos.y = Pos2F.y ; Vertex[ 2 ][ 1 ].pos.z = Pos1F.z ;
		Vertex[ 2 ][ 2 ].pos.x = Pos1F.x ; Vertex[ 2 ][ 2 ].pos.y = Pos1F.y ; Vertex[ 2 ][ 2 ].pos.z = Pos2F.z ;
		Vertex[ 2 ][ 3 ].pos.x = Pos1F.x ; Vertex[ 2 ][ 3 ].pos.y = Pos1F.y ; Vertex[ 2 ][ 3 ].pos.z = Pos1F.z ;
		Vertex[ 2 ][ 0 ].norm.x = -1.0f ;
		Vertex[ 2 ][ 0 ].norm.y = 0.0f ;
		Vertex[ 2 ][ 0 ].norm.z = 0.0f ;
		Vertex[ 2 ][ 1 ].norm = Vertex[ 2 ][ 0 ].norm ;
		Vertex[ 2 ][ 2 ].norm = Vertex[ 2 ][ 0 ].norm ;
		Vertex[ 2 ][ 3 ].norm = Vertex[ 2 ][ 0 ].norm ;

		Vertex[ 3 ][ 0 ].pos.x = Pos2F.x ; Vertex[ 3 ][ 0 ].pos.y = Pos2F.y ; Vertex[ 3 ][ 0 ].pos.z = Pos1F.z ;
		Vertex[ 3 ][ 1 ].pos.x = Pos2F.x ; Vertex[ 3 ][ 1 ].pos.y = Pos2F.y ; Vertex[ 3 ][ 1 ].pos.z = Pos2F.z ;
		Vertex[ 3 ][ 2 ].pos.x = Pos2F.x ; Vertex[ 3 ][ 2 ].pos.y = Pos1F.y ; Vertex[ 3 ][ 2 ].pos.z = Pos1F.z ;
		Vertex[ 3 ][ 3 ].pos.x = Pos2F.x ; Vertex[ 3 ][ 3 ].pos.y = Pos1F.y ; Vertex[ 3 ][ 3 ].pos.z = Pos2F.z ;
		Vertex[ 3 ][ 0 ].norm.x = 1.0f ;
		Vertex[ 3 ][ 0 ].norm.y = 0.0f ;
		Vertex[ 3 ][ 0 ].norm.z = 0.0f ;
		Vertex[ 3 ][ 1 ].norm = Vertex[ 3 ][ 0 ].norm ;
		Vertex[ 3 ][ 2 ].norm = Vertex[ 3 ][ 0 ].norm ;
		Vertex[ 3 ][ 3 ].norm = Vertex[ 3 ][ 0 ].norm ;

		Vertex[ 4 ][ 0 ].pos.x = Pos1F.x ; Vertex[ 4 ][ 0 ].pos.y = Pos2F.y ; Vertex[ 4 ][ 0 ].pos.z = Pos2F.z ;
		Vertex[ 4 ][ 1 ].pos.x = Pos2F.x ; Vertex[ 4 ][ 1 ].pos.y = Pos2F.y ; Vertex[ 4 ][ 1 ].pos.z = Pos2F.z ;
		Vertex[ 4 ][ 2 ].pos.x = Pos1F.x ; Vertex[ 4 ][ 2 ].pos.y = Pos2F.y ; Vertex[ 4 ][ 2 ].pos.z = Pos1F.z ;
		Vertex[ 4 ][ 3 ].pos.x = Pos2F.x ; Vertex[ 4 ][ 3 ].pos.y = Pos2F.y ; Vertex[ 4 ][ 3 ].pos.z = Pos1F.z ;
		Vertex[ 4 ][ 0 ].norm.x = 0.0f ;
		Vertex[ 4 ][ 0 ].norm.y = 1.0f ;
		Vertex[ 4 ][ 0 ].norm.z = 0.0f ;
		Vertex[ 4 ][ 1 ].norm = Vertex[ 4 ][ 0 ].norm ;
		Vertex[ 4 ][ 2 ].norm = Vertex[ 4 ][ 0 ].norm ;
		Vertex[ 4 ][ 3 ].norm = Vertex[ 4 ][ 0 ].norm ;

		Vertex[ 5 ][ 0 ].pos.x = Pos1F.x ; Vertex[ 5 ][ 0 ].pos.y = Pos1F.y ; Vertex[ 5 ][ 0 ].pos.z = Pos1F.z ;
		Vertex[ 5 ][ 1 ].pos.x = Pos2F.x ; Vertex[ 5 ][ 1 ].pos.y = Pos1F.y ; Vertex[ 5 ][ 1 ].pos.z = Pos1F.z ;
		Vertex[ 5 ][ 2 ].pos.x = Pos1F.x ; Vertex[ 5 ][ 2 ].pos.y = Pos1F.y ; Vertex[ 5 ][ 2 ].pos.z = Pos2F.z ;
		Vertex[ 5 ][ 3 ].pos.x = Pos2F.x ; Vertex[ 5 ][ 3 ].pos.y = Pos1F.y ; Vertex[ 5 ][ 3 ].pos.z = Pos2F.z ;
		Vertex[ 5 ][ 0 ].norm.x = 0.0f ;
		Vertex[ 5 ][ 0 ].norm.y = -1.0f ;
		Vertex[ 5 ][ 0 ].norm.z = 0.0f ;
		Vertex[ 5 ][ 1 ].norm = Vertex[ 5 ][ 0 ].norm ;
		Vertex[ 5 ][ 2 ].norm = Vertex[ 5 ][ 0 ].norm ;
		Vertex[ 5 ][ 3 ].norm = Vertex[ 5 ][ 0 ].norm ;
		for( i = 0 ; i < 4 * 6 ; i ++ )
		{
			( ( VERTEX3D * )Vertex )[ i ].dif.b = ( BYTE )db ;
			( ( VERTEX3D * )Vertex )[ i ].dif.g = ( BYTE )dg ;
			( ( VERTEX3D * )Vertex )[ i ].dif.r = ( BYTE )dr ;
			( ( VERTEX3D * )Vertex )[ i ].dif.a = ( BYTE )a ;
			( ( VERTEX3D * )Vertex )[ i ].spc.b = ( BYTE )sb ;
			( ( VERTEX3D * )Vertex )[ i ].spc.g = ( BYTE )sg ;
			( ( VERTEX3D * )Vertex )[ i ].spc.r = ( BYTE )sr ;
			( ( VERTEX3D * )Vertex )[ i ].spc.a = 0 ;
			( ( VERTEX3D * )Vertex )[ i ].u = 0.0f ;
			( ( VERTEX3D * )Vertex )[ i ].v = 0.0f ;
			( ( VERTEX3D * )Vertex )[ i ].su = 0.0f ;
			( ( VERTEX3D * )Vertex )[ i ].sv = 0.0f ;
		}

		NS_DrawPrimitiveIndexed3D( ( VERTEX3D * )Vertex, 6 * 4, FillIndex, 36, DX_PRIMTYPE_TRIANGLELIST, DX_NONE_GRAPH, TRUE ) ;
	}
	else
	{
		static WORD FillIndex[ 36 ] =
		{
			0, 1, 3,  3, 1, 2,  3, 2, 7,
			7, 2, 6,  2, 1, 6,  6, 1, 5,
			1, 0, 5,  5, 0, 4,  0, 3, 4,
			4, 3, 7,  7, 6, 4,  4, 6, 5,
		} ;
		static WORD LineIndex[ 24 ] =
		{
			0, 1,   1, 2,   2, 3,   3, 0,
			0, 4,   1, 5,   2, 6,   3, 7,
			4, 5,   5, 6,   6, 7,   7, 4,
		} ;
		VERTEX3D Vertex[ 8 ] ;
		int i, dr, dg, db, a, sr, sg, sb ;

		// ���_�f�[�^�̍쐬
		NS_GetColor2( DifColor, &dr, &dg, &db ) ;
		NS_GetColor2( SpcColor, &sr, &sg, &sb ) ;
		a = GSYS.DrawSetting.BlendMode == DX_BLENDMODE_NOBLEND ? 255 : GSYS.DrawSetting.BlendParam ;
		Vertex[ 0 ].pos.x = Pos1F.x ; Vertex[ 0 ].pos.y = Pos2F.y ; Vertex[ 0 ].pos.z = Pos2F.z ;
		Vertex[ 1 ].pos.x = Pos2F.x ; Vertex[ 1 ].pos.y = Pos2F.y ; Vertex[ 1 ].pos.z = Pos2F.z ;
		Vertex[ 2 ].pos.x = Pos2F.x ; Vertex[ 2 ].pos.y = Pos2F.y ; Vertex[ 2 ].pos.z = Pos1F.z ;
		Vertex[ 3 ].pos.x = Pos1F.x ; Vertex[ 3 ].pos.y = Pos2F.y ; Vertex[ 3 ].pos.z = Pos1F.z ;
		Vertex[ 4 ].pos.x = Pos1F.x ; Vertex[ 4 ].pos.y = Pos1F.y ; Vertex[ 4 ].pos.z = Pos2F.z ;
		Vertex[ 5 ].pos.x = Pos2F.x ; Vertex[ 5 ].pos.y = Pos1F.y ; Vertex[ 5 ].pos.z = Pos2F.z ;
		Vertex[ 6 ].pos.x = Pos2F.x ; Vertex[ 6 ].pos.y = Pos1F.y ; Vertex[ 6 ].pos.z = Pos1F.z ;
		Vertex[ 7 ].pos.x = Pos1F.x ; Vertex[ 7 ].pos.y = Pos1F.y ; Vertex[ 7 ].pos.z = Pos1F.z ;
		for( i = 0 ; i < 8 ; i ++ )
		{
			Vertex[ i ].dif.b = ( BYTE )db ;
			Vertex[ i ].dif.g = ( BYTE )dg ;
			Vertex[ i ].dif.r = ( BYTE )dr ;
			Vertex[ i ].dif.a = ( BYTE )a ;
			Vertex[ i ].spc.b = ( BYTE )sb ;
			Vertex[ i ].spc.g = ( BYTE )sg ;
			Vertex[ i ].spc.r = ( BYTE )sr ;
			Vertex[ i ].spc.a = 0 ;
			Vertex[ i ].u = 0.0f ;
			Vertex[ i ].v = 0.0f ;
			Vertex[ i ].su = 0.0f ;
			Vertex[ i ].sv = 0.0f ;
		}

		if( FillFlag == TRUE ) 
		{
			NS_DrawPrimitiveIndexed3D( Vertex, 8, FillIndex, 36, DX_PRIMTYPE_TRIANGLELIST, DX_NONE_GRAPH, TRUE ) ;
		}
		else
		{
			NS_DrawPrimitiveIndexed3D( Vertex, 8, LineIndex, 24, DX_PRIMTYPE_LINELIST, DX_NONE_GRAPH, TRUE ) ;
		}
	}

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_EndLarge3DPositionSupportDraw( &Large3DPosDrawInfo ) ;
	}

	// �I��
	return 0 ;
}

// �R�c�̋��̂�`�悷��
extern int NS_DrawSphere3D( VECTOR CenterPos, float r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag )
{
	return NS_DrawSphere3DD( VConvFtoD( CenterPos ), r, DivNum, DifColor, SpcColor, FillFlag ) ;
}

// �R�c�̋��̂�`�悷��
extern int NS_DrawSphere3DD( VECTOR_D CenterPos, double r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag )
{
	VERTEX3D *Vertex, *vert ;
	WORD *Index, *ind ;
	float *SinCosTable1, *SinCosTable2, *t1, *t2, rf ;
	int CirVertNumH, CirVertNumV, i, j, k, l, indexnum, vertnum ;
	int dr, dg, db, a, sr, sg, sb ;
	VECTOR CenterPosF ;
	LARGE3DPOSITIONSUPPORT_DRAWINFO Large3DPosDrawInfo ;

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_BeginLarge3DPositionSupportDraw( &Large3DPosDrawInfo, CenterPos ) ; 

		CenterPos.x = 0.0 ;
		CenterPos.y = 0.0 ;
		CenterPos.z = 0.0 ;
	}

	CenterPosF = VConvDtoF( CenterPos ) ;
	rf         = ( float )r ;

	NS_GetColor2( DifColor, &dr, &dg, &db ) ;
	NS_GetColor2( SpcColor, &sr, &sg, &sb ) ;
	a = GSYS.DrawSetting.BlendMode == DX_BLENDMODE_NOBLEND ? 255 : GSYS.DrawSetting.BlendParam ;

	CirVertNumH = DivNum + 4 ;
	CirVertNumV = DivNum + 1 ;
	indexnum = FillFlag ? ( CirVertNumV - 1 ) * CirVertNumH * 6 + CirVertNumH * 3 * 2 : CirVertNumH * ( CirVertNumV + 1 ) * 2 + CirVertNumV * CirVertNumH * 2 ;
	vertnum = CirVertNumV * CirVertNumH + 2 ;
	Vertex = ( VERTEX3D * )DXALLOC( sizeof( WORD ) * indexnum + sizeof( VERTEX3D ) * vertnum + sizeof( float ) * ( CirVertNumV + CirVertNumH ) * 2 ) ;
	if( Vertex == NULL )
	{
		if( GSYS.DrawSetting.Large3DPositionSupport )
		{
			Graphics_Draw_EndLarge3DPositionSupportDraw( &Large3DPosDrawInfo ) ;
		}
		return 0 ;
	}
	SinCosTable1 = ( float * )( Vertex + vertnum ) ;
	SinCosTable2 = SinCosTable1 + CirVertNumV * 2 ;
	Index = ( WORD * )( SinCosTable2 + CirVertNumH * 2 ) ;

	// ���_�f�[�^�̍쐬
	t1 = SinCosTable1 ;
	t2 = SinCosTable2 ;
	for( i = 0 ; i < CirVertNumH ; i ++, t2 += 2 )
	{
		_SINCOS_PLATFORM( 2 * ( float )DX_PI / CirVertNumH * i, t2, t2 + 1 ) ;
	}
	for( i = 0 ; i < CirVertNumV ; i ++, t1 += 2 )
	{
		_SINCOS_PLATFORM( ( float )DX_PI / 2.0f - ( float )DX_PI / ( CirVertNumV + 1 ) * ( i + 1 ), t1, t1 + 1 ) ;
	}
	vert = Vertex ;
	t1 = SinCosTable1 ;
	for( i = 0 ; i < CirVertNumV ; i ++, t1 += 2 )
	{
		t2 = SinCosTable2 ;
		for( j = 0 ; j < CirVertNumH ; j ++, vert ++, t2 += 2 )
		{
			vert->pos.x = t2[ 1 ] * t1[ 1 ] * rf + CenterPosF.x ;
			vert->pos.y = t1[ 0 ] * rf           + CenterPosF.y ;
			vert->pos.z = t2[ 0 ] * t1[ 1 ] * rf + CenterPosF.z ;
			vert->norm.x = t2[ 1 ] * t1[ 1 ] ;
			vert->norm.y = t1[ 0 ] ;
			vert->norm.z = t2[ 0 ] * t1[ 1 ] ;

			vert->dif.b = ( BYTE )db ;
			vert->dif.g = ( BYTE )dg ;
			vert->dif.r = ( BYTE )dr ;
			vert->dif.a = ( BYTE )a ;
			vert->spc.b = ( BYTE )sb ;
			vert->spc.g = ( BYTE )sg ;
			vert->spc.r = ( BYTE )sr ;
			vert->spc.a = 0 ;
			vert->u = 0.0f ;
			vert->v = 0.0f ;
			vert->su = 0.0f ;
			vert->sv = 0.0f ;
		}
	}
	vert->pos.x = 0.0f + CenterPosF.x ;
	vert->pos.y =   rf + CenterPosF.y ;
	vert->pos.z = 0.0f + CenterPosF.z ;
	vert->norm.x = 0.0f ;
	vert->norm.y = 1.0f ;
	vert->norm.z = 0.0f ;

	vert->dif.b = ( BYTE )db ;
	vert->dif.g = ( BYTE )dg ;
	vert->dif.r = ( BYTE )dr ;
	vert->dif.a = ( BYTE )a ;
	vert->spc.b = ( BYTE )sb ;
	vert->spc.g = ( BYTE )sg ;
	vert->spc.r = ( BYTE )sr ;
	vert->spc.a = 0 ;
	vert->u = 0.0f ;
	vert->v = 0.0f ;
	vert->su = 0.0f ;
	vert->sv = 0.0f ;

	vert ++ ;

	vert->pos.x = 0.0f + CenterPosF.x ;
	vert->pos.y =  -rf + CenterPosF.y ;
	vert->pos.z = 0.0f + CenterPosF.z ;
	vert->norm.x = 0.0f ;
	vert->norm.y = -1.0f ;
	vert->norm.z = 0.0f ;

	vert->dif.b = ( BYTE )db ;
	vert->dif.g = ( BYTE )dg ;
	vert->dif.r = ( BYTE )dr ;
	vert->dif.a = ( BYTE )a ;
	vert->spc.b = ( BYTE )sb ;
	vert->spc.g = ( BYTE )sg ;
	vert->spc.r = ( BYTE )sr ;
	vert->spc.a = 0 ;
	vert->u = 0.0f ;
	vert->v = 0.0f ;
	vert->su = 0.0f ;
	vert->sv = 0.0f ;

	vert ++ ;

	// �h��Ԃ����ǂ����ŏ����𕪊�
	if( FillFlag )
	{
		ind = Index ;
		l = CirVertNumV * CirVertNumH ;
		for( j = 0 ; j < CirVertNumH - 1 ; j ++, ind += 3 )
		{
			ind[ 0 ] = ( WORD )j ;
			ind[ 1 ] = ( WORD )l ;
			ind[ 2 ] = ( WORD )( j + 1 ) ;
		}
		ind[ 0 ] = ( WORD )j ;
		ind[ 1 ] = ( WORD )l ;
		ind[ 2 ] = 0 ;

		ind += 3 ;

		k = 0 ;
		for( i = 0 ; i < CirVertNumV - 1 ; i ++, k += CirVertNumH )
		{
			for( j = 0 ; j < CirVertNumH - 1 ; j ++, ind += 6 )
			{
				ind[ 0 ] = ( WORD )( j + k ) ;
				ind[ 1 ] = ( WORD )( ind[ 0 ] + 1 ) ;
				ind[ 2 ] = ( WORD )( ind[ 0 ] + CirVertNumH ) ;
				
				ind[ 3 ] = ind[ 2 ] ;
				ind[ 4 ] = ind[ 1 ] ;
				ind[ 5 ] = ( WORD )( ind[ 3 ] + 1 ) ;
			}
			ind[ 0 ] = ( WORD )( j + k ) ;
			ind[ 1 ] = ( WORD )k ;
			ind[ 2 ] = ( WORD )( ind[ 0 ] + CirVertNumH ) ;
			
			ind[ 3 ] = ind[ 2 ] ;
			ind[ 4 ] = ind[ 1 ] ;
			ind[ 5 ] = ( WORD )( k + CirVertNumH ) ;

			ind += 6 ;
		}

		l = CirVertNumV * CirVertNumH + 1 ;
		for( j = 0 ; j < CirVertNumH - 1 ; j ++, ind += 3 )
		{
			ind[ 0 ] = ( WORD )( j + 1 + k ) ;
			ind[ 1 ] = ( WORD )l ;
			ind[ 2 ] = ( WORD )( j + k ) ;
		}
		ind[ 0 ] = ( WORD )k ;
		ind[ 1 ] = ( WORD )l ;
		ind[ 2 ] = ( WORD )( j + k ) ;

		ind += 3 ;
		NS_DrawPrimitiveIndexed3D( Vertex, vertnum, Index, indexnum, DX_PRIMTYPE_TRIANGLELIST, DX_NONE_GRAPH, TRUE ) ;
	}
	else
	{
		ind = Index ;
		for( i = 0 ; i < CirVertNumH ; i ++ )
		{
			ind[ 0 ] = ( WORD )( CirVertNumV * CirVertNumH ) ;
			ind[ 1 ] = ( WORD )i ;
			ind += 2 ;

			k = i ;
			for( j = 0 ; j < CirVertNumV - 1 ; j ++, k += CirVertNumH, ind += 2 )
			{
				ind[ 0 ] = ( WORD )k ;
				ind[ 1 ] = ( WORD )( k + CirVertNumH ) ;
			}
			
			ind[ 0 ] = ( WORD )k ;
			ind[ 1 ] = ( WORD )( CirVertNumV * CirVertNumH + 1 ) ;
			ind += 2 ;
		}

		k = 0 ;
		for( i = 0 ; i < CirVertNumV ; i ++, k += CirVertNumH )
		{
			for( j = 0 ; j < CirVertNumH - 1 ; j ++, ind += 2 )
			{
				ind[ 0 ] = ( WORD )( j + k ) ;
				ind[ 1 ] = ( WORD )( j + k + 1 ) ;
			}
			ind[ 0 ] = ( WORD )( j + k ) ;
			ind[ 1 ] = ( WORD )k ;
			ind += 2 ;
		}

		NS_DrawPrimitiveIndexed3D( Vertex, vertnum, Index, indexnum, DX_PRIMTYPE_LINELIST, DX_NONE_GRAPH, TRUE ) ;
	}

	// �������̉��
	DXFREE( Vertex ) ;

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_EndLarge3DPositionSupportDraw( &Large3DPosDrawInfo ) ;
	}

	// �I��
	return 0 ;
}

// �R�c�̃J�v�Z����`��
extern int NS_DrawCapsule3D( VECTOR Pos1, VECTOR Pos2, float r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag )
{
	return NS_DrawCapsule3DD( VConvFtoD( Pos1 ), VConvFtoD( Pos2 ), r, DivNum, DifColor, SpcColor, FillFlag ) ;
}

// �R�c�̃J�v�Z����`��
extern int NS_DrawCapsule3DD( VECTOR_D Pos1, VECTOR_D Pos2, double r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag )
{
	VERTEX3D *Vertex, *vert1, *vert2 ;
	WORD *Index, *ind ;
	VECTOR_D SubV, xv, yv, zv ;
	VECTOR xvf, yvf, zvf, Pos1F ;
	double Length ;
	float *SinCosTable1, *SinCosTable2, *t1, *t2, x, y, z, LengthF, rf ;
	int CirVertNumH, CirVertNumV, i, j, k, topind, botind, ettopind, etbotind, indexnum ;
	int dr, dg, db, a, sr, sg, sb, vertnum ;
	LARGE3DPOSITIONSUPPORT_DRAWINFO Large3DPosDrawInfo ;

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_BeginLarge3DPositionSupportDraw( &Large3DPosDrawInfo, Pos1 ) ; 

		Pos2 = VSubD( Pos2, Pos1 ) ;
		Pos1 = VGetD( 0.0, 0.0, 0.0 ) ;
	}

	rf = ( float )r ;
	Pos1F = VConvDtoF( Pos1 ) ;

	NS_GetColor2( DifColor, &dr, &dg, &db ) ;
	NS_GetColor2( SpcColor, &sr, &sg, &sb ) ;
	a = GSYS.DrawSetting.BlendMode == DX_BLENDMODE_NOBLEND ? 255 : GSYS.DrawSetting.BlendParam ;

	SubV   = VSubD( Pos2, Pos1 ) ;
	Length = VectorInnerProductD( &SubV, &SubV ) ;
	if( Length < 0.00000000001 )
	{
		xvf = VGet( 1.0, 0.0, 0.0 ) ;
		yvf = VGet( 0.0, 1.0, 0.0 ) ;
		zvf = VGet( 0.0, 0.0, 1.0 ) ;
	}
	else
	{
		Length = _SQRTD( Length ) ;
		yv.x = SubV.x / Length ;
		yv.y = SubV.y / Length ;
		yv.z = SubV.z / Length ;

		xv = VGetD( 1.0, 0.0, 0.0 ) ;
		VectorOuterProductD( &zv, &xv, &yv ) ;
		if( VSquareSizeD( zv ) < 0.0000000001f )
		{
			xv = VGetD( 0.0, 0.0, 1.0 ) ;
			VectorOuterProductD( &zv, &xv, &yv ) ;
		}
		VectorOuterProductD( &xv, &yv, &zv ) ;
		VectorNormalizeD( &xv, &xv ) ;
		VectorNormalizeD( &zv, &zv ) ;

		xvf = VConvDtoF( xv ) ;
		yvf = VConvDtoF( yv ) ;
		zvf = VConvDtoF( zv ) ;
	}
	LengthF = ( float )Length ;

	CirVertNumH = DivNum + 4 ;
	CirVertNumV = DivNum + 2 ;
	indexnum = FillFlag ? ( CirVertNumH * 3 + CirVertNumH * 6 * ( CirVertNumV - 1 ) ) * 2 + CirVertNumH * 6 : CirVertNumH * ( CirVertNumV * 2 + 1 ) * 2 + CirVertNumH * CirVertNumV * 2 * 2 ;
	vertnum  = ( CirVertNumV * CirVertNumH + 1 ) * 2 ;
	Vertex   = ( VERTEX3D * )DXALLOC( sizeof( WORD ) * indexnum + sizeof( VERTEX3D ) * vertnum + sizeof( float ) * ( CirVertNumH + CirVertNumV ) * 2 ) ;
	if( Vertex == NULL )
	{
		if( GSYS.DrawSetting.Large3DPositionSupport )
		{
			Graphics_Draw_EndLarge3DPositionSupportDraw( &Large3DPosDrawInfo ) ;
		}
		return 0 ;
	}
	SinCosTable1 = ( float * )( Vertex + vertnum ) ;
	SinCosTable2 = SinCosTable1 + CirVertNumV * 2 ;
	Index        = ( WORD * )( SinCosTable2 + CirVertNumH * 2 ) ;
	topind       = CirVertNumV * CirVertNumH * 2 ;
	botind       = CirVertNumV * CirVertNumH * 2 + 1 ;
	ettopind     = ( CirVertNumV - 1 ) * CirVertNumH ;
	etbotind     = CirVertNumV * CirVertNumH ;

	// ���_�f�[�^�̍쐬
	t1 = SinCosTable1 ;
	t2 = SinCosTable2 ;
	for( i = 0 ; i < CirVertNumH ; i ++, t2 += 2 )
	{
		_SINCOS_PLATFORM( 2 * ( float )DX_PI / CirVertNumH * i, t2, t2 + 1 ) ;
	}
	for( i = 0 ; i < CirVertNumV ; i ++, t1 += 2 )
	{
		_SINCOS_PLATFORM( ( float )DX_PI / 2.0f - ( float )( DX_PI / 2.0f ) / CirVertNumV * ( i + 1 ), t1, t1 + 1 ) ;
	}
	vert1 = Vertex ;
	t1 = SinCosTable1 ;
	for( i = 0 ; i < CirVertNumV ; i ++, t1 += 2 )
	{
		t2 = SinCosTable2 ;
		vert2 = Vertex + etbotind + ( CirVertNumV - 1 - i ) * CirVertNumH ;
		for( j = 0 ; j < CirVertNumH ; j ++, vert1 ++, vert2 ++, t2 += 2 )
		{
			vert1->pos.x = t2[ 1 ] * t1[ 1 ] * rf ;
			vert1->pos.y = t1[ 0 ] * rf + LengthF ;
			vert1->pos.z = t2[ 0 ] * t1[ 1 ] * rf ;
			vert1->norm.x = t2[ 1 ] * t1[ 1 ] ;
			vert1->norm.y = t1[ 0 ] ;
			vert1->norm.z = t2[ 0 ] * t1[ 1 ] ;

			vert1->dif.b = ( BYTE )db ;
			vert1->dif.g = ( BYTE )dg ;
			vert1->dif.r = ( BYTE )dr ;
			vert1->dif.a = ( BYTE )a ;
			vert1->spc.b = ( BYTE )sb ;
			vert1->spc.g = ( BYTE )sg ;
			vert1->spc.r = ( BYTE )sr ;
			vert1->spc.a = 0 ;
			vert1->u = 0.0f ;
			vert1->v = 0.0f ;
			vert1->su = 0.0f ;
			vert1->sv = 0.0f ;

			vert2->pos.x =    vert1->pos.x ;
			vert2->pos.y = -( vert1->pos.y - LengthF ) ;
			vert2->pos.z =    vert1->pos.z ;
			vert2->norm.x =  vert1->norm.x ;
			vert2->norm.y = -vert1->norm.y ;
			vert2->norm.z =  vert1->norm.z ;

			vert2->dif.b = ( BYTE )db ;
			vert2->dif.g = ( BYTE )dg ;
			vert2->dif.r = ( BYTE )dr ;
			vert2->dif.a = ( BYTE )a ;
			vert2->spc.b = ( BYTE )sb ;
			vert2->spc.g = ( BYTE )sg ;
			vert2->spc.r = ( BYTE )sr ;
			vert2->spc.a = 0 ;
			vert2->u = 0.0f ;
			vert2->v = 0.0f ;
			vert2->su = 0.0f ;
			vert2->sv = 0.0f ;
		}
	}

	vert1 = Vertex + topind ;
	vert1->pos.x = 0.0f ;
	vert1->pos.y = rf + LengthF ;
	vert1->pos.z = 0.0f ;
	vert1->norm.x = 0.0f ;
	vert1->norm.y = 1.0f ;
	vert1->norm.z = 0.0f ;

	vert1->dif.b = ( BYTE )db ;
	vert1->dif.g = ( BYTE )dg ;
	vert1->dif.r = ( BYTE )dr ;
	vert1->dif.a = ( BYTE )a ;
	vert1->spc.b = ( BYTE )sb ;
	vert1->spc.g = ( BYTE )sg ;
	vert1->spc.r = ( BYTE )sr ;
	vert1->spc.a = 0 ;
	vert1->u = 0.0f ;
	vert1->v = 0.0f ;
	vert1->su = 0.0f ;
	vert1->sv = 0.0f ;

	vert1 ++ ;

	vert1->pos.x = 0.0f ;
	vert1->pos.y = -rf ;
	vert1->pos.z = 0.0f ;
	vert1->norm.x = 0.0f ;
	vert1->norm.y = -1.0f ;
	vert1->norm.z = 0.0f ;

	vert1->dif.b = ( BYTE )db ;
	vert1->dif.g = ( BYTE )dg ;
	vert1->dif.r = ( BYTE )dr ;
	vert1->dif.a = ( BYTE )a ;
	vert1->spc.b = ( BYTE )sb ;
	vert1->spc.g = ( BYTE )sg ;
	vert1->spc.r = ( BYTE )sr ;
	vert1->spc.a = 0 ;
	vert1->u = 0.0f ;
	vert1->v = 0.0f ;
	vert1->su = 0.0f ;
	vert1->sv = 0.0f ;

	vert1 ++ ;

	vert1 = Vertex ;
	for( i = 0 ; i < vertnum ; i ++, vert1 ++ )
	{
		x = vert1->pos.x ;
		y = vert1->pos.y ;
		z = vert1->pos.z ;
		vert1->pos.x = x * xvf.x + y * yvf.x + z * zvf.x + Pos1F.x ;
		vert1->pos.y = x * xvf.y + y * yvf.y + z * zvf.y + Pos1F.y ;
		vert1->pos.z = x * xvf.z + y * yvf.z + z * zvf.z + Pos1F.z ;
		x = vert1->norm.x ;
		y = vert1->norm.y ;
		z = vert1->norm.z ;
		vert1->norm.x = x * xvf.x + y * yvf.x + z * zvf.x ;
		vert1->norm.y = x * xvf.y + y * yvf.y + z * zvf.y ;
		vert1->norm.z = x * xvf.z + y * yvf.z + z * zvf.z ;
	}

	// �h��Ԃ����ǂ����ŏ����𕪊�
	if( FillFlag )
	{
		ind = Index ;
		for( j = 0 ; j < CirVertNumH - 1 ; j ++, ind += 3 )
		{
			ind[ 0 ] = ( WORD )j ;
			ind[ 1 ] = ( WORD )topind ;
			ind[ 2 ] = ( WORD )( j + 1 ) ;
		}
		ind[ 0 ] = ( WORD )j ;
		ind[ 1 ] = ( WORD )topind ;
		ind[ 2 ] = 0 ;

		ind += 3 ;

		// �㕔�̋������̃C���f�b�N�X���X�g���쐬
		k = 0 ;
		for( i = 0 ; i < CirVertNumV - 1 ; i ++, k += CirVertNumH )
		{
			for( j = 0 ; j < CirVertNumH - 1 ; j ++, ind += 6 )
			{
				ind[ 0 ] = ( WORD )( j + k ) ;
				ind[ 1 ] = ( WORD )( ind[ 0 ] + 1 ) ;
				ind[ 2 ] = ( WORD )( ind[ 0 ] + CirVertNumH ) ;
				
				ind[ 3 ] = ind[ 2 ] ;
				ind[ 4 ] = ind[ 1 ] ;
				ind[ 5 ] = ( WORD )( ind[ 3 ] + 1 ) ;
			}
			ind[ 0 ] = ( WORD )( j + k ) ;
			ind[ 1 ] = ( WORD )k ;
			ind[ 2 ] = ( WORD )( ind[ 0 ] + CirVertNumH ) ;
			
			ind[ 3 ] = ind[ 2 ] ;
			ind[ 4 ] = ind[ 1 ] ;
			ind[ 5 ] = ( WORD )( k + CirVertNumH ) ;

			ind += 6 ;
		}

		// ���S�̉~�������̃C���f�b�N�X���X�g���쐬
		for( i = 0 ; i < CirVertNumH - 1 ; i ++, ind += 6 )
		{
			ind[ 0 ] = ( WORD )( ettopind + i ) ;
			ind[ 1 ] = ( WORD )( ind[ 0 ] + 1 ) ;
			ind[ 2 ] = ( WORD )( etbotind + i ) ;

			ind[ 3 ] = ind[ 2 ] ;
			ind[ 4 ] = ind[ 1 ] ;
			ind[ 5 ] = ( WORD )( ind[ 3 ] + 1 ) ;
		}
		ind[ 0 ] = ( WORD )( ettopind + i ) ;
		ind[ 1 ] = ( WORD )ettopind ;
		ind[ 2 ] = ( WORD )( etbotind + i ) ;

		ind[ 3 ] = ind[ 2 ] ;
		ind[ 4 ] = ind[ 1 ] ;
		ind[ 5 ] = ( WORD )etbotind ;

		ind += 6 ;
		k += CirVertNumH ;

		// �����̋������̃C���f�b�N�X���X�g���쐬
		for( i = 0 ; i < CirVertNumV - 1 ; i ++, k += CirVertNumH )
		{
			for( j = 0 ; j < CirVertNumH - 1 ; j ++, ind += 6 )
			{
				ind[ 0 ] = ( WORD )( j + k ) ;
				ind[ 1 ] = ( WORD )( ind[ 0 ] + 1 ) ;
				ind[ 2 ] = ( WORD )( ind[ 0 ] + CirVertNumH ) ;
				
				ind[ 3 ] = ind[ 2 ] ;
				ind[ 4 ] = ind[ 1 ] ;
				ind[ 5 ] = ( WORD )( ind[ 3 ] + 1 ) ;
			}
			ind[ 0 ] = ( WORD )( j + k ) ;
			ind[ 1 ] = ( WORD )k ;
			ind[ 2 ] = ( WORD )( ind[ 0 ] + CirVertNumH ) ;
			
			ind[ 3 ] = ind[ 2 ] ;
			ind[ 4 ] = ind[ 1 ] ;
			ind[ 5 ] = ( WORD )( k + CirVertNumH ) ;

			ind += 6 ;
		}

		for( j = 0 ; j < CirVertNumH - 1 ; j ++, ind += 3 )
		{
			ind[ 0 ] = ( WORD )( j + 1 + k ) ;
			ind[ 1 ] = ( WORD )botind ;
			ind[ 2 ] = ( WORD )( j + k ) ;
		}
		ind[ 0 ] = ( WORD )k ;
		ind[ 1 ] = ( WORD )botind ;
		ind[ 2 ] = ( WORD )( j + k ) ;

		ind += 3 ;
		NS_DrawPrimitiveIndexed3D( Vertex, vertnum, Index, indexnum, DX_PRIMTYPE_TRIANGLELIST, DX_NONE_GRAPH, TRUE ) ;
	}
	else
	{
		ind = Index ;
		for( i = 0 ; i < CirVertNumH ; i ++ )
		{
			ind[ 0 ] = ( WORD )topind ;
			ind[ 1 ] = ( WORD )i ;
			ind += 2 ;

			k = i ;
			for( j = 0 ; j < CirVertNumV * 2 - 1 ; j ++, k += CirVertNumH, ind += 2 )
			{
				ind[ 0 ] = ( WORD )k ;
				ind[ 1 ] = ( WORD )( k + CirVertNumH ) ;
			}
			
			ind[ 0 ] = ( WORD )k ;
			ind[ 1 ] = ( WORD )botind ;
			ind += 2 ;
		}

		k = 0 ;
		for( i = 0 ; i < CirVertNumV * 2 ; i ++, k += CirVertNumH )
		{
			for( j = 0 ; j < CirVertNumH - 1 ; j ++, ind += 2 )
			{
				ind[ 0 ] = ( WORD )( j + k ) ;
				ind[ 1 ] = ( WORD )( j + k + 1 ) ;
			}
			ind[ 0 ] = ( WORD )( j + k ) ;
			ind[ 1 ] = ( WORD )k ;
			ind += 2 ;
		}

		NS_DrawPrimitiveIndexed3D( Vertex, vertnum, Index, indexnum, DX_PRIMTYPE_LINELIST, DX_NONE_GRAPH, TRUE ) ;
	}

	// �������̉��
	DXFREE( Vertex ) ;

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_EndLarge3DPositionSupportDraw( &Large3DPosDrawInfo ) ;
	}

	// �I��
	return 0 ;
}

// �R�c�̉~����`�悷��
extern int NS_DrawCone3D( VECTOR TopPos, VECTOR BottomPos, float r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag )
{
	return NS_DrawCone3DD( VConvFtoD( TopPos ), VConvFtoD( BottomPos ), r, DivNum, DifColor, SpcColor, FillFlag ) ;
}

// �R�c�̉~����`�悷��
extern int NS_DrawCone3DD( VECTOR_D TopPos, VECTOR_D BottomPos, double r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag )
{
	VERTEX3D *Vertex, *vert1, *vert2, *vert3 ;
	MATRIX   Axis ;
	WORD *Index, *ind ;
	VECTOR_D SubV, xv, yv, zv ;
	VECTOR bottomvec, norm, TopPosF ;
	double Length ;
	float *SinCosTable, *t, rf ;
	int CirVertNum, vertnum, indexnum ;
	int dr, dg, db, a, sr, sg, sb, i, j, k, topind, bottomind, num ;
	LARGE3DPOSITIONSUPPORT_DRAWINFO Large3DPosDrawInfo ;

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_BeginLarge3DPositionSupportDraw( &Large3DPosDrawInfo, TopPos ) ; 

		BottomPos = VSubD( BottomPos, TopPos ) ;
		TopPos    = VGetD( 0.0, 0.0, 0.0 ) ;
	}

	TopPosF = VConvDtoF( TopPos ) ;
	rf      = ( float )r ;

	NS_GetColor2( DifColor, &dr, &dg, &db ) ;
	NS_GetColor2( SpcColor, &sr, &sg, &sb ) ;
	a = GSYS.DrawSetting.BlendMode == DX_BLENDMODE_NOBLEND ? 255 : GSYS.DrawSetting.BlendParam ;

	SubV   = VSubD( TopPos, BottomPos ) ;
	Length = VSizeD( SubV ) ;
	zv     = VScaleD( SubV, 1.0 / Length ) ;
	xv     = VGetD( 1.0, 0.0, 0.0 ) ;
	VectorOuterProductD( &yv, &xv, &zv ) ;
	if( VSquareSizeD( yv ) < 0.000000001 )
	{
		xv = VGetD( 0.0, 0.0, 1.0 ) ;
		VectorOuterProductD( &yv, &xv, &zv ) ;
	}
	xv     = VNormD( VCrossD( yv, zv ) ) ;
	yv     = VNormD( yv ) ;
	Axis   = MGetAxis1( VConvDtoF( xv ), VConvDtoF( yv ), VConvDtoF( zv ), VConvDtoF( BottomPos ) ) ;

	norm = VConvDtoF( VNormD( VCrossD( VGetD( 0.0, 0.0, 1.0 ), VSubD( VGetD( r, 0.0, 0.0 ), VGetD( 0.0, Length, 0.0 ) ) ) ) ) ;

	CirVertNum = DivNum + 4 ;
	vertnum    = CirVertNum * 3 ;
	topind     = CirVertNum ;
	bottomind  = CirVertNum * 2 ;
	indexnum   = FillFlag ? CirVertNum * 3 + ( CirVertNum - 2 ) * 3 : CirVertNum * 2 + CirVertNum * 2 ;
	Vertex     = ( VERTEX3D * )DXALLOC( sizeof( WORD ) * indexnum + sizeof( VERTEX3D ) * vertnum + sizeof( float ) * CirVertNum * 2 ) ;
	if( Vertex == NULL )
	{
		if( GSYS.DrawSetting.Large3DPositionSupport )
		{
			Graphics_Draw_EndLarge3DPositionSupportDraw( &Large3DPosDrawInfo ) ;
		}
		return 0 ;
	}
	SinCosTable = ( float * )( Vertex + vertnum ) ;
	Index       = ( WORD * )( SinCosTable + CirVertNum * 2 ) ;

	// ���_�f�[�^�̍쐬
	t = SinCosTable ;
	for( i = 0 ; i < CirVertNum ; i ++, t += 2 )
	{
		_SINCOS_PLATFORM( 2 * DX_PI_F / CirVertNum * i, t, t + 1 ) ;
	}
	vert1 = Vertex ;
	vert2 = Vertex + CirVertNum ;
	vert3 = Vertex + CirVertNum * 2 ;
	t = SinCosTable ;
	bottomvec = VTransformSR( VGet( 0.0f, 0.0f, -1.0f ), Axis ) ;
	for( i = 0 ; i < CirVertNum ; i ++, vert1 ++, vert2 ++, vert3 ++, t += 2 )
	{
		vert1->pos = VTransform( VGet( t[ 0 ] * rf, t[ 1 ] * rf, 0.0f ), Axis ) ;
		vert2->pos = TopPosF ;
		vert3->pos = vert1->pos ;
		vert1->norm = VTransformSR( VGet( t[ 0 ] * norm.x, t[ 1 ] * norm.x, norm.y ), Axis ) ;
		vert2->norm = vert1->norm ;
		vert3->norm = bottomvec ;

		vert1->dif.b = ( BYTE )db ; vert1->dif.g = ( BYTE )dg ; vert1->dif.r = ( BYTE )dr ; vert1->dif.a = ( BYTE )a ;
		vert2->dif.b = ( BYTE )db ; vert2->dif.g = ( BYTE )dg ; vert2->dif.r = ( BYTE )dr ; vert2->dif.a = ( BYTE )a ;
		vert3->dif.b = ( BYTE )db ; vert3->dif.g = ( BYTE )dg ; vert3->dif.r = ( BYTE )dr ; vert3->dif.a = ( BYTE )a ;
		vert1->spc.b = ( BYTE )sb ; vert1->spc.g = ( BYTE )sg ; vert1->spc.r = ( BYTE )sr ; vert1->spc.a = 0 ;
		vert2->spc.b = ( BYTE )sb ; vert2->spc.g = ( BYTE )sg ; vert2->spc.r = ( BYTE )sr ; vert2->spc.a = 0 ;
		vert3->spc.b = ( BYTE )sb ; vert3->spc.g = ( BYTE )sg ; vert3->spc.r = ( BYTE )sr ; vert3->spc.a = 0 ;
		vert1->u = 0.0f ; vert1->v = 0.0f ;
		vert2->u = 0.0f ; vert2->v = 0.0f ;
		vert3->u = 0.0f ; vert3->v = 0.0f ;
		vert1->su = 0.0f ; vert1->sv = 0.0f ;
		vert2->su = 0.0f ; vert2->sv = 0.0f ;
		vert3->su = 0.0f ; vert3->sv = 0.0f ;
	}

	// �h��Ԃ����ǂ����ŏ����𕪊�
	if( FillFlag )
	{
		ind = Index ;
		for( i = 0 ; i < CirVertNum - 1 ; i ++, ind += 3 )
		{
			ind[ 0 ] = ( WORD )i ;
			ind[ 1 ] = ( WORD )( topind + i ) ;
			ind[ 2 ] = ( WORD )( i + 1 ) ;
		}
		ind[ 0 ] = ( WORD )i ;
		ind[ 1 ] = ( WORD )( topind + i ) ;
		ind[ 2 ] = 0 ;
		ind += 3 ;

		j = bottomind + CirVertNum - 1 ;
		i = bottomind ;
		ind[ 0 ] = ( WORD )i ;
		ind[ 1 ] = ( WORD )( i + 1 ) ;
		ind[ 2 ] = ( WORD )j ;
		ind += 3 ;

		num = CirVertNum - 2 ;
		for( k = 1 ; k < num ; )
		{
			ind[ 0 ] = ( WORD )j ;
			ind[ 1 ] = ( WORD )( i + 1 ) ;
			ind[ 2 ] = ( WORD )( i + 2 ) ;
			ind += 3 ;
			k ++ ;
			i ++ ;
			if( k >= num ) break ;

			ind[ 0 ] = ( WORD )j ;
			ind[ 1 ] = ( WORD )( i + 1 ) ;
			ind[ 2 ] = ( WORD )( j - 1 ) ;
			ind += 3 ;

			j -- ;
			k ++ ;
			if( k >= num ) break ;
		}
		NS_DrawPrimitiveIndexed3D( Vertex, vertnum, Index, indexnum, DX_PRIMTYPE_TRIANGLELIST, DX_NONE_GRAPH, TRUE ) ;
	}
	else
	{
		ind = Index ;
		for( i = 0 ; i < CirVertNum ; i ++, ind += 2 )
		{
			ind[ 0 ] = ( WORD )i ;
			ind[ 1 ] = ( WORD )( topind + i ) ;
		}
		for( i = 0 ; i < CirVertNum - 1 ; i ++, ind += 2 )
		{
			ind[ 0 ] = ( WORD )( bottomind + i ) ;
			ind[ 1 ] = ( WORD )( bottomind + i + 1 ) ;
		}
		ind[ 0 ] = ( WORD )( bottomind + i ) ;
		ind[ 1 ] = ( WORD )bottomind ;
		NS_DrawPrimitiveIndexed3D( Vertex, vertnum, Index, indexnum, DX_PRIMTYPE_LINELIST, DX_NONE_GRAPH, TRUE ) ;
	}

	// �������̉��
	DXFREE( Vertex ) ;

	if( GSYS.DrawSetting.Large3DPositionSupport )
	{
		Graphics_Draw_EndLarge3DPositionSupportDraw( &Large3DPosDrawInfo ) ;
	}

	// �I��
	return 0 ;
}


























// �摜�`��֐�

// �a�l�o�t�@�C����ǂ݂���ŉ�ʂɕ`�悷��
extern	int NS_LoadGraphScreen( int x, int y, const TCHAR *GraphName, int TransFlag )
{
#ifdef UNICODE
	return LoadGraphScreen_WCHAR_T(
		x, y, GraphName, TransFlag
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( GraphName, return -1 )

	Result = LoadGraphScreen_WCHAR_T(
		x, y, UseGraphNameBuffer, TransFlag
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( GraphName )

	return Result ;
#endif
}

// �a�l�o�t�@�C����ǂ݂���ŉ�ʂɕ`�悷��
extern	int LoadGraphScreen_WCHAR_T( int x, int y, const wchar_t *GraphName, int TransFlag )
{
	int GrHandle ;
	LOADGRAPH_GPARAM GParam ;
	
//	if( GSYS.NotDrawFlag ) return 0 ;

	// �O���t�B�b�N��ǂ݂���
	Graphics_Image_InitLoadGraphGParam( &GParam ) ;
	GrHandle = Graphics_Image_LoadBmpToGraph_UseGParam( &GParam, FALSE, -1, GraphName, TRUE, FALSE, DX_MOVIESURFACE_NORMAL, FALSE ) ;
	if( GrHandle < 0 )
	{
		return -1 ;
	}

	// �O���t�B�b�N�̕`��
	NS_DrawGraph( x, y, GrHandle, TransFlag ) ;

	// �O���t�B�b�N�̍폜
	NS_DeleteGraph( GrHandle ) ;

	// �I��
	return 0 ;
}

// �O���t�B�b�N�̕`��
#define SETDRAWRECTCODE\
	SETRECT( DrawRect, x, y, x + Image->WidthI, y + Image->HeightI )\
	DRAWRECT_DRAWAREA_CLIP
extern	int NS_DrawGraph( int x, int y, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret ;

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE ) return -1 ;

	// ��`��ݒ肪����Ă���ꍇ�͉��������I��
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) ) return -1 ;
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �`��
	DRAW_DEF( 
		Graphics_Hardware_DrawGraph_PF( x, y, ( float )x, ( float )y, Image, BlendImage, TransFlag, TRUE ),
		Graphics_Software_DrawGraph(    x, y,                         Image,             TransFlag       ),
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

// �O���t�B�b�N�̕`��
extern	int NS_DrawGraphF( float xf, float yf, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret = -1 ;
	int x, y ;

	x = _FTOL( xf ) ;
	y = _FTOL( yf ) ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) ) return -1 ;
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawGraph_PF( x, y, xf, yf, Image, BlendImage, TransFlag, FALSE ),
		Graphics_Software_DrawGraph(    x, y,         Image,             TransFlag        ),
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}
#undef SETDRAWRECTCODE

// �O���t�B�b�N�̊g��k���`��
#define SETDRAWRECTCODE\
	SETRECT( DrawRect, x1, y1, x2, y2 ) ;\
	if( x1 > x2 ){ DrawRect.left = x2 ; DrawRect.right = x1; }\
	if( y1 > y2 ){ DrawRect.top = y2 ; DrawRect.bottom = y1; }\
	DRAWRECT_DRAWAREA_CLIP

extern	int NS_DrawExtendGraph( int x1, int y1, int x2, int y2, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret = -1 ;
	
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) ) return -1 ;
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �e�N�X�`���̏ꍇ�Ƃ���ȊO�ŏ����𕪊�
	DRAW_DEF(
		Graphics_Hardware_DrawExtendGraph_PF( x1, y1, x2, y2, (float)x1, (float)y1, (float)x2, (float)y2, Image, BlendImage, TransFlag, TRUE ),
		Graphics_Software_DrawExtendGraph(    x1, y1, x2, y2,                                             Image,             TransFlag       ),
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

// �O���t�B�b�N�̊g��k���`��
extern	int NS_DrawExtendGraphF( float x1f, float y1f, float x2f, float y2f, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret = -1 ;
	int x1, y1, x2, y2 ;
	
	x1 = _FTOL( x1f ) ;
	y1 = _FTOL( y1f ) ;
	x2 = _FTOL( x2f ) ;
	y2 = _FTOL( y2f ) ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) ) return -1 ;
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawExtendGraph_PF( x1, y1, x2, y2, x1f, y1f, x2f, y2f, Image, BlendImage, TransFlag, FALSE ),
		Graphics_Software_DrawExtendGraph(    x1, y1, x2, y2,                     Image,             TransFlag        ),
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

#undef SETDRAWRECTCODE

// �O���t�B�b�N�̉�]�`��
#define SETDRAWRECTCODE\
	int Width, Height ;\
	int i ;\
	float SinF, CosF ;\
	double xb, Lx, Ly ;\
	double px[4], py[4] ;\
	double Sin, Cos ;\
	double MaxMin[4] ; /* 0:MinX 1:MinY 2:MaxX 3:MaxY */\
\
	Lx = x ; Ly = y ;\
\
	_SINCOS_PLATFORM( (float)Angle, &SinF, &CosF ) ;\
	Sin = (double)SinF ;\
	Cos = (double)CosF ;\
\
	Width = Image->WidthI / 2 ;\
	Height = Image->HeightI / 2 ;\
\
	px[0] = -Width ;				py[0] = -Height ;\
	px[1] = Image->WidthI - Width ;	py[1] = -Height ;\
	px[2] = -Width ;				py[2] = Image->HeightI - Height ;\
	px[3] = Image->WidthI - Width ;	py[3] = Image->HeightI - Height ;\
\
	xb = px[0] ;\
	px[0] = ( xb * Cos - py[0] * Sin ) * ExRate + Lx ;\
	py[0] = ( xb * Sin + py[0] * Cos ) * ExRate + Ly ;\
	MaxMin[0] = px[0] ;\
	MaxMin[1] = py[0] ;\
	MaxMin[2] = px[0] ;\
	MaxMin[3] = py[0] ;\
	for( i = 1 ; i < 4 ; i ++ )\
	{\
		xb = px[i] ;\
		px[i] = ( xb * Cos - py[i] * Sin ) * ExRate + Lx ;\
		py[i] = ( xb * Sin + py[i] * Cos ) * ExRate + Ly ;\
\
		if( MaxMin[0] > px[i] ) MaxMin[0] = px[i];\
		if( MaxMin[2] < px[i] ) MaxMin[2] = px[i];\
		if( MaxMin[1] > py[i] ) MaxMin[1] = py[i];\
		if( MaxMin[3] < py[i] ) MaxMin[3] = py[i];\
	}\
\
	MaxMin[0] -= 5.0F ; MaxMin[1] -= 5.0F ;\
	MaxMin[2] += 5.0F ; MaxMin[3] += 5.0F ; \
\
	_DTOL4( MaxMin, (int *)&DrawRect ) ;\
	DRAWRECT_DRAWAREA_CLIP

extern	int NS_DrawRotaGraph( int x, int y, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret = -1 ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) ) return -1 ;
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawRotaGraph_PF( x, y, (float)x, (float)y, ExRate, Angle, Image, BlendImage, TransFlag, TurnFlag, TRUE ),
		Graphics_Software_DrawRotaGraph(          (float)x, (float)y, ExRate, Angle, Image,             TransFlag, TurnFlag       ),
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

// �O���t�B�b�N�̉�]�`��
extern	int NS_DrawRotaGraphF( float xf, float yf, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret = -1 ;
	int x, y ;
	
	x = _FTOL( xf ) ;
	y = _FTOL( yf ) ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) ) return -1 ;
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawRotaGraph_PF( x, y, xf, yf, ExRate, Angle, Image, BlendImage, TransFlag, TurnFlag, FALSE ),
		Graphics_Software_DrawRotaGraph(          xf, yf, ExRate, Angle, Image,             TransFlag, TurnFlag        ),
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

#undef SETDRAWRECTCODE

// �O���t�B�b�N�̉�]�`��
#define SETDRAWRECTCODE\
	int Width, Height ;\
	int i ;\
	float xb, Lx, Ly ;\
	float px[4], py[4] ;\
	float Sin, Cos ;\
	float MaxMin[4] ; /* 0:MinX 1:MinY 2:MaxX 3:MaxY */\
\
	Lx = ( float )x ; Ly = ( float )y ;\
\
	_TABLE_SINCOS( Angle, &Sin, &Cos ) ;\
\
	Width = Image->WidthI / 2 ;\
	Height = Image->HeightI / 2 ;\
\
	px[0] = ( float )-Width ;						py[0] = ( float )-Height ;\
	px[1] = ( float )( Image->WidthI - Width ) ;	py[1] = ( float )-Height ;\
	px[2] = ( float )-Width ;						py[2] = ( float )( Image->HeightI - Height ) ;\
	px[3] = ( float )( Image->WidthI - Width ) ;	py[3] = ( float )( Image->HeightI - Height ) ;\
\
	xb = px[0] ;\
	px[0] = ( xb * Cos - py[0] * Sin ) * ExRate + Lx ;\
	py[0] = ( xb * Sin + py[0] * Cos ) * ExRate + Ly ;\
	MaxMin[0] = px[0] ;\
	MaxMin[1] = py[0] ;\
	MaxMin[2] = px[0] ;\
	MaxMin[3] = py[0] ;\
	for( i = 1 ; i < 4 ; i ++ )\
	{\
		xb = px[i] ;\
		px[i] = ( xb * Cos - py[i] * Sin ) * ExRate + Lx ;\
		py[i] = ( xb * Sin + py[i] * Cos ) * ExRate + Ly ;\
\
		if( MaxMin[0] > px[i] ) MaxMin[0] = px[i];\
		if( MaxMin[2] < px[i] ) MaxMin[2] = px[i];\
		if( MaxMin[1] > py[i] ) MaxMin[1] = py[i];\
		if( MaxMin[3] < py[i] ) MaxMin[3] = py[i];\
	}\
\
	MaxMin[0] -= 5.0F ; MaxMin[1] -= 5.0F ;\
	MaxMin[2] += 5.0F ; MaxMin[3] += 5.0F ; \
\
	_FTOL4( MaxMin, (int *)&DrawRect ) ;\
	DRAWRECT_DRAWAREA_CLIP

extern	int NS_DrawRotaGraphFast( int x, int y, float ExRate, float Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret = -1 ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) ) return -1 ;
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawRotaGraphFast_PF( x, y, (float)x, (float)y, ExRate, Angle, Image, BlendImage, TransFlag, TurnFlag, TRUE ),
		Graphics_Software_DrawRotaGraphFast(          (float)x, (float)y, ExRate, Angle, Image,             TransFlag, TurnFlag       ),
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

// �O���t�B�b�N�̉�]�`��A������
extern	int NS_DrawRotaGraphFastF( float xf, float yf, float ExRate, float Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret = -1 ;
	int x, y ;
	
	x = _FTOL( xf ) ;
	y = _FTOL( yf ) ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) ) return -1 ;
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawRotaGraphFast_PF( x, y, xf, yf, ExRate, Angle, Image, BlendImage, TransFlag, TurnFlag, FALSE ),
		Graphics_Software_DrawRotaGraphFast(          xf, yf, ExRate, Angle, Image,             TransFlag, TurnFlag        ),
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

#undef SETDRAWRECTCODE

// �O���t�B�b�N�̉�]�`��Q
extern	int NS_DrawRotaGraph2( int x, int y, int cx, int cy, double ExtRate, double Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	int w, h ;

	NS_GetGraphSize( GrHandle, &w,&h ) ;

	if( Angle == 0.0 && ExtRate == 1.0 )
	{
		if( TurnFlag )	return NS_DrawTurnGraph( x - cx, y - cy, GrHandle, TransFlag ) ;
		else			return NS_DrawGraph( x - cx, y - cy, GrHandle, TransFlag ) ;
	}
	else
	{
		return NS_DrawRotaGraph2F( ( float )x, ( float )y, ( float )cx, ( float )cy, ExtRate, Angle, GrHandle, TransFlag, TurnFlag ) ;
	}
}

// �O���t�B�b�N�̉�]�`��R
extern	int NS_DrawRotaGraph3( int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	if( Angle == 0.0 && ExtRateX == 1.0 && ExtRateY == 1.0 )
	{
		int w, h ;

		NS_GetGraphSize( GrHandle, &w,&h ) ;

		if( TurnFlag )	return NS_DrawTurnGraph( x - cx, y - cy, GrHandle, TransFlag ) ;
		else			return NS_DrawGraph( x - cx, y - cy, GrHandle, TransFlag ) ;
	}
	else
	{
		return NS_DrawRotaGraph3F( ( float )x, ( float )y, ( float )cx, ( float )cy, ExtRateX, ExtRateY, Angle, GrHandle, TransFlag, TurnFlag ) ;
	}
}

// �O���t�B�b�N�̉�]�`��Q�A������
extern	int NS_DrawRotaGraphFast2( int x, int y, int cx, int cy, float ExtRate, float Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	int w, h ;

	NS_GetGraphSize( GrHandle, &w,&h ) ;

	if( Angle == 0.0f && ExtRate == 1.0f )
	{
		if( TurnFlag )	return NS_DrawTurnGraph( x - cx, y - cy, GrHandle, TransFlag ) ;
		else			return NS_DrawGraph( x - cx, y - cy, GrHandle, TransFlag ) ;
	}
	else
	{
		return NS_DrawRotaGraphFast2F( ( float )x, ( float )y, ( float )cx, ( float )cy, ExtRate, Angle, GrHandle, TransFlag, TurnFlag ) ;
	}
}

// �O���t�B�b�N�̉�]�`��R�A������
extern	int NS_DrawRotaGraphFast3( int x, int y, int cx, int cy, float ExtRateX, float ExtRateY, float Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	if( Angle == 0.0f && ExtRateX == 1.0f && ExtRateY == 1.0f )
	{
		int w, h ;

		NS_GetGraphSize( GrHandle, &w,&h ) ;

		if( TurnFlag )	return NS_DrawTurnGraph( x - cx, y - cy, GrHandle, TransFlag ) ;
		else			return NS_DrawGraph( x - cx, y - cy, GrHandle, TransFlag ) ;
	}
	else
	{
		return NS_DrawRotaGraphFast3F( ( float )x, ( float )y, ( float )cx, ( float )cy, ExtRateX, ExtRateY, Angle, GrHandle, TransFlag, TurnFlag ) ;
	}
}

// �摜�̎��R�ό`�`��
extern	int NS_DrawModiGraph( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int GrHandle, int TransFlag )
{
	return Graphics_Draw_ModiGraphBase( x1, y1, x2, y2, x3, y3, x4, y4, GrHandle, TransFlag, false ) ;
}

// �摜�̍��E���]�`��
extern	int NS_DrawTurnGraph( int x, int y, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

	return NS_DrawExtendGraph( Image->WidthI + x, y, x, y + Image->HeightI, GrHandle, TransFlag ) ;
}





// �O���t�B�b�N�̉�]�`��Q
extern	int NS_DrawRotaGraph2F( float xf, float yf, float cxf, float cyf, double ExtRate, double Angle, int GrHandle, int TransFlag, int TurnFlag )
{
#if 1
	int w, h ;

	NS_GetGraphSize( GrHandle, &w,&h ) ;

	if( Angle == 0.0 )
	{
		if( ExtRate == 1.0 )
		{
			if( TurnFlag )	return NS_DrawTurnGraphF( xf - cxf, yf - cyf, GrHandle, TransFlag ) ;
			else			return NS_DrawGraphF( xf - cxf, yf - cyf, GrHandle, TransFlag ) ;
		}
		else
		{
			float numF[ 4 ] ;

			numF[ 0 ] = xf - ( cxf * ( float )ExtRate ) ;
			numF[ 1 ] = yf - ( cyf * ( float )ExtRate ) ;
			numF[ 2 ] = numF[ 0 ] + ( w * ( float )ExtRate ) ;
			numF[ 3 ] = numF[ 1 ] + ( h * ( float )ExtRate ) ;
			if( TurnFlag )
			{
				float d ;

				d = numF[ 0 ] ; numF[ 0 ] = numF[ 2 ] ; numF[ 2 ] = d ;
			}
			return NS_DrawExtendGraphF( numF[0], numF[1],
										numF[2], numF[3],
										 GrHandle, TransFlag ) ;
		}
	}
	else
	{
		float dcx, dcy ;
		float Sin, Cos ;
		float bcx, bcy ;

		bcx = ( -cxf + ( float )w * 0.5F ) * ( float )ExtRate ;
		bcy = ( -cyf + ( float )h * 0.5F ) * ( float )ExtRate ;

		_SINCOS_PLATFORM( ( float )Angle, &Sin, &Cos ) ;
		dcx = bcx * Cos - bcy * Sin ;
		dcy = bcx * Sin + bcy * Cos ;

		return NS_DrawRotaGraphF( xf + dcx, yf + dcy, ExtRate, Angle, GrHandle, TransFlag, TurnFlag ) ; 
	}
#else
/*
	double xx[4],yy[4];
	int xxi[4], yyi[4] ;
	int w, h, i, Result ;
	double xb, yb, xd, yd ;
	double Sin, Cos ;
	float SinF, CosF ;

	NS_GetGraphSize( GrHandle, &w,&h ) ;

	xx[0] = (   - cx )	* ExtRate ;	yy[0] = (   - cy )	* ExtRate ;
	xx[1] = ( w - cx )	* ExtRate ;	yy[1] = yy[0] ;
	xx[2] = xx[0] ;					yy[2] = ( h - cy )	* ExtRate ;
	xx[3] = xx[1] ;					yy[3] = yy[2] ;

	_SINCOS_PLATFORM( (float)Angle, &SinF, &CosF ) ;
	Sin = (double)SinF ;
	Cos = (double)CosF ;
//	Sin = sin( Angle ) ;
//	Cos = cos( Angle ) ;
	xd = (double)x ;
	yd = (double)y ;
	for( i = 0 ; i < 4 ; i ++ )
	{
		xb = xx[i] * Cos - yy[i] * Sin ;
		yb = xx[i] * Sin + yy[i] * Cos ;
		xx[i] = xb + xd ;
		yy[i] = yb + yd ;
	}

	if( TurnFlag )
	{
		double d ;

		d = xx[0] ; xx[0] = xx[1] ; xx[1] = d ;
		d = xx[2] ; xx[2] = xx[3] ; xx[3] = d ;
		d = yy[0] ; yy[0] = yy[1] ; yy[1] = d ;
		d = yy[2] ; yy[2] = yy[3] ; yy[3] = d ;
	}

	_DTOL4( xx, xxi ) ;
	_DTOL4( yy, yyi ) ;
	Result = NS_DrawModiGraph( xxi[0],yyi[0], xxi[1],yyi[1], xxi[3],yyi[3], xxi[2],yyi[2], GrHandle, TransFlag ) ;

	return Result ;
*/
#endif
}


// �O���t�B�b�N�̉�]�`��R
extern	int NS_DrawRotaGraph3F( float xf, float yf, float cxf, float cyf, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	int w, h ;

	NS_GetGraphSize( GrHandle, &w,&h ) ;

	if( Angle == 0.0 )
	{
		if( ExtRateX == 1.0 && ExtRateY == 1.0 )
		{
			if( TurnFlag )	return NS_DrawTurnGraphF( xf - cxf, yf - cyf, GrHandle, TransFlag ) ;
			else			return NS_DrawGraphF( xf - cxf, yf - cyf, GrHandle, TransFlag ) ;
		}
		else
		{
			float numF[ 4 ] ;

			numF[ 0 ] = xf - ( cxf * ( float )ExtRateX ) ;
			numF[ 1 ] = yf - ( cyf * ( float )ExtRateY ) ;
			numF[ 2 ] = numF[ 0 ] + ( w * ( float )ExtRateX ) ;
			numF[ 3 ] = numF[ 1 ] + ( h * ( float )ExtRateY ) ;
			if( TurnFlag )
			{
				float d ;

				d = numF[ 0 ] ; numF[ 0 ] = numF[ 2 ] ; numF[ 2 ] = d ;
			}
			return NS_DrawExtendGraphF( numF[0], numF[1],
										numF[2], numF[3],
										 GrHandle, TransFlag ) ;
		}
	}
	else
	{
		if( ExtRateX == 1.0 && ExtRateY == 1.0 )
		{
			float dcx, dcy ;
			float Sin, Cos ;
			float bcx, bcy ;

			bcx = ( -cxf + ( float )w * 0.5F ) ;
			bcy = ( -cyf + ( float )h * 0.5F ) ;

			_SINCOS_PLATFORM( ( float )Angle, &Sin, &Cos ) ;
			dcx = bcx * Cos - bcy * Sin ;
			dcy = bcx * Sin + bcy * Cos ;

			return NS_DrawRotaGraphF( xf + dcx, yf + dcy, 1.0, Angle, GrHandle, TransFlag, TurnFlag ) ; 
		}
		else
		{
			float xx[ 4 ], yy[ 4 ] ;
			float xxt[ 4 ], yyt[ 4 ] ;
			int i ;
			float SinF, CosF ;

			xxt[0] = (   - cxf )	* ( float )ExtRateX ;	yyt[0] = (   - cyf )	* ( float )ExtRateY ;
			xxt[1] = ( w - cxf )	* ( float )ExtRateX ;	yyt[1] = yyt[0] ;
			xxt[2] = xxt[0] ;								yyt[2] = ( h - cyf )	* ( float )ExtRateY ;
			xxt[3] = xxt[1] ;								yyt[3] = yyt[2] ;

			_SINCOS_PLATFORM( ( float )Angle, &SinF, &CosF ) ;
			for( i = 0 ; i < 4 ; i ++ )
			{
				xx[ i ] = xxt[ i ] * CosF - yyt[ i ] * SinF + xf ;
				yy[ i ] = xxt[ i ] * SinF + yyt[ i ] * CosF + yf ;
			}

			if( TurnFlag )
			{
				float d ;

				d = xx[ 0 ] ; xx[ 0 ] = xx[ 1 ] ; xx[ 1 ] = d ;
				d = xx[ 2 ] ; xx[ 2 ] = xx[ 3 ] ; xx[ 3 ] = d ;
				d = yy[ 0 ] ; yy[ 0 ] = yy[ 1 ] ; yy[ 1 ] = d ;
				d = yy[ 2 ] ; yy[ 2 ] = yy[ 3 ] ; yy[ 3 ] = d ;
			}

			return Graphics_Draw_ModiGraphFBase( xx[0], yy[0], xx[1], yy[1], xx[3], yy[3], xx[2], yy[2], GrHandle, TransFlag, TRUE ) ;
		}
	}
}

// �O���t�B�b�N�̉�]�`��Q(������)
extern	int NS_DrawRotaGraphFast2F( float xf, float yf, float cxf, float cyf, float ExtRate, float Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	int w, h ;

	NS_GetGraphSize( GrHandle, &w,&h ) ;

	if( Angle == 0.0f )
	{
		if( ExtRate == 1.0f )
		{
			if( TurnFlag )	return NS_DrawTurnGraphF( xf - cxf, yf - cyf, GrHandle, TransFlag ) ;
			else			return NS_DrawGraphF( xf - cxf, yf - cyf, GrHandle, TransFlag ) ;
		}
		else
		{
			float numF[ 4 ] ;

			numF[ 0 ] = xf - ( cxf * ExtRate ) ;
			numF[ 1 ] = yf - ( cyf * ExtRate ) ;
			numF[ 2 ] = numF[ 0 ] + ( w * ExtRate ) ;
			numF[ 3 ] = numF[ 1 ] + ( h * ExtRate ) ;
			if( TurnFlag )
			{
				float d ;

				d = numF[ 0 ] ; numF[ 0 ] = numF[ 2 ] ; numF[ 2 ] = d ;
			}
			return NS_DrawExtendGraphF( numF[0], numF[1],
										numF[2], numF[3],
										 GrHandle, TransFlag ) ;
		}
	}
	else
	{
		float dcx, dcy ;
		float Sin, Cos ;
		float bcx, bcy ;

		bcx = ( -cxf + ( float )w * 0.5f ) * ExtRate ;
		bcy = ( -cyf + ( float )h * 0.5f ) * ExtRate ;

		_TABLE_SINCOS( Angle, &Sin, &Cos ) ;
		dcx = bcx * Cos - bcy * Sin ;
		dcy = bcx * Sin + bcy * Cos ;

		return NS_DrawRotaGraphFastF( xf + dcx, yf + dcy, ExtRate, Angle, GrHandle, TransFlag, TurnFlag ) ; 
	}
}


// �O���t�B�b�N�̉�]�`��R(������)
extern	int NS_DrawRotaGraphFast3F( float xf, float yf, float cxf, float cyf, float ExtRateX, float ExtRateY, float Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	int w, h ;

	NS_GetGraphSize( GrHandle, &w,&h ) ;

	if( Angle == 0.0f )
	{
		if( ExtRateX == 1.0f && ExtRateY == 1.0f )
		{
			if( TurnFlag )	return NS_DrawTurnGraphF( xf - cxf, yf - cyf, GrHandle, TransFlag ) ;
			else			return NS_DrawGraphF( xf - cxf, yf - cyf, GrHandle, TransFlag ) ;
		}
		else
		{
			float numF[ 4 ] ;

			numF[ 0 ] = xf - ( cxf * ExtRateX ) ;
			numF[ 1 ] = yf - ( cyf * ExtRateY ) ;
			numF[ 2 ] = numF[ 0 ] + ( w * ExtRateX ) ;
			numF[ 3 ] = numF[ 1 ] + ( h * ExtRateY ) ;
			if( TurnFlag )
			{
				float d ;

				d = numF[ 0 ] ; numF[ 0 ] = numF[ 2 ] ; numF[ 2 ] = d ;
			}
			return NS_DrawExtendGraphF( numF[0], numF[1],
										numF[2], numF[3],
										 GrHandle, TransFlag ) ;
		}
	}
	else
	{
		if( ExtRateX == 1.0f && ExtRateY == 1.0f )
		{
			float dcx, dcy ;
			float Sin, Cos ;
			float bcx, bcy ;

			bcx = ( -cxf + ( float )w * 0.5f ) ;
			bcy = ( -cyf + ( float )h * 0.5f ) ;

			_TABLE_SINCOS( Angle, &Sin, &Cos ) ;
			dcx = bcx * Cos - bcy * Sin ;
			dcy = bcx * Sin + bcy * Cos ;

			return NS_DrawRotaGraphFastF( xf + dcx, yf + dcy, 1.0f, Angle, GrHandle, TransFlag, TurnFlag ) ; 
		}
		else
		{
			float xxt[ 4 ], yyt[ 4 ] ;
			float xx[ 4 ], yy[ 4 ] ;
			int i ;
			float SinF, CosF ;

			xxt[0] = (   - cxf )	* ExtRateX ;	yyt[0] = (   - cyf )	* ExtRateY ;
			xxt[1] = ( w - cxf )	* ExtRateX ;	yyt[1] = yyt[0] ;
			xxt[2] = xxt[0] ;						yyt[2] = ( h - cyf )	* ExtRateY ;
			xxt[3] = xxt[1] ;						yyt[3] = yyt[2] ;

			_TABLE_SINCOS( Angle, &SinF, &CosF ) ;
			for( i = 0 ; i < 4 ; i ++ )
			{
				xx[ i ] = xxt[ i ] * CosF - yyt[ i ] * SinF + xf ;
				yy[ i ] = xxt[ i ] * SinF + yyt[ i ] * CosF + yf ;
			}

			if( TurnFlag )
			{
				float d ;

				d = xx[ 0 ] ; xx[ 0 ] = xx[ 1 ] ; xx[ 1 ] = d ;
				d = xx[ 2 ] ; xx[ 2 ] = xx[ 3 ] ; xx[ 3 ] = d ;
				d = yy[ 0 ] ; yy[ 0 ] = yy[ 1 ] ; yy[ 1 ] = d ;
				d = yy[ 2 ] ; yy[ 2 ] = yy[ 3 ] ; yy[ 3 ] = d ;
			}

			return Graphics_Draw_ModiGraphFBase( xx[0], yy[0], xx[1], yy[1], xx[3], yy[3], xx[2], yy[2], GrHandle, TransFlag, TRUE ) ;
		}
	}
}

// �摜�̎��R�ό`�`��( float �� )
extern	int NS_DrawModiGraphF( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int GrHandle, int TransFlag )
{
	return Graphics_Draw_ModiGraphFBase( x1, y1, x2, y2, x3, y3, x4, y4, GrHandle, TransFlag, false ) ;
}


// �摜�̍��E���]�`��
extern	int NS_DrawTurnGraphF( float xf, float yf, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

	return NS_DrawExtendGraphF( Image->WidthF + xf, yf, xf, yf + Image->HeightF, GrHandle, TransFlag ) ;
}

// �`�b�v�O���t�B�b�N���g�����}�b�v�`��
#define SETDRAWRECTCODE\
	SETRECT( DrawRect, Sx, Sy, Sx + XNum * Image->WidthI , Sy + YNum * Image->HeightI )\
	DRAWRECT_DRAWAREA_CLIP
extern	int NS_DrawChipMap( int Sx, int Sy, int XNum, int YNum, const int *MapData, int ChipTypeNum, int MapDataPitch, const int *ChipGrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	int Ret = -1 ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( ChipGrHandle[0], Image ) )
		return -1 ;

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawChipMap( Sx, Sy, XNum, YNum, MapData, MapDataPitch, ChipTypeNum, ChipGrHandle, TransFlag ),
		Graphics_Software_DrawChipMap( Sx, Sy, XNum, YNum, MapData, MapDataPitch, ChipTypeNum, ChipGrHandle, TransFlag ),
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}
#undef SETDRAWRECTCODE

// �`�b�v�O���t�B�b�N���g�����}�b�v�`��
// int MapWidth, MapHeight : �}�b�v�f�[�^�S�̂̕��ƍ���
// int *MapData : �}�b�v�f�[�^���i�[���ꂽ�A�h���X
// int ChipTypeNum : �}�b�v�Ɏg���`�b�v�̎�ނ̐�
// int *ChipGrHandle : �`�b�v�̃O���t�B�b�N�z��̃A�h���X
// int TransFlag : ���ߐF�����̗L��
// int MapDrawX, MapDrawY : �`�悷��}�b�v�̋N�_�ƂȂ鍶����W
// int MapDrawWidth, MapDrawHeight : �N�_���W����`�悷��}�b�v�̕��ƍ���
// int ScreenX, ScreenY : �}�b�v��`�悷���ʏ�̍��W
extern	int NS_DrawChipMap( int MapWidth, int MapHeight, const int *MapData, int ChipTypeNum, const int *ChipGrHandle, int TransFlag, int MapDrawPointX, int MapDrawPointY, int MapDrawWidth, int MapDrawHeight, int ScreenX, int ScreenY )
{
	int Width, Height ;
	int Result ;
	
	// �`�b�v�O���t�B�b�N�̃T�C�Y�𓾂�
	NS_GetGraphSize( ChipGrHandle[0], &Width, &Height ) ;

	// �}�b�v��񂪂͂ݏo��ꍇ�̕␳����
	if( MapDrawPointX + MapDrawWidth > MapWidth ) 
		MapDrawWidth -= ( MapDrawPointX + MapDrawWidth ) - MapWidth ;

	if( MapDrawPointY + MapDrawHeight > MapHeight ) 
		MapDrawHeight -= ( MapDrawPointY + MapDrawHeight ) - MapHeight ; 

	if( MapDrawPointX < 0 )
	{
		ScreenX = -MapDrawPointX * Width ;
		MapDrawWidth += MapDrawPointX ; 
		MapDrawPointX = 0 ;
	}
	if( MapDrawPointY < 0 )
	{
		ScreenY = -MapDrawPointY * Height ;
		MapDrawHeight += MapDrawPointY ; 
		MapDrawPointY = 0 ;
	}

	// �}�b�v��`��
	Result = NS_DrawChipMap( ScreenX, ScreenY, MapDrawWidth, MapDrawHeight,
							 MapData + MapDrawPointX + MapDrawPointY * MapWidth, MapWidth, ChipTypeNum, ChipGrHandle, TransFlag ) ;
						 
	return Result ;
}

// �O���t�B�b�N���w��̈�Ƀ^�C����ɕ`�悷��
#define SETDRAWRECTCODE\
	SETRECT( DrawRect, x1, y1, x2, y2 )\
	DRAWRECT_DRAWAREA_CLIP
extern	int NS_DrawTile( int x1, int y1, int x2, int y2, int Tx, int Ty, double ExtRate, double Angle, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret = -1 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;
	if( x1 == x2 || y1 == y2 ) return 0 ;

	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;
	}

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawTile( x1, y1, x2, y2, Tx, Ty, ExtRate, Angle, Image, BlendImage, TransFlag ),
		Graphics_Software_DrawTile( x1, y1, x2, y2, Tx, Ty, ExtRate, Angle, Image,             TransFlag ),
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}
#undef SETDRAWRECTCODE

// �O���t�B�b�N�̎w���`�����݂̂�`��
extern	int NS_DrawRectGraph( int DestX, int DestY, int SrcX, int SrcY, int Width, int Height, int GraphHandle, int TransFlag, int TurnFlag )
{
	RECT  BackupDrawRect ;
	RECTF BackupDrawRectF ;
	int hr ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �w���`���������`��o���Ȃ��悤�ɂ���
	BackupDrawRect  = GSYS.DrawSetting.DrawArea ;
	BackupDrawRectF = GSYS.DrawSetting.DrawAreaF ;
	SETRECT( GSYS.DrawSetting.DrawArea, DestX, DestY, DestX + Width, DestY + Height ) ;
	RectClipping_Inline( &GSYS.DrawSetting.DrawArea, &BackupDrawRect ) ;
	if( GSYS.Setting.ValidHardware == FALSE )
	{
		SetMemImgDrawArea( &GSYS.DrawSetting.DrawArea ) ;
	}
	GSYS.DrawSetting.DrawAreaF.left   = ( float )GSYS.DrawSetting.DrawArea.left ;
	GSYS.DrawSetting.DrawAreaF.top    = ( float )GSYS.DrawSetting.DrawArea.top ;
	GSYS.DrawSetting.DrawAreaF.right  = ( float )GSYS.DrawSetting.DrawArea.right ;
	GSYS.DrawSetting.DrawAreaF.bottom = ( float )GSYS.DrawSetting.DrawArea.bottom ;

	// �O���t�B�b�N�̕`��
	if( TurnFlag )
	{
		IMAGEDATA *Image ;

		// �G���[����
		if( GRAPHCHK( GraphHandle, Image ) )
			return -1 ;

		hr = NS_DrawTurnGraph( DestX - ( Image->WidthI - SrcX - Width ), DestY - SrcY, GraphHandle, TransFlag ) ;
	}
	else
	{
		hr = NS_DrawGraph( DestX - SrcX, DestY - SrcY, GraphHandle, TransFlag ) ;
	}

	// �`���`�����ɖ߂�
#ifndef DX_NON_MASK
	if( MASKD.MaskValidFlag )
	{
		// �}�X�N���g�p���Ă���ꍇ�̓}�X�N���������ŐF�X�ύX���������Ă���̂Œʏ�� SetDrawArea ���g�p����
		NS_SetDrawArea( BackupDrawRect.left, BackupDrawRect.top, BackupDrawRect.right, BackupDrawRect.bottom ) ;
	}
	else 
#endif
	{
		// �}�X�N���g�p���Ă��Ȃ��ꍇ�͊ȈՓI�Ȑݒ蕜�A����
		GSYS.DrawSetting.DrawArea = BackupDrawRect ;
		if( GSYS.Setting.ValidHardware == FALSE )
		{
			SetMemImgDrawArea( &BackupDrawRect ) ;
		}
		GSYS.DrawSetting.DrawAreaF = BackupDrawRectF ;
	}

	// �I��
	return hr ;
}

// �O���t�B�b�N�̎w���`�����݂̂��g��`��
extern	int NS_DrawRectExtendGraph( int DestX1, int DestY1, int DestX2, int DestY2, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int GraphHandle, int TransFlag )
{
	RECT MotoDrawRect ;
	RECTF MotoDrawRectF ;
	int hr, tmp ;
	int x1, y1, x2, y2 ;	
	bool RevX, RevY ;
	int DestWidth, DestHeight ;
	double exX, exY ;
	IMAGEDATA *Image ;

	// �T�C�Y����
	if( SrcWidth <= 0 || SrcHeight <= 0 )
		return -1 ;
	
	// �G���[����
	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	// ���]����
	{
		if( DestX2 < DestX1 )
		{
			RevX = true ;
			tmp = DestX1 ; DestX1 = DestX2 ; DestX2 = tmp ;
		}
		else
		{
			RevX = false ;
		}
		
		if( DestY2 < DestY1 )
		{
			RevY = true ;
			tmp = DestY1 ; DestY1 = DestY2 ; DestY2 = tmp ;
		}
		else
		{
			RevY = false ;
		}
	}

	// �w���`���������`��o���Ȃ��悤�ɂ���
	MotoDrawRect = GSYS.DrawSetting.DrawArea ;
	MotoDrawRectF = GSYS.DrawSetting.DrawAreaF ;
	SETRECT( GSYS.DrawSetting.DrawArea, DestX1, DestY1, DestX2, DestY2 ) ;
	RectClipping_Inline( &GSYS.DrawSetting.DrawArea, &MotoDrawRect ) ;
	SetMemImgDrawArea( &GSYS.DrawSetting.DrawArea ) ;
	GSYS.DrawSetting.DrawAreaF.left   = (float)GSYS.DrawSetting.DrawArea.left ;
	GSYS.DrawSetting.DrawAreaF.top    = (float)GSYS.DrawSetting.DrawArea.top ;
	GSYS.DrawSetting.DrawAreaF.right  = (float)GSYS.DrawSetting.DrawArea.right ;
	GSYS.DrawSetting.DrawAreaF.bottom = (float)GSYS.DrawSetting.DrawArea.bottom ;

	// �O���t�B�b�N�̕`��
	{
		DestWidth = DestX2 - DestX1 ;
		DestHeight = DestY2 - DestY1 ;
		exX = (double)DestWidth / SrcWidth ;
		exY = (double)DestHeight / SrcHeight ;
		if( RevX == true )
		{
			x1 = DestX2 + _DTOL( SrcX * exX ) ;
			x2 = DestX2 - _DTOL( ( Image->WidthI - SrcX ) * exX ) ;
		}
		else
		{
			x1 = DestX1 - _DTOL( SrcX * exX ) ;
			x2 = DestX1 + _DTOL( ( Image->WidthI - SrcX ) * exX ) ;
		}

		if( RevY == true )
		{
			y1 = DestY2 + _DTOL( SrcY * exY ) ;
			y2 = DestY2 - _DTOL( ( Image->HeightI - SrcY ) * exY ) ;
		}
		else
		{
			y1 = DestY1 - _DTOL( SrcY * exY ) ;
			y2 = DestY1 + _DTOL( ( Image->HeightI - SrcY ) * exY ) ;
		}
		hr = NS_DrawExtendGraph( x1, y1, x2, y2, GraphHandle, TransFlag ) ;
	}

	// �`���`�����ɖ߂�
	GSYS.DrawSetting.DrawArea = MotoDrawRect ;
	SetMemImgDrawArea( &MotoDrawRect ) ;
	GSYS.DrawSetting.DrawAreaF = MotoDrawRectF ;

	// �I��
	return hr ;
}

extern	int NS_DrawRectRotaGraph( int X, int Y, int SrcX, int SrcY, int Width, int Height, double ExtRate, double Angle, int GraphHandle, int TransFlag, int TurnFlag ) 
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ��]�`��
	NS_DrawRotaGraph( X, Y, ExtRate, Angle, TempHandle, TransFlag, TurnFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

extern int NS_DrawRectRotaGraph2( int x, int y, int SrcX, int SrcY, int Width, int Height, int cx, int cy, double ExtRate, double Angle, int GraphHandle, int TransFlag, int TurnFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ��]�`��
	NS_DrawRotaGraph2( x, y, cx, cy, ExtRate, Angle, TempHandle, TransFlag, TurnFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

extern int NS_DrawRectRotaGraph3(  int x,   int y,   int SrcX, int SrcY, int Width, int Height, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, int GraphHandle, int TransFlag, int TurnFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ��]�`��
	NS_DrawRotaGraph3( x, y, cx, cy, ExtRateX, ExtRateY, Angle, TempHandle, TransFlag, TurnFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

extern	int NS_DrawRectRotaGraphFast( int X, int Y, int SrcX, int SrcY, int Width, int Height, float ExtRate, float Angle, int GraphHandle, int TransFlag, int TurnFlag ) 
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ��]�`��
	NS_DrawRotaGraphFast( X, Y, ExtRate, Angle, TempHandle, TransFlag, TurnFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

extern int NS_DrawRectRotaGraphFast2( int x, int y, int SrcX, int SrcY, int Width, int Height, int cx, int cy, float ExtRate, float Angle, int GraphHandle, int TransFlag, int TurnFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ��]�`��
	NS_DrawRotaGraphFast2( x, y, cx, cy, ExtRate, Angle, TempHandle, TransFlag, TurnFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

extern int NS_DrawRectRotaGraphFast3(  int x,   int y,   int SrcX, int SrcY, int Width, int Height, int cx, int cy, float ExtRateX, float ExtRateY, float Angle, int GraphHandle, int TransFlag, int TurnFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ��]�`��
	NS_DrawRotaGraphFast3( x, y, cx, cy, ExtRateX, ExtRateY, Angle, TempHandle, TransFlag, TurnFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

// �摜�̎w���`�����݂̂����R�ό`�`��
extern int NS_DrawRectModiGraph( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int SrcX, int SrcY, int Width, int Height, int GraphHandle, int TransFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ���R�ό`�`��
	NS_DrawModiGraph( x1, y1, x2, y2, x3, y3, x4, y4, TempHandle, TransFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

// �O���t�B�b�N�̎w���`�����݂̂�`��
extern int NS_DrawRectGraphF( float DestX, float DestY, int SrcX, int SrcY, int Width, int Height, int GraphHandle, int TransFlag, int TurnFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// �`��
	if( TurnFlag )
	{
		NS_DrawTurnGraphF( DestX, DestY, TempHandle, TransFlag ) ;
	}
	else
	{
		NS_DrawGraphF( DestX, DestY, TempHandle, TransFlag ) ;
	}

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

// �O���t�B�b�N�̎w���`�����݂̂��g��`��
extern int NS_DrawRectExtendGraphF( float DestX1, float DestY1, float DestX2, float DestY2, int SrcX, int SrcY, int SrcWidth, int SrcHeight, int GraphHandle, int TransFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( SrcWidth <= 0 || SrcHeight <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, SrcWidth, SrcHeight, GraphHandle ) ;

	// �g��`��
	NS_DrawExtendGraphF( DestX1, DestY1, DestX2, DestY2, TempHandle, TransFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

extern int NS_DrawRectRotaGraphF( float X, float Y, int SrcX, int SrcY, int Width, int Height, double ExtRate, double Angle, int GraphHandle, int TransFlag, int TurnFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ��]�`��
	NS_DrawRotaGraphF( X, Y, ExtRate, Angle, TempHandle, TransFlag, TurnFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

extern int NS_DrawRectRotaGraph2F( float x, float y, int SrcX, int SrcY, int Width, int Height, float cxf, float cyf, double ExtRate, double Angle, int GraphHandle, int TransFlag, int TurnFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ��]�`��
	NS_DrawRotaGraph2F( x, y, cxf, cyf, ExtRate, Angle, TempHandle, TransFlag, TurnFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

extern int NS_DrawRectRotaGraph3F( float x, float y, int SrcX, int SrcY, int Width, int Height, float cxf, float cyf, double ExtRateX, double ExtRateY, double Angle, int GraphHandle, int TransFlag, int TurnFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ��]�`��
	NS_DrawRotaGraph3F( x, y, cxf, cyf, ExtRateX, ExtRateY, Angle, TempHandle, TransFlag, TurnFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

extern int NS_DrawRectRotaGraphFastF( float X, float Y, int SrcX, int SrcY, int Width, int Height, float ExtRate, float Angle, int GraphHandle, int TransFlag, int TurnFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ��]�`��
	NS_DrawRotaGraphFastF( X, Y, ExtRate, Angle, TempHandle, TransFlag, TurnFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

extern int NS_DrawRectRotaGraphFast2F( float x, float y, int SrcX, int SrcY, int Width, int Height, float cxf, float cyf, float ExtRate, float Angle, int GraphHandle, int TransFlag, int TurnFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ��]�`��
	NS_DrawRotaGraphFast2F( x, y, cxf, cyf, ExtRate, Angle, TempHandle, TransFlag, TurnFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

extern int NS_DrawRectRotaGraphFast3F( float x, float y, int SrcX, int SrcY, int Width, int Height, float cxf, float cyf, float ExtRateX, float ExtRateY, float Angle, int GraphHandle, int TransFlag, int TurnFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ��]�`��
	NS_DrawRotaGraphFast3F( x, y, cxf, cyf, ExtRateX, ExtRateY, Angle, TempHandle, TransFlag, TurnFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

// �摜�̎w���`�����݂̂����R�ό`�`��( ���W�w�肪 float �� )
extern int NS_DrawRectModiGraphF( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int SrcX, int SrcY, int Width, int Height,         int GraphHandle, int TransFlag )
{
	int TempHandle ;

	// �T�C�Y����
	if( Width <= 0 || Height <= 0 )
		return -1 ;

	// �؂������O���t�B�b�N���쐬
	TempHandle = NS_DerivationGraph( SrcX, SrcY, Width, Height, GraphHandle ) ;

	// ���R�ό`�`��
	NS_DrawModiGraphF( x1, y1, x2, y2, x3, y3, x4, y4, TempHandle, TransFlag ) ;

	// �폜
	NS_DeleteGraph( TempHandle ) ;

	// �I��
	return 0 ;
}

// �u�����h�摜�ƍ������ĉ摜��`�悷��
extern	int NS_DrawBlendGraph( int x, int y, int GrHandle, int TransFlag, int BlendGraph, int BorderParam, int BorderRange )
{
	int B_BlendGraph, B_BorderParam, B_BorderRange ;

	B_BlendGraph = GSYS.DrawSetting.BlendGraph ;
	B_BorderParam = GSYS.DrawSetting.BlendGraphBorderParam ;
	B_BorderRange = GSYS.DrawSetting.BlendGraphBorderRange ;

	NS_SetBlendGraph( BlendGraph, BorderParam, BorderRange ) ;
	NS_DrawGraph( x, y, GrHandle, TransFlag ) ;
	NS_SetBlendGraph( B_BlendGraph, B_BorderParam, B_BorderRange ) ;

	// �I��
	return 0 ;
}

// �u�����h�摜�ƍ������ĉ摜��`�悷��( �u�����h�摜�̋N�_���W���w�肷��� )
extern	int NS_DrawBlendGraphPos( int x, int y, int GrHandle, int TransFlag, int bx, int by, int BlendGraph, int BorderParam, int BorderRange )
{
	int B_BlendGraph, B_BorderParam, B_BorderRange, B_PosX, B_PosY ;

	B_BlendGraph = GSYS.DrawSetting.BlendGraph ;
	B_BorderParam = GSYS.DrawSetting.BlendGraphBorderParam ;
	B_BorderRange = GSYS.DrawSetting.BlendGraphBorderRange ;
	B_PosX = GSYS.DrawSetting.BlendGraphX ;
	B_PosY = GSYS.DrawSetting.BlendGraphY ;

	NS_SetBlendGraph( BlendGraph, BorderParam, BorderRange ) ;
	NS_SetBlendGraphPosition( bx, by ) ;
	NS_DrawGraph( x, y, GrHandle, TransFlag ) ;
	NS_SetBlendGraphPosition( B_PosX, B_PosY ) ;
	NS_SetBlendGraph( B_BlendGraph, B_BorderParam, B_BorderRange ) ;

	// �I��
	return 0 ;
}

// DrawCircleGauge �֐��̕⏕�֐�
void DrawCircleGaugePolygon(
	int GraphHandle,
	float CenterX,
	float CenterY,
	VECTOR *pVector,
	float SizeR,
	float UScale,
	float VScale,
	float UStart,
	float VStart
)
{
	int i ;
	VERTEX vert[ 3 ] ;
	float AdjustPosition ;

	AdjustPosition = GSYS.HardInfo.DrawFloatCoordType == DX_DRAWFLOATCOORDTYPE_DIRECT3D9 ? 0.5f : 0.0f ;
	
	for( i = 0; i < 3; i ++ )
	{
		vert[i].x = pVector[i].x + CenterX + AdjustPosition ;
		vert[i].y = pVector[i].y + CenterY + AdjustPosition ;
		vert[i].u = ( pVector[i].x * SizeR + 0.5f ) * UScale + UStart ;
		vert[i].v = ( pVector[i].y * SizeR + 0.5f ) * VScale + VStart ;
		vert[i].b = 255 ;
		vert[i].g = 255 ;
		vert[i].r = 255 ;
		vert[i].a = 255 ;
	}
	NS_DrawPolygon( vert, 1, GraphHandle, TRUE );
}

// �~�O���t�I�ȕ`����s��
int NS_DrawCircleGauge( int CenterX, int CenterY, double Percent, int GrHandle, double StartPercent )
{
	VECTOR vertex[3] ;
	VECTOR position[4] ;
	VECTOR positionR ;
	VECTOR positionR2 ;
	float radian ;
	float radian2 ;
	float length ;
	float Sin ;
	float Cos ;
	float CenterXF ;
	float CenterYF ;
	float SizeR ;
	int SizeX ;
	int SizeY ;
	int i ;
	float StartRad ;
	float EndRad ;
	IMAGEDATA *Image ;
	float UScale ;
	float VScale ;
	float UStart ;
	float VStart ;
	RECT DrawRect ;
	RECT GaugeDrawRect ;

	Image = Graphics_Image_GetData( GrHandle ) ;
	if( Image == NULL )
	{
		return -1 ;
	}

	if( Image->Orig->FormatDesc.TextureFlag )
	{
		UStart = ( float )Image->Hard.Draw[ 0 ].UsePosXF / Image->Hard.Draw[ 0 ].Tex->TexWidth ;
		VStart = ( float )Image->Hard.Draw[ 0 ].UsePosYF / Image->Hard.Draw[ 0 ].Tex->TexHeight ;
		UScale = ( float )Image->WidthF                  / Image->Hard.Draw[ 0 ].Tex->TexWidth ;
		VScale = ( float )Image->HeightF                 / Image->Hard.Draw[ 0 ].Tex->TexHeight ;
	}
	else
	{
		UScale = 1.0f ;
		VScale = 1.0f ;
		UStart = 0.0f ;
		VStart = 0.0f ;
	}

	if( StartPercent > 200.0 )
	{
		StartPercent = 200.0 ;
	}
	else
	if( StartPercent < -100.0 )
	{
		StartPercent = -100.0 ;
	}

	if( Percent > 200.0 )
	{
		Percent = 200.0 ;
	}
	else
	if( Percent < -100.0 )
	{
		Percent = -100.0 ;
	}

	if( Percent - StartPercent > 100.0 )
	{
		StartPercent = 0.0 ;
		Percent = 100.0 ;
	}

	if( StartPercent == Percent ||
		StartPercent > Percent )
	{
		return 0 ;
	}

	NS_GetGraphSize( GrHandle, &SizeX, &SizeY );

	radian   = ( float )( Percent      * 2 * DX_PI / 100.0 );
	radian2  = ( float )( StartPercent * 2 * DX_PI / 100.0 );
	length   = SizeX * 0.75f;
	CenterXF = ( float )CenterX;
	CenterYF = ( float )CenterY;
	SizeR    = 1.0f / SizeX;

	vertex[0].x = 0.0f;
	vertex[0].y = 0.0f;
	vertex[1] = vertex[0];
	vertex[2] = vertex[0];

	position[0].x = 0.0f;
	position[0].y = -length;

	position[1].x = length;
	position[1].y = 0.0f;

	position[2].x = 0.0f;
	position[2].y = length;

	position[3].x = -length;
	position[3].y = 0.0f;

	_SINCOS_PLATFORM( radian2 - DX_PI_F * 0.5f, &Sin, &Cos ) ;
	positionR2.x = Cos * length;
	positionR2.y = Sin * length;

	_SINCOS_PLATFORM( radian - DX_PI_F * 0.5f, &Sin, &Cos ) ;
	positionR.x = Cos * length;
	positionR.y = Sin * length;

	DrawRect = GSYS.DrawSetting.DrawArea ;
	GaugeDrawRect.left   = CenterX - SizeX / 2 ;
	GaugeDrawRect.top    = CenterY - SizeY / 2 ;
	GaugeDrawRect.right  = CenterX + SizeX / 2 ;
	GaugeDrawRect.bottom = CenterY + SizeY / 2 ;
	RectClipping_Inline( &GaugeDrawRect, &DrawRect ) ;
	NS_SetDrawArea( GaugeDrawRect.left, GaugeDrawRect.top, GaugeDrawRect.right, GaugeDrawRect.bottom ) ; 

	// 90�x�����`��
	for( i = 0 ; i < 12 ; i ++ )
	{
		StartRad = DX_PI_F * 0.5f * ( i - 4 ) ;
		EndRad   = DX_PI_F * 0.5f * ( i - 4 + 1 ) ;

		if( ( radian2 >= StartRad && radian2 <= EndRad ) || ( StartRad >= radian2 && StartRad <= radian ) )
		{
			vertex[1] = radian2 <= StartRad ? position[ i         % 4 ] : positionR2;
			vertex[2] = radian  >= EndRad   ? position[ ( i + 1 ) % 4 ] : positionR;
			DrawCircleGaugePolygon( GrHandle, CenterXF, CenterYF, vertex, SizeR, UScale, VScale, UStart, VStart );
		}
	}

	// �`��\�͈͂����ɖ߂�
	NS_SetDrawArea( DrawRect.left, DrawRect.top, DrawRect.right, DrawRect.bottom ) ;

	// �I��
	return 0;
}

static struct
{
	float DrawZ;
	int BlendMode, BlendParam;
	int AlphaTestMode, AlphaTestParam;
	int ZBufferCmpType2D, WriteZBufferFlag2D;
	int ZBufferCmpType3D, WriteZBufferFlag3D;
} DrawZBuffer_Static;

static void FASTCALL DrawZBuffer_Pre( int WriteZMode )
{
	DrawZBuffer_Static.DrawZ              = GSYS.DrawSetting.DrawZ;
	DrawZBuffer_Static.BlendMode          = GSYS.DrawSetting.BlendMode;
	DrawZBuffer_Static.BlendParam         = GSYS.DrawSetting.BlendParam;
	DrawZBuffer_Static.AlphaTestMode      = GSYS.DrawSetting.AlphaTestMode;
	DrawZBuffer_Static.AlphaTestParam     = GSYS.DrawSetting.AlphaTestParam;
	DrawZBuffer_Static.WriteZBufferFlag2D = GSYS.DrawSetting.WriteZBufferFlag2D;
	DrawZBuffer_Static.WriteZBufferFlag3D = GSYS.DrawSetting.WriteZBufferFlag3D;
	DrawZBuffer_Static.ZBufferCmpType2D   = GSYS.DrawSetting.ZBufferCmpType2D;
	DrawZBuffer_Static.ZBufferCmpType3D   = GSYS.DrawSetting.ZBufferCmpType3D;

	NS_SetDrawZ( WriteZMode == DX_ZWRITE_MASK ? 0.0f : 0.9f );
	NS_SetDrawBlendMode( DX_BLENDMODE_DESTCOLOR, 255 );
	NS_SetDrawAlphaTest( DX_CMP_GREATER, 0 );
	NS_SetWriteZBufferFlag( TRUE );
	NS_SetZBufferCmpType( DX_CMP_ALWAYS );
}

static void FASTCALL DrawZBuffer_Post( void )
{
	NS_SetZBufferCmpType( DrawZBuffer_Static.ZBufferCmpType2D );
	NS_SetWriteZBufferFlag( DrawZBuffer_Static.WriteZBufferFlag2D );
	NS_SetDrawAlphaTest( DrawZBuffer_Static.AlphaTestMode, DrawZBuffer_Static.AlphaTestParam );
	NS_SetDrawBlendMode( DrawZBuffer_Static.BlendMode, DrawZBuffer_Static.BlendParam );
	NS_SetDrawZ( DrawZBuffer_Static.DrawZ );

	NS_SetZBufferCmpType3D( DrawZBuffer_Static.ZBufferCmpType3D );
	NS_SetWriteZBuffer3D( DrawZBuffer_Static.WriteZBufferFlag3D );
}

// �y�o�b�t�@�ɑ΂��ĉ摜�̕`����s��
extern	int NS_DrawGraphToZBuffer( int X, int Y, int GrHandle, int WriteZMode )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawGraph( X, Y, GrHandle, TRUE );
	DrawZBuffer_Post();

	// �I��
	return 0 ;
}

// �y�o�b�t�@�ɑ΂��ĉ摜�̍��E���]�`��
extern	int NS_DrawTurnGraphToZBuffer( int x, int y, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawTurnGraph( x, y, GrHandle, TRUE );
	DrawZBuffer_Post();

	// �I��
	return 0 ;
}

// �y�o�b�t�@�ɑ΂��ĉ摜�̊g��k���`��
extern	int NS_DrawExtendGraphToZBuffer( int x1, int y1, int x2, int y2, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawExtendGraph( x1, y1, x2, y2, GrHandle, TRUE );
	DrawZBuffer_Post();

	// �I��
	return 0 ;
}

// �y�o�b�t�@�ɑ΂��ĉ摜�̉�]�`��
extern	int NS_DrawRotaGraphToZBuffer( int x, int y, double ExRate, double Angle, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK �� */, int TurnFlag )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawRotaGraph(  x,  y,  ExRate,  Angle,  GrHandle,  TRUE, TurnFlag );
	DrawZBuffer_Post();

	// �I��
	return 0 ;
}

// �y�o�b�t�@�ɑ΂��ĉ摜�̉�]�`��Q
extern	int NS_DrawRotaGraph2ToZBuffer(  int x, int y, int cx, int cy, double ExtRate,                   double Angle, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK �� */ , int TurnFlag )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawRotaGraph2( x,  y,  cx,  cy,  ExtRate,  Angle,  GrHandle,  TRUE, TurnFlag ) ;
	DrawZBuffer_Post();

	// �I��
	return 0 ;
}

// �y�o�b�t�@�ɑ΂��ĉ摜�̉�]�`��R
extern	int NS_DrawRotaGraph3ToZBuffer(  int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK �� */ , int TurnFlag )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawRotaGraph3(   x,  y,  cx,  cy,  ExtRateX,  ExtRateY,  Angle,  GrHandle, TRUE, TurnFlag );
	DrawZBuffer_Post();

	// �I��
	return 0 ;
}

// �y�o�b�t�@�ɑ΂��ĉ摜�̉�]�`��A������
extern	int NS_DrawRotaGraphFastToZBuffer( int x, int y, float ExRate, float Angle, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK �� */, int TurnFlag )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawRotaGraphFast(  x,  y,  ExRate,  Angle,  GrHandle,  TRUE, TurnFlag );
	DrawZBuffer_Post();

	// �I��
	return 0 ;
}

// �y�o�b�t�@�ɑ΂��ĉ摜�̉�]�`��Q�A������
extern	int NS_DrawRotaGraphFast2ToZBuffer(  int x, int y, int cx, int cy, float ExtRate,                   float Angle, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK �� */ , int TurnFlag )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawRotaGraphFast2( x,  y,  cx,  cy,  ExtRate,  Angle,  GrHandle,  TRUE, TurnFlag ) ;
	DrawZBuffer_Post();

	// �I��
	return 0 ;
}

// �y�o�b�t�@�ɑ΂��ĉ摜�̉�]�`��R�A������
extern	int NS_DrawRotaGraphFast3ToZBuffer(  int x, int y, int cx, int cy, float ExtRateX, float ExtRateY, float Angle, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK �� */ , int TurnFlag )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawRotaGraphFast3(   x,  y,  cx,  cy,  ExtRateX,  ExtRateY,  Angle,  GrHandle, TRUE, TurnFlag );
	DrawZBuffer_Post();

	// �I��
	return 0 ;
}

// �y�o�b�t�@�ɑ΂��ĉ摜�̎��R�ό`�`��
extern	int NS_DrawModiGraphToZBuffer( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int GrHandle, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawModiGraph(  x1,  y1,  x2,  y2,  x3,  y3,  x4,  y4,  GrHandle,  TRUE );
	DrawZBuffer_Post();

	// �I��
	return 0 ;
}

// �y�o�b�t�@�ɑ΂��ċ�`�̕`����s��
extern int NS_DrawBoxToZBuffer( int x1, int y1, int x2, int y2, int FillFlag, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawBox( x1, y1, x2, y2, NS_GetColor(255,255,255), FillFlag );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ĉ~�̕`����s��
extern int NS_DrawCircleToZBuffer( int x, int y, int r, int FillFlag, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawCircle( x, y, r, GetColor(255,255,255), FillFlag );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ĎO�p�`��`�悷��
extern int NS_DrawTriangleToZBuffer( int x1, int y1, int x2, int y2, int x3, int y3, int FillFlag, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawTriangle( x1, y1, x2, y2, x3, y3, GetColor(255,255,255), FillFlag ) ;
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��Ďl�p�`��`�悷��
extern int NS_DrawQuadrangleToZBuffer(  int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int FillFlag, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawQuadrangle( x1, y1, x2, y2, x3, y3, x4, y4, GetColor(255,255,255), FillFlag ) ;
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��Ċp�̊ۂ��l�p�`��`�悷��
extern int NS_DrawRoundRectToZBuffer(   int x1, int y1, int x2, int y2, int rx, int ry, int FillFlag, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawRoundRect( x1, y1, x2, y2, rx, ry, GetColor(255,255,255), FillFlag ) ;
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �Q�c�|���S����`�悷��
extern	int NS_DrawPolygon( const VERTEX *Vertex, int PolygonNum, int GrHandle, int TransFlag, int UVScaling )
{
	return NS_DrawPolygonBase( Vertex, PolygonNum * 3, DX_PRIMTYPE_TRIANGLELIST, GrHandle, TransFlag,  UVScaling ) ;
}

// �Q�c�|���S����`�悷��
extern int NS_DrawPolygon2D( const VERTEX2D *Vertex, int PolygonNum, int GrHandle, int TransFlag )
{
	return NS_DrawPrimitive2D( Vertex, PolygonNum * 3, DX_PRIMTYPE_TRIANGLELIST, GrHandle, TransFlag ) ;
}

// �R�c�|���S����`�悷��
extern	int NS_DrawPolygon3D( const VERTEX_3D *Vertex, int PolygonNum, int GrHandle, int TransFlag )
{
	return NS_DrawPolygon3DBase( Vertex, PolygonNum * 3, DX_PRIMTYPE_TRIANGLELIST, GrHandle, TransFlag ) ;
}

// �Q�c�|���S����`�悷��( ���_�C���f�b�N�X���g�p )
extern int NS_DrawPolygonIndexed2D( const VERTEX2D  *Vertex, int VertexNum, const unsigned short *Indices, int PolygonNum, int GrHandle, int TransFlag )
{
	return NS_DrawPrimitiveIndexed2D( Vertex, VertexNum, Indices, PolygonNum * 3, DX_PRIMTYPE_TRIANGLELIST, GrHandle, TransFlag ) ;
}

// �R�c�|���S����`�悷��(�C���f�b�N�X)
extern int NS_DrawPolygonIndexed3D( const VERTEX3D *Vertex, int VertexNum, const unsigned short *Indices, int PolygonNum, int GrHandle, int TransFlag )
{
	return NS_DrawPrimitiveIndexed3D( Vertex, VertexNum, Indices, PolygonNum * 3, DX_PRIMTYPE_TRIANGLELIST, GrHandle, TransFlag ) ;
}

// �R�c�|���S����`�悷��(�C���f�b�N�X)
extern	int NS_DrawPolygonIndexed3DBase( const VERTEX_3D *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	int Ret, i, Flag ;
	const VERTEX_3D *UseVertex ;

	Ret = -1;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	Image = NULL ;
	if( GrHandle != DX_NONE_GRAPH )
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
		Flag = Image->Orig->FormatDesc.TextureFlag ;
	}
	else
	{
		Flag = GSYS.Setting.ValidHardware ;
	}

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// �F�̕ϊ����K�v�ȏꍇ�͂����ŏ�������
	if( GSYS.DrawSetting.DrawBright.Red   != 255 ||
		GSYS.DrawSetting.DrawBright.Green != 255 ||
		GSYS.DrawSetting.DrawBright.Blue  != 255 ||
		( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND && GSYS.DrawSetting.BlendParam != 255 ) )
	{
		DWORD *RedTable, *GreenTable, *BlueTable, *AlphaTable ;
		const VERTEX_3D *sv ;
		VERTEX_3D *dv ;
		unsigned int Size ;

		// �e�[�u����p�ӂ���
		RedTable   = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Red] ;
		GreenTable = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Green] ;
		BlueTable  = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Blue] ;
		AlphaTable = MemImgManage.RateTable[GSYS.DrawSetting.BlendParam] ;
		if( GSYS.DrawSetting.BlendMode == DX_BLENDMODE_NOBLEND ) AlphaTable = MemImgManage.RateTable[255] ;

		// ���_�f�[�^���i�[���郁�����̈悪����Ȃ��ꍇ�͊m�ۂ���
		Size = sizeof( VERTEX_3D ) * VertexNum ;
		if( GSYS.Resource.TempVertexBufferSize < ( int )Size )
		{
			if( GSYS.Resource.TempVertexBuffer )
			{
				DXFREE( GSYS.Resource.TempVertexBuffer ) ;
				GSYS.Resource.TempVertexBufferSize = 0 ;
			}
			GSYS.Resource.TempVertexBuffer = DXALLOC( Size ) ;
			if( GSYS.Resource.TempVertexBuffer == NULL )
				return -1 ;
			GSYS.Resource.TempVertexBufferSize = ( int )Size ;
		}

		// �ꎞ�o�b�t�@�ɕϊ����Ȃ���]��
		sv = Vertex;
		dv = ( VERTEX_3D * )GSYS.Resource.TempVertexBuffer ;
		for( i = 0 ; i < VertexNum ; i ++, sv ++, dv ++ )
		{
			dv->pos = sv->pos ;
			dv->u = sv->u ;
			dv->v = sv->v ;
			dv->b = ( BYTE )BlueTable [ sv->b ] ;
			dv->g = ( BYTE )GreenTable[ sv->g ] ;
			dv->r = ( BYTE )RedTable  [ sv->r ] ;
			dv->a = ( BYTE )AlphaTable[ sv->a ] ;
		}
		UseVertex = ( VERTEX_3D * )GSYS.Resource.TempVertexBuffer ;
	}
	else
	{
		UseVertex = Vertex ;
	}
	
	DRAW_DEF(
		Graphics_Hardware_DrawIndexedPrimitive_PF( UseVertex, VertexNum, Indices, IndexNum, PrimitiveType, Image, TransFlag ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	return Ret ;
}

// �R�c�|���S����`�悷��
extern	int NS_DrawPolygon3DBase( const VERTEX_3D *Vertex, int VertexNum, int PrimitiveType, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	int Ret, i, Flag ;
	const VERTEX_3D *UseVertex ;

	Ret = -1;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	Image = NULL ;
	if( GrHandle != DX_NONE_GRAPH )
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
		Flag = Image->Orig->FormatDesc.TextureFlag ;
	}
	else
	{
		Flag = GSYS.Setting.ValidHardware ;
	}

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// �F�̕ϊ����K�v�ȏꍇ�͂����ŏ�������
	if( GSYS.DrawSetting.DrawBright.Red   != 255 ||
		GSYS.DrawSetting.DrawBright.Green != 255 ||
		GSYS.DrawSetting.DrawBright.Blue  != 255 ||
		( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND && GSYS.DrawSetting.BlendParam != 255 ) )
	{
		DWORD *RedTable, *GreenTable, *BlueTable, *AlphaTable ;
		const VERTEX_3D *sv ;
		VERTEX_3D *dv ;
		unsigned int Size ;

		// �e�[�u����p�ӂ���
		RedTable   = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Red] ;
		GreenTable = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Green] ;
		BlueTable  = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Blue] ;
		AlphaTable = MemImgManage.RateTable[GSYS.DrawSetting.BlendParam] ;
		if( GSYS.DrawSetting.BlendMode == DX_BLENDMODE_NOBLEND ) AlphaTable = MemImgManage.RateTable[255] ;

		// ���_�f�[�^���i�[���郁�����̈悪����Ȃ��ꍇ�͊m�ۂ���
		Size = sizeof( VERTEX_3D ) * VertexNum ;
		if( GSYS.Resource.TempVertexBufferSize < ( int )Size )
		{
			if( GSYS.Resource.TempVertexBuffer )
			{
				DXFREE( GSYS.Resource.TempVertexBuffer ) ;
				GSYS.Resource.TempVertexBufferSize = 0 ;
			}
			GSYS.Resource.TempVertexBuffer = DXALLOC( Size ) ;
			if( GSYS.Resource.TempVertexBuffer == NULL )
				return -1 ;
			GSYS.Resource.TempVertexBufferSize = ( int )Size ;
		}

		// �ꎞ�o�b�t�@�ɕϊ����Ȃ���]��
		sv = Vertex;
		dv = ( VERTEX_3D * )GSYS.Resource.TempVertexBuffer ;
		for( i = 0 ; i < VertexNum ; i ++, sv ++, dv ++ )
		{
			dv->pos = sv->pos ;
			dv->u = sv->u ;
			dv->v = sv->v ;
			dv->b = ( BYTE )BlueTable [ sv->b ] ;
			dv->g = ( BYTE )GreenTable[ sv->g ] ;
			dv->r = ( BYTE )RedTable  [ sv->r ] ;
			dv->a = ( BYTE )AlphaTable[ sv->a ] ;
		}
		UseVertex = ( VERTEX_3D * )GSYS.Resource.TempVertexBuffer ;
	}
	else
	{
		UseVertex = Vertex ;
	}
	
	DRAW_DEF(
		Graphics_Hardware_DrawPrimitive_PF( UseVertex, VertexNum, PrimitiveType, Image, TransFlag ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	return Ret ;
}

// �R�c�|���S����`�悷��
extern int NS_DrawPolygon3D( const VERTEX3D *Vertex, int PolygonNum, int GrHandle, int TransFlag )
{
	return NS_DrawPrimitive3D( Vertex, PolygonNum * 3, DX_PRIMTYPE_TRIANGLELIST, GrHandle, TransFlag ) ;
}

// �Q�c�|���S����`�悷��
#define SETDRAWRECTCODE\
	float MinX, MinY, MaxX, MaxY ;\
	const VERTEX *Vert ;\
\
	MinX =  100000000.0F ;\
	MinY =  100000000.0F ;\
	MaxX = -100000000.0F ;\
	MaxY = -100000000.0F ;\
	Vert = Vertex ;\
	for( i = 0 ; i < VertexNum ; i ++, Vert ++ )\
	{\
		if( Vert->x - 1.0F < MinX ) MinX = Vert->x - 1.0F ;\
		if( Vert->x + 1.0F > MaxX ) MaxX = Vert->x + 1.0F ;\
\
		if( Vert->y - 1.0F < MinY ) MinY = Vert->y - 1.0F ;\
		if( Vert->y + 1.0F > MaxY ) MaxY = Vert->y + 1.0F ;\
	}\
\
	DrawRect.left   = _FTOL( MinX ) ;\
	DrawRect.top    = _FTOL( MinY ) ;\
	DrawRect.right  = _FTOL( MaxX ) ;\
	DrawRect.bottom = _FTOL( MaxY ) ;\
	DRAWRECT_DRAWAREA_CLIP

extern	int NS_DrawPolygonBase( const VERTEX *Vertex, int VertexNum, int PrimitiveType, int GrHandle, int TransFlag, int UVScaling )
{
	IMAGEDATA *Image ;
	VERTEX_2D *TransVert = NULL ;
	VERTEX_2D *DestP ;
	int Ret, i, Flag ;
	float UPos = 0.0f, VPos = 0.0f, UScale = 1.0f, VScale = 1.0f ;

	Ret = -1;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	Image = NULL ;
	if( GrHandle != DX_NONE_GRAPH )
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;

		Flag = Image->Orig->FormatDesc.TextureFlag ;

		// �摜�̃T�C�Y���ŏ�����Q�̂��悾������t�u�␳�͂��Ȃ�
		if( UVScaling )
		{
			if( Image->Orig->FormatDesc.TextureFlag == FALSE ||
				( Image->Hard.DrawNum == 1 && 
				  Image->Hard.Draw->Tex->TexWidth  == Image->Hard.Draw->WidthI  &&
				  Image->Hard.Draw->Tex->TexHeight == Image->Hard.Draw->HeightI ) )
			{
				UVScaling = FALSE ;
			}
		}
	}
	else
	{
		Flag = GSYS.Setting.ValidHardware ;
		UVScaling = FALSE ;
	}

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// UV�X�P�[�����O���s���ꍇ�͂����Ńf�[�^���Z�o����
	if( UVScaling )
	{
		UPos   = Image->Hard.Draw->UsePosXF / Image->Hard.Draw->Tex->TexWidth ;
		VPos   = Image->Hard.Draw->UsePosYF / Image->Hard.Draw->Tex->TexHeight ;
		UScale = Image->Hard.Draw->WidthF   / Image->Hard.Draw->Tex->TexWidth ;
		VScale = Image->Hard.Draw->HeightF  / Image->Hard.Draw->Tex->TexHeight ;
	}

	// ���_�̏o�͐���m�ۂ���
	TransVert = ( VERTEX_2D * )DXALLOC( sizeof( VERTEX_2D ) * VertexNum ) ;
	if( TransVert == NULL ) return -1 ;

	// ���_�ϊ����s��
	if( GSYS.DrawSetting.DrawBright.Red != 255 || GSYS.DrawSetting.DrawBright.Green != 255 || GSYS.DrawSetting.DrawBright.Blue != 255 ||
		( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND && GSYS.DrawSetting.BlendParam != 255 ) )
	{
		DWORD *RedTable, *GreenTable, *BlueTable, *AlphaTable ;

		// �e�[�u����p�ӂ���
		RedTable   = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Red] ;
		GreenTable = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Green] ;
		BlueTable  = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Blue] ;
		AlphaTable = MemImgManage.RateTable[GSYS.DrawSetting.BlendParam] ;
		if( GSYS.DrawSetting.BlendMode == DX_BLENDMODE_NOBLEND ) AlphaTable = MemImgManage.RateTable[255] ;

		// ���_�ϊ�����
		if( UVScaling )
		{
			DestP = TransVert ;
			for( i = 0 ; i < VertexNum ; i ++, DestP ++, Vertex ++ )
			{
				DestP->pos.x = Vertex->x ;
				DestP->pos.y = Vertex->y ;
				DestP->pos.z = GSYS.DrawSetting.DrawZ ;
				DestP->rhw = 1.0F ;
				((BYTE *)&DestP->color)[0] = (BYTE)BlueTable [Vertex->b] ;
				((BYTE *)&DestP->color)[1] = (BYTE)GreenTable[Vertex->g] ;
				((BYTE *)&DestP->color)[2] = (BYTE)RedTable  [Vertex->r] ;
				((BYTE *)&DestP->color)[3] = (BYTE)AlphaTable[Vertex->a] ;
				DestP->u = Vertex->u * UScale + UPos ;
				DestP->v = Vertex->v * VScale + VPos ;
			}
		}
		else
		{
			DestP = TransVert ;
			for( i = 0 ; i < VertexNum ; i ++, DestP ++, Vertex ++ )
			{
				DestP->pos.x = Vertex->x ;
				DestP->pos.y = Vertex->y ;
				DestP->pos.z = GSYS.DrawSetting.DrawZ ;
				DestP->rhw = 1.0F ;
				((BYTE *)&DestP->color)[0] = (BYTE)BlueTable [Vertex->b] ;
				((BYTE *)&DestP->color)[1] = (BYTE)GreenTable[Vertex->g] ;
				((BYTE *)&DestP->color)[2] = (BYTE)RedTable  [Vertex->r] ;
				((BYTE *)&DestP->color)[3] = (BYTE)AlphaTable[Vertex->a] ;
				DestP->u = Vertex->u ;
				DestP->v = Vertex->v ;
			}
		}
	}
	else
	{
		// ���_�ϊ�����
		if( UVScaling )
		{
			DestP = TransVert ;
			for( i = 0 ; i < VertexNum ; i ++, DestP ++, Vertex ++ )
			{
				DestP->pos.x = Vertex->x ;
				DestP->pos.y = Vertex->y ;
				DestP->pos.z = GSYS.DrawSetting.DrawZ ;
				DestP->rhw = 1.0F ;
				DestP->color = *( ( DWORD * )&Vertex->b ) ;
				DestP->u = Vertex->u * UScale + UPos ;
				DestP->v = Vertex->v * VScale + VPos ;
			}
		}
		else
		{
			DestP = TransVert ;
			for( i = 0 ; i < VertexNum ; i ++, DestP ++, Vertex ++ )
			{
				DestP->pos.x = Vertex->x ;
				DestP->pos.y = Vertex->y ;
				DestP->pos.z = GSYS.DrawSetting.DrawZ ;
				DestP->rhw = 1.0F ;
				DestP->color = *( ( DWORD * )&Vertex->b ) ;
				DestP->u = Vertex->u ;
				DestP->v = Vertex->v ;
			}
		}
	}

	DRAW_DEF(
		Graphics_Hardware_DrawPrimitive2D_PF( TransVert, VertexNum, PrimitiveType, Image, TransFlag, FALSE, FALSE, FALSE, 0, FALSE ),
		0,
		SETDRAWRECTCODE,
		Ret,
		Flag
	)

	if( TransVert != NULL )
	{
		DXFREE( TransVert ) ;
		TransVert = NULL ;
	}

	// �I��
	return Ret ;
}

#undef SETDRAWRECTCODE

// �Q�c�v���~�e�B�u��`�悷��
extern int NS_DrawPrimitive2D( const VERTEX2D *Vertex, int VertexNum, int PrimitiveType, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	int Ret, i, Flag ;
	const VERTEX2D *UseVertex ;

	Ret = -1;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	Image = NULL ;
	if( GrHandle != DX_NONE_GRAPH )
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
		Flag = Image->Orig->FormatDesc.TextureFlag ;
	}
	else
	{
		Flag = GSYS.Setting.ValidHardware ;
	}

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �F�̕ϊ����K�v�ȏꍇ�͂����ŏ�������
	if( GSYS.DrawSetting.DrawBright.Red   != 255 ||
		GSYS.DrawSetting.DrawBright.Green != 255 ||
		GSYS.DrawSetting.DrawBright.Blue  != 255 ||
		( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND && GSYS.DrawSetting.BlendParam != 255 ) )
	{
		DWORD *RedTable, *GreenTable, *BlueTable, *AlphaTable ;
		const VERTEX2D *sv ;
		VERTEX2D *dv ;
		unsigned int Size ;

		// �e�[�u����p�ӂ���
		RedTable   = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Red] ;
		GreenTable = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Green] ;
		BlueTable  = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Blue] ;
		AlphaTable = MemImgManage.RateTable[GSYS.DrawSetting.BlendParam] ;
		if( GSYS.DrawSetting.BlendMode == DX_BLENDMODE_NOBLEND ) AlphaTable = MemImgManage.RateTable[255] ;

		// ���_�f�[�^���i�[���郁�����̈悪����Ȃ��ꍇ�͊m�ۂ���
		Size = sizeof( VERTEX2D ) * VertexNum ;
		if( GSYS.Resource.TempVertexBufferSize < ( int )Size )
		{
			if( GSYS.Resource.TempVertexBuffer )
			{
				DXFREE( GSYS.Resource.TempVertexBuffer ) ;
				GSYS.Resource.TempVertexBufferSize = 0 ;
			}
			GSYS.Resource.TempVertexBuffer = DXALLOC( Size ) ;
			if( GSYS.Resource.TempVertexBuffer == NULL )
				return -1 ;
			GSYS.Resource.TempVertexBufferSize = ( int )Size ;
		}

		// �ꎞ�o�b�t�@�ɕϊ����Ȃ���]��
		sv = Vertex;
		dv = ( VERTEX2D * )GSYS.Resource.TempVertexBuffer ;
		for( i = 0 ; i < VertexNum ; i ++, sv ++, dv ++ )
		{
			dv->pos = sv->pos ;
			dv->rhw = sv->rhw ;
			dv->dif.b = ( BYTE )BlueTable [ sv->dif.b ] ;
			dv->dif.g = ( BYTE )GreenTable[ sv->dif.g ] ;
			dv->dif.r = ( BYTE )RedTable  [ sv->dif.r ] ;
			dv->dif.a = ( BYTE )AlphaTable[ sv->dif.a ] ;
			dv->u = sv->u ;
			dv->v = sv->v ;
		}
		UseVertex = ( VERTEX2D * )GSYS.Resource.TempVertexBuffer ;
	}
	else
	{
		UseVertex = Vertex ;
	}
	
	DRAW_DEF(
		Graphics_Hardware_DrawPrimitive2DUser_PF( UseVertex, VertexNum, PrimitiveType, Image, TransFlag, FALSE, FALSE, 0 ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	return Ret ;
}

// �R�c�|���S����`�悷��
extern int NS_DrawPrimitive3D( const VERTEX3D *Vertex, int VertexNum, int PrimitiveType, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	int Ret, i, Flag ;
	const VERTEX3D *UseVertex ;

	Ret = -1;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	Image = NULL ;
	if( GrHandle != DX_NONE_GRAPH )
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
		Flag = Image->Orig->FormatDesc.TextureFlag ;
	}
	else
	{
		Flag = GSYS.Setting.ValidHardware ;
	}

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// �F�̕ϊ����K�v�ȏꍇ�͂����ŏ�������
	if( GSYS.DrawSetting.DrawBright.Red   != 255 ||
		GSYS.DrawSetting.DrawBright.Green != 255 ||
		GSYS.DrawSetting.DrawBright.Blue  != 255 ||
		( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND && GSYS.DrawSetting.BlendParam != 255 ) )
	{
		DWORD *RedTable, *GreenTable, *BlueTable, *AlphaTable ;
		const VERTEX3D *sv ;
		VERTEX3D *dv ;
		unsigned int Size ;

		// �e�[�u����p�ӂ���
		RedTable   = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Red] ;
		GreenTable = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Green] ;
		BlueTable  = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Blue] ;
		AlphaTable = MemImgManage.RateTable[GSYS.DrawSetting.BlendParam] ;
		if( GSYS.DrawSetting.BlendMode == DX_BLENDMODE_NOBLEND ) AlphaTable = MemImgManage.RateTable[255] ;

		// ���_�f�[�^���i�[���郁�����̈悪����Ȃ��ꍇ�͊m�ۂ���
		Size = sizeof( VERTEX3D ) * VertexNum ;
		if( GSYS.Resource.TempVertexBufferSize < ( int )Size )
		{
			if( GSYS.Resource.TempVertexBuffer )
			{
				DXFREE( GSYS.Resource.TempVertexBuffer ) ;
				GSYS.Resource.TempVertexBufferSize = 0 ;
			}
			GSYS.Resource.TempVertexBuffer = DXALLOC( Size ) ;
			if( GSYS.Resource.TempVertexBuffer == NULL )
				return -1 ;
			GSYS.Resource.TempVertexBufferSize = ( int )Size ;
		}

		// �ꎞ�o�b�t�@�ɕϊ����Ȃ���]��
		sv = Vertex;
		dv = ( VERTEX3D * )GSYS.Resource.TempVertexBuffer ;
		for( i = 0 ; i < VertexNum ; i ++, sv ++, dv ++ )
		{
			dv->pos = sv->pos ;
			dv->norm = sv->norm ;
			dv->dif.b = ( BYTE )BlueTable [ sv->dif.b ] ;
			dv->dif.g = ( BYTE )GreenTable[ sv->dif.g ] ;
			dv->dif.r = ( BYTE )RedTable  [ sv->dif.r ] ;
			dv->dif.a = ( BYTE )AlphaTable[ sv->dif.a ] ;
			dv->spc.b = ( BYTE )BlueTable [ sv->spc.b ] ;
			dv->spc.g = ( BYTE )GreenTable[ sv->spc.g ] ;
			dv->spc.r = ( BYTE )RedTable  [ sv->spc.r ] ;
			dv->spc.a = ( BYTE )AlphaTable[ sv->spc.a ] ;
			dv->u = sv->u ;
			dv->v = sv->v ;
			dv->su = sv->su ;
			dv->sv = sv->sv ;
		}
		UseVertex = ( VERTEX3D * )GSYS.Resource.TempVertexBuffer ;
	}
	else
	{
		UseVertex = Vertex ;
	}
	
	DRAW_DEF(
		Graphics_Hardware_DrawPrimitiveLight_PF( UseVertex, VertexNum, PrimitiveType, Image, TransFlag ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	return Ret ;
}

// �Q�c�v���~�e�B�u��`�悷��(�C���f�b�N�X)
extern int NS_DrawPrimitiveIndexed2D( const VERTEX2D *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	int Ret, i, Flag ;
	const VERTEX2D *UseVertex ;

	Ret = -1;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	Image = NULL ;
	if( GrHandle != DX_NONE_GRAPH )
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
		Flag = Image->Orig->FormatDesc.TextureFlag ;
	}
	else
	{
		Flag = GSYS.Setting.ValidHardware ;
	}

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// �F�̕ϊ����K�v�ȏꍇ�͂����ŏ�������
	if( GSYS.DrawSetting.DrawBright.Red   != 255 ||
		GSYS.DrawSetting.DrawBright.Green != 255 ||
		GSYS.DrawSetting.DrawBright.Blue  != 255 ||
		( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND && GSYS.DrawSetting.BlendParam != 255 ) )
	{
		DWORD *RedTable, *GreenTable, *BlueTable, *AlphaTable ;
		const VERTEX2D *sv ;
		VERTEX2D *dv ;
		unsigned int Size ;

		// �e�[�u����p�ӂ���
		RedTable   = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Red] ;
		GreenTable = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Green] ;
		BlueTable  = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Blue] ;
		AlphaTable = MemImgManage.RateTable[GSYS.DrawSetting.BlendParam] ;
		if( GSYS.DrawSetting.BlendMode == DX_BLENDMODE_NOBLEND ) AlphaTable = MemImgManage.RateTable[255] ;

		// ���_�f�[�^���i�[���郁�����̈悪����Ȃ��ꍇ�͊m�ۂ���
		Size = sizeof( VERTEX2D ) * VertexNum ;
		if( GSYS.Resource.TempVertexBufferSize < ( int )Size )
		{
			if( GSYS.Resource.TempVertexBuffer )
			{
				DXFREE( GSYS.Resource.TempVertexBuffer ) ;
				GSYS.Resource.TempVertexBufferSize = 0 ;
			}
			GSYS.Resource.TempVertexBuffer = DXALLOC( Size ) ;
			if( GSYS.Resource.TempVertexBuffer == NULL )
				return -1 ;
			GSYS.Resource.TempVertexBufferSize = ( int )Size ;
		}

		// �ꎞ�o�b�t�@�ɕϊ����Ȃ���]��
		sv = Vertex;
		dv = ( VERTEX2D * )GSYS.Resource.TempVertexBuffer ;
		for( i = 0 ; i < VertexNum ; i ++, sv ++, dv ++ )
		{
			dv->pos = sv->pos ;
			dv->rhw = sv->rhw ;
			dv->dif.b = ( BYTE )BlueTable [ sv->dif.b ] ;
			dv->dif.g = ( BYTE )GreenTable[ sv->dif.g ] ;
			dv->dif.r = ( BYTE )RedTable  [ sv->dif.r ] ;
			dv->dif.a = ( BYTE )AlphaTable[ sv->dif.a ] ;
			dv->u = sv->u ;
			dv->v = sv->v ;
		}
		UseVertex = ( VERTEX2D * )GSYS.Resource.TempVertexBuffer ;
	}
	else
	{
		UseVertex = Vertex ;
	}
	
	DRAW_DEF(
		Graphics_Hardware_DrawIndexedPrimitive2DUser_PF( UseVertex, VertexNum, Indices, IndexNum, PrimitiveType, Image, TransFlag ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	return Ret ;
}

// �R�c�|���S����`�悷��(�C���f�b�N�X)
extern int NS_DrawPrimitiveIndexed3D( const VERTEX3D *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	int Ret, i, Flag ;
	const VERTEX3D *UseVertex ;

	Ret = -1;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	Image = NULL ;
	if( GrHandle != DX_NONE_GRAPH )
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
		Flag = Image->Orig->FormatDesc.TextureFlag ;
	}
	else
	{
		Flag = GSYS.Setting.ValidHardware ;
	}

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// �F�̕ϊ����K�v�ȏꍇ�͂����ŏ�������
	if( GSYS.DrawSetting.DrawBright.Red   != 255 ||
		GSYS.DrawSetting.DrawBright.Green != 255 ||
		GSYS.DrawSetting.DrawBright.Blue  != 255 ||
		( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND && GSYS.DrawSetting.BlendParam != 255 ) )
	{
		DWORD *RedTable, *GreenTable, *BlueTable, *AlphaTable ;
		const VERTEX3D *sv ;
		VERTEX3D *dv ;
		unsigned int Size ;

		// �e�[�u����p�ӂ���
		RedTable   = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Red] ;
		GreenTable = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Green] ;
		BlueTable  = MemImgManage.RateTable[GSYS.DrawSetting.DrawBright.Blue] ;
		AlphaTable = MemImgManage.RateTable[GSYS.DrawSetting.BlendParam] ;
		if( GSYS.DrawSetting.BlendMode == DX_BLENDMODE_NOBLEND ) AlphaTable = MemImgManage.RateTable[255] ;

		// ���_�f�[�^���i�[���郁�����̈悪����Ȃ��ꍇ�͊m�ۂ���
		Size = sizeof( VERTEX3D ) * VertexNum ;
		if( GSYS.Resource.TempVertexBufferSize < ( int )Size )
		{
			if( GSYS.Resource.TempVertexBuffer )
			{
				DXFREE( GSYS.Resource.TempVertexBuffer ) ;
				GSYS.Resource.TempVertexBufferSize = 0 ;
			}
			GSYS.Resource.TempVertexBuffer = DXALLOC( Size ) ;
			if( GSYS.Resource.TempVertexBuffer == NULL )
				return -1 ;
			GSYS.Resource.TempVertexBufferSize = ( int )Size ;
		}

		// �ꎞ�o�b�t�@�ɕϊ����Ȃ���]��
		sv = Vertex;
		dv = ( VERTEX3D * )GSYS.Resource.TempVertexBuffer ;
		for( i = 0 ; i < VertexNum ; i ++, sv ++, dv ++ )
		{
			dv->pos = sv->pos ;
			dv->norm = sv->norm ;
			dv->dif.b = ( BYTE )BlueTable [ sv->dif.b ] ;
			dv->dif.g = ( BYTE )GreenTable[ sv->dif.g ] ;
			dv->dif.r = ( BYTE )RedTable  [ sv->dif.r ] ;
			dv->dif.a = ( BYTE )AlphaTable[ sv->dif.a ] ;
			dv->spc.b = ( BYTE )BlueTable [ sv->spc.b ] ;
			dv->spc.g = ( BYTE )GreenTable[ sv->spc.g ] ;
			dv->spc.r = ( BYTE )RedTable  [ sv->spc.r ] ;
			dv->spc.a = ( BYTE )AlphaTable[ sv->spc.a ] ;
			dv->u = sv->u ;
			dv->v = sv->v ;
			dv->su = sv->su ;
			dv->sv = sv->sv ;
		}
		UseVertex = ( VERTEX3D * )GSYS.Resource.TempVertexBuffer ;
	}
	else
	{
		UseVertex = Vertex ;
	}
	
	DRAW_DEF(
		Graphics_Hardware_DrawIndexedPrimitiveLight_PF( UseVertex, VertexNum, Indices, IndexNum, PrimitiveType, Image, TransFlag ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	return Ret ;
}

// ���_�o�b�t�@���g�p���ĂR�c�|���S����`�悷��
extern int NS_DrawPolygon3D_UseVertexBuffer( int VertexBufHandle, int GrHandle, int TransFlag )
{
	return NS_DrawPrimitive3D_UseVertexBuffer( VertexBufHandle, DX_PRIMTYPE_TRIANGLELIST, GrHandle, TransFlag ) ;
}

// ���_�o�b�t�@���g�p���ĂR�c�v���~�e�B�u��`�悷��
extern int NS_DrawPrimitive3D_UseVertexBuffer( int VertexBufHandle, int PrimitiveType  /* DX_PRIMTYPE_TRIANGLELIST �� */, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;
	int Ret, Flag ;

	Ret = -1;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	Image = NULL ;
	if( GrHandle != DX_NONE_GRAPH )
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
		Flag = Image->Orig->FormatDesc.TextureFlag ;
	}
	else
	{
		Flag = GSYS.Setting.ValidHardware ;
	}
	VertexBuffer = NULL ;
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
		return -1 ;

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	DRAW_DEF(
		Graphics_Hardware_DrawPrimitiveLight_UseVertexBuffer_PF( VertexBuffer, PrimitiveType, 0, VertexBuffer->Num, Image, TransFlag ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	return Ret ;
}

// ���_�o�b�t�@���g�p���ĂR�c�v���~�e�B�u��`�悷��
extern int NS_DrawPrimitive3D_UseVertexBuffer2( int VertexBufHandle, int PrimitiveType  /* DX_PRIMTYPE_TRIANGLELIST �� */, int StartVertex, int UseVertexNum, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;
	int Ret, Flag ;

	Ret = -1;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	Image = NULL ;
	if( GrHandle != DX_NONE_GRAPH )
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
		Flag = Image->Orig->FormatDesc.TextureFlag ;
	}
	else
	{
		Flag = GSYS.Setting.ValidHardware ;
	}
	VertexBuffer = NULL ;
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
		return -1 ;

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	DRAW_DEF(
		Graphics_Hardware_DrawPrimitiveLight_UseVertexBuffer_PF( VertexBuffer, PrimitiveType, StartVertex, UseVertexNum, Image, TransFlag ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	return Ret ;
}

// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���g�p���ĂR�c�|���S����`�悷��
extern int NS_DrawPolygonIndexed3D_UseVertexBuffer( int VertexBufHandle, int IndexBufHandle, int GrHandle, int TransFlag )
{
	return NS_DrawPrimitiveIndexed3D_UseVertexBuffer( VertexBufHandle, IndexBufHandle, DX_PRIMTYPE_TRIANGLELIST, GrHandle, TransFlag ) ;
}

// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���g�p���ĂR�c�v���~�e�B�u��`�悷��
extern int NS_DrawPrimitiveIndexed3D_UseVertexBuffer( int VertexBufHandle, int IndexBufHandle, int PrimitiveType  /* DX_PRIMTYPE_TRIANGLELIST �� */, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;
	INDEXBUFFERHANDLEDATA *IndexBuffer ;
	int Ret, Flag ;

	Ret = -1;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	Image = NULL ;
	if( GrHandle != DX_NONE_GRAPH )
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
		Flag = Image->Orig->FormatDesc.TextureFlag ;
	}
	else
	{
		Flag = GSYS.Setting.ValidHardware ;
	}
	VertexBuffer = NULL ;
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
		return -1 ;
	if( INDEXBUFFERCHK( IndexBufHandle, IndexBuffer ) )
		return -1 ;

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	DRAW_DEF(
		Graphics_Hardware_DrawIndexedPrimitiveLight_UseVertexBuffer_PF( VertexBuffer, IndexBuffer, PrimitiveType, 0, 0, VertexBuffer->Num, 0, IndexBuffer->Num, Image, TransFlag ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	return Ret ;
}

// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���g�p���ĂR�c�v���~�e�B�u��`�悷��
extern int NS_DrawPrimitiveIndexed3D_UseVertexBuffer2( int VertexBufHandle, int IndexBufHandle, int PrimitiveType  /* DX_PRIMTYPE_TRIANGLELIST �� */, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;
	INDEXBUFFERHANDLEDATA *IndexBuffer ;
	int Ret, Flag ;

	Ret = -1;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	Image = NULL ;
	if( GrHandle != DX_NONE_GRAPH )
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
		Flag = Image->Orig->FormatDesc.TextureFlag ;
	}
	else
	{
		Flag = GSYS.Setting.ValidHardware ;
	}
	VertexBuffer = NULL ;
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
		return -1 ;
	if( INDEXBUFFERCHK( IndexBufHandle, IndexBuffer ) )
		return -1 ;

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	DRAW_DEF(
		Graphics_Hardware_DrawIndexedPrimitiveLight_UseVertexBuffer_PF( VertexBuffer, IndexBuffer, PrimitiveType, BaseVertex, StartVertex, UseVertexNum, StartIndex, UseIndexNum, Image, TransFlag ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Flag
	)

	return Ret ;
}

// �O���t�B�b�N�̂R�c�`��
#define SETDRAWRECTCODE\
	float MinX, MinY, MaxX, MaxY ;\
\
	MinX =  100000000.0F ;\
	MinY =  100000000.0F ;\
	MaxX = -100000000.0F ;\
	MaxY = -100000000.0F ;\
	for( i = 0 ; i < 4 ; i ++ )\
	{\
		if( TransVert[i].pos.x - 1.0F < MinX ) MinX = TransVert[i].pos.x - 1.0F ;\
		if( TransVert[i].pos.x + 1.0F > MaxX ) MaxX = TransVert[i].pos.x + 1.0F ;\
\
		if( TransVert[i].pos.y - 1.0F < MinY ) MinY = TransVert[i].pos.y - 1.0F ;\
		if( TransVert[i].pos.y + 1.0F > MaxY ) MaxY = TransVert[i].pos.y + 1.0F ;\
	}\
\
	DrawRect.left   = _FTOL( MinX ) ;\
	DrawRect.top    = _FTOL( MinY ) ;\
	DrawRect.right  = _FTOL( MaxX ) ;\
	DrawRect.bottom = _FTOL( MaxY ) ;\
	DRAWRECT_DRAWAREA_CLIP

extern	int NS_DrawGraph3D( float x, float y, float z, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	int Ret = -1, i ;
	VERTEX_2D TransVert[6] ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// ���W�ϊ����s��
	{
		MATRIX *TransMatrix ;

		TransMatrix = &GSYS.DrawSetting.BlendMatrixF ;

		// ���_�����Z�b�g����
		{
			VECTOR SrcVec[4], Pos ;
			VERTEX_2D DestVec[4] ;
			float SizeX, SizeY ;
			
			// �r���{�[�h�̂S���_�𓾂�
			SizeX = Image->WidthF  * 0.5f ;
			SizeY = Image->HeightF * 0.5f ;
			Pos.x = x ;	Pos.y = y ;	Pos.z = z ;
			SrcVec[0].x = -SizeX ;	SrcVec[0].y =  SizeY ;	SrcVec[0].z = 0.0F ;
			SrcVec[1].x =  SizeX ;	SrcVec[1].y =  SizeY ;	SrcVec[1].z = 0.0F ;
			SrcVec[2].x = -SizeX ;	SrcVec[2].y = -SizeY ;	SrcVec[2].z = 0.0F ;
			SrcVec[3].x =  SizeX ;	SrcVec[3].y = -SizeY ;	SrcVec[3].z = 0.0F ;

			// ���W�ϊ�
			for( i = 0 ; i < 4 ; i ++ )
			{
				// �r���{�[�h���W�����[���h���W�֕ϊ�
				Graphics_Draw_VectorTransformToBillboard( &SrcVec[i], &Pos ) ;

				// �X�N���[�����W�ɕϊ�
				DestVec[i].rhw = 1.0F ;
				VectorTransform4( &DestVec[i].pos,  &DestVec[i].rhw,
								  &SrcVec[i],       &DestVec[i].rhw, TransMatrix ) ;
				DestVec[i].rhw = 1.0F / DestVec[i].rhw ;
				DestVec[i].pos.x *= DestVec[i].rhw ;
				DestVec[i].pos.y *= DestVec[i].rhw ;
				DestVec[i].pos.z *= DestVec[i].rhw ;
			}
									            
			// ���_���̃Z�b�g
			TransVert[0].pos = DestVec[0].pos ;
			TransVert[1].pos = DestVec[1].pos ;
			TransVert[2].pos = DestVec[2].pos ;
			TransVert[3].pos = DestVec[3].pos ;
			TransVert[0].rhw = DestVec[0].rhw ;
			TransVert[1].rhw = DestVec[1].rhw ;
			TransVert[2].rhw = DestVec[2].rhw ;
			TransVert[3].rhw = DestVec[3].rhw ;

			((BYTE *)&TransVert[0].color)[0] = (BYTE)GSYS.DrawSetting.DrawBright.Blue ;
			((BYTE *)&TransVert[0].color)[1] = (BYTE)GSYS.DrawSetting.DrawBright.Green ;
			((BYTE *)&TransVert[0].color)[2] = (BYTE)GSYS.DrawSetting.DrawBright.Red ;
			((BYTE *)&TransVert[0].color)[3] = ( BYTE )( ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND || GSYS.DrawSetting.UseNoBlendModeParam ) ? GSYS.DrawSetting.BlendParam : 255 ) ;
			TransVert[1].color =
			TransVert[2].color =
			TransVert[3].color = TransVert[0].color ;

			TransVert[0].u = 0.0F ;	TransVert[0].v = 0.0F ;
			TransVert[1].u = 1.0F ;	TransVert[1].v = 0.0F ;
			TransVert[2].u = 0.0F ;	TransVert[2].v = 1.0F ;
			TransVert[3].u = 1.0F ;	TransVert[3].v = 1.0F ;

			TransVert[4] = TransVert[2] ;
			TransVert[5] = TransVert[1] ;
		}
	}

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawPrimitive2D_PF( TransVert, 6, DX_PRIMTYPE_TRIANGLELIST, Image, TransFlag, TRUE, TRUE, FALSE, 0, FALSE ),
		0,
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

// �O���t�B�b�N�̊g��R�c�`��
extern	int NS_DrawExtendGraph3D( float x, float y, float z, double ExRateX, double ExRateY, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	int Ret = -1, i ;
	VERTEX_2D TransVert[6] ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle ) ;
#endif

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// ���W�ϊ����s��
	{
		MATRIX *TransMatrix ;

		TransMatrix = &GSYS.DrawSetting.BlendMatrixF ;

		// ���_�����Z�b�g����
		{
			VECTOR SrcVec[4], Pos ;
			VERTEX_2D DestVec[4] ;
			float SizeX, SizeY ;
			
			// �r���{�[�h�̂S���_�𓾂�
			SizeX = Image->WidthF  * ( float )ExRateX * 0.5F ;
			SizeY = Image->HeightF * ( float )ExRateY * 0.5F ;
			Pos.x = x ;	Pos.y = y ;	Pos.z = z ;
			SrcVec[0].x = -SizeX ;	SrcVec[0].y =  SizeY ;	SrcVec[0].z = 0.0F ;
			SrcVec[1].x =  SizeX ;	SrcVec[1].y =  SizeY ;	SrcVec[1].z = 0.0F ;
			SrcVec[2].x = -SizeX ;	SrcVec[2].y = -SizeY ;	SrcVec[2].z = 0.0F ;
			SrcVec[3].x =  SizeX ;	SrcVec[3].y = -SizeY ;	SrcVec[3].z = 0.0F ;

			// ���W�ϊ�
			for( i = 0 ; i < 4 ; i ++ )
			{
				// �r���{�[�h���W�����[���h���W�֕ϊ�
				Graphics_Draw_VectorTransformToBillboard( &SrcVec[i], &Pos ) ;

				// �X�N���[�����W�ɕϊ�
				DestVec[i].rhw = 1.0F ;
				VectorTransform4( &DestVec[i].pos,  &DestVec[i].rhw,
									      &SrcVec[i],  &DestVec[i].rhw, TransMatrix ) ;
				DestVec[i].rhw = 1.0F / DestVec[i].rhw ;
				DestVec[i].pos.x *= DestVec[i].rhw ;
				DestVec[i].pos.y *= DestVec[i].rhw ;
				DestVec[i].pos.z *= DestVec[i].rhw ;
			}
									            
			// ���_���̃Z�b�g
			TransVert[0].pos = DestVec[0].pos ;
			TransVert[1].pos = DestVec[1].pos ;
			TransVert[2].pos = DestVec[2].pos ;
			TransVert[3].pos = DestVec[3].pos ;
			TransVert[0].rhw = DestVec[0].rhw ;
			TransVert[1].rhw = DestVec[1].rhw ;
			TransVert[2].rhw = DestVec[2].rhw ;
			TransVert[3].rhw = DestVec[3].rhw ;

			((BYTE *)&TransVert[0].color)[0] = (BYTE)GSYS.DrawSetting.DrawBright.Blue ;
			((BYTE *)&TransVert[0].color)[1] = (BYTE)GSYS.DrawSetting.DrawBright.Green ;
			((BYTE *)&TransVert[0].color)[2] = (BYTE)GSYS.DrawSetting.DrawBright.Red ;
			((BYTE *)&TransVert[0].color)[3] = ( BYTE )( ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND || GSYS.DrawSetting.UseNoBlendModeParam ) ? GSYS.DrawSetting.BlendParam : 255 ) ;
			TransVert[1].color =
			TransVert[2].color =
			TransVert[3].color = TransVert[0].color ;

			TransVert[0].u = 0.0F ;	TransVert[0].v = 0.0F ;
			TransVert[1].u = 1.0F ;	TransVert[1].v = 0.0F ;
			TransVert[2].u = 0.0F ;	TransVert[2].v = 1.0F ;
			TransVert[3].u = 1.0F ;	TransVert[3].v = 1.0F ;

			TransVert[4] = TransVert[2] ;
			TransVert[5] = TransVert[1] ;
		}
	}

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawPrimitive2D_PF( TransVert, 6, DX_PRIMTYPE_TRIANGLELIST, Image, TransFlag, TRUE, TRUE, FALSE, 0, FALSE ),
		0,
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

// �O���t�B�b�N�̉�]�R�c�`��
extern	int NS_DrawRotaGraph3D( float x, float y, float z, double ExRate, double Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	IMAGEDATA *Image ;
	int Ret = -1, i ;
	VERTEX_2D TransVert[6] ;
	float Sin, Cos ;
	float ExtendRate = ( float )ExRate ; 
	float ExtendRateX ;
	float XBuf, YBuf ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle ) ;
#endif

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// ���W�ϊ����s��
	{
		MATRIX *TransMatrix ;

		TransMatrix = &GSYS.DrawSetting.BlendMatrixF ;

		// ���_�����Z�b�g����
		{
			VECTOR SrcVec[4], Pos ;
			VERTEX_2D DestVec[4]/*, VectBuf */ ;
			float SizeX, SizeY ;

			// ���]���l�������������̊g�嗦���Z�b�g
			ExtendRateX = ( ( TurnFlag == TRUE ) ? -1.0f : 1.0f ) * ExtendRate ;

			// �r���{�[�h�̂S���_�𓾂�
			{
				SizeX = Image->WidthF  * 0.5F * ExtendRateX ;
				SizeY = Image->HeightF * 0.5F * ExtendRate  ;
				Pos.x = x ;	Pos.y = y ;	Pos.z = z ;
				SrcVec[0].x = -SizeX ;	SrcVec[0].y =  SizeY ;	SrcVec[0].z = 0.0F ;
				SrcVec[1].x =  SizeX ;	SrcVec[1].y =  SizeY ;	SrcVec[1].z = 0.0F ;
				SrcVec[2].x = -SizeX ;	SrcVec[2].y = -SizeY ;	SrcVec[2].z = 0.0F ;
				SrcVec[3].x =  SizeX ;	SrcVec[3].y = -SizeY ;	SrcVec[3].z = 0.0F ;

				// ��]
				_SINCOS_PLATFORM( (float)Angle, &Sin, &Cos ) ;
				for( i = 0 ; i < 4 ; i ++ )
				{
					XBuf = SrcVec[i].x ;
					YBuf = SrcVec[i].y ;

					SrcVec[i].x = XBuf * Cos - YBuf * Sin ;	
					SrcVec[i].y = XBuf * Sin + YBuf * Cos ;
				}
			}

			// ���W�ϊ�
			for( i = 0 ; i < 4 ; i ++ )
			{
				// �r���{�[�h���W�����[���h���W�֕ϊ�
				Graphics_Draw_VectorTransformToBillboard( &SrcVec[i], &Pos ) ;

				// �X�N���[�����W�ɕϊ�
				DestVec[i].rhw = 1.0F ;
				VectorTransform4( &DestVec[i].pos,  &DestVec[i].rhw,
									      &SrcVec[i],  &DestVec[i].rhw, TransMatrix ) ;
				DestVec[i].rhw = 1.0F / DestVec[i].rhw ;
				DestVec[i].pos.x *= DestVec[i].rhw ;
				DestVec[i].pos.y *= DestVec[i].rhw ;
				DestVec[i].pos.z *= DestVec[i].rhw ;
			}
									            
			// ���_���̃Z�b�g
			TransVert[0].pos = DestVec[0].pos ;
			TransVert[1].pos = DestVec[1].pos ;
			TransVert[2].pos = DestVec[2].pos ;
			TransVert[3].pos = DestVec[3].pos ;
			TransVert[0].rhw = DestVec[0].rhw ;
			TransVert[1].rhw = DestVec[1].rhw ;
			TransVert[2].rhw = DestVec[2].rhw ;
			TransVert[3].rhw = DestVec[3].rhw ;

			((BYTE *)&TransVert[0].color)[0] = (BYTE)GSYS.DrawSetting.DrawBright.Blue ;
			((BYTE *)&TransVert[0].color)[1] = (BYTE)GSYS.DrawSetting.DrawBright.Green ;
			((BYTE *)&TransVert[0].color)[2] = (BYTE)GSYS.DrawSetting.DrawBright.Red ;
			((BYTE *)&TransVert[0].color)[3] = ( BYTE )( ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND || GSYS.DrawSetting.UseNoBlendModeParam ) ? GSYS.DrawSetting.BlendParam : 255 ) ;
			TransVert[1].color =
			TransVert[2].color =
			TransVert[3].color = TransVert[0].color ;

			TransVert[0].u = 0.0F ;	TransVert[0].v = 0.0F ;
			TransVert[1].u = 1.0F ;	TransVert[1].v = 0.0F ;
			TransVert[2].u = 0.0F ;	TransVert[2].v = 1.0F ;
			TransVert[3].u = 1.0F ;	TransVert[3].v = 1.0F ;

			// ���]����
//			if( TurnFlag == TRUE )
//			{
//				VectBuf = TransVert[ 1 ] ; TransVert[ 1 ] = TransVert[ 2 ] ; TransVert[ 2 ] = VectBuf ;
//			}

			TransVert[4] = TransVert[2] ;
			TransVert[5] = TransVert[1] ;
		}
	}

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawPrimitive2D_PF( TransVert, 6, DX_PRIMTYPE_TRIANGLELIST, Image, TransFlag, TRUE, TRUE, FALSE, 0, FALSE ),
		0,
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

// �O���t�B�b�N�̉�]�R�c�`��(��]���S�w��^)
extern	int NS_DrawRota2Graph3D( float x, float y, float z, float cx, float cy, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	IMAGEDATA *Image ;
	int Ret = -1, i ;
	VERTEX_2D TransVert[6] ;
	float Sin, Cos ;
	float ExtendRateX = ( float )ExtRateX ; 
	float ExtendRateY = ( float )ExtRateY ; 
	float XBuf, YBuf ;

	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

#ifndef DX_NON_MOVIE
	if( GrHandle != DX_NONE_GRAPH )
		UpdateMovie( Image->MovieHandle ) ;
#endif

	// �s����n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_ApplyLibMatrixToHardware() ;

	// ���W�ϊ����s��
	{
		MATRIX *TransMatrix ;

		TransMatrix = &GSYS.DrawSetting.BlendMatrixF ;

		// ���_�����Z�b�g����
		{
			VECTOR SrcVec[4], Pos ;
			VERTEX_2D DestVec[4]/* , VectBuf */ ;
			float SizeX, SizeY ;
			
			// �r���{�[�h�̂S���_�𓾂�
			{
				SizeX = Image->WidthF ;
				SizeY = Image->HeightF ;
				Pos.x = x ;	Pos.y = y ;	Pos.z = z ;
				if( TurnFlag == TRUE )
				{
					SrcVec[0].x = ( -cx + SizeX ) * ExtendRateX ;	SrcVec[0].y = ( -cy + SizeY ) * ExtendRateY ;	SrcVec[0].z = 0.0F ;
					SrcVec[1].x = -cx             * ExtendRateX ;	SrcVec[1].y = ( -cy + SizeY ) * ExtendRateY ;	SrcVec[1].z = 0.0F ;
					SrcVec[2].x = ( -cx + SizeX ) * ExtendRateX ;	SrcVec[2].y = -cy             * ExtendRateY ;	SrcVec[2].z = 0.0F ;
					SrcVec[3].x = -cx             * ExtendRateX ;	SrcVec[3].y = -cy             * ExtendRateY ;	SrcVec[3].z = 0.0F ;
				}
				else
				{
					SrcVec[0].x = -cx             * ExtendRateX ;	SrcVec[0].y = ( -cy + SizeY ) * ExtendRateY ;	SrcVec[0].z = 0.0F ;
					SrcVec[1].x = ( -cx + SizeX ) * ExtendRateX ;	SrcVec[1].y = ( -cy + SizeY ) * ExtendRateY ;	SrcVec[1].z = 0.0F ;
					SrcVec[2].x = -cx             * ExtendRateX ;	SrcVec[2].y = -cy             * ExtendRateY ;	SrcVec[2].z = 0.0F ;
					SrcVec[3].x = ( -cx + SizeX ) * ExtendRateX ;	SrcVec[3].y = -cy             * ExtendRateY ;	SrcVec[3].z = 0.0F ;
				}

				// ��]
				_SINCOS_PLATFORM( (float)Angle, &Sin, &Cos ) ;
				for( i = 0 ; i < 4 ; i ++ )
				{
					XBuf = SrcVec[i].x ;
					YBuf = SrcVec[i].y ;

					SrcVec[i].x = XBuf * Cos - YBuf * Sin ;	
					SrcVec[i].y = XBuf * Sin + YBuf * Cos ;
				}
			}

			// ���W�ϊ�
			for( i = 0 ; i < 4 ; i ++ )
			{
				// �r���{�[�h���W�����[���h���W�֕ϊ�
				Graphics_Draw_VectorTransformToBillboard( &SrcVec[i], &Pos ) ;

				// �X�N���[�����W�ɕϊ�
				DestVec[i].rhw = 1.0F ;
				VectorTransform4( &DestVec[i].pos,  &DestVec[i].rhw,
									      &SrcVec[i],  &DestVec[i].rhw, TransMatrix ) ;
				DestVec[i].rhw = 1.0F / DestVec[i].rhw ;
				DestVec[i].pos.x *= DestVec[i].rhw ;
				DestVec[i].pos.y *= DestVec[i].rhw ;
				DestVec[i].pos.z *= DestVec[i].rhw ;
			}
									            
			// ���_���̃Z�b�g
			TransVert[0].pos = DestVec[0].pos ;
			TransVert[1].pos = DestVec[1].pos ;
			TransVert[2].pos = DestVec[2].pos ;
			TransVert[3].pos = DestVec[3].pos ;
			TransVert[0].rhw = DestVec[0].rhw ;
			TransVert[1].rhw = DestVec[1].rhw ;
			TransVert[2].rhw = DestVec[2].rhw ;
			TransVert[3].rhw = DestVec[3].rhw ;

			((BYTE *)&TransVert[0].color)[0] = (BYTE)GSYS.DrawSetting.DrawBright.Blue ;
			((BYTE *)&TransVert[0].color)[1] = (BYTE)GSYS.DrawSetting.DrawBright.Green ;
			((BYTE *)&TransVert[0].color)[2] = (BYTE)GSYS.DrawSetting.DrawBright.Red ;
			((BYTE *)&TransVert[0].color)[3] = ( BYTE )( ( GSYS.DrawSetting.BlendMode != DX_BLENDMODE_NOBLEND || GSYS.DrawSetting.UseNoBlendModeParam ) ? GSYS.DrawSetting.BlendParam : 255 ) ;
			TransVert[1].color =
			TransVert[2].color =
			TransVert[3].color = TransVert[0].color ;

			TransVert[0].u = 0.0F ;	TransVert[0].v = 0.0F ;
			TransVert[1].u = 1.0F ;	TransVert[1].v = 0.0F ;
			TransVert[2].u = 0.0F ;	TransVert[2].v = 1.0F ;
			TransVert[3].u = 1.0F ;	TransVert[3].v = 1.0F ;

			TransVert[4] = TransVert[2] ;
			TransVert[5] = TransVert[1] ;

			// ���]����
//			if( TurnFlag == TRUE )
//			{
//				VectBuf = TransVert[ 1 ] ; TransVert[ 1 ] = TransVert[ 2 ] ; TransVert[ 2 ] = VectBuf ;
//			}

			TransVert[4] = TransVert[2] ;
			TransVert[5] = TransVert[1] ;
		}
	}

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawPrimitive2D_PF( TransVert, 6, DX_PRIMTYPE_TRIANGLELIST, Image, TransFlag, TRUE, TRUE, FALSE, 0, FALSE ),
		0,
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

#undef SETDRAWRECTCODE

// �O���t�B�b�N�̎��R�ό`�R�c�`��
extern int NS_DrawModiBillboard3D( VECTOR Pos, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int GrHandle, int TransFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret ;

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE ) return -1 ;

	// ��`��ݒ肪����Ă���ꍇ�͉��������I��
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) ) return -1 ;
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawModiBillboard3D_PF( Pos, x1, y1, x2, y2, x3, y3, x4, y4, Image, BlendImage, TransFlag ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

// �R�c��ԏ�ɃO���t�B�b�N��`��
extern int NS_DrawBillboard3D( VECTOR Pos, float cx, float cy, float Size, float Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret ;

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE ) return -1 ;

	// ��`��ݒ肪����Ă���ꍇ�͉��������I��
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) ) return -1 ;
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	// �`��
	DRAW_DEF(
		Graphics_Hardware_DrawBillboard3D_PF( Pos, cx, cy, Size, Angle, Image, BlendImage, TransFlag, TurnFlag ),
		0,
		DrawRect = GSYS.DrawSetting.DrawArea ;,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

























// �`��ݒ�֌W�֐�

// �`�惂�[�h���Z�b�g����
extern	int NS_SetDrawMode( int DrawMode )
{
	if( /* GSYS.NotDrawFlag || */ DrawMode == GSYS.DrawSetting.DrawMode )
		return 0 ;

	// �`�惂�[�h�̕ۑ�
	GSYS.DrawSetting.DrawMode = DrawMode ;
	GSYS.ChangeSettingFlag = TRUE ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����̐ݒ�ɔ��f
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetDrawMode_PF( DrawMode ) ;
	}

	// �I��
	return 0 ;
}

// �`�惂�[�h���擾����
extern int NS_GetDrawMode( void )
{
	return GSYS.DrawSetting.DrawMode ;
}

// �`��u�����h���[�h���Z�b�g����
extern	int NS_SetDrawBlendMode( int BlendMode, int BlendParam )
{
	if( /* GSYS.NotDrawFlag || */ ( GSYS.DrawSetting.BlendMode == BlendMode && GSYS.DrawSetting.BlendParam == BlendParam ) )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �u�����h���[�h�̕ۑ�
	if( GSYS.DrawSetting.BlendMode != BlendMode )
	{
		GSYS.DrawSetting.BlendMode = BlendMode ;
		GSYS.ChangeSettingFlag = TRUE ;
	}

	if( BlendParam > 255 ) BlendParam = 255 ;
	if( BlendParam < 0	 ) BlendParam = 0 ;
	GSYS.DrawSetting.BlendParam = BlendParam ;
	SetMemImgDrawBlendMode( GSYS.DrawSetting.BlendMode, GSYS.DrawSetting.BlendParam ) ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����̐ݒ�ɔ��f
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetDrawBlendMode_PF( BlendMode, BlendParam ) ;
	}

	// �I��
	return 0 ;
}

// �`��u�����h���[�h���擾����
extern int NS_GetDrawBlendMode( int *BlendMode, int *BlendParam )
{
	if( BlendMode	!= NULL ) *BlendMode =	GSYS.DrawSetting.BlendMode ;
	if( BlendParam	!= NULL ) *BlendParam =	GSYS.DrawSetting.BlendParam ;

	// �I��
	return 0 ;
}

// �`�掞�̃A���t�@�e�X�g�̐ݒ���s��( TestMode:DX_CMP_GREATER��( -1:�f�t�H���g����ɖ߂� )  TestParam:�`��A���t�@�l�Ƃ̔�r�Ɏg�p����l )
extern	int	NS_SetDrawAlphaTest( int TestMode, int TestParam )
{
	if( /* GSYS.NotDrawFlag || */ ( GSYS.DrawSetting.AlphaTestMode == TestMode && GSYS.DrawSetting.AlphaTestParam == TestParam ) )
		return 0 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �A���t�@�e�X�g�p�����[�^��ۑ�
	GSYS.DrawSetting.AlphaTestMode = TestMode ;
	if( TestParam > 255 ) TestParam = 255 ;
	if( TestParam < 0	 ) TestParam = 0 ;
	GSYS.DrawSetting.AlphaTestParam = TestParam ;
	GSYS.ChangeSettingFlag = TRUE ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����̐ݒ�ɔ��f
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetDrawAlphaTest_PF( TestMode, TestParam ) ;
	}

	// �I��
	return 0 ;
}

// �`�揈�����ɕ`�悷��摜�ƃu�����h���郿�`�����l���t���摜���Z�b�g����
extern	int NS_SetBlendGraph( int BlendGraph, int BorderParam, int BorderRange )
{
	return NS_SetBlendGraphParam( BlendGraph, DX_BLENDGRAPHTYPE_WIPE, BorderParam, BorderRange ) ;
}

// �`�揈�����ɕ`�悷��摜�ƃu�����h����摜�̃u�����h�ݒ���s��
extern	int NS_SetBlendGraphParam( int BlendGraph, int BlendType, ... )
{
	int Result ;
	va_list VaList ;

	if( BlendType < 0 || BlendType >= DX_BLENDGRAPHTYPE_NUM )
	{
		return -1 ;
	}

	va_start( VaList, BlendType ) ;

	Result = Graphics_DrawSetting_SetBlendGraphParamBase(  BlendGraph,  BlendType, VaList ) ;

	va_end( VaList ) ;

	return Result ;
}

// �u�����h�摜�̋N�_���W���Z�b�g����
extern	int NS_SetBlendGraphPosition( int x, int y )
{
	// ���܂łƒl�������ꍇ�͉��������I��
	if( GSYS.DrawSetting.BlendGraphX == x && GSYS.DrawSetting.BlendGraphY == y ) return 0 ;

	// ���W��ۑ�
	GSYS.DrawSetting.BlendGraphX = x ;
	GSYS.DrawSetting.BlendGraphY = y ;

	// �I��
	return 0 ;
}

// �u�����h�摜�̓K�����W���[�h��ݒ肷��
extern int NS_SetBlendGraphPositionMode( int BlendGraphPositionMode /* DX_BLENDGRAPH_POSMODE_DRAWGRAPH �Ȃ� */ )
{
	if( BlendGraphPositionMode < 0 || BlendGraphPositionMode >= DX_BLENDGRAPH_POSMODE_NUM )
	{
		return -1 ;
	}

	// ���W���[�h��ۑ�
	GSYS.DrawSetting.BlendGraphPosMode = BlendGraphPositionMode ;

	// �I��
	return 0 ;
}

// �`��P�x���Z�b�g
extern	int NS_SetDrawBright( int RedBright, int GreenBright, int BlueBright )
{
	if( GSYS.DrawSetting.DrawBright.Red   == RedBright  &&
		GSYS.DrawSetting.DrawBright.Green == GreenBright &&
		GSYS.DrawSetting.DrawBright.Blue  == BlueBright )
	{
		return 0 ;
	}

	if( (unsigned int)RedBright   > 255 )
	{
		     if( RedBright   > 255 ) RedBright   = 255 ;
		else if( RedBright   < 0   ) RedBright   = 0   ;
	}
	if( (unsigned int)GreenBright > 255 )
	{
		     if( GreenBright > 255 ) GreenBright = 255 ;
		else if( GreenBright < 0   ) GreenBright = 0   ;
	}
	if( (unsigned int)BlueBright  > 255 )
	{
		     if( BlueBright  > 255 ) BlueBright  = 255 ;
		else if( BlueBright  < 0   ) BlueBright  = 0   ;
	}

	// �P�x��ۑ�
	GSYS.DrawSetting.DrawBright.Red   = ( BYTE )RedBright   ;
	GSYS.DrawSetting.DrawBright.Blue  = ( BYTE )BlueBright  ;
	GSYS.DrawSetting.DrawBright.Green = ( BYTE )GreenBright ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����̐ݒ�ɔ��f
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetDrawBright_PF( RedBright, GreenBright, BlueBright ) ;
	}
	else
	{
		SetMemImgDrawBright( GSYS.DrawSetting.bDrawBright ) ;
	}

	// �I��
	return 0 ;
}

// �`��P�x���擾����
extern int NS_GetDrawBright( int *Red, int *Green, int *Blue )
{
	if( Red		!= NULL ) *Red =	GSYS.DrawSetting.DrawBright.Red ;
	if( Green	!= NULL ) *Green =	GSYS.DrawSetting.DrawBright.Green ;
	if( Blue	!= NULL ) *Blue =	GSYS.DrawSetting.DrawBright.Blue ;

	// �I��
	return 0 ;
}

// �`�悷��摜�̐F�����𖳎����邩�ǂ������w�肷��( EnableFlag:���̋@�\���g�����ǂ���( TRUE:�g��  FALSE:�g��Ȃ� ) )
extern int NS_SetIgnoreDrawGraphColor( int EnableFlag )
{
	if( GSYS.DrawSetting.IgnoreGraphColorFlag == EnableFlag )
		return 0 ;

	// �ݒ��ۑ�
	GSYS.DrawSetting.IgnoreGraphColorFlag = EnableFlag ;
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �ő�ٕ����̒l���Z�b�g����
extern int NS_SetMaxAnisotropy( int MaxAnisotropy )
{
	if( /* GSYS.NotDrawFlag || */ MaxAnisotropy == GSYS.DrawSetting.MaxAnisotropy )
		return 0 ;

	// �ő�ٕ����̕ۑ�
	GSYS.DrawSetting.MaxAnisotropy = MaxAnisotropy ;
	GSYS.ChangeSettingFlag = TRUE ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����̐ݒ�ɔ��f
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetMaxAnisotropy_PF( MaxAnisotropy ) ;
	}

	// �I��
	return 0 ;
}

// �R�c�����Ŏg�p������W�l�� 10000000.0f �Ȃǂ̑傫�Ȓl�ɂȂ��Ă��`��̕�����������}���鏈�����g�p���邩�ǂ�����ݒ肷��ADxLib_Init �̌Ăяo���O�ł̂ݎg�p�\( TRUE:�`��̕����}���鏈�����g�p����( CPU���ׂ��オ��܂� )�@�@FALSE:�`��̕����}���鏈���͎g�p���Ȃ�( �f�t�H���g ) )
extern int NS_SetUseLarge3DPositionSupport( int UseFlag )
{
	// �������O�̂ݗL��
	if( DxSysData.DxLib_InitializeFlag )
	{
		return -1 ;
	}

	// �t���O��ۑ�
	GSYS.DrawSetting.Large3DPositionSupport = UseFlag ;

#ifdef __WINDOWS__
	// TRUE �̏ꍇ�� SetUseFPUPreserveFlag ���L���ɂ���
	NS_SetUseFPUPreserveFlag( TRUE ) ;
#endif // __WINDOWS__

	// �I��
	return 0 ;
}


// �y�o�b�t�@��L���ɂ��邩�A�t���O���Z�b�g����
extern	int NS_SetUseZBufferFlag( int Flag )
{
	if( GSYS.DrawSetting.EnableZBufferFlag2D == Flag &&
		GSYS.DrawSetting.EnableZBufferFlag3D == Flag ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GSYS.DrawSetting.EnableZBufferFlag2D = Flag ;
	GSYS.DrawSetting.EnableZBufferFlag3D = Flag ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �y�o�b�t�@�ɏ������݂��s�����A�t���O���Z�b�g����
extern	int NS_SetWriteZBufferFlag( int Flag )
{
	if( GSYS.DrawSetting.WriteZBufferFlag2D == Flag &&
		GSYS.DrawSetting.WriteZBufferFlag3D == Flag ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GSYS.DrawSetting.WriteZBufferFlag2D = Flag ;
	GSYS.DrawSetting.WriteZBufferFlag3D = Flag ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �y�l�̔�r���[�h���Z�b�g����
extern	int NS_SetZBufferCmpType( int CmpType /* DX_CMP_NEVER �� */ )
{
	if( GSYS.DrawSetting.ZBufferCmpType2D == CmpType &&
		GSYS.DrawSetting.ZBufferCmpType3D == CmpType ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GSYS.DrawSetting.ZBufferCmpType2D = CmpType ;
	GSYS.DrawSetting.ZBufferCmpType3D = CmpType ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �y�o�C�A�X���Z�b�g����
extern	int NS_SetZBias( int Bias )
{
	if( GSYS.DrawSetting.ZBias2D == Bias &&
		GSYS.DrawSetting.ZBias3D == Bias ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GSYS.DrawSetting.ZBias2D = Bias ;
	GSYS.DrawSetting.ZBias3D = Bias ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �y�o�b�t�@��L���ɂ��邩�A�t���O���Z�b�g����( �R�c�}�`�`��݂̂ɉe�� )
extern int NS_SetUseZBuffer3D( int Flag )
{
	if( GSYS.DrawSetting.EnableZBufferFlag3D == Flag ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GSYS.DrawSetting.EnableZBufferFlag3D = Flag ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �y�o�b�t�@�ɏ������݂��s�����A�t���O���Z�b�g����( �R�c�}�`�`��݂̂ɉe�� )
extern int NS_SetWriteZBuffer3D( int Flag )
{
	if( GSYS.DrawSetting.WriteZBufferFlag3D == Flag ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GSYS.DrawSetting.WriteZBufferFlag3D = Flag ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �y�l�̔�r���[�h���Z�b�g����( �R�c�}�`�`��݂̂ɉe�� )
extern int NS_SetZBufferCmpType3D( int CmpType /* DX_CMP_NEVER �� */ )
{
	if( GSYS.DrawSetting.ZBufferCmpType3D == CmpType ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GSYS.DrawSetting.ZBufferCmpType3D = CmpType ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �y�o�C�A�X���Z�b�g����( �R�c�}�`�`��݂̂ɉe�� )
extern int NS_SetZBias3D( int Bias )
{
	if( GSYS.DrawSetting.ZBias3D == Bias ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GSYS.DrawSetting.ZBias3D = Bias ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �Q�c�`�掞�ɂy�o�b�t�@�ɏ������ނy�l��ύX����
extern int NS_SetDrawZ( float Z )
{
	GSYS.DrawSetting.DrawZ = Z;

	// �I��
	return 0;
}



// �`��\�̈�̃Z�b�g
extern	int NS_SetDrawArea( int x1, int y1, int x2, int y2 )
{
	const char *HandleString = NULL ;
	int Buf, SizeX, SizeY ;
	IMAGEDATA *Image ;
	SHADOWMAPDATA *ShadowMap ;

//	if( GSYS.NotDrawFlag ) return 0 ;

	// �t���X�N���[���}�X�N�������s���Ă���ꍇ�̓}�X�N�`��̌��ʂ𔽉f������
	MASK_FULLSCREEN_MASKUPDATE

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
//	CheckActiveState() ;

	// �I���W�i���̕`��G���A��ۑ�
	GSYS.DrawSetting.OriginalDrawRect.left   = x1 ;
	GSYS.DrawSetting.OriginalDrawRect.top    = y1 ;
	GSYS.DrawSetting.OriginalDrawRect.right  = x2 ;
	GSYS.DrawSetting.OriginalDrawRect.bottom = y2 ;
	
	// �`���̕`��\�T�C�Y���擾
	switch( ( DWORD )GSYS.DrawSetting.TargetScreen[ 0 ] )
	{
	case DX_SCREEN_BACK :
	case DX_SCREEN_FRONT :
		HandleString = GSYS.DrawSetting.TargetScreen[ 0 ] == DX_SCREEN_BACK ? "Back Buffer" : "Front Buffer" ;
		SizeX = GSYS.Screen.MainScreenSizeX ;
		SizeY = GSYS.Screen.MainScreenSizeY ;
		break ;

	default :
		HandleString = "Image Buffer" ;
		// �G���[����
		if( !GRAPHCHK( GSYS.DrawSetting.TargetScreen[ 0 ], Image ) )
		{
			SizeX = Image->WidthI ;
			SizeY = Image->HeightI ;
		}
		else
		if( !SHADOWMAPCHK( GSYS.DrawSetting.TargetScreen[ 0 ], ShadowMap ) )
		{
			SizeX = ShadowMap->BaseSizeX ;
			SizeY = ShadowMap->BaseSizeY ;
		}
		else
		{
			return -1 ;
		}
		break ;
	}

	// ���E�t�␳�Ƃ͂ݏo���␳
	if( x1 > x2 ){ Buf = x1 ; x1 = x2 ; x2 = Buf ; }
	if( y1 > y2 ){ Buf = y1 ; y1 = y2 ; y2 = Buf ; }

	if( x1 < 0 )		x1 = 0 ;
	if( x1 > SizeX )	x1 = SizeX ; 
	if( x2 < 0 )		x2 = 0 ;
	if( x2 > SizeX )	x2 = SizeX ;

	if( y1 < 0 )		y1 = 0 ;
	if( y1 > SizeY )	y1 = SizeY ; 
	if( y2 < 0 )		y2 = 0 ;
	if( y2 > SizeY )	y2 = SizeY ;

	// �`��\��`�Z�b�g
	GSYS.DrawSetting.DrawAreaF.left   = ( float )( GSYS.DrawSetting.DrawArea.left   = x1 ) ;
	GSYS.DrawSetting.DrawAreaF.top    = ( float )( GSYS.DrawSetting.DrawArea.top    = y1 ) ;
	GSYS.DrawSetting.DrawAreaF.right  = ( float )( GSYS.DrawSetting.DrawArea.right  = x2 ) ;
	GSYS.DrawSetting.DrawAreaF.bottom = ( float )( GSYS.DrawSetting.DrawArea.bottom = y2 ) ;
	SetMemImgDrawArea( &GSYS.DrawSetting.DrawArea ) ;

	// �`��\�t���O���Z�b�g����
	GSYS.DrawSetting.NotDrawFlagInSetDrawArea = x1 == x2 || y1 == y2 ;
	if( GSYS.DrawSetting.NotDrawFlagInSetDrawArea == FALSE )
	{
		// Direct3D �̃r���[�|�[�g�s��̍X�V
		CreateViewportMatrixD(
			&GSYS.DrawSetting.Direct3DViewportMatrix,
			( double )( GSYS.DrawSetting.DrawArea.right  - GSYS.DrawSetting.DrawArea.left ) / 2.0 + GSYS.DrawSetting.DrawArea.left,
			( double )( GSYS.DrawSetting.DrawArea.bottom - GSYS.DrawSetting.DrawArea.top  ) / 2.0 + GSYS.DrawSetting.DrawArea.top,
			( double )( GSYS.DrawSetting.DrawArea.right  - GSYS.DrawSetting.DrawArea.left ),
			( double )( GSYS.DrawSetting.DrawArea.bottom - GSYS.DrawSetting.DrawArea.top  ) ) ;
		CreateInverseMatrixD( &GSYS.DrawSetting.Direct3DViewportMatrixAnti, &GSYS.DrawSetting.Direct3DViewportMatrix ) ;
		ConvertMatrixDtoF( &GSYS.DrawSetting.Direct3DViewportMatrixAntiF, &GSYS.DrawSetting.Direct3DViewportMatrixAnti ) ;
		ConvertMatrixDtoF( &GSYS.DrawSetting.Direct3DViewportMatrixF,     &GSYS.DrawSetting.Direct3DViewportMatrix ) ;

		// �n�[�h�E�G�A�A�N�Z�����[�^���g�p����ꍇ�̏���
		if( GSYS.Setting.ValidHardware )
		{
			Graphics_Hardware_SetDrawArea_PF( x1, y1, x2, y2 ) ;
		}
	}

	// �I��
	return 0 ;
}

// �`��\�̈�𓾂�
extern int NS_GetDrawArea( RECT *Rect )
{
	// �Z�b�g
	*Rect = GSYS.DrawSetting.DrawArea ;

	// �I��
	return 0 ;
}

// �`��\�̈��`��Ώۉ�ʑS�̂ɂ���
extern int NS_SetDrawAreaFull( void )
{
	int Width, Height ;

	NS_GetGraphSize( GSYS.DrawSetting.TargetScreen[ 0 ], &Width, &Height ) ;
	return NS_SetDrawArea( 0, 0, Width, Height ) ;
}

// �R�c�`��̃X�P�[�����Z�b�g����
extern int NS_SetDraw3DScale( float Scale )
{
	// �X�P�[���l��␳
	if( Scale < 0.00001f && Scale > -0.00001f )
		Scale = 1.0f ;

	// �X�P�[���l��ۑ�
	GSYS.DrawSetting.Draw3DScale = Scale ;

	// �����_�̍Čv�Z
	NS_SetCameraScreenCenterD( GSYS.Camera.ScreenCenterX, GSYS.Camera.ScreenCenterY ) ;

	// �I��
	return 0 ;
}

// SetRestoreGraphCallback �̋���
extern int NS_SetRestoreShredPoint( void (* ShredPoint )( void ) )
{
	return NS_SetRestoreGraphCallback( ShredPoint ) ;
}

// �O���t�B�b�N�n���h�������֐��̓o�^
extern	int NS_SetRestoreGraphCallback( void (* Callback )( void ) )
{
	// �O���t�B�b�N�����X���b�h�A�h���X�̓o�^
	if( Callback == NULL )
	{
		GSYS.Setting.GraphRestoreShred = Graphics_Image_DefaultRestoreGraphFunction ;
	}
	else
	{
		GSYS.Setting.GraphRestoreShred = Callback ;
	}

	// �I��
	return 0 ;
}

// �O���t�B�b�N�����֐��̎��s
extern	int NS_RunRestoreShred( void )
{
#ifndef DX_NON_ASYNCLOAD
	// �񓯊��ǂݍ��݂͈ꎞ�I�ɖ����ɂ���
	int UseASyncThread = GetASyncLoadFlag() ;
	NS_SetUseASyncLoadFlag( FALSE ) ;
#endif

	// �O���t�B�b�N�����X���b�h�̎��s
	if( GSYS.Setting.GraphRestoreShred == NULL )
	{
		Graphics_Image_DefaultRestoreGraphFunction() ;
	}
	else
	{
		GSYS.Setting.GraphRestoreShred() ;
	}

#ifndef DX_NON_MODEL
	// ���f���̃e�N�X�`�����ēǂݍ���
	MV1ReloadTexture() ;
#endif

#ifndef DX_NON_FONT
	// �t�H���g�n���h�������L���b�V����������
	InitCacheFontToHandle() ;
#endif

#ifndef DX_NON_ASYNCLOAD
	// �񓯊��ǂݍ��݂̐ݒ�����ɖ߂�
	NS_SetUseASyncLoadFlag( UseASyncThread ) ;
#endif

	// �I��
	return 0 ;
}

// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A�����ۂɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern int NS_SetGraphicsDeviceRestoreCallbackFunction( void (* Callback )( void *Data ), void *CallbackData )
{
	return Graphics_Hardware_SetGraphicsDeviceRestoreCallbackFunction_PF( Callback, CallbackData ) ;
}

// �O���t�B�b�N�X�f�o�C�X�����X�g���畜�A����O�ɌĂ΂��R�[���o�b�N�֐���ݒ肷��
extern int NS_SetGraphicsDeviceLostCallbackFunction( void (* Callback )( void *Data ), void *CallbackData )
{
	return Graphics_Hardware_SetGraphicsDeviceLostCallbackFunction_PF( Callback, CallbackData ) ;
}

// ���[���h�ϊ��p�s����Z�b�g����
extern	int NS_SetTransformToWorld( const MATRIX *Matrix )
{
	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �f�[�^��ۑ�
	GSYS.DrawSetting.WorldMatrixF = *Matrix ;
	ConvertMatrixFtoD( &GSYS.DrawSetting.WorldMatrix, &GSYS.DrawSetting.WorldMatrixF ) ;
	GSYS.DrawSetting.ValidBlendMatrix = FALSE ;

	// �S�Ă̍s����|�����킹���s����X�V
	Graphics_DrawSetting_RefreshBlendTransformMatrix() ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetTransformToWorld_PF( &GSYS.DrawSetting.WorldMatrixF ) ;

		GSYS.DrawSetting.MatchHardwareWorldMatrix = TRUE ;
	}

	// �I��
	return 0 ;
}

// ���[���h�ϊ��p�s����Z�b�g����
extern	int NS_SetTransformToWorldD( const MATRIX_D *Matrix )
{
	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �f�[�^��ۑ�
	GSYS.DrawSetting.WorldMatrix = *Matrix ;
	ConvertMatrixDtoF( &GSYS.DrawSetting.WorldMatrixF, &GSYS.DrawSetting.WorldMatrix ) ;
	GSYS.DrawSetting.ValidBlendMatrix = FALSE ;

	// �S�Ă̍s����|�����킹���s����X�V
	Graphics_DrawSetting_RefreshBlendTransformMatrix() ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetTransformToWorld_PF( &GSYS.DrawSetting.WorldMatrixF ) ;

		GSYS.DrawSetting.MatchHardwareWorldMatrix = TRUE ;
	}

	// �I��
	return 0 ;
}

// ���[���h�s����擾����
extern	int NS_GetTransformToWorldMatrix( MATRIX *MatBuf )
{
	ConvertMatrixDtoF( MatBuf, &GSYS.DrawSetting.WorldMatrix ) ;

	// �I��
	return 0 ;
}

// ���[���h�s����擾����
extern	int NS_GetTransformToWorldMatrixD( MATRIX_D *MatBuf )
{
	*MatBuf = GSYS.DrawSetting.WorldMatrix ;

	// �I��
	return 0 ;
}

// �r���[�ϊ��p�s����Z�b�g����
extern	int NS_SetTransformToView( const MATRIX *Matrix )
{
	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �f�[�^��ۑ�
	GSYS.DrawSetting.ViewMatrixF      = *Matrix ;
	ConvertMatrixFtoD( &GSYS.DrawSetting.ViewMatrix, &GSYS.DrawSetting.ViewMatrixF ) ;
	GSYS.DrawSetting.ValidBlendMatrix = FALSE ;

	// �S�Ă̍s����|�����킹���s����X�V
	Graphics_DrawSetting_RefreshBlendTransformMatrix() ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetTransformToView_PF( &GSYS.DrawSetting.ViewMatrixF ) ;

		GSYS.DrawSetting.MatchHardwareViewMatrix = TRUE ;
	}

	// �I��
	return 0 ;
}

// �r���[�ϊ��p�s����Z�b�g����
extern	int NS_SetTransformToViewD( const MATRIX_D *Matrix )
{
	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �f�[�^��ۑ�
	GSYS.DrawSetting.ViewMatrix       = *Matrix ;
	ConvertMatrixDtoF( &GSYS.DrawSetting.ViewMatrixF, &GSYS.DrawSetting.ViewMatrix ) ;
	GSYS.DrawSetting.ValidBlendMatrix = FALSE ;

	// �S�Ă̍s����|�����킹���s����X�V
	Graphics_DrawSetting_RefreshBlendTransformMatrix() ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetTransformToView_PF( &GSYS.DrawSetting.ViewMatrixF ) ;

		GSYS.DrawSetting.MatchHardwareViewMatrix = TRUE ;
	}

	// �I��
	return 0 ;
}

// �r���[�s����擾����
extern	int NS_GetTransformToViewMatrix( MATRIX *MatBuf )
{
	ConvertMatrixDtoF( MatBuf, &GSYS.DrawSetting.ViewMatrix ) ;

	// �I��
	return 0 ;
}

// �r���[�s����擾����
extern	int NS_GetTransformToViewMatrixD( MATRIX_D *MatBuf )
{
	*MatBuf = GSYS.DrawSetting.ViewMatrix ;

	// �I��
	return 0 ;
}

// �ˉe�s���ݒ肷��
extern int NS_SetTransformToProjection( const MATRIX *Matrix )
{
	return NS_SetupCamera_ProjectionMatrix( *Matrix ) ;
}

// �ˉe�s���ݒ肷��
extern int NS_SetTransformToProjectionD( const MATRIX_D *Matrix )
{
	return NS_SetupCamera_ProjectionMatrixD( *Matrix ) ;
}

// �ˉe�s����擾����
extern	int NS_GetTransformToProjectionMatrix( MATRIX *MatBuf )
{
	*MatBuf = GSYS.DrawSetting.ProjectionMatrixF ;

	// �I��
	return 0 ;
}

// �ˉe�s����擾����
extern	int NS_GetTransformToProjectionMatrixD( MATRIX_D *MatBuf )
{
	*MatBuf = GSYS.DrawSetting.ProjectionMatrix ;

	// �I��
	return 0 ;
}

// �r���[�|�[�g�s����Z�b�g����
extern	int NS_SetTransformToViewport( const MATRIX *Matrix )
{
	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �f�[�^��ۑ�
	GSYS.DrawSetting.ViewportMatrixF   = *Matrix ;
	ConvertMatrixFtoD( &GSYS.DrawSetting.ViewportMatrix, &GSYS.DrawSetting.ViewportMatrixF ) ;
	GSYS.DrawSetting.ValidBlendMatrix = FALSE ;

	// �S�Ă̍s����|�����킹���s����X�V
	Graphics_DrawSetting_RefreshBlendTransformMatrix() ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetTransformToViewport_PF( &GSYS.DrawSetting.ViewportMatrixF ) ;

		GSYS.DrawSetting.MatchHardwareProjectionMatrix = TRUE ;
	}

	// �I��
	return 0 ;
}

// �r���[�|�[�g�s����Z�b�g����
extern	int NS_SetTransformToViewportD( const MATRIX_D *Matrix )
{
	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �f�[�^��ۑ�
	GSYS.DrawSetting.ViewportMatrix   = *Matrix ;
	ConvertMatrixDtoF( &GSYS.DrawSetting.ViewportMatrixF, &GSYS.DrawSetting.ViewportMatrix ) ;
	GSYS.DrawSetting.ValidBlendMatrix = FALSE ;

	// �S�Ă̍s����|�����킹���s����X�V
	Graphics_DrawSetting_RefreshBlendTransformMatrix() ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetTransformToViewport_PF( &GSYS.DrawSetting.ViewportMatrixF ) ;

		GSYS.DrawSetting.MatchHardwareProjectionMatrix = TRUE ;
	}

	// �I��
	return 0 ;
}

// �r���[�|�[�g�s����擾����
extern	int NS_GetTransformToViewportMatrix( MATRIX *MatBuf )
{
	*MatBuf = GSYS.DrawSetting.ViewportMatrixF ;

	// �I��
	return 0 ;
}

// �r���[�|�[�g�s����擾����
extern	int NS_GetTransformToViewportMatrixD( MATRIX_D *MatBuf )
{
	*MatBuf = GSYS.DrawSetting.ViewportMatrix ;

	// �I��
	return 0 ;
}

// Direct3D�Ŏ����K�p�����r���[�|�[�g�s����擾����
extern	int NS_GetTransformToAPIViewportMatrix( MATRIX *MatBuf )
{
	*MatBuf = GSYS.DrawSetting.Direct3DViewportMatrixF ;

	// �I��
	return 0 ;
}

// Direct3D�Ŏ����K�p�����r���[�|�[�g�s����擾����
extern	int NS_GetTransformToAPIViewportMatrixD( MATRIX_D *MatBuf )
{
	*MatBuf = GSYS.DrawSetting.Direct3DViewportMatrix ;

	// �I��
	return 0 ;
}

// �f�t�H���g�̕ϊ��s����Z�b�g����
#define FOV				(60.0 * DX_PI / 180.0)
#define NEARZ			(0.0)
#define FARZ			(1000.0)
#define TAN_FOV_HALF	(0.52359877559829887307710723054658) // /*(0.57735026918962573)*/ tan( FOV * 0.5 )
extern	int NS_SetDefTransformMatrix( void )
{
	MATRIX_D mat ;
	double D ;
	
	// ���[���h�ϊ��s��͒P�ʍs��
	CreateIdentityMatrixD( &mat ) ;
	NS_SetTransformToWorldD( &mat ) ;

	// �r���[�|�[�g�s��̃Z�b�g
	CreateViewportMatrixD(
		&mat,
		( double )GSYS.DrawSetting.DrawSizeX / 2,
		( double )GSYS.DrawSetting.DrawSizeY / 2,
		( double )( GSYS.DrawSetting.DrawSizeX * GSYS.DrawSetting.Draw3DScale ),
		( double )( GSYS.DrawSetting.DrawSizeY * GSYS.DrawSetting.Draw3DScale )
	) ;
	NS_SetTransformToViewportD( &mat ) ;

	// �r���[�s��� z = 0.0 �̎��ɒ��x�X�N���[���S�̂��ʂ�ʒu�ƕ��������J������
	{
		VECTOR_D up, at, eye ;
		int w, h ;

		w = GSYS.DrawSetting.DrawSizeX ;
		h = GSYS.DrawSetting.DrawSizeY ;
		D = ( double )( ( h / 2 ) / TAN_FOV_HALF ) ;
		eye.x = ( double )w / 2 ;
		eye.y = ( double )h / 2 ;
		eye.z = -D ;

		at.x = eye.x ;
		at.y = eye.y ;
		at.z = eye.z + 1.0 ;

		up.x = 0.0 ;
		up.y = 1.0 ;
		up.z = 0.0 ;

		CreateLookAtMatrixD( &mat, &eye, &at, &up ) ;
		NS_SetTransformToViewD( &mat ) ;
	}

	// �v���W�F�N�V�����s��͕��ʂ�
	CreatePerspectiveFovMatrixD( &mat, FOV, D * 0.1, D + FARZ ) ;
	NS_SetTransformToProjectionD( &mat ) ;

	// �I��
	return 0 ;
}

// ���[�J�����W�����ʍ��W���擾����
extern	int NS_GetTransformPosition( VECTOR *LocalPos, float *x, float *y )
{
	float w ;
	VECTOR ScreenPos ;

	if( LocalPos == NULL ) return -1 ;

	// ���W�ϊ�
	w = 1.0F ;
	VectorTransform4(
		&ScreenPos, &w,
		LocalPos,   &w,
		&GSYS.DrawSetting.BlendMatrixF
	) ;
	w = 1.0F / w ;

	// �y�l���O�ȉ��̏ꍇ�͐���Ȃw�x�l�͓����Ȃ��̂ŃG���[
	if( ScreenPos.z * w < 0.0F )
	{
		return -1 ;
	}

	if( x != NULL )
	{
		*x = ScreenPos.x * w ;
	}

	if( y != NULL )
	{
		*y = ScreenPos.y * w ;
	}

	// ����I��
	return 0 ;
}

// ���[�J�����W�����ʍ��W���擾����
extern	int NS_GetTransformPositionD( VECTOR_D *LocalPos, double *x, double *y )
{
	double w ;
	VECTOR_D ScreenPos ;

	if( LocalPos == NULL ) return -1 ;

	// ���W�ϊ�
	w = 1.0 ;
	VectorTransform4D(
		&ScreenPos, &w,
		LocalPos,   &w,
		&GSYS.DrawSetting.BlendMatrix
	) ;
	w = 1.0 / w ;

	// �y�l���O�ȉ��̏ꍇ�͐���Ȃw�x�l�͓����Ȃ��̂ŃG���[
	if( ScreenPos.z * w < 0.0 )
	{
		return -1 ;
	}

	if( x != NULL )
	{
		*x = ScreenPos.x * w ;
	}

	if( y != NULL )
	{
		*y = ScreenPos.y * w ;
	}

	// ����I��
	return 0 ;
}

// ���[���h��ԏ�̃r���{�[�h�̃T�C�Y����X�N���[���ɓ��e�����ꍇ�̃s�N�Z���P�ʂ̃T�C�Y���擾����
extern float NS_GetBillboardPixelSize( VECTOR WorldPos, float WorldSize )
{
	return ( float )NS_GetBillboardPixelSizeD( VConvFtoD( WorldPos ), WorldSize ) ;
}

// ���[���h��ԏ�̃r���{�[�h�̃T�C�Y����X�N���[���ɓ��e�����ꍇ�̃s�N�Z���P�ʂ̃T�C�Y���擾����
extern double NS_GetBillboardPixelSizeD( VECTOR_D WorldPos, double WorldSize )
{
	VECTOR_D ScreenPos, ScreenLeft, ScreenRightDirection ;

	ScreenPos            = NS_ConvWorldPosToScreenPosD( WorldPos ) ;
	ScreenRightDirection = VNormD( VGetD( GSYS.DrawSetting.ViewMatrix.m[ 0 ][ 0 ], GSYS.DrawSetting.ViewMatrix.m[ 1 ][ 0 ], GSYS.DrawSetting.ViewMatrix.m[ 2 ][ 0 ] ) ) ;
	ScreenLeft           = NS_ConvWorldPosToScreenPosD( VSubD( WorldPos, VScaleD( ScreenRightDirection, WorldSize / 2.0 ) ) ) ;
	return VSubD( ScreenPos, ScreenLeft ).x * 2.0 ;
}

// ���[���h���W���J�������W�ɕϊ�����
extern VECTOR NS_ConvWorldPosToViewPos( VECTOR WorldPos )
{
	return VConvDtoF( NS_ConvWorldPosToViewPosD( VConvFtoD( WorldPos ) ) ) ;
}

// ���[���h���W���J�������W�ɕϊ�����
extern VECTOR_D NS_ConvWorldPosToViewPosD( VECTOR_D WorldPos )
{
	VECTOR_D ViewPos ;

	VectorTransformD( &ViewPos, &WorldPos, &GSYS.DrawSetting.ViewMatrix ) ;

	return ViewPos ;
}

// ���[���h���W���X�N���[�����W�ɕϊ�����
extern VECTOR NS_ConvWorldPosToScreenPos( VECTOR WorldPos )
{
	float w ;
	VECTOR ScreenPos ;

	// ���W�ϊ�
	w = 1.0F ;
	VectorTransform4( &ScreenPos, &w, &WorldPos, &w, &GSYS.DrawSetting.BlendMatrixF ) ;

	// ��ʍ��W��Ԃ�
	w = 1.0F / w ;
	ScreenPos.x *= w ;
	ScreenPos.y *= w ;
	ScreenPos.z *= w ;

	return ScreenPos ;
}

// ���[���h���W���X�N���[�����W�ɕϊ�����
extern VECTOR_D NS_ConvWorldPosToScreenPosD( VECTOR_D WorldPos )
{
	double   w ;
	VECTOR_D ScreenPos ;

	// ���W�ϊ�
	w = 1.0 ;
	VectorTransform4D( &ScreenPos, &w, &WorldPos, &w, &GSYS.DrawSetting.BlendMatrix ) ;

	// ��ʍ��W��Ԃ�
	w = 1.0 / w ;
	ScreenPos.x *= w ;
	ScreenPos.y *= w ;
	ScreenPos.z *= w ;

	return ScreenPos ;
}

// ���[���h���W���X�N���[�����W�ɕϊ�����A�Ō�̂w�x�y���W���v�Ŋ���O�̒l�𓾂�
extern FLOAT4 NS_ConvWorldPosToScreenPosPlusW( VECTOR WorldPos )
{
	VECTOR ScreenPos ;
	float w ;
	FLOAT4 Result ;

	// ���W�ϊ�
	w = 1.0f ;
	VectorTransform4( &ScreenPos, &w, &WorldPos, &w, &GSYS.DrawSetting.BlendMatrixF ) ;

	Result.x = ScreenPos.x ;
	Result.y = ScreenPos.y ;
	Result.z = ScreenPos.z ;
	Result.w = w ;

	return Result ;
}

// ���[���h���W���X�N���[�����W�ɕϊ�����A�Ō�̂w�x�y���W���v�Ŋ���O�̒l�𓾂�
extern DOUBLE4 NS_ConvWorldPosToScreenPosPlusWD( VECTOR_D WorldPos )
{
	VECTOR_D ScreenPos ;
	double   w ;
	DOUBLE4 Result ;

	// ���W�ϊ�
	w = 1.0 ;
	VectorTransform4D( &ScreenPos, &w, &WorldPos, &w, &GSYS.DrawSetting.BlendMatrix ) ;

	Result.x = ScreenPos.x ;
	Result.y = ScreenPos.y ;
	Result.z = ScreenPos.z ;
	Result.w = w ;

	return Result ;
}

// �X�N���[�����W�����[���h���W�ɕϊ�����
extern VECTOR NS_ConvScreenPosToWorldPos( VECTOR ScreenPos )
{
	float w ;
	VECTOR WorldPos ;

	// �u�����h�s��̋t�s�񂪗L���ł͂Ȃ�������t�s����\�z����
	if( GSYS.DrawSetting.ValidInverseBlendMatrix == FALSE )
	{
		CreateInverseMatrixD( &GSYS.DrawSetting.InverseBlendMatrix, &GSYS.DrawSetting.BlendMatrix ) ;
		ConvertMatrixDtoF( &GSYS.DrawSetting.InverseBlendMatrixF, &GSYS.DrawSetting.InverseBlendMatrix ) ;
		GSYS.DrawSetting.ValidInverseBlendMatrix = TRUE ;
	}

	// ���W�ϊ�
	w = 1.0F ;
	VectorTransform4( &WorldPos, &w, &ScreenPos, &w, &GSYS.DrawSetting.InverseBlendMatrixF ) ;

	// ���[���h���W��Ԃ�
	w = 1.0F / w ;
	WorldPos.x *= w ;
	WorldPos.y *= w ;
	WorldPos.z *= w ;
	return WorldPos ;
}

// �X�N���[�����W�����[���h���W�ɕϊ�����
extern VECTOR_D NS_ConvScreenPosToWorldPosD( VECTOR_D ScreenPos )
{
	double w ;
	VECTOR_D WorldPos ;

	// �u�����h�s��̋t�s�񂪗L���ł͂Ȃ�������t�s����\�z����
	if( GSYS.DrawSetting.ValidInverseBlendMatrix == FALSE )
	{
		CreateInverseMatrixD( &GSYS.DrawSetting.InverseBlendMatrix, &GSYS.DrawSetting.BlendMatrix ) ;
		ConvertMatrixDtoF( &GSYS.DrawSetting.InverseBlendMatrixF, &GSYS.DrawSetting.InverseBlendMatrix ) ;
		GSYS.DrawSetting.ValidInverseBlendMatrix = TRUE ;
	}

	// ���W�ϊ�
	w = 1.0 ;
	VectorTransform4D( &WorldPos, &w, &ScreenPos, &w, &GSYS.DrawSetting.InverseBlendMatrix ) ;

	// ���[���h���W��Ԃ�
	w = 1.0 / w ;
	WorldPos.x *= w ;
	WorldPos.y *= w ;
	WorldPos.z *= w ;
	return WorldPos ;
}

// �X�N���[�����W�����[���h���W�ɕϊ�����( Z���W�����` )
extern VECTOR NS_ConvScreenPosToWorldPos_ZLinear( VECTOR ScreenPos )
{
	return VConvDtoF( NS_ConvScreenPosToWorldPos_ZLinearD( VConvFtoD( ScreenPos ) ) ) ;
}

// �X�N���[�����W�����[���h���W�ɕϊ�����( Z���W�����` )
extern VECTOR_D NS_ConvScreenPosToWorldPos_ZLinearD( VECTOR_D ScreenPos )
{
	double w, inz ;
	VECTOR_D WorldPos ;

	// �u�����h�s��̋t�s�񂪗L���ł͂Ȃ�������t�s����\�z����
	if( GSYS.DrawSetting.ValidInverseBlendMatrix == FALSE )
	{
		CreateInverseMatrixD( &GSYS.DrawSetting.InverseBlendMatrix, &GSYS.DrawSetting.BlendMatrix ) ;
		ConvertMatrixDtoF( &GSYS.DrawSetting.InverseBlendMatrixF, &GSYS.DrawSetting.InverseBlendMatrix ) ;
		GSYS.DrawSetting.ValidInverseBlendMatrix = TRUE ;
	}

	// �X�N���[�����W�����[���h���W�ɕϊ�����ۂɎg�p����s�񂪗L���ł͂Ȃ�������\�z����
	if( GSYS.DrawSetting.ValidConvScreenPosToWorldPosMatrix == FALSE )
	{
		MATRIX_D TempMatrix ;

		CreateMultiplyMatrixD( &TempMatrix, &GSYS.DrawSetting.ProjectionMatrix, &GSYS.DrawSetting.ViewportMatrix ) ;
		CreateInverseMatrixD( &GSYS.DrawSetting.ConvScreenPosToWorldPosMatrix, &TempMatrix ) ;
		GSYS.DrawSetting.ValidConvScreenPosToWorldPosMatrix = TRUE ;
	}

	// VectorTransform4 �ɓn���ׂ� z �l�����߂�
	{
		double k1, k2, resz, testz, minz, maxz, sa, target, sikii_plus, sikii_minus ;
		int num ;

		k1 = ScreenPos.x * GSYS.DrawSetting.ConvScreenPosToWorldPosMatrix.m[0][2] + ScreenPos.y * GSYS.DrawSetting.ConvScreenPosToWorldPosMatrix.m[1][2] + GSYS.DrawSetting.ConvScreenPosToWorldPosMatrix.m[3][2] ;
		k2 = ScreenPos.x * GSYS.DrawSetting.ConvScreenPosToWorldPosMatrix.m[0][3] + ScreenPos.y * GSYS.DrawSetting.ConvScreenPosToWorldPosMatrix.m[1][3] + GSYS.DrawSetting.ConvScreenPosToWorldPosMatrix.m[3][3] ;
		target = ScreenPos.z * ( GSYS.DrawSetting.ProjFar - GSYS.DrawSetting.ProjNear ) + GSYS.DrawSetting.ProjNear ;
		testz = 0.5 ;
		sikii_plus  =  ( GSYS.DrawSetting.ProjFar - GSYS.DrawSetting.ProjNear ) / 100000.0 ;
		sikii_minus = -sikii_plus ;
		minz = 0.0 ;
		maxz = 1.0 ;
		for( num = 0 ; num < 20 ; num ++ )
		{
			resz = ( k1 + testz * GSYS.DrawSetting.ConvScreenPosToWorldPosMatrix.m[2][2] ) / ( k2 + testz * GSYS.DrawSetting.ConvScreenPosToWorldPosMatrix.m[2][3] ) ;
			if( resz > target )
			{
				maxz = testz ;
			}
			else
			{
				minz = testz ;
			}
			sa = resz - target ;
			if( sa < 0.0 )
			{
				if( sa > sikii_minus ) break ;
			}
			else
			{
				if( sa < sikii_plus  ) break ;
			}
			testz = ( maxz + minz ) * 0.5 ;
		}
		inz = testz ;
	}

	// ���W�ϊ�
	w = 1.0 ;
	ScreenPos.z = inz ;
	VectorTransform4D( &WorldPos, &w, &ScreenPos, &w, &GSYS.DrawSetting.InverseBlendMatrix ) ;

	// ���[���h���W��Ԃ�
	w = 1.0 / w ;
	WorldPos.x *= w ;
	WorldPos.y *= w ;
	WorldPos.z *= w ;
	return WorldPos ;
}



// �|���S���J�����O�̗L���A�������Z�b�g����
extern int NS_SetUseCullingFlag( int Flag )
{
	if( GSYS.DrawSetting.CullMode == Flag ) return 0 ;

	// �J�����O���[�h�̃t���O��ۑ�
	GSYS.DrawSetting.CullMode = Flag ;
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �|���S���J�����O�̗L���A�������Z�b�g����
extern int NS_SetUseBackCulling( int Flag )
{
	return NS_SetUseCullingFlag( Flag ) ;
}

// �|���S���J�����O���[�h���擾����
extern int NS_GetUseBackCulling( void )
{
	return GSYS.DrawSetting.CullMode ;
}


// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int NS_SetTextureAddressMode( int Mode /* DX_TEXADDRESS_WRAP �� */, int Stage )
{
	int i ;

	if( Stage == -1 )
	{
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( GSYS.DrawSetting.TexAddressModeU[ i ] != Mode ||
				GSYS.DrawSetting.TexAddressModeV[ i ] != Mode ||
				GSYS.DrawSetting.TexAddressModeW[ i ] != Mode )
			{
				break ;
			}
		}

		if( i == USE_TEXTURESTAGE_NUM )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			GSYS.DrawSetting.TexAddressModeU[ i ] = Mode ;
			GSYS.DrawSetting.TexAddressModeV[ i ] = Mode ;
			GSYS.DrawSetting.TexAddressModeW[ i ] = Mode ;
		}

		GSYS.ChangeSettingFlag = TRUE ;
	}
	else
	{
		if( Stage < 0 || Stage >= USE_TEXTURESTAGE_NUM ) return 0 ;
		if( GSYS.DrawSetting.TexAddressModeU[ Stage ] == Mode &&
			GSYS.DrawSetting.TexAddressModeV[ Stage ] == Mode &&
			GSYS.DrawSetting.TexAddressModeW[ Stage ] == Mode ) return 0;

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		GSYS.DrawSetting.TexAddressModeU[ Stage ] = Mode ;
		GSYS.DrawSetting.TexAddressModeV[ Stage ] = Mode ;
		GSYS.DrawSetting.TexAddressModeW[ Stage ] = Mode ;
		GSYS.ChangeSettingFlag = TRUE ;
	}

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetTextureAddressMode_PF( Mode, Stage ) ;
	}

	// �I��
	return 0 ;
}

// �e�N�X�`���A�h���X���[�h��ݒ肷��
extern	int NS_SetTextureAddressModeUV( int ModeU, int ModeV, int Stage )
{
	int i ;

	if( Stage == -1 )
	{
		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			if( GSYS.DrawSetting.TexAddressModeU[ i ] != ModeU ||
				GSYS.DrawSetting.TexAddressModeV[ i ] != ModeV )
			{
				break ;
			}
		}

		if( i == USE_TEXTURESTAGE_NUM )
		{
			return 0 ;
		}

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
		{
			GSYS.DrawSetting.TexAddressModeU[ i ] = ModeU ;
			GSYS.DrawSetting.TexAddressModeV[ i ] = ModeV ;
		}

		GSYS.ChangeSettingFlag = TRUE ;
	}
	else
	{
		if( Stage < 0 || Stage >= USE_TEXTURESTAGE_NUM ) return 0 ;
		if( GSYS.DrawSetting.TexAddressModeU[ Stage ] == ModeU &&
			GSYS.DrawSetting.TexAddressModeV[ Stage ] == ModeV ) return 0;

		// �`��ҋ@���Ă���`�敨��`��
		DRAWSTOCKINFO

		GSYS.DrawSetting.TexAddressModeU[ Stage ] = ModeU ;
		GSYS.DrawSetting.TexAddressModeV[ Stage ] = ModeV ;
		GSYS.ChangeSettingFlag = TRUE ;
	}

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetTextureAddressModeUV_PF( ModeU, ModeV, Stage ) ;
	}

	// �I��
	return 0 ;
}

// �e�N�X�`�����W�ϊ��p�����[�^���Z�b�g����
extern int NS_SetTextureAddressTransform( float TransU, float TransV, float ScaleU, float ScaleV, float RotCenterU, float RotCenterV, float Rotate )
{
	MATRIX Transform, Temp1, Temp2, Temp3 ;
	int UseFlag ;

	UseFlag = FALSE ;

	if( Rotate != 0.0f )
	{
		CreateTranslationMatrix( &Temp1, -RotCenterU, -RotCenterV, 0.0f ) ; 
		CreateRotationZMatrix( &Temp2, Rotate ) ;
		CreateTranslationMatrix( &Temp3,  RotCenterU,  RotCenterV, 0.0f ) ; 
		CreateMultiplyMatrix( &Transform, &Temp1, &Temp2 ) ;
		CreateMultiplyMatrix( &Transform, &Transform, &Temp3 ) ;
		UseFlag = TRUE ;
	}
	else
	{
		CreateIdentityMatrix( &Transform ) ;
	}

	if( TransU != 0.0f || TransV != 0.0f )
	{
		CreateTranslationMatrix( &Temp1, TransU, TransV, 0.0f ) ;
		CreateMultiplyMatrix( &Transform, &Transform, &Temp1 ) ;
		UseFlag = TRUE ;
	}

	if( ScaleU != 1.0f || ScaleV != 1.0f )
	{
		CreateScalingMatrix( &Temp1, ScaleU, ScaleV, 1.0f ) ;
		CreateMultiplyMatrix( &Transform, &Transform, &Temp1 ) ;
		UseFlag = TRUE ;
	}

	// ������g�p���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.DrawSetting.TextureAddressTransformUse == UseFlag && UseFlag == FALSE )
		return 0 ;

	// �ݒ��ۑ�
	GSYS.DrawSetting.TextureAddressTransformUse = UseFlag ;
	GSYS.DrawSetting.TextureAddressTransformMatrix = Transform ;
//	GSYS.DrawSetting.TextureMatrixValid = FALSE ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetTextureAddressTransformMatrix_PF( UseFlag, &Transform ) ;

		GSYS.DrawSetting.MatchHardwareTextureAddressTransformMatrix = TRUE ;
	}

	// ����I��
	return 0 ;
}

// �e�N�X�`�����W�ϊ��s����Z�b�g����
extern int NS_SetTextureAddressTransformMatrix( MATRIX Matrix )
{
	// �ݒ��ۑ�
	GSYS.DrawSetting.TextureAddressTransformUse    = TRUE ;
//	GSYS.DrawSetting.TextureMatrixValid     = TRUE ;
//	GSYS.TextureMatrix          = Matrix ;
	GSYS.DrawSetting.TextureAddressTransformMatrix = Matrix ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetTextureAddressTransformMatrix_PF( TRUE, &Matrix ) ;

		GSYS.DrawSetting.MatchHardwareTextureAddressTransformMatrix = TRUE ;
	}

	// ����I��
	return 0 ;
}

// �e�N�X�`�����W�ϊ��p�����[�^�����Z�b�g����
extern int NS_ResetTextureAddressTransform( void )
{
	return NS_SetTextureAddressTransform( 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f ) ;
}

// �t�H�O��L���ɂ��邩�ǂ�����ݒ肷��( TRUE:�L��  FALSE:���� )
extern int NS_SetFogEnable( int Flag )
{
	if( GSYS.DrawSetting.FogEnable == Flag ) return 0;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GSYS.DrawSetting.FogEnable = Flag ;
	GSYS.ChangeSettingFlag     = TRUE ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetFogEnable_PF( Flag ) ;
	}

	// �I��
	return 0 ;
}

// �t�H�O���L�����ǂ������擾����( TRUE:�L��  FALSE:���� )
extern int NS_GetFogEnable( void )
{
	return GSYS.DrawSetting.FogEnable ;
}

// �t�H�O���[�h��ݒ肷��
extern	int NS_SetFogMode( int Mode /* DX_FOGMODE_NONE �� */ )
{
	if( GSYS.DrawSetting.FogMode == Mode ) return 0;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GSYS.DrawSetting.FogMode = Mode ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetFogMode_PF( Mode ) ;
	}

	// �I��
	return 0 ;
}

// �t�H�O���[�h���擾����
extern int NS_GetFogMode( void )
{
	return GSYS.DrawSetting.FogMode ;
}

// �t�H�O�J���[��ύX����
extern	int NS_SetFogColor( int r, int g, int b )
{
	DWORD color = ( DWORD )( ( ( 0xff ) << 24 ) | ( ( r & 0xff ) << 16 ) | ( ( g & 0xff ) << 8 ) | ( b & 0xff ) ) ;

	if( GSYS.DrawSetting.FogColor == color ) return 0;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GSYS.DrawSetting.FogColor = color ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetFogColor_PF( color ) ;
	}

	// �I��
	return 0 ;
}

// �t�H�O�J���[���擾����
extern	int NS_GetFogColor( int *r, int *g, int *b )
{
	if( r != NULL )
	{
		*r = ( int )( ( GSYS.DrawSetting.FogColor >> 16 ) & 0xff ) ;
	}

	if( g != NULL )
	{
		*g = ( int )( ( GSYS.DrawSetting.FogColor >> 8  ) & 0xff ) ;
	}

	if( b != NULL )
	{
		*b = ( int )( ( GSYS.DrawSetting.FogColor       ) & 0xff ) ;
	}

	return 0 ;
}

// �t�H�O���n�܂鋗���ƏI�����鋗����ݒ肷��( 0.0f �` 1.0f )
extern	int	NS_SetFogStartEnd( float start, float end )
{
	if( GSYS.DrawSetting.FogStart == start && GSYS.DrawSetting.FogEnd == end ) return 0 ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GSYS.DrawSetting.FogStart = start ;
	GSYS.DrawSetting.FogEnd = end ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetFogStartEnd_PF( start, end ) ;
	}

	// �I��
	return 0 ;
}

// �t�H�O���n�܂鋗���ƏI�����鋗�����擾����( 0.0f �` 1.0f )
extern	int NS_GetFogStartEnd( float *start, float *end )
{
	if( start != NULL )
	{
		*start = GSYS.DrawSetting.FogStart ;
	}

	if( end != NULL )
	{
		*end = GSYS.DrawSetting.FogEnd ;
	}

	return 0 ;
}

// �t�H�O�̖��x��ݒ肷��( 0.0f �` 1.0f )
extern	int	NS_SetFogDensity( float density )
{
	if( GSYS.DrawSetting.FogDensity == density ) return 0;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	GSYS.DrawSetting.FogDensity = density ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetFogDensity_PF( density ) ;
	}

	// �I��
	return 0 ;
}

// �t�H�O�̖��x���擾����( 0.0f �` 1.0f )
extern float NS_GetFogDensity( void )
{
	return GSYS.DrawSetting.FogDensity ;
}


























// ��ʊ֌W�֐�

// �w����W�̐F���擾����
extern unsigned int NS_GetPixel( int x, int y )
{
	unsigned int Ret = 0xffffffff ;

	// �`����W�`�F�b�N
	if( x < 0 || y < 0 || x >= GSYS.DrawSetting.DrawSizeX || y >= GSYS.DrawSetting.DrawSizeY ) return 0xffffffff ;

	// �n�[�h�E�G�A�@�\���g�p���邩�ǂ����ŏ����𕪊�
	if( GSYS.Setting.ValidHardware )
	{
		// �n�[�h�E�F�A���g�p����ꍇ

		Ret = Graphics_Hardware_GetPixel_PF( x, y ) ;
	}
	else
	{
		MEMIMG    *Img ;
		IMAGEDATA *Image ;

		// �n�[�h�E�F�A���g�p���Ȃ��ꍇ

		// �G���[����
		if( GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], Image ) )
			Image = NULL ;

		// �`���ɂ���ď����𕪊�
		if( Image )
		{
			Img = &Image->Soft.MemImg ;
		}
		else
		{
			Img = &GSYS.SoftRender.MainBufferMemImg ;
		}

		switch( GSYS.Screen.MainScreenColorBitDepth )
		{
		case 16 : Ret = *( (  WORD * )( ( char * )Img->UseImage + 2 * x + Img->Base->Pitch * y ) ) ; break ;
		case 32 : Ret = *( ( DWORD * )( ( char * )Img->UseImage + 4 * x + Img->Base->Pitch * y ) ) ; break ;
		}
	}

	// �I��
	return Ret ;
}

// ���C���E�C���h�E�̃o�b�N�O���E���h�J���[��ݒ肷��
extern int NS_SetBackgroundColor( int Red, int Green, int Blue )
{
	// �F�̕ۑ�
	GSYS.Screen.BackgroundRed   = Red ;
	GSYS.Screen.BackgroundGreen = Green ;
	GSYS.Screen.BackgroundBlue  = Blue ;

	// �o�b�N�O���E���h�J���[���L�����ǂ����̃t���O�𗧂Ă�
	GSYS.Screen.EnableBackgroundColor = TRUE ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetBackgroundColor_PF( Red, Green, Blue ) ;
	}

	// �I��
	return 0;
}

// ���C���E�C���h�E�̃o�b�N�O���E���h�J���[���擾����
extern int NS_GetBackgroundColor( int *Red, int *Green, int *Blue )
{
	if( Red != NULL )
	{
		*Red = GSYS.Screen.BackgroundRed ;
	}

	if( Green != NULL )
	{
		*Green = GSYS.Screen.BackgroundGreen ;
	}

	if( Blue != NULL )
	{
		*Blue = GSYS.Screen.BackgroundBlue ;
	}

	// �I��
	return 0;
}

// �A�N�e�B�u�ɂȂ��Ă����ʂ���w��̈�̃O���t�B�b�N���擾����
extern	int NS_GetDrawScreenGraph( int x1, int y1, int x2, int y2, int GrHandle, int /*UseClientFlag*/ )
{
	IMAGEDATA *Image ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

	// ��荞�݃T�C�Y�Ɖ摜�T�C�Y���Ⴄ�ꍇ�̓G���[
	if( Image->WidthI != x2 - x1 || Image->HeightI != y2 - y1 ) return -1 ;

	return Graphics_Image_GetDrawScreenGraphBase( GSYS.DrawSetting.TargetScreen[ 0 ], GSYS.DrawSetting.TargetScreenSurface[ 0 ], x1, y1, x2, y2, 0, 0, GrHandle ) ;
}

// �`��\�摜����w��̈�̃O���t�B�b�N���擾����
extern int NS_BltDrawValidGraph( int TargetDrawValidGrHandle, int x1, int y1, int x2, int y2, int DestX, int DestY, int DestGrHandle )
{
	IMAGEDATA *Image ;

	// �G���[����
	if( GRAPHCHK( TargetDrawValidGrHandle, Image ) )
		return -1 ;

	return Graphics_Image_GetDrawScreenGraphBase( TargetDrawValidGrHandle, 0, x1, y1, x2, y2, DestX, DestY, DestGrHandle ) ;
}

// ����ʂƕ\��ʂ���������
extern int NS_ScreenFlip( void )
{
	return Graphics_Screen_FlipBase() ;
}

// ScreenCopy �̃x�[�X�֐�
extern int Graphics_Screen_ScreenCopyBase( int DrawTargetFrontScreenMode_Copy )
{
	if( GSYS.Setting.ValidHardware == TRUE )
	{
		Graphics_Hardware_ScreenCopy_PF( DrawTargetFrontScreenMode_Copy ) ;
	}

	// �t���b�v
	return Graphics_Screen_FlipBase() ;
}

// ����ʂ̓��e��\��ʂɕ`�悷��
extern int NS_ScreenCopy( void )
{
	return Graphics_Screen_ScreenCopyBase( FALSE ) ;
}

// ���������M����҂�
extern	int		NS_WaitVSync( int SyncNum )
{
	return Graphics_Hardware_WaitVSync_PF( SyncNum ) ;
}

// ��ʂ̏�Ԃ�����������
extern	int NS_ClearDrawScreen( const RECT *ClearRect )
{
//	HRESULT hr ;
//	RECT Rect ;

	// �t���X�N���[���}�X�N�������s���Ă���ꍇ�̓}�X�N�`��̌��ʂ𔽉f������
	MASK_FULLSCREEN_MASKUPDATE

	// �`�F�b�N
	if( ClearRect && ( ClearRect->left < 0 || ClearRect->top < 0 ) )
		ClearRect = NULL ;

	// �n�[�h�E�G�A�@�\���g���Ă��邩�ǂ����ŏ����𕪊�
	if( GSYS.Setting.ValidHardware )
	{
		// �n�[�h�E�G�A�̋@�\���g���Ă���ꍇ
		Graphics_Hardware_ClearDrawScreen_PF( ClearRect ) ;
	}
	else
	{
		IMAGEDATA *Image = NULL ;
		MEMIMG    *ClearTarget ;

		// �\�t�g�E�G�A�����_�����O�̏ꍇ

		// �`���摜�̉摜�����擾
		GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], Image ) ;

		// �`��\�摜�̏ꍇ�͂��̉摜���N���A
		ClearTarget = Image ? &Image->Soft.MemImg : &GSYS.SoftRender.MainBufferMemImg ;
		ClearMemImg( ClearTarget, ClearRect, NS_GetColor3( ClearTarget->Base->ColorDataP, GSYS.Screen.BackgroundRed, GSYS.Screen.BackgroundGreen, GSYS.Screen.BackgroundBlue, 0 ) ) ;
	}

	// �I��
	return 0 ;
}

// ��ʂ̂y�o�b�t�@�̏�Ԃ�����������
extern int NS_ClearDrawScreenZBuffer( const RECT *ClearRect )
{
	// �`�F�b�N
	if( ClearRect && ( ClearRect->left < 0 || ClearRect->top < 0 ) )
		ClearRect = NULL ;

	// �n�[�h�E�G�A�@�\���g���Ă��邩�ǂ����ŏ����𕪊�
	if( GSYS.Setting.ValidHardware )
	{
		// �n�[�h�E�G�A�̋@�\���g���Ă���ꍇ
		Graphics_Hardware_ClearDrawScreenZBuffer_PF( ClearRect ) ;
	}
	else
	{
		// �\�t�g�E�G�A�����_�����O�̏ꍇ�͉������Ȃ�
	}

	// �I��
	return 0 ;
}

// ��ʂ̏�Ԃ�����������(ClearDrawScreen�̋�����)
extern int NS_ClsDrawScreen( void )
{
	return NS_ClearDrawScreen() ;
}

// �`����ʂ̃Z�b�g
extern	int NS_SetDrawScreen( int DrawScreen )
{
	int OldScreen ;
	int OldScreenSurface ;
#ifndef DX_NON_MASK
	int MaskUseFlag ;
#endif
//	int Result ;
	IMAGEDATA     *Image        = NULL ;
	IMAGEDATA     *OldImage     = NULL ;
	SHADOWMAPDATA *ShadowMap    = NULL ;
	SHADOWMAPDATA *OldShadowMap = NULL ;
	
	if( GSYS.InitializeFlag == FALSE ) return -1 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �O���t�B�b�N�`��悪���܂łƓ����������ꍇ���������I��
//	if( DrawScreen == GSYS.DrawSetting.TargetScreen[ 0 ] && GSYS.DrawSetting.TargetScreenSurface == 0 ) return 0 ;

	// �摜���̎擾
	if( GSYS.DrawSetting.ShadowMapDrawSetupRequest )
	{
		// �A�h���X�̎擾
		if( SHADOWMAPCHK( DrawScreen, ShadowMap ) )
			return -1 ;
	}
	else
	{
		Image = NULL ;
		if( !GRAPHCHKFULL( DrawScreen, Image ) )
		{
			// �摜�������ꍇ�͕`��\�ł͖����ꍇ�̓G���[
			if( Image->Orig->FormatDesc.DrawValidFlag == FALSE )
				return -1 ;
		}
		else
		{
			if( Image != NULL )
			{
				return -1 ;
			}
		}
	}

	// ���܂ł̕`���̉摜���̎擾
	if( GRAPHCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], OldImage ) )
	{
		OldImage = NULL ;
		if( SHADOWMAPCHKFULL( GSYS.DrawSetting.TargetScreen[ 0 ], OldShadowMap ) )
		{
			OldShadowMap = NULL ;
		}
	}

	// �w�肵���X�N���[���������������牽�������I��
	if( DrawScreen != DX_SCREEN_FRONT && DrawScreen != DX_SCREEN_BACK &&
		DrawScreen != DX_SCREEN_WORK  && DrawScreen != DX_SCREEN_TEMPFRONT &&
		ShadowMap == NULL &&
		( ( DrawScreen & DX_HANDLEERROR_MASK ) != 0                   ||
		  ( DrawScreen & DX_HANDLETYPE_MASK  ) != DX_HANDLETYPE_MASK_GRAPH || 
		  GetHandleInfo( DrawScreen ) == NULL ) )
	{
		return -1 ;
	}

	// ���܂ł̉�ʂ��\��ʂ������ꍇ�͈�x ScreenCopy ���s��
	if( GSYS.DrawSetting.TargetScreen[ 0 ] == DX_SCREEN_FRONT )
	{
		Graphics_Screen_ScreenCopyBase( TRUE ) ;
	}

	// �V���h�E�}�b�v�ւ̕`��ݒ肪����Ă��������
	if( GSYS.DrawSetting.ShadowMapDraw )
	{
		NS_ShadowMap_DrawEnd() ;
	}

#ifndef DX_NON_MASK
	// �}�X�N���g�p�I������葱�������
	MaskUseFlag = MASKD.MaskUseFlag ;
	NS_SetUseMaskScreenFlag( FALSE ) ;

	// �`���̃T�C�Y�Ń}�X�N��ʂ��쐬���Ȃ���
	if( GSYS.DrawSetting.ShadowMapDrawSetupRequest == FALSE && MASKD.CreateMaskFlag )
	{
		int w, h ;
		NS_GetGraphSize( DrawScreen, &w, &h ) ;
		Mask_CreateScreenFunction( TRUE, w, h ) ;
	}
#endif

	// �`����ۑ�
	OldScreen                                 = GSYS.DrawSetting.TargetScreen[ 0 ] ;
	OldScreenSurface                          = GSYS.DrawSetting.TargetScreenSurface[ 0 ] ;
	GSYS.DrawSetting.TargetScreen[ 0 ]        = DrawScreen ;
	GSYS.DrawSetting.TargetScreenSurface[ 0 ] = 0 ;

	// �`��\�T�C�Y�̎擾
	NS_GetGraphSize( DrawScreen, &GSYS.DrawSetting.DrawSizeX, &GSYS.DrawSetting.DrawSizeY ) ;

	// �`���ɐ��������l���������ނ��ǂ����̃t���O���X�V����
	Graphics_DrawSetting_RefreshAlphaChDrawMode() ;

	// �R�c�`��֌W�̕`�����Z�b�g
	if( GSYS.Setting.ValidHardware )
	{
		// �n�[�h�E�G�A�A�N�Z�����[�^���g�p����ꍇ�̏���
		Graphics_Hardware_SetDrawScreen_PF( DrawScreen, OldScreenSurface, Image, OldImage, ShadowMap, OldShadowMap ) ;
	}
	else
	{
		// �\�t�g�E�G�A�����_�����O�̏ꍇ�̏���

		// �`���̌���
#ifndef DX_NON_MASK
		if( MASKD.MaskValidFlag )
		{
			// �}�X�N���L���ȏꍇ�̓}�X�N��`���ɂ���
			GSYS.SoftRender.TargetMemImg = &MASKD.MaskDrawMemImg ;
		}
		else
#endif
		{
			// �摜���`��Ώۂ��ǂ����ŏ����𕪊�
			if( Image != NULL )
			{
				// �A���t�@�`�����l���t���̉摜�̏ꍇ�͕`��Ώۂɂł��Ȃ�
				if( Image->Soft.MemImg.Base->UseAlpha == TRUE )
					return -1 ;

				// �`��\�摜��`����
				GSYS.SoftRender.TargetMemImg = &Image->Soft.MemImg ;
			}
			else
			{
				// ���C���o�b�t�@��`����
				GSYS.SoftRender.TargetMemImg = &GSYS.SoftRender.MainBufferMemImg ;
			}
		}
	}

	// �Q�c�`��p�̍s����쐬
//	{
//		CreateViewportMatrix(
//			&GSYS.DrawSetting.ViewportMatrix2D,
//			GSYS.DrawSetting.DrawSizeX / 2.0f,
//			GSYS.DrawSetting.DrawSizeY / 2.0f, 
//			( float )GSYS.DrawSetting.DrawSizeX,
//			( float )GSYS.DrawSetting.DrawSizeY
//		) ;
//
//		CreateIdentityMatrix( &GSYS.DrawSetting.ProjectionMatrix2D ) ;
//		GSYS.DrawSetting.ProjectionMatrix2D.m[ 0 ][ 0 ] =  1.0f / ( ( float )GSYS.DrawSetting.DrawSizeX / 2.0f ) ;
//		GSYS.DrawSetting.ProjectionMatrix2D.m[ 1 ][ 1 ] = -1.0f / ( ( float )GSYS.DrawSetting.DrawSizeY / 2.0f ) ;
//		GSYS.DrawSetting.ProjectionMatrix2D.m[ 3 ][ 0 ] = -1.0f ;
//		GSYS.DrawSetting.ProjectionMatrix2D.m[ 3 ][ 1 ] =  1.0f ;
//	}

	// �`��̈���X�V
	{
		RECT SrcRect ;
		int NewWidth, NewHeight, OldWidth, OldHeight ;

		NS_GetGraphSize( DrawScreen, &NewWidth, &NewHeight ) ;
		NS_GetGraphSize( OldScreen,  &OldWidth, &OldHeight ) ;

		SrcRect = GSYS.DrawSetting.DrawArea ;

		if( GSYS.DrawSetting.SetDrawScreenNoSettingResetFlag )
		{
			if( GSYS.DrawSetting.ShadowMapDrawSetupRequest )
			{
				RECT OriginalDrawRect = GSYS.DrawSetting.OriginalDrawRect ;

				NS_SetDrawArea( 0, 0, NewWidth, NewHeight ) ;

				GSYS.DrawSetting.OriginalDrawRect = OriginalDrawRect ;
			}
			else
			{
				NS_SetDrawArea(
					GSYS.DrawSetting.OriginalDrawRect.left,
					GSYS.DrawSetting.OriginalDrawRect.top,
					GSYS.DrawSetting.OriginalDrawRect.right,
					GSYS.DrawSetting.OriginalDrawRect.bottom
				) ;
			}
		}
		else
		{
			if( GSYS.DrawSetting.ShadowMapDrawSetupRequest || ( NewWidth != OldWidth ) || ( NewHeight != OldHeight ) )
			{
				NS_SetDrawArea( 0, 0, NewWidth, NewHeight ) ;
			}
			else
			{
				NS_SetDrawArea( SrcRect.left, SrcRect.top, SrcRect.right, SrcRect.bottom ) ;
			}
		}
	}
		
	// �R�c�`�揈���p�̍s��ݒ�̏��������s��
	if( GSYS.DrawSetting.SetDrawScreenNoSettingResetFlag == FALSE )
	{
		MATRIX mat ;
		float D ;

		// �h�b�g�A�X�y�N�g���P�D�O�ɖ߂�
		NS_SetCameraDotAspect( 1.0f ) ;

		// ���[���h�ϊ��s��͒P�ʍs��
		CreateIdentityMatrix( &mat ) ;
		NS_SetTransformToWorld( &mat ) ;

		// ���C�u�����Ǘ��̃r���[�|�[�g�s����X�V
		GSYS.Camera.ScreenCenterX = ( double )GSYS.DrawSetting.DrawSizeX / 2.0 ;
		GSYS.Camera.ScreenCenterY = ( double )GSYS.DrawSetting.DrawSizeY / 2.0 ;
		NS_SetCameraScreenCenterD( GSYS.Camera.ScreenCenterX, GSYS.Camera.ScreenCenterY ) ;

		// �r���[�s��� z = 0.0 �̎��ɒ��x�X�N���[���S�̂��ʂ�ʒu�ƕ��������J������
		{
			VECTOR up, at, eye ;

			D = (float)( ( GSYS.DrawSetting.DrawSizeY / 2 ) / DEFAULT_TAN_FOV_HALF ) ;

			eye.x = (float)GSYS.DrawSetting.DrawSizeX / 2 ;
			eye.y = (float)GSYS.DrawSetting.DrawSizeY / 2 ;
			eye.z = -D;
			
			at.x = eye.x ;
			at.y = eye.y ;
			at.z = eye.z + 1.0F ;
			
			up.x = 0.0f ;
			up.y = 1.0F ;
			up.z = 0.0f ;
			
			NS_SetCameraPositionAndTargetAndUpVec( eye, at, up ) ;
//				CreateLookAtMatrix( &mat, &eye, &at, &up ) ;
//				NS_SetTransformToView( &mat ) ;
		}

		// �v���W�F�N�V�����s��͕��ʂ�
		NS_SetupCamera_Perspective( DEFAULT_FOV ) ;
		NS_SetCameraNearFar( D * 0.1f + DEFAULT_NEAR, D + DEFAULT_FAR ) ;
	}

#ifndef DX_NON_MASK
	// �}�X�N���g�p����葱�������
	NS_SetUseMaskScreenFlag( MaskUseFlag ) ;
#endif

	// �V���h�E�}�b�v�ɑ΂���`�揀�����N�G�X�g�������ꍇ�̓��N�G�X�g�t���O��|���A�`�揀����Ԃ�ۑ�����
	if( GSYS.DrawSetting.ShadowMapDrawSetupRequest )
	{
		GSYS.DrawSetting.ShadowMapDrawSetupRequest = FALSE ;

		// �V���h�E�}�b�v�ɑ΂���`��ł��邩�ǂ����̃t���O�𗧂Ă�
		GSYS.DrawSetting.ShadowMapDraw = TRUE ;

		// �V���h�E�}�b�v�ɑ΂���`��̏ꍇ�́A�ΏۂƂȂ�V���h�E�}�b�v�̃n���h����ۑ�
		GSYS.DrawSetting.ShadowMapDrawHandle = DrawScreen ;
	}

	// SetDrawScreen �̌�ɍs�����ˑ����������s����
	Graphics_Hardware_SetDrawScreen_Post_PF( DrawScreen ) ;

	// �I��
	return 0 ;
}

// �`����ʂ̎擾
extern int NS_GetDrawScreen( void )
{
	return GSYS.DrawSetting.TargetScreen[ 0 ] ;
}

// �A�N�e�B�u�ɂȂ��Ă���O���t�B�b�N�̃n���h���𓾂�
extern int NS_GetActiveGraph( void )
{
	return NS_GetDrawScreen() ;
}

// SetDrawScreen �����s�����ۂɃJ������`��͈͂̐ݒ�����Z�b�g���邩��ݒ肷��( UseFlag  TRUE:���Z�b�g����( �f�t�H���g )  FALSE:���Z�b�g���Ȃ� )
extern	int NS_SetUseSetDrawScreenSettingReset( int UseFlag )
{
	GSYS.DrawSetting.SetDrawScreenNoSettingResetFlag = UseFlag ? FALSE : TRUE ;

	// ����I��
	return 0 ;
}

// SetDrawScreen �����s�����ۂɃJ������`��͈͂̐ݒ�����Z�b�g���邩���擾����
extern	int NS_GetUseSetDrawScreenSettingReset( void )
{
	return GSYS.DrawSetting.SetDrawScreenNoSettingResetFlag ? FALSE : TRUE ;
}

// �`���y�o�b�t�@�̃Z�b�g
extern	int	NS_SetDrawZBuffer( int DrawScreen )
{
	IMAGEDATA *Image ;
	
	if( GSYS.InitializeFlag == FALSE ) return -1 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �摜���̎擾
	Image = NULL ;
	GRAPHCHKFULL( DrawScreen, Image ) ;

	// ���ˑ�����
	if( Graphics_Hardware_SetDrawZBuffer_PF( DrawScreen, Image ) < 0 )
	{
		return -1 ;
	}

	// �`���y�o�b�t�@�̃n���h����ۑ�
	GSYS.DrawSetting.TargetZBuffer = DrawScreen ;

	// �`���y�o�b�t�@���X�V
	Graphics_Screen_SetupUseZBuffer() ;

	// �I��
	return 0 ;
}

// ��ʃ��[�h�̃Z�b�g
extern int NS_SetGraphMode( int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, int RefreshRate )
{
	int Width ;
	int Height ;
	int CDepth ;
	int RRate ;
//	int MaxScreenSizeX ;
//	int MaxScreenSizeY ;

	if( ColorBitDepth == 0 )
	{
		ColorBitDepth = DEFAULT_COLOR_BITDEPTH ;
	}

	// �R�Q�O�~�Q�S�O�̉�ʂ��G�~�����[�V��������t���O�������Ă���ꍇ�͋����I�ɉ𑜓x���R�Q�O�~�Q�S�O�ɂ���
	if( GSYS.Screen.Emulation320x240Flag )
	{
		ScreenSizeX = 320 ;
		ScreenSizeY = 240 ;
	}

	// �G���[�`�F�b�N
	if( ScreenSizeX <= 0 ||
		ScreenSizeY <= 0 ||
		( ColorBitDepth != 0 && ColorBitDepth != 8 && ColorBitDepth != 16 && ColorBitDepth != 32 ) )
	{
		return DX_CHANGESCREEN_RETURN ;
	}

	// ���j�^���Ή����Ă���𑜓x�ł͖����A�����j�^���Ή����Ă���ő�𑜓x�����傫���𑜓x���w�肳�ꂽ�ꍇ�̓G���[
//	if( Graphics_Screen_CheckDisplaySetting( ScreenSizeX, ScreenSizeY, ColorBitDepth ) == FALSE )
//	{
//		NS_GetDisplayMaxResolution( &MaxScreenSizeX, &MaxScreenSizeY ) ;
//		if( MaxScreenSizeX < ScreenSizeX || MaxScreenSizeY < ScreenSizeY )
//		{
//			return -1 ;
//		}
//	}

	// �������O�̏ꍇ�͐ݒ�l�����ۑ�����
	if( DxSysData.DxLib_InitializeFlag == FALSE )
	{
		Graphics_Screen_SetMainScreenSize( ScreenSizeX, ScreenSizeY ) ;
		GSYS.Screen.MainScreenColorBitDepth = ColorBitDepth ;
		GSYS.Screen.MainScreenRefreshRate   = RefreshRate ;
		SetMemImgDefaultColorType( ColorBitDepth == 32 ? 1 : 0 ) ;
		return 0 ;
	}

	// ���̒l������Ă���
	Width	= GSYS.Screen.MainScreenSizeX ;
	Height	= GSYS.Screen.MainScreenSizeX ;
	CDepth	= GSYS.Screen.MainScreenColorBitDepth ;
	RRate	= GSYS.Screen.MainScreenRefreshRate ;

	// ��ʃ��[�h��ύX
	Graphics_Screen_ChangeMode( ScreenSizeX, ScreenSizeY, ColorBitDepth, FALSE, RefreshRate ) ;

	// �߂�l�̃Z�b�g
	if( ScreenSizeX == GSYS.Screen.MainScreenSizeX &&
		ScreenSizeY == GSYS.Screen.MainScreenSizeY 
#ifdef __WINDOWS__
		&&
		(
		  NS_GetWindowModeFlag() == FALSE ||
		  (
		    NS_GetWindowModeFlag() == TRUE &&
		    ColorBitDepth	== GSYS.Screen.MainScreenColorBitDepth &&
		    RefreshRate		== GSYS.Screen.MainScreenRefreshRate
		  )
		)
#endif // __WINDOWS__
	  )
	{
		return DX_CHANGESCREEN_OK ;
	}
	else
	{
		if( Width	== GSYS.Screen.MainScreenSizeX &&
			Height	== GSYS.Screen.MainScreenSizeY &&
			CDepth	== GSYS.Screen.MainScreenColorBitDepth &&
			RRate	== GSYS.Screen.MainScreenRefreshRate )
		{
			return DX_CHANGESCREEN_RETURN ;
		}
		else
		{
			if( GSYS.Screen.MainScreenSizeX == DEFAULT_SCREEN_SIZE_X && 
				GSYS.Screen.MainScreenSizeY == DEFAULT_SCREEN_SIZE_Y 
#ifdef __WINDOWS__
				&& 
				(
				  NS_GetWindowModeFlag() == FALSE ||
				  (
					NS_GetWindowModeFlag() == TRUE &&
				    GSYS.Screen.MainScreenColorBitDepth == DEFAULT_COLOR_BITDEPTH &&
				    GSYS.Screen.MainScreenRefreshRate   == 0
				  )
				)
#endif // __WINDOWS__
			  )
			{
				return DX_CHANGESCREEN_DEFAULT ;
			}
			else
			{
				return -1 ;
			}
		}
	}
}

// �t���X�N���[���𑜓x���[�h��ݒ肷��
extern int NS_SetFullScreenResolutionMode( int ResolutionMode /* DX_FSRESOLUTIONMODE_NATIVE �� */ )
{
	// ������̏ꍇ�͉��������I��
	if( DxSysData.DxLib_InitializeFlag == TRUE )
	{
		return -1 ;
	}

	// �t���X�N���[���𑜓x���[�h��ۑ�����
	GSYS.Screen.FullScreenResolutionMode    = ResolutionMode ;
	GSYS.Screen.FullScreenResolutionModeAct = ResolutionMode ;

	// �I��
	return 0 ;
}

// �t���X�N���[�����[�h���̉�ʊg�僂�[�h��ݒ肷��
extern int NS_SetFullScreenScalingMode( int ScalingMode /* DX_FSSCALINGMODE_NEAREST �� */ )
{
	// �t���X�N���[���X�P�[�����O���[�h��ۑ�����
	GSYS.Screen.FullScreenScalingMode = ScalingMode ;

	// �I��
	return 0 ;
}

// �U�S�O���S�W�O�̉�ʂłR�Q�O���Q�S�O�̉�ʉ𑜓x�ɂ��邩�ǂ����̃t���O���Z�b�g����A�U�S�O���S�W�O�ȊO�̉𑜓x�ł͖���( TRUE:�L��  FALSE:���� )
extern int NS_SetEmulation320x240( int Flag )
{
	// ������̏ꍇ�͉��������I��
	if( DxSysData.DxLib_InitializeFlag == TRUE )
	{
		return -1 ;
	}

	// �t���O��ۑ�
	GSYS.Screen.Emulation320x240Flag = Flag;

	// �t���O�� TRUE �̏ꍇ�͉�ʃ��[�h�� 320x240 �ɐݒ肷��
	if( Flag != FALSE )
	{
		NS_SetGraphMode( 320, 240, GSYS.Screen.MainScreenColorBitDepth, GSYS.Screen.MainScreenRefreshRate ) ;
	}

	// �I��
	return 0;
}

// ��ʗp�̂y�o�b�t�@�̃T�C�Y��ݒ肷��
extern int NS_SetZBufferSize( int ZBufferSizeX, int ZBufferSizeY )
{
	return Graphics_Screen_SetZBufferMode( ZBufferSizeX, ZBufferSizeY, -1 ) ;
}

// ��ʗp�̂y�o�b�t�@�̃r�b�g�[�x��ݒ肷��( 16 or 24 or 32 )
extern int NS_SetZBufferBitDepth( int BitDepth )
{
	return Graphics_Screen_SetZBufferMode( -1, -1, BitDepth ) ;
}

// �u�r�x�m�b�҂������邩�̃t���O�Z�b�g
extern int NS_SetWaitVSyncFlag( int Flag )
{
	// ������̏ꍇ�͉��������I��
	if( DxSysData.DxLib_InitializeFlag == TRUE )
	{
		return -1 ;
	}

	GSYS.Screen.NotWaitVSyncFlag = !Flag ;

//	if( DxSysData.DxLib_InitializeFlag == FALSE )
//	{
//		GSYS.Screen.PreSetWaitVSyncFlag = Flag ;
//	}

	return 0 ;
}

// �u�r�x�m�b�҂�������ݒ�ɂȂ��Ă��邩�ǂ������擾����
extern int NS_GetWaitVSyncFlag( void )
{
	return !GSYS.Screen.NotWaitVSyncFlag ;
}

// ��ʂ̃t���X�N���[���A���`�G�C���A�X���[�h�̐ݒ������
extern int NS_SetFullSceneAntiAliasingMode( int Samples, int Quality )
{
	// �ݒ�ł���̂͏������O�̂�
	if( DxSysData.DxLib_InitializeFlag == TRUE )
		return 0 ;

	GSYS.Setting.FSAAMultiSampleCount   = Samples ;
	GSYS.Setting.FSAAMultiSampleQuality = Quality ;

	// �I��
	return 0 ;
}

// ScreenFlip ���ɕ\��ʑS�̂ɓ]�����闠��ʂ̗̈��ݒ肷��( DxLib_Init �̑O�ł̂ݎg�p�\ )
extern int NS_SetGraphDisplayArea( int x1, int y1, int x2, int y2 )
{
	// ������̏ꍇ�͉��������I��
	if( DxSysData.DxLib_InitializeFlag == TRUE )
		return -1 ;

	// �p�����[�^�`�F�b�N
	if( x1 >= x2 || y1 >= y2 )
	{
		GSYS.Screen.ValidGraphDisplayArea = FALSE ;
		return -1 ;
	}

	// �p�����[�^�ۑ�
	GSYS.Screen.GraphDisplayArea.left   = x1 ;
	GSYS.Screen.GraphDisplayArea.right  = x2 ;
	GSYS.Screen.GraphDisplayArea.top    = y1 ;
	GSYS.Screen.GraphDisplayArea.bottom = y2 ;

	GSYS.Screen.ValidGraphDisplayArea   = TRUE ;

	// �I��
	return 0 ;
}

// ��ʃ��[�h�ύX��( �ƃE�C���h�E���[�h�ύX�� )�ɃO���t�B�b�N�X�V�X�e���̐ݒ��O���t�B�b�N�n���h�������Z�b�g���邩�ǂ�����ݒ肷��( TRUE:���Z�b�g����  FALSE:���Z�b�g���Ȃ� )
extern int NS_SetChangeScreenModeGraphicsSystemResetFlag( int Flag )
{
	GSYS.Setting.ChangeScreenModeNotGraphicsSystemFlag = Flag ? FALSE : TRUE ;

	return 0 ;
}

// ���݂̉�ʂ̑傫���ƃJ���[�r�b�g���𓾂� 
extern	int NS_GetScreenState( int *SizeX, int *SizeY, int *ColorBitDepth )
{
	if( SizeX ) *SizeX = GSYS.Screen.MainScreenSizeX ;
	if( SizeY ) *SizeY = GSYS.Screen.MainScreenSizeY ;

	if( ColorBitDepth ) *ColorBitDepth = GSYS.Screen.MainScreenColorBitDepth ;

	// �I��
	return 0 ;
}

// �`��T�C�Y���擾����
extern int NS_GetDrawScreenSize( int *XBuf, int *YBuf )
{
	if( XBuf ) *XBuf = GSYS.Screen.MainScreenSizeX_Result == 0 ? DEFAULT_SCREEN_SIZE_X : GSYS.Screen.MainScreenSizeX_Result ;
	if( YBuf ) *YBuf = GSYS.Screen.MainScreenSizeY_Result == 0 ? DEFAULT_SCREEN_SIZE_Y : GSYS.Screen.MainScreenSizeY_Result ;

	// �I��
	return 0 ;
}

// �g�p�F�r�b�g����Ԃ�
extern int NS_GetScreenBitDepth( void )
{
	// �I��
	return NS_GetColorBitDepth() ;
}

// ��ʂ̐F�r�b�g�[�x�𓾂�
extern int NS_GetColorBitDepth( void )
{
	// �I��
	return GSYS.Screen.MainScreenColorBitDepth == 0 ? DEFAULT_COLOR_BITDEPTH : GSYS.Screen.MainScreenColorBitDepth ;
}

// ��ʃ��[�h���ύX����Ă��邩�ǂ����̃t���O���擾����
extern	int						NS_GetChangeDisplayFlag( void )
{
#ifdef __WINDOWS__
	// �I��
	return NS_GetWindowModeFlag() == FALSE ;
#else // __WINDOWS__
	return FALSE ;
#endif // __WINDOWS__
}

// �r�f�I�������̗e�ʂ𓾂�
extern	int						NS_GetVideoMemorySize( int *AllSize, int *FreeSize )
{
	return Graphics_Hardware_GetVideoMemorySize_PF( AllSize, FreeSize ) ;
}

// ���݂̉�ʂ̃��t���b�V�����[�g���擾����
extern int NS_GetRefreshRate( void )
{
	return Graphics_GetRefreshRate_PF() ;
}

// �f�B�X�v���C�̐����擾
extern int NS_GetDisplayNum( void )
{
	// �f�B�X�v���C���̃Z�b�g�A�b�v���s���Ă��Ȃ��ꍇ�̓Z�b�g�A�b�v������
	if( GSYS.Screen.DisplayInfo == NULL )
	{
		if( Graphics_SetupDisplayInfo_PF() < 0 )
		{
			return -1 ;
		}
	}

	return GSYS.Screen.DisplayNum ;
}

// �ύX�\�ȃf�B�X�v���C���[�h�̐����擾����
extern int NS_GetDisplayModeNum( int DisplayIndex )
{
	// �f�B�X�v���C���̃Z�b�g�A�b�v���s���Ă��Ȃ��ꍇ�̓Z�b�g�A�b�v������
	if( GSYS.Screen.DisplayInfo == NULL )
	{
		if( Graphics_SetupDisplayInfo_PF() < 0 )
		{
			return -1 ;
		}
	}

	if( DisplayIndex < 0 || DisplayIndex >= GSYS.Screen.DisplayNum )
	{
		return -1 ;
	}

	return GSYS.Screen.DisplayInfo[ DisplayIndex ].ModeNum ;
}

// �ύX�\�ȃf�B�X�v���C���[�h�̏����擾����( ModeIndex �� 0 �` GetDisplayModeNum �̖߂�l-1 )
extern DISPLAYMODEDATA NS_GetDisplayMode( int ModeIndex, int DisplayIndex )
{
	static DISPLAYMODEDATA ErrorResult = { -1, -1, -1, -1 } ;

	// �f�B�X�v���C���̃Z�b�g�A�b�v���s���Ă��Ȃ��ꍇ�̓Z�b�g�A�b�v������
	if( GSYS.Screen.DisplayInfo == NULL )
	{
		if( Graphics_SetupDisplayInfo_PF() < 0 )
		{
			return ErrorResult ;
		}
	}

	if( DisplayIndex < 0 || DisplayIndex >= GSYS.Screen.DisplayNum )
	{
		return ErrorResult ;
	}

	if( GSYS.Screen.DisplayInfo[ DisplayIndex ].ModeNum <= ModeIndex || ModeIndex < 0 )
	{
		return ErrorResult ;
	}

	return GSYS.Screen.DisplayInfo[ DisplayIndex ].ModeData[ ModeIndex ] ;
}

// �f�B�X�v���C�̍ő�𑜓x���擾����
extern int NS_GetDisplayMaxResolution( int *SizeX, int *SizeY, int DisplayIndex )
{
	int MaxSizeX ;
	int MaxSizeY ;
	DISPLAYMODEDATA *DisplayModeData ;
	int i ;

	// �f�B�X�v���C���̃Z�b�g�A�b�v���s���Ă��Ȃ��ꍇ�̓Z�b�g�A�b�v������
	if( GSYS.Screen.DisplayInfo == NULL )
	{
		if( Graphics_SetupDisplayInfo_PF() < 0 )
		{
			return -1 ;
		}
	}

	if( DisplayIndex < 0 || DisplayIndex >= GSYS.Screen.DisplayNum )
	{
		return -1 ;
	}

	// �ő�̖ʐς����𑜓x�𒲂ׂ�
	MaxSizeX = 0 ;
	MaxSizeY = 0 ;
	DisplayModeData = GSYS.Screen.DisplayInfo[ DisplayIndex ].ModeData ;
	for( i = 0 ; i < GSYS.Screen.DisplayInfo[ DisplayIndex ].ModeNum ; i ++, DisplayModeData ++ )
	{
		if( DisplayModeData->Width * DisplayModeData->Height > MaxSizeX * MaxSizeY )
		{
			MaxSizeX = DisplayModeData->Width ;
			MaxSizeY = DisplayModeData->Height ;
		}
	}

	// �T�C�Y��ۑ�
	if( SizeX != NULL )
	{
		*SizeX = MaxSizeX ;
	}

	if( SizeY != NULL )
	{
		*SizeY = MaxSizeY ;
	}

	// ����I��
	return 0 ;
}

// �f�B�X�v���[�̃J���[�f�[�^�|�C���^�𓾂�
extern	const COLORDATA *NS_GetDispColorData( void )
{
	if( GSYS.Setting.ValidHardware )
	{
		return Graphics_Hardware_GetDispColorData_PF() ;
	}
	else
	{
		return GSYS.SoftRender.MainBufferMemImg.Base->ColorDataP ;
	}
}

// �����ɕ`����s�����Ƃ��ł����ʂ̐����擾����
extern int NS_GetMultiDrawScreenNum( void )
{
	return GSYS.HardInfo.RenderTargetNum ;
}

// DrawGraphF ���̕��������_�l�ō��W���w�肷��֐��ɂ�������W�^�C�v���擾����( �߂�l : DX_DRAWFLOATCOORDTYPE_DIRECT3D9 �Ȃ� )
extern int NS_GetDrawFloatCoordType( void )
{
	return GSYS.HardInfo.DrawFloatCoordType ;
}

























// ���̑��ݒ�֌W�֐�

// �ʏ�`��Ƀv���O���}�u���V�F�[�_�[���g�p���邩�ǂ�����ݒ肷��( TRUE:�g�p����( �f�t�H���g )  FALSE:�g�p���Ȃ� )
extern	int		NS_SetUseNormalDrawShader( int Flag )
{
	return Graphics_Hardware_SetUseNormalDrawShader_PF( Flag ) ;
}

// �\�t�g�E�G�A�����_�����O���[�h���g�p���邩�ǂ������Z�b�g����
extern int NS_SetUseSoftwareRenderModeFlag( int Flag )
{
	return NS_SetScreenMemToVramFlag( Flag == TRUE ? FALSE : TRUE ) ;
}

// �R�c�@�\���g��Ȃ��t���O�̃Z�b�g
extern	int		NS_SetNotUse3DFlag( int Flag )
{
	// �������O�̂ݎg�p�\�@
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return -1 ;

	GSYS.Setting.NotUseHardware = Flag ;

	// �I��
	return 0 ;
}

// �R�c�@�\���g�����A�̃t���O���Z�b�g
extern	int NS_SetUse3DFlag( int Flag )
{
	// �������O�̂ݗL��
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return -1 ;

	GSYS.Setting.NotUseHardware = !Flag ;

	// �I��
	return 0 ;
}

// �`��ɂR�c�@�\���g�����t���O���擾
extern	int NS_GetUse3DFlag( void )
{
	// �I��
	return !GSYS.Setting.NotUseHardware ;
}

// ��ʃf�[�^���u�q�`�l�ɒu�����A�t���O
extern	int		NS_SetScreenMemToVramFlag( int Flag )
{
	// �������O�̂ݎg�p�\�@
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return -1 ;

	GSYS.Setting.NotUseHardware = !Flag ;

	// �I��
	return 0 ;
}

// ��ʃO���t�B�b�N�f�[�^���V�X�e���������ɑ��݂��邩�t���O�̎擾
extern	int						NS_GetScreenMemToSystemMemFlag( void )
{
	// �I��
	return GSYS.Setting.ValidHardware == FALSE ;
}

// �ʏ�g�p���Ȃ�
extern int NS_SetWindowDrawRect( const RECT * /* DrawRect */ )
{
//	GSYS.WindowDrawRect = *DrawRect ;

	return 0 ;
}

// �c�w���C�u�����̃O���t�B�b�N�֘A�̕��A�������s��
extern	int NS_RestoreGraphSystem( void )
{
	Graphics_RestoreOrChangeSetupGraphSystem( FALSE ) ;

	// �I��
	return 0 ;
}

// �n�[�h�E�G�A�̒��_���Z�����@�\���g�p���邩�ǂ�����ݒ肷��
extern	int NS_SetUseHardwareVertexProcessing( int Flag )
{
	return Graphics_Hardware_SetUseHardwareVertexProcessing_PF( Flag ) ;
}

// �s�N�Z���P�ʂŃ��C�e�B���O���s�����ǂ�����ݒ肷��A�v ShaderModel 3.0( TRUE:�s�N�Z���P�ʂ̃��C�e�B���O���s��  FALSE:���_�P�ʂ̃��C�e�B���O���s��( �f�t�H���g ) )
extern int NS_SetUsePixelLighting( int Flag )
{
	return Graphics_Hardware_SetUsePixelLighting_PF( Flag ) ;
}

// �ȑO�� DrawModiGraph �֐��̃R�[�h���g�p���邩�ǂ����̃t���O���Z�b�g����
extern int NS_SetUseOldDrawModiGraphCodeFlag( int Flag )
{
	return Graphics_Hardware_SetUseOldDrawModiGraphCodeFlag_PF( Flag ) ;
}

// �u�q�`�l���g�p���邩�̃t���O���Z�b�g����
extern	int NS_SetUseVramFlag( int Flag )
{
	return NS_SetUseSystemMemGraphCreateFlag( Flag == TRUE ? FALSE : TRUE ) ;
}

// �Q�c�O���t�B�b�N�T�[�t�F�X�쐬���ɃV�X�e���������[���g�p���邩�̃t���O�擾
extern	int NS_GetUseVramFlag( void )
{
	return NS_GetUseSystemMemGraphCreateFlag() == TRUE ? FALSE : TRUE ;
}

// �ȗ����u�����h�������s�����ۂ��̃t���O���Z�b�g����
extern	int		NS_SetBasicBlendFlag( int /*Flag*/ )
{
	// �I��
	return 0 ;
}

// �P���}�`�̕`��ɂR�c�f�o�C�X�̋@�\���g�p���邩�ǂ����̃t���O���Z�b�g����
extern int NS_SetUseBasicGraphDraw3DDeviceMethodFlag( int /* Flag */ )
{
//	GSYS.DrawSetting.NotUseBasicGraphDraw3DDeviceMethodFlag = !Flag ;

	return 0 ;
}

// �c�w���C�u�����̃E�C���h�E��\������f�B�X�v���C�f�o�C�X��ݒ肷��
extern int NS_SetUseDisplayIndex( int Index )
{
	if( GSYS.Screen.DisplayInfo == NULL )
	{
		Graphics_SetupDisplayInfo_PF() ;
	}

	if( Index < 0 || Index >= GSYS.Screen.DisplayNum )
	{
		return -1 ;
	}

	GSYS.Screen.ValidUseDisplayIndex = TRUE ;
	GSYS.Screen.UseDisplayIndex      = Index ;

	return 0 ;
}

// ���_�o�b�t�@�ɗ��܂������_�f�[�^��f���o��
extern int NS_RenderVertex( void )
{
	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	return 0 ;
}

























// �`����ʕۑ��֐�

#ifndef DX_NON_SAVEFUNCTION

// Jpeg_Quality         = 0:��掿�`100:���掿
// Png_CompressionLevel = 0:�����k�`  9:�ō����k
// ���ݕ`��ΏۂɂȂ��Ă����ʂ�ۑ�����
extern int NS_SaveDrawScreen( int x1, int y1, int x2, int y2, const TCHAR *FileName, int SaveType, int Jpeg_Quality, int Jpeg_Sample2x1, int Png_CompressionLevel )
{
#ifdef UNICODE
	return SaveDrawScreen_WCHAR_T(
		x1, y1, x2, y2, FileName, SaveType, Jpeg_Quality, Jpeg_Sample2x1, Png_CompressionLevel
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = SaveDrawScreen_WCHAR_T(
		x1, y1, x2, y2, UseFileNameBuffer, SaveType, Jpeg_Quality, Jpeg_Sample2x1, Png_CompressionLevel
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// Jpeg_Quality         = 0:��掿�`100:���掿
// Png_CompressionLevel = 0:�����k�`  9:�ō����k
// ���ݕ`��ΏۂɂȂ��Ă����ʂ�ۑ�����
extern int SaveDrawScreen_WCHAR_T( int x1, int y1, int x2, int y2, const wchar_t *FileName, int SaveType, int Jpeg_Quality, int Jpeg_Sample2x1, int Png_CompressionLevel )
{
	BASEIMAGE BaseImage ;
	RECT      LockRect ;

	// �T�C�Y���s���ȏꍇ�͉��������ɏI��
	if( x1 >= x2 || y1 >= y2 || x1 < 0 || y1 < 0 || x2 > GSYS.DrawSetting.DrawSizeX || y2 > GSYS.DrawSetting.DrawSizeY )
	{
		return -1 ;
	}

	// �`�������b�N
	LockRect.left   = x1 ;
	LockRect.top    = y1 ;
	LockRect.right  = x2 ;
	LockRect.bottom = y2 ;
	if( Graphics_Screen_LockDrawScreen( &LockRect, &BaseImage, -1, -1, TRUE, 0 ) < 0 )
	{
		return -1 ;
	}

	// �ۑ��`���ɂ���ď�����ύX����
	switch( SaveType )
	{
	case DX_IMAGESAVETYPE_JPEG:
#ifndef DX_NON_JPEGREAD
		SaveBaseImageToJpeg_WCHAR_T( FileName, &BaseImage, Jpeg_Quality, Jpeg_Sample2x1 );
#endif
		break;

	case DX_IMAGESAVETYPE_PNG:
#ifndef DX_NON_PNGREAD
		SaveBaseImageToPng_WCHAR_T( FileName, &BaseImage, Png_CompressionLevel );
#endif
		break;

	case DX_IMAGESAVETYPE_BMP:	// Bitmap 
		SaveBaseImageToBmp_WCHAR_T( FileName, &BaseImage ) ;
		break ;

	case DX_IMAGESAVETYPE_DDS:	// Direct Draw Surface
		SaveBaseImageToDds_WCHAR_T( FileName, &BaseImage ) ;
		break ;
	}

	// �`�����A�����b�N
	Graphics_Screen_UnlockDrawScreen() ;

	// �I��
	return 0 ;
}

// ���ݕ`��ΏۂɂȂ��Ă����ʂ��a�l�o�`���ŕۑ�����
extern int NS_SaveDrawScreenToBMP( int x1, int y1, int x2, int y2, const TCHAR *FileName )
{
	return NS_SaveDrawScreen( x1, y1, x2, y2, FileName, DX_IMAGESAVETYPE_BMP );
}

// ���ݕ`��ΏۂɂȂ��Ă����ʂ��a�l�o�`���ŕۑ�����
extern int SaveDrawScreenToBMP_WCHAR_T( int x1, int y1, int x2, int y2, const wchar_t *FileName )
{
	return SaveDrawScreen_WCHAR_T( x1, y1, x2, y2, FileName, DX_IMAGESAVETYPE_BMP );
}

// ���ݕ`��ΏۂɂȂ��Ă����ʂ��c�c�r�`���ŕۑ�����
extern int NS_SaveDrawScreenToDDS(  int x1, int y1, int x2, int y2, const TCHAR *FileName )
{
	return NS_SaveDrawScreen( x1, y1, x2, y2, FileName, DX_IMAGESAVETYPE_DDS );
}

// ���ݕ`��ΏۂɂȂ��Ă����ʂ��a�l�o�`���ŕۑ�����
extern int SaveDrawScreenToDDS_WCHAR_T( int x1, int y1, int x2, int y2, const wchar_t *FileName )
{
	return SaveDrawScreen_WCHAR_T( x1, y1, x2, y2, FileName, DX_IMAGESAVETYPE_DDS );
}

// ���ݕ`��ΏۂɂȂ��Ă����ʂ��i�o�d�f�`���ŕۑ����� Quality = �掿�A�l���傫���قǒሳ�k���掿,0�`100 
extern int NS_SaveDrawScreenToJPEG( int x1, int y1, int x2, int y2, const TCHAR *FileName, int Quality, int Sample2x1 )
{
	return NS_SaveDrawScreen( x1, y1, x2, y2, FileName, DX_IMAGESAVETYPE_JPEG, Quality, Sample2x1 );
}

// ���ݕ`��ΏۂɂȂ��Ă����ʂ��i�o�d�f�`���ŕۑ����� Quality = �掿�A�l���傫���قǒሳ�k���掿,0�`100 
extern int SaveDrawScreenToJPEG_WCHAR_T( int x1, int y1, int x2, int y2, const wchar_t *FileName, int Quality, int Sample2x1 )
{
	return SaveDrawScreen_WCHAR_T( x1, y1, x2, y2, FileName, DX_IMAGESAVETYPE_JPEG, Quality, Sample2x1 );
}

// ���ݕ`��ΏۂɂȂ��Ă����ʂ��o�m�f�`���ŕۑ����� CompressionLevel = ���k���A�l���傫���قǍ����k�������ׁA�O�͖����k,0�`9
extern int NS_SaveDrawScreenToPNG( int x1, int y1, int x2, int y2, const TCHAR *FileName, int CompressionLevel )
{
	return NS_SaveDrawScreen( x1, y1, x2, y2, FileName, DX_IMAGESAVETYPE_PNG, 80, CompressionLevel );
}

// ���ݕ`��ΏۂɂȂ��Ă����ʂ��o�m�f�`���ŕۑ����� CompressionLevel = ���k���A�l���傫���قǍ����k�������ׁA�O�͖����k,0�`9
extern int SaveDrawScreenToPNG_WCHAR_T( int x1, int y1, int x2, int y2, const wchar_t *FileName, int CompressionLevel )
{
	return SaveDrawScreen_WCHAR_T( x1, y1, x2, y2, FileName, DX_IMAGESAVETYPE_PNG, 80, CompressionLevel );
}

#endif // DX_NON_SAVEFUNCTION

























// ���_�o�b�t�@�֌W�֐�

// ���_�o�b�t�@���쐬����( -1:�G���[  0�ȏ�:���_�o�b�t�@�n���h�� )
extern int NS_CreateVertexBuffer( int VertexNum, int VertexType /* DX_VERTEX_TYPE_NORMAL_3D �� */ )
{
	return Graphics_VertexBuffer_Create( VertexNum, VertexType, FALSE ) ;
}

// ���_�o�b�t�@���폜����
extern int NS_DeleteVertexBuffer( int VertexBufHandle )
{
	return SubHandle( VertexBufHandle ) ;
}

// ���ׂĂ̒��_�o�b�t�@���폜����
extern int NS_InitVertexBuffer()
{
	return AllHandleSub( DX_HANDLETYPE_VERTEX_BUFFER ) ;
}

// ���_�o�b�t�@�ɒ��_�f�[�^��]������
extern int NS_SetVertexBufferData( int SetIndex, const void *VertexData, int VertexNum, int VertexBufHandle )
{
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;

	// �G���[����
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
		return -1 ;

	// �G���[�`�F�b�N
	if( VertexBuffer->Num < SetIndex + VertexNum || SetIndex < 0 )
		return -1 ;

	// ���ˑ��̒��_�f�[�^�]������
	if( Graphics_Hardware_VertexBuffer_SetData_PF( VertexBuffer, SetIndex, VertexData, VertexNum ) < 0 )
	{
		return -1 ;
	}

	// �V�X�e���������̃o�b�t�@�ւ��]��
	_MEMCPY( ( BYTE * )VertexBuffer->Buffer + SetIndex * VertexBuffer->UnitSize, VertexData, ( size_t )( VertexNum * VertexBuffer->UnitSize ) ) ;

	// �I��
	return 0 ;
}

// �C���f�b�N�X�o�b�t�@���쐬����( -1:�G���[�@0�ȏ�F�C���f�b�N�X�o�b�t�@�n���h�� )
extern int NS_CreateIndexBuffer( int IndexNum, int IndexType /* DX_INDEX_TYPE_16BIT �� */  )
{
	return Graphics_IndexBuffer_Create( IndexNum, IndexType, FALSE ) ;
}

// �C���f�b�N�X�o�b�t�@���폜����
extern int NS_DeleteIndexBuffer( int IndexBufHandle )
{
	return SubHandle( IndexBufHandle ) ;
}

// ���ׂẴC���f�b�N�X�o�b�t�@���폜����
extern int NS_InitIndexBuffer()
{
	return AllHandleSub( DX_HANDLETYPE_INDEX_BUFFER ) ;
}

// �C���f�b�N�X�o�b�t�@�ɃC���f�b�N�X�f�[�^��]������
extern int NS_SetIndexBufferData( int SetIndex, const void *IndexData, int IndexNum, int IndexBufHandle )
{
	INDEXBUFFERHANDLEDATA *IndexBuffer ;

	// �G���[����
	if( INDEXBUFFERCHK( IndexBufHandle, IndexBuffer ) )
		return -1 ;

	// �G���[�`�F�b�N
	if( IndexBuffer->Num < SetIndex + IndexNum || SetIndex < 0 )
		return -1 ;

	// ���ˑ��̃C���f�b�N�X�f�[�^�]������
	if( Graphics_Hardware_IndexBuffer_SetData_PF( IndexBuffer, SetIndex, IndexData, IndexNum ) < 0 )
	{
		return -1 ;
	}

	// �V�X�e���������̃o�b�t�@�ւ��]��
	_MEMCPY( ( BYTE * )IndexBuffer->Buffer + SetIndex * IndexBuffer->UnitSize, IndexData, ( size_t )( IndexNum * IndexBuffer->UnitSize ) ) ;

	// �I��
	return 0 ;
}

// �O���t�B�b�N�X�f�o�C�X���Ή����Ă����x�ɕ`��ł���v���~�e�B�u�̍ő吔���擾����
extern int NS_GetMaxPrimitiveCount( void )
{
	return GSYS.HardInfo.MaxPrimitiveCount ;
}

// �O���t�B�b�N�X�f�o�C�X���Ή����Ă����x�Ɏg�p���邱�Ƃ̂ł���ő咸�_�����擾����
extern int NS_GetMaxVertexIndex( void )
{
	return GSYS.HardInfo.MaxVertexIndex ;
}






















// �V�F�[�_�[�֌W�֐�

// �g�p�ł���V�F�[�_�[�̃o�[�W�������擾����( 0=�g���Ȃ�  200=�V�F�[�_�[���f���Q�D�O���g�p�\  300=�V�F�[�_�[���f���R�D�O���g�p�\ )
extern int NS_GetValidShaderVersion( void )
{
	return Graphics_Hardware_Shader_GetValidShaderVersion_PF() ;
}

// ���_�V�F�[�_�[�o�C�i�����t�@�C������ǂݍ��ݒ��_�V�F�[�_�[�n���h�����쐬����( �߂�l -1:�G���[  -1�ȊO:�V�F�[�_�[�n���h�� )
extern int NS_LoadVertexShader( const TCHAR *FileName )
{
#ifdef UNICODE
	return LoadVertexShader_WCHAR_T(
		FileName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = LoadVertexShader_WCHAR_T(
		UseFileNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// ���_�V�F�[�_�[�o�C�i�����t�@�C������ǂݍ��ݒ��_�V�F�[�_�[�n���h�����쐬����( �߂�l -1:�G���[  -1�ȊO:�V�F�[�_�[�n���h�� )
extern int LoadVertexShader_WCHAR_T( const wchar_t *FileName )
{
	return Graphics_Shader_LoadShader_UseGParam( TRUE, FileName, GetASyncLoadFlag() ) ;
}

// �s�N�Z���V�F�[�_�[�o�C�i�����t�@�C������ǂݍ��݃s�N�Z���V�F�[�_�[�n���h�����쐬����( �߂�l -1:�G���[  -1�ȊO:�V�F�[�_�[�n���h�� )
extern int NS_LoadPixelShader( const TCHAR *FileName )
{
#ifdef UNICODE
	return LoadPixelShader_WCHAR_T(
		FileName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = LoadPixelShader_WCHAR_T(
		UseFileNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �s�N�Z���V�F�[�_�[�o�C�i�����t�@�C������ǂݍ��݃s�N�Z���V�F�[�_�[�n���h�����쐬����( �߂�l -1:�G���[  -1�ȊO:�V�F�[�_�[�n���h�� )
extern int LoadPixelShader_WCHAR_T( const wchar_t *FileName )
{
	return Graphics_Shader_LoadShader_UseGParam( FALSE, FileName, GetASyncLoadFlag() ) ;
}

// ��������ԏ�ɑ��݂��钸�_�V�F�[�_�[�o�C�i�����璸�_�V�F�[�_�[�n���h�����쐬����( �߂�l -1:�G���[  -1�ȊO:�V�F�[�_�[�n���h�� )
extern int NS_LoadVertexShaderFromMem( const void *ImageAddress, int ImageSize )
{
	return Graphics_Shader_CreateHandle_UseGParam( TRUE, ( void * )ImageAddress, ImageSize, FALSE, GetASyncLoadFlag() ) ;
}

// ��������ԏ�ɑ��݂���s�N�Z���V�F�[�_�[�o�C�i������s�N�Z���V�F�[�_�[�n���h�����쐬����( �߂�l -1:�G���[  -1�ȊO:�V�F�[�_�[�n���h�� )
extern int NS_LoadPixelShaderFromMem( const void *ImageAddress, int ImageSize )
{
	return Graphics_Shader_CreateHandle_UseGParam( FALSE, ( void * )ImageAddress, ImageSize, FALSE, GetASyncLoadFlag() ) ;
}

// �V�F�[�_�[�n���h���̍폜
extern int NS_DeleteShader( int ShaderHandle )
{
	return SubHandle( ShaderHandle ) ;
}

// �V�F�[�_�[�n���h����S�č폜����
extern int NS_InitShader( void )
{
	if( GSYS.InitializeFlag == FALSE ) return 0 ;

	return AllHandleSub( DX_HANDLETYPE_SHADER ) ;
}

// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̔ԍ����擾����
extern int NS_GetConstIndexToShader( const TCHAR *ConstantName, int ShaderHandle )
{
#ifdef UNICODE
	return GetConstIndexToShader_WCHAR_T(
		ConstantName, ShaderHandle
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( ConstantName, return -1 )

	Result = GetConstIndexToShader_WCHAR_T(
		UseConstantNameBuffer, ShaderHandle
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( ConstantName )

	return Result ;
#endif
}

// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̔ԍ����擾����
extern int GetConstIndexToShader_WCHAR_T( const wchar_t *ConstantName, int ShaderHandle )
{
	SHADERHANDLEDATA *Shader ;

	// �G���[����
	if( SHADERCHK( ShaderHandle, Shader ) )
	{
		return -1 ;
	}

	// ���ˑ�����
	return Graphics_Hardware_Shader_GetConstIndex_PF( ConstantName, Shader ) ;
}

// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̐����擾����
extern int NS_GetConstCountToShader( const TCHAR *ConstantName, int ShaderHandle )
{
#ifdef UNICODE
	return GetConstCountToShader_WCHAR_T(
		ConstantName, ShaderHandle
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( ConstantName, return -1 )

	Result = GetConstCountToShader_WCHAR_T(
		UseConstantNameBuffer, ShaderHandle
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( ConstantName )

	return Result ;
#endif
}

// �w��̖��O�����萔���g�p����V�F�[�_�[�萔�̐����擾����
extern int GetConstCountToShader_WCHAR_T( const wchar_t *ConstantName, int ShaderHandle )
{
	SHADERHANDLEDATA *Shader ;

	// �G���[����
	if( SHADERCHK( ShaderHandle, Shader ) )
		return -1 ;

	// ���ˑ�����
	return Graphics_Hardware_Shader_GetConstCount_PF( ConstantName, Shader ) ;
}

// �w��̖��O�������������_�萔�̃f�t�H���g�p�����[�^���i�[����Ă��郁�����A�h���X���擾����
extern	const FLOAT4 *NS_GetConstDefaultParamFToShader( const TCHAR *ConstantName, int ShaderHandle )
{
#ifdef UNICODE
	return GetConstDefaultParamFToShader_WCHAR_T(
		ConstantName, ShaderHandle
	) ;
#else
	const FLOAT4 * Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( ConstantName, return NULL )

	Result = GetConstDefaultParamFToShader_WCHAR_T(
		UseConstantNameBuffer, ShaderHandle
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( ConstantName )

	return Result ;
#endif
}

// �w��̖��O�������������_�萔�̃f�t�H���g�p�����[�^���i�[����Ă��郁�����A�h���X���擾����
extern	const FLOAT4 *GetConstDefaultParamFToShader_WCHAR_T( const wchar_t *ConstantName, int ShaderHandle )
{
	SHADERHANDLEDATA *Shader ;

	// �G���[����
	if( SHADERCHK( ShaderHandle, Shader ) )
		return NULL ;

	// ���ˑ�����
	return Graphics_Hardware_Shader_GetConstDefaultParamF_PF( ConstantName, Shader ) ;
}

// ���_�V�F�[�_�[�� float �^�萔��ݒ肷��
extern int NS_SetVSConstSF( int ConstantIndex, float Param )
{
	FLOAT4 ParamF4 ;

	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex >= 256 )
		return -1 ;

	// �萔�X�V
	ParamF4.x = Param ;
	ParamF4.y = Param ;
	ParamF4.z = Param ;
	ParamF4.w = Param ;
	NS_SetVSConstF( ConstantIndex, ParamF4 ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� float �^�萔��ݒ肷��
extern int NS_SetVSConstF( int ConstantIndex, FLOAT4 Param )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex >= 256 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_VS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, &Param, 1, TRUE ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� float �^�萔�ɍs���ݒ肷��
extern	int			NS_SetVSConstFMtx( int ConstantIndex, MATRIX Param )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + 4 > 256 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_VS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ( FLOAT4 * )&Param, 4, TRUE ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� float �^�萔�ɓ]�u�����s���ݒ肷��
extern	int			NS_SetVSConstFMtxT( int ConstantIndex, MATRIX Param )
{
	MATRIX Transpose ;

	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + 4 > 256 )
		return -1 ;

	// �萔�X�V
	CreateTransposeMatrix( &Transpose, &Param ) ;
	NS_SetVSConstFMtx( ConstantIndex, Transpose ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� int �^�萔��ݒ肷��
extern	int			NS_SetVSConstSI( int ConstantIndex, int Param )
{
	INT4 ParamI4 ;

	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex >= 16 )
		return -1 ;

	// �萔�X�V
	ParamI4.x = Param ;
	ParamI4.y = Param ;
	ParamI4.z = Param ;
	ParamI4.w = Param ;
	return NS_SetVSConstI( ConstantIndex, ParamI4 ) ;
}

// ���_�V�F�[�_�[�� int �^�萔��ݒ肷��
extern	int			NS_SetVSConstI( int ConstantIndex, INT4 Param )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex >= 16 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_VS_INT, DX_SHADERCONSTANTSET_USER, ConstantIndex, &Param, 1, TRUE ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� BOOL �^�萔��ݒ肷��
extern	int			NS_SetVSConstB( int ConstantIndex, BOOL Param )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex >= 16 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_VS_BOOL, DX_SHADERCONSTANTSET_USER, ConstantIndex, &Param, 1, TRUE ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� float �^�萔��ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetVSConstSFArray( int ConstantIndex, const float *ParamArray, int ParamNum )
{
	FLOAT4 ParamArrayF4[ 256 ] ;
	int i ;

	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 256 )
		return -1 ;

	// �z��Ƀf�[�^���Z�b�g
	for( i = 0 ; i < ParamNum ; i ++ )
	{
		ParamArrayF4[ i ].x = ParamArray[ i ] ;
		ParamArrayF4[ i ].y = ParamArray[ i ] ;
		ParamArrayF4[ i ].z = ParamArray[ i ] ;
		ParamArrayF4[ i ].w = ParamArray[ i ] ;
	}

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_VS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamArrayF4, ParamNum, TRUE ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� float �^�萔��ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetVSConstFArray( int ConstantIndex, const FLOAT4 *ParamArray, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 256 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_VS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamArray, ParamNum, TRUE ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� float �^�萔�ɍs���ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetVSConstFMtxArray( int ConstantIndex, const MATRIX *ParamArray, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum * 4 > 256 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_VS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamArray, ParamNum * 4, TRUE ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� float �^�萔�ɓ]�u�����s���ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetVSConstFMtxTArray( int ConstantIndex, const MATRIX *ParamArray, int ParamNum )
{
	MATRIX Transpose[ 256 / 4 ] ;
	int i ;

	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum * 4 > 256 )
		return -1 ;

	// �萔�X�V
	for( i = 0 ; i < ParamNum ; i ++ )
	{
		Transpose[ i ].m[ 0 ][ 0 ] = ParamArray[ i ].m[ 0 ][ 0 ] ;
		Transpose[ i ].m[ 1 ][ 0 ] = ParamArray[ i ].m[ 0 ][ 1 ] ;
		Transpose[ i ].m[ 2 ][ 0 ] = ParamArray[ i ].m[ 0 ][ 2 ] ;
		Transpose[ i ].m[ 3 ][ 0 ] = ParamArray[ i ].m[ 0 ][ 3 ] ;

		Transpose[ i ].m[ 0 ][ 1 ] = ParamArray[ i ].m[ 1 ][ 0 ] ;
		Transpose[ i ].m[ 1 ][ 1 ] = ParamArray[ i ].m[ 1 ][ 1 ] ;
		Transpose[ i ].m[ 2 ][ 1 ] = ParamArray[ i ].m[ 1 ][ 2 ] ;
		Transpose[ i ].m[ 3 ][ 1 ] = ParamArray[ i ].m[ 1 ][ 3 ] ;

		Transpose[ i ].m[ 0 ][ 2 ] = ParamArray[ i ].m[ 2 ][ 0 ] ;
		Transpose[ i ].m[ 1 ][ 2 ] = ParamArray[ i ].m[ 2 ][ 1 ] ;
		Transpose[ i ].m[ 2 ][ 2 ] = ParamArray[ i ].m[ 2 ][ 2 ] ;
		Transpose[ i ].m[ 3 ][ 2 ] = ParamArray[ i ].m[ 2 ][ 3 ] ;

		Transpose[ i ].m[ 0 ][ 3 ] = ParamArray[ i ].m[ 3 ][ 0 ] ;
		Transpose[ i ].m[ 1 ][ 3 ] = ParamArray[ i ].m[ 3 ][ 1 ] ;
		Transpose[ i ].m[ 2 ][ 3 ] = ParamArray[ i ].m[ 3 ][ 2 ] ;
		Transpose[ i ].m[ 3 ][ 3 ] = ParamArray[ i ].m[ 3 ][ 3 ] ;
	}
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_VS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, Transpose, ParamNum * 4, TRUE ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� int �^�萔��ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetVSConstSIArray( int ConstantIndex, const int *ParamArray, int ParamNum )
{
	INT4 ParamArrayI4[ 16 ] ;
	int i ;

	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 16 )
		return -1 ;

	// �z��̃Z�b�g
	for( i = 0 ; i < ParamNum ; i ++ )
	{
		ParamArrayI4[ i ].x = ParamArray[ i ] ;
		ParamArrayI4[ i ].y = ParamArray[ i ] ;
		ParamArrayI4[ i ].z = ParamArray[ i ] ;
		ParamArrayI4[ i ].w = ParamArray[ i ] ;
	}

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_VS_INT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamArrayI4, ParamNum, TRUE ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� int �^�萔��ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetVSConstIArray( int ConstantIndex, const INT4 *ParamArray, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 16 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_VS_INT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamArray, ParamNum, TRUE ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� BOOL �^�萔��ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetVSConstBArray( int ConstantIndex, const BOOL *ParamArray, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 16 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_VS_BOOL, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamArray, ParamNum, TRUE ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� float �^�萔�̐ݒ�����Z�b�g����
extern	int			NS_ResetVSConstF( int ConstantIndex, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 256 )
		return -1 ;

	// �萔���Z�b�g
	Graphics_Hardware_Shader_ResetConst_PF( DX_SHADERCONSTANTTYPE_VS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamNum ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� int �^�萔�̐ݒ�����Z�b�g����
extern	int			NS_ResetVSConstI( int ConstantIndex, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 16 )
		return -1 ;

	// �萔���Z�b�g
	Graphics_Hardware_Shader_ResetConst_PF( DX_SHADERCONSTANTTYPE_VS_INT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamNum ) ;

	// �I��
	return 0 ;
}

// ���_�V�F�[�_�[�� BOOL �^�萔�̐ݒ�����Z�b�g����
extern	int			NS_ResetVSConstB( int ConstantIndex, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 16 )
		return -1 ;

	// �萔���Z�b�g
	Graphics_Hardware_Shader_ResetConst_PF( DX_SHADERCONSTANTTYPE_VS_BOOL, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamNum ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� float �^�萔��ݒ肷��
extern	int			NS_SetPSConstSF( int ConstantIndex, float Param )
{
	FLOAT4 ParamF4 ;

	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex >= 224 )
		return -1 ;

	// �萔�X�V
	ParamF4.x = Param ;
	ParamF4.y = Param ;
	ParamF4.z = Param ;
	ParamF4.w = Param ;
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, &ParamF4, 1, TRUE ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� float �^�萔��ݒ肷��
extern	int			NS_SetPSConstF( int ConstantIndex, FLOAT4 Param )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex >= 224 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, &Param, 1, TRUE ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� float �^�萔�ɍs���ݒ肷��
extern	int			NS_SetPSConstFMtx( int ConstantIndex, MATRIX Param )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + 4 > 224 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ( FLOAT4 * )&Param, 4, TRUE ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� float �^�萔�ɓ]�u�����s���ݒ肷��
extern	int			NS_SetPSConstFMtxT( int ConstantIndex, MATRIX Param )
{
	MATRIX Transpose ;

	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + 4 > 224 )
		return -1 ;

	// �萔�X�V
	CreateTransposeMatrix( &Transpose, &Param ) ;
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ( FLOAT4 * )&Transpose, 4, TRUE ) ;

	// �I��
	return 0 ;
}


// �s�N�Z���V�F�[�_�[�� int �^�萔��ݒ肷��
extern	int			NS_SetPSConstSI( int ConstantIndex, int Param )
{
	INT4 ParamI4 ;

	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex >= 16 )
		return -1 ;

	// �萔�X�V
	ParamI4.x = Param ;
	ParamI4.y = Param ;
	ParamI4.z = Param ;
	ParamI4.w = Param ;
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_INT, DX_SHADERCONSTANTSET_USER, ConstantIndex, &ParamI4, 1, TRUE ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� int �^�萔��ݒ肷��
extern	int			NS_SetPSConstI( int ConstantIndex, INT4 Param )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex >= 16 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_INT, DX_SHADERCONSTANTSET_USER, ConstantIndex, &Param, 1, TRUE ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� BOOL �^�萔��ݒ肷��
extern	int			NS_SetPSConstB( int ConstantIndex, BOOL Param )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex >= 16 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_BOOL, DX_SHADERCONSTANTSET_USER, ConstantIndex, &Param, 1, TRUE ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� float �^�萔��ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetPSConstSFArray( int ConstantIndex, const float *ParamArray, int ParamNum )
{
	FLOAT4 ParamArrayF4[ 256 ] ;
	int i ;

	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 224 )
		return -1 ;

	// �z��ɃZ�b�g
	for( i = 0 ; i < ParamNum ; i ++ )
	{
		ParamArrayF4[ i ].x = ParamArray[ i ] ;
		ParamArrayF4[ i ].y = ParamArray[ i ] ;
		ParamArrayF4[ i ].z = ParamArray[ i ] ;
		ParamArrayF4[ i ].w = ParamArray[ i ] ;
	}

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamArrayF4, ParamNum, TRUE ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� float �^�萔��ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetPSConstFArray( int ConstantIndex, const FLOAT4 *ParamArray, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 224 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamArray, ParamNum, TRUE ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� float �^�萔�ɍs���ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetPSConstFMtxArray( int ConstantIndex, const MATRIX *ParamArray, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum * 4 > 224 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamArray, ParamNum * 4, TRUE ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� float �^�萔�ɓ]�u�����s���ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetPSConstFMtxTArray( int ConstantIndex, const MATRIX *ParamArray, int ParamNum )
{
	int i ;
	MATRIX Transpose[ 224 / 4 ] ;

	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum * 4 > 224 )
		return -1 ;

	// �萔�X�V
	for( i = 0 ; i < ParamNum ; i ++ )
	{
		Transpose[ i ].m[ 0 ][ 0 ] = ParamArray[ i ].m[ 0 ][ 0 ] ;
		Transpose[ i ].m[ 1 ][ 0 ] = ParamArray[ i ].m[ 0 ][ 1 ] ;
		Transpose[ i ].m[ 2 ][ 0 ] = ParamArray[ i ].m[ 0 ][ 2 ] ;
		Transpose[ i ].m[ 3 ][ 0 ] = ParamArray[ i ].m[ 0 ][ 3 ] ;

		Transpose[ i ].m[ 0 ][ 1 ] = ParamArray[ i ].m[ 1 ][ 0 ] ;
		Transpose[ i ].m[ 1 ][ 1 ] = ParamArray[ i ].m[ 1 ][ 1 ] ;
		Transpose[ i ].m[ 2 ][ 1 ] = ParamArray[ i ].m[ 1 ][ 2 ] ;
		Transpose[ i ].m[ 3 ][ 1 ] = ParamArray[ i ].m[ 1 ][ 3 ] ;

		Transpose[ i ].m[ 0 ][ 2 ] = ParamArray[ i ].m[ 2 ][ 0 ] ;
		Transpose[ i ].m[ 1 ][ 2 ] = ParamArray[ i ].m[ 2 ][ 1 ] ;
		Transpose[ i ].m[ 2 ][ 2 ] = ParamArray[ i ].m[ 2 ][ 2 ] ;
		Transpose[ i ].m[ 3 ][ 2 ] = ParamArray[ i ].m[ 2 ][ 3 ] ;

		Transpose[ i ].m[ 0 ][ 3 ] = ParamArray[ i ].m[ 3 ][ 0 ] ;
		Transpose[ i ].m[ 1 ][ 3 ] = ParamArray[ i ].m[ 3 ][ 1 ] ;
		Transpose[ i ].m[ 2 ][ 3 ] = ParamArray[ i ].m[ 3 ][ 2 ] ;
		Transpose[ i ].m[ 3 ][ 3 ] = ParamArray[ i ].m[ 3 ][ 3 ] ;
	}
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, Transpose, ParamNum * 4, TRUE ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� int �^�萔��ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetPSConstSIArray( int ConstantIndex, const int *ParamArray, int ParamNum )
{
	INT4 ParamArrayI4[ 16 ] ;
	int i ;

	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 16 )
		return -1 ;

	// �z��Ƀp�����[�^���Z�b�g
	for( i = 0 ; i < ParamNum ; i ++ )
	{
		ParamArrayI4[ i ].x = ParamArray[ i ] ;
		ParamArrayI4[ i ].y = ParamArray[ i ] ;
		ParamArrayI4[ i ].z = ParamArray[ i ] ;
		ParamArrayI4[ i ].w = ParamArray[ i ] ;
	}

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_INT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamArrayI4, ParamNum, TRUE ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� int �^�萔��ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetPSConstIArray( int ConstantIndex, const INT4 *ParamArray, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 16 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_INT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamArray, ParamNum, TRUE ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� BOOL �^�萔��ݒ肷��( �z����g���ĘA�ԃC���f�b�N�X�Ɉ�x�ɐݒ� )
extern	int			NS_SetPSConstBArray( int ConstantIndex, const BOOL *ParamArray, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 16 )
		return -1 ;

	// �萔�X�V
	Graphics_Hardware_Shader_SetConst_PF( DX_SHADERCONSTANTTYPE_PS_BOOL, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamArray, ParamNum, TRUE ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� float �^�萔�̐ݒ�����Z�b�g����
extern	int			NS_ResetPSConstF( int ConstantIndex, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 256 )
		return -1 ;

	// �萔���Z�b�g
	Graphics_Hardware_Shader_ResetConst_PF( DX_SHADERCONSTANTTYPE_PS_FLOAT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamNum ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� int �^�萔�̐ݒ�����Z�b�g����
extern	int			NS_ResetPSConstI( int ConstantIndex, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 16 )
		return -1 ;

	// �萔���Z�b�g
	Graphics_Hardware_Shader_ResetConst_PF( DX_SHADERCONSTANTTYPE_PS_INT, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamNum ) ;

	// �I��
	return 0 ;
}

// �s�N�Z���V�F�[�_�[�� BOOL �^�萔�̐ݒ�����Z�b�g����
extern	int			NS_ResetPSConstB( int ConstantIndex, int ParamNum )
{
	// �V�F�[�_�[���g���Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.HardInfo.UseShader == FALSE )
		return 0 ;

	// �͈͉z���`�F�b�N
	if( ConstantIndex < 0 || ConstantIndex + ParamNum > 16 )
		return -1 ;

	// �萔���Z�b�g
	Graphics_Hardware_Shader_ResetConst_PF( DX_SHADERCONSTANTTYPE_PS_BOOL, DX_SHADERCONSTANTSET_USER, ConstantIndex, ParamNum ) ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[�`��ł̕`����ݒ肷��
extern int NS_SetRenderTargetToShader( int TargetIndex, int DrawScreen, int SurfaceIndex )
{
	// �^�[�Q�b�g�C���f�b�N�X�͈̔̓`�F�b�N
	if( TargetIndex < 0 || TargetIndex >= GSYS.HardInfo.RenderTargetNum )
		return -1 ;

	// �l�������ꍇ�͉��������I��
//	if( GSYS.DrawSetting.TargetScreen[ TargetIndex ] == DrawScreen && GSYS.DrawSetting.TargetScreenSurface[ TargetIndex ] == SurfaceIndex )
//		return 0 ;

	// �^�[�Q�b�g���O�ŃT�[�t�F�X�C���f�b�N�X���O�̏ꍇ�� SetDrawScreen �ɔC����
	if( TargetIndex == 0 && SurfaceIndex == 0 )
	{
		return NS_SetDrawScreen( DrawScreen ) ;
	}

	// �n�[�h�E�G�A�œ��삵�Ă��Ȃ��ꍇ�͉������Ȃ�
	if( GSYS.Setting.ValidHardware == FALSE )
	{
		return -1 ;
	}

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ʂ̏��������s����
	if( Graphics_Hardware_SetRenderTargetToShader_PF( TargetIndex, DrawScreen, SurfaceIndex ) < 0 )
	{
		return -1 ;
	}

	// �摜�n���h����ۑ�
	GSYS.DrawSetting.TargetScreen[ TargetIndex ] = DrawScreen ;
	GSYS.DrawSetting.TargetScreenSurface[ TargetIndex ] = SurfaceIndex ;

	// �`���ɐ��������l���������ނ��ǂ����̃t���O���X�V����
	Graphics_DrawSetting_RefreshAlphaChDrawMode() ;

	// ����I��
	return 0 ;
}

// �V�F�[�_�[�`��Ŏg�p����O���t�B�b�N��ݒ肷��
extern	int			NS_SetUseTextureToShader( int StageIndex, int GraphHandle )
{
	// �L���͈̓`�F�b�N
	if( StageIndex < 0 || StageIndex >= USE_TEXTURESTAGE_NUM )
		return -1 ;

	// �O���t�B�b�N�n���h���`�F�b�N
	if( GraphHandle != -1 )
	{
		IMAGEDATA *Image ;
		SHADOWMAPDATA *ShadowMap ;

		if( GRAPHCHKFULL(     GraphHandle, Image    ) &&
			SHADOWMAPCHKFULL( GraphHandle, ShadowMap ) )
			return -1 ;

		// �n���h����ۑ�
		GSYS.DrawSetting.UserShaderRenderInfo.SetTextureGraphHandle[ StageIndex ] = GraphHandle ;
	}
	else
	{
		// �n���h�������Z�b�g
		GSYS.DrawSetting.UserShaderRenderInfo.SetTextureGraphHandle[ StageIndex ] = 0 ;
	}

	// �I��
	return 0 ;
}

// �V�F�[�_�[�`��Ɏg�p���钸�_�V�F�[�_�[��ݒ肷��
extern	int			NS_SetUseVertexShader( int ShaderHandle )
{
	SHADERHANDLEDATA *VertexShader ;

	if( ShaderHandle > 0 && ( SHADERCHK( ShaderHandle, VertexShader ) || VertexShader->IsVertexShader == FALSE ) )
		return -1 ;

	// �n���h�����Z�b�g
	GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle = ShaderHandle <= 0 ? 0 : ShaderHandle ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[�`��Ɏg�p����s�N�Z���V�F�[�_�[��ݒ肷��
extern	int			NS_SetUsePixelShader( int ShaderHandle )
{
	SHADERHANDLEDATA *PixelShader ;

	if( ShaderHandle > 0 && ( SHADERCHK( ShaderHandle, PixelShader ) || PixelShader->IsVertexShader == TRUE ) )
		return -1 ;

	// �n���h�����Z�b�g
	GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle = ShaderHandle <= 0 ? 0 : ShaderHandle  ;

	// �I��
	return 0 ;
}

// �|���S���̒��_�̐ڐ��Ə]�@�����t�u���W����v�Z���ăZ�b�g����
extern int NS_CalcPolygonBinormalAndTangentsToShader( VERTEX3DSHADER *Vertex, int PolygonNum )
{
	unsigned short *Indices ;
	int IndexNum ;
	int i ;
	int Result ;

	IndexNum = PolygonNum * 3 ;

	Indices = ( unsigned short * )DXALLOC( IndexNum * sizeof( unsigned short ) ) ;
	if( Indices == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x02\x98\xb9\x70\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���_�C���f�b�N�X���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
		return -1 ;
	}

	// ���_�C���f�b�N�X���Z�b�g
	for( i = 0 ; i < IndexNum ; i ++ )
	{
		Indices[ i ] = ( unsigned short )i ;
	}

	// �ڐ��Ə]�@�����v�Z
	Result = NS_CalcPolygonIndexedBinormalAndTangentsToShader( Vertex, IndexNum, Indices, PolygonNum ) ;

	// ���_�C���f�b�N�X���i�[���Ă����������̈���J��
	DXFREE( Indices ) ;

	// �I��
	return Result ;
}

// �|���S���̒��_�̐ڐ��Ə]�@�����t�u���W����v�Z���ăZ�b�g����(�C���f�b�N�X)
extern int NS_CalcPolygonIndexedBinormalAndTangentsToShader( VERTEX3DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int PolygonNum )
{
	int i ;
	VECTOR v1, v2, vt, du, dv, vb, vn ;
	BYTE *UseFlag ;
	const unsigned short *Index ;
	int IndexNum ;
	VERTEX3DSHADER *Vert[ 3 ] ;

	// �g�p���Ă��钸�_�̃e�[�u�����쐬����
	{
		UseFlag = ( BYTE * )DXALLOC( sizeof( BYTE ) * VertexNum ) ;
		if( UseFlag == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x02\x98\xb9\x70\x6e\x30\xa5\x63\xda\x7d\x68\x30\x93\x5f\xd5\x6c\xda\x7d\x6e\x30\x5c\x4f\x10\x62\x5c\x4f\x6d\x69\x6b\x30\xc5\x5f\x81\x89\x6a\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���_�̐ڐ��Ə]�@���̍쐬��ƂɕK�v�ȃ������̈�̊m�ۂɎ��s���܂���\n" @*/ )) ;
			return -1 ;
		}
		_MEMSET( UseFlag, 0, ( size_t )VertexNum ) ;

		IndexNum = PolygonNum * 3 ;
		for( i = 0 ; i < IndexNum ; i ++ )
		{
			UseFlag[ Indices[ i ] ] = 1 ;
		}
	}

	// �ڐ��Ə]�@���̏�����
	for( i = 0 ; i < VertexNum ; i ++ )
	{
		if( UseFlag[ i ] == 0 ) continue ;
		Vertex[ i ].binorm.x = 0.0f ;
		Vertex[ i ].binorm.y = 0.0f ;
		Vertex[ i ].binorm.z = 0.0f ;
		Vertex[ i ].tan.x = 0.0f ;
		Vertex[ i ].tan.y = 0.0f ;
		Vertex[ i ].tan.z = 0.0f ;
	}

	// �S�Ă̖ʂ̐������J��Ԃ�
	Index = Indices ;
	for( i = 0 ; i < PolygonNum ; i ++, Index += 3 )
	{
		Vert[ 0 ] = &Vertex[ Index[ 0 ] ] ;
		Vert[ 1 ] = &Vertex[ Index[ 1 ] ] ;
		Vert[ 2 ] = &Vertex[ Index[ 2 ] ] ;

		v1.x = Vert[ 1 ]->pos.x - Vert[ 0 ]->pos.x ;
		v1.y = Vert[ 1 ]->u - Vert[ 0 ]->u ;
		v1.z = Vert[ 1 ]->v - Vert[ 0 ]->v ;

		v2.x = Vert[ 2 ]->pos.x - Vert[ 0 ]->pos.x ;
		v2.y = Vert[ 2 ]->u - Vert[ 0 ]->u ;
		v2.z = Vert[ 2 ]->v - Vert[ 0 ]->v ;

		vt = VCross( v1, v2 ) ;
		du.x = 1.0f ;
		if( VDot( vt, vt ) >= 0.0000001f )
		{
			du.x = -vt.y / vt.x ;
			dv.x = -vt.z / vt.x ;
		}

		v1.x = Vert[ 1 ]->pos.y - Vert[ 0 ]->pos.y ;
		v2.x = Vert[ 2 ]->pos.y - Vert[ 0 ]->pos.y ;

		vt = VCross( v1, v2 ) ;
		du.y = 1.0f ;
		if( VDot( vt, vt ) >= 0.0000001f )
		{
			du.y = -vt.y / vt.x ;
			dv.y = -vt.z / vt.x ;
		}

		v1.x = Vert[ 1 ]->pos.z - Vert[ 0 ]->pos.z ;
		v2.x = Vert[ 2 ]->pos.z - Vert[ 0 ]->pos.z ;

		vt = VCross( v1, v2 ) ;
		du.z = 1.0f ;
		if( VDot( vt, vt ) >= 0.0000001f )
		{
			du.z = -vt.y / vt.x ;
			dv.z = -vt.z / vt.x ;
		}

		VectorAdd( &Vert[ 0 ]->tan, &Vert[ 0 ]->tan, &du ) ;
		VectorAdd( &Vert[ 1 ]->tan, &Vert[ 1 ]->tan, &du ) ;
		VectorAdd( &Vert[ 2 ]->tan, &Vert[ 2 ]->tan, &du ) ;

		VectorAdd( &Vert[ 0 ]->binorm, &Vert[ 0 ]->binorm, &dv ) ;
		VectorAdd( &Vert[ 1 ]->binorm, &Vert[ 1 ]->binorm, &dv ) ;
		VectorAdd( &Vert[ 2 ]->binorm, &Vert[ 2 ]->binorm, &dv ) ;
	}

	// �@���̎Z�o�Ɛ��K��
	for( i = 0 ; i < VertexNum ; i ++ )
	{
		if( UseFlag[ i ] == 0 ) continue ;

		vt = VNorm( Vertex[ i ].tan ) ;
		vn = VNorm( VCross( vt, Vertex[ i ].binorm ) ) ;
		vb = VNorm( VCross( vn, vt ) ) ;

		// ���K��
		Vertex[ i ].tan    = vt ;
		Vertex[ i ].binorm = vb ;
	}

	// �������̉��
	DXFREE( UseFlag ) ;

	// �I��
	return 0 ;
}


// �V�F�[�_�[���g���ăr���{�[�h��`�悷��
extern	int NS_DrawBillboard3DToShader( VECTOR Pos, float cx, float cy, float Size, float Angle, int GrHandle, int TransFlag, int TurnFlag )
{
	IMAGEDATA_HARD_DRAW *DrawTex ;
	IMAGEDATA_ORIG *Orig ;
	IMAGEDATA_HARD_VERT *TexVect ;
	DWORD DiffuseColor ;
	float SizeX ;
	float SizeY ;
	float f ;
	VECTOR SrcVec[ 4 ] ;
	float Sin = 0.0f ;
	float Cos = 1.0f ;
	float ScaleX ;
	float ScaleY ;
	IMAGEDATA *Image ;
	VERTEX3DSHADER DrawVect3D[ 6 ] ;

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE ) return -1 ;

	// ��`��ݒ肪����Ă���ꍇ�͉��������I��
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
		return 0 ;

	// �g�p����e�N�X�`���O�ԂƂ��ăZ�b�g
	NS_SetUseTextureToShader( 0, GrHandle ) ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
		UpdateMovie( Image->MovieHandle, FALSE ) ;
#endif

	Orig = Image->Orig ;

	if( Graphics_Hardware_CheckValid_PF() == 0 ) return -1 ;

	// �`����z��̃A�h���X���Z�b�g���Ă���
	DrawTex = Image->Hard.Draw ;

	// ���_�f�[�^���擾
	DiffuseColor = GetDiffuseColor() ;

	// �T�C�Y�ƍ��W�֌W�̎��O�v�Z
	SizeX = Size ;
	SizeY = Size * Image->HeightF / Image->WidthF ;

	ScaleX = SizeX / Image->WidthF ;
	ScaleY = SizeY / Image->HeightF ;
	cx *= Image->WidthF ;
	cy *= Image->HeightF ;

	// ��]����ꍇ�͉�]�l�����߂Ă���
	if( Angle != 0.0 )
	{
		_SINCOS_PLATFORM( (float)Angle, &Sin, &Cos ) ;
	}

	TexVect = DrawTex->Vertex ;

	// ��]����ꍇ�Ƃ��Ȃ��ꍇ�ŏ����𕪊�
	if( Angle != 0.0 )
	{
		// ���[�J�����W����
		SrcVec[2].x = SrcVec[0].x = ( -cx + TexVect[0].x ) * ScaleX ;
		SrcVec[3].x = SrcVec[1].x = ( -cx + TexVect[1].x ) * ScaleX ;

		SrcVec[1].y = SrcVec[0].y = ( -cy + Image->HeightF - TexVect[0].y ) * ScaleY ;
		SrcVec[3].y = SrcVec[2].y = ( -cy + Image->HeightF - TexVect[2].y ) * ScaleY ;

		// ��]�v�Z
		f             = SrcVec[ 0 ].x * Cos - SrcVec[ 0 ].y * Sin ;	
		SrcVec[ 0 ].y = SrcVec[ 0 ].x * Sin + SrcVec[ 0 ].y * Cos ;
		SrcVec[ 0 ].x = f ;

		f             = SrcVec[ 1 ].x * Cos - SrcVec[ 1 ].y * Sin ;	
		SrcVec[ 1 ].y = SrcVec[ 1 ].x * Sin + SrcVec[ 1 ].y * Cos ;
		SrcVec[ 1 ].x = f ;

		f             = SrcVec[ 2 ].x * Cos - SrcVec[ 2 ].y * Sin ;	
		SrcVec[ 2 ].y = SrcVec[ 2 ].x * Sin + SrcVec[ 2 ].y * Cos ;
		SrcVec[ 2 ].x = f ;

		f             = SrcVec[ 3 ].x * Cos - SrcVec[ 3 ].y * Sin ;	
		SrcVec[ 3 ].y = SrcVec[ 3 ].x * Sin + SrcVec[ 3 ].y * Cos ;
		SrcVec[ 3 ].x = f ;

		// �r���{�[�h���W�����[���h���W�֕ϊ�
		DrawVect3D[ 0 ].pos.x = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
		DrawVect3D[ 0 ].pos.y = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
		DrawVect3D[ 0 ].pos.z = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

		DrawVect3D[ 1 ].pos.x = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
		DrawVect3D[ 1 ].pos.y = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
		DrawVect3D[ 1 ].pos.z = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 1 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

		DrawVect3D[ 2 ].pos.x = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
		DrawVect3D[ 2 ].pos.y = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
		DrawVect3D[ 2 ].pos.z = SrcVec[ 2 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;

		DrawVect3D[ 3 ].pos.x = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] + Pos.x ;
		DrawVect3D[ 3 ].pos.y = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] + Pos.y ;
		DrawVect3D[ 3 ].pos.z = SrcVec[ 3 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + SrcVec[ 3 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] + Pos.z ;
	}
	else
	{
		VECTOR TempVecX[ 2 ], TempVecY[ 2 ] ;

		// ���[�J�����W����
		SrcVec[0].x = ( -cx + TexVect[0].x ) * ScaleX ;
		SrcVec[1].x = ( -cx + TexVect[1].x ) * ScaleX ;

		SrcVec[0].y = ( -cy + Image->HeightF - TexVect[0].y ) * ScaleY ;
		SrcVec[2].y = ( -cy + Image->HeightF - TexVect[2].y ) * ScaleY ;

		// �r���{�[�h���W�����[���h���W�֕ϊ�
		TempVecX[ 0 ].x = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + Pos.x ;
		TempVecX[ 0 ].y = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + Pos.y ;
		TempVecX[ 0 ].z = SrcVec[ 0 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + Pos.z ;

		TempVecX[ 1 ].x = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][0] + Pos.x ;
		TempVecX[ 1 ].y = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][1] + Pos.y ;
		TempVecX[ 1 ].z = SrcVec[ 1 ].x * GSYS.DrawSetting.BillboardMatrixF.m[0][2] + Pos.z ;

		TempVecY[ 0 ].x = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] ;
		TempVecY[ 0 ].y = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] ;
		TempVecY[ 0 ].z = SrcVec[ 0 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] ;

		TempVecY[ 1 ].x = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][0] ;
		TempVecY[ 1 ].y = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][1] ;
		TempVecY[ 1 ].z = SrcVec[ 2 ].y * GSYS.DrawSetting.BillboardMatrixF.m[1][2] ;

		DrawVect3D[ 0 ].pos.x = TempVecX[ 0 ].x + TempVecY[ 0 ].x ;
		DrawVect3D[ 0 ].pos.y = TempVecX[ 0 ].y + TempVecY[ 0 ].y ;
		DrawVect3D[ 0 ].pos.z = TempVecX[ 0 ].z + TempVecY[ 0 ].z ;

		DrawVect3D[ 1 ].pos.x = TempVecX[ 1 ].x + TempVecY[ 0 ].x ;
		DrawVect3D[ 1 ].pos.y = TempVecX[ 1 ].y + TempVecY[ 0 ].y ;
		DrawVect3D[ 1 ].pos.z = TempVecX[ 1 ].z + TempVecY[ 0 ].z ;

		DrawVect3D[ 2 ].pos.x = TempVecX[ 0 ].x + TempVecY[ 1 ].x ;
		DrawVect3D[ 2 ].pos.y = TempVecX[ 0 ].y + TempVecY[ 1 ].y ;
		DrawVect3D[ 2 ].pos.z = TempVecX[ 0 ].z + TempVecY[ 1 ].z ;

		DrawVect3D[ 3 ].pos.x = TempVecX[ 1 ].x + TempVecY[ 1 ].x ;
		DrawVect3D[ 3 ].pos.y = TempVecX[ 1 ].y + TempVecY[ 1 ].y ;
		DrawVect3D[ 3 ].pos.z = TempVecX[ 1 ].z + TempVecY[ 1 ].z ;
	}

	DrawVect3D[4].pos = DrawVect3D[2].pos ;
	DrawVect3D[5].pos = DrawVect3D[1].pos ;

	*( ( DWORD * )&DrawVect3D[0].dif ) = 
	*( ( DWORD * )&DrawVect3D[1].dif ) = 
	*( ( DWORD * )&DrawVect3D[2].dif ) = 
	*( ( DWORD * )&DrawVect3D[3].dif ) =
	*( ( DWORD * )&DrawVect3D[4].dif ) =
	*( ( DWORD * )&DrawVect3D[5].dif ) = DiffuseColor ;

	if( TurnFlag )
	{
		DrawVect3D[5].u = DrawVect3D[3].u = DrawVect3D[1].u = TexVect[0].u ;
		DrawVect3D[4].u = DrawVect3D[2].u = DrawVect3D[0].u = TexVect[1].u ;
	}
	else
	{
		DrawVect3D[4].u = DrawVect3D[2].u = DrawVect3D[0].u = TexVect[0].u ;
		DrawVect3D[5].u = DrawVect3D[3].u = DrawVect3D[1].u = TexVect[1].u ;
	}
	DrawVect3D[5].v = DrawVect3D[1].v = DrawVect3D[0].v = TexVect[0].v ;
	DrawVect3D[4].v = DrawVect3D[3].v = DrawVect3D[2].v = TexVect[2].v ;

	DrawVect3D[0].spos.x = 0.0f ;
	DrawVect3D[0].spos.y = 0.0f ;
	DrawVect3D[0].spos.z = 0.0f ;
	DrawVect3D[0].norm.x = 0.0f ;
	DrawVect3D[0].norm.y = 0.0f ;
	DrawVect3D[0].norm.z = 0.0f ;
	DrawVect3D[0].tan.x = 0.0f ;
	DrawVect3D[0].tan.y = 0.0f ;
	DrawVect3D[0].tan.z = 0.0f ;
	DrawVect3D[0].binorm.x = 0.0f ;
	DrawVect3D[0].binorm.y = 0.0f ;
	DrawVect3D[0].binorm.z = 0.0f ;
	DrawVect3D[0].spc.r = 0 ;
	DrawVect3D[0].spc.g = 0 ;
	DrawVect3D[0].spc.b = 0 ;
	DrawVect3D[0].spc.a = 0 ;
	DrawVect3D[0].su = 0.0f ;
	DrawVect3D[0].sv = 0.0f ;
	DrawVect3D[1].spos		= DrawVect3D[0].spos ;
	DrawVect3D[1].norm		= DrawVect3D[0].norm ;
	DrawVect3D[1].tan		= DrawVect3D[0].tan ;
	DrawVect3D[1].binorm	= DrawVect3D[0].binorm ;
	DrawVect3D[1].spc		= DrawVect3D[0].spc ;
	DrawVect3D[1].su		= 0.0f ;
	DrawVect3D[1].sv		= 0.0f ;
	DrawVect3D[2].spos		= DrawVect3D[0].spos ;
	DrawVect3D[2].norm		= DrawVect3D[0].norm ;
	DrawVect3D[2].tan		= DrawVect3D[0].tan ;
	DrawVect3D[2].binorm	= DrawVect3D[0].binorm ;
	DrawVect3D[2].spc		= DrawVect3D[0].spc ;
	DrawVect3D[2].su		= 0.0f ;
	DrawVect3D[2].sv		= 0.0f ;
	DrawVect3D[3].spos		= DrawVect3D[0].spos ;
	DrawVect3D[3].norm		= DrawVect3D[0].norm ;
	DrawVect3D[3].tan		= DrawVect3D[0].tan ;
	DrawVect3D[3].binorm	= DrawVect3D[0].binorm ;
	DrawVect3D[3].spc		= DrawVect3D[0].spc ;
	DrawVect3D[3].su		= 0.0f ;
	DrawVect3D[3].sv		= 0.0f ;
	DrawVect3D[4].spos		= DrawVect3D[0].spos ;
	DrawVect3D[4].norm		= DrawVect3D[0].norm ;
	DrawVect3D[4].tan		= DrawVect3D[0].tan ;
	DrawVect3D[4].binorm	= DrawVect3D[0].binorm ;
	DrawVect3D[4].spc		= DrawVect3D[0].spc ;
	DrawVect3D[4].su		= 0.0f ;
	DrawVect3D[4].sv		= 0.0f ;
	DrawVect3D[5].spos		= DrawVect3D[0].spos ;
	DrawVect3D[5].norm		= DrawVect3D[0].norm ;
	DrawVect3D[5].tan		= DrawVect3D[0].tan ;
	DrawVect3D[5].binorm	= DrawVect3D[0].binorm ;
	DrawVect3D[5].spc		= DrawVect3D[0].spc ;
	DrawVect3D[5].su		= 0.0f ;
	DrawVect3D[5].sv		= 0.0f ;

	NS_DrawPrimitive3DToShader( DrawVect3D, 6, DX_PRIMTYPE_TRIANGLELIST ) ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[���g���ĂQ�c�|���S����`�悷��
extern	int			NS_DrawPolygon2DToShader( const VERTEX2DSHADER *Vertex, int PolygonNum )
{
	return NS_DrawPrimitive2DToShader( Vertex, PolygonNum * 3, DX_PRIMTYPE_TRIANGLELIST ) ;
}

// �V�F�[�_�[���g���ĂR�c�|���S����`�悷��
extern	int			NS_DrawPolygon3DToShader( const VERTEX3DSHADER *Vertex, int PolygonNum )
{
	return NS_DrawPrimitive3DToShader( Vertex, PolygonNum * 3, DX_PRIMTYPE_TRIANGLELIST ) ;
}

// �V�F�[�_�[���g���ĂQ�c�|���S����`�悷��(�C���f�b�N�X)
extern	int			NS_DrawPolygonIndexed2DToShader( const VERTEX2DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int PolygonNum )
{
	return NS_DrawPrimitiveIndexed2DToShader( Vertex, VertexNum, Indices, PolygonNum * 3, DX_PRIMTYPE_TRIANGLELIST ) ;
}

// �V�F�[�_�[���g���ĂR�c�|���S����`�悷��(�C���f�b�N�X)
extern	int			NS_DrawPolygonIndexed3DToShader( const VERTEX3DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int PolygonNum )
{
	return NS_DrawPrimitiveIndexed3DToShader( Vertex, VertexNum, Indices, PolygonNum * 3, DX_PRIMTYPE_TRIANGLELIST ) ;
}

// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��
extern	int			NS_DrawPrimitive2DToShader( const VERTEX2DSHADER *Vertex, int VertexNum, int PrimitiveType )
{
	return Graphics_Hardware_DrawPrimitive2DToShader_PF( Vertex, VertexNum, PrimitiveType ) ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��
extern	int			NS_DrawPrimitive3DToShader( const VERTEX3DSHADER *Vertex, int VertexNum, int PrimitiveType )
{
	return Graphics_Hardware_DrawPrimitive3DToShader_PF( Vertex, VertexNum, PrimitiveType ) ;
}

// �V�F�[�_�[���g���ĂQ�c�v���~�e�B�u��`�悷��(�C���f�b�N�X)
extern	int			NS_DrawPrimitiveIndexed2DToShader( const VERTEX2DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType )
{
	return Graphics_Hardware_DrawPrimitiveIndexed2DToShader_PF( Vertex, VertexNum, Indices, IndexNum, PrimitiveType ) ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��(�C���f�b�N�X)
extern	int			NS_DrawPrimitiveIndexed3DToShader( const VERTEX3DSHADER *Vertex, int VertexNum, const unsigned short *Indices, int IndexNum, int PrimitiveType )
{
	return Graphics_Hardware_DrawPrimitiveIndexed3DToShader_PF( Vertex, VertexNum, Indices, IndexNum, PrimitiveType ) ;
}

// �V�F�[�_�[���g���ĂR�c�|���S����`�悷��( ���_�o�b�t�@�g�p�� )
extern	int			NS_DrawPolygon3DToShader_UseVertexBuffer( int VertexBufHandle )
{
	return NS_DrawPrimitive3DToShader_UseVertexBuffer( VertexBufHandle, DX_PRIMTYPE_TRIANGLELIST ) ;
}

// �V�F�[�_�[���g���ĂR�c�|���S����`�悷��( ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�g�p�� )
extern	int			NS_DrawPolygonIndexed3DToShader_UseVertexBuffer( int VertexBufHandle, int IndexBufHandle )
{
	return NS_DrawPrimitiveIndexed3DToShader_UseVertexBuffer( VertexBufHandle, IndexBufHandle, DX_PRIMTYPE_TRIANGLELIST ) ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�g�p�� )
extern	int			NS_DrawPrimitive3DToShader_UseVertexBuffer( int VertexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;

	// �G���[����
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
	{
		return -1 ;
	}

	return NS_DrawPrimitive3DToShader_UseVertexBuffer2( VertexBufHandle, PrimitiveType, 0, VertexBuffer->Num ) ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�g�p�� )
extern	int			NS_DrawPrimitive3DToShader_UseVertexBuffer2( int VertexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int StartVertex, int UseVertexNum )
{
	return Graphics_Hardware_DrawPrimitive3DToShader_UseVertexBuffer2_PF( VertexBufHandle, PrimitiveType, StartVertex, UseVertexNum ) ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�g�p�� )
extern	int			NS_DrawPrimitiveIndexed3DToShader_UseVertexBuffer( int VertexBufHandle, int IndexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */ )
{
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;
	INDEXBUFFERHANDLEDATA  *IndexBuffer ;

	// �G���[����
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
	{
		return -1 ;
	}
	if( INDEXBUFFERCHK( IndexBufHandle, IndexBuffer ) )
	{
		return -1 ;
	}

	return NS_DrawPrimitiveIndexed3DToShader_UseVertexBuffer2( VertexBufHandle, IndexBufHandle, PrimitiveType, 0, 0, VertexBuffer->Num, 0, IndexBuffer->Num ) ;
}

// �V�F�[�_�[���g���ĂR�c�v���~�e�B�u��`�悷��( ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�g�p�� )
extern	int			NS_DrawPrimitiveIndexed3DToShader_UseVertexBuffer2( int VertexBufHandle, int IndexBufHandle, int PrimitiveType /* DX_PRIMTYPE_TRIANGLELIST �� */, int BaseVertex, int StartVertex, int UseVertexNum, int StartIndex, int UseIndexNum )
{
	return Graphics_Hardware_DrawPrimitiveIndexed3DToShader_UseVertexBuffer2_PF( VertexBufHandle, IndexBufHandle, PrimitiveType, BaseVertex, StartVertex, UseVertexNum, StartIndex, UseIndexNum ) ;
}






















// �V�F�[�_�[�p�萔�o�b�t�@�֌W�֐�

// �S�ẴV�F�[�_�[�p�萔�o�b�t�@�n���h�����폜����
extern int NS_InitShaderConstantBuffer( void )
{
	if( GSYS.InitializeFlag == FALSE ) return 0 ;

	return AllHandleSub( DX_HANDLETYPE_SHADER_CONSTANT_BUFFER ) ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h��������������
extern int NS_CreateShaderConstantBuffer(	int BufferSize )
{
	return Graphics_ShaderConstantBuffer_Create( BufferSize, GetASyncLoadFlag() ) ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̌�n��
extern int NS_DeleteShaderConstantBuffer( int SConstBufHandle )
{
	return SubHandle( SConstBufHandle ) ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�̃A�h���X���擾����
extern void *NS_GetBufferShaderConstantBuffer(	int SConstBufHandle )
{
	SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer ;

	// �G���[����
	if( SHADERCONSTANTBUFFERCHK( SConstBufHandle, ShaderConstantBuffer ) )
		return NULL ;

	// ���ˑ�����
	return Graphics_Hardware_ShaderConstantBuffer_GetBuffer_PF( ShaderConstantBuffer ) ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@�ւ̕ύX��K�p����
extern int NS_UpdateShaderConstantBuffer( int SConstBufHandle )
{
	SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer ;

	// �G���[����
	if( SHADERCONSTANTBUFFERCHK( SConstBufHandle, ShaderConstantBuffer ) )
		return -1 ;

	// ���ˑ�����
	return Graphics_Hardware_ShaderConstantBuffer_Update_PF( ShaderConstantBuffer ) ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̒萔�o�b�t�@���w��̃V�F�[�_�[�̎w��̃X���b�g�ɃZ�b�g����
extern int NS_SetShaderConstantBuffer( int SConstBufHandle, int TargetShader /* DX_SHADERTYPE_VERTEX �Ȃ� */ , int Slot )
{
	SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer ;

	// �G���[����
	if( SHADERCONSTANTBUFFERCHK( SConstBufHandle, ShaderConstantBuffer ) )
		return -1 ;

	// ���ˑ�����
	return Graphics_Hardware_ShaderConstantBuffer_Set_PF( ShaderConstantBuffer, TargetShader, Slot ) ;
}
























#ifndef DX_NON_MOVIE

// ���[�r�[�O���t�B�b�N�֌W�֐�
// ����t�@�C���̍Đ�
extern int NS_PlayMovie( const TCHAR *FileName, int ExRate, int PlayType )
{
#ifdef UNICODE
	return PlayMovie_WCHAR_T(
		FileName, ExRate, PlayType
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = PlayMovie_WCHAR_T(
		UseFileNameBuffer, ExRate, PlayType
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// ���[�r�[�O���t�B�b�N�֌W�֐�
// ����t�@�C���̍Đ�
extern int PlayMovie_WCHAR_T( const wchar_t *FileName, int ExRate, int PlayType )
{
	int MovieHandle ;
	LOADGRAPH_GPARAM GParam ;
	int x1, y1, x2, y2, w, h ;
	int DrawScreen ;
	int DrawScreenSurface ;
#ifndef DX_NON_INPUT
	int KeyOffFlag ;
	int State ;
#endif // DX_NON_INPUT
	int ScreenW, ScreenH ;

	Graphics_Image_InitLoadGraphGParam( &GParam ) ;

	MovieHandle = Graphics_Image_LoadBmpToGraph_UseGParam( &GParam, FALSE, -1, FileName, FALSE, FALSE, DX_MOVIESURFACE_NORMAL, FALSE ) ;
	if( MovieHandle < 0 )
		return -1 ;

#ifndef DX_NON_INPUT
	KeyOffFlag = NS_CheckHitKeyAll() ;
#endif // DX_NON_INPUT
	if( MovieHandle != -1 )
	{
		NS_PlayMovieToGraph( MovieHandle ) ;

		NS_GetDrawScreenSize( &ScreenW, &ScreenH ) ;
		NS_GetGraphSize( MovieHandle, &w, &h ) ;
		x1 = ( ScreenW - w * ExRate ) >> 1 ;
		y1 = ( ScreenH - h * ExRate ) >> 1 ;

		x2 = x1 + w * ExRate ;
		y2 = y1 + h * ExRate ;

		DrawScreen = GSYS.DrawSetting.TargetScreen[ 0 ] ;
		DrawScreenSurface = GSYS.DrawSetting.TargetScreenSurface[ 0 ] ;

		NS_SetDrawScreen( DX_SCREEN_BACK ) ;
		NS_ClearDrawScreen() ;

		while( NS_GetMovieStateToGraph( MovieHandle ) == 1 )
		{
//			NS_DrawBox( 0,0,32,32, GetColor( 255,255,255 ), TRUE ) ;
			NS_DrawExtendGraph( x1, y1, x2, y2, MovieHandle, FALSE ) ;

			NS_ScreenFlip() ;

#ifndef DX_NON_INPUT
			State = NS_CheckHitKeyAll() ;
			if( State == FALSE ) 
			{
				KeyOffFlag = FALSE ;
			}
#endif // DX_NON_INPUT

			if( ProcessMessage() != 0 
#ifndef DX_NON_INPUT
				|| ( PlayType == DX_MOVIEPLAYTYPE_BCANCEL &&
#ifdef __WINDOWS__
				GetActiveFlag() && 
#endif // __WINDOWS__
				State && KeyOffFlag == FALSE )
#endif // DX_NON_INPUT
				) break ;
		}
		NS_DeleteGraph( MovieHandle ) ;

		NS_SetRenderTargetToShader( 0, DrawScreen, DrawScreenSurface ) ;
	}

	// �I��
	return 0;
}

// ���[�r�[���J��
extern int NS_OpenMovieToGraph( const TCHAR *FileName, int FullColor )
{
	return NS_LoadBmpToGraph( FileName, FALSE, FALSE, FullColor ? DX_MOVIESURFACE_FULLCOLOR : DX_MOVIESURFACE_NORMAL );
}

// ���[�r�[���J��
extern int OpenMovieToGraph_WCHAR_T( const wchar_t *FileName, int FullColor )
{
	return LoadBmpToGraph_WCHAR_T( FileName, FALSE, FALSE, FullColor ? DX_MOVIESURFACE_FULLCOLOR : DX_MOVIESURFACE_NORMAL );
}

// ���[�r�[�O���t�B�b�N�Ɋ܂܂�郀�[�r�[�̍Đ����J�n����
extern 	int		NS_PlayMovieToGraph( int GraphHandle, int PlayType, int SysPlay  )
{
	IMAGEDATA *Image ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return  0 ;

	return PlayMovie_( Image->MovieHandle, PlayType, SysPlay ) ;
}

// ���[�r�[�O���t�B�b�N�Ɋ܂܂�郀�[�r�[�̍Đ����X�g�b�v����
extern 	int		NS_PauseMovieToGraph( int GraphHandle, int SysPause  )
{
	IMAGEDATA *Image ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return  0 ;

	return PauseMovie( Image->MovieHandle, SysPause ) ;
}

// ���[�r�[�̃t���[����i�߂�A�߂����Ƃ͏o���Ȃ�( ���[�r�[����~��ԂŁA���� Ogg Theora �̂ݗL�� )
extern	int		NS_AddMovieFrameToGraph( int GraphHandle, unsigned int FrameNum )
{
	IMAGEDATA *Image ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return 0 ;

	return AddMovieFrame( Image->MovieHandle, FrameNum ) ;
}

// ���[�r�[�̍Đ��ʒu��ݒ肷��(�~���b�P��)
extern	int		NS_SeekMovieToGraph( int GraphHandle, int Time )
{
	IMAGEDATA *Image ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return 0 ;

	return SeekMovie( Image->MovieHandle, Time ) ;
}

// ����t�@�C���̍Đ����x��ݒ肷��( 1.0 = ���{��  2.0 = �Q�{�� )�A�ꕔ�̃t�@�C���t�H�[�}�b�g�݂̂ŗL���ȋ@�\�ł�
extern	int		NS_SetPlaySpeedRateMovieToGraph( int GraphHandle, double SpeedRate )
{
	IMAGEDATA *Image ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return 0 ;

	return SetPlaySpeedRateMovie( Image->MovieHandle, SpeedRate ) ;
}

// ���[�r�[�̍Đ���Ԃ𓾂�
extern 	int		NS_GetMovieStateToGraph( int GraphHandle )
{
	IMAGEDATA *Image ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return 0 ;

	return GetMovieState( Image->MovieHandle ) ;
}

// ���[�r�[�̃{�����[�����Z�b�g����(0�`10000)
extern	int		NS_SetMovieVolumeToGraph( int Volume, int GraphHandle )	
{
	IMAGEDATA *Image ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return 0 ;

	return SetMovieVolume( Volume, Image->MovieHandle ) ;
}

// ���[�r�[�̃{�����[�����Z�b�g����(0�`255)
extern	int		NS_ChangeMovieVolumeToGraph( int Volume, int GraphHandle )
{
	IMAGEDATA *Image ;
	int temp ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return 0 ;

	if( Volume < 0 )
	{
		Volume = 0 ;
	}
	else
	if( Volume > 255 )
	{
		Volume = 255 ;
	}

	if( Volume == 0 )
	{
		temp = 0 ;
	}
	else
	{
		if( SoundSysData.OldVolumeTypeFlag )
		{
			temp = _DTOL( _LOG10( Volume / 255.0f ) * 10.0 * 100.0 ) + 10000;
		}
		else
		{
			temp = _DTOL( _LOG10( Volume / 255.0f ) * 50.0 * 100.0 ) + 10000;
		}
	}

	return SetMovieVolume( temp, Image->MovieHandle ) ;
}

// ���[�r�[�̊�{�C���[�W�f�[�^���擾����
extern	const BASEIMAGE *NS_GetMovieBaseImageToGraph( int GraphHandle, int *ImageUpdateFlag, int ImageUpdateFlagSetOnly )
{
	IMAGEDATA *Image ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return NULL ;

	if( Image->MovieHandle < 0 ) return  NULL ;

	return GetMovieBaseImage( Image->MovieHandle, ImageUpdateFlag, ImageUpdateFlagSetOnly ) ;
}

// ���[�r�[�̑��t���[�����𓾂�( Ogg Theora �ł̂ݗL�� )
extern	int		NS_GetMovieTotalFrameToGraph( int GraphHandle )
{
	IMAGEDATA *Image ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return 0 ;

	return GetMovieTotalFrame( Image->MovieHandle ) ;
}

// ���[�r�[�̍Đ��ʒu���擾����(�~���b�P��)
extern	int		NS_TellMovieToGraph( int GraphHandle )
{
	IMAGEDATA *Image ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return  0 ;

	return TellMovie( Image->MovieHandle ) ;
}

// ���[�r�[�̍Đ��ʒu���擾����(�t���[���P��)
extern	int		NS_TellMovieToGraphToFrame( int GraphHandle )
{
	IMAGEDATA *Image ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return  0 ;

	return TellMovieToFrame( Image->MovieHandle ) ;
}

// ���[�r�[�̍Đ��ʒu��ݒ肷��(�t���[���P��)
extern	int		NS_SeekMovieToGraphToFrame( int GraphHandle, int Frame )
{
	IMAGEDATA *Image ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return  0 ;

	return SeekMovieToFrame( Image->MovieHandle, Frame ) ;
}

// ���[�r�[�̂P�t���[��������̎��Ԃ𓾂�
extern	LONGLONG NS_GetOneFrameTimeMovieToGraph( int GraphHandle )
{
	IMAGEDATA *Image ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return  0 ;

	return GetOneFrameTimeMovie( Image->MovieHandle ) ;
}

// ���[�r�[�̃C���[�W���Ō�ɍX�V�������Ԃ𓾂�(�~���b�P��)
extern int NS_GetLastUpdateTimeMovieToGraph( int GraphHandle )
{
	IMAGEDATA *Image ;
	MOVIEGRAPH *Movie ;

	if( GRAPHCHK( GraphHandle, Image ) )
		return -1 ;

	if( Image->MovieHandle < 0 ) return  -1 ;

	Movie = GetMovieData( Image->MovieHandle ) ;
	if( Movie == NULL ) return -1 ;

	return Movie->RefreshTime ;
}

#endif // DX_NON_MOVIE



















// �J�����֌W�֐�

// �J������ Near�N���b�v�ʂ� Far�N���b�v�ʂ̋�����ݒ肷��
extern int NS_SetCameraNearFar( float Near, float Far )
{
	return NS_SetCameraNearFarD( Near, Far ) ;
}

// �J������ Near�N���b�v�ʂ� Far�N���b�v�ʂ̋�����ݒ肷��
extern int NS_SetCameraNearFarD( double Near, double Far )
{
	// �p�����[�^�̕ۑ�
	GSYS.DrawSetting.ProjNear = Near ;
	GSYS.DrawSetting.ProjFar = Far ;

	// �ˉe�s��̍X�V
	Graphics_DrawSetting_RefreshProjectionMatrix() ;

	// �I��
	return 0 ;
}

// �J�����̐ݒ������A���_�A�����_�A�A�b�v�x�N�g���͂x����
extern int NS_SetCameraPositionAndTarget_UpVecY( VECTOR Position, VECTOR Target )
{
	return NS_SetCameraPositionAndTarget_UpVecYD( VConvFtoD( Position ), VConvFtoD( Target ) ) ;
}

// �J�����̐ݒ������A���_�A�����_�A�A�b�v�x�N�g���͂x����
extern int NS_SetCameraPositionAndTarget_UpVecYD( VECTOR_D Position, VECTOR_D Target )
{
	VECTOR_D Up = { 0.0, 1.0, 0.0 }, Side, Dir ;
	MATRIX_D LookAt ;

	// �Z�b�g�A�b�v�^�C�v���Z�b�g
	GSYS.Camera.SetupType = CAMERA_SETUPTYPE_POS_TARG ;

	// ���_�ƒ����_��ۑ�
	GSYS.Camera.Position = Position ;
	GSYS.Camera.Target   = Target ;

	// �A�b�v�x�N�g�����Z�o
	VectorSubD( &Dir, &GSYS.Camera.Target, &GSYS.Camera.Position ) ;
	VectorOuterProductD( &Side, &Dir, &Up ) ;
	VectorOuterProductD( &GSYS.Camera.Up, &Side, &Dir ) ;
	VectorNormalizeD( &GSYS.Camera.Up, &GSYS.Camera.Up ) ;

	// �s��̍쐬
	CreateLookAtMatrixD( &LookAt, &GSYS.Camera.Position, &GSYS.Camera.Target, &GSYS.Camera.Up ) ;

	// �r���[�s��ɃZ�b�g
	NS_SetTransformToViewD( &LookAt ) ;
	GSYS.Camera.Matrix = LookAt ;

	// ��]�p�x���Z�o
	Graphics_Camera_CalcCameraRollViewMatrix() ;

	// �I��
	return 0 ;
}

// �J�����̐ݒ������A���_�A�����_�A�A�b�v�x�N�g���w���
extern int NS_SetCameraPositionAndTargetAndUpVec( VECTOR Position, VECTOR Target, VECTOR Up )
{
	return NS_SetCameraPositionAndTargetAndUpVecD( VConvFtoD( Position ), VConvFtoD( Target ), VConvFtoD( Up ) ) ;
}

// �J�����̐ݒ������A���_�A�����_�A�A�b�v�x�N�g���w���
extern int NS_SetCameraPositionAndTargetAndUpVecD( VECTOR_D Position, VECTOR_D Target, VECTOR_D Up )
{
	VECTOR_D Side, Dir ;
	MATRIX_D LookAt ;

	// �Z�b�g�A�b�v�^�C�v���Z�b�g
	GSYS.Camera.SetupType = CAMERA_SETUPTYPE_POS_TARG_UP ;

	// ���_�ƒ����_��ۑ�
	GSYS.Camera.Position = Position ;
	GSYS.Camera.Target   = Target ;

	// �A�b�v�x�N�g�����Z�o
	VectorSubD( &Dir, &Target, &Position ) ;
	VectorOuterProductD( &Side, &Dir, &Up ) ;
	VectorOuterProductD( &GSYS.Camera.Up, &Side, &Dir ) ;
	VectorNormalizeD( &GSYS.Camera.Up, &GSYS.Camera.Up ) ;

	// �s��̍쐬
	CreateLookAtMatrixD( &LookAt, &GSYS.Camera.Position, &GSYS.Camera.Target, &GSYS.Camera.Up ) ;

	// �r���[�s��ɃZ�b�g
	NS_SetTransformToViewD( &LookAt ) ;
	GSYS.Camera.Matrix = LookAt ;

	// ��]�p�x���Z�o
	Graphics_Camera_CalcCameraRollViewMatrix() ;

	// �I��
	return 0 ;
}

// �J�����̐ݒ������A������]�p�x�A������]�p�x�A�P���]�p�x�A���_�w���
extern int NS_SetCameraPositionAndAngle( VECTOR Position, float VRotate, float HRotate, float TRotate )
{
	return NS_SetCameraPositionAndAngleD( VConvFtoD( Position ), VRotate, HRotate, TRotate ) ;
}

// �J�����̐ݒ������A������]�p�x�A������]�p�x�A�P���]�p�x�A���_�w���
extern int NS_SetCameraPositionAndAngleD( VECTOR_D Position, double VRotate, double HRotate, double TRotate )
{
	VECTOR_D ZVec, YVec, XVec, TVec, Target ;
	MATRIX_D LookAt ;
	double Sin, Cos ;

	// �Z�b�g�A�b�v�^�C�v���Z�b�g
	GSYS.Camera.SetupType = CAMERA_SETUPTYPE_POS_ANGLE ;

	// ������]�p�x�A������]�p�x�A�P���]�p�x�A���_��ۑ�����
	GSYS.Camera.HRotate = HRotate ;
	GSYS.Camera.VRotate = VRotate ;
	GSYS.Camera.TRotate = TRotate ;
	GSYS.Camera.Position = Position ;

	// �s��̍쐬
	_SINCOSD( VRotate, &Sin, &Cos ) ;
	ZVec.x  =  0.0 ;
	ZVec.y  = -Sin ;
	ZVec.z  =  Cos ;

	YVec.z  =  Sin ;
	YVec.y  =  Cos ;
	YVec.x  =  0.0 ;

	_SINCOSD( HRotate, &Sin, &Cos ) ;
	ZVec.x  =  Sin * ZVec.z ;
	ZVec.z *=  Cos ;

	YVec.x  =  Sin * YVec.z ;
	YVec.z *=  Cos ;

	XVec.x  =  Cos ;
	XVec.y  =  0.0 ;
	XVec.z  = -Sin ;

	_SINCOSD( TRotate, &Sin, &Cos ) ;
	TVec.x = -Sin * XVec.x + Cos * YVec.x ;
	TVec.y = -Sin * XVec.y + Cos * YVec.y ;
	TVec.z = -Sin * XVec.z + Cos * YVec.z ;

	VectorAddD( &Target, &Position, &ZVec ) ;
	CreateLookAtMatrixD( &LookAt, &Position, &Target, &TVec ) ;

	// �r���[�s��ɃZ�b�g
	NS_SetTransformToViewD( &LookAt ) ;
	GSYS.Camera.Matrix = LookAt ;

	// ���_�ƒ����_��ۑ�
	GSYS.Camera.Position = Position ;
	GSYS.Camera.Target   = Target ;
	GSYS.Camera.Up       = TVec ;

//	Graphics_Camera_CalcCameraRollViewMatrix() ;

	// �I��
	return 0 ;
}

// �r���[�s��𒼐ڐݒ肷��
extern int NS_SetCameraViewMatrix( MATRIX ViewMatrix )
{
	MATRIX_D ViewMatrixD ;

	ConvertMatrixFtoD( &ViewMatrixD, &ViewMatrix ) ;

	// �r���[�s��ɃZ�b�g
	NS_SetCameraViewMatrixD( ViewMatrixD ) ;

	// �I��
	return 0 ;
}

// �r���[�s��𒼐ڐݒ肷��
extern int NS_SetCameraViewMatrixD( MATRIX_D ViewMatrix )
{
	// �Z�b�g�A�b�v�^�C�v���Z�b�g
	GSYS.Camera.SetupType = CAMERA_SETUPTYPE_MATRIX ;

	// �p�����[�^��ۑ�
	GSYS.Camera.Matrix = ViewMatrix ;

	// �r���[�s��ɃZ�b�g
	NS_SetTransformToViewD( &ViewMatrix ) ;

	// �I��
	return 0 ;
}

// ��ʏ�ɂ�����J���������Ă���f���̒��S�̍��W��ݒ肷��
extern int NS_SetCameraScreenCenter( float x, float y )
{
	return NS_SetCameraScreenCenterD( x, y ) ;
}

// ��ʏ�ɂ�����J���������Ă���f���̒��S�̍��W��ݒ肷��
extern int NS_SetCameraScreenCenterD( double x, double y )
{
	MATRIX_D ViewportMatrix ;

	// �l��ۑ�
	GSYS.Camera.ScreenCenterX = x ;
	GSYS.Camera.ScreenCenterY = y ;

	// �r���[�|�[�g�s��̍쐬
	CreateViewportMatrixD(
		&ViewportMatrix,
		x, y, 
		GSYS.DrawSetting.DrawSizeX * GSYS.DrawSetting.Draw3DScale,
		GSYS.DrawSetting.DrawSizeY * GSYS.DrawSetting.Draw3DScale ) ;

	// �r���[�|�[�s��Ƃ��ăZ�b�g
	NS_SetTransformToViewportD( &ViewportMatrix ) ;

	// �I��
	return 0 ;
}






// ���ߖ@�J�������Z�b�g�A�b�v����
extern int NS_SetupCamera_Perspective( float Fov )
{
	return NS_SetupCamera_PerspectiveD( Fov ) ;
}

// ���ߖ@�J�������Z�b�g�A�b�v����
extern int NS_SetupCamera_PerspectiveD( double Fov )
{
	// �p�����[�^�̕ۑ�
	GSYS.DrawSetting.ProjectionMatrixMode = 0 ;
	GSYS.DrawSetting.ProjFov = Fov ;

	// �ˉe�s��̍X�V
	Graphics_DrawSetting_RefreshProjectionMatrix() ;

	// �I��
	return 0 ;
}

// ���ˉe�J�������Z�b�g�A�b�v����
extern int NS_SetupCamera_Ortho( float Size )
{
	return NS_SetupCamera_OrthoD( Size ) ;
}

// ���ˉe�J�������Z�b�g�A�b�v����
extern int NS_SetupCamera_OrthoD( double Size )
{
	// �p�����[�^�̕ۑ�
	GSYS.DrawSetting.ProjectionMatrixMode = 1 ;
	GSYS.DrawSetting.ProjSize = Size ;

	// �ˉe�s��̍X�V
	Graphics_DrawSetting_RefreshProjectionMatrix() ;

	// �I��
	return 0 ;
}

// �ˉe�s���ݒ肷��
extern int NS_SetupCamera_ProjectionMatrix( MATRIX ProjectionMatrix )
{
	MATRIX_D ProjectionMatrixD ;

	ConvertMatrixFtoD( &ProjectionMatrixD, &ProjectionMatrix ) ;

	return NS_SetupCamera_ProjectionMatrixD( ProjectionMatrixD ) ;
}

// �ˉe�s���ݒ肷��
extern int NS_SetupCamera_ProjectionMatrixD( MATRIX_D ProjectionMatrix )
{
	// �p�����[�^�̕ۑ�
	GSYS.DrawSetting.ProjectionMatrixMode = 2 ;
	GSYS.DrawSetting.ProjMatrix = ProjectionMatrix ;

	// �ˉe�s��̍X�V
	Graphics_DrawSetting_RefreshProjectionMatrix() ;

	// �I��
	return 0 ;
}

// �J�����̃h�b�g�A�X�y�N�g���ݒ肷��
extern int NS_SetCameraDotAspect( float DotAspect )
{
	return NS_SetCameraDotAspectD( DotAspect ) ;
}

// �J�����̃h�b�g�A�X�y�N�g���ݒ肷��
extern int NS_SetCameraDotAspectD( double DotAspect )
{
	// �p�����[�^�̕ۑ�
	GSYS.DrawSetting.ProjDotAspect = DotAspect ;

	// �ˉe�s��̍X�V
	Graphics_DrawSetting_RefreshProjectionMatrix() ;

	// �I��
	return 0 ;
}





// �w��̍��W���J�����̎��E�ɓ����Ă��邩�ǂ����𔻒肷��( �߂�l TRUE:���E�ɓ����Ă��Ȃ�  FALSE:���E�ɓ����Ă��� )
extern int NS_CheckCameraViewClip( VECTOR CheckPos )
{
	if( GSYS.DrawSetting.ClippingPlane[ 0 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 0 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 0 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 0 ].w < 0.0f ||
		GSYS.DrawSetting.ClippingPlane[ 1 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 1 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 1 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 1 ].w < 0.0f ||
		GSYS.DrawSetting.ClippingPlane[ 2 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 2 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 2 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 2 ].w < 0.0f ||
		GSYS.DrawSetting.ClippingPlane[ 3 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 3 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 3 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 3 ].w < 0.0f ||
		GSYS.DrawSetting.ClippingPlane[ 4 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 4 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 4 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 4 ].w < 0.0f ||
		GSYS.DrawSetting.ClippingPlane[ 5 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 5 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 5 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 5 ].w < 0.0f )
		return TRUE ;

	return FALSE ;
}

// �w��̍��W���J�����̎��E�ɓ����Ă��邩�ǂ����𔻒肷��( �߂�l TRUE:���E�ɓ����Ă��Ȃ�  FALSE:���E�ɓ����Ă��� )
extern int NS_CheckCameraViewClipD( VECTOR_D CheckPos )
{
	if( GSYS.DrawSetting.ClippingPlane[ 0 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 0 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 0 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 0 ].w < 0.0 ||
		GSYS.DrawSetting.ClippingPlane[ 1 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 1 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 1 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 1 ].w < 0.0 ||
		GSYS.DrawSetting.ClippingPlane[ 2 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 2 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 2 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 2 ].w < 0.0 ||
		GSYS.DrawSetting.ClippingPlane[ 3 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 3 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 3 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 3 ].w < 0.0 ||
		GSYS.DrawSetting.ClippingPlane[ 4 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 4 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 4 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 4 ].w < 0.0 ||
		GSYS.DrawSetting.ClippingPlane[ 5 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 5 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 5 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 5 ].w < 0.0 )
		return TRUE ;

	return FALSE ;
}

// �w��̍��W���J�����̎��E�ɓ����Ă��邩�ǂ����𔻒肷��A�߂�l�ŊO��Ă���������m�邱�Ƃ��ł���( �߂�l 0:���E�ɓ����Ă���  0�ȊO:���E�ɓ����Ă��Ȃ�( DX_CAMERACLIP_LEFT �� DX_CAMERACLIP_RIGHT �� or ���Z�ō������ꂽ���́Aand ���Z�ŕ������m�F�ł��� ) )
extern int NS_CheckCameraViewClip_Dir( VECTOR CheckPos )
{
	int clip ;

	clip = 0 ;
	if( GSYS.DrawSetting.ClippingPlane[ 0 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 0 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 0 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 0 ].w < 0.0f ) clip |= DX_CAMERACLIP_LEFT ;
	if( GSYS.DrawSetting.ClippingPlane[ 1 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 1 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 1 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 1 ].w < 0.0f ) clip |= DX_CAMERACLIP_RIGHT ;
	if( GSYS.DrawSetting.ClippingPlane[ 2 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 2 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 2 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 2 ].w < 0.0f ) clip |= DX_CAMERACLIP_BOTTOM ;
	if( GSYS.DrawSetting.ClippingPlane[ 3 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 3 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 3 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 3 ].w < 0.0f ) clip |= DX_CAMERACLIP_TOP ;
	if( GSYS.DrawSetting.ClippingPlane[ 4 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 4 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 4 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 4 ].w < 0.0f ) clip |= DX_CAMERACLIP_BACK ;
	if( GSYS.DrawSetting.ClippingPlane[ 5 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 5 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 5 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 5 ].w < 0.0f ) clip |= DX_CAMERACLIP_FRONT ;

	return clip ;
}

// �w��̍��W���J�����̎��E�ɓ����Ă��邩�ǂ����𔻒肷��A�߂�l�ŊO��Ă���������m�邱�Ƃ��ł���( �߂�l 0:���E�ɓ����Ă���  0�ȊO:���E�ɓ����Ă��Ȃ�( DX_CAMERACLIP_LEFT �� DX_CAMERACLIP_RIGHT �� or ���Z�ō������ꂽ���́Aand ���Z�ŕ������m�F�ł��� ) )
extern int NS_CheckCameraViewClip_DirD( VECTOR_D CheckPos )
{
	int clip ;

	clip = 0 ;
	if( GSYS.DrawSetting.ClippingPlane[ 0 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 0 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 0 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 0 ].w < 0.0 ) clip |= DX_CAMERACLIP_LEFT ;
	if( GSYS.DrawSetting.ClippingPlane[ 1 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 1 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 1 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 1 ].w < 0.0 ) clip |= DX_CAMERACLIP_RIGHT ;
	if( GSYS.DrawSetting.ClippingPlane[ 2 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 2 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 2 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 2 ].w < 0.0 ) clip |= DX_CAMERACLIP_BOTTOM ;
	if( GSYS.DrawSetting.ClippingPlane[ 3 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 3 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 3 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 3 ].w < 0.0 ) clip |= DX_CAMERACLIP_TOP ;
	if( GSYS.DrawSetting.ClippingPlane[ 4 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 4 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 4 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 4 ].w < 0.0 ) clip |= DX_CAMERACLIP_BACK ;
	if( GSYS.DrawSetting.ClippingPlane[ 5 ].x * CheckPos.x + GSYS.DrawSetting.ClippingPlane[ 5 ].y * CheckPos.y + GSYS.DrawSetting.ClippingPlane[ 5 ].z * CheckPos.z + GSYS.DrawSetting.ClippingPlane[ 5 ].w < 0.0 ) clip |= DX_CAMERACLIP_FRONT ;

	return clip ;
}


// ��̍��W�ŕ\�����{�b�N�X���J�����̎��E�ɓ����Ă��邩�ǂ����𔻒肷��( �߂�l TRUE:���E�ɓ����Ă��Ȃ�  FALSE:���E�ɓ����Ă��� )
extern int NS_CheckCameraViewClip_Box( VECTOR BoxPos1, VECTOR BoxPos2 )
{
	VECTOR CheckBoxPos[ 8 ] ;
	VECTOR Tmp ;

	Tmp   = BoxPos2   ; CheckBoxPos[ 0 ] = Tmp ;
	Tmp.x = BoxPos1.x ; CheckBoxPos[ 1 ] = Tmp ;
	Tmp.y = BoxPos1.y ; CheckBoxPos[ 2 ] = Tmp ;
	Tmp.x = BoxPos2.x ; CheckBoxPos[ 3 ] = Tmp ;
	Tmp.z = BoxPos1.z ; CheckBoxPos[ 4 ] = Tmp ;
	Tmp.y = BoxPos2.y ; CheckBoxPos[ 5 ] = Tmp ;
	Tmp.x = BoxPos1.x ; CheckBoxPos[ 6 ] = Tmp ;
	Tmp.y = BoxPos1.y ; CheckBoxPos[ 7 ] = Tmp ;

	return Graphics_Camera_CheckCameraViewClip_Box_PosDim( CheckBoxPos ) ;
}

// ��̍��W�ŕ\�����{�b�N�X���J�����̎��E�ɓ����Ă��邩�ǂ����𔻒肷��( �߂�l TRUE:���E�ɓ����Ă��Ȃ�  FALSE:���E�ɓ����Ă��� )
extern int NS_CheckCameraViewClip_BoxD( VECTOR_D BoxPos1, VECTOR_D BoxPos2 )
{
	VECTOR_D CheckBoxPos[ 8 ] ;
	VECTOR_D Tmp ;

	Tmp   = BoxPos2   ; CheckBoxPos[ 0 ] = Tmp ;
	Tmp.x = BoxPos1.x ; CheckBoxPos[ 1 ] = Tmp ;
	Tmp.y = BoxPos1.y ; CheckBoxPos[ 2 ] = Tmp ;
	Tmp.x = BoxPos2.x ; CheckBoxPos[ 3 ] = Tmp ;
	Tmp.z = BoxPos1.z ; CheckBoxPos[ 4 ] = Tmp ;
	Tmp.y = BoxPos2.y ; CheckBoxPos[ 5 ] = Tmp ;
	Tmp.x = BoxPos1.x ; CheckBoxPos[ 6 ] = Tmp ;
	Tmp.y = BoxPos1.y ; CheckBoxPos[ 7 ] = Tmp ;

	return Graphics_Camera_CheckCameraViewClip_Box_PosDimD( CheckBoxPos ) ;
}

// �J������ Near �N���b�v�ʂ̋������擾����
extern float NS_GetCameraNear( void )
{
	return ( float )GSYS.DrawSetting.ProjNear ;
}

// �J������ Near �N���b�v�ʂ̋������擾����
extern double NS_GetCameraNearD( void )
{
	return GSYS.DrawSetting.ProjNear ;
}

// �J������ Far �N���b�v�ʂ̋������擾����
extern float NS_GetCameraFar( void )
{
	return ( float )GSYS.DrawSetting.ProjFar ;
}

// �J������ Far �N���b�v�ʂ̋������擾����
extern double NS_GetCameraFarD( void )
{
	return GSYS.DrawSetting.ProjFar ;
}

// �J�����̈ʒu���擾����
extern	VECTOR	NS_GetCameraPosition( void )
{
	return VConvDtoF( GSYS.Camera.Position ) ;
}

// �J�����̈ʒu���擾����
extern	VECTOR_D NS_GetCameraPositionD( void )
{
	return GSYS.Camera.Position ;
}

// �J�����̒����_���擾����
extern	VECTOR	NS_GetCameraTarget( void )
{
	return VConvDtoF( GSYS.Camera.Target ) ;
}

// �J�����̒����_���擾����
extern	VECTOR_D NS_GetCameraTargetD( void )
{
	return GSYS.Camera.Target ;
}

// �J�����̎����ɐ����ȏ�����̃x�N�g���𓾂�
extern	VECTOR	NS_GetCameraUpVector( void )
{
	return VConvDtoF( GSYS.Camera.Up ) ;
}

// �J�����̎����ɐ����ȏ�����̃x�N�g���𓾂�
extern	VECTOR_D NS_GetCameraUpVectorD( void )
{
	return GSYS.Camera.Up ;
}

// �J�����̐��������̌������擾����
extern	float	NS_GetCameraAngleHRotate( void )
{
	return ( float )GSYS.Camera.HRotate ;
}

// �J�����̐��������̌������擾����
extern	double NS_GetCameraAngleHRotateD( void )
{
	return GSYS.Camera.HRotate ;
}

// �J�����̐��������̌������擾����
extern	float	NS_GetCameraAngleVRotate( void )
{
	return ( float )GSYS.Camera.VRotate ;
}

// �J�����̐��������̌������擾����
extern	double NS_GetCameraAngleVRotateD( void )
{
	return GSYS.Camera.VRotate ;
}

// �J�����̌����̔P��p�x���擾����
extern	float	NS_GetCameraAngleTRotate( void )
{
	return ( float )GSYS.Camera.TRotate ;
}

// �J�����̌����̔P��p�x���擾����
extern	double NS_GetCameraAngleTRotateD( void )
{
	return GSYS.Camera.TRotate ;
}




// �r���[�s����擾����
extern MATRIX NS_GetCameraViewMatrix( void )
{
	MATRIX Result ;

	ConvertMatrixDtoF( &Result, &GSYS.DrawSetting.ViewMatrix ) ;
	return Result ;
}

// �r���[�s����擾����
extern MATRIX_D NS_GetCameraViewMatrixD( void )
{
	return GSYS.DrawSetting.ViewMatrix ;
}

// �r���{�[�h�s����擾����
extern MATRIX NS_GetCameraBillboardMatrix( void )
{
	return GSYS.DrawSetting.BillboardMatrixF ;
}

// �r���{�[�h�s����擾����
extern MATRIX_D NS_GetCameraBillboardMatrixD( void )
{
	return GSYS.DrawSetting.BillboardMatrix ;
}

// ��ʏ�ɂ�����J���������Ă���f���̒��S�̍��W���擾����
extern int NS_GetCameraScreenCenter( float  *x, float  *y )
{
	if( x != NULL )
	{
		*x = ( float )GSYS.Camera.ScreenCenterX ;
	}

	if( y != NULL )
	{
		*y = ( float )GSYS.Camera.ScreenCenterY ;
	}

	return 0 ;
}

// ��ʏ�ɂ�����J���������Ă���f���̒��S�̍��W���擾����
extern int NS_GetCameraScreenCenterD( double *x, double *y )
{
	if( x != NULL )
	{
		*x = GSYS.Camera.ScreenCenterX ;
	}

	if( y != NULL )
	{
		*y = GSYS.Camera.ScreenCenterY ;
	}

	return 0 ;
}

// �J�����̎���p���擾����
extern float NS_GetCameraFov( void )
{
	return ( float )GSYS.DrawSetting.ProjFov ;
}

// �J�����̎���p���擾����
extern double NS_GetCameraFovD( void )
{
	return GSYS.DrawSetting.ProjFov ;
}

// �J�����̎���T�C�Y���擾����
extern float NS_GetCameraSize( void )
{
	return ( float )GSYS.DrawSetting.ProjSize ;
}

// �J�����̎���T�C�Y���擾����
extern double NS_GetCameraSizeD( void )
{
	return GSYS.DrawSetting.ProjSize ;
}

// �ˉe�s����擾����
extern MATRIX NS_GetCameraProjectionMatrix( void )
{
	return GSYS.DrawSetting.ProjectionMatrixF ;
}

// �ˉe�s����擾����
extern MATRIX_D NS_GetCameraProjectionMatrixD( void )
{
	return GSYS.DrawSetting.ProjectionMatrix ;
}

// �J�����̃h�b�g�A�X�y�N�g��𓾂�
extern float NS_GetCameraDotAspect( void )
{
	return ( float )GSYS.DrawSetting.ProjDotAspect ;
}

// �J�����̃h�b�g�A�X�y�N�g��𓾂�
extern double NS_GetCameraDotAspectD( void )
{
	return GSYS.DrawSetting.ProjDotAspect ;
}

// �r���[�|�[�g�s����擾����
extern MATRIX NS_GetCameraViewportMatrix( void )
{
	return GSYS.DrawSetting.ViewportMatrixF ;
}

// �r���[�|�[�g�s����擾����
extern MATRIX_D NS_GetCameraViewportMatrixD( void )
{
	return GSYS.DrawSetting.ViewportMatrix ;
}

// Direct3D�Ŏ����K�p�����r���[�|�[�g�s����擾����
extern MATRIX NS_GetCameraAPIViewportMatrix( void )
{
	return GSYS.DrawSetting.Direct3DViewportMatrixF ;
}

// Direct3D�Ŏ����K�p�����r���[�|�[�g�s����擾����
extern MATRIX_D NS_GetCameraAPIViewportMatrixD( void )
{
	return GSYS.DrawSetting.Direct3DViewportMatrix ;
}



























// ���C�g�֌W�֐�

// ���C�e�B���O�v�Z�@�\���g�p���邩�ǂ�����ݒ肷��
extern int NS_SetUseLighting( int Flag )
{
//	if( GSYS.Light.ProcessDisable == !Flag )
//		return 0 ;
	GSYS.Light.ProcessDisable = Flag ? FALSE : TRUE ;

	// �Z�b�g
	Graphics_Hardware_Light_SetUse_PF( Flag ) ;

	// ���C�g�̐ݒ肪�ύX���ꂽ�t���O�𗧂Ă�
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�e�B���O�v�Z�ɒ��_�J���[�̃f�B�t���[�Y�J���[���g�p���邩�ǂ�����ݒ肷��
extern int NS_SetMaterialUseVertDifColor( int UseFlag )
{
	GSYS.Light.MaterialNotUseVertexDiffuseColor = UseFlag ? FALSE : TRUE ;
	
	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�e�B���O�v�Z�ɒ��_�J���[�̃X�y�L�����J���[���g�p���邩�ǂ�����ݒ肷��
extern int NS_SetMaterialUseVertSpcColor( int UseFlag )
{
	GSYS.Light.MaterialNotUseVertexSpecularColor = UseFlag ? FALSE : TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�e�B���O�v�Z�Ɏg�p����}�e���A���p�����[�^��ݒ肷��
extern int NS_SetMaterialParam( MATERIALPARAM Material )
{
	GSYS.Light.ChangeMaterial = 1 ;
	GSYS.Light.Material = Material ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �R�c�}�`�`��ɃX�y�L�������g�p���邩�ǂ�����ݒ肷��
extern int NS_SetUseSpecular( int UseFlag )
{
	GSYS.DrawSetting.NotUseSpecular = UseFlag ? FALSE : TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �O���[�o���A���r�G���g���C�g�J���[��ݒ肷��
extern int NS_SetGlobalAmbientLight( COLOR_F Color )
{
	     if( Color.r < 0.0f ) Color.r = 0.0f ;
	else if( Color.r > 1.0f ) Color.r = 1.0f ;
	     if( Color.g < 0.0f ) Color.g = 0.0f ;
	else if( Color.g > 1.0f ) Color.g = 1.0f ;
	     if( Color.b < 0.0f ) Color.b = 0.0f ;
	else if( Color.b > 1.0f ) Color.b = 1.0f ;

	// �n�[�h�E�G�A�ɐݒ肷��
	Graphics_Hardware_Light_GlobalAmbient_PF( &Color ) ;

	// �I��
	return 0 ;
}

// ���C�g�̃^�C�v���f�B���N�V���i�����C�g�ɂ���
extern int NS_ChangeLightTypeDir( VECTOR Direction )
{
	LIGHT_HANDLE *pLH ;
	float Length ;

	LIGHTCHK( GSYS.Light.DefaultHandle, pLH, -1 ) ;

	// �^�C�v��ύX
	pLH->Param.LightType = DX_LIGHTTYPE_D3DLIGHT_DIRECTIONAL ;

	// �������Z�b�g
	Length = _SQRT( VectorInnerProduct( &Direction, &Direction ) ) ;
	Direction.x /= Length ;
	Direction.y /= Length ;
	Direction.z /= Length ;
	pLH->Param.Direction = Direction ;

	// �ύX�t���O�𗧂Ă�
	pLH->HardwareChangeFlag = TRUE ;
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�̃^�C�v���X�|�b�g���C�g�ɂ���
extern int NS_ChangeLightTypeSpot( VECTOR Position, VECTOR Direction, float OutAngle, float InAngle/*, float Falloff*/, float Range, float Atten0, float Atten1, float Atten2 )
{
	float Falloff = 1.0f ;

	LIGHT_HANDLE *pLH ;
	LIGHTCHK( GSYS.Light.DefaultHandle, pLH, -1 ) ;

	// �^�C�v���Z�b�g
	pLH->Param.LightType = DX_LIGHTTYPE_D3DLIGHT_SPOT ;

	// �p�����[�^���Z�b�g
	pLH->Param.Position = Position ;
	pLH->Param.Direction = Direction ;
	pLH->Param.Phi = OutAngle ;
	pLH->Param.Theta = InAngle ;
	pLH->Param.Falloff = Falloff ;
	pLH->Param.Range = Range ;
	pLH->Param.Attenuation0 = Atten0 ;
	pLH->Param.Attenuation1 = Atten1 ;
	pLH->Param.Attenuation2 = Atten2 ;

	// �ύX�t���O�𗧂Ă�
	pLH->HardwareChangeFlag = TRUE ;
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�̃^�C�v���|�C���g���C�g�ɂ���
extern int NS_ChangeLightTypePoint( VECTOR Position, float Range, float Atten0, float Atten1, float Atten2 )
{
	LIGHT_HANDLE *pLH ;
	LIGHTCHK( GSYS.Light.DefaultHandle, pLH, -1 ) ;

	// �^�C�v���Z�b�g
	pLH->Param.LightType = DX_LIGHTTYPE_D3DLIGHT_POINT ;

	// �p�����[�^���Z�b�g
	pLH->Param.Range = Range ;
	pLH->Param.Position = Position ;
	pLH->Param.Attenuation0 = Atten0 ;
	pLH->Param.Attenuation1 = Atten1 ;
	pLH->Param.Attenuation2 = Atten2 ;

	// �ύX�t���O�𗧂Ă�
	pLH->HardwareChangeFlag = TRUE ;
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�n���h���̃��C�g�̃^�C�v���擾����( �߂�l�� DX_LIGHTTYPE_DIRECTIONAL �� )
extern	int NS_GetLightType( void )
{
	return NS_GetLightTypeHandle( GSYS.Light.DefaultHandle ) ;
}

// ���C�g�̗L���A�������Z�b�g����
extern int NS_SetLightEnable( int EnableFlag )
{
	return NS_SetLightEnableHandle( GSYS.Light.DefaultHandle, EnableFlag ) ;
}

// ���C�g�n���h���̃��C�g���ʂ̗L���A������ݒ肷��( TRUE:�L��  FALSE:���� )
extern	int NS_GetLightEnable( void )
{
	return NS_GetLightEnableHandle( GSYS.Light.DefaultHandle ) ;
}

// ���C�g�̃f�B�t���[�Y�J���[��ݒ肷��
extern int NS_SetLightDifColor( COLOR_F Color )
{
	return NS_SetLightDifColorHandle( GSYS.Light.DefaultHandle, Color ) ;
}

// ���C�g�n���h���̃��C�g�̃f�B�t���[�Y�J���[��ݒ肷��
extern COLOR_F NS_GetLightDifColor( void )
{
	return NS_GetLightDifColorHandle( GSYS.Light.DefaultHandle ) ;
}

// ���C�g�̃X�y�L�����J���[��ݒ肷��
extern int NS_SetLightSpcColor( COLOR_F Color )
{
	return NS_SetLightSpcColorHandle( GSYS.Light.DefaultHandle, Color ) ;
}

// ���C�g�n���h���̃��C�g�̃X�y�L�����J���[��ݒ肷��
extern COLOR_F NS_GetLightSpcColor(  void )
{
	return NS_GetLightSpcColorHandle( GSYS.Light.DefaultHandle ) ;
}

// ���C�g�̃A���r�G���g�J���[��ݒ肷��
extern int NS_SetLightAmbColor( COLOR_F Color )
{
	return NS_SetLightAmbColorHandle( GSYS.Light.DefaultHandle, Color ) ;
}

// ���C�g�n���h���̃��C�g�̃A���r�G���g�J���[��ݒ肷��
extern COLOR_F NS_GetLightAmbColor(  void )
{
	return NS_GetLightAmbColorHandle( GSYS.Light.DefaultHandle ) ;
}

// ���C�g�̕�����ݒ肷��
extern int NS_SetLightDirection( VECTOR Direction )
{
	return NS_SetLightDirectionHandle( GSYS.Light.DefaultHandle, Direction ) ;
}

// ���C�g�n���h���̃��C�g�̕�����ݒ肷��
extern	VECTOR NS_GetLightDirection( void )
{
	return NS_GetLightDirectionHandle( GSYS.Light.DefaultHandle ) ;
}

// ���C�g�̈ʒu��ݒ肷��
extern int NS_SetLightPosition( VECTOR Position )
{
	return NS_SetLightPositionHandle( GSYS.Light.DefaultHandle, Position ) ;
}

// ���C�g�n���h���̃��C�g�̈ʒu��ݒ肷��
extern	VECTOR NS_GetLightPosition( void )
{
	return NS_GetLightPositionHandle( GSYS.Light.DefaultHandle ) ;
}

// ���C�g�̋��������p�����[�^��ݒ肷��( �L�������A���������W���O�A�P�A�Q )
extern int NS_SetLightRangeAtten( float Range, float Atten0, float Atten1, float Atten2 )
{
	return NS_SetLightRangeAttenHandle( GSYS.Light.DefaultHandle, Range,  Atten0,  Atten1,  Atten2 ) ;
}

// ���C�g�̋��������p�����[�^���擾����( �L�������A���������W���O�A�P�A�Q )
extern int NS_GetLightRangeAtten( float *Range, float *Atten0, float *Atten1, float *Atten2 )
{
	return NS_GetLightRangeAttenHandle( GSYS.Light.DefaultHandle, Range, Atten0, Atten1, Atten2 ) ;
}

// ���C�g�̃X�|�b�g���C�g�̃p�����[�^��ݒ肷��( �O���R�[���p�x�A�����R�[���p�x�A�t�H�[���I�t( 1.0f �𐄏� ) )
extern int NS_SetLightAngle( float OutAngle, float InAngle /*, float Falloff */ )
{
	return NS_SetLightAngleHandle( GSYS.Light.DefaultHandle,  OutAngle,  InAngle /*,  Falloff */ ) ;
}

// ���C�g�̃X�|�b�g���C�g�̃p�����[�^���擾����( �O���R�[���p�x�A�����R�[���p�x�A�t�H�[���I�t )
extern int NS_GetLightAngle( float *OutAngle, float *InAngle /*, float *Falloff */ )
{
	return NS_GetLightAngleHandle( GSYS.Light.DefaultHandle, OutAngle, InAngle /*, Falloff */ ) ;
}

// �f�t�H���g���C�g�� SetUseShadowMap �Ŏw�肵���V���h�E�}�b�v��K�p���邩�ǂ�����ݒ肷��( SmSlotIndex:�V���h�E�}�b�v�X���b�g( SetUseShadowMap �̑������ɐݒ肷��l ) UseFlag:�K�p�ɂ��邩�ǂ����̃t���O( TRUE:�K�p����( �f�t�H���g )  FALSE:�K�p���Ȃ� ) )
extern int NS_SetLightUseShadowMap( int SmSlotIndex, int UseFlag )
{
	return NS_SetLightUseShadowMapHandle( GSYS.Light.DefaultHandle, SmSlotIndex, UseFlag ) ;
}

// �f�B���N�V���i�����C�g�n���h�����쐬����
extern int NS_CreateDirLightHandle( VECTOR Direction )
{
	int LHandle ;
	LIGHT_HANDLE *pLH ;
	float Length ;

	// �V�����n���h���̒ǉ�
	LHandle = Graphics_Light_AddHandle() ;
	LIGHTCHK( LHandle, pLH, -1 ) ;

	// �^�C�v���Z�b�g
	pLH->Param.LightType = DX_LIGHTTYPE_D3DLIGHT_DIRECTIONAL ;

	// �������Z�b�g
	Length = _SQRT( VectorInnerProduct( &Direction, &Direction ) ) ;
	Direction.x /= Length ;
	Direction.y /= Length ;
	Direction.z /= Length ;
	pLH->Param.Direction = Direction ;

	// �n���h����Ԃ�
	return LHandle ;
}

// �X�|�b�g���C�g�n���h�����쐬����
extern int NS_CreateSpotLightHandle( VECTOR Position, VECTOR Direction, float OutAngle, float InAngle/*, float Falloff*/, float Range, float Atten0, float Atten1, float Atten2 )
{
	float Falloff = 1.0f ;
	int LHandle ;
	LIGHT_HANDLE *pLH ;

	// �V�����n���h���̒ǉ�
	LHandle = Graphics_Light_AddHandle() ;
	LIGHTCHK( LHandle, pLH, -1 ) ;

	// �^�C�v���Z�b�g
	pLH->Param.LightType = DX_LIGHTTYPE_D3DLIGHT_SPOT ;

	// �p�����[�^���Z�b�g
	pLH->Param.Position = Position ;
	pLH->Param.Direction = Direction ;
	pLH->Param.Phi = OutAngle ;
	pLH->Param.Theta = InAngle ;
	pLH->Param.Falloff = Falloff ;
	pLH->Param.Range = Range ;
	pLH->Param.Attenuation0 = Atten0 ;
	pLH->Param.Attenuation1 = Atten1 ;
	pLH->Param.Attenuation2 = Atten2 ;

	// �n���h����Ԃ�
	return LHandle ;
}

// �|�C���g���C�g�n���h�����쐬����
extern int NS_CreatePointLightHandle( VECTOR Position, float Range, float Atten0, float Atten1, float Atten2 )
{
	int LHandle ;
	LIGHT_HANDLE *pLH ;

	// �V�����n���h���̒ǉ�
	LHandle = Graphics_Light_AddHandle() ;
	LIGHTCHK( LHandle, pLH, -1 ) ;

	// �^�C�v���Z�b�g
	pLH->Param.LightType = DX_LIGHTTYPE_D3DLIGHT_POINT ;

	// �p�����[�^���Z�b�g
	pLH->Param.Position = Position ;
	pLH->Param.Range = Range ;
	pLH->Param.Attenuation0 = Atten0 ;
	pLH->Param.Attenuation1 = Atten1 ;
	pLH->Param.Attenuation2 = Atten2 ;

	// �n���h����Ԃ�
	return LHandle ;
}

// ���C�g�n���h�����폜����
extern int NS_DeleteLightHandle( int LHandle )
{
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	// ���C�g�n���h���̑��������炷
	GSYS.Light.Num -- ;

	// �L���ȃ��C�g�n���h�������݂���͈͂̍X�V
	if( ( LHandle & DX_HANDLEINDEX_MASK ) == GSYS.Light.Area - 1 )
	{
		if( GSYS.Light.Num == 0 )
		{
			GSYS.Light.Area = 0 ;
		}
		else
		{
			while( GSYS.Light.Data[ GSYS.Light.Area - 1 ] == NULL )
				GSYS.Light.Area -- ; 
		}
	}

	// �f�[�^�̈���������
	DXFREE( pLH ) ;

	// �e�[�u���� NULL ���Z�b�g����
	GSYS.Light.Data[ LHandle & DX_HANDLEINDEX_MASK ] = NULL ;

	// �ύX�t���O�𗧂Ă�
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�n���h����S�č폜����
extern int NS_DeleteLightHandleAll( void )
{
	int i ;
	LIGHT_HANDLE **ppLH ;

	// �W�����C�g�ȊO�̂��ׂẴ��C�g�f�[�^���폜����
	ppLH = GSYS.Light.Data ;
	for( i = 0 ; i < GSYS.Light.Area ; i ++, ppLH ++ )
	{
		if( *ppLH && ( *ppLH )->Handle != GSYS.Light.DefaultHandle )
			NS_DeleteLightHandle( ( *ppLH )->Handle ) ;
	}

	// �I��
	return 0 ;
}

// ���C�g�n���h���̃��C�g�̃^�C�v��ύX����( DX_LIGHTTYPE_DIRECTIONAL �� )
extern int NS_SetLightTypeHandle( int LHandle, int LightType )
{
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	// ���C�g�^�C�v���s���������ꍇ�̓G���[
	if( LightType != DX_LIGHTTYPE_DIRECTIONAL &&
		LightType != DX_LIGHTTYPE_SPOT &&
		LightType != DX_LIGHTTYPE_POINT )
		return -1 ;

	// �^�C�v���Z�b�g
	pLH->Param.LightType = LightType ;

	// �ύX�t���O�𗧂Ă�
	pLH->HardwareChangeFlag = TRUE ;
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�n���h���̃��C�g�̗L���A������ݒ肷��( TRUE:�L��  FALSE:���� )
extern int NS_SetLightEnableHandle( int LHandle, int EnableFlag )
{
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	// ���܂łƓ����ꍇ�͉��������ɏI��
	if( pLH->EnableFlag == EnableFlag )
		return 0 ;

	// �t���O��ۑ�
	pLH->EnableFlag = EnableFlag ;
	if( EnableFlag == FALSE )
	{
		pLH->SetHardwareIndex = -1 ;
	}

	// �ύX�t���O�𗧂Ă�
	pLH->HardwareChangeFlag = TRUE ;
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�n���h���̃��C�g�̃f�B�t���[�Y�J���[��ݒ肷��
extern int NS_SetLightDifColorHandle( int LHandle, COLOR_F Color )
{
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	Color.a = 0.0f ;
	pLH->Param.Diffuse = Color ;

	// �ύX�t���O�𗧂Ă�
	pLH->HardwareChangeFlag = TRUE ;
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�n���h���̃��C�g�̃X�y�L�����J���[��ݒ肷��
extern int NS_SetLightSpcColorHandle( int LHandle, COLOR_F Color )
{
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	Color.a = 0.0f ;
	pLH->Param.Specular = Color ;

	// �ύX�t���O�𗧂Ă�
	pLH->HardwareChangeFlag = TRUE ;
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�n���h���̃��C�g�̃A���r�G���g�J���[��ݒ肷��
extern int NS_SetLightAmbColorHandle( int LHandle, COLOR_F Color )
{
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	Color.a = 0.0f ;
	pLH->Param.Ambient = Color ;

	// �ύX�t���O�𗧂Ă�
	pLH->HardwareChangeFlag = TRUE ;
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�n���h���̃��C�g�̕�����ݒ肷��
extern int NS_SetLightDirectionHandle( int LHandle, VECTOR Direction )
{
	LIGHT_HANDLE *pLH ;
	float Length ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	// ������ۑ�����
	Length = _SQRT( VectorInnerProduct( &Direction, &Direction ) ) ;
	Direction.x /= Length ;
	Direction.y /= Length ;
	Direction.z /= Length ;
	pLH->Param.Direction = Direction ;

	// �ύX�t���O�𗧂Ă�
	pLH->HardwareChangeFlag = TRUE ;
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�n���h���̃��C�g�̈ʒu��ݒ肷��
extern int NS_SetLightPositionHandle( int LHandle, VECTOR Position )
{
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	pLH->Param.Position = Position ;

	// �ύX�t���O�𗧂Ă�
	pLH->HardwareChangeFlag = TRUE ;
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�n���h���̃��C�g�̋��������p�����[�^��ݒ肷��( �L�������A���������W���O�A�P�A�Q )
extern int NS_SetLightRangeAttenHandle( int LHandle, float Range, float Atten0, float Atten1, float Atten2 )
{
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	pLH->Param.Range = Range ;
	pLH->Param.Attenuation0 = Atten0 ;
	pLH->Param.Attenuation1 = Atten1 ;
	pLH->Param.Attenuation2 = Atten2 ;

	// �ύX�t���O�𗧂Ă�
	pLH->HardwareChangeFlag = TRUE ;
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�n���h���̃��C�g�̃X�|�b�g���C�g�̃p�����[�^��ݒ肷��( �O���R�[���p�x�A�����R�[���p�x�A�t�H�[���I�t( 1.0f �𐄏� ) )
extern int NS_SetLightAngleHandle( int LHandle, float OutAngle, float InAngle /*, float Falloff */ )
{
	LIGHT_HANDLE *pLH ;
	float Falloff = 1.0f ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	pLH->Param.Phi = OutAngle ;
	pLH->Param.Theta = InAngle ;
	pLH->Param.Falloff = Falloff ;

	// �ύX�t���O�𗧂Ă�
	pLH->HardwareChangeFlag = TRUE ;
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�n���h���̃��C�g�� SetUseShadowMap �Ŏw�肵���V���h�E�}�b�v��K�p���邩�ǂ�����ݒ肷��
// ( SmSlotIndex:�V���h�E�}�b�v�X���b�g( SetUseShadowMap �̑������ɐݒ肷��l )
//   UseFlag:�K�p�ɂ��邩�ǂ����̃t���O( TRUE:�K�p����( �f�t�H���g )  FALSE:�K�p���Ȃ� ) )
extern int NS_SetLightUseShadowMapHandle( int LHandle, int SmSlotIndex, int UseFlag )
{
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	// �G���[�`�F�b�N
	if( SmSlotIndex < 0 || SmSlotIndex >= MAX_USE_SHADOWMAP_NUM )
	{
		return -1 ;
	}

	// �t���O�Z�b�g
	pLH->ShadowMapSlotDisableFlag[ SmSlotIndex ] = UseFlag == FALSE ? TRUE : FALSE ;

	// �ύX�t���O�𗧂Ă�
	pLH->HardwareChangeFlag = TRUE ;
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���C�g�n���h���̃��C�g�̃^�C�v���擾����( �߂�l�� DX_LIGHTTYPE_DIRECTIONAL �� )
extern int NS_GetLightTypeHandle( int LHandle )
{
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	return pLH->Param.LightType ;
}

// ���C�g�n���h���̃��C�g���ʂ̗L���A�������擾����( TRUE:�L��  FALSE:���� )
extern int NS_GetLightEnableHandle( int LHandle )
{
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	return pLH->EnableFlag ;
}

// ���C�g�n���h���̃��C�g�̃f�B�t���[�Y�J���[���擾����
extern COLOR_F NS_GetLightDifColorHandle( int LHandle )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, ErrorResult ) ;

	// �F��Ԃ�
	return pLH->Param.Diffuse ;
}

// ���C�g�n���h���̃��C�g�̃X�y�L�����J���[���擾����
extern COLOR_F NS_GetLightSpcColorHandle( int LHandle )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, ErrorResult ) ;

	// �F��Ԃ�
	return pLH->Param.Specular ;
}

// ���C�g�n���h���̃��C�g�̃A���r�G���g�J���[���擾����
extern COLOR_F NS_GetLightAmbColorHandle( int LHandle )
{
	COLOR_F ErrorResult = { -1.0f, -1.0f, -1.0f, -1.0f } ;
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, ErrorResult ) ;

	// �F��Ԃ�
	return pLH->Param.Ambient ;
}

// ���C�g�n���h���̃��C�g�̕������擾����
extern VECTOR NS_GetLightDirectionHandle( int LHandle )
{
	LIGHT_HANDLE *pLH ;
	VECTOR ErrorResult = { -1.0f, -1.0f, -1.0f } ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, ErrorResult ) ;

	return pLH->Param.Direction ;
}

// ���C�g�n���h���̃��C�g�̈ʒu���擾����
extern VECTOR NS_GetLightPositionHandle( int LHandle )
{
	LIGHT_HANDLE *pLH ;
	VECTOR ErrorResult = { -1.0f, -1.0f, -1.0f } ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, ErrorResult ) ;

	return pLH->Param.Position ;
}

// ���C�g�n���h���̃��C�g�̋��������p�����[�^���擾����( �L�������A���������W���O�A�P�A�Q )
extern int NS_GetLightRangeAttenHandle( int LHandle, float *Range, float *Atten0, float *Atten1, float *Atten2 )
{
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	if( Range ) *Range = pLH->Param.Range ;
	if( Atten0 ) *Atten0 = pLH->Param.Attenuation0 ;
	if( Atten1 ) *Atten1 = pLH->Param.Attenuation1 ;
	if( Atten2 ) *Atten2 = pLH->Param.Attenuation2 ;

	// �I��
	return 0 ;
}

// ���C�g�n���h���̃��C�g�̃X�|�b�g���C�g�̃p�����[�^���擾����( �O���R�[���p�x�A�����R�[���p�x�A�t�H�[���I�t )
extern int NS_GetLightAngleHandle( int LHandle, float *OutAngle, float *InAngle /*, float *Falloff */ )
{
	LIGHT_HANDLE *pLH ;

	// �G���[����
	LIGHTCHK( LHandle, pLH, -1 ) ;

	if( OutAngle ) *OutAngle = pLH->Param.Phi ;
	if( InAngle  ) *InAngle  = pLH->Param.Theta ;
//	if( Falloff  ) *Falloff  = pLH->Param.Falloff ;

	// �I��
	return 0 ;
}

// �L���ɂȂ��Ă��郉�C�g�n���h���̐����擾����
extern int NS_GetEnableLightHandleNum( void )
{
	// �X�V�t���O�������Ă���������X�V����
	if( GSYS.Light.HardwareChange )
	{
		Graphics_Light_RefreshState() ;
	}

	// �L���ɂȂ��Ă��郉�C�g�n���h���̐���Ԃ�
	return GSYS.Light.EnableNum ;
}

// �L���ɂȂ��Ă��郉�C�g�n���h�����擾����
extern int NS_GetEnableLightHandle( int Index )
{
	// �C���f�b�N�X�`�F�b�N
	if( Index < 0 || Index >= MAX_HARDWAREENABLELIGHTINDEX_NUM )
		return -1 ;

	// �X�V�t���O�������Ă���������X�V����
	if( GSYS.Light.HardwareChange )
	{
		Graphics_Light_RefreshState() ;
	}

	// �͈͊O�̏ꍇ�̓G���[
	if( Index >= GSYS.Light.EnableNum )
		return -1 ;

	// �n���h���l��Ԃ�
	return GSYS.Light.Data[ GSYS.Light.EnableHardwareIndex[ Index ] ]->Handle ;
}

























// �F���擾�p�֐�

// �e�N�X�`���t�H�[�}�b�g�̃C���f�b�N�X�𓾂�
extern int NS_GetTexFormatIndex( const IMAGEFORMATDESC *Format )
{
	int Result = DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ;

	switch( Format->BaseFormat )
	{
	case DX_BASEIMAGE_FORMAT_DXT1 : return DX_GRAPHICSIMAGE_FORMAT_3D_DXT1 ;
	case DX_BASEIMAGE_FORMAT_DXT2 : return DX_GRAPHICSIMAGE_FORMAT_3D_DXT2 ;
	case DX_BASEIMAGE_FORMAT_DXT3 : return DX_GRAPHICSIMAGE_FORMAT_3D_DXT3 ;
	case DX_BASEIMAGE_FORMAT_DXT4 : return DX_GRAPHICSIMAGE_FORMAT_3D_DXT4 ;
	case DX_BASEIMAGE_FORMAT_DXT5 : return DX_GRAPHICSIMAGE_FORMAT_3D_DXT5 ;
	case DX_BASEIMAGE_FORMAT_PLATFORM0 : return DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM0 ;
	case DX_BASEIMAGE_FORMAT_PLATFORM1 : return DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM1 ;
	case DX_BASEIMAGE_FORMAT_PLATFORM2 : return DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM2 ;
	case DX_BASEIMAGE_FORMAT_PLATFORM3 : return DX_GRAPHICSIMAGE_FORMAT_3D_PLATFORM3 ;

	default:
		switch( Format->ChannelNum )
		{
		case 1 :
			if( Format->DrawValidFlag == 0 )
			{
				if( Format->FloatTypeFlag )
				{
					if( Format->ChannelBitDepth != 0 )
					{
						if( Format->ChannelBitDepth < 32 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ;
						}
					}
					else
					{
						if( Format->ColorBitDepth < 32 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F16 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_ONE_F32 ;
						}
					}
				}
				else
				{
					if( Format->ChannelBitDepth != 0 )
					{
						if( Format->ChannelBitDepth < 16 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16 ;
						}
					}
					else
					{
						if( Format->ColorBitDepth < 16 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I8 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_ONE_I16 ;
						}
					}
				}
			}
			else
			{
				if( Format->FloatTypeFlag )
				{
					if( Format->ChannelBitDepth != 0 )
					{
						if( Format->ChannelBitDepth < 32 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ;
						}
					}
					else
					{
						if( Format->ColorBitDepth < 32 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F16 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_F32 ;
						}
					}
				}
				else
				{
					if( Format->ChannelBitDepth != 0 )
					{
						if( Format->ChannelBitDepth < 16 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16 ;
						}
					}
					else
					{
						if( Format->ColorBitDepth < 16 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I8 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ONE_I16 ;
						}
					}
				}
			}

		case 2 :
			if( Format->DrawValidFlag == 0 )
			{
				if( Format->FloatTypeFlag )
				{
					if( Format->ChannelBitDepth != 0 )
					{
						if( Format->ChannelBitDepth < 32 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32 ;
						}
					}
					else
					{
						if( Format->ColorBitDepth < 64 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F16 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_TWO_F32 ;
						}
					}
				}
				else
				{
					if( Format->ChannelBitDepth != 0 )
					{
						if( Format->ChannelBitDepth < 16 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I8 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I16 ;
						}
					}
					else
					{
						if( Format->ColorBitDepth < 32 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I8 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_TWO_I16 ;
						}
					}
				}
			}
			else
			{
				if( Format->FloatTypeFlag )
				{
					if( Format->ChannelBitDepth != 0 )
					{
						if( Format->ChannelBitDepth < 32 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32 ;
						}
					}
					else
					{
						if( Format->ColorBitDepth < 64 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F16 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_F32 ;
						}
					}
				}
				else
				{
					if( Format->ChannelBitDepth != 0 )
					{
						if( Format->ChannelBitDepth < 16 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I8 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16 ;
						}
					}
					else
					{
						if( Format->ColorBitDepth < 32 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I8 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_TWO_I16 ;
						}
					}
				}
			}

		case 4 :
		default :
			if( Format->ColorBitDepth == 4 )
			{
					 if( Format->AlphaChFlag   ) Result = DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL4 ;
				else if( Format->AlphaTestFlag ) Result = DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL4 ;
				else                             Result = DX_GRAPHICSIMAGE_FORMAT_3D_PAL4 ;
			}
			else
			if( Format->ColorBitDepth == 8 )
			{
					 if( Format->AlphaChFlag   ) Result = DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_PAL8 ;
				else if( Format->AlphaTestFlag ) Result = DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_PAL8 ;
				else                             Result = DX_GRAPHICSIMAGE_FORMAT_3D_PAL8 ;
			}
			else
			{
				if( Format->DrawValidFlag == 0 )
				{
					if( Format->FloatTypeFlag )
					{
						if( Format->ChannelBitDepth != 0 )
						{
							if( Format->ChannelBitDepth < 32 )
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F16 ;
							}
							else
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32 ;
							}
						}
						else
						{
							if( Format->ColorBitDepth < 128 )
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F16 ;
							}
							else
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_F32 ;
							}
						}
					}
					else
					{
						if( Format->ChannelBitDepth >= 9 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_ABGR_I16 ;
						}
						else
						if( Format->AlphaChFlag )
						{
							if( Format->ColorBitDepth == 16 )
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB16 ;
							}
							else
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_ALPHA_RGB32 ;
							}
						}
						else
						if( Format->AlphaTestFlag )
						{
							if( Format->ColorBitDepth == 16 )
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB16 ;
							}
							else
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_ALPHATEST_RGB32 ;
							}
						}
						else
						{
							if( Format->ColorBitDepth == 16 )
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_RGB16 ;
							}
							else
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_RGB32 ;
							}
						}
					}
				}
				else
				{
					if( Format->FloatTypeFlag )
					{
						if( Format->ChannelBitDepth != 0 )
						{
							if( Format->ChannelBitDepth < 32 )
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F16 ;
							}
							else
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32 ;
							}
						}
						else
						{
							if( Format->ColorBitDepth < 128 )
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F16 ;
							}
							else
							{
								return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_F32 ;
							}
						}
					}
					else
					{
						if( Format->ChannelBitDepth >= 9 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ABGR_I16 ;
						}
						else
						if( Format->AlphaChFlag == TRUE || Format->AlphaTestFlag == TRUE )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_ALPHA_RGB32 ;
						}
						else
						if( Format->ColorBitDepth == 16 )
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB16 ;
						}
						else
						{
							return DX_GRAPHICSIMAGE_FORMAT_3D_DRAWVALID_RGB32 ;
						}
					}
				}
			}
			break ;
		}
		break ;
	}

	return Result ;
}

#ifdef __WINDOWS__

// �F�����
extern int NS_ColorKaiseki( const void *PixelData, LPCOLORDATA ColorData )
{
	D_DDPIXELFORMAT *PData = ( D_DDPIXELFORMAT * )PixelData ;
	DWORD ColorMask ;
	int ColorWidth, ColorLoc ;
	int i, j ;

	unsigned int  ColorMaskBuf[ 5 ] ;
	unsigned char *ColorWidthBuf[ 5 ] ;
	unsigned char *ColorLocBuf[ 5 ] ;

	// �r�b�g�f�v�X��ۑ�
	ColorData->ColorBitDepth = ( unsigned char )PData->dwRGBBitCount ;

	// �s�N�Z��������̃o�C�g����ۑ�
	ColorData->PixelByte = ( unsigned char )( ColorData->ColorBitDepth / 8 ) ;

	// �F������͂���
	{
		// �J���[�}�X�N���𓾂鏀��������
		ColorData->RedMask		= PData->dwRBitMask;
		ColorMaskBuf[ 0 ]		= ColorData->RedMask ;
		ColorWidthBuf[ 0 ]		= &ColorData->RedWidth ;
		ColorLocBuf[ 0 ]		= &ColorData->RedLoc ;

		ColorData->GreenMask	= PData->dwGBitMask;
		ColorMaskBuf[ 1 ]		= ColorData->GreenMask ;
		ColorWidthBuf[ 1 ]		= &ColorData->GreenWidth ;
		ColorLocBuf[ 1 ]		= &ColorData->GreenLoc ;

		ColorData->BlueMask		= PData->dwBBitMask;
		ColorMaskBuf[ 2 ]		= ColorData->BlueMask ;
		ColorWidthBuf[ 2 ]		= &ColorData->BlueWidth ;
		ColorLocBuf[ 2 ]		= &ColorData->BlueLoc ;

		ColorData->AlphaMask	= PData->dwRGBAlphaBitMask ;
		ColorMaskBuf[ 3 ]		= ColorData->AlphaMask ;
		ColorWidthBuf[ 3 ]		= &ColorData->AlphaWidth ;
		ColorLocBuf[ 3 ]		= &ColorData->AlphaLoc ;

		if( ColorData->ColorBitDepth == 32 ) ColorData->NoneMask = 0xffffffff;
		else                                 ColorData->NoneMask = ( unsigned int )( ( 1 << ColorData->ColorBitDepth ) - 1 ) ;
		ColorData->NoneMask		&= ~( ColorData->RedMask | ColorData->GreenMask | ColorData->BlueMask | ColorData->AlphaMask ) ;
		ColorMaskBuf[ 4 ]		= ColorData->NoneMask ;
		ColorWidthBuf[ 4 ]		= &ColorData->NoneWidth ;
		ColorLocBuf[ 4 ]		= &ColorData->NoneLoc ;
	}

	// �F�̉�͊J�n
 	for( j = 0 ; j < 5 ; j ++ )
	{
		// �J���[�}�X�N���擾
		ColorMask = ColorMaskBuf[ j ] ;

		if( ColorMask == 0 )
		{
			*ColorLocBuf[ j ] = 0 ;
			*ColorWidthBuf[ j ] = 0 ;
			continue ;
		}

		// �J���[���̐擪�r�b�g�܂ł̃r�b�g���ƃr�b�g���̏�����
		ColorLoc = 0 ;
		ColorWidth = 0 ;

		// �J���[���̂���擪�r�b�g�܂ł̃r�b�g�������
		for( i = 0 ; i < 32 ; i ++ )
		{
			// �擪�r�b�g�ɓ��B���Ȃ��Ԃ͌J��Ԃ�
			if( ColorMask & 1 )
			{	
				ColorLoc = i ;
				break ;
			}
			ColorMask >>= 1 ;
		}
		// �����J���[��񂪂Ȃ���Έُ�I��
		if( i == 32 ) return -1 ;

		// �J���[���̃r�b�g���𒲂ׂ�
		for( i = 0 ; i < 32 ; i ++ )
		{
			// �r�b�g���r�؂�Ȃ��ԌJ��Ԃ�
			if( !( ColorMask & 1 ) ) 
			{
				ColorWidth = i ;
				break ;
			}
			ColorMask >>= 1 ;
		}

		// ��͌��ʂ�ۑ�����
		*ColorLocBuf[ j ] = ( unsigned char )ColorLoc ;
		*ColorWidthBuf[ j ] = ( unsigned char )ColorWidth ;
	}

	// �Ȃ�̋@�\������t�����Ă��Ȃ��r�b�g�̃}�X�N���Z�b�g����
//	NS_SetColorDataNoneMask( ColorData ) ;

	// �I��
	return 0 ;
}

#endif // __WINDOWS__





















// ������`��֐�

#ifndef DX_NON_FONT

// �y�o�b�t�@�ɑ΂��ĕ�����̕`��
extern int NS_DrawStringToZBuffer( int x, int y, const TCHAR *String, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawString(  x,  y, String,  GetColor( 255,255,255 ),  GetColor( 0,0,0 ) );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ĕ�����̕`��
extern int NS_DrawVStringToZBuffer( int x, int y, const TCHAR *String, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawVString(  x,  y, String,  GetColor( 255,255,255 ),  GetColor( 0,0,0 ) );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ĕ������`�悷��
extern int NS_DrawStringToHandleToZBuffer( int x, int y, const TCHAR *String, int FontHandle, int WriteZMode /* DX_ZWRITE_MASK �� */ , int VerticalFlag )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawStringToHandle(  x,  y, String,  GetColor( 255,255,255 ),  FontHandle,  GetColor( 0,0,0 ) ,  VerticalFlag );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ĕ������`�悷��
extern int NS_DrawVStringToHandleToZBuffer( int x, int y, const TCHAR *String, int FontHandle, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawVStringToHandle(  x,  y, String,  GetColor( 255,255,255 ),  FontHandle,  GetColor( 0,0,0 ) );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ď����w�蕶�����`�悷��
extern int NS_DrawFormatStringToZBuffer( int x, int y, int WriteZMode /* DX_ZWRITE_MASK �� */, const TCHAR *FormatString, ... )
{
	va_list VaList ;
	TCHAR String[ 2048 ] ;

	va_start( VaList, FormatString ) ;
	_TVSNPRINTF( String, sizeof( String ) / sizeof( TCHAR ), FormatString, VaList ) ;
	va_end( VaList ) ;

	DrawZBuffer_Pre( WriteZMode );
	NS_DrawString(  x,  y, String,  GetColor( 255,255,255 ) );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ď����w�蕶�����`�悷��
extern int NS_DrawFormatVStringToZBuffer( int x, int y, int WriteZMode /* DX_ZWRITE_MASK �� */, const TCHAR *FormatString, ... )
{
	va_list VaList ;
	TCHAR String[ 2048 ] ;

	va_start( VaList, FormatString ) ;
	_TVSNPRINTF( String, sizeof( String ) / sizeof( TCHAR ), FormatString, VaList ) ;
	va_end( VaList ) ;

	DrawZBuffer_Pre( WriteZMode );
	NS_DrawVString(  x,  y, String,  GetColor( 255,255,255 ) );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ď����w�蕶�����`�悷��
extern int NS_DrawFormatStringToHandleToZBuffer( int x, int y, int FontHandle, int WriteZMode /* DX_ZWRITE_MASK �� */, const TCHAR *FormatString, ... )
{
	va_list VaList ;
	TCHAR String[ 2048 ] ;

	va_start( VaList, FormatString ) ;
	_TVSNPRINTF( String, sizeof( String ) / sizeof( TCHAR ), FormatString, VaList ) ;
	va_end( VaList ) ;

	DrawZBuffer_Pre( WriteZMode );
	NS_DrawStringToHandle(  x,  y, String,  GetColor( 255,255,255 ), FontHandle );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ď����w�蕶�����`�悷��
extern int NS_DrawFormatVStringToHandleToZBuffer( int x, int y, int FontHandle, int WriteZMode /* DX_ZWRITE_MASK �� */, const TCHAR *FormatString, ... )
{
	va_list VaList ;
	TCHAR String[ 2048 ] ;

	va_start( VaList, FormatString ) ;
	_TVSNPRINTF( String, sizeof( String ) / sizeof( TCHAR ), FormatString, VaList ) ;
	va_end( VaList ) ;

	DrawZBuffer_Pre( WriteZMode );
	NS_DrawVStringToHandle(  x,  y, String,  GetColor( 255,255,255 ), FontHandle );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ĕ�����̊g��`��
extern int NS_DrawExtendStringToZBuffer( int x, int y, double ExRateX, double ExRateY, const TCHAR *String, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawExtendString(  x,  y,  ExRateX,  ExRateY, String,  GetColor( 255,255,255 ),  GetColor( 0,0,0 ) );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ĕ�����̊g��`��
extern int NS_DrawExtendVStringToZBuffer( int x, int y, double ExRateX, double ExRateY, const TCHAR *String, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawExtendVString(  x,  y,  ExRateX,  ExRateY, String,  GetColor( 255,255,255 ),  GetColor( 0,0,0 ) );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ĕ�������g��`�悷��
extern int NS_DrawExtendStringToHandleToZBuffer( int x, int y, double ExRateX, double ExRateY, const TCHAR *String, int FontHandle, int WriteZMode /* DX_ZWRITE_MASK �� */ , int VerticalFlag )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawExtendStringToHandle(  x,  y,  ExRateX,  ExRateY, String,  GetColor( 255,255,255 ),  FontHandle,  GetColor( 0,0,0 ) ,  VerticalFlag );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ĕ�������g��`�悷��
extern int NS_DrawExtendVStringToHandleToZBuffer( int x, int y, double ExRateX, double ExRateY, const TCHAR *String, int FontHandle, int WriteZMode /* DX_ZWRITE_MASK �� */ )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawExtendVStringToHandle(  x,  y,  ExRateX,  ExRateY, String,  GetColor( 255,255,255 ),  FontHandle,  GetColor( 0,0,0 ) );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ď����w�蕶������g��`�悷��
extern int NS_DrawExtendFormatStringToZBuffer( int x, int y, double ExRateX, double ExRateY, int WriteZMode /* DX_ZWRITE_MASK �� */, const TCHAR *FormatString, ... )
{
	va_list VaList ;
	TCHAR String[ 2048 ] ;

	va_start( VaList, FormatString ) ;
	_TVSNPRINTF( String, sizeof( String ) / sizeof( TCHAR ), FormatString, VaList ) ;
	va_end( VaList ) ;

	DrawZBuffer_Pre( WriteZMode );
	NS_DrawExtendString(  x,  y, ExRateX, ExRateY, String,  GetColor( 255,255,255 ) );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ď����w�蕶������g��`�悷��
extern int NS_DrawExtendFormatVStringToZBuffer( int x, int y, double ExRateX, double ExRateY, int WriteZMode /* DX_ZWRITE_MASK �� */, const TCHAR *FormatString, ... )
{
	va_list VaList ;
	TCHAR String[ 2048 ] ;

	va_start( VaList, FormatString ) ;
	_TVSNPRINTF( String, sizeof( String ) / sizeof( TCHAR ), FormatString, VaList ) ;
	va_end( VaList ) ;

	DrawZBuffer_Pre( WriteZMode );
	NS_DrawExtendVString(  x,  y, ExRateX, ExRateY, String,  GetColor( 255,255,255 ) );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ď����w�蕶������g��`�悷��
extern int NS_DrawExtendFormatStringToHandleToZBuffer( int x, int y, double ExRateX, double ExRateY, int FontHandle, int WriteZMode /* DX_ZWRITE_MASK �� */, const TCHAR *FormatString, ... )
{
	va_list VaList ;
	TCHAR String[ 2048 ] ;

	va_start( VaList, FormatString ) ;
	_TVSNPRINTF( String, sizeof( String ) / sizeof( TCHAR ), FormatString, VaList ) ;
	va_end( VaList ) ;

	DrawZBuffer_Pre( WriteZMode );
	NS_DrawExtendStringToHandle(  x,  y, ExRateX, ExRateY, String,  GetColor( 255,255,255 ), FontHandle );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �y�o�b�t�@�ɑ΂��ď����w�蕶������g��`�悷��
extern int NS_DrawExtendFormatVStringToHandleToZBuffer( int x, int y, double ExRateX, double ExRateY, int FontHandle, int WriteZMode /* DX_ZWRITE_MASK �� */, const TCHAR *FormatString, ... )
{
	va_list VaList ;
	TCHAR String[ 2048 ] ;

	va_start( VaList, FormatString ) ;
	_TVSNPRINTF( String, sizeof( String ) / sizeof( TCHAR ), FormatString, VaList ) ;
	va_end( VaList ) ;

	DrawZBuffer_Pre( WriteZMode );
	NS_DrawExtendVStringToHandle(  x,  y, ExRateX, ExRateY, String,  GetColor( 255,255,255 ), FontHandle );
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �f�t�H���g�t�H���g�n���h�����g�p���Ăy�o�b�t�@�ɑ΂��ĕ��������]�`�悷��
extern int NS_DrawRotaStringToZBuffer( int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, int WriteZMode /* DX_ZWRITE_MASK �� */ , int VerticalFlag , const TCHAR *String )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawRotaString(  x,  y,  ExRateX,  ExRateY, RotCenterX, RotCenterY, RotAngle,  GetColor( 255,255,255 ), GetColor( 0,0,0 ), VerticalFlag, String ) ;
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �t�H���g�n���h�����g�p���Ăy�o�b�t�@�ɑ΂��ĕ��������]�`�悷��
extern int NS_DrawRotaStringToHandleToZBuffer( int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, int FontHandle, int WriteZMode /* DX_ZWRITE_MASK �� */ , int VerticalFlag , const TCHAR *String )
{
	DrawZBuffer_Pre( WriteZMode );
	NS_DrawRotaStringToHandle(  x,  y,  ExRateX,  ExRateY, RotCenterX, RotCenterY, RotAngle,  GetColor( 255,255,255 ), FontHandle, GetColor( 0,0,0 ), VerticalFlag, String ) ;
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �f�t�H���g�t�H���g�n���h�����g�p���Ăy�o�b�t�@�ɑ΂��ď����w�蕶�������]�`�悷��
extern int NS_DrawRotaFormatStringToZBuffer( int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, int WriteZMode /* DX_ZWRITE_MASK �� */ , int VerticalFlag, const TCHAR *FormatString , ... )
{
	va_list VaList ;
	TCHAR String[ 2048 ] ;

	va_start( VaList, FormatString ) ;
	_TVSNPRINTF( String, sizeof( String ) / sizeof( TCHAR ), FormatString, VaList ) ;
	va_end( VaList ) ;

	DrawZBuffer_Pre( WriteZMode ) ;
	NS_DrawRotaString(  x,  y,  ExRateX,  ExRateY, RotCenterX, RotCenterY, RotAngle,  GetColor( 255,255,255 ), GetColor( 0,0,0 ), VerticalFlag, String ) ;
	DrawZBuffer_Post();

	// �I��
	return 0;
}

// �t�H���g�n���h�����g�p���Ăy�o�b�t�@�ɑ΂��ď����w�蕶�������]�`�悷��
extern int NS_DrawRotaFormatStringToHandleToZBuffer( int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, int FontHandle, int WriteZMode /* DX_ZWRITE_MASK �� */ , int VerticalFlag, const TCHAR *FormatString, ... )
{
	va_list VaList ;
	TCHAR String[ 2048 ] ;

	va_start( VaList, FormatString ) ;
	_TVSNPRINTF( String, sizeof( String ) / sizeof( TCHAR ), FormatString, VaList ) ;
	va_end( VaList ) ;

	DrawZBuffer_Pre( WriteZMode ) ;
	NS_DrawRotaStringToHandle(  x,  y,  ExRateX,  ExRateY, RotCenterX, RotCenterY, RotAngle,  GetColor( 255,255,255 ), FontHandle, GetColor( 0,0,0 ), VerticalFlag, String ) ;
	DrawZBuffer_Post();

	// �I��
	return 0;
}

#endif // DX_NON_FONT
























// ��{�C���[�W�`��

// ��{�C���[�W�f�[�^��`�悷��
extern	int		NS_DrawBaseImage( int x, int y, BASEIMAGE *BaseImage )
{
	int TempHandle ;

/*
	if( NS_GetActiveGraph() == DX_SCREEN_BACK )
	{
		DWORD Pitch;
		int Width, Height;
		void *Image;
		COLORDATA *ColorData;
		BASEIMAGE DestImage;

		NS_GetDrawScreenSize( &Width, &Height ) ;

		NS_GraphLock( DX_SCREEN_BACK, (int *)&Pitch, &Image, &ColorData ) ;
		DestImage.ColorData = *ColorData;
		DestImage.Width     = Width;
		DestImage.Height    = Height;
		DestImage.Pitch     = Pitch;
		DestImage.GraphData = Image;

		NS_BltBaseImage( 0, 0, BaseImage->Width, BaseImage->Height, x, y, BaseImage, &DestImage ) ;

		NS_GraphUnLock( DX_SCREEN_BACK ) ;
	}
	else
*/
	{
		TempHandle = Graphics_Image_CreateGraphFromGraphImageBase( BaseImage, NULL, TRUE, FALSE ) ;
		NS_DrawGraph( x, y, TempHandle, BaseImage->ColorData.AlphaWidth ? TRUE : FALSE ) ;
		NS_DeleteGraph( TempHandle ) ;
	}

	return 0 ;
}




















// ��{�C���[�W�f�[�^�\���̊֌W

// �`��Ώۂ̉�ʂ���w��̈�̃O���t�B�b�N����{�C���[�W�f�[�^�ɓ]������
extern int NS_GetDrawScreenBaseImage( int x1, int y1, int x2, int y2, BASEIMAGE *BaseImage )
{
	return NS_GetDrawScreenBaseImageDestPos( x1, y1, x2, y2, BaseImage, 0, 0 ) ;
}

// �`��Ώۂ̉�ʂ���w��̈����{�C���[�W�f�[�^�ɓ]������( �]������W�w��� )
extern	int NS_GetDrawScreenBaseImageDestPos( int x1, int y1, int x2, int y2, BASEIMAGE *BaseImage, int DestX, int DestY )
{
	RECT SrcRect ;
	BASEIMAGE BufferImage ;

	// ���W�`�F�b�N
	if( x1 < 0 || y1 < 0 || x1 >= x2 || y1 >= y2 )
	{
		return -1 ;
	}

	// ��荞�݃T�C�Y�Ǝ�荞�ݐ悪�摜�T�C�Y���傫���ꍇ�̓G���[
	if( BaseImage->Width < DestX + x2 - x1 || BaseImage->Height < DestY + y2 - y1 )
	{
		return -1 ;
	}

	// �`���o�b�t�@�����b�N
	SrcRect.left   = x1 ;
	SrcRect.right  = x2 ;
	SrcRect.top    = y1 ;
	SrcRect.bottom = y2 ;
	if( Graphics_Screen_LockDrawScreen( &SrcRect, &BufferImage, -1, -1, TRUE, 0 ) < 0 )
	{
		return -1 ;
	}

//	// ��荞�݃T�C�Y�Ɖ摜�T�C�Y���Ⴄ�ꍇ�̓G���[
//	if( BaseImage->Width != BufferImage.Width || BaseImage->Height != BufferImage.Height ) return -1 ;

	// �f�[�^��]��
	NS_BltBaseImage( DestX, DestY, &BufferImage, BaseImage ) ;

	// �`���o�b�t�@���A�����b�N
	Graphics_Screen_UnlockDrawScreen() ;

	// �I��
	return 0 ;
}























// �O���t�B�b�N�֘A�̏������ƌ�n��

// �O���t�B�b�N�X�V�X�e���̏�����
extern int Graphics_Initialize( void )
{
	// ���ɏ������ς݂̏ꍇ�͉������Ȃ�
	if( GSYS.InitializeFlag == TRUE ) return 0 ;

	// ��ʂ̐ݒ��������
	if( GSYS.Screen.MainScreenSizeX == 0 || GSYS.Screen.MainScreenSizeY == 0 )
	{
		Graphics_Screen_SetMainScreenSize( DEFAULT_SCREEN_SIZE_X, DEFAULT_SCREEN_SIZE_Y ) ;
	}

	if( GSYS.Screen.MainScreenColorBitDepth == 0 )
	{
		GSYS.Screen.MainScreenColorBitDepth = DEFAULT_COLOR_BITDEPTH ;
		SetMemImgDefaultColorType( 0 ) ;
	}

	// �O���t�B�b�N�n���h���Ǘ����̏�����
	InitializeHandleManage( DX_HANDLETYPE_GRAPH, sizeof( IMAGEDATA ), MAX_IMAGE_NUM, Graphics_Image_InitializeHandle, Graphics_Image_TerminateHandle, L"Graph" ) ;

	// �V�F�[�_�[�n���h���Ǘ����̏�����
	InitializeHandleManage( DX_HANDLETYPE_SHADER, sizeof( SHADERHANDLEDATA ) + sizeof( SHADERHANDLEDATA_PF ) , MAX_SHADER_NUM, Graphics_Shader_InitializeHandle, Graphics_Shader_TerminateHandle, L"Shader" ) ;

	// �V�F�[�_�[�p�萔�o�b�t�@�n���h���Ǘ����̏�����
	InitializeHandleManage( DX_HANDLETYPE_SHADER_CONSTANT_BUFFER, sizeof( SHADERCONSTANTBUFFERHANDLEDATA ) + sizeof( SHADERCONSTANTBUFFERHANDLEDATA_PF ) , MAX_CONSTANT_BUFFER_NUM, Graphics_ShaderConstantBuffer_InitializeHandle, Graphics_ShaderConstantBuffer_TerminateHandle, L"ShaderConstantBuffer" ) ;

	// ���_�o�b�t�@�n���h���Ǘ����̏�����
	InitializeHandleManage( DX_HANDLETYPE_VERTEX_BUFFER, sizeof( VERTEXBUFFERHANDLEDATA ) + sizeof( VERTEXBUFFERHANDLEDATA_PF ), MAX_VERTEX_BUFFER_NUM, Graphics_VertexBuffer_InitializeHandle, Graphics_VertexBuffer_TerminateHandle, L"VertexBuffer" ) ;

	// �C���f�b�N�X�o�b�t�@�n���h���Ǘ����̏�����
	InitializeHandleManage( DX_HANDLETYPE_INDEX_BUFFER, sizeof( INDEXBUFFERHANDLEDATA ) + sizeof( INDEXBUFFERHANDLEDATA_PF ), MAX_INDEX_BUFFER_NUM, Graphics_IndexBuffer_InitializeHandle, Graphics_IndexBuffer_TerminateHandle, L"IndexBuffer" ) ;

	// �V���h�E�}�b�v�n���h���Ǘ����̏�����
	InitializeHandleManage( DX_HANDLETYPE_SHADOWMAP, sizeof( SHADOWMAPDATA ) + sizeof( SHADOWMAPDATA_PF ), MAX_SHADOWMAP_NUM, Graphics_ShadowMap_InitializeHandle, Graphics_ShadowMap_TerminateHandle, L"ShadowMap" ) ;

	// �`�揈���̊��ˑ������̏��������̂P
	if( Graphics_Initialize_Timing0_PF() < 0 )
	{
		goto ERR ;
	}

	// �������ς݃t���O�𗧂Ă�
	GSYS.InitializeFlag = TRUE ;

	// �n�[�h�E�G�A�A�N�Z�����[�^���g�p�ł���ꍇ
	// �n�[�h�E�G�A�A�N�Z�����[�^����̏��������s��
	if( GSYS.Setting.ValidHardware == TRUE )
	{
		Graphics_Hardware_Initialize_PF() ;
	}
	else
	// ����ȊO�̏ꍇ�̓\�t�g�E�G�A�����_�����O����̏��������s��
	{
		Graphics_Software_Initialize() ;
	}

	// �O���t�B�b�N�`��ݒ�֌W�̏���������
	Graphics_DrawSetting_Initialize() ;

	// MEMIMG �̏�����
	InitializeMemImgManage() ;

#ifndef DX_NON_FONT
	// �t�H���g�̏��������s��
	InitFontManage() ;
#endif

#ifndef DX_NON_MASK
	// �}�X�N�����̏�����
	Mask_Initialize() ;
#endif

	// �`�揈���̊��ˑ������̏��������̂Q
	if( Graphics_Initialize_Timing1_PF() < 0 )
	{
		goto ERR ;
	}

	// �I��
	return 0 ;

	// �G���[����
ERR:
	Graphics_Terminate() ;

	return -1;
}

// �O���t�B�b�N�V�X�e���̌�n��
extern int Graphics_Terminate( void )
{
#ifndef DX_NON_FONT
	// �t�H���g�L���b�V���̊Ǘ��n���I��
	TermFontManage() ;
#endif

	// ���Ɍ�n���ς݂̏ꍇ�͉������Ȃ�
	if( GSYS.InitializeFlag == FALSE ) return 0 ;

#ifndef DX_NON_MASK
	// �}�X�N�����̌�n��
	Mask_Terminate() ;
#endif

#ifndef DX_NON_MODEL
	// ���ׂẴ��f�����폜
	NS_MV1InitModel() ;
	MV1InitModelBase() ;
#endif

	// �S�Ẳ摜���폜
	NS_InitGraph() ;

	// ���ׂĂ̒��_�f�[�^�ƃC���f�b�N�X�f�[�^���폜
	NS_InitVertexBuffer() ;
	NS_InitIndexBuffer() ;

	// �S�ẴV�F�[�_�[�p�萔�o�b�t�@���폜
	NS_InitShaderConstantBuffer() ;

	// �S�ẴV�F�[�_�[���폜
	NS_InitShader() ;

	// ���C�g��S�č폜
	NS_DeleteLightHandleAll() ;
	NS_DeleteLightHandle( GSYS.Light.DefaultHandle ) ;
	GSYS.Light.DefaultHandle = -1 ;

	// �\�t�g�E�G�A�����_�����O�@�\�̌�n��
	Graphics_Software_Terminate() ;

	// �`�揈���̊��ˑ������̌�n��
	Graphics_Terminate_PF() ;

	// ���_���ꎞ�I�ɕۑ����郁�����̈�̉��
	if( GSYS.Resource.TempVertexBuffer )
	{
		DXFREE( GSYS.Resource.TempVertexBuffer ) ;
		GSYS.Resource.TempVertexBuffer = NULL ;
		GSYS.Resource.TempVertexBufferSize = 0 ;
	}

	// �V���h�E�}�b�v�n���h���Ǘ����̌�n��
	TerminateHandleManage( DX_HANDLETYPE_SHADOWMAP ) ;

	// ���_�o�b�t�@�n���h���Ǘ����̌�n��
	TerminateHandleManage( DX_HANDLETYPE_VERTEX_BUFFER ) ;

	// �C���f�b�N�X�o�b�t�@�n���h���Ǘ����̌�n��
	TerminateHandleManage( DX_HANDLETYPE_INDEX_BUFFER ) ;

	// �V�F�[�_�[�n���h���Ǘ����̌�n��
	TerminateHandleManage( DX_HANDLETYPE_SHADER ) ;

	// �V�F�[�_�[�p�萔�o�b�t�@�n���h���Ǘ����̌�n��
	TerminateHandleManage( DX_HANDLETYPE_SHADER_CONSTANT_BUFFER ) ;

	// �O���t�B�b�N�n���h���Ǘ����̌�n��
	TerminateHandleManage( DX_HANDLETYPE_GRAPH ) ;

	// �ėp�o�b�t�@�̉��
	Graphics_Other_TerminateCommonBuffer() ;

	// �������ς݃t���O��|��
	GSYS.InitializeFlag = FALSE ;

	// �I��
	return 0 ;
}

// �O���t�B�b�N�X�V�X�e���̕��A�A���͕ύX�t���̍ăZ�b�g�A�b�v���s��
extern int Graphics_RestoreOrChangeSetupGraphSystem( int Change, int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, int RefreshRate )
{
	int Result ;

	// ���ˑ��̏������s��
	Result = Graphics_RestoreOrChangeSetupGraphSystem_PF( Change, ScreenSizeX, ScreenSizeY, ColorBitDepth, RefreshRate ) ;

	// ���[�r�[�O���t�B�b�N�̍Đ�
	if( DxSysData.NotDrawFlag == FALSE )
	{
#ifndef DX_NON_MOVIE
		RestoreMovieAll() ;
#endif
	}


	// �I��
	return Result ;
}























// ��ʊ֌W�֐�

// �t���X�N���[�����[�h�̃��[�h�̃`�F�b�N��g�p����𑜓x���̌�����s��
extern	int		Graphics_Screen_SetupFullScreenModeInfo( void )
{
	int             DispModeNum ;
	int             i ;
	int				IntMulNum ;
	int             ColorBitDepthResolutionMatchIndex ;
	int             ColorBitDepthResolutionMatchIntMulNum ;
	int             ResolutionMatchIndex ;
	int             ResolutionMatchIntMulNum ;
	int             RefreshRateColorBitDepthResolutionMatchIndex ;
	int             RefreshRateColorBitDepthResolutionMatchIntMulNum ;
	DISPLAYMODEDATA DispModeData = { 0 } ;
	int				UseDisplayIndex ;

	// ��ʉ𑜓x���ݒ肳��Ă��Ȃ�������f�t�H���g�l���Z�b�g
	if( GSYS.Screen.MainScreenSizeX == 0 || GSYS.Screen.MainScreenSizeY == 0 )
	{
		Graphics_Screen_SetMainScreenSize( DEFAULT_SCREEN_SIZE_X, DEFAULT_SCREEN_SIZE_Y ) ;
	}

	// �g�p����f�B�X�v���C�ԍ�������
	UseDisplayIndex = GSYS.Screen.ValidUseDisplayIndex ? GSYS.Screen.UseDisplayIndex : 0 ;

	// ��ʂ̃J���[�r�b�g�[�x�����ݒ肳��Ă��Ȃ�������f�t�H���g�l���Z�b�g
	if( GSYS.Screen.MainScreenColorBitDepth == 0 )
	{
		GSYS.Screen.MainScreenColorBitDepth = DEFAULT_COLOR_BITDEPTH ;
		SetMemImgDefaultColorType( 0 ) ;
	}

	GSYS.Screen.FullScreenResolutionModeAct = GSYS.Screen.FullScreenResolutionMode ;

LABEL_FULLSCREENMODE_SWITCH :
	switch( GSYS.Screen.FullScreenResolutionModeAct )
	{
	case DX_FSRESOLUTIONMODE_NATIVE :
		// �w��̉�ʃ��[�h�ɑΉ����Ă��邩���ׂ�
		DispModeNum = NS_GetDisplayModeNum( UseDisplayIndex ) ;
		ResolutionMatchIndex                               = -1 ;
		ResolutionMatchIntMulNum                           = -1 ;
		ColorBitDepthResolutionMatchIndex                  = -1 ;
		ColorBitDepthResolutionMatchIntMulNum              = -1 ;
		RefreshRateColorBitDepthResolutionMatchIndex       = -1 ;
		RefreshRateColorBitDepthResolutionMatchIntMulNum   = -1 ;
		for( i = 0 ; i < DispModeNum ; i ++ )
		{
			DispModeData = NS_GetDisplayMode( i, UseDisplayIndex ) ;

			// �w��̉𑜓x�ƈ�v���Ă��邩�A�Ⴕ���͎w��̉𑜓x�̐����{�̉𑜓x���ǂ����ŕ���
			if( DispModeData.Width  % GSYS.Screen.MainScreenSizeX == 0 &&
				DispModeData.Height % GSYS.Screen.MainScreenSizeY == 0 &&
				( DispModeData.Width  / GSYS.Screen.MainScreenSizeX == DispModeData.Height / GSYS.Screen.MainScreenSizeY ) )
			{
				// �w��̉𑜓x�̉��{�����Z�o
				IntMulNum = DispModeData.Width  / GSYS.Screen.MainScreenSizeX ;

				// �ŏ��Ɍ�������ʃ��[�h���A�Ⴕ���͍��܂Ō�������ʃ��[�h�����𑜓x�{�����������ꍇ�͋L��
				if( ResolutionMatchIntMulNum < 0 || IntMulNum < ResolutionMatchIntMulNum )
				{
					ResolutionMatchIndex     = i ;
					ResolutionMatchIntMulNum = IntMulNum ;
				}

				// �w��̃J���[�r�b�g�[�x�ƈ�v���Ă��邩�ǂ����ŕ���
				if( GSYS.Screen.MainScreenColorBitDepth == DispModeData.ColorBitDepth )
				{
					// �ŏ��Ɍ�������ʃ��[�h���A�Ⴕ���͍��܂Ō�������ʃ��[�h�����𑜓x�{�����������ꍇ�͋L��
					if( ColorBitDepthResolutionMatchIntMulNum < 0 || IntMulNum < ColorBitDepthResolutionMatchIntMulNum )
					{
						ColorBitDepthResolutionMatchIndex     = i ;
						ColorBitDepthResolutionMatchIntMulNum = IntMulNum ;
					}

					// ���t���b�V�����[�g�̎w�肪�������A�w��̃��t���b�V�����[�g�ƈ�v���Ă��邩�ǂ����ŕ���
					if( GSYS.Screen.MainScreenRefreshRate == 0 ||
						GSYS.Screen.MainScreenRefreshRate == DispModeData.RefreshRate )
					{
						// �ŏ��Ɍ�������ʃ��[�h���A�Ⴕ���͍��܂Ō�������ʃ��[�h�����𑜓x�{�����������ꍇ�͋L��
						if( RefreshRateColorBitDepthResolutionMatchIntMulNum < 0 || IntMulNum < RefreshRateColorBitDepthResolutionMatchIntMulNum )
						{
							RefreshRateColorBitDepthResolutionMatchIndex     = i ;
							RefreshRateColorBitDepthResolutionMatchIntMulNum = IntMulNum ;

							// �����𑜓x�{�����P�̂Ƃ��́A�w��̉�ʃ��[�h�Ɋ��S�Ɉ�v���Ă���Ƃ������ƂȂ̂ŁA���[�v���甲����
							if( IntMulNum == 1 )
							{
								break ;
							}
						}
					}
				}
			}
		}

		// �𑜓x���w��ʂ�A�Ⴕ���͐����{�ŁA���t���b�V�����[�g�E�J���[�r�b�g�[�x���w��̒ʂ�̉�ʃ��[�h���������ꍇ�́A����ɂ���
		if( RefreshRateColorBitDepthResolutionMatchIndex != -1 )
		{
			GSYS.Screen.FullScreenUseDispModeData = NS_GetDisplayMode( RefreshRateColorBitDepthResolutionMatchIndex, UseDisplayIndex ) ;
		}
		else
		// �𑜓x���w��ʂ�A�Ⴕ���͐����{�ŁA�J���[�r�b�g�[�x���w��ʂ�̉�ʃ��[�h���������ꍇ�́A����ɂ���
		if( ColorBitDepthResolutionMatchIndex != -1 )
		{
			GSYS.Screen.FullScreenUseDispModeData = NS_GetDisplayMode( ColorBitDepthResolutionMatchIndex, UseDisplayIndex ) ;
		}
		else
		// �𑜓x���w��ʂ�A�Ⴕ���͐����{�̉�ʃ��[�h���������ꍇ�́A����ɂ���
		if( ResolutionMatchIndex != -1 )
		{
			GSYS.Screen.FullScreenUseDispModeData = NS_GetDisplayMode( ResolutionMatchIndex, UseDisplayIndex ) ;
		}
		else
		// �𑜓x���w��ʂ�A�Ⴕ���͐����{�̉�ʃ��[�h�����������ꍇ�̓t���X�N���[���𑜓x���[�h�� DX_FSRESOLUTIONMODE_DESKTOP �ɕύX����
		{
			GSYS.Screen.FullScreenResolutionModeAct = DX_FSRESOLUTIONMODE_DESKTOP ;
			goto LABEL_FULLSCREENMODE_SWITCH ;
		}

		// ��ʃ��[�h���w��Ɋ��S��v���Ă��Ȃ��ꍇ�́A�t���X�N���[���𑜓x���[�h�� DX_FSRESOLUTIONMODE_MAXIMUM �ɂ���
		if( RefreshRateColorBitDepthResolutionMatchIndex == -1 || 
			RefreshRateColorBitDepthResolutionMatchIntMulNum != 1 )
		{
			GSYS.Screen.FullScreenResolutionModeAct = DX_FSRESOLUTIONMODE_MAXIMUM ;
		}
		break ;

	case DX_FSRESOLUTIONMODE_DESKTOP :
		if( GSYS.Screen.DisplayInfo == NULL )
		{
			// �f�B�X�v���C���̎擾�Ɏ��s������ DX_FSRESOLUTIONMODE_MAXIMUM ���g�p����
			if( Graphics_SetupDisplayInfo_PF() < 0 )
			{
				GSYS.Screen.FullScreenResolutionModeAct = DX_FSRESOLUTIONMODE_MAXIMUM ;
				goto LABEL_FULLSCREENMODE_SWITCH ;
			}
		}

		// �w��̉𑜓x���f�X�N�g�b�v��ʂ�荂���ꍇ�� DX_FSRESOLUTIONMODE_MAXIMUM �ɕύX
		if( GSYS.Screen.DisplayInfo[ UseDisplayIndex ].DesktopSizeX < GSYS.Screen.MainScreenSizeX ||
			GSYS.Screen.DisplayInfo[ UseDisplayIndex ].DesktopSizeY < GSYS.Screen.MainScreenSizeY )
		{
			GSYS.Screen.FullScreenResolutionModeAct = DX_FSRESOLUTIONMODE_MAXIMUM ;
			goto LABEL_FULLSCREENMODE_SWITCH ;
		}

		// �f�X�N�g�b�v�̃T�C�Y���Z�b�g
		GSYS.Screen.FullScreenUseDispModeData.Width         = GSYS.Screen.DisplayInfo[ UseDisplayIndex ].DesktopSizeX ;
		GSYS.Screen.FullScreenUseDispModeData.Height        = GSYS.Screen.DisplayInfo[ UseDisplayIndex ].DesktopSizeY ;
		GSYS.Screen.FullScreenUseDispModeData.ColorBitDepth = GSYS.Screen.DisplayInfo[ UseDisplayIndex ].DesktopColorBitDepth ;
		GSYS.Screen.FullScreenUseDispModeData.RefreshRate   = GSYS.Screen.DisplayInfo[ UseDisplayIndex ].DesktopRefreshRate ;
		break ;

	case DX_FSRESOLUTIONMODE_MAXIMUM :
		// �ő�𑜓x���擾����
		{
			int MatchDispModeIndex ;
			int MatchDispModeWidth ;
			int MatchDispModeHeight ;
			int MaxResDispModeIndex ;
			int MaxResDispModeWidth ;
			int MaxResDispModeHeight ;
			int MaxResFullColorDispModeIndex ;
			int MaxResFullColorDispModeWidth ;
			int MaxResFullColorDispModeHeight ;

			MatchDispModeIndex            = -1 ;
			MaxResDispModeIndex           = -1 ;
			MaxResFullColorDispModeIndex  = -1 ;
			MatchDispModeWidth            = 0 ;
			MatchDispModeHeight           = 0 ;
			MaxResDispModeWidth           = 0 ;
			MaxResDispModeHeight          = 0 ;
			MaxResFullColorDispModeWidth  = 0 ;
			MaxResFullColorDispModeHeight = 0 ;

			DispModeNum = NS_GetDisplayModeNum() ;
			for( i = 0 ; i < DispModeNum ; i ++ )
			{
				DispModeData = NS_GetDisplayMode( i, UseDisplayIndex ) ;

				if(   GSYS.Screen.MainScreenColorBitDepth == DispModeData.ColorBitDepth &&
					( GSYS.Screen.MainScreenRefreshRate == 0 ||
					  GSYS.Screen.MainScreenRefreshRate == DispModeData.RefreshRate ) && 
					( MatchDispModeIndex == -1 ||
					DispModeData.Width * DispModeData.Height > MatchDispModeWidth * MatchDispModeHeight ) )
				{
					MatchDispModeIndex = i ;
					MatchDispModeWidth  = DispModeData.Width ;
					MatchDispModeHeight = DispModeData.Height ;
				}

				if( MaxResDispModeIndex == -1 ||
					DispModeData.Width * DispModeData.Height > MaxResDispModeWidth * MaxResDispModeHeight )
				{
					MaxResDispModeIndex = i ;
					MaxResDispModeWidth  = DispModeData.Width ;
					MaxResDispModeHeight = DispModeData.Height ;
				}

				if( DispModeData.ColorBitDepth == GSYS.Screen.MainScreenColorBitDepth &&
					( MaxResFullColorDispModeIndex == -1 ||
					DispModeData.Width * DispModeData.Height > MaxResFullColorDispModeWidth * MaxResFullColorDispModeHeight ) )
				{
					MaxResFullColorDispModeIndex = i ;
					MaxResFullColorDispModeWidth  = DispModeData.Width ;
					MaxResFullColorDispModeHeight = DispModeData.Height ;
				}
			}

			if( MaxResDispModeWidth * MaxResDispModeHeight <= MaxResFullColorDispModeWidth * MaxResFullColorDispModeHeight )
			{
				GSYS.Screen.FullScreenUseDispModeData = NS_GetDisplayMode( MaxResFullColorDispModeIndex, UseDisplayIndex ) ;
			}
			else
			if( MaxResDispModeWidth * MaxResDispModeHeight <= MatchDispModeWidth * MatchDispModeHeight )
			{
				GSYS.Screen.FullScreenUseDispModeData = NS_GetDisplayMode( MatchDispModeIndex, UseDisplayIndex ) ;
			}
			else
			if( MaxResDispModeIndex > 0 )
			{
				GSYS.Screen.FullScreenUseDispModeData = NS_GetDisplayMode( MaxResDispModeIndex, UseDisplayIndex ) ;
			}
			else
			{
				GSYS.Screen.FullScreenUseDispModeData = NS_GetDisplayMode( 0, UseDisplayIndex ) ;
			}
		}
		break ;
	}

	// ����I��
	return 0 ;
}

// GSYS.Screen.FullScreenScalingDestRect �̒l���Z�b�g�A�b�v����
extern	int		Graphics_Screen_SetupFullScreenScalingDestRect( void )
{
	// DX_FSRESOLUTIONMODE_NATIVE �ȊO�̏ꍇ�͂����œ]�����`�̎Z�o
	switch( GSYS.Screen.FullScreenResolutionModeAct )
	{
	case DX_FSRESOLUTIONMODE_NATIVE :
		break ;

	case DX_FSRESOLUTIONMODE_MAXIMUM :
	case DX_FSRESOLUTIONMODE_DESKTOP :
		// �T�u�o�b�N�o�b�t�@�̓��e���s�N�Z���̏c������P�F�P�ɕۂ��o�b�N�o�b�t�@�ɍő�܂Ŋg�債�ē]�������`�̎Z�o
		{
			int DestSizeX ;
			int DestSizeY ;
			int ScalingSizeX ;
			int ScalingSizeY ;
			static int DestSizeXBackup = -1000 ;
			static int DestSizeYBackup = -1000 ;
			static int ScalingSizeXBackup = -1000 ;
			static int ScalingSizeYBackup = -1000 ;
			static int MainScreenSizeXBackup = -1000 ;
			static int MainScreenSizeYBackup = -1000 ;

			// �]����̃T�C�Y���Z�b�g
			if( GSYS.Screen.FullScreenResolutionModeAct == DX_FSRESOLUTIONMODE_DESKTOP )
			{
				GRAPHICSSYS_DISPLAYINFO *DisplayInfo ;

				if( GSYS.Screen.DisplayInfo == NULL )
				{
					return -1 ;
				}

				DisplayInfo = &GSYS.Screen.DisplayInfo[ GSYS.Screen.ValidUseDisplayIndex ? GSYS.Screen.UseDisplayIndex : 0 ] ;
				DestSizeX = DisplayInfo->DesktopSizeX ;
				DestSizeY = DisplayInfo->DesktopSizeY ;

				if( DestSizeXBackup != DestSizeX || DestSizeYBackup != DestSizeY )
				{
					DestSizeXBackup = DestSizeX ;
					DestSizeYBackup = DestSizeY ;
//					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x11\xff\x20\x00\x20\x00\x44\x00\x65\x00\x73\x00\x74\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x44\x00\x65\x00\x73\x00\x74\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x00"/*@ L"�P  DestSizeX:%d  DestSizeY:%d" @*/, DestSizeX, DestSizeY )) ;
				}
			}
			else
			{
				DestSizeX = GSYS.Screen.FullScreenUseDispModeData.Width ;
				DestSizeY = GSYS.Screen.FullScreenUseDispModeData.Height ;

				if( DestSizeXBackup != DestSizeX || DestSizeYBackup != DestSizeY )
				{
					DestSizeXBackup = DestSizeX ;
					DestSizeYBackup = DestSizeY ;
//					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x12\xff\x20\x00\x20\x00\x44\x00\x65\x00\x73\x00\x74\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x44\x00\x65\x00\x73\x00\x74\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x00"/*@ L"�Q  DestSizeX:%d  DestSizeY:%d" @*/, DestSizeX, DestSizeY )) ;
				}
			}

			// �]����̃T�C�Y������
			ScalingSizeX = DestSizeY * GSYS.Screen.MainScreenSizeX / GSYS.Screen.MainScreenSizeY ;
			if( ScalingSizeX < DestSizeX )
			{
				ScalingSizeY = DestSizeY ;
			}
			else
			{
				ScalingSizeX = DestSizeX ;
				ScalingSizeY = DestSizeX * GSYS.Screen.MainScreenSizeY / GSYS.Screen.MainScreenSizeX ;
			}

			if( ScalingSizeXBackup != ScalingSizeX ||
				ScalingSizeYBackup != ScalingSizeY ||
				MainScreenSizeXBackup != GSYS.Screen.MainScreenSizeX ||
				MainScreenSizeYBackup != GSYS.Screen.MainScreenSizeY )
			{
				ScalingSizeXBackup = ScalingSizeX ;
				ScalingSizeYBackup = ScalingSizeY ;
				MainScreenSizeXBackup = GSYS.Screen.MainScreenSizeX ;
				MainScreenSizeYBackup = GSYS.Screen.MainScreenSizeY ;
//				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x53\x00\x63\x00\x61\x00\x6c\x00\x69\x00\x6e\x00\x67\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x00\x30\x53\x00\x63\x00\x61\x00\x6c\x00\x69\x00\x6e\x00\x67\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x4d\x00\x61\x00\x69\x00\x6e\x00\x53\x00\x63\x00\x72\x00\x65\x00\x65\x00\x6e\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x58\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x4d\x00\x61\x00\x69\x00\x6e\x00\x53\x00\x63\x00\x72\x00\x65\x00\x65\x00\x6e\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x59\x00\x3a\x00\x25\x00\x64\x00\x00"/*@ L"ScalingSizeX:%d�@ScalingSizeY:%d  MainScreenSizeX:%d  MainScreenSizeY:%d" @*/,
//					ScalingSizeX, ScalingSizeY,
//					GSYS.Screen.MainScreenSizeX, GSYS.Screen.MainScreenSizeY )) ;
			}

			// �]����̋�`������
			GSYS.Screen.FullScreenScalingDestRect.left   = ( DestSizeX - ScalingSizeX ) / 2 ;
			GSYS.Screen.FullScreenScalingDestRect.top    = ( DestSizeY - ScalingSizeY ) / 2 ;
			GSYS.Screen.FullScreenScalingDestRect.right  = GSYS.Screen.FullScreenScalingDestRect.left + ScalingSizeX ;
			GSYS.Screen.FullScreenScalingDestRect.bottom = GSYS.Screen.FullScreenScalingDestRect.top  + ScalingSizeY ;
		}
		break ;
	}

	// ����I��
	return 0 ;
}

// �X�N���[�����W���T�u�o�b�N�o�b�t�@�[���W�ɕϊ�����
extern int Graphics_Screen_ScreenPosConvSubBackbufferPos( int ScreenPosX, int ScreenPosY, int *BackBufferPosX, int *BackBufferPosY )
{
	ScreenPosX -= GSYS.Screen.FullScreenDesktopRect.left ;
	ScreenPosY -= GSYS.Screen.FullScreenDesktopRect.top ;

	switch( GSYS.Screen.FullScreenResolutionModeAct )
	{
	case DX_FSRESOLUTIONMODE_NATIVE :
		if( BackBufferPosX != NULL )
		{
			*BackBufferPosX = ScreenPosX ;
		}

		if( BackBufferPosY != NULL )
		{
			*BackBufferPosY = ScreenPosY ;
		}
		break ;

	case DX_FSRESOLUTIONMODE_MAXIMUM :
	case DX_FSRESOLUTIONMODE_DESKTOP :
		if( Graphics_Screen_SetupFullScreenScalingDestRect() < 0 )
		{
			return -1 ;
		}

		if( BackBufferPosX != NULL )
		{
			*BackBufferPosX = ( ScreenPosX - GSYS.Screen.FullScreenScalingDestRect.left ) * GSYS.Screen.MainScreenSizeX / ( GSYS.Screen.FullScreenScalingDestRect.right  - GSYS.Screen.FullScreenScalingDestRect.left ) ;
		}

		if( BackBufferPosY != NULL )
		{
			*BackBufferPosY = ( ScreenPosY - GSYS.Screen.FullScreenScalingDestRect.top  ) * GSYS.Screen.MainScreenSizeY / ( GSYS.Screen.FullScreenScalingDestRect.bottom - GSYS.Screen.FullScreenScalingDestRect.top ) ;
		}
		break ;
	}

	return 0 ;
}

// �T�u�o�b�N�o�b�t�@�[���W���X�N���[�����W�ɕϊ�����
extern int Graphics_Screen_SubBackbufferPosConvScreenPos( int BackBufferPosX, int BackBufferPosY, int *ScreenPosX, int *ScreenPosY )
{
	switch( GSYS.Screen.FullScreenResolutionModeAct )
	{
	case DX_FSRESOLUTIONMODE_NATIVE :
		if( ScreenPosX != NULL )
		{
			*ScreenPosX = BackBufferPosX ;
		}

		if( ScreenPosY != NULL )
		{
			*ScreenPosY = BackBufferPosY ;
		}
		break ;

	case DX_FSRESOLUTIONMODE_MAXIMUM :
	case DX_FSRESOLUTIONMODE_DESKTOP :
		if( Graphics_Screen_SetupFullScreenScalingDestRect() < 0 )
		{
			return -1 ;
		}

		if( ScreenPosX != NULL )
		{
			*ScreenPosX = BackBufferPosX * ( GSYS.Screen.FullScreenScalingDestRect.right  - GSYS.Screen.FullScreenScalingDestRect.left ) / GSYS.Screen.MainScreenSizeX + GSYS.Screen.FullScreenScalingDestRect.left ;
		}

		if( ScreenPosY != NULL )
		{
			*ScreenPosY = BackBufferPosY * ( GSYS.Screen.FullScreenScalingDestRect.bottom - GSYS.Screen.FullScreenScalingDestRect.top  ) / GSYS.Screen.MainScreenSizeY + GSYS.Screen.FullScreenScalingDestRect.top ;
		}
		break ;
	}

	return 0 ;
}

// ���C����ʂ̂y�o�b�t�@�̐ݒ��ύX����
extern int Graphics_Screen_SetZBufferMode( int ZBufferSizeX, int ZBufferSizeY, int ZBufferBitDepth )
{
	return Graphics_Hardware_SetZBufferMode_PF( ZBufferSizeX, ZBufferSizeY, ZBufferBitDepth ) ;
}

// �ݒ�Ɋ�Â��Ďg�p����y�o�b�t�@���Z�b�g����
extern int Graphics_Screen_SetupUseZBuffer( void )
{
	return Graphics_Hardware_SetupUseZBuffer_PF() ;
}

// ���C����ʂ̃T�C�Y�l��ύX����
extern void Graphics_Screen_SetMainScreenSize( int SizeX, int SizeY )
{
	GSYS.Screen.MainScreenSizeX = SizeX ;
	GSYS.Screen.MainScreenSizeY = SizeY ;

	GSYS.Screen.MainScreenSizeX_Result = GSYS.Screen.MainScreenSizeX == 0 ? DEFAULT_SCREEN_SIZE_X : GSYS.Screen.MainScreenSizeX ;
	GSYS.Screen.MainScreenSizeY_Result = GSYS.Screen.MainScreenSizeY == 0 ? DEFAULT_SCREEN_SIZE_Y : GSYS.Screen.MainScreenSizeY ;

	SetMathScreenSize( GSYS.Screen.MainScreenSizeX_Result, GSYS.Screen.MainScreenSizeY_Result ) ;
}

// ��ʃ��[�h�̕ύX�Q
extern int Graphics_Screen_ChangeMode( int ScreenSizeX, int ScreenSizeY, int ColorBitDepth, int ChangeWindowFlag, int RefreshRate )
{
	int BackScSizeX, BackScSizeY, BackScColorBitDepth, BackRefreshRate ;
	int Ret = DX_CHANGESCREEN_OK ;
	int InitFlag ;
#ifndef DX_NON_FONT
	wchar_t FontName[ 128 ] ;
	int FontSize     = 0 ;
	int FontThick    = 0 ;
	int FontType     = 0 ;
	int FontCharSet  = 0 ;
	int FontEdgeSize = 0 ;
	int FontItalic   = 0 ;
	int FontSpace    = 0 ;
	int FontUse      = 0 ;
#endif // DX_NON_FONT
	int Result ;
	int i ;

	// Graphics_Screen_ChangeMode���s���t���O�𗧂Ă�
	GSYS.Screen.Graphics_Screen_ChangeModeFlag = TRUE ;

	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// ���ɏ������ς݂��ǂ������擾���Ă���
	InitFlag = GSYS.InitializeFlag ;

	// -1 �����������͊����̒l�ő�p
	if( ScreenSizeX		== -1 ) ScreenSizeX   = GSYS.Screen.MainScreenSizeX ;
	if( ScreenSizeY		== -1 ) ScreenSizeY   = GSYS.Screen.MainScreenSizeY ;
	if( ColorBitDepth	== -1 ) ColorBitDepth = GSYS.Screen.MainScreenColorBitDepth ;
	if( RefreshRate		== -1 ) RefreshRate   = GSYS.Screen.MainScreenRefreshRate ;

	// �����ȑO�Ɖ�ʃ��[�h���S�������ꍇ�͉��������I��
	if( ScreenSizeX      == GSYS.Screen.MainScreenSizeX &&
		ScreenSizeY      == GSYS.Screen.MainScreenSizeY &&
		ColorBitDepth    == GSYS.Screen.MainScreenColorBitDepth &&
		ChangeWindowFlag == FALSE &&
		RefreshRate      == GSYS.Screen.MainScreenRefreshRate )
	{
		goto END ;
	}

	DXST_ERRORLOG_ADDUTF16LE( "\x3b\x75\x62\x97\xe2\x30\xfc\x30\xc9\x30\x09\x59\xf4\x66\xe6\x51\x06\x74\x92\x30\x8b\x95\xcb\x59\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"��ʃ��[�h�ύX�������J�n���܂�\n" @*/ ) ;
	DXST_ERRORLOG_TABADD ;

#ifndef DX_NON_FONT

	// ���ɏ���������Ă�����t�H���g�̏����Ƃ��Ă���
	if( InitFlag && NS_CheckFontHandleValid( NS_GetDefaultFontHandle() ) )
	{
		GetFontStateToHandle_WCHAR_T( FontName, &FontSize, &FontThick, DX_DEFAULT_FONT_HANDLE, &FontType, &FontCharSet, &FontEdgeSize, &FontItalic ) ;
		FontSpace = NS_GetFontSpaceToHandle( DX_DEFAULT_FONT_HANDLE ) ;
		FontUse = TRUE ;
	}

#endif // DX_NON_FONT

	// �����̉�ʃ��[�h�̃p�����[�^���Ƃ��Ă���
	BackScSizeX			= GSYS.Screen.MainScreenSizeX ; 
	BackScSizeY			= GSYS.Screen.MainScreenSizeY ;
	BackScColorBitDepth	= GSYS.Screen.MainScreenColorBitDepth ;
	BackRefreshRate		= GSYS.Screen.MainScreenRefreshRate ;

	// �c�w���C�u�����̏������O�͎��s�ł��Ȃ�
	if( DxSysData.DxLib_RunInitializeFlag == FALSE && DxSysData.DxLib_InitializeFlag == FALSE )
	{
		goto END ;
	}

	// ��ʃ��[�h�ύX���ɃO���t�B�b�N�X�V�X�e�������Z�b�g���邩�ǂ����ŏ����𕪊�
	if( GSYS.Setting.ChangeScreenModeNotGraphicsSystemFlag )
	{
		// ���Z�b�g���Ȃ��ꍇ

#ifdef __WINDOWS__
		// �E�C���h�E���[�h�̏ꍇ�͎w��̉𑜓x���f�X�N�g�b�v�̃T�C�Y�𒴂��Ă��Ȃ����ǂ����𒲂ׂ�
		if( NS_GetWindowModeFlag() == TRUE )
		{
			GRAPHICSSYS_DISPLAYINFO *DisplayInfo ;

			DisplayInfo = &GSYS.Screen.DisplayInfo[ GSYS.Screen.ValidUseDisplayIndex ? GSYS.Screen.UseDisplayIndex : 0 ] ;

			if( DisplayInfo->DesktopSizeX < ScreenSizeX || DisplayInfo->DesktopSizeY < ScreenSizeY )
			{
				goto END ;
			}
		}
#endif // __WINDOWS__

		// �`�F���W
		Graphics_RestoreOrChangeSetupGraphSystem( TRUE, ScreenSizeX, ScreenSizeY, ColorBitDepth, RefreshRate ) ;

		// �\�t�g�E�G�A�����_�����O�̏ꍇ�����Ō�n���Ə��������s��
		if( GSYS.Setting.ValidHardware == FALSE )
		{
			Graphics_Software_Terminate() ;
			Graphics_Software_Initialize() ;
		}

#ifdef __WINDOWS__
		// �E�C���h�E�X�^�C���Z�b�g
		if( NS_GetWindowModeFlag() == TRUE )
		{
			SetWindowStyle() ;
		}
#endif // __WINDOWS__

		DXST_ERRORLOG_TABSUB ;
		DXST_ERRORLOG_ADDUTF16LE( "\x3b\x75\x62\x97\xe2\x30\xfc\x30\xc9\x30\x09\x59\xf4\x66\xe6\x51\x06\x74\x6f\x30\x63\x6b\x38\x5e\x6b\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"��ʃ��[�h�ύX�����͐���ɏI�����܂���\n" @*/ ) ;
	}
	else
	{
		// ���Z�b�g����ꍇ

		// �O���t�B�N�X�V�X�e���̏I��
		if( GSYS.InitializeFlag )
		{
			Graphics_Terminate() ;
		}

		// ��ʃ��[�h�̃Z�b�g
		Graphics_Screen_SetMainScreenSize( ScreenSizeX, ScreenSizeY ) ;
		GSYS.Screen.FullScreenResolutionModeAct = GSYS.Screen.FullScreenResolutionMode ;
		GSYS.Screen.MainScreenColorBitDepth     = ColorBitDepth ;
		GSYS.Screen.MainScreenRefreshRate       = RefreshRate ;
		SetMemImgDefaultColorType( ColorBitDepth == 32 ? 1 : 0 ) ;

#ifndef DX_NON_FONT
		// �f�t�H���g�t�H���g�̐ݒ���Z�b�g
		if( FontUse == TRUE )
		{
			SetDefaultFontState_WCHAR_T( FontName, FontSize, FontThick, FontType, FontCharSet, FontEdgeSize, FontItalic ) ;
			NS_SetFontSpace( FontSpace ) ;
		}
#endif // DX_NON_FONT

		// �O���t�B�b�N�V�X�e���̍ď�����
		Result = Graphics_Initialize() ;

		// ���s�����ꍇ�̏���
		if( Result == -1 )
		{
			// �G���[�����������ꍇ�ŉ�ʃ��[�h�� 320x240 �������ꍇ��
			// �[�� 320x240 ���[�h�Ŏ���
			if( GSYS.Screen.MainScreenSizeX == 320 && GSYS.Screen.MainScreenSizeY == 240 )
			{
				Graphics_Screen_SetMainScreenSize( 640, 480 ) ;
				NS_SetEmulation320x240( TRUE ) ;
				Result = Graphics_Initialize() ;
			}

			// �G���[�����������ꍇ�͌��̉�ʃ��[�h�ɖ߂�
			if( Result == -1 )
			{
				Graphics_Screen_SetMainScreenSize( BackScSizeX, BackScSizeY ) ;
				GSYS.Screen.MainScreenColorBitDepth = BackScColorBitDepth ;
				GSYS.Screen.MainScreenRefreshRate   = BackRefreshRate ;
				SetMemImgDefaultColorType( BackScColorBitDepth == 32 ? 1 : 0 ) ;
				if( Graphics_Initialize() == -1 )
				{
					// ����ł��ʖڂ������ꍇ�͂c�w���C�u�����̃f�t�H���g�̉�ʃ��[�h�ɕύX
					Graphics_Screen_SetMainScreenSize( DEFAULT_SCREEN_SIZE_X, DEFAULT_SCREEN_SIZE_Y ) ;
					GSYS.Screen.MainScreenColorBitDepth = DEFAULT_COLOR_BITDEPTH ;
					GSYS.Screen.MainScreenRefreshRate   = 0 ;
					SetMemImgDefaultColorType( 0 ) ;
					if( Graphics_Initialize() == -1 )
					{
						// ����ł��ʖڂ������ꍇ��640x480 32bit ������
						Graphics_Screen_SetMainScreenSize( DEFAULT_SCREEN_SIZE_X, DEFAULT_SCREEN_SIZE_Y ) ;
						GSYS.Screen.MainScreenColorBitDepth = 32 ;
						GSYS.Screen.MainScreenRefreshRate   = 0 ;
						SetMemImgDefaultColorType( 0 ) ;
						if( Graphics_Initialize() == -1 )
						{
							Ret = -1 ;
							DxLib_ErrorUTF16LE( "\x88\x4e\x1f\x67\x5b\x30\x6c\x30\x3b\x75\x62\x97\xe2\x30\xfc\x30\xc9\x30\x09\x59\xf4\x66\x0d\x4e\xfd\x80\xfe\x73\x61\x8c\x4c\x30\x77\x8d\x4d\x30\x7e\x30\x57\x30\x5f\x30\x32\x00\x0a\x00\x00"/*@ L"�\�����ʉ�ʃ��[�h�ύX�s�\���ۂ��N���܂���2\n" @*/ ) ;
							goto END ;
						}
					}
				}
			}
		}

#ifdef __WINDOWS__
		// �E�C���h�E�X�^�C���Z�b�g
		if( NS_GetWindowModeFlag() == TRUE /*|| GSYS.Setting.ValidHardware == FALSE*/ )
		{
			SetWindowStyle() ;
		}
#endif // __WINDOWS__

		DXST_ERRORLOG_TABSUB ;
		DXST_ERRORLOG_ADDUTF16LE( "\x3b\x75\x62\x97\xe2\x30\xfc\x30\xc9\x30\x09\x59\xf4\x66\xe6\x51\x06\x74\x6f\x30\x63\x6b\x38\x5e\x6b\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"��ʃ��[�h�ύX�����͐���ɏI�����܂���\n" @*/ ) ;

		// ���b�Z�[�W����
		NS_ProcessMessage() ;

		// �`���̐ݒ肵�Ȃ���
		NS_SetRenderTargetToShader( 0, GSYS.DrawSetting.TargetScreen[ 0 ], GSYS.DrawSetting.TargetScreenSurface[ 0 ] ) ;
		for( i = 1 ; i < GSYS.HardInfo.RenderTargetNum ; i ++ )
		{
			NS_SetRenderTargetToShader( i, -1 ) ;
		}
	}

#ifdef __WINDOWS__
	// �h���b�O���h���b�v�̐ݒ��������
	RefreshDragFileValidFlag() ;
#endif // __WINDOWS__

END:
	// Graphics_Screen_ChangeMode ���s���t���O��|��
	GSYS.Screen.Graphics_Screen_ChangeModeFlag = FALSE ;

	// �I��
	return Ret ;
}

// �`���o�b�t�@�����b�N����
extern int Graphics_Screen_LockDrawScreen( RECT *LockRect, BASEIMAGE *BaseImage, int TargetScreen, int TargetScreenSurface, int ReadOnly, int TargetScreenTextureNo )
{
	IMAGEDATA *TargImage ;
	int TargetScreenWidth ;
	int TargetScreenHeight ;

	// �`���̉摜�f�[�^�A�h���X���擾����
	if( TargetScreen == -1 )
	{
		TargetScreen        = GSYS.DrawSetting.TargetScreen[ 0 ] ;
		TargetScreenSurface = GSYS.DrawSetting.TargetScreenSurface[ 0 ] ;
	}
	else
	{
		if( TargetScreenSurface == -1 )
		{
			TargetScreenSurface = 0 ;
		}
	}
	if( GRAPHCHKFULL( TargetScreen, TargImage ) )
	{
		TargImage          = NULL ;
		TargetScreenWidth  = GSYS.DrawSetting.DrawSizeX ;
		TargetScreenHeight = GSYS.DrawSetting.DrawSizeY ;
	}
	else
	{
		TargetScreenWidth  = TargImage->WidthI ;
		TargetScreenHeight = TargImage->HeightI ;
	}

	// ��`�̃f�[�^���s���������ꍇ�G���[
	if( LockRect->left < 0 || LockRect->left >= LockRect->right  ||
		LockRect->top  < 0 || LockRect->top  >= LockRect->bottom ||
		LockRect->right  > TargetScreenWidth ||
		LockRect->bottom > TargetScreenHeight ) return -1 ;

	// �n�[�h�E�G�A���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( GSYS.Setting.ValidHardware )
	{
		// �n�[�h�E�F�A�A�N�Z�����[�^���g�p����ꍇ
		if( Graphics_Hardware_LockDrawScreenBuffer_PF( LockRect, BaseImage, TargetScreen, TargImage, TargetScreenSurface, ReadOnly, TargetScreenTextureNo ) < 0 )
		{
			return -1 ;
		}
	}
	else
	{
		int Width, Height ;

		// �\�t�g�E�G�A�����_�����O�̏ꍇ

		Width  = LockRect->right  - LockRect->left ;
		Height = LockRect->bottom - LockRect->top ;

		// �f�[�^���Z�b�g
		if( TargImage )
		{
			BaseImage->ColorData = *TargImage->Soft.MemImg.Base->ColorDataP ;
			BaseImage->Pitch     = ( int )TargImage->Soft.MemImg.Base->Pitch ;
			BaseImage->GraphData = ( BYTE * )TargImage->Soft.MemImg.UseImage + BaseImage->ColorData.PixelByte * LockRect->left + BaseImage->Pitch * LockRect->top ;
		}
		else
		{
			BaseImage->ColorData = *GSYS.SoftRender.MainBufferMemImg.Base->ColorDataP ;
			BaseImage->Pitch     = ( int )GSYS.SoftRender.MainBufferMemImg.Base->Pitch ;
			BaseImage->GraphData = ( BYTE * )GSYS.SoftRender.MainBufferMemImg.UseImage + BaseImage->ColorData.PixelByte * LockRect->left + BaseImage->Pitch * LockRect->top ;
		}
		BaseImage->Width          = Width ;
		BaseImage->Height         = Height ;
		BaseImage->MipMapCount    = 0 ;
		BaseImage->GraphDataCount = 0 ;
	}

	// ���b�N���t���O�𗧂Ă�
	GSYS.Screen.DrawScreenBufferLockFlag = TRUE ;

	// �I��
	return 0 ;
}

// �`���o�b�t�@���A�����b�N����
extern int Graphics_Screen_UnlockDrawScreen( void )
{
	// ���b�N���Ă��Ȃ������牽�������ɏI��
	if( GSYS.Screen.DrawScreenBufferLockFlag == FALSE ) return 0 ;

	// �n�[�h�E�G�A���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_UnlockDrawScreenBuffer_PF() ;
	}
	else
	{
	}

	// ���b�N���t���O��|��
	GSYS.Screen.DrawScreenBufferLockFlag = FALSE ;

	return 0 ;
}

// ����ʂƕ\��ʂ���������
// �\��ʂ�����ʂ���̃X�P�[�����O�̏ꍇ�́ACopyRect �̓R�s�[���` )
extern int Graphics_Screen_FlipBase( void )
{
	static int EndFlag = FALSE ;
	int Result = 0 ;

	GSYS.Screen.ScreenFlipFlag = TRUE ;

	// �����t���O�������Ă�����Ȃɂ������I��
	if( EndFlag )
	{
		goto END ;
	}

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE || DxLib_GetEndRequest() )
	{
		goto ERR ;
	}

	// �t���X�N���[���}�X�N�������s���Ă���ꍇ�̓}�X�N�`��̌��ʂ𔽉f������
	MASK_FULLSCREEN_MASKUPDATE

#if !defined( DX_NON_LOG ) && !defined( DX_NON_PRINTF_DX )
	// ���O�o�͂��s���ݒ�̏ꍇ�̂݃��O�o�͂��s��
	if( NS_GetLogDrawFlag() )
	{
		int BlendMode, BlendParam, RedBright, GreenBright, BlueBright ;
		RECT DrawRect ;

		BlendMode   = GSYS.DrawSetting.BlendMode ;
		BlendParam  = GSYS.DrawSetting.BlendParam ;
		DrawRect    = GSYS.DrawSetting.DrawArea ;
		RedBright   = GSYS.DrawSetting.DrawBright.Red ;
		GreenBright = GSYS.DrawSetting.DrawBright.Green ;
		BlueBright  = GSYS.DrawSetting.DrawBright.Blue ;

		NS_SetDrawArea( 0, 0, GSYS.Screen.MainScreenSizeX, GSYS.Screen.MainScreenSizeY ) ;

		// �`�惂�[�h���m�[�u�����h�ɃZ�b�g
		NS_SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 ) ;

		// �P�x���P�O�O���ɃZ�b�g
		NS_SetDrawBright( 255,255,255 ) ;

		// �t���O�����Ă�
		EndFlag = TRUE ;

		// ���O�`��
		DrawLog() ;

		// �t���O��|��
		EndFlag = FALSE ;

		// �ݒ�����ɖ߂�
		NS_SetDrawArea( DrawRect.left, DrawRect.top, DrawRect.right, DrawRect.bottom ) ; 
		NS_SetDrawBlendMode( BlendMode, BlendParam ) ;
		NS_SetDrawBright( RedBright, GreenBright, BlueBright ) ;
	}
#endif

	// ���ˑ�����
	Result = Graphics_ScreenFlipBase_PF() ;


END:
	GSYS.Screen.ScreenFlipFlag = FALSE ;

	// �I��
	return Result ;


ERR:
	GSYS.Screen.ScreenFlipFlag = FALSE ;

	// �G���[�I��
	return -1 ;
}




















// �摜�֌W�֐�

// �t�H�[�}�b�g�����Z�b�g�A�b�v����
extern int Graphics_Image_SetupFormatDesc( 
	IMAGEFORMATDESC *Format,
	SETUP_GRAPHHANDLE_GPARAM *GParam,
	int Width,
	int Height,
	int AlphaValidFlag,
	int UsePaletteFlag,
	int PaletteBitDepth,
	int BaseFormat,
	int MipMapCount
)
{
	// �t�H�[�}�b�g����������
	_MEMSET( Format, 0, sizeof( *Format ) ) ;

	// �e�N�X�`���t�H�[�}�b�g���ǂ������Z�b�g
	Format->TextureFlag = ( unsigned char )GSYS.Setting.ValidHardware ;

	// DirectX ���Ǘ�����e�N�X�`���ɂ��Ȃ����ǂ������Z�b�g
	Format->UseManagedTextureFlag = ( unsigned char )GParam->UseManagedTextureFlag ;

	// �L���[�u�}�b�v���ǂ������Z�b�g
	Format->CubeMapTextureFlag = ( unsigned char )GParam->CubeMapTextureCreateFlag ;

	// ���j�A�������z�u�̃e�N�X�`�����g�p���邩�ǂ������Z�b�g
	Format->UseLinearMapTextureFlag = ( unsigned char )GParam->UseLinearMapTextureFlag ;

	// �e�N�X�`���t�H�[�}�b�g���ڎw������Z�b�g
	Format->PlatformTextureFormat = ( unsigned char )GParam->PlatformTextureFormat ;

	// ��{�t�H�[�}�b�g�̃Z�b�g
	Format->BaseFormat = ( unsigned char )BaseFormat ;

	// �~�b�v�}�b�v�̐���ۑ�
	if( MipMapCount != -1 )
	{
		Format->MipMapCount = ( unsigned char )MipMapCount ;
	}
	else
	{
		if( GParam->MipMapCount == -1 )
		{
			int tx, ty ;

			tx = Width ;
			ty = Height ;
			Format->MipMapCount = 1 ;
			for(;;)
			{
				if( tx == 1 && ty == 1 ) break ;
				Format->MipMapCount ++ ;
				tx /= 2 ;
				ty /= 2 ;
				if( tx == 0 ) tx = 1 ;
				if( ty == 0 ) ty = 1 ;
			}
		}
		else
		{
			Format->MipMapCount = ( unsigned char )( GParam->MipMapCount == 0 ? 1 : GParam->MipMapCount ) ;
		}
	}

	// �`��\�摜���ǂ����ŏ����𕪊�
	if( GParam->DrawValidImageCreateFlag == TRUE )
	{
		// �`��\�ȏꍇ�̓A���t�@�e�X�g�A�u�����h�O���t�B�b�N�͖����ŁA
		// �A���t�@�`�����l�����������ǂ����� GSYS.CreateImage.DrawValidAlphaFlag �̒l�ɏ]��
		// ��{�t�H�[�}�b�g�͋����I�� DX_BASEIMAGE_FORMAT_NORMAL
		Format->DrawValidFlag         = TRUE ;
		Format->AlphaChFlag           = ( unsigned char )GParam->DrawValidAlphaImageCreateFlag ;
		Format->AlphaTestFlag         = FALSE ;		// �`��\�ȏꍇ�̓A���t�@�e�X�g�͖���
		Format->BlendGraphFlag        = FALSE ;		// �`��\�ȏꍇ�̓u�����h�摜������
		Format->UseManagedTextureFlag = FALSE ;
		Format->MipMapCount           = 1 ;
		Format->MSSamples             = ( unsigned char )GParam->DrawValidMSSamples ;
		Format->MSQuality             = ( unsigned char )GParam->DrawValidMSQuality ;
	}
	else
	{
		// �`��\�w�肪�����ꍇ�̓A���t�@�`�����l����t���邩�ǂ����͈�����
		// �A���t�@�e�X�g��t���邩�ǂ����� GSYS.CreateImage.AlphaTestFlag ��
		// �u�����h�摜�ɂ��邩�ǂ����� GSYS.CreateImage.BlendImageFlag �ɏ]��
		Format->DrawValidFlag  = FALSE ;
		Format->AlphaChFlag    = ( unsigned char )AlphaValidFlag ;
		Format->AlphaTestFlag  = ( unsigned char )GParam->AlphaTestImageCreateFlag ;
		Format->BlendGraphFlag = ( unsigned char )GParam->BlendImageCreateFlag ;
	}

	// �e�N�X�`�����ǂ����ŏ����𕪊�
	if( Format->TextureFlag == TRUE )
	{
		// �e�N�X�`���̏ꍇ

		// �J���[�r�b�g�[�x�̌���
		Format->ColorBitDepth   = ( unsigned char )( GParam->CreateImageColorBitDepth == 0 ? ( AlphaValidFlag == TRUE ? 32 : GSYS.Screen.MainScreenColorBitDepth ) : GParam->CreateImageColorBitDepth ) ;
		Format->ChannelNum      = GParam->CreateImageChannelNum ;
		Format->ChannelBitDepth = GParam->CreateImageChannelNum > 0 ? GParam->CreateImageChannelBitDepth : 0 ;
		Format->FloatTypeFlag   = GParam->FloatTypeGraphCreateFlag ;
		Format->SystemMemFlag   = FALSE ;

		// �A���t�@�`�����l������̏ꍇ�̓A���t�@�e�X�g�͖���
		if( Format->AlphaChFlag )
		{
			Format->AlphaTestFlag = FALSE ;
		}

		// �u�����h�摜�̏ꍇ�̓t�H�[�}�b�g�͌Œ肳���
		if( Format->BlendGraphFlag == TRUE )
		{
			Format->ColorBitDepth = 32 ;
			Format->AlphaTestFlag = FALSE ;
			Format->AlphaChFlag   = TRUE ;
			Format->DrawValidFlag = FALSE ;
		}
		else
		// �`��\�摜�̏ꍇ�̓`�����l�����ƃ`�����l���ӂ�̃r�b�g���A���������_�^���ǂ�����ݒ肷��
		if( GParam->DrawValidImageCreateFlag == TRUE )
		{
			Format->ChannelNum      = ( unsigned char )GParam->CreateDrawValidGraphChannelNum ;
			Format->ChannelBitDepth = ( unsigned char )GParam->CreateImageChannelBitDepth ;
			Format->FloatTypeFlag   = ( unsigned char )GParam->DrawValidFloatTypeGraphCreateFlag ;
		}
		else
		// �u�����h�摜�ł��`��\�摜�ł��Ȃ��ꍇ�̂݃p���b�g�摜���g�p�ł���
		if( UsePaletteFlag )
		{
			Format->ColorBitDepth = ( unsigned char )( PaletteBitDepth == 4 ? 4 : 8 ) ;
		}
	}
	else
	{
		// �e�N�X�`���ł͂Ȃ��ꍇ
		Format->BaseFormat            = DX_BASEIMAGE_FORMAT_NORMAL ;
		Format->ColorBitDepth         = ( unsigned char )( GParam->CreateImageColorBitDepth == 0 ? GSYS.Screen.MainScreenColorBitDepth : GParam->CreateImageColorBitDepth ) ;
		Format->ChannelNum            = 0 ;
		Format->ChannelBitDepth       = 0 ;
		Format->FloatTypeFlag         = FALSE ;
		Format->SystemMemFlag         = TRUE ;
		Format->CubeMapTextureFlag    = FALSE ;
		Format->UsePaletteFlag        = ( unsigned char )UsePaletteFlag ;
		Format->UseManagedTextureFlag = FALSE ;		// DirectX �̊Ǘ�������
		Format->AlphaTestFlag         = FALSE ;		// �A���t�@�e�X�g�͖���
		Format->DrawValidFlag         = ( unsigned char )( Format->BlendGraphFlag ? FALSE : TRUE ) ;	// ���ʂ̉摜�̏ꍇ�̓u�����h�摜�ł͂Ȃ���Ε`��\�@ 
		Format->MipMapCount           = 0 ;
	}

	// ����I��
	return 0 ;
}

// �f�o�C�X���X�g�������ɍ폜����t���O�������Ă���O���t�B�b�N���폜����
extern int Graphics_Image_DeleteDeviceLostDelete( void )
{
	int i ;
	IMAGEDATA *Image ;

	if( HandleManageArray[ DX_HANDLETYPE_GRAPH ].InitializeFlag )
	{
		for( i = HandleManageArray[ DX_HANDLETYPE_GRAPH ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_GRAPH ].AreaMax ; i ++ )
		{
			Image = ( IMAGEDATA * )HandleManageArray[ DX_HANDLETYPE_GRAPH ].Handle[ i ] ;

			if( Image == NULL ) continue ;
			if( Image->Orig == NULL ) continue ;
			if( Image->DeviceLostDeleteFlag == FALSE ) continue ;

			SubHandle( Image->HandleInfo.Handle ) ;
		}
	}

	// �I��
	return 0 ;
}

// �l�r�`�`���g�p����`��\�摜���ǂ����𒲂ׂ�( TRUE:MSAA�摜  FALSE:MSAA�摜�ł͂Ȃ� )
extern int Graphics_Image_CheckMultiSampleDrawValid( int GrHandle )
{
	IMAGEDATA *Image ;

	if( GRAPHCHKFULL( GrHandle, Image ) )
		return FALSE ;

	if( Image->Orig->FormatDesc.TextureFlag == FALSE )
		return FALSE ;

	return Image->Orig->FormatDesc.MSSamples != 0 ? TRUE : FALSE ;
}

// �V�����O���t�B�b�N�n���h�����m�ۂ���
extern int Graphics_Image_AddHandle( int ASyncThread )
{
	return AddHandle( DX_HANDLETYPE_GRAPH, ASyncThread, -1 );
}

// �O���t�B�b�N�n���h��������������
extern int Graphics_Image_SetupHandle_UseGParam(
	SETUP_GRAPHHANDLE_GPARAM *GParam,
	int GrHandle,
	int Width,
	int Height,
	int /*TextureFlag*/,
	int AlphaValidFlag,
	int UsePaletteFlag,
	int PaletteBitDepth,
	int BaseFormat,
	int MipMapCount,
	int ASyncThread
) 
{
	IMAGEDATA					*Image ;
	IMAGEDATA_ORIG				*Orig ;
	int							IsTex ;
	int							DivXNum ;
	int							DivYNum ;
	int							DivNum ;
	short						TexSizeX[ 256 ] ;
	short						TexSizeY[ 256 ] ;
	short						TexPosX[ 256 ] ;
	short						TexPosY[ 256 ] ;
	short						XList[ 32 ] ;
	short						YList[ 32 ] ;
	int							XPos ;
	int							YPos ;
	int							BaseAllocSize ;
	unsigned int				AllocSize ;
	int							i ;
	int							j ;
	IMAGEDATA_ORIG_HARD_TEX	*OrigTex ;
	IMAGEFORMATDESC				Format ;
	IMAGEDATA_ORIG_HARD_TEX_PF	*OrigTexPF ;

	// �A�h���X�̎擾
	if( GRAPHCHK_ASYNC( GrHandle, Image ) )
	{
		return -1 ;
	}

	// �e�N�X�`�����ǂ��������肷��
	IsTex = GSYS.Setting.ValidHardware ;

	// �t�H�[�}�b�g�����Z�b�g����
	Graphics_Image_SetupFormatDesc( &Format, GParam, Width, Height, AlphaValidFlag, UsePaletteFlag, PaletteBitDepth, BaseFormat, MipMapCount ) ;

	// �e�N�X�`���̏ꍇ�͉������ɂȂ邩���ׂ�
	if( IsTex )
	{
		int IsPow2 = FALSE ;

//		// �`���ɂł���摜�̏ꍇ�͂Q�̏搔�T�C�Y�ɂ��Ȃ�
//		if( Format.DrawValidFlag )
//		{
//			IsPow2 = FALSE ;
//		}

		// �쐬����e�N�X�`���̃T�C�Y�����X�g�A�b�v
//		DivXNum = Graphics_Image_ListUpTexSize( Width,  XList, Format.DrawValidFlag | Format.BlendGraphFlag | GParam->NotUseDivFlag, Format.DrawValidFlag == FALSE, GParam->UserMaxTextureSize ) ;
//		DivYNum = Graphics_Image_ListUpTexSize( Height, YList, Format.DrawValidFlag | Format.BlendGraphFlag | GParam->NotUseDivFlag, Format.DrawValidFlag == FALSE, GParam->UserMaxTextureSize ) ;
		// ������������f�����b�g���傫���Ȃ��Ă����̂ŁA��{�I�ɍő�e�N�X�`���[�T�C�Y�Ɏ��܂����͂P���ōς܂��悤�ɏ�����ύX
		DivXNum = Graphics_Image_ListUpTexSize( Width,  XList, TRUE, IsPow2, GParam->UserMaxTextureSize ) ;
		DivYNum = Graphics_Image_ListUpTexSize( Height, YList, TRUE, IsPow2, GParam->UserMaxTextureSize ) ;

		// ���������Z�b�g
		DivNum = DivXNum * DivYNum ;

		// �ʒu�ƃT�C�Y���Z�b�g����
		YPos = 0 ;
		for( i = 0 ; i < DivYNum ; i ++ )
		{
			XPos = 0 ;
			for( j = 0 ; j < DivXNum ; j ++ )
			{
				TexPosX[ i * DivXNum + j ] = ( short )XPos ;
				TexPosY[ i * DivXNum + j ] = ( short )YPos ;

				TexSizeX[ i * DivXNum + j ] = XList[ j ] ;
				TexSizeY[ i * DivXNum + j ] = YList[ i ] ;

				XPos += XList[ j ] ;
			}
			YPos += YList[ i ] ;
		}

		// �n�[�h�E�F�A�����ɉ����ăe�N�X�`���𐳕��`�ɂ���
		if( GSYS.HardInfo.TextureSquareOnly )
		{
			int Size ;
			int DNum ;
			int MotoDivNum ;
			int PosX ;
			int PosY ;

			MotoDivNum = DivXNum * DivYNum ;
			for( i = 0 ; i < MotoDivNum ; i ++ )
			{
				if( TexSizeX[ i ] != TexSizeY[ i ] )
				{
					if( TexSizeX[ i ] < TexSizeY[ i ] )
					{
						Size = TexSizeX[ i ] ;
						DNum = TexSizeY[ i ] / TexSizeX[ i ] ;
						TexSizeY[ i ] = ( short )Size ;
						PosX = TexPosX[ i ] ;
						PosY = TexPosY[ i ] ;
						for( j = 0 ; j < DNum - 1 ; j ++ )
						{
							TexSizeX[ DivNum + j ] = ( short )Size ;
							TexSizeY[ DivNum + j ] = ( short )Size ;
							TexPosX[ DivNum + j ] = ( short )PosX ;
							TexPosY[ DivNum + j ] = ( short )( PosY + Size * ( j + 1 ) ) ;
						}
						DivNum += DNum - 1 ;
					}
					else
					{
						Size = TexSizeY[ i ] ;
						DNum = TexSizeX[ i ] / TexSizeY[ i ] ;
						TexSizeX[ i ] = ( short )Size ;
						PosX = TexPosX[ i ] ;
						PosY = TexPosY[ i ] ;
						for( j = 0 ; j < DNum - 1 ; j ++ )
						{
							TexSizeY[ DivNum + j ] = ( short )Size ;
							TexSizeX[ DivNum + j ] = ( short )Size ;
							TexPosY[ DivNum + j ] = ( short )PosY ;
							TexPosX[ DivNum + j ] = ( short )( PosX + Size * ( j + 1 ) ) ;
						}
						DivNum += DNum - 1 ;
					}
				}
			}
		}
	}

	// �m�ۃ������̊m��
	BaseAllocSize = sizeof( IMAGEDATA_ORIG ) - ( ( sizeof( IMAGEDATA_ORIG_SOFT ) > sizeof( IMAGEDATA_ORIG_HARD ) ? sizeof( IMAGEDATA_ORIG_SOFT ) : sizeof( IMAGEDATA_ORIG_HARD ) ) ) ;
	if( IsTex )
	{
		AllocSize = sizeof( IMAGEDATA_ORIG_HARD ) + ( DivNum - 4 ) * sizeof( IMAGEDATA_ORIG_HARD_TEX ) + DivNum * sizeof( IMAGEDATA_ORIG_HARD_TEX_PF ) ;
	}
	else
	{
		AllocSize = sizeof( IMAGEDATA_ORIG_SOFT ) ;
	}

	// �I���W�i���摜���\���̗p�̃������̊m��
	Image->Orig = ( IMAGEDATA_ORIG * )DXCALLOC( AllocSize + BaseAllocSize ) ;
	if( Image->Orig == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xb0\x30\xe9\x30\xd5\x30\xa3\x30\xc3\x30\xaf\x30\x6e\x30\xaa\x30\xea\x30\xb8\x30\xca\x30\xeb\x30\x3b\x75\xcf\x50\xc7\x30\xfc\x30\xbf\x30\xdd\x4f\x58\x5b\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�O���t�B�b�N�̃I���W�i���摜�f�[�^�ۑ��p�������̊m�ۂɎ��s���܂���\n" @*/ ) ;
		goto ERR ;
	}
	Orig = Image->Orig ;

	// �e�N�X�`���̏ꍇ�͊��ˑ��f�[�^�̃A�h���X���Z�b�g
	if( IsTex )
	{
		OrigTexPF = ( IMAGEDATA_ORIG_HARD_TEX_PF * )( ( BYTE * )Orig + BaseAllocSize + sizeof( IMAGEDATA_ORIG_HARD ) + ( DivNum - 4 ) * sizeof( IMAGEDATA_ORIG_HARD_TEX ) ) ;
		OrigTex   = Orig->Hard.Tex ;
		for( i = 0; i < DivNum; i++, OrigTex++, OrigTexPF++ )
		{
			OrigTex->PF = OrigTexPF ;
		}
	}

	// �t�H�[�}�b�g���Z�b�g
	Orig->FormatDesc = Format ;

	// ���ߐF���Z�b�g
	Orig->TransCode = GParam->TransColor ;

	// InitGraph �ō폜���邩�ǂ������Z�b�g
	Image->NotInitGraphDelete = GParam->NotInitGraphDelete ;

	// ���ƍ�����ۑ�
	Image->UseOrigXI    = 0 ;
	Image->UseOrigXF    = 0.0f ;
	Image->UseOrigYI    = 0 ;
	Image->UseOrigYF    = 0.0f ;
	Image->WidthI       = Width ;
	Image->WidthF       = ( float )Width ;
	Image->HeightI      = Height ;
	Image->HeightF      = ( float )Height ;
	Image->Orig->Width  = Width ;
	Image->Orig->Height = Height ;

	// �Q�Ɛ���������
	Image->Orig->RefCount = 1 ;

	// �I���W�i���摜���̏�����
	if( Orig->FormatDesc.TextureFlag )
	{
		// �e�N�X�`���̏ꍇ

		// �t�H�[�}�b�g�̌���
		Orig->ColorFormat = NS_GetTexFormatIndex( &Orig->FormatDesc ) ;

		// �e�N�X�`���������ɍ��킹�ăe�N�X�`����񃊃X�g�p�̃��������m�ۂ���
		Orig->Hard.TexNum = DivNum ;

		// �~�b�v�}�b�v�̐����Z�b�g
//		Orig->Hard.MipMap = Orig->FormatDesc.DrawValidFlag ? FALSE : GRH.MipMapFlag ;
/*		if( Orig->ColorFormat == DX_GRAPHICSIMAGE_FORMAT_3D_DXT1 ||
			Orig->ColorFormat == DX_GRAPHICSIMAGE_FORMAT_3D_DXT2 ||
			Orig->ColorFormat == DX_GRAPHICSIMAGE_FORMAT_3D_DXT3 ||
			Orig->ColorFormat == DX_GRAPHICSIMAGE_FORMAT_3D_DXT4 ||
			Orig->ColorFormat == DX_GRAPHICSIMAGE_FORMAT_3D_DXT5 )
		{
			Orig->Hard.MipMapCount = 1 ;
		}
		else*/
		{
			Orig->Hard.MipMapCount = Format.MipMapCount ;
		}

		// ���肵���e�N�X�`���[�T�C�Y�Ńe�N�X�`���[��������������
		OrigTex = Orig->Hard.Tex ;
		for( i = 0 ; i < DivNum ; i ++, OrigTex ++ )
		{
			OrigTex->UseWidth  = Orig->Width  - TexPosX[ i ] ;
			OrigTex->UseHeight = Orig->Height - TexPosY[ i ] ;
			if( OrigTex->UseWidth  > TexSizeX[ i ] ) OrigTex->UseWidth  = TexSizeX[ i ] ; 
			if( OrigTex->UseHeight > TexSizeY[ i ] ) OrigTex->UseHeight = TexSizeY[ i ] ;

			OrigTex->OrigPosX = TexPosX[ i ] ;
			OrigTex->OrigPosY = TexPosY[ i ] ;

			OrigTex->TexWidth  = TexSizeX[ i ] ;
			OrigTex->TexHeight = TexSizeY[ i ] ;
		}

/*
		// �`��\�ŉ𑜓x���o�b�N�o�b�t�@�ȏ�̏ꍇ�͐�p�̂y�o�b�t�@������
		if( Orig->FormatDesc.DrawValidFlag &&
			( Orig->Hard.Tex[ 0 ].TexWidth  > GSYS.Screen.MainScreenSizeX ||
			  Orig->Hard.Tex[ 0 ].TexHeight > GSYS.Screen.MainScreenSizeY ) )
		{
			Orig->ZBufferFlag = TRUE ;
		}
*/
		// �`��\�ȏꍇ�ły�o�b�t�@���쐬����t���O�������Ă���ꍇ�͂y�o�b�t�@������
		if( Orig->FormatDesc.DrawValidFlag && GParam->DrawValidGraphCreateZBufferFlag )
		{
			Orig->ZBufferFlag = TRUE ;
			switch( GParam->CreateDrawValidGraphZBufferBitDepth )
			{
			default :
			case 16 : Orig->ZBufferBitDepthIndex = ZBUFFER_FORMAT_16BIT ; break ;
			case 24 : Orig->ZBufferBitDepthIndex = ZBUFFER_FORMAT_24BIT ; break ;
			case 32 : Orig->ZBufferBitDepthIndex = ZBUFFER_FORMAT_32BIT ; break ;
			}
		}

		// �e�N�X�`���̍쐬
		if( Graphics_Hardware_CreateOrigTexture_PF( Orig, ASyncThread ) == -1 )
		{
			goto ERR ;
		}
	}
	else
	{
		// �e�N�X�`���ł͂Ȃ��ꍇ

		// �u�����h�p�摜�̏ꍇ�Ƃ���ȊO�ŏ����𕪊�
		if( Orig->FormatDesc.BlendGraphFlag == TRUE )
		{
			// �C���[�W�̍쐬
			if( InitializeMemImg( &Orig->Soft.MemImg, Orig->Width, Orig->Height, -1, 0, 2, FALSE, FALSE, FALSE ) < 0 )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xd6\x30\xec\x30\xf3\x30\xc9\x30\x3b\x75\xcf\x50\x28\x75\xe1\x30\xe2\x30\xea\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�u�����h�摜�p�������C���[�W�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
				goto ERR ;
			}
		}
		else
		{
			DWORD TransColor ;

			// �s�N�Z���t�H�[�}�b�g�̃C���f�b�N�X���Z�b�g
			if( Orig->FormatDesc.ColorBitDepth == 16 )
			{
				if( Orig->FormatDesc.AlphaChFlag )	Orig->ColorFormat = DX_GRAPHICSIMAGE_FORMAT_X8A8R5G6B5 ;
				else								Orig->ColorFormat = DX_GRAPHICSIMAGE_FORMAT_R5G6B5 ;
			}
			else
			{
				if( Orig->FormatDesc.AlphaChFlag )	Orig->ColorFormat = DX_GRAPHICSIMAGE_FORMAT_A8R8G8B8 ;
				else								Orig->ColorFormat = DX_GRAPHICSIMAGE_FORMAT_X8R8G8B8 ;
			}
			
			if( Orig->FormatDesc.UsePaletteFlag == TRUE )
			{
				TransColor = 0xffff ;
			}
			else
			{
				TransColor = NS_GetColor3( 
						GetMemImgColorData( Orig->FormatDesc.ColorBitDepth == 16 ? 0 : 1, ( int )Orig->FormatDesc.AlphaChFlag, FALSE ),
						( int )( ( GSYS.CreateImage.TransColor >> 16 ) & 0xff ),
						( int )( ( GSYS.CreateImage.TransColor >> 8  ) & 0xff ),
						( int )( ( GSYS.CreateImage.TransColor >> 0  ) & 0xff ),
						0xff ) ;
			}

			// �C���[�W�̍쐬
			if( InitializeMemImg(
					&Orig->Soft.MemImg,
					Orig->Width,
					Orig->Height,
					-1,
					TransColor,
					Orig->FormatDesc.ColorBitDepth == 16 ? 0 : 1,
					Orig->FormatDesc.UsePaletteFlag,
					Orig->FormatDesc.AlphaChFlag,
					FALSE
				) < 0 )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xe1\x30\xe2\x30\xea\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�������C���[�W�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
				goto ERR ;
			}
		}
	}

	// �`����̏�����
	if( Graphics_Image_InitializeDrawInfo( GrHandle, FALSE, ASyncThread ) < 0 )
	{
		goto ERR ;
	}

	// �I��
	return 0 ;

	// �G���[�I��
ERR :
//	NS_DeleteGraph( GrHandle ) ;

	return -1 ;
}

// �w��̃e�N�X�`���[�T�C�Y����肭��������
extern int Graphics_Image_ListUpTexSize( int Size, short *SizeList, int NotDivFlag, int Pow2Flag, int MaxTextureSize )
{
	int DivNum ;
	int TexSize, GraphSize, MaxTexSize ;

	// �ő�T�C�Y���Z�b�g
	MaxTexSize = MaxTextureSize != 0 ? MaxTextureSize : GSYS.HardInfo.MaxTextureSize ;

	// �Q�̂���ł͂Ȃ��ėǂ��ꍇ�͎w��̃T�C�Y���ő�e�N�X�`���T�C�Y�ȉ��������炻�̂܂ܕԂ�
	if( Pow2Flag == FALSE && ( GSYS.HardInfo.TextureSizePow2 == FALSE || GSYS.HardInfo.TextureSizeNonPow2Conditional ) )
	{
		if( Size <= MaxTexSize )
		{
			if( SizeList ) SizeList[ 0 ] = ( short )Size ;
			return 1 ;
		}
	}

	// �������s���ŏ��T�C�Y�̃e�N�X�`����菬�����ꍇ�͍ŏ��T�C�Y���Z�b�g���ĂP��Ԃ�
	if( MIN_TEXTURE_SIZE >= Size )
	{
		if( SizeList ) SizeList[ 0 ] = MIN_TEXTURE_SIZE ;
		return 1 ;
	}

	// �P�ԍŏ��̃e�N�X�`���[�͎w��T�C�Y�Ɏ��܂�o���邾���傫���e�N�X�`���[�ɂ���
	DivNum = 0 ;
	TexSize = MaxTexSize ;
	while( TexSize > MIN_TEXTURE_SIZE )
	{
		if( TexSize <= Size ) break ;
		TexSize >>= 1 ;
	}

	// �������s��Ȃ��ꍇ�͈ꖇ�Ŏ��߂���ꍇ�͎��߂ďI��
	if( NotDivFlag != FALSE )
	{
		if( ( TexSize << 1 ) <= MaxTexSize )
		{
			if( TexSize < Size ) TexSize <<= 1 ;
			if( SizeList ) SizeList[ 0 ] = ( short )TexSize ;
			return 1 ;
		}
	}
	else
	// �������s���w��̏ꍇ�ł��쐬����摜�̃T�C�Y��
	// ��ʂƓ�������������������A�����[�U�[�w��̃T�C�Y�������ꍇ��
	// �Ȃ�ׂ��������s��Ȃ�
	if( MaxTextureSize == 0 )
	{
		if( Size <= GSYS.Screen.MainScreenSizeX )
		{
			if( ( TexSize << 1 ) <= MaxTexSize )
			{
				if( TexSize < Size ) TexSize <<= 1 ;
				if( SizeList ) SizeList[ 0 ] = ( short )TexSize ;
				return 1 ;
			}
		}
	}

	// ��������
	GraphSize = Size ;
	for(;;)
	{
		if( SizeList ) SizeList[ DivNum ] = ( short )TexSize ;
		GraphSize -= TexSize ;

		if( GraphSize <= 0 ) break ;

		// ������e�N�X�`���[�������T�C�Y���Q�{�ɂ����ق��������ꍇ�𔻒�
		if( ( GraphSize > TexSize * 3 / 4 ) && ( TexSize < MaxTexSize ) )
		{
			if( SizeList ) SizeList[ DivNum ] = ( short )( TexSize << 1 ) ;
			break ;
		}
		else
		{
			// �e�N�X�`���[�̃T�C�Y���c��O���t�B�b�N�T�C�Y�ɍ��킹�ĕ␳����
			if( GraphSize < TexSize )
			{
				while( GraphSize < TexSize  &&  TexSize > MIN_TEXTURE_SIZE )
				{
					TexSize = TexSize >> 1 ;
				}
				DivNum ++ ; 
				continue ;
			}
		}
		DivNum ++ ;
	}

	// �Ō�̃T�C�Y���A�����Ō�̈�O�̃T�C�Y�Ɠ������ꍇ��
	// �Ō�̈�O�̃T�C�Y���Q�{�ɂ��ĕ�����������炷
	if( DivNum > 0 && SizeList[ DivNum - 1 ] == SizeList[ DivNum ] &&
		( SizeList[ DivNum - 1 ] << 1 ) <= MaxTexSize )
	{
		DivNum -- ;
		SizeList[ DivNum ] <<= 1 ;
	}

	// ��������Ԃ�
	return DivNum + 1 ;
}

// �O���t�B�b�N�n���h���̏�����
extern int Graphics_Image_InitializeHandle( HANDLEINFO *HandleInfo )
{
	IMAGEDATA *Image = ( IMAGEDATA * )HandleInfo ;

#ifndef DX_NON_MOVIE
	// ���[�r�[�n���h���͖���
	Image->MovieHandle = -1 ;
#endif

	// �I��
	return 0 ;
}

// �O���t�B�b�N�n���h���̌�n��
extern int Graphics_Image_TerminateHandle( HANDLEINFO *HandleInfo )
{
	IMAGEDATA *Image = ( IMAGEDATA * )HandleInfo ;
	int i ;

	// ���[�r�[�O���t�B�b�N�������ꍇ���̌�n�����s��
#ifndef DX_NON_MOVIE
	CloseMovie( Image->MovieHandle ) ;
#endif

	// �`���ɂȂ��Ă�����`����\��ʂɂ���
	for( i = 0 ; i < GSYS.HardInfo.RenderTargetNum ; i ++ )
	{
		if( GSYS.DrawSetting.TargetScreen[ i ] == Image->HandleInfo.Handle )
		{
			NS_SetRenderTargetToShader( i, ( int )( i == 0 ? ( int )DX_SCREEN_FRONT : -1 ) ) ;
		}
	}

	// �`���y�o�b�t�@�ɂȂ��Ă�����`���𗠉�ʂɂ���
	if( GSYS.DrawSetting.TargetZBuffer == Image->HandleInfo.Handle )
	{
		NS_SetDrawZBuffer( DX_SCREEN_BACK ) ;
	}

	// �I���W�i���f�[�^������ꍇ�̓I���W�i���f�[�^�̉���������s��
	if( Image->Orig )
	{
		// �e�N�X�`�����ǂ����ŏ����𕪊�
		if( Image->Orig->FormatDesc.TextureFlag )
		{
			// �e�N�X�`���̏ꍇ
		}
		else
		{
			// �e�N�X�`���ł͖����ꍇ

			// �`��p MEMIMG �̌�n��
			TerminateMemImg( &Image->Soft.MemImg ) ;
		}

		// �I���W�i���f�[�^�̎Q�Ɛ����f�N�������g
		Image->Orig->RefCount -- ;

		// �O�ɂȂ�����I���W�i���f�[�^�̉���������s��
		if( Image->Orig->RefCount == 0 )
		{
			// �y�o�b�t�@���������
			//ReleaseOrigZBuffer_PF( Image->Orig ) ;

			// �e�N�X�`�����ǂ����ŏ����𕪊�
			if( Image->Orig->FormatDesc.TextureFlag )
			{
				// �e�N�X�`���̏ꍇ

				// �e�N�X�`���I�u�W�F�N�g�̉��
				Graphics_Hardware_ReleaseOrigTexture_PF( Image->Orig ) ;
			}
			else
			{
				// �e�N�X�`���ł͖����ꍇ

				// MEMIMG ���������
				TerminateMemImg( &Image->Orig->Soft.MemImg ) ;
			}

			// �I���W�i���摜�f�[�^�p�Ɋm�ۂ����������̈���J��
			DXFREE( Image->Orig ) ;
		}

		Image->Orig = NULL ;
	}

	// �t���J���[�C���[�W������ꍇ�͉��
	if( Image->FullColorImage != NULL )
	{
		DXFREE( Image->FullColorImage ) ;
		Image->FullColorImage = NULL ;
	}

	// ���b�N�C���[�W������ꍇ�͉��
	if( Image->LockImage != NULL )
	{
		DXFREE( Image->LockImage ) ;
		Image->LockImage = NULL ;
	}

	// �t�@�C�����ۑ��p�Ƀ��������m�ۂ��Ă����ꍇ�͉��
	Graphics_Image_SetGraphBaseInfo( Image->HandleInfo.Handle, NULL, NULL, NULL, NULL, NULL, 0, NULL, 0, NULL, NULL, FALSE, FALSE, -1, FALSE ) ;

	// ������ɗ��Ă�t���O�ւ̃|�C���^���L���ł���ꍇ�͗��Ă�
	if( Image->LostFlag != NULL )
		*Image->LostFlag = TRUE ;

	// �I��
	return 0 ;
}

// �w�蕔�������𔲂��o�����O���t�B�b�N�n���h��������������
extern int Graphics_Image_InitializeDerivationHandle( int GrHandle, int IsFloat, int SrcXI, float SrcXF, int SrcYI, float SrcYF, int WidthI, float WidthF, int HeightI, float HeightF, int SrcGrHandle, int ASyncThread )
{
	IMAGEDATA *Image, *SrcImage ;
	IMAGEDATA_ORIG *Orig ;

	// �A�h���X�̎擾
	if( ASyncThread )
	{
		if( GRAPHCHK_ASYNC( GrHandle, Image ) ||
			GRAPHCHK_ASYNC( SrcGrHandle, SrcImage ) )
			return -1 ;
	}
	else
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;

		if( GRAPHCHK( SrcGrHandle, SrcImage ) )
			return -1 ;
	}
	Orig = SrcImage->Orig ;

	// �I���W�i���摜�̎Q�Ɛ����C���N�������g����
	Image->Orig = Orig ;
	Orig->RefCount ++ ;

	// �t�@�C���̏����R�s�[����
	Graphics_Image_SetName( GrHandle, NULL, FALSE, FALSE, SrcGrHandle, ASyncThread ) ;
	Image->UseBaseXI = SrcImage->UseBaseXI + SrcXI ;
	Image->UseBaseXF = SrcImage->UseBaseXF + SrcXF ;
	Image->UseBaseYI = SrcImage->UseBaseYI + SrcYI ;
	Image->UseBaseYF = SrcImage->UseBaseYF + SrcYF ;

	// �I���W�i���摜���̎Q�ƍ��W���Z�b�g
	Image->UseOrigXI = SrcImage->UseOrigXI + SrcXI ;
	Image->UseOrigXF = SrcImage->UseOrigXF + SrcXF ;
	Image->UseOrigYI = SrcImage->UseOrigYI + SrcYI ;
	Image->UseOrigYF = SrcImage->UseOrigYF + SrcYF ;

	// �T�C�Y��ۑ�
	Image->WidthI = WidthI ;
	Image->WidthF = WidthF ;
	Image->HeightI = HeightI ;
	Image->HeightF = HeightF ;

	// InitGraph �ō폜���邩�ǂ�����ۑ�
	Image->NotInitGraphDelete = SrcImage->NotInitGraphDelete ;

	//�`����̏�����
	Graphics_Image_InitializeDrawInfo( GrHandle, IsFloat, ASyncThread ) ;

	// �I��
	return 0 ;
}

// �O���t�B�b�N�n���h���̕`���������������
extern int Graphics_Image_InitializeDrawInfo( int GrHandle, int IsFloat, int ASyncThread )
{
	IMAGEDATA				*Image ;
	IMAGEDATA_ORIG			*Orig ;
	IMAGEDATA_ORIG_HARD_TEX	*OrigTex ;
	IMAGEDATA_HARD_DRAW		*DrawTex ;
	int						i ;
	int						Num ;
	int						Size ;
	int						Handle ;
	bool					List[ 256 ] ;
	float					d ;

	// �A�h���X�̎擾
	if( ASyncThread )
	{
		if( GRAPHCHK_ASYNC( GrHandle, Image ) )
		{
			return -1 ;
		}
	}
	else
	{
		if( GRAPHCHK( GrHandle, Image ) )
		{
			return -1 ;
		}
	}
	Orig   = Image->Orig ;
	Handle = Image->HandleInfo.Handle ;

	// ��{�m�ۃ������̎Z�o
	Size = sizeof( IMAGEDATA ) - ( ( sizeof( IMAGEDATA_SOFT ) > sizeof( IMAGEDATA_HARD ) ? sizeof( IMAGEDATA_SOFT ) : sizeof( IMAGEDATA_HARD ) ) ) ;

	// �e�N�X�`�����ǂ����ŏ����𕪊�
	if( Orig->FormatDesc.TextureFlag )
	{
		// �e�N�X�`���̏ꍇ

		// ���摜�����镔���̐��𐔂���
		OrigTex = Orig->Hard.Tex ;
		Num = 0 ;
		for( i = 0 ; i < Orig->Hard.TexNum ; i ++, OrigTex ++ )
		{
			if( IsFloat )
			{
				if(
					(
					  (
						OrigTex->OrigPosX <= Image->UseOrigXF  &&
						OrigTex->OrigPosX + OrigTex->UseWidth  > Image->UseOrigXF
					  ) ||
					  (
						Image->UseOrigXF  <= OrigTex->OrigPosX &&
						Image->UseOrigXF  + Image->WidthF      > OrigTex->OrigPosX
					  )
					) &&
					(
					  (
						OrigTex->OrigPosY <= Image->UseOrigYF  &&
						OrigTex->OrigPosY + OrigTex->UseHeight > Image->UseOrigYF
					  ) ||
					  (
						Image->UseOrigYF  <= OrigTex->OrigPosY &&
						Image->UseOrigYF  + Image->HeightF     > OrigTex->OrigPosY
					  )
					)
				  )
				{
					Num ++ ;
					List[ i ] = true ;
				}
				else
				{
					List[ i ] = false ;
				}
			}
			else
			{
				if(
					(
					  (
						OrigTex->OrigPosX <= Image->UseOrigXI  &&
						OrigTex->OrigPosX + OrigTex->UseWidth  > Image->UseOrigXI
					  ) ||
					  (
						Image->UseOrigXI  <= OrigTex->OrigPosX &&
						Image->UseOrigXI  + Image->WidthI      > OrigTex->OrigPosX
					  )
					) &&
					(
					  (
						OrigTex->OrigPosY <= Image->UseOrigYI  &&
						OrigTex->OrigPosY + OrigTex->UseHeight > Image->UseOrigYI
					  ) ||
					  (
						Image->UseOrigYI  <= OrigTex->OrigPosY &&
						Image->UseOrigYI  + Image->HeightI     > OrigTex->OrigPosY
					  )
					)
				  )
				{
					Num ++ ;
					List[ i ] = true ;
				}
				else
				{
					List[ i ] = false ;
				}
			}
		}

		// ��镔�����O�̏ꍇ�̓G���[
		if( Num == 0 )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xc6\x30\xaf\x30\xb9\x30\xc1\x30\xe3\x30\xcf\x63\x3b\x75\xc5\x60\x31\x58\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�e�N�X�`���`����̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}

		// �`������i�[���郁�����̈悪����Ȃ��ꍇ�͍Ċm��
		if( Num > 4 )
		{
			if( ReallocHandle( Handle, Size + sizeof( IMAGEDATA_HARD ) + ( Num - 4 ) * sizeof( IMAGEDATA_HARD_DRAW ) ) < 0 )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x3b\x75\xcf\x50\xcf\x63\x3b\x75\xc5\x60\x31\x58\x3c\x68\x0d\x7d\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�摜�`����i�[�p�������̊m�ۂɎ��s���܂���\n" @*/ ) ;
				return -1 ;
			}
			Image = ( IMAGEDATA * )GetHandleInfo( Handle ) ;
			Orig = Image->Orig ;
		}

		Image->Hard.DrawNum = Num ;

		// �`������Z�b�g����
		d = Image->HeightF / Image->WidthF ;
		OrigTex = Orig->Hard.Tex ;
		DrawTex = Image->Hard.Draw ;
		Num     = 0 ;
		for( i = 0 ; i < Orig->Hard.TexNum ; i ++, OrigTex ++ )
		{
			if( List[ i ] == false ) continue ;

			// �g�p����I���W�i���摜�e�N�X�`�����̃A�h���X���Z�b�g
			DrawTex->Tex = OrigTex ;

			// �I���W�i���摜�e�N�X�`�����̂ǂ̕������g�p���邩�A�Ȃǂ̏����Z�b�g
			if( IsFloat )
			{
				if( OrigTex->OrigPosX > Image->UseOrigXF )
				{
					DrawTex->DrawPosXF = OrigTex->OrigPosX - Image->UseOrigXF ;
					DrawTex->UsePosXF  = 0.0f ;

					if( OrigTex->OrigPosX + OrigTex->UseWidth > Image->UseOrigXF + Image->WidthF )
					{
						DrawTex->WidthF = Image->UseOrigXF + Image->WidthF - OrigTex->OrigPosX ;
					}
					else
					{
						DrawTex->WidthF = ( float )OrigTex->UseWidth ;
					}
				}
				else
				{
					DrawTex->DrawPosXF = 0.0f ;
					DrawTex->UsePosXF  = Image->UseOrigXF - OrigTex->OrigPosX ;

					if( OrigTex->OrigPosX + OrigTex->UseWidth > Image->UseOrigXF + Image->WidthF )
					{
						DrawTex->WidthF = Image->WidthF ;
					}
					else
					{
						DrawTex->WidthF = OrigTex->OrigPosX + OrigTex->UseWidth - Image->UseOrigXF ;
					}
				}

				if( OrigTex->OrigPosY > Image->UseOrigYF )
				{
					DrawTex->DrawPosYF = OrigTex->OrigPosY - Image->UseOrigYF ;
					DrawTex->UsePosYF  = 0.0f ;

					if( OrigTex->OrigPosY + OrigTex->UseHeight > Image->UseOrigYF + Image->HeightF )
					{
						DrawTex->HeightF = Image->UseOrigYF + Image->HeightF - OrigTex->OrigPosY ;
					}
					else
					{
						DrawTex->HeightF = ( float )OrigTex->UseHeight ;
					}
				}
				else
				{
					DrawTex->DrawPosYF = 0.0f ;
					DrawTex->UsePosYF  = Image->UseOrigYF - OrigTex->OrigPosY ;

					if( OrigTex->OrigPosY + OrigTex->UseHeight > Image->UseOrigYF + Image->HeightF )
					{
						DrawTex->HeightF = Image->HeightF ;
					}
					else
					{
						DrawTex->HeightF = OrigTex->OrigPosY + OrigTex->UseHeight - Image->UseOrigYF ;
					}
				}

				// �����l�̊���o��
				{
					float lTempWidthF ;
					float lTempHeightF ;

					DrawTex->DrawPosXI = _FTOL( DrawTex->DrawPosXF ) ;
					DrawTex->DrawPosYI = _FTOL( DrawTex->DrawPosYF ) ;
					DrawTex->UsePosXI  = _FTOL( DrawTex->UsePosXF  ) ;
					DrawTex->UsePosYI  = _FTOL( DrawTex->UsePosYF  ) ;

					lTempWidthF  = ( DrawTex->UsePosXF + DrawTex->WidthF  ) - DrawTex->UsePosXI ;
					lTempHeightF = ( DrawTex->UsePosYF + DrawTex->HeightF ) - DrawTex->UsePosYI ;
					DrawTex->WidthI  = _FTOL( lTempWidthF ) ;
					DrawTex->HeightI = _FTOL( lTempHeightF ) ;

					if( lTempWidthF - DrawTex->WidthI > 0.01f )
					{
						DrawTex->WidthI ++ ;
						if( DrawTex->UsePosXI + DrawTex->WidthI > OrigTex->UseWidth )
						{
							DrawTex->WidthI = OrigTex->UseWidth - ( DrawTex->UsePosXI + DrawTex->WidthI ) ;
						}
					}

					if( lTempHeightF - DrawTex->HeightI > 0.01f )
					{
						DrawTex->HeightI ++ ;
						if( DrawTex->UsePosYI + DrawTex->HeightI > OrigTex->UseHeight )
						{
							DrawTex->HeightI = OrigTex->UseHeight - ( DrawTex->UsePosYI + DrawTex->HeightI ) ;
						}
					}
				}
			}
			else
			{
				if( OrigTex->OrigPosX > Image->UseOrigXI )
				{
					DrawTex->DrawPosXI = OrigTex->OrigPosX - Image->UseOrigXI ;
					DrawTex->UsePosXI  = 0 ;

					if( OrigTex->OrigPosX + OrigTex->UseWidth > Image->UseOrigXI + Image->WidthI )
					{
						DrawTex->WidthI = Image->UseOrigXI + Image->WidthI - OrigTex->OrigPosX ;
					}
					else
					{
						DrawTex->WidthI = OrigTex->UseWidth ;
					}
				}
				else
				{
					DrawTex->DrawPosXI = 0 ;
					DrawTex->UsePosXI  = Image->UseOrigXI - OrigTex->OrigPosX ;

					if( OrigTex->OrigPosX + OrigTex->UseWidth > Image->UseOrigXI + Image->WidthI )
					{
						DrawTex->WidthI = Image->WidthI ;
					}
					else
					{
						DrawTex->WidthI = OrigTex->OrigPosX + OrigTex->UseWidth - Image->UseOrigXI ;
					}
				}

				if( OrigTex->OrigPosY > Image->UseOrigYI )
				{
					DrawTex->DrawPosYI = OrigTex->OrigPosY - Image->UseOrigYI ;
					DrawTex->UsePosYI  = 0 ;

					if( OrigTex->OrigPosY + OrigTex->UseHeight > Image->UseOrigYI + Image->HeightI )
					{
						DrawTex->HeightI = Image->UseOrigYI + Image->HeightI - OrigTex->OrigPosY ;
					}
					else
					{
						DrawTex->HeightI = OrigTex->UseHeight ;
					}
				}
				else
				{
					DrawTex->DrawPosYI = 0 ;
					DrawTex->UsePosYI  = Image->UseOrigYI - OrigTex->OrigPosY ;

					if( OrigTex->OrigPosY + OrigTex->UseHeight > Image->UseOrigYI + Image->HeightI )
					{
						DrawTex->HeightI = Image->HeightI ;
					}
					else
					{
						DrawTex->HeightI = OrigTex->OrigPosY + OrigTex->UseHeight - Image->UseOrigYI ;
					}
				}

				// ���������_�l�̃Z�b�g
				{
					DrawTex->DrawPosXF = ( float )DrawTex->DrawPosXI ;
					DrawTex->DrawPosYF = ( float )DrawTex->DrawPosYI ;
					DrawTex->UsePosXF  = ( float )DrawTex->UsePosXI ;
					DrawTex->UsePosYF  = ( float )DrawTex->UsePosYI ;
					DrawTex->WidthF    = ( float )DrawTex->WidthI ;
					DrawTex->HeightF   = ( float )DrawTex->HeightI ;
				}
			}

			// �Z�o�����g�p���񂩂璸�_�f�[�^�Ɏg�p����l�̊���o��
			DrawTex->Vertex[ 0 ].x =   DrawTex->DrawPosXF ;
			DrawTex->Vertex[ 1 ].x = ( DrawTex->DrawPosXF + DrawTex->WidthF ) ;
			DrawTex->Vertex[ 2 ].x =   DrawTex->DrawPosXF ;
			DrawTex->Vertex[ 3 ].x = ( DrawTex->DrawPosXF + DrawTex->WidthF ) ;

			DrawTex->Vertex[ 0 ].y =   DrawTex->DrawPosYF ;
			DrawTex->Vertex[ 1 ].y =   DrawTex->DrawPosYF ;
			DrawTex->Vertex[ 2 ].y = ( DrawTex->DrawPosYF + DrawTex->HeightF ) ;
			DrawTex->Vertex[ 3 ].y = ( DrawTex->DrawPosYF + DrawTex->HeightF ) ;

			DrawTex->Vertex[ 0 ].u =   DrawTex->UsePosXF                     / ( float )OrigTex->TexWidth ;
			DrawTex->Vertex[ 1 ].u = ( DrawTex->UsePosXF + DrawTex->WidthF ) / ( float )OrigTex->TexWidth ;
			DrawTex->Vertex[ 2 ].u =   DrawTex->UsePosXF                     / ( float )OrigTex->TexWidth ;
			DrawTex->Vertex[ 3 ].u = ( DrawTex->UsePosXF + DrawTex->WidthF ) / ( float )OrigTex->TexWidth ;

			DrawTex->Vertex[ 0 ].v =   DrawTex->UsePosYF                      / ( float )OrigTex->TexHeight ;
			DrawTex->Vertex[ 1 ].v =   DrawTex->UsePosYF                      / ( float )OrigTex->TexHeight ;
			DrawTex->Vertex[ 2 ].v = ( DrawTex->UsePosYF + DrawTex->HeightF ) / ( float )OrigTex->TexHeight ;
			DrawTex->Vertex[ 3 ].v = ( DrawTex->UsePosYF + DrawTex->HeightF ) / ( float )OrigTex->TexHeight ;

			// ���_�^�C�v�̃Z�b�g
			DrawTex->VertType[ 0 ] = ( unsigned char )( DrawTex->Vertex[ 0 ].y <= Image->HeightF - ( d * DrawTex->Vertex[ 0 ].x ) ? 1 : 0 ) ;
			DrawTex->VertType[ 1 ] = ( unsigned char )( DrawTex->Vertex[ 1 ].y <= Image->HeightF - ( d * DrawTex->Vertex[ 1 ].x ) ? 1 : 0 ) ;
			DrawTex->VertType[ 2 ] = ( unsigned char )( DrawTex->Vertex[ 2 ].y <= Image->HeightF - ( d * DrawTex->Vertex[ 2 ].x ) ? 1 : 0 ) ;
			DrawTex->VertType[ 3 ] = ( unsigned char )( DrawTex->Vertex[ 3 ].y <= Image->HeightF - ( d * DrawTex->Vertex[ 3 ].x ) ? 1 : 0 ) ;

			DrawTex ++ ;
		}
	}
	else
	{
		// �e�N�X�`���ł͂Ȃ��ꍇ

		// �`��p MEMIMG �̏���
		DerivationMemImg( &Image->Soft.MemImg, &Orig->Soft.MemImg, Image->UseOrigXI, Image->UseOrigYI, Image->WidthI, Image->HeightI ) ;
	}

	// �I��
	return 0 ;
}

// �O���t�B�b�N�n���h�����L�����ǂ����𒲂ׂ�( TRUE:�L��  FALSE:���� )
extern	int Graphics_Image_IsValidHandle( int GrHandle )
{
	IMAGEDATA *Image ;

	// �G���[����
	if( GRAPHCHKFULL( GrHandle, Image ) )
		return FALSE ;

	// �I��
	return TRUE ;
}

// GraphImage �f�[�^����T�C�Y������o���A����ɍ������O���t�B�b�N�n���h�����쐬����
extern int Graphics_Image_CreateDXGraph_UseGParam(
	SETUP_GRAPHHANDLE_GPARAM *GParam,
	int GrHandle,
	const BASEIMAGE *RgbBaseImage,
	const BASEIMAGE *AlphaBaseImage,
	int TextureFlag,
	int ASyncThread
)
{
	SETUP_GRAPHHANDLE_GPARAM TempGParam ;

	if( GSYS.InitializeFlag == FALSE ) return -1 ;

	if( ASyncThread == FALSE )
	{
		CheckActiveState() ;
	}

	// ��̃O���t�B�b�N�̃T�C�Y��������ꍇ�̓G���[
	if( AlphaBaseImage != NULL && ( RgbBaseImage->Width != AlphaBaseImage->Width || RgbBaseImage->Height != AlphaBaseImage->Height ) )
		return -1 ;

	// �L���[�u�}�b�v�e�N�X�`�����쐬���邩�ǂ������Z�b�g
	GParam->CubeMapTextureCreateFlag = RgbBaseImage->GraphDataCount == CUBEMAP_SURFACE_NUM ? TRUE : FALSE ;

	// �n���h���̏�����
	{
		int UsePaletteFlag ;
		int PaletteColorBitDepth = 0 ;

		// �p���b�g�e�N�X�`�����g�p���邩�ǂ������Z�b�g
		UsePaletteFlag = RgbBaseImage->ColorData.PixelByte  == 1 && 
						 RgbBaseImage->ColorData.AlphaWidth == 0 &&
						 AlphaBaseImage == NULL ;

		// �O���t�B�b�N�X�f�o�C�X��4bit�p���b�g�e�N�X�`���ɑΉ����Ă���ꍇ�ŁA����8bit�J���[�̏ꍇ��
		// �g�p����Ă���ő�p���b�g�ԍ��� 4bit �Ɏ��܂邩�ǂ����𒲂ׁA���܂�ꍇ�� 4bit �p���b�g�e�N�X�`���ɂ���
		if( UsePaletteFlag )
		{
			if( GSYS.HardInfo.Support4bitPaletteTexture &&
				RgbBaseImage->ColorData.ColorBitDepth == 8 )
			{
				if( RgbBaseImage->ColorData.MaxPaletteNo == 0 || RgbBaseImage->ColorData.MaxPaletteNo == 255 )
				{
					PaletteColorBitDepth = NS_GetBaseImageUseMaxPaletteNo( RgbBaseImage ) < 16 ? 4 : 8 ;
				}
				else
				{
					PaletteColorBitDepth = RgbBaseImage->ColorData.MaxPaletteNo < 16 ? 4 : 8 ;
				}
			}
			else
			{
				PaletteColorBitDepth = 8 ;
			}
		}

		// BaseImage �̃J���[���Ń`�����l������`�����l���r�b�g�����w�肳��Ă���ꍇ�� GParam �ɒl�𔽉f����
		if( RgbBaseImage->ColorData.ChannelNum != 0 &&
			RgbBaseImage->ColorData.ChannelBitDepth != 0 )
		{
			TempGParam                                   = *GParam ;
			TempGParam.DrawValidImageCreateFlag          = GParam->DrawValidImageCreateFlag ;
			TempGParam.CreateImageChannelBitDepth        = RgbBaseImage->ColorData.ChannelBitDepth ;
			if( GParam->DrawValidImageCreateFlag )
			{
				TempGParam.DrawValidFloatTypeGraphCreateFlag	= RgbBaseImage->ColorData.FloatTypeFlag ;
				TempGParam.CreateDrawValidGraphChannelNum		= RgbBaseImage->ColorData.ChannelNum ;
			}
			else
			{
				TempGParam.FloatTypeGraphCreateFlag				= RgbBaseImage->ColorData.FloatTypeFlag ;
				TempGParam.CreateImageChannelNum				= RgbBaseImage->ColorData.ChannelNum ;
			}

			GParam = &TempGParam ;
		}

		if( Graphics_Image_SetupHandle_UseGParam(
				GParam,
				GrHandle,
				RgbBaseImage->Width, RgbBaseImage->Height, TextureFlag,
				RgbBaseImage->ColorData.AlphaWidth != 0 || AlphaBaseImage != NULL,
				UsePaletteFlag,
				PaletteColorBitDepth,
				RgbBaseImage->ColorData.Format,
				RgbBaseImage->MipMapCount == 0 ? -1 : RgbBaseImage->MipMapCount,
				ASyncThread ) == -1 )
			return -1 ;
	}

	// �I��
	return 0 ;
}

// DerivationGraph �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_DerivationGraph_UseGParam(
	int IsFloat,
	int SrcXI,   float SrcXF,
	int SrcYI,   float SrcYF,
	int WidthI,  float WidthF,
	int HeightI, float HeightF,
	int SrcGraphHandle,
	int ASyncThread
)
{
	int NewGraphHandle ;
	int Result ;

	// �V���ȃO���t�B�b�N�f�[�^�̒ǉ�
	NewGraphHandle = Graphics_Image_AddHandle( ASyncThread ) ;
	if( NewGraphHandle == -1 )
	{
		return -1 ;
	}

	Result = Graphics_Image_DerivationGraphBase( NewGraphHandle, IsFloat, SrcXI, SrcXF, SrcYI, SrcYF, WidthI, WidthF, HeightI, HeightF, SrcGraphHandle, ASyncThread ) ;
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( NewGraphHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD
	if( Result < 0 )
	{
		NS_DeleteGraph( NewGraphHandle ) ;
		return -1 ;
	}

	// �n���h����Ԃ�
	return NewGraphHandle ;
}

#ifndef DX_NON_MOVIE

extern int Graphics_Image_OpenMovie_UseGParam(
	LOADGRAPH_GPARAM *GParam,
	int GrHandle,
	const wchar_t *GraphName,
	const void *FileImage,
	size_t FileImageSize,
	int TextureFlag,
	int SurfaceMode,
	int ASyncThread
)
{
	int MovieHandle ;
	int Width, Height ;
	MOVIEGRAPH *Movie;
	IMAGEDATA *Image ;

	// ���[�r�[�O���t�B�b�N�Ƃ��Ĉ������Ƃ��Ă݂�
	MovieHandle = OpenMovie_UseGParam( &GParam->OpenMovieGParam, GraphName, FileImage, FileImageSize, &Width, &Height, SurfaceMode, ASyncThread ) ;
	if( MovieHandle < 0 )
	{
		DXST_ERRORLOGFMT_ADDW(( L"Image File Load Error : %s \n", GraphName )) ;
		return -1 ;
	}

	Movie = GetMovieData( MovieHandle );

	// ��ʂ̃J���[�r�b�g�f�v�X�ƑΉ�����悤�ɃZ�b�g�A�y�тu�q�`�l���g�p�Z�b�g
	GParam->CreateGraphGParam.InitHandleGParam.CreateImageColorBitDepth = NS_GetColorBitDepth() ;
#ifndef DX_NON_DSHOW_MOVIE
	#ifndef DX_NON_OGGTHEORA
		GParam->CreateGraphGParam.InitHandleGParam.AlphaChannelImageCreateFlag = Movie->A8R8G8B8Flag && Movie->TheoraFlag == FALSE && Movie->PF.pMovieImage->ImageType == 1 ;
	#else // DX_NON_OGGTHEORA
		GParam->CreateGraphGParam.InitHandleGParam.AlphaChannelImageCreateFlag = Movie->A8R8G8B8Flag && Movie->PF.pMovieImage->ImageType == 1 ;
	#endif // DX_NON_OGGTHEORA
#else // DX_NON_DSHOW_MOVIE
	GParam->CreateGraphGParam.InitHandleGParam.AlphaChannelImageCreateFlag = FALSE ;
#endif // DX_NON_DSHOW_MOVIE
	GParam->CreateGraphGParam.InitHandleGParam.CreateImageChannelNum = 0 ;
	GParam->CreateGraphGParam.InitHandleGParam.CreateImageChannelBitDepth = 0 ;
	GParam->CreateGraphGParam.InitHandleGParam.NotUseDivFlag = TRUE ;
	GParam->CreateGraphGParam.InitHandleGParam.AlphaTestImageCreateFlag = FALSE ;
	GParam->CreateGraphGParam.InitHandleGParam.CubeMapTextureCreateFlag = FALSE ;
	GParam->CreateGraphGParam.InitHandleGParam.UseManagedTextureFlag = FALSE ;
	GParam->CreateGraphGParam.InitHandleGParam.MipMapCount = 0 ;
	GParam->CreateGraphGParam.InitHandleGParam.UseLinearMapTextureFlag = TRUE ;
	GParam->CreateGraphGParam.InitHandleGParam.PlatformTextureFormat = 0 ;
#ifndef DX_NON_OGGTHEORA
	if( Movie->TheoraFlag && /* Movie->RightAlpha == FALSE && */ GSYS.HardInfo.OggTheoraUseDrawValidGraph )
	{
		GParam->CreateGraphGParam.InitHandleGParam.DrawValidImageCreateFlag = TRUE ;
		GParam->CreateGraphGParam.InitHandleGParam.DrawValidAlphaImageCreateFlag = FALSE ;
		GParam->CreateGraphGParam.InitHandleGParam.DrawValidFloatTypeGraphCreateFlag = FALSE ;
		GParam->CreateGraphGParam.InitHandleGParam.DrawValidGraphCreateZBufferFlag = FALSE ;
		GParam->CreateGraphGParam.InitHandleGParam.CreateDrawValidGraphChannelNum = 0 ;
		GParam->CreateGraphGParam.InitHandleGParam.CreateDrawValidGraphZBufferBitDepth = 0 ;
		GParam->CreateGraphGParam.InitHandleGParam.DrawValidMSSamples = 0 ;
		GParam->CreateGraphGParam.InitHandleGParam.DrawValidMSQuality = 0 ;
	}
	else
#endif // DX_NON_OGGTHEORA
	if( Movie->RightAlpha == FALSE && GSYS.HardInfo.UseShader && ( Movie->A8R8G8B8Flag == FALSE
#ifndef DX_NON_DSHOW_MOVIE
		|| Movie->PF.pMovieImage->ImageType != 1
#endif // DX_NON_DSHOW_MOVIE
		) )
	{
		GParam->CreateGraphGParam.InitHandleGParam.DrawValidImageCreateFlag = TRUE ;
		GParam->CreateGraphGParam.InitHandleGParam.DrawValidAlphaImageCreateFlag = FALSE ;
		GParam->CreateGraphGParam.InitHandleGParam.DrawValidFloatTypeGraphCreateFlag = FALSE ;
		GParam->CreateGraphGParam.InitHandleGParam.DrawValidGraphCreateZBufferFlag = FALSE ;
		GParam->CreateGraphGParam.InitHandleGParam.CreateDrawValidGraphChannelNum = 0 ;
		GParam->CreateGraphGParam.InitHandleGParam.CreateDrawValidGraphZBufferBitDepth = 0 ;
		GParam->CreateGraphGParam.InitHandleGParam.DrawValidMSSamples = 0 ;
		GParam->CreateGraphGParam.InitHandleGParam.DrawValidMSQuality = 0 ;
	}

	// �ǂ݂��񂾃r�b�g�}�b�v�̃T�C�Y�̃O���t�B�b�N�f�[�^���쐬����
	// �E�����������`�����l���Ƃ��Ĉ����ꍇ�̓��`�����l���t���摜�ɂ���
	if( Graphics_Image_SetupHandle_UseGParam(
			&GParam->CreateGraphGParam.InitHandleGParam,
			GrHandle,
			Movie->RightAlpha ? Width / 2 : Width,
			Height,
			TextureFlag,
			Movie->RightAlpha || GParam->CreateGraphGParam.InitHandleGParam.AlphaChannelImageCreateFlag,
			FALSE,
			0,
			DX_BASEIMAGE_FORMAT_NORMAL,
			-1,
			ASyncThread
		) == -1 )
	{
		DXST_ERRORLOGFMT_ADDW(( L"Graph Handle Setup Error : %s", GraphName )) ;
		return -1 ;
	}

	Image = Graphics_Image_GetData( GrHandle, ASyncThread ) ;
	if( Image )
	{
		// ���[�r�[�̃n���h�����Z�b�g
		Image->MovieHandle = MovieHandle ;

		// �R�[���o�b�N�֐����Z�b�g
		SetCallbackMovie( MovieHandle, ( void (*)( MOVIEGRAPH *, void * ) )Graphics_Image_UpdateGraphMovie, ( void * )( DWORD_PTR )GrHandle ) ;
	}

	// ����I��
	return 0 ;
}

#endif // DX_NON_MOVIE

// �O���t�B�b�N�n���h�����쐬���Ȃ� DerivationGraph
extern int Graphics_Image_DerivationGraphBase( int GrHandle, int IsFloat, int SrcXI, float SrcXF, int SrcYI, float SrcYF, int WidthI, float WidthF, int HeightI, float HeightF, int SrcGraphHandle, int ASyncThread )
{
	IMAGEDATA *Image ;

//	if( GSYS.NotDrawFlag == TRUE ) return 0 ;

	// �G���[����
	if( ASyncThread )
	{
		if( GRAPHCHK_ASYNC( SrcGraphHandle, Image ) )
			return -1 ;
	}
	else
	{
		if( GRAPHCHK( SrcGraphHandle, Image ) )
			return -1 ;
	}

	// �␳
	if( SrcXI < 0 || SrcYI < 0 || SrcXI >= Image->WidthI || SrcYI >= Image->HeightI ||
		SrcXF < 0 || SrcYF < 0 || SrcXF >= Image->WidthF || SrcYF >= Image->HeightF ) return -1 ;
	if( SrcXI + WidthI  > Image->WidthI  ) WidthI  = Image->WidthI  - SrcXI ;
	if( SrcXF + WidthF  > Image->WidthF  ) WidthF  = Image->WidthF  - SrcXF ;
	if( SrcYI + HeightI > Image->HeightI ) HeightI = Image->HeightI - SrcYI ;
	if( SrcYF + HeightF > Image->HeightF ) HeightF = Image->HeightF - SrcYF ;

	// �����o���O���t�B�b�N�̏�����
	if( Graphics_Image_InitializeDerivationHandle( GrHandle, IsFloat, SrcXI, SrcXF, SrcYI, SrcYF, WidthI, WidthF, HeightI, HeightF, SrcGraphHandle, ASyncThread ) == -1 )
		return -1 ;

	// ����I��
	return 0 ;
}

// Graphics_Image_SetGraphBaseInfo �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_SetBaseInfo_UseGParam(
	SETGRAPHBASEINFO_GPARAM *GParam,
	int GrHandle,
	const wchar_t *FileName,
	const COLORDATA *BmpColorData,
	HBITMAP RgbBmp,
	HBITMAP AlphaBmp,
	const void *MemImage,
	int MemImageSize,
	const void *AlphaMemImage,
	int AlphaMemImageSize,
	const BASEIMAGE *BaseImage,
	const BASEIMAGE *AlphaBaseImage,
	int ReverseFlag,
	int ConvertPremultipliedAlpha,
	int UnionGrHandle,
	int ASyncThread
)
{
	IMAGEDATA *Image, *SrcImage = NULL ;
	wchar_t Path[ 1024 ] ;
	int Len ;

	if( ASyncThread )
	{
		if( GRAPHCHK_ASYNC( GrHandle, Image ) )
			return -1 ;
		GRAPHCHKFULL_ASYNC( UnionGrHandle, SrcImage ) ;
	}
	else
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
		GRAPHCHKFULL( UnionGrHandle, SrcImage ) ;
	}

	// ���Ƀt�@�C���p�X�p�̃��������m�ۂ���Ă�������
	if( Image->ReadBase )
	{
		Image->ReadBase->RefCount -- ;
		if( Image->ReadBase->RefCount == 0 )
		{
			DXFREE( Image->ReadBase ) ;
			Image->ReadBase = NULL ;
		}
	}

	// �r�b�g�}�b�v���L���ȏꍇ�� BASEIMAGE �\���̂��\�z����
	if( RgbBmp )
	{
#ifdef __WINDOWS__
		BITMAP BitmapInfo ;
		BITMAP AlphaBitmapInfo ;
		BASEIMAGE BmpImage ;
		BASEIMAGE AlphaBmpImage ;

		GetObject( RgbBmp, sizeof( BITMAP ), ( void * )&BitmapInfo ) ;
		_MEMSET( &BmpImage, 0, sizeof( BASEIMAGE ) ) ;
		BmpImage.GraphData = BitmapInfo.bmBits ;
		BmpImage.Width = BitmapInfo.bmWidth ;
		BmpImage.Height = BitmapInfo.bmHeight ;
		BmpImage.Pitch = ( BmpImage.Pitch + 3 ) / 4 * 4 ;
		_MEMCPY( &BmpImage.ColorData, BmpColorData, sizeof( COLORDATA ) ) ; 
		BaseImage = &BmpImage ;

		if( AlphaBmp )
		{
			GetObject( AlphaBmp, sizeof( BITMAP ), ( void * )&AlphaBitmapInfo ) ;
			_MEMSET( &AlphaBmpImage, 0, sizeof( BASEIMAGE ) ) ;
			AlphaBmpImage.GraphData = AlphaBitmapInfo.bmBits ;
			AlphaBmpImage.Width = AlphaBitmapInfo.bmWidth ;
			AlphaBmpImage.Height = AlphaBitmapInfo.bmHeight ;
			AlphaBmpImage.Pitch = ( AlphaBmpImage.Pitch + 3 ) / 4 * 4 ;
			_MEMCPY( &AlphaBmpImage.ColorData, BmpColorData, sizeof( COLORDATA ) ) ; 
			AlphaBaseImage = &AlphaBmpImage ;
		}
#else // __WINDOWS__
		return -1 ;
#endif // __WINDOWS__
	}

	// ���f�[�^�̏�񂪈�؂Ȃ����A�o�b�N�A�b�v�����ۂ���t���O�������Ă����ꍇ�̓o�b�N�A�b�v�����Ȃ�
	if( GParam->NotGraphBaseDataBackupFlag || ( FileName == NULL && MemImage == NULL && BaseImage == NULL && SrcImage == NULL ) )
	{
		Image->ReadBase = NULL ;
	}
	else
	{
		// ���L�n���h���̎w�肪����ꍇ�͋��L�����Z�b�g����
		if( SrcImage != NULL )
		{
			// �t�@�C�������̃A�h���X��Ⴄ
			Image->ReadBase = SrcImage->ReadBase ;

			// �Q�Ɛ��𑝂₷
			if( SrcImage->ReadBase )
			{
				SrcImage->ReadBase->RefCount ++ ;
			}
		}
		else
		{
			// �t�@�C�������L���ȏꍇ�̓t�@�C������ۑ�
			if( FileName )
			{
				// �t���p�X���擾
				ConvertFullPathW_( FileName, Path, sizeof( Path ) ) ;

				// �t�@�C�����ۑ��p�̃��������m��
				Len = ( int )_WCSLEN( Path ) ;
				Image->ReadBase = ( IMAGEDATA_READBASE * )DXCALLOC( sizeof( IMAGEDATA_READBASE ) + ( Len + 1 ) * sizeof( wchar_t ) ) ;
				if( Image->ReadBase == NULL )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xe2\x8e\x01\x90\x59\x30\x8b\x30\x3b\x75\xcf\x50\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x0d\x54\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�]������摜�t�@�C���̃t�@�C������ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ ) ;
					return -1 ;
				}
				Image->ReadBase->FileName = ( wchar_t * )( Image->ReadBase + 1 ) ;

				// �t�@�C���p�X��ۑ�
				_MEMCPY( Image->ReadBase->FileName, Path, ( Len + 1 ) * sizeof( wchar_t ) ) ;
			}
			else
			// �������C���[�W���L���ȏꍇ�̓������C���[�W��ۑ�
			if( MemImage )
			{
				// �������C���[�W�ۑ��p�̃��������m��
				Image->ReadBase = ( IMAGEDATA_READBASE * )DXALLOC( sizeof( IMAGEDATA_READBASE ) + MemImageSize + AlphaMemImageSize ) ;
				if( Image->ReadBase == NULL )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xe2\x8e\x01\x90\x59\x30\x8b\x30\x3b\x75\xcf\x50\xc7\x30\xfc\x30\xbf\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�]������摜�f�[�^��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ ) ;
					return -1 ;
				}
				_MEMSET( Image->ReadBase, 0, sizeof( IMAGEDATA_READBASE ) ) ;

				// �������C���[�W��ۑ�
				Image->ReadBase->MemImage = Image->ReadBase + 1 ;
				Image->ReadBase->MemImageSize = MemImageSize ;
				_MEMCPY( Image->ReadBase->MemImage, MemImage, ( size_t )MemImageSize ) ;

				if( AlphaMemImage )
				{
					Image->ReadBase->AlphaMemImage = ( BYTE * )Image->ReadBase->MemImage + MemImageSize ;
					Image->ReadBase->AlphaMemImageSize = AlphaMemImageSize ;
					_MEMCPY( Image->ReadBase->AlphaMemImage, AlphaMemImage, ( size_t )AlphaMemImageSize ) ;
				}
			}
			else
			// ��{�C���[�W�f�[�^���L���ȏꍇ�͊�{�C���[�W�f�[�^��ۑ�
			if( BaseImage )
			{
				unsigned int ImageSize, AlphaImageSize = 0 ;
				unsigned int AllocSize ;

				ImageSize =  ( unsigned int )NS_GetBaseImageGraphDataSize( BaseImage ) ;
				AllocSize = ImageSize + sizeof( BASEIMAGE ) ;
				if( AlphaBaseImage )
				{
					AlphaImageSize = ( unsigned int )NS_GetBaseImageGraphDataSize( AlphaBaseImage ) ;
					AllocSize += AlphaImageSize + sizeof( BASEIMAGE ) ;
				}

				// ��{�C���[�W�ۑ��p�̃��������m��
				Image->ReadBase = ( IMAGEDATA_READBASE * )DXALLOC( sizeof( IMAGEDATA_READBASE ) + AllocSize ) ;
				if( Image->ReadBase == NULL )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xe2\x8e\x01\x90\x59\x30\x8b\x30\xfa\x57\x2c\x67\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�]�������{�C���[�W��ۑ����郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ ) ;
					return -1 ;
				}
				_MEMSET( Image->ReadBase, 0, sizeof( IMAGEDATA_READBASE ) ) ;

				// ��{�C���[�W���������ɕۑ�
				Image->ReadBase->BaseImage = ( BASEIMAGE * )( Image->ReadBase + 1 ) ;
				_MEMCPY( Image->ReadBase->BaseImage, BaseImage, sizeof( BASEIMAGE ) ) ;
				Image->ReadBase->BaseImage->GraphData = Image->ReadBase->BaseImage + 1 ;
				_MEMCPY( Image->ReadBase->BaseImage->GraphData, BaseImage->GraphData, ImageSize ) ;

				if( AlphaBaseImage )
				{
					Image->ReadBase->AlphaBaseImage = ( BASEIMAGE * )( ( BYTE * )Image->ReadBase->BaseImage->GraphData + ImageSize ) ;
					_MEMCPY( Image->ReadBase->AlphaBaseImage, AlphaBaseImage, sizeof( BASEIMAGE ) ) ;
					Image->ReadBase->AlphaBaseImage->GraphData = Image->ReadBase->AlphaBaseImage + 1 ;
					_MEMCPY( Image->ReadBase->AlphaBaseImage->GraphData, AlphaBaseImage->GraphData, AlphaImageSize ) ;
				}
			}

			// ���]�t���O��ۑ�����
			Image->ReadBase->ReverseFlag = ReverseFlag ;

			// ��Z�ς݃��ϊ��t���O��ۑ�����
			Image->ReadBase->ConvertPremultipliedAlpha = ConvertPremultipliedAlpha ;

			// �Q�Ɛ����P�ɂ���
			Image->ReadBase->RefCount = 1 ;
		}
	}

	// �I��
	return 0 ;
}

// �摜�̌��f�[�^�̏���ۑ�����
extern int Graphics_Image_SetGraphBaseInfo(
	int GrHandle,
	const wchar_t *FileName,
	const COLORDATA *BmpColorData,
	HBITMAP RgbBmp,
	HBITMAP AlphaBmp,
	const void *MemImage,
	int MemImageSize,
	const void *AlphaMemImage,
	int AlphaMemImageSize,
	const BASEIMAGE *BaseImage,
	const BASEIMAGE *AlphaBaseImage,
	int ReverseFlag,
	int ConvertPremultipliedAlpha,
	int UnionGrHandle,
	int ASyncThread
)
{
	SETGRAPHBASEINFO_GPARAM GParam ;

	Graphics_Image_InitSetGraphBaseInfoGParam( &GParam ) ;

	return Graphics_Image_SetBaseInfo_UseGParam( &GParam, GrHandle, FileName, BmpColorData, RgbBmp, AlphaBmp, MemImage, MemImageSize,
										AlphaMemImage, AlphaMemImageSize, BaseImage, AlphaBaseImage, ReverseFlag, ConvertPremultipliedAlpha, UnionGrHandle, ASyncThread ) ;
}

// ����̃t�@�C������摜��ǂݍ��񂾏ꍇ�̃t�@�C���p�X���Z�b�g����
extern int Graphics_Image_SetName( int GrHandle, const wchar_t *GraphName, int ReverseFlag, int ConvertPremultipliedAlpha, int UnionGrHandle, int ASyncThread )
{
	return Graphics_Image_SetGraphBaseInfo( GrHandle, GraphName, NULL, NULL, NULL, NULL, 0, NULL, 0, NULL, NULL, ReverseFlag, ConvertPremultipliedAlpha, UnionGrHandle, ASyncThread ) ;
}

// FillGraph �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_FillGraph_UseGParam(
	int GrHandle,
	int Red,
	int Green,
	int Blue,
	int Alpha,
	int ASyncThread
)
{
	IMAGEDATA *Image ;
	IMAGEDATA_ORIG *Orig ;
	RECT Rect ;

	// �G���[����
	if( ASyncThread )
	{
		if( GRAPHCHK_ASYNC( GrHandle, Image ) )
			return -1 ;
	}
	else
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
	}
	Orig = Image->Orig ;

	// �l�̕␳
	if( Red   < 0 ) Red   = 0 ; else if( Red   > 255 ) Red   = 255 ;
	if( Green < 0 ) Green = 0 ; else if( Green > 255 ) Green = 255 ;
	if( Blue  < 0 ) Blue  = 0 ; else if( Blue  > 255 ) Blue  = 255 ;
	if( Alpha < 0 ) Alpha = 0 ; else if( Alpha > 255 ) Alpha = 255 ;

	// �e�N�X�`�����ǂ����ŏ����𕪊�
	if( Image->Orig->FormatDesc.TextureFlag )
	{
		int Result ;

		// �e�N�X�`���̏ꍇ
		Result = Graphics_Hardware_FillGraph_PF( Image, Red, Green, Blue, Alpha, ASyncThread ) ;

		// �߂�l���Q�������ꍇ�͕W���������s��
		if( Result == 2 )
		{
			BASEIMAGE TempImage ;

			if( NS_CreateARGB8ColorBaseImage( Image->WidthI, Image->HeightI, &TempImage ) == 0 )
			{
				NS_FillBaseImage( &TempImage, Red, Green, Blue, Alpha ) ;
				NS_BltBmpOrGraphImageToGraph( NULL, NULL, NULL, FALSE, &TempImage, NULL, 0, 0, GrHandle ) ;
				NS_ReleaseBaseImage( &TempImage ) ;
			}
			else
			{
				return -1 ;
			}
		}
		else
		if( Result < 0 )
		{
			return -1 ;
		}
	}
	else
	{
		// �e�N�X�`���ł͂Ȃ��ꍇ

		// DrawFillBoxMemImg �œh��Ԃ�
		Rect.left   = 0 ;
		Rect.top    = 0 ;
		Rect.right  = Image->WidthI ;
		Rect.bottom = Image->HeightI ;
		DrawFillBoxMemImg( &Image->Soft.MemImg, &Rect, NS_GetColor( Red, Green, Blue ) ) ; 
	}

	// �I��
	return 0 ;
}

#ifndef DX_NON_MOVIE

// ���[�r�[�摜���X�V����
extern void Graphics_Image_UpdateGraphMovie( MOVIEGRAPH *Movie, DWORD_PTR GrHandle )
{
	IMAGEDATA *Image ;
	IMAGEDATA_ORIG *Orig ;

	if( GRAPHCHK( ( LONG_PTR )GrHandle, Image ) )
		return;
	Orig = Image->Orig ;

	// �e�N�X�`�����ǂ����ŏ����𕪊�
	if( Image->Orig->FormatDesc.TextureFlag )
	{
		// �e�N�X�`���̏ꍇ

	//	D_DDSURFACEDESC2 SrcDesc ;
		BASEIMAGE MovieImage, AlphaBaseImage ;
		RECT SrcRect ;

		// ���[�r�[�C���[�W�̍\�z
#ifndef DX_NON_OGGTHEORA
		// Ogg Theora ���g�p���Ă���ꍇ�͂����ōX�V����
		if( Movie->TheoraFlag )
		{
//			if( Movie->RightAlpha == TRUE )
//			{
//				if( TheoraDecode_SetupImage( Movie->TheoraHandle, 1, 0, 0 ) == 0 )
//				{
//					return ;
//				}
//			}
//			else
			if( TheoraDecode_GetYUVImage( Movie->TheoraHandle ) != NULL )
			{
				if( TheoraDecode_SetupImage( Movie->TheoraHandle, 0, 0, 1 ) == 0 )
				{
					if( Movie->TheoraSetupGraphHandleImage )
					{
						return ;
					}
				}
			}
			else
			if( GSYS.HardInfo.UseShader )
			{
				if( TheoraDecode_SetupImage( Movie->TheoraHandle, 0, 1, 0 ) == 0 )
				{
					if( Movie->TheoraSetupGraphHandleImage )
					{
						return ;
					}
				}
			}
			else
			{
				if( TheoraDecode_SetupImage( Movie->TheoraHandle, 1, 0, 0 ) == 0 )
				{
					if( Movie->TheoraSetupGraphHandleImage )
					{
						return ;
					}
				}
			}
		}

		// Theora ���g�p���Ă��āA�摜�̉E�����A���t�@���ł͂Ȃ��A���� YUY2 �t�H�[�}�b�g���g�p���Ă��邩�ŏ����𕪊�
		if( Movie->TheoraFlag && Movie->RightAlpha == FALSE && TheoraDecode_GetYUVImage( Movie->TheoraHandle ) != NULL )
		{
			Graphics_Hardware_UpdateGraphMovie_TheoraYUV_PF( Movie, Image ) ;
			Movie->TheoraSetupGraphHandleImage = TRUE ;
		}
		else
		// Theora ���g�p���Ă��āA�摜�̉E�����A���t�@���ł͂Ȃ��A���V�F�[�_�[���g�p�ł���ꍇ�͏����𕪊�
		if( Movie->TheoraFlag && GSYS.HardInfo.UseShader )
		{
			// �t�B���^�[���g�p����RGB�J���[�ɕϊ�����
			if( Movie->RightAlpha )
			{
				NS_GraphFilterRectBlt(
					TheoraDecode_GetYGrHandle( Movie->TheoraHandle ),
					( int )GrHandle,
					0, 0, Movie->Width / 2, Movie->Height, 0, 0,
					DX_GRAPH_FILTER_Y2UV1_TO_RGB_RRA,
					TheoraDecode_GetUVGrHandle( Movie->TheoraHandle )
				) ;
			}
			else
			{
				NS_GraphFilterBlt(
					TheoraDecode_GetYGrHandle( Movie->TheoraHandle ),
					( int )GrHandle,
					DX_GRAPH_FILTER_Y2UV1_TO_RGB, 
					TheoraDecode_GetUVGrHandle( Movie->TheoraHandle )
				) ;
			}
			Movie->TheoraSetupGraphHandleImage = TRUE ;
		}
		else
#endif
		if( Movie->YGrHandle >= 0 )
		{
			// �t�B���^�[���g�p����RGB�J���[�ɕϊ�����
			NS_GraphFilterBlt(
				Movie->YGrHandle,
				( int )GrHandle,
				DX_GRAPH_FILTER_Y2UV1_TO_RGB, 
				Movie->UVGrHandle
			) ;
		}
		else
		if( Movie->UseNowImage != NULL )
		{
			// ���[�r�[�̃C���[�W�����쐬����
			MovieImage = *Movie->UseNowImage ;
			
			// �E���������A���t�@���̏ꍇ�̓A���t�@����]�����鏀��������
			if( Movie->RightAlpha )
			{
				MovieImage.Width /= 2;
				AlphaBaseImage = MovieImage;
				AlphaBaseImage.GraphData = (BYTE *)MovieImage.GraphData + MovieImage.Width * MovieImage.ColorData.PixelByte ;
			}

			// �]������`���Z�b�g
			SETRECT( SrcRect, Image->UseOrigXI, Image->UseOrigYI, Image->UseOrigXI + Image->WidthI, Image->UseOrigYI + Image->HeightI ) ;

			// ��������Ă���e�N�X�`���[�ɓ\��t����
			Graphics_Image_BltBmpOrBaseImageToGraph3(
				&SrcRect,
				0,
				0,
				( int )GrHandle,
				&MovieImage,
				Movie->RightAlpha ? &AlphaBaseImage : NULL,
				FALSE,
				FALSE,
				FALSE ) ;
		}
	}
	else
	{
		// �e�N�X�`���ł͖����ꍇ
		MEMIMG *MemImg ;

		MemImg = &Image->Soft.MemImg ;

#ifndef DX_NON_OGGTHEORA
		// Ogg Theora ���g�p���Ă���ꍇ�͂����ōX�V
		if( Movie->TheoraFlag )
		{
			if( TheoraDecode_SetupImage( Movie->TheoraHandle, 1, 0, 0 ) == 0 )
			{
				if( Movie->TheoraSetupGraphHandleImage )
				{
					return ;
				}
				Movie->TheoraSetupGraphHandleImage = TRUE ;
			}
		}
#endif

		// �s�N�Z���t�H�[�}�b�g���Ⴄ���A�E���������A���t�@���Ƃ��Ĉ����ꍇ�̓J���[�}�b�`���O�]�����s��
		if( Movie->SurfaceMode == DX_MOVIESURFACE_FULLCOLOR ||
			MemImg->Base->ColorDataP->ColorBitDepth != Movie->UseNowImage->ColorData.ColorBitDepth ||
			Movie->RightAlpha == 1 )
		{
			// �E�������A���t�@���ł͂Ȃ��ꍇ�͊ȈՓ]��
			if( Movie->RightAlpha == 0 )
			{
				BltBaseImageToMemImg(
					Movie->UseNowImage,
					NULL,
					MemImg,
					0,
					0,
					Image->WidthI,
					Image->HeightI,
					0,
					0,
					FALSE ) ;
			}
			else
			{
				// �E�������A���t�@���̏ꍇ�͒ʏ�]��
				BASEIMAGE MovieImage, AlphaBaseImage;
				RECT SrcRect ;

				SETRECT( SrcRect, 0, 0, Image->WidthI, Image->HeightI ) ;
				MovieImage.ColorData	= Movie->UseNowImage->ColorData ;
				MovieImage.GraphData	= Movie->UseNowImage->GraphData ;
				MovieImage.Pitch		= Movie->UseNowImage->Pitch ;
				MovieImage.Width		= Movie->UseNowImage->Width / 2;
				MovieImage.Height		= Movie->UseNowImage->Height ;

				AlphaBaseImage = MovieImage;
				AlphaBaseImage.GraphData = (BYTE *)MovieImage.GraphData + MovieImage.Width * MovieImage.ColorData.PixelByte ;

				// �O���t�B�b�N�̓]��
				BltBaseImageToMemImg(
					&MovieImage,
					&AlphaBaseImage,
					MemImg,
					0,
					0,
					Image->WidthI,
					Image->HeightI,
					0,
					0,
					FALSE ) ;
			}
		}
		else
		{
			// �����]�������v���O����
			void *DestP, *SrcP ;
			int SrcAddPitch, DestAddPitch ;
			int DwMoveSetNum, NokoriMoveSetNum ;
			int MoveLineByte, ColorBitDepth ;
			int PixelByte ;
			int BltHeight ;

			// �]�����郉�C�������Z�b�g
			BltHeight = Movie->Height ;

			// �F�r�b�g�����Z�b�g
			ColorBitDepth = NS_GetColorBitDepth() ;

			// �P�h�b�g������̃o�C�g���擾
			PixelByte = ColorBitDepth / 8 ;

			// �P���C��������̓]���T�C�Y���Z�b�g
			MoveLineByte = Movie->Width * PixelByte ;

			// �]�����A��A�h���X�Z�b�g
			DestP = MemImg->UseImage ;
			SrcP = Movie->UseNowImage->GraphData ;

			// �P���C���]����A���̓]�����A�h���X�܂ł̃o�C�g���v�Z
			DestAddPitch = ( int )( MemImg->Base->Pitch - MoveLineByte ) ;
			SrcAddPitch  = Movie->UseNowImage->Pitch - MoveLineByte ;

			// �S�o�C�g�]������A���̌゠�܂�]���������s�N�Z�������邩�Z�o
			DwMoveSetNum = MoveLineByte / 4 ;
			NokoriMoveSetNum = ( MoveLineByte - DwMoveSetNum * 4 ) / PixelByte ;

			// �]������
#ifndef DX_NON_INLINE_ASM
			_asm{
				PUSHF
				CLD
				MOV		EDI, DestP
				MOV		ESI, SrcP
				MOV		EAX, DestAddPitch
				MOV		EBX, SrcAddPitch
				MOV		EDX, BltHeight
				CMP		ColorBitDepth, 8
				JZ		CB8
				CMP		ColorBitDepth, 16
				JZ		CB16


			CB32:
				MOV		ECX, DwMoveSetNum
				REP		MOVSD
				ADD		EDI, DestAddPitch
				ADD		ESI, SrcAddPitch
				DEC		EDX
				JNZ		CB32
				JMP		END_


			CB16:
				MOV		ECX, DwMoveSetNum
				TEST	ECX, 0xffffffff
				JZ		B161
				REP		MOVSD
			B161:
				MOV		ECX, NokoriMoveSetNum
				TEST	ECX, 0xffffffff
				JZ		B162
				REP		MOVSW
			B162:
				ADD		EDI, DestAddPitch
				ADD		ESI, SrcAddPitch
				DEC		EDX
				JNZ		CB16
				JMP		END_


			CB8:
				MOV		ECX, DwMoveSetNum
				TEST	ECX, 0xffffffff
				JZ		B81
				REP		MOVSD
			B81:
				MOV		ECX, NokoriMoveSetNum
				TEST	ECX, 0xffffffff
				JZ		B82
				REP		MOVSB
			B82:
				ADD		EDI, DestAddPitch
				ADD		ESI, SrcAddPitch
				DEC		EDX
				JNZ		CB8
				JMP		END_

			END_:
				POPF
			}
#else
			int i, j ;
			BYTE *DestBP, *SrcBP ;

			DestBP = ( BYTE * )DestP ;
			SrcBP = ( BYTE * )SrcP ;
			switch( ColorBitDepth )
			{
			case 8 :
				for( i = BltHeight ; i > 0 ; i -- )
				{
					for( j = DwMoveSetNum ; j > 0 ; j --, DestBP += 4, SrcBP += 4 )
					{
						*( ( DWORD * )DestBP ) = *( ( DWORD * )SrcBP ) ;
					}
					for( j = NokoriMoveSetNum ; j > 0 ; j --, DestBP ++, SrcBP ++ )
					{
						*DestBP = *SrcBP ;
					}
					DestBP += DestAddPitch ;
					SrcBP += SrcAddPitch ;
				}
				break ;

			case 16 :
				for( i = BltHeight ; i > 0 ; i -- )
				{
					for( j = DwMoveSetNum ; j > 0 ; j --, DestBP += 4, SrcBP += 4 )
					{
						*( ( DWORD * )DestBP ) = *( ( DWORD * )SrcBP ) ;
					}
					for( j = NokoriMoveSetNum ; j > 0 ; j --, DestBP += 2, SrcBP += 2 )
					{
						*( ( WORD * )DestBP ) = *( ( WORD * )SrcBP ) ;
					}
					DestBP += DestAddPitch ;
					SrcBP += SrcAddPitch ;
				}
				break ;

			case 32 :
				for( i = BltHeight ; i > 0 ; i -- )
				{
					for( j = DwMoveSetNum ; j > 0 ; j --, DestBP += 4, SrcBP += 4 )
					{
						*( ( DWORD * )DestBP ) = *( ( DWORD * )SrcBP ) ;
					}
					DestBP += DestAddPitch ;
					SrcBP += SrcAddPitch ;
				}
				break ;
			}
#endif
		}
	}
}

#endif

// BltBmpOrGraphImageToGraph �̓����֐�
extern int Graphics_Image_BltBmpOrGraphImageToGraphBase(
	const BASEIMAGE		*RgbBaseImage,
	const BASEIMAGE		*AlphaBaseImage,
	      int			CopyPointX,
	      int			CopyPointY,
	      int			GrHandle,
	      int			UseTransColorConvAlpha,
	      int			ASyncThread
)
{
	IMAGEDATA		*Image ;
	IMAGEDATA_ORIG	*Orig ;
	RECT			SrcRect ;

	if( GSYS.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( GRAPHCHK_ASYNC( GrHandle, Image ) )
			return -1 ;
	}
	else
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
	}
	Orig = Image->Orig ;

	// �]������`�̏���
	SrcRect.left   = CopyPointX ;
	SrcRect.top    = CopyPointY ;
	SrcRect.right  = CopyPointX + RgbBaseImage->Width ;
	SrcRect.bottom = CopyPointY + RgbBaseImage->Height ;

	// �g�p����̈��ۑ�
	Image->UseBaseXI =          SrcRect.left ;
	Image->UseBaseXF = ( float )SrcRect.left ;
	Image->UseBaseYI =          SrcRect.top ; 
	Image->UseBaseYF = ( float )SrcRect.top ; 

	// �摜�̓]��
	return Graphics_Image_BltBmpOrBaseImageToGraph3(
				&SrcRect,
				0,
				0,
				GrHandle,
				RgbBaseImage,
				AlphaBaseImage,
				Orig->FormatDesc.BlendGraphFlag,
				UseTransColorConvAlpha,
				FALSE,
				ASyncThread
			) ;
}

// BltBmpOrGraphImageToGraph2 �̓����֐�
extern int Graphics_Image_BltBmpOrGraphImageToGraph2Base(
	const BASEIMAGE		*RgbBaseImage,
	const BASEIMAGE		*AlphaBaseImage,
	const RECT			*SrcRect,
	      int			DestX,
	      int			DestY,
	      int			GrHandle,
	      int			UseTransColorConvAlpha,
	      int			ASyncThread
)
{
	IMAGEDATA		*Image ;
	IMAGEDATA_ORIG *Orig ;

	if( GSYS.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( GRAPHCHK_ASYNC( GrHandle, Image ) )
			return -1 ;
	}
	else
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
	}
	Orig = Image->Orig ;

	// �g�p����̈��ۑ�
	Image->UseBaseXI =            SrcRect->left - DestX   ;
	Image->UseBaseXF = ( float )( SrcRect->left - DestX ) ;
	Image->UseBaseYI =            SrcRect->top  - DestY   ; 
	Image->UseBaseYF = ( float )( SrcRect->top  - DestY ) ; 

	// �摜�̓]��
	return Graphics_Image_BltBmpOrBaseImageToGraph3(
				SrcRect,
				DestX,
				DestY,
				GrHandle,
				RgbBaseImage,
				AlphaBaseImage, 
				Orig->FormatDesc.BlendGraphFlag,
				UseTransColorConvAlpha,
				FALSE,
				ASyncThread
			) ;
}

// BltBmpOrGraphImageToDivGraph �̓����֐�
extern int Graphics_Image_BltBmpOrGraphImageToDivGraphBase(
	const BASEIMAGE	*RgbBaseImage,
	const BASEIMAGE	*AlphaBaseImage,
	      int		AllNum,
	      int		XNum,
	      int		/*YNum*/,
		  int		IsFloat,
	      int		WidthI,
	      float		WidthF,
	      int		HeightI,
	      float		HeightF,
	const int		*GrHandle,
	      int		ReverseFlag,
	      int		UseTransColorConvAlpha,
	      int		ASyncThread
)
{
	IMAGEDATA		*Image ;
	IMAGEDATA		*DivImage ;
	IMAGEDATA_ORIG	*Orig ;
	RECT			Rect ;
	int				i ;
	int				Count ;
	int				x ;
	int				y ;

	if( GSYS.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( GRAPHCHK_ASYNC( GrHandle[ 0 ], Image ) )
			return -1 ;
	}
	else
	{
		if( GRAPHCHK( GrHandle[ 0 ], Image ) )
			return -1 ;
	}
	Orig = Image->Orig ;

	// �ŏ��̉摜�Ƀt�@�C�������Z�b�g
	Image->UseBaseXI = 0    ;
	Image->UseBaseXF = 0.0f ;
	Image->UseBaseYI = 0    ;
	Image->UseBaseYF = 0.0f ;

	// �t�@�C���ɏ����Z�b�g���Ȃ��炷�ׂẲ摜������
	// �I���W�i���摜���g�p���Ă��邩���ׂ�
	Count = 1 ;
	x     = 0 ;
	y     = 0 ;
	for( i = 1 ; i < AllNum ; i ++ )
	{
		// �摜���̃A�h���X���擾
		if( ASyncThread )
		{
			if( GRAPHCHK_ASYNC( GrHandle[ i ], DivImage ) )
				return -1 ;
		}
		else
		{
			if( GRAPHCHK( GrHandle[ i ], DivImage ) )
				return -1 ;
		}

		// �����I���W�i���摜���g�p���Ă��邩�ǂ������ׂ�
		if( DivImage->Orig == Orig ) Count ++ ;

		// �t�@�C�������Z�b�g
		if( ReverseFlag )
		{
			DivImage->UseBaseXI = ( ( XNum - 1 ) - x ) * WidthI ;
			DivImage->UseBaseXF = ( ( XNum - 1 ) - x ) * WidthF ;
		}
		else
		{
			DivImage->UseBaseXI = x * WidthI ;
			DivImage->UseBaseXF = x * WidthF ;
		}
		DivImage->UseBaseYI = y * HeightI ;
		DivImage->UseBaseYF = y * HeightF ;
		x ++ ;
		if( x == XNum )
		{
			x = 0 ;
			y ++ ;
		}
	}

	// ���ׂẴO���t�B�b�N�n���h���������I���W�i���摜��
	// �g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( AllNum == Count )
	{
		// �����I���W�i���摜���g�p���Ă���ꍇ

		// �I���W�i���摜���ŏ��̃O���t�B�b�N�n���h���ɓ]��
		Rect.left   = 0 ;
		Rect.right  = Image->Orig->Width ;
		Rect.top    = 0 ;
		Rect.bottom = Image->Orig->Height ;
		Graphics_Image_BltBmpOrBaseImageToGraph3(
			&Rect,
			0, 0,
			GrHandle[ 0 ],
			RgbBaseImage, AlphaBaseImage,
			Orig->FormatDesc.BlendGraphFlag,
			UseTransColorConvAlpha,
			TRUE,
			ASyncThread
		) ;
	}
	else
	{
		// �����I���W�i���摜���g�p���Ă��Ȃ��ꍇ

		// ���ׂẴO���t�B�b�N�n���h���Ɍʂɓ]������
		for( i = 0 ; i < AllNum ; i ++ )
		{
			// �摜���̃A�h���X���擾
			if( ASyncThread )
			{
				if( GRAPHCHK_ASYNC( GrHandle[ i ], DivImage ) )
					return -1 ;
			}
			else
			{
				if( GRAPHCHK( GrHandle[ i ], DivImage ) )
					return -1 ;
			}

			// �]������`�̃Z�b�g
			Rect.left   = DivImage->UseBaseXI ;
			Rect.top    = DivImage->UseBaseYI ;
			Rect.right  = Rect.left + DivImage->WidthI ;
			Rect.bottom = Rect.top  + DivImage->HeightI ;

			// �摜��]��
			Graphics_Image_BltBmpOrBaseImageToGraph3(
				&Rect,
				0, 0,
				GrHandle[ i ],
				RgbBaseImage, AlphaBaseImage,
				Orig->FormatDesc.BlendGraphFlag,
				UseTransColorConvAlpha,
				FALSE,
				ASyncThread
			) ;
		}
	}

	// �I��
	return 0 ;
}

// �O���t�B�b�N�n���h���ɉ摜�f�[�^��]�����邽�߂̊֐�
extern int Graphics_Image_BltBmpOrBaseImageToGraph3(
	const RECT		*SrcRect,
	      int		DestX,
	      int		DestY,
	      int		GrHandle,
	const BASEIMAGE	*RgbBaseImage,
	const BASEIMAGE	*AlphaBaseImage,
	      int		RedIsAlphaFlag,
	      int		UseTransColorConvAlpha,
	      int		TargetOrig,
	      int		ASyncThread
)
{
	IMAGEDATA		*Image ;
	IMAGEDATA_ORIG	*Orig ;
	int				SrcWidth ;
	int				SrcHeight ;
	BASEIMAGE		TempBaseRGB ;
	BASEIMAGE		TempBaseAlpha ;
	int				RequiredReleaseBaseRGB = FALSE ;
	int				RequiredReleaseBaseAlpha = FALSE ;

	if( GSYS.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( GRAPHCHK_ASYNC( GrHandle, Image ) )
			return -1 ;
	}
	else
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return -1 ;
	}
	Orig = Image->Orig ;

	// �]�������v�Z���Ă���
	SrcWidth  = SrcRect->right  - SrcRect->left ;
	SrcHeight = SrcRect->bottom - SrcRect->top ;
	if( SrcWidth <= 0 || SrcHeight <= 0 )
	{
		return 0 ;
	}

	// �]�����̏����Z�b�g
	_MEMSET( &TempBaseAlpha, 0, sizeof( TempBaseAlpha ) ) ;
	RequiredReleaseBaseRGB   = FALSE ;
	RequiredReleaseBaseAlpha = FALSE ;

	TempBaseRGB.GraphData      = RgbBaseImage->GraphData ;
	TempBaseRGB.Width          = RgbBaseImage->Width ;
	TempBaseRGB.Height         = RgbBaseImage->Height ;
	TempBaseRGB.Pitch          = RgbBaseImage->Pitch ;
	TempBaseRGB.ColorData      = RgbBaseImage->ColorData ;
	TempBaseRGB.MipMapCount    = RgbBaseImage->MipMapCount ;
	TempBaseRGB.GraphDataCount = RgbBaseImage->GraphDataCount ;

	if( AlphaBaseImage )
	{
		TempBaseAlpha.GraphData      = AlphaBaseImage->GraphData ;
		TempBaseAlpha.Width          = AlphaBaseImage->Width ;
		TempBaseAlpha.Height         = AlphaBaseImage->Height ;
		TempBaseAlpha.Pitch          = AlphaBaseImage->Pitch ;
		TempBaseAlpha.ColorData      = AlphaBaseImage->ColorData ;
		TempBaseAlpha.MipMapCount    = AlphaBaseImage->MipMapCount ;
		TempBaseAlpha.GraphDataCount = AlphaBaseImage->GraphDataCount ;
	}

	// �]�������L���[�u�}�b�v�œ]���悪�L���[�u�}�b�v�ł͂Ȃ��ꍇ�̓G���[
	if( ( TempBaseRGB.GraphDataCount          != 0 ) !=
		( Orig->FormatDesc.CubeMapTextureFlag != 0 ) )
	{
		return -1 ;
	}

	// BASEIMAGE �� DXT �ŁA�]���悪 DXT �ł͖����ꍇ�͂����ŕW���t�H�[�}�b�g�ɕϊ�����
	{
		int RgbConvFlag   = FALSE ;
		int AlphaConvFlag = FALSE ;
		int Hr1 = 0 ;
		int Hr2 = 0 ;

		// �e�N�X�`���ł͂Ȃ��ꍇ�͂c�w�s�t�H�[�}�b�g�̏ꍇ�͕W���t�H�[�}�b�g�ɕϊ�����
		if( Orig->FormatDesc.TextureFlag == FALSE )
		{
			if( TempBaseRGB.ColorData.Format == DX_BASEIMAGE_FORMAT_DXT1 ||
				TempBaseRGB.ColorData.Format == DX_BASEIMAGE_FORMAT_DXT2 ||
				TempBaseRGB.ColorData.Format == DX_BASEIMAGE_FORMAT_DXT3 ||
				TempBaseRGB.ColorData.Format == DX_BASEIMAGE_FORMAT_DXT4 ||
				TempBaseRGB.ColorData.Format == DX_BASEIMAGE_FORMAT_DXT5 )
			{
				RgbConvFlag = TRUE ;
			}

			if( TempBaseAlpha.GraphData != NULL )
			{
				if( TempBaseAlpha.ColorData.Format == DX_BASEIMAGE_FORMAT_DXT1 ||
					TempBaseAlpha.ColorData.Format == DX_BASEIMAGE_FORMAT_DXT2 ||
					TempBaseAlpha.ColorData.Format == DX_BASEIMAGE_FORMAT_DXT3 ||
					TempBaseAlpha.ColorData.Format == DX_BASEIMAGE_FORMAT_DXT4 ||
					TempBaseAlpha.ColorData.Format == DX_BASEIMAGE_FORMAT_DXT5 )
				{
					AlphaConvFlag = TRUE ;
				}
			}
		}
		else
		{
			Graphics_CheckRequiredNormalImageConv_BaseImageFormat_PF(
				Orig,
				TempBaseRGB.ColorData.Format,
				&RgbConvFlag,
				TempBaseAlpha.GraphData != NULL ? TempBaseAlpha.ColorData.Format : -1,
				&AlphaConvFlag
			) ;
		}

		if( RgbConvFlag == TRUE )
		{
			Hr1 = ConvertNormalFormatBaseImage( &TempBaseRGB, FALSE ) ;
			if( Hr1 == 0 )
			{
				RequiredReleaseBaseRGB = TRUE ;
			}
		}

		if( AlphaConvFlag == TRUE )
		{
			Hr2 = ConvertNormalFormatBaseImage( &TempBaseAlpha, FALSE ) ;
			if( Hr2 == 0 )
			{
				RequiredReleaseBaseAlpha = TRUE ;
			}
		}

		if( Hr1 < 0 || Hr2 < 0 )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x19\x6a\x96\x6e\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�W���C���[�W�p�̃������m�ۂɎ��s���܂���\n" @*/ ) ;
			goto ERR ;
		}
	}

	// �e�N�X�`�����ǂ����ŏ����𕪊�
	if( Orig->FormatDesc.TextureFlag )
	{
		// �e�N�X�`���̏ꍇ�@��ˑ��֐����Ă�
		if( Graphics_Hardware_BltBmpOrBaseImageToGraph3_PF(
			SrcRect,
			DestX,
			DestY,
			GrHandle,
			&TempBaseRGB,
			&TempBaseAlpha,
			RedIsAlphaFlag,
			UseTransColorConvAlpha,
			TargetOrig,
			ASyncThread ) < 0 )
		{
			goto ERR ;
		}
	}
	else
	{
		// �e�N�X�`���ł͂Ȃ��ꍇ

		// MEMIMG �̓]�������֐����g�����߂̏������s��

		// �I���W�i���摜���Ώۂ��ǂ����ŏ����𕪊�
		if( TargetOrig == TRUE )
		{
			// �I���W�i���摜���Ώۂ̏ꍇ

			// �T�C�Y�̕␳
			if( DestX + SrcWidth  > ( int )Orig->Soft.MemImg.Width  )
			{
				SrcWidth  = ( int )( Orig->Soft.MemImg.Width  - DestX ) ;
			}

			if( DestY + SrcHeight > ( int )Orig->Soft.MemImg.Height )
			{
				SrcHeight = ( int )( Orig->Soft.MemImg.Height - DestY ) ;
			}

			if( SrcWidth <= 0 || SrcHeight <= 0 )
			{
				goto ERR ;
			}

			// �摜�̓]��
			BltBaseImageToMemImg(
				&TempBaseRGB,
				TempBaseAlpha.GraphData == NULL ? NULL : &TempBaseAlpha,
				&Orig->Soft.MemImg,
				SrcRect->left,
				SrcRect->top,
				SrcWidth,
				SrcHeight,
				DestX,
				DestY,
				UseTransColorConvAlpha
			) ;
		}
		else
		{
			// �I���W�i���摜���Ώۂł͂Ȃ��ꍇ

			// �T�C�Y�̕␳
			if( DestX + SrcWidth  > Image->WidthI  )
			{
				SrcWidth  = Image->WidthI  - DestX ;
			}

			if( DestY + SrcHeight > Image->HeightI )
			{
				SrcHeight = Image->HeightI - DestY ;
			}

			if( SrcWidth <= 0 || SrcHeight <= 0 ) 
			{
				goto ERR ;
			}

			// �摜�̓]��
			BltBaseImageToMemImg(
				&TempBaseRGB,
				TempBaseAlpha.GraphData == NULL ? NULL : &TempBaseAlpha,
				&Orig->Soft.MemImg,
				SrcRect->left,
				SrcRect->top,
				SrcWidth,
				SrcHeight,
				Image->UseOrigXI + DestX,
				Image->UseOrigYI + DestY,
				UseTransColorConvAlpha
			) ;
		}

		// �p���b�g�摜�̏ꍇ�͓��ߐF�R�[�h�𒲐�
		if( Orig->FormatDesc.UsePaletteFlag == TRUE && Orig->Soft.MemImg.Base->Palette )
		{
			int   i ;
			DWORD TransCode ;

			// ���ߐF�̎擾
			TransCode = NS_GetColor3( 
					GetMemImgColorData( Orig->FormatDesc.ColorBitDepth == 16 ? 0 : 1, Orig->FormatDesc.AlphaChFlag, FALSE ),
					( int )( ( GSYS.CreateImage.TransColor >> 16 ) & 0xff ),
					( int )( ( GSYS.CreateImage.TransColor >> 8  ) & 0xff ),
					( int )( ( GSYS.CreateImage.TransColor >> 0  ) & 0xff ),
					0xff ) ;

			// �w��̃J���[���p���b�g�̒��ɂ������炻��𓧉ߐF�ɂ���
			if( Orig->Soft.MemImg.Base->ColorType == 1 )
			{
				TransCode &= 0xffffff;
				for( i = 0 ; i < 256 ; i ++ )
				{
					if( ( Orig->Soft.MemImg.Base->Palette[i] & 0xffffff ) == TransCode )
					{
						break ;
					}
				}
			}
			else
			{
				TransCode &= 0xffff;
				for( i = 0 ; i < 256 ; i ++ )
				{
					if( ((WORD *)Orig->Soft.MemImg.Base->Palette)[i] == TransCode )
					{
						break ;
					}
				}
			}

			if( i == 256 )
			{
				i = 0 ;
			}

			Orig->Soft.MemImg.Base->TransColor = ( unsigned int )i ;
		}
	}

	if( RequiredReleaseBaseRGB )
	{
		ReleaseBaseImage( &TempBaseRGB ) ;
	}

	if( RequiredReleaseBaseAlpha )
	{
		ReleaseBaseImage( &TempBaseAlpha ) ;
	}

	// �I��
	return 0 ;

	// �G���[�I��
ERR :

	if( RequiredReleaseBaseRGB )
	{
		ReleaseBaseImage( &TempBaseRGB ) ;
	}

	if( RequiredReleaseBaseAlpha )
	{
		ReleaseBaseImage( &TempBaseAlpha ) ;
	}

	return -1 ;
}

// �w��̃I���W�i���摜���ɓ]�������`�����쐬����
// �߂�l  -1:�͈͊O   0:����I��
extern	int		Graphics_Image_BltBmpOrBaseImageToGraph3_Make_OrigTex_MoveRect(
	const IMAGEDATA_ORIG_HARD_TEX	*OrigTex,
	const RECT						*SrcRect,
		  int						SrcWidth,
		  int						SrcHeight,
	      int						DestX,
	      int						DestY,
	      RECT						*DestRect,
	      RECT						*MoveRect
)
{
	// �͈͊O�������牽�����Ȃ�
	if( OrigTex->OrigPosX >= DestX + SrcWidth  ||
		OrigTex->OrigPosY >= DestY + SrcHeight ||
		DestX >= OrigTex->OrigPosX + OrigTex->UseWidth ||
		DestY >= OrigTex->OrigPosY + OrigTex->UseHeight )
	{
		return -1 ;
	}

	// ���b�N����]���͈͂Ɠ]������͈͂̊m��
	if( OrigTex->OrigPosX > DestX )
	{
		DestRect->left = 0 ;
		MoveRect->left = OrigTex->OrigPosX - DestX ;
	}
	else
	{
		DestRect->left = DestX - OrigTex->OrigPosX ;
		MoveRect->left = 0 ;
	}

	if( OrigTex->OrigPosX + OrigTex->UseWidth < DestX + SrcWidth )
	{
		DestRect->right = OrigTex->UseWidth ;
		MoveRect->right = ( OrigTex->OrigPosX + OrigTex->UseWidth ) - DestX ;
	}
	else
	{
		DestRect->right = ( DestX + SrcWidth ) - OrigTex->OrigPosX ;
		MoveRect->right = SrcWidth ;
	}

	if( OrigTex->OrigPosY > DestY )
	{
		DestRect->top = 0 ;
		MoveRect->top = OrigTex->OrigPosY - DestY ;
	}
	else
	{
		DestRect->top = DestY - OrigTex->OrigPosY ;
		MoveRect->top = 0 ;
	}

	if( OrigTex->OrigPosY + OrigTex->UseHeight < DestY + SrcHeight )
	{
		DestRect->bottom = OrigTex->UseHeight ;
		MoveRect->bottom = ( OrigTex->OrigPosY + OrigTex->UseHeight ) - DestY ;
	}
	else
	{
		DestRect->bottom = ( DestY + SrcHeight ) - OrigTex->OrigPosY ;
		MoveRect->bottom = SrcHeight ;
	}

	// ���摜�̓]���̈�l�����Z
	MoveRect->left   += SrcRect->left ;
	MoveRect->top    += SrcRect->top ;
	MoveRect->right  += SrcRect->left ;
	MoveRect->bottom += SrcRect->top ;

	// ����I��
	return 0 ;
}

// �w��̕`��p�摜���ɓ]�������`�����쐬����
// �߂�l  -1:�͈͊O   0:����I��
extern	int		Graphics_Image_BltBmpOrBaseImageToGraph3_Make_DrawTex_MoveRect(
	const IMAGEDATA_HARD_DRAW		*DrawTex,
	const RECT						*SrcRect,
		  int						SrcWidth,
		  int						SrcHeight,
	      int						DestX,
	      int						DestY,
	      RECT						*DestRect,
	      RECT						*MoveRect
)
{
	// �͈͊O�������牽�����Ȃ�
	if( DrawTex->DrawPosXI >= DestX + SrcWidth ||
		DrawTex->DrawPosYI >= DestY + SrcHeight ||
		DestX >= DrawTex->DrawPosXI + DrawTex->WidthI ||
		DestY >= DrawTex->DrawPosYI + DrawTex->HeightI )
	{
		return -1 ;
	}

	// ���b�N����]���͈͂Ɠ]������͈͂̊m��
	if( DrawTex->DrawPosXI > DestX )
	{
		DestRect->left = 0 ;
		MoveRect->left = DrawTex->DrawPosXI - DestX ;
	}
	else
	{
		DestRect->left = DestX - DrawTex->DrawPosXI ;
		MoveRect->left = 0 ;
	}

	if( DrawTex->DrawPosXI + DrawTex->WidthI < DestX + SrcWidth )
	{
		DestRect->right = DrawTex->WidthI ;
		MoveRect->right = ( DrawTex->DrawPosXI + DrawTex->WidthI ) - DestX ;
	}
	else
	{
		DestRect->right = ( DestX + SrcWidth ) - DrawTex->DrawPosXI ;
		MoveRect->right = SrcWidth ;
	}

	if( DrawTex->DrawPosYI > DestY )
	{
		DestRect->top = 0 ;
		MoveRect->top = DrawTex->DrawPosYI - DestY ;
	}
	else
	{
		DestRect->top = DestY - DrawTex->DrawPosYI ;
		MoveRect->top = 0 ;
	}

	if( DrawTex->DrawPosYI + DrawTex->HeightI < DestY + SrcHeight )
	{
		DestRect->bottom = DrawTex->HeightI ;
		MoveRect->bottom = ( DrawTex->DrawPosYI + DrawTex->HeightI ) - DestY ;
	}
	else
	{
		DestRect->bottom = ( DestY + SrcHeight ) - DrawTex->DrawPosYI ;
		MoveRect->bottom = SrcHeight ;
	}

	// �e�N�X�`���̎w��̈�l�����Z
	DestRect->left   += DrawTex->UsePosXI ;
	DestRect->top    += DrawTex->UsePosYI ;
	DestRect->right  += DrawTex->UsePosXI ;
	DestRect->bottom += DrawTex->UsePosYI ;

	// ���摜�̓]���̈�l�����Z
	MoveRect->left   += SrcRect->left ;
	MoveRect->top    += SrcRect->top ;
	MoveRect->right  += SrcRect->left ;
	MoveRect->bottom += SrcRect->top ;

	// ����I��
	return 0 ;
}

// �`��\�摜��o�b�N�o�b�t�@����w��̈�̃O���t�B�b�N���擾����
extern	int Graphics_Image_GetDrawScreenGraphBase( int TargetScreen, int TargetScreenSurface, int x1, int y1, int x2, int y2, int destX, int destY, int GrHandle )
{
	IMAGEDATA *Image, *TargImage ;
//	IMAGEDATA_HARD_DRAW *DrawTex ;
	int Width, Height ;
//	int i ;
	int TargetScreenWidth ;
	int TargetScreenHeight ;
	RECT Rect ;
	RECT DestBaseRect ;
//	HRESULT hr ;

	// �G���[����
	if( GRAPHCHK( GrHandle, Image ) )
		return -1 ;

	// �`���̉摜�f�[�^�A�h���X���擾����
	if( GRAPHCHKFULL( TargetScreen, TargImage ) )
	{
		TargImage = NULL ;
		TargetScreenWidth  = GSYS.DrawSetting.DrawSizeX ;
		TargetScreenHeight = GSYS.DrawSetting.DrawSizeY ;
	}
	else
	{
		TargetScreenWidth  = TargImage->WidthI ;
		TargetScreenHeight = TargImage->HeightI ;
	}

	// ��`�̃f�[�^���s���������ꍇ�G���[
	if( x1 < 0 || x1 >= x2 ||
		y1 < 0 || y1 >= y2 ||
		x2 > TargetScreenWidth ||
		y2 > TargetScreenHeight ) return -1 ;

	// ��荞�ݐ悪�摜���͂ݏo�Ă�����G���[
	Width  = x2 - x1 ;
	Height = y2 - y1 ;
	if( Image->WidthI < Width + destX || Image->HeightI < Height + destY ) return -1 ;
	DestBaseRect.left   = destX ;
	DestBaseRect.top    = destY ;
	DestBaseRect.right  = destX + Width ;
	DestBaseRect.bottom = destY + Height ;

	// �e�N�X�`�����ǂ����ŏ����𕪊�
	if( Image->Orig->FormatDesc.TextureFlag )
	{
		// �e�N�X�`���̏ꍇ
		if( Graphics_Hardware_GetDrawScreenGraphBase_PF( Image, TargImage, TargetScreen, TargetScreenSurface, TargetScreenWidth, TargetScreenHeight, x1, y1, x2, y2, destX, destY ) < 0 )
		{
			return -1 ;
		}
	}
	else
	{
		// �\�t�g�E�G�A�����_�����O�̏ꍇ

		// �`��Ώۂ���]��
		Rect.left   = x1 ;
		Rect.top    = y1 ;
		Rect.right  = x2 ;
		Rect.bottom = y2 ;
		BltMemImg( &Image->Soft.MemImg, TargImage ? &TargImage->Soft.MemImg : &GSYS.SoftRender.MainBufferMemImg, &Rect, destX, destY ) ;
	}

	// �I��
	return 0 ;
}

// �O���t�B�b�N�̃f�[�^���C���f�b�N�X�l������o��
extern IMAGEDATA *Graphics_Image_GetData( int GrHandle, int ASyncThread )
{
	IMAGEDATA *Image ;

	// �G���[����
	if( ASyncThread )
	{
		if( GRAPHCHK_ASYNC( GrHandle, Image ) )
			return NULL ;
	}
	else
	{
		if( GRAPHCHK( GrHandle, Image ) )
			return NULL ;
	}

	return Image ;
}

// �^�����̃e�N�X�`���̃n���h�����擾����
extern int Graphics_Image_GetWhiteTexHandle( void )
{
	if( GSYS.Resource.WhiteTexHandle <= 0 )
	{
		LOADGRAPH_GPARAM GParam ;

		Graphics_Image_InitLoadGraphGParam( &GParam ) ;
		Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 16, TRUE, FALSE ) ;
		GParam.CreateGraphGParam.InitHandleGParam.TransColor = ( BYTE )255 << 16 | ( BYTE )0 << 8 | ( BYTE )255 ;
		GSYS.Resource.WhiteTexHandle = Graphics_Image_CreateGraphFromMem_UseGParam( &GParam, FALSE, GSYS.Resource.WhiteTexHandle, WhiteTga8x8TextureFileImage, 84 ) ;
		NS_SetDeleteHandleFlag( GSYS.Resource.WhiteTexHandle, &GSYS.Resource.WhiteTexHandle ) ;
		NS_SetDeviceLostDeleteGraphFlag( GSYS.Resource.WhiteTexHandle, TRUE ) ;
	}

	return GSYS.Resource.WhiteTexHandle ;
}

// �A���`�G�C���A�X�t���̐���`�悷�邽�߂̃e�N�X�`���n���h�����擾����
extern int Graphics_Image_GetLineTexHandle( int IsPMA )
{
	if( IsPMA )
	{
		if( GSYS.Resource.LineTexHandle_PMA <= 0 )
		{
			LOADGRAPH_GPARAM GParam ;
			BASEIMAGE BaseImage ;
			BYTE *ImageBase ;
			BYTE *Image ;
			DWORD Pitch ;
			int i, j, k ;

			NS_CreateARGB8ColorBaseImage( 8, 8, &BaseImage ) ;
			ImageBase = ( BYTE * )BaseImage.GraphData ;
			Pitch = ( DWORD )BaseImage.Pitch ;
			k = 0 ;
			for( i = 0 ; i < 8; i ++, ImageBase += Pitch )
			{
				Image = ImageBase ;
				for( j = 0 ; j < 8 ; j ++, Image += 4, k++ )
				{
					Image[ 0 ] = LineTexTga8x8TextureImage[ k ] ;
					Image[ 1 ] = LineTexTga8x8TextureImage[ k ] ;
					Image[ 2 ] = LineTexTga8x8TextureImage[ k ] ;
					Image[ 3 ] = LineTexTga8x8TextureImage[ k ] ;
				}
			}

			Graphics_Image_InitLoadGraphGParam( &GParam ) ;
			Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 16, TRUE, FALSE ) ;
			GParam.CreateGraphGParam.InitHandleGParam.TransColor = ( BYTE )255 << 16 | ( BYTE )0 << 8 | ( BYTE )255 ;
			GSYS.Resource.LineTexHandle_PMA = Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, FALSE, GSYS.Resource.LineTexHandle_PMA, &BaseImage, NULL ) ;
			NS_SetDeleteHandleFlag( GSYS.Resource.LineTexHandle_PMA, &GSYS.Resource.LineTexHandle_PMA ) ;
			NS_SetDeviceLostDeleteGraphFlag( GSYS.Resource.LineTexHandle_PMA, TRUE ) ;

			NS_ReleaseBaseImage( &BaseImage ) ;
		}

		return GSYS.Resource.LineTexHandle_PMA ;
	}
	else
	{
		if( GSYS.Resource.LineTexHandle <= 0 )
		{
			LOADGRAPH_GPARAM GParam ;
			BASEIMAGE BaseImage ;
			BYTE *ImageBase ;
			BYTE *Image ;
			DWORD Pitch ;
			int i, j, k ;

			NS_CreateARGB8ColorBaseImage( 8, 8, &BaseImage ) ;
			ImageBase = ( BYTE * )BaseImage.GraphData ;
			Pitch = ( DWORD )BaseImage.Pitch ;
			k = 0 ;
			for( i = 0 ; i < 8; i ++, ImageBase += Pitch )
			{
				Image = ImageBase ;
				for( j = 0 ; j < 8 ; j ++, Image += 4, k++ )
				{
					Image[ 0 ] = 0xff ;
					Image[ 1 ] = 0xff ;
					Image[ 2 ] = 0xff ;
					Image[ 3 ] = LineTexTga8x8TextureImage[ k ] ;
				}
			}

			Graphics_Image_InitLoadGraphGParam( &GParam ) ;
			Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 16, TRUE, FALSE ) ;
			GParam.CreateGraphGParam.InitHandleGParam.TransColor = ( BYTE )255 << 16 | ( BYTE )0 << 8 | ( BYTE )255 ;
			GSYS.Resource.LineTexHandle = Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, FALSE, GSYS.Resource.LineTexHandle, &BaseImage, NULL ) ;
			NS_SetDeleteHandleFlag( GSYS.Resource.LineTexHandle, &GSYS.Resource.LineTexHandle ) ;
			NS_SetDeviceLostDeleteGraphFlag( GSYS.Resource.LineTexHandle, TRUE ) ;

			NS_ReleaseBaseImage( &BaseImage ) ;
		}

		return GSYS.Resource.LineTexHandle ;
	}
}

// �A���`�G�C���A�X�t���̐���`�悷�邽�߂̃e�N�X�`���n���h�����擾����( ����1�s�N�Z���p )
extern int Graphics_Image_GetLineOneThicknessTexHandle( int IsPMA )
{
	if( IsPMA )
	{
		if( GSYS.Resource.LineOneThicknessTexHandle_PMA <= 0 )
		{
			LOADGRAPH_GPARAM GParam ;
			BASEIMAGE BaseImage ;
			BYTE *ImageBase ;
			BYTE *Image ;
			DWORD Pitch ;
			int i, j, k ;

			NS_CreateARGB8ColorBaseImage( 8, 8, &BaseImage ) ;
			ImageBase = ( BYTE * )BaseImage.GraphData ;
			Pitch = ( DWORD )BaseImage.Pitch ;
			k = 0 ;
			for( i = 0 ; i < 8; i ++, ImageBase += Pitch )
			{
				Image = ImageBase ;
				for( j = 0 ; j < 8 ; j ++, Image += 4, k++ )
				{
					Image[ 0 ] = LineOneThicknessTexTga8x8TextureImage[ k ] ;
					Image[ 1 ] = LineOneThicknessTexTga8x8TextureImage[ k ] ;
					Image[ 2 ] = LineOneThicknessTexTga8x8TextureImage[ k ] ;
					Image[ 3 ] = LineOneThicknessTexTga8x8TextureImage[ k ] ;
				}
			}

			Graphics_Image_InitLoadGraphGParam( &GParam ) ;
			Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 16, TRUE, FALSE ) ;
			GParam.CreateGraphGParam.InitHandleGParam.TransColor = ( BYTE )255 << 16 | ( BYTE )0 << 8 | ( BYTE )255 ;
			GSYS.Resource.LineOneThicknessTexHandle_PMA = Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, FALSE, GSYS.Resource.LineOneThicknessTexHandle_PMA, &BaseImage, NULL ) ;
			NS_SetDeleteHandleFlag( GSYS.Resource.LineOneThicknessTexHandle_PMA, &GSYS.Resource.LineOneThicknessTexHandle_PMA ) ;
			NS_SetDeviceLostDeleteGraphFlag( GSYS.Resource.LineOneThicknessTexHandle_PMA, TRUE ) ;

			NS_ReleaseBaseImage( &BaseImage ) ;
		}

		return GSYS.Resource.LineOneThicknessTexHandle_PMA ;
	}
	else
	{
		if( GSYS.Resource.LineOneThicknessTexHandle <= 0 )
		{
			LOADGRAPH_GPARAM GParam ;
			BASEIMAGE BaseImage ;
			BYTE *ImageBase ;
			BYTE *Image ;
			DWORD Pitch ;
			int i, j, k ;

			NS_CreateARGB8ColorBaseImage( 8, 8, &BaseImage ) ;
			ImageBase = ( BYTE * )BaseImage.GraphData ;
			Pitch = ( DWORD )BaseImage.Pitch ;
			k = 0 ;
			for( i = 0 ; i < 8; i ++, ImageBase += Pitch )
			{
				Image = ImageBase ;
				for( j = 0 ; j < 8 ; j ++, Image += 4, k++ )
				{
					Image[ 0 ] = 0xff ;
					Image[ 1 ] = 0xff ;
					Image[ 2 ] = 0xff ;
					Image[ 3 ] = LineOneThicknessTexTga8x8TextureImage[ k ] ;
				}
			}

			Graphics_Image_InitLoadGraphGParam( &GParam ) ;
			Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 16, TRUE, FALSE ) ;
			GParam.CreateGraphGParam.InitHandleGParam.TransColor = ( BYTE )255 << 16 | ( BYTE )0 << 8 | ( BYTE )255 ;
			GSYS.Resource.LineOneThicknessTexHandle = Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, FALSE, GSYS.Resource.LineOneThicknessTexHandle, &BaseImage, NULL ) ;
			NS_SetDeleteHandleFlag( GSYS.Resource.LineOneThicknessTexHandle, &GSYS.Resource.LineOneThicknessTexHandle ) ;
			NS_SetDeviceLostDeleteGraphFlag( GSYS.Resource.LineOneThicknessTexHandle, TRUE ) ;

			NS_ReleaseBaseImage( &BaseImage ) ;
		}

		return GSYS.Resource.LineOneThicknessTexHandle ;
	}
}

// �A���`�G�C���A�X�t���̐���`��`�悷�邽�߂̃e�N�X�`���n���h�����擾����
extern int Graphics_Image_GetLineBoxTexHandle( int IsPMA )
{
	if( IsPMA )
	{
		if( GSYS.Resource.LineBoxTexHandle_PMA <= 0 )
		{
			LOADGRAPH_GPARAM GParam ;
			BASEIMAGE BaseImage ;
			BYTE *ImageBase ;
			BYTE *Image ;
			DWORD Pitch ;
			int i, j, k ;

			NS_CreateARGB8ColorBaseImage( 16, 16, &BaseImage ) ;
			ImageBase = ( BYTE * )BaseImage.GraphData ;
			Pitch = ( DWORD )BaseImage.Pitch ;
			k = 0 ;
			for( i = 0 ; i < 16; i ++, ImageBase += Pitch )
			{
				Image = ImageBase ;
				for( j = 0 ; j < 16 ; j ++, Image += 4, k++ )
				{
					Image[ 0 ] = LineBoxTexTga16x16TextureImage[ k ] ;
					Image[ 1 ] = LineBoxTexTga16x16TextureImage[ k ] ;
					Image[ 2 ] = LineBoxTexTga16x16TextureImage[ k ] ;
					Image[ 3 ] = LineBoxTexTga16x16TextureImage[ k ] ;
				}
			}

			Graphics_Image_InitLoadGraphGParam( &GParam ) ;
			Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 16, TRUE, FALSE ) ;
			GParam.CreateGraphGParam.InitHandleGParam.TransColor = ( BYTE )255 << 16 | ( BYTE )0 << 8 | ( BYTE )255 ;
			GSYS.Resource.LineBoxTexHandle_PMA = Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, FALSE, GSYS.Resource.LineBoxTexHandle_PMA, &BaseImage, NULL ) ;
			NS_SetDeleteHandleFlag( GSYS.Resource.LineBoxTexHandle_PMA, &GSYS.Resource.LineBoxTexHandle_PMA ) ;
			NS_SetDeviceLostDeleteGraphFlag( GSYS.Resource.LineBoxTexHandle_PMA, TRUE ) ;

			NS_ReleaseBaseImage( &BaseImage ) ;
		}

		return GSYS.Resource.LineBoxTexHandle_PMA ;
	}
	else
	{
		if( GSYS.Resource.LineBoxTexHandle <= 0 )
		{
			LOADGRAPH_GPARAM GParam ;
			BASEIMAGE BaseImage ;
			BYTE *ImageBase ;
			BYTE *Image ;
			DWORD Pitch ;
			int i, j, k ;

			NS_CreateARGB8ColorBaseImage( 16, 16, &BaseImage ) ;
			ImageBase = ( BYTE * )BaseImage.GraphData ;
			Pitch = ( DWORD )BaseImage.Pitch ;
			k = 0 ;
			for( i = 0 ; i < 16; i ++, ImageBase += Pitch )
			{
				Image = ImageBase ;
				for( j = 0 ; j < 16 ; j ++, Image += 4, k++ )
				{
					Image[ 0 ] = 0xff ;
					Image[ 1 ] = 0xff ;
					Image[ 2 ] = 0xff ;
					Image[ 3 ] = LineBoxTexTga16x16TextureImage[ k ] ;
				}
			}

			Graphics_Image_InitLoadGraphGParam( &GParam ) ;
			Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 16, TRUE, FALSE ) ;
			GParam.CreateGraphGParam.InitHandleGParam.TransColor = ( BYTE )255 << 16 | ( BYTE )0 << 8 | ( BYTE )255 ;
			GSYS.Resource.LineBoxTexHandle = Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, FALSE, GSYS.Resource.LineBoxTexHandle, &BaseImage, NULL ) ;
			NS_SetDeleteHandleFlag( GSYS.Resource.LineBoxTexHandle, &GSYS.Resource.LineBoxTexHandle ) ;
			NS_SetDeviceLostDeleteGraphFlag( GSYS.Resource.LineBoxTexHandle, TRUE ) ;

			NS_ReleaseBaseImage( &BaseImage ) ;
		}

		return GSYS.Resource.LineBoxTexHandle ;
	}
}

// �A���`�G�C���A�X�t���̐���`��`�悷�邽�߂̃e�N�X�`���n���h�����擾����( ����1�s�N�Z���p )
extern int Graphics_Image_GetLineBoxOneThicknessTexHandle( int IsPMA )
{
	if( IsPMA )
	{
		if( GSYS.Resource.LineBoxOneThicknessTexHandle_PMA <= 0 )
		{
			LOADGRAPH_GPARAM GParam ;
			BASEIMAGE BaseImage ;
			BYTE *ImageBase ;
			BYTE *Image ;
			DWORD Pitch ;
			int i, j, k ;

			NS_CreateARGB8ColorBaseImage( 16, 16, &BaseImage ) ;
			ImageBase = ( BYTE * )BaseImage.GraphData ;
			Pitch = ( DWORD )BaseImage.Pitch ;
			k = 0 ;
			for( i = 0 ; i < 16; i ++, ImageBase += Pitch )
			{
				Image = ImageBase ;
				for( j = 0 ; j < 16 ; j ++, Image += 4, k++ )
				{
					Image[ 0 ] = LineBoxOneThicknessTexTga16x16TextureImage[ k ] ;
					Image[ 1 ] = LineBoxOneThicknessTexTga16x16TextureImage[ k ] ;
					Image[ 2 ] = LineBoxOneThicknessTexTga16x16TextureImage[ k ] ;
					Image[ 3 ] = LineBoxOneThicknessTexTga16x16TextureImage[ k ] ;
				}
			}

			Graphics_Image_InitLoadGraphGParam( &GParam ) ;
			Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 16, TRUE, FALSE ) ;
			GParam.CreateGraphGParam.InitHandleGParam.TransColor = ( BYTE )255 << 16 | ( BYTE )0 << 8 | ( BYTE )255 ;
			GSYS.Resource.LineBoxOneThicknessTexHandle_PMA = Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, FALSE, GSYS.Resource.LineBoxOneThicknessTexHandle_PMA, &BaseImage, NULL ) ;
			NS_SetDeleteHandleFlag( GSYS.Resource.LineBoxOneThicknessTexHandle_PMA, &GSYS.Resource.LineBoxOneThicknessTexHandle_PMA ) ;
			NS_SetDeviceLostDeleteGraphFlag( GSYS.Resource.LineBoxOneThicknessTexHandle_PMA, TRUE ) ;

			NS_ReleaseBaseImage( &BaseImage ) ;
		}

		return GSYS.Resource.LineBoxOneThicknessTexHandle_PMA ;
	}
	else
	{
		if( GSYS.Resource.LineBoxOneThicknessTexHandle <= 0 )
		{
			LOADGRAPH_GPARAM GParam ;
			BASEIMAGE BaseImage ;
			BYTE *ImageBase ;
			BYTE *Image ;
			DWORD Pitch ;
			int i, j, k ;

			NS_CreateARGB8ColorBaseImage( 16, 16, &BaseImage ) ;
			ImageBase = ( BYTE * )BaseImage.GraphData ;
			Pitch = ( DWORD )BaseImage.Pitch ;
			k = 0 ;
			for( i = 0 ; i < 16; i ++, ImageBase += Pitch )
			{
				Image = ImageBase ;
				for( j = 0 ; j < 16 ; j ++, Image += 4, k++ )
				{
					Image[ 0 ] = 0xff ;
					Image[ 1 ] = 0xff ;
					Image[ 2 ] = 0xff ;
					Image[ 3 ] = LineBoxOneThicknessTexTga16x16TextureImage[ k ] ;
				}
			}

			Graphics_Image_InitLoadGraphGParam( &GParam ) ;
			Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( &GParam.CreateGraphGParam.InitHandleGParam, 16, TRUE, FALSE ) ;
			GParam.CreateGraphGParam.InitHandleGParam.TransColor = ( BYTE )255 << 16 | ( BYTE )0 << 8 | ( BYTE )255 ;
			GSYS.Resource.LineBoxOneThicknessTexHandle = Graphics_Image_CreateGraphFromGraphImage_UseGParam( &GParam, FALSE, GSYS.Resource.LineBoxOneThicknessTexHandle, &BaseImage, NULL ) ;
			NS_SetDeleteHandleFlag( GSYS.Resource.LineBoxOneThicknessTexHandle, &GSYS.Resource.LineBoxOneThicknessTexHandle ) ;
			NS_SetDeviceLostDeleteGraphFlag( GSYS.Resource.LineBoxOneThicknessTexHandle, TRUE ) ;

			NS_ReleaseBaseImage( &BaseImage ) ;
		}

		return GSYS.Resource.LineBoxOneThicknessTexHandle ;
	}
}

// MakeGraph �̎������֐�
static int Graphics_Image_MakeGraph_Static(
	SETUP_GRAPHHANDLE_GPARAM *GParam,
	int GrHandle,
	int SizeX,
	int SizeY,
	int NotUse3DFlag,
	int UsePaletteFlag,
	int PaletteBitDepth,
	int AlphaValidFlag,
	int ASyncThread
)
{
	// �n���h���̏�����
	if( Graphics_Image_SetupHandle_UseGParam(
			GParam,
			GrHandle,
			SizeX,
			SizeY,
			!NotUse3DFlag,
			AlphaValidFlag,
			UsePaletteFlag,
			PaletteBitDepth,
			DX_BASEIMAGE_FORMAT_NORMAL,
			-1,
			ASyncThread
		) == -1 )
		return -1 ;

	// �I��
	return 0 ;
}

#ifndef DX_NON_ASYNCLOAD

// MakeGraph �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void Graphics_Image_MakeGraph_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	SETUP_GRAPHHANDLE_GPARAM *GParam ;
	int GrHandle ;
	int SizeX ;
	int SizeY ;
	int NotUse3DFlag ;
	int UsePaletteFlag ;
	int PaletteBitDepth ;
	int AlphaValidFlag ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	GParam = ( SETUP_GRAPHHANDLE_GPARAM * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	GrHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	SizeX = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	SizeY = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	NotUse3DFlag = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	UsePaletteFlag = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	PaletteBitDepth = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	AlphaValidFlag = GetASyncLoadParamInt( AParam->Data, &Addr ) ;

	Result = Graphics_Image_MakeGraph_Static( GParam, GrHandle, SizeX, SizeY, NotUse3DFlag, UsePaletteFlag, PaletteBitDepth, AlphaValidFlag, TRUE ) ;

	DecASyncLoadCount( GrHandle ) ;
	if( Result < 0 )
	{
		NS_DeleteGraph( GrHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// ��̃O���t�B�b�N�n���h�����쐬����֐�
extern int Graphics_Image_MakeGraph_UseGParam(
	SETUP_GRAPHHANDLE_GPARAM *GParam,
	int SizeX,
	int SizeY,
	int NotUse3DFlag,
	int UsePaletteFlag,
	int PaletteBitDepth,
	int ASyncLoadFlag,
	int ASyncThread
)
{
	int GrHandle ;

	CheckActiveState() ;

	// �n���h���̍쐬
	GrHandle = Graphics_Image_AddHandle( ASyncThread ) ;
	if( GrHandle == -1 )
	{
		return -1 ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag && ASyncThread == FALSE )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamStruct( NULL, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( NULL, &Addr, GrHandle ) ;
		AddASyncLoadParamInt( NULL, &Addr, SizeX ) ;
		AddASyncLoadParamInt( NULL, &Addr, SizeY ) ;
		AddASyncLoadParamInt( NULL, &Addr, NotUse3DFlag ) ;
		AddASyncLoadParamInt( NULL, &Addr, UsePaletteFlag ) ;
		AddASyncLoadParamInt( NULL, &Addr, PaletteBitDepth ) ;
		AddASyncLoadParamInt( NULL, &Addr, GParam->AlphaChannelImageCreateFlag ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = Graphics_Image_MakeGraph_ASync ;
		Addr = 0 ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, GrHandle ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SizeX ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SizeY ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, NotUse3DFlag ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, UsePaletteFlag ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, PaletteBitDepth ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, GParam->AlphaChannelImageCreateFlag ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( GrHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( Graphics_Image_MakeGraph_Static( GParam, GrHandle, SizeX, SizeY, NotUse3DFlag, UsePaletteFlag, PaletteBitDepth, GParam->AlphaChannelImageCreateFlag, ASyncThread ) < 0 )
		{
			goto ERR ;
		}
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( GrHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	// �I��
	return GrHandle ;

ERR :
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( GrHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	NS_DeleteGraph( GrHandle ) ;

	return -1 ;
}

// CreateGraph �̎������֐�
static int Graphics_Image_CreateGraph_Static(
	LOADGRAPH_PARAM *Param,
	int ASyncThread
)
{
	char    FullPath_UTF16LE[ 2048 ] ;
	int Result = 0 ;
	CREATEGRAPH_LOADBASEIMAGE_PARAM LParam ;

	// �摜�f�[�^�̓ǂݍ���
	Graphics_Image_CreateGraph_LoadBaseImage( Param, &LParam ) ;

	if( LParam.LoadHr == -1 ) 
	{
		// �ēǂݍ��݂ł͂Ȃ��ꍇ�̓��[�r�[�̉\��������
		if( Param->ReCreateFlag == FALSE && ( Param->FileName != NULL || Param->RgbMemImage != NULL ) )
		{
#ifndef DX_NON_MOVIE
			// �t�@�C���̏ꍇ�̓t�@�C�������邩�m�F
			if( Param->FileName != NULL )
			{
				DWORD_PTR fp ;

				fp = DX_FOPEN( Param->FileName ) ;
				if( fp == 0 )
				{
					wchar_t FullPath_WCHAR_T[ 1024 ] ;

					ConvertFullPathW_( Param->FileName, FullPath_WCHAR_T, sizeof( FullPath_WCHAR_T ) ) ;
					ConvString( ( const char * )FullPath_WCHAR_T, WCHAR_T_CHARCODEFORMAT, FullPath_UTF16LE, sizeof( FullPath_UTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;

					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x3b\x75\xcf\x50\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x20\x00\x25\x00\x73\x00\x20\x00\x4c\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x00"/*@ L"�摜�t�@�C�� %s ������܂���" @*/, FullPath_UTF16LE )) ;
					return -1 ;
				}
				DX_FCLOSE( fp ) ;
			}

			// ����̃I�[�v�������݂�
			if( Graphics_Image_OpenMovie_UseGParam( &Param->GParam, Param->GrHandle, Param->FileName, Param->RgbMemImage, Param->RgbMemImageSize, Param->TextureFlag, Param->SurfaceMode, ASyncThread ) < 0 )
			{
				return -1 ;
			}

			// ����摜���N���A����
			NS_FillGraph( Param->GrHandle, 0, 0, 0, 0 ) ;

			// ����I��
			return 0 ;
#else	// DX_NON_MOVIE
			{
				if( Param->FileName != NULL )
				{
					ConvString( ( const char * )Param->FileName, WCHAR_T_CHARCODEFORMAT, FullPath_UTF16LE, sizeof( FullPath_UTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x3b\x75\xcf\x50\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xaa\x30\xfc\x30\xd7\x30\xf3\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�摜�t�@�C�� %s �̃I�[�v���Ɏ��s���܂���" @*/, FullPath_UTF16LE )) ;
				}
				return -1 ;
			}
#endif	// DX_NON_MOVIE
		}
		else
		{
			return -1 ;
		}
	}

	// �O���t�B�b�N�n���h�����쐬����
	Result = Graphics_Image_CreateGraphFromGraphImageBase_UseGParam(
				&Param->GParam.CreateGraphGParam,
				Param->ReCreateFlag,
				Param->GrHandle,
				LParam.UseRgbBaseImage,
				LParam.UseAlphaBaseImage,
				Param->TextureFlag,
				ASyncThread
			) ;

	// �t�@�C���p�X���Z�b�g����
	if( Result == 0 )
	{
		Graphics_Image_SetBaseInfo_UseGParam(
			&Param->GParam.SetGraphBaseInfoGParam,
			Param->GrHandle,
			Param->FileName,
			NULL,
			NULL,
			NULL,
			Param->RgbMemImage,
			Param->RgbMemImageSize,
			Param->AlphaMemImage,
			Param->AlphaMemImageSize,
			LParam.UseRgbBaseImage,
			LParam.UseAlphaBaseImage,
			Param->ReverseFlag,
			Param->GParam.LoadBaseImageGParam.ConvertPremultipliedAlpha,
			-1,
			ASyncThread
		) ;
	}

	// �ǂݍ��񂾃O���t�B�b�N�̌�n��������
	Graphics_Image_CreateGraph_TerminateBaseImage( Param, &LParam ) ;

	// �I��
	return Result ;
}

#ifndef DX_NON_ASYNCLOAD

// CreateGraph �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void Graphics_Image_CreateGraph_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	LOADGRAPH_PARAM *Param ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	Param = ( LOADGRAPH_PARAM * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;

	if( Param->FileName != NULL )
	{
		Param->FileName = GetASyncLoadParamString( AParam->Data, &Addr ) ;
	}

	if( Param->RgbBaseImage != NULL )
	{
		Param->RgbBaseImage = ( BASEIMAGE * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	}

	if( Param->AlphaBaseImage != NULL )
	{
		Param->AlphaBaseImage = ( BASEIMAGE * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	}

	Result = Graphics_Image_CreateGraph_Static( Param, TRUE ) ;
	DecASyncLoadCount( Param->GrHandle ) ;
	if( Param->ReCreateFlag == FALSE )
	{
		if( Result < 0 )
		{
			NS_DeleteGraph( Param->GrHandle ) ;
		}
	}
}
#endif // DX_NON_ASYNCLOAD

// CreateGraph �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_CreateGraph_UseGParam( 
	LOADGRAPH_PARAM *Param,
	int ASyncLoadFlag,
	int ASyncThread
)
{
	if( ASyncThread == FALSE )
	{
		CheckActiveState() ;
	}

	if( Param->ReCreateFlag == FALSE )
	{
		Param->GrHandle = Graphics_Image_AddHandle( ASyncThread ) ;
		if( Param->GrHandle < 0 )
		{
			return -1 ;
		}
	}
	else
	{
#ifndef DX_NON_ASYNCLOAD
		if( NS_CheckHandleASyncLoad( Param->GrHandle ) > 0 )
		{
			return -1 ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		if( ASyncThread )
		{
			IncASyncLoadCount( Param->GrHandle, -1 ) ;
		}
#endif // DX_NON_ASYNCLOAD
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread == FALSE && ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;
		wchar_t FullPath[ 1024 ] ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamStruct( NULL, &Addr, Param, sizeof( *Param ) ) ;
		if( Param->FileName != NULL )
		{
			ConvertFullPathW_( Param->FileName, FullPath, sizeof( FullPath ) ) ;
			AddASyncLoadParamString( NULL, &Addr, FullPath ) ; 
		}
		if( Param->RgbBaseImage != NULL )
		{
			AddASyncLoadParamStruct( NULL, &Addr, Param->RgbBaseImage, sizeof( *Param->RgbBaseImage ) ) ;
		}
		if( Param->AlphaBaseImage != NULL )
		{
			AddASyncLoadParamStruct( NULL, &Addr, Param->AlphaBaseImage, sizeof( *Param->AlphaBaseImage ) ) ;
		}

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = Graphics_Image_CreateGraph_ASync ;
		Addr = 0 ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, Param, sizeof( *Param ) ) ;
		if( Param->FileName != NULL )
		{
			AddASyncLoadParamString( AParam->Data, &Addr, FullPath ) ; 
		}
		if( Param->RgbBaseImage != NULL )
		{
			AddASyncLoadParamStruct( AParam->Data, &Addr, Param->RgbBaseImage, sizeof( *Param->RgbBaseImage ) ) ;
		}
		if( Param->AlphaBaseImage != NULL )
		{
			AddASyncLoadParamStruct( AParam->Data, &Addr, Param->AlphaBaseImage, sizeof( *Param->AlphaBaseImage ) ) ;
		}

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( Param->GrHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( Graphics_Image_CreateGraph_Static( Param, ASyncThread ) < 0 )
		{
			goto ERR ;
		}
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( Param->GrHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	// ����I��
	return Param->ReCreateFlag == FALSE ? Param->GrHandle : 0 ;

ERR :
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( Param->GrHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	if( Param->ReCreateFlag == FALSE )
	{
		NS_DeleteGraph( Param->GrHandle ) ;
		Param->GrHandle = -1 ;
	}

	return -1 ;
}


// Graphics_Image_CreateDivGraph �̎������֐�
static int Graphics_Image_CreateDivGraph_Static(
	LOADGRAPH_PARAM *Param,
	int ASyncThread
)
{
	int Result ;
	int i ;
	CREATEGRAPH_LOADBASEIMAGE_PARAM LParam ;

	// �摜�f�[�^�̓ǂݍ���
	Graphics_Image_CreateGraph_LoadBaseImage( Param, &LParam ) ;
	if( LParam.LoadHr == -1 )
	{
		if( Param->FileName != NULL )
		{
			wchar_t FullPath_WCHAR_T[ 1024 ] ;
			char    FullPath_UTF16LE[ 2048 ] ;

			ConvertFullPathW_( Param->FileName, FullPath_WCHAR_T, sizeof( FullPath_WCHAR_T ) ) ;
			ConvString( ( const char * )FullPath_WCHAR_T, WCHAR_T_CHARCODEFORMAT, FullPath_UTF16LE, sizeof( FullPath_UTF16LE ), DX_CHARCODEFORMAT_UTF16LE ) ;

			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x3b\x75\xcf\x50\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x20\x00\x25\x00\x73\x00\x20\x00\x6e\x30\xed\x30\xfc\x30\xc9\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�摜�t�@�C�� %s �̃��[�h�Ɏ��s���܂���" @*/, FullPath_UTF16LE )) ;
		}
		return -1 ;
	}

	// �����O���t�B�b�N�n���h�����쐬����
	Result = Graphics_Image_CreateDivGraphFromGraphImageBase_UseGParam(
				&Param->GParam.CreateGraphGParam,
				Param->ReCreateFlag,
				Param->BaseHandle,
				LParam.UseRgbBaseImage,
				LParam.UseAlphaBaseImage,
				Param->AllNum,
				Param->XNum,
				Param->YNum,
				Param->IsFloat,
				Param->SizeXI,
				Param->SizeXF,
				Param->SizeYI,
				Param->SizeYF,
				Param->HandleBuf,
				Param->TextureFlag,
				Param->ReverseFlag,
				ASyncThread
			) ;

	// �t�@�C���p�X���Z�b�g����
	if( Result == 0 )
	{
		Graphics_Image_SetBaseInfo_UseGParam(
			&Param->GParam.SetGraphBaseInfoGParam,
			Param->HandleBuf[ 0 ],
			Param->FileName,
			NULL,
			NULL,
			NULL,
			Param->RgbMemImage,
			Param->RgbMemImageSize,
			Param->AlphaMemImage,
			Param->AlphaMemImageSize,
			LParam.UseRgbBaseImage,
			LParam.UseAlphaBaseImage,
			Param->ReverseFlag,
			Param->GParam.LoadBaseImageGParam.ConvertPremultipliedAlpha,
			-1,
			ASyncThread
		) ;
		for( i = 1 ; i < Param->AllNum ; i ++ )
		{
			Graphics_Image_SetBaseInfo_UseGParam(
				&Param->GParam.SetGraphBaseInfoGParam,
				Param->HandleBuf[ i ],
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				0,
				NULL,
				0,
				NULL,
				NULL,
				FALSE,
				FALSE,
				Param->HandleBuf[ 0 ],
				ASyncThread
			) ;
		}
	}

	// �ǂݍ��񂾃O���t�B�b�N�̌�n��������
	Graphics_Image_CreateGraph_TerminateBaseImage( Param, &LParam ) ;

	// �I��
	return Result ;
}

#ifndef DX_NON_ASYNCLOAD

// Graphics_Image_CreateDivGraph �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void Graphics_Image_CreateDivGraph_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	LOADGRAPH_PARAM *Param ;
	int Addr ;
	int i ;
	int Result ;

	Addr = 0 ;
	Param = ( LOADGRAPH_PARAM * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;

	Param->HandleBuf = ( int * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;

	if( Param->FileName != NULL )
	{
		Param->FileName = GetASyncLoadParamString( AParam->Data, &Addr ) ;
	}

	if( Param->RgbBaseImage != NULL )
	{
		Param->RgbBaseImage = ( BASEIMAGE * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	}

	if( Param->AlphaBaseImage != NULL )
	{
		Param->AlphaBaseImage = ( BASEIMAGE * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	}

	Result = Graphics_Image_CreateDivGraph_Static( Param, TRUE ) ;

	DecASyncLoadCount( Param->BaseHandle ) ;
	for( i = 0 ; i < Param->AllNum ; i ++ )
		DecASyncLoadCount( Param->HandleBuf[ i ] ) ;

	if( Param->ReCreateFlag == FALSE )
	{
		NS_DeleteGraph( Param->BaseHandle ) ;
		if( Result < 0 )
		{
			for( i = 0 ; i < Param->AllNum ; i ++ )
				NS_DeleteGraph( Param->HandleBuf[ i ] ) ;
		}
	}
}
#endif // DX_NON_ASYNCLOAD

// �摜�𕪊����ăO���t�B�b�N�n���h�����쐬����֐�
extern int Graphics_Image_CreateDivGraph_UseGParam( 
	LOADGRAPH_PARAM *Param,
	int ASyncLoadFlag,
	int ASyncThread
)
{
	int i ;

	CheckActiveState() ;

	if( Param->AllNum == 0 )
		return -1 ;

	Param->BaseHandle = -1 ;
	if( Param->ReCreateFlag == FALSE )
	{
		// �O���t�B�b�N�n���h���̍쐬
		_MEMSET( Param->HandleBuf, 0, sizeof( int ) * Param->AllNum ) ;
		for( i = 0 ; i < Param->AllNum ; i ++ )
		{
			Param->HandleBuf[ i ] = Graphics_Image_AddHandle( ASyncThread ) ;
			if( Param->HandleBuf[ i ] < 0 )
			{
				goto ERR ;
			}
		}
		Param->BaseHandle = Graphics_Image_AddHandle( ASyncThread ) ;
		if( Param->BaseHandle < 0 )
		{
			goto ERR ;
		}
	}
	else
	{
#ifndef DX_NON_ASYNCLOAD
		for( i = 0 ; i < Param->AllNum ; i ++ )
		{
			if( NS_CheckHandleASyncLoad( Param->HandleBuf[ i ] ) > 0 )
			{
				return -1 ;
			}
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		if( ASyncThread )
		{
			IncASyncLoadCount( Param->BaseHandle, -1 ) ;
			for( i = 0 ; i < Param->AllNum ; i ++ )
			{
				IncASyncLoadCount( Param->HandleBuf[ i ], -1 ) ;
			}
		}
#endif // DX_NON_ASYNCLOAD
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamStruct( NULL, &Addr, Param, sizeof( *Param ) ) ;
		AddASyncLoadParamStruct( NULL, &Addr, Param->HandleBuf, ( int )( sizeof( int ) * Param->AllNum ) ) ;
		if( Param->FileName != NULL )
		{
			AddASyncLoadParamString( NULL, &Addr, Param->FileName ) ; 
		}
		if( Param->RgbBaseImage != NULL )
		{
			AddASyncLoadParamStruct( NULL, &Addr, Param->RgbBaseImage, sizeof( *Param->RgbBaseImage ) ) ;
		}
		if( Param->AlphaBaseImage != NULL )
		{
			AddASyncLoadParamStruct( NULL, &Addr, Param->AlphaBaseImage, sizeof( *Param->AlphaBaseImage ) ) ;
		}

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = Graphics_Image_CreateDivGraph_ASync ;
		Addr = 0 ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, Param, sizeof( *Param ) ) ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, Param->HandleBuf, ( int )( sizeof( int ) * Param->AllNum ) ) ;
		if( Param->FileName != NULL )
		{
			AddASyncLoadParamString( AParam->Data, &Addr, Param->FileName ) ; 
		}
		if( Param->RgbBaseImage != NULL )
		{
			AddASyncLoadParamStruct( AParam->Data, &Addr, Param->RgbBaseImage, sizeof( *Param->RgbBaseImage ) ) ;
		}
		if( Param->AlphaBaseImage != NULL )
		{
			AddASyncLoadParamStruct( AParam->Data, &Addr, Param->AlphaBaseImage, sizeof( *Param->AlphaBaseImage ) ) ;
		}

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		for( i = 0 ; i < Param->AllNum ; i ++ )
			IncASyncLoadCount( Param->HandleBuf[ i ], AParam->Index ) ;
		IncASyncLoadCount( Param->BaseHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( Graphics_Image_CreateDivGraph_Static( Param, FALSE ) < 0 )
			goto ERR ;

		if( Param->ReCreateFlag == FALSE )
		{
			NS_DeleteGraph( Param->BaseHandle ) ;
		}
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		// �񓯊��ǂݍ��݃J�E���g���f�N�������g
		DecASyncLoadCount( Param->BaseHandle ) ;
		for( i = 0 ; i < Param->AllNum ; i ++ )
		{
			DecASyncLoadCount( Param->HandleBuf[ i ] ) ;
		}
	}
#endif // DX_NON_ASYNCLOAD

	// ����I��
	return 0 ;

ERR :
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		// �񓯊��ǂݍ��݃J�E���g���f�N�������g
		DecASyncLoadCount( Param->BaseHandle ) ;
		for( i = 0 ; i < Param->AllNum ; i ++ )
		{
			DecASyncLoadCount( Param->HandleBuf[ i ] ) ;
		}
	}
#endif // DX_NON_ASYNCLOAD

	if( Param->ReCreateFlag == FALSE )
	{
		for( i = 0 ; i < Param->AllNum ; i ++ )
		{
			NS_DeleteGraph( Param->HandleBuf[ i ] ) ;
		}
		NS_DeleteGraph( Param->BaseHandle ) ;
	}

	return -1 ;
}


// LoadBmpToGraph �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_LoadBmpToGraph_UseGParam(
	LOADGRAPH_GPARAM *GParam,
	int ReCreateFlag,
	int GrHandle,
	const wchar_t *GraphName,
	int TextureFlag,
	int ReverseFlag,
	int SurfaceMode,
	int ASyncLoadFlag
)
{
	LOADGRAPH_PARAM Param ;

	_MEMSET( &Param, 0, sizeof( Param ) ) ;
	Param.GParam = *GParam ;
	Param.ReCreateFlag = ReCreateFlag ;
	Param.GrHandle = GrHandle ;

	Param.FileName = GraphName ;

	Param.TextureFlag = TextureFlag ;
	Param.ReverseFlag = ReverseFlag ;
	Param.SurfaceMode = SurfaceMode ;

	return Graphics_Image_CreateGraph_UseGParam( &Param, ASyncLoadFlag ) ;
}

// LoadDivBmpToGraph �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_LoadDivBmpToGraph_UseGParam(
	LOADGRAPH_GPARAM *GParam,
	int ReCreateFlag,
	const wchar_t *FileName,
	int AllNum,
	int XNum,
	int YNum,
	int IsFloat,
	int   SizeXI,
	float SizeXF,
	int   SizeYI,
	float SizeYF,
	int *HandleBuf,
	int TextureFlag,
	int ReverseFlag,
	int ASyncLoadFlag,
	int ASyncThread
)
{
	LOADGRAPH_PARAM Param ;

	_MEMSET( &Param, 0, sizeof( Param ) ) ;
	Param.GParam = *GParam ;
	Param.ReCreateFlag = ReCreateFlag ;

	Param.FileName = FileName ;

	Param.AllNum = AllNum ;
	Param.XNum = XNum ;
	Param.YNum = YNum ;
	Param.IsFloat = IsFloat ;
	Param.SizeXI = SizeXI ;
	Param.SizeXF = SizeXF ;
	Param.SizeYI = SizeYI ;
	Param.SizeYF = SizeYF ;
	Param.HandleBuf = HandleBuf ;
	Param.TextureFlag = TextureFlag ;
	Param.ReverseFlag = ReverseFlag ;

	return Graphics_Image_CreateDivGraph_UseGParam( &Param, ASyncLoadFlag, ASyncThread ) ;
}

// CreateGraphFromMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_CreateGraphFromMem_UseGParam( 
	LOADGRAPH_GPARAM *GParam,
	int ReCreateFlag,
	int GrHandle,
	const void *MemImage,
	int MemImageSize,
	const void *AlphaImage,
	int AlphaImageSize,
	int TextureFlag,
	int ReverseFlag,
	int ASyncLoadFlag
)
{
	LOADGRAPH_PARAM Param ;

	_MEMSET( &Param, 0, sizeof( Param ) ) ;
	Param.GParam = *GParam ;
	Param.ReCreateFlag = ReCreateFlag ;
	Param.GrHandle = GrHandle ;

	Param.RgbMemImage = MemImage ;
	Param.RgbMemImageSize = MemImageSize ;
	Param.AlphaMemImage = AlphaImage ;
	Param.AlphaMemImageSize = AlphaImageSize ;

	Param.TextureFlag = TextureFlag ;
	Param.ReverseFlag = ReverseFlag ;
	Param.SurfaceMode = DX_MOVIESURFACE_NORMAL ;

	return Graphics_Image_CreateGraph_UseGParam( &Param, ASyncLoadFlag ) ;
}

// CreateDivGraphFromMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_CreateDivGraphFromMem_UseGParam(
	LOADGRAPH_GPARAM *GParam,
	int ReCreateFlag,
	const void *MemImage,
	int MemImageSize,
	int AllNum,
	int XNum,
	int YNum,
	int IsFloat,
	int   SizeXI,
	float SizeXF,
	int   SizeYI,
	float SizeYF,
	int *HandleBuf,
	int TextureFlag,
	int ReverseFlag,
	const void *AlphaImage,
	int AlphaImageSize,
	int ASyncLoadFlag,
	int ASyncThread
)
{
	LOADGRAPH_PARAM Param ;

	_MEMSET( &Param, 0, sizeof( Param ) ) ;
	Param.GParam = *GParam ;
	Param.ReCreateFlag = ReCreateFlag ;

	Param.RgbMemImage = MemImage ;
	Param.RgbMemImageSize = MemImageSize ;
	Param.AlphaMemImage = AlphaImage ;
	Param.AlphaMemImageSize = AlphaImageSize ;

	Param.AllNum = AllNum ;
	Param.XNum = XNum ;
	Param.YNum = YNum ;
	Param.IsFloat = IsFloat ;
	Param.SizeXI = SizeXI ;
	Param.SizeXF = SizeXF ;
	Param.SizeYI = SizeYI ;
	Param.SizeYF = SizeYF ;
	Param.HandleBuf = HandleBuf ;
	Param.TextureFlag = TextureFlag ;
	Param.ReverseFlag = ReverseFlag ;

	return Graphics_Image_CreateDivGraph_UseGParam( &Param, ASyncLoadFlag, ASyncThread ) ;
}

// CreateGraphFromBmp �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_CreateGraphFromBmp_UseGParam(
	LOADGRAPH_GPARAM *GParam,
	int ReCreateFlag,
	int GrHandle,
	const BITMAPINFO *BmpInfo,
	const void *GraphData,
	const BITMAPINFO *AlphaInfo,
	const void *AlphaData,
	int TextureFlag,
	int ReverseFlag,
	int ASyncLoadFlag
)
{
	LOADGRAPH_PARAM Param ;

	_MEMSET( &Param, 0, sizeof( Param ) ) ;
	Param.GParam = *GParam ;
	Param.ReCreateFlag = ReCreateFlag ;
	Param.GrHandle = GrHandle ;

	Param.RgbBmpInfo = BmpInfo ;
	Param.RgbBmpImage = ( void * )GraphData ;
	Param.AlphaBmpInfo = AlphaInfo ;
	Param.AlphaBmpImage = ( void * )AlphaData ;

	Param.TextureFlag = TextureFlag ;
	Param.ReverseFlag = ReverseFlag ;
	Param.SurfaceMode = DX_MOVIESURFACE_NORMAL ;

	return Graphics_Image_CreateGraph_UseGParam( &Param, ASyncLoadFlag ) ;
}

// CreateDivGraphFromBmp �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_CreateDivGraphFromBmp_UseGParam(
	LOADGRAPH_GPARAM *GParam,
	int ReCreateFlag,
	const BITMAPINFO *BmpInfo,
	const void *GraphData,
	int AllNum,
	int XNum,
	int YNum,
	int IsFloat,
	int   SizeXI,
	float SizeXF,
	int   SizeYI,
	float SizeYF,
	int *HandleBuf,
	int TextureFlag,
	int ReverseFlag,
	const BITMAPINFO *AlphaInfo,
	const void *AlphaData,
	int ASyncLoadFlag
)
{
	LOADGRAPH_PARAM Param ;

	_MEMSET( &Param, 0, sizeof( Param ) ) ;
	Param.GParam = *GParam ;
	Param.ReCreateFlag = ReCreateFlag ;

	Param.RgbBmpInfo = BmpInfo ;
	Param.RgbBmpImage = ( void * )GraphData ;
	Param.AlphaBmpInfo = AlphaInfo ;
	Param.AlphaBmpImage = ( void * )AlphaData ;

	Param.AllNum = AllNum ;
	Param.XNum = XNum ;
	Param.YNum = YNum ;
	Param.IsFloat = IsFloat ;
	Param.SizeXI = SizeXI ;
	Param.SizeXF = SizeXF ;
	Param.SizeYI = SizeYI ;
	Param.SizeYF = SizeYF ;
	Param.HandleBuf = HandleBuf ;
	Param.TextureFlag = TextureFlag ;
	Param.ReverseFlag = ReverseFlag ;

	return Graphics_Image_CreateDivGraph_UseGParam( &Param, ASyncLoadFlag, FALSE ) ;
}

// CreateGraphFromGraphImage �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_CreateGraphFromGraphImage_UseGParam(
	LOADGRAPH_GPARAM *GParam,
	int ReCreateFlag,
	int GrHandle,
	const BASEIMAGE *BaseImage,
	const BASEIMAGE *AlphaBaseImage,
	int TextureFlag,
	int ReverseFlag,
	int ASyncLoadFlag,
	int ASyncThread
)
{
	LOADGRAPH_PARAM Param ;

	_MEMSET( &Param, 0, sizeof( Param ) ) ;
	Param.GParam = *GParam ;
	Param.ReCreateFlag = ReCreateFlag ;
	Param.GrHandle = GrHandle ;

	Param.RgbBaseImage = ( BASEIMAGE * )BaseImage ;
	Param.AlphaBaseImage = ( BASEIMAGE * )AlphaBaseImage ;

	Param.TextureFlag = TextureFlag ;
	Param.ReverseFlag = ReverseFlag ;
	Param.SurfaceMode = DX_MOVIESURFACE_NORMAL ;

	return Graphics_Image_CreateGraph_UseGParam( &Param, ASyncLoadFlag, ASyncThread ) ;
}

// CreateDivGraphFromGraphImage �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_CreateDivGraphFromGraphImage_UseGParam(
	LOADGRAPH_GPARAM *GParam,
	int ReCreateFlag,
	const BASEIMAGE *BaseImage,
	const BASEIMAGE *AlphaBaseImage,
	int AllNum,
	int XNum,
	int YNum,
	int IsFloat,
	int   SizeXI,
	float SizeXF,
	int   SizeYI,
	float SizeYF,
	int *HandleBuf,
	int TextureFlag,
	int ReverseFlag,
	int ASyncLoadFlag
)
{
	LOADGRAPH_PARAM Param ;

	_MEMSET( &Param, 0, sizeof( Param ) ) ;
	Param.GParam = *GParam ;
	Param.ReCreateFlag = ReCreateFlag ;

	Param.RgbBaseImage = ( BASEIMAGE * )BaseImage ;
	Param.AlphaBaseImage = ( BASEIMAGE * )AlphaBaseImage ;

	Param.AllNum = AllNum ;
	Param.XNum = XNum ;
	Param.YNum = YNum ;
	Param.IsFloat = IsFloat ;
	Param.SizeXI = SizeXI ;
	Param.SizeXF = SizeXF ;
	Param.SizeYI = SizeYI ;
	Param.SizeYF = SizeYF ;
	Param.HandleBuf = HandleBuf ;
	Param.TextureFlag = TextureFlag ;
	Param.ReverseFlag = ReverseFlag ;

	return Graphics_Image_CreateDivGraph_UseGParam( &Param, ASyncLoadFlag, FALSE ) ;
}

// Graphics_Image_CreateGraphFromGraphImageBase �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_CreateGraphFromGraphImageBase_UseGParam(
	CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM *GParam,
	int ReCreateFlag,
	int GrHandle,
	BASEIMAGE *BaseImage,
	const BASEIMAGE *AlphaBaseImage,
	int TextureFlag,
	int ASyncThread
)
{
	int SizeX, SizeY ;
	int UseTempImage = FALSE ;
	BASEIMAGE TempImage ;
	int Result ;

	// �č쐬�ł͂Ȃ��ꍇ�͉摜�̓��ߐF�̏������s��
	if( ReCreateFlag == FALSE )
	{
		// ���ߐF�J���[�g�p��O��Ƃ����摜�ɂ���ꍇ�́A�����œ��ߐF�J���[�����O��摜���쐬����
		if( GParam->UseTransColorGraphCreateFlag && GParam->NotUseTransColor == FALSE )
		{
			int w, h, i, j, r, g, b, a, r2, g2, b2, a2, tr, tg, tb, useAlpha;

			w = BaseImage->Width;
			h = BaseImage->Height;
			tr = ( int )( ( GParam->InitHandleGParam.TransColor >> 16 ) & 0xff ) ;
			tg = ( int )( ( GParam->InitHandleGParam.TransColor >>  8 ) & 0xff ) ;
			tb = ( int )( ( GParam->InitHandleGParam.TransColor >>  0 ) & 0xff ) ;
			useAlpha = BaseImage->ColorData.AlphaWidth != 0;
			if( NS_CreateARGB8ColorBaseImage( w, h, &TempImage ) == 0 )
			{
				// ���ߐF�̃`�F�b�N���s���Ȃ���摜��]��
				for( i = 0; i < h; i++ )
				{
					for( j = 0; j < w; j++ )
					{
						NS_GetPixelBaseImage(      BaseImage, j, i, &r, &g, &b, &a );
						NS_SetPixelBaseImage( &TempImage, j, i,  r,  g,  b, r == tr && g == tg && b == tb ? 0 : ( useAlpha ? a : 255 ) );
					}
				}

				// ���ߐF�ł͂Ȃ������̐F�𓧉ߐF�̕����ɑ������
				for( i = 0; i < h; i++ )
				{
					for( j = 0; j < w; j++ )
					{
						NS_GetPixelBaseImage( BaseImage, j, i, &r, &g, &b, &a );
						if( r != tr || g != tg || b != tb )
						{
							if( j >    0 ){ NS_GetPixelBaseImage( BaseImage, j-1, i  ,&r2,&g2,&b2,&a2); if( r2 == tr && g2 == tg && b2 == tb ) NS_SetPixelBaseImage( &TempImage, j-1, i  ,r,g,b,0); }
							if( i >    0 ){ NS_GetPixelBaseImage( BaseImage, j  , i-1,&r2,&g2,&b2,&a2); if( r2 == tr && g2 == tg && b2 == tb ) NS_SetPixelBaseImage( &TempImage, j  , i-1,r,g,b,0); }
							if( j < w -1 ){ NS_GetPixelBaseImage( BaseImage, j+1, i  ,&r2,&g2,&b2,&a2); if( r2 == tr && g2 == tg && b2 == tb ) NS_SetPixelBaseImage( &TempImage, j+1, i  ,r,g,b,0); }
							if( i < h -1 ){ NS_GetPixelBaseImage( BaseImage, j  , i+1,&r2,&g2,&b2,&a2); if( r2 == tr && g2 == tg && b2 == tb ) NS_SetPixelBaseImage( &TempImage, j  , i+1,r,g,b,0); }
						}
					}
				}

				// �V�����C���[�W�̏����Z�b�g
				BaseImage = &TempImage;

				// ���摜���g�p���Ă���t���O�𗧂Ă�
				UseTempImage = TRUE ;
			}
		}
	}

	// �Ȃ�̋@�\������t�����Ă��Ȃ��r�b�g�̃}�X�N���Z�b�g����
	// (NoneMask �͓r��������ꂽ�����o�ϐ��Ȃ̂ŁA���ʌ݊������������邽�߂ɁE�E�E)
	NS_SetColorDataNoneMask( ( COLORDATA * )&BaseImage->ColorData ) ;
	if( AlphaBaseImage != NULL ) NS_SetColorDataNoneMask( ( COLORDATA * )&AlphaBaseImage->ColorData ) ;

	// �č쐬�̏ꍇ�̓T�C�Y�����ꂩ�`�F�b�N����
	if( ReCreateFlag )
	{
		// �T�C�Y���������G���[
		if( NS_GetGraphSize( GrHandle, &SizeX, &SizeY ) == -1 ) return -1 ;
		if( SizeX != BaseImage->Width || SizeY != BaseImage->Height ) return -1 ;
		if( AlphaBaseImage != NULL && ( SizeX != AlphaBaseImage->Width || SizeY != AlphaBaseImage->Height ) ) return -1 ;

		LUTRANS_RE_GPARAM( GParam, BaseImage, GrHandle )

		Result = 0 ;
	}
	else
	{
		LUTRANS_GPARAM( GParam, BaseImage ) ;

		// �O���t�B�b�N�n���h�����쐬����
		Result = Graphics_Image_CreateDXGraph_UseGParam( &GParam->InitHandleGParam, GrHandle, BaseImage, AlphaBaseImage, TextureFlag, ASyncThread ) ;
	}

	if( Result != -1 )
	{
		Graphics_Image_BltBmpOrGraphImageToGraphBase(
			BaseImage,
			AlphaBaseImage,
			0,
			0,
			GrHandle,
			GParam->NotUseTransColor ? FALSE : TRUE,
			ASyncThread
		) ;
	}

	// ���摜���g�p���Ă����ꍇ�͉������
	if( UseTempImage )
	{
		NS_ReleaseBaseImage( &TempImage );
	}

	// ���������s����Ԃ�
	return Result ;
}

// Graphics_Image_CreateDivGraphFromGraphImageBase �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int Graphics_Image_CreateDivGraphFromGraphImageBase_UseGParam(
	CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM *GParam,
	int ReCreateFlag,
	int BaseHandle,
	BASEIMAGE *BaseImage,
	const BASEIMAGE *AlphaBaseImage,
	int AllNum,
	int XNum,
	int YNum,
	int IsFloat,
	int   SizeXI,
	float SizeXF,
	int   SizeYI,
	float SizeYF,
	int *HandleBuf,
	int TextureFlag,
	int ReverseFlag,
	int ASyncThread
)
{
	int Result ;
	int GrSizeX, GrSizeY ;

	if( ReCreateFlag == FALSE )
	{
		int i, j, k ;
		int basexI, addxI ;
		float basexF, addxF ;

		// ���ʂɃO���t�B�b�N���쐬����
		if( Graphics_Image_CreateGraphFromGraphImageBase_UseGParam( GParam, FALSE, BaseHandle, BaseImage, AlphaBaseImage, TextureFlag, ASyncThread ) < 0 )
			return -1 ;

		if( ReverseFlag == TRUE )
		{
			basexI = ( XNum - 1 ) * SizeXI ;
			basexF = ( XNum - 1 ) * SizeXF ;
			addxI = -SizeXI ;
			addxF = -SizeXF ;
		}
		else
		{
			basexI = 0 ;
			basexF = 0 ;
			addxI = SizeXI ;
			addxF = SizeXF ;
		}
		
		// �w��̖����ɕ�������
		k = 0 ;
		for( i = 0 ; k < AllNum && i < YNum ; i ++ )
		{
			for( j = 0 ; k < AllNum && j < XNum ; j ++, k ++ )
			{
				if( Graphics_Image_DerivationGraphBase(
						HandleBuf[ k ],
						IsFloat,
						basexI + addxI * j,
						basexF + addxF * j,
						SizeYI * i,
						SizeYF * i,
						SizeXI,
						SizeXF,
						SizeYI,
						SizeYF,
						BaseHandle,
						ASyncThread ) < 0 )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xb0\x30\xe9\x30\xd5\x30\xa3\x30\xc3\x30\xaf\x30\x6e\x30\x06\x52\x72\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x43\x00\x72\x00\x65\x00\x61\x00\x74\x00\x65\x00\x44\x00\x69\x00\x76\x00\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x46\x00\x72\x00\x6f\x00\x6d\x00\x47\x00\x72\x00\x61\x00\x70\x00\x68\x00\x49\x00\x6d\x00\x61\x00\x67\x00\x65\x00\x0a\x00\x00"/*@ L"�O���t�B�b�N�̕����Ɏ��s���܂��� in CreateDivGraphFromGraphImage\n" @*/ ) ;
					return -1 ;
				}
			}
		}
		Result = 0 ;
	}
	else
	{
		// �Ȃ�̋@�\������t�����Ă��Ȃ��r�b�g�̃}�X�N���Z�b�g����
		// (NoneMask �͓r��������ꂽ�����o�ϐ��Ȃ̂ŁA���ʌ݊������������邽�߂ɁE�E�E)
		NS_SetColorDataNoneMask( ( COLORDATA * )&BaseImage->ColorData ) ;
		if( AlphaBaseImage != NULL ) NS_SetColorDataNoneMask( ( COLORDATA * )&AlphaBaseImage->ColorData ) ;

		// �T�C�Y���������G���[
		if( NS_GetGraphSize( HandleBuf[ 0 ], &GrSizeX, &GrSizeY ) == -1 ) return -1 ;
		if( BaseImage->Width < XNum * SizeXI || BaseImage->Height < YNum * SizeYI ) return -1 ;
		if( AlphaBaseImage != NULL && ( AlphaBaseImage->Width != BaseImage->Width || AlphaBaseImage->Height != BaseImage->Height ) ) return -1 ;

		LUTRANS_RE_DIV_GPARAM( GParam, BaseImage, HandleBuf, AllNum )

		// �����O���t�B�b�N���O���t�B�b�N�n���h���ɓ]������
		Result = Graphics_Image_BltBmpOrGraphImageToDivGraphBase(
				BaseImage,
				AlphaBaseImage,
				AllNum,
				XNum,
				YNum,
				IsFloat,
				SizeXI,
				SizeXF,
				SizeYI,
				SizeYF,
				HandleBuf,
				ReverseFlag,
				GParam->NotUseTransColor ? FALSE : TRUE,
				ASyncThread
		) ; 
	}

	// �I��
	return Result ;
}

// CreateGraphFromGraphImage �̓����֐�
extern int Graphics_Image_CreateGraphFromGraphImageBase( BASEIMAGE *BaseImage, const BASEIMAGE *AlphaBaseImage, int TextureFlag, int ASyncThread )
{
	CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM GParam ;
	int GrHandle ;
	int Result ;

	CheckActiveState() ;

	GrHandle = Graphics_Image_AddHandle( ASyncThread ) ;
	if( GrHandle == -1 )
	{
		return -1 ;
	}

	Graphics_Image_InitCreateGraphHandleAndBltGraphImageGParam( &GParam ) ;

	Result = Graphics_Image_CreateGraphFromGraphImageBase_UseGParam( &GParam, FALSE, GrHandle, BaseImage, AlphaBaseImage, TextureFlag, FALSE ) ;
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( GrHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD
	if( Result < 0 )
	{
		NS_DeleteGraph( GrHandle ) ;
		return -1 ;
	}

	return GrHandle ;
}

// CreateDivGraphFromGraphImage �̓����֐�
extern int Graphics_Image_CreateDivGraphFromGraphImageBase(
	BASEIMAGE *BaseImage,
	const BASEIMAGE *AlphaBaseImage,
	int AllNum,
	int XNum,
	int YNum,
	int IsFloat,
	int   SizeXI,
	float SizeXF,
	int   SizeYI,
	float SizeYF,
	int *HandleBuf,
	int TextureFlag,
	int ReverseFlag
)
{
	int BaseHandle, i ;
	CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM GParam ;

	// �����O���t�B�b�N�n���h���̒l��������
	for( i = 0 ; i < AllNum ; i ++ )
	{
		HandleBuf[ i ] = -1 ;
	}

	// ��{�O���t�B�b�N�n���h���̍쐬
	BaseHandle = Graphics_Image_AddHandle( FALSE ) ;
	if( BaseHandle == -1 )
	{
		return -1 ;
	}

	// �����O���t�B�b�N�n���h���̍쐬
	_MEMSET( HandleBuf, 0, AllNum * sizeof( int ) ) ;
	for( i = 0 ; i < AllNum ; i ++ )
	{
		HandleBuf[ i ] = Graphics_Image_AddHandle( FALSE ) ;
		if( HandleBuf[ i ] == -1 )
		{
			goto ERR ;
		}
	}

	// �������p�f�[�^�̏���
	Graphics_Image_InitCreateGraphHandleAndBltGraphImageGParam( &GParam ) ;

	// �����O���t�B�b�N�n���h���̍쐬
	if( Graphics_Image_CreateDivGraphFromGraphImageBase_UseGParam( &GParam, FALSE, BaseHandle, BaseImage, AlphaBaseImage, AllNum, XNum, YNum, IsFloat, SizeXI, SizeXF, SizeYI, SizeYF, HandleBuf, TextureFlag, ReverseFlag, FALSE ) < 0 )
		goto ERR ;
	
	// ���ƂȂ����n���h�������
	NS_DeleteGraph( BaseHandle ) ;

	// �I��
	return 0 ;
	
ERR :
	for( i = 0 ; i < AllNum ; i ++ )
	{
		NS_DeleteGraph( HandleBuf[ i ] ) ;
	}
	NS_DeleteGraph( BaseHandle ) ;

	// �G���[�I��	
	return -1 ;
}

// ReCreateGraphFromGraphImage �̓����֐�
extern int Graphics_Image_ReCreateGraphFromGraphImageBase( BASEIMAGE *BaseImage, const BASEIMAGE *AlphaBaseImage, int GrHandle, int TextureFlag )
{
	CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM GParam ;
	int Result ;

	CheckActiveState() ;

	Graphics_Image_InitCreateGraphHandleAndBltGraphImageGParam( &GParam ) ;

	Result = Graphics_Image_CreateGraphFromGraphImageBase_UseGParam( &GParam, TRUE, GrHandle, BaseImage, AlphaBaseImage, TextureFlag, FALSE ) ;

	return Result ;
}

// ReCreateDivGraphFromGraphImage �̓����֐�
extern int Graphics_Image_ReCreateDivGraphFromGraphImageBase( BASEIMAGE *BaseImage, const BASEIMAGE *AlphaBaseImage, int AllNum, int XNum, int YNum, int IsFloat, int SizeXI, float SizeXF, int SizeYI, float SizeYF, const int *HandleBuf, int TextureFlag, int ReverseFlag )
{
	CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM GParam ;
	int Result ;

	CheckActiveState() ;

	Graphics_Image_InitCreateGraphHandleAndBltGraphImageGParam( &GParam ) ;

	Result = Graphics_Image_CreateDivGraphFromGraphImageBase_UseGParam( &GParam, TRUE, -1, BaseImage, AlphaBaseImage, AllNum, XNum, YNum, IsFloat, SizeXI, SizeXF, SizeYI, SizeYF, ( int * )HandleBuf, TextureFlag, ReverseFlag, FALSE ) ;

	// �I��
	return Result ;
}

// CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM �̃f�[�^���Z�b�g����
extern void Graphics_Image_InitCreateGraphHandleAndBltGraphImageGParam( CREATE_GRAPHHANDLE_AND_BLTGRAPHIMAGE_GPARAM *GParam )
{
	Graphics_Image_InitSetupGraphHandleGParam( &GParam->InitHandleGParam ) ;

	GParam->UseTransColorGraphCreateFlag	= GSYS.CreateImage.UseTransColorFlag ;
	GParam->NotUseTransColor				= GSYS.CreateImage.NotUseTransColor ;
	GParam->LeftUpColorIsTransColorFlag		= GSYS.CreateImage.LeftUpColorIsTransColorFlag ;
}

// SETUP_GRAPHHANDLE_GPARAM �̃f�[�^���Z�b�g����
extern void Graphics_Image_InitSetupGraphHandleGParam( SETUP_GRAPHHANDLE_GPARAM *GParam )
{
	GParam->TransColor							= GSYS.CreateImage.TransColor ;

	GParam->CreateImageColorBitDepth			= GSYS.CreateImage.ColorBitDepth ;
	GParam->FloatTypeGraphCreateFlag			= FALSE ;
	GParam->CreateImageChannelNum				= 0 ;
	GParam->CreateImageChannelBitDepth			= GSYS.CreateImage.ChannelBitDepth ;
	GParam->AlphaTestImageCreateFlag			= GSYS.CreateImage.AlphaTestFlag ;
	GParam->AlphaChannelImageCreateFlag			= GSYS.CreateImage.AlphaChannelFlag ;
	GParam->CubeMapTextureCreateFlag			= GSYS.CreateImage.CubeMapFlag ;
	GParam->BlendImageCreateFlag				= GSYS.CreateImage.BlendImageFlag ;
	GParam->UseManagedTextureFlag				= GSYS.CreateImage.NotUseManagedTextureFlag ? FALSE : TRUE ;
	GParam->UseLinearMapTextureFlag				= FALSE ;
	GParam->PlatformTextureFormat				= GSYS.CreateImage.PlatformTextureFormat ;

	GParam->DrawValidImageCreateFlag			= GSYS.CreateImage.DrawValidFlag ;
	GParam->DrawValidAlphaImageCreateFlag		= GSYS.CreateImage.DrawValidAlphaFlag ;
	GParam->DrawValidFloatTypeGraphCreateFlag	= GSYS.CreateImage.DrawValidFloatTypeFlag ;
	GParam->DrawValidGraphCreateZBufferFlag		= GSYS.CreateImage.NotDrawValidCreateZBufferFlag ? FALSE : TRUE ;
	GParam->CreateDrawValidGraphZBufferBitDepth	= GSYS.CreateImage.DrawValidZBufferBitDepth ;
	GParam->CreateDrawValidGraphChannelNum		= GSYS.CreateImage.DrawValidChannelNum ;
	GParam->DrawValidMSSamples					= GSYS.CreateImage.DrawValidMSSamples ;
	GParam->DrawValidMSQuality					= GSYS.CreateImage.DrawValidMSQuality ;

	GParam->MipMapCount							= GSYS.CreateImage.MipMapCount ;
	GParam->UserMaxTextureSize					= GSYS.CreateImage.UserMaxTextureSize ;
	GParam->NotUseDivFlag						= GSYS.CreateImage.NotUseDivFlag ;
	GParam->NotUseAlphaImageLoadFlag			= GSYS.CreateImage.NotUseAlphaImageLoadFlag ;
	GParam->NotInitGraphDelete					= FALSE ;
}

extern void Graphics_Image_InitSetupGraphHandleGParam_Normal_NonDrawValid( SETUP_GRAPHHANDLE_GPARAM *GParam, int BitDepth, int AlphaChannel, int AlphaTest )
{
	GParam->TransColor							= 0x00ff00ff ;

	GParam->CreateImageColorBitDepth			= BitDepth ;
	GParam->FloatTypeGraphCreateFlag			= FALSE ;
	GParam->CreateImageChannelNum				= 0 ;
	GParam->CreateImageChannelBitDepth			= 0 ;
	GParam->AlphaTestImageCreateFlag			= AlphaTest ;
	GParam->AlphaChannelImageCreateFlag			= AlphaChannel ;
	GParam->CubeMapTextureCreateFlag			= FALSE ;
	GParam->BlendImageCreateFlag				= FALSE ;
	GParam->UseManagedTextureFlag				= TRUE ;
	GParam->UseLinearMapTextureFlag				= FALSE ;
	GParam->PlatformTextureFormat				= 0 ;

	GParam->DrawValidImageCreateFlag			= FALSE ;
	GParam->DrawValidAlphaImageCreateFlag		= FALSE ;
	GParam->DrawValidFloatTypeGraphCreateFlag	= FALSE ;
	GParam->DrawValidGraphCreateZBufferFlag		= FALSE ;
	GParam->CreateDrawValidGraphZBufferBitDepth	= 0 ;
	GParam->CreateDrawValidGraphChannelNum		= 0 ;
	GParam->DrawValidMSSamples					= 0 ;
	GParam->DrawValidMSQuality					= 0 ;

	GParam->MipMapCount							= 1 ;
	GParam->UserMaxTextureSize					= 0 ;
	GParam->NotUseDivFlag						= TRUE ;
	GParam->NotUseAlphaImageLoadFlag			= TRUE ;
	GParam->NotInitGraphDelete					= FALSE ;
}

extern void Graphics_Image_InitSetupGraphHandleGParam_Normal_DrawValid_NoneZBuffer( SETUP_GRAPHHANDLE_GPARAM *GParam, int BitDepth, int AlphaChannel )
{
	GParam->TransColor							= 0x00ff00ff ;

	GParam->CreateImageColorBitDepth			= BitDepth ;
	GParam->FloatTypeGraphCreateFlag			= FALSE ;
	GParam->CreateImageChannelNum				= 0 ;
	GParam->CreateImageChannelBitDepth			= 0 ;
	GParam->AlphaTestImageCreateFlag			= FALSE ;
	GParam->AlphaChannelImageCreateFlag			= AlphaChannel ;
	GParam->CubeMapTextureCreateFlag			= FALSE ;
	GParam->BlendImageCreateFlag				= FALSE ;
	GParam->UseManagedTextureFlag				= FALSE ;
	GParam->UseLinearMapTextureFlag				= FALSE ;
	GParam->PlatformTextureFormat				= 0 ;

	GParam->DrawValidImageCreateFlag			= TRUE ;
	GParam->DrawValidAlphaImageCreateFlag		= AlphaChannel ;
	GParam->DrawValidFloatTypeGraphCreateFlag	= FALSE ;
	GParam->DrawValidGraphCreateZBufferFlag		= FALSE ;
	GParam->CreateDrawValidGraphZBufferBitDepth	= 0 ;
	GParam->CreateDrawValidGraphChannelNum		= 0 ;
	GParam->DrawValidMSSamples					= 0 ;
	GParam->DrawValidMSQuality					= 0 ;

	GParam->MipMapCount							= 1 ;
	GParam->UserMaxTextureSize					= 0 ;
	GParam->NotUseDivFlag						= TRUE ;
	GParam->NotUseAlphaImageLoadFlag			= TRUE ;
	GParam->NotInitGraphDelete					= FALSE ;
}

// SETGRAPHBASEINFO_GPARAM �̃f�[�^���Z�b�g����
extern void Graphics_Image_InitSetGraphBaseInfoGParam( SETGRAPHBASEINFO_GPARAM *GParam )
{
	GParam->NotGraphBaseDataBackupFlag = GSYS.CreateImage.NotGraphBaseDataBackupFlag ;
}

// LOADGRAPH_GPARAM �̃f�[�^���Z�b�g����
extern void Graphics_Image_InitLoadGraphGParam( LOADGRAPH_GPARAM *GParam )
{
	InitLoadBaseImageGParam( &GParam->LoadBaseImageGParam, FALSE ) ;
#ifndef DX_NON_MOVIE
	Graphics_Image_InitOpenMovieGParam( &GParam->OpenMovieGParam ) ;
#endif
	Graphics_Image_InitCreateGraphHandleAndBltGraphImageGParam( &GParam->CreateGraphGParam ) ;
	Graphics_Image_InitSetGraphBaseInfoGParam( &GParam->SetGraphBaseInfoGParam ) ;
}























// �`��ݒ�֌W�֐�

// �O���t�B�b�N�`��ݒ�֌W�̏���������
extern	int		Graphics_DrawSetting_Initialize( void )
{
	int i ;

//	int NotDrawFlag                            = GSYS.NotDrawFlag ;
//	int NotUseBasicGraphDraw3DDeviceMethodFlag = GSYS.DrawSetting.NotUseBasicGraphDraw3DDeviceMethodFlag ;
//	int PreSetWaitVSyncFlag                    = GSYS.Screen.PreSetWaitVSyncFlag ;
//	int NotGraphBaseDataBackupFlag             = GSYS.CreateImage.NotGraphBaseDataBackupFlag ;
//	int NotUseManagedTextureFlag               = GSYS.CreateImage.NotUseManagedTextureFlag ;
	RECT OriginalDrawRect                      = GSYS.DrawSetting.OriginalDrawRect ;
	int Large3DPositionSupport                 = GSYS.DrawSetting.Large3DPositionSupport ;

	// �[��������
	_MEMSET( &GSYS.DrawSetting, 0, sizeof( GSYS.DrawSetting ) ) ;
	_MEMSET( &GSYS.Camera,      0, sizeof( GSYS.Camera      ) ) ;
//	_MEMSET( &GSYS.CreateImage, 0, sizeof( GSYS.CreateImage ) ) ;

//	GSYS.NotDrawFlag						     = NotDrawFlag ;
//	GSYS.DrawSetting.NotUseBasicGraphDraw3DDeviceMethodFlag = NotUseBasicGraphDraw3DDeviceMethodFlag ;
//	GSYS.Screen.PreSetWaitVSyncFlag             = PreSetWaitVSyncFlag ;
//	GSYS.CreateImage.NotGraphBaseDataBackupFlag = NotGraphBaseDataBackupFlag ;
//	GSYS.CreateImage.NotUseManagedTextureFlag   = NotUseManagedTextureFlag ;
	GSYS.DrawSetting.OriginalDrawRect           = OriginalDrawRect ;
	GSYS.DrawSetting.Large3DPositionSupport     = Large3DPositionSupport ;

	// �e�N�X�`���A�h���X���[�h�̏����ݒ�
	for( i = 0 ; i < USE_TEXTURESTAGE_NUM ; i ++ )
	{
		if( GSYS.DrawSetting.TexAddressModeU[ i ] == 0 )
			GSYS.DrawSetting.TexAddressModeU[ i ] = DX_TEXADDRESS_CLAMP ;
		if( GSYS.DrawSetting.TexAddressModeV[ i ] == 0 )
			GSYS.DrawSetting.TexAddressModeV[ i ] = DX_TEXADDRESS_CLAMP ;
		if( GSYS.DrawSetting.TexAddressModeW[ i ] == 0 )
			GSYS.DrawSetting.TexAddressModeW[ i ] = DX_TEXADDRESS_CLAMP ;
	}

	// �u�����h�摜�͐ݒ肳��Ă��Ȃ���Ԃɂ���
	GSYS.DrawSetting.BlendGraph = -1 ;

	// �y�o�b�t�@�̏����ݒ�
	GSYS.DrawSetting.EnableZBufferFlag2D = FALSE ;
	GSYS.DrawSetting.WriteZBufferFlag2D  = FALSE ;
	GSYS.DrawSetting.EnableZBufferFlag3D = FALSE ;
	GSYS.DrawSetting.WriteZBufferFlag3D  = FALSE ;

	// ���C�g�̐ݒ���n�[�h�E�G�A�A�N�Z�����[�^�ɔ��f����悤�Ƀt���O���Z�b�g
	GSYS.Light.HardwareChange = TRUE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �`��y�l���Z�b�g
	GSYS.DrawSetting.DrawZ = 0.2f;

	// �����y��r���[�h���Z�b�g
	GSYS.DrawSetting.ZBufferCmpType2D = DX_CMP_LESSEQUAL;
	GSYS.DrawSetting.ZBufferCmpType3D = DX_CMP_LESSEQUAL;

	// �����̓e�N�X�`���T�[�t�F�X���쐬���������
//	GSYS.TextureImageCreateFlag = TRUE ;

	// ���ߐF�ɃA���t�@�e�X�g���g�p����
	GSYS.CreateImage.AlphaTestFlag = TRUE ;
	BASEIM.AlphaTestImageCreateFlag = TRUE ;

	// �`�惂�[�h���l�A���X�g�l�C�o�[�ɃZ�b�g
	GSYS.DrawSetting.DrawMode = DX_DRAWMODE_NEAREST ;

	// �ő�ٕ������P�ɃZ�b�g
	GSYS.DrawSetting.MaxAnisotropy = 1 ;

	// �t�B�����[�h�̓|���S���`��
	GSYS.DrawSetting.FillMode = DX_FILL_SOLID ;

	// �u�����h���[�h�����u�����h�ɃZ�b�g
	GSYS.DrawSetting.BlendMode = DX_BLENDMODE_NOBLEND ;

	// �A���t�@�e�X�g���[�h�̓f�t�H���g����
	GSYS.DrawSetting.AlphaTestMode = -1 ;

	// �`��P�x���Z�b�g
	NS_SetDrawBright( 255, 255, 255 ) ;
//	GSYS.DrawSetting.bDrawBright = 0xffffff ;

	// �t�H�O�̏����ݒ�
	NS_SetFogMode( DX_FOGMODE_LINEAR ) ;
	NS_SetFogColor( 255, 255, 255 ) ;
	NS_SetFogStartEnd( 0.0f, 1500.0f ) ;

	// �g�p����s�N�Z���t�H�[�}�b�g���Z�b�g
//	SetCreateGraphColorBitDepth( COLORBITDEPTH ) ;
	NS_SetCreateGraphColorBitDepth( 0 ) ;

#ifndef DX_NON_MASK
	// �}�X�N�͎g�p����ɃZ�b�g
	MASKD.MaskUseFlag = TRUE ;
	MASKD.MaskValidFlag = FALSE ;
#endif

	// �`���X�N���[���̈���Z�b�g
	GSYS.DrawSetting.TargetScreen[ 0 ] = DX_SCREEN_BACK ;
	GSYS.DrawSetting.TargetScreenSurface[ 0 ] = 0 ;
	NS_SetDrawScreen( DX_SCREEN_FRONT ) ;

	// �`��\�̈��ݒ�
	{
		int Width, Height ;

		NS_GetGraphSize( GSYS.DrawSetting.TargetScreen[ 0 ], &Width, &Height ) ;
		NS_SetDrawArea( 0, 0, Width, Height ) ;
	}

	// �W�������u�����h�ɂ���
//	NS_SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 ) ;
	
	// �`����\�ȏ�Ԃɂ���
//	GSYS.NotDrawFlag = FALSE ;

	// �f�t�H���g�̃��C�g���쐬
	{
		VECTOR Dir ;

		Dir.x =  1.0f ;
		Dir.y = -1.0f ;
		Dir.z =  1.0f ;
		GSYS.Light.DefaultHandle = NS_CreateDirLightHandle( Dir ) ;
	}

	// �e��s��ɒP�ʍs��̃Z�b�g
	CreateIdentityMatrixD( &GSYS.DrawSetting.WorldMatrix ) ;
	CreateIdentityMatrix(  &GSYS.DrawSetting.WorldMatrixF ) ;
	CreateIdentityMatrixD( &GSYS.DrawSetting.ViewMatrix ) ;
	CreateIdentityMatrix(  &GSYS.DrawSetting.ViewMatrixF ) ;
	CreateIdentityMatrixD( &GSYS.DrawSetting.ProjectionMatrix ) ;
	CreateIdentityMatrix(  &GSYS.DrawSetting.ProjectionMatrixF ) ;
	CreateIdentityMatrixD( &GSYS.DrawSetting.Direct3DViewportMatrix ) ;
	CreateIdentityMatrix(  &GSYS.DrawSetting.Direct3DViewportMatrixF ) ;
	CreateIdentityMatrixD( &GSYS.DrawSetting.Direct3DViewportMatrixAnti ) ;
	CreateIdentityMatrix(  &GSYS.DrawSetting.Direct3DViewportMatrixAntiF ) ;
	CreateIdentityMatrixD( &GSYS.DrawSetting.ViewportMatrix ) ;
	CreateIdentityMatrix(  &GSYS.DrawSetting.ViewportMatrixF ) ;
	CreateIdentityMatrixD( &GSYS.DrawSetting.BlendMatrix ) ;
	CreateIdentityMatrix(  &GSYS.DrawSetting.BlendMatrixF ) ;
	CreateIdentityMatrixD( &GSYS.DrawSetting.InverseBlendMatrix ) ;
	CreateIdentityMatrix(  &GSYS.DrawSetting.InverseBlendMatrixF ) ;
	CreateIdentityMatrixD( &GSYS.DrawSetting.WorldViewMatrix ) ;
	CreateIdentityMatrixD( &GSYS.DrawSetting.ViewProjectionViewportMatrix ) ;
	CreateIdentityMatrixD( &GSYS.DrawSetting.BillboardMatrix ) ;
	CreateIdentityMatrix(  &GSYS.DrawSetting.BillboardMatrixF ) ;
	CreateIdentityMatrixD( &GSYS.DrawSetting.ConvScreenPosToWorldPosMatrix ) ;
	CreateIdentityMatrix(  &GSYS.DrawSetting.TextureAddressTransformMatrix ) ;

	// �ˉe�s��̐ݒ��������
	GSYS.DrawSetting.ProjectionMatrixMode = 0 ;
	GSYS.DrawSetting.ProjNear             = 10.0 ;
	GSYS.DrawSetting.ProjFar              = 10000.0 ;
	GSYS.DrawSetting.ProjDotAspect        = 1.0 ;
	GSYS.DrawSetting.ProjFov              = DEFAULT_FOV ;
	GSYS.DrawSetting.ProjSize             = 1.0 ;
	CreateIdentityMatrixD( &GSYS.DrawSetting.ProjMatrix ) ;

	// �}�e���A���̏����l���Z�b�g
	GSYS.Light.ChangeMaterial    = 1 ;
	GSYS.Light.Material.Diffuse  = GetColorF( 0.8f, 0.8f, 0.8f, 1.0f ) ;
	GSYS.Light.Material.Specular = GetColorF( 0.8f, 0.8f, 0.8f, 0.0f ) ;
	GSYS.Light.Material.Ambient  = GetColorF( 0.0f, 0.0f, 0.0f, 0.0f ) ;
	GSYS.Light.Material.Emissive = GetColorF( 0.0f, 0.0f, 0.0f, 0.0f ) ;
	GSYS.Light.Material.Power    = 20.0f ;

	// �R�c�`��X�P�[���l��������
	GSYS.DrawSetting.Draw3DScale = 1.0f ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �I��
	return 0 ;
}

// �⏕�֐�
extern void FASTCALL Graphics_DrawSetting_BlendModeSub_Pre( RECT *DrawRect )
{
	DWORD Bright ;
	DWORD Cr ;
	int   BlendParam ;

	Cr         = NS_GetColor( 255,255,255 ) ;
	Bright     = GSYS.DrawSetting.bDrawBright ;
	BlendParam = GSYS.DrawSetting.BlendParam ;

	Graphics_DrawSetting_SetDrawBrightToOneParam( 0xffffff ) ;
	NS_SetDrawBlendMode( DX_BLENDMODE_SUB, 255 ) ;

	Graphics_Hardware_DrawFillBox_PF( DrawRect->left, DrawRect->top, DrawRect->right, DrawRect->bottom, Cr ) ;

	Graphics_DrawSetting_SetDrawBrightToOneParam( Bright ) ;
	NS_SetDrawBlendMode( DX_BLENDMODE_SUB2, BlendParam ) ;
}

extern void FASTCALL Graphics_DrawSetting_BlendModeSub_Post( RECT *DrawRect )
{
	DWORD Bright ;
	DWORD Cr ;
	int   BlendParam ;

	Cr         = NS_GetColor( 255,255,255 ) ;
	Bright     = GSYS.DrawSetting.bDrawBright ;
	BlendParam = GSYS.DrawSetting.BlendParam ;

	Graphics_DrawSetting_SetDrawBrightToOneParam( 0xffffff ) ;
	NS_SetDrawBlendMode( DX_BLENDMODE_SUB, 255 ) ; 
	
	Graphics_Hardware_DrawFillBox_PF( DrawRect->left, DrawRect->top, DrawRect->right, DrawRect->bottom, Cr ) ;

	Graphics_DrawSetting_SetDrawBrightToOneParam( Bright ) ;
	NS_SetDrawBlendMode( DX_BLENDMODE_SUB, BlendParam ) ;
}

// SetBlendGraphParam �̉ϒ������p�����[�^�t��
extern int Graphics_DrawSetting_SetBlendGraphParamBase( int BlendGraph, int BlendType, va_list ParamList )
{
	IMAGEDATA *BlendImage ;
	int Param[ 16 ] ;

	// �u�����h����摜�� -1 ��������u�����h�摜�������~�߂�Ƃ�������
	if( BlendGraph <= 0 )
	{
		BlendImage = NULL ;
		GSYS.DrawSetting.BlendGraph = -1 ;
	}
	else
	{
		// �G���[����
		if( GRAPHCHK( BlendGraph, BlendImage ) )
			return -1 ;
		GSYS.DrawSetting.BlendGraph = BlendGraph ;
	}

	CheckActiveState() ;

	if( BlendImage != NULL )
	{
		switch( BlendType )
		{
		case DX_BLENDGRAPHTYPE_NORMAL :
			int FadeRatio ;

			FadeRatio = va_arg( ParamList, int ) ;

			if( FadeRatio > 255 ) FadeRatio = 255 ;
			else
			if( FadeRatio < 0   ) FadeRatio = 0   ;

			Param[ 0 ] = FadeRatio ;
			break ;

		case DX_BLENDGRAPHTYPE_WIPE :
			int BorderRange ;
			int BorderParam ;

			BorderParam = va_arg( ParamList, int ) ;
			BorderRange = va_arg( ParamList, int ) ;

			if( BorderRange > 255 ) BorderRange = 255 ;
			else
			if( BorderRange < 0   ) BorderRange = 0   ;

			if( BorderParam > 255 ) BorderParam = 255 ;
			else
			if( BorderParam < 0	  ) BorderParam = 0   ;
			
			Param[ 0 ] = GSYS.DrawSetting.BlendGraphBorderParam = BorderParam ;
			Param[ 1 ] = GSYS.DrawSetting.BlendGraphBorderRange = BorderRange ;

			// Direct3D ���g�p���Ă��Ȃ��ꍇ�̂ݏ����𕪊�
			if( GSYS.Setting.ValidHardware == FALSE )
			{
				// MEMIMG �V�X�e���ւ̐ݒ�
				SetBlendGraphParamMemImg( BorderParam, BorderRange ) ;
			}
			break ;

		case DX_BLENDGRAPHTYPE_ALPHA :
			break ;
		}
	}

	// �n�[�h�E�G�A�A�N�Z�����[�V�������g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( GSYS.Setting.ValidHardware )
	{
		// �n�[�h�E�G�A�A�N�Z�����[�V�������g�p���Ă���ꍇ�̏���
		Graphics_Hardware_SetBlendGraphParamBase_PF( BlendImage, BlendType, Param ) ;
	}
	else
	{
		// �n�[�h�E�G�A�A�N�Z�����[�V�������g�p���Ă��Ȃ��ꍇ�̏���

		// �u�����h�O���t�B�b�N�̃A�h���X��ݒ�
		GSYS.SoftRender.BlendMemImg = GSYS.DrawSetting.BlendGraph <= 0 ? NULL : &BlendImage->Orig->Soft.MemImg ;
	}

	// �I��
	return 0 ;
}

// �`���ɐ��������l���������ނ��ǂ����̃t���O���X�V����
extern	int Graphics_DrawSetting_RefreshAlphaChDrawMode( void )
{
	return Graphics_Hardware_RefreshAlphaChDrawMode_PF() ;
}

// ��{�f�[�^�ɐݒ肳��Ă���s����n�[�h�E�G�A�ɔ��f����
extern void Graphics_DrawSetting_ApplyLibMatrixToHardware()
{
	if( GSYS.DrawSetting.MatchHardwareMatrix == TRUE ) return ;

	if( GSYS.DrawSetting.MatchHardwareWorldMatrix == FALSE )
	{
		Graphics_Hardware_DeviceDirect_SetWorldMatrix_PF( &GSYS.DrawSetting.WorldMatrixF ) ;
		GSYS.DrawSetting.MatchHardwareWorldMatrix = TRUE ;
	}

	if( GSYS.DrawSetting.MatchHardwareViewMatrix == FALSE )
	{
		Graphics_Hardware_DeviceDirect_SetViewMatrix_PF( &GSYS.DrawSetting.ViewMatrixF ) ;
		GSYS.DrawSetting.MatchHardwareViewMatrix = TRUE ;
	}

	if( GSYS.DrawSetting.MatchHardwareProjectionMatrix == FALSE )
	{
		Graphics_Hardware_DeviceDirect_SetProjectionMatrix_PF( &GSYS.DrawSetting.ProjectionMatrixF ) ;
		GSYS.DrawSetting.MatchHardwareProjectionMatrix = TRUE ;
	}

	if( GSYS.DrawSetting.MatchHardwareTextureAddressTransformMatrix == FALSE )
	{
		Graphics_Hardware_SetTextureAddressTransformMatrix_PF( GSYS.DrawSetting.TextureAddressTransformUse, &GSYS.DrawSetting.TextureAddressTransformMatrix ) ;
		GSYS.DrawSetting.MatchHardwareTextureAddressTransformMatrix = TRUE ;
	}

	GSYS.DrawSetting.MatchHardwareMatrix = TRUE ;
}

// ��{�f�[�^�ɐݒ肳��Ă���t�H�O�����n�[�h�E�F�A�ɔ��f����
extern void Graphics_DrawSetting_ApplyLibFogToHardware( void )
{
	Graphics_Hardware_ApplyLigFogToHardware_PF() ;
}

// �e�N�X�`�����W�ϊ��s����Z�b�g����
extern int Graphics_DrawSetting_SetTextureAddressTransformMatrix_Direct( int Use, MATRIX *Matrix, int Stage )
{
	Graphics_Hardware_SetTextureAddressTransformMatrix_PF( Use, Matrix, Stage ) ;
	GSYS.DrawSetting.MatchHardwareTextureAddressTransformMatrix = FALSE ;
	GSYS.DrawSetting.MatchHardwareMatrix = FALSE ;

	// �I��
	return 0 ;
}

// ���[�J�������[���h�s���ύX����
extern int Graphics_DrawSetting_SetTransformToWorld_Direct( MATRIX *Matrix )
{
	Graphics_Hardware_DeviceDirect_SetWorldMatrix_PF( Matrix ) ;
	GSYS.DrawSetting.MatchHardwareWorldMatrix = FALSE ;
	GSYS.DrawSetting.MatchHardwareMatrix = FALSE ;

	// �I��
	return 0 ;
}

// ���e�ϊ��p�s����Z�b�g����
extern	int Graphics_DrawSetting_SetTransformToProjection_Direct( const MATRIX_D *Matrix )
{
	// �`��ҋ@���Ă���`�敨��`��
	DRAWSTOCKINFO

	// �f�[�^��ۑ�
	GSYS.DrawSetting.ProjectionMatrix = *Matrix ;
	ConvertMatrixDtoF( &GSYS.DrawSetting.ProjectionMatrixF, &GSYS.DrawSetting.ProjectionMatrix ) ;
	GSYS.DrawSetting.ValidBlendMatrix = FALSE ;

	// �S�Ă̍s����|�����킹���s����X�V
	Graphics_DrawSetting_RefreshBlendTransformMatrix() ;

	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
	if( GSYS.Setting.ValidHardware )
	{
		Graphics_Hardware_SetTransformToProjection_PF( &GSYS.DrawSetting.ProjectionMatrixF ) ;

		GSYS.DrawSetting.MatchHardwareProjectionMatrix = TRUE ;
	}

	// �I��
	return 0 ;
}

// �ˉe�s��p�����[�^�Ɋ�Â��Ďˉe�s����\�z����
extern void Graphics_DrawSetting_RefreshProjectionMatrix( void )
{
	double Aspect = 1.0 ;

	if( GSYS.DrawSetting.ProjectionMatrixMode != 2 )
	{
		Aspect = ( double )GSYS.DrawSetting.DrawSizeY / ( double )GSYS.DrawSetting.DrawSizeX * GSYS.DrawSetting.ProjDotAspect ;
	}

	switch( GSYS.DrawSetting.ProjectionMatrixMode )
	{
	case 0 :	// ���ߖ@
		CreatePerspectiveFovMatrixD( &GSYS.DrawSetting.ProjMatrix, GSYS.DrawSetting.ProjFov, GSYS.DrawSetting.ProjNear, GSYS.DrawSetting.ProjFar, Aspect ) ;
		break ;

	case 1 :	// ���ˉe
		CreateOrthoMatrixD( &GSYS.DrawSetting.ProjMatrix, GSYS.DrawSetting.ProjSize, GSYS.DrawSetting.ProjNear, GSYS.DrawSetting.ProjFar, Aspect ) ;
		break ;

	case 2 :	// �s�񒼎w��
		break ;
	}

	// �n�[�h�E�G�A�ɔ��f����
	Graphics_DrawSetting_SetTransformToProjection_Direct( &GSYS.DrawSetting.ProjMatrix ) ;
}

// ���_�ϊ��s����|�����킹���s����X�V����
extern void Graphics_DrawSetting_RefreshBlendTransformMatrix( void )
{
	if( GSYS.DrawSetting.ValidBlendMatrix == TRUE ) return ;

	CreateMultiplyMatrixD( &GSYS.DrawSetting.WorldViewMatrix, &GSYS.DrawSetting.WorldMatrix, &GSYS.DrawSetting.ViewMatrix ) ;
	CreateMultiplyMatrixD( &GSYS.DrawSetting.ViewProjectionViewportMatrix, &GSYS.DrawSetting.ViewMatrix, &GSYS.DrawSetting.ProjectionMatrix ) ;
	CreateMultiplyMatrixD( &GSYS.DrawSetting.ViewProjectionViewportMatrix, &GSYS.DrawSetting.ViewProjectionViewportMatrix, &GSYS.DrawSetting.ViewportMatrix ) ;
	CreateMultiplyMatrixD( &GSYS.DrawSetting.BlendMatrix, &GSYS.DrawSetting.WorldMatrix, &GSYS.DrawSetting.ViewProjectionViewportMatrix ) ;
	ConvertMatrixDtoF( &GSYS.DrawSetting.BlendMatrixF, &GSYS.DrawSetting.BlendMatrix ) ;

	GSYS.DrawSetting.BillboardMatrix.m[0][0] = GSYS.DrawSetting.ViewMatrix.m[0][0] ;
	GSYS.DrawSetting.BillboardMatrix.m[0][1] = GSYS.DrawSetting.ViewMatrix.m[1][0] ;
	GSYS.DrawSetting.BillboardMatrix.m[0][2] = GSYS.DrawSetting.ViewMatrix.m[2][0] ;
	GSYS.DrawSetting.BillboardMatrix.m[0][3] = 0.0F ;

	GSYS.DrawSetting.BillboardMatrix.m[1][0] = GSYS.DrawSetting.ViewMatrix.m[0][1] ;
	GSYS.DrawSetting.BillboardMatrix.m[1][1] = GSYS.DrawSetting.ViewMatrix.m[1][1] ;
	GSYS.DrawSetting.BillboardMatrix.m[1][2] = GSYS.DrawSetting.ViewMatrix.m[2][1] ;
	GSYS.DrawSetting.BillboardMatrix.m[1][3] = 0.0F ;

	GSYS.DrawSetting.BillboardMatrix.m[2][0] = GSYS.DrawSetting.ViewMatrix.m[0][2] ;
	GSYS.DrawSetting.BillboardMatrix.m[2][1] = GSYS.DrawSetting.ViewMatrix.m[1][2] ;
	GSYS.DrawSetting.BillboardMatrix.m[2][2] = GSYS.DrawSetting.ViewMatrix.m[2][2] ;
	GSYS.DrawSetting.BillboardMatrix.m[2][3] = 0.0F ;

	GSYS.DrawSetting.BillboardMatrix.m[3][0] = 
	GSYS.DrawSetting.BillboardMatrix.m[3][1] = 
	GSYS.DrawSetting.BillboardMatrix.m[3][2] = 
	GSYS.DrawSetting.BillboardMatrix.m[3][3] = 0.0F ;

	ConvertMatrixDtoF( &GSYS.DrawSetting.BillboardMatrixF, &GSYS.DrawSetting.BillboardMatrix ) ;

	GSYS.DrawSetting.ValidInverseBlendMatrix            = FALSE ;
	GSYS.DrawSetting.ValidBlendMatrix                   = TRUE ;
	GSYS.DrawSetting.ValidConvScreenPosToWorldPosMatrix = FALSE ;

	if( GSYS.DrawSetting.DrawArea.left == GSYS.DrawSetting.DrawArea.right ||
		GSYS.DrawSetting.DrawArea.top  == GSYS.DrawSetting.DrawArea.bottom )
	{
		_MEMSET( GSYS.DrawSetting.ViewClipPos,   0, sizeof( GSYS.DrawSetting.ViewClipPos   ) ) ;
		_MEMSET( GSYS.DrawSetting.ClippingPlane, 0, sizeof( GSYS.DrawSetting.ClippingPlane ) ) ;
	}
	else
	{
		GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 0 ] = NS_ConvScreenPosToWorldPosD( VGetD( GSYS.DrawSetting.DrawAreaF.left,  GSYS.DrawSetting.DrawAreaF.top,    0.0 ) ) ;
		GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 1 ] = NS_ConvScreenPosToWorldPosD( VGetD( GSYS.DrawSetting.DrawAreaF.right, GSYS.DrawSetting.DrawAreaF.top,    0.0 ) ) ;
		GSYS.DrawSetting.ViewClipPos[ 0 ][ 1 ][ 0 ] = NS_ConvScreenPosToWorldPosD( VGetD( GSYS.DrawSetting.DrawAreaF.left,  GSYS.DrawSetting.DrawAreaF.bottom, 0.0 ) ) ;
		GSYS.DrawSetting.ViewClipPos[ 0 ][ 1 ][ 1 ] = NS_ConvScreenPosToWorldPosD( VGetD( GSYS.DrawSetting.DrawAreaF.right, GSYS.DrawSetting.DrawAreaF.bottom, 0.0 ) ) ;
		GSYS.DrawSetting.ViewClipPos[ 1 ][ 0 ][ 0 ] = NS_ConvScreenPosToWorldPosD( VGetD( GSYS.DrawSetting.DrawAreaF.left,  GSYS.DrawSetting.DrawAreaF.top,    1.0 ) ) ;
		GSYS.DrawSetting.ViewClipPos[ 1 ][ 0 ][ 1 ] = NS_ConvScreenPosToWorldPosD( VGetD( GSYS.DrawSetting.DrawAreaF.right, GSYS.DrawSetting.DrawAreaF.top,    1.0 ) ) ;
		GSYS.DrawSetting.ViewClipPos[ 1 ][ 1 ][ 0 ] = NS_ConvScreenPosToWorldPosD( VGetD( GSYS.DrawSetting.DrawAreaF.left,  GSYS.DrawSetting.DrawAreaF.bottom, 1.0 ) ) ;
		GSYS.DrawSetting.ViewClipPos[ 1 ][ 1 ][ 1 ] = NS_ConvScreenPosToWorldPosD( VGetD( GSYS.DrawSetting.DrawAreaF.right, GSYS.DrawSetting.DrawAreaF.bottom, 1.0 ) ) ;

		VECTOR_D Normal ;

		// �w�y����
		Normal = VCrossD( VSubD( GSYS.DrawSetting.ViewClipPos[ 1 ][ 0 ][ 0 ], GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 0 ] ), VSubD( GSYS.DrawSetting.ViewClipPos[ 0 ][ 1 ][ 0 ], GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 0 ] ) ) ;
		CreateNormalizePlaneD( &GSYS.DrawSetting.ClippingPlane[ 0 ], &GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 0 ], &Normal ) ;

		Normal = VCrossD( VSubD( GSYS.DrawSetting.ViewClipPos[ 0 ][ 1 ][ 1 ], GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 1 ] ), VSubD( GSYS.DrawSetting.ViewClipPos[ 1 ][ 0 ][ 1 ], GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 1 ] ) ) ;
		CreateNormalizePlaneD( &GSYS.DrawSetting.ClippingPlane[ 1 ], &GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 1 ], &Normal ) ;

		// �x�y����
		Normal = VCrossD( VSubD( GSYS.DrawSetting.ViewClipPos[ 1 ][ 1 ][ 0 ], GSYS.DrawSetting.ViewClipPos[ 0 ][ 1 ][ 0 ] ), VSubD( GSYS.DrawSetting.ViewClipPos[ 0 ][ 1 ][ 1 ], GSYS.DrawSetting.ViewClipPos[ 0 ][ 1 ][ 0 ] ) ) ;
		CreateNormalizePlaneD( &GSYS.DrawSetting.ClippingPlane[ 2 ], &GSYS.DrawSetting.ViewClipPos[ 0 ][ 1 ][ 0 ], &Normal ) ;

		Normal = VCrossD( VSubD( GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 1 ], GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 0 ] ), VSubD( GSYS.DrawSetting.ViewClipPos[ 1 ][ 0 ][ 0 ], GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 0 ] ) ) ;
		CreateNormalizePlaneD( &GSYS.DrawSetting.ClippingPlane[ 3 ], &GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 0 ], &Normal ) ;

		// �w�x����
		Normal = VCrossD( VSubD( GSYS.DrawSetting.ViewClipPos[ 0 ][ 1 ][ 0 ], GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 0 ] ), VSubD( GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 1 ], GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 0 ] ) ) ;
		CreateNormalizePlaneD( &GSYS.DrawSetting.ClippingPlane[ 4 ], &GSYS.DrawSetting.ViewClipPos[ 0 ][ 0 ][ 0 ], &Normal ) ;

		Normal = VCrossD( VSubD( GSYS.DrawSetting.ViewClipPos[ 1 ][ 0 ][ 1 ], GSYS.DrawSetting.ViewClipPos[ 1 ][ 0 ][ 0 ] ), VSubD( GSYS.DrawSetting.ViewClipPos[ 1 ][ 1 ][ 0 ], GSYS.DrawSetting.ViewClipPos[ 1 ][ 0 ][ 0 ] ) ) ;
		CreateNormalizePlaneD( &GSYS.DrawSetting.ClippingPlane[ 5 ], &GSYS.DrawSetting.ViewClipPos[ 1 ][ 0 ][ 0 ], &Normal ) ;
	}
}

// �g�p����ˉe�s����R�c�p�ɂ��邩�Q�c�p�ɂ��邩��ݒ肷��
//extern void Graphics_DrawSetting_SetUse2DProjectionMatrix( int Use2DProjectionMatrix )
//{
//	if( Use2DProjectionMatrix == GSYS.DrawSetting.UseProjectionMatrix2D )
//	{
//		return ;
//	}
//
//	// �`��ҋ@���Ă���`�敨��`��
//	DRAWSTOCKINFO
//
//	// �t���O��ۑ�
//	GSYS.DrawSetting.UseProjectionMatrix2D = Use2DProjectionMatrix ;
//
//	// �n�[�h�E�G�A�A�N�Z�����[�V�����ɐݒ�
//	if( GSYS.Setting.ValidHardware )
//	{
//		Graphics_Hardware_SetUse2DProjectionMatrix_PF( Use2DProjectionMatrix ) ;
//	}
//}

// �c�w���C�u���������� SetDrawScreen ���g�p���ĕ`����ύX����ۂ́A���̃J�����ݒ��`��̈�𕜌�����ׂ̏����擾���鏈�����s��
extern int Graphics_DrawSetting_GetScreenDrawSettingInfo( SCREENDRAWSETTINGINFO *ScreenDrawSettingInfo )
{
	// �e��f�[�^�ۑ�
	ScreenDrawSettingInfo->Use3DFlag			= NS_GetUse3DFlag() ;
	ScreenDrawSettingInfo->DrawMode				= NS_GetDrawMode() ;
	ScreenDrawSettingInfo->WaitVSync			= NS_GetWaitVSyncFlag() ;
	ScreenDrawSettingInfo->UseSysMemFlag		= NS_GetUseSystemMemGraphCreateFlag() ;
	ScreenDrawSettingInfo->DrawScreen			= NS_GetActiveGraph() ;
	ScreenDrawSettingInfo->ViewMatrix           = GSYS.DrawSetting.ViewMatrix ;
	ScreenDrawSettingInfo->ProjectionMatrixMode = GSYS.DrawSetting.ProjectionMatrixMode ;
	ScreenDrawSettingInfo->ProjFov              = GSYS.DrawSetting.ProjFov ;
	ScreenDrawSettingInfo->ProjSize             = GSYS.DrawSetting.ProjSize ;
	ScreenDrawSettingInfo->ProjMatrix           = GSYS.DrawSetting.ProjMatrix ;
	ScreenDrawSettingInfo->ProjNear             = GSYS.DrawSetting.ProjNear ;
	ScreenDrawSettingInfo->ProjFar              = GSYS.DrawSetting.ProjFar ;
	ScreenDrawSettingInfo->ProjDotAspect        = GSYS.DrawSetting.ProjDotAspect ;
	ScreenDrawSettingInfo->ProjectionMatrix     = GSYS.DrawSetting.ProjectionMatrix ;
	ScreenDrawSettingInfo->ViewportMatrix       = GSYS.DrawSetting.ViewportMatrix ;
	ScreenDrawSettingInfo->WorldMatrix          = GSYS.DrawSetting.WorldMatrix ;
	
	// �`��̈�𓾂�
	NS_GetDrawArea( &ScreenDrawSettingInfo->DrawRect ) ;

	// ����I��
	return 0 ;
}

// �c�w���C�u���������� SetDrawScreen ���g�p���ĕ`����ύX����ۂ́A���̃J�����ݒ��`��̈�𕜌����鏈�����s��
extern int Graphics_DrawSetting_SetScreenDrawSettingInfo( const SCREENDRAWSETTINGINFO *ScreenDrawSettingInfo )
{
	// �ۑ����Ă������ݒ�𔽉f������
	NS_SetDrawMode( ScreenDrawSettingInfo->DrawMode ) ;
	NS_SetWaitVSyncFlag( ScreenDrawSettingInfo->WaitVSync ) ;
	NS_SetDrawScreen( ScreenDrawSettingInfo->DrawScreen ) ;
	NS_SetUse3DFlag( ScreenDrawSettingInfo->Use3DFlag ) ;
	NS_SetUseSystemMemGraphCreateFlag( ScreenDrawSettingInfo->UseSysMemFlag ) ;
	NS_SetDrawArea( ScreenDrawSettingInfo->DrawRect.left, ScreenDrawSettingInfo->DrawRect.top, ScreenDrawSettingInfo->DrawRect.right, ScreenDrawSettingInfo->DrawRect.bottom ) ;

	NS_SetTransformToWorldD( &ScreenDrawSettingInfo->WorldMatrix );
	NS_SetTransformToViewD( &ScreenDrawSettingInfo->ViewMatrix );
	switch( ScreenDrawSettingInfo->ProjectionMatrixMode )
	{
	case 0 :	// ���ߖ@
		NS_SetupCamera_PerspectiveD( ScreenDrawSettingInfo->ProjFov ) ;
		break ;

	case 1 :	// ���ˉe
		NS_SetupCamera_OrthoD( ScreenDrawSettingInfo->ProjSize ) ;
		break ;

	case 2 :	// �s�񒼎w��
		NS_SetupCamera_ProjectionMatrixD( ScreenDrawSettingInfo->ProjMatrix ) ;
		break ;
	}
	NS_SetCameraNearFarD( ScreenDrawSettingInfo->ProjNear, ScreenDrawSettingInfo->ProjFar ) ;
	NS_SetCameraDotAspectD( ScreenDrawSettingInfo->ProjDotAspect ) ;
//	NS_SetTransformToProjection( &ScreenDrawSettingInfo->ProjectionMatrix );
	NS_SetTransformToViewportD( &ScreenDrawSettingInfo->ViewportMatrix );

	// ����I��
	return 0 ;
}
























// �`��֌W�֐�

#define DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK									\
	if( y1 >= DrawRect.top && y1 < DrawRect.bottom )							\
	{																			\
		if( ( CirclePos[ y1 ][ 4 ] & 1 ) == 0 || CirclePos[ y1 ][ 0 ] > x1 )	\
		{																		\
			CirclePos[ y1 ][ 4 ] |= 1 ;											\
			CirclePos[ y1 ][ 0 ] = ( short )x1 ;								\
		}																		\
	}

#define DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK									\
	if( y1 >= DrawRect.top && y1 < DrawRect.bottom )							\
	{																			\
		if( ( CirclePos[ y1 ][ 4 ] & 2 ) == 0 || CirclePos[ y1 ][ 1 ] < x1 )	\
		{																		\
			CirclePos[ y1 ][ 4 ] |= 2 ;											\
			CirclePos[ y1 ][ 1 ] = ( short )x1 ;								\
		}																		\
	}

#define DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK									\
	if( y1 >= DrawRect.top && y1 < DrawRect.bottom )							\
	{																			\
		if( ( CirclePos[ y1 ][ 4 ] & 4 ) == 0 || CirclePos[ y1 ][ 2 ] < x1 )	\
		{																		\
			CirclePos[ y1 ][ 4 ] |= 4 ;											\
			CirclePos[ y1 ][ 2 ] = ( short )x1 ;								\
		}																		\
	}

#define DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK									\
	if( y1 >= DrawRect.top && y1 < DrawRect.bottom )							\
	{																			\
		if( ( CirclePos[ y1 ][ 4 ] & 8 ) == 0 || CirclePos[ y1 ][ 3 ] > x1 )	\
		{																		\
			CirclePos[ y1 ][ 4 ] |= 8 ;											\
			CirclePos[ y1 ][ 3 ] = ( short )x1 ;								\
		}																		\
	}

// ���̕��t���~�̕`��p���_���擾����֐�
extern int	Graphics_Draw_GetCircle_ThicknessDrawPosition( int x, int y, int r, int Thickness, short ( *CirclePos )[ 5 ] )
{
	int rmax, rmin ;
	int x1, y1 ;
	int Dx, Dy, F ;
	RECT DrawRect ;

	DrawRect = GSYS.DrawSetting.DrawArea ;

	// �e���̑������Z�o
	rmax = r + Thickness / 2 ;
	rmin = r - Thickness / 2 ;
	if( rmax - rmin + 1 > Thickness )
	{
		rmin ++ ;
	}

	// �O���~�̍��W���X�g�쐬
	{
		// �����l�Z�b�g
		Dx = rmax ; Dy = 0 ; F = -2 * rmax + 3 ;

		// �ŏ��̓_
		{
			x1 = -Dx + x ; y1 = Dy + y ;
			DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;
			x1 = Dx + x ;  
			DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;

			x1 = Dy + x ; y1 = Dx + y ;
			DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;
			DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;

			x1 = Dy + x ; y1 = -Dx + y;
			DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;
			DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;

			// ���W�f�[�^��i�߂�
			if( F >= 0 )
			{
				Dx -- ;
				F -= 4 * Dx ;
			}	

			Dy ++ ;
			F += 4 * Dy + 2 ;
		}

		while( Dx >= Dy )
		{
			x1 = -Dx + x ; y1 = Dy + y ;
			DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;
			x1 = Dx + x ;  
			DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;

			x1 = Dy + x ; y1 = Dx + y ;
			DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;
			x1 = -Dy + x ; 
			DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;
		
			x1 = Dx + x ; y1 = -Dy + y;
			DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;
			x1 = -Dx + x ; 
			DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;

			x1 = Dy + x ; y1 = -Dx + y;
			DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;
			x1 = -Dy + x ; 
			DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;

			// ���W�f�[�^��i�߂�
			if( F >= 0 )
			{
				Dx -- ;
				F -= 4 * Dx ;
			}

			Dy ++ ;
			F += 4 * Dy + 2 ;
		}
	}

	// �����~�̍��W���X�g�쐬
	{
		// �����l�Z�b�g
		Dx = rmin ; Dy = 0 ; F = -2 * rmin + 3 ;

		// �ŏ��̓_
		{
			x1 = -Dx + x ; y1 = Dy + y ;
			DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;
			x1 = Dx + x ;  
			DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;

			x1 = Dy + x ; y1 = Dx + y ;
			DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;
			DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;

			x1 = Dy + x ; y1 = -Dx + y;
			DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;
			DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;

			// ���W�f�[�^��i�߂�
			if( F >= 0 )
			{
				Dx -- ;
				F -= 4 * Dx ;
			}	

			Dy ++ ;
			F += 4 * Dy + 2 ;
		}

		while( Dx >= Dy )
		{
			x1 = -Dx + x ; y1 = Dy + y ;
			DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;
			x1 = Dx + x ;  
			DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;

			x1 = Dy + x ; y1 = Dx + y ;
			DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;
			x1 = -Dy + x ; 
			DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;
		
			x1 = Dx + x ; y1 = -Dy + y;
			DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;
			x1 = -Dx + x ; 
			DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;

			x1 = Dy + x ; y1 = -Dx + y;
			DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;
			x1 = -Dy + x ; 
			DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;

			// ���W�f�[�^��i�߂�
			if( F >= 0 )
			{
				Dx -- ;
				F -= 4 * Dx ;
			}

			Dy ++ ;
			F += 4 * Dy + 2 ;
		}
	}

	// �I��
	return 0 ;
}

// ���̕��t���ȉ~�̕`��p���_���擾����֐�
extern int	Graphics_Draw_GetOval_ThicknessDrawPosition( int x, int y, int rx, int ry, int Thickness, short ( *CirclePos )[ 5 ] )
{
	int x1, y1 ;
	int Dx, Dy, F, H, Df, yc, xc, rc, r ;
	int rxmax, rymax ;
	int rxmin, rymin ;
	RECT DrawRect ;

	DrawRect = GSYS.DrawSetting.DrawArea ;

	// �e���̑������Z�o
	rxmax = rx + Thickness / 2 ;
	rxmin = rx - Thickness / 2 ;
	if( rxmax - rxmin + 1 > Thickness )
	{
		rxmin ++ ;
	}
	rymax = ry + Thickness / 2 ;
	rymin = ry - Thickness / 2 ;
	if( rymax - rymin + 1 > Thickness )
	{
		rymin ++ ;
	}

	// �O���~�̍��W���X�g�쐬
	{
		if( rxmax >= rymax )
		{
			// �����l���Z�b�g
			rc = ( rxmax * rxmax ) / rymax ;

			xc = 0 ; yc = 0 ;
			r = rxmax ;
			Df = 2 ;
	
			Dx = r ;
			Dy = 0;
			F = -2 * r + 1 + 2 * 1;
			H = -4 * r + 2 + 1;

			// �ŏ��̓_��`��
			{
				x1 = -Dx + x ; y1 = Dy + y ;
				DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;
				x1 = Dx + x ;  
				DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;

				Df = 0 ;

				// ���W�f�[�^��i�߂�
				if( F < 0 )
				{
					yc += rymax ;
					if( yc >= rc )
					{
						Dy ++ ;
						yc -= rc ;
						Df = 1 ;
					}
					F += 4 * Dy + 2 ;
					H += 4 * Dy;
				}
				else
				{
					Df = 1 ;

					if( H>=0 )
					{
						Dx--;
						F -= 4 * Dx;
						H -= 4 * Dx - 2 ;
					}
					else
					{
						Dx--;	
						yc += rymax ;
						if( yc >= rc )
						{
							Dy ++ ;
							yc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dy - 4 * Dx + 2 ;
						H += 4 * Dy - 4 * Dx + 2 ;
					}
				}
			}

			while( Dx > 0 )
			{
				if( Df )
				{
					x1 = -Dx + x ; y1 = Dy + y ;
					DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;
					x1 = Dx + x ;  
					DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;

					x1 = -Dx + x ; y1 = -Dy + y ;
					DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;
					x1 = Dx + x ;  
					DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;
				}

				Df = 0 ;

				// ���W�f�[�^��i�߂�
				if( F < 0 )
				{
					yc += rymax ;
					if( yc >= rc )
					{
						Dy ++ ;
						yc -= rc ;
						Df = 1 ;
					}
					F += 4 * Dy + 2 ;
					H += 4 * Dy;
				}
				else
				{
					Df = 1 ;

					if( H>=0 )
					{
						Dx--;
						F -= 4 * Dx;
						H -= 4 * Dx - 2 ;
					}
					else
					{
						Dx--;	
						yc += rymax ;
						if( yc >= rc )
						{
							Dy ++ ;
							yc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dy - 4 * Dx + 2 ;
						H += 4 * Dy - 4 * Dx + 2 ;
					}
				}
			}

			// �Ō�̓_��`��
			x1 = Dx + x ; y1 = -Dy + y;
			DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;

			x1 = -Dx + x ; y1 = Dy + y ;
			DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;
		}
		else
		{
			// �����l���Z�b�g
			rc = ( rymax * rymax ) / rxmax ;

			xc = 0 ; yc = 0 ;
			r = rxmax ;
			Df = 2 ;
	
			Dx = r ;
			Dy = 0;
			F = -2 * r + 1 + 2 * 1;
			H = -4 * r + 2 + 1;

			// �ŏ��̓_��`��
			{
				x1 = -Dx + x ; y1 = Dy + y ;
				DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;
				x1 = Dx + x ;  
				DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;

				Df = 0 ;

				// ���W�f�[�^��i�߂�
				if( F < 0 )
				{
					Df = 1 ;

					Dy ++ ;
					F += 4 * Dy + 2 ;
					H += 4 * Dy;
				}
				else
				{
					if( H>=0 )
					{
						xc += rxmax ;
						if( xc >= rc )
						{
							Dx--;
							xc -= rc ;
							Df = 1 ;
						}
						F -= 4 * Dx;
						H -= 4 * Dx - 2 ;
					}
					else
					{
						Df = 1 ;

						xc += rxmax ;
						if( xc >= rc )
						{
							Dx--;
							xc -= rc ;
						}

						Dy ++ ;
						F += 4 * Dy - 4 * Dx + 2 ;
						H += 4 * Dy - 4 * Dx + 2 ;
					}
				}
			}

			while( Dx > 0 )
			{
				if( Df )
				{
					x1 = -Dx + x ; y1 = Dy + y ;
					DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;
					x1 = Dx + x ;  
					DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;

					x1 = -Dx + x ; y1 = -Dy + y ;
					DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;
					x1 = Dx + x ;  
					DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;
				}

				Df = 0 ;

				// ���W�f�[�^��i�߂�
				if( F < 0 )
				{
					Df = 1 ;

					Dy ++ ;
					F += 4 * Dy + 2 ;
					H += 4 * Dy;
				}
				else
				{
					if( H>=0 )
					{
						xc += rxmax ;
						if( xc >= rc )
						{
							Dx--;
							xc -= rc ;
							Df = 1 ;
						}
						F -= 4 * Dx;
						H -= 4 * Dx - 2 ;
					}
					else
					{
						Df = 1 ;

						xc += rxmax ;
						if( xc >= rc )
						{
							Dx--;
							xc -= rc ;
						}

						Dy ++ ;
						F += 4 * Dy - 4 * Dx + 2 ;
						H += 4 * Dy - 4 * Dx + 2 ;
					}
				}
			}

			// �Ō�̓_��`��
			x1 = Dx + x ; y1 = -Dy + y;
			DX_CIRCLE_THICKNESS_R_MAX_RIGHT_CHECK ;

			x1 = -Dx + x ; y1 = Dy + y ;
			DX_CIRCLE_THICKNESS_R_MAX_LEFT_CHECK ;
		}
	}

	// �����~�̍��W���X�g�쐬
	{
		if( rxmin >= rymin )
		{
			// �����l���Z�b�g
			rc = ( rxmin * rxmin ) / rymin ;

			xc = 0 ; yc = 0 ;
			r = rxmin ;
			Df = 2 ;
	
			Dx = r ;
			Dy = 0;
			F = -2 * r + 1 + 2 * 1;
			H = -4 * r + 2 + 1;

			// �ŏ��̓_��`��
			{
				x1 = -Dx + x ; y1 = Dy + y ;
				DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;
				x1 = Dx + x ;  
				DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;

				Df = 0 ;

				// ���W�f�[�^��i�߂�
				if( F < 0 )
				{
					yc += rymin ;
					if( yc >= rc )
					{
						Dy ++ ;
						yc -= rc ;
						Df = 1 ;
					}
					F += 4 * Dy + 2 ;
					H += 4 * Dy;
				}
				else
				{
					Df = 1 ;

					if( H>=0 )
					{
						Dx--;
						F -= 4 * Dx;
						H -= 4 * Dx - 2 ;
					}
					else
					{
						Dx--;	
						yc += rymin ;
						if( yc >= rc )
						{
							Dy ++ ;
							yc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dy - 4 * Dx + 2 ;
						H += 4 * Dy - 4 * Dx + 2 ;
					}
				}
			}

			while( Dx > 0 )
			{
				if( Df )
				{
					x1 = -Dx + x ; y1 = Dy + y ;
					DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;
					x1 = Dx + x ;  
					DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;

					x1 = -Dx + x ; y1 = -Dy + y ;
					DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;
					x1 = Dx + x ;  
					DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;
				}

				Df = 0 ;

				// ���W�f�[�^��i�߂�
				if( F < 0 )
				{
					yc += rymin ;
					if( yc >= rc )
					{
						Dy ++ ;
						yc -= rc ;
						Df = 1 ;
					}
					F += 4 * Dy + 2 ;
					H += 4 * Dy;
				}
				else
				{
					Df = 1 ;

					if( H>=0 )
					{
						Dx--;
						F -= 4 * Dx;
						H -= 4 * Dx - 2 ;
					}
					else
					{
						Dx--;	
						yc += rymin ;
						if( yc >= rc )
						{
							Dy ++ ;
							yc -= rc ;
							Df = 1 ;
						}
						F += 4 * Dy - 4 * Dx + 2 ;
						H += 4 * Dy - 4 * Dx + 2 ;
					}
				}
			}

			// �Ō�̓_��`��
			x1 = Dx + x ; y1 = -Dy + y;
			DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;

			x1 = -Dx + x ; y1 = Dy + y ;
			DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;
		}
		else
		{
			// �����l���Z�b�g
			rc = ( rymin * rymin ) / rxmin ;

			xc = 0 ; yc = 0 ;
			r = rxmin ;
			Df = 2 ;
	
			Dx = r ;
			Dy = 0;
			F = -2 * r + 1 + 2 * 1;
			H = -4 * r + 2 + 1;

			// �ŏ��̓_��`��
			{
				x1 = -Dx + x ; y1 = Dy + y ;
				DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;
				x1 = Dx + x ;  
				DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;

				Df = 0 ;

				// ���W�f�[�^��i�߂�
				if( F < 0 )
				{
					Df = 1 ;

					Dy ++ ;
					F += 4 * Dy + 2 ;
					H += 4 * Dy;
				}
				else
				{
					if( H>=0 )
					{
						xc += rxmin ;
						if( xc >= rc )
						{
							Dx--;
							xc -= rc ;
							Df = 1 ;
						}
						F -= 4 * Dx;
						H -= 4 * Dx - 2 ;
					}
					else
					{
						Df = 1 ;

						xc += rxmin ;
						if( xc >= rc )
						{
							Dx--;
							xc -= rc ;
						}

						Dy ++ ;
						F += 4 * Dy - 4 * Dx + 2 ;
						H += 4 * Dy - 4 * Dx + 2 ;
					}
				}
			}

			while( Dx > 0 )
			{
				if( Df )
				{
					x1 = -Dx + x ; y1 = Dy + y ;
					DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;
					x1 = Dx + x ;  
					DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;

					x1 = -Dx + x ; y1 = -Dy + y ;
					DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;
					x1 = Dx + x ;  
					DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;
				}

				Df = 0 ;

				// ���W�f�[�^��i�߂�
				if( F < 0 )
				{
					Df = 1 ;

					Dy ++ ;
					F += 4 * Dy + 2 ;
					H += 4 * Dy;
				}
				else
				{
					if( H>=0 )
					{
						xc += rxmin ;
						if( xc >= rc )
						{
							Dx--;
							xc -= rc ;
							Df = 1 ;
						}
						F -= 4 * Dx;
						H -= 4 * Dx - 2 ;
					}
					else
					{
						Df = 1 ;

						xc += rxmin ;
						if( xc >= rc )
						{
							Dx--;
							xc -= rc ;
						}

						Dy ++ ;
						F += 4 * Dy - 4 * Dx + 2 ;
						H += 4 * Dy - 4 * Dx + 2 ;
					}
				}
			}

			// �Ō�̓_��`��
			x1 = Dx + x ; y1 = -Dy + y;
			DX_CIRCLE_THICKNESS_R_MIN_RIGHT_CHECK ;

			x1 = -Dx + x ; y1 = Dy + y ;
			DX_CIRCLE_THICKNESS_R_MIN_LEFT_CHECK ;
		}
	}

	// �I��
	return 0 ;
}

// ���W�␳���s��Ȃ��Q�|���S���`����s��( �P�e�N�X�`���摜�̂ݗL�� )

#define SETDRAWRECTCODE\
	int x[ 4 ], y[ 4 ], MaxX, MaxY, MinX, MinY ;\
	int i, ix, iy ;\
\
	x[ 0 ] = _FTOL( Param->Vertex[ 0 ].x ) ; x[ 1 ] = _FTOL( Param->Vertex[ 1 ].x ) ; x[ 2 ] = _FTOL( Param->Vertex[ 2 ].x ) ; x[ 3 ] = _FTOL( Param->Vertex[ 3 ].x ) ;\
	y[ 0 ] = _FTOL( Param->Vertex[ 0 ].y ) ; y[ 1 ] = _FTOL( Param->Vertex[ 1 ].y ) ; y[ 2 ] = _FTOL( Param->Vertex[ 2 ].y ) ; y[ 3 ] = _FTOL( Param->Vertex[ 3 ].y ) ;\
\
	MaxX = 0 ; MaxY = 0 ;\
	MinX = 0xfffffff ; MinY = 0xfffffff ;\
\
	for( i = 0 ; i < 4 ; i ++ )\
	{\
		ix = x[ i ] ; iy = y[ i ] ;\
		if( ix > MaxX ) MaxX = ix ; if( iy > MaxY ) MaxY = iy ;\
		if( ix < MinX ) MinX = ix ; if( iy < MinY ) MinY = iy ;\
	}\
\
	SETRECT( DrawRect, MinX, MinY, MaxX, MaxY ) ;\

extern int Graphics_Draw_DrawSimpleQuadrangleGraphF( const GRAPHICS_DRAW_DRAWSIMPLEQUADRANGLEGRAPHF_PARAM *Param )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret ;

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// ��`��ݒ肪����Ă���ꍇ�͉��������I��
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
	{
		return 0 ;
	}

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( Param->GraphHandle, Image ) )
	{
		return -1 ;
	}
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) )
		{
			return -1 ;
		}
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
	{
		UpdateMovie( Image->MovieHandle, FALSE ) ;
	}
#endif

	// �`��
	DRAW_DEF( 
		Graphics_Hardware_DrawSimpleQuadrangleGraphF_PF( Param, Image, BlendImage ),
		0,
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

#undef SETDRAWRECTCODE


// ���W�␳���s��Ȃ��P�|���S���`����s��( �P�e�N�X�`���摜�̂ݗL�� )

#define SETDRAWRECTCODE\
	int x[ 3 ], y[ 3 ], MaxX, MaxY, MinX, MinY ;\
	int i, ix, iy ;\
\
	x[ 0 ] = _FTOL( Param->Vertex[ 0 ].x ) ; x[ 1 ] = _FTOL( Param->Vertex[ 1 ].x ) ; x[ 2 ] = _FTOL( Param->Vertex[ 2 ].x ) ;\
	y[ 0 ] = _FTOL( Param->Vertex[ 0 ].y ) ; y[ 1 ] = _FTOL( Param->Vertex[ 1 ].y ) ; y[ 2 ] = _FTOL( Param->Vertex[ 2 ].y ) ;\
\
	MaxX = 0 ; MaxY = 0 ;\
	MinX = 0xfffffff ; MinY = 0xfffffff ;\
\
	for( i = 0 ; i < 3 ; i ++ )\
	{\
		ix = x[ i ] ; iy = y[ i ] ;\
		if( ix > MaxX ) MaxX = ix ; if( iy > MaxY ) MaxY = iy ;\
		if( ix < MinX ) MinX = ix ; if( iy < MinY ) MinY = iy ;\
	}\
\
	SETRECT( DrawRect, MinX, MinY, MaxX, MaxY ) ;\

extern int Graphics_Draw_DrawSimpleTriangleGraphF( const GRAPHICS_DRAW_DRAWSIMPLETRIANGLEGRAPHF_PARAM *Param )
{
	IMAGEDATA *Image ;
	IMAGEDATA *BlendImage ;
	int Ret ;

	// ����������
	if( DxSysData.DxLib_InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// ��`��ݒ肪����Ă���ꍇ�͉��������I��
	if( /* GSYS.NotDrawFlag || */ GSYS.DrawSetting.NotDrawFlagInSetDrawArea )
	{
		return 0 ;
	}

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( GRAPHCHK( Param->GraphHandle, Image ) )
	{
		return -1 ;
	}
	BlendImage = NULL ;
	if( GSYS.DrawSetting.BlendGraph > 0 )
	{
		GRAPHCHK( GSYS.DrawSetting.BlendGraph, BlendImage ) ;

		// �u�����h�e�N�X�`�����g�p����ꍇ�A�`��摜����������������G���[
		if( Graphics_Image_CheckBlendGraphSize( Image ) )
		{
			return -1 ;
		}
	}

#ifndef DX_NON_MOVIE
	if( Image->MovieHandle != -1 )
	{
		UpdateMovie( Image->MovieHandle, FALSE ) ;
	}
#endif

	// �`��
	DRAW_DEF( 
		Graphics_Hardware_DrawSimpleTriangleGraphF_PF( Param, Image, BlendImage ),
		0,
		SETDRAWRECTCODE,
		Ret,
		Image->Orig->FormatDesc.TextureFlag
	)

	// �I��
	return Ret ;
}

#undef SETDRAWRECTCODE



























// �J�����֌W�֐�

// �W���W�Ō`���{�b�N�X���J�����̎��E�ɓ����Ă��邩�ǂ����𔻒肷��( �߂�l TRUE:���E�ɓ����Ă��Ȃ�  FALSE:���E�ɓ����Ă��� )( CheckPos�� VECTOR 8���̔z��̐擪�A�h���X�A�z��̊e�v�f�ԍ��̓��e 0:+x +y +z   1:-x +y +z   2:-x -y +z   3:+x -y +z   4:+x -y -z   5:+x +y -z   6:-x +y -z   7:-x -y -z )
extern int Graphics_Camera_CheckCameraViewClip_Box_PosDim( VECTOR *CheckBoxPos )
{
	BYTE clip[ 8 ] ;
	VECTOR *pos ;
	int i ;

	// �e���_�����̈�ɓ����Ă��邩�ǂ����𒲂ׂ�
	pos = CheckBoxPos ;
	for( i = 0 ; i < 8 ; i ++, pos ++ )
	{
		clip[ i ] = 0 ;
		if( GSYS.DrawSetting.ClippingPlane[ 0 ].x * pos->x + GSYS.DrawSetting.ClippingPlane[ 0 ].y * pos->y + GSYS.DrawSetting.ClippingPlane[ 0 ].z * pos->z + GSYS.DrawSetting.ClippingPlane[ 0 ].w < 0.0f ) clip[ i ] |= CLIP_XM ;
		if( GSYS.DrawSetting.ClippingPlane[ 1 ].x * pos->x + GSYS.DrawSetting.ClippingPlane[ 1 ].y * pos->y + GSYS.DrawSetting.ClippingPlane[ 1 ].z * pos->z + GSYS.DrawSetting.ClippingPlane[ 1 ].w < 0.0f ) clip[ i ] |= CLIP_XP ;
		if( GSYS.DrawSetting.ClippingPlane[ 2 ].x * pos->x + GSYS.DrawSetting.ClippingPlane[ 2 ].y * pos->y + GSYS.DrawSetting.ClippingPlane[ 2 ].z * pos->z + GSYS.DrawSetting.ClippingPlane[ 2 ].w < 0.0f ) clip[ i ] |= CLIP_YM ;
		if( GSYS.DrawSetting.ClippingPlane[ 3 ].x * pos->x + GSYS.DrawSetting.ClippingPlane[ 3 ].y * pos->y + GSYS.DrawSetting.ClippingPlane[ 3 ].z * pos->z + GSYS.DrawSetting.ClippingPlane[ 3 ].w < 0.0f ) clip[ i ] |= CLIP_YP ;
		if( GSYS.DrawSetting.ClippingPlane[ 4 ].x * pos->x + GSYS.DrawSetting.ClippingPlane[ 4 ].y * pos->y + GSYS.DrawSetting.ClippingPlane[ 4 ].z * pos->z + GSYS.DrawSetting.ClippingPlane[ 4 ].w < 0.0f ) clip[ i ] |= CLIP_ZM ;
		if( GSYS.DrawSetting.ClippingPlane[ 5 ].x * pos->x + GSYS.DrawSetting.ClippingPlane[ 5 ].y * pos->y + GSYS.DrawSetting.ClippingPlane[ 5 ].z * pos->z + GSYS.DrawSetting.ClippingPlane[ 5 ].w < 0.0f ) clip[ i ] |= CLIP_ZP ;
		if( clip[ i ] == 0 ) return FALSE ;
	}

	// �ǂꂩ�̑��ɑS�Ă̒��_������Ă���ꍇ�͂P�O�O�����̈�ɓ����Ă��Ȃ�
	if( ( ( clip[0] & CLIP_XP ) && ( clip[1] & CLIP_XP ) &&
		  ( clip[2] & CLIP_XP ) && ( clip[3] & CLIP_XP ) &&
		  ( clip[4] & CLIP_XP ) && ( clip[5] & CLIP_XP ) &&
		  ( clip[6] & CLIP_XP ) && ( clip[7] & CLIP_XP ) ) ||
		( ( clip[0] & CLIP_XM ) && ( clip[1] & CLIP_XM ) &&
		  ( clip[2] & CLIP_XM ) && ( clip[3] & CLIP_XM ) &&
		  ( clip[4] & CLIP_XM ) && ( clip[5] & CLIP_XM ) &&
		  ( clip[6] & CLIP_XM ) && ( clip[7] & CLIP_XM ) ) ||
		( ( clip[0] & CLIP_YP ) && ( clip[1] & CLIP_YP ) &&
		  ( clip[2] & CLIP_YP ) && ( clip[3] & CLIP_YP ) &&
		  ( clip[4] & CLIP_YP ) && ( clip[5] & CLIP_YP ) &&
		  ( clip[6] & CLIP_YP ) && ( clip[7] & CLIP_YP ) ) ||
		( ( clip[0] & CLIP_YM ) && ( clip[1] & CLIP_YM ) &&
		  ( clip[2] & CLIP_YM ) && ( clip[3] & CLIP_YM ) &&
		  ( clip[4] & CLIP_YM ) && ( clip[5] & CLIP_YM ) &&
		  ( clip[6] & CLIP_YM ) && ( clip[7] & CLIP_YM ) ) ||
		( ( clip[0] & CLIP_ZP ) && ( clip[1] & CLIP_ZP ) &&
		  ( clip[2] & CLIP_ZP ) && ( clip[3] & CLIP_ZP ) &&
		  ( clip[4] & CLIP_ZP ) && ( clip[5] & CLIP_ZP ) &&
		  ( clip[6] & CLIP_ZP ) && ( clip[7] & CLIP_ZP ) ) ||
		( ( clip[0] & CLIP_ZM ) && ( clip[1] & CLIP_ZM ) &&
		  ( clip[2] & CLIP_ZM ) && ( clip[3] & CLIP_ZM ) &&
		  ( clip[4] & CLIP_ZM ) && ( clip[5] & CLIP_ZM ) &&
		  ( clip[6] & CLIP_ZM ) && ( clip[7] & CLIP_ZM ) ) )
		  return TRUE ;

	// �����ɂ����猩���Ă���Ƃ�������
	return FALSE ;
}

// �W���W�Ō`���{�b�N�X���J�����̎��E�ɓ����Ă��邩�ǂ����𔻒肷��( �߂�l TRUE:���E�ɓ����Ă��Ȃ�  FALSE:���E�ɓ����Ă��� )( CheckPos�� VECTOR 8���̔z��̐擪�A�h���X�A�z��̊e�v�f�ԍ��̓��e 0:+x +y +z   1:-x +y +z   2:-x -y +z   3:+x -y +z   4:+x -y -z   5:+x +y -z   6:-x +y -z   7:-x -y -z )
extern int Graphics_Camera_CheckCameraViewClip_Box_PosDimD( VECTOR_D *CheckBoxPos )
{
	BYTE clip[ 8 ] ;
	VECTOR_D *pos ;
	int i ;

	// �e���_�����̈�ɓ����Ă��邩�ǂ����𒲂ׂ�
	pos = CheckBoxPos ;
	for( i = 0 ; i < 8 ; i ++, pos ++ )
	{
		clip[ i ] = 0 ;
		if( GSYS.DrawSetting.ClippingPlane[ 0 ].x * pos->x + GSYS.DrawSetting.ClippingPlane[ 0 ].y * pos->y + GSYS.DrawSetting.ClippingPlane[ 0 ].z * pos->z + GSYS.DrawSetting.ClippingPlane[ 0 ].w < 0.0 ) clip[ i ] |= CLIP_XM ;
		if( GSYS.DrawSetting.ClippingPlane[ 1 ].x * pos->x + GSYS.DrawSetting.ClippingPlane[ 1 ].y * pos->y + GSYS.DrawSetting.ClippingPlane[ 1 ].z * pos->z + GSYS.DrawSetting.ClippingPlane[ 1 ].w < 0.0 ) clip[ i ] |= CLIP_XP ;
		if( GSYS.DrawSetting.ClippingPlane[ 2 ].x * pos->x + GSYS.DrawSetting.ClippingPlane[ 2 ].y * pos->y + GSYS.DrawSetting.ClippingPlane[ 2 ].z * pos->z + GSYS.DrawSetting.ClippingPlane[ 2 ].w < 0.0 ) clip[ i ] |= CLIP_YM ;
		if( GSYS.DrawSetting.ClippingPlane[ 3 ].x * pos->x + GSYS.DrawSetting.ClippingPlane[ 3 ].y * pos->y + GSYS.DrawSetting.ClippingPlane[ 3 ].z * pos->z + GSYS.DrawSetting.ClippingPlane[ 3 ].w < 0.0 ) clip[ i ] |= CLIP_YP ;
		if( GSYS.DrawSetting.ClippingPlane[ 4 ].x * pos->x + GSYS.DrawSetting.ClippingPlane[ 4 ].y * pos->y + GSYS.DrawSetting.ClippingPlane[ 4 ].z * pos->z + GSYS.DrawSetting.ClippingPlane[ 4 ].w < 0.0 ) clip[ i ] |= CLIP_ZM ;
		if( GSYS.DrawSetting.ClippingPlane[ 5 ].x * pos->x + GSYS.DrawSetting.ClippingPlane[ 5 ].y * pos->y + GSYS.DrawSetting.ClippingPlane[ 5 ].z * pos->z + GSYS.DrawSetting.ClippingPlane[ 5 ].w < 0.0 ) clip[ i ] |= CLIP_ZP ;
		if( clip[ i ] == 0 ) return FALSE ;
	}

	// �ǂꂩ�̑��ɑS�Ă̒��_������Ă���ꍇ�͂P�O�O�����̈�ɓ����Ă��Ȃ�
	if( ( ( clip[0] & CLIP_XP ) && ( clip[1] & CLIP_XP ) &&
		  ( clip[2] & CLIP_XP ) && ( clip[3] & CLIP_XP ) &&
		  ( clip[4] & CLIP_XP ) && ( clip[5] & CLIP_XP ) &&
		  ( clip[6] & CLIP_XP ) && ( clip[7] & CLIP_XP ) ) ||
		( ( clip[0] & CLIP_XM ) && ( clip[1] & CLIP_XM ) &&
		  ( clip[2] & CLIP_XM ) && ( clip[3] & CLIP_XM ) &&
		  ( clip[4] & CLIP_XM ) && ( clip[5] & CLIP_XM ) &&
		  ( clip[6] & CLIP_XM ) && ( clip[7] & CLIP_XM ) ) ||
		( ( clip[0] & CLIP_YP ) && ( clip[1] & CLIP_YP ) &&
		  ( clip[2] & CLIP_YP ) && ( clip[3] & CLIP_YP ) &&
		  ( clip[4] & CLIP_YP ) && ( clip[5] & CLIP_YP ) &&
		  ( clip[6] & CLIP_YP ) && ( clip[7] & CLIP_YP ) ) ||
		( ( clip[0] & CLIP_YM ) && ( clip[1] & CLIP_YM ) &&
		  ( clip[2] & CLIP_YM ) && ( clip[3] & CLIP_YM ) &&
		  ( clip[4] & CLIP_YM ) && ( clip[5] & CLIP_YM ) &&
		  ( clip[6] & CLIP_YM ) && ( clip[7] & CLIP_YM ) ) ||
		( ( clip[0] & CLIP_ZP ) && ( clip[1] & CLIP_ZP ) &&
		  ( clip[2] & CLIP_ZP ) && ( clip[3] & CLIP_ZP ) &&
		  ( clip[4] & CLIP_ZP ) && ( clip[5] & CLIP_ZP ) &&
		  ( clip[6] & CLIP_ZP ) && ( clip[7] & CLIP_ZP ) ) ||
		( ( clip[0] & CLIP_ZM ) && ( clip[1] & CLIP_ZM ) &&
		  ( clip[2] & CLIP_ZM ) && ( clip[3] & CLIP_ZM ) &&
		  ( clip[4] & CLIP_ZM ) && ( clip[5] & CLIP_ZM ) &&
		  ( clip[6] & CLIP_ZM ) && ( clip[7] & CLIP_ZM ) ) )
		  return TRUE ;

	// �����ɂ����猩���Ă���Ƃ�������
	return FALSE ;
}

// �r���[�s�񂩂琅���A�����A�P��p�x���Z�o����
extern void Graphics_Camera_CalcCameraRollViewMatrix( void )
{
	VECTOR_D XVec, YVec, ZVec ;
	double Sin, Cos, f ;

	XVec.x = GSYS.DrawSetting.ViewMatrix.m[ 0 ][ 0 ] ;
	XVec.y = GSYS.DrawSetting.ViewMatrix.m[ 1 ][ 0 ] ;
	XVec.z = GSYS.DrawSetting.ViewMatrix.m[ 2 ][ 0 ] ;

	YVec.x = GSYS.DrawSetting.ViewMatrix.m[ 0 ][ 1 ] ;
	YVec.y = GSYS.DrawSetting.ViewMatrix.m[ 1 ][ 1 ] ;
	YVec.z = GSYS.DrawSetting.ViewMatrix.m[ 2 ][ 1 ] ;

	ZVec.x = GSYS.DrawSetting.ViewMatrix.m[ 0 ][ 2 ] ;
	ZVec.y = GSYS.DrawSetting.ViewMatrix.m[ 1 ][ 2 ] ;
	ZVec.z = GSYS.DrawSetting.ViewMatrix.m[ 2 ][ 2 ] ;

	if( ZVec.x < 0.00000001 && ZVec.x > -0.00000001 &&
		ZVec.z < 0.00000001 && ZVec.z > -0.00000001 )
	{
		GSYS.Camera.HRotate = 0.0 ;
		if( ZVec.y < 0.0 )
		{
			GSYS.Camera.VRotate =  DX_PI / 2.0 ;
			GSYS.Camera.TRotate = _ATAN2D( -YVec.x, YVec.z ) ;
		}
		else
		{
			GSYS.Camera.VRotate = -DX_PI / 2.0 ;
			GSYS.Camera.TRotate = _ATAN2D( -YVec.x, -YVec.z ) ;
		}
	}
	else
	{
		if( YVec.y < 0.0 )
		{
			GSYS.Camera.HRotate = _ATAN2D( -ZVec.x, -ZVec.z ) ;

			_SINCOSD( -GSYS.Camera.HRotate, &Sin, &Cos ) ;
			f      = XVec.z * Cos - XVec.x * Sin ;
			XVec.x = XVec.z * Sin + XVec.x * Cos ;
			XVec.z = f ;

			f      = ZVec.z * Cos - ZVec.x * Sin ;
			ZVec.x = ZVec.z * Sin + ZVec.x * Cos ;
			ZVec.z = f ;

			if( ZVec.y > 0.0 )
			{
				GSYS.Camera.VRotate = -_ATAN2D( -ZVec.z,  ZVec.y ) - DX_PI / 2.0 ;
			}
			else
			{
				GSYS.Camera.VRotate =  _ATAN2D( -ZVec.z, -ZVec.y ) + DX_PI / 2.0 ;
			}

			_SINCOSD( -GSYS.Camera.VRotate, &Sin, &Cos ) ;
			f      = XVec.y * Cos - XVec.z * Sin ;
			XVec.z = XVec.y * Sin + XVec.z * Cos ;
			XVec.y = f ;

			f      = ZVec.y * Cos - ZVec.z * Sin ;
			ZVec.z = ZVec.y * Sin + ZVec.z * Cos ;
			ZVec.y = f ;

			GSYS.Camera.TRotate = _ATAN2D( XVec.y, XVec.x ) ;
		}
		else
		{
			GSYS.Camera.HRotate = _ATAN2D( ZVec.x, ZVec.z ) ;

			_SINCOSD( -GSYS.Camera.HRotate, &Sin, &Cos ) ;
			f      = XVec.z * Cos - XVec.x * Sin ;
			XVec.x = XVec.z * Sin + XVec.x * Cos ;
			XVec.z = f ;

			f      = ZVec.z * Cos - ZVec.x * Sin ;
			ZVec.x = ZVec.z * Sin + ZVec.x * Cos ;
			ZVec.z = f ;

			GSYS.Camera.VRotate = _ATAN2D( -ZVec.y, ZVec.z ) ;

			_SINCOSD( -GSYS.Camera.VRotate, &Sin, &Cos ) ;
			f      = XVec.y * Cos - XVec.z * Sin ;
			XVec.z = XVec.y * Sin + XVec.z * Cos ;
			XVec.y = f ;

			f      = ZVec.y * Cos - ZVec.z * Sin ;
			ZVec.z = ZVec.y * Sin + ZVec.z * Cos ;
			ZVec.y = f ;

			GSYS.Camera.TRotate = _ATAN2D( XVec.y, XVec.x ) ;
		}
	}

//	NS_DrawFormatString( 0, 20, NS_GetColor( 255,255,255 ), "H:%.3f V:%.3f T:%.3f", GSYS.Camera.HRotate, GSYS.Camera.VRotate, GSYS.Camera.TRotate ) ;

}






















// ���C�g�֌W�֐�

// ���C�g�n���h���̒ǉ�
extern int Graphics_Light_AddHandle( void )
{
	LIGHT_HANDLE *pLH, **ppLH ;
	int i ;

	// �g�p����Ă��Ȃ����C�g�n���h����T��
	if( GSYS.Light.Area != MAX_LIGHT_NUM )
	{
		ppLH = &GSYS.Light.Data[ GSYS.Light.Area ] ;
		i = GSYS.Light.Area ;
	}
	else
	{
		ppLH = GSYS.Light.Data ;
		for( i = 0 ; *ppLH != NULL ; i ++, ppLH ++ ){}
	}

	// �f�[�^�̈���m�ۂ���
	if( ( pLH = *ppLH = ( LIGHT_HANDLE * )DXCALLOC( sizeof( LIGHT_HANDLE ) ) ) == NULL )
		return DxLib_ErrorUTF16LE( "\xe9\x30\xa4\x30\xc8\x30\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"���C�g�p�̃������̊m�ۂɎ��s���܂���" @*/ ) ;

	// �[��������
	_MEMSET( pLH, 0, sizeof( LIGHT_HANDLE ) ) ;

	// �����l���Z�b�g
	pLH->Param.Ambient.r = 0.33f ;
	pLH->Param.Ambient.g = 0.33f ;
	pLH->Param.Ambient.b = 0.33f ;
	pLH->Param.Ambient.a = 0.33f ;
	pLH->Param.Diffuse.r = 1.00f ;
	pLH->Param.Diffuse.g = 1.00f ;
	pLH->Param.Diffuse.b = 1.00f ;
	pLH->Param.Diffuse.a = 1.00f ;
	pLH->Param.Specular.r = 1.00f ;
	pLH->Param.Specular.g = 1.00f ;
	pLH->Param.Specular.b = 1.00f ;
	pLH->Param.Specular.a = 1.00f ;
	pLH->Param.Attenuation0 = 0.1f ;
	pLH->Param.Attenuation1 = 0.0f ;
	pLH->Param.Attenuation2 = 0.0f ;
	pLH->Param.Falloff = 1.0f ;
	pLH->Param.Theta = 2.0f ;
	pLH->Param.Phi = 2.0f ;
	pLH->Param.Range = 2000.0f ;
	pLH->EnableFlag = FALSE ;
	pLH->SetHardwareIndex = -1 ;
	pLH->HardwareChangeFlag = FALSE ;

	// �G���[�`�F�b�N�p�h�c�̐ݒ�
	pLH->ID = ( unsigned int )GSYS.Light.HandleID ;
	GSYS.Light.HandleID ++ ;
	if( GSYS.Light.HandleID >= ( DX_HANDLECHECK_MASK >> DX_HANDLECHECK_ADDRESS ) )
		GSYS.Light.HandleID = 0 ;

	// �n���h���l���Z�b�g
	pLH->Handle = ( int )( i | DX_HANDLETYPE_MASK_LIGHT | ( pLH->ID << DX_HANDLECHECK_ADDRESS ) ) ;

	// ���C�g�̐��𑝂₷
	GSYS.Light.Num ++ ;

	// �g�p����Ă��郉�C�g�n���h�������݂���͈͂��X�V����
	if( GSYS.Light.Area == i ) GSYS.Light.Area ++ ;

	// �L���ɂ���
	NS_SetLightEnableHandle( pLH->Handle, TRUE ) ;

	// �n���h����Ԃ�
	return pLH->Handle ;
}

// ���C�g�̕ύX�𔽉f����
extern int Graphics_Light_RefreshState( void )
{
	int i, j, k, OldEnableNum, OldMaxIndex ;
	LIGHT_HANDLE *pLH ;

	// ���C�g�̐ݒ�𔽉f����
	Graphics_Hardware_Light_SetUse_PF( GSYS.Light.ProcessDisable ? FALSE : TRUE ) ;

	// �ݒ肪�ύX����Ă��Ȃ������牽�����Ȃ�
	if( GSYS.Light.HardwareChange == FALSE && GSYS.Light.HardwareRefresh == FALSE )
	{
		return 0 ;
	}

	// �L���ȃ��C�g�����Ԃɔ��f����
	OldEnableNum                      = GSYS.Light.EnableNum ;
	OldMaxIndex                       = GSYS.Light.MaxHardwareEnableIndex ;
	GSYS.Light.EnableNum              = 0 ;
	GSYS.Light.MaxHardwareEnableIndex = -1 ;
	for( k = 3 ; k >= 1 ; k -- )
	{
		for( i = 0, j = 0 ; i < GSYS.Light.Num ; j ++ )
		{
			if( GSYS.Light.Data[ j ] == NULL )
			{
				continue ;
			}
			i ++ ;

			pLH = GSYS.Light.Data[ j ] ;
			if( pLH->EnableFlag == FALSE || pLH->Param.LightType != k )
			{
				continue ;
			}

			if( MAX_HARDWAREENABLELIGHTINDEX_NUM > GSYS.Light.EnableNum )
			{
				GSYS.Light.EnableHardwareIndex[ GSYS.Light.EnableNum ] = j ;
			}

			if( pLH->SetHardwareIndex != GSYS.Light.EnableNum || pLH->HardwareChangeFlag == TRUE || GSYS.Light.HardwareRefresh == TRUE )
			{
				pLH->SetHardwareIndex   = GSYS.Light.EnableNum ;
				pLH->HardwareChangeFlag = FALSE ;
//				D_SetLightParam( pLH->SetHardwareIndex, pLH->EnableFlag, &pLH->Param ) ;
				if( pLH->EnableFlag )
				{
					Graphics_Hardware_Light_SetState_PF( pLH->SetHardwareIndex, &pLH->Param ) ;
				}
				Graphics_Hardware_Light_SetEnable_PF( pLH->SetHardwareIndex, pLH->EnableFlag ) ;
			}
			GSYS.Light.MaxHardwareEnableIndex = GSYS.Light.EnableNum ;
			GSYS.Light.EnableNum ++ ;
		}
	}

	// �O����L���ȃ��C�g�̐������Ȃ��ꍇ�͗L���ȃ��C�g��OFF�ɂ���
	for( i = GSYS.Light.MaxHardwareEnableIndex + 1 ; i <= OldMaxIndex ; i ++ )
	{
//		D_SetLightParam( i, FALSE, NULL ) ;
		Graphics_Hardware_Light_SetEnable_PF( i, FALSE ) ;
	}

	// �ݒ肪�ύX���ꂽ�t���O��|��
	GSYS.Light.HardwareChange = FALSE ;

	// �ݒ肪�ύX���ꂽ���ǂ����̃t���O�𗧂Ă�
	GSYS.ChangeSettingFlag = TRUE ;

	// �ݒ�̍۔��f���K�v���ǂ����̃t���O��|��
	GSYS.Light.HardwareRefresh = FALSE ;

	// �s�N�Z���V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
	Graphics_ShadowMap_RefreshPSParam() ;

	// �I��
	return 0 ;
}






















// �V�F�[�_�[�֌W�֐�

// �V�F�[�_�[�n���h�����쐬����
extern int Graphics_Shader_CreateHandle( int ASyncThread )
{
	if( GSYS.InitializeFlag == FALSE )
		return -1 ;

	// �V�F�[�_���g���Ȃ��ꍇ�͎��s
	if( GSYS.HardInfo.UseShader == FALSE )
		return -1 ;

	return AddHandle( DX_HANDLETYPE_SHADER, ASyncThread, -1 ) ;
}

// CreateShader �̎������֐�
static int Graphics_Shader_CreateHandle_Static(
	int ShaderHandle,
	int IsVertexShader,
	void *Image,
	int ImageSize,
	int ImageAfterFree,
	int ASyncThread
)
{
	return Graphics_Hardware_Shader_Create_PF( ShaderHandle, IsVertexShader, Image, ImageSize, ImageAfterFree, ASyncThread ) ;
}

#ifndef DX_NON_ASYNCLOAD

// CreateShader �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void Graphics_Shader_CreateHandle_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	int   ShaderHandle ;
	int   IsVertexShader ;
	void *Image ;
	int   ImageSize ;
	int   ImageAfterFree ;
	int   Addr ;
	int   Result ;

	Addr = 0 ;
	ShaderHandle   = GetASyncLoadParamInt(   AParam->Data, &Addr ) ;
	IsVertexShader = GetASyncLoadParamInt(   AParam->Data, &Addr ) ;
	Image          = GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	ImageSize      = GetASyncLoadParamInt(   AParam->Data, &Addr ) ;
	ImageAfterFree = GetASyncLoadParamInt(   AParam->Data, &Addr ) ;

	Result = Graphics_Shader_CreateHandle_Static( ShaderHandle, IsVertexShader, Image, ImageSize, ImageAfterFree, TRUE ) ;
	DecASyncLoadCount( ShaderHandle ) ;
	if( Result < 0 )
	{
		NS_DeleteShader( ShaderHandle ) ;
	}
}

#endif // DX_NON_ASYNCLOAD

// �V�F�[�_�[�n���h�����쐬����
extern int Graphics_Shader_CreateHandle_UseGParam(
	int IsVertexShader,
	void *Image,
	int ImageSize,
	int ImageAfterFree,
	int ASyncLoadFlag,
	int ASyncThread
)
{
	int ShaderHandle = -1 ;

	// �V�F�[�_���g���Ȃ��ꍇ�͎��s
	if( Graphics_Hardware_CheckValid_PF() == 0 || GSYS.HardInfo.UseShader == FALSE )
		return -1 ;

	ShaderHandle = Graphics_Shader_CreateHandle( ASyncThread ) ;
	if( ShaderHandle == -1 )
		goto ERR ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag && ASyncThread == FALSE )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt( NULL, &Addr, ShaderHandle ) ;
		AddASyncLoadParamInt( NULL, &Addr, IsVertexShader ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, Image ) ;
		AddASyncLoadParamInt( NULL, &Addr, ImageSize ) ;
		AddASyncLoadParamInt( NULL, &Addr, ImageAfterFree ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = Graphics_Shader_CreateHandle_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt( AParam->Data, &Addr, ShaderHandle ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, IsVertexShader ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, Image ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, ImageSize ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, ImageAfterFree ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( ShaderHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( Graphics_Shader_CreateHandle_Static( ShaderHandle, IsVertexShader, Image, ImageSize, ImageAfterFree, ASyncThread ) < 0 )
			goto ERR ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( ShaderHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	// �n���h����Ԃ�
	return ShaderHandle ;

ERR :
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( ShaderHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	NS_DeleteShader( ShaderHandle ) ;
	ShaderHandle = -1 ;

	// �I��
	return -1 ;
}

// LoadShader �̎������֐�
static int Graphics_Shader_LoadShader_Static( int ShaderHandle, int IsVertexShader, const wchar_t *FileName, int ASyncThread )
{
	SHADERHANDLEDATA *pShader ;
	DWORD_PTR fp ;
	LONGLONG size ;
	void *buffer ;

	// �G���[����
	if( ASyncThread )
	{
		if( SHADERCHK_ASYNC( ShaderHandle, pShader ) )
			return -1 ;
	}
	else
	{
		if( SHADERCHK( ShaderHandle, pShader ) )
			return -1 ;
	}

	fp = DX_FOPEN( FileName ) ;
	if( fp == 0 ) return -1 ;
	DX_FSEEK( fp, 0, SEEK_END ) ;
	size = DX_FTELL( fp ) ;
	DX_FSEEK( fp, 0, SEEK_SET ) ;
	buffer = DXALLOC( ( size_t )size ) ;
	if( buffer == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xb7\x30\xa7\x30\xfc\x30\xc0\x30\xfc\x30\x6e\x30\xd0\x30\xa4\x30\xca\x30\xea\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�V�F�[�_�[�̃o�C�i���f�[�^���i�[���郁�����̈�̊m�ۂɎ��s���܂���\n" @*/ ) ;
		return -1 ;
	}
	DX_FREAD( buffer, size, 1, fp ) ;
	DX_FCLOSE( fp ) ;

	return Graphics_Shader_CreateHandle_Static( ShaderHandle, IsVertexShader, buffer, ( int )size, TRUE, ASyncThread ) ;
}

#ifndef DX_NON_ASYNCLOAD

// Graphics_Shader_LoadShader �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void Graphics_Shader_LoadShader_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	int          ShaderHandle ;
	int          IsVertexShader ;
	const wchar_t *FileName ;
	int          Addr ;
	int          Result ;

	Addr = 0 ;
	ShaderHandle   = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	IsVertexShader = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	FileName       = GetASyncLoadParamString( AParam->Data, &Addr ) ;

	Result = Graphics_Shader_LoadShader_Static( ShaderHandle, IsVertexShader, FileName, TRUE ) ;
	DecASyncLoadCount( ShaderHandle ) ;
	if( Result < 0 )
	{
		NS_DeleteShader( ShaderHandle ) ;
	}
}

#endif // DX_NON_ASYNCLOAD

// �V�F�[�_�[�o�C�i�����t�@�C������ǂݍ���
extern int Graphics_Shader_LoadShader_UseGParam( int IsVertexShader, const wchar_t *FileName, int ASyncLoadFlag )
{
	int ShaderHandle = -1 ;

	// �V�F�[�_���g���Ȃ��ꍇ�͎��s
	if( Graphics_Hardware_CheckValid_PF() == 0 || GSYS.HardInfo.UseShader == FALSE )
		return -1 ;

	ShaderHandle = Graphics_Shader_CreateHandle( FALSE ) ;
	if( ShaderHandle == -1 )
		goto ERR ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;
		wchar_t FullPath[ 1024 ] ;

		ConvertFullPathW_( FileName, FullPath, sizeof( FullPath ) ) ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt( NULL, &Addr, ShaderHandle ) ;
		AddASyncLoadParamInt( NULL, &Addr, IsVertexShader ) ;
		AddASyncLoadParamString( NULL, &Addr, FullPath ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = Graphics_Shader_LoadShader_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt( AParam->Data, &Addr, ShaderHandle ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, IsVertexShader ) ;
		AddASyncLoadParamString( AParam->Data, &Addr, FullPath ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( ShaderHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( Graphics_Shader_LoadShader_Static( ShaderHandle, IsVertexShader, FileName, FALSE ) < 0 )
			goto ERR ;
	}

	// �n���h����Ԃ�
	return ShaderHandle ;

ERR :
	NS_DeleteShader( ShaderHandle ) ;
	ShaderHandle = -1 ;

	// �I��
	return -1 ;
}

// �V�F�[�_�[�n���h���̏�����
extern int Graphics_Shader_InitializeHandle( HANDLEINFO *HandleInfo )
{
	SHADERHANDLEDATA *Shader = ( SHADERHANDLEDATA * )HandleInfo ;

	// ���ˑ��f�[�^�̃A�h���X���Z�b�g
	Shader->PF = ( SHADERHANDLEDATA_PF * )( Shader + 1 ) ;

	// ����I��
	return 0 ;
}

// �V�F�[�_�[�n���h���̌�n��
extern int Graphics_Shader_TerminateHandle( HANDLEINFO *HandleInfo )
{
	SHADERHANDLEDATA *Shader = ( SHADERHANDLEDATA * )HandleInfo ;

	// �g�p����V�F�[�_�[�Ƃ��ăZ�b�g����Ă�����O��
	if( GSYS.DrawSetting.UserShaderRenderInfo.SetVertexShaderHandle == HandleInfo->Handle )
	{
		NS_SetUseVertexShader( -1 ) ;
	}
	if( GSYS.DrawSetting.UserShaderRenderInfo.SetPixelShaderHandle == HandleInfo->Handle )
	{
		NS_SetUsePixelShader( -1 ) ;
	}

	// �V�F�[�_�[�̃o�C�i���f�[�^�����
	if( Shader->FunctionCode != NULL )
	{
		DXFREE( Shader->FunctionCode ) ;
		Shader->FunctionCode = NULL ;
	}

	// ���ˑ��̌�n�����������s
	Graphics_Hardware_Shader_TerminateHandle_PF( Shader ) ;

	// �I��
	return 0 ;
}

// �V�F�[�_�[�f�[�^���n���h���l������o��
extern	SHADERHANDLEDATA *Graphics_Shader_GetData( int ShaderHandle, int ASyncThread )
{
	SHADERHANDLEDATA *Shader ;

	// �G���[����
	if( ASyncThread )
	{
		if( SHADERCHK_ASYNC( ShaderHandle, Shader ) )
		{
			return NULL ;
		}
	}
	else
	{
		if( SHADERCHK( ShaderHandle, Shader ) )
		{
			return NULL ;
		}
	}

	return Shader ;
}



















// �萔�o�b�t�@�֌W�֐�

// �V�F�[�_�[�p�萔�o�b�t�@�n���h�����쐬����
extern int Graphics_ShaderConstantBuffer_CreateHandle( int ASyncThread )
{
	if( GSYS.InitializeFlag == FALSE )
		return -1 ;

	// �V�F�[�_���g���Ȃ��ꍇ�͎��s
	if( GSYS.HardInfo.UseShader == FALSE )
		return -1 ;

	return AddHandle( DX_HANDLETYPE_SHADER_CONSTANT_BUFFER, ASyncThread, -1 ) ;
}


// Graphics_ShaderConstantBuffer_CreateHandle �̎������֐�
static int Graphics_ShaderConstantBuffer_CreateHandle_Static(
	int ShaderConstantBufferHandle,
	int BufferSize,
	int ASyncThread
)
{
	return Graphics_Hardware_ShaderConstantBuffer_Create_PF( ShaderConstantBufferHandle, BufferSize, ASyncThread ) ;
}

#ifndef DX_NON_ASYNCLOAD

// Graphics_ShaderConstantBuffer_CreateHandle �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void Graphics_ShaderConstantBuffer_CreateHandle_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	int   ShaderConstantBufferHandle ;
	int   BufferSize ;
	int   Addr ;
	int   Result ;

	Addr = 0 ;
	ShaderConstantBufferHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	BufferSize                 = GetASyncLoadParamInt( AParam->Data, &Addr ) ;

	Result = Graphics_ShaderConstantBuffer_CreateHandle_Static( ShaderConstantBufferHandle, BufferSize, TRUE ) ;
	DecASyncLoadCount( ShaderConstantBufferHandle ) ;
	if( Result < 0 )
	{
		NS_DeleteShaderConstantBuffer( ShaderConstantBufferHandle ) ;
	}
}

#endif // DX_NON_ASYNCLOAD

// �V�F�[�_�[�p�萔�o�b�t�@�n���h�����쐬����
extern int Graphics_ShaderConstantBuffer_Create( int BufferSize, int ASyncLoadFlag, int ASyncThread )
{
	int ShaderConstantBufferHandle = -1 ;

	// �V�F�[�_���g���Ȃ��ꍇ�͎��s
	if( Graphics_Hardware_CheckValid_PF() == 0 || GSYS.HardInfo.UseShader == FALSE )
		return -1 ;

	ShaderConstantBufferHandle = Graphics_ShaderConstantBuffer_CreateHandle( ASyncThread ) ;
	if( ShaderConstantBufferHandle == -1 )
		goto ERR ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag && ASyncThread == FALSE )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt( NULL, &Addr, ShaderConstantBufferHandle ) ;
		AddASyncLoadParamInt( NULL, &Addr, BufferSize ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = Graphics_ShaderConstantBuffer_CreateHandle_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt( AParam->Data, &Addr, ShaderConstantBufferHandle ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, BufferSize ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( ShaderConstantBufferHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( Graphics_ShaderConstantBuffer_CreateHandle_Static( ShaderConstantBufferHandle, BufferSize, ASyncThread ) < 0 )
			goto ERR ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( ShaderConstantBufferHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	// �n���h����Ԃ�
	return ShaderConstantBufferHandle ;

ERR :
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( ShaderConstantBufferHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	NS_DeleteShaderConstantBuffer( ShaderConstantBufferHandle ) ;
	ShaderConstantBufferHandle = -1 ;

	// �I��
	return -1 ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̏�����
extern int Graphics_ShaderConstantBuffer_InitializeHandle( HANDLEINFO *HandleInfo )
{
	SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer = ( SHADERCONSTANTBUFFERHANDLEDATA * )HandleInfo ;

	// ���ˑ��f�[�^�̃A�h���X���Z�b�g
	ShaderConstantBuffer->PF = ( SHADERCONSTANTBUFFERHANDLEDATA_PF * )( ShaderConstantBuffer + 1 ) ;

	// ����I��
	return 0 ;
}

// �V�F�[�_�[�p�萔�o�b�t�@�n���h���̌�n��
extern int Graphics_ShaderConstantBuffer_TerminateHandle( HANDLEINFO *HandleInfo )
{
	SHADERCONSTANTBUFFERHANDLEDATA *ShaderConstantBuffer = ( SHADERCONSTANTBUFFERHANDLEDATA * )HandleInfo ;

	// ���ˑ��̌�n�����������s
	Graphics_Hardware_ShaderConstantBuffer_TerminateHandle_PF( ShaderConstantBuffer ) ;

	// �I��
	return 0 ;
}




























// ���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@�֌W�֐�

// ���_�o�b�t�@���쐬����
extern int Graphics_VertexBuffer_Create( int VertexNum, int VertexType /* DX_VERTEX_TYPE_NORMAL_3D �� */, int ASyncThread )
{
	int NewHandle ;
	int Result ;

	// �V�����n���h�����쐬����
	NewHandle = AddHandle( DX_HANDLETYPE_VERTEX_BUFFER, ASyncThread, -1 ) ;
	if( NewHandle < 0 )
	{
		return -1 ;
	}

	// �n���h���̃Z�b�g�A�b�v
	Result = Graphics_VertexBuffer_SetupHandle( NewHandle, VertexNum, VertexType ) ;
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( NewHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD
	if( Result < 0 )
	{
		SubHandle( NewHandle ) ;
		return -1 ;
	}

	// �n���h����Ԃ�
	return NewHandle ;
}

// ���_�o�b�t�@�n���h���̃Z�b�g�A�b�v���s��
extern int Graphics_VertexBuffer_SetupHandle( int VertexBufHandle, int VertexNum, int VertexType /* DX_VERTEX_TYPE_NORMAL_3D �� */ )
{
	VERTEXBUFFERHANDLEDATA *VertexBuffer ;
	int VertexSize ;

	// �G���[����
	if( VERTEXBUFFERCHK( VertexBufHandle, VertexBuffer ) )
		return -1 ;

	// �Ή����Ă��Ȃ��^�C�v�̏ꍇ�̓G���[
	switch( VertexType )
	{
	case DX_VERTEX_TYPE_NORMAL_3D :
		VertexSize = sizeof( VERTEX3D ) ;
		break ;

	case DX_VERTEX_TYPE_SHADER_3D :
		VertexSize = sizeof( VERTEX3DSHADER ) ;

		// �V�F�[�_�[���g���Ȃ��ꍇ�͎��s
		if( GSYS.HardInfo.UseShader == FALSE )
			return -1 ;
		break ;

	default :
		DXST_ERRORLOG_ADDUTF16LE( "\x02\x98\xb9\x70\xbf\x30\xa4\x30\xd7\x30\x6e\x30\x24\x50\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x0a\x00\x00"/*@ L"���_�^�C�v�̒l���s���ł�\n" @*/ ) ;
		return -1 ;
	}

	// �^�C�v��ۑ�
	VertexBuffer->Type = VertexType ;

	// ���_����ۑ�
	VertexBuffer->Num = VertexNum ;

	// ���_�f�[�^��ӂ�̃o�C�g����ۑ�
	VertexBuffer->UnitSize = VertexSize ;

	// ���ˑ��̒��_�o�b�t�@�쐬����
	if( Graphics_Hardware_VertexBuffer_Create_PF( VertexBuffer ) < 0 )
	{
		return -1 ;
	}

	// �f�[�^�̈���m�ۂ���
	if( ReallocHandle( VertexBufHandle, sizeof( VERTEXBUFFERHANDLEDATA ) + sizeof( VERTEXBUFFERHANDLEDATA_PF ) + VertexSize * VertexNum + 32 ) < 0 )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�������̊m�ۂɎ��s���܂���" @*/ ) ;
		return -1 ;
	}
	VertexBuffer     = ( VERTEXBUFFERHANDLEDATA *    )GetHandleInfo( VertexBufHandle ) ;
	VertexBuffer->PF = ( VERTEXBUFFERHANDLEDATA_PF * )( VertexBuffer + 1 ) ;

	// �V�X�e����������̒��_�f�[�^��ۑ�����A�h���X���Z�b�g
	VertexBuffer->Buffer = ( void * )( ( ( DWORD_PTR )VertexBuffer + sizeof( VERTEXBUFFERHANDLEDATA ) + sizeof( VERTEXBUFFERHANDLEDATA_PF ) + 31 ) / 32 * 32 ) ;

	// ����I��
	return 0 ;
}

// ���_�o�b�t�@�n���h���̏�����
extern int Graphics_VertexBuffer_InitializeHandle( HANDLEINFO *HandleInfo )
{
	VERTEXBUFFERHANDLEDATA *VertexBuffer = ( VERTEXBUFFERHANDLEDATA * )HandleInfo ;

	VertexBuffer->PF = ( VERTEXBUFFERHANDLEDATA_PF * )( VertexBuffer + 1 ) ;

	// ����I��
	return 0 ;
}

// ���_�o�b�t�@�n���h���̌�n��
extern int Graphics_VertexBuffer_TerminateHandle( HANDLEINFO *HandleInfo )
{
	VERTEXBUFFERHANDLEDATA *VertexBuffer = ( VERTEXBUFFERHANDLEDATA * )HandleInfo ;

	// ���ˑ���n������
	Graphics_Hardware_VertexBuffer_Terminate_PF( VertexBuffer ) ;

	// �I��
	return 0 ;
}

// �C���f�b�N�X�o�b�t�@���쐬����
extern int Graphics_IndexBuffer_Create( int IndexNum, int IndexType /* DX_INDEX_TYPE_16BIT �� */, int ASyncThread )
{
	int NewHandle ;
	int Result ;

	// �V�����n���h�����쐬����
	NewHandle = AddHandle( DX_HANDLETYPE_INDEX_BUFFER, ASyncThread, -1 ) ;
	if( NewHandle < 0 )
	{
		return -1 ;
	}

	// �n���h���̃Z�b�g�A�b�v
	Result = Graphics_IndexBuffer_SetupHandle( NewHandle, IndexNum, IndexType ) ;
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( NewHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD
	if( Result < 0 )
	{
		SubHandle( NewHandle ) ;
		return -1 ;
	}

	// �n���h����Ԃ�
	return NewHandle ;
}

// �C���f�b�N�X�o�b�t�@�n���h���̃Z�b�g�A�b�v���s��
extern int Graphics_IndexBuffer_SetupHandle( int IndexBufHandle, int IndexNum, int IndexType /* DX_INDEX_TYPE_16BIT �� */ )
{
	INDEXBUFFERHANDLEDATA *IndexBuffer ;
	int IndexSize ;

	// �G���[����
	if( INDEXBUFFERCHK( IndexBufHandle, IndexBuffer ) )
		return -1 ;

	// �Ή����Ă��Ȃ��^�C�v�̏ꍇ�̓G���[
	switch( IndexType )
	{
	case DX_INDEX_TYPE_16BIT :
		IndexSize = sizeof( WORD ) ;
		break ;

	case DX_INDEX_TYPE_32BIT :
		IndexSize = sizeof( DWORD ) ;
		break ;

	default :
		DXST_ERRORLOG_ADDUTF16LE( "\xa4\x30\xf3\x30\xc7\x30\xc3\x30\xaf\x30\xb9\x30\xbf\x30\xa4\x30\xd7\x30\x6e\x30\x24\x50\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x0a\x00\x00"/*@ L"�C���f�b�N�X�^�C�v�̒l���s���ł�\n" @*/ ) ;
		return -1 ;
	}

	// �^�C�v��ۑ�
	IndexBuffer->Type = IndexType ;

	// �C���f�b�N�X�f�[�^��ӂ�̃o�C�g����ۑ�
	IndexBuffer->UnitSize = IndexSize ;

	// �C���f�b�N�X����ۑ�
	IndexBuffer->Num = IndexNum ;

	// ���ˑ��̃C���f�b�N�X�o�b�t�@�쐬����
	if( Graphics_Hardware_IndexBuffer_Create_PF( IndexBuffer ) < 0 )
	{
		return -1 ;
	}

	// �f�[�^�̈���m�ۂ���
	if( ReallocHandle( IndexBufHandle, sizeof( INDEXBUFFERHANDLEDATA ) + sizeof( INDEXBUFFERHANDLEDATA_PF ) + IndexSize * IndexNum + 32 ) < 0 )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�������̊m�ۂɎ��s���܂���" @*/ ) ;
		return -1 ;
	}
	IndexBuffer     = ( INDEXBUFFERHANDLEDATA *    )GetHandleInfo( IndexBufHandle ) ;
	IndexBuffer->PF = ( INDEXBUFFERHANDLEDATA_PF * )( IndexBuffer + 1 ) ;

	// �V�X�e����������̃C���f�b�N�X�f�[�^��ۑ�����A�h���X���Z�b�g
	IndexBuffer->Buffer = ( void * )( ( ( DWORD_PTR )IndexBuffer + sizeof( INDEXBUFFERHANDLEDATA ) + sizeof( INDEXBUFFERHANDLEDATA_PF ) + 31 ) / 32 * 32 ) ;

	// ����I��
	return 0 ;
}

// �C���f�b�N�X�o�b�t�@�n���h���̏�����
extern int Graphics_IndexBuffer_InitializeHandle( HANDLEINFO *HandleInfo )
{
	INDEXBUFFERHANDLEDATA *IndexBuffer = ( INDEXBUFFERHANDLEDATA * )HandleInfo ;

	IndexBuffer->PF = ( INDEXBUFFERHANDLEDATA_PF * )( IndexBuffer + 1 ) ;

	// ����I��
	return 0 ;
}

// �C���f�b�N�X�o�b�t�@�n���h���̌�n��
extern int Graphics_IndexBuffer_TerminateHandle( HANDLEINFO *HandleInfo )
{
	INDEXBUFFERHANDLEDATA *IndexBuffer = ( INDEXBUFFERHANDLEDATA * )HandleInfo ;

	// ���ˑ���n������
	Graphics_Hardware_IndexBuffer_Terminate_PF( IndexBuffer ) ;

	// �I��
	return 0 ;
}



























// �V���h�E�}�b�v�֌W�֐�

// MakeShadowMap �̎������֐�
static int Graphics_ShadowMap_MakeShadowMap_Static(
	SETUP_SHADOWMAPHANDLE_GPARAM *GParam,
	int SmHandle,
	int SizeX,
	int SizeY,
	int ASyncThread
)
{
	// �n���h���̏�����
	if( Graphics_ShadowMap_SetupHandle_UseGParam(
			GParam,
			SmHandle,
			SizeX,
			SizeY,
			TRUE,
			16,
			ASyncThread
		) == -1 )
		return -1 ;

	// �I��
	return 0 ;
}

#ifndef DX_NON_ASYNCLOAD

// MakeShadowMap �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void Graphics_ShadowMap_MakeShadowMap_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	SETUP_SHADOWMAPHANDLE_GPARAM *GParam ;
	int SmHandle ;
	int SizeX ;
	int SizeY ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	GParam = ( SETUP_SHADOWMAPHANDLE_GPARAM * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	SmHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	SizeX = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	SizeY = GetASyncLoadParamInt( AParam->Data, &Addr ) ;

	Result = Graphics_ShadowMap_MakeShadowMap_Static( GParam, SmHandle, SizeX, SizeY, TRUE ) ;

	DecASyncLoadCount( SmHandle ) ;
	if( Result < 0 )
	{
		NS_DeleteGraph( SmHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// �V���h�E�}�b�v�n���h�����쐬����֐�
extern int Graphics_ShadowMap_MakeShadowMap_UseGParam(
	SETUP_SHADOWMAPHANDLE_GPARAM *GParam,
	int SizeX,
	int SizeY,
	int ASyncLoadFlag,
	int ASyncThread
)
{
	int SmHandle ;

	CheckActiveState() ;

	// �n���h���̍쐬
	SmHandle = Graphics_ShadowMap_AddHandle( ASyncThread ) ;
	if( SmHandle == -1 )
	{
		return -1 ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag && ASyncThread == FALSE )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamStruct( NULL, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( NULL, &Addr, SmHandle ) ;
		AddASyncLoadParamInt( NULL, &Addr, SizeX ) ;
		AddASyncLoadParamInt( NULL, &Addr, SizeY ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = Graphics_ShadowMap_MakeShadowMap_ASync ;
		Addr = 0 ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SmHandle ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SizeX ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SizeY ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( SmHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( Graphics_ShadowMap_MakeShadowMap_Static( GParam, SmHandle, SizeX, SizeY, ASyncThread ) < 0 )
			goto ERR ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( SmHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	// �I��
	return SmHandle ;

ERR :
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( SmHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	NS_DeleteGraph( SmHandle ) ;

	return -1 ;
}

// �V�����V���h�E�}�b�v�n���h�����m�ۂ���
extern int Graphics_ShadowMap_AddHandle( int ASyncThread )
{
	return AddHandle( DX_HANDLETYPE_SHADOWMAP, ASyncThread, -1 );
}

// �V���h�E�}�b�v�n���h���̃Z�b�g�A�b�v���s��
extern int Graphics_ShadowMap_SetupHandle_UseGParam( SETUP_SHADOWMAPHANDLE_GPARAM * /* GParam */, int SmHandle, int SizeX, int SizeY, int TexFormat_Float, int TexFormat_BitDepth, int /* ASyncThread */ )
{
	SHADOWMAPDATA *ShadowMap ;

	// �A�h���X�̎擾
	if( SHADOWMAPCHK_ASYNC( SmHandle, ShadowMap ) )
		return -1 ;

	// �p�����[�^�̕ۑ�
	ShadowMap->TexFormat_Float = TexFormat_Float ;
	ShadowMap->TexFormat_BitDepth = TexFormat_BitDepth ;
	ShadowMap->BaseSizeX = SizeX ;
	ShadowMap->BaseSizeY = SizeY ;
	ShadowMap->ShadowMapViewMatrix.m[ 3 ][ 3 ] = 1.0f ;

	// �e�N�X�`���̍쐬
	if( Graphics_ShadowMap_CreateTexture( ShadowMap ) == -1 )
	{
		goto ERR ;
	}

	// �I��
	return 0 ;

	// �G���[�I��
ERR :
	NS_DeleteGraph( SmHandle ) ;

	return -1 ;
}

// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����쐬����
extern int Graphics_ShadowMap_CreateTexture( SHADOWMAPDATA *ShadowMap, int ASyncThread )
{
	return Graphics_Hardware_ShadowMap_CreateTexture_PF( ShadowMap, ASyncThread ) ;
}

// �V���h�E�}�b�v�f�[�^�ɕK�v�ȃe�N�X�`�����������
extern int Graphics_ShadowMap_ReleaseTexture( SHADOWMAPDATA *ShadowMap )
{
	return Graphics_Hardware_ShadowMap_ReleaseTexture_PF( ShadowMap ) ;
}

// �V���h�E�}�b�v�f�[�^���n���h���l������o��
extern SHADOWMAPDATA *Graphics_ShadowMap_GetData( int SmHandle, int ASyncThread )
{
	SHADOWMAPDATA *ShadowMap ;

	// �G���[����
	if( ASyncThread )
	{
		if( SHADOWMAPCHK_ASYNC( SmHandle, ShadowMap ) )
			return NULL ;
	}
	else
	{
		if( SHADOWMAPCHK( SmHandle, ShadowMap ) )
			return NULL ;
	}

	return ShadowMap ;
}

// ���_�V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern void Graphics_ShadowMap_RefreshVSParam( void )
{
	Graphics_Hardware_ShadowMap_RefreshVSParam_PF() ;
}

// �s�N�Z���V�F�[�_�[�ɐݒ肷��V���h�E�}�b�v�̏����X�V����
extern void Graphics_ShadowMap_RefreshPSParam( void )
{
	Graphics_Hardware_ShadowMap_RefreshPSParam_PF() ;
}

// �V���h�E�}�b�v�ւ̃����_�����O�Ŏg�p����s��̏����X�V����
extern int Graphics_ShadowMap_RefreshMatrix( SHADOWMAPDATA *ShadowMap )
{
	VECTOR TempPos ;
	VECTOR MaxPos ;
	VECTOR MinPos ;
	VECTOR *SrcPos ;
	VECTOR EyePos ;
	VECTOR AreaSize ;
	int i ;

	// ���C�g�n�_����̍ő���W�ƍŏ����W������o��
	SrcPos = ShadowMap->EnableDrawArea ? ShadowMap->DrawAreaViewClipPos : ShadowMap->DefaultViewClipPos ;
	MinPos.x =  100000000.0f ;
	MinPos.y =  100000000.0f ;
	MinPos.z =  100000000.0f ;
	MaxPos.x = -100000000.0f ;
	MaxPos.y = -100000000.0f ;
	MaxPos.z = -100000000.0f ;
	for( i = 0 ; i < 8 ; i ++, SrcPos ++ )
	{
		VectorTransformSR( &TempPos, SrcPos, &ShadowMap->LightMatrix ) ;

		if( MinPos.x > TempPos.x ) MinPos.x = TempPos.x ;
		if( MinPos.y > TempPos.y ) MinPos.y = TempPos.y ;
		if( MinPos.z > TempPos.z ) MinPos.z = TempPos.z ;

		if( MaxPos.x < TempPos.x ) MaxPos.x = TempPos.x ;
		if( MaxPos.y < TempPos.y ) MaxPos.y = TempPos.y ;
		if( MaxPos.z < TempPos.z ) MaxPos.z = TempPos.z ;
	}

	// �V���h�E�}�b�v�`��p�r���[�s��̕��s�ړ�������ݒ�
	EyePos.x = ( MaxPos.x + MinPos.x ) / 2.0f ;
	EyePos.y = ( MaxPos.y + MinPos.y ) / 2.0f ;
	EyePos.z = MinPos.z ;
	ShadowMap->ShadowMapViewMatrix.m[ 3 ][ 0 ] = -EyePos.x ;
	ShadowMap->ShadowMapViewMatrix.m[ 3 ][ 1 ] = -EyePos.y ;
	ShadowMap->ShadowMapViewMatrix.m[ 3 ][ 2 ] = -EyePos.z ;

	// �V���h�E�}�b�v�`��p�̎ˉe�s��̊g�听����ݒ�
	AreaSize = VSub( MaxPos, MinPos ) ;
	ShadowMap->ShadowMapProjectionMatrix.m[ 0 ][ 0 ] = 2.0f / AreaSize.x ;
	ShadowMap->ShadowMapProjectionMatrix.m[ 1 ][ 1 ] = 2.0f / AreaSize.y ;
	ShadowMap->ShadowMapProjectionMatrix.m[ 2 ][ 2 ] = 1.0f / ( MaxPos.z - MinPos.z ) ;

	// �V���h�E�}�b�v�`��p�r���[�s��ƃV���h�E�}�b�v�`��p�ˉe�s�����Z�������̂�ۑ�
	CreateMultiplyMatrix( &ShadowMap->ShadowMapViewProjectionMatrix, &ShadowMap->ShadowMapViewMatrix, &ShadowMap->ShadowMapProjectionMatrix ) ;

	// �r���[�s��Ǝˉe�s��̃Z�b�g
	NS_SetTransformToView( &ShadowMap->ShadowMapViewMatrix ) ;
	NS_SetTransformToProjection( &ShadowMap->ShadowMapProjectionMatrix ) ;

	// �I��
	return 0 ;
}

// �V���h�E�}�b�v�n���h���̏�����
extern int Graphics_ShadowMap_InitializeHandle( HANDLEINFO *HandleInfo )
{
	SHADOWMAPDATA *ShadowMap = ( SHADOWMAPDATA * )HandleInfo ;

	ShadowMap->PF             = ( SHADOWMAPDATA_PF * )( ShadowMap + 1 ) ;

	ShadowMap->BaseSizeX      = -1 ;
	ShadowMap->BaseSizeY      = -1 ;

	ShadowMap->AdjustDepth    = DEFAULT_SHADOWMAP_ADJUST_DEPTH ;
	ShadowMap->BlurParam      = DEFAULT_SHADOWMAP_BLUR_PARAM ;
	ShadowMap->GradationParam = DEFAULT_SHADOWMAP_GRADATION_RANGE ;

	ShadowMap->LightMatrix.m[ 3 ][ 3 ]               = 1.0f ;
	ShadowMap->ShadowMapProjectionMatrix.m[ 3 ][ 3 ] = 1.0f ;
	CreateMultiplyMatrix( &ShadowMap->ShadowMapViewProjectionMatrix, &ShadowMap->ShadowMapViewMatrix, &ShadowMap->ShadowMapProjectionMatrix ) ;

	// �������C�g�����̐ݒ�
	NS_SetShadowMapLightDirection( HandleInfo->Handle, VGet( 0.0f, -1.0f, 0.0f ) ) ;

	// �I��
	return 0 ;
}

// �V���h�E�}�b�v�n���h���̌�n��
extern int Graphics_ShadowMap_TerminateHandle( HANDLEINFO *HandleInfo )
{
	SHADOWMAPDATA *ShadowMap = ( SHADOWMAPDATA * )HandleInfo ;
	int i ;

	// �`��Ŏg�p����V���h�E�}�b�v�ɐݒ肳��Ă��������
	for( i = 0 ; i < MAX_USE_SHADOWMAP_NUM ; i ++ )
	{
		if( GSYS.DrawSetting.ShadowMap[ i ] == HandleInfo->Handle )
		{
			NS_SetUseShadowMap( i, -1 ) ;
		}
	}

	// �V���h�E�}�b�v�̕`�揀�����s���Ă������������
	if( ShadowMap->DrawSetupFlag )
	{
		NS_ShadowMap_DrawEnd() ;
	}

	// �T�[�t�F�X��e�N�X�`�������
	Graphics_ShadowMap_ReleaseTexture( ShadowMap ) ;

	// ������ɗ��Ă�t���O�ւ̃|�C���^���L���ł���ꍇ�͗��Ă�
	if( ShadowMap->LostFlag != NULL )
		*ShadowMap->LostFlag = TRUE ;

	// �I��
	return 0 ;
}




























// �⏕�֌W�֐�

// ���L�������̊m��
extern int Graphics_Other_AllocCommonBuffer( int Index, DWORD Size )
{
	if( Index >= COMMON_BUFFER_NUM || Index < 0 )
	{
		return -1 ;
	}

	if( GSYS.Resource.CommonBufferSize[ Index ] >= Size )
	{
		return 0 ;
	}

	GSYS.Resource.CommonBuffer[ Index ] = DXREALLOC( GSYS.Resource.CommonBuffer[ Index ], Size ) ;
	if( GSYS.Resource.CommonBuffer[ Index ] == NULL )
	{
		return -1 ;
	}
	GSYS.Resource.CommonBufferSize[ Index ] = Size ;

	return 0 ;
}

// ���L�������̉��
extern int Graphics_Other_TerminateCommonBuffer( void ) 
{
	int i ;

	for( i = 0 ; i < COMMON_BUFFER_NUM ; i ++ )
	{
		if( GSYS.Resource.CommonBuffer[ i ] != NULL )
		{
			DXFREE( GSYS.Resource.CommonBuffer[ i ] ) ;
			GSYS.Resource.CommonBuffer[ i ] = 0 ;
		}

		GSYS.Resource.CommonBufferSize[ i ] = 0 ;
	}

	return 0 ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE


#endif // DX_NON_GRAPHICS
