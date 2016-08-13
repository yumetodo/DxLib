// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�n���h���Ǘ��v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXHANDLE_H__
#define __DXHANDLE_H__

// �C���N���[�h ------------------------------------------------------------------
#include "DxCompileConfig.h"
#include "DxThread.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �n���h���̓���
#define DX_HANDLEINDEX_MASK							(0x0000ffff)		// �n���h���z��C���f�b�N�X�}�X�N
#define DX_HANDLEINDEX_MAX							(65536)				// �n���h���z��C���f�b�N�X�̍ő吔

#define DX_HANDLECHECK_MASK							(0x03ff0000)		// �n���h���̐������`�F�b�N�p�}�X�N
#define DX_HANDLECHECK_ADDRESS						(16)				// �n���h���̐������`�F�b�N�p�}�X�N�̊J�n�A�h���X
#define DX_HANDLECHECK_MAX							(1024)				// �n���h���̐������`�F�b�N�l�̍ő吔

#define DX_HANDLETYPE_MASK							(0x7c000000)		// �n���h���^�C�v�}�X�N
#define DX_HANDLETYPE_ADDRESS						(26)				// �n���h���^�C�v�}�X�N�̊J�n�A�h���X
#define DX_HANDLETYPE_MAX							(32)				// �n���h���^�C�v�̍ő吔

#define DX_HANDLEERROR_MASK							(0x80000000)		// �G���[�`�F�b�N�}�X�N( �O�ł͂Ȃ�������G���[ )

// �n���h���^�C�v��`
#define DX_HANDLETYPE_NONE							(0)					// �n���h���^�C�v�O�͖��g�p
#define DX_HANDLETYPE_GRAPH							(1)					// �O���t�B�b�N�n���h��
#define DX_HANDLETYPE_SOFTIMAGE						(2)					// �\�t�g�E�G�A�ň����C���[�W�n���h��
#define DX_HANDLETYPE_SOUND							(3)					// �T�E���h�n���h��
#define DX_HANDLETYPE_SOFTSOUND						(4)					// �\�t�g�T�E���h�n���h��
#define DX_HANDLETYPE_MUSIC							(5)					// �~���[�W�b�N�n���h��
#define DX_HANDLETYPE_MOVIE							(6)					// ���[�r�[�n���h��
#define DX_HANDLETYPE_GMASK							(7)					// �}�X�N�n���h��
#define DX_HANDLETYPE_FONT							(8)					// �t�H���g�n���h��
#define DX_HANDLETYPE_KEYINPUT						(9)					// ��������̓n���h��
#define DX_HANDLETYPE_NETWORK						(10)				// �l�b�g���[�N�n���h��
#define DX_HANDLETYPE_LIGHT							(11)				// ���C�g
#define DX_HANDLETYPE_SHADER						(12)				// �V�F�[�_�[�n���h��
#define DX_HANDLETYPE_MODEL_BASE					(13)				// �R�c���f����{�f�[�^
#define DX_HANDLETYPE_MODEL							(14)				// �R�c���f��
#define DX_HANDLETYPE_VERTEX_BUFFER					(15)				// ���_�o�b�t�@�n���h��
#define DX_HANDLETYPE_INDEX_BUFFER					(16)				// �C���f�b�N�X�o�b�t�@�n���h��
#define DX_HANDLETYPE_FILE							(17)				// �t�@�C���n���h��
#define DX_HANDLETYPE_SHADOWMAP						(18)				// �V���h�E�}�b�v�n���h��
#define DX_HANDLETYPE_SHADER_CONSTANT_BUFFER		(19)				// �V�F�[�_�[�p�萔�o�b�t�@�n���h��

