// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�q�[�v�֘A�v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

// �C���N���[�h ------------------------------------------------------------------
#include "DxHeap.h"
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


#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )

#define DX_HEAP_ENTER_CRITICAL_SECTION( Heap )\
	CRITICALSECTION_LOCK( &(Heap)->MemoryAllocCriticalSection ) ;

#define DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )\
	CriticalSection_Unlock( &(Heap)->MemoryAllocCriticalSection ) ;

#else

#define DX_HEAP_ENTER_CRITICAL_SECTION( Heap )
#define DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )

#endif

// �\���̒�` --------------------------------------------------------------------

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �V�����������^�O�Ɋ�{�����Z�b�g����֐�
static	void		AllocMemTag_SetBaseInfo( HEAPINFO *Heap, ALLOCMEMTAG *MemTag, int Use, int NameLineWrite, const char *Name = NULL, int Line = 0 ) ;

static	void		AllocMemTag_SetDebugData( ALLOCMEMTAG *MemTag ) ;							// �f�o�b�O�̈�ɏ����Z�b�g����
static	int			AllocMemTag_CheckDebugData( ALLOCMEMTAG *MemTag ) ;							// �f�o�b�O�̈�̏����`�F�b�N����( 0:����  -1:�G���[ )

// ������������Ȃ������ꍇ�Ɏ����I�ɐV�����q�[�v���m�ۂ��Ă������烁�������m�ۂ��鏈�����s��
static	void *		AllocMemory_AutoAllocHeap( HEAPINFO *Heap, ALLOCMEM_SIZE_TYPE Size, ALLOCMEM_SIZE_TYPE Aligned, int Reverse, const char *Name, int Line ) ;

static	void		Heap_AddNotUseSepMemTag( HEAPINFO *Heap, ALLOCMEMTAG *AddNotUseSepMemTag ) ;								// �q�[�v�̖��g�p�������^�O���X�g�ɐV�����������^�O��ǉ�����
static	void		Heap_SubNotUseSepMemTag( HEAPINFO *Heap, ALLOCMEMTAG *SubNotUseSepMemTag ) ;								// �q�[�v�̖��g�p�������^�O���X�g����w��̃������^�O���O��

static	void		Heap_AddNotUseSepHeapMem( HEAPINFO *Heap, HEAPMEMINFO *AddNotUseSepHeapMem ) ;								// �q�[�v�̖��g�p�q�[�v��������񃊃X�g�ɐV�����q�[�v����������ǉ�����
static	void		Heap_SubNotUseSepHeapMem( HEAPINFO *Heap, HEAPMEMINFO *SubNotUseSepHeapMem ) ;								// �q�[�v�̖��g�p�q�[�v��������񃊃X�g����w��̃q�[�v�����������O��

static	void		Heap_AddUseMemTag(  HEAPINFO *Heap, ALLOCMEMTAG *AddUseMemTag, ALLOCMEMTAG *PrevUseMemTag ) ;				// �q�[�v�̎g�p���������X�g�ɐV�����������^�O��ǉ�����
static	void		Heap_SubUseMemTag(  HEAPINFO *Heap, ALLOCMEMTAG *SubUseMemTag ) ;											// �q�[�v�̎g�p���������X�g����w��̃������^�O���O��

// ALLOCMEMTYPE_SIMPLE�p�֐�
static	void		Heap_AddFreeMemTag_Simple( HEAPINFO *Heap, ALLOCMEMTAG *AddFreeMemTag, ALLOCMEMTAG *PrevFreeMemTag ) ;		// �q�[�v�̋󂫃��������X�g�ɐV�����������^�O��ǉ�����( ALLOCMEMTYPE_SIMPLE�p )
static	void		Heap_SubFreeMemTag_Simple( HEAPINFO *Heap, ALLOCMEMTAG *SubFreeMemTag ) ;									// �q�[�v�̋󂫃��������X�g����w��̃������^�O���O��( ALLOCMEMTYPE_SIMPLE�p )

// ALLOCMEMTYPE_TLSF�p�֐�
static	void		Heap_AddFreeMemTag_TLSF( HEAPINFO *Heap, ALLOCMEMTAG *AddFreeMemTag ) ;										// �q�[�v�̋󂫃��������X�g�ɐV�����������^�O��ǉ�����( ALLOCMEMTYPE_TLSF�p )
static	void		Heap_SubFreeMemTag_TLSF( HEAPINFO *Heap, ALLOCMEMTAG *SubFreeMemTag ) ;										// �q�[�v�̋󂫃��������X�g����w��̃������^�O���O��( ALLOCMEMTYPE_TLSF�p )

// �w���p�[�֐�
static	void *		Heap_AllocMemory( HEAPINFO *Heap, ALLOCMEMTAG *MemTag, DWORD_PTR UseAddress, ALLOCMEM_SIZE_TYPE UserSize, ALLOCMEM_SIZE_TYPE Size, ALLOCMEM_SIZE_TYPE Aligned, int Reverse, const char *Name, int Line ) ;		// �w��̋󂫃������^�O�ɑ΂��Ďw��T�C�Y�̃��������m�ۂ����ۂ̏������s��
static	void		Heap_FreeMemory(  HEAPINFO *Heap, ALLOCMEMTAG *MemTag, int ReallocProcess = FALSE ) ;						// �w��̎g�p�������^�O��������鏈�����s��


// �r�b�g�֌W

// �r�b�g�̐��𐔂���
static __inline DWORD GetBitCount( ALLOCMEM_SIZE_TYPE Num )
{
#ifdef __64BIT__

#if defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
	Num = (   Num        & 0x5555555555555555ULL ) + ( ( Num >>  1 ) & 0x5555555555555555ULL ) ;
	Num = (   Num        & 0x3333333333333333ULL ) + ( ( Num >>  2 ) & 0x3333333333333333ULL ) ;
	Num = (   Num        & 0x0f0f0f0f0f0f0f0fULL ) + ( ( Num >>  4 ) & 0x0f0f0f0f0f0f0f0fULL ) ;
	Num = (   Num        & 0x00ff00ff00ff00ffULL ) + ( ( Num >>  8 ) & 0x00ff00ff00ff00ffULL ) ;
	Num = (   Num        & 0x0000ffff0000ffffULL ) + ( ( Num >> 16 ) & 0x0000ffff0000ffffULL ) ;
	Num = (   Num        & 0x00000000ffffffffULL ) + ( ( Num >> 32 ) & 0x00000000ffffffffULL ) ;
#else // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
	Num = (   Num        & 0x5555555555555555 ) + ( ( Num >>  1 ) & 0x5555555555555555 ) ;
	Num = (   Num        & 0x3333333333333333 ) + ( ( Num >>  2 ) & 0x3333333333333333 ) ;
	Num = (   Num        & 0x0f0f0f0f0f0f0f0f ) + ( ( Num >>  4 ) & 0x0f0f0f0f0f0f0f0f ) ;
	Num = (   Num        & 0x00ff00ff00ff00ff ) + ( ( Num >>  8 ) & 0x00ff00ff00ff00ff ) ;
	Num = (   Num        & 0x0000ffff0000ffff ) + ( ( Num >> 16 ) & 0x0000ffff0000ffff ) ;
	Num = (   Num        & 0x00000000ffffffff ) + ( ( Num >> 32 ) & 0x00000000ffffffff ) ;
#endif // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )

#else // __64BIT__
	Num = (   Num        & 0x55555555 ) + ( ( Num >>  1 ) & 0x55555555 ) ;
	Num = (   Num        & 0x33333333 ) + ( ( Num >>  2 ) & 0x33333333 ) ;
	Num = (   Num        & 0x0f0f0f0f ) + ( ( Num >>  4 ) & 0x0f0f0f0f ) ;
	Num = (   Num        & 0x00ff00ff ) + ( ( Num >>  8 ) & 0x00ff00ff ) ;
	Num = (   Num        & 0x0000ffff ) + ( ( Num >> 16 ) & 0x0000ffff ) ;
#endif // __64BIT__

	return ( DWORD )Num ;
}

// �ŏ�ʃr�b�g�̃r�b�g�ԍ����擾����
static __inline int GetMSB( ALLOCMEM_SIZE_TYPE Num )
{
	if( Num == 0 )
	{
		return -1 ;
	}

#ifdef __64BIT__
	Num |= Num >>  1 ;
	Num |= Num >>  2 ;
	Num |= Num >>  4 ;
	Num |= Num >>  8 ;
	Num |= Num >> 16 ;
	Num |= Num >> 32 ;
#else // __64BIT__
	Num |= Num >>  1 ;
	Num |= Num >>  2 ;
	Num |= Num >>  4 ;
	Num |= Num >>  8 ;
	Num |= Num >> 16 ;
#endif // __64BIT__
	return ( int )GetBitCount( Num ) - 1 ;
}

// �ŉ��ʃr�b�g�̃r�b�g�ԍ����擾����
static __inline int GetLSB( ALLOCMEM_SIZE_TYPE Num )
{
	if( Num == 0 )
	{
		return -1 ;
	}

#ifdef __64BIT__
	Num |= Num <<  1 ;
	Num |= Num <<  2 ;
	Num |= Num <<  4 ;
	Num |= Num <<  8 ;
	Num |= Num << 16 ;
	Num |= Num << 32 ;
	return 64 - ( int )GetBitCount( Num ) ;
#else // __64BIT__
	Num |= Num <<  1 ;
	Num |= Num <<  2 ;
	Num |= Num <<  4 ;
	Num |= Num <<  8 ;
	Num |= Num << 16 ;
	return 32 - ( int )GetBitCount( Num ) ;
#endif // __64BIT__
}

// �������m�ۂ̃J�e�S���Q�̒l���擾����
static __inline DWORD GetSLI( ALLOCMEM_SIZE_TYPE Num, DWORD MSB, DWORD N )
{
#ifdef __64BIT__

	#if defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )

		return ( DWORD )( ( Num & ( ( 1ULL << MSB ) - 1ULL ) ) >> ( MSB - N ) ) ;

	#else // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )

		return ( DWORD )( ( Num & ( ( ( ( ULONGLONG )1 ) << MSB ) - 1 ) ) >> ( MSB - N ) ) ;

	#endif // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )

#else // __64BIT__

	return ( Num & ( ( 1 << MSB ) - 1 ) ) >> ( MSB - N ) ;

#endif // __64BIT__
}

// �v���O���� --------------------------------------------------------------------

// �V�����������^�O�Ɋ�{�����Z�b�g����֐�
static	void		AllocMemTag_SetBaseInfo( HEAPINFO *Heap, ALLOCMEMTAG *MemTag, int Use, int NameLineWrite, const char *Name, int Line )
{
	// �}�W�b�N�h�c���Z�b�g
	MemTag->MagicID = ALLOCMEMTAG_MAGIC_ID ;

	if( Use )
	{
		// �g�p���t���O�𗧂Ă�
		MemTag->Flag |= ALLOCMEMTAG_FLAG_USE ;
	}
	else
	{
		// �g�p���t���O��|��
		MemTag->Flag &= ~ALLOCMEMTAG_FLAG_USE ;
	}

	// �w�肪����ꍇ�̂ݖ��O�ƍs�ԍ����X�V����
	if( NameLineWrite )
	{
		// ���O��ۑ�
		if( Name == NULL )
		{
			CL_strcpy( DX_CHARCODEFORMAT_ASCII, MemTag->Name, Use ? "NoName" : "Free" ) ;
		}
		else
		{
//			CL_strncpy( DX_CHARCODEFORMAT_ASCII, MemTag->Name, Name, ALLOCMEMTAG_NAMELENGTH - 1 ) ;
			int Length = ( int )CL_strlen( DX_CHARCODEFORMAT_ASCII, Name ) ;
			CL_strcpy( DX_CHARCODEFORMAT_ASCII, MemTag->Name, &Name[ Length < ( ALLOCMEMTAG_NAMELENGTH - 1 ) ? 0 : Length - ( ALLOCMEMTAG_NAMELENGTH - 1 ) ] ) ;
		}

		// �s�ԍ��ۑ�
		if( Line < 0 )
		{
			Line = 0 ;
		}
		MemTag->Flag &= ~( ALLOCMEMTAG_FLAG_LINE_BIT_16 | ALLOCMEMTAG_FLAG_LINE_BIT_17 ) ;
		MemTag->Line = ( WORD )Line ;
		MemTag->Flag |= ( WORD )( ( DWORD )Line >> 16 ) ;
	}

	MemTag->Padding	= 0 ;

	// �������Ă���q�[�v���Z�b�g
	MemTag->Owner	= Heap ;

	// ID���Z�b�g
	MemTag->ID		= Heap->AllocMemoryID ;
	Heap->AllocMemoryID ++ ;

	// ���Ԃ��Z�b�g
	MemTag->Time	= Heap->AllocMemoryTime ;
}

// �f�o�b�O�̈�ɏ����Z�b�g����֐�
static	void		AllocMemTag_SetDebugData( ALLOCMEMTAG *MemTag )
{
	DWORD *CheckAddress ;

	CheckAddress = ( DWORD * )( ( BYTE * )MemTag + ALLOCMEMTAG_SIZE ) ;
	CheckAddress[ 0 ] = 0xaaaaaaaa ;
	CheckAddress[ 1 ] = 0xaaaaaaaa ;
	CheckAddress[ 2 ] = 0xaaaaaaaa ;
	CheckAddress[ 3 ] = 0xaaaaaaaa ;

	CheckAddress = ( DWORD * )( ( BYTE * )MemTag + MemTag->Size - ALLOCMEMTAG_DEBUGAREA_SIZE ) ;
	CheckAddress[ 0 ] = 0xaaaaaaaa ;
	CheckAddress[ 1 ] = 0xaaaaaaaa ;
	CheckAddress[ 2 ] = 0xaaaaaaaa ;
	CheckAddress[ 3 ] = 0xaaaaaaaa ;
}

// �f�o�b�O�̈�̏����`�F�b�N����( 0:����  -1:�G���[ )
static	int			AllocMemTag_CheckDebugData( ALLOCMEMTAG *MemTag )
{
	DWORD *CheckAddress ;

	CheckAddress = ( DWORD * )( ( BYTE * )MemTag + ALLOCMEMTAG_SIZE ) ;
	if( CheckAddress[ 0 ] != 0xaaaaaaaa || CheckAddress[ 1 ] != 0xaaaaaaaa || CheckAddress[ 2 ] != 0xaaaaaaaa || CheckAddress[ 3 ] != 0xaaaaaaaa )
	{
		return -1 ;
	}

	CheckAddress = ( DWORD * )( ( BYTE * )MemTag + MemTag->Size - ALLOCMEMTAG_DEBUGAREA_SIZE ) ;
	if( CheckAddress[ 0 ] != 0xaaaaaaaa || CheckAddress[ 1 ] != 0xaaaaaaaa || CheckAddress[ 2 ] != 0xaaaaaaaa || CheckAddress[ 3 ] != 0xaaaaaaaa )
	{
		return -1 ;
	}

	return 0 ;
}

// ������������Ȃ������ꍇ�Ɏ����I�ɐV�����q�[�v���m�ۂ��Ă������烁�������m�ۂ��鏈�����s��
static	void *		AllocMemory_AutoAllocHeap( HEAPINFO *Heap, ALLOCMEM_SIZE_TYPE Size, ALLOCMEM_SIZE_TYPE Aligned, int Reverse, const char *Name, int Line )
{
	ALLOCMEM_SIZE_TYPE RequestHeapSize ;

	// �����������m�ۂ����Ȃ��ꍇ�� NULL ��Ԃ�
	if( Heap->AutoAlloc == FALSE )
	{
		return NULL ;
	}

	// �V�����q�[�v�p�������̈�̃T�C�Y������
	RequestHeapSize = Size ;

	// TLSF �̏ꍇ�͗v���������T�C�Y�����啝�ɑ傫�ȃT�C�Y�̃�������
	// �K�v�Ƃ���ꍇ������̂Ō��菈�������������ڂ����s��
	if( Heap->AllocType == ALLOCMEMTYPE_TLSF )
	{
		ALLOCMEM_SIZE_TYPE AddSize ;
		DWORD FLI ;
		DWORD SLI ;
		DWORD TempFLI ;
		DWORD TempSLI ;

		// �K�v�T�C�Y���K�����܂�K�v������̂ŁA�J�e�S���Q�̃C���f�b�N�X�͂ЂƂ傫���Ȃ�
		FLI = GetMSB( Size ) ;
		SLI = GetSLI( Size, FLI, ALLOCMEM_CATEGORY_2_SHIFT_NUM ) ;
		SLI += 1 ;
		if( SLI >= ( 1 << ALLOCMEM_CATEGORY_2_SHIFT_NUM ) )
		{
			SLI -= ( 1 << ALLOCMEM_CATEGORY_2_SHIFT_NUM ) ;
			FLI ++ ;
		}

		// �ǉ�����q�[�v�������Ɋ�]���郁�����e�ʂ����肷��
		do
		{
			AddSize = RequestHeapSize / 100 ;
			if( AddSize <= 1024 * 1024 )
			{
				AddSize = 1024 * 1024 ;
			}
			RequestHeapSize += AddSize ;

			TempFLI = GetMSB( RequestHeapSize ) ;
			TempSLI = GetSLI( RequestHeapSize, TempFLI, ALLOCMEM_CATEGORY_2_SHIFT_NUM ) ;
		}while( TempFLI <= FLI && TempSLI < SLI ) ;
	}

	// �V�����q�[�v�p�������̈��ǉ�
	if( AddHeapMemory( Heap, NULL, RequestHeapSize + Heap->AutoAllocUnitSize ) < 0 )
	{
		return NULL ;
	}

	// ���������m��
	return AllocMemory( Heap, Size, Aligned, Reverse, Name, Line ) ;
}

// �q�[�v�̖��g�p�������^�O���X�g�ɐV�����������^�O��ǉ�����
static	void		Heap_AddNotUseSepMemTag( HEAPINFO *Heap, ALLOCMEMTAG *AddNotUseSepMemTag )
{
	Heap->SepTagArrayUseNum -- ;

	AddNotUseSepMemTag->ListNext = Heap->SepTagArrayNotUseFirst ;
	AddNotUseSepMemTag->ListPrev = NULL ;
	Heap->SepTagArrayNotUseFirst = AddNotUseSepMemTag ;
	if( AddNotUseSepMemTag->ListNext != NULL )
	{
		AddNotUseSepMemTag->ListNext->ListPrev = AddNotUseSepMemTag ;
	}
}

// �q�[�v�̖��g�p�������^�O���X�g����w��̃������^�O���O��
static	void		Heap_SubNotUseSepMemTag( HEAPINFO *Heap, ALLOCMEMTAG *SubNotUseSepMemTag )
{
	Heap->SepTagArrayUseNum ++ ;

	if( SubNotUseSepMemTag->ListPrev == NULL )
	{
		Heap->SepTagArrayNotUseFirst = SubNotUseSepMemTag->ListNext ;
	}
	else
	{
		SubNotUseSepMemTag->ListPrev->ListNext = SubNotUseSepMemTag->ListNext ;
	}

	if( SubNotUseSepMemTag->ListNext != NULL )
	{
		SubNotUseSepMemTag->ListNext->ListPrev = SubNotUseSepMemTag->ListPrev ;
	}

	SubNotUseSepMemTag->ListPrev = NULL ;
	SubNotUseSepMemTag->ListNext = NULL ;
}

// �q�[�v�̖��g�p�q�[�v��������񃊃X�g�ɐV�����q�[�v����������ǉ�����
static	void		Heap_AddNotUseSepHeapMem( HEAPINFO *Heap, HEAPMEMINFO *AddNotUseSepHeapMem )
{
	Heap->SepHeapMemArrayUseNum -- ;

	AddNotUseSepHeapMem->Next = Heap->SepHeapMemArrayNotUseFirst ;
	AddNotUseSepHeapMem->Prev = NULL ;
	Heap->SepHeapMemArrayNotUseFirst = AddNotUseSepHeapMem ;
	if( AddNotUseSepHeapMem->Next != NULL )
	{
		AddNotUseSepHeapMem->Next->Prev = AddNotUseSepHeapMem ;
	}
}

// �q�[�v�̖��g�p�q�[�v��������񃊃X�g����w��̃q�[�v�����������O��
static	void		Heap_SubNotUseSepHeapMem( HEAPINFO *Heap, HEAPMEMINFO *SubNotUseSepHeapMem )
{
	Heap->SepHeapMemArrayUseNum ++ ;

	if( SubNotUseSepHeapMem->Prev == NULL )
	{
		Heap->SepHeapMemArrayNotUseFirst = SubNotUseSepHeapMem->Next ;
	}
	else
	{
		SubNotUseSepHeapMem->Prev->Next = SubNotUseSepHeapMem->Next ;
	}

	if( SubNotUseSepHeapMem->Next != NULL )
	{
		SubNotUseSepHeapMem->Next->Prev = SubNotUseSepHeapMem->Prev ;
	}

	SubNotUseSepHeapMem->Prev = NULL ;
	SubNotUseSepHeapMem->Next = NULL ;
}

// �q�[�v�̎g�p���������X�g�ɐV�����������^�O��ǉ�����
static	void		Heap_AddUseMemTag( HEAPINFO *Heap, ALLOCMEMTAG *AddUseMemTag, ALLOCMEMTAG *PrevUseMemTag )
{
	// ��ԋ߂��g�p�������^�O��T��
	while( PrevUseMemTag != NULL && ( PrevUseMemTag->Flag & ALLOCMEMTAG_FLAG_USE ) == 0 )
	{
		PrevUseMemTag = PrevUseMemTag->Prev ;
	}

	AddUseMemTag->ListPrev = PrevUseMemTag ;
	if( AddUseMemTag->ListPrev == NULL )
	{
		AddUseMemTag->ListNext = Heap->UseFirstTag ;
		Heap->UseFirstTag = AddUseMemTag ;
	}
	else
	{
		AddUseMemTag->ListNext = AddUseMemTag->ListPrev->ListNext ;
		AddUseMemTag->ListPrev->ListNext = AddUseMemTag ;
	}

	if( AddUseMemTag->ListNext == NULL )
	{
		Heap->UseLastTag = AddUseMemTag ;
	}
	else
	{
		AddUseMemTag->ListNext->ListPrev = AddUseMemTag ;
	}
}

// �q�[�v�̎g�p���������X�g����w��̃������^�O���O��
static	void		Heap_SubUseMemTag( HEAPINFO *Heap, ALLOCMEMTAG *SubUseMemTag )
{
	if( SubUseMemTag->ListPrev == NULL )
	{
		Heap->UseFirstTag = SubUseMemTag->ListNext ;
	}
	else
	{
		SubUseMemTag->ListPrev->ListNext = SubUseMemTag->ListNext ;
	}

	if( SubUseMemTag->ListNext == NULL )
	{
		Heap->UseLastTag = SubUseMemTag->ListPrev ;
	}
	else
	{
		SubUseMemTag->ListNext->ListPrev = SubUseMemTag->ListPrev ;
	}

	SubUseMemTag->ListPrev = NULL ;
	SubUseMemTag->ListNext = NULL ;
}

// �q�[�v�̋󂫃��������X�g�ɐV�����������^�O��ǉ�����( ALLOCMEMTYPE_SIMPLE�p )
static	void		Heap_AddFreeMemTag_Simple( HEAPINFO *Heap, ALLOCMEMTAG *AddFreeMemTag, ALLOCMEMTAG *PrevFreeMemTag )
{
	AddFreeMemTag->ListPrev = PrevFreeMemTag ;
	if( AddFreeMemTag->ListPrev == NULL )
	{
		AddFreeMemTag->ListNext = Heap->FreeFirstTag ;
		Heap->FreeFirstTag      = AddFreeMemTag ;
	}
	else
	{
		AddFreeMemTag->ListNext = AddFreeMemTag->ListPrev->ListNext ;
		AddFreeMemTag->ListPrev->ListNext = AddFreeMemTag ;
	}

	if( AddFreeMemTag->ListNext == NULL )
	{
		Heap->FreeLastTag = AddFreeMemTag ;
	}
	else
	{
		AddFreeMemTag->ListNext->ListPrev = AddFreeMemTag ;
	}
}

// �q�[�v�̋󂫃��������X�g����w��̃������^�O���O��( ALLOCMEMTYPE_SIMPLE�p )
static	void		Heap_SubFreeMemTag_Simple( HEAPINFO *Heap, ALLOCMEMTAG *SubFreeMemTag )
{
	if( SubFreeMemTag->ListPrev == NULL )
	{
		Heap->FreeFirstTag = SubFreeMemTag->ListNext ;
	}
	else
	{
		SubFreeMemTag->ListPrev->ListNext = SubFreeMemTag->ListNext ;
	}

	if( SubFreeMemTag->ListNext == NULL )
	{
		Heap->FreeLastTag = SubFreeMemTag->ListPrev ;
	}
	else
	{
		SubFreeMemTag->ListNext->ListPrev = SubFreeMemTag->ListPrev ;
	}

	SubFreeMemTag->ListPrev = NULL ;
	SubFreeMemTag->ListNext = NULL ;
}

// �q�[�v�̋󂫃��������X�g�ɐV�����������^�O��ǉ�����( ALLOCMEMTYPE_TLSF�p )
static	void		Heap_AddFreeMemTag_TLSF( HEAPINFO *Heap, ALLOCMEMTAG *AddFreeMemTag )
{
	// �J�e�S���P���擾����
	AddFreeMemTag->FLI = ( BYTE )GetMSB( AddFreeMemTag->Size ) ;

	// �J�e�S���Q���擾����
	AddFreeMemTag->SLI = ( BYTE )GetSLI( AddFreeMemTag->Size, AddFreeMemTag->FLI, ALLOCMEM_CATEGORY_2_SHIFT_NUM ) ;

	// �J�e�S���P�̃t���[�r�b�g�𗧂Ă�
#ifdef __64BIT__
	#if defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
		Heap->FreeTagBitList1 |= 1ULL << AddFreeMemTag->FLI ;
	#else // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
		Heap->FreeTagBitList1 |= ( ( ULONGLONG )1 )  << AddFreeMemTag->FLI ;
	#endif // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
#else // __64BIT__
	Heap->FreeTagBitList1 |= 1    << AddFreeMemTag->FLI ;
#endif // __64BIT__

	// �J�e�S���Q�̃t���[�r�b�g�𗧂Ă�
	Heap->FreeTagBitList2[ AddFreeMemTag->FLI ] |= 1 << AddFreeMemTag->SLI ;

	// �󂫃��������X�g�̐擪�ɒǉ�����
	AddFreeMemTag->ListPrev = NULL ;
	AddFreeMemTag->ListNext = Heap->FreeFirstTagArray[ AddFreeMemTag->FLI ][ AddFreeMemTag->SLI ] ;
	Heap->FreeFirstTagArray[ AddFreeMemTag->FLI ][ AddFreeMemTag->SLI ] = AddFreeMemTag ;

	if( AddFreeMemTag->ListNext == NULL )
	{
		Heap->FreeFirstTagArray[ AddFreeMemTag->FLI ][ AddFreeMemTag->SLI ] = AddFreeMemTag ;
	}
	else
	{
		AddFreeMemTag->ListNext->ListPrev = AddFreeMemTag ;
	}
}

// �q�[�v�̋󂫃��������X�g����w��̃������^�O���O��( ALLOCMEMTYPE_TLSF�p )
static	void		Heap_SubFreeMemTag_TLSF( HEAPINFO *Heap, ALLOCMEMTAG *SubFreeMemTag )
{
	// �󂫃��������X�g����O��
	if( SubFreeMemTag->ListPrev == NULL )
	{
		Heap->FreeFirstTagArray[ SubFreeMemTag->FLI ][ SubFreeMemTag->SLI ] = SubFreeMemTag->ListNext ;
	}
	else
	{
		SubFreeMemTag->ListPrev->ListNext = SubFreeMemTag->ListNext ;
	}

	if( SubFreeMemTag->ListNext == NULL )
	{
		Heap->FreeLastTagArray[ SubFreeMemTag->FLI ][ SubFreeMemTag->SLI ] = SubFreeMemTag->ListPrev ;
	}
	else
	{
		SubFreeMemTag->ListNext->ListPrev = SubFreeMemTag->ListPrev ;
	}
	SubFreeMemTag->ListPrev = NULL ;
	SubFreeMemTag->ListNext = NULL ;

	// �����g�p���Ă����J�e�S���Q����ЂƂ��󂫃������^�O�������Ȃ�����t���[�r�b�g��|��
	if( Heap->FreeFirstTagArray[ SubFreeMemTag->FLI ][ SubFreeMemTag->SLI ] == NULL )
	{
		Heap->FreeTagBitList2[ SubFreeMemTag->FLI ] &= ~( 1 << SubFreeMemTag->SLI ) ;

		// �����g�p���Ă����J�e�S���P�Ǌ��̃J�e�S���Q����ЂƂ��󂫃������^�O�������Ȃ�����t���[�r�b�g��|��
		if( Heap->FreeTagBitList2[ SubFreeMemTag->FLI ] == 0 )
		{
			#ifdef __64BIT__
				#if defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
					Heap->FreeTagBitList1 &= ~( 1ULL << SubFreeMemTag->FLI ) ;
				#else // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
					Heap->FreeTagBitList1 &= ~( ( ( ULONGLONG )1 )  << SubFreeMemTag->FLI ) ;
				#endif // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
			#else // __64BIT__
				Heap->FreeTagBitList1 &= ~( 1 << SubFreeMemTag->FLI ) ;
			#endif // __64BIT__
		}
	}
}

// �w���p�[�֐�

// �w��̋󂫃������^�O�ɑ΂��Ďw��̃��������m�ۂ����ۂ̏������s��
static void * Heap_AllocMemory( HEAPINFO *Heap, ALLOCMEMTAG *MemTag, DWORD_PTR UseAddress, ALLOCMEM_SIZE_TYPE UserSize, ALLOCMEM_SIZE_TYPE Size, ALLOCMEM_SIZE_TYPE Aligned, int Reverse, const char *Name, int Line )
{
	void *ReturnAddress = NULL ;

	// �^�O�z��𕪗����Ă���ꍇ�͋󂫂���͖����ƃG���[
	if( Heap->UseSeparateInfo )
	{
		if( Heap->SepTagArrayNotUseFirst == NULL )
		{
			return NULL ;
		}
	}

	// �g�p���Ă��郁�����̈�̐��𑝂₷
	Heap->UseTagNum ++ ;

	// �������̈���g�p���Ă��鐔�𑝂₷
	MemTag->UseHeapMem->UseMemTagCount ++ ;

	// ���o�[�X���������m�ۃ^�C�v�� ALLOCMEMTYPE_SIMPLE �̏ꍇ�ƁA����ȊO�ŏ����𕪊�
	if( Heap->AllocType == ALLOCMEMTYPE_SIMPLE && Reverse )
	{
		// �󂫃���������K�v�ȃ������T�C�Y�������Ă��A�c�����󂫃������Ƀ������^�O���������ꍇ��
		// �����̋󂫃������^�O�͂��̂܂܂ɁA�g�p�������^�O��V�K�ɍ쐬����
		{
			ALLOCMEM_SIZE_TYPE	NewMemTagAddress ;
			ALLOCMEM_SIZE_TYPE	AfterFreeSize ;

			// �c��t���[�������̃T�C�Y���Z�o
			if( Heap->UseSeparateInfo )
			{
				AfterFreeSize = UseAddress - ( DWORD_PTR )MemTag->AllocAddress ;
			}
			else
			{
				// �V���ȃ������^�O�̃A�h���X���Z�o
				{
					NewMemTagAddress = UseAddress - ( ALLOCMEMTAG_TOTAL_SIZE - ALLOCMEMTAG_DEBUGAREA_SIZE ) ;

					// �A���C����K�p
					NewMemTagAddress = NewMemTagAddress / Aligned * Aligned ;
				}

				// �c��t���[�������̃T�C�Y���Z�o
				AfterFreeSize = NewMemTagAddress - ( DWORD_PTR )MemTag ;
			}

			// ��^�O����邱�Ƃ��o���邩�`�F�b�N
			if( ( Heap->UseSeparateInfo          && AfterFreeSize >= ALLOCMEM_MIN_ALIGNED ) ||
				( Heap->UseSeparateInfo == FALSE && AfterFreeSize >= ALLOCMEMTAG_MIN_SIZE ) )
			{
				ALLOCMEMTAG * NewUseMemTag ;

				// ����ꍇ�͐V�����g�p�������^�O���쐬
				if( Heap->UseSeparateInfo )
				{
					NewUseMemTag = Heap->SepTagArrayNotUseFirst ;
					Heap_SubNotUseSepMemTag( Heap, NewUseMemTag ) ;

					// �g�p���郁�����̈�̃A�h���X���Z�b�g
					NewUseMemTag->AllocAddress = ( void * )UseAddress ;
				}
				else
				{
					NewUseMemTag = ( ALLOCMEMTAG * )NewMemTagAddress ;
				}

				// ��{�����Z�b�g
				NewUseMemTag->Flag = 0 ;
				AllocMemTag_SetBaseInfo( Heap, NewUseMemTag, TRUE, TRUE, Name, Line ) ;

				// �g�p���Ă��郁�����̈���Z�b�g
				NewUseMemTag->UseHeapMem = MemTag->UseHeapMem ;

				// �O��̃������̈�ƘA�����Ă��邩�ǂ������Z�b�g
				NewUseMemTag->Flag |= ALLOCMEMTAG_FLAG_PREV_CONNECT ;
				if( MemTag->Flag & ALLOCMEMTAG_FLAG_NEXT_CONNECT )
				{
					NewUseMemTag->Flag |= ALLOCMEMTAG_FLAG_NEXT_CONNECT ;
				}
				else
				{
					NewUseMemTag->Flag &= ~ALLOCMEMTAG_FLAG_NEXT_CONNECT ;
				}

				// �����̃������m�ۃ^�O�̎��͕K���������̈悪�A�����Ă���
				MemTag->Flag |= ALLOCMEMTAG_FLAG_NEXT_CONNECT ;

				// �T�C�Y���Z�b�g
				if( Heap->UseSeparateInfo )
				{
					NewUseMemTag->Size = ( ( DWORD_PTR )MemTag->AllocAddress + MemTag->Size ) - ( DWORD_PTR )UseAddress ;
				}
				else
				{
					NewUseMemTag->Size = ( ( DWORD_PTR )MemTag + MemTag->Size ) - ( DWORD_PTR )NewUseMemTag ;

					// �f�o�b�O�̈�ɏ����Z�b�g
					AllocMemTag_SetDebugData( NewUseMemTag ) ;
				}

				// �O��̃������ƃ��X�g��A��
				{
					NewUseMemTag->Prev = MemTag ;
					NewUseMemTag->Next = MemTag->Next ;
					MemTag->Next       = NewUseMemTag ;
					if( NewUseMemTag->Next == NULL )
					{
						Heap->LastTag = NewUseMemTag ;
					}
					else
					{
						NewUseMemTag->Next->Prev = NewUseMemTag ;
					}
				}

				// �g�p���������X�g�ɒǉ�����
				Heap_AddUseMemTag( Heap, NewUseMemTag, MemTag->Prev ) ;

				// ���܂ł̃������^�O�̃T�C�Y��ύX
				if( Heap->UseSeparateInfo )
				{
					MemTag->Size = AfterFreeSize ;
				}
				else
				{
					MemTag->Size = NewMemTagAddress - ( DWORD_PTR )MemTag ;

					// ���܂ł̃������^�O�̃T�C�Y���ύX���ꂽ�̂ŁA�f�o�b�O�̈���ēx�Z�b�g
					AllocMemTag_SetDebugData( MemTag ) ;
				}

				// �󂫃������̃T�C�Y�����炷
				Heap->TotalFreeSize -= NewUseMemTag->Size ;

				// �g�p�������̃T�C�Y�𑝂₷
				Heap->TotalUseSize += NewUseMemTag->Size ;

				// �w�肳�ꂽ�T�C�Y���Z�b�g
				NewUseMemTag->UserSize = UserSize ;

				// �Ԃ��A�h���X���Z�b�g
				NewUseMemTag->UserAddress = ( void * )UseAddress ;
				if( Heap->UseSeparateInfo )
				{
					ReturnAddress = NewUseMemTag ;
				}
				else
				{
					ReturnAddress = NewUseMemTag->UserAddress ;

					// �Ԃ��A�h���X�̂S�o�C�g���Ƀ^�O�܂ł̃o�C�g�����Z�b�g
					*( ( DWORD * )( ( BYTE * )NewUseMemTag->UserAddress - sizeof( DWORD ) ) ) = ( DWORD )( UseAddress - ( DWORD_PTR )NewUseMemTag ) ;
				}
			}
			else
			{
				// ����Ȃ��ꍇ�́A�����̃������^�O�S�����g�p�������^�O�ɂ��Ă��܂�

				// �w�肳�ꂽ�T�C�Y���Z�b�g
				MemTag->UserSize = UserSize ;

				// �Ԃ��A�h���X���Z�b�g
				MemTag->UserAddress = ( void * )UseAddress ;
				if( Heap->UseSeparateInfo )
				{
					ReturnAddress = MemTag ;
				}
				else
				{
					ReturnAddress = MemTag->UserAddress ;

					// �Ԃ��A�h���X�̂S�o�C�g���Ƀ^�O�܂ł̃o�C�g�����Z�b�g
					*( ( DWORD * )( ( BYTE * )MemTag->UserAddress - sizeof( DWORD ) ) ) = ( DWORD )( UseAddress - ( DWORD_PTR )MemTag ) ;
				}

				// �������^�O���󂫃��������X�g����O��
				Heap_SubFreeMemTag_Simple( Heap, MemTag ) ;

				// ��{�����Z�b�g
				AllocMemTag_SetBaseInfo( Heap, MemTag, TRUE, TRUE, Name, Line ) ;

				// �g�p���������X�g�ɒǉ�����
				Heap_AddUseMemTag( Heap, MemTag, MemTag->Prev ) ;

				// �󂫃������̐������炷
				Heap->FreeTagNum -- ;

				// �󂫃������̃T�C�Y�����炷
				Heap->TotalFreeSize -= MemTag->Size ;

				// �g�p�������̃T�C�Y�𑝂₷
				Heap->TotalUseSize += MemTag->Size ;
			}
		}
	}
	else
	{
		ALLOCMEMTAG *	FreePrevMemTag ;

		// �w�肳�ꂽ�T�C�Y���Z�b�g
		MemTag->UserSize = UserSize ;

		// �Ԃ��A�h���X���Z�b�g
		MemTag->UserAddress = ( void * )UseAddress ;
		if( Heap->UseSeparateInfo )
		{
			ReturnAddress = MemTag ;
		}
		else
		{
			ReturnAddress = MemTag->UserAddress ;

			// �Ԃ��A�h���X�̂S�o�C�g���Ƀ^�O�܂ł̃o�C�g�����Z�b�g
			*( ( DWORD * )( ( BYTE * )MemTag->UserAddress - sizeof( DWORD ) ) ) = ( DWORD )( UseAddress - ( DWORD_PTR )MemTag ) ;
		}

		// �󂫃������^�O�̈�O�̃^�O���L�����Ă���
		FreePrevMemTag = MemTag->ListPrev ;

		// �������^�O���󂫃��������X�g����O��
		if( Heap->AllocType == ALLOCMEMTYPE_SIMPLE )
		{
			Heap_SubFreeMemTag_Simple( Heap, MemTag ) ;
		}
		else
		if( Heap->AllocType == ALLOCMEMTYPE_TLSF )
		{
			Heap_SubFreeMemTag_TLSF( Heap, MemTag ) ;
		}

		// ��{�����Z�b�g����
		AllocMemTag_SetBaseInfo( Heap, MemTag, TRUE, TRUE, Name, Line ) ;

		// �g�p���������X�g�ɒǉ�����
		Heap_AddUseMemTag( Heap, MemTag, MemTag->Prev ) ;

		// �󂫃���������K�v�ȃ������T�C�Y�������Ă��A�c�����󂫃������Ƀ������^�O���������ꍇ�͋󂫃������^�O�𑝂₷
		{
			ALLOCMEM_SIZE_TYPE	NextAddress ;
			ALLOCMEM_SIZE_TYPE	NextFreeSize ;

			// �c��󂫃������̃T�C�Y���Z�o
			if( Heap->UseSeparateInfo )
			{
				// �c��󂫃������̃T�C�Y���Z�o
				NextFreeSize = ( ( DWORD_PTR )MemTag->AllocAddress + MemTag->Size ) - ( UseAddress + Size ) ;
			}
			else
			{
				// �󂫃��������m�ۂ����ꍇ�Ɏ��̃^�O��z�u�ł���A�h���X���Z�o
				NextAddress = UseAddress + Size + ALLOCMEMTAG_DEBUGAREA_SIZE ;

				// �A���C����K�p
				NextAddress = ( NextAddress + ALLOCMEM_MIN_ALIGNED - 1 ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED ;

				// �c��󂫃������̃T�C�Y���Z�o
				NextFreeSize = ( ( DWORD_PTR )MemTag + MemTag->Size ) - NextAddress ;
			}

			// ��^�O����邱�Ƃ��o���邩�`�F�b�N
			if( ( Heap->UseSeparateInfo          && NextFreeSize >= ALLOCMEM_MIN_ALIGNED ) ||
				( Heap->UseSeparateInfo == FALSE && NextFreeSize >= ALLOCMEMTAG_MIN_SIZE ) )
			{
				ALLOCMEMTAG * NewFreeMemTag ;

				// ����ꍇ�͐V�����󂫃������^�O���쐬
				if( Heap->UseSeparateInfo )
				{
					NewFreeMemTag = Heap->SepTagArrayNotUseFirst ;
					Heap_SubNotUseSepMemTag( Heap, NewFreeMemTag ) ;

					// �g�p���郁�����̈�̃A�h���X���Z�b�g
					NewFreeMemTag->AllocAddress = ( void * )( UseAddress + Size ) ;
				}
				else
				{
					NewFreeMemTag = ( ALLOCMEMTAG * )NextAddress ;
				}

				// ��{�����Z�b�g
				NewFreeMemTag->Flag = 0 ;
				AllocMemTag_SetBaseInfo( Heap, NewFreeMemTag, FALSE, TRUE, "Free", 0 ) ;

				// �g�p���Ă��郁�����̈���Z�b�g
				NewFreeMemTag->UseHeapMem = MemTag->UseHeapMem ;

				// �O��̃������̈�ƘA�����Ă��邩�ǂ������Z�b�g
				NewFreeMemTag->Flag = ALLOCMEMTAG_FLAG_PREV_CONNECT ;
				if( MemTag->Flag & ALLOCMEMTAG_FLAG_NEXT_CONNECT )
				{
					NewFreeMemTag->Flag |= ALLOCMEMTAG_FLAG_NEXT_CONNECT ;
				}
				else
				{
					NewFreeMemTag->Flag &= ~ALLOCMEMTAG_FLAG_NEXT_CONNECT ;
				}

				// �V�����󂫃������^�O�̃T�C�Y���Z�b�g
				if( Heap->UseSeparateInfo )
				{
					NewFreeMemTag->Size		= NextFreeSize ;
				}
				else
				{
					NewFreeMemTag->Size		= ( ( DWORD_PTR )MemTag + MemTag->Size ) - ( DWORD_PTR )NewFreeMemTag ;

					// �f�o�b�O�̈�ɏ����Z�b�g
					AllocMemTag_SetDebugData( NewFreeMemTag ) ;
				}

				// �O��̃������ƃ��X�g��A��
				{
					NewFreeMemTag->Prev = MemTag ;
					NewFreeMemTag->Next = MemTag->Next ;
					MemTag->Next        = NewFreeMemTag ;
					if( NewFreeMemTag->Next == NULL )
					{
						Heap->LastTag = NewFreeMemTag ;
					}
					else
					{
						NewFreeMemTag->Next->Prev = NewFreeMemTag ;
					}
				}

				// �����̃������m�ۃ^�O�̎��͕K���������̈悪�A�����Ă���
				MemTag->Flag |= ALLOCMEMTAG_FLAG_NEXT_CONNECT ;

				// �󂫃��������X�g�ɒǉ�
				if( Heap->AllocType == ALLOCMEMTYPE_SIMPLE )
				{
					Heap_AddFreeMemTag_Simple( Heap, NewFreeMemTag, FreePrevMemTag ) ;
				}
				else
				if( Heap->AllocType == ALLOCMEMTYPE_TLSF )
				{
					Heap_AddFreeMemTag_TLSF( Heap, NewFreeMemTag ) ;
				}

				// ���܂ł̃������^�O�̃T�C�Y��ύX
				if( Heap->UseSeparateInfo )
				{
					MemTag->Size = ( DWORD_PTR )NewFreeMemTag->AllocAddress - ( DWORD_PTR )MemTag->AllocAddress ;
				}
				else
				{
					MemTag->Size = NextAddress - ( DWORD_PTR )MemTag ;

					// ���܂ł̃������^�O�̃T�C�Y���ύX���ꂽ�̂ŁA�f�o�b�O�̈���ēx�Z�b�g
					AllocMemTag_SetDebugData( MemTag ) ;
				}
			}
			else
			{
				// ��邱�Ƃ��o���Ȃ��ꍇ�͋󂫃������^�O�𑝂₳�Ȃ�

				// �󂫃������̐������炷
				Heap->FreeTagNum -- ;
			}

			// �󂫃������̃T�C�Y�����炷
			Heap->TotalFreeSize -= MemTag->Size ;

			// �g�p�������̃T�C�Y�𑝂₷
			Heap->TotalUseSize += MemTag->Size ;
		}
	}

	return ReturnAddress ;
}

// �w��̎g�p�������^�O��������鏈�����s��
static void Heap_FreeMemory( HEAPINFO *Heap, ALLOCMEMTAG *MemTag, int ReallocProcess )
{
	// �g�p���Ă��郁�����̐������炷
	Heap->UseTagNum -- ;

	// �������̈���g�p���Ă��鐔�����炷
	MemTag->UseHeapMem->UseMemTagCount -- ;

	// �󂫃������̐��𑝂₷
	Heap->FreeTagNum ++ ;

	// �g�p�������̃T�C�Y�����炷
	Heap->TotalUseSize -= MemTag->Size ;

	// �󂫃������̃T�C�Y�𑝂₷
	Heap->TotalFreeSize += MemTag->Size ;

	// �g�p�������̃t���O��|��
	MemTag->Flag &= ~ALLOCMEMTAG_FLAG_USE ;

	// �g�p�������̃��X�g����O��
	Heap_SubUseMemTag( Heap, MemTag ) ;
	
	// �󂫃��������X�g�ɒǉ�����
	{
		ALLOCMEMTAG *	SubMemTag ;
		int				AddFreeListFlag = FALSE ;

		// ��Ⴂ�A�h���X�ׂ̗̃��������󂫃�������������A������
		// ( ReallocMemory �̏����̈�̏ꍇ�͒Ⴂ�A�h���X�̋󂫃������Ƃ̘A���͍s��Ȃ� )
		if( ReallocProcess == FALSE &&
			( MemTag->Flag & ALLOCMEMTAG_FLAG_PREV_CONNECT ) != 0 &&
			MemTag->Prev != NULL &&
			( MemTag->Prev->Flag & ALLOCMEMTAG_FLAG_USE ) == 0 )
		{
			SubMemTag = MemTag->Prev ;

			// �m�ۃ^�C�v�� ALLOCMEMTYPE_TLSF �̏ꍇ�́A��U�󂫃��������X�g����O��
			if( Heap->AllocType == ALLOCMEMTYPE_TLSF )
			{
				Heap_SubFreeMemTag_TLSF( Heap, SubMemTag ) ;
			}

			// �T�C�Y�𑫂�
			SubMemTag->Size += MemTag->Size ;

			// ���X�g�̘A����ύX����
			SubMemTag->Next = MemTag->Next ;
			if( SubMemTag->Next == NULL )
			{
				Heap->LastTag = SubMemTag ;
			}
			else
			{
				SubMemTag->Next->Prev = SubMemTag ;
			}

			// ���̃^�O�ƃ������̈悪�A�����Ă��邩���Z�b�g
			if( MemTag->Flag & ALLOCMEMTAG_FLAG_NEXT_CONNECT )
			{
				SubMemTag->Flag |= ALLOCMEMTAG_FLAG_NEXT_CONNECT ;
			}
			else
			{
				SubMemTag->Flag &= ~ALLOCMEMTAG_FLAG_NEXT_CONNECT ;
			}

			// �s�v�ɂȂ����������^�O�𖢎g�p���X�g�ɒǉ�����
			if( Heap->UseSeparateInfo )
			{
				Heap_AddNotUseSepMemTag( Heap, MemTag ) ;
			}

			// �󂫃������̐������炷
			Heap->FreeTagNum -- ;

			// �����Ώۂ̃^�O��ύX
			MemTag = SubMemTag ;

			// MemTag ���t���[���X�g�ɒǉ�����Ă��邩�ǂ����̃t���O�𗧂Ă�
			AddFreeListFlag = TRUE ;
		}

		// ������A�h���X�ׂ̗̃��������󂫃�������������A������
		if( ( MemTag->Flag & ALLOCMEMTAG_FLAG_NEXT_CONNECT ) != 0 &&
			MemTag->Next != NULL &&
			( MemTag->Next->Flag & ALLOCMEMTAG_FLAG_USE ) == 0 )
		{
			SubMemTag = MemTag->Next ;

			// �m�ۃ^�C�v�� ALLOCMEMTYPE_TLSF �̏ꍇ�́A��U�󂫃��������X�g����O��
			if( Heap->AllocType == ALLOCMEMTYPE_TLSF )
			{
				Heap_SubFreeMemTag_TLSF( Heap, SubMemTag ) ;
			}

			// �T�C�Y�𑫂�
			MemTag->Size += SubMemTag->Size ;

			// ���X�g�̘A����ύX����
			MemTag->Next = SubMemTag->Next ;
			if( MemTag->Next == NULL )
			{
				Heap->LastTag = MemTag ;
			}
			else
			{
				MemTag->Next->Prev = MemTag ;
			}

			// �m�ۃ^�C�v�� ALLOCMEMTYPE_SIMPLE �̏ꍇ�́A�����ŋ󂫃��������X�g�̘A����ύX����
			if( Heap->AllocType == ALLOCMEMTYPE_SIMPLE )
			{
				MemTag->ListNext = SubMemTag->ListNext ;
				if( MemTag->ListNext == NULL )
				{
					Heap->FreeLastTag = MemTag ;
				}
				else
				{
					MemTag->ListNext->ListPrev = MemTag ;
				}

				if( AddFreeListFlag == FALSE )
				{
					MemTag->ListPrev = SubMemTag->ListPrev ;
					if( MemTag->ListPrev == NULL )
					{
						Heap->FreeFirstTag = MemTag ;
					}
					else
					{
						MemTag->ListPrev->ListNext = MemTag ;
					}
				}
			}

			// ���̃^�O�ƃ������̈悪�A�����Ă��邩���Z�b�g
			if( SubMemTag->Flag & ALLOCMEMTAG_FLAG_NEXT_CONNECT )
			{
				MemTag->Flag |= ALLOCMEMTAG_FLAG_NEXT_CONNECT ;
			}
			else
			{
				MemTag->Flag &= ~ALLOCMEMTAG_FLAG_NEXT_CONNECT ;
			}

			// �s�v�ɂȂ����������^�O�𖢎g�p���X�g�ɒǉ�����
			if( Heap->UseSeparateInfo )
			{
				Heap_AddNotUseSepMemTag( Heap, SubMemTag ) ;
			}

			// �󂫃������̐������炷
			Heap->FreeTagNum -- ;

			// MemTag ���t���[���X�g�ɒǉ�����Ă��邩�ǂ����̃t���O�𗧂Ă�
			AddFreeListFlag = TRUE ;
		}

		// �m�ۃ^�C�v�ɂ���ď����𕪊�
		if( Heap->AllocType == ALLOCMEMTYPE_SIMPLE )
		{
			// �O��ɋ󂫃����������������炻�̂܂܋󂫃��������X�g�ɒǉ�����
			if( AddFreeListFlag == FALSE )
			{
				// �������A�h���X���Ⴍ�A��ԋ߂��󂫃�������T��
				for( SubMemTag = MemTag->Prev ; SubMemTag != NULL && ( SubMemTag->Flag & ALLOCMEMTAG_FLAG_USE ) != 0 ; SubMemTag = SubMemTag->Prev ){}
				
				// �󂫃��������X�g�ɒǉ�
				Heap_AddFreeMemTag_Simple( Heap, MemTag, SubMemTag ) ;
			}
		}
		else
		if( Heap->AllocType == ALLOCMEMTYPE_TLSF )
		{
			// �󂫃��������X�g�ɒǉ�����
			Heap_AddFreeMemTag_TLSF( Heap, MemTag ) ;
		}
	}

	// �����g�p���Ă��郁�����q�[�v�������g�p�������������Ȃ�A
	// ���� ReallocMemory �̏����̈�ł��Ȃ������Ď����m�ۃ������������ꍇ�͉������
	if( MemTag->UseHeapMem->UseMemTagCount == 0 &&
		ReallocProcess == 0 && ( MemTag->UseHeapMem->Flag & HEAPMEM_FLAG_AUTOALLOC ) != 0 )
	{
		SubHeapMemory( Heap, MemTag->UseHeapMem->Address ) ;
	}
}















// �q�[�v�֐�

// �q�[�v������������
extern int InitializeHeap(
	HEAPINFO *				Heap,
	int						AllocType,
	ALLOCMEM_SIZE_TYPE		Aligned,
	int						AutoAlloc,
	ALLOCMEM_SIZE_TYPE		AutoAllocUnitSize,
	int						AutoAlloc_CallbackParam,
	int					 ( *AutoAlloc_CreateHeapCallback     )( int Param, void *Buffer, ALLOCMEM_SIZE_TYPE Size ),
	void *				 ( *AutoAlloc_GetHeapAddressCallback )( int Param, void *Buffer ),
	ALLOCMEM_SIZE_TYPE	 ( *AutoAlloc_GetHeapSizeCallback    )( int Param, void *Buffer ),
	int					 ( *AutoAlloc_DeleteHeapCallback     )( int Param, void *Buffer ),
	void *					HeapAddress,
	ALLOCMEM_SIZE_TYPE		HeapSize,
	int						UseSeparateInfo,
	int						SepTagArrayLength,
	ALLOCMEMTAG *			SepTagArray,
	int						SepHeapMemArrayLength,
	HEAPMEMINFO *			SepHeapMemArray
)
{
	// �\���̃T�C�Y�G���[�`�F�b�N
	if( sizeof( ALLOCMEMTAG ) != ALLOCMEMTAG_SIZE )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xcb\x69\x20\x90\x53\x4f\x20\x00\x41\x00\x4c\x00\x4c\x00\x4f\x00\x43\x00\x4d\x00\x45\x00\x4d\x00\x54\x00\x41\x00\x47\x00\x20\x00\x6e\x30\xb5\x30\xa4\x30\xba\x30\x4c\x30\x20\x00\x25\x00\x64\x00\x20\x00\x62\x00\x79\x00\x74\x00\x65\x00\x20\x00\x67\x30\x6f\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x00"/*@ L"�\���� ALLOCMEMTAG �̃T�C�Y�� %d byte �ł͂���܂���" @*/, ALLOCMEMTAG_SIZE )) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// �[��������
	_MEMSET( Heap, 0, sizeof( *Heap ) ) ;

#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )

	// �q�[�v�p�̃N���e�B�J���Z�N�V�����̏�����
	CriticalSection_Initialize( &Heap->MemoryAllocCriticalSection ) ;

#endif // defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )

	DX_HEAP_ENTER_CRITICAL_SECTION( Heap )

	// �A���C���� ALLOCMEM_MIN_ALIGNED �o�C�g�̔{���ɂ���
	if( Aligned == 0 )
	{
		Aligned = ALLOCMEM_MIN_ALIGNED ;
	}
	else
	{
		Aligned = ( Aligned + ALLOCMEM_MIN_ALIGNED - 1 ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED ;
	}

	// �p�����[�^��ۑ�
	Heap->AllocType							= AllocType ;
	Heap->Aligned							= Aligned ;
	Heap->AutoAlloc							= AutoAlloc ;
	Heap->AutoAllocUnitSize					= AutoAllocUnitSize ;
	Heap->AutoAlloc_CallbackParam			= AutoAlloc_CallbackParam ;
	Heap->AutoAlloc_CreateHeapCallback		= AutoAlloc_CreateHeapCallback ;
	Heap->AutoAlloc_GetHeapAddressCallback	= AutoAlloc_GetHeapAddressCallback ;
	Heap->AutoAlloc_GetHeapSizeCallback		= AutoAlloc_GetHeapSizeCallback ;
	Heap->AutoAlloc_DeleteHeapCallback		= AutoAlloc_DeleteHeapCallback ;
	Heap->UseSeparateInfo					= UseSeparateInfo ;

	// �������^�O��q�[�v�̏���ʃ������̈�Ŋm�ۂ��邩�ǂ����̏����Z�b�g
	if( Heap->UseSeparateInfo )
	{
		// �z�񂪗L���Ȓl�ł͂Ȃ��ꍇ�̓G���[
		if( SepTagArrayLength     <= 0 || SepTagArray     == NULL ||
			SepHeapMemArrayLength <= 0 || SepHeapMemArray == NULL )
		{
			DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )
			return -1 ;
		}

		// �z��̃A�h���X��z��̒����Ȃǂ��Z�b�g
		Heap->SepTagArrayLength		= SepTagArrayLength ;
		Heap->SepTagArray			= SepTagArray ;
		Heap->SepHeapMemArrayLength	= SepHeapMemArrayLength ;
		Heap->SepHeapMemArray		= SepHeapMemArray ;

		// �[��������
		_MEMSET( SepTagArray,     0, sizeof( *SepTagArray     ) * SepTagArrayLength     ) ;
		_MEMSET( SepHeapMemArray, 0, sizeof( *SepHeapMemArray ) * SepHeapMemArrayLength ) ;

		// ���g�p�������^�O���X�g�̍쐬
		{
			Heap->SepTagArrayNotUseFirst     = SepTagArray ;

			if( SepTagArrayLength > 1 )
			{
				ALLOCMEMTAG *MemTag ;
				int i ;

				MemTag = SepTagArray ;
				MemTag->ListNext = MemTag + 1 ;
				MemTag ++ ;
				for( i = 1 ; i < SepTagArrayLength - 1 ; i ++, MemTag ++ )
				{
					MemTag->ListPrev = MemTag - 1 ;
					MemTag->ListNext = MemTag + 1 ;
				}
				MemTag->ListPrev = MemTag - 1 ;
			}
		}

		// ���g�p�q�[�v��������񃊃X�g�̍쐬
		{
			Heap->SepHeapMemArrayNotUseFirst = SepHeapMemArray ;

			if( SepHeapMemArrayLength > 1 )
			{
				HEAPMEMINFO *HeapMem ;
				int i ;

				HeapMem = SepHeapMemArray ;
				HeapMem->Next = HeapMem + 1 ;
				HeapMem ++ ;
				for( i = 1 ; i < SepHeapMemArrayLength - 1 ; i ++, HeapMem ++ )
				{
					HeapMem->Prev = HeapMem - 1 ;
					HeapMem->Next = HeapMem + 1 ;
				}
				HeapMem->Prev = HeapMem - 1 ;
			}
		}
	}

	// �q�[�v�������̃A�h���X���L���ȏꍇ�̓q�[�v�p��������ǉ�
	if( HeapAddress != NULL && HeapSize != 0 )
	{
		if( AddHeapMemory( Heap, HeapAddress, HeapSize ) < 0 )
		{
			DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )
			return -1 ;
		}
	}

	DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )

	// ����I��
	return 0 ;
}

// �q�[�v�̌�n�����s��
extern int TerminateHeap( HEAPINFO *Heap )
{
	HEAPMEMINFO *HeapMemInfo ;
	HEAPMEMINFO *NextHeapMemInfo ;

	DX_HEAP_ENTER_CRITICAL_SECTION( Heap )

	// �����m�ۂ��ꂽ�q�[�v���������������
	for( HeapMemInfo = Heap->FirstHeapMem ; HeapMemInfo != NULL ; HeapMemInfo = NextHeapMemInfo )
	{
		NextHeapMemInfo = HeapMemInfo->Next ;

		if( HeapMemInfo->Flag & HEAPMEM_FLAG_AUTOALLOC )
		{
			BYTE CallbackBuffer[ ALLOCHEAP_CALLBACK_INFO_SIZE ] ;
			_MEMCPY( CallbackBuffer, HeapMemInfo->CallbackBuffer, ALLOCHEAP_CALLBACK_INFO_SIZE ) ;

			Heap->AutoAlloc_DeleteHeapCallback( Heap->AutoAlloc_CallbackParam, CallbackBuffer ) ;
		}
	}

	DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )

	// �[��������
	_MEMSET( Heap, 0, sizeof( *Heap ) ) ;

	return 0 ;
}


// �q�[�v�Ƃ��Ĉ�����������ǉ�����
extern int AddHeapMemory( HEAPINFO *Heap, void *HeapAddress, ALLOCMEM_SIZE_TYPE HeapSize )
{
	BYTE				CallbackBuffer[ ALLOCHEAP_CALLBACK_INFO_SIZE ] ;
	HEAPMEMINFO *		HeapMemInfo ;
	DWORD_PTR			UseAddress ;
	ALLOCMEM_SIZE_TYPE	UseSize ;
	int					IsAutoAlloc = FALSE ;

	DX_HEAP_ENTER_CRITICAL_SECTION( Heap )

	if( HeapAddress == NULL && Heap->AutoAlloc )
	{
		if( HeapSize < Heap->AutoAllocUnitSize )
		{
			HeapSize = Heap->AutoAllocUnitSize ;
		}

		// �A���C��������
		HeapSize += ALLOCMEM_MIN_ALIGNED ;

		// �X�ɃA���C����K�p����
		HeapSize = ( HeapSize + ALLOCMEM_MIN_ALIGNED - 1 ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED ;

		// �����m�ۃ������̃T�C�Y���`�F�b�N
		if( Heap->UseSeparateInfo == FALSE && HeapSize < ( sizeof( HEAPMEMINFO ) + ALLOCMEM_MIN_ALIGNED - 1 ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED + ALLOCMEMTAG_MIN_SIZE )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd2\x30\xfc\x30\xd7\x30\x6e\x30\xea\x81\xd5\x52\xba\x78\xdd\x4f\xb5\x30\xa4\x30\xba\x30\x28\x00\x20\x00\x25\x00\x64\x00\x62\x00\x79\x00\x74\x00\x65\x00\x20\x00\x29\x00\x4c\x30\x0f\x5c\x55\x30\x59\x30\x4e\x30\x7e\x30\x59\x30\x01\x30\x00\x67\x4e\x4f\x67\x30\x82\x30\x20\x00\x25\x00\x64\x00\x62\x00\x79\x00\x74\x00\x65\x00\x20\x00\xc5\x5f\x81\x89\x67\x30\x59\x30\x00"/*@ L"�q�[�v�̎����m�ۃT�C�Y( %dbyte )�����������܂��A�Œ�ł� %dbyte �K�v�ł�" @*/, HeapSize, ( sizeof( HEAPMEMINFO ) + ALLOCMEM_MIN_ALIGNED - 1 ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED + ALLOCMEMTAG_MIN_SIZE )) ;
			DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )
			return -1 ;
		}

		// ���ˑ��q�[�v�̍쐬
		_MEMSET( CallbackBuffer, 0, ALLOCHEAP_CALLBACK_INFO_SIZE ) ;
		if( Heap->AutoAlloc_CreateHeapCallback( Heap->AutoAlloc_CallbackParam, CallbackBuffer, HeapSize ) < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd2\x30\xfc\x30\xd7\x30\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�q�[�v�p�������̊m�ۂɎ��s���܂���" @*/ )) ;
			DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )
			return -1 ;
		}

		// ���ۂɊm�ۂ����������̃T�C�Y���擾
		HeapSize = Heap->AutoAlloc_GetHeapSizeCallback( Heap->AutoAlloc_CallbackParam, CallbackBuffer ) ;

		// ���ˑ��q�[�v�̃������A�h���X���擾
		HeapAddress = Heap->AutoAlloc_GetHeapAddressCallback( Heap->AutoAlloc_CallbackParam, CallbackBuffer ) ;

		// �A���C�����l�����Ďg�p�ł���A�h���X���Z�o
		UseAddress = ( ( DWORD_PTR )HeapAddress + ALLOCMEM_MIN_ALIGNED - 1 ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED ;

		// �A���C�����l�����Ďg�p�ł���T�C�Y���Z�o
		UseSize    = ( ( ( DWORD_PTR )HeapAddress + HeapSize ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED ) - UseAddress ;

		// �����m�ۂ��ꂽ�������̃t���O�𗧂Ă�
		IsAutoAlloc = TRUE ;
	}
	else
	{
		if( HeapAddress == NULL || HeapSize == 0 )
		{
			DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )
			return -1 ;
		}

		// �A���C�����l�����Ďg�p�ł���A�h���X���Z�o
		UseAddress = ( ( DWORD_PTR )HeapAddress + ALLOCMEM_MIN_ALIGNED - 1 ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED ;

		// �A���C�����l�����Ďg�p�ł���T�C�Y���Z�o
		UseSize    = ( ( ( DWORD_PTR )HeapAddress + HeapSize ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED ) - UseAddress ;

		// �q�[�v�p�������̃T�C�Y���`�F�b�N
		if( Heap->UseSeparateInfo == FALSE && UseSize < ( sizeof( HEAPMEMINFO ) + ALLOCMEM_MIN_ALIGNED - 1 ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED + ALLOCMEMTAG_MIN_SIZE )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd2\x30\xfc\x30\xd7\x30\x6e\x30\xb5\x30\xa4\x30\xba\x30\x28\x00\x20\x00\x25\x00\x64\x00\x62\x00\x79\x00\x74\x00\x65\x00\x20\x00\x29\x00\x4c\x30\x0f\x5c\x55\x30\x59\x30\x4e\x30\x7e\x30\x59\x30\x01\x30\x00\x67\x4e\x4f\x67\x30\x82\x30\x20\x00\x25\x00\x64\x00\x62\x00\x79\x00\x74\x00\x65\x00\x20\x00\xc5\x5f\x81\x89\x67\x30\x59\x30\x00"/*@ L"�q�[�v�̃T�C�Y( %dbyte )�����������܂��A�Œ�ł� %dbyte �K�v�ł�" @*/, UseSize, ( sizeof( HEAPMEMINFO ) + ALLOCMEM_MIN_ALIGNED - 1 ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED + ALLOCMEMTAG_MIN_SIZE )) ;
			DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )
			return -1 ;
		}
	}

	if( Heap->UseSeparateInfo )
	{
		if( Heap->SepHeapMemArrayNotUseFirst == NULL )
		{
			DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )
			return -1 ;
		}

		HeapMemInfo = Heap->SepHeapMemArrayNotUseFirst ;
		Heap_SubNotUseSepHeapMem( Heap, HeapMemInfo ) ;
	}
	else
	{
		HeapMemInfo = ( HEAPMEMINFO * )UseAddress ;
	}

	// �[��������
	_MEMSET( HeapMemInfo, 0, sizeof( *HeapMemInfo ) ) ;

	if( IsAutoAlloc )
	{
		// �����m�ۂ��ꂽ���������ǂ�����ۑ�
		HeapMemInfo->Flag		|= HEAPMEM_FLAG_AUTOALLOC ;

		// �����m�ۂ̏ꍇ�̓R�[���o�b�N�֐��ɓn��������ۑ�
		_MEMCPY( HeapMemInfo->CallbackBuffer, CallbackBuffer, ALLOCHEAP_CALLBACK_INFO_SIZE ) ;
	}

	// �n���ꂽ or �m�ۂ����������̈�̃A�h���X�ƃT�C�Y��ۑ�
	HeapMemInfo->Address		= HeapAddress ;
	HeapMemInfo->Size			= HeapSize ;

	// �A���C�����l�����Ď��ۂɎg�p�ł��郁�����̈�̃A�h���X�ƃT�C�Y��ۑ�
	HeapMemInfo->UseAddress		= ( void * )UseAddress ;
	HeapMemInfo->UseSize		= UseSize ;

	// �q�[�v�Ƃ��Ď��ۂɎg�p�ł���A�h���X�ƃT�C�Y���Z�o
	{
		DWORD_PTR HeapUseAddress ;
		DWORD_PTR HeapUseSize ;

		if( Heap->UseSeparateInfo )
		{
			HeapUseAddress = UseAddress ;
		}
		else
		{
			HeapUseAddress = ( DWORD_PTR )( HeapMemInfo + 1 ) ;
		}

		// �A���C����K�p����
		HeapUseAddress = ( HeapUseAddress + ALLOCMEM_MIN_ALIGNED - 1 ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED ;

		HeapUseSize = ( ( DWORD_PTR )HeapMemInfo->Address + HeapMemInfo->Size ) - HeapUseAddress ;

		// �A���C����K�p����
		HeapUseSize = HeapUseSize / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED ;

		HeapMemInfo->HeapAddress	= ( void * )HeapUseAddress ;
		HeapMemInfo->HeapSize		= HeapUseSize ;
	}

	// �q�[�v�ň����������̃��X�g�ɒǉ�
	HeapMemInfo->Next  = Heap->FirstHeapMem ;
	Heap->FirstHeapMem = HeapMemInfo ;
	if( HeapMemInfo->Next != NULL )
	{
		HeapMemInfo->Next->Prev = HeapMemInfo ;
	}

	// �󂫃������^�O��ǉ�
	{
		ALLOCMEMTAG *FreeMemTag ;

		if( Heap->UseSeparateInfo )
		{
			if( Heap->SepTagArrayNotUseFirst == NULL )
			{
				DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )
				return -1 ;
			}

			FreeMemTag = Heap->SepTagArrayNotUseFirst ;
			Heap_SubNotUseSepMemTag( Heap, FreeMemTag ) ;
		}
		else
		{
			FreeMemTag = ( ALLOCMEMTAG * )HeapMemInfo->HeapAddress ;
		}

		// ��{�����Z�b�g
		FreeMemTag->Flag		= 0 ;
		AllocMemTag_SetBaseInfo( Heap, FreeMemTag, FALSE, TRUE, "Free", 0 ) ;

		// UseSeparateInfo = TRUE �̏ꍇ�͊m�ۃ������̃A�h���X���Z�b�g
		if( Heap->UseSeparateInfo )
		{
			FreeMemTag->AllocAddress = HeapMemInfo->HeapAddress ;
		}

		// �T�C�Y���Z�b�g
		FreeMemTag->Size		= HeapMemInfo->HeapSize ;

		// �g�p���Ă���q�[�v���Z�b�g
		FreeMemTag->UseHeapMem	= HeapMemInfo ;

		// �q�[�v���������̐擪�������^�O�ɓo�^
		HeapMemInfo->FirstMemTag = FreeMemTag ;

		// �f�o�b�O�̈�ɏ����Z�b�g
		if( Heap->UseSeparateInfo == FALSE )
		{
			AllocMemTag_SetDebugData( FreeMemTag ) ;
		}

		// �������^�O�̃��X�g�ɒǉ�
		FreeMemTag->Next = Heap->FirstTag ;
		FreeMemTag->Prev = NULL ;
		Heap->FirstTag = FreeMemTag ;
		if( FreeMemTag->Next != NULL )
		{
			FreeMemTag->Next->Prev = FreeMemTag ;
		}
		else
		{
			Heap->LastTag          = FreeMemTag ;
		}

		// �󂫃������^�O�̐���ǉ�
		Heap->FreeTagNum		+= 1 ;

		// �󂫃������̑��T�C�Y��ǉ�
		Heap->TotalFreeSize		+= FreeMemTag->Size ;

		// �󂫃������^�O���X�g�ɒǉ�
		if( Heap->AllocType == ALLOCMEMTYPE_SIMPLE )
		{
			Heap_AddFreeMemTag_Simple( Heap, FreeMemTag, NULL ) ;
		}
		else
		if( Heap->AllocType == ALLOCMEMTYPE_TLSF )
		{
			Heap_AddFreeMemTag_TLSF( Heap, FreeMemTag ) ;
		}
	}

	DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )

	return 0 ;
}

// �q�[�v�Ƃ��Ĉ��������������炷
extern int SubHeapMemory( HEAPINFO *Heap, void *HeapAddress )
{
	HEAPMEMINFO *	HeapMemInfo ;
	ALLOCMEMTAG *	MemTag ;

	DX_HEAP_ENTER_CRITICAL_SECTION( Heap )

	if( Heap->UseSeparateInfo )
	{
		for( HeapMemInfo = Heap->FirstHeapMem ; HeapMemInfo != NULL && HeapMemInfo->Address != HeapAddress ; HeapMemInfo = HeapMemInfo->Next ){}
		if( HeapMemInfo == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x53\x00\x75\x00\x62\x00\x48\x00\x65\x00\x61\x00\x70\x00\x4d\x00\x65\x00\x6d\x00\x6f\x00\x72\x00\x79\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x07\x63\x9a\x5b\x55\x30\x8c\x30\x5f\x30\xe1\x30\xe2\x30\xea\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x92\x30\x01\x63\x64\x30\xd2\x30\xfc\x30\xd7\x30\xe1\x30\xe2\x30\xea\x30\x4c\x30\x8b\x89\x64\x30\x4b\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x41\x00\x64\x00\x64\x00\x72\x00\x65\x00\x73\x00\x73\x00\x3a\x00\x30\x00\x78\x00\x25\x00\x6c\x00\x6c\x00\x30\x00\x31\x00\x36\x00\x78\x00\x00"/*@ L"SubHeapMemory Error : �w�肳�ꂽ�������A�h���X�����q�[�v��������������܂���  HeapAddress:0x%ll016x" @*/, ( ULONGLONG )HeapAddress )) ;
			*( ( DWORD * )0x00000000 ) = 0xffffffff ;
		}
	}
	else
	{
		HeapMemInfo = ( HEAPMEMINFO * )( ( ( DWORD_PTR )HeapAddress + ALLOCMEM_MIN_ALIGNED - 1 ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED ) ;
	}

	// �w�肳�ꂽ�q�[�v���g�p���Ă��郁�����^�O������ꍇ�͉���ł��Ȃ�
	if( HeapMemInfo->UseMemTagCount != 0 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x53\x00\x75\x00\x62\x00\x48\x00\x65\x00\x61\x00\x70\x00\x4d\x00\x65\x00\x6d\x00\x6f\x00\x72\x00\x79\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x7f\x4f\x28\x75\x57\x30\x66\x30\x44\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x4c\x30\x42\x30\x8b\x30\xb6\x72\x4b\x61\x67\x30\xd2\x30\xfc\x30\xd7\x30\xe1\x30\xe2\x30\xea\x30\x92\x30\x16\x59\x5d\x30\x46\x30\x68\x30\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x4d\x00\x65\x00\x6d\x00\x49\x00\x6e\x00\x66\x00\x6f\x00\x2d\x00\x3e\x00\x55\x00\x73\x00\x65\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x43\x00\x6f\x00\x75\x00\x6e\x00\x74\x00\x3a\x00\x25\x00\x64\x00\x00"/*@ L"SubHeapMemory Error : �g�p���Ă��郁�����^�O�������ԂŃq�[�v���������O�����Ƃ��܂���  HeapMemInfo->UseMemTagCount:%d" @*/, HeapMemInfo->UseMemTagCount )) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// �q�[�v���������g�p���Ă���^�O���󂫃������^�O��ł͂Ȃ�������G���[
	MemTag = ( ALLOCMEMTAG * )HeapMemInfo->FirstMemTag ;
	if( MemTag->Flag & ALLOCMEMTAG_FLAG_USE )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x53\x00\x75\x00\x62\x00\x48\x00\x65\x00\x61\x00\x70\x00\x4d\x00\x65\x00\x6d\x00\x6f\x00\x72\x00\x79\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd2\x30\xfc\x30\xd7\x30\xe1\x30\xe2\x30\xea\x30\x92\x30\x7f\x4f\x28\x75\x57\x30\x66\x30\x44\x30\x8b\x30\x48\x51\x2d\x98\xbf\x30\xb0\x30\x4c\x30\x7f\x4f\x28\x75\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x67\x30\x57\x30\x5f\x30\x00"/*@ L"SubHeapMemory Error : �q�[�v���������g�p���Ă���擪�^�O���g�p�������^�O�ł���" @*/ )) ;
		PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}
	if( MemTag->UseHeapMem != HeapMemInfo )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x53\x00\x75\x00\x62\x00\x48\x00\x65\x00\x61\x00\x70\x00\x4d\x00\x65\x00\x6d\x00\x6f\x00\x72\x00\x79\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd2\x30\xfc\x30\xd7\x30\xe1\x30\xe2\x30\xea\x30\x92\x30\x7f\x4f\x28\x75\x57\x30\x66\x30\x44\x30\x8b\x30\x48\x51\x2d\x98\xbf\x30\xb0\x30\x4c\x30\x7f\x4f\x28\x75\x57\x30\x66\x30\x44\x30\x8b\x30\xd2\x30\xfc\x30\xd7\x30\xe1\x30\xe2\x30\xea\x30\xc5\x60\x31\x58\x68\x30\x00\x4e\xf4\x81\x57\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x55\x00\x73\x00\x65\x00\x48\x00\x65\x00\x61\x00\x70\x00\x4d\x00\x65\x00\x6d\x00\x3a\x00\x30\x00\x78\x00\x25\x00\x6c\x00\x6c\x00\x30\x00\x31\x00\x36\x00\x78\x00\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x4d\x00\x65\x00\x6d\x00\x49\x00\x6e\x00\x66\x00\x6f\x00\x3a\x00\x30\x00\x78\x00\x25\x00\x6c\x00\x6c\x00\x30\x00\x31\x00\x36\x00\x78\x00\x00"/*@ L"SubHeapMemory Error : �q�[�v���������g�p���Ă���擪�^�O���g�p���Ă���q�[�v���������ƈ�v���܂��� MemTag->UseHeapMem:0x%ll016x  HeapMemInfo:0x%ll016x" @*/, ( ULONGLONG )MemTag->UseHeapMem, ( ULONGLONG )HeapMemInfo )) ;
		PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}
	if( MemTag->Next != NULL && MemTag->Next->UseHeapMem == HeapMemInfo )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x53\x00\x75\x00\x62\x00\x48\x00\x65\x00\x61\x00\x70\x00\x4d\x00\x65\x00\x6d\x00\x6f\x00\x72\x00\x79\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd2\x30\xfc\x30\xd7\x30\xe1\x30\xe2\x30\xea\x30\x92\x30\x7f\x4f\x28\x75\x57\x30\x66\x30\x44\x30\x8b\x30\x7f\x4f\x28\x75\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x4c\x30\x8c\x4e\x64\x30\xe5\x4e\x0a\x4e\x42\x30\x8a\x30\x7e\x30\x59\x30\x00"/*@ L"SubHeapMemory Error : �q�[�v���������g�p���Ă���g�p�������^�O����ȏ゠��܂�" @*/ )) ;
		PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// �󂫃������̐������炷
	Heap->TotalFreeSize -= MemTag->Size ;

	// �󂫃������^�O�̐������炷
	Heap->FreeTagNum -- ;

	// �������^�O�̘A����ύX����
	{
		if( MemTag->Prev != NULL )
		{
			MemTag->Prev->Next = MemTag->Next ;
		}
		else
		{
			Heap->FirstTag = MemTag->Next ;
		}

		if( MemTag->Next != NULL )
		{
			MemTag->Next->Prev = MemTag->Prev ;
		}
		else
		{
			Heap->LastTag = MemTag->Prev ;
		}
	}

	// �󂫃��������X�g����O��
	if( Heap->AllocType == ALLOCMEMTYPE_SIMPLE )
	{
		Heap_SubFreeMemTag_Simple( Heap, MemTag ) ;
	}
	else
	if( Heap->AllocType == ALLOCMEMTYPE_TLSF )
	{
		Heap_SubFreeMemTag_TLSF( Heap, MemTag ) ;
	}

	// ���X�g�̘A������O��
	{
		if( HeapMemInfo->Prev == NULL )
		{
			Heap->FirstHeapMem = HeapMemInfo->Next ;
		}
		else
		{
			HeapMemInfo->Prev->Next = HeapMemInfo->Next ;
		}

		if( HeapMemInfo->Next != NULL )
		{
			HeapMemInfo->Next->Prev = HeapMemInfo->Prev ;
		}
	}

	// ���g�p���X�g�ɒǉ�����
	if( Heap->UseSeparateInfo )
	{
		Heap_AddNotUseSepHeapMem( Heap, HeapMemInfo ) ;
	}

	// �����m�ۂ��ꂽ�������������ꍇ�͉������
	if( HeapMemInfo->Flag & HEAPMEM_FLAG_AUTOALLOC )
	{
		BYTE CallbackBuffer[ ALLOCHEAP_CALLBACK_INFO_SIZE ] ;
		_MEMCPY( CallbackBuffer, HeapMemInfo->CallbackBuffer, ALLOCHEAP_CALLBACK_INFO_SIZE ) ;

		Heap->AutoAlloc_DeleteHeapCallback( Heap->AutoAlloc_CallbackParam, CallbackBuffer ) ;
	}

	DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )

	// ����I��
	return 0 ;
}

// �q�[�v���g�p���Ă��郁�����̑��T�C�Y���擾����
extern	ALLOCMEM_SIZE_TYPE	GetHeapTotalMemorySize( HEAPINFO *Heap )
{
	return Heap->TotalUseSize + Heap->TotalFreeSize ;
}

// �q�[�v���烁�������m�ۂ���( UseSeparateInfo=TRUE �̏ꍇ�́A�߂��Ă���̂� ALLOCMEMTAG �\���̂̃A�h���X )
extern void * AllocMemory(
	HEAPINFO *			Heap,
	ALLOCMEM_SIZE_TYPE	Size,
	ALLOCMEM_SIZE_TYPE	Aligned,
	int					Reverse,
	const char *		Name,
	int					Line
)
{
	void *	ReturnAddress = NULL ;
	ALLOCMEM_SIZE_TYPE	OrigSize = Size ;

	DX_HEAP_ENTER_CRITICAL_SECTION( Heap )

	// �T�C�Y���O�̏ꍇ�� NULL ��Ԃ�
	if( Size == 0 )
	{
		DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )
		return NULL ;
	}

	// �m�ۃT�C�Y���A���C���̔{���ɂ���
	if( Aligned == 0 )
	{
		Aligned = Heap->Aligned ;
	}
	else
	{
		if( Aligned < ALLOCMEM_MIN_ALIGNED )
		{
			Aligned = ALLOCMEM_MIN_ALIGNED ;
		}
	}
	Size = ( Size + Aligned - 1 ) / Aligned * Aligned ;

	// �������m�ۃ^�C�v�ɂ���ď����𕪊�
	if( Heap->AllocType == ALLOCMEMTYPE_SIMPLE )
	{
		ALLOCMEMTAG *		MemTag ;
		DWORD_PTR			UseAddress = 0 ;

		if( Reverse )
		{
			for( MemTag = Heap->FreeLastTag ; MemTag != NULL ; MemTag = MemTag->ListPrev )
			{
				// �A���C�����l�����Ă��w��̃������T�C�Y���m�ۂł��邩�`�F�b�N
				if( Heap->UseSeparateInfo )
				{
					// �T�C�Y���ԈႢ�Ȃ�����Ȃ���Ύ���
					if( MemTag->Size < Size )
					{
						continue ;
					}

					// �Ԃ��A�h���X�̎Z�o
					{
						UseAddress = ( ( DWORD_PTR )MemTag->AllocAddress + MemTag->Size ) - Size ;

						// �A���C����K�p
						UseAddress = UseAddress / Aligned * Aligned ;
					}

					// �L���ȃA�h���X�ɂȂ��Ă����烋�[�v�𔲂���
					if( UseAddress >= ( DWORD_PTR )MemTag->AllocAddress )
					{
						break ;
					}
				}
				else
				{
					// �T�C�Y���ԈႢ�Ȃ�����Ȃ���Ύ���
					if( MemTag->Size - ALLOCMEMTAG_TOTAL_SIZE < Size )
					{
						continue ;
					}

					// �Ԃ��A�h���X�̎Z�o
					{
						UseAddress = ( ( DWORD_PTR )MemTag + MemTag->Size - ALLOCMEMTAG_DEBUGAREA_SIZE ) - Size ;

						// �A���C����K�p
						UseAddress = UseAddress / Aligned * Aligned ;
					}

					// �L���ȃA�h���X�ɂȂ��Ă����烋�[�v�𔲂���
					if( UseAddress >= ( DWORD_PTR )MemTag + ALLOCMEMTAG_TOTAL_SIZE - ALLOCMEMTAG_DEBUGAREA_SIZE )
					{
						break ;
					}
				}
			}

			// �󂫃����������������ꍇ�͏����𕪊�
			if( MemTag == NULL )
			{
				// �q�[�v��������ǉ����ă������̊m�ۂ����݂�
				ReturnAddress = AllocMemory_AutoAllocHeap( Heap, Size, Aligned, Reverse, Name, Line ) ;
			}
			else
			{
				// �󂫃������^�O�ɑ΂��ă������̊m�ۏ������s��
				ReturnAddress = Heap_AllocMemory( Heap, MemTag, UseAddress, OrigSize, Size, Aligned, Reverse, Name, Line ) ;
			}
		}
		else
		{
			for( MemTag = Heap->FreeFirstTag ; MemTag != NULL ; MemTag = MemTag->ListNext )
			{
				ALLOCMEM_SIZE_TYPE	FreeSize ;

				// �w��̃������T�C�Y���m�ۂł��邩�`�F�b�N
				if( Heap->UseSeparateInfo )
				{
					// �Ԃ��A�h���X�̎Z�o
					{
						UseAddress = ( DWORD_PTR )MemTag->AllocAddress ;

						// �A���C����K�p
						UseAddress = ( UseAddress + Aligned - 1 ) / Aligned * Aligned ;
					}

					// �L���ȋ󂫃������T�C�Y���Z�o
					{
						FreeSize = ( ( DWORD_PTR )MemTag->AllocAddress + MemTag->Size ) - UseAddress ;

						// �A���C����K�p
						FreeSize = FreeSize / Aligned * Aligned ;
					}
				}
				else
				{
					// �Ԃ��A�h���X�̎Z�o
					{
						UseAddress = ( DWORD_PTR )MemTag + ALLOCMEMTAG_TOTAL_SIZE - ALLOCMEMTAG_DEBUGAREA_SIZE ;

						// �A���C����K�p
						UseAddress = ( UseAddress + Aligned - 1 ) / Aligned * Aligned ;
					}

					// �L���ȋ󂫃������T�C�Y���Z�o
					{
						FreeSize = ( ( DWORD_PTR )MemTag + MemTag->Size ) - UseAddress - ALLOCMEMTAG_DEBUGAREA_SIZE ;

						// �A���C����K�p
						FreeSize = FreeSize / Aligned * Aligned ;
					}
				}

				// �󂫃������������ꍇ�̓��[�v�𔲂���
				if( FreeSize >= Size )
				{
					break ;
				}
			}

			// �󂫃����������������ꍇ�͏����𕪊�
			if( MemTag == NULL )
			{
				// �q�[�v��������ǉ����ă������̊m�ۂ����݂�
				ReturnAddress = AllocMemory_AutoAllocHeap( Heap, Size, Aligned, Reverse, Name, Line ) ;
			}
			else
			{
				// �󂫃������^�O�ɑ΂��ă������̊m�ۏ������s��
				ReturnAddress = Heap_AllocMemory( Heap, MemTag, UseAddress, OrigSize, Size, Aligned, Reverse, Name, Line ) ;
			}
		}
	}
	else
	if( Heap->AllocType == ALLOCMEMTYPE_TLSF )
	{
		DWORD				FLI ;
		DWORD				SLI ;
		ALLOCMEM_SIZE_TYPE	UseSize ;
		DWORD_PTR			UseAddress ;
		ALLOCMEMTAG *		MemTag = NULL ;
//		int					Root = 0 ;
		DWORD				EnableBitList2 ;
		ALLOCMEM_SIZE_TYPE	EnableBitList1 ;

		// �K�v�ȃ������̈���Z�o
		if( Heap->UseSeparateInfo )
		{
			UseSize = Size ;
		}
		else
		{
			// �^�O���܂߂ĕK�v�ȃ������̈���Z�o
			UseSize = Size + ALLOCMEMTAG_MIN_SIZE ;
		}

		// �A���C���K�p
		UseSize = ( UseSize + Aligned * 2 - 1 ) / Aligned * Aligned ;

		// �K�v�T�C�Y�̃J�e�S���P�ƃJ�e�S���Q���擾
		FLI = GetMSB( UseSize ) ;
		SLI = GetSLI( UseSize, FLI, ALLOCMEM_CATEGORY_2_SHIFT_NUM ) ;

		// �K�v�T�C�Y���K�����܂�K�v������̂ŁA�J�e�S���Q�̃C���f�b�N�X�͂ЂƂ傫���Ȃ�
		SLI += 1 ;
		if( SLI >= ( 1 << ALLOCMEM_CATEGORY_2_SHIFT_NUM ) )
		{
			SLI -= ( 1 << ALLOCMEM_CATEGORY_2_SHIFT_NUM ) ;
			FLI ++ ;
		}

		// ��v����J�e�S���ɋ󂫃������^�O�����邩�`�F�b�N
		if( Heap->FreeFirstTagArray[ FLI ][ SLI ] != NULL )
		{
			// �������炻�̃������^�O���g�p����
			MemTag = Heap->FreeFirstTagArray[ FLI ][ SLI ] ;

//			Root = 1 ;
		}
		else
		{
			// ���������ꍇ�͕K�v�ȃT�C�Y�����܂�ŏ��̃������^�O��T��

			// �܂������J�e�S���P���ɂ��傫���T�C�Y�̃J�e�S���Q�̋󂫃������^�O�����邩���ׂ�
			EnableBitList2 = Heap->FreeTagBitList2[ FLI ] & ( 0xffffffff << SLI ) ;

			// �������炠�������ōŏ��̃J�e�S���Q�̃������^�O���g�p����
			if( EnableBitList2 != 0 )
			{
				SLI = GetLSB( EnableBitList2 ) ;
				MemTag = Heap->FreeFirstTagArray[ FLI ][ SLI ] ;

//				Root = 2 ;
			}
			else
			{
				// ������������傫���T�C�Y�̃J�e�S���P�ɋ󂫃������^�O�����邩���ׂ�

				FLI ++ ;

			#ifdef __64BIT__
				#if defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
					EnableBitList1 = Heap->FreeTagBitList1 & ( 0xffffffffffffffffULL << FLI ) ;
				#else // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
					EnableBitList1 = Heap->FreeTagBitList1 & ( 0xffffffffffffffff    << FLI ) ;
				#endif // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
			#else // __64BIT__
				EnableBitList1 = Heap->FreeTagBitList1 & ( 0xffffffff << FLI ) ;
			#endif // __64BIT__

				// ����������w��̃T�C�Y�����܂�󂫃������������Ƃ�������
				if( EnableBitList1 == 0 )
				{
					// �q�[�v��������ǉ����ă������̊m�ۂ����݂�
					ReturnAddress = AllocMemory_AutoAllocHeap( Heap, Size, Aligned, Reverse, Name, Line ) ;

					DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )
					return ReturnAddress ;
				}

				// �������炠�������ōŏ��̃J�e�S���P���擾
				FLI = GetLSB( EnableBitList1 ) ;

				// �X�ɍŏ��̃J�e�S���P�̒��̍ŏ��̃J�e�S���Q�̃������^�O���g�p
				SLI = GetLSB( Heap->FreeTagBitList2[ FLI ] ) ;
				MemTag = Heap->FreeFirstTagArray[ FLI ][ SLI ] ;

//				Root = 3 ;
			}
		}

		// �Ԃ��A�h���X�̎Z�o
		if( Heap->UseSeparateInfo )
		{
			UseAddress = ( DWORD_PTR )MemTag->AllocAddress ;

			// �A���C����K�p
			UseAddress = ( UseAddress + Aligned - 1 ) / Aligned * Aligned ;

			// �T�C�Y�̃`�F�b�N
			{
				ALLOCMEM_SIZE_TYPE FreeSize = ( ( DWORD_PTR )MemTag->AllocAddress + MemTag->Size ) - UseAddress ;
				if( FreeSize < Size )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x54\x00\x4c\x00\x53\x00\x46\x00\x20\x00\xab\x30\xc6\x30\xb4\x30\xea\x30\xcb\x69\x20\x90\xa8\x30\xe9\x30\xfc\x30\x20\x00\x31\x00\x20\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x20\x00\x25\x00\x64\x00\x00"/*@ L"TLSF �J�e�S���\���G���[ 1  MemTag->Size %d" @*/, MemTag->Size )) ;
					*( ( DWORD * )0x00000000 ) = 0xffffffff ;
				}
			}
		}
		else
		{
			UseAddress = ( DWORD_PTR )MemTag + ALLOCMEMTAG_TOTAL_SIZE - ALLOCMEMTAG_DEBUGAREA_SIZE ;

			// �A���C����K�p
			UseAddress = ( UseAddress + Aligned - 1 ) / Aligned * Aligned ;

			// �T�C�Y�̃`�F�b�N
			{
				ALLOCMEM_SIZE_TYPE FreeSize = ( ( DWORD_PTR )MemTag + MemTag->Size ) - UseAddress ;
				if( FreeSize < Size + ALLOCMEMTAG_DEBUGAREA_SIZE )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x54\x00\x4c\x00\x53\x00\x46\x00\x20\x00\xab\x30\xc6\x30\xb4\x30\xea\x30\xcb\x69\x20\x90\xa8\x30\xe9\x30\xfc\x30\x20\x00\x32\x00\x20\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x20\x00\x25\x00\x64\x00\x00"/*@ L"TLSF �J�e�S���\���G���[ 2  MemTag->Size %d" @*/, MemTag->Size )) ;
					*( ( DWORD * )0x00000000 ) = 0xffffffff ;
				}
			}
		}

		// �󂫃������^�O�ɑ΂��ă������̊m�ۏ������s��
		ReturnAddress = Heap_AllocMemory( Heap, MemTag, UseAddress, OrigSize, Size, Aligned, Reverse, Name, Line ) ;
	}

	DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )

	return ReturnAddress ;
}

// �m�ۂ����������̃T�C�Y��ύX����( UseSeparateInfo=TRUE �̏ꍇ�́AAllocAddress �� ALLOCMEMTAG �\���̂̃A�h���X��n�� )
extern void * ReallocMemory( void *AllocAddress, int UseSeparateInfo, ALLOCMEM_SIZE_TYPE Size, ALLOCMEM_SIZE_TYPE Aligned, const char *Name, int Line )
{
	ALLOCMEMTAG *		MemTag ;
	HEAPINFO *			Heap ;
	ALLOCMEM_SIZE_TYPE	OrigSize = Size ;
	void *				ReturnAddress = NULL ;

	if( AllocAddress == NULL )
	{
		return 0 ;
	}

	// �������^�O�̃A�h���X���Z�o
	if( UseSeparateInfo )
	{
		MemTag = ( ALLOCMEMTAG * )AllocAddress ;
	}
	else
	{
		MemTag = ( ALLOCMEMTAG * )( ( BYTE * )AllocAddress - *( ( DWORD * )( ( BYTE * )AllocAddress - sizeof( DWORD ) ) ) ) ;
	}

	// �}�W�b�N�h�c�̊m�F
	if( MemTag->MagicID != ALLOCMEMTAG_MAGIC_ID )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x6c\x00\x6c\x00\x6f\x00\x63\x00\x4d\x00\x65\x00\x6d\x00\x6f\x00\x72\x00\x79\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\x20\x00\x4d\x00\x61\x00\x67\x00\x69\x00\x63\x00\x49\x00\x44\x00\x20\x00\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x01\x30\xa4\x8a\x63\x30\x5f\x30\xe1\x30\xe2\x30\xea\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x4c\x30\x21\x6e\x55\x30\x8c\x30\x5f\x30\x4b\x30\x01\x30\xe1\x30\xe2\x30\xea\x30\x4c\x30\x34\x78\xca\x58\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x00"/*@ L"ReallocMemory Error : �������^�O�� MagicID ���s���ł��A������������A�h���X���n���ꂽ���A���������j�󂳂�Ă��܂�" @*/ )) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// �q�[�v�̃A�h���X���Z�b�g
	Heap = MemTag->Owner ;

	DX_HEAP_ENTER_CRITICAL_SECTION( Heap )

	// �T�C�Y���O�̏ꍇ�� NULL ��Ԃ�
	if( Size == 0 )
	{
		DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )
		return NULL ;
	}

	// �m�ۃT�C�Y���A���C���̔{���ɂ���
	if( Aligned == 0 )
	{
		Aligned = Heap->Aligned ;
	}
	else
	{
		if( Aligned < ALLOCMEM_MIN_ALIGNED )
		{
			Aligned = ALLOCMEM_MIN_ALIGNED ;
		}
	}
	Size = ( Size + Aligned - 1 ) / Aligned * Aligned ;

	// �m�ێ��Ǝw�肳�ꂽ�T�C�Y�������ꍇ�̓t�@�C�����ƍs�ԍ������X�V����
	if( MemTag->UserSize == OrigSize )
	{
		AllocMemTag_SetBaseInfo( Heap, MemTag, TRUE, TRUE, Name, Line ) ;

		DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )
		return AllocAddress ;
	}

	// �f�o�b�O�̈�̃`�F�b�N
	if( Heap->UseSeparateInfo == FALSE )
	{
		if( AllocMemTag_CheckDebugData( MemTag ) < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x6c\x00\x6c\x00\x6f\x00\x63\x00\x4d\x00\x65\x00\x6d\x00\x6f\x00\x72\x00\x79\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xc7\x30\xd0\x30\xc3\x30\xb0\x30\x18\x98\xdf\x57\x6e\x30\x34\x78\xca\x58\x92\x30\xba\x78\x8d\x8a\x57\x30\x7e\x30\x57\x30\x5f\x30\x01\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\x0d\x4e\x63\x6b\x6a\x30\xa2\x30\xaf\x30\xbb\x30\xb9\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x5f\x30\x53\x30\x68\x30\x4c\x30\x03\x80\x48\x30\x89\x30\x8c\x30\x7e\x30\x59\x30\x00"/*@ L"ReallocMemory Error : �f�o�b�O�̈�̔j����m�F���܂����A�������̕s���ȃA�N�Z�X�������������Ƃ��l�����܂�" @*/ )) ;
			PrintInfoMemory( AllocAddress, UseSeparateInfo ) ;
			*( ( DWORD * )0x00000000 ) = 0xffffffff ;
		}
	}

	// �g�p���`�F�b�N
	if( ( MemTag->Flag & ALLOCMEMTAG_FLAG_USE ) == 0 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x52\x00\x65\x00\x61\x00\x6c\x00\x6c\x00\x6f\x00\x63\x00\x4d\x00\x65\x00\x6d\x00\x6f\x00\x72\x00\x79\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe3\x89\x3e\x65\x08\x6e\x7f\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6b\x30\xfe\x5b\x57\x30\x66\x30\x8d\x51\xba\x78\xdd\x4f\x92\x30\x57\x30\x88\x30\x46\x30\x68\x30\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"ReallocMemory Error : ����ς݂̃������ɑ΂��čĊm�ۂ����悤�Ƃ��܂���" @*/ )) ;
		PrintInfoMemory( AllocAddress, UseSeparateInfo ) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// �m�ۃ�������������̂�����̂��ŏ����𕪊�
	{
		ALLOCMEM_SIZE_TYPE	FreeSize ;
		DWORD_PTR			UseAddress ;
		int					IsRealloc = FALSE ;

		// �g�p���Ă��郁�����A�h���X���Z�b�g
		UseAddress = ( DWORD_PTR )MemTag->UserAddress ;

		// �L���ȋ󂫃������T�C�Y���Z�o
		if( Heap->UseSeparateInfo )
		{
			FreeSize = ( ( DWORD_PTR )MemTag->AllocAddress + MemTag->Size ) - UseAddress ;
		}
		else
		{
			FreeSize = ( ( DWORD_PTR )MemTag + MemTag->Size ) - UseAddress - ALLOCMEMTAG_DEBUGAREA_SIZE ;
		}

		// �A���C����K�p
		FreeSize = FreeSize / Aligned * Aligned ;

		// ���݂̊m�ۃ������Ɏ��܂�Ȃ��ꍇ�͂���ɍĊm�ۂ��K�v���`�F�b�N
		if( Size > FreeSize )
		{
			// ���܂�Ȃ��ꍇ�������A�h���X�̍������ׂ̗̃������^�O���󂫃������ŁA�����₵�����̃��������i�[�ł��邩���`�F�b�N

			// ���̃������^�O�ƃ������A�h���X���A�����Ă��Ȃ����A���̃������^�O���������A���̃������^�O���󂫃������ł͂Ȃ��ꍇ�͊g���s�\�@
			if( MemTag->Next == NULL || 
				( MemTag->Flag & ALLOCMEMTAG_FLAG_NEXT_CONNECT ) == 0 ||
				( MemTag->Next->Flag & ALLOCMEMTAG_FLAG_USE ) != 0 )
			{
				// �Ċm�ۂ̃t���O�𗧂Ă�
				IsRealloc = TRUE ;
			}
			else
			{
				// �󂫃������̃T�C�Y�𑫂����ꍇ�A�g�������T�C�Y�Ɏ��܂邩���`�F�b�N

				// �L���ȋ󂫃������T�C�Y���Z�o
				if( Heap->UseSeparateInfo )
				{
					FreeSize = ( ( DWORD_PTR )MemTag->AllocAddress + MemTag->Size + MemTag->Next->Size ) - UseAddress ;
				}
				else
				{
					FreeSize = ( ( DWORD_PTR )MemTag + MemTag->Size + MemTag->Next->Size ) - UseAddress - ALLOCMEMTAG_DEBUGAREA_SIZE ;
				}

				// �A���C����K�p
				FreeSize = FreeSize / Aligned * Aligned ;

				// �󂫃�����������Ȃ��ꍇ�͍Ċm�ۂ̃t���O�𗧂Ă�
				if( Size > FreeSize )
				{
					IsRealloc = TRUE ;
				}
			}
		}

		// �Ċm�ۂ�����ꍇ�Ɗg������ꍇ�ŏ����𕪊�
		if( IsRealloc )
		{
			// �Ċm�ۂ���ꍇ

			// �V�����������̈���m��
			ReturnAddress = AllocMemory(
				Heap,
				OrigSize,
				Aligned,
				FALSE,
				Name,
				Line
			) ;

			// �������m�ۂɎ��s�����ꍇ�� NULL ��Ԃ�
			if( ReturnAddress == NULL )
			{
				DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )
				return NULL ;
			}

			// ���������ꍇ�͍��܂ł̊m�ۃ������̓��e���R�s�[����
			if( Heap->UseSeparateInfo )
			{
				_MEMCPY( ( ( ALLOCMEMTAG * )ReturnAddress )->AllocAddress, MemTag->UserAddress, ( ( DWORD_PTR )MemTag->AllocAddress + MemTag->Size ) - UseAddress ) ;
			}
			else
			{
				_MEMCPY( ReturnAddress, MemTag->UserAddress, ( ( DWORD_PTR )MemTag + MemTag->Size ) - UseAddress - ALLOCMEMTAG_DEBUGAREA_SIZE ) ;
			}

			// ���܂ł̃������͉������
			FreeMemory( AllocAddress, Heap->UseSeparateInfo ) ;
		}
		else
		{
			// �g������ꍇ�́A��U�g�����郁�����^�O���������( �󂫃������ƘA������ꍇ�̓������A�h���X�������ꍇ�݂̂ɂ���t���O�� TRUE �ɂ��� )
			Heap_FreeMemory( Heap, MemTag, TRUE ) ;

			// �������̍Ċm��
			Heap_AllocMemory(
				Heap,
				MemTag,
				UseAddress,
				OrigSize,
				Size,
				Aligned,
				FALSE,
				Name,
				Line
			) ;

			// �������A�h���X�͂��̂܂�
			ReturnAddress = AllocAddress ;
		}
	}

	DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )

	return ReturnAddress ;
}

// �q�[�v����m�ۂ������������J������( UseSeparateInfo = TRUE �̏ꍇ�́AAllocAddress �� ALLOCMEMTAG �\���̂̃A�h���X��n�� )
extern int FreeMemory( void *AllocAddress, int UseSeparateInfo )
{
	ALLOCMEMTAG *	MemTag ;
	HEAPINFO *		Heap ;

	if( AllocAddress == NULL )
	{
		return 0 ;
	}

	// �������^�O�̃A�h���X���Z�o
	if( UseSeparateInfo )
	{
		MemTag = ( ALLOCMEMTAG * )AllocAddress ;
	}
	else
	{
		MemTag = ( ALLOCMEMTAG * )( ( BYTE * )AllocAddress - *( ( DWORD * )( ( BYTE * )AllocAddress - sizeof( DWORD ) ) ) ) ;
	}

	// �}�W�b�N�h�c�̊m�F
	if( MemTag->MagicID != ALLOCMEMTAG_MAGIC_ID )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x72\x00\x65\x00\x65\x00\x4d\x00\x65\x00\x6d\x00\x6f\x00\x72\x00\x79\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\x20\x00\x4d\x00\x61\x00\x67\x00\x69\x00\x63\x00\x49\x00\x44\x00\x20\x00\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x01\x30\xa4\x8a\x63\x30\x5f\x30\xe1\x30\xe2\x30\xea\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x4c\x30\x21\x6e\x55\x30\x8c\x30\x5f\x30\x4b\x30\x01\x30\xe1\x30\xe2\x30\xea\x30\x4c\x30\x34\x78\xca\x58\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x00"/*@ L"FreeMemory Error : �������^�O�� MagicID ���s���ł��A������������A�h���X���n���ꂽ���A���������j�󂳂�Ă��܂�" @*/ )) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// �q�[�v�̃A�h���X���Z�b�g
	Heap = MemTag->Owner ;

	DX_HEAP_ENTER_CRITICAL_SECTION( Heap )

	// �f�o�b�O�̈�̃`�F�b�N
	if( Heap->UseSeparateInfo == FALSE )
	{
		if( AllocMemTag_CheckDebugData( MemTag ) < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x72\x00\x65\x00\x65\x00\x4d\x00\x65\x00\x6d\x00\x6f\x00\x72\x00\x79\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xc7\x30\xd0\x30\xc3\x30\xb0\x30\x18\x98\xdf\x57\x6e\x30\x34\x78\xca\x58\x92\x30\xba\x78\x8d\x8a\x57\x30\x7e\x30\x57\x30\x5f\x30\x01\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\x0d\x4e\x63\x6b\x6a\x30\xa2\x30\xaf\x30\xbb\x30\xb9\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x5f\x30\x53\x30\x68\x30\x4c\x30\x03\x80\x48\x30\x89\x30\x8c\x30\x7e\x30\x59\x30\x00"/*@ L"FreeMemory Error : �f�o�b�O�̈�̔j����m�F���܂����A�������̕s���ȃA�N�Z�X�������������Ƃ��l�����܂�" @*/ )) ;
			PrintInfoMemory( AllocAddress, UseSeparateInfo ) ;
			*( ( DWORD * )0x00000000 ) = 0xffffffff ;
		}
	}

	// �g�p���`�F�b�N
	if( ( MemTag->Flag & ALLOCMEMTAG_FLAG_USE ) == 0 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x46\x00\x72\x00\x65\x00\x65\x00\x4d\x00\x65\x00\x6d\x00\x6f\x00\x72\x00\x79\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe3\x89\x3e\x65\x08\x6e\x7f\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6b\x30\xfe\x5b\x57\x30\x66\x30\x8d\x51\xa6\x5e\xe3\x89\x3e\x65\x57\x30\x88\x30\x46\x30\x68\x30\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"FreeMemory Error : ����ς݂̃������ɑ΂��čēx������悤�Ƃ��܂���" @*/ )) ;
		PrintInfoMemory( AllocAddress, UseSeparateInfo ) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// �m�ۃ������̏�����
//	_MEMSET( ( BYTE * )MemTag + ALLOCMEMTAG_SIZE + ALLOCMEMTAG_DEBUGAREA_SIZE, 0xff,
//		     ( MemTag->Size - ALLOCMEMTAG_DEBUGAREA_SIZE ) - ( ALLOCMEMTAG_SIZE + ALLOCMEMTAG_DEBUGAREA_SIZE ) ) ;

	// �������^�O��������鏈�����s��
	Heap_FreeMemory( Heap, MemTag, FALSE ) ;

	DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )

	return 0 ;
}

// �q�[�v����m�ۂ����������̈�̃A�h���X���擾����( UseSeparateInfo = TRUE �̏ꍇ�́AAllocAddress �� ALLOCMEMTAG �\���̂̃A�h���X��n�� )
extern void * GetAllocMemoryAddress( void *AllocAddress, int UseSeparateInfo )
{
	ALLOCMEMTAG *	MemTag ;

	// �������^�O�̃A�h���X���Z�o
	if( UseSeparateInfo )
	{
		MemTag = ( ALLOCMEMTAG * )AllocAddress ;
	}
	else
	{
		MemTag = ( ALLOCMEMTAG * )( ( BYTE * )AllocAddress - *( ( DWORD * )( ( BYTE * )AllocAddress - sizeof( DWORD ) ) ) ) ;
	}

	// �}�W�b�N�h�c�̊m�F
	if( MemTag->MagicID != ALLOCMEMTAG_MAGIC_ID )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x47\x00\x65\x00\x74\x00\x41\x00\x6c\x00\x6c\x00\x6f\x00\x63\x00\x4d\x00\x65\x00\x6d\x00\x6f\x00\x72\x00\x79\x00\x41\x00\x64\x00\x64\x00\x72\x00\x65\x00\x73\x00\x73\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\x20\x00\x4d\x00\x61\x00\x67\x00\x69\x00\x63\x00\x49\x00\x44\x00\x20\x00\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x01\x30\xa4\x8a\x63\x30\x5f\x30\xe1\x30\xe2\x30\xea\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x4c\x30\x21\x6e\x55\x30\x8c\x30\x5f\x30\x4b\x30\x01\x30\xe1\x30\xe2\x30\xea\x30\x4c\x30\x34\x78\xca\x58\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x00"/*@ L"GetAllocMemoryAddress Error : �������^�O�� MagicID ���s���ł��A������������A�h���X���n���ꂽ���A���������j�󂳂�Ă��܂�" @*/ )) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// �g�p���`�F�b�N
	if( ( MemTag->Flag & ALLOCMEMTAG_FLAG_USE ) == 0 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x47\x00\x65\x00\x74\x00\x41\x00\x6c\x00\x6c\x00\x6f\x00\x63\x00\x4d\x00\x65\x00\x6d\x00\x6f\x00\x72\x00\x79\x00\x41\x00\x64\x00\x64\x00\x72\x00\x65\x00\x73\x00\x73\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe3\x89\x3e\x65\x08\x6e\x7f\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6b\x30\xfe\x5b\x57\x30\x66\x30\xe1\x30\xe2\x30\xea\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x6e\x30\xd6\x53\x97\x5f\x92\x30\x4c\x88\x4a\x30\x46\x30\x68\x30\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"GetAllocMemoryAddress Error : ����ς݂̃������ɑ΂��ă������A�h���X�̎擾���s�����Ƃ��܂���" @*/ )) ;
		PrintInfoMemory( AllocAddress, UseSeparateInfo ) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// �A�h���X��Ԃ�
	return MemTag->UserAddress ;
}

// �q�[�v����m�ۂ����������̃T�C�Y���擾����( UseSeparateInfo = TRUE �̏ꍇ�́AAllocAddress �� ALLOCMEMTAG �\���̂̃A�h���X��n�� )
extern ALLOCMEM_SIZE_TYPE GetAllocSize( void *AllocAddress, int UseSeparateInfo )
{
	ALLOCMEMTAG *	MemTag ;

	// �������^�O�̃A�h���X���Z�o
	if( UseSeparateInfo )
	{
		MemTag = ( ALLOCMEMTAG * )AllocAddress ;
	}
	else
	{
		MemTag = ( ALLOCMEMTAG * )( ( BYTE * )AllocAddress - *( ( DWORD * )( ( BYTE * )AllocAddress - sizeof( DWORD ) ) ) ) ;
	}

	// �}�W�b�N�h�c�̊m�F
	if( MemTag->MagicID != ALLOCMEMTAG_MAGIC_ID )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x47\x00\x65\x00\x74\x00\x41\x00\x6c\x00\x6c\x00\x6f\x00\x63\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\x20\x00\x4d\x00\x61\x00\x67\x00\x69\x00\x63\x00\x49\x00\x44\x00\x20\x00\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x01\x30\xa4\x8a\x63\x30\x5f\x30\xe1\x30\xe2\x30\xea\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x4c\x30\x21\x6e\x55\x30\x8c\x30\x5f\x30\x4b\x30\x01\x30\xe1\x30\xe2\x30\xea\x30\x4c\x30\x34\x78\xca\x58\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x00"/*@ L"GetAllocSize Error : �������^�O�� MagicID ���s���ł��A������������A�h���X���n���ꂽ���A���������j�󂳂�Ă��܂�" @*/ )) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// �g�p���`�F�b�N
	if( ( MemTag->Flag & ALLOCMEMTAG_FLAG_USE ) == 0 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x47\x00\x65\x00\x74\x00\x41\x00\x6c\x00\x6c\x00\x6f\x00\x63\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe3\x89\x3e\x65\x08\x6e\x7f\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6b\x30\xfe\x5b\x57\x30\x66\x30\xba\x78\xdd\x4f\xe1\x30\xe2\x30\xea\x30\xb5\x30\xa4\x30\xba\x30\x6e\x30\xd6\x53\x97\x5f\x92\x30\x4c\x88\x4a\x30\x46\x30\x68\x30\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"GetAllocSize Error : ����ς݂̃������ɑ΂��Ċm�ۃ������T�C�Y�̎擾���s�����Ƃ��܂���" @*/ )) ;
		PrintInfoMemory( AllocAddress, UseSeparateInfo ) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// �T�C�Y��Ԃ�
	return MemTag->UserSize ;
}

// �q�[�v����m�ۂ����������� ALLOCMEMTAG�\���̂��擾����( UseSeparateInfo = TRUE �̏ꍇ�́AAllocAddress �� ALLOCMEMTAG �\���̂̃A�h���X��n�� )
extern ALLOCMEMTAG * GetAllocMemTag( void *AllocAddress, int UseSeparateInfo )
{
	ALLOCMEMTAG *	MemTag ;

	// �������^�O�̃A�h���X���Z�o
	if( UseSeparateInfo )
	{
		MemTag = ( ALLOCMEMTAG * )AllocAddress ;
	}
	else
	{
		MemTag = ( ALLOCMEMTAG * )( ( BYTE * )AllocAddress - *( ( DWORD * )( ( BYTE * )AllocAddress - sizeof( DWORD ) ) ) ) ;
	}

	// �}�W�b�N�h�c�̊m�F
	if( MemTag->MagicID != ALLOCMEMTAG_MAGIC_ID )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x47\x00\x65\x00\x74\x00\x41\x00\x6c\x00\x6c\x00\x6f\x00\x63\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\x20\x00\x4d\x00\x61\x00\x67\x00\x69\x00\x63\x00\x49\x00\x44\x00\x20\x00\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x01\x30\xa4\x8a\x63\x30\x5f\x30\xe1\x30\xe2\x30\xea\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x4c\x30\x21\x6e\x55\x30\x8c\x30\x5f\x30\x4b\x30\x01\x30\xe1\x30\xe2\x30\xea\x30\x4c\x30\x34\x78\xca\x58\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x00"/*@ L"GetAllocMemTag Error : �������^�O�� MagicID ���s���ł��A������������A�h���X���n���ꂽ���A���������j�󂳂�Ă��܂�" @*/ )) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// �g�p���`�F�b�N
	if( ( MemTag->Flag & ALLOCMEMTAG_FLAG_USE ) == 0 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x47\x00\x65\x00\x74\x00\x41\x00\x6c\x00\x6c\x00\x6f\x00\x63\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe3\x89\x3e\x65\x08\x6e\x7f\x30\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6b\x30\xfe\x5b\x57\x30\x66\x30\xba\x78\xdd\x4f\xe1\x30\xe2\x30\xea\x30\xb5\x30\xa4\x30\xba\x30\x6e\x30\xd6\x53\x97\x5f\x92\x30\x4c\x88\x4a\x30\x46\x30\x68\x30\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"GetAllocMemTag Error : ����ς݂̃������ɑ΂��Ċm�ۃ������T�C�Y�̎擾���s�����Ƃ��܂���" @*/ )) ;
		PrintInfoMemory( AllocAddress, UseSeparateInfo ) ;
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// �^�O�̃A�h���X��Ԃ�
	return MemTag ;
}

// �q�[�v����m�ۂ����������̏������O�ɏo�͂���( UseSeparateInfo = TRUE �̏ꍇ�́AAllocAddress �� ALLOCMEMTAG �\���̂̃A�h���X��n�� )
extern void PrintInfoMemory( void *AllocAddress, int UseSeparateInfo )
{
#ifndef DX_NON_LITERAL_STRING
	ALLOCMEMTAG *	MemTag ;
	char			str[ 512 ] ;

	// �������^�O�̃A�h���X���Z�o
	if( UseSeparateInfo )
	{
		MemTag = ( ALLOCMEMTAG * )AllocAddress ;
	}
	else
	{
		MemTag = ( ALLOCMEMTAG * )( ( BYTE * )AllocAddress - *( ( DWORD * )( ( BYTE * )AllocAddress - sizeof( DWORD ) ) ) ) ;
	}

	CL_snprintf( DX_CHARCODEFORMAT_ASCII, FALSE, DX_CHARCODEFORMAT_ASCII, WCHAR_T_CHARCODEFORMAT, str, sizeof( str ),
#ifdef __64BIT__
		"\tsize:%10d(%10.3fkb)  user size:%10d(%10.3fkb)  time:%05d  file:%-26s  line:%-6d  ID:%-5d  addr:%016llx",
#else // __64BIT__
		"\tsize:%10d(%10.3fkb)  user size:%10d(%10.3fkb)  time:%05d  file:%-26s  line:%-6d  ID:%-5d  addr:%08x",
#endif // __64BIT__
		MemTag->Size,
		MemTag->Size / 1024.0f,
		MemTag->UserSize,
		MemTag->UserSize / 1024.0f,
		( int )MemTag->Time,
		MemTag->Name,
		MemTag->Line + ( ( MemTag->Flag & ( ALLOCMEMTAG_FLAG_LINE_BIT_16 | ALLOCMEMTAG_FLAG_LINE_BIT_17 ) ) << 16 ),
		( int )MemTag->ID,
		( DWORD_PTR )MemTag->UserAddress
	) ;

	{
		char *	p ;
		BYTE *	d ;
		size_t	len ;
		size_t	i ;

//		CL_strcpy( DX_CHARCODEFORMAT_ASCII, str + CL_strlen( DX_CHARCODEFORMAT_ASCII, str ), "  data:<" ) ;
		len = 16 < MemTag->UserSize ? 16 : MemTag->UserSize ;
		p = str + CL_strlen( DX_CHARCODEFORMAT_ASCII, str ) ;
		d = ( BYTE * )MemTag->UserAddress ;
//		for( i = 0 ; i < len ; i ++, p ++ )
//		{
//			*p = d[ i ] < 0x20 ? '.' : d[ i ] ;
//		}
//		CL_strcpy( DX_CHARCODEFORMAT_ASCII, p, "> [" ) ;
//		p += 3 ;

		CL_strcpy( DX_CHARCODEFORMAT_ASCII, p, "  data:[" ) ;
		p += 8 ;
		for( i = 0 ; i < len ; i ++, p += 3, d ++ )
		{
			CL_sprintf( DX_CHARCODEFORMAT_ASCII, FALSE, DX_CHARCODEFORMAT_ASCII, WCHAR_T_CHARCODEFORMAT, p, "%02x ", *( ( unsigned char * ) d ) ) ;
		}
		CL_strcpy( DX_CHARCODEFORMAT_ASCII, &p[ -1 ], "]\n" ) ;
	}

	DXST_ERRORLOG_ADDA( str ) ;

#endif // DX_NON_LITERAL_STRING
}

// �q�[�v�̏������O�ɏo�͂���
extern int HeapInfoDump( HEAPINFO *Heap )
{
	ALLOCMEMTAG *MemTag ;

	DX_HEAP_ENTER_CRITICAL_SECTION( Heap )

	// �S�Ă̊m�ۃ������^�O�̏����o��
	for( MemTag = Heap->UseFirstTag ; MemTag != NULL ; MemTag = MemTag->ListNext )
	{
		PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
	}

	DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )

	return 0 ;
}

// �q�[�v����m�ۂ����������ɐݒ肷�鎞�Ԃ�ύX����
extern int SetHeapTime( HEAPINFO *Heap, WORD Time )
{
	Heap->AllocMemoryTime = Time ;

	return 0 ;
}

