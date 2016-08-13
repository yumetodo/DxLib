// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�q�[�v�֌W�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXHEAP_H__
#define __DXHEAP_H__

// �C���N���[�h ------------------------------------------------------------------
#include "DxCompileConfig.h"
#include "DxThread.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------


#define HEAPMEM_FLAG_AUTOALLOC					(0x0001)			// �����m�ۂ����������̈悩�ǂ���

#define ALLOCMEMTAG_MAGIC_ID					(0x4D454D44)		// "DMEM"( �r�b�O�G���f�B�A�����Ƌt�ɂȂ����Ⴄ���ǁE�E�E )

#define ALLOCMEM_MIN_ALIGNED					(16)				// �A���C���̍Œ�l
#define ALLOCMEM_CATEGORY_2_SHIFT_NUM			(5)					// �J�e�S���[�Q�̐�( 32 )��2�̏搔

#ifdef __64BIT__
#define ALLOCMEMTAG_NAMELENGTH					(18)				// �m�ۃ������̖��O
#define ALLOCMEM_CATEGORY_1_NUM					(64)				// �J�e�S���[�P�̐�( ������ 0 �` ALLOCMEM_CATEGORY_2_SHIFT_NUM �̃J�e�S���͎g�p���Ȃ�( �ł��Ȃ� ) )
#define ALLOCMEM_SIZE_TYPE						ULONGLONG			// �m�ۃ������̃T�C�Y�̌^
#define ALLOCMEMTAG_SIZE						(112)				// ALLOCMEMTAG�\���̂̃T�C�Y
#else // __64BIT__
#define ALLOCMEMTAG_NAMELENGTH					(18)				// �m�ۃ������̖��O
#define ALLOCMEM_CATEGORY_1_NUM					(32)				// �J�e�S���[�P�̐�( ������ 0 �` ALLOCMEM_CATEGORY_2_SHIFT_NUM �̃J�e�S���͎g�p���Ȃ�( �ł��Ȃ� ) )
#define ALLOCMEM_SIZE_TYPE						DWORD				// �m�ۃ������̃T�C�Y�̌^
#define ALLOCMEMTAG_SIZE						(72)				// ALLOCMEMTAG�\���̂̃T�C�Y
#endif // __64BIT__

#define ALLOCMEMTAG_DEBUGAREA_SIZE				(16)				// �f�o�b�O�p�̒l�Ŗ��߂�̈�̃T�C�Y

// ��̃������m�ۂŕK�v�ȃ^�O�֌W�̃f�[�^�T�C�Y
#define ALLOCMEMTAG_TOTAL_SIZE					( ALLOCMEMTAG_SIZE + ALLOCMEMTAG_DEBUGAREA_SIZE * 2 + sizeof( DWORD ) )

	// �������^�O�����邽�߂ɍŒ���K�v�ȋ󂫃������T�C�Y( �A���C�����l���������� )
#define ALLOCMEMTAG_MIN_SIZE					( ( ALLOCMEMTAG_TOTAL_SIZE + ALLOCMEM_MIN_ALIGNED * 2 + ALLOCMEM_MIN_ALIGNED - 1 ) / ALLOCMEM_MIN_ALIGNED * ALLOCMEM_MIN_ALIGNED )

#define ALLOCMEMTAG_FLAG_LINE_BIT_16			(0x01)				// �s�ԍ���16bit��( WORD �ł� 65535�s�܂ł����\���ł��Ȃ��̂ŁA262143�s�܂ŕ\���ł���悤�ɂ��邽�߂̑Ή� )
#define ALLOCMEMTAG_FLAG_LINE_BIT_17			(0x02)				// �s�ԍ���17bit��( ���� )
#define ALLOCMEMTAG_FLAG_USE					(0x04)				// �m�ہE�g�p����Ă��郁�������ǂ���
#define ALLOCMEMTAG_FLAG_NEXT_CONNECT			(0x08)				// Next �̃������̈�ƃA�h���X���A�����Ă��邩�ǂ���
#define ALLOCMEMTAG_FLAG_PREV_CONNECT			(0x10)				// Prev �̃������̈�ƃA�h���X���A�����Ă��邩�ǂ���

// �������m�ۏ����^�C�v
#define ALLOCMEMTYPE_SIMPLE						(0)					// �V���v���Œᑬ�ȃA���S���Y��
#define ALLOCMEMTYPE_TLSF						(1)					// TLSF

