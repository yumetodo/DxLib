// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�X���b�h�֌W�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXTHREAD_H__
#define __DXTHREAD_H__

// �C���N���[�h ------------------------------------------------------------------
#include "DxCompileConfig.h"

#ifdef __WINDOWS__
#include "Windows/DxThreadWin.h"
#endif

#ifdef __psp2__
#include "PSVita/DxThreadPSVita.h"
#endif

#ifdef __ORBIS__
#include "PS4/DxThreadPS4.h"
#endif

#ifdef __ANDROID__
#include "Android/DxThreadAndroid.h"
#endif

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �D�揇��
#define DX_THREAD_PRIORITY_LOWEST			(0)
#define DX_THREAD_PRIORITY_BELOW_NORMAL		(1)
#define DX_THREAD_PRIORITY_NORMAL			(2)
#define DX_THREAD_PRIORITY_HIGHEST			(3)
#define DX_THREAD_PRIORITY_NUM				(4)

#if !defined( __BCC ) || defined( _DEBUG )
	#define CRITICALSECTION_LOCK( csection )			CriticalSection_Lock( (csection), __FILE__, __LINE__ )
#else
	#define CRITICALSECTION_LOCK( csection )			CriticalSection_Lock( (csection) )
#endif

// �^��` ------------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

extern	void		Thread_Initialize( void ) ;																				// �X���b�h�̏���������������
extern	int			Thread_Create( THREAD_INFO *pThreadInfo, void ( *pFunction )( THREAD_INFO *, void * ), void *pParam ) ;	// �X���b�h���쐬����
extern	void		Thread_Delete( THREAD_INFO *pThreadInfo ) ;																// �X���b�h�̌�n�����s��
extern	int			Thread_IsValid( THREAD_INFO *pThreadInfo ) ;															// �X���b�h���L�����ǂ������擾����( 1:�L��  0:���� )
extern	void		Thread_SetPriority( THREAD_INFO *pThreadInfo, int Priority /* DX_THREAD_PRIORITY_LOWEST �Ȃ� */ ) ;		// �X���b�h�̎��s�D�揇�ʂ�ݒ肷��
extern	DWORD_PTR	Thread_GetCurrentId( void ) ;																			// �J�����g�X���b�h�̂h�c���擾����
extern	DWORD_PTR	Thread_GetId( THREAD_INFO *pThreadInfo ) ;																// �X���b�h�̂h�c���擾����
extern	void		Thread_Suspend( THREAD_INFO *pThreadInfo ) ;															// �X���b�h���x�~��Ԃɂ���
extern	int			Thread_Resume( THREAD_INFO *pThreadInfo ) ;																// �X���b�h�̋x�~��Ԃ���������( 0:�x�~��Ԃ���Ȃ�����  1:�x�~��Ԃ����� )

extern	void		Thread_Sleep( DWORD MiliSecond ) ;																		// �w�莞�ԃX���b�h���~����

extern	int			CriticalSection_Initialize( DX_CRITICAL_SECTION *pCSection ) ;									// �N���e�B�J���Z�N�V�����̏�����
extern	int			CriticalSection_Delete( DX_CRITICAL_SECTION *pCSection ) ;										// �N���e�B�J���Z�N�V�����̍폜

#if !defined( __BCC ) || defined( _DEBUG )
extern	int			CriticalSection_Lock( DX_CRITICAL_SECTION *pCSection, const char *FilePath, int LineNo ) ;		// �N���e�B�J���Z�N�V�����̃��b�N�̎擾
#else
extern	int			CriticalSection_Lock( DX_CRITICAL_SECTION *pCSection ) ;										// �N���e�B�J���Z�N�V�����̃��b�N�̎擾
#endif
extern	int			CriticalSection_Unlock( DX_CRITICAL_SECTION *pCSection ) ;										// �N���e�B�J���Z�N�V�����̃��b�N���������

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXTHREAD_H__
