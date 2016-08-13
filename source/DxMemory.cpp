// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�������֘A�v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

// �C���N���[�h ------------------------------------------------------------------
#include "DxMemory.h"
#include "DxLib.h"
#include "DxStatic.h"
#include "DxBaseFunc.h"
#include "DxSystem.h"
#include "DxLog.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

#define DXMEMORY_ALIGNED				(16)						// �������̃A���C��


#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )

#define DX_ALLOC_ENTER_CRITICAL_SECTION\
	if( MemData.InitMemoryAllocCriticalSection == 0 )\
	{\
		/* �������m�ہA��������p�N���e�B�J���Z�N�V�����̏����� */\
		CriticalSection_Initialize( &MemData.MemoryAllocCriticalSection ) ;\
		MemData.InitMemoryAllocCriticalSection = 1 ;\
	}\
\
	/* �������m�ۊ֌W�̏��������s���̏ꍇ�͑҂� */\
	CRITICALSECTION_LOCK( &MemData.MemoryAllocCriticalSection ) ;

#define DX_ALLOC_LEAVE_CRITICAL_SECTION\
	/* ���̊֐����g�p���A�t���O��|�� */\
	CriticalSection_Unlock( &MemData.MemoryAllocCriticalSection ) ;

#else

#define DX_ALLOC_ENTER_CRITICAL_SECTION
#define DX_ALLOC_LEAVE_CRITICAL_SECTION

#endif

// �\���̒�` --------------------------------------------------------------------

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

MEMORYDATA MemData ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

static	void		DxPrintAllocSize( void ) ;													// �������̊m�ۑ��ʂ��o�͂���

// �v���O���� --------------------------------------------------------------------

// �������̊m�ۑ��ʂ��o�͂���
static void DxPrintAllocSize( void )
{
	DXST_ERRORLOGFMT_ADDW(( L"\tTotal size:%d(%.3fkb)  Alloc num:%d",MemData.AllocMemorySize, MemData.AllocMemorySize / 1024.0F, MemData.AllocMemoryNum )) ;
}

// �����������̏��������s��
extern int MemoryInitialize( void )
{
	DX_ALLOC_ENTER_CRITICAL_SECTION

	// ���ɏ���������Ă����牽�����Ȃ�
	if( MemData.InitializeFlag )
	{
		DX_ALLOC_LEAVE_CRITICAL_SECTION
		return 0 ;
	}

	// �������t���O�𗧂Ă�
	MemData.InitializeFlag = TRUE ;

	// ���ˑ�����
	if( MemoryInitialize_PF() < 0 )
	{
		DX_ALLOC_LEAVE_CRITICAL_SECTION
		return 1 ;
	}

	// �������������m�ۗp�̃q�[�v��������
	InitializeHeap(
		&MemData.SmallHeap,
		ALLOCMEMTYPE_TLSF,
		DXMEMORY_ALIGNED,
		TRUE,
		DXMEMORY_SMALLHEAP_UNITSIZE,
		0,
		NormalMemory_AutoAlloc_CreateHeapCallback_PF,
		NormalMemory_AutoAlloc_GetHeapAddressCallback_PF,
		NormalMemory_AutoAlloc_GetHeapSizeCallback_PF,
		NormalMemory_AutoAlloc_DeleteHeapCallback_PF
	) ;

	// �傫���������m�ۗp�̃q�[�v��������
	InitializeHeap(
		&MemData.BigHeap,
		ALLOCMEMTYPE_TLSF,
		DXMEMORY_ALIGNED,
		TRUE,
		DXMEMORY_BIGHEAP_UNITSIZE,
		0,
		NormalMemory_AutoAlloc_CreateHeapCallback_PF,
		NormalMemory_AutoAlloc_GetHeapAddressCallback_PF,
		NormalMemory_AutoAlloc_GetHeapSizeCallback_PF,
		NormalMemory_AutoAlloc_DeleteHeapCallback_PF
	) ;

	DX_ALLOC_LEAVE_CRITICAL_SECTION

	// �I��
	return 0 ;
}

// �����������̌�n�����s��
extern int MemoryTerminate( void )
{
	DX_ALLOC_ENTER_CRITICAL_SECTION

	// ���Ɍ�n������Ă����牽�����Ȃ�
	if( MemData.InitializeFlag == FALSE )
	{
		DX_ALLOC_LEAVE_CRITICAL_SECTION
		return 0 ;
	}

	// �������t���O��|��
	MemData.InitializeFlag = FALSE ;

	// �������������m�ۗp�̃q�[�v�̌�n��
	TerminateHeap( &MemData.SmallHeap ) ;

	// �傫���������m�ۗp�̃q�[�v�̌�n��
	TerminateHeap( &MemData.BigHeap ) ;

	// ���ˑ�����
	MemoryTerminate_PF() ;

	DX_ALLOC_LEAVE_CRITICAL_SECTION

	// �I��
	return 0 ;
}

// �����������̎����I����
extern int MemoryProcess( void )
{
	int Time ;
	int NowTime ;
	WORD NowSec ;

	DX_ALLOC_ENTER_CRITICAL_SECTION

	Time = NS_GetNowCount() ;
	if( Time < MemData.StartTime )
	{
		NowTime = ( 0x7fffffff - MemData.StartTime ) + Time ;
	}
	else
	{
		NowTime = Time - MemData.StartTime ;
	}
	NowSec = ( WORD )( NowTime / 1000 ) ;

	// �q�[�v�Ɍ��݂̕b�����Z�b�g
	SetHeapTime( &MemData.SmallHeap, NowSec ) ;
	SetHeapTime( &MemData.BigHeap,   NowSec ) ;

	// ���ˑ�����
	if( MemoryProcess_PF() < 0 )
	{
		DX_ALLOC_LEAVE_CRITICAL_SECTION
		return -1 ;
	}

	DX_ALLOC_LEAVE_CRITICAL_SECTION

	// �I��
	return 0 ;
}

// �������̊m�ۏ󋵂�`�悷��
extern int MemoryDrawAllocInfo( int x, int y, int Width, int Height )
{
	ALLOCMEM_SIZE_TYPE TotalSize ;
	ALLOCMEM_SIZE_TYPE SmallSize ;
	ALLOCMEM_SIZE_TYPE BigSize ;
	int SmallHeight ;
	int BigHeight ;

	SmallSize = GetHeapTotalMemorySize( &MemData.SmallHeap ) ;
	BigSize   = GetHeapTotalMemorySize( &MemData.BigHeap ) ;
	TotalSize = SmallSize + BigSize ;

	SmallHeight = ( int )( SmallSize * ( ALLOCMEM_SIZE_TYPE )Height / TotalSize ) ;
	BigHeight   = Height - SmallHeight ;

	HeapDrawAllocInfo( &MemData.SmallHeap, x, y,               Width, SmallHeight ) ;
	HeapDrawAllocInfo( &MemData.BigHeap,   x, y + SmallHeight, Width, BigHeight   ) ;

	return 0 ;
}









// �������m�یn�֐�

// DxAlloc �̓��������p�֐�
extern void *DxAllocBase( size_t AllocSize, size_t Aligned, const char *File, int Line )
{
	void *ReturnAddress ;

	DX_ALLOC_ENTER_CRITICAL_SECTION

	{
		HEAPINFO *Heap ;

		// �������̏��������s���Ă��Ȃ������珉�������s��
		if( MemData.InitializeFlag == FALSE )
		{
			if( MemoryInitialize() < 0 )
			{
				DX_ALLOC_LEAVE_CRITICAL_SECTION
				return NULL ;
			}
		}

		// �������̊m��
		Heap = AllocSize > DXMEMORY_SMALL_MAX_SIZE ? &MemData.BigHeap : &MemData.SmallHeap ;
		ReturnAddress = AllocMemory( Heap, AllocSize, Aligned, FALSE, File, Line ) ;
		if( ReturnAddress == NULL )
		{
			goto ERR ;
		}

		// �����������Ă���ꍇ�̓��O���o�͂���
		if( ( int )MemData.AllocTrapSize < 0 || MemData.AllocTrapSize == AllocSize || MemData.AllocMemoryPrintFlag == TRUE )
		{
			DXST_ERRORLOG_ADDW( L"mem alloc  " ) ;
			PrintInfoMemory( ReturnAddress ) ;
		}
	}

	// �m�ۂ����������̑��ʂƑ��������Z����
	MemData.AllocMemorySize += GetAllocSize( ReturnAddress ) ;
	MemData.AllocMemoryNum ++ ;

	// �m�ۂ��Ă��郁�����̑��ʂ��o�͂���
	if( MemData.AllocMemorySizeOutFlag == TRUE )
	{
		DxPrintAllocSize() ;
	}

	// �������j��̃`�F�b�N
	if( MemData.AllocMemoryErrorCheckFlag == TRUE )
	{
		NS_DxErrorCheckAlloc() ;
	}

	DX_ALLOC_LEAVE_CRITICAL_SECTION

	// �������A�h���X��Ԃ�
	return ReturnAddress ;

ERR :
	// ������������Ȃ������炻�̎��̃��������_���v����
	NS_DxDumpAlloc() ;

	// �G���[�����o�͂���
	NS_DxErrorCheckAlloc() ;

	DX_ALLOC_LEAVE_CRITICAL_SECTION

	return NULL ;
}


// ���������m�ۂ���
extern void *NS_DxAlloc( size_t AllocSize, const char *File, int Line )
{
	return DxAllocBase( AllocSize, 0, File, Line ) ;
}

// ���������m�ۂ��ĂO�ŏ���������
extern void *NS_DxCalloc( size_t AllocSize, const char *File, int Line )
{
	void *buf ;

	// �������̊m��
	buf = NS_DxAlloc( AllocSize, File, Line ) ;
	if( buf == NULL )
	{
		return NULL ;
	}
	
	// �m�ۂ�����������������
	_MEMSET( buf, 0, AllocSize ) ;

	// �m�ۂ����������̃A�h���X��Ԃ�
	return buf ;
}

// DxRealloc �̓��������p�֐�
extern void *DxReallocBase( void *Memory, size_t AllocSize, size_t Aligned, const char *File, int Line )
{
	void *ReturnAddress ;

	if( Memory == NULL )
	{
		ReturnAddress = NS_DxAlloc( AllocSize, File, Line ) ;

		return ReturnAddress ;
	}

	DX_ALLOC_ENTER_CRITICAL_SECTION

	ALLOCMEM_SIZE_TYPE PrevSize = GetAllocSize( Memory ) ;

	ReturnAddress = ReallocMemory( Memory, FALSE, AllocSize, Aligned, File, Line ) ;
	if( ReturnAddress == NULL )
	{
		goto ERR ;
	}

	// �m�ۂ����������̑��ʂ̏C��
	MemData.AllocMemorySize -= PrevSize ;
	MemData.AllocMemorySize += GetAllocSize( ReturnAddress ) ;

	// �����������Ă���ꍇ�̓��O���o�͂���
	if( (int)MemData.AllocTrapSize < 0 || MemData.AllocTrapSize == AllocSize || MemData.AllocMemoryPrintFlag == TRUE )
	{
		DXST_ERRORLOG_ADDW( L"mem realloc  " ) ;
		PrintInfoMemory( Memory ) ;
	}

	// �m�ۂ��Ă��郁�����̑��ʂ��o�͂���
	if( MemData.AllocMemorySizeOutFlag == TRUE )
	{
		DxPrintAllocSize() ;
	}

	// �������j��̃`�F�b�N
	if( MemData.AllocMemoryErrorCheckFlag == TRUE )
	{
		NS_DxErrorCheckAlloc() ;
	}

	DX_ALLOC_LEAVE_CRITICAL_SECTION

	// �m�ۂ����������A�h���X��Ԃ�
	return ReturnAddress ;

ERR :
	// ������������Ȃ������炻�̎��̃��������_���v����
	NS_DxDumpAlloc() ;

	DX_ALLOC_LEAVE_CRITICAL_SECTION

	return NULL ;
}

