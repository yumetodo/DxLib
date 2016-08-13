// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Windows�p�t�H���g�֌W�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXFONTWIN_H__
#define __DXFONTWIN_H__

// �C���N���[�h ------------------------------------------------------------------
#include "../DxCompileConfig.h"
#include "../DxFile.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

// Windows�p �t�H���g�L���b�V���̊��ˑ��Ǘ��f�[�^
struct FONTMANAGE_PF
{
	int						UseTextOut ;						// TextOut ���g�p����^�C�v�̃t�H���g���ǂ���( TRUE:TextOut���g�p����  FALSE:GetGlyphOutline ���g�p���� )

	HBITMAP					CacheBitmap ;						// �e�L�X�g�L���b�V���p�r�b�g�}�b�v
	unsigned char *			CacheBitmapMem ;					// �e�L�X�g�L���b�V���p�r�b�g�}�b�v�̃�����
	DWORD					CacheBitmapMemPitch ;				// �e�L�X�g�L���b�V���p�r�b�g�}�b�v�̃s�b�`

	HFONT					FontObj ;							// �t�H���g�̃I�u�W�F�N�g�f�[�^

	void *					GetGlyphOutlineBuffer ;				// GetGlyphOutline �̃f�[�^�擾�p�Ɏg�p����o�b�t�@
	DWORD					GetGlyphOutlineBufferSize ;			// GetGlyphOutline �̃f�[�^�擾�p�Ɏg�p����o�b�t�@�̃T�C�Y
} ;

// Windows�p �t�H���g�V�X�e���p�\����
struct FONTSYSTEM_WIN
{
	HDC						Devicecontext ;						// FontCacheCharAddToHandle �̏����Ŏg�p����f�o�C�X�R���e�L�X�g
	HFONT					OldFont ;							// FontCacheCharAddToHandle �̏����Ŏg�p����t�H���g
	HBITMAP					OldBitmap ;							// FontCacheCharAddToHandle �̏����Ŏg�p����r�b�g�}�b�v
	TEXTMETRIC				TextMetric ;						// FontCacheCharAddToHandle �̏����Ŏg�p����t�H���g�̏��
} ;

// �������ϐ��錾 --------------------------------------------------------------

extern FONTSYSTEM_WIN FontSystem_Win ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

extern	HANDLE		AddFontFile_WCHAR_T( const wchar_t *FontFilePath ) ;																			// �w��̃t�H���g�t�@�C�����V�X�e���ɒǉ�����( �߂�l  NULL:���s  NULL�ȊO:�t�H���g�n���h��( WindowsOS �̂��̂Ȃ̂ŁA�c�w���C�u�����̃t�H���g�n���h���Ƃ͕ʕ��ł� ) )

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXFONTWIN_H__
