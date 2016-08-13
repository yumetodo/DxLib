//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Windows�p�������֌W�v���O����
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

// �C���N���[�h ---------------------------------------------------------------
#include "../DxMemory.h"
#include <windows.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------------------------------------

// �\���̐錾 -----------------------------------------------------------------

// �f�[�^��` -----------------------------------------------------------------

// �֐��錾 -------------------------------------------------------------------

// �v���O���� -----------------------------------------------------------------

// �����������̏��������s���֐��̊��ˑ�����
extern int MemoryInitialize_PF( void )
{
	return 0 ;
}

// �����������̌�n�����s���֐��̊��ˑ�����
extern int MemoryTerminate_PF( void )
{
	return 0 ;
}

// �����������̎����I�����̊��ˑ�����
extern int MemoryProcess_PF( void )
{
	return 0 ;
}

// ���ˑ��̈�ʓI�ȃq�[�v�p�̃������̈���m�ۂ��邽�߂̃R�[���o�b�N�֐��Q�� AutoAlloc_CreateHeapCallback
extern int NormalMemory_AutoAlloc_CreateHeapCallback_PF( int Param, void *Buffer, ALLOCMEM_SIZE_TYPE Size )
{
	MEMORY_BASIC_INFORMATION	MemoryBasicInfo ;
	PLATFORMHEAP_WIN *			PlatformHeap = ( PLATFORMHEAP_WIN * )Buffer ;
	SIZE_T						Result ;

	// �o�b�t�@�T�C�Y�`�F�b�N
	if( sizeof( PLATFORMHEAP_WIN ) > ALLOCHEAP_CALLBACK_INFO_SIZE )
	{
		*( ( DWORD * )0x00000000 ) = 0xffffffff ;
	}

	// ���������m��
	PlatformHeap->AllocAddress = VirtualAlloc( NULL, Size, MEM_COMMIT, PAGE_READWRITE ) ;
	if( PlatformHeap->AllocAddress == NULL )
	{
		return -1 ;
	}

	// �m�ۂ����������̃T�C�Y���擾
	Result = VirtualQuery( PlatformHeap->AllocAddress, &MemoryBasicInfo, sizeof( MemoryBasicInfo ) ) ;
	if( Result == 0 )
	{
		VirtualFree( PlatformHeap->AllocAddress, 0, MEM_RELEASE ) ;
		return -1 ;
	}
	PlatformHeap->AllocSize = MemoryBasicInfo.RegionSize ;

	return 0 ;
}

// ���ˑ��̈�ʓI�ȃq�[�v�p�̃������̈���m�ۂ��邽�߂̃R�[���o�b�N�֐��Q�� AutoAlloc_GetHeapAddressCallback
extern void * NormalMemory_AutoAlloc_GetHeapAddressCallback_PF( int Param, void *Buffer )
{
	PLATFORMHEAP_WIN *			PlatformHeap = ( PLATFORMHEAP_WIN * )Buffer ;

	return PlatformHeap->AllocAddress ;
}

// ���ˑ��̈�ʓI�ȃq�[�v�p�̃������̈���m�ۂ��邽�߂̃R�[���o�b�N�֐��Q�� AutoAlloc_GetHeapSizeCallback
extern	ALLOCMEM_SIZE_TYPE	 NormalMemory_AutoAlloc_GetHeapSizeCallback_PF( int Param, void *Buffer )
{
	PLATFORMHEAP_WIN *			PlatformHeap = ( PLATFORMHEAP_WIN * )Buffer ;

	return ( ALLOCMEM_SIZE_TYPE )PlatformHeap->AllocSize ;
}

// ���ˑ��̈�ʓI�ȃq�[�v�p�̃������̈���m�ۂ��邽�߂̃R�[���o�b�N�֐��Q�� AutoAlloc_DeleteHeapCallback
extern int  NormalMemory_AutoAlloc_DeleteHeapCallback_PF( int Param, void *Buffer )
{
	PLATFORMHEAP_WIN *			PlatformHeap = ( PLATFORMHEAP_WIN * )Buffer ;

	// �����������
	VirtualFree( PlatformHeap->AllocAddress, 0, MEM_RELEASE ) ;

	return 0 ;
}

// �m�ۂ��Ă��郁������񋓂���֐��̊��ˑ�����
extern int DxDumpAlloc_PF( void )
{
	return 0 ;
}


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

