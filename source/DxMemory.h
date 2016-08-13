// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�������֌W�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMEMORY_H__
#define __DXMEMORY_H__

// �C���N���[�h ------------------------------------------------------------------
#include "DxCompileConfig.h"
#include "DxThread.h"
#include "DxHeap.h"

#ifdef __WINDOWS__
#include "Windows/DxMemoryWin.h"
#endif // __WINDOWS__

#ifdef __psp2__
#include "PSVita/DxMemoryPSVita.h"
#endif // __psp2__

#ifdef __ORBIS__
#include "PS4/DxMemoryPS4.h"
#endif // __ORBIS__

#ifdef __ANDROID__
#include "Android/DxMemoryAndroid.h"
#endif // __ANDROID__

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �������m�یn�֐�
#if !defined( __BCC ) || defined( _DEBUG )
	#define DXALLOCBASE( size, aligned )	DxAllocBase( (size), (aligned), __FILE__, __LINE__ )
	#define DXALLOC( size )					NS_DxAlloc( (size), __FILE__, __LINE__ )
	#define DXCALLOC( size )				NS_DxCalloc( (size), __FILE__, __LINE__ )
	#define DXREALLOC( mem, size )			NS_DxRealloc( (mem), (size), __FILE__, __LINE__ )
	#define DXFREE( mem )					NS_DxFree( (mem) )
#else
	#define DXALLOCBASE( size, aligned )	DxAllocBase( (size), (aligned), "", 0 )
	#define DXALLOC( size )					NS_DxAlloc( (size), "", 0 )
	#define DXCALLOC( size )				NS_DxCalloc( (size), "", 0 )
	#define DXREALLOC( mem, size )			NS_DxRealloc( (mem), (size), "", 0 )
	#define DXFREE( mem )					NS_DxFree( (mem) )
#endif



// �\���̒�` --------------------------------------------------------------------

// �������֌W�̏��\����
struct MEMORYDATA
{
	size_t					AllocMemorySize ;					// �m�ۂ��Ă��郁�����̑���
	int						AllocMemoryNum ;					// ���C�u�����Ŋm�ۂ����������̐�
	int						AllocMemoryPrintFlag ;				// �m�ۂ���鎞�ɏ����o�͂��邩�ǂ����̃t���O
	int						AllocMemorySizeOutFlag ;			// �m�ۂ������s����x�Ɋm�ۃ������̑��ʂ����O�ɏo�͂��邩�ǂ����̃t���O
	int						AllocMemoryErrorCheckFlag ;			// �m�ۂ������s����x�Ɋm�ۃ������̏�񂪔j�����Ă��Ȃ������`�F�b�N���邩�ǂ����̃t���O
	size_t					AllocTrapSize ;						// �񋓑Ώۂɂ���m�ۃT�C�Y

	int						InitializeFlag ;					// �������t���O
	int						StartTime ;							// �J�E���g�J�n����
	HEAPINFO				SmallHeap ;							// �������T�C�Y�̃������m�ۗp�q�[�v
	HEAPINFO				BigHeap ;							// �傫���T�C�Y�̃������m�ۗp�q�[�v

#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )
	int						InitMemoryAllocCriticalSection ;	// MemoryAllocCriticalSection �̏��������ς�ł��邩�ǂ����̃t���O
	DX_CRITICAL_SECTION		MemoryAllocCriticalSection ;		// �������m�ۏ����p�N���e�B�J���Z�N�V����
#endif
} ;

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �������֌W�̏��
extern MEMORYDATA MemData ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

extern	int			MemoryInitialize( void ) ;												// �����������̏��������s��
extern	int			MemoryTerminate( void ) ;												// �����������̌�n�����s��
extern	int			MemoryProcess( void ) ;													// �����������̎����I����
extern	int			MemoryDrawAllocInfo( int x, int y, int Width, int Height ) ;			// �������̊m�ۏ󋵂�`�悷��

extern	void		MemoryDump( void *buffer, int size ) ;

extern	void *		DxAllocBase( size_t AllocSize, size_t Aligned, const char *File = NULL, int Line = -1 ) ;						// DxAlloc �̓��������p�֐�
extern	void *		DxReallocBase( void *Memory, size_t AllocSize, size_t Aligned, const char *File = NULL, int Line = -1 ) ;		// DxRealloc �̓��������p�֐�


// ���ˑ��֐�
extern	int			MemoryInitialize_PF( void ) ;								// �����������̏��������s���֐��̊��ˑ�����
extern	int			MemoryTerminate_PF( void ) ;								// �����������̌�n�����s���֐��̊��ˑ�����
extern	int			MemoryProcess_PF( void ) ;									// �����������̎����I�����̊��ˑ�����

extern	int					 NormalMemory_AutoAlloc_CreateHeapCallback_PF(     int Param, void *Buffer, ALLOCMEM_SIZE_TYPE Size ) ;		// ���ˑ��̈�ʓI�ȃq�[�v�p�̃������̈���m�ۂ��邽�߂̃R�[���o�b�N�֐��Q�� AutoAlloc_CreateHeapCallback
extern	void *				 NormalMemory_AutoAlloc_GetHeapAddressCallback_PF( int Param, void *Buffer ) ;			// ���ˑ��̈�ʓI�ȃq�[�v�p�̃������̈���m�ۂ��邽�߂̃R�[���o�b�N�֐��Q�� AutoAlloc_GetHeapAddressCallback
extern	ALLOCMEM_SIZE_TYPE	 NormalMemory_AutoAlloc_GetHeapSizeCallback_PF(    int Param, void *Buffer ) ;			// ���ˑ��̈�ʓI�ȃq�[�v�p�̃������̈���m�ۂ��邽�߂̃R�[���o�b�N�֐��Q�� AutoAlloc_GetHeapSizeCallback
extern	int					 NormalMemory_AutoAlloc_DeleteHeapCallback_PF(     int Param, void *Buffer ) ;			// ���ˑ��̈�ʓI�ȃq�[�v�p�̃������̈���m�ۂ��邽�߂̃R�[���o�b�N�֐��Q�� AutoAlloc_DeleteHeapCallback

extern	int			DxDumpAlloc_PF( void ) ;									// �m�ۂ��Ă��郁������񋓂���֐��̊��ˑ�����

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXMEMORY_H__
