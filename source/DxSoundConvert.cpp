// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�T�E���h�f�[�^�ϊ��v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "DxCompileConfig.h"

#ifndef DX_NON_SOUND

// �C���N���[�h ------------------------------------------------------------------
#include "DxSoundConvert.h"
#include "DxStatic.h"
#include "DxLog.h"

#include "DxBaseFunc.h"
#include "DxMemory.h"
#include "DxUseCLib.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

// �T�E���h�ϊ������S�̂Ŏg�p����f�[�^�\����
struct SOUNDCONVERTDATA
{
	int							InitializeFlag ;				// �������t���O
} ;

// �������ϐ��錾 --------------------------------------------------------------

// �T�E���h�ϊ������S�̂Ŏg�p����f�[�^
SOUNDCONVERTDATA GSoundConvertData ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

static	int CheckNormalWaveFormat( STREAMDATA *Stream ) ;											// �ʏ�̂o�b�l�`�������ׂ�( TRUE:�o�b�l�`��  FALSE:�Ⴄ )

static	int SetupSoundConvert_WAVE(            SOUNDCONV *SoundConv ) ;								// �v�`�u�d�t�@�C���̃Z�b�g�A�b�v�������s��( [��] -1:�G���[ )
static	int TerminateSoundConvert_WAVE(        SOUNDCONV *SoundConv ) ;								// �v�`�u�d�t�@�C���̌�n���������s��
static	int ConvertProcessSoundConvert_WAVE(   SOUNDCONV *SoundConv ) ;								// �ϊ���̃o�b�t�@�Ƀf�[�^���[����
//static	int SetTimeSoundConvert_WAVE(      SOUNDCONV *SoundConv, int Time ) ;					// �ϊ������̈ʒu��ύX����( �~���b�P�� )
static	int SetSampleTimeSoundConvert_WAVE(    SOUNDCONV *SoundConv, int SampleTime ) ;				// �ϊ������̈ʒu��ύX����( �T���v���P�� )
static	int GetSoundConvertDestSize_Fast_WAVE( SOUNDCONV *SoundConv ) ;								// �ϊ���̑�}�̃f�[�^�T�C�Y�𓾂�

// �v���O���� --------------------------------------------------------------------

// �������E�I���֐�

// �T�E���h�f�[�^�ϊ������̏��������s��
extern	int InitializeSoundConvert( void )
{
	// ���ɏ������ς݂̏ꍇ�͉������Ȃ�
	if( GSoundConvertData.InitializeFlag == TRUE )
		return -1 ;

	// ���ˑ�����
	if( InitializeSoundConvert_PF() < 0 )
	{
		return -1 ;
	}

	// �����������t���O�𗧂Ă�
	GSoundConvertData.InitializeFlag = TRUE ;

	// ����I��
	return 0 ;
}

// �T�E���h�f�[�^�ϊ������̏I���������s��
extern	int TerminateSoundConvert( void )
{
	// ���Ɍ�n�������ς݂̏ꍇ�͉������Ȃ�
	if( GSoundConvertData.InitializeFlag == FALSE )
		return -1 ;

	// ���ˑ�����
	if( TerminateSoundConvert_PF() < 0 )
	{
		return -1 ;
	}

	// �����������t���O��|��
	GSoundConvertData.InitializeFlag = FALSE ;

	// ����I��
	return 0 ;
}

