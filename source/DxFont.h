// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�t�H���g�����p�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXFONT_H__
#define __DXFONT_H__

#include "DxCompileConfig.h"

#ifndef DX_NON_FONT

// �C���N���[�h ------------------------------------------------------------------
#include "DxLib.h"
#include "DxStatic.h"
#include "DxMemImg.h"
#include "DxHandle.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

#define FSYS FontSystem

// �t�H���g�̌��C���[�W�̃f�[�^�^�C�v
#define DX_FONT_SRCIMAGETYPE_1BIT				(0)					// 1�s�N�Z��1�r�b�g
#define DX_FONT_SRCIMAGETYPE_1BIT_SCALE4		(1)					// 1�s�N�Z��1�r�b�g�A�摜�𑜓x4�{
#define DX_FONT_SRCIMAGETYPE_1BIT_SCALE8		(2)					// 1�s�N�Z��1�r�b�g�A�摜�𑜓x8�{
#define DX_FONT_SRCIMAGETYPE_1BIT_SCALE16		(3)					// 1�s�N�Z��1�r�b�g�A�摜�𑜓x16�{
#define DX_FONT_SRCIMAGETYPE_4BIT_MAX15			(4)					// 1�s�N�Z��4�r�b�g�A�l�͈̔͂�0�`15
#define DX_FONT_SRCIMAGETYPE_8BIT_ON_OFF		(5)					// 1�s�N�Z��8�r�b�g�A�l�͈̔͂�0����0�ȊO
#define DX_FONT_SRCIMAGETYPE_8BIT_MAX16			(6)					// 1�s�N�Z��8�r�b�g�A�l�͈̔͂�0�`16
#define DX_FONT_SRCIMAGETYPE_8BIT_MAX64			(7)					// 1�s�N�Z��8�r�b�g�A�l�͈̔͂�0�`64
#define DX_FONT_SRCIMAGETYPE_8BIT_MAX255		(8)					// 1�s�N�Z��8�r�b�g�A�l�͈̔͂�0�`255

#define FONT_CACHE_MAXNUM						(2024)				// �t�H���g�L���b�V���Ɋi�[�ł���ő啶����
#define FONT_CACHE_EX_NUM						(1024)				// 0xffff �𒴂���R�[�h�̕����f�[�^�A�h���X��ێ����鐔
#define FONT_CACHE_MEMORYSIZE					(0x50000)			// �t�H���g�L���b�V���̍ő�e��
#define FONT_CACHE_MAX_YLENGTH					(0x4000)			// �t�H���g�L���b�V���T�[�t�F�X�̍ő�c��
#define FONT_GRAPHICHANDLE_IMAGE_MAXNUM			(256)				// ����̕����R�[�h���O���t�B�b�N�n���h���̉摜�ɒu����������ő吔

// �\���̒�` --------------------------------------------------------------------

// �t�H���g�񋓎��p�f�[�^�\����
struct ENUMFONTDATA
{
	wchar_t *				FontBuffer ;
	int						FontNum ;
	int						BufferNum ;
	int						JapanOnlyFlag ;
	int						Valid ;
	const wchar_t *			EnumFontName ;
} ;

// CreateFontToHandle �Ŏg�p�����O���[�o���ϐ���Z�߂�����
struct CREATEFONTTOHANDLE_GPARAM
{
	int						NotTextureFlag ;					// �쐬����t�H���g�L���b�V�����e�N�X�`���ɂ��邩�A�t���O
	int						TextureCacheColorBitDepth16Flag ;	// �쐬����t�H���g�L���b�V���e�N�X�`���̃J���[�r�b�g����16�r�b�g�ɂ��邩�ǂ����̃t���O
	int						CacheCharNum ;						// �t�H���g�L���b�V���ŃL���b�V���ł��镶���̐�
	int						UsePremulAlphaFlag ;				// ��Z�ς݃����g�p���邩�ǂ����̃t���O( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
} ;

// �t�H���g�̊�{���
struct FONTBASEINFO
{
	WORD					FontThickness ;						// �t�H���g�̑���
	WORD					FontSize ;							// �`�悷��t�H���g�̃T�C�Y
	WORD					FontHeight ;						// �`�悷��t�H���g�̍ő�c��
	WORD					FontAddHeight ;						// �T�C�Y�����ׂ̈ɑ���������
	BYTE					Italic ;							// �C�^���b�N�t�H���g���ǂ���(TRUE:�C�^���b�N FALSE:��C�^���b�N)
	BYTE					Padding ;
	WORD					MaxWidth ;							// �t�H���g�̍ő啝
	WORD					CharSet ;							// �L�����N�^�Z�b�g
	WORD					CharCodeFormat ;					// �����R�[�h�`��( 0xffff �̏ꍇ�͓��Ɏw��Ȃ� )
	WORD					Ascent ;							// �����̃x�[�X���C�������ԏ�܂ł̍���
	WORD					Padding2 ;
} ;

// �t�H���g�t�@�C���̃w�b�_���̈��k���镔��
struct FONTDATAFILEPRESSHEADER
{
	WORD					FontName[ 128 ] ;	// �t�H���g�̖��O( UTF-16LE )
	FONTBASEINFO			BaseInfo ;			// ��{���
	BYTE					ImageBitDepth ;		// �摜�̃r�b�g�[�x( DX_FONTIMAGE_BIT_1 �� )
	BYTE					Padding[ 3 ] ;
	DWORD					Padding2[ 16 ] ;
} ;

// �t�H���g�t�@�C���̃w�b�_���
struct FONTDATAFILEHEADER
{
	BYTE					Magic[ 4 ] ;		// FNTF
	WORD					Version ;			// �o�[�W����
	WORD					Padding ;
	DWORD					ImageAddress ;		// �摜�f�[�^���ۑ�����Ă���擪�A�h���X
	DWORD					MaxImageBytes ;		// �摜�f�[�^�̍ő�T�C�Y
	DWORD					CharaNum ;			// �������̐�
	DWORD					CharaExNum ;		// �������̓��A�����R�[�h�� 0x10000 �ȏ�̕����̐�
	FONTDATAFILEPRESSHEADER	Press ;				// �w�b�_�t�@�C�����̈��k�Ώۂ̕���
} ;