#define DX_HANDLETYPE_MASK_GRAPH					(DX_HANDLETYPE_GRAPH         << DX_HANDLETYPE_ADDRESS)		// �O���t�B�b�N�n���h��
#define DX_HANDLETYPE_MASK_SOFTIMAGE				(DX_HANDLETYPE_SOFTIMAGE     << DX_HANDLETYPE_ADDRESS)		// �\�t�g�E�G�A�ň����C���[�W�n���h��
#define DX_HANDLETYPE_MASK_SOUND					(DX_HANDLETYPE_SOUND         << DX_HANDLETYPE_ADDRESS)		// �T�E���h�n���h��
#define DX_HANDLETYPE_MASK_SOFTSOUND				(DX_HANDLETYPE_SOFTSOUND     << DX_HANDLETYPE_ADDRESS)		// �\�t�g�T�E���h�n���h��
#define DX_HANDLETYPE_MASK_MUSIC					(DX_HANDLETYPE_MUSIC         << DX_HANDLETYPE_ADDRESS)		// �~���[�W�b�N�n���h��
#define DX_HANDLETYPE_MASK_MOVIE					(DX_HANDLETYPE_MOVIE         << DX_HANDLETYPE_ADDRESS)		// ���[�r�[�n���h��
#define DX_HANDLETYPE_MASK_GMASK					(DX_HANDLETYPE_GMASK         << DX_HANDLETYPE_ADDRESS)		// �}�X�N�n���h��
#define DX_HANDLETYPE_MASK_FONT						(DX_HANDLETYPE_FONT          << DX_HANDLETYPE_ADDRESS)		// �t�H���g�n���h��
#define DX_HANDLETYPE_MASK_KEYINPUT					(DX_HANDLETYPE_KEYINPUT      << DX_HANDLETYPE_ADDRESS)		// ��������̓n���h��
#define DX_HANDLETYPE_MASK_NETWORK					(DX_HANDLETYPE_NETWORK       << DX_HANDLETYPE_ADDRESS)		// �l�b�g���[�N�n���h��
#define DX_HANDLETYPE_MASK_LIGHT					(DX_HANDLETYPE_LIGHT         << DX_HANDLETYPE_ADDRESS)		// ���C�g
#define DX_HANDLETYPE_MASK_SHADER					(DX_HANDLETYPE_SHADER        << DX_HANDLETYPE_ADDRESS)		// �V�F�[�_�[�n���h��
#define DX_HANDLETYPE_MASK_MODEL_BASE				(DX_HANDLETYPE_MODEL_BASE    << DX_HANDLETYPE_ADDRESS)		// �R�c���f����{�f�[�^
#define DX_HANDLETYPE_MASK_MODEL					(DX_HANDLETYPE_MODEL         << DX_HANDLETYPE_ADDRESS)		// �R�c���f��
#define DX_HANDLETYPE_MASK_VERTEX_BUFFER			(DX_HANDLETYPE_VERTEX_BUFFER << DX_HANDLETYPE_ADDRESS)		// ���_�o�b�t�@�n���h��
#define DX_HANDLETYPE_MASK_INDEX_BUFFER				(DX_HANDLETYPE_INDEX_BUFFER  << DX_HANDLETYPE_ADDRESS)		// �C���f�b�N�X�o�b�t�@�n���h��
#define DX_HANDLETYPE_MASK_FILE						(DX_HANDLETYPE_FILE          << DX_HANDLETYPE_ADDRESS)		// �t�@�C���n���h��
#define DX_HANDLETYPE_MASK_SHADOWMAP				(DX_HANDLETYPE_SHADOWMAP     << DX_HANDLETYPE_ADDRESS)		// �V���h�E�}�b�v�n���h��

// �n���h���̗L���`�F�b�N

#define HANDLECHKFULL_ASYNC( TYPE, HANDLE, INFO )																\
	  ( ( HandleManageArray[ (TYPE) ].InitializeFlag == FALSE ) ||												\
		( ( (HANDLE) & DX_HANDLEERROR_MASK ) != 0 ) ||															\
		( ( (HANDLE) & DX_HANDLETYPE_MASK  ) != HandleManageArray[ (TYPE) ].HandleTypeMask ) ||					\
		( ( (HANDLE) & DX_HANDLEINDEX_MASK ) >= HandleManageArray[ (TYPE) ].MaxNum ) ||							\
		( ( INFO = HandleManageArray[ (TYPE) ].Handle[ (HANDLE) & DX_HANDLEINDEX_MASK ] ) == NULL ) ||			\
		( (int)( (INFO)->ID << DX_HANDLECHECK_ADDRESS ) != ( (HANDLE) & DX_HANDLECHECK_MASK ) ) )