// �q�[�v�������m�ۃR�[���o�b�N�֐��Ŏg�p�ł�����̃T�C�Y
#define ALLOCHEAP_CALLBACK_INFO_SIZE					(32)

// �\���̒�` --------------------------------------------------------------------

// �m�ۃ������̃^�O���
struct ALLOCMEMTAG /* �T�C�Y�� ALLOCMEMTAG_SIZE byte �ɂȂ�悤�ɒ��� */
{
	DWORD					MagicID ;							// DMEM
	char					Name[ ALLOCMEMTAG_NAMELENGTH ] ;	// �t�@�C���p�X
	BYTE					Flag ;								// �t���O( ALLOCMEMTAG_FLAG_USE �Ȃ� )
	BYTE					Padding ;							// �\��
	BYTE					FLI ;								// �󂫃��������X�g�̃J�e�S���P( �^�C�v�� ALLOCMEMTYPE_TLSF �̍ۂɎg�p����� )
	BYTE					SLI ;								// �󂫃��������X�g�̃J�e�S���Q( �^�C�v�� ALLOCMEMTYPE_TLSF �̍ۂɎg�p����� )
	WORD					ID ;								// ID
	WORD					Time ;								// ����
	WORD					Line ;								// �s�ԍ�
	ALLOCMEM_SIZE_TYPE		Size ;								// ��L���Ă���T�C�Y
	void *					AllocAddress ;						// �m�ۂ��Ă��郁�����̈�̐擪�A�h���X( Owner->UseSeparateInfo = TRUE �̏ꍇ�̂ݎg�p )
	void *					UserAddress ;						// AllocMemory �̖߂�l�Ƃ��ĕԂ����A�h���X
	ALLOCMEM_SIZE_TYPE		UserSize ;							// AllocMemory �Ŏw�肳�ꂽ�T�C�Y
	ALLOCMEMTAG *			Prev ;								// ��O�̃^�O���ւ̃|�C���^( ��[�̏ꍇ�� NULL )
	ALLOCMEMTAG *			Next ;								// ���̃^�O���ւ̃|�C���^( �I�[�̏ꍇ�� NULL )
	ALLOCMEMTAG *			ListPrev ;							// ���X�g�̈�O�̃^�O���ւ̃|�C���^( ��[�̏ꍇ�� NULL )
	ALLOCMEMTAG *			ListNext ;							// ���X�g�̈��̃^�O���ւ̃|�C���^( �I�[�̏ꍇ�� NULL )
	struct HEAPINFO *		Owner ;								// �������^�O�������Ă���q�[�v���ւ̃|�C���^
	struct HEAPMEMINFO *	UseHeapMem ;						// �g�p���Ă��郁�����̈�ւ̃|�C���^
} ;

// �q�[�v�p�Ɋm�ۂ��ꂽ�������̏��
struct HEAPMEMINFO
{
	void *					Address ;							// �g�p�\�Ƃ��ēn���ꂽ�������̈�̐擪�A�h���X
	ALLOCMEM_SIZE_TYPE		Size ;								// �g�p�\�Ƃ��ēn���ꂽ�������̃T�C�Y
	void *					UseAddress ;						// �A���C�����l�������ꍇ�Ɏg�p�ł���擪�A�h���X
	ALLOCMEM_SIZE_TYPE		UseSize ;							// �A���C�����l�������ꍇ�Ɏg�p�ł��郁�����̃T�C�Y
	void *					HeapAddress ;						// �q�[�v�p�Ɏg�p�ł��郁�����̐擪�A�h���X
	ALLOCMEM_SIZE_TYPE		HeapSize ;							// �q�[�v�p�Ɏg�p�ł��郁�����̃T�C�Y
	ALLOCMEM_SIZE_TYPE		UseMemTagCount ;					// ���̃��������g�p���Ă���g�p�������^�O�̐�
	ALLOCMEMTAG *			FirstMemTag ;						// ���̃��������g�p���Ă���ŏ��̃������^�O�ւ̃|�C���^

	HEAPMEMINFO *			Prev ;								// ��O�̃q�[�v�p�Ɋm�ۂ��ꂽ�������̏��ւ̃|�C���^( ��[�̏ꍇ�� NULL )
	HEAPMEMINFO *			Next ;								// ���̃q�[�v�p�Ɋm�ۂ��ꂽ�������̏��ւ̃|�C���^( �I�[�̏ꍇ�� NULL )

