// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�񓯊��ǂݍ��ݏ����v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXASYNCLOAD_H__
#define __DXASYNCLOAD_H__

// �C���N���[�h ------------------------------------------------------------------
#include "DxCompileConfig.h"

#ifndef DX_NON_ASYNCLOAD

#include "DxLib.h"
#include "DxThread.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �񓯊��ǂݍ��ݏ����̃W���u�^�C�v
#define ASYNCLOAD_JOBTYPE_ADD_DATA					(0)		// �f�[�^�ǉ�
#define ASYNCLOAD_JOBTYPE_DEL_DATA					(1)		// �f�[�^�폜
#define ASYNCLOAD_JOBTYPE_ERROR_DATA				(2)		// �f�[�^�G���[
#define ASYNCLOAD_JOBTYPE_RELEASE_OTHER_THREAD		(3)		// ���̑��X���b�h�S�������̉���

// �񓯊��ǂݍ��݃f�[�^�̍ő�X�g�b�N��
#define ASYNCLOADDATA_MAXNUM				(32768)

// �񓯊��ǂݍ��݃X���b�h�̍ő吔
#define ASYNCLOADTHREAD_MAXNUM				(32)

// �\���̒�` --------------------------------------------------------------------

// �񓯊��ǂݍ��ݏ����p���ʃf�[�^�\����
struct ASYNCLOADDATA_COMMON
{
	int						Index ;								// �񓯊��ǂݍ��݃f�[�^�̔z��ԍ�
	int						Run ;								// �ǂݍ��ݏ������J�n���Ă��邩�ǂ���
	int						StartTime ;							// �f�[�^��o�^�����ۂ̎���
	void					( *ProcessFunction )( struct ASYNCLOADDATA_COMMON *Data ) ;
	BYTE					Data[ 4 ] ;
} ;

// ���C���X���b�h�ւ̏����˗��f�[�^�\����
struct ASYNCLOAD_MAINTHREAD_REQUESTINFO
{
	int						( *Function )( struct ASYNCLOAD_MAINTHREAD_REQUESTINFO *Info ) ;		// �˗��������s���֐�
	int						ThreadNo ;																// �˗����o�����X���b�h�̔ԍ�
	volatile int			Result ;																// ��������
	volatile DWORD_PTR		Data[ 13 ] ;															// �֐��Ŏg�p������
} ;

// �񓯊��ǂݍ��݃X���b�h�ɕK�v�ȏ���Z�߂��\����
struct ASYNCLOADTHREADINFO
{
	THREAD_INFO				ThreadInfo ;						// �X���b�h�̏��
	DWORD					ExitFlag ;							// �X���b�h���I�����ɗ��Ă�t���O
	int						JobFlag ;							// ���ݎd�������Ă��邩�ǂ����̃t���O
	int						MainThreadRequest ;					// ���C���X���b�h�ւ̃��N�G�X�g���s���Ă���Œ����̃t���O
	int						SuspendFlag ;						// �X���b�h���Ƃ܂��Ă��邩�ǂ����̃t���O
	int						SuspendStartTime ;					// �X���b�h���Ƃ܂����Ƃ��̎���
	struct ASYNCLOADDATA_COMMON *Data ;							// �������̃f�[�^
} ;

// �񓯊��ǂݍ��ݏ������g�p����O���[�o���f�[�^�̍\����
struct ASYNCLOADDATA
{
	int						InitializeFlag ;								// �������t���O

	DWORD_PTR				MainThreadID ;									// ���C���X���b�h�̂h�c
	int						ThreadEndRequestFlag ;							// �񓯊��ǂݍ��ݏ����̏I���𑣂��t���O
	int						ThreadNum ;										// �񓯊��ǂݍ��ݏ������s���X���b�h�̐�
	int						ThreadMaxResumeNum ;							// �񓯊��ǂݍ��ݏ����𓯎��ɍs���X���b�h�̐�
	ASYNCLOADTHREADINFO		Thread[ ASYNCLOADTHREAD_MAXNUM ] ;				// �񓯊��ǂݍ��ݏ������s���X���b�h�̏��
	volatile int			ThreadResumeNum ;								// ���삵�Ă���񓯊��ǂݍ��݃X���b�h�̐�
	DX_CRITICAL_SECTION		CriticalSection ;								// �񓯊��ǂݍ��ݏ����S��( �񓯊��ǂݍ��݃X���b�h�ւ̃��b�Z�[�W�{�񓯊��ǂݍ��݃f�[�^�A�N�Z�X���g�p )�Ŏg�p����N���e�B�J���Z�N�V����

	ASYNCLOAD_MAINTHREAD_REQUESTINFO	*MainThreadRequestInfo[ ASYNCLOADTHREAD_MAXNUM ] ;		// ���C���X���b�h�ւ̏����˗�
	volatile int						MainThreadRequestInfoNum ;								// ���C���X���b�h�ւ̏����˗��̐�

	ASYNCLOADDATA_COMMON	*Data[ ASYNCLOADDATA_MAXNUM ] ;					// �񓯊��ǂݍ��݃f�[�^�̃|�C���^�z��ւ̃|�C���^
	int						DataNum ;										// �񓯊��ǂݍ��݃f�[�^�̐�
	int						DataArea ;										// �񓯊��ǂݍ��݃f�[�^������͈�
} ;