// �ϊ������̃Z�b�g�A�b�v( [��] -1:�G���[ )
extern	int SetupSoundConvert( SOUNDCONV *SoundConv, STREAMDATA *Stream, int DisableReadSoundFunctionMask, int OggVorbisBitDepth, int OggVorbisFromTheoraFile )
{
	// ����
#ifndef DX_NON_OGGVORBIS
	SoundConv->OggVorbisBitDepth = OggVorbisBitDepth ;
	SoundConv->OggVorbisFromTheoraFile = OggVorbisFromTheoraFile ;
#endif // DX_NON_OGGVORBIS
	SoundConv->Stream = *Stream ;
	SoundConv->SeekLockPosition = 0 ;
	Stream->ReadShred.Seek( Stream->DataPoint, 0, SEEK_SET ) ;
	if( ( DisableReadSoundFunctionMask & DX_READSOUNDFUNCTION_PCM ) == 0 )
	{
		if( SetupSoundConvert_WAVE( SoundConv ) == 0 ) goto R1 ;
		Stream->ReadShred.Seek( Stream->DataPoint, 0, SEEK_SET ) ;
	}
#ifndef DX_NON_OGGVORBIS
	if( ( DisableReadSoundFunctionMask & DX_READSOUNDFUNCTION_OGG ) == 0 )
	{
		if( SetupSoundConvert_OGG( SoundConv ) == 0 ) goto R1 ;
		Stream->ReadShred.Seek( Stream->DataPoint, 0, SEEK_SET ) ;
	}
#endif // DX_NON_OGGVORBIS
#ifndef DX_NON_OPUS
	if( ( DisableReadSoundFunctionMask & DX_READSOUNDFUNCTION_OPUS ) == 0 )
	{
		if( SetupSoundConvert_OPUS( SoundConv ) == 0 ) goto R1 ;
		Stream->ReadShred.Seek( Stream->DataPoint, 0, SEEK_SET ) ;
	}
#endif // DX_NON_OPUS
	if( SetupSoundConvert_PF( SoundConv, Stream, DisableReadSoundFunctionMask ) == 0 )
	{
		goto R1 ;
	}
	return -1 ;
	
R1 :
	// ���ʏ������������s��
	SoundConv->InitializeFlag = FALSE ;
	SoundConv->EndFlag = FALSE ;
	SoundConv->DestData = DXALLOC( ( size_t )SoundConv->DestDataSize ) ;
	if( SoundConv->DestData == NULL )
	{
		TerminateSoundConvert( SoundConv ) ;
		return -1 ;
	}
	SoundConv->DestDataValidSize = 0 ;
	SoundConv->DestDataCompSize = 0 ;
	SoundConv->DestDataCompSizeAll = 0 ;

	// ����������
	SoundConv->InitializeFlag = TRUE ;

	// �I��
	return 0 ;
}

// �ϊ������̈ʒu��ύX����( �T���v���P�� )
extern int SetSampleTimeSoundConvert( SOUNDCONV *SoundConv, int SampleTime )
{
	int res = -1 ;
	int BytePos ;

	BytePos = SampleTime * SoundConv->OutFormat.nBlockAlign ;
	if( _ABS( BytePos - SoundConv->DestDataCompSizeAll ) < 16 ) return 0 ;

	switch( SoundConv->MethodType )
	{
	case SOUND_METHODTYPE_NORMAL : res = SetSampleTimeSoundConvert_WAVE( SoundConv, SampleTime ) ; break ;
#ifndef DX_NON_OGGVORBIS
	case SOUND_METHODTYPE_OGG : res = SetSampleTimeSoundConvert_OGG( SoundConv, SampleTime ) ; break ;
#endif
#ifndef DX_NON_OPUS
	case SOUND_METHODTYPE_OPUS : res = SetSampleTimeSoundConvert_OPUS( SoundConv, SampleTime ) ; break ;
#endif
	default: res = SetSampleTimeSoundConvert_PF( SoundConv, SampleTime ) ; break ;
	}
	SoundConv->EndFlag = FALSE ;
	return res ;
}

// �V�[�N�\��̈ʒu���Z�b�g���Ă���(�`�b�l�ׂ̈�)
extern	int SetupSeekPosSoundConvert( SOUNDCONV *SoundConv, int SeekPos )
{
	SoundConv->SeekLockPosition = SeekPos * SoundConv->OutFormat.nBlockAlign ;

	return 0 ;
}