// �t�@�C���ۑ��p�̃t�H���g�P�����̏��
struct FONTDATAFILECHARADATA
{
	DWORD					CodeUnicode ;		// Unicode �̕����R�[�h
	BYTE					Press ;				// ���k���Ă��邩�ǂ���( 1:���k���Ă���  0:���k���Ă��Ȃ� )
	BYTE					Padding ;
	short					DrawX ;				// �����摜��`�悷�ׂ��w���W
	short					DrawY ;				// �����摜��`�悷�ׂ��x���W
	short					AddX ;				// ���̕�����`�悷�ׂ����΂w���W
	WORD					SizeX ;				// �����̂w�T�C�Y
	WORD					SizeY ;				// �����̂x�T�C�Y
	DWORD					ImageAddress ;		// �摜�f�[�^���ۑ�����Ă���A�h���X( FONTDATAFILEHEADER.ImageAddress �̎����A�h���X�� 0 �Ƃ��� )
	DWORD					ImagePitch ;		// �摜�f�[�^�̂P���C��������̃o�C�g��
	DWORD					Padding2[ 2 ] ;
} ;

// �t�H���g�P�����̃L���b�V�����
struct FONTCHARDATA
{
	DWORD					CodeUnicode ;		// Unicode �̕����R�[�h
	BYTE					ValidFlag ;			// �L���t���O
	BYTE					GraphHandleFlag ;	// GraphIndex ���O���t�B�b�N�n���h�����ǂ����̃t���O( TRUE:�O���t�B�b�N�n���h��  FALSE:�����f�[�^�̃C���f�b�N�X )
	short					DrawX ;				// �����摜��`�悷�ׂ��w���W
	short					DrawY ;				// �����摜��`�悷�ׂ��x���W
	short					AddX ;				// ���̕�����`�悷�ׂ����΂w���W
	WORD					SizeX ;				// �����摜�̕�
	WORD					SizeY ;				// �����摜�̍���
	int						GraphIndex ;		// �����f�[�^�̃C���f�b�N�X( �Ⴕ���̓O���t�B�b�N�n���h�� )
	int						GraphIndexX ;		// �����f�[�^�̂w�}�X�ʒu
	int						GraphIndexY ;		// �����f�[�^�̂x�}�X�ʒu
	struct FONTCODEDATA *	CodeData ;			// ���̃t�H���g���Ǘ����Ă���f�[�^�̃|�C���^
} ;

// �t�H���g�L���b�V�����݃t���O�\����
struct FONTCODEDATA
{
	int						ExistFlag ;			// �L���b�V���̑��݃t���O
	FONTCHARDATA *			CharData ;			// �L���b�V���f�[�^�̑��݈ʒu
} ;

// �t�H���g�f�[�^�t�@�C���^�p���Ɏg�p����ϐ���Z�߂��\����
struct FONTDATAFILEUSEINFO
{
	BYTE *					FileBuffer ;						// �t�H���g�f�[�^�t�@�C���̒��g���i�[�����������̈�̐擪�A�h���X
	FONTDATAFILEHEADER *	Header ;							// �t�H���g�f�[�^�t�@�C���w�b�_
	FONTDATAFILECHARADATA *	Chara ;								// �t�H���g�f�[�^�t�@�C�����̊e�����̏��
	BYTE *					Image ;								// �t�H���g�f�[�^�t�@�C���̉摜�f�[�^�̃A�h���X
	int						ImageType ;							// �t�H���g�f�[�^�t�@�C���̃C���[�W�f�[�^�^�C�v( DX_FONT_SRCIMAGETYPE_1BIT �� )
	FONTDATAFILECHARADATA **CharaTable ;						// �t�H���g�f�[�^�t�@�C�����̊e�����̏��ւ̃A�h���X�̃e�[�u��( �R�[�h 0x0000�`0xffff �͈̔� )
	FONTDATAFILECHARADATA **CharaExArray ;						// 0xffff �𒴂��镶���R�[�h�̕������ւ̃A�h���X
	void *					PressImageDecodeBuffer ;			// �𓀂��������C���[�W���i�[����o�b�t�@
} ;

// �t�H���g�L���b�V���̊Ǘ��f�[�^
struct FONTMANAGE
{
	HANDLEINFO				HandleInfo ;						// �n���h�����ʃf�[�^

	struct FONTMANAGE_PF *	PF ;								// ���ˑ��f�[�^

	int *					LostFlag ;							// ������� TRUE �ɂ���t���O�ւ̃|�C���^

	int						UseFontDataFile ;					// �t�H���g�f�[�^�t�@�C�����g�p���Ă��邩�ǂ���( TRUE:�g�p���Ă���  FALSE:�g�p���Ă��Ȃ� )
	FONTDATAFILEUSEINFO		FontDataFile ;						// �t�H���g�f�[�^�t�@�C���̏��

	FONTCHARDATA			CharData[ FONT_CACHE_MAXNUM + 1 ] ;	// �L���b�V���f�[�^
	FONTCODEDATA			CodeData[ 0x10000 ] ;				// ���݃f�[�^���܂ރf�[�^�z��
	int						CodeDataExNum ;						// 0xffff �𒴂��镶���R�[�h�̕����f�[�^�̐�
	FONTCHARDATA *			CodeDataEx[ FONT_CACHE_EX_NUM ] ;	// 0xffff �𒴂��镶���R�[�h�̕����f�[�^�ւ̃|�C���^
	unsigned int			Index ;								// ���f�[�^�ǉ����̔z��C���f�b�N�X
	int						MaxCacheCharNum ;					// �L���b�V���ł���ő吔
	unsigned char *			CacheMem ;							// �e�L�X�g�L���b�V��������
	int						CachePitch ;						// �e�L�X�g�L���b�V���������̃s�b�`	
	int						CacheDataBitNum ;					// �e�L�X�g�L���b�V����̂P�s�N�Z���̃r�b�g��

	int						GraphHandleFontImageNum ;			// ����̕����R�[�h���O���t�B�b�N�n���h���̉摜�ɒu�������Ă��鐔
	FONTCHARDATA			GraphHandleFontImage[ FONT_GRAPHICHANDLE_IMAGE_MAXNUM ] ;	// ����̕����R�[�h���O���t�B�b�N�n���h���̉摜�ɒu�������鏈���p�̃f�[�^

