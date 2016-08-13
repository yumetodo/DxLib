// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�����O�o�b�t�@�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXRINGBUFFER_H__
#define __DXRINGBUFFER_H__

// �C���N���[�h ------------------------------------------------------------------

#include "DxCompileConfig.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

// �����O�o�b�t�@�f�[�^�\����
struct RINGBUF
{
	char					*DataBuffer ;						// �f�[�^�̈�̃|�C���^
	int						Start, End ;						// �L���f�[�^�̎n�܂�ƏI��
	int						DataLength ;						// �L���ȃf�[�^�̒���
	int						BufferSize ;						// �����O�o�b�t�@�̑傫��
} ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �����O�o�b�t�@
extern	int			RingBufInitialize( RINGBUF * RingBuf ) ;									// �����O�o�b�t�@������������
extern	int			RingBufTerminate( RINGBUF * RingBuf ) ;										// �����O�o�b�t�@�̌�n��������
extern	int			RingBufDataAdd( RINGBUF * RingBuf, const void *Data, int Len ) ;			// �����O�o�b�t�@�Ƀf�[�^��ǉ�����
extern	int			RingBufDataGet( RINGBUF * RingBuf, void *Buf, int Len, int PeekFlag) ;		// �����O�o�b�t�@����f�[�^��ǂ݂���
extern	int			RingBufGetDataLength( RINGBUF * RingBuf ) ;									// �����O�o�b�t�@�̗L���f�[�^�ʂ̎擾 
extern	int			RingBufGetRestDataLength( RINGBUF * RingBuf ) ;								// �����O�o�b�t�@�̎c��󂫃f�[�^�̈���擾
extern	int			RingBufReSize( RINGBUF * RingBuf, int AddSize ) ;							// �����O�o�b�t�@�̗e�ʂ��w��ʂ̃f�[�^��ǉ����Ă����܂�T�C�Y�ɒ�������

// �����O�o�b�t�@�̃A�h���X�ɕs�����Ȃ����`�F�b�N����
__inline void RingBufAddressCheck( RINGBUF * RingBuf )
{
	if( RingBuf->Start == RingBuf->BufferSize ) RingBuf->Start = 0 ;
	if( RingBuf->End == RingBuf->BufferSize ) RingBuf->End = 0 ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXRINGBUFFER_H__