// �w��̃T�C�Y��������������( [��] -1:�G���[  0�ȏ�:�ϊ������T�C�Y )
extern	int RunSoundConvert( SOUNDCONV *SoundConv, void *DestBuffer, int DestSize )
{
	int pos, movesize, res = -1, copysize ;
	
	if( SoundConv->InitializeFlag == FALSE ) return -1 ;
	if( SoundConv->EndFlag == TRUE ) return 0 ;
	
	pos = 0 ;
	copysize = 0 ;
	while( DestSize != 0 )
	{
		if( SoundConv->DestDataValidSize == 0 )
		{
			SoundConv->DestDataCompSize = 0 ;
			switch( SoundConv->MethodType )
			{
			case SOUND_METHODTYPE_NORMAL : res = ConvertProcessSoundConvert_WAVE( SoundConv ) ; break ;
#ifndef DX_NON_OGGVORBIS
			case SOUND_METHODTYPE_OGG : res = ConvertProcessSoundConvert_OGG( SoundConv ) ; break ;
#endif
#ifndef DX_NON_OPUS
			case SOUND_METHODTYPE_OPUS : res = ConvertProcessSoundConvert_OPUS( SoundConv ) ; break ;
#endif
			default : res = ConvertProcessSoundConvert_PF( SoundConv ) ; break ;
			}
			if( res == -1 ) break ;
		}
	
		movesize = DestSize ;
		if( movesize > SoundConv->DestDataValidSize ) movesize = SoundConv->DestDataValidSize ;
/*
		int i ;
		int tmp ;
		short *dst, *src ;

		for( i = 0 ; i < movesize / 4 ; i ++ )
		{
			dst = ( short * )( (BYTE *)DestBuffer + pos + i * 4 ) ;
			src = ( short * )( (BYTE *)SoundConv->DestData + SoundConv->DestDataCompSize + i * 4 ) ;

			tmp = src[ 0 ] * 3 / 2 ;
			if( tmp >  32767 ) tmp =  32767 ;
			if( tmp < -32768 ) tmp = -32768 ;
			dst[ 0 ] = ( short )tmp ;

//			NS_DrawPixel( i, ( tmp + 32768 ) / 256, GetColor( 255,255,255 ) ) ;

			tmp = src[ 1 ] * 3 / 2 ;
			if( tmp >  32767 ) tmp =  32767 ;
			if( tmp < -32768 ) tmp = -32768 ;
			dst[ 1 ] = ( short )tmp ;
		}
//		NS_ScreenFlip() ;
//		WaitKey() ;
*/
		_MEMCPY( (BYTE *)DestBuffer + pos,
				(BYTE *)SoundConv->DestData + SoundConv->DestDataCompSize,
				( size_t )movesize ) ;
		SoundConv->DestDataValidSize -= movesize ;
		SoundConv->DestDataCompSize += movesize ;
		SoundConv->DestDataCompSizeAll += movesize ;
		
		DestSize -= movesize ;
		pos += movesize ;
		copysize += movesize ;
	}
	if( DestSize != 0 )
	{
		SoundConv->EndFlag = TRUE ;
	}

	// �I��
	return copysize ;
}

// �ϊ������̌�n�����s��
extern	int TerminateSoundConvert( SOUNDCONV *SoundConv )
{
	if( SoundConv->InitializeFlag == FALSE ) return -1 ;
	
	if( SoundConv->DestData != NULL )
	{
		DXFREE( SoundConv->DestData ) ;
		SoundConv->DestData = NULL ;
	}
	
	switch( SoundConv->MethodType )
	{
	case SOUND_METHODTYPE_NORMAL : TerminateSoundConvert_WAVE( SoundConv ) ; break ;
#ifndef DX_NON_OGGVORBIS
	case SOUND_METHODTYPE_OGG : TerminateSoundConvert_OGG( SoundConv ) ; break ;
#endif
#ifndef DX_NON_OPUS
	case SOUND_METHODTYPE_OPUS : TerminateSoundConvert_OPUS( SoundConv ) ; break ;
#endif
	default : TerminateSoundConvert_PF( SoundConv ) ; break ;
	}

	SoundConv->InitializeFlag = FALSE ;
	
	// �I��
	return 0 ;	
}

// �ϊ���̉����`�����擾( [��] -1:�G���[  0�ȏ�:�ϊ���̃T�C�Y )
extern	int GetOutSoundFormatInfo( SOUNDCONV *SoundConv, WAVEFORMATEX *OutWaveFormat )
{
	*OutWaveFormat = SoundConv->OutFormat ;
	return 0 ;
}

// ���[�v�����擾( [��] -1:�G���[ )
extern int GetSoundConvertLoopAreaInfo( SOUNDCONV *SoundConv, int *LoopStartPos, int *LoopEndPos )
{
	if( LoopStartPos != NULL ) *LoopStartPos = -1 ;
	if( LoopEndPos   != NULL ) *LoopEndPos   = -1 ;

	switch( SoundConv->MethodType )
	{
#ifndef DX_NON_OGGVORBIS
	case SOUND_METHODTYPE_OGG : return GetSoundConvertLoopAreaInfo_OGG( SoundConv, LoopStartPos, LoopEndPos ) ;
#endif
	}
	return -1 ;
}

// �ϊ��������I�����Ă��邩�ǂ������擾����( [��] TRUE:�I������  FALSE:�܂��I�����Ă��Ȃ� )
extern	int GetSoundConvertEndState( SOUNDCONV *SoundConv )
{
	return SoundConv->EndFlag ;
}

// �ϊ���̑�}�̃f�[�^�T�C�Y�𓾂�
extern	int GetSoundConvertDestSize_Fast( SOUNDCONV *SoundConv )
{
	switch( SoundConv->MethodType )
	{
	case SOUND_METHODTYPE_NORMAL : return GetSoundConvertDestSize_Fast_WAVE( SoundConv ) ;
#ifndef DX_NON_OGGVORBIS
	case SOUND_METHODTYPE_OGG : return GetSoundConvertDestSize_Fast_OGG( SoundConv ) ;
#endif
#ifndef DX_NON_OPUS
	case SOUND_METHODTYPE_OPUS : return GetSoundConvertDestSize_Fast_OPUS( SoundConv ) ;
#endif
	default : return GetSoundConvertDestSize_Fast_PF( SoundConv ) ;
	}
	return 0 ;
}

//extern void *SoundBuf ;
//extern int SoundSize ;

// �����Ȉꊇ�ϊ�
extern	int SoundConvertFast( SOUNDCONV *SoundConv, 
									WAVEFORMATEX *FormatP, void **DestBufferP, int *DestSizeP )
{
	void *Buffer ;
	int BufferSize ;

	// �o�b�t�@�̊m��
	BufferSize = GetSoundConvertDestSize_Fast( SoundConv ) ;

	Buffer = DXALLOC( ( size_t )BufferSize ) ;
	if( Buffer == NULL ) return -1 ;

	// �ϊ��ʒu�̃Z�b�g
//	SetTimeSoundConvert( SoundConv, 0 ) ;
	SetSampleTimeSoundConvert( SoundConv, 0 ) ;

	// �f�[�^�`���������k�o�b�l�̏ꍇ�݂̂��̂܂܃R�s�[
	if( SoundConv->MethodType == SOUND_METHODTYPE_NORMAL &&
		( ( SOUNDCONV_WAVE * )SoundConv->ConvFunctionBuffer )->SrcFormat.wFormatTag == WAVE_FORMAT_PCM )
	{
		SoundConv->Stream.ReadShred.Read( Buffer, ( size_t )BufferSize, 1, SoundConv->Stream.DataPoint ) ;
	}
	else
	{
		// ����ȊO�̏ꍇ�͕��ʂɓW�J
		BufferSize = RunSoundConvert( SoundConv, Buffer, BufferSize ) ;
	}

	// ����ۑ�
	*FormatP = SoundConv->OutFormat ;
	*DestBufferP = Buffer ;
	*DestSizeP = BufferSize ;
	
	// �I��
	return 0 ;
}















// �t�H�[�}�b�g�ϊ��p

// �ʏ�̂o�b�l�`�������ׂ�( TRUE:�o�b�l�`��  FALSE:�Ⴄ )
extern	int CheckNormalWaveFormat( STREAMDATA *Stream )
{
	___RIFFCHUNK chunk ;
	char type[5] ;
	WAVEFORMAT format ;

	// RIFF�t�@�C�����ǂ������ׂ�
	Stream->ReadShred.Read( &chunk, sizeof( chunk ), 1, Stream->DataPoint ) ;
	if( chunk.fcc != RIFFCHUNKID( 'R', 'I', 'F', 'F' ) )
		return FALSE ;
		
	// WAVE�t�@�C�����ǂ������ׂ�
	type[4] = '\0' ;
	Stream->ReadShred.Read( type, 4, 1, Stream->DataPoint ) ;
	if( _STRCMP( type, "WAVE" ) != 0 )
		return FALSE ;
		
	// �m�[�}���Ȃo�b�l�`�������ׂ�
	Stream->ReadShred.Read( &chunk, sizeof( chunk ), 1, Stream->DataPoint ) ;
	if( chunk.fcc != RIFFCHUNKID( 'f', 'm', 't', ' ' ) )
		return FALSE ;

	Stream->ReadShred.Read( &format, sizeof( WAVEFORMAT ), 1, Stream->DataPoint ) ;
	if( format.wFormatTag != WAVE_FORMAT_PCM )
		return FALSE ;
		
	// �����܂ŗ�����ʏ�̃t�H�[�}�b�g�Ƃ�������
	return TRUE ;
}

// �v�`�u�d�t�@�C���̃Z�b�g�A�b�v�������s��( [��] -1:�G���[ )
static	int SetupSoundConvert_WAVE( SOUNDCONV *SoundConv )
{
	___RIFFCHUNK chunk ;
	char type[5] ;
	WAVEFORMATEX *format = NULL ;
	STREAMDATA *Stream = &SoundConv->Stream ;
	SOUNDCONV_WAVE *Wave = ( SOUNDCONV_WAVE * )SoundConv->ConvFunctionBuffer ;

	Wave->SrcBuffer = NULL ;

	// RIFF�t�@�C�����ǂ������ׂ�
	Stream->ReadShred.Read( &chunk, sizeof( chunk ), 1, Stream->DataPoint ) ;
	if( chunk.fcc != RIFFCHUNKID( 'R', 'I', 'F', 'F' ) )
		goto ERR ;

	// WAVE�t�@�C�����ǂ������ׂ�
	type[4] = '\0' ;
	Stream->ReadShred.Read( type, 4, 1, Stream->DataPoint ) ;
	if( _STRCMP( type, "WAVE" ) != 0 )
		goto ERR ;

	// �m�[�}���Ȃo�b�l�`�������ׂ�
	{
		// �t�H�[�}�b�g�`�����N������܂ŃX�L�b�v
		while( Stream->ReadShred.Eof( Stream->DataPoint ) == 0 )
		{
			Stream->ReadShred.Read( &chunk, sizeof( chunk ), 1, Stream->DataPoint ) ;
			if( chunk.fcc == RIFFCHUNKID( 'f', 'm', 't', ' ' ) )
			{
				break ;
			}
			Stream->ReadShred.Seek( Stream->DataPoint, chunk.cb + ( ( chunk.cb % 2 != 0 ) ? 1 : 0 ) , SEEK_CUR ) ;
		}
		if( Stream->ReadShred.Eof( Stream->DataPoint ) != 0 )
			goto ERR ;

		format = (WAVEFORMATEX *)DXALLOC( chunk.cb ) ;
		if( format == NULL ) goto ERR ;

		// �w�b�_�̈ʒu�ƃT�C�Y���Z�b�g
		SoundConv->HeaderPos = ( int )Stream->ReadShred.Tell( Stream->DataPoint ) ;
		SoundConv->HeaderSize = ( int )chunk.cb ;

		_MEMSET( SoundConv->ConvFunctionBuffer, 0, sizeof( SoundConv->ConvFunctionBuffer ) ) ;

		_MEMSET( &SoundConv->OutFormat, 0, sizeof( SoundConv->OutFormat ) ) ;
		Stream->ReadShred.Read( format, chunk.cb, 1, Stream->DataPoint ) ;
		switch( format->wFormatTag )
		{
		case WAVE_FORMAT_PCM :

			// �`���� WAVEFORMATEX �ł͂Ȃ������ꍇ�͕⊮����
			_MEMCPY( &SoundConv->OutFormat, format, sizeof( SoundConv->OutFormat ) < chunk.cb ? sizeof( SoundConv->OutFormat ) : chunk.cb ) ;
			if( chunk.cb == 16 )
			{
				SoundConv->OutFormat.wBitsPerSample = ( WORD )( format->nBlockAlign / format->nChannels * 8 ) ;
				SoundConv->OutFormat.cbSize = 0 ;
			}
			break ;


		case WAVE_FORMAT_IEEE_FLOAT :

			// �o�̓t�H�[�}�b�g���Z�b�g
			SoundConv->OutFormat.nSamplesPerSec  = format->nSamplesPerSec ;
			SoundConv->OutFormat.nChannels       = format->nChannels ;
			SoundConv->OutFormat.wBitsPerSample  = 16 ;
			SoundConv->OutFormat.wFormatTag      = WAVE_FORMAT_PCM ;
			SoundConv->OutFormat.nBlockAlign     = ( WORD )( SoundConv->OutFormat.wBitsPerSample  * SoundConv->OutFormat.nChannels / 8 ) ;
			SoundConv->OutFormat.nAvgBytesPerSec = SoundConv->OutFormat.nBlockAlign * SoundConv->OutFormat.nSamplesPerSec ;

			Wave->SrcSampleNum = SoundConv->OutFormat.nSamplesPerSec ;
			Wave->SrcBufferSize = format->nAvgBytesPerSec ;
			Wave->SrcBuffer = ( BYTE * )DXALLOC( Wave->SrcBufferSize ) ;
			if( Wave->SrcBuffer == NULL )
				goto ERR ;
			break ;


		default :
			goto ERR ;
		}

		// �t�H�[�}�b�g�̃R�s�[
		_MEMCPY( &Wave->SrcFormat, format, sizeof( Wave->SrcFormat ) < chunk.cb ? sizeof( Wave->SrcFormat ) : chunk.cb ) ;

		// �������̉��
		DXFREE( format ) ;
		format = NULL ;
	}

	// �f�[�^�`�����N��T��
	while( Stream->ReadShred.Eof( Stream->DataPoint ) == 0 )
	{
		Stream->ReadShred.Read( &chunk , sizeof( chunk ) , 1 , Stream->DataPoint ) ;
		if( chunk.fcc == RIFFCHUNKID( 'd', 'a', 't', 'a' ) ) break ;
		Stream->ReadShred.Seek( Stream->DataPoint, chunk.cb + ( ( chunk.cb % 2 != 0 ) ? 1 : 0 ) , SEEK_CUR ) ; 
	}
	if( Stream->ReadShred.Eof( Stream->DataPoint ) != 0 )
		goto ERR ;

	SoundConv->DataPos = ( int )Stream->ReadShred.Tell( Stream->DataPoint ) ;
	SoundConv->DataSize = ( int )chunk.cb ;

	// �ϊ���̂o�b�l�f�[�^���ꎞ�I�ɕۑ����郁�����̈�̃T�C�Y���Z�b�g
	SoundConv->DestDataSize = ( int )SoundConv->OutFormat.nAvgBytesPerSec ;

	// �^�C�v�Z�b�g
	SoundConv->MethodType = SOUND_METHODTYPE_NORMAL ;

	// ����I��
	return 0 ;

ERR :
	if( Wave->SrcBuffer != NULL )
	{
		DXFREE( Wave->SrcBuffer ) ;
		Wave->SrcBuffer = NULL ;
	}

	if( format != NULL )
	{
		DXFREE( format ) ;
		format = NULL ;
	}

	return -1 ;
}




// �v�`�u�d�t�@�C���̌�n���������s��
static	int TerminateSoundConvert_WAVE( SOUNDCONV *SoundConv )
{
	SOUNDCONV_WAVE *Wave = ( SOUNDCONV_WAVE * )SoundConv->ConvFunctionBuffer ;

	// ���������
	if( Wave->SrcBuffer )
	{
		DXFREE( Wave->SrcBuffer ) ;
		Wave->SrcBuffer = NULL ;
	}

	// �I��
	return 0 ;
}


// �ϊ���̃o�b�t�@�Ƀf�[�^���[����
static	int ConvertProcessSoundConvert_WAVE( SOUNDCONV *SoundConv )
{
	int readsize, pos ;
	STREAMDATA *Stream = &SoundConv->Stream ;
	SOUNDCONV_WAVE *Wave = ( SOUNDCONV_WAVE * )SoundConv->ConvFunctionBuffer ;

	switch( Wave->SrcFormat.wFormatTag )
	{
	case WAVE_FORMAT_PCM :
		pos = ( int )( Stream->ReadShred.Tell( Stream->DataPoint ) - SoundConv->DataPos ) ;
		if( pos == SoundConv->DataSize ) return -1 ;

		// �ǂݍ��ރf�[�^�T�C�Y�����肷��
		readsize = SoundConv->DataSize - pos ;
		if( SoundConv->DestDataSize < readsize ) readsize = SoundConv->DestDataSize ;

		// �ǂݍ���
		Stream->ReadShred.Read( SoundConv->DestData,
								( size_t )readsize, 1, Stream->DataPoint ) ;
		SoundConv->DestDataValidSize = readsize ;
		break ;

	case WAVE_FORMAT_IEEE_FLOAT :
		{
			short *Dest ;
			float *Src ;
			DWORD i ;
			DWORD SampleNum ;
			int j ;
			int DestI ;

			pos = ( int )( Stream->ReadShred.Tell( Stream->DataPoint ) - SoundConv->DataPos ) ;
			if( pos == SoundConv->DataSize ) return -1 ;

			// �ǂݍ��ރf�[�^�T�C�Y�����肷��
			readsize = SoundConv->DataSize - pos ;
			if( Wave->SrcBufferSize < ( DWORD )readsize ) readsize = ( int )Wave->SrcBufferSize ;

			// �ǂݍ���
			Stream->ReadShred.Read( Wave->SrcBuffer,
									( size_t )readsize, 1, Stream->DataPoint ) ;

			// �����^�ɕϊ�
			Dest = ( short * )SoundConv->DestData ;
			Src = ( float * )Wave->SrcBuffer ;
			SampleNum = ( DWORD )( readsize / Wave->SrcFormat.nBlockAlign ) ;
			for( i = 0 ; i < SampleNum ; i ++ )
			{
				for( j = 0 ; j < Wave->SrcFormat.nChannels ; j ++, Dest ++, Src ++ )
				{
					DestI = _FTOL( *Src * 32768.0f ) ;
					if( DestI < -32768 )
					{
						*Dest = -32768 ;
					}
					else
					if( DestI > 32767 )
					{
						*Dest = 32767 ;
					}
					else
					{
						*Dest = ( short )DestI ;
					}
				}
			}
			SoundConv->DestDataValidSize = ( int )( SampleNum * SoundConv->OutFormat.nBlockAlign ) ;
		}
		break ;
	}

	return 0 ;
}

// �ϊ������̈ʒu��ύX����( �T���v���P�� )
static int SetSampleTimeSoundConvert_WAVE( SOUNDCONV *SoundConv, int SampleTime )
{
	int BytePos ;
	STREAMDATA *Stream = &SoundConv->Stream ;
	
	BytePos = SampleTime * SoundConv->OutFormat.nBlockAlign + SoundConv->DataPos ;
	Stream->ReadShred.Seek( Stream->DataPoint, BytePos, SEEK_SET ) ;

	SoundConv->DestDataValidSize = 0 ;
	SoundConv->DestDataCompSize = 0 ;
	SoundConv->DestDataCompSizeAll = BytePos ;

	return 0 ;
}


// �ϊ���̑�}�̃f�[�^�T�C�Y�𓾂�
static	int GetSoundConvertDestSize_Fast_WAVE( SOUNDCONV *SoundConv )
{
	SOUNDCONV_WAVE *Wave = ( SOUNDCONV_WAVE * )SoundConv->ConvFunctionBuffer ;

	switch( Wave->SrcFormat.wFormatTag )
	{
	case WAVE_FORMAT_PCM :
		return SoundConv->DataSize ;

	case WAVE_FORMAT_IEEE_FLOAT :
		return SoundConv->DataSize / ( Wave->SrcFormat.wBitsPerSample / SoundConv->OutFormat.wBitsPerSample ) ;
	}

	return -1 ;
}



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE


#endif // DX_NON_SOUND