	DWORD					Flag ;								// �t���O( HEAPMEM_FLAG_AUTOALLOC �Ȃ� )

	BYTE					CallbackBuffer[ ALLOCHEAP_CALLBACK_INFO_SIZE ] ;	// �R�[���o�b�N�֐����Ŏ��R�Ɏg�p�ł���o�b�t�@
} ;

// �q�[�v���
struct HEAPINFO
{
	int						AutoAlloc ;							// �q�[�v�p�̃������̈�������Ŋm�ۂ��邩�ǂ���( TRUE:�����Ŋm�ۂ���  FALSE:�����ł͊m�ۂ��Ȃ� )
	ALLOCMEM_SIZE_TYPE		AutoAllocUnitSize ;					// �q�[�v�p�̃������̈�������Ŋm�ۂ���ꍇ�́A��x�Ɋm�ۂ���q�[�v�̈�̃T�C�Y
	int						AutoAlloc_CallbackParam ;			// InitializeHeap �̈��� AutoAlloc_CallbackParam �̒l
	int					 ( *AutoAlloc_CreateHeapCallback     )( int Param, void *Buffer, ALLOCMEM_SIZE_TYPE Size ) ;	// �q�[�v�p�̃������̈�������Ŋm�ۂ��邽�߂̃R�[���o�b�N�֐�
	void *				 ( *AutoAlloc_GetHeapAddressCallback )( int Param, void *Buffer ) ;	// �q�[�v�p�̃������̈�������Ŋm�ۂ����ꍇ�́A�������A�h���X���擾���邽�߂̊֐�
	ALLOCMEM_SIZE_TYPE	 ( *AutoAlloc_GetHeapSizeCallback    )( int Param, void *Buffer ) ;	// �q�[�v�p�̃������̈�������Ŋm�ۂ����ꍇ�́A�������T�C�Y���擾���邽�߂̊֐�
	int					 ( *AutoAlloc_DeleteHeapCallback     )( int Param, void *Buffer ) ;	// �q�[�v�p�̃������̈�������Ŋm�ۂ����ꍇ�́A�������̈���J�����邽�߂̊֐�

	int						AllocType ;							// �������m�ۏ����^�C�v( ALLOCMEMTYPE_SIMPLE �� )
	ALLOCMEM_SIZE_TYPE		Aligned ;							// �Ԃ��������A�h���X�̃A���C��

	WORD					AllocMemoryID ;						// �m�ۂ����������Ɋ��蓖�Ă�h�c
	WORD					AllocMemoryTime ;					// �m�ۂ����������ɐݒ肷�鎞��

	int						UseSeparateInfo ;					// �������^�O��q�[�v�������̏����q�[�v�ȊO�̗̈�Ɋm�ۂ����z����g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )
	int						SepTagArrayLength ;					// �������^�O���q�[�v�ȊO�̗̈�Ɋm�ۂ����������^�O�z��̒���
	int						SepTagArrayUseNum ;					// �������^�O���q�[�v�ȊO�̗̈�Ɋm�ۂ����������^�O�z��̓��̎g�p���Ă��鐔
	ALLOCMEMTAG *			SepTagArray ;						// �������^�O���q�[�v�ȊO�̗̈�Ɋm�ۂ����������^�O�z��ւ̃|�C���^
	ALLOCMEMTAG *			SepTagArrayNotUseFirst ;			// �������^�O���q�[�v�ȊO�̗̈�Ɋm�ۂ����������^�O�z��̂܂����g�p�̃������^�O�̃��X�g�̐擪�v�f�ւ̃|�C���^
	int						SepHeapMemArrayLength ;				// �q�[�v�p�Ɋm�ۂ��ꂽ�������̏����q�[�v�ȊO�̗̈�Ɋm�ۂ������z��̒���
	int						SepHeapMemArrayUseNum ;				// �q�[�v�p�Ɋm�ۂ��ꂽ�������̏����q�[�v�ȊO�̗̈�Ɋm�ۂ������z��̓��̎g�p���Ă��鐔
	HEAPMEMINFO *			SepHeapMemArray ;					// �q�[�v�p�Ɋm�ۂ��ꂽ�������̏����q�[�v�ȊO�̗̈�Ɋm�ۂ������z��ւ̃|�C���^
	HEAPMEMINFO *			SepHeapMemArrayNotUseFirst ;		// �q�[�v�p�Ɋm�ۂ��ꂽ�������̏����q�[�v�ȊO�̗̈�Ɋm�ۂ������z��̂܂����g�p�̏��̃��X�g�̐擪�v�f�ւ̃|�C���^

	HEAPMEMINFO *			FirstHeapMem ;						// �q�[�v�p�Ɋm�ۂ��ꂽ�������̏��̃��X�g�̍ŏ��̏��ւ̃|�C���^

	int						FreeTagNum ;						// �󂫃������^�O�̐�
	ALLOCMEM_SIZE_TYPE		TotalFreeSize ;						// �󂫃������̍��v�T�C�Y

	ALLOCMEMTAG *			FirstTag ;							// �ŏ��̃^�O�ւ̃|�C���^
	ALLOCMEMTAG *			LastTag ;							// �Ō�̃^�O�ւ̃|�C���^

	int						UseTagNum ;							// �g�p����Ă��郁�����̐�
	ALLOCMEM_SIZE_TYPE		TotalUseSize ;						// �g�p����Ă��郁�����̍��v�T�C�Y
	ALLOCMEMTAG *			UseFirstTag ;						// �g�p�������̍ŏ��̃^�O�ւ̃|�C���^
	ALLOCMEMTAG *			UseLastTag ;						// �g�p�������̍Ō�̃^�O�ւ̃|�C���^


#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )
	DX_CRITICAL_SECTION		MemoryAllocCriticalSection ;		// �������m�ۏ����p�N���e�B�J���Z�N�V����
#endif

	// ALLOCMEMTYPE_TLSF �^�C�v�ł̂ݎg�p
#ifdef __64BIT__
	ULONGLONG				FreeTagBitList1 ;					// �J�e�S���P�̋󂫃�����������J�e�S���̃r�b�g���X�g
#else // __64BIT__
	DWORD					FreeTagBitList1 ;					// �J�e�S���P�̋󂫃�����������J�e�S���̃r�b�g���X�g
#endif // __64BIT__
	DWORD					FreeTagBitList2[ ALLOCMEM_CATEGORY_1_NUM ] ;												// �e�J�e�S���P�̋󂫃�����������J�e�S���Q�̃r�b�g���X�g
	ALLOCMEMTAG *			FreeFirstTagArray[ ALLOCMEM_CATEGORY_1_NUM ][ 1 << ALLOCMEM_CATEGORY_2_SHIFT_NUM ] ;		// �e�J�e�S���̋󂫃������ŏ��̃^�O�ւ̃|�C���^
	ALLOCMEMTAG *			FreeLastTagArray[  ALLOCMEM_CATEGORY_1_NUM ][ 1 << ALLOCMEM_CATEGORY_2_SHIFT_NUM ] ;		// �e�J�e�S���̋󂫃������Ō�̃^�O�ւ̃|�C���^