#ifndef DX_NON_ASYNCLOAD

	#define HANDLECHKFULL( TYPE, HANDLE, INFO )																			\
		  ( ( HandleManageArray[ (TYPE) ].InitializeFlag == FALSE ) ||												\
			( ( (HANDLE) & DX_HANDLEERROR_MASK ) != 0 ) ||															\
			( ( (HANDLE) & DX_HANDLETYPE_MASK ) != HandleManageArray[ (TYPE) ].HandleTypeMask ) ||					\
			( ( (HANDLE) & DX_HANDLEINDEX_MASK ) >= HandleManageArray[ (TYPE) ].MaxNum ) ||							\
			( ( INFO = HandleManageArray[ (TYPE) ].Handle[ (HANDLE) & DX_HANDLEINDEX_MASK ] ) == NULL ) ||			\
			( (int)( (INFO)->ID << DX_HANDLECHECK_ADDRESS ) != ( (HANDLE) & DX_HANDLECHECK_MASK ) ) ||				\
			( (INFO)->ASyncLoadCount != 0 ) )

#else // DX_NON_ASYNCLOAD

	#define HANDLECHKFULL( MANAGE, HANDLE, INFO )		HANDLECHKFULL_ASYNC( MANAGE, HANDLE, INFO )

#endif // DX_NON_ASYNCLOAD


#ifdef DX_NON_HANDLE_ERROR_CHECK

	#define HANDLECHK_ASYNC( TYPE, HANDLE, INFO )																	\
		  ( ( ( INFO = HandleManageArray[ (TYPE) ].Handle[ (HANDLE) & DX_HANDLEINDEX_MASK ] ) == NULL ) )

	#ifndef DX_NON_ASYNCLOAD

		#define HANDLECHK( TYPE, HANDLE, INFO )																			\
			  ( ( ( INFO = HandleManageArray[ (TYPE) ].Handle[ (HANDLE) & DX_HANDLEINDEX_MASK ] ) == NULL ) ||			\
				( (INFO)->ASyncLoadCount != 0 ) )

	#else // DX_NON_ASYNCLOAD

		#define HANDLECHK( MANAGE, HANDLE, INFO )		HANDLECHK_ASYNC( MANAGE, HANDLE, INFO )

	#endif // DX_NON_ASYNCLOAD


#else // DX_NON_HANDLE_ERROR_CHECK

	#define HANDLECHK_ASYNC( TYPE, HANDLE, INFO )	HANDLECHKFULL_ASYNC( TYPE, HANDLE, INFO )
	#define HANDLECHK( TYPE, HANDLE, INFO )			HANDLECHKFULL( TYPE, HANDLE, INFO )

#endif // DX_NON_HANDLE_ERROR_CHECK

// �\���̒�` --------------------------------------------------------------------

// �n���h�����X�g�\����
struct HANDLELIST
{
	int						Handle ;							// �n���h��
	void					*Data ;								// �f�[�^�ւ̃|�C���^
	struct HANDLELIST		*Prev, *Next ;						// ���X�g�̈�O�Ǝ��̗v�f�ւ̃|�C���^
} ;

// �n���h���̋��ʃf�[�^
struct HANDLEINFO
{
	int						ID ;								// �G���[�`�F�b�N�p�h�c
	int						Handle ;							// ���g�̃n���h���l
	int						AllocSize ;							// �������̊m�ۃT�C�Y
	int						*DeleteFlag ;						// �n���h���폜���Ɂ|�P�ɂ���ϐ��ւ̃|�C���^
#ifndef DX_NON_ASYNCLOAD
	int						ASyncLoadCount ;					// �񓯊��ǂݍ��ݏ����̑ΏۂƂȂ��Ă��鐔
	int						ASyncLoadResult ;					// �񓯊��ǂݍ��ݏ����̌���
	int						ASyncDataNumber ;					// �񓯊��ǂݍ��ݏ����ԍ�
	volatile int			ASyncLoadFinishDeleteRequestFlag ;	// �񓯊��ǂݍ��݂�����������n���h�����폜����t���O
#endif
	HANDLELIST				List ;								// �n���h�����X�g�̈�O�Ǝ��̗v�f�ւ̃|�C���^
} ;

