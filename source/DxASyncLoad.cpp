// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�񓯊��ǂݍ��ݏ����v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

// �C���N���[�h ------------------------------------------------------------------
#include "DxASyncLoad.h"

#ifndef DX_NON_ASYNCLOAD

#include "DxStatic.h"
#include "DxBaseFunc.h"
#include "DxSystem.h"
#include "DxMemory.h"
#include "DxLog.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------
	
// �������ϐ��錾 --------------------------------------------------------------

// �񓯊��ǂݍ��ݏ������g�p����O���[�o���f�[�^
ASYNCLOADDATA GASyncLoadData ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

static void ASyncLoadThreadFunction( THREAD_INFO *pThreadInfo, void *ASyncLoadThreadData ) ;

// �񓯊��ǂݍ��݂��s���X���b�h�̃}�N��
#define ASYNCLOADTHREADFUNCTION_MACRO( Number )		\
	void ASyncLoadThreadFunctionT##Number( THREAD_INFO *pThreadInfo, void *ASyncLoadThreadData )\
{\
	ASyncLoadThreadFunction( pThreadInfo, ASyncLoadThreadData ) ;\
}

ASYNCLOADTHREADFUNCTION_MACRO( 0 )
ASYNCLOADTHREADFUNCTION_MACRO( 1 )
ASYNCLOADTHREADFUNCTION_MACRO( 2 )
ASYNCLOADTHREADFUNCTION_MACRO( 3 )
ASYNCLOADTHREADFUNCTION_MACRO( 4 )
ASYNCLOADTHREADFUNCTION_MACRO( 5 )
ASYNCLOADTHREADFUNCTION_MACRO( 6 )
ASYNCLOADTHREADFUNCTION_MACRO( 7 )
ASYNCLOADTHREADFUNCTION_MACRO( 8 )
ASYNCLOADTHREADFUNCTION_MACRO( 9 )
ASYNCLOADTHREADFUNCTION_MACRO( 10 )
ASYNCLOADTHREADFUNCTION_MACRO( 11 )
ASYNCLOADTHREADFUNCTION_MACRO( 12 )
ASYNCLOADTHREADFUNCTION_MACRO( 13 )
ASYNCLOADTHREADFUNCTION_MACRO( 14 )
ASYNCLOADTHREADFUNCTION_MACRO( 15 )
ASYNCLOADTHREADFUNCTION_MACRO( 16 )
ASYNCLOADTHREADFUNCTION_MACRO( 17 )
ASYNCLOADTHREADFUNCTION_MACRO( 18 )
ASYNCLOADTHREADFUNCTION_MACRO( 19 )
ASYNCLOADTHREADFUNCTION_MACRO( 20 )
ASYNCLOADTHREADFUNCTION_MACRO( 21 )
ASYNCLOADTHREADFUNCTION_MACRO( 22 )
ASYNCLOADTHREADFUNCTION_MACRO( 23 )
ASYNCLOADTHREADFUNCTION_MACRO( 24 )
ASYNCLOADTHREADFUNCTION_MACRO( 25 )
ASYNCLOADTHREADFUNCTION_MACRO( 26 )
ASYNCLOADTHREADFUNCTION_MACRO( 27 )
ASYNCLOADTHREADFUNCTION_MACRO( 28 )
ASYNCLOADTHREADFUNCTION_MACRO( 29 )
ASYNCLOADTHREADFUNCTION_MACRO( 30 )
ASYNCLOADTHREADFUNCTION_MACRO( 31 )

void ( *ASyncLoadThreadFunctionList[ ASYNCLOADTHREAD_MAXNUM ] )( THREAD_INFO *pThreadInfo, void *ASyncLoadThreadData ) =
{
	ASyncLoadThreadFunctionT0,
	ASyncLoadThreadFunctionT1,
	ASyncLoadThreadFunctionT2,
	ASyncLoadThreadFunctionT3,
	ASyncLoadThreadFunctionT4,
	ASyncLoadThreadFunctionT5,
	ASyncLoadThreadFunctionT6,
	ASyncLoadThreadFunctionT7,
	ASyncLoadThreadFunctionT8,
	ASyncLoadThreadFunctionT9,
	ASyncLoadThreadFunctionT10,
	ASyncLoadThreadFunctionT11,
	ASyncLoadThreadFunctionT12,
	ASyncLoadThreadFunctionT13,
	ASyncLoadThreadFunctionT14,
	ASyncLoadThreadFunctionT15,
	ASyncLoadThreadFunctionT16,
	ASyncLoadThreadFunctionT17,
	ASyncLoadThreadFunctionT18,
	ASyncLoadThreadFunctionT19,
	ASyncLoadThreadFunctionT20,
	ASyncLoadThreadFunctionT21,
	ASyncLoadThreadFunctionT22,
	ASyncLoadThreadFunctionT23,
	ASyncLoadThreadFunctionT24,
	ASyncLoadThreadFunctionT25,
	ASyncLoadThreadFunctionT26,
	ASyncLoadThreadFunctionT27,
	ASyncLoadThreadFunctionT28,
	ASyncLoadThreadFunctionT29,
	ASyncLoadThreadFunctionT30,
	ASyncLoadThreadFunctionT31,
} ;

// �v���O���� --------------------------------------------------------------------

// �񓯊��ǂݍ��ݏ����̏������ƌ�n��

// �񓯊��ǂݍ��ݏ����̏�����
extern int InitializeASyncLoad( DWORD_PTR MainThreadID )
{
	// ���ɏ������ς݂̏ꍇ�͉������Ȃ�
	if( GASyncLoadData.InitializeFlag == TRUE )
		return -1 ;

	// ���C���X���b�h�̂h�c��ۑ�����
	GASyncLoadData.MainThreadID = MainThreadID ;

	// �񓯊��ǂݍ��ݏ����p�̃N���e�B�J���Z�N�V�����̏�����
	CriticalSection_Initialize( &GASyncLoadData.CriticalSection ) ;

	// �����������t���O�𗧂Ă�
	GASyncLoadData.InitializeFlag = TRUE ;

	// ����I��
	return 0 ;
}

// �񓯊��ǂݍ��ݏ������s���X���b�h�𗧂Ă�
extern int SetupASyncLoadThread( int ProcessorNum )
{
	int i ;
	ASYNCLOADTHREADINFO *AInfo ;

	GASyncLoadData.ThreadNum = 32 ;

	// �������O�ɒl���ݒ肳��Ă����� NS_SetASyncLoadThreadNum ���Ăяo��
	if( GASyncLoadData.ThreadMaxResumeNum != 0 )
	{
		NS_SetASyncLoadThreadNum( GASyncLoadData.ThreadMaxResumeNum ) ;
	}
	else
	{
		GASyncLoadData.ThreadMaxResumeNum = ProcessorNum - 1 ;
		if( GASyncLoadData.ThreadMaxResumeNum <= 0 )
		{
			GASyncLoadData.ThreadMaxResumeNum = 1 ;
		}
	}

	_MEMSET( GASyncLoadData.Thread, 0, sizeof( GASyncLoadData.Thread ) ) ;
	AInfo = GASyncLoadData.Thread ;
	for( i = 0 ; i < GASyncLoadData.ThreadNum ; i ++, AInfo ++ )
	{
		if( Thread_Create( &AInfo->ThreadInfo, ASyncLoadThreadFunctionList[ i ], NULL ) == -1 )
			return -1 ;
		Thread_SetPriority( &AInfo->ThreadInfo, DX_THREAD_PRIORITY_LOWEST ) ;
		AInfo->SuspendFlag = TRUE ;
		AInfo->SuspendStartTime = NS_GetNowCount() ;
	}

	// �����Q�āA�S���̃X���b�h���Q��̂�҂�
	Thread_Sleep( 16 ) ;

	// ����I��
	return 0 ;
}

// �񓯊��ǂݍ��ݏ����̃X���b�h�����
extern int CloseASyncLoadThread( void )
{
	int i ;
	ASYNCLOADTHREADINFO *AInfo ;

	GASyncLoadData.ThreadEndRequestFlag = TRUE ;

	AInfo = GASyncLoadData.Thread ;
	for( i = 0 ; i < GASyncLoadData.ThreadNum ; i ++, AInfo ++ )
	{
		if( Thread_IsValid( &AInfo->ThreadInfo ) != 0 )
		{
			// �X���b�h���I������܂ő҂�
			while( AInfo->ExitFlag == 0 )
			{
				ProcessASyncLoadRequestMainThread() ;

				if( GASyncLoadData.ThreadResumeNum == 0 )
				{
					ResumeASyncLoadThread() ;
				}

				Thread_Sleep( 1 ) ;
			}

			// �X���b�h�̃n���h�������
			Thread_Delete( &AInfo->ThreadInfo ) ;
		}
	}

	GASyncLoadData.ThreadEndRequestFlag = FALSE ;

	// ����I��
	return 0 ;
}

// �񓯊��ǂݍ��ݏ����̌�n��
extern int TerminateASyncLoad( void )
{
	// ���Ɍ�n�������ς݂̏ꍇ�͉������Ȃ�
	if( GASyncLoadData.InitializeFlag == FALSE )
		return -1 ;

	// �񓯊��ǂݍ��ݏ����p�̃N���e�B�J���Z�N�V�����̍폜
	CriticalSection_Delete( &GASyncLoadData.CriticalSection ) ;

	// �����������t���O��|��
	GASyncLoadData.InitializeFlag = FALSE ;

	// ����I��
	return 0 ;
}














// �񓯊��ǂݍ��݊֌W

// �񓯊��ǂݍ��݃f�[�^���i�[���郁�����̈���m�ۂ���
extern ASYNCLOADDATA_COMMON *AllocASyncLoadDataMemory( int AddAllocSize )
{
	ASYNCLOADDATA_COMMON *ASyncData ;

	// �������̊m��
	ASyncData = ( ASYNCLOADDATA_COMMON * )DXALLOC( sizeof( ASYNCLOADDATA_COMMON ) + AddAllocSize ) ;
	if( ASyncData == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x5e\x97\x0c\x54\x1f\x67\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x20\x00\x25\x00\x64\x00\x42\x00\x79\x00\x74\x00\x65\x00\x20\x00\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�񓯊��ǂݍ��݃f�[�^���i�[���郁�����̈� %dByte �̊m�ۂɎ��s���܂���" @*/, AddAllocSize + sizeof( ASYNCLOADDATA_COMMON ) )) ;
		return NULL ;
	}

	// �A�h���X��Ԃ�
	return ASyncData ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�ɍ\���̃p�����[�^��ǉ�
extern	void	AddASyncLoadParamStruct( BYTE *Data, int *Addr, const void *Param, int Size )
{
	if( Data )
	{
		*( ( WORD * )( Data + *Addr ) ) = ( WORD )( Size + sizeof( WORD ) ) ;
		_MEMCPY( Data + *Addr + sizeof( WORD ), Param, ( size_t )Size ) ;
	}
	*Addr += Size + sizeof( WORD ) ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�� void �^�|�C���^�̃p�����[�^��ǉ�
extern	void	AddASyncLoadParamConstVoidP( BYTE *Data, int *Addr, const void *Param )
{
	if( Data )
	{
		*( ( const void ** )( Data + *Addr ) ) = Param ;
	}
	*Addr += sizeof( const void * ) ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�� DWORD_PTR �^�̃p�����[�^��ǉ�
extern	void	AddASyncLoadParamDWORD_PTR( BYTE *Data, int *Addr, DWORD_PTR Param )
{
	if( Data )
	{
		*( ( DWORD_PTR * )( Data + *Addr ) ) = Param ;
	}
	*Addr += sizeof( DWORD_PTR ) ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�� LONGLONG �^�̃p�����[�^��ǉ�
extern void AddASyncLoadParamLONGLONG( BYTE *Data, int *Addr, LONGLONG Param )
{
	if( Data )
	{
		*( ( LONGLONG * )( Data + *Addr ) ) = Param ;
	}
	*Addr += sizeof( LONGLONG ) ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�� int �^�̃p�����[�^��ǉ�
extern	void	AddASyncLoadParamInt( BYTE *Data, int *Addr, int Param )
{
	if( Data )
	{
		*( ( int * )( Data + *Addr ) ) = Param ;
	}
	*Addr += sizeof( int ) ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�� BYTE �^�̃p�����[�^��ǉ�
extern	void	AddASyncLoadParamByte( BYTE *Data, int *Addr, BYTE Param )
{
	if( Data )
	{
		*( ( BYTE * )( Data + *Addr ) ) = Param ;
	}
	*Addr += sizeof( BYTE ) ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�� float �^�̃p�����[�^��ǉ�
extern	void	AddASyncLoadParamFloat( BYTE *Data, int *Addr, float Param )
{
	if( Data )
	{
		*( ( float * )( Data + *Addr ) ) = Param ;
	}
	*Addr += sizeof( float ) ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^�ɕ�����p�����[�^��ǉ�
extern	void	AddASyncLoadParamString( BYTE *Data, int *Addr, const wchar_t *Param )
{
	DWORD StrLen ;

	if( Param == NULL )
	{
		StrLen = 0 ;
	}
	else
	{
		StrLen = ( DWORD )( ( _WCSLEN( Param ) + 1 ) * sizeof( wchar_t ) ) ;
	}

	if( Data )
	{
		*( ( WORD * )( Data + *Addr ) ) = ( WORD )( StrLen + sizeof( WORD ) ) ;
		if( StrLen > 0 )
		{
			_MEMCPY( ( Data + *Addr ) + sizeof( WORD ), Param, StrLen ) ;
		}
	}
	*Addr += sizeof( WORD ) + StrLen ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^����\���̃p�����[�^���擾
extern	void *	GetASyncLoadParamStruct( BYTE *Data, int *Addr )
{
	void *Ret ;

	Ret = Data + *Addr + sizeof( WORD ) ;
	*Addr += *( ( WORD * )( Data + *Addr ) ) ;

	return Ret ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���� void �^�|�C���^�̃p�����[�^���擾
extern	void *	GetASyncLoadParamVoidP( BYTE *Data, int *Addr )
{
	void *Ret ;

	Ret = *( ( void ** )( Data + *Addr ) ) ;
	*Addr += sizeof( void * ) ;

	return Ret ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���� DWORD_PTR �^�̃p�����[�^���擾
extern	DWORD_PTR GetASyncLoadParamDWORD_PTR( BYTE *Data, int *Addr )
{
	DWORD_PTR Ret ;

	Ret = *( ( DWORD_PTR * )( Data + *Addr ) ) ;
	*Addr += sizeof( DWORD_PTR ) ;

	return Ret ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���� LONGLONG �^�̃p�����[�^���擾
extern LONGLONG GetASyncLoadParamLONGLONG( BYTE *Data, int *Addr )
{
	LONGLONG Ret ;

	Ret = *( ( LONGLONG * )( Data + *Addr ) ) ;
	*Addr += sizeof( LONGLONG ) ;

	return Ret ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���� int �^�̃p�����[�^���擾
extern	int		GetASyncLoadParamInt( BYTE *Data, int *Addr )
{
	int Ret ;

	Ret = *( ( int * )( Data + *Addr ) ) ;
	*Addr += sizeof( int ) ;

	return Ret ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���� BYTE �^�̃p�����[�^���擾
extern	BYTE	GetASyncLoadParamByte( BYTE *Data, int *Addr )
{
	BYTE Ret ;

	Ret = *( ( BYTE * )( Data + *Addr ) ) ;
	*Addr += sizeof( BYTE ) ;

	return Ret ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���� float �^�̃p�����[�^���擾
extern	float	GetASyncLoadParamFloat( BYTE *Data, int *Addr )
{
	float Ret ;

	Ret = *( ( float * )( Data + *Addr ) ) ;
	*Addr += sizeof( float ) ;

	return Ret ;
}

// �񓯊��ǂݍ��݃f�[�^�̃p�����[�^���當����p�����[�^���擾
extern	wchar_t *	GetASyncLoadParamString( BYTE *Data, int *Addr )
{
	wchar_t *Ret ;
	DWORD StrLen ;

	Ret = ( wchar_t * )( ( Data + *Addr ) + sizeof( WORD ) ) ;
	StrLen = *( ( WORD * )( Data + *Addr ) ) - sizeof( WORD ) ;
	*Addr += *( ( WORD * )( Data + *Addr ) ) ;

	return StrLen == 0 ? NULL : Ret ;
}

// �񓯊��ǂݍ��݃f�[�^��ǉ�����
extern int AddASyncLoadData( ASYNCLOADDATA_COMMON *ASyncData )
{
	int NewIndex ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &GASyncLoadData.CriticalSection ) ;

	// ���ɔ񓯊��ǂݍ��݃f�[�^���ő吔�ɒB���Ă���ꍇ�̓G���[
	if( GASyncLoadData.DataNum == ASYNCLOADDATA_MAXNUM )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;

		return DxLib_ErrorUTF16LE( "\x5e\x97\x0c\x54\x1f\x67\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x70\x65\x4c\x30\x31\x8a\xb9\x5b\xcf\x91\x92\x30\x8a\x8d\x48\x30\x7e\x30\x57\x30\x5f\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x59\x30\x00"/*@ L"�񓯊��ǂݍ��݃f�[�^�̐������e�ʂ��z���܂����I�����܂�" @*/ ) ;
	}

	// �g�p����Ă��Ȃ��z��v�f��T��
	if( GASyncLoadData.DataArea != ASYNCLOADDATA_MAXNUM )
	{
		NewIndex = GASyncLoadData.DataArea ;
	}
	else
	{
		for( NewIndex = 0 ; NewIndex < ASYNCLOADDATA_MAXNUM && GASyncLoadData.Data[ NewIndex ] != NULL ; NewIndex ++ ){}
	}

	// ���������e���m�肵�Ă��郁���o�ϐ��̂ݏ�����
	ASyncData->Index = NewIndex ;
	ASyncData->Run = FALSE ;

	// �o�^���Ԃ�ۑ�
	ASyncData->StartTime = NS_GetNowCount() ;

	// �g�p����Ă���|�C���^�����݂���͈͂��X�V����
	if( GASyncLoadData.DataArea == NewIndex ) GASyncLoadData.DataArea ++ ;

	// �f�[�^�|�C���^�̕ۑ�
	GASyncLoadData.Data[ NewIndex ] = ASyncData ;

	// �񓯊��ǂݍ��݃f�[�^�̐��𑝂₷
	GASyncLoadData.DataNum ++ ;

	// �ǂݍ��݃X���b�h����ғ���Ԃɂ���
	ResumeASyncLoadThread() ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;

	// ����I��
	return 0 ;
}


// �w��̔񓯊��ǂݍ��݃f�[�^���폜����
extern int DeleteASyncLoadData( int DeleteIndex, int MainThread )
{
	ASYNCLOADDATA_COMMON *ASyncData ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &GASyncLoadData.CriticalSection ) ;

	// �f�[�^�̃A�h���X���擾
	ASyncData = GASyncLoadData.Data[ DeleteIndex ] ;

	// �w��̃C���f�b�N�X�����ɉ������Ă����牽�����Ȃ�
	if( ASyncData == NULL )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;
		return -1 ;
	}

	// ���ɏ����������Ă�����G���[
	if( MainThread && ASyncData->Run )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;
		return -2 ;
	}

	// �񓯊��ǂݍ��݃f�[�^�̑��������炷
	GASyncLoadData.DataNum -- ;

	// �L���Ȕ񓯊��ǂݍ��݃f�[�^�����݂���͈͂̍X�V
	if( DeleteIndex == GASyncLoadData.DataArea - 1 )
	{
		if( GASyncLoadData.DataNum == 0 ) GASyncLoadData.DataArea = 0 ;
		else
		{
			int DataArea = GASyncLoadData.DataArea - 1 ;
			while( GASyncLoadData.Data[ DataArea - 1 ] == NULL ) DataArea -- ; 
			GASyncLoadData.DataArea = DataArea ;
		}
	}

	// �f�[�^�̈���������
	DXFREE( ASyncData ) ;

	// �e�[�u���� NULL ���Z�b�g����
	GASyncLoadData.Data[ DeleteIndex ] = NULL ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;

	// ����I��
	return 0 ;
}


// �񓯊��ǂݍ��݂̏������s��
extern int ProcessASyncLoad( int ThreadNumber )
{
	int i ;
	ASYNCLOADDATA_COMMON *Data ;
	ASYNCLOADTHREADINFO *AInfo ;

	AInfo = &GASyncLoadData.Thread[ ThreadNumber ] ;

	// �f�[�^�����������牽�����Ȃ�
	if( GASyncLoadData.DataArea == 0 )
		return -2 ;

	// ��������Ă��Ȃ��f�[�^��T��

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &GASyncLoadData.CriticalSection ) ;

	// �f�[�^�͈̔͂̂݃��[�v
	Data = NULL ;
	for( i = 0 ; i < GASyncLoadData.DataArea ; i ++ )
	{
		// �f�[�^�����������牽�����Ȃ�
		if( GASyncLoadData.Data[ i ] == NULL ) continue ;

		Data = GASyncLoadData.Data[ i ] ;

		// ���ɂǂꂩ�̃X���b�h���������Ă���ꍇ�͉������Ȃ�
		if( Data->Run )
		{
			Data = NULL ;
			continue ;
		}

		break ;
	}

	// ���������s���t���O�𗧂Ă�
	if( Data )
	{
		Data->Run = TRUE ;
	}

	// �d�����t���O�𗧂Ă�
	AInfo->JobFlag = TRUE ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;

	if( Data )
	{
		// �ǂݍ��ݏ���
		Data->ProcessFunction( Data ) ;

		// �ǂݍ��ݏ������I��������폜����
		DeleteASyncLoadData( i ) ;
	}

	// ����I��
	return 0 ;
}

// ���݂̃X���b�h�����C���X���b�h���ǂ������擾����( TRUE:���C���X���b�h  FALSE:����ȊO�̃X���b�h )
extern int CheckMainThread( void )
{
	return Thread_GetCurrentId() == GASyncLoadData.MainThreadID ? TRUE : FALSE ;
}

// �񓯊��ǂݍ��݂��s���X���b�h
void ASyncLoadThreadFunction( THREAD_INFO *pThreadInfo, void * /*ASyncLoadThreadData*/ )
{
	int i ;
	int Index = 0 ;
//	int Ret ;
	int ThreadNumber ;
	ASYNCLOADDATA_COMMON *Data ;
	ASYNCLOADTHREADINFO *AInfo ;

	// �X���b�h�̔ԍ����擾���Ă���
	AInfo = GASyncLoadData.Thread ;
	for( ThreadNumber = 0 ; ThreadNumber < ASYNCLOADTHREAD_MAXNUM ; ThreadNumber ++, AInfo ++ )
	{
		if( &AInfo->ThreadInfo == pThreadInfo )
			break ;
	}

	for(;;)
	{
		// �X���b�h���I�����ׂ��Ƃ����t���O���������烋�[�v����O���
		if( GASyncLoadData.ThreadEndRequestFlag == TRUE ) goto ENDLABEL ;

		// �f�[�^�����������牽�����Ȃ�
		if( GASyncLoadData.DataArea == 0 )
		{
SLEEP_LABEL :
			// �N���e�B�J���Z�N�V�����̎擾
			CRITICALSECTION_LOCK( &GASyncLoadData.CriticalSection ) ;

			// �X���b�h��Q��������
			AInfo->SuspendFlag = TRUE ;
			AInfo->SuspendStartTime = NS_GetNowCount() ;
			GASyncLoadData.ThreadResumeNum -- ;

			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;

			// �x�~��Ԃɂ���
			Thread_Suspend( pThreadInfo ) ;

			// �ŏ��ɖ߂�
			continue ;
		}

		// ��������Ă��Ȃ���ԌÂ��f�[�^��T��

		// �N���e�B�J���Z�N�V�����̎擾
		CRITICALSECTION_LOCK( &GASyncLoadData.CriticalSection ) ;

		// �f�[�^�͈̔͂̂݃��[�v
		Data = NULL ;
		for( i = 0 ; i < GASyncLoadData.DataArea ; i ++ )
		{
			if( GASyncLoadData.Data[ i ] == NULL || GASyncLoadData.Data[ i ]->Run )
				continue ;

			if( Data != NULL && Data->StartTime < GASyncLoadData.Data[ i ]->StartTime )
				continue ;

			Data = GASyncLoadData.Data[ i ] ;
			Index = i ;
		}
		if( Data == NULL )
		{
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;

			// �N��������X���b�h���N����
			ResumeASyncLoadThread( 1 ) ;

			// �Q��
			goto SLEEP_LABEL ;
		}

		// �f�[�^���������t���O�𗧂Ă�
		Data->Run = TRUE ;

		// �d�����t���O�𗧂Ă�
		AInfo->JobFlag = TRUE ;

		// �S���̃f�[�^�̃A�h���X��ۑ�
		AInfo->Data = Data ;

		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;

		// �ǂݍ��ݏ���
		Data->ProcessFunction( Data ) ;

		// �N���e�B�J���Z�N�V�����̎擾
		CRITICALSECTION_LOCK( &GASyncLoadData.CriticalSection ) ;

		// �ǂݍ��ݏ������I��������폜����
		DeleteASyncLoadData( Index ) ;

		// �d�����t���O��|��
		AInfo->JobFlag = FALSE ;

		// �S���̃f�[�^�̃A�h���X��������
		AInfo->Data = NULL ;

		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;
	}

ENDLABEL :
	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &GASyncLoadData.CriticalSection ) ;

	// �X���b�h���I���������Ƃ������t���O�𗧂Ă�
	AInfo->ExitFlag = TRUE ;
	GASyncLoadData.ThreadResumeNum -- ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;
}

// �Q�Ă���X���b�h����N����
extern int ResumeASyncLoadThread( int AddMaxThreadNum )
{
	int i ;
	int SelectNo ;
	ASYNCLOADTHREADINFO *AInfo ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &GASyncLoadData.CriticalSection ) ;

	// �N���Ă���X���b�h����萔�ȏゾ������N�����Ȃ�
	if( GASyncLoadData.ThreadResumeNum >= GASyncLoadData.ThreadMaxResumeNum + AddMaxThreadNum )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;

		return 0 ;
	}

	for(;;)
	{
		// �d�������Ă��ă��C���X���b�h�����҂��ł͂Ȃ���ԌÂ��f�[�^��S�����Ă���X���b�h����������
		SelectNo = -1 ;
		AInfo = GASyncLoadData.Thread ;
		for( i = 0 ; i < GASyncLoadData.ThreadNum ; i ++, AInfo ++ )
		{
			if( AInfo->ExitFlag == TRUE ||
				AInfo->SuspendFlag == FALSE ||
				AInfo->JobFlag == FALSE ||
				AInfo->MainThreadRequest == TRUE )
				continue ;

			if( SelectNo == -1 ||
				AInfo->Data->StartTime < GASyncLoadData.Thread[ SelectNo ].Data->StartTime )
			{
				SelectNo = i ;
			}
		}

		// �d�������Ă��郁�C���X���b�h�����҂��ł͂Ȃ��X���b�h�������������ԐQ�Ă��鎞�Ԃ̒����X���b�h����������
		if( SelectNo == -1 )
		{
			SelectNo = -1 ;
			AInfo = GASyncLoadData.Thread ;
			for( i = 0 ; i < GASyncLoadData.ThreadNum ; i ++, AInfo ++ )
			{
				if( AInfo->ExitFlag == TRUE ||
					AInfo->SuspendFlag == FALSE ||
					AInfo->MainThreadRequest == TRUE )
					continue ;

				if( SelectNo == -1 ||
					AInfo->SuspendStartTime < GASyncLoadData.Thread[ SelectNo ].SuspendStartTime )
				{
					SelectNo = i ;
				}
			}
		}

		// ���������������X���b�h���N�����̂�f�O����
		if( SelectNo == -1 )
			break ;

		// �N����
		AInfo = &GASyncLoadData.Thread[ SelectNo ] ;
		if( Thread_Resume( &AInfo->ThreadInfo ) == 0 )
		{
			// ���ɋN���Ă������蒼��
			continue ;
		}

		// �Q�Ă���t���O��|��
		AInfo->SuspendFlag = FALSE ;
		GASyncLoadData.ThreadResumeNum ++ ;

		// ���[�v���甲����
		break ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;

	// �I��
	return 0 ;
}

// �񓯊��ǂݍ��݃X���b�h���烁�C���X���b�h�ւ̏����˗�����������
extern int ProcessASyncLoadRequestMainThread( void )
{
	int i ;
	ASYNCLOAD_MAINTHREAD_REQUESTINFO *Data ;
	int StartTime ;
	int RunNum ;

	// ���C���X���b�h�ȊO����Ă΂ꂽ�牽�����Ȃ�
	if( Thread_GetCurrentId() != GASyncLoadData.MainThreadID )
		return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &GASyncLoadData.CriticalSection ) ;

	// �˗�������ꍇ�݈̂˗�������
	if( GASyncLoadData.MainThreadRequestInfoNum != 0 )
	{
		// �˗��̐������J��Ԃ�
		StartTime = NS_GetNowCount() ;
		for( i = 0 ; i < GASyncLoadData.MainThreadRequestInfoNum ; i ++ )
		{
			if( NS_GetNowCount() - StartTime > 2 )
				break ;

			Data = GASyncLoadData.MainThreadRequestInfo[ i ] ;
			Data->Result = Data->Function( Data ) ;
			GASyncLoadData.Thread[ Data->ThreadNo ].MainThreadRequest = FALSE ;

			// �X���b�h���ĊJ����
			ResumeASyncLoadThread() ;
		}

		// �c�����玟��Ɏ����z��
		if( i != GASyncLoadData.MainThreadRequestInfoNum )
		{
			int Num ;

			Num = GASyncLoadData.MainThreadRequestInfoNum - i ;
			_MEMMOVE( &GASyncLoadData.MainThreadRequestInfo[ 0 ], &GASyncLoadData.MainThreadRequestInfo[ i ], sizeof( ASYNCLOAD_MAINTHREAD_REQUESTINFO * ) * Num ) ;
			GASyncLoadData.MainThreadRequestInfoNum = Num ;
		}
		else
		{
			// �˗��̐������Z�b�g
			GASyncLoadData.MainThreadRequestInfoNum = 0 ;
		}
	}

	// �f�[�^�͂���̂ɓ����Ă���X���b�h�����Ȃ��ꍇ�̓X���b�h�𓮂���
	RunNum = GASyncLoadData.ThreadMaxResumeNum - 1 ;
	if( RunNum == 0 )
	{
		RunNum = 1 ;
	}
	if( RunNum > GASyncLoadData.DataNum )
	{
		RunNum = GASyncLoadData.DataNum ;
	}
	if( GASyncLoadData.DataNum != 0 && RunNum > GASyncLoadData.ThreadResumeNum )
	{
		for( i = GASyncLoadData.ThreadResumeNum ; i < RunNum ; i ++ )
		{
			ResumeASyncLoadThread() ;
		}
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;

	// ����I��
	return 0 ;
}

// ���C���X���b�h�ŏ������Ăق����f�[�^�̏���ǉ�����
extern int AddASyncLoadRequestMainThreadInfo( ASYNCLOAD_MAINTHREAD_REQUESTINFO *Info )
{
	int i ;
	DWORD_PTR CurrentThreadId ;
	ASYNCLOADTHREADINFO *AInfo ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &GASyncLoadData.CriticalSection ) ;

	// ���Ƀ��C���X���b�h�ւ̏����˗������ő吔�ɒB���Ă���ꍇ�̓G���[
	if( GASyncLoadData.MainThreadRequestInfoNum == ASYNCLOADDATA_MAXNUM )
	{
		DxLib_ErrorUTF16LE( "\x5e\x97\x0c\x54\x1f\x67\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\xb9\x30\xec\x30\xc3\x30\xc9\x30\x4b\x30\x89\x30\xe1\x30\xa4\x30\xf3\x30\xb9\x30\xec\x30\xc3\x30\xc9\x30\x78\x30\x6e\x30\xe6\x51\x06\x74\x9d\x4f\x3c\x98\x70\x65\x4c\x30\x31\x8a\xb9\x5b\xcf\x91\x92\x30\x8a\x8d\x48\x30\x7e\x30\x57\x30\x5f\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x59\x30\x00"/*@ L"�񓯊��ǂݍ��݃X���b�h���烁�C���X���b�h�ւ̏����˗��������e�ʂ��z���܂����I�����܂�" @*/ ) ;
		goto ERR ;
	}

	// ����ǉ�
	GASyncLoadData.MainThreadRequestInfo[ GASyncLoadData.MainThreadRequestInfoNum ] = Info ;
	Info->Result = 0 ;

	// �X���b�h�ԍ����o
	CurrentThreadId = Thread_GetCurrentId() ;
	AInfo = GASyncLoadData.Thread ;
	for( i = 0 ; i < ASYNCLOADTHREAD_MAXNUM && Thread_GetId( &AInfo->ThreadInfo ) != CurrentThreadId ; i ++, AInfo ++ ){}
	if( i == ASYNCLOADTHREAD_MAXNUM )
	{
		DxLib_ErrorUTF16LE( "\x5e\x97\x0c\x54\x1f\x67\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\xb9\x30\xec\x30\xc3\x30\xc9\x30\xe5\x4e\x16\x59\x4b\x30\x89\x30\xe1\x30\xa4\x30\xf3\x30\xb9\x30\xec\x30\xc3\x30\xc9\x30\x78\x30\x6e\x30\xe6\x51\x06\x74\x9d\x4f\x3c\x98\x4c\x30\x4c\x88\x8f\x30\x8c\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�񓯊��ǂݍ��݃X���b�h�ȊO���烁�C���X���b�h�ւ̏����˗����s���܂���" @*/ ) ;
		goto ERR ;
	}

	// �C�x���g���擾
//	Info->EndEvent = GASyncLoadData.ThreadEvent[ i ] ;

	// �X���b�h�ԍ���ۑ�
	Info->ThreadNo = i ;

	// ���C���X���b�h�ւ̃��N�G�X�g���s���Ă���t���O�𗧂Ă�
	AInfo->MainThreadRequest = TRUE ;

	// �f�[�^�̐����C���N�������g
	GASyncLoadData.MainThreadRequestInfoNum ++ ;

	// �Q�����ɃX���b�h����N����
	ResumeASyncLoadThread( 1 ) ;

	// �X���b�h��Q��������
	AInfo->SuspendFlag = TRUE ;
	AInfo->SuspendStartTime = NS_GetNowCount() ;
	GASyncLoadData.ThreadResumeNum -- ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;

	// �X���b�h���~�߂�
	Thread_Suspend( &AInfo->ThreadInfo ) ;

	// �����I���҂�
//	WaitForSingleObject( Info->EndEvent, INFINITE ) ;

	// ����I��
	return Info->Result ;

ERR :
	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;

	// �G���[�I��
	return -1 ;
}





// �񓯊��ǂݍ��ݒ��̏����̐����擾����
extern int NS_GetASyncLoadNum( void )
{
	int Result ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &GASyncLoadData.CriticalSection ) ;

	Result = GASyncLoadData.DataNum ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;

	return Result ;
}

// �񓯊��ǂݍ��ݏ������s���X���b�h�̐���ݒ肷��
extern int NS_SetASyncLoadThreadNum( int ThreadNum )
{
	// ���������̏ꍇ�͒l������ۑ�����
	if( GASyncLoadData.InitializeFlag == FALSE )
	{
		GASyncLoadData.ThreadMaxResumeNum = ThreadNum ;
	}
	else
	{
		if( ThreadNum <= 0 )
		{
			ThreadNum = 1 ;
		}
		if( ThreadNum > GASyncLoadData.ThreadNum )
		{
			ThreadNum = GASyncLoadData.ThreadNum ;
		}

		// �N���e�B�J���Z�N�V�����̎擾
		CRITICALSECTION_LOCK( &GASyncLoadData.CriticalSection ) ;

		GASyncLoadData.ThreadMaxResumeNum = ThreadNum ;

		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &GASyncLoadData.CriticalSection ) ;
	}

	return 0 ;
}


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_ASYNCLOAD