// �������̍Ċm�ۂ��s��
extern void *NS_DxRealloc( void *Memory, size_t AllocSize, const char *File, int Line )
{
	return DxReallocBase( Memory, AllocSize, 0, File, Line ) ;
}

// ���������������
extern void NS_DxFree( void *Memory )
{
	// NULL ���n���ꂽ�ꍇ�͉������Ȃ�
	if( Memory == NULL )
	{
		return ;
	}

	DX_ALLOC_ENTER_CRITICAL_SECTION

	// �������j��̃`�F�b�N
	if( MemData.AllocMemoryErrorCheckFlag == TRUE )
	{
		NS_DxErrorCheckAlloc() ;
	}

	ALLOCMEM_SIZE_TYPE AllocSize = GetAllocSize( Memory ) ;

	// ������郁�����̕������m�ۂ����������̑��ʂƐ������炷
	MemData.AllocMemorySize -= AllocSize ;
	MemData.AllocMemoryNum -- ;

	// �����������Ă���ꍇ�̓��O���o�͂���
	if( (int)MemData.AllocTrapSize < 0 || MemData.AllocTrapSize == AllocSize || MemData.AllocMemoryPrintFlag == TRUE )
	{
		DXST_ERRORLOG_ADDW( L"mem free  " ) ;
		PrintInfoMemory( Memory ) ;
	}

	// �������̉��
	FreeMemory( Memory ) ;

	// �m�ۂ��Ă��郁�����̑��ʂ��o�͂���
	if( MemData.AllocMemorySizeOutFlag == TRUE )
	{
		DxPrintAllocSize() ;
	}

	DX_ALLOC_LEAVE_CRITICAL_SECTION
}

// �񋓑Ώۂɂ��郁�����̊m�ۗe�ʂ��Z�b�g����
extern size_t NS_DxSetAllocSizeTrap( size_t Size )
{
	size_t trapsize ;

	trapsize = MemData.AllocTrapSize ;
	MemData.AllocTrapSize = Size ;

	return trapsize ;
}

// �c�w���C�u�������Ń������m�ۂ��s���鎞�ɏ����o�͂��邩�ǂ������Z�b�g����
extern int NS_DxSetAllocPrintFlag( int Flag )
{
	int printflag ;
	
	printflag = MemData.AllocMemoryPrintFlag ;

	MemData.AllocMemoryPrintFlag = Flag ;

	return printflag ;
}

// �m�ۂ��Ă��郁�����T�C�Y���擾����
extern size_t NS_DxGetAllocSize( void )
{
	return MemData.AllocMemorySize ;
}

// �m�ۂ��Ă��郁�����̐����擾����
extern int NS_DxGetAllocNum( void )
{
	return MemData.AllocMemoryNum ;
}

// �m�ۂ��Ă��郁������񋓂���
extern void NS_DxDumpAlloc( void )
{
	DX_ALLOC_ENTER_CRITICAL_SECTION

	DXST_ERRORLOG_ADDW( L"\n" ) ;
	DXST_ERRORLOG_ADDW( L"Alloc memory dump\n" ) ;

	// �������������m�ۂ̏����o��
	HeapInfoDump( &MemData.SmallHeap ) ;

	// �傫���������m�ۂ̏����o��
	HeapInfoDump( &MemData.BigHeap ) ;

	DxPrintAllocSize() ;

	// ���ˑ��̃������m�ۂ̏����o��
	DxDumpAlloc_PF() ;

	DXST_ERRORLOG_ADDW( L"\n" ) ;

	DX_ALLOC_LEAVE_CRITICAL_SECTION
}

// DxAlloc �� DxCalloc �Ŋm�ۂ��Ă��郁�����̏󋵂�`�悷��
extern void NS_DxDrawAlloc( int x, int y, int Width, int Height )
{
	MemoryDrawAllocInfo( x, y, Width, Height ) ;
}