	wchar_t					FontName[ 128 ] ;					// �t�H���g��
	FONTBASEINFO			BaseInfo ;							// ��{���
	int						UseCharCodeFormat ;					// �����œn�������R�[�h�`��
	int						Space ;								// ���̕�����\��������W�ɉ��Z�h�b�g��
	int						LineSpaceValidFlag ;				// LineSpace ���L�����ǂ����̃t���O
	int						LineSpace ;							// ���s�̍ۂɍ��W�ɉ��Z����h�b�g��
	int						FontType ;							// �t�H���g�̃^�C�v
	int						EdgeSize ;							// �G�b�W�̑���

	SIZE					CacheImageSize ;					// �L���b�V���摜�̂̃T�C�Y 
	int						LengthCharNum ;						// �L���b�V���摜�̏c�ɕ��ׂ��镶���̐�

	BYTE *					TempBuffer ;						// �t�H���g�̍ő�T�C�Y���̂P�s�N�Z���P�o�C�g�̍�Ɨp�o�b�t�@
	DWORD					TempBufferSize ;					// ��Ɨp�o�b�t�@�̃T�C�Y

	int						TextureCache ;						// �e�N�X�`���ɃL���b�V������ꍇ�̃e�N�X�`���O���t�B�b�N�n���h��
	int						TextureCacheSub ;					// �e�N�X�`���ɃL���b�V������ꍇ�̃e�N�X�`���O���t�B�b�N�n���h��(���p)
	int						TextureCacheLostFlag ;				// TextureCache �������ɂȂ����Ƃ��� TRUE �ɂȂ�ϐ�
	BASEIMAGE				TextureCacheBaseImage ;				// TextureCache �ɓ]���������̂Ɠ������̂��i�[������{�C���[�W
	int						TextureCacheColorBitDepth ;			// �e�N�X�`���L���b�V���̃J���[�r�b�g��
	int						TextureCacheUsePremulAlpha ;		// �e�N�X�`���L���b�V���̃��`�����l������Z�ς݃��ɂ��邩�ǂ���
	int						TextureCacheSizeX ;					// �e�N�X�`���L���b�V���̕�
	int						TextureCacheSizeY ;					// �e�N�X�`���L���b�V���̍���
	float					TextureCacheInvSizeX ;				// �e�N�X�`���L���b�V���̕��̋t��
	float					TextureCacheInvSizeY ;				// �e�N�X�`���L���b�V���̍����̋t��

	int						TextureCacheUpdateRectValid ;		// �e�N�X�`���L���b�V���X�V��`��񂪗L�����ǂ���
	RECT					TextureCacheUpdateRect ;			// �e�N�X�`���L���b�V���X�V��`���

	int						TextureCacheFlag ;					// �e�N�X�`���L���b�V�������Ă��邩�A�t���O
} ;

// �t�H���g�V�X�e���p�\����
struct FONTSYSTEM
{
	int						InitializeFlag ;					// �������t���O

	int						NotTextureFlag ;					// �쐬����t�H���g�L���b�V�����e�N�X�`���ɂ��邩�A�t���O
	int						TextureCacheColorBitDepth16Flag ;	// �쐬����t�H���g�L���b�V���e�N�X�`���̃J���[�r�b�g����16�r�b�g�ɂ��邩�ǂ����̃t���O
	int						CacheCharNum ;						// �t�H���g�L���b�V���ŃL���b�V���ł��镶���̐�
	int						UsePremulAlphaFlag ;				// ��Z�ς݃����g�p���邩�ǂ����̃t���O( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
	BYTE					BitCountTable[ 256 ] ;				// �r�b�g�J�E���g�e�[�u��
	BYTE					MAX15ToMAX16[ 16 ] ;				// 0�`15  �̒l�� 0�`16 �ɕϊ����邽�߂̃e�[�u��
	BYTE					MAX255ToMAX16[ 256 ] ;				// 0�`255 �̒l�� 0�`16 �ɕϊ����邽�߂̃e�[�u��
	BYTE					MAX15ToMAX64[ 16 ] ;				// 0�`15  �̒l�� 0�`64 �ɕϊ����邽�߂̃e�[�u��
	BYTE					MAX255ToMAX64[ 256 ] ;				// 0�`255 �̒l�� 0�`64 �ɕϊ����邽�߂̃e�[�u��

	wchar_t					DoubleByteSpaceCharCode ;			// �S�p�X�y�[�X�� wchar_t �R�[�h

	int						UseDefaultFontImage ;				// �f�t�H���g�t�H���g�C���[�W���g�p���Ă��邩�ǂ���( TRUE:�g�p���Ă���  FALSE:�g�p���Ă��Ȃ� )
	void *					DefaultFontImage ;					// �f�t�H���g�t�H���g�C���[�W���i�[���Ă���A�h���X��ۑ�����|�C���^
	int						DefaultFontImageGraphHandle[ 8 ][ 16 ] ;	// �f�t�H���g�t�H���g�C���[�W�̃O���t�B�b�N�n���h��

	int						DefaultFontHandle ;					// �f�t�H���g�Ŏg�p����t�H���g�̃n���h��

	int						EnableInitDefaultFontName ;			// �f�t�H���g�Ŏg�p����t�H���g�̐ݒ�
	wchar_t					DefaultFontName[ 256 ] ;

	int						EnableInitDefaultFontSize ;
	int						DefaultFontSize ;

	int						EnableInitDefaultFontThick ;
	int						DefaultFontThick ;

	int						EnableInitDefaultFontType ;
	int						DefaultFontType ;

	int						EnableInitDefaultFontCharSet ;
	int						DefaultFontCharSet ;

	int						EnableInitDefaultFontEdgeSize ;
	int						DefaultFontEdgeSize ;

	int						EnableInitDefaultFontItalic ;
	int						DefaultFontItalic ;

	int						EnableInitDefaultFontSpace ;
	int						DefaultFontSpace ;

	int						EnableInitDefaultFontLineSpace ;
	int						DefaultFontLineSpaceValid ;
	int						DefaultFontLineSpace ;
} ;

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

extern FONTSYSTEM FontSystem ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

extern	int			InitFontManage( void ) ;																					// �t�H���g�V�X�e���̏�����
extern	int			TermFontManage( void ) ;																					// �t�H���g����̏I��
extern	int			InitCacheFontToHandle( void ) ;																				// �t�H���g�̃L���b�V����������������
extern	int			InitFontCacheToHandle( FONTMANAGE *ManageData, int ASyncThread = FALSE ) ;											// ����̃t�H���g�̃L���b�V����������������

extern	int			InitializeFontHandle( HANDLEINFO *HandleInfo ) ;															// �t�H���g�n���h��������������
extern	int			TerminateFontHandle( HANDLEINFO *HandleInfo ) ;																// �t�H���g�n���h���̌�n��������

extern	int			RefreshFontDrawResourceToHandle( FONTMANAGE *ManageData, int ASyncThread = FALSE ) ;								// �t�H���g�n���h�����g�p����`��o�b�t�@��e�N�X�`���L���b�V�����ď���������

extern	int			FontCacheStringDrawToHandleST(
							int DrawFlag,
							int   xi, int   yi,
							float xf, float yf, int PosIntFlag,
							int ExRateValidFlag,
							double ExRateX, double ExRateY,
							int RotateValidFlag,
							float RotCenterX, float RotCenterY, double RotAngle, 
							const wchar_t *StrData,
							unsigned int Color, MEMIMG *DestMemImg, const RECT *ClipRect,
							int TransFlag, FONTMANAGE *ManageData, unsigned int EdgeColor,
							int StrLen, int VerticalFlag, SIZE *DrawSize,
							int *LineCount, 
							DRAWCHARINFO *CharInfos, size_t CharInfoBufferSize, int *CharInfoNum ) ;
extern	int			RefreshDefaultFont( void ) ;																				// �f�t�H���g�t�H���g���č쐬����
extern	int			InitFontToHandleBase( int Terminate = FALSE ) ;																// InitFontToHandle �̓����֐�

extern	FONTMANAGE *GetFontManageDataToHandle( int FontHandle ) ;																// �t�H���g�Ǘ��f�[�^�̎擾

extern	void		InitCreateFontToHandleGParam( CREATEFONTTOHANDLE_GPARAM *GParam ) ;											// CREATEFONTTOHANDLE_GPARAM �̃f�[�^���Z�b�g����

extern	int			CreateFontToHandle_UseGParam(          CREATEFONTTOHANDLE_GPARAM *GParam, const wchar_t *FontName, int Size, int Thick, int FontType, int CharSet, int EdgeSize, int Italic, int Handle, int ASyncLoadFlag = FALSE ) ;	// CreateFontToHandle �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int			LoadFontDataFromMemToHandle_UseGParam( CREATEFONTTOHANDLE_GPARAM *GParam, const void *FontDataImage, int FontDataImageSize, int EdgeSize = -1, int ASyncLoadFlag = FALSE ) ;						// LoadFontDataFromMemToHandle �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int			LoadFontDataToHandle_UseGParam(        CREATEFONTTOHANDLE_GPARAM *GParam, const wchar_t FileName, int EdgeSize, int ASyncLoadFlag = FALSE ) ;																// LoadFontDataToHandle �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����

extern	int			SetupFontCache( CREATEFONTTOHANDLE_GPARAM *GParam, FONTMANAGE *ManageData, int ASyncThread ) ;											// �����L���b�V���̃Z�b�g�A�b�v���s��
extern	FONTCHARDATA *	FontCacheCharAddToHandle( int AddNum, const DWORD *CharCode, FONTMANAGE *ManageData, int TextureCacheUpdate = TRUE ) ;	// �����L���b�V���ɐV����������������
extern	int				FontCacheCharImageBltToHandle( FONTMANAGE *ManageData, FONTCHARDATA *CharData, DWORD CharCode, int Space, int ImageType /* DX_FONT_SRCIMAGETYPE_1BIT �� */, void *ImageBuffer, DWORD ImageSizeX, DWORD ImageSizeY, DWORD ImagePitch, int ImageDrawX, int ImageDrawY, int ImageAddX, int TextureCacheUpdate ) ;		// �w��̃t�H���g�f�[�^�ɉ摜��]������
extern	int			FontTextureCacheAddUpdateRect( FONTMANAGE *ManageData, RECT *Rect ) ;	// �t�H���g�̃e�N�X�`���L���b�V���̍X�V��`��ǉ�����
extern	int			FontTextureCacheUpdateRectApply( FONTMANAGE *ManageData ) ;				// �t�H���g�̃e�N�X�`���L���b�V���̍X�V��`�����ۂ̃e�N�X�`���ɓK�p����

extern	int			GetFontHandleCharCodeFormat( int FontHandle ) ;		// �t�H���g�n���h���ɐݒ肳��Ă��镶���R�[�h�`�����擾����( �߂�l  -1:�G���[  -1�ȊO:�����R�[�h�`�� )

// �w��̕����R�[�h�̃t�H���g�L���b�V���f�[�^���擾����A�L���b�V�����ɖ����ꍇ�̓L���b�V���ւ̒ǉ������݂āA���s������ NULL ��Ԃ�
extern	FONTCHARDATA *GetFontCacheChar( FONTMANAGE *ManageData, DWORD CharCode, int ErrorMessage = TRUE ) ;


// wchar_t�Ŋ֐�
extern	int			EnumFontName_WCHAR_T(							wchar_t *NameBuffer, int NameBufferNum, int JapanOnlyFlag = TRUE ) ;													// �g�p�\�ȃt�H���g�̖��O��񋓂���( NameBuffer �� 64�o�C�g��؂�Ŗ��O���i�[����܂� )
extern	int			EnumFontNameEx_WCHAR_T(							wchar_t *NameBuffer, int NameBufferNum,                              int CharSet = -1 /* DX_CHARSET_DEFAULT �� */ ) ;	// �g�p�\�ȃt�H���g�̖��O��񋓂���( NameBuffer �� 64�o�C�g��؂�Ŗ��O���i�[����܂� )( �����Z�b�g�w��� )
extern	int			EnumFontNameEx2_WCHAR_T(						wchar_t *NameBuffer, int NameBufferNum, const wchar_t *EnumFontName, int CharSet = -1 /* DX_CHARSET_DEFAULT �� */ ) ;	// �w��̃t�H���g���̃t�H���g��񋓂���
extern	int			CheckFontName_WCHAR_T(							const wchar_t *FontName, int CharSet = -1 /* DX_CHARSET_DEFAULT �� */ ) ;												// �w��̃t�H���g���̃t�H���g�����݂��邩�ǂ������`�F�b�N����( �߂�l  TRUE:���݂���  FALSE:���݂��Ȃ� )

extern	int			CreateFontToHandle_WCHAR_T(                     const wchar_t *FontName, int Size, int Thick, int FontType = -1 , int CharSet = -1 , int EdgeSize = -1 , int Italic = FALSE , int Handle = -1 ) ;		// �t�H���g�n���h�����쐬����
extern	int			LoadFontDataToHandle_WCHAR_T(					const wchar_t *FileName,                      int EdgeSize = -1 ) ;			// �t�H���g�f�[�^�t�@�C������t�H���g�n���h�����쐬����
extern	int			ChangeFont_WCHAR_T(                             const wchar_t *FontName, int CharSet = -1 /* DX_CHARSET_SHFTJIS �� */ ) ;							// �f�t�H���g�t�H���g�n���h���Ŏg�p����t�H���g��ύX
extern	int			SetDefaultFontState_WCHAR_T(                    const wchar_t *FontName, int Size, int Thick, int FontType = -1 , int CharSet = -1 , int EdgeSize = -1 , int Italic = FALSE ) ;	// �f�t�H���g�t�H���g�n���h���̐ݒ��ύX����
extern	int			AddFontImageToHandle_WCHAR_T(					int FontHandle, const wchar_t *Char, int GrHandle, int DrawX, int DrawY, int AddX ) ;				// �w��̕����̑���ɕ`�悷��O���t�B�b�N�n���h����o�^����
extern	int			SubFontImageToHandle_WCHAR_T(					int FontHandle, const wchar_t *Char ) ;															// �w��̕����̑���ɕ`�悷��O���t�B�b�N�n���h���̓o�^����������

extern	int			GetDrawStringWidth_WCHAR_T(                     const wchar_t *String, int StrLen, int VerticalFlag = FALSE ) ;									// �f�t�H���g�t�H���g�n���h�����g�p����������̕`�敝���擾����
extern	int			GetDrawFormatStringWidth_WCHAR_T(               const wchar_t *FormatString, ... ) ;																// �f�t�H���g�t�H���g�n���h�����g�p���������t��������̕`�敝���擾����
extern	int			GetDrawExtendStringWidth_WCHAR_T(               double ExRateX, const wchar_t *String, int StrLen, int VerticalFlag = FALSE ) ;					// �f�t�H���g�t�H���g�n���h�����g�p����������̕`�敝���擾����( �g�嗦�t�� )
extern	int			GetDrawExtendFormatStringWidth_WCHAR_T(         double ExRateX, const wchar_t *FormatString, ... ) ;												// �f�t�H���g�t�H���g�n���h�����g�p���������t��������̕`�敝���擾����( �g�嗦�t�� )

extern	int			GetFontCharInfo_WCHAR_T(                        int FontHandle, const wchar_t *Char, int *DrawX, int *DrawY, int *NextCharX, int *SizeX, int *SizeY ) ;	// �t�H���g�n���h���̎w��̕����̕`������擾����
extern	int			GetDrawStringWidthToHandle_WCHAR_T(             const wchar_t   *String, int StrLen, int FontHandle, int VerticalFlag = FALSE ) ;					// �t�H���g�n���h�����g�p����������̕`�敝���擾����
extern	int			GetDrawFormatStringWidthToHandle_WCHAR_T(       int FontHandle, const wchar_t *FormatString, ... ) ;												// �t�H���g�n���h�����g�p���������t��������̕`�敝���擾����
extern	int			GetDrawExtendStringWidthToHandle_WCHAR_T(       double ExRateX, const wchar_t *String, int StrLen, int FontHandle, int VerticalFlag = FALSE ) ;	// �t�H���g�n���h�����g�p����������̕`�敝���擾����
extern	int			GetDrawExtendFormatStringWidthToHandle_WCHAR_T( double ExRateX, int FontHandle, const wchar_t *FormatString, ... ) ;								// �t�H���g�n���h�����g�p���������t��������̕`�敝���擾����
extern	int			GetDrawStringSizeToHandle_WCHAR_T(              int *SizeX, int *SizeY, int *LineCount, const wchar_t   *String, int StrLen, int FontHandle, int VerticalFlag = FALSE ) ;					// �t�H���g�n���h�����g�p����������̕`�敝�E�����E�s�����擾����
extern	int			GetDrawExtendStringSizeToHandle_WCHAR_T(        int *SizeX, int *SizeY, int *LineCount, double ExRateX, double ExRateY, const wchar_t *String, int StrLen, int FontHandle, int VerticalFlag = FALSE ) ;	// �t�H���g�n���h�����g�p����������̕`�敝�E�����E�s�����擾����
extern	int			GetDrawStringCharInfoToHandle_WCHAR_T(          DRAWCHARINFO *InfoBuffer, size_t InfoBufferSize, const wchar_t   *String, int StrLen, int FontHandle, int VerticalFlag = FALSE ) ;									// �t�H���g�n���h�����g�p����������̂P�������̏����擾����
extern	int			GetDrawExtendStringCharInfoToHandle_WCHAR_T(    DRAWCHARINFO *InfoBuffer, size_t InfoBufferSize, double ExRateX, double ExRateY, const wchar_t *String, int StrLen, int FontHandle, int VerticalFlag = FALSE ) ;	// �t�H���g�n���h�����g�p����������̂P�������̏����擾����
extern	int			GetFontStateToHandle_WCHAR_T(                   wchar_t   *FontName, int *Size, int *Thick, int FontHandle, int *FontType = NULL , int *CharSet = NULL , int *EdgeSize = NULL , int *Italic = NULL ) ;		// �t�H���g�n���h���̏����擾����

extern	int			FontCacheStringDrawToHandle_WCHAR_T(            int x, int y, const wchar_t   *StrData, unsigned int Color, unsigned int EdgeColor, BASEIMAGE *DestImage, const RECT *ClipRect /* NULL �� */ , int FontHandle, int VerticalFlag = FALSE , SIZE *DrawSizeP = NULL ) ;
extern	int			FontBaseImageBlt_WCHAR_T(                       int x, int y, const wchar_t   *StrData, BASEIMAGE *DestImage, BASEIMAGE *DestEdgeImage,                 int VerticalFlag = FALSE ) ;	// ��{�C���[�W�ɕ������`�悷��( �f�t�H���g�t�H���g�n���h�����g�p���� )
extern	int			FontBaseImageBltToHandle_WCHAR_T(               int x, int y, const wchar_t   *StrData, BASEIMAGE *DestImage, BASEIMAGE *DestEdgeImage, int FontHandle, int VerticalFlag = FALSE ) ;	// ��{�C���[�W�ɕ������`�悷��

extern	int			DrawString_WCHAR_T(                             int x, int y,                                              const wchar_t *String, unsigned int Color, unsigned int EdgeColor = 0 ) ;							// �f�t�H���g�t�H���g�n���h�����g�p���ĕ������`�悷��
extern	int			DrawVString_WCHAR_T(                            int x, int y,                                              const wchar_t *String, unsigned int Color, unsigned int EdgeColor = 0 ) ;							// �f�t�H���g�t�H���g�n���h�����g�p���ĕ������`�悷��( �c���� )
extern	int			DrawFormatString_WCHAR_T(                       int x, int y,                                 unsigned int Color, const wchar_t *FormatString, ... ) ;														// �f�t�H���g�t�H���g�n���h�����g�p���ď����w�蕶�����`�悷��
extern	int			DrawFormatVString_WCHAR_T(                      int x, int y,                                 unsigned int Color, const wchar_t *FormatString, ... ) ;														// �f�t�H���g�t�H���g�n���h�����g�p���ď����w�蕶�����`�悷��( �c���� )
extern	int			DrawExtendString_WCHAR_T(                       int x, int y, double ExRateX, double ExRateY,              const wchar_t *String, unsigned int Color, unsigned int EdgeColor = 0 ) ;							// �f�t�H���g�t�H���g�n���h�����g�p���ĕ�����̊g��`��
extern	int			DrawExtendVString_WCHAR_T(                      int x, int y, double ExRateX, double ExRateY,              const wchar_t *String, unsigned int Color, unsigned int EdgeColor = 0 ) ;							// �f�t�H���g�t�H���g�n���h�����g�p���ĕ�����̊g��`��( �c���� )
extern	int			DrawExtendFormatString_WCHAR_T(                 int x, int y, double ExRateX, double ExRateY, unsigned int Color, const wchar_t *FormatString, ... ) ;														// �f�t�H���g�t�H���g�n���h�����g�p���ď����w�蕶������g��`�悷��
extern	int			DrawExtendFormatVString_WCHAR_T(                int x, int y, double ExRateX, double ExRateY, unsigned int Color, const wchar_t *FormatString, ... ) ;														// �f�t�H���g�t�H���g�n���h�����g�p���ď����w�蕶������g��`�悷��( �c���� )
extern	int			DrawRotaString_WCHAR_T(							int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, unsigned int EdgeColor = 0 , int VerticalFlag = FALSE , const wchar_t *String       = NULL       ) ;		// �f�t�H���g�t�H���g�n���h�����g�p���ĕ��������]�`�悷��
extern	int			DrawRotaFormatString_WCHAR_T(					int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, unsigned int EdgeColor = 0 , int VerticalFlag = FALSE , const wchar_t *FormatString = NULL , ... ) ;		// �f�t�H���g�t�H���g�n���h�����g�p���ď����w�蕶�������]�`�悷��

extern	int			DrawStringF_WCHAR_T(                            float x, float y,                                              const wchar_t *String, unsigned int Color, unsigned int EdgeColor = 0 ) ;						// �f�t�H���g�t�H���g�n���h�����g�p���ĕ������`�悷��( ���W�w�肪 float �� )
extern	int			DrawVStringF_WCHAR_T(                           float x, float y,                                              const wchar_t *String, unsigned int Color, unsigned int EdgeColor = 0 ) ;						// �f�t�H���g�t�H���g�n���h�����g�p���ĕ������`�悷��( �c���� )( ���W�w�肪 float �� )
extern	int			DrawFormatStringF_WCHAR_T(                      float x, float y,                                 unsigned int Color, const wchar_t *FormatString, ... ) ;													// �f�t�H���g�t�H���g�n���h�����g�p���ď����w�蕶�����`�悷��( ���W�w�肪 float �� )
extern	int			DrawFormatVStringF_WCHAR_T(                     float x, float y,                                 unsigned int Color, const wchar_t *FormatString, ... ) ;													// �f�t�H���g�t�H���g�n���h�����g�p���ď����w�蕶�����`�悷��( �c���� )( ���W�w�肪 float �� )
extern	int			DrawExtendStringF_WCHAR_T(                      float x, float y, double ExRateX, double ExRateY,              const wchar_t *String, unsigned int Color, unsigned int EdgeColor = 0 ) ;						// �f�t�H���g�t�H���g�n���h�����g�p���ĕ�����̊g��`��( ���W�w�肪 float �� )
extern	int			DrawExtendVStringF_WCHAR_T(                     float x, float y, double ExRateX, double ExRateY,              const wchar_t *String, unsigned int Color, unsigned int EdgeColor = 0 ) ;						// �f�t�H���g�t�H���g�n���h�����g�p���ĕ�����̊g��`��( �c���� )( ���W�w�肪 float �� )
extern	int			DrawExtendFormatStringF_WCHAR_T(                float x, float y, double ExRateX, double ExRateY, unsigned int Color, const wchar_t *FormatString, ... ) ;													// �f�t�H���g�t�H���g�n���h�����g�p���ď����w�蕶������g��`�悷��( ���W�w�肪 float �� )
extern	int			DrawExtendFormatVStringF_WCHAR_T(               float x, float y, double ExRateX, double ExRateY, unsigned int Color, const wchar_t *FormatString, ... ) ;													// �f�t�H���g�t�H���g�n���h�����g�p���ď����w�蕶������g��`�悷��( �c���� )( ���W�w�肪 float �� )
extern	int			DrawRotaStringF_WCHAR_T(						float x, float y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, unsigned int EdgeColor = 0 , int VerticalFlag = FALSE , const wchar_t *String       = NULL       ) ;		// �f�t�H���g�t�H���g�n���h�����g�p���ĕ��������]�`�悷��( ���W�w�肪 float �� )
extern	int			DrawRotaFormatStringF_WCHAR_T(					float x, float y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, unsigned int EdgeColor = 0 , int VerticalFlag = FALSE , const wchar_t *FormatString = NULL , ... ) ;		// �f�t�H���g�t�H���g�n���h�����g�p���ď����w�蕶�������]�`�悷��( ���W�w�肪 float �� )

extern	int			DrawNumberPlusToI_WCHAR_T(                      int x, int y, const wchar_t *NoteString, int    Num, int RisesNum, unsigned int Color, unsigned int EdgeColor = 0 ) ;											// �f�t�H���g�t�H���g�n���h�����g�p���Đ����^�̐��l�Ƃ��̐����̕��������x�ɕ`�悷��
extern 	int			DrawNumberPlusToF_WCHAR_T(                      int x, int y, const wchar_t *NoteString, double Num, int Length,   unsigned int Color, unsigned int EdgeColor = 0 ) ;											// �f�t�H���g�t�H���g�n���h�����g�p���ĕ��������_�^�̐��l�Ƃ��̐����̕��������x�ɕ`�悷��

extern	int			DrawStringToZBuffer_WCHAR_T(                    int x, int y, const wchar_t *String,                                                 int WriteZMode /* DX_ZWRITE_MASK �� */ ) ;									// �f�t�H���g�t�H���g�n���h�����g�p���Ăy�o�b�t�@�ɑ΂��ĕ������`�悷��
extern	int			DrawVStringToZBuffer_WCHAR_T(                   int x, int y, const wchar_t *String,                                                 int WriteZMode /* DX_ZWRITE_MASK �� */ ) ;									// �f�t�H���g�t�H���g�n���h�����g�p���Ăy�o�b�t�@�ɑ΂��ĕ������`�悷��( �c���� )
extern	int			DrawFormatStringToZBuffer_WCHAR_T(              int x, int y,                                                                      int WriteZMode /* DX_ZWRITE_MASK �� */, const wchar_t *FormatString, ... ) ;	// �f�t�H���g�t�H���g�n���h�����g�p���Ăy�o�b�t�@�ɑ΂��ď����w�蕶�����`�悷��
extern	int			DrawFormatVStringToZBuffer_WCHAR_T(             int x, int y,                                                                      int WriteZMode /* DX_ZWRITE_MASK �� */, const wchar_t *FormatString, ... ) ;	// �f�t�H���g�t�H���g�n���h�����g�p���Ăy�o�b�t�@�ɑ΂��ď����w�蕶�����`�悷��( �c���� )
extern	int			DrawExtendStringToZBuffer_WCHAR_T(              int x, int y, double ExRateX, double ExRateY, const wchar_t *String,                 int WriteZMode /* DX_ZWRITE_MASK �� */ ) ;									// �f�t�H���g�t�H���g�n���h�����g�p���Ăy�o�b�t�@�ɑ΂��ĕ�������g��`�悷��
extern	int			DrawExtendVStringToZBuffer_WCHAR_T(             int x, int y, double ExRateX, double ExRateY, const wchar_t *String,                 int WriteZMode /* DX_ZWRITE_MASK �� */ ) ;									// �f�t�H���g�t�H���g�n���h�����g�p���Ăy�o�b�t�@�ɑ΂��ĕ�������g��`�悷��( �c���� )
extern	int			DrawExtendFormatStringToZBuffer_WCHAR_T(        int x, int y, double ExRateX, double ExRateY,                                      int WriteZMode /* DX_ZWRITE_MASK �� */, const wchar_t *FormatString, ... ) ;	// �f�t�H���g�t�H���g�n���h�����g�p���Ăy�o�b�t�@�ɑ΂��ď����w�蕶������g��`�悷��
extern	int			DrawExtendFormatVStringToZBuffer_WCHAR_T(       int x, int y, double ExRateX, double ExRateY,                                      int WriteZMode /* DX_ZWRITE_MASK �� */, const wchar_t *FormatString, ... ) ;	// �f�t�H���g�t�H���g�n���h�����g�p���Ăy�o�b�t�@�ɑ΂��ď����w�蕶������g��`�悷��( �c���� )
extern	int			DrawRotaStringToZBuffer_WCHAR_T(				int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle,                        int WriteZMode /* DX_ZWRITE_MASK �� */ , int VerticalFlag = FALSE , const wchar_t *String       = NULL       ) ;		// �f�t�H���g�t�H���g�n���h�����g�p���ĕ��������]�`�悷��
extern	int			DrawRotaFormatStringToZBuffer_WCHAR_T(			int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle,                        int WriteZMode /* DX_ZWRITE_MASK �� */ , int VerticalFlag = FALSE , const wchar_t *FormatString = NULL , ... ) ;		// �f�t�H���g�t�H���g�n���h�����g�p���ď����w�蕶�������]�`�悷��


extern	int			DrawStringToHandle_WCHAR_T(                     int x, int y, const wchar_t *String, unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 , int VerticalFlag = FALSE ) ;											// �t�H���g�n���h�����g�p���ĕ������`�悷��
extern	int			DrawVStringToHandle_WCHAR_T(                    int x, int y, const wchar_t *String, unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 ) ;																		// �t�H���g�n���h�����g�p���ĕ������`�悷��( �c���� )
extern	int			DrawFormatStringToHandle_WCHAR_T(               int x, int y, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;																						// �t�H���g�n���h�����g�p���ď����w�蕶�����`�悷��
extern	int			DrawFormatVStringToHandle_WCHAR_T(              int x, int y, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;																						// �t�H���g�n���h�����g�p���ď����w�蕶�����`�悷��( �c���� )
extern	int			DrawExtendStringToHandle_WCHAR_T(               int x, int y, double ExRateX, double ExRateY, const wchar_t *String, unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 , int VerticalFlag = FALSE ) ;			// �t�H���g�n���h�����g�p���ĕ�������g��`�悷��
extern	int			DrawExtendVStringToHandle_WCHAR_T(              int x, int y, double ExRateX, double ExRateY, const wchar_t *String, unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 ) ;										// �t�H���g�n���h�����g�p���ĕ�������g��`�悷��( �c���� )
extern	int			DrawExtendFormatStringToHandle_WCHAR_T(         int x, int y, double ExRateX, double ExRateY, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;														// �t�H���g�n���h�����g�p���ď����w�蕶������g��`�悷��
extern	int			DrawExtendFormatVStringToHandle_WCHAR_T(        int x, int y, double ExRateX, double ExRateY, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;														// �t�H���g�n���h�����g�p���ď����w�蕶������g��`�悷��( �c���� )
extern	int			DrawRotaStringToHandle_WCHAR_T(					int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 , int VerticalFlag = FALSE , const wchar_t *String       = NULL       ) ;		// �t�H���g�n���h�����g�p���ĕ��������]�`�悷��
extern	int			DrawRotaFormatStringToHandle_WCHAR_T(			int x, int y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 , int VerticalFlag = FALSE , const wchar_t *FormatString = NULL , ... ) ;		// �t�H���g�n���h�����g�p���ď����w�蕶�������]�`�悷��

extern	int			DrawStringFToHandle_WCHAR_T(                    float x, float y, const wchar_t *String, unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 , int VerticalFlag = FALSE ) ;										// �t�H���g�n���h�����g�p���ĕ������`�悷��( ���W�w�肪 float �� )
extern	int			DrawVStringFToHandle_WCHAR_T(                   float x, float y, const wchar_t *String, unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 ) ;																	// �t�H���g�n���h�����g�p���ĕ������`�悷��( �c���� )( ���W�w�肪 float �� )
extern	int			DrawFormatStringFToHandle_WCHAR_T(              float x, float y, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;																					// �t�H���g�n���h�����g�p���ď����w�蕶�����`�悷��( ���W�w�肪 float �� )
extern	int			DrawFormatVStringFToHandle_WCHAR_T(             float x, float y, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;																					// �t�H���g�n���h�����g�p���ď����w�蕶�����`�悷��( �c���� )( ���W�w�肪 float �� )
extern	int			DrawExtendStringFToHandle_WCHAR_T(              float x, float y, double ExRateX, double ExRateY, const wchar_t *String, unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 , int VerticalFlag = FALSE ) ;		// �t�H���g�n���h�����g�p���ĕ�������g��`�悷��( ���W�w�肪 float �� )
extern	int			DrawExtendVStringFToHandle_WCHAR_T(             float x, float y, double ExRateX, double ExRateY, const wchar_t *String, unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 ) ;									// �t�H���g�n���h�����g�p���ĕ�������g��`�悷��( �c���� )( ���W�w�肪 float �� )
extern	int			DrawExtendFormatStringFToHandle_WCHAR_T(        float x, float y, double ExRateX, double ExRateY, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;													// �t�H���g�n���h�����g�p���ď����w�蕶������g��`�悷��( ���W�w�肪 float �� )
extern	int			DrawExtendFormatVStringFToHandle_WCHAR_T(       float x, float y, double ExRateX, double ExRateY, unsigned int Color, int FontHandle, const wchar_t *FormatString, ... ) ;													// �t�H���g�n���h�����g�p���ď����w�蕶������g��`�悷��( �c���� )( ���W�w�肪 float �� )
extern	int			DrawRotaStringFToHandle_WCHAR_T(				float x, float y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 , int VerticalFlag = FALSE , const wchar_t *String       = NULL       ) ;		// �t�H���g�n���h�����g�p���ĕ��������]�`�悷��( ���W�w�肪 float �� )
extern	int			DrawRotaFormatStringFToHandle_WCHAR_T(			float x, float y, double ExRateX, double ExRateY, double RotCenterX, double RotCenterY, double RotAngle, unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 , int VerticalFlag = FALSE , const wchar_t *FormatString = NULL , ... ) ;		// �t�H���g�n���h�����g�p���ď����w�蕶�������]�`�悷��( ���W�w�肪 float �� )

extern	int			DrawNumberPlusToIToHandle_WCHAR_T(              int x, int y, const wchar_t *NoteString, int    Num, int RisesNum, unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 ) ;											// �t�H���g�n���h�����g�p���Đ����^�̐��l�Ƃ��̐����̕��������x�ɕ`�悷��
extern	int			DrawNumberPlusToFToHandle_WCHAR_T(              int x, int y, const wchar_t *NoteString, double Num, int Length,   unsigned int Color, int FontHandle, unsigned int EdgeColor = 0 ) ;											// �t�H���g�n���h�����g�p���ĕ��������_�^�̐��l�Ƃ��̐����̕��������x�ɕ`�悷��


// ���ˑ��֌W
extern	int			CreateFontToHandle_PF( FONTMANAGE *	ManageData, int DefaultCharSet ) ;																	// CreateFontToHandle �̊��ˑ��������s���֐�
extern	int			CreateFontToHandle_Error_PF( FONTMANAGE * ManageData ) ;																				// CreateFontToHandle �̊��ˑ��G���[�������s���֐�
extern	int			TerminateFontHandle_PF( FONTMANAGE *ManageData ) ;																						// TerminateFontHandle �̊��ˑ��G���[�������s���֐�

extern	int			FontCacheCharAddToHandle_Timing0_PF( FONTMANAGE *ManageData ) ;																			// FontCacheCharaAddToHandle�̊��ˑ��������s���֐�( ���s�ӏ���� 0 )
extern	int			FontCacheCharAddToHandle_Timing1_PF( FONTMANAGE *ManageData, FONTCHARDATA *CharData, DWORD CharCode, int TextureCacheUpdate ) ;			// FontCacheCharaAddToHandle�̊��ˑ��������s���֐�( ���s�ӏ���� 1 )
extern	int			FontCacheCharAddToHandle_Timing2_PF( FONTMANAGE *ManageData ) ;																			// FontCacheCharaAddToHandle�̊��ˑ��������s���֐�( ���s�ӏ���� 2 )

extern	int			EnumFontName_PF( ENUMFONTDATA *EnumFontData, int IsEx = FALSE, int CharSet = -1 ) ;														// EnumFontName �̊��ˑ��������s���֐�




#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_FONT

#endif // __DXFONT_H__
