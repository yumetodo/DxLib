// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�}�X�N�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMASK_H__
#define __DXMASK_H__

#include "DxCompileConfig.h"

#ifndef DX_NON_MASK

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

// �\���̒�` --------------------------------------------------------------------

// �}�X�N�f�[�^�p�\����
struct MASKDATA
{
	HANDLEINFO				HandleInfo ;							// �n���h�����ʏ��

	BYTE					*SrcData ;								// �}�X�N�\�[�X�f�[�^
	int						SrcDataPitch ;							// �\�[�X�f�[�^�s�b�`
	int						MaskWidth, MaskHeight ;					// �}�X�N�̕��ƍ���
	int						ValidFlag ;								// �L���t���O
	int						TransMode ;								// ���ߐF�������[�h
} ;

// �}�X�N�f�[�^�Ǘ��p�\����
struct MASKMANAGEDATA
{
	int						InitializeFlag ;						// �������t���O

	int						MaskBufferSizeX, MaskBufferSizeY ;		// �}�X�N�f�[�^�o�b�t�@�̃T�C�Y
	BYTE					*MaskBuffer ;							// �}�X�N�f�[�^�o�b�t�@
	int						MaskBufferPitch ;						// �}�X�N�f�[�^�o�b�t�@�̃s�b�`

	int						MaskScreenGraphHandle ;					// �}�X�N��ʂƂ��Ďg�p����O���t�B�b�N�n���h���A �����̏ꍇ�� 0 ( �A���t�@�`�����l�����}�X�N�Ƃ��Ďg�p����� )
	int						MaskScreenGraphHandleUseChannel ;		// �}�X�N��ʂƂ��Ďg�p����O���t�B�b�N�n���h���ŁA�}�X�N�Ƃ��Ďg�p����`�����l��( DX_MASKGRAPH_CH_A �� )

	int						MaskReverseEffectFlag ;					// �}�X�N�̐��l�ɑ΂�����ʂ��t�]������
	int						FullScreenMaskUpdateFlag ;				// �S��ʃ}�X�N�X�V���s���Ă��邩�̃t���O
	int						CreateMaskFlag ;						// �}�X�N�T�[�t�F�X���쐬����Ă��邩�A�̃t���O
	int						MaskUseFlag ;							// �}�X�N���g�p���邩�A�t���O
	int						MaskValidFlag ;							// MaskUseFlag �� CreateMaskFlag �� & ��������

	int						MaskBeginFunctionCount ;				// �}�X�N���g�p�����`��̑O�ɌĂԊ֐����Ă΂ꂽ��

	int						ValidMaskDrawMemImg ;					// MaskDrawMemImg ���L�����ǂ����A�̃t���O
	MEMIMG					MaskDrawMemImg ;						// �}�X�N�g�p���̕`���T�[�t�F�X�̑���� MEMIMG
} ;

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �}�X�N�֌W�̏��
extern MASKMANAGEDATA MaskManageData ;


// �֐��v���g�^�C�v�錾-----------------------------------------------------------

extern	int			Mask_Initialize( void ) ;																				// �}�X�N�����̏�����
extern	int			Mask_Terminate( void ) ;																				// �}�X�N�����̌�n��

extern	int			Mask_ReleaseSurface( void ) ;																			// �}�X�N�X�N���[�����ꎞ�폜����
extern	int			Mask_ReCreateSurface( void ) ;																			// �}�X�N�X�N���[���𕜋�������
extern	int			Mask_CreateScreenFunction( int MaskSurfaceFlag, int Width, int Height ) ;								// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_CreateSurface( BYTE **MaskBuffer, int *BufferPitch, int Width, int Height, int *TransModeP ) ;		// �}�X�N�T�[�t�F�X�̍쐬

extern	int			Mask_DrawBeginFunction( RECT Rect ) ;																	// �}�X�N���g�p�����`��̑O�ɌĂԊ֐�
extern	int			Mask_DrawAfterFunction( RECT Rect ) ;																	// �}�X�N���g�p�����`��̌�ɌĂԊ֐�

extern	int			Mask_InitializeHandle( HANDLEINFO *HandleInfo ) ;														// �}�X�N�n���h���̏�����
extern	int			Mask_TerminateHandle( HANDLEINFO *HandleInfo ) ;															// �}�X�N�n���h���̌�n��

extern	int			Mask_MakeMask_UseGParam( int Width, int Height, int ASyncLoadFlag = FALSE ) ;								// �}�X�N�f�[�^�̒ǉ�
extern	int			Mask_LoadMask_UseGParam(             const wchar_t *FileName,                                                                                        int ASyncLoadFlag = FALSE ) ;	// �}�X�N�f�[�^���摜�t�@�C������ǂݍ���
extern	int			Mask_LoadDivMask_UseGParam(          const wchar_t *FileName,                  int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf, int ASyncLoadFlag = FALSE ) ;	// �}�X�N���摜�t�@�C�����番���ǂ݂���
extern	int			Mask_CreateMaskFromMem_UseGParam(    const void *FileImage, int FileImageSize,                                                                       int ASyncLoadFlag = FALSE ) ;	// �}�X�N�f�[�^����������̉摜�t�@�C���C���[�W����ǂݍ���
extern	int			Mask_CreateDivMaskFromMem_UseGParam( const void *FileImage, int FileImageSize, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf, int ASyncLoadFlag = FALSE ) ;	// �}�X�N����������̉摜�t�@�C���C���[�W���番���ǂ݂���



// ���ˑ��֐�
extern	int			Mask_CreateScreenFunction_Timing0_PF( void ) ;															// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_CreateScreenFunction_Timing1_PF( int Width, int Height ) ;											// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_CreateScreenFunction_Timing2_PF( int MaskBufferSizeXOld, int MaskBufferSizeYOld ) ;				// �}�X�N�X�N���[�����쐬����֐�
extern	int			Mask_ReleaseSurface_PF( void ) ;																		// �}�X�N�X�N���[�����ꎞ�폜����
extern	int			Mask_SetUseMaskScreenFlag_PF( void ) ;																	// �}�X�N�g�p���[�h��ύX
extern	int			Mask_DrawBeginFunction_PF( RECT *Rect ) ;																// �}�X�N���g�p�����`��̑O�ɌĂԊ֐�( ���łɃT�u�o�b�t�@���g�p�����`��G���A�@�\���g�p���Ă���ꍇ�̏���������Ă��܂��Ă���� )
extern	int			Mask_DrawAfterFunction_PF( RECT *Rect ) ;																// �}�X�N���g�p�����`��̌�ɌĂԊ֐�( ���łɃT�u�o�b�t�@���g�p�����`��G���A�@�\���g�p���Ă���ꍇ�̏���������Ă��܂��Ă���� )
extern	int			Mask_FillMaskScreen_PF( int Flag ) ;																	// �}�X�N�X�N���[�����w��̐F�œh��Ԃ�
extern	int			Mask_UpdateMaskImageTexture_PF( RECT *Rect ) ;															// �w��̈�̃}�X�N�C���[�W�e�N�X�`�����X�V����




// wchar_t�Ŋ֐�
extern	int			LoadMask_WCHAR_T(						const wchar_t *FileName ) ;
extern	int			LoadDivMask_WCHAR_T(					const wchar_t *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf ) ;
#ifndef DX_NON_FONT
extern	int			DrawFormatStringMask_WCHAR_T(			int x, int y, int Flag,                 const wchar_t *FormatString, ... ) ;
extern	int			DrawFormatStringMaskToHandle_WCHAR_T(	int x, int y, int Flag, int FontHandle, const wchar_t *FormatString, ... ) ;
extern	int			DrawStringMask_WCHAR_T(					int x, int y, int Flag,                 const wchar_t *String ) ;
extern	int			DrawStringMaskToHandle_WCHAR_T(			int x, int y, int Flag, int FontHandle, const wchar_t *String ) ;
#endif // DX_NON_FONT



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif //	DX_NON_MASK

#endif // __DXMASK_H__
