// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�V�X�e���v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXSYSTEM_H__
#define __DXSYSTEM_H__

// �C���N���[�h ------------------------------------------------------------------
#include "DxCompileConfig.h"
#include "DxLib.h"
#include "DxStatic.h"
#include "DxBaseFunc.h"

#ifdef __WINDOWS__
#include "Windows/DxSystemWin.h"
#endif

#ifdef __psp2__
#include "PSVita/DxSystemPSVita.h"
#endif

#ifdef __ORBIS__
#include "PS4/DxSystemPS4.h"
#endif

#ifdef __ANDROID__
#include "Android/DxSystemAndroid.h"
#endif

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------


// �\���̒�` --------------------------------------------------------------------

// �V�X�e���f�[�^�\����
struct DXSYSTEMDATA
{
	int						DxLib_InitializeFlag ;				// �c�w���C�u�����̏��������������Ă��邩�t���O
	int						DxLib_RunInitializeFlag ;			// �c�w���C�u�����̏����������A�t���O

	int						NotWinFlag ;						// �c�w���C�u�����̃E�C���h�E�֘A�@�\���g�p���Ȃ��t���O
	int						NotDrawFlag ;						// �`��@�\���g�p���Ȃ��t���O
	int						NotSoundFlag ;						// �T�E���h�@�\���g�p���Ȃ��t���O
	int						NotInputFlag ;						// ���͏�Ԏ擾�@�\���g�p���Ȃ��t���O

	int						EndRequestFlag ;					// �I�����N�G�X�g�t���O
	int						ASyncLoadFlag ;						// �񓯊��ǂݍ��݃t���O
} ;

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

extern DXSYSTEMDATA DxSysData ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �������E�I���֌W
extern	int			DxLib_SysInit( void ) ;													// DxSysData �֌W�̏��������s��
extern	int			DxLib_EndRequest( void ) ;												// �I�����N�G�X�g���s��
extern	int			DxLib_GetEndRequest( void ) ;											// �I�����N�G�X�g�̏�Ԃ��擾����

// �G���[�����֐�
extern	int			DxLib_Error(        const wchar_t *ErrorStr ) ;							// ���C�u�����̃G���[�������s��
extern	int			DxLib_ErrorUTF16LE( const char    *ErrorStr ) ;							// ���C�u�����̃G���[�������s��( UTF16LE�� )
extern	int			DxLib_FmtError(        const wchar_t *FormatString, ... ) ;				// �����t�����C�u�����̃G���[�������s��
extern	int			DxLib_FmtErrorUTF16LE( const char    *FormatString, ... ) ;				// �����t�����C�u�����̃G���[�������s��( UTF16LE�� )

// �񓯊��ǂݍ��݊֌W
extern	int			GetASyncLoadFlag( void ) ;												// �񓯊��ǂݍ��݂��s�����ǂ������擾����

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXSYSTEM_H__