// �m�ۂ�����������񂪔j�󂳂�Ă��Ȃ������ׂ�( -1:�j�󂠂�  0:�Ȃ� )
extern int NS_DxErrorCheckAlloc( void )
{
	DX_ALLOC_ENTER_CRITICAL_SECTION

	// �������������m�ۗp�̃q�[�v�̃G���[�`�F�b�N
	if( HeapErrorCheck( &MemData.SmallHeap ) < 0 )
	{
		return -1 ;
	}

	// �傫���������m�ۗp�̃q�[�v�̃G���[�`�F�b�N
	if( HeapErrorCheck( &MemData.BigHeap ) < 0 )
	{
		return -1 ;
	}

	DX_ALLOC_LEAVE_CRITICAL_SECTION

	// �����������I��
	return 0 ;
}

// ���������m�ہA�������邽�тɊm�ۂ��Ă��郁�����̗e�ʂ��o�͂��邩�ǂ����̃t���O���Z�b�g����
extern int NS_DxSetAllocSizeOutFlag( int Flag )
{
	int OutFlag ;

	OutFlag = MemData.AllocMemorySizeOutFlag ;
	MemData.AllocMemorySizeOutFlag = Flag ;

	return OutFlag ;
}

// �������̊m�ہA������s����x�Ɋm�ۂ��Ă��郁�����m�ۏ�񂪔j�����Ă��Ȃ������ׂ邩�ǂ����̃t���O���Z�b�g����
extern int NS_DxSetAllocMemoryErrorCheckFlag( int Flag )
{
	int CheckFlag ;

	CheckFlag = MemData.AllocMemoryErrorCheckFlag ;
	MemData.AllocMemoryErrorCheckFlag = Flag ;

	return CheckFlag ;
}

// �������_���v
extern void	MemoryDump( void *buffer, int size )
{
#ifndef DX_NON_LITERAL_STRING
	BYTE *dp ;
	int i, j, linenum ;
	wchar_t string[128], string2[128], *sp = NULL, *sp2 = NULL ;

	DXST_ERRORLOG_ADDW( L"\n" ) ;
	linenum = size / 16 ;
	dp = (BYTE *)buffer ;
	for( i = 0 ; i < linenum ; i ++, dp += 16 )
	{
		DXST_ERRORLOGFMT_ADDW(( L"%08x : %02x %02x %02x %02x  %02x %02x %02x %02x  %02x %02x %02x %02x  %02x %02x %02x %02x : %C%C%C%C %C%C%C%C %C%C%C%C %C%C%C%C\n",
			dp, dp[0], dp[1], dp[2], dp[3],  dp[4], dp[5], dp[6], dp[7],  dp[8], dp[9], dp[10], dp[11],  dp[12], dp[13], dp[14], dp[15],
			dp[0], dp[1], dp[2], dp[3],  dp[4], dp[5], dp[6], dp[7],  dp[8], dp[9], dp[10], dp[11],  dp[12], dp[13], dp[14], dp[15] )) ;
	}

	size -= linenum * 16 ;
	j     = 0 ;
	for( i = 0 ; i < size ; i ++, dp ++ )
	{
		if( j == 0 )
		{
			sp  = string  ;
			sp2 = string2 ;
			_SWPRINTF( sp, L"%08x : ", dp ) ;
			sp += 11 ;
		}

		_SWPRINTF( sp, L"%02x ", *dp ) ;
		sp += 3 ;
		sp2[0] = ( wchar_t )*dp ;
		sp2[1] = '\0' ;
		sp2 ++ ;

		j ++ ;
		if( j == 16 )
		{
			_SWPRINTF( sp, L": %s\n", string2 ) ;
			DXST_ERRORLOG_ADDW( string ) ;
			j = 0 ;
		}
	}
	if( j != 0 )
	{
		_SWPRINTF( sp, L": %s\n", string2 ) ;
		DXST_ERRORLOG_ADDW( string ) ;
	}
#endif
}









#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE
