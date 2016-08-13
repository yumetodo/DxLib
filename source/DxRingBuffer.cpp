// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�����O�o�b�t�@�v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

// �C���N���[�h ------------------------------------------------------------------
#include "DxRingBuffer.h"
#include "DxStatic.h"
#include "DxLib.h"
#include "DxBaseFunc.h"
#include "DxMemory.h"
#include "DxLog.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �v���O���� --------------------------------------------------------------------

// �����O�o�b�t�@������������
extern int RingBufInitialize( RINGBUF * RingBuf )
{
	// �f�[�^�̏�����
	_MEMSET( RingBuf, 0, sizeof( RINGBUF ) ) ;
	RingBuf->DataBuffer = NULL ;
	RingBuf->Start = RingBuf->End = 0 ;
	RingBuf->DataLength = 0 ;
	RingBuf->BufferSize = 0 ;
	
	// �I��
	return 0 ;
}

// �����O�o�b�t�@�̌�n��������
extern int RingBufTerminate( RINGBUF * RingBuf )
{
	// �f�[�^���m�ۂ���Ă�����������
	if( RingBuf->DataBuffer != NULL ) DXFREE( RingBuf->DataBuffer ) ;
	
	// �[��������
	_MEMSET( RingBuf, 0, sizeof( RINGBUF ) ) ;
	
	// �I��
	return 0 ;
}

// �����O�o�b�t�@�Ƀf�[�^��ǉ�����
extern int RingBufDataAdd( RINGBUF * RingBuf, const void *Data, int Len )
{
	// �A�h���X�`�F�b�N
	RingBufAddressCheck( RingBuf ) ;
	
	// �f�[�^�����܂�悤�Ƀ��T�C�Y
	if( RingBufReSize( RingBuf, Len ) == -1 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xea\x30\xf3\x30\xb0\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x78\x30\x6e\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\xfd\x8f\xa0\x52\x59\x30\x8b\x30\x77\x95\x55\x30\x20\x00\x25\x00\x64\x00\x20\x00\x62\x00\x79\x00\x74\x00\x65\x00\x29\x00\x00"/*@ L"�����O�o�b�t�@�ւ̃f�[�^�̒ǉ��Ɏ��s���܂���(�ǉ����钷�� %d byte)" @*/, Len )) ;
		return -1 ;
	}

	// �Q��ɕ����Ċi�[���Ȃ���΂Ȃ�Ȃ����ǂ����ŏ����𕪊�
	if( RingBuf->End + Len > RingBuf->BufferSize )
	{
		// �Q��ɕʂ��Ċi�[����ꍇ�̏���

		_MEMCPY( (BYTE *)RingBuf->DataBuffer + RingBuf->End, Data,                                                  ( size_t )(         RingBuf->BufferSize - RingBuf->End )   ) ;
		_MEMCPY( RingBuf->DataBuffer,                        (BYTE *)Data + ( RingBuf->BufferSize - RingBuf->End ), ( size_t )( Len - ( RingBuf->BufferSize - RingBuf->End ) ) ) ;
		
		RingBuf->End = Len - ( RingBuf->BufferSize - RingBuf->End ) ;
	}
	else
	{
		// �P��Ŋi�[����ꍇ�̏���
		
		_MEMCPY( (BYTE *)RingBuf->DataBuffer + RingBuf->End, Data, ( size_t )Len ) ;
		
		RingBuf->End += Len ;
	}
	
	// �i�[���Ă���f�[�^�̗ʂ𑝂₷
	RingBuf->DataLength += Len ;

	// �I��
	return 0 ;
}

// �����O�o�b�t�@����f�[�^��ǂ݂���
extern int RingBufDataGet( RINGBUF * RingBuf , void *Buf , int Len , int PeekFlag )
{
	// ���݂���f�[�^�ʂ�葽��������G���[
	if( RingBuf->DataLength < Len ) return -1 ;

	// �A�h���X�`�F�b�N
	RingBufAddressCheck( RingBuf ) ;

	// �Q��ɕʂ��Ȃ���΂����Ȃ����ǂ����ŏ����𕪊�
	if( RingBuf->Start + Len > RingBuf->BufferSize )
	{
		// �Q��ɕʂ���ꍇ�̏���
		_MEMCPY( Buf,                                                    (BYTE *)RingBuf->DataBuffer + RingBuf->Start, ( size_t )(         RingBuf->BufferSize - RingBuf->Start )   ) ;
		_MEMCPY( (BYTE *)Buf + ( RingBuf->BufferSize - RingBuf->Start ), RingBuf->DataBuffer,                          ( size_t )( Len - ( RingBuf->BufferSize - RingBuf->Start ) ) ) ;

		if( PeekFlag == FALSE ) RingBuf->Start = Len - ( RingBuf->BufferSize - RingBuf->Start ) ;
	}
	else
	{
		// �P��Ŋi�[����ꍇ�̏���
		_MEMCPY( Buf, (BYTE *)RingBuf->DataBuffer + RingBuf->Start, ( size_t )Len ) ;
		
		if( PeekFlag == FALSE )  RingBuf->Start += Len ;
	}

	// �f�[�^�̗ʂ����炷
	if( PeekFlag == FALSE )  RingBuf->DataLength -= Len ;

	// �I��
	return 0 ;
}

// �����O�o�b�t�@�̗L���f�[�^�ʂ̎擾 
extern int RingBufGetDataLength( RINGBUF * RingBuf )
{
	return RingBuf->DataLength ;
}

// �����O�o�b�t�@�̎c��󂫃f�[�^�̈�
extern int RingBufGetRestDataLength( RINGBUF * RingBuf )
{
	return RingBuf->BufferSize - RingBuf->DataLength ;
}

// �����O�o�b�t�@�̗e�ʂ��w��ʂ̃f�[�^��ǉ����Ă����܂�T�C�Y�ɒ�������
extern int RingBufReSize( RINGBUF * RingBuf, int AddSize )
{
	// �A�h���X�`�F�b�N
	RingBufAddressCheck( RingBuf ) ;

	// �i�[����f�[�^�����݂̃o�b�t�@�e�ʂł͓���Ȃ��ꍇ�̓o�b�t�@���Ċm�ۂ���
	if( RingBuf->BufferSize < AddSize + RingBuf->DataLength )
	{
		void *OldBuffer ;
		int OldSize ;

		OldBuffer = RingBuf->DataBuffer ;
		OldSize = RingBuf->BufferSize ;

		// �T�C�Y���Đݒ�A�o�b�t�@���Ċm��
		RingBuf->BufferSize = RingBuf->DataLength * 3 / 2 + AddSize + 1000 ;
		if( ( RingBuf->DataBuffer = ( char * )DXALLOC( ( size_t )RingBuf->BufferSize ) ) == NULL )
		{
			// �Ċm�ۂɎ��s��������Ƃ̃o�b�t�@��������ďI��
			DXFREE( OldBuffer ) ;
			_MEMSET( RingBuf, 0, sizeof( RINGBUF ) ) ;
			
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xea\x30\xf3\x30\xb0\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x8d\x51\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\xba\x78\xdd\x4f\x57\x30\x88\x30\x46\x30\x68\x30\x57\x30\x5f\x30\xb5\x30\xa4\x30\xba\x30\x20\x00\x25\x00\x64\x00\x20\x00\x62\x00\x79\x00\x74\x00\x65\x00\x29\x00\x00"/*@ L"�����O�o�b�t�@�̍Ċm�ۂɎ��s���܂���(�m�ۂ��悤�Ƃ����T�C�Y %d byte)" @*/, RingBuf->BufferSize )) ;
			return -1 ;
		}

		// �Ċm�ۂ����������̈�ɍ��܂ł̃o�b�t�@�̓��e���R�s�[����
		if( RingBuf->DataLength != 0 )
		{
			if( RingBuf->Start + RingBuf->DataLength > OldSize )
			{
				_MEMCPY( RingBuf->DataBuffer,                                          ( BYTE * )OldBuffer + RingBuf->Start, ( size_t )(                         OldSize - RingBuf->Start )   ) ;
				_MEMCPY( ( BYTE * )RingBuf->DataBuffer + ( OldSize - RingBuf->Start ), OldBuffer,                            ( size_t )( RingBuf->DataLength - ( OldSize - RingBuf->Start ) ) ) ;
			}
			else
			{
				_MEMCPY( RingBuf->DataBuffer, ( BYTE * )OldBuffer + RingBuf->Start, ( size_t )RingBuf->DataLength ) ;
			}
		}

		// �A�h���X���Đݒ�
		RingBuf->Start = 0 ;
		RingBuf->End = RingBuf->DataLength ;

		// ���X�g�p���Ă����������̈�����
		DXFREE( OldBuffer ) ;
	}
	
	// �I��
	return 0 ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