// �q�[�v�̓��e�ɃG���[���Ȃ����`�F�b�N����
extern int HeapErrorCheck( HEAPINFO *Heap )
{
	ALLOCMEMTAG *MemTag ;

	DX_HEAP_ENTER_CRITICAL_SECTION( Heap )

	// �������g�p�̈�ƃ}�W�b�NID�ƃf�o�b�O�̈�̃`�F�b�N
	for( MemTag = Heap->FirstTag ; MemTag != NULL ; MemTag = MemTag->Next )
	{
		if( MemTag->MagicID != ALLOCMEMTAG_MAGIC_ID )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\x20\x00\x4d\x00\x61\x00\x67\x00\x69\x00\x63\x00\x49\x00\x44\x00\x20\x00\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x01\x30\xa4\x8a\x63\x30\x5f\x30\xe1\x30\xe2\x30\xea\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\x4c\x30\x21\x6e\x55\x30\x8c\x30\x5f\x30\x4b\x30\x01\x30\xe1\x30\xe2\x30\xea\x30\x4c\x30\x34\x78\xca\x58\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x00"/*@ L"HeapErrorCheck Error : �������^�O�� MagicID ���s���ł��A������������A�h���X���n���ꂽ���A���������j�󂳂�Ă��܂�" @*/ )) ;
			*( ( DWORD * )0x00000000 ) = 0xffffffff ;
		}

		if( Heap->UseSeparateInfo )
		{
			if( ( DWORD_PTR )MemTag->AllocAddress <  ( DWORD_PTR )MemTag->UseHeapMem->HeapAddress ||
				( DWORD_PTR )MemTag->AllocAddress >= ( DWORD_PTR )MemTag->UseHeapMem->HeapAddress + MemTag->UseHeapMem->HeapSize )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x4c\x30\xba\x78\xdd\x4f\x57\x30\x5f\x30\xd2\x30\xfc\x30\xd7\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xc4\x7b\xf2\x56\x6b\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x55\x00\x73\x00\x65\x00\x53\x00\x65\x00\x70\x00\x61\x00\x72\x00\x61\x00\x74\x00\x65\x00\x49\x00\x6e\x00\x66\x00\x6f\x00\x20\x00\x3d\x00\x20\x00\x54\x00\x52\x00\x55\x00\x45\x00\x00"/*@ L"HeapErrorCheck Error : �������^�O���m�ۂ����q�[�v�������͈̔͂ɂ���܂���  Heap->UseSeparateInfo = TRUE" @*/ )) ;
				PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
				*( ( DWORD * )0x00000000 ) = 0xffffffff ;
			}
		}
		else
		{
			if( ( DWORD_PTR )MemTag <  ( DWORD_PTR )MemTag->UseHeapMem->HeapAddress ||
				( DWORD_PTR )MemTag >= ( DWORD_PTR )MemTag->UseHeapMem->HeapAddress + MemTag->UseHeapMem->HeapSize )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x4c\x30\xba\x78\xdd\x4f\x57\x30\x5f\x30\xd2\x30\xfc\x30\xd7\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xc4\x7b\xf2\x56\x6b\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x55\x00\x73\x00\x65\x00\x53\x00\x65\x00\x70\x00\x61\x00\x72\x00\x61\x00\x74\x00\x65\x00\x49\x00\x6e\x00\x66\x00\x6f\x00\x20\x00\x3d\x00\x20\x00\x46\x00\x41\x00\x4c\x00\x53\x00\x45\x00\x00"/*@ L"HeapErrorCheck Error : �������^�O���m�ۂ����q�[�v�������͈̔͂ɂ���܂���  Heap->UseSeparateInfo = FALSE" @*/ )) ;
				PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
				*( ( DWORD * )0x00000000 ) = 0xffffffff ;
			}

			if( AllocMemTag_CheckDebugData( MemTag ) < 0 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xc7\x30\xd0\x30\xc3\x30\xb0\x30\x18\x98\xdf\x57\x6e\x30\x34\x78\xca\x58\x92\x30\xba\x78\x8d\x8a\x57\x30\x7e\x30\x57\x30\x5f\x30\x01\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\x0d\x4e\x63\x6b\x6a\x30\xa2\x30\xaf\x30\xbb\x30\xb9\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x5f\x30\x53\x30\x68\x30\x4c\x30\x03\x80\x48\x30\x89\x30\x8c\x30\x7e\x30\x59\x30\x00"/*@ L"HeapErrorCheck Error : �f�o�b�O�̈�̔j����m�F���܂����A�������̕s���ȃA�N�Z�X�������������Ƃ��l�����܂�" @*/ )) ;
				PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
				*( ( DWORD * )0x00000000 ) = 0xffffffff ;
			}
		}
	}

	// �A���C���`�F�b�N
	{
		if( Heap->UseSeparateInfo )
		{
			for( MemTag = Heap->FirstTag ; MemTag != NULL ; MemTag = MemTag->Next )
			{
				if( ( DWORD_PTR )MemTag->AllocAddress % ALLOCMEM_MIN_ALIGNED != 0 )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\xa2\x30\xe9\x30\xa4\x30\xf3\x30\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x55\x00\x73\x00\x65\x00\x53\x00\x65\x00\x70\x00\x61\x00\x72\x00\x61\x00\x74\x00\x65\x00\x49\x00\x6e\x00\x66\x00\x6f\x00\x20\x00\x3d\x00\x20\x00\x54\x00\x52\x00\x55\x00\x45\x00\x00"/*@ L"HeapErrorCheck Error : �������^�O�̃A���C�����s���ł�  Heap->UseSeparateInfo = TRUE" @*/ )) ;
					PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
					*( ( DWORD * )0x00000000 ) = 0xffffffff ;
				}
			}
		}
		else
		{
			for( MemTag = Heap->FirstTag ; MemTag != NULL ; MemTag = MemTag->Next )
			{
				if( ( DWORD_PTR )MemTag % ALLOCMEM_MIN_ALIGNED != 0 )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\xa2\x30\xe9\x30\xa4\x30\xf3\x30\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x55\x00\x73\x00\x65\x00\x53\x00\x65\x00\x70\x00\x61\x00\x72\x00\x61\x00\x74\x00\x65\x00\x49\x00\x6e\x00\x66\x00\x6f\x00\x20\x00\x3d\x00\x20\x00\x46\x00\x41\x00\x4c\x00\x53\x00\x45\x00\x00"/*@ L"HeapErrorCheck Error : �������^�O�̃A���C�����s���ł�  Heap->UseSeparateInfo = FALSE" @*/ )) ;
					PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
					*( ( DWORD * )0x00000000 ) = 0xffffffff ;
				}
			}
		}
	}

	// ���������T�C�Y�̃`�F�b�N
	{
		ALLOCMEM_SIZE_TYPE TotalSize ;
		ALLOCMEM_SIZE_TYPE TotalSize2 ;
		HEAPMEMINFO *HeapMem ;

		TotalSize = 0 ;
		for( MemTag = Heap->FirstTag ; MemTag != NULL ; MemTag = MemTag->Next )
		{
			TotalSize += MemTag->Size ;
		}

		TotalSize2 = 0 ;
		for( HeapMem = Heap->FirstHeapMem ; HeapMem != NULL ; HeapMem = HeapMem->Next )
		{
			TotalSize2 += HeapMem->HeapSize ;
		}
		if( TotalSize != Heap->TotalUseSize + Heap->TotalFreeSize )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xcf\x7d\xe1\x30\xe2\x30\xea\x30\xb5\x30\xa4\x30\xba\x30\x4c\x30\x00\x4e\xf4\x81\x57\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x20\x00\x54\x00\x6f\x00\x74\x00\x61\x00\x6c\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x3a\x00\x25\x00\x6c\x00\x6c\x00\x64\x00\x20\x00\x20\x00\x20\x00\x28\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x54\x00\x6f\x00\x74\x00\x61\x00\x6c\x00\x55\x00\x73\x00\x65\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x20\x00\x2b\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x54\x00\x6f\x00\x74\x00\x61\x00\x6c\x00\x46\x00\x72\x00\x65\x00\x65\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x20\x00\x29\x00\x3a\x00\x25\x00\x6c\x00\x6c\x00\x64\x00\x00"/*@ L"HeapErrorCheck Error : ���������T�C�Y����v���܂���  TotalSize:%lld   ( Heap->TotalUseSize + Heap->TotalFreeSize ):%lld" @*/, ( ULONGLONG )TotalSize, ( ULONGLONG )( Heap->TotalUseSize + Heap->TotalFreeSize ) )) ;
			*( ( DWORD * )0x00000000 ) = 0xffffffff ;
		}
		if( TotalSize != TotalSize2 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xcf\x7d\xe1\x30\xe2\x30\xea\x30\xb5\x30\xa4\x30\xba\x30\x4c\x30\x00\x4e\xf4\x81\x57\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x20\x00\x54\x00\x6f\x00\x74\x00\x61\x00\x6c\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x3a\x00\x25\x00\x6c\x00\x6c\x00\x64\x00\x20\x00\x20\x00\x20\x00\x54\x00\x6f\x00\x74\x00\x61\x00\x6c\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x32\x00\x3a\x00\x25\x00\x6c\x00\x6c\x00\x64\x00\x00"/*@ L"HeapErrorCheck Error : ���������T�C�Y����v���܂���  TotalSize:%lld   TotalSize2:%lld" @*/, ( ULONGLONG )TotalSize, ( ULONGLONG )TotalSize2 )) ;
			*( ( DWORD * )0x00000000 ) = 0xffffffff ;
		}
	}

	// �������̈�̑O��֌W�̃`�F�b�N
	{
		if( Heap->UseSeparateInfo )
		{
			for( MemTag = Heap->FirstTag ; MemTag != NULL ; MemTag = MemTag->Next )
			{
				if( MemTag->Flag & ALLOCMEMTAG_FLAG_PREV_CONNECT )
				{
					if( ( DWORD_PTR )MemTag->Prev->AllocAddress + MemTag->Prev->Size != ( DWORD_PTR )MemTag->AllocAddress )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\x23\x90\x50\x7d\xc5\x60\x31\x58\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x20\x00\x20\x00\x28\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x50\x00\x72\x00\x65\x00\x76\x00\x2d\x00\x3e\x00\x41\x00\x6c\x00\x6c\x00\x6f\x00\x63\x00\x41\x00\x64\x00\x64\x00\x72\x00\x65\x00\x73\x00\x73\x00\x20\x00\x2b\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x50\x00\x72\x00\x65\x00\x76\x00\x2d\x00\x3e\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x20\x00\x29\x00\x3a\x00\x30\x00\x78\x00\x25\x00\x6c\x00\x6c\x00\x30\x00\x31\x00\x36\x00\x78\x00\x20\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x41\x00\x6c\x00\x6c\x00\x6f\x00\x63\x00\x41\x00\x64\x00\x64\x00\x72\x00\x65\x00\x73\x00\x73\x00\x3a\x00\x30\x00\x78\x00\x25\x00\x6c\x00\x6c\x00\x30\x00\x31\x00\x36\x00\x78\x00\x00"/*@ L"HeapErrorCheck Error : �������^�O�̘A����񂪕s���ł�  ( MemTag->Prev->AllocAddress + MemTag->Prev->Size ):0x%ll016x  MemTag->AllocAddress:0x%ll016x" @*/, ( ULONGLONG )( ( DWORD_PTR )MemTag->Prev->AllocAddress + MemTag->Prev->Size ), ( ULONGLONG )MemTag->AllocAddress )) ;
						PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
						*( ( DWORD * )0x00000000 ) = 0xffffffff ;
					}
				}

				if( MemTag->Flag & ALLOCMEMTAG_FLAG_NEXT_CONNECT )
				{
					if( ( DWORD_PTR )MemTag->Next->AllocAddress != ( DWORD_PTR )MemTag->AllocAddress + MemTag->Size )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\x23\x90\x50\x7d\xc5\x60\x31\x58\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x20\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x4e\x00\x65\x00\x78\x00\x74\x00\x2d\x00\x3e\x00\x41\x00\x6c\x00\x6c\x00\x6f\x00\x63\x00\x41\x00\x64\x00\x64\x00\x72\x00\x65\x00\x73\x00\x73\x00\x3a\x00\x30\x00\x78\x00\x25\x00\x6c\x00\x6c\x00\x30\x00\x31\x00\x36\x00\x78\x00\x20\x00\x20\x00\x28\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x41\x00\x6c\x00\x6c\x00\x6f\x00\x63\x00\x41\x00\x64\x00\x64\x00\x72\x00\x65\x00\x73\x00\x73\x00\x20\x00\x2b\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x20\x00\x29\x00\x3a\x00\x30\x00\x78\x00\x25\x00\x6c\x00\x6c\x00\x30\x00\x31\x00\x36\x00\x78\x00\x00"/*@ L"HeapErrorCheck Error : �������^�O�̘A����񂪕s���ł�  MemTag->Next->AllocAddress:0x%ll016x  ( MemTag->AllocAddress + MemTag->Size ):0x%ll016x" @*/, ( ULONGLONG )MemTag->Next->AllocAddress, ( ULONGLONG )( ( DWORD_PTR )MemTag->AllocAddress + MemTag->Size ) )) ;
						*( ( DWORD * )0x00000000 ) = 0xffffffff ;
					}
				}
			}
		}
		else
		{
			for( MemTag = Heap->FirstTag ; MemTag != NULL ; MemTag = MemTag->Next )
			{
				if( MemTag->Flag & ALLOCMEMTAG_FLAG_PREV_CONNECT )
				{
					if( ( DWORD_PTR )MemTag->Prev + MemTag->Prev->Size != ( DWORD_PTR )MemTag )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\x23\x90\x50\x7d\xc5\x60\x31\x58\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x20\x00\x20\x00\x28\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x50\x00\x72\x00\x65\x00\x76\x00\x20\x00\x2b\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x50\x00\x72\x00\x65\x00\x76\x00\x2d\x00\x3e\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x20\x00\x29\x00\x3a\x00\x30\x00\x78\x00\x25\x00\x6c\x00\x6c\x00\x30\x00\x31\x00\x36\x00\x78\x00\x20\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x3a\x00\x30\x00\x78\x00\x25\x00\x6c\x00\x6c\x00\x30\x00\x31\x00\x36\x00\x78\x00\x00"/*@ L"HeapErrorCheck Error : �������^�O�̘A����񂪕s���ł�  ( MemTag->Prev + MemTag->Prev->Size ):0x%ll016x  MemTag:0x%ll016x" @*/, ( ULONGLONG )( MemTag->Prev + MemTag->Prev->Size ), ( ULONGLONG )MemTag )) ;
						PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
						*( ( DWORD * )0x00000000 ) = 0xffffffff ;
					}
				}

				if( MemTag->Flag & ALLOCMEMTAG_FLAG_NEXT_CONNECT )
				{
					if( ( DWORD_PTR )MemTag->Next != ( DWORD_PTR )MemTag + MemTag->Size )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\x23\x90\x50\x7d\xc5\x60\x31\x58\x4c\x30\x0d\x4e\x63\x6b\x67\x30\x59\x30\x20\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x4e\x00\x65\x00\x78\x00\x74\x00\x3a\x00\x30\x00\x78\x00\x25\x00\x6c\x00\x6c\x00\x30\x00\x31\x00\x36\x00\x78\x00\x20\x00\x20\x00\x28\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x20\x00\x2b\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x20\x00\x29\x00\x3a\x00\x30\x00\x78\x00\x25\x00\x6c\x00\x6c\x00\x30\x00\x31\x00\x36\x00\x78\x00\x00"/*@ L"HeapErrorCheck Error : �������^�O�̘A����񂪕s���ł�  MemTag->Next:0x%ll016x  ( MemTag + MemTag->Size ):0x%ll016x" @*/, ( ULONGLONG )MemTag->Next, ( ULONGLONG )( MemTag + MemTag->Size ) )) ;
						PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
						*( ( DWORD * )0x00000000 ) = 0xffffffff ;
					}
				}
			}
		}
	}

	// �󂫃������^�O���A�����Ă��܂��Ă��Ȃ����`�F�b�N
	{
		for( MemTag = Heap->FirstTag ; MemTag != NULL ; MemTag = MemTag->Next )
		{
			if( MemTag->Flag & ALLOCMEMTAG_FLAG_USE )
			{
				continue ;
			}

			if( MemTag->Flag & ALLOCMEMTAG_FLAG_PREV_CONNECT )
			{
				if( ( MemTag->Prev->Flag & ALLOCMEMTAG_FLAG_USE ) == 0 )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x7a\x7a\x4d\x30\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x4c\x30\x0b\x4e\x4d\x4f\xe1\x30\xe2\x30\xea\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\xb9\x65\x11\x54\x6b\x30\x23\x90\x9a\x7d\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x00"/*@ L"HeapErrorCheck Error : �󂫃������^�O�����ʃ������A�h���X�����ɘA�����Ă��܂�" @*/ )) ;
					PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
					*( ( DWORD * )0x00000000 ) = 0xffffffff ;
				}
			}

			if( MemTag->Flag & ALLOCMEMTAG_FLAG_NEXT_CONNECT )
			{
				if( ( MemTag->Next->Flag & ALLOCMEMTAG_FLAG_USE ) == 0 )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x7a\x7a\x4d\x30\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x4c\x30\x0a\x4e\x4d\x4f\xe1\x30\xe2\x30\xea\x30\xa2\x30\xc9\x30\xec\x30\xb9\x30\xb9\x65\x11\x54\x6b\x30\x23\x90\x9a\x7d\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x00"/*@ L"HeapErrorCheck Error : �󂫃������^�O����ʃ������A�h���X�����ɘA�����Ă��܂�" @*/ )) ;
					PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
					*( ( DWORD * )0x00000000 ) = 0xffffffff ;
				}
			}
		}
	}

	// ���g�p���X�g�̃`�F�b�N
	if( Heap->UseSeparateInfo )
	{
		// ���g�p�^�O�̐����`�F�b�N
		{
			int TagNum ;
			ALLOCMEMTAG *MemTag ;

			for( MemTag = Heap->SepTagArrayNotUseFirst, TagNum = 0 ; MemTag != NULL ; MemTag = MemTag->ListNext, TagNum ++ ){}
			if( TagNum != Heap->SepTagArrayLength - Heap->SepTagArrayUseNum )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xbf\x30\xb0\x30\x4d\x91\x17\x52\x06\x52\xe2\x96\xbf\x30\xa4\x30\xd7\x30\x6e\x30\x2a\x67\x7f\x4f\x28\x75\xbf\x30\xb0\x30\x6e\x30\x70\x65\x4c\x30\x00\x4e\xf4\x81\x57\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x20\x00\x54\x00\x61\x00\x67\x00\x4e\x00\x75\x00\x6d\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x28\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x53\x00\x65\x00\x70\x00\x54\x00\x61\x00\x67\x00\x41\x00\x72\x00\x72\x00\x61\x00\x79\x00\x4c\x00\x65\x00\x6e\x00\x67\x00\x74\x00\x68\x00\x20\x00\x2d\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x53\x00\x65\x00\x70\x00\x54\x00\x61\x00\x67\x00\x41\x00\x72\x00\x72\x00\x61\x00\x79\x00\x55\x00\x73\x00\x65\x00\x4e\x00\x75\x00\x6d\x00\x20\x00\x29\x00\x3a\x00\x25\x00\x64\x00\x00"/*@ L"HeapErrorCheck Error : �^�O�z�񕪗��^�C�v�̖��g�p�^�O�̐�����v���܂���  TagNum:%d  ( Heap->SepTagArrayLength - Heap->SepTagArrayUseNum ):%d" @*/, TagNum, Heap->SepTagArrayLength - Heap->SepTagArrayUseNum )) ;
				*( ( DWORD * )0x00000000 ) = 0xffffffff ;
			}
		}

		// ���g�p�������q�[�v�̐����`�F�b�N
		{
			int HeapMemNum ;
			HEAPMEMINFO *HeapMem ;

			for( HeapMem = Heap->SepHeapMemArrayNotUseFirst, HeapMemNum = 0 ; HeapMem != NULL ; HeapMem = HeapMem->Next, HeapMemNum ++ ){}
			if( HeapMemNum != Heap->SepHeapMemArrayLength - Heap->SepHeapMemArrayUseNum )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xd2\x30\xfc\x30\xd7\x30\xe1\x30\xe2\x30\xea\x30\xc5\x60\x31\x58\x06\x52\xe2\x96\xbf\x30\xa4\x30\xd7\x30\x6e\x30\x2a\x67\x7f\x4f\x28\x75\xc5\x60\x31\x58\x6e\x30\x70\x65\x4c\x30\x00\x4e\xf4\x81\x57\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x4d\x00\x65\x00\x6d\x00\x4e\x00\x75\x00\x6d\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x28\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x53\x00\x65\x00\x70\x00\x48\x00\x65\x00\x61\x00\x70\x00\x4d\x00\x65\x00\x6d\x00\x41\x00\x72\x00\x72\x00\x61\x00\x79\x00\x4c\x00\x65\x00\x6e\x00\x67\x00\x74\x00\x68\x00\x20\x00\x2d\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x53\x00\x65\x00\x70\x00\x48\x00\x65\x00\x61\x00\x70\x00\x4d\x00\x65\x00\x6d\x00\x41\x00\x72\x00\x72\x00\x61\x00\x79\x00\x55\x00\x73\x00\x65\x00\x4e\x00\x75\x00\x6d\x00\x20\x00\x29\x00\x3a\x00\x25\x00\x64\x00\x00"/*@ L"HeapErrorCheck Error : �q�[�v��������񕪗��^�C�v�̖��g�p���̐�����v���܂���  HeapMemNum:%d  ( Heap->SepHeapMemArrayLength - Heap->SepHeapMemArrayUseNum ):%d" @*/, HeapMemNum, Heap->SepHeapMemArrayLength - Heap->SepHeapMemArrayUseNum )) ;
				*( ( DWORD * )0x00000000 ) = 0xffffffff ;
			}
		}
	}

	// ���g�p�������T�C�Y�̃`�F�b�N
	{
		ALLOCMEM_SIZE_TYPE TotalUseSize ;
		int UseNum ;

		TotalUseSize = 0 ;
		UseNum = 0 ;
		for( MemTag = Heap->UseFirstTag ; MemTag != NULL ; MemTag = MemTag->ListNext, UseNum ++ )
		{
			if( ( MemTag->Flag & ALLOCMEMTAG_FLAG_USE ) == 0 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x7f\x4f\x28\x75\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\xea\x30\xb9\x30\xc8\x30\x6e\x30\x2d\x4e\x6b\x30\x7a\x7a\x4d\x30\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"HeapErrorCheck Error : �g�p�������^�O�̃��X�g�̒��ɋ󂫃������^�O������܂���" @*/ )) ;
				PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
				*( ( DWORD * )0x00000000 ) = 0xffffffff ;
			}
			TotalUseSize += MemTag->Size ;
		}
		if( UseNum != Heap->UseTagNum )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x7f\x4f\x28\x75\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\x70\x65\x4c\x30\x00\x4e\xf4\x81\x57\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x20\x00\x55\x00\x73\x00\x65\x00\x4e\x00\x75\x00\x6d\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x55\x00\x73\x00\x65\x00\x54\x00\x61\x00\x67\x00\x4e\x00\x75\x00\x6d\x00\x3a\x00\x25\x00\x64\x00\x00"/*@ L"HeapErrorCheck Error : �g�p�������^�O�̐�����v���܂���  UseNum:%d  Heap->UseTagNum:%d" @*/, UseNum, Heap->UseTagNum )) ;
			*( ( DWORD * )0x00000000 ) = 0xffffffff ;
		}
		if( TotalUseSize != Heap->TotalUseSize )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x7f\x4f\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xb5\x30\xa4\x30\xba\x30\x4c\x30\x00\x4e\xf4\x81\x57\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x20\x00\x54\x00\x6f\x00\x74\x00\x61\x00\x6c\x00\x55\x00\x73\x00\x65\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x3a\x00\x25\x00\x6c\x00\x6c\x00\x64\x00\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x54\x00\x6f\x00\x74\x00\x61\x00\x6c\x00\x55\x00\x73\x00\x65\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x3a\x00\x25\x00\x6c\x00\x6c\x00\x64\x00\x00"/*@ L"HeapErrorCheck Error : �g�p�������̃T�C�Y����v���܂���  TotalUseSize:%lld  Heap->TotalUseSize:%lld" @*/, TotalUseSize, Heap->TotalUseSize )) ;
			*( ( DWORD * )0x00000000 ) = 0xffffffff ;
		}
	}

	// ���󂫃������T�C�Y�̃`�F�b�N
	{
		ALLOCMEM_SIZE_TYPE TotalFreeSize ;
		int FreeNum ;

		TotalFreeSize = 0 ;
		FreeNum = 0 ;
		if( Heap->AllocType == ALLOCMEMTYPE_SIMPLE )
		{
			for( MemTag = Heap->FreeFirstTag ; MemTag != NULL ; MemTag = MemTag->ListNext, FreeNum ++ )
			{
				if( ( MemTag->Flag & ALLOCMEMTAG_FLAG_USE ) != 0 )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x7a\x7a\x4d\x30\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\xea\x30\xb9\x30\xc8\x30\x6e\x30\x2d\x4e\x6b\x30\x7f\x4f\x28\x75\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x4c\x30\x42\x30\x8a\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"HeapErrorCheck Error : �󂫃������^�O�̃��X�g�̒��Ɏg�p�������^�O������܂���" @*/ )) ;
					PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
					*( ( DWORD * )0x00000000 ) = 0xffffffff ;
				}
				TotalFreeSize += MemTag->Size ;
			}
		}
		else
		if( Heap->AllocType == ALLOCMEMTYPE_TLSF )
		{
			int i ;
			int j ;
			int ValidCount ;

#ifdef __64BIT__
			for( i = 0 ; i < 64 ; i ++ )
#else // __64BIT__
			for( i = 0 ; i < 32 ; i ++ )
#endif // __64BIT__
			{
#ifdef __64BIT__
#if defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
				if( ( Heap->FreeTagBitList1 & ( 1ULL                  << i ) ) == 0 )
#else // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
				if( ( Heap->FreeTagBitList1 & ( ( ( ULONGLONG )1 )    << i ) ) == 0 )
#endif // defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
#else // __64BIT__
				if( ( Heap->FreeTagBitList1 & ( 1    << i ) ) == 0 )
#endif // __64BIT__
				{
					continue ;
				}

				ValidCount = 0 ;
				for( j = 0 ; j < ( 1 << ALLOCMEM_CATEGORY_2_SHIFT_NUM ) ; j ++ )
				{
					if( ( Heap->FreeTagBitList2[ i ] & ( 1 << j ) ) == 0 )
					{
						continue ;
					}

					ValidCount ++ ;

					if( Heap->FreeFirstTagArray[ i ][ j ] == NULL )
					{
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x54\x00\x4c\x00\x53\x00\x46\x00\xa8\x30\xe9\x30\xfc\x30\x00\x30\x00\x30\xab\x30\xc6\x30\xb4\x30\xea\x30\x12\xff\xc6\x30\xfc\x30\xd6\x30\xeb\x30\xa8\x30\xe9\x30\xfc\x30\x00"/*@ L"HeapErrorCheck Error : TLSF�G���[�@�@�J�e�S���Q�e�[�u���G���[" @*/ )) ;
						*( ( DWORD * )0x00000000 ) = 0xffffffff ;
					}

					for( MemTag = Heap->FreeFirstTagArray[ i ][ j ] ; MemTag != NULL ; MemTag = MemTag->ListNext, FreeNum ++ )
					{
						// �J�e�S���ɓK�������T�C�Y�����`�F�b�N
						int MSB = GetMSB( MemTag->Size ) ; 
						int SLI = GetSLI( MemTag->Size, MSB, ALLOCMEM_CATEGORY_2_SHIFT_NUM ) ; 

						if( MSB != i || SLI != j )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x54\x00\x4c\x00\x53\x00\x46\x00\xa8\x30\xe9\x30\xfc\x30\x00\x30\x00\x30\x7a\x7a\x4d\x30\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\xb5\x30\xa4\x30\xba\x30\xa8\x30\xe9\x30\xfc\x30\x20\x00\x20\x00\x4d\x00\x53\x00\x42\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x69\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x4c\x00\x49\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x6a\x00\x3a\x00\x25\x00\x64\x00\x00"/*@ L"HeapErrorCheck Error : TLSF�G���[�@�@�󂫃������^�O�T�C�Y�G���[  MSB:%d  i:%d  SLI:%d  j:%d" @*/, MSB, i, SLI, j )) ;
							*( ( DWORD * )0x00000000 ) = 0xffffffff ;
						}

						if( MemTag->FLI != MSB || MemTag->SLI != SLI )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x54\x00\x4c\x00\x53\x00\x46\x00\xa8\x30\xe9\x30\xfc\x30\x00\x30\x00\x30\x7a\x7a\x4d\x30\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\xab\x30\xc6\x30\xb4\x30\xea\x30\xc5\x60\x31\x58\xa8\x30\xe9\x30\xfc\x30\x20\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x46\x00\x4c\x00\x49\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x4d\x00\x53\x00\x42\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x53\x00\x4c\x00\x49\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x53\x00\x4c\x00\x49\x00\x3a\x00\x25\x00\x64\x00\x00"/*@ L"HeapErrorCheck Error : TLSF�G���[�@�@�󂫃������^�O�J�e�S�����G���[  MemTag->FLI:%d  MSB:%d  MemTag->SLI:%d  SLI:%d" @*/, MemTag->FLI, MSB, MemTag->SLI, SLI )) ;
							*( ( DWORD * )0x00000000 ) = 0xffffffff ;
						}

						if( ( MemTag->Flag & ALLOCMEMTAG_FLAG_USE ) != 0 )
						{
							DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x54\x00\x4c\x00\x53\x00\x46\x00\xa8\x30\xe9\x30\xfc\x30\x00\x30\x00\x30\x7a\x7a\x4d\x30\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\xea\x30\xb9\x30\xc8\x30\x6b\x30\x7f\x4f\x28\x75\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x4c\x30\x2b\x54\x7e\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"HeapErrorCheck Error : TLSF�G���[�@�@�󂫃������^�O���X�g�Ɏg�p�������^�O���܂܂�Ă��܂���" @*/ )) ;
							PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
							*( ( DWORD * )0x00000000 ) = 0xffffffff ;
						}
						TotalFreeSize += MemTag->Size ;
					}
				}

				if( ValidCount == 0 )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x54\x00\x4c\x00\x53\x00\x46\x00\xa8\x30\xe9\x30\xfc\x30\x00\x30\x00\x30\xab\x30\xc6\x30\xb4\x30\xea\x30\x11\xff\xc6\x30\xfc\x30\xd6\x30\xeb\x30\xa8\x30\xe9\x30\xfc\x30\x00"/*@ L"HeapErrorCheck Error : TLSF�G���[�@�@�J�e�S���P�e�[�u���G���[" @*/ )) ;
					*( ( DWORD * )0x00000000 ) = 0xffffffff ;
				}
			}
		}
		if( FreeNum != Heap->FreeTagNum )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x7a\x7a\x4d\x30\xe1\x30\xe2\x30\xea\x30\xbf\x30\xb0\x30\x6e\x30\x70\x65\x4c\x30\x00\x4e\xf4\x81\x57\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x20\x00\x46\x00\x72\x00\x65\x00\x65\x00\x4e\x00\x75\x00\x6d\x00\x3a\x00\x25\x00\x64\x00\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x46\x00\x72\x00\x65\x00\x65\x00\x54\x00\x61\x00\x67\x00\x4e\x00\x75\x00\x6d\x00\x3a\x00\x25\x00\x64\x00\x00"/*@ L"HeapErrorCheck Error : �󂫃������^�O�̐�����v���܂���  FreeNum:%d  Heap->FreeTagNum:%d" @*/, FreeNum, Heap->FreeTagNum )) ;
			*( ( DWORD * )0x00000000 ) = 0xffffffff ;
		}
		if( TotalFreeSize != Heap->TotalFreeSize )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\x7a\x7a\x4d\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xb5\x30\xa4\x30\xba\x30\x4c\x30\x00\x4e\xf4\x81\x57\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x20\x00\x54\x00\x6f\x00\x74\x00\x61\x00\x6c\x00\x46\x00\x72\x00\x65\x00\x65\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x3a\x00\x25\x00\x6c\x00\x6c\x00\x64\x00\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x54\x00\x6f\x00\x74\x00\x61\x00\x6c\x00\x46\x00\x72\x00\x65\x00\x65\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x3a\x00\x25\x00\x6c\x00\x6c\x00\x64\x00\x00"/*@ L"HeapErrorCheck Error : �󂫃������̃T�C�Y����v���܂���  TotalFreeSize:%lld  Heap->TotalFreeSize:%lld" @*/, TotalFreeSize, Heap->TotalFreeSize )) ;
			*( ( DWORD * )0x00000000 ) = 0xffffffff ;
		}
	}

	// ���[�U�[�������A�h���X�`�F�b�N
	{
		for( MemTag = Heap->UseFirstTag ; MemTag != NULL ; MemTag = MemTag->ListNext )
		{
			if( ( DWORD_PTR )MemTag->UserAddress % Heap->Aligned != 0 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe6\x30\xfc\x30\xb6\x30\xfc\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xa2\x30\xe9\x30\xa4\x30\xf3\x30\x4c\x30\x07\x63\x9a\x5b\x55\x30\x8c\x30\x5f\x30\xa2\x30\xe9\x30\xa4\x30\xf3\x30\x68\x30\x70\x75\x6a\x30\x8a\x30\x7e\x30\x59\x30\x20\x00\x20\x00\x4d\x00\x65\x00\x6d\x00\x54\x00\x61\x00\x67\x00\x2d\x00\x3e\x00\x55\x00\x73\x00\x65\x00\x72\x00\x41\x00\x64\x00\x64\x00\x72\x00\x65\x00\x73\x00\x73\x00\x3a\x00\x30\x00\x78\x00\x25\x00\x6c\x00\x6c\x00\x30\x00\x31\x00\x36\x00\x78\x00\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x41\x00\x6c\x00\x69\x00\x67\x00\x6e\x00\x65\x00\x64\x00\x3a\x00\x30\x00\x78\x00\x25\x00\x30\x00\x38\x00\x78\x00\x00"/*@ L"HeapErrorCheck Error : ���[�U�[�������̃A���C�����w�肳�ꂽ�A���C���ƈقȂ�܂�  MemTag->UserAddress:0x%ll016x  Heap->Aligned:0x%08x" @*/, ( ULONGLONG )MemTag->UserAddress, ( DWORD )Heap->Aligned )) ;
				PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
				*( ( DWORD * )0x00000000 ) = 0xffffffff ;
			}
			if( Heap->UseSeparateInfo )
			{
				if( ( DWORD_PTR )MemTag->UserAddress <  ( DWORD_PTR )MemTag->UseHeapMem->HeapAddress ||
					( DWORD_PTR )MemTag->UserAddress >= ( DWORD_PTR )MemTag->UseHeapMem->HeapAddress + MemTag->UseHeapMem->HeapSize )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe6\x30\xfc\x30\xb6\x30\xfc\x30\xe1\x30\xe2\x30\xea\x30\x4c\x30\xba\x78\xdd\x4f\x57\x30\x5f\x30\xd2\x30\xfc\x30\xd7\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xc4\x7b\xf2\x56\x6b\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x55\x00\x73\x00\x65\x00\x53\x00\x65\x00\x70\x00\x61\x00\x72\x00\x61\x00\x74\x00\x65\x00\x49\x00\x6e\x00\x66\x00\x6f\x00\x20\x00\x3d\x00\x20\x00\x54\x00\x52\x00\x55\x00\x45\x00\x00"/*@ L"HeapErrorCheck Error : ���[�U�[���������m�ۂ����q�[�v�������͈̔͂ɂ���܂���  Heap->UseSeparateInfo = TRUE" @*/ )) ;
					PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
					*( ( DWORD * )0x00000000 ) = 0xffffffff ;
				}
			}
			else
			{
				if( ( DWORD_PTR )MemTag->UserAddress - *( ( DWORD * )( ( BYTE * )MemTag->UserAddress - sizeof( DWORD ) ) ) != ( DWORD_PTR )MemTag )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\x48\x00\x65\x00\x61\x00\x70\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x43\x00\x68\x00\x65\x00\x63\x00\x6b\x00\x20\x00\x45\x00\x72\x00\x72\x00\x6f\x00\x72\x00\x20\x00\x3a\x00\x20\x00\xe6\x30\xfc\x30\xb6\x30\xfc\x30\xe1\x30\xe2\x30\xea\x30\x4c\x30\xba\x78\xdd\x4f\x57\x30\x5f\x30\xd2\x30\xfc\x30\xd7\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xc4\x7b\xf2\x56\x6b\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x20\x00\x48\x00\x65\x00\x61\x00\x70\x00\x2d\x00\x3e\x00\x55\x00\x73\x00\x65\x00\x53\x00\x65\x00\x70\x00\x61\x00\x72\x00\x61\x00\x74\x00\x65\x00\x49\x00\x6e\x00\x66\x00\x6f\x00\x20\x00\x3d\x00\x20\x00\x46\x00\x41\x00\x4c\x00\x53\x00\x45\x00\x00"/*@ L"HeapErrorCheck Error : ���[�U�[���������m�ۂ����q�[�v�������͈̔͂ɂ���܂���  Heap->UseSeparateInfo = FALSE" @*/ )) ;
					PrintInfoMemory( Heap->UseSeparateInfo ? MemTag : MemTag->UserAddress, Heap->UseSeparateInfo ) ;
					*( ( DWORD * )0x00000000 ) = 0xffffffff ;
				}
			}
		}
	}

	DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )

	return 0 ;
}

// �q�[�v�̊m�ۏ󋵂�`�悷��

#ifndef DX_NON_GRAPHICS

static void HeapDrawAllocInfo_Help( ALLOCMEMTAG *MemTag, int x, int y, int Width, float Scale, int Color )
{
	DWORD_PTR		Address ;
	HEAPMEMINFO *	HeapMemInfo ;
	int				DrawY ;
	int				DrawCheckHeight ;
	int				DrawPixelPosition ;
	int				DrawPixelWidth ;
	int				CheckPixelPosition ;
	int				i ;

	Address = 0 ;
	for( HeapMemInfo = MemTag->Owner->FirstHeapMem ; HeapMemInfo != MemTag->UseHeapMem ; HeapMemInfo = HeapMemInfo->Next )
	{
		Address += HeapMemInfo->HeapSize ;
	}

	if( MemTag->Owner->UseSeparateInfo )
	{
		Address += ( DWORD_PTR )MemTag->AllocAddress - ( DWORD_PTR )MemTag->UseHeapMem->HeapAddress ;
	}
	else
	{
		Address += ( DWORD_PTR )MemTag - ( DWORD_PTR )MemTag->UseHeapMem->HeapAddress ;
	}

	DrawPixelPosition = _FTOL( Address      * Scale ) ;
	DrawPixelWidth    = _FTOL( MemTag->Size * Scale ) ;
	if( DrawPixelWidth <= 0 )
	{
		DrawPixelWidth = 1 ;
	}

	DrawY           = DrawPixelPosition / Width ;
	DrawCheckHeight = ( DrawPixelPosition + DrawPixelWidth + Width - 1 ) / Width - DrawY ;

	CheckPixelPosition = DrawY * Width ;
	for( i = DrawY ; i < DrawY + DrawCheckHeight ; i ++, CheckPixelPosition += Width )
	{
		int DrawPixelTempX1 ;
		int DrawPixelTempX2 ;

		if( DrawPixelPosition <= CheckPixelPosition )
		{
			DrawPixelTempX1 = 0 ;
		}
		else
		{
			DrawPixelTempX1 = DrawPixelPosition - CheckPixelPosition ;
		}

		if( DrawPixelPosition + DrawPixelWidth >= CheckPixelPosition + Width )
		{
			DrawPixelTempX2 = Width ;
		}
		else
		{
			DrawPixelTempX2 = ( DrawPixelPosition + DrawPixelWidth ) - CheckPixelPosition ;
		}

		NS_DrawBox( x + DrawPixelTempX1, y + i, x + DrawPixelTempX2, y + i + 1, Color, TRUE ) ;
	}
}

#endif // DX_NON_GRAPHICS

extern int HeapDrawAllocInfo( HEAPINFO *Heap, int x, int y, int Width, int Height )
{
#ifndef DX_NON_GRAPHICS
	int				Pixels ;
	float			Scale ;
	ALLOCMEMTAG *	MemTag ;
	int				ColorUse ;
	int				ColorFree ;
	int				BlendMode ;
	int				BlendParam ;
	int				RedBright ;
	int				GreenBright ;
	int				BlueBright ;
	RECT			DrawArea ;

	DX_HEAP_ENTER_CRITICAL_SECTION( Heap )

	NS_GetDrawBlendMode( &BlendMode, &BlendParam ) ;
	NS_GetDrawBright( &RedBright, &GreenBright, &BlueBright ) ;
	NS_GetDrawArea( &DrawArea ) ;

	NS_SetDrawArea( x, y, x + Width, y + Height ) ;
	NS_SetDrawBright( 255,255,255 ) ;

	Pixels = Width * Height ;

	Scale = ( float )Pixels / ( Heap->TotalFreeSize + Heap->TotalUseSize ) ;

	NS_SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 ) ;
	NS_DrawBox( x, y, x + Width, y + Height, GetColor( 0, 0, 0 ), TRUE ) ;

	NS_SetDrawBlendMode( DX_BLENDMODE_ADD, 255 ) ;

	ColorUse  = GetColor( 255,0,0 ) ;
	ColorFree = GetColor( 0,255,0 ) ;
	for( MemTag = Heap->FirstTag ; MemTag != NULL ; MemTag = MemTag->Next )
	{
		HeapDrawAllocInfo_Help( MemTag, x, y, Width, Scale, ( MemTag->Flag & ALLOCMEMTAG_FLAG_USE ) ? ColorUse : ColorFree ) ;
	}

	NS_SetDrawBlendMode( BlendMode, BlendParam ) ;
	NS_SetDrawBright( RedBright, GreenBright, BlueBright ) ;
	NS_SetDrawArea( DrawArea.left, DrawArea.top, DrawArea.right, DrawArea.bottom ) ;

	DX_HEAP_LEAVE_CRITICAL_SECTION( Heap )

	return 0 ;
#else // DX_NON_GRAPHICS
	return -1 ;
#endif // DX_NON_GRAPHICS
}











#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE
