// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�\�t�g�E�F�A�ň����摜�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXSOFTIMAGE_H__
#define __DXSOFTIMAGE_H__

// �C���N���[�h ------------------------------------------------------------------
#include "DxCompileConfig.h"

#ifndef DX_NON_SOFTIMAGE
#include "DxHandle.h"
#include "DxBaseImage.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\�t�g�C���[�W�n���h���̗L�����`�F�b�N
#define SFTIMGCHK( HAND, SPOINT )			HANDLECHK(       DX_HANDLETYPE_SOFTIMAGE, HAND, *( ( HANDLEINFO ** )&SPOINT ) )
#define SFTIMGCHK_ASYNC( HAND, SPOINT )		HANDLECHK_ASYNC( DX_HANDLETYPE_SOFTIMAGE, HAND, *( ( HANDLEINFO ** )&SPOINT ) )

// �\���̒�` --------------------------------------------------------------------

// �\�t�g�C���[�W�̏��\����
struct SOFTIMAGE
{
	HANDLEINFO				HandleInfo ;			// �n���h�����ʃf�[�^
	BASEIMAGE				BaseImage ;				// ��{�C���[�W�\����
} ;

// �\�t�g�E�G�A�ň����摜�̏��\����
struct SOFTIMAGEMANAGE
{
	int						InitializeFlag ;			// �������t���O
} ;

// �������ϐ��錾 --------------------------------------------------------------

extern SOFTIMAGEMANAGE SoftImageManage ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �������A��n��
extern	int		InitializeSoftImageManage( void ) ;										// �\�t�g�C���[�W�Ǘ����̏�����
extern	int		TerminateSoftImageManage( void ) ;										// �\�t�g�C���[�W�Ǘ����̌�n��

// �ǂݍ��݊֘A
extern	int		InitializeSoftImageHandle( HANDLEINFO *HandleInfo ) ;															// �\�t�g�E�G�A�C���[�W�n���h���̏�����
extern	int		TerminateSoftImageHandle( HANDLEINFO *HandleInfo ) ;															// �\�t�g�E�G�A�C���[�W�n���h���̌�n��
extern	int		LoadSoftImage_UseGParam( const wchar_t *FileName, int ASyncLoadFlag = FALSE ) ;									// LoadSoftImage �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		LoadSoftImageToMem_UseGParam( void *FileImage, int FileImageSize, int ASyncLoadFlag = FALSE ) ;					// LoadSoftImageToMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����


// wchar_t�Ŋ֐�
extern	int		LoadSoftImage_WCHAR_T(              const wchar_t *FileName ) ;
#ifndef DX_NON_FONT
extern	int		BltStringSoftImage_WCHAR_T(         int x, int y, const wchar_t *StrData, int DestSIHandle, int DestEdgeSIHandle = -1 ,                                        int VerticalFlag = FALSE ) ;
extern	int		BltStringSoftImageToHandle_WCHAR_T(	int x, int y, const wchar_t *StrData, int DestSIHandle, int DestEdgeSIHandle /* �����K�v�Ȃ��ꍇ�� -1 */ , int FontHandle, int VerticalFlag = FALSE ) ;
#endif // DX_NON_FONT

#ifndef DX_NON_SAVEFUNCTION
extern	int		SaveSoftImageToBmp_WCHAR_T(         const wchar_t *FilePath, int SIHandle ) ;
#ifndef DX_NON_PNGREAD
extern	int		SaveSoftImageToPng_WCHAR_T(         const wchar_t *FilePath, int SIHandle, int CompressionLevel ) ;
#endif // DX_NON_PNGREAD
#ifndef DX_NON_JPEGREAD
extern	int		SaveSoftImageToJpeg_WCHAR_T(        const wchar_t *FilePath, int SIHandle, int Quality, int Sample2x1 ) ;
#endif // DX_NON_JPEGREAD
#endif // DX_NON_SAVEFUNCTION



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_SOFTIMAGE

#endif // __DXSOFTIMAGE_H__