// �������ϐ��錾 --------------------------------------------------------------

// �񓯊��ǂݍ��ݏ������g�p����O���[�o���f�[�^
extern ASYNCLOADDATA GASyncLoadData ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �񓯊��ǂݍ��ݏ����̏������ƌ�n��
extern	int		InitializeASyncLoad( DWORD_PTR MainThreadID ) ;										// �񓯊��ǂݍ��ݏ����̏�����
extern	int		SetupASyncLoadThread( int ProcessorNum ) ;											// �񓯊��ǂݍ��ݏ������s���X���b�h�𗧂Ă�
extern	int		CloseASyncLoadThread( void ) ;														// �񓯊��ǂݍ��ݏ����̃X���b�h�����
extern	int		TerminateASyncLoad( void ) ;														// �񓯊��ǂݍ��ݏ����̌�n��


// �񓯊��ǂݍ��݊֌W
extern	ASYNCLOADDATA_COMMON *AllocASyncLoadDataMemory( int AddAllocSize ) ;						// �񓯊��ǂݍ��݃f�[�^���i�[���郁�����̈���m�ۂ���
extern	void		AddASyncLoadParamStruct( BYTE *Data, int *Addr, const void *Param, int Size ) ;	// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�ɍ\���̃p�����[�^��ǉ�
extern	void		AddASyncLoadParamConstVoidP( BYTE *Data, int *Addr, const void *Param ) ;		// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�� void �^�|�C���^�̃p�����[�^��ǉ�
extern	void		AddASyncLoadParamDWORD_PTR( BYTE *Data, int *Addr, DWORD_PTR Param ) ;			// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�� DWORD_PTR �^�̃p�����[�^��ǉ�
extern	void		AddASyncLoadParamLONGLONG( BYTE *Data, int *Addr, LONGLONG Param ) ;			// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�� LONGLONG �^�̃p�����[�^��ǉ�
extern	void		AddASyncLoadParamInt( BYTE *Data, int *Addr, int Param ) ;						// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�� int �^�̃p�����[�^��ǉ�
extern	void		AddASyncLoadParamByte( BYTE *Data, int *Addr, BYTE Param ) ;					// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�� BYTE �^�̃p�����[�^��ǉ�
extern	void		AddASyncLoadParamFloat( BYTE *Data, int *Addr, float Param ) ;					// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�� float �^�̃p�����[�^��ǉ�
extern	void		AddASyncLoadParamString( BYTE *Data, int *Addr, const wchar_t *Param ) ;		// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�ɕ�����p�����[�^��ǉ�
extern	void *		GetASyncLoadParamStruct( BYTE *Data, int *Addr ) ;								// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^����\���̃p�����[�^���擾
extern	void *		GetASyncLoadParamVoidP( BYTE *Data, int *Addr ) ;								// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���� void �^�|�C���^�̃p�����[�^���擾
extern	DWORD_PTR	GetASyncLoadParamDWORD_PTR( BYTE *Data, int *Addr ) ;							// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���� DWORD_PTR �^�̃p�����[�^���擾
extern	LONGLONG	GetASyncLoadParamLONGLONG( BYTE *Data, int *Addr ) ;							// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���� LONGLONG �^�̃p�����[�^���擾
extern	int			GetASyncLoadParamInt( BYTE *Data, int *Addr ) ;									// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���� int �^�̃p�����[�^���擾
extern	BYTE		GetASyncLoadParamByte( BYTE *Data, int *Addr ) ;								// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���� BYTE �^�̃p�����[�^���擾
extern	float		GetASyncLoadParamFloat( BYTE *Data, int *Addr ) ;								// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���� float �^�̃p�����[�^���擾
extern	wchar_t *	GetASyncLoadParamString( BYTE *Data, int *Addr ) ;								// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���當����p�����[�^���擾
extern	int			AddASyncLoadData( ASYNCLOADDATA_COMMON *ASyncData ) ;							// �񓯊��ǂݍ��݃f�[�^��ǉ�����
extern	int			DeleteASyncLoadData( int DeleteIndex, int MainThread = FALSE ) ;				// �w��̔񓯊��ǂݍ��݃f�[�^���폜����
extern	int			ProcessASyncLoad( int ThreadNumber ) ;											// �񓯊��ǂݍ��݂̏������s��
extern	int			CheckMainThread( void ) ;														// ���݂̃X���b�h�����C���X���b�h���ǂ������擾����( TRUE:���C���X���b�h  FALSE:����ȊO�̃X���b�h )

extern	int			ResumeASyncLoadThread( int AddMaxThreadNum = 0 ) ;								// �Q�Ă���X���b�h����N����

extern	int			ProcessASyncLoadRequestMainThread( void ) ;										// �񓯊��ǂݍ��݃X���b�h���烁�C���X���b�h�ւ̏����˗�����������

extern	int			AddASyncLoadRequestMainThreadInfo( ASYNCLOAD_MAINTHREAD_REQUESTINFO *Info ) ;	// ���C���X���b�h�ŏ������Ăق����f�[�^�̏���ǉ�����

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_ASYNCLOAD

#endif // __DXASYNCLOAD_H__