// �n���h���Ǘ��̋��ʃf�[�^
struct HANDLEMANAGE
{
	int						InitializeFlag ;					// �������t���O
	HANDLEINFO				**Handle ;							// �n���h���ւ̃|�C���^�z��
	HANDLELIST				ListFirst ;							// �n���h�����X�g�̐擪
	HANDLELIST				ListLast ;							// �n���h�����X�g�̖��[
	int						HandleTypeMask ;					// �n���h���^�C�v�}�X�N
	int						OneSize ;							// �n���h����ӂ�̑傫��
	int						MaxNum ;							// �n���h���̍ő�T�C�Y
	int						Num ;								// �n���h���̐�
	int						AreaMin ;							// �n���h�����z�G���A�ŏ��l
	int						AreaMax ;							// �n���h�����z�G���A�ő�l
	int						NextID ;							// ���̃n���h���Ɋ��蓖�Ă�h�c
	DX_CRITICAL_SECTION		CriticalSection ;					// �f�[�^�A�N�Z�X���p�N���e�B�J���Z�N�V����
	int						( *InitializeFunction )( HANDLEINFO *HandleInfo ) ;	// �n���h���̏�����������֐��ւ̃|�C���^
	int						( *TerminateFunction )( HANDLEINFO *HandleInfo ) ;	// �n���h���̌�n��������֐��ւ̃|�C���^
	const wchar_t			*Name ;								// �n���h����
	char					NameUTF16LE[ 128 ] ;				// �n���h����( UTF16LE )
} ;

// �������ϐ��錾 --------------------------------------------------------------

extern HANDLEMANAGE HandleManageArray[ DX_HANDLETYPE_MAX ] ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �n���h�����ʊ֌W
extern	int		InitializeHandleManage( int HandleType, int OneSize, int MaxNum, int ( *InitializeFunction )( HANDLEINFO *HandleInfo ), int ( *TerminateFunction )( HANDLEINFO *HandleInfo ), const wchar_t *Name ) ;	// �n���h���Ǘ���������������( InitializeFlag �ɂ� FALSE �������Ă���K�v������ )
extern	int		TerminateHandleManage( int HandleType ) ;																		// �n���h���Ǘ����̌�n�����s��

extern	int		AddHandle( int HandleType, int ASyncThread, int Handle /* = -1 */ ) ;										// �n���h����ǉ�����
extern	int		SubHandle( int Handle ) ;															// �n���h�����폜����
extern	int		ReallocHandle( int Handle, size_t NewSize ) ;										// �n���h���̏����i�[���郁�����̈�̃T�C�Y��ύX����A�񓯊��ǂݍ��ݒ��łȂ����Ƃ��O��
extern	HANDLEINFO *GetHandleInfo( int Handle ) ;													// �n���h���̏����擾����
extern	int		AllHandleSub( int HandleType, int (*DeleteCancelCheckFunction)( HANDLEINFO *HandleInfo ) = NULL ) ;	// �n���h���Ǘ����ɓo�^����Ă��邷�ׂẴn���h�����폜
#ifndef DX_NON_ASYNCLOAD
extern	int		IncASyncLoadCount( int Handle, int ASyncDataNumber ) ;								// �n���h���̔񓯊��ǂݍ��ݒ��J�E���g���C���N�������g����
extern	int		DecASyncLoadCount( int Handle ) ;													// �n���h���̔񓯊��ǂݍ��ݒ��J�E���g���f�N�������g����
extern	int		GetASyncLoadFinishDeleteFlag( int Handle ) ;										// �n���h���̔񓯊��ǂݍ��݊�����ɍ폜���邩�ǂ����̃t���O���擾����
extern	int		WaitASyncLoad( int Handle ) ;														// �n���h�����񓯊��ǂݍ��ݒ��������ꍇ�A�񓯊��ǂݍ��݂���������܂ő҂�
#endif // DX_NON_ASYNCLOAD


// �n���h�����X�g
extern	int		InitializeHandleList( HANDLELIST *First, HANDLELIST *Last ) ;					// ���X�g�̏�����
extern	int		AddHandleList( HANDLELIST *First, HANDLELIST *List, int Handle, void *Data ) ;	// ���X�g�֗v�f��ǉ�
extern	int		SubHandleList( HANDLELIST *List ) ;												// ���X�g����v�f���O��
extern	int		NewMemoryHandleList( HANDLELIST *List, void *Data ) ;							// ���X�g�����݂��郁�������ύX���ꂽ�ꍇ�Ƀ��X�g�̑O����X�V����

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXHANDLE_H__