	// ALLOCMEMTYPE_SIMPLE �^�C�v�ł̂ݎg�p
	ALLOCMEMTAG *			FreeFirstTag ;						// �󂫃������̍ŏ��̃^�O�ւ̃|�C���^
	ALLOCMEMTAG *			FreeLastTag ;						// �󂫃������̍Ō�̃^�O�ւ̃|�C���^
} ;

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �q�[�v������������
extern	int			InitializeHeap(
						HEAPINFO *				Heap,
						int						AllocType				= ALLOCMEMTYPE_SIMPLE,
						ALLOCMEM_SIZE_TYPE		Aligned					= 16,
						int						AutoAlloc				= TRUE,
						ALLOCMEM_SIZE_TYPE		AutoAllocUnitSize		= 16 * 1024 * 1024,
						int						AutoAlloc_CallbackParam	= 0,
						int					 ( *AutoAlloc_CreateHeapCallback     )( int Param, void *Buffer, ALLOCMEM_SIZE_TYPE Size ) = NULL,
						void *				 ( *AutoAlloc_GetHeapAddressCallback )( int Param, void *Buffer ) = NULL,
						ALLOCMEM_SIZE_TYPE	 ( *AutoAlloc_GetHeapSizeCallback    )( int Param, void *Buffer ) = NULL,
						int					 ( *AutoAlloc_DeleteHeapCallback     )( int Param, void *Buffer ) = NULL,
						void *					HeapAddress				= NULL,
						ALLOCMEM_SIZE_TYPE		HeapSize				= 0,
						int						UseSeparateInfo			= FALSE,
						int						SepTagArrayLength		= 0,
						ALLOCMEMTAG *			SepTagArray				= NULL,
						int						SepHeapMemArrayLength	= 0,
						HEAPMEMINFO *			SepHeapMemArray			= NULL
					) ;
extern	int			TerminateHeap( HEAPINFO *Heap ) ;				// �q�[�v�̌�n�����s��
extern	int			AddHeapMemory( HEAPINFO *Heap, void *HeapAddress, ALLOCMEM_SIZE_TYPE HeapSize ) ;	// �q�[�v�Ƃ��Ĉ�����������ǉ�����
extern	int			SubHeapMemory( HEAPINFO *Heap, void *HeapAddress ) ;								// �q�[�v�Ƃ��Ĉ��������������炷
extern	ALLOCMEM_SIZE_TYPE	GetHeapTotalMemorySize( HEAPINFO *Heap ) ;	// �q�[�v���g�p���Ă��郁�����̑��T�C�Y���擾����

// �q�[�v���烁�������m�ۂ���( UseSeparateInfo=TRUE �̏ꍇ�́A�߂��Ă���̂� ALLOCMEMTAG �\���̂̃A�h���X )
extern	void *		AllocMemory(
						HEAPINFO *			Heap,
						ALLOCMEM_SIZE_TYPE	Size,
						ALLOCMEM_SIZE_TYPE	Aligned			= 0,
						int					Reverse			= FALSE,
						const char *		Name			= NULL,
						int					Line			= 0
					) ;
// �m�ۂ����������̃T�C�Y��ύX����( UseSeparateInfo=TRUE �̏ꍇ�́AAllocAddress �� ALLOCMEMTAG �\���̂̃A�h���X��n�� )
extern	void *		ReallocMemory(
						void *				AllocAddress,
						int					UseSeparateInfo	= FALSE,
						ALLOCMEM_SIZE_TYPE	Size			= 0,
						ALLOCMEM_SIZE_TYPE	Aligned			= 0,
						const char *		Name			= NULL,
						int					Line			= 0
					) ;
extern	int			FreeMemory(            void *AllocAddress, int UseSeparateInfo = FALSE ) ;	// �q�[�v����m�ۂ������������J������( UseSeparateInfo = TRUE �̏ꍇ�́AAllocAddress �� ALLOCMEMTAG �\���̂̃A�h���X��n�� )
extern	void *		GetAllocMemoryAddress( void *AllocAddress, int UseSeparateInfo = FALSE ) ;	// �q�[�v����m�ۂ����������̈�̃A�h���X���擾����( UseSeparateInfo = TRUE �̏ꍇ�́AAllocAddress �� ALLOCMEMTAG �\���̂̃A�h���X��n�� )
extern	ALLOCMEM_SIZE_TYPE	GetAllocSize(  void *AllocAddress, int UseSeparateInfo = FALSE ) ;	// �q�[�v����m�ۂ����������̃T�C�Y���擾����( UseSeparateInfo = TRUE �̏ꍇ�́AAllocAddress �� ALLOCMEMTAG �\���̂̃A�h���X��n�� )
extern	ALLOCMEMTAG * GetAllocMemTag(      void *AllocAddress, int UseSeparateInfo = FALSE ) ;	// �q�[�v����m�ۂ����������� ALLOCMEMTAG�\���̂��擾����( UseSeparateInfo = TRUE �̏ꍇ�́AAllocAddress �� ALLOCMEMTAG �\���̂̃A�h���X��n�� )
extern	void		PrintInfoMemory(       void *AllocAddress, int UseSeparateInfo = FALSE ) ;	// �q�[�v����m�ۂ����������̏������O�ɏo�͂���( UseSeparateInfo = TRUE �̏ꍇ�́AAllocAddress �� ALLOCMEMTAG �\���̂̃A�h���X��n�� )
extern	int			HeapInfoDump(      HEAPINFO *Heap ) ;										// �q�[�v�̏������O�ɏo�͂���
extern	int			SetHeapTime(       HEAPINFO *Heap, WORD Time ) ;							// �q�[�v����m�ۂ����������ɐݒ肷�鎞�Ԃ�ύX����
extern	int			HeapErrorCheck(    HEAPINFO *Heap ) ;										// �q�[�v�̓��e�ɃG���[���Ȃ����`�F�b�N����
extern	int			HeapDrawAllocInfo( HEAPINFO *Heap, int x, int y, int Width, int Height ) ;	// �q�[�v�̊m�ۏ󋵂�`�悷��





#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXMEMORY_H__
