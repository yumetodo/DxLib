// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�c�����������r������������v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "DxSound.h"

#ifndef DX_NON_SOUND

// �C���N���[�h ------------------------------------------------------------------
#include "DxFile.h"
#include "DxBaseFunc.h"
#include "DxSystem.h"
#include "DxMemory.h"
#include "DxUseCLib.h"
#include "DxASyncLoad.h"
#include "DxLog.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -------------------------------------

// �\�t�g�T�E���h�֌W�̒�`
#define SSND_PLAYER_SEC_DIVNUM				(256)		// �P�b�̕�����
#define SSND_PLAYER_STRM_BUFSEC				(256)		// �v���C���[�̃T�E���h�o�b�t�@�̃T�C�Y( 1 / SSND_PLAYER_SEC_DIVNUM�@�b )
#define SSND_PLAYER_STRM_ONECOPYSEC			(8)			// �v���C���[�̃T�E���h�o�b�t�@�̃T�C�Y( 1 / SSND_PLAYER_SEC_DIVNUM�@�b )
#define SSND_PLAYER_STRM_SAKICOPYSEC		(256 / 5)	// �v���C���[�̍Đ����ɉ����f�[�^���s�W�J���Ă����ő厞��( �P�ʂ� SSND_PLAYER_SEC_DIVNUM����1�b )
#define SSND_PLAYER_STRM_MINSAKICOPYSEC		(256 / 20)	// �v���C���[�̍Đ����ɉ����f�[�^���s�W�J���Ă����ŏ�����( �P�ʂ� SSND_PLAYER_SEC_DIVNUM����1�b )

// �X�g���[���֌W�̒�`
#define STS_DIVNUM							(256)		// �P�b�̕�����
#define STS_BUFSEC							(256)		// �X�g���[�����T�E���h�Đ����Ɋm�ۂ���T�E���h�o�b�t�@�̎���( �P�ʂ� STS_DIVNUM����1�b )
#define STS_ONECOPYSEC						(8)			// �X�g���[�����T�E���h�Đ����Ɉ�x�ɓ]������T�E���h�f�[�^�̒���( 1/STS_ONECOPYSEC �b )
#define STS_SAKICOPYSEC						(192)		// �X�g���[�����T�E���h�Đ����ɉ����f�[�^���s�W�J���Ă����ő厞��( �P�ʂ� STS_DIVNUM����1�b )
#define STS_MINSAKICOPYSEC					(96)		// �X�g���[�����T�E���h�Đ����ɉ����f�[�^���s�W�J���Ă����ŏ�����( �P�ʂ� STS_DIVNUM����1�b )
#define STS_CACHEBUFFERSEC					(8)			// �X�g���[�����T�E���h�Đ����ɁA�t�@�C������̃X�g���[���̂Ƃ��Ƀ�������Ɋm�ۂ���ꎞ�o�b�t�@�̒���( 1/STS_CACHEBUFFERSEC �b )

#define DSMP3ONEFILLSEC						(8)			// DirectShow ���g�����l�o�R�ϊ��̃t���O��ӂ肪�J�o�[���鎞��( 1/DSMP3ONEFILLSEC �b )

#define SOUNDSIZE( size, align )			( ( ( size ) + ( align - 1 ) ) / ( align ) * ( align ) )

// �L���v�`���ŕۑ�����T�E���h�̃T���v�����O���[�g
#define CAPTURE_SAMPLERATE					(44100)

// �W���v�`�u�d�t�@�C���̃w�b�_�̃T�C�Y�ƕW���v�`�u�d�t�@�C���� WAVEFORMATEX �\���̂̃T�C�Y
#define NORMALWAVE_HEADERSIZE				(46)
#define NORMALWAVE_FORMATSIZE				(18)


#define DX_DSBVOLUME_MIN					(-10000)
#define DX_DSBVOLUME_MAX					(0)


// �\���̒�` --------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �T�E���h�V�X�e���f�[�^
SOUNDSYSTEMDATA SoundSysData ;

// �l�h�c�h�Ǘ��p�f�[�^
MIDISYSTEMDATA MidiSystemData ;	

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

static  int GetSoundBuffer( int SoundHandle, SOUND * Sound, SOUNDBUFFER **BufferP = NULL, bool BufferGet = true ) ;	// �g�p�\�ȃT�E���h�o�b�t�@���擾����(-1:�T�E���h�o�b�t�@�̎g�p���𓾂��Ȃ�����)
static	int CreateSoundBuffer( WAVEFORMATEX *WaveFormat, DWORD BufferSize ,
									 int SoundType, int BufferNum,
									 int SoundHandle, int SrcSoundHandle = -1, int ASyncThread = FALSE ) ;			// �V�����T�E���h�o�b�t�@�̍쐬



// �⏕�n�֐�
static	int AdjustSoundDataBlock( int Length, SOUND * Sound ) ;										// �T�C�Y�␳

// �\�t�g�E�G�A�Ő��䂷��T�E���h�f�[�^�n���h���֌W
//static int AddSoftSoundData( int IsPlayer, int Channels, int BitsPerSample, int SamplesPerSec, int SampleNum ) ;	// �\�t�g�E�G�A�Ő��䂷��T�E���h�f�[�^�n���h���̒ǉ�
//static int SubSoftSoundData( int SoftSoundHandle, int CheckIsPlayer ) ;											// �\�t�g�E�G�A�Ő��䂷��T�E���h�f�[�^�n���h���̍폜
static	int _SoftSoundPlayerProcess( SOFTSOUND *SPlayer ) ;														// �\�t�g�E�G�A�Ő��䂷��T�E���h�f�[�^�v���C���[�̒������

// �⏕�n
extern	int CreateWaveFileImage( 	void **DestBufferP, int *DestBufferSizeP,
									const WAVEFORMATEX *Format, int FormatSize,
									const void *WaveData, int WaveDataSize ) ;											// �����t�H�[�}�b�g�Ɣg�`�C���[�W����v�`�u�d�t�@�C���C���[�W���쐬����
static	int FileFullRead( const wchar_t *FileName, void **BufferP, int *SizeP, int OutputFileOpenErrorLog = TRUE ) ;	// �t�@�C�����ۂ܂郁�����ɓǂݍ���
extern	int StreamFullRead( STREAMDATA *Stream, void **BufferP, int *SizeP ) ;											// �X�g���[���f�[�^���ۂ܂郁�����ɓǂݍ���
static	int SoundTypeChangeToStream( int SoundHandle ) ;																// �T�E���h�n���h���̃^�C�v���X�g���[���n���h���ł͂Ȃ��ꍇ�ɃX�g���[���n���h���ɕύX����
static	int MilliSecPositionToSamplePosition( int SamplesPerSec, int MilliSecTime ) ;									// �~���b�P�ʂ̐��l���T���v���P�ʂ̐��l�ɕϊ�����
static	int SamplePositionToMilliSecPosition( int SamplesPerSec, int SampleTime ) ;										// �T���v���P�ʂ̐��l���~���b�P�ʂ̐��l�ɕϊ�����
static	int _PlaySetupSoundMem( SOUND * Sound, int TopPositionFlag ) ;													// �T�E���h�n���h���̍Đ��������s��( -1:�G���[ 0:����I�� 1:�Đ�����K�v�Ȃ� )
static	int	_PlaySetupStreamSoundMem( int SoundHandle, SOUND * Sound, int PlayType, int TopPositionFlag, SOUNDBUFFER **DestSBuffer ) ;	// �X�g���[���T�E���h�n���h���̍Đ��������s��( -1:�G���[ 0:����I�� 1:�Đ�����K�v�Ȃ� )
static	int _CreateSoundHandle( int Is3DSound, int NotInitSoundMemDelete, int ASyncThread ) ;									// �T�E���h�n���h�����쐬����
static	int SetupSoundPitchRateTimeStretchRateChangeHandle( int SoundHandle, LOADSOUND_GPARAM *GParam, int SSSHandleI, int BufferNum, int ASyncThread ) ;		// �s�b�`�ύX��^�C���X�g���b�`���̐L�k�ύX�T�E���h�n���h�����Z�b�g�A�b�v����w���p�[�֐�



// �v���O���� --------------------------------------------------------------------
/*
static __inline void TableSin( int Angle, float * RST Sin )
{
	*Sin = SoundSysData.SinTable[ Angle & ( SOUND_SINTABLE_DIV - 1 ) ] ;
}

static __inline void TableCos( float Angle, float * RST Sin, float * RST Cos )
{
	if( Angle > 10.0f || Angle < -10.0f )
	{
		_SINCOS_PLATFORM( Angle, Sin, Cos ) ;
	}
	else
	{
		Angle = Angle * ( MV1_SINTABLE_DIV / ( ( float )DX_PI * 2 ) ) + 12582912.0f ;
		*Sin = MV1Man.SinTable[ *( ( DWORD * )&Angle ) & ( MV1_SINTABLE_DIV - 1 ) ] ;
		*Cos = MV1Man.SinTable[ ( *( ( DWORD * )&Angle ) + MV1_SINTABLE_DIV / 4 ) & ( MV1_SINTABLE_DIV - 1 ) ] ;
	}
}
*/
// �T�E���h�V�X�e��������������
extern int InitializeSoundSystem( void )
{
	if( SoundSysData.InitializeFlag )
	{
		return -1 ;
	}

	// �o�̓��[�g���Z�b�g
	if( SoundSysData.OutputSmaplesPerSec == 0 )
	{
		SoundSysData.OutputSmaplesPerSec = 44100 ;
	}

	// �R�c�T�E���h�̂P���[�g����ݒ�
	if( SoundSysData._3DSoundOneMetreEnable == FALSE )
	{
		SoundSysData._3DSoundOneMetre = 1.0f ;
	}

	// �T�E���h�n���h���Ǘ���񏉊���
	InitializeHandleManage( DX_HANDLETYPE_SOUND, sizeof( SOUND ), MAX_SOUND_NUM, InitializeSoundHandle, TerminateSoundHandle, L"Sound" ) ;

	// �\�t�g�E�G�A�ň����g�`�f�[�^�n���h���Ǘ���񏉊���
	InitializeHandleManage( DX_HANDLETYPE_SOFTSOUND, sizeof( SOFTSOUND ), MAX_SOFTSOUND_NUM, InitializeSoftSoundHandle, TerminateSoftSoundHandle, L"SoftSound" ) ;

	// �l�h�c�h�n���h���Ǘ���񏉊���
	InitializeHandleManage( DX_HANDLETYPE_MUSIC, sizeof( MIDIHANDLEDATA ), MAX_MUSIC_NUM, InitializeMidiHandle, TerminateMidiHandle, L"Music" ) ;

	// �N���e�B�J���Z�N�V�����̏�����
	CriticalSection_Initialize( &SoundSysData._3DSoundListCriticalSection ) ;
	CriticalSection_Initialize( &SoundSysData.Play3DSoundListCriticalSection ) ;
	CriticalSection_Initialize( &SoundSysData.StreamSoundListCriticalSection ) ;

	// �n���h�����X�g��������
	InitializeHandleList( &SoundSysData._3DSoundListFirst,				&SoundSysData._3DSoundListLast ) ;
	InitializeHandleList( &SoundSysData.StreamSoundListFirst,			&SoundSysData.StreamSoundListLast ) ;
	InitializeHandleList( &SoundSysData.SoftSoundPlayerListFirst,		&SoundSysData.SoftSoundPlayerListLast ) ;
	InitializeHandleList( &SoundSysData.PlayFinishDeleteSoundListFirst, &SoundSysData.PlayFinishDeleteSoundListLast ) ;
	InitializeHandleList( &SoundSysData.Play3DSoundListFirst,			&SoundSysData.Play3DSoundListLast ) ;

	// �T�C���e�[�u���̏�����
	{
		int i ;
		float Sin ;
		float Cos ;

		for( i = 0 ; i < SOUND_SINTABLE_DIV ; i ++ )
		{
			_SINCOS( i * DX_PI_F * 2 / SOUND_SINTABLE_DIV, &Sin, &Cos ) ;
			SoundSysData.SinTable[ i ] = ( short )_FTOL( Sin * 16384.0f ) ;
		}
	}

	// ���ˑ�����
	if( InitializeSoundSystem_PF_Timing0() < 0 )
	{
		return -1 ;
	}

	// �쐬���鉹�̃f�[�^�^�C�v���Z�b�g
	SoundSysData.CreateSoundDataType = DX_SOUNDDATATYPE_MEMNOPRESS ;

	// �쐬���鉹�̃s�b�`���[�g���Z�b�g
	SoundSysData.CreateSoundPitchRateEnable = FALSE ;
	SoundSysData.CreateSoundPitchRate = 1.0f ;

	// �쐬���鉹�̃^�C���X�g���b�`���[�g���Z�b�g
	SoundSysData.CreateSoundTimeStretchRateEnable = FALSE ;
	SoundSysData.CreateSoundTimeStretchRate = 1.0f ;

#ifndef DX_NON_OGGVORBIS
	// �n�����u�����������̂o�b�l�f�R�[�h���́A�r�b�g�[�x���P�Ubit�ɃZ�b�g
	SoundSysData.OggVorbisBitDepth = 2 ;
#endif // DX_NON_OGGVORBIS

	// �R�c�T�E���h�����p�̃��X�i�[����������
	SoundSysData.ListenerInfo.Position			= VGet( 0.0f, 0.0f, 0.0f ) ;
	SoundSysData.ListenerInfo.FrontDirection	= VGet( 0.0f, 0.0f, 1.0f ) ;
	SoundSysData.ListenerInfo.Velocity			= VGet( 0.0f, 0.0f, 0.0f ) ;
	SoundSysData.ListenerInfo.InnerAngle		= DX_PI_F * 5.0f / 6.0f ;
	SoundSysData.ListenerInfo.OuterAngle		= DX_PI_F * 11.0f / 6.0f ;
	SoundSysData.ListenerInfo.InnerVolume		= 1.0f ;
	SoundSysData.ListenerInfo.OuterVolume		= 0.75f ;
	SoundSysData.ListenerSideDirection			= VGet( 1.0f, 0.0f, 0.0f ) ;

	// �������t���O�𗧂Ă�
	SoundSysData.InitializeFlag = TRUE ;

	NS_InitSoundMem() ;
	NS_InitSoftSound() ;
	NS_InitSoftSoundPlayer() ;

	// ���ˑ�����
	if( InitializeSoundSystem_PF_Timing1() < 0 )
	{
		return -1 ;
	}

	// �I��
	return 0 ;
}

// �T�E���h�V�X�e���̌�n��������
extern int TerminateSoundSystem( void )
{
	if( SoundSysData.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// ���ˑ�����
	if( TerminateSoundSystem_PF_Timing0() < 0 )
	{
		return -1 ;
	}

	// �T�E���h�L���v�`���������I������
	EndSoundCapture() ;

	// ���ׂẴT�E���h�f�[�^���������
	NS_InitSoundMem() ;

	// �S�Ẵ\�t�g�T�E���h�f�[�^���������
	NS_InitSoftSound() ;
	NS_InitSoftSoundPlayer() ;

	// ���ׂĂ̂l�h�c�h�f�[�^���������
	NS_InitMusicMem() ;

	// �T�E���h�n���h���Ǘ����̌�n��
	TerminateHandleManage( DX_HANDLETYPE_SOUND ) ;

	// �\�t�g�T�E���h�n���h���Ǘ����̌�n��
	TerminateHandleManage( DX_HANDLETYPE_SOFTSOUND ) ;

	// �l�h�c�h�n���h���Ǘ����̌�n��
	TerminateHandleManage( DX_HANDLETYPE_MUSIC ) ;

	// �N���e�B�J���Z�N�V�����̍폜
	CriticalSection_Delete( &SoundSysData._3DSoundListCriticalSection ) ;
	CriticalSection_Delete( &SoundSysData.Play3DSoundListCriticalSection ) ;
	CriticalSection_Delete( &SoundSysData.StreamSoundListCriticalSection ) ;

	// ���ˑ�����
	if( TerminateSoundSystem_PF_Timing1() < 0 )
	{
		return -1 ;
	}

	// �������t���O��|��
	SoundSysData.InitializeFlag = FALSE ;

	// �I��
	return 0 ;
}









// �T�E���h�f�[�^�Ǘ��n�֐�

// InitSoundMem �Ń������ɓǂݍ���WAVE�f�[�^���폜���邩�ǂ������`�F�b�N����֐�
static int DeleteCancelCheckInitSoundMemFunction( HANDLEINFO *HandleInfo )
{
	SOUND * Sound = ( SOUND * )HandleInfo ;

	return Sound->NotInitSoundMemDelete ;
}

// �������ɓǂ݂���WAVE�f�[�^���폜���A����������
extern int NS_InitSoundMem( int /*LogOutFlag*/ )
{
	int Ret ;

	if( SoundSysData.InitializeFlag == FALSE )
		return -1 ;

	// �n���h����������
	Ret = AllHandleSub( DX_HANDLETYPE_SOUND, DeleteCancelCheckInitSoundMemFunction ) ;

	// PlayWav�p�T�E���h�n���h����������
	SoundSysData.PlayWavSoundHandle = -1 ;

	// �I��
	return 0 ;
}

// �V�����T�E���h�f�[�^�̈���m�ۂ���
extern int NS_AddSoundData( int Handle )
{
	int Result ;

	if( SoundSysData.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	Result = AddHandle( DX_HANDLETYPE_SOUND, FALSE, Handle ) ;

	return Result ;
}

// �T�E���h�n���h���̏�����
extern int InitializeSoundHandle( HANDLEINFO *HandleInfo )
{
	SOUND *Sound = ( SOUND * )HandleInfo ;
	int i ;

	// ������
	Sound->PresetReverbParam = 0 ;
	NS_Get3DPresetReverbParamSoundMem( &Sound->ReverbParam, DX_REVERB_PRESET_DEFAULT ) ;

	for( i = 0 ; i < SOUNDBUFFER_MAX_CHANNEL_NUM ; i ++ )
	{
		Sound->BaseVolume[ i ] = 0 ;
	}
	Sound->BasePan = 0 ;
	Sound->BaseFrequency = 0 ;
	Sound->Base3DPosition = VGet( 0.0f, 0.0f, 0.0f ) ;
	Sound->Base3DRadius = 14.0f ;
	Sound->Base3DVelocity = VGet( 0.0f, 0.0f, 0.0f ) ;
	Sound->PitchRateEnable = FALSE ;
	Sound->PitchRate = 0.0f ;

	// �T�E���h�̍Đ����I��������n���h���������I�ɍ폜���邩�ǂ����̃t���O��|��
	Sound->PlayFinishDeleteFlag = FALSE ;

	// �f�[�^�̐���������
	Sound->Stream.FileNum = 0 ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̌�n��
extern int TerminateSoundHandle( HANDLEINFO *HandleInfo )
{
	SOUND *Sound = ( SOUND * )HandleInfo ;
	int i ;

	// ���̍Đ����~�߂�
	if( Sound->Type == DX_SOUNDTYPE_STREAMSTYLE )
	{
		NS_StopStreamSoundMem( HandleInfo->Handle ) ;
	}
	else
	{
		if( Sound->Buffer[0].Valid ) NS_StopSoundMem( HandleInfo->Handle ) ;
	}

	if( Sound->Is3DSound != FALSE )
	{
		// �N���e�B�J���Z�N�V�����̎擾
		CRITICALSECTION_LOCK( &SoundSysData._3DSoundListCriticalSection ) ;

		// �R�c�T�E���h�������ꍇ�͂R�c�T�E���h���X�g����O��
		SubHandleList( &Sound->_3DSoundList ) ;

		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &SoundSysData._3DSoundListCriticalSection ) ;

		// �Đ������X�g�ɒǉ�����Ă�����O��
		if( Sound->AddPlay3DSoundList )
		{
			Sound->AddPlay3DSoundList = FALSE ;

			// �N���e�B�J���Z�N�V�����̎擾
			CRITICALSECTION_LOCK( &SoundSysData.Play3DSoundListCriticalSection ) ;

			SubHandleList( &Sound->Play3DSoundList ) ;

			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &SoundSysData.Play3DSoundListCriticalSection ) ;
		}
	}

	// �Đ����I��������폜����ݒ�ɂȂ��Ă�����A�Đ����I��������폜����T�E���h�̃��X�g����O��
	if( Sound->PlayFinishDeleteFlag )
	{
		SubHandleList( &Sound->PlayFinishDeleteSoundList ) ;
	}

	// �T�E���h�^�C�v�ɂ���ď����𕪊�
	switch( Sound->Type )
	{
	case DX_SOUNDTYPE_NORMAL :
		for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
		{
			if( Sound->Buffer[ i ].Valid ) SoundBuffer_Terminate( &Sound->Buffer[ i ] ) ;
		}

		// �T�E���h�f�[�^���������
		if( Sound->Normal.WaveDataUseCount )
		{
			*Sound->Normal.WaveDataUseCount = *Sound->Normal.WaveDataUseCount - 1 ;
			if( *Sound->Normal.WaveDataUseCount == 0 )
			{
				if( Sound->Normal.WaveData != NULL )
				{
					DXFREE( Sound->Normal.WaveData ) ;
				}
				DXFREE( Sound->Normal.WaveDataUseCount ) ;
			}
		}
		Sound->Normal.WaveDataUseCount = NULL ;
		Sound->Normal.WaveData = NULL ;
		break ;

	case DX_SOUNDTYPE_STREAMSTYLE :
		{
			int i ;
			STREAMFILEDATA *PlayData ;
			SOUND *UniSound = NULL ;
			
			// �T�E���h�o�b�t�@�����L���Ă���ꍇ�͏����𕪊�
			if( Sound->Stream.BufferBorrowSoundHandle != -1 &&
				SOUNDHCHK( Sound->Stream.BufferBorrowSoundHandle, UniSound ) == false )
			{
				// �T�E���h�o�b�t�@���g�p���Ă���̐������炷
				UniSound->Stream.BufferBorrowSoundHandleCount-- ;
				
				// �J�E���g���O�ɂȂ��Ă��āA�������ȊO�̃T�E���h�n���h���ŁA
				// �폜�҂������Ă����ꍇ�͍폜����
				if( Sound->Stream.BufferBorrowSoundHandle != HandleInfo->Handle &&
					UniSound->Stream.DeleteWaitFlag == TRUE )
				{
					NS_DeleteSoundMem( Sound->Stream.BufferBorrowSoundHandle ) ;
				}
			}
			
			// �T�E���h�o�b�t�@�����̃n���h���Ŏg���Ă�����폜�҂���Ԃɂ���
			if( Sound->Stream.BufferBorrowSoundHandleCount != 0 )
			{
				Sound->Stream.DeleteWaitFlag = TRUE ;

				// �폜�L�����Z���̖߂�l
				return 1 ;
			}

			// �N���e�B�J���Z�N�V�����̎擾
			CRITICALSECTION_LOCK( &SoundSysData.StreamSoundListCriticalSection ) ;

			// �X�g���[���T�E���h���X�g����O��
			SubHandleList( &Sound->Stream.StreamSoundList ) ;

			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &SoundSysData.StreamSoundListCriticalSection ) ;

			// �o�^����Ă��邷�ׂẴT�E���h�f�[�^�̗̈�����
			PlayData = Sound->Stream.File ;
			for( i = 0 ; i < Sound->Stream.FileNum ; i ++, PlayData ++ )
			{
				TerminateSoundConvert( &PlayData->ConvData ) ;
			
				if( PlayData->FileImage != NULL )
					DXFREE( PlayData->FileImage ) ;

				if( Sound->Stream.FileLibraryLoadFlag[ i ] == TRUE &&
					PlayData->DataType == DX_SOUNDDATATYPE_FILE &&
					PlayData->ConvData.Stream.DataPoint )
					PlayData->ConvData.Stream.ReadShred.Close( PlayData->ConvData.Stream.DataPoint ) ;

				if( PlayData->MemStream.DataPoint != NULL )
					PlayData->MemStream.ReadShred.Close( PlayData->MemStream.DataPoint ) ;
			}

			if( Sound->Buffer[0].Valid ) SoundBuffer_Terminate( &Sound->Buffer[0] ) ;
		}
		break ;
	}

	// �I��
	return 0 ;
}


// �g�p�\�ȃT�E���h�o�b�t�@���擾����(-1:�T�E���h�o�b�t�@�̎g�p���𓾂��Ȃ�����)
static int GetSoundBuffer( int SoundHandle, SOUND * Sound, SOUNDBUFFER **BufferP, bool BufferGet )
{
	SOUND * UniSound = NULL ;

	// �X�g���[���ł͂Ȃ�������g�p���͓�����
	if( Sound->Type != DX_SOUNDTYPE_STREAMSTYLE )
	{
		if( BufferP ) *BufferP = &Sound->Buffer[ 0 ] ;
		return 0 ;
	}
	
	// ���L�o�b�t�@���g�p���Ă��邩�ǂ����ŕ���
	if( BufferP ) *BufferP = &Sound->Buffer[0] ;
	if( Sound->Stream.BufferBorrowSoundHandle == -1 )
	{
		// �g�p���Ă��Ȃ��ꍇ�́A�g�p����Ă��Ȃ���Ύg�p���𓾂���
		if( Sound->Stream.BufferUseSoundHandle == -1 )
		{
			Sound->Stream.BufferUseSoundHandle = SoundHandle ;
			return 0 ;
		}
		
		// �g�p����Ă���ꍇ�Ŏg�p���Ă���̂������ł͂Ȃ��A
		// �g�p���𓾂�w��������ꍇ�͎g�p���𓾂��Ȃ�
		if( SoundHandle != Sound->Stream.BufferUseSoundHandle && BufferGet == false ) return -1 ;
		
		// �g�p���������Ă���̂������ł͂Ȃ��ꍇ�͍Đ����X�g�b�v����
		if( SoundHandle != Sound->Stream.BufferUseSoundHandle )
			NS_StopStreamSoundMem( Sound->Stream.BufferUseSoundHandle ) ;

		// �g�p���𓾂�
		Sound->Stream.BufferUseSoundHandle = SoundHandle ;
		return 0 ;
	}
	else
	{
		// �؂�悪������������g�p���𓾂��Ȃ�
		if( SOUNDHCHK( Sound->Stream.BufferBorrowSoundHandle, UniSound ) == true ) return -1 ;
		if( BufferP ) *BufferP = &UniSound->Buffer[0] ;
		
		// �N���g�p���Ă��Ȃ��ꍇ�͎g�p���𓾂邱�Ƃ��o����
		if( UniSound->Stream.BufferUseSoundHandle == -1 )
		{
			UniSound->Stream.BufferUseSoundHandle = SoundHandle ;
			return 0 ;
		}
		
		// �g�p����Ă���ꍇ�Ŏg�p���Ă���̂������ł͂Ȃ��A
		// �g�p���𓾂�w��������ꍇ�͎g�p���𓾂��Ȃ�
		if( SoundHandle != UniSound->Stream.BufferUseSoundHandle && BufferGet == false ) return -1 ;

		// �g�p���������Ă���̂������ł͂Ȃ��ꍇ�͍Đ����X�g�b�v����
		if( SoundHandle != UniSound->Stream.BufferUseSoundHandle )
			NS_StopStreamSoundMem( UniSound->Stream.BufferUseSoundHandle ) ;

		// �g�p���𓾂�
		UniSound->Stream.BufferUseSoundHandle = SoundHandle ;
		return 0 ;
	}
}

// �V�����T�E���h�o�b�t�@�̍쐬
static int CreateSoundBuffer( WAVEFORMATEX *WaveFormat , DWORD BufferSize, int SoundType , int BufferNum , int SoundHandle, int SrcSoundHandle, int ASyncThread )
{
	int i ;
	SOUND *Sound ;
	SOUND *SrcSound = NULL ;
	int hr ;
	
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, Sound ) )
			return -1 ;
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, Sound ) )
			return -1 ;
	}

	// ���n���h��������ꍇ�̏���
	if( SrcSoundHandle != -1 )
	{
		if( SOUNDHCHK( SrcSoundHandle, SrcSound ) )
			return -1 ;

		// �^�C�v���X�g���[����������G���[
		if( SoundType != DX_SOUNDTYPE_NORMAL ) return -1 ;

		// �p�����[�^�͌��n���h����������Ă���
		WaveFormat = &SrcSound->BufferFormat ;
		BufferSize = ( DWORD )SrcSound->Normal.WaveSize ;
	}

	// �^�C�v�ɂ���ď����𕪊�
	switch( SoundType )
	{
	case DX_SOUNDTYPE_NORMAL :	// �m�[�}���T�E���h
		{
			// �Z�J���_���T�E���h�o�b�t�@���쐬

			// �����쐬
			for( i = 0 ; i < BufferNum ; i ++ )
			{
				// �\�[�X�n���h���������ꍇ�͂P�Ԗڈȍ~�͓����o�b�t�@�𕡐�����
				if( i == 0 && SrcSound == NULL )
				{
NORMAL_CREATEBUFFER :
					hr = SoundBuffer_Initialize( &Sound->Buffer[ i ], BufferSize, WaveFormat, NULL, TRUE, FALSE, Sound->Is3DSound ) ;
					if( hr != 0 )
					{
						i -- ;
						while( i != -1 )
						{
							if( Sound->Buffer[ i ].Valid ) SoundBuffer_Terminate( &Sound->Buffer[ i ] ) ;

							i -- ;
						}
						DXST_ERRORLOG_ADDUTF16LE( "\x44\x00\x69\x00\x72\x00\x65\x00\x63\x00\x74\x00\x53\x00\x6f\x00\x75\x00\x6e\x00\x64\x00\x42\x00\x75\x00\x66\x00\x66\x00\x65\x00\x72\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�T�E���h�o�b�t�@�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
						return -1 ;
					}
					
					// �������ǂ����t���O��|��
					Sound->Normal.BufferDuplicateFlag[ i ] = FALSE ;
				}
				else
				{
					if( SrcSound )
					{
						hr = SoundBuffer_Duplicate( &Sound->Buffer[ i ], &SrcSound->Buffer[ 0 ], Sound->Is3DSound ) ;
					}
					else
					{
						hr = SoundBuffer_Duplicate( &Sound->Buffer[ i ], &Sound->Buffer[ 0 ], Sound->Is3DSound ) ;
					}

					// �������ǂ����t���O���Ƃ肠�����|���Ă���
					Sound->Normal.BufferDuplicateFlag[ i ] = FALSE ;

					// �����Ɏ��s�����ꍇ�͕����̃T�E���h�o�b�t�@���g�p���Ȃ�
					if( hr != 0 )
					{
						// �\�[�X�T�E���h�������āA���O�Ŏ��s��������n���h��������
						if( SrcSound != NULL && i == 0 )
						{
							goto NORMAL_CREATEBUFFER ;
						}
						else
						{
							BufferNum = i ;
							break ;
						}
					}
				}
			}

			// �T�E���h�o�b�t�@����ۑ�
			Sound->ValidBufferNum = BufferNum ;
		}
		break ;

	case DX_SOUNDTYPE_STREAMSTYLE :		// �X�g���[�����T�E���h
		if( SoundBuffer_Initialize(
				&Sound->Buffer[ 0 ],
				SOUNDSIZE( STS_BUFSEC * WaveFormat->nAvgBytesPerSec / STS_DIVNUM, WaveFormat->nBlockAlign ),
				WaveFormat,
				NULL,
				TRUE,
				TRUE,
				Sound->Is3DSound ) != 0 )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb9\x30\xc8\x30\xea\x30\xfc\x30\xe0\x30\xa8\x98\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�X�g���[�����T�E���h�o�b�t�@�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}

		// �T�E���h�o�b�t�@����ۑ�
		Sound->ValidBufferNum = 1 ;
		break ;
	}

	// �f�[�^��ۑ�
	Sound->Type = SoundType ;

	// �X�g���[���`���̏ꍇ�̓X�g���[���T�E���h�n���h�����X�g�ɒǉ�
	if( SoundType == DX_SOUNDTYPE_STREAMSTYLE )
	{
		// �N���e�B�J���Z�N�V�����̎擾
		CRITICALSECTION_LOCK( &SoundSysData.StreamSoundListCriticalSection ) ;

		AddHandleList( &SoundSysData.StreamSoundListFirst, &Sound->Stream.StreamSoundList, SoundHandle, Sound ) ;

		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &SoundSysData.StreamSoundListCriticalSection ) ;
	}

	// �I��
	return 0 ;
}


// AddStreamSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int AddStreamSoundMem_UseGParam(
	LOADSOUND_GPARAM *GParam,
	STREAMDATA *Stream,
	int LoopNum,
	int SoundHandle,
	int StreamDataType,
	int *CanStreamCloseFlag,
	int UnionHandle,
	int ASyncThread
)
{
	SOUND * Sound ;
	STREAMFILEDATA *PlayData ;
	WAVEFORMATEX Format ;
	SOUND * UniSound;
	SOUNDBUFFER *SBuffer ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, Sound ) )
			goto ERR ;
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, Sound ) )
			goto ERR ;
	}

	// ��ł��f�[�^�����݂���ꍇ�̓`�F�b�N����������
	if( Sound->Stream.FileNum > 0 )
	{
		// �X�g���[�����T�E���h�ł͂Ȃ������ꍇ�͏I��
		if( Sound->Type != DX_SOUNDTYPE_STREAMSTYLE )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb9\x30\xc8\x30\xea\x30\xfc\x30\xe0\x30\xa8\x98\xb5\x30\xa6\x30\xf3\x30\xc9\x30\x67\x30\x6f\x30\x6a\x30\x44\x30\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xc7\x30\xfc\x30\xbf\x30\x4c\x30\x21\x6e\x55\x30\x8c\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x41\x00\x64\x00\x64\x00\x53\x00\x72\x00\x65\x00\x61\x00\x6d\x00\x53\x00\x6f\x00\x75\x00\x6e\x00\x64\x00\x0a\x00\x00"/*@ L"�X�g���[�����T�E���h�ł͂Ȃ��T�E���h�f�[�^���n����܂��� in AddSreamSound\n" @*/ ) ;
			goto ERR ;
		}

		// ���ɍĐ����̏ꍇ���I��
		if( GetSoundBuffer( SoundHandle, Sound, &SBuffer, false ) == 0 )
		{
			if( SoundBuffer_CheckPlay( SBuffer ) )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xe2\x65\x6b\x30\x8d\x51\x1f\x75\x2d\x4e\x6e\x30\xb9\x30\xc8\x30\xea\x30\xfc\x30\xe0\x30\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xc7\x30\xfc\x30\xbf\x30\x6b\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\xfd\x8f\xa0\x52\x57\x30\x88\x30\x46\x30\x68\x30\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���ɍĐ����̃X�g���[���T�E���h�f�[�^�Ƀf�[�^��ǉ����悤�Ƃ��܂���\n" @*/ ) ;
				goto ERR ;
			}
		}

		// �����X�g���[���T�E���h����f�[�^�̗̈�ɗ]�T���Ȃ��ꍇ���I��
		if( Sound->Stream.FileNum == STS_SETSOUNDNUM - 1 )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb9\x30\xc8\x30\xea\x30\xfc\x30\xe0\x30\xa8\x98\xb5\x30\xa6\x30\xf3\x30\xc9\x30\x6e\x30\x36\x52\xa1\x5f\xc7\x30\xfc\x30\xbf\x30\x18\x98\xdf\x57\x4c\x30\x44\x30\x63\x30\x71\x30\x44\x30\x67\x30\x59\x30\x0a\x00\x00"/*@ L"�X�g���[�����T�E���h�̐���f�[�^�̈悪�����ς��ł�\n" @*/ ) ;
			goto ERR ;
		}
	}

	PlayData = &Sound->Stream.File[ Sound->Stream.FileNum ] ;
	_MEMSET( PlayData, 0, sizeof( STREAMFILEDATA ) ) ;

	// �f�[�^�^�C�v��ۑ�
	PlayData->DataType = StreamDataType ;

	// ���[�v�J�n�ʒu�͖���
	PlayData->LoopStartSamplePosition = -1 ;

	// �X�g���[���|�C���^��擪�ʒu��
	Stream->ReadShred.Seek( Stream->DataPoint, 0, SEEK_SET ) ;

	// �V�����v�`�u�d�t�@�C���̃��[�h�A�f�[�^�̃^�C�v�ɂ���ď����𕪊�
	switch( StreamDataType )
	{
	case DX_SOUNDDATATYPE_MEMNOPRESS :
		// ��������ɒ��ڍĐ��\�ȃf�[�^��ێ�����ꍇ
		{
			STREAMDATA MemStream ;
			void *WaveImage ;
			int WaveSize ;

			// �o�b�l�`���ɕϊ�
			{
				SOUNDCONV ConvData ;
				int res ;
				void *SrcBuffer ;
				int SrcSize ;
				
				// �ہX�������ɓǂݍ���
				if( StreamFullRead( Stream, &SrcBuffer, &SrcSize ) < 0 )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\xf3\x97\xf0\x58\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�����f�[�^�̓ǂݍ��݂Ɏ��s���܂���" @*/ )) ;
					goto ERR ;
				}
				
				MemStream.DataPoint = MemStreamOpen( SrcBuffer, ( size_t )SrcSize ) ;
				MemStream.ReadShred = *GetMemStreamDataShredStruct() ;
			
				SetupSoundConvert( &ConvData, &MemStream, GParam->DisableReadSoundFunctionMask
#ifndef DX_NON_OGGVORBIS
									,GParam->OggVorbisBitDepth, GParam->OggVorbisFromTheoraFile
#endif
				) ;
				res = SoundConvertFast( &ConvData, &Format, &PlayData->FileImage, &PlayData->FileImageSize ) ;

				TerminateSoundConvert( &ConvData ) ;
				MemStreamClose( MemStream.DataPoint ) ;
				DXFREE( SrcBuffer ) ;

				if( res < 0 )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\xf3\x97\xf0\x58\xc7\x30\xfc\x30\xbf\x30\x6e\x30\x09\x59\xdb\x63\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�����f�[�^�̕ϊ��Ɏ��s���܂���" @*/ )) ;
					goto ERR ;
				}
			}

			// �v�`�u�d�t�@�C�����ł����グ��
			if( CreateWaveFileImage( &WaveImage, &WaveSize,
								 &Format, sizeof( WAVEFORMATEX ),
								 PlayData->FileImage, PlayData->FileImageSize ) < 0 )
				goto ERR ;

			// �W�J���ꂽ�f�[�^���X�g���[���Ƃ��čēx�J��				
			PlayData->MemStream.DataPoint = MemStreamOpen( WaveImage, ( size_t )WaveSize ) ;
			PlayData->MemStream.ReadShred = *GetMemStreamDataShredStruct() ;
			SetupSoundConvert( &PlayData->ConvData, &PlayData->MemStream, GParam->DisableReadSoundFunctionMask
#ifndef DX_NON_OGGVORBIS
				,GParam->OggVorbisBitDepth, GParam->OggVorbisFromTheoraFile
#endif
			) ;

			// �������A�h���X�̓���ւ�
			DXFREE( PlayData->FileImage ) ;
			PlayData->FileImage = WaveImage ;
			PlayData->FileImageSize = WaveSize ;

			// �X�g���[������t���O�𗧂Ă�
			*CanStreamCloseFlag = TRUE ;
		}
		break ;

	case DX_SOUNDDATATYPE_MEMPRESS :
		// �f�[�^�̓�������ɑ��݂��āA���k����Ă���ꍇ
		{
			// �ہX�������ɓǂݍ���
			if( StreamFullRead( Stream, &PlayData->FileImage, &PlayData->FileImageSize ) < 0 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xf3\x97\xf0\x58\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�����f�[�^�̓ǂݍ��݂Ɏ��s���܂���" @*/ )) ;
				goto ERR ;
			}

			// �W�J���ꂽ�f�[�^���X�g���[���Ƃ��čēx�J��				
			PlayData->MemStream.DataPoint = MemStreamOpen( PlayData->FileImage, ( size_t )PlayData->FileImageSize ) ;
			PlayData->MemStream.ReadShred = *GetMemStreamDataShredStruct() ;
			if( SetupSoundConvert( &PlayData->ConvData, &PlayData->MemStream, GParam->DisableReadSoundFunctionMask
#ifndef DX_NON_OGGVORBIS
									,GParam->OggVorbisBitDepth, GParam->OggVorbisFromTheoraFile
#endif
									) < 0 )
			{
				MemStreamClose( PlayData->MemStream.DataPoint ) ;
				DXFREE( PlayData->FileImage ) ;
				PlayData->FileImage = NULL ;
				
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xf3\x97\xf0\x58\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xaa\x30\xfc\x30\xd7\x30\xf3\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�����f�[�^�̃I�[�v���Ɏ��s���܂���" @*/ )) ;
				goto ERR ;
			}

			// ���[�v��񂪂���ꍇ�̓Z�b�g
			if( ( GParam->CreateSoundLoopStartTimePosition   == GParam->CreateSoundLoopEndTimePosition   ) &&
				( GParam->CreateSoundLoopStartSamplePosition == GParam->CreateSoundLoopEndSamplePosition ) )
			{
				int LoopStartPos, LoopEndPos ;
				if( GetSoundConvertLoopAreaInfo( &PlayData->ConvData, &LoopStartPos, &LoopEndPos ) >= 0 )
				{
					GParam->CreateSoundLoopStartSamplePosition = LoopStartPos ;
					GParam->CreateSoundLoopEndSamplePosition   = LoopEndPos ;
				}
			}

			// �X�g���[������t���O�𗧂Ă�
			*CanStreamCloseFlag = TRUE ;
		}
		break ;

	case DX_SOUNDDATATYPE_FILE :
		// �f�[�^���t�@�C�����璀���ǂݍ���ł���ꍇ
		{
			// �g��Ȃ��f�[�^�� NULL ���Z�b�g���Ă���
			PlayData->FileImage = NULL ;
			PlayData->FileImageSize = 0 ;
			PlayData->MemStream.DataPoint = NULL ;

			// �W�J���ꂽ�f�[�^���X�g���[���Ƃ��čēx�J��				
			if( SetupSoundConvert( &PlayData->ConvData, Stream, GParam->DisableReadSoundFunctionMask
#ifndef DX_NON_OGGVORBIS
									,GParam->OggVorbisBitDepth, GParam->OggVorbisFromTheoraFile
#endif
								) < 0 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xf3\x97\xf0\x58\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xaa\x30\xfc\x30\xd7\x30\xf3\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�����f�[�^�̃I�[�v���Ɏ��s���܂���" @*/ )) ;
				goto ERR ;
			}

			// ���[�v��񂪂���ꍇ�̓Z�b�g
			if( ( GParam->CreateSoundLoopStartTimePosition   == GParam->CreateSoundLoopEndTimePosition   ) &&
				( GParam->CreateSoundLoopStartSamplePosition == GParam->CreateSoundLoopEndSamplePosition ) )
			{
				int LoopStartPos, LoopEndPos ;
				if( GetSoundConvertLoopAreaInfo( &PlayData->ConvData, &LoopStartPos, &LoopEndPos ) >= 0 )
				{
					GParam->CreateSoundLoopStartSamplePosition = LoopStartPos ;
					GParam->CreateSoundLoopEndSamplePosition   = LoopEndPos ;
				}
			}

			// �X�g���[������t���O��|��
			*CanStreamCloseFlag = FALSE ;
		}
		break ;
	}
	
	// ��ԍŏ��̃f�[�^�̏ꍇ�͏����𕪊�
	if( Sound->Stream.FileNum == 0 )
	{
		// �t�H�[�}�b�g���擾
		GetOutSoundFormatInfo( &PlayData->ConvData, &Sound->BufferFormat ) ;

		// �T�E���h�o�b�t�@�����L���邩�ǂ����𒲂ׂ�
		UniSound = NULL;
		while( UnionHandle != -1 )
		{
			// �����ȃn���h���̏ꍇ�͖���
			if( SOUNDHCHK( UnionHandle, UniSound ) )
			{
				UniSound = NULL;
				UnionHandle = -1;
				break;
			}

			// �t�H�[�}�b�g���Ⴄ���A�I���W�i���̃o�b�t�@���Ȃ����A�X�g���[���`���łȂ��ꍇ���A�E�g
			if( UniSound->Type != DX_SOUNDTYPE_STREAMSTYLE ||
				UniSound->Stream.BufferBorrowSoundHandle != -1 ||
				_MEMCMP( &Sound->BufferFormat, &UniSound->BufferFormat, sizeof( WAVEFORMATEX ) ) != 0 )
			{
				UniSound = NULL;
				UnionHandle = -1;
				break;
			}
			break;
		}

		// �T�E���h�o�b�t�@�����L���邩�ǂ����ŏ����𕪊�
		if( UniSound == NULL )
		{
			// �T�E���h�o�b�t�@���쐬
			CreateSoundBuffer( &Sound->BufferFormat, SOUNDSIZE( STS_BUFSEC * Sound->BufferFormat.nAvgBytesPerSec / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ), DX_SOUNDTYPE_STREAMSTYLE, 1, SoundHandle, -1, ASyncThread ) ;
			Sound->Stream.SoundBufferSize = SOUNDSIZE( STS_BUFSEC * Sound->BufferFormat.nAvgBytesPerSec / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ) ;
			Sound->BaseFrequency          = ( int )Sound->BufferFormat.nSamplesPerSec ;
//			Sound->PitchRateEnable        = GParam->CreateSoundPitchRateEnable ;
//			Sound->PitchRate              = GParam->CreateSoundPitchRate ;

			// ���L�����Z�b�g
			Sound->Stream.BufferBorrowSoundHandle = -1 ;
		}
		else
		{
			// �N���e�B�J���Z�N�V�����̎擾
			CRITICALSECTION_LOCK( &SoundSysData.StreamSoundListCriticalSection ) ;

			// �X�g���[���T�E���h�n���h�����X�g�ɒǉ�
			Sound->Type = DX_SOUNDTYPE_STREAMSTYLE ;
			Sound->ValidBufferNum = 1 ;
			AddHandleList( &SoundSysData.StreamSoundListFirst, &Sound->Stream.StreamSoundList, SoundHandle, Sound ) ;

			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &SoundSysData.StreamSoundListCriticalSection ) ;

			// ���L�����Z�b�g
			Sound->Stream.BufferBorrowSoundHandle = UnionHandle ;
			UniSound->Stream.BufferBorrowSoundHandleCount ++ ;
		}
		Sound->Stream.BufferBorrowSoundHandleCount = 0 ;
		Sound->Stream.BufferUseSoundHandle = -1 ;
		Sound->Stream.DeleteWaitFlag = FALSE ;
	}
	else
	{
		// ��ڈȍ~�̏ꍇ�̓o�b�t�@�̃t�H�[�}�b�g�ƈ�v���Ă��邩���ׂ�
		GetOutSoundFormatInfo( &PlayData->ConvData, &Format ) ;
		if( _MEMCMP( &Format, &Sound->BufferFormat, sizeof( WAVEFORMATEX ) ) != 0 )
		{
			TerminateSoundConvert( &PlayData->ConvData ) ;
			if( PlayData->MemStream.DataPoint != NULL )
			{
				MemStreamClose( PlayData->MemStream.DataPoint ) ;
				PlayData->MemStream.DataPoint = NULL ;
			}
			if( PlayData->FileImage != NULL )
			{
				DXFREE( PlayData->FileImage ) ;
				PlayData->FileImage = NULL ;
			}

			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd5\x30\xa9\x30\xfc\x30\xde\x30\xc3\x30\xc8\x30\x6e\x30\x55\x90\x46\x30\xf3\x97\xf0\x58\xc7\x30\xfc\x30\xbf\x30\x6f\x30\x0c\x54\x42\x66\x6b\x30\x7f\x4f\x46\x30\x53\x30\x68\x30\x6f\x30\xfa\x51\x65\x67\x7e\x30\x5b\x30\x93\x30\x00"/*@ L"�t�H�[�}�b�g�̈Ⴄ�����f�[�^�͓����Ɏg�����Ƃ͏o���܂���" @*/ )) ;
			goto ERR ;
		}
	}

	// ���[�v�̐���ۑ�
	PlayData->LoopNum = ( char )LoopNum ;

	// �v���C�f�[�^�̐��𑝂₷
	Sound->Stream.FileNum ++ ;
	
	// �X�g���[���f�[�^�̑��T���v�������X�V
	{
		int i ;
		Sound->Stream.TotalSample = 0 ;
		for( i = 0 ; i < Sound->Stream.FileNum ; i ++ )
			Sound->Stream.TotalSample += GetSoundConvertDestSize_Fast( &Sound->Stream.File[i].ConvData ) / Sound->BufferFormat.nBlockAlign ;
	}

	// �I��
	return 0 ;

ERR :

	return -1 ;
}

// �X�g���[�����T�E���h�f�[�^�ɃT�E���h�f�[�^��ǉ�����
extern int NS_AddStreamSoundMem( STREAMDATA *Stream, int LoopNum, int SoundHandle, int StreamDataType, int *CanStreamCloseFlag, int UnionHandle )
{
	LOADSOUND_GPARAM GParam ;

	InitLoadSoundGParam( &GParam ) ;

	return AddStreamSoundMem_UseGParam( &GParam, Stream, LoopNum, SoundHandle, StreamDataType, CanStreamCloseFlag, UnionHandle, FALSE ) ;
}

// AddStreamSoundMemToFile �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int AddStreamSoundMemToFile_UseGParam(
	LOADSOUND_GPARAM *GParam,
	const wchar_t *WaveFile,
	int LoopNum,
	int SoundHandle,
	int StreamDataType,
	int UnionHandle,
	int ASyncThread
)
{
	DWORD_PTR fp ;
	SOUND * Sound ;
	int CanStreamCloseFlag ;
	STREAMDATA Stream ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, Sound ) )
			goto ERR ;
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, Sound ) )
			goto ERR ;
	}

	// �t�@�C�����J��
	fp = DX_FOPEN( WaveFile ) ;
	if( fp == 0 )
	{
		DXST_ERRORLOGFMT_ADDW(( L"Sound File Open Error : %s \n" , WaveFile )) ;
		goto ERR ;
	}

	// �X�g���[���f�[�^�����Z�b�g����
	Stream.DataPoint = (void *)fp ;
	Stream.ReadShred = *GetFileStreamDataShredStruct() ;

	// �X�g���[���n���h����ǉ�
	if( AddStreamSoundMem_UseGParam( GParam, &Stream, LoopNum, SoundHandle, StreamDataType, &CanStreamCloseFlag, UnionHandle, ASyncThread ) == -1 )
	{
		DX_FCLOSE( fp ) ;
		DXST_ERRORLOGFMT_ADDW(( L"Sound File Setup Error : %s \n" , WaveFile )) ;
		goto ERR ;
	}

	// �t�@�C������Ă��ǂ��Ƃ̂��B����������t�@�C�������
	if( CanStreamCloseFlag == TRUE )
	{
		DX_FCLOSE( fp ) ;
	}

	// ���C�u�����Ń��[�h�����A�t���O�𗧂Ă�
	Sound->Stream.FileLibraryLoadFlag[ Sound->Stream.FileNum - 1 ] = TRUE ;

	// �I��
	return 0 ;

ERR :

	// �G���[�I��
	return -1 ;
}

// AddStreamSoundMemToMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int AddStreamSoundMemToMem_UseGParam(
	LOADSOUND_GPARAM *GParam,
	const void *FileImageBuffer,
	int ImageSize,
	int LoopNum,
	int SoundHandle,
	int StreamDataType,
	int UnionHandle,
	int ASyncThread
)
{
	void *StreamHandle ;
	SOUND * Sound ;
	int CanStreamCloseFlag ;
	STREAMDATA Stream ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, Sound ) )
			goto ERR ;
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, Sound ) )
			goto ERR ;
	}

	// �f�[�^�^�C�v���t�@�C������̒����ǂݍ��݂������ꍇ�̓������ɃC���[�W��u���A�ɕύX
	if( StreamDataType == DX_SOUNDDATATYPE_FILE ) StreamDataType = DX_SOUNDDATATYPE_MEMPRESS ;

	// �������C���[�W�X�g���[�����J��
	StreamHandle = MemStreamOpen( FileImageBuffer, ( size_t )ImageSize ) ;
	if( StreamHandle == NULL )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x37\xff\x21\xff\x36\xff\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\xb9\x30\xc8\x30\xea\x30\xfc\x30\xe0\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x0a\x00\x00"/*@ L"�v�`�u�t�@�C���̃X�g���[���C���[�W�n���h���̍쐬�Ɏ��s���܂��� \n" @*/ )) ;
		goto ERR ;
	}

	// �X�g���[���f�[�^�����Z�b�g
	Stream.DataPoint = StreamHandle ;
	Stream.ReadShred = *GetMemStreamDataShredStruct() ;

	// �X�g���[���n���h����ǉ�
	if( AddStreamSoundMem_UseGParam( GParam, &Stream, LoopNum, SoundHandle, StreamDataType, &CanStreamCloseFlag, UnionHandle, ASyncThread ) == -1 )
	{
		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x37\xff\x21\xff\x36\xff\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\xb9\x30\xc8\x30\xea\x30\xfc\x30\xe0\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x6e\x30\xbb\x30\xc3\x30\xc8\x30\xa2\x30\xc3\x30\xd7\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x0a\x00\x00"/*@ L"�v�`�u�t�@�C���̃X�g���[���C���[�W�̃Z�b�g�A�b�v�Ɏ��s���܂��� \n" @*/ )) ;
		goto ERR ;
	}

	// �X�g���[������Ă��ǂ��Ƃ̂��B����������X�g���[�������
	if( CanStreamCloseFlag == TRUE )
	{
		MemStreamClose( StreamHandle ) ;
	}

	// ���C�u�����Ń��[�h�����A�t���O�𗧂Ă�
	Sound->Stream.FileLibraryLoadFlag[ Sound->Stream.FileNum - 1 ] = TRUE ;

	// �I��
	return 0 ;

ERR:

	return -1 ;
}

// �X�g���[�����T�E���h�f�[�^�ɃT�E���h�f�[�^��ǉ�����
extern int NS_AddStreamSoundMemToFile( const TCHAR *WaveFile, int LoopNum,  int SoundHandle, int StreamDataType, int UnionHandle )
{
#ifdef UNICODE
	return AddStreamSoundMemToFile_WCHAR_T(
		WaveFile, LoopNum, SoundHandle, StreamDataType, UnionHandle
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( WaveFile, return -1 )

	Result = AddStreamSoundMemToFile_WCHAR_T(
		UseWaveFileBuffer, LoopNum, SoundHandle, StreamDataType, UnionHandle
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( WaveFile )

	return Result ;
#endif
}

// �X�g���[�����T�E���h�f�[�^�ɃT�E���h�f�[�^��ǉ�����
extern int AddStreamSoundMemToFile_WCHAR_T( const wchar_t *WaveFile, int LoopNum,  int SoundHandle, int StreamDataType, int UnionHandle )
{
	LOADSOUND_GPARAM GParam ;

	InitLoadSoundGParam( &GParam ) ;
	return AddStreamSoundMemToFile_UseGParam( &GParam, WaveFile, LoopNum, SoundHandle, StreamDataType, UnionHandle, FALSE ) ;
}

// �X�g���[�����T�E���h�f�[�^�ɃT�E���h�f�[�^��ǉ�����
extern int NS_AddStreamSoundMemToMem( const void *FileImageBuffer, int ImageSize, int LoopNum, int SoundHandle, int StreamDataType, int UnionHandle )
{
	LOADSOUND_GPARAM GParam ;

	InitLoadSoundGParam( &GParam ) ;
	return AddStreamSoundMemToMem_UseGParam( &GParam, FileImageBuffer, ImageSize, LoopNum, SoundHandle, StreamDataType, UnionHandle, FALSE ) ;
}



// SetupStreamSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int SetupStreamSoundMem_UseGParam( int SoundHandle, int ASyncThread )
{
	SOUND * Sound ;
	SOUNDBUFFER *SBuffer ;

	if( SoundSysData.InitializeFlag == FALSE ||
		CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, Sound ) )
			goto ERR ;
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, Sound ) )
			goto ERR ;
	}

	// �X�g���[������Ȃ������牽�������I��
	if( Sound->Type != DX_SOUNDTYPE_STREAMSTYLE ) goto END ;

	// �Đ����������牽�������I��
	if( ASyncThread == FALSE && NS_CheckStreamSoundMem( SoundHandle ) == 1 ) goto END ;

	// �g�p���������ꍇ�͉��������I��
	if( GetSoundBuffer( SoundHandle, Sound, &SBuffer, false ) == -1 ) goto END ;

	// �������������Ă����牽�������I��
	if( Sound->Stream.StartSetupCompFlag == TRUE ) goto END ;

	// �Đ��ʒu��擪�ɃZ�b�g
//	SBuffer->SetCurrentPosition( 0 ) ;
	SoundBuffer_SetCurrentPosition( SBuffer, 0 ) ;

	// �p�����[�^��������
	Sound->Stream.SoundBufferCompCopyOffset	 = -800 ;
	Sound->Stream.SoundBufferCopyStartOffset = -1 ;
	Sound->Stream.PrevCheckPlayPosition      = 0 ;
	Sound->Stream.EndWaitFlag                = FALSE ;

	// ���k�f�[�^��W�J���鏀�����s��
	{
		STREAMFILEDATA *PlayData ;

		PlayData = &Sound->Stream.File[ Sound->Stream.FileActive ] ;
//		SetTimeSoundConvert( &PlayData->ConvData, Sound->Stream.FileCompCopyLength * 1000 / Sound->BufferFormat.nAvgBytesPerSec ) ;
		SetSampleTimeSoundConvert( &PlayData->ConvData, Sound->Stream.FileCompCopyLength / Sound->BufferFormat.nBlockAlign ) ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// ��������
	Sound->Stream.StartSetupCompFlag = TRUE ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �ŏ��̃T�E���h�f�[�^����������
	ProcessStreamSoundMem_UseGParam( SoundHandle, ASyncThread ) ;

END :

	// �I��
	return 0 ;

ERR :

	// �I��
	return 0 ;
}

// �X�g���[�����T�E���h�f�[�^�̍Đ��������s��
extern int NS_SetupStreamSoundMem( int SoundHandle )
{
	return SetupStreamSoundMem_UseGParam( SoundHandle, FALSE ) ;
}


// �X�g���[���T�E���h�n���h���̍Đ��������s��( -1:�G���[ 0:����I�� 1:�Đ�����K�v�Ȃ� )
static	int	_PlaySetupStreamSoundMem( int SoundHandle, SOUND * Sound, int PlayType, int TopPositionFlag, SOUNDBUFFER **DestSBuffer )
{
	SOUNDBUFFER *SBuffer ;
	int i ;

	// �g�p����T�E���h�o�b�t�@���擾
	GetSoundBuffer( SoundHandle, Sound, &SBuffer );
	if( DestSBuffer ) *DestSBuffer = SBuffer ;
	
	// �Đ����œ������ł��Ȃ��ꍇ�͍����ŏI��
	if( TopPositionFlag == FALSE && NS_CheckStreamSoundMem( SoundHandle ) == 1 )
		return 1 ;

	// �Đ����̏ꍇ�͎~�߂�
	NS_StopStreamSoundMem( SoundHandle ) ;

	// �Đ��^�C�v��������番��
	if( Sound->PlayType != PlayType )
	{
		// �ꍇ�ɂ���Ă͓������Đ��ɂ���
		if( ( Sound->PlayType & DX_PLAYTYPE_LOOPBIT ) != ( PlayType & DX_PLAYTYPE_LOOPBIT ) &&
			Sound->Stream.AllPlayFlag == TRUE )
		{
			Sound->Stream.AllPlayFlag = FALSE ;
			Sound->Stream.FileCompCopyLength = 0 ;
			Sound->Stream.CompPlayWaveLength = 0 ;
			Sound->Stream.LoopPositionValidFlag = FALSE ;
			Sound->Stream.FileLoopCount = 0 ;
			Sound->Stream.FileActive = 0 ;
			Sound->Stream.StartSetupCompFlag = FALSE ;		// �����t���O��|��
		}
	}

	// ������̍Đ��������ꍇ�̏���
	if( TopPositionFlag == TRUE )
	{
		Sound->Stream.AllPlayFlag = FALSE ;
		if( Sound->Stream.CompPlayWaveLength != 0 )
		{
			Sound->Stream.FileCompCopyLength = 0 ;
			Sound->Stream.CompPlayWaveLength = 0 ;
			Sound->Stream.LoopPositionValidFlag = FALSE ;
			Sound->Stream.FileLoopCount = 0 ;
			Sound->Stream.FileActive = 0 ;
			Sound->Stream.StartSetupCompFlag = FALSE ;		// �����t���O��|��
		}
	}

	// �Đ��^�C�v��ۑ�
	Sound->PlayType = PlayType ;

	// �Đ�����
	NS_SetupStreamSoundMem( SoundHandle ) ;

	// �{�����[�����Z�b�g����
	for( i = 0 ; i < SOUNDBUFFER_MAX_CHANNEL_NUM ; i ++ )
	{
		if( Sound->ValidNextPlayVolume[ i ] )
		{
			SoundBuffer_SetVolumeAll( &Sound->Buffer[ 0 ], ( LONG )Sound->NextPlayVolume[ i ] ) ;
			Sound->ValidNextPlayVolume[ i ] = 0 ;
		}
		else
		{
			SoundBuffer_SetVolumeAll( &Sound->Buffer[ 0 ], ( LONG )Sound->BaseVolume[ i ] ) ;
		}
	}

	// �p�����Z�b�g����
	if( Sound->ValidNextPlayPan )
	{
		SoundBuffer_SetPan( &Sound->Buffer[ 0 ], ( LONG )Sound->NextPlayPan ) ;
		Sound->ValidNextPlayPan = 0 ;
	}
	else
	{
		SoundBuffer_SetPan( &Sound->Buffer[ 0 ], ( LONG )Sound->BasePan ) ;
	}

	// ���g�����Z�b�g����
	{
		int Frequency ;

		if( Sound->ValidNextPlayFrequency )
		{
			Frequency = Sound->NextPlayFrequency ;
			Sound->ValidNextPlayFrequency = 0 ;
		}
		else
		{
			Frequency = Sound->BaseFrequency ;
		}

		if( Sound->PitchRateEnable )
		{
			Frequency = _FTOL( Frequency * Sound->PitchRate ) ;
		}
		SoundBuffer_SetFrequency( &Sound->Buffer[ 0 ], ( DWORD )Frequency ) ;
	}

	// �R�c�T�E���h�̏ꍇ�͂R�c�T�E���h�p�����[�^�̍X�V���s��
	if( Sound->Is3DSound != FALSE )
	{
		VECTOR *Position ;
		float Radius ;
		VECTOR *Velocity ;

		if( Sound->ValidNextPlay3DPosition )
		{
			Position = &Sound->NextPlay3DPosition ;
			Sound->ValidNextPlay3DPosition = 0 ;
		}
		else
		{
			Position = &Sound->Base3DPosition ;
		}

		if( Sound->ValidNextPlay3DRadius )
		{
			Radius = Sound->NextPlay3DRadius ;
			Sound->ValidNextPlay3DRadius = 0 ;
		}
		else
		{
			Radius = Sound->Base3DRadius ;
		}

		if( Sound->ValidNextPlay3DVelocity )
		{
			Velocity = &Sound->NextPlay3DVelocity ;
			Sound->ValidNextPlay3DVelocity = 0 ;
		}
		else
		{
			Velocity = &Sound->Base3DVelocity ;
		}
		SoundBuffer_Set3DPosition( &Sound->Buffer[ 0 ], Position ) ;
		SoundBuffer_Set3DRadius(   &Sound->Buffer[ 0 ], Radius ) ;
		SoundBuffer_Set3DVelocity( &Sound->Buffer[ 0 ], Velocity ) ;

		SoundBuffer_Refresh3DSoundParam( &Sound->Buffer[ 0 ] ) ;
	}

	// ����I��
	return 0 ;
}

// �T�E���h�n���h�����쐬����
static int _CreateSoundHandle( int Is3DSound, int NotInitSoundMemDelete, int ASyncThread )
{
	int SoundHandle ;
	SOUND *Sound ;

	SoundHandle = AddHandle( DX_HANDLETYPE_SOUND, ASyncThread, -1 ) ;
	if( SoundHandle == -1 )
		return -1 ;

	if( SOUNDHCHK_ASYNC( SoundHandle, Sound ) )
		return -1 ;

	Sound->Is3DSound = Is3DSound ;
	Sound->NotInitSoundMemDelete = NotInitSoundMemDelete ;

	if( Sound->Is3DSound != FALSE )
	{
		// �N���e�B�J���Z�N�V�����̎擾
		CRITICALSECTION_LOCK( &SoundSysData._3DSoundListCriticalSection ) ;

		// �R�c�T�E���h�`���̏ꍇ�͂R�c�T�E���h�n���h�����X�g�ɒǉ�
		AddHandleList( &SoundSysData._3DSoundListFirst, &Sound->_3DSoundList, SoundHandle, Sound ) ;

		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &SoundSysData._3DSoundListCriticalSection ) ;
	}

	return SoundHandle ;
}

// �X�g���[�����T�E���h�f�[�^�̍Đ��J�n
extern int NS_PlayStreamSoundMem( int SoundHandle , int PlayType, int TopPositionFlag )
{
	SOUND * Sound ;
	SOUNDBUFFER *SBuffer ;

	if( SoundSysData.InitializeFlag == FALSE )
	{
		return -1 ;
	}
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

		// �G���[�I��
		return -1 ;
	}

	// �Đ��̏���
	if( _PlaySetupStreamSoundMem( SoundHandle, Sound, PlayType, TopPositionFlag, &SBuffer ) != 0 )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

		// �G���[�I��
		return -1 ;
	}

	// �Đ��J�n
//	SBuffer->Play( 0 , 0 , D_DSBPLAY_LOOPING ) ; 
	SoundBuffer_Play( SBuffer, TRUE ) ;

	// �R�c�T�E���h�̏ꍇ�͍Đ����̂R�c�T�E���h���X�g�ɒǉ�����
	if( Sound->Is3DSound != FALSE )
	{
		if( Sound->AddPlay3DSoundList == FALSE )
		{
			Sound->AddPlay3DSoundList = TRUE ;

			// �N���e�B�J���Z�N�V�����̎擾
			CRITICALSECTION_LOCK( &SoundSysData.Play3DSoundListCriticalSection ) ;

			AddHandleList( &SoundSysData.Play3DSoundListFirst, &Sound->Play3DSoundList, SoundHandle, Sound ) ;

			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &SoundSysData.Play3DSoundListCriticalSection ) ;
		}
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �Đ��X�e�[�^�X�ɂ���Ă͍Đ��I����҂�
	if( PlayType == DX_PLAYTYPE_NORMAL )
	{
		while( NS_ProcessMessage() == 0 && NS_CheckSoundMem( SoundHandle ) == 1 ){ Thread_Sleep( 1 ) ; }
	}

	// �I��
	return 0 ;
}

// �X�g���[�����T�E���h�f�[�^�̍Đ���Ԃ𓾂�
extern int NS_CheckStreamSoundMem( int SoundHandle )
{
	SOUND * Sound ;
	SOUNDBUFFER *SBuffer ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	// �T�E���h�o�b�t�@�̎g�p���������ꍇ�͏��Ȃ��Ƃ��Đ��͍s���Ă��Ȃ�
	if( GetSoundBuffer( SoundHandle, Sound, &SBuffer, false ) == -1 ) return 0 ;

	// �V�X�e�����Ŏ~�߂Ă���ꍇ�͎~�߂�O�̏�Ԃ�Ԃ�
	if( Sound->BufferPlayStateBackupFlagValid[ 0 ] )
	{
		return Sound->BufferPlayStateBackupFlag[ 0 ] ? 1 : 0 ;
	}
	else
	{
		if( SoundBuffer_CheckPlay( SBuffer ) )
		{
			return 1 ;
		}
	}

	// �����܂ł��Ă���΍Đ��͂���Ă��Ȃ�
	return 0 ;
}



// �X�g���[�����T�E���h�f�[�^�̍Đ��I��
extern int NS_StopStreamSoundMem( int SoundHandle )
{
	SOUND *Sound, *UniSound = NULL ;
	int IsPlay ;
	SOUNDBUFFER *SBuffer ;

	if( SoundSysData.InitializeFlag == FALSE )
	{
		return -1 ;
	}
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

		// �G���[�I��
		return -1 ;
	}

	// �Đ����Ԃ��X�V����
	RefreshStreamSoundPlayCompLength( SoundHandle ) ;

	// �n���h���̎g�p���������ꍇ�͏��Ȃ��Ƃ��Đ����ł͂Ȃ�
	if( GetSoundBuffer( SoundHandle, Sound, &SBuffer, false ) == -1 )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

		return 0 ;
	}
	
	// �Đ����̏ꍇ�͎~�߂�
	if( Sound->Stream.BufferBorrowSoundHandle != -1 )
	{
		if( SOUNDHCHK( Sound->Stream.BufferBorrowSoundHandle, UniSound ) == false )
		{
			UniSound->Stream.BufferUseSoundHandle = -1;
		}
	}

	IsPlay = SoundBuffer_CheckPlay( SBuffer ) ;
	if( IsPlay == -1 )
	{
		return -1 ;
	}
	if( IsPlay )
	{
		SoundBuffer_Stop( SBuffer, TRUE ) ;

		Sound->BufferPlayStateBackupFlagValid[ 0 ] = FALSE ;
		Sound->BufferPlayStateBackupFlag[ 0 ] = FALSE ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �I��
	return 0 ;
}

// SetStreamSoundCurrentPosition �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int SetStreamSoundCurrentPosition_UseGParam( int Byte, int SoundHandle, int ASyncThread )
{
	SOUND * sd ;
//	STREAMFILEDATA * pl ;
//	int i, pos ;
//	int datasize ;
//	WAVEFORMATEX *wf ;

	if( SoundSysData.InitializeFlag == FALSE )
	{
		return -1 ;
	}
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �G���[����
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, sd ) )
			goto ERR ;
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, sd ) )
			goto ERR ;
	}

	if( Byte / sd->BufferFormat.nBlockAlign > sd->Stream.TotalSample )
		goto ERR ;

	// �t�@�C������ȏ�̏ꍇ�͂܂����Ή�
	if( sd->Stream.FileNum > 1 ) goto ERR ;

/*
***	���k�f�[�^�̏ꍇ�͐���Ɍv�Z�ł��Ȃ����� ****

	// �Đ��ʒu�̎Z�o
	pl = sd->Stream.File ;
	pos = Byte ;
	for( i = 0 ; i < sd->Stream.FileNum ; i ++, pl ++ )
	{
		wf = &sd->BufferFormat ;
		datasize = pl->ConvData.DataSize ;
		if( pos < datasize ) break ;
		pos -= datasize ;
	}
	if( i == sd->Stream.FileNum ) return -1 ;
*/
	sd->Stream.FileLoopCount         = 0 ;
	sd->Stream.LoopPositionValidFlag = FALSE ;
//	sd->Stream.FileActive            = i ;
	sd->Stream.FileActive            = 0 ;
	sd->Stream.CompPlayWaveLength    = ( DWORD )Byte ;
	sd->Stream.FileCompCopyLength    = Byte ;

	// ���������t���O��|��
	sd->Stream.StartSetupCompFlag = FALSE ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �I��
	return 0 ;

ERR :
	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	return -1 ;
}

// �T�E���h�n���h���̍Đ��ʒu���o�C�g�P�ʂŕύX����(�Đ����~�܂��Ă��鎞�̂ݗL��)
extern int NS_SetStreamSoundCurrentPosition( int Byte, int SoundHandle )
{
	return SetStreamSoundCurrentPosition_UseGParam( Byte, SoundHandle, FALSE ) ;
}

// �T�E���h�n���h���̍Đ��ʒu���o�C�g�P�ʂŎ擾����
extern int NS_GetStreamSoundCurrentPosition( int SoundHandle )
{
	SOUND * sd ;
//	STREAMFILEDATA * pl ;
	int /*i,*/ pos ;
//	WAVEFORMATEX *wf ;

	if( SoundSysData.InitializeFlag == FALSE )
	{
		return -1 ;
	}
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

		// �G���[�I��
		return -1 ;
	}

	// �Đ��ʒu�̎Z�o
/*	pl = sd->Stream.File ;
	pos = 0 ;
	for( i = 0 ; i < sd->Stream.FileNum ; i ++, pl ++ )
	{
		wf = &sd->BufferFormat ;
		pos += pl->ConvData.DataSize ;
	}
	pos += sd->Stream.FileCompCopyLength ;
*/
	// �Đ����Ԃ̍X�V
	RefreshStreamSoundPlayCompLength( SoundHandle ) ;

	// �Đ����Ԃ̎擾
	pos = ( int )sd->Stream.CompPlayWaveLength ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �l��Ԃ�
	return pos ;
}

// �T�E���h�n���h���̍Đ��ʒu���~���b�P�ʂŐݒ肷��(���k�`���̏ꍇ�͐������ݒ肳��Ȃ��ꍇ������)
extern int NS_SetStreamSoundCurrentTime( int Time, int SoundHandle )
{
	SOUND * sd ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �Đ��ʒu�̕ύX
	return NS_SetStreamSoundCurrentPosition( ( int )( MilliSecPositionToSamplePosition( ( int )sd->BufferFormat.nSamplesPerSec, Time ) * sd->BufferFormat.nBlockAlign ), SoundHandle ) ;
}

// �T�E���h�n���h���̍Đ��ʒu���~���b�P�ʂŎ擾����(���k�`���̏ꍇ�͐������l���Ԃ��Ă��Ȃ��ꍇ������)
extern int NS_GetStreamSoundCurrentTime( int SoundHandle )
{
	SOUND * sd ;
//	STREAMFILEDATA * pl ;
	int /*i,*/ time ;
//	int datasize ;
//	WAVEFORMATEX *wf ;

	if( SoundSysData.InitializeFlag == FALSE )
	{
		return -1 ;
	}
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

		// �G���[�I��
		return -1 ;
	}

	// �Đ����Ԃ̍X�V
	RefreshStreamSoundPlayCompLength( SoundHandle ) ;

	// �Đ����Ԃ��~���b�P�ʂɕϊ�
//	time = _DTOL( (double)sd->Stream.CompPlayWaveLength / sd->BufferFormat.nAvgBytesPerSec * 1000 ) ;
	time = SamplePositionToMilliSecPosition( ( int )sd->BufferFormat.nSamplesPerSec, ( int )( sd->Stream.CompPlayWaveLength / sd->BufferFormat.nBlockAlign ) ) ;
	
	// �Đ����Ԃ��ŏ��Ɏ擾�������̒��������������Ƃ�����̂�
	// �����Đ����Ԃ̕��������Ȃ��Ă��܂����ꍇ�͕␳����
	if( time > NS_GetSoundTotalTime( SoundHandle ) )
		time = NS_GetSoundTotalTime( SoundHandle ) ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �l��Ԃ�
	return time ;
}

// �T�E���h�n���h���̍Đ��������Ԃ��X�V����
extern int RefreshStreamSoundPlayCompLength( int SoundHandle, int CurrentPosition, int ASyncThread )
{
	SOUND * sd ;
	DWORD CurPosition ;
	int UpdateFlag ;
	SOUNDBUFFER *SBuffer ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, sd ) )
			return -1 ;
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, sd ) )
			return -1 ;
	}

	// �X�g���[���Đ��p�f�[�^�ł͂Ȃ��ꍇ�͉��������I��
	if( sd->Type != DX_SOUNDTYPE_STREAMSTYLE ) return 0 ;
	
	// �T�E���h�o�b�t�@�̎g�p���������Ȃ��ꍇ�͏��Ȃ��Ƃ��Đ����ł͂Ȃ�
	if( GetSoundBuffer( SoundHandle, sd, &SBuffer, false ) == -1 ) return 0 ;
	
	// �Đ��������������Ă��Ȃ������I��
	if( sd->Stream.StartSetupCompFlag == FALSE ) return 0 ;

	// ���݂̍Đ��ʒu���擾����
	if( CurrentPosition == -1 )
	{
//		SBuffer->GetCurrentPosition( &CurPosition , NULL );
		SoundBuffer_GetCurrentPosition( SBuffer, &CurPosition , NULL ) ;
	}
	else
	{
		CurPosition = ( DWORD )CurrentPosition ;
	}

	// �O��̈ʒu�Ƃ̍��������Z����
	if( sd->Stream.PrevCheckPlayPosition != CurPosition )
	{
		UpdateFlag = 0 ;

		// ���[�v�w�肪����ꍇ�Ɩ����ꍇ�ŏ����𕪊�
		if( sd->Stream.LoopPositionValidFlag == TRUE )
		{
			if( sd->Stream.PrevCheckPlayPosition < CurPosition )
			{
				if( sd->Stream.LoopPosition > sd->Stream.PrevCheckPlayPosition &&
					sd->Stream.LoopPosition <= CurPosition )
				{
					sd->Stream.CompPlayWaveLength = CurPosition - sd->Stream.LoopPosition ;
					UpdateFlag = 1 ;
				}
			}
			else
			{
				if( sd->Stream.LoopPosition > sd->Stream.PrevCheckPlayPosition )
				{
					sd->Stream.CompPlayWaveLength = sd->Stream.SoundBufferSize - sd->Stream.LoopPosition + CurPosition ;
					UpdateFlag = 1 ;
				}
				else
				if( sd->Stream.LoopPosition <= CurPosition )
				{
					sd->Stream.CompPlayWaveLength = CurPosition - sd->Stream.LoopPosition ;
					UpdateFlag = 1 ;
				}
			}
		}
		if( UpdateFlag == 1 )
		{
			sd->Stream.CompPlayWaveLength += sd->Stream.LoopAfterCompPlayWaveLength ;
			sd->Stream.LoopPositionValidFlag = FALSE ;
		}
		else
		{
			if( sd->Stream.PrevCheckPlayPosition < CurPosition )
			{
				sd->Stream.CompPlayWaveLength += CurPosition - sd->Stream.PrevCheckPlayPosition ;
			}
			else
			{
				sd->Stream.CompPlayWaveLength += ( sd->Stream.SoundBufferSize - sd->Stream.PrevCheckPlayPosition ) + CurPosition ;
			}
		}
		
		// ����擾�����ʒu���Z�b�g����
		sd->Stream.PrevCheckPlayPosition = CurPosition ;
	}

	// �I��
	return 0 ;
}


// ProcessStreamSoundMem �֐��̕⏕�֐�
// �����f�[�^����������
void NoneSoundDataCopy( SOUND * Sound, SOUNDBUFFERLOCKDATA *LockData, DWORD MoveLength )
{
	DWORD MLen, MOff ;
	BYTE *WOff ;

	MOff = 0 ;

	while( MoveLength != 0 )
	{
		MLen = MoveLength ;

		if( LockData->Valid != 0 )
		{
			WOff = LockData->WriteP + LockData->Offset ;

			if( MLen > LockData->Valid ) MLen = LockData->Valid ;

			if( Sound->BufferFormat.wBitsPerSample == 8 )	_MEMSET( WOff, 127, MLen ) ;
			else											_MEMSET( WOff, 0,   MLen ) ;

			LockData->Valid -= MLen ;
			LockData->Offset += MLen ;
			MoveLength -= MLen ;
			MOff += MLen ;
		}
		else
		{
			if( LockData->Valid2 == 0 ) break ;

			WOff = LockData->WriteP2 + LockData->Offset2 ;

			if( MLen > LockData->Valid2 ) MLen = LockData->Valid2 ;

			if( Sound->BufferFormat.wBitsPerSample == 8 )	_MEMSET( WOff, 127, MLen ) ;
			else											_MEMSET( WOff, 0,   MLen ) ;

			LockData->Valid2 -= MLen ;
			LockData->Offset2 += MLen ;
			MoveLength -= MLen ;
			MOff += MLen ;
		}
	}
}


// ProcessStreamSoundMem �֐��̕⏕�֐�
// �����f�[�^����������
int SoundDataCopy( SOUNDBUFFERLOCKDATA *LockData, SOUNDCONV *ConvData, DWORD MoveLength )
{
	DWORD MLen, MAllLen ;
	BYTE *WOff ;

	MAllLen = 0 ;

	while( MoveLength != 0 )
	{
		MLen = MoveLength ;

		if( LockData->Valid != 0 )
		{
			WOff = LockData->WriteP + LockData->Offset ;

			if( MLen > LockData->Valid ) MLen = LockData->Valid ;
			MLen = ( DWORD )RunSoundConvert( ConvData, WOff, ( int )MLen ) ;
			if( MLen == 0 ) break ;

			LockData->Valid -= MLen ;
			LockData->Offset += MLen ;
			MoveLength -= MLen ;
			MAllLen += MLen ;
		}
		else
		{
			WOff = LockData->WriteP2 + LockData->Offset2 ;

			if( MLen > LockData->Valid2 ) MLen = LockData->Valid2 ;
			MLen = ( DWORD )RunSoundConvert( ConvData, WOff, ( int )MLen ) ;
			if( MLen == 0 ) break ;

			LockData->Valid2 -= MLen ;
			LockData->Offset2 += MLen ;
			MoveLength -= MLen ;
			MAllLen += MLen ;
		}
	}
	
	return ( int )MAllLen ;
}

// ProcessStreamSoundMem�֐��̕⏕�֐�
// ���̃t�@�C���Ɉڂ邩�ǂ������`�F�b�N����
int StreamSoundNextData( SOUND * Sound, SOUNDBUFFERLOCKDATA *LockData, int CurrentPosition )
{
	STREAMFILEDATA *PlayData ;
		
	// ���[�v���𑝂₷
	Sound->Stream.FileLoopCount ++ ;

	// �������[�v���ǂ����ŏ����𕪊�
	if( Sound->Stream.File[Sound->Stream.FileActive].LoopNum == -1 )
	{
		WAVEFORMATEX *wfmt = &Sound->BufferFormat ;

		PlayData = &Sound->Stream.File[ Sound->Stream.FileActive ] ;

		// ���Ƀ��[�v�ʒu���Z�b�g���Ă���ꍇ�͈�x���[�v����܂ő҂�
		if( Sound->Stream.LoopPositionValidFlag == TRUE )
			return -1 ;

		// �Đ��ʒu���Z�b�g����
		Sound->Stream.FileCompCopyLength = PlayData->LoopSamplePosition * wfmt->nBlockAlign ;

		// �Đ��A�h���X�X�V�p�����Z�b�g
		Sound->Stream.LoopAfterCompPlayWaveLength = ( DWORD )( PlayData->LoopSamplePosition * wfmt->nBlockAlign ) ;
		Sound->Stream.LoopPositionValidFlag       = TRUE ;
		if( LockData->Valid == 0 )
		{
			Sound->Stream.LoopPosition = LockData->Length2 - LockData->Valid2 ;
		}
		else
		{
			Sound->Stream.LoopPosition = LockData->StartOffst + LockData->Length - LockData->Valid ;
		}
	}
	else
	{
		// ���[�v�񐔂��K��l�ɂ��Ă����玟�̃f�[�^��
		if( Sound->Stream.FileLoopCount > Sound->Stream.File[Sound->Stream.FileActive].LoopNum  )
		{
			// ���[�v�Đ��w��Ń��[�v����������������ꍇ�A���Ƀ��[�v�ʒu���Z�b�g���Ă���ꍇ�͈�x���[�v����܂ő҂�
			if( Sound->Stream.FileNum - 1 == Sound->Stream.FileActive &&
				Sound->PlayType == DX_PLAYTYPE_LOOP &&
				Sound->Stream.LoopPositionValidFlag == TRUE )
				return -1 ;

			Sound->Stream.FileLoopCount = 0 ;
			Sound->Stream.FileActive ++ ;
			if( Sound->Stream.FileNum == Sound->Stream.FileActive )
			{
				Sound->Stream.AllPlayFlag = TRUE ;
			
				if( Sound->PlayType == DX_PLAYTYPE_LOOP )
				{
					Sound->Stream.FileActive = 0  ;
	//				Sound->Stream.CompPlayWaveLength = 0 ;

					Sound->Stream.LoopAfterCompPlayWaveLength = 0 ;
					Sound->Stream.LoopPositionValidFlag = TRUE ;
					if( LockData->Valid == 0 )
					{
						Sound->Stream.LoopPosition = LockData->Length2 - LockData->Valid2 ;
					}
					else
					{
						Sound->Stream.LoopPosition = LockData->StartOffst + LockData->Length - LockData->Valid ;
					}
				}
				else
				{
					// �I���t���O�𗧂Ă�
					Sound->Stream.EndWaitFlag = TRUE ;

					// �I���I�t�Z�b�g���Z�b�g����
					if( LockData->Valid == 0 )
					{
						Sound->Stream.EndOffset = LockData->Length2 - LockData->Valid2 ;
					}
					else
					{
						Sound->Stream.EndOffset = LockData->StartOffst + LockData->Length - LockData->Valid ;
					}
					Sound->Stream.EndStartOffset = ( DWORD )CurrentPosition ;
					NoneSoundDataCopy( Sound, LockData, ( DWORD )AdjustSoundDataBlock( ( int )( Sound->BufferFormat.nAvgBytesPerSec / STS_ONECOPYSEC ), Sound ) ) ;

					return 0 ;
				}
			}
		}

		// �Đ����̏�����
		Sound->Stream.FileCompCopyLength = 0 ;

	}

	// ���k�f�[�^��W�J���鏀�����s��
	PlayData = &Sound->Stream.File[ Sound->Stream.FileActive ] ;
//	SetTimeSoundConvert( &PlayData->ConvData,  Sound->Stream.FileCompCopyLength * 1000 / Sound->BufferFormat.nAvgBytesPerSec ) ;
	SetSampleTimeSoundConvert( &PlayData->ConvData,  Sound->Stream.FileCompCopyLength / Sound->BufferFormat.nBlockAlign ) ;

	// �I��
	return 0 ;
}


// ProcessStreamSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int ProcessStreamSoundMem_UseGParam( int SoundHandle, int ASyncThread )
{
	SOUND * Sound ;
	STREAMFILEDATA *PlayData ;
	DWORD CurPosition = 0 ;
	SOUNDBUFFERLOCKDATA LockData ;
	int MoveByte, MoveByte2, MoveStartOffset ;
	int MoveB ;
	SOUNDBUFFER *SBuffer ;
	int BreakFlag ;

	if( SoundSysData.InitializeFlag == FALSE )
	{
		return -1 ;
	}
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[�`�F�b�N
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, Sound ) )
			return -1 ;
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, Sound ) )
			return -1 ;
	}

	// �Đ��`�����X�g���[���^�C�v����Ȃ������炱���ŏI��
	if( Sound->Type != DX_SOUNDTYPE_STREAMSTYLE ) return 0 ;
	
	// �T�E���h�o�b�t�@�̎g�p�����Ȃ��ꍇ�������ŏI��
	if( GetSoundBuffer( SoundHandle, Sound, &SBuffer, false ) == -1 ) return 0 ;
	
	// �Đ��������������Ă��Ȃ������牽�������I��
	if( Sound->Stream.StartSetupCompFlag == FALSE ) return 0 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �ēx�n���h���`�F�b�N
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, Sound ) )
		{
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

			// �G���[�I��
			return -1 ;
		}
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, Sound ) )
		{
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

			// �G���[�I��
			return -1 ;
		}
	}

	// �Đ��������Ԃ̍X�V
	RefreshStreamSoundPlayCompLength( SoundHandle, -1, ASyncThread ) ;

	// �G���h�t���O�������Ă���ꍇ�͏I�����菈�����s��
	if( Sound->Stream.EndWaitFlag )
	{
		// ���݂̍Đ��ʒu�𓾂�
//		SBuffer->GetCurrentPosition( &CurPosition , NULL );
		SoundBuffer_GetCurrentPosition( SBuffer, &CurPosition, NULL ) ;
  
		// �I������
		if(
		    (
			  Sound->Stream.EndStartOffset   < Sound->Stream.EndOffset &&
			  (
			    Sound->Stream.EndStartOffset > CurPosition ||
				Sound->Stream.EndOffset      < CurPosition
			  )
			) ||
			(
			  Sound->Stream.EndOffset        < CurPosition &&
			  Sound->Stream.EndStartOffset   > CurPosition &&
			  Sound->Stream.EndStartOffset   > Sound->Stream.EndOffset
			)
		  )
		{
			// �Đ����Ƃ߂�
//			SBuffer->Stop() ;
			SoundBuffer_Stop( SBuffer ) ;

			// �G���h�t���O��|��
			Sound->Stream.EndWaitFlag = FALSE ;

			// �Đ����ԏ�������������
			Sound->Stream.FileCompCopyLength = 0 ;
			Sound->Stream.CompPlayWaveLength = 0 ;
			Sound->Stream.LoopPositionValidFlag = FALSE ;
			Sound->Stream.FileLoopCount = 0 ;
			Sound->Stream.FileActive = 0 ;
			Sound->Stream.StartSetupCompFlag = FALSE ;		// �����t���O��|��
		}
		else
		{
			MoveByte = ( int )AdjustSoundDataBlock( ( int )( Sound->BufferFormat.nAvgBytesPerSec / STS_ONECOPYSEC ), Sound ) ;
			MoveStartOffset = Sound->Stream.SoundBufferCompCopyOffset ;

			// �]�����ׂ����ǂ����̔���
			{
				DWORD C, S ;

//				SBuffer->GetCurrentPosition( &CurPosition , NULL );
				SoundBuffer_GetCurrentPosition( SBuffer, &CurPosition, NULL ) ;

				C = ( DWORD )Sound->Stream.SoundBufferCompCopyOffset ;
				S = ( DWORD )Sound->Stream.SoundBufferCopyStartOffset ;
				if( ( C < S && ( S > CurPosition && C < CurPosition ) ) || 
					( C > S && ( S > CurPosition || C < CurPosition ) ) )
				{
					goto ERR ;
				}
			}

			MoveB = MoveByte ;

			// ���b�N����
			{
				LockData.StartOffst = ( DWORD )MoveStartOffset ;

				if( Sound->Stream.SoundBufferCompCopyOffset > (int)SOUNDSIZE( Sound->BufferFormat.nAvgBytesPerSec * STS_BUFSEC / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ) )
					Sound->Stream.SoundBufferCompCopyOffset = Sound->Stream.SoundBufferCompCopyOffset ;

//				SBuffer->Lock( MoveStartOffset, MoveByte,
//								( void ** )&(LockData.WriteP), &LockData.Length,
//								( void ** )&(LockData.WriteP2), &LockData.Length2, 0 ) ; 
				SoundBuffer_Lock( SBuffer, ( DWORD )MoveStartOffset, ( DWORD )MoveByte,
								( void ** )&(LockData.WriteP), &LockData.Length,
								( void ** )&(LockData.WriteP2), &LockData.Length2 ) ; 
				LockData.Offset  = 0 ;
				LockData.Offset2 = 0 ;
				LockData.Valid  = LockData.Length ;
				LockData.Valid2 = LockData.Length2 ;
			}

			// �f�[�^�]������
			while( MoveByte != 0 )
			{
				// �g�p����f�[�^���Z�b�g
				PlayData = &Sound->Stream.File[ Sound->Stream.FileActive ] ;

				// �]���o�C�g�̃Z�b�g
				MoveByte2 = MoveByte ;

				// �]��
				NoneSoundDataCopy( Sound, &LockData, ( DWORD )MoveByte2 ) ;
				MoveByte -= MoveByte2 ;
			}

			// ���b�N����
//			SBuffer->Unlock( ( void * )LockData.WriteP, LockData.Length,
//							( void * )LockData.WriteP2, LockData.Length2 ) ; 
			SoundBuffer_Unlock( SBuffer, ( void * )LockData.WriteP, LockData.Length,
										( void * )LockData.WriteP2, LockData.Length2 ) ; 

			// ���ɓ]���������s���I�t�Z�b�g�̕ύX
			{
				Sound->Stream.SoundBufferCompCopyOffset += MoveB ;
				if( Sound->Stream.SoundBufferCompCopyOffset >= (int)SOUNDSIZE( Sound->BufferFormat.nAvgBytesPerSec * STS_BUFSEC / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ) )
					Sound->Stream.SoundBufferCompCopyOffset -= (int)SOUNDSIZE( Sound->BufferFormat.nAvgBytesPerSec * STS_BUFSEC / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ) ;

				Sound->Stream.SoundBufferCopyStartOffset = ( int )( Sound->Stream.SoundBufferCompCopyOffset - SOUNDSIZE( Sound->BufferFormat.nAvgBytesPerSec * STS_SAKICOPYSEC / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ) ) ;
				if( Sound->Stream.SoundBufferCopyStartOffset < 0 )
					Sound->Stream.SoundBufferCopyStartOffset += SOUNDSIZE( Sound->BufferFormat.nAvgBytesPerSec * STS_BUFSEC / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ) ;
			}
		}

		// �I��
		goto END ;
	}
	else
	{
		// �]����������
		{
			// ���݉��t����Ă��邩�`�F�b�N

			// ���������ł��Ȃ��A���t���ł��Ȃ��ꍇ�͂����ŏI��
			if( SoundBuffer_CheckPlay( SBuffer ) == FALSE )
			{
				if( Sound->Stream.SoundBufferCompCopyOffset == -800 )
				{
					MoveByte = AdjustSoundDataBlock( ( int )( Sound->BufferFormat.nAvgBytesPerSec * STS_SAKICOPYSEC / STS_DIVNUM ), Sound ) ;
					MoveStartOffset = 0 ;
				}
				else
				{
					goto END ;
				}
			}
			else
			{
				DWORD SakiyomiSec ;
				MoveByte = AdjustSoundDataBlock( ( int )( Sound->BufferFormat.nAvgBytesPerSec / STS_ONECOPYSEC ), Sound ) ;
				MoveStartOffset = Sound->Stream.SoundBufferCompCopyOffset ;

				// �]�����ׂ����ǂ����̔���A���݂̐�ǂ݂̕b���̎Z�o
				{
					DWORD C, S ;

//					SBuffer->GetCurrentPosition( &CurPosition , NULL );
					SoundBuffer_GetCurrentPosition( SBuffer, &CurPosition, NULL ) ;
					
					C = ( DWORD )Sound->Stream.SoundBufferCompCopyOffset ;
					S = ( DWORD )Sound->Stream.SoundBufferCopyStartOffset ;
					if( ( C < S && ( S > CurPosition && C < CurPosition ) ) || 
						( C > S && ( S > CurPosition || C < CurPosition ) ) )
					{
						goto ERR ;
					}

					if( C < S )
					{
						if( CurPosition < C ) CurPosition += SOUNDSIZE( STS_BUFSEC * Sound->BufferFormat.nAvgBytesPerSec / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ) ;
						C += SOUNDSIZE( STS_BUFSEC * Sound->BufferFormat.nAvgBytesPerSec / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ) ;
					}
					SakiyomiSec = ( C - CurPosition ) * 0x200 / Sound->BufferFormat.nAvgBytesPerSec ;
				}

				// ��ǂݕb�����K���������Ă�����K��̕b���܂ň�C�ɓ]��
				if( SakiyomiSec < STS_MINSAKICOPYSEC * 0x200 / STS_DIVNUM )
				{
					MoveByte = AdjustSoundDataBlock( ( int )( Sound->BufferFormat.nAvgBytesPerSec * ( STS_MINSAKICOPYSEC * 0x200 / STS_DIVNUM - SakiyomiSec ) / 0x200 ), Sound ) ;
				}
			}
		}

		MoveB = MoveByte ;

		// ���b�N����
		{
			LockData.StartOffst = ( DWORD )MoveStartOffset ;

//			SBuffer->Lock( MoveStartOffset, MoveByte,
//							( void ** )&(LockData.WriteP), &LockData.Length,
//							( void ** )&(LockData.WriteP2), &LockData.Length2, 0 ) ; 
			SoundBuffer_Lock( SBuffer, ( DWORD )MoveStartOffset, ( DWORD )MoveByte,
							( void ** )&(LockData.WriteP), &LockData.Length,
							( void ** )&(LockData.WriteP2), &LockData.Length2 ) ; 
			LockData.Offset  = 0 ;
			LockData.Offset2 = 0 ;
			LockData.Valid  = LockData.Length ;
			LockData.Valid2 = LockData.Length2 ;
		}

		// �f�[�^�]������
		while( MoveByte != 0 && Sound->Stream.EndWaitFlag == FALSE )
		{
			// �g�p����f�[�^���Z�b�g
			PlayData = &Sound->Stream.File[ Sound->Stream.FileActive ] ;

			// �]���o�C�g�̃Z�b�g
			MoveByte2 = MoveByte ;

			// ���[�v�J�n�ʒu���ݒ肳��Ă���ꍇ�͂��̈ʒu����܂ł̓f�[�^���Z�b�g���Ȃ�
			if( PlayData->LoopStartSamplePosition != -1 && MoveByte2 + Sound->Stream.FileCompCopyLength > PlayData->LoopStartSamplePosition * Sound->BufferFormat.nBlockAlign )
			{
				MoveByte2 = PlayData->LoopStartSamplePosition * Sound->BufferFormat.nBlockAlign - Sound->Stream.FileCompCopyLength ;
				if( MoveByte2 <= 0 )
					MoveByte2 = 0 ;
			}

			// �]��
			MoveByte2 = SoundDataCopy( &LockData, &PlayData->ConvData, ( DWORD )MoveByte2 ) ;

			// �]���ʂ��O�o�C�g�̏ꍇ�͎��̃t�@�C���Ɉڂ�
			BreakFlag = FALSE ;
			if( MoveByte2 == 0 )
			{
//				CurPosition = 0;
				if( StreamSoundNextData( Sound, &LockData, ( int )CurPosition ) < 0 )
				{
					BreakFlag = TRUE ;
				}
			}
			else
			{
				MoveByte -= MoveByte2 ;
				Sound->Stream.FileCompCopyLength += MoveByte2 ;
			}

			// ���[�v���ׂ��ʒu�ɗ��Ă���̂Ɏ��̍Đ��f�[�^�Ɉڍs���Ă��Ȃ��ꍇ�͍Đ����[�v�҂�( Sound->Stream.LoopPositionValidFlag �� TRUE )�̏�ԂȂ̂ŁA���[�v���甲����
			if( ( PlayData == &Sound->Stream.File[ Sound->Stream.FileActive ] &&
				  PlayData->LoopStartSamplePosition != -1 &&
				  Sound->Stream.FileCompCopyLength >= PlayData->LoopStartSamplePosition * Sound->BufferFormat.nBlockAlign ) ||
				BreakFlag )
				break ;
		}
		MoveB -= MoveByte ;

		// ���b�N����
//		SBuffer->Unlock( ( void * )LockData.WriteP,  LockData.Length,
//						  ( void * )LockData.WriteP2, LockData.Length2 ) ; 
		SoundBuffer_Unlock( SBuffer, ( void * )LockData.WriteP,  LockData.Length,
						  			( void * )LockData.WriteP2, LockData.Length2 ) ; 

		// ���ɓ]���������s���I�t�Z�b�g�̕ύX
		if( Sound->Stream.SoundBufferCompCopyOffset == -800 )
		{
//			Sound->Stream.SoundBufferCompCopyOffset = SOUNDSIZE( Sound->BufferFormat.nAvgBytesPerSec * STS_SAKICOPYSEC / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ) ;
			Sound->Stream.SoundBufferCompCopyOffset = MoveB ;
			Sound->Stream.SoundBufferCopyStartOffset = 0 ;
		}
		else
		{
			Sound->Stream.SoundBufferCompCopyOffset += MoveB ;
			if( Sound->Stream.SoundBufferCompCopyOffset >= (int)SOUNDSIZE( Sound->BufferFormat.nAvgBytesPerSec * STS_BUFSEC / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ) )
				Sound->Stream.SoundBufferCompCopyOffset -= (int)SOUNDSIZE( Sound->BufferFormat.nAvgBytesPerSec * STS_BUFSEC / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ) ;

			Sound->Stream.SoundBufferCopyStartOffset = ( int )( Sound->Stream.SoundBufferCompCopyOffset - SOUNDSIZE( Sound->BufferFormat.nAvgBytesPerSec * STS_SAKICOPYSEC / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ) ) ;
			if( Sound->Stream.SoundBufferCopyStartOffset < 0 )
				Sound->Stream.SoundBufferCopyStartOffset += SOUNDSIZE( Sound->BufferFormat.nAvgBytesPerSec * STS_BUFSEC / STS_DIVNUM, Sound->BufferFormat.nBlockAlign ) ;
		}
	}

END :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �I��
	return 0 ;

ERR :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �G���[�I��
	return -1 ;
}

// �S�Ẳ��̈ꎞ��~��Ԃ�ύX����
extern int PauseSoundMemAll( int PauseFlag )
{
	HANDLELIST *List ;
	SOUND *Sound ;
	int i ;

	if( SoundSysData.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// ��~���邩�J�n���邩�ŏ����𕪊�
	if( PauseFlag )
	{
		// ��~����ꍇ

		for( List = HandleManageArray[ DX_HANDLETYPE_SOUND ].ListFirst.Next ; List->Next != NULL ; List = List->Next )
		{
			Sound = ( SOUND * )List->Data ;

			for( i = 0 ; i < MAX_SOUNDBUFFER_NUM ; i ++ )
			{
				if( Sound->Buffer[ i ].Valid == FALSE || SoundBuffer_CheckEnable( &Sound->Buffer[ i ] ) == FALSE )
					continue ;

				// ���ɏ�ԕۑ��ς݂̏ꍇ�͉������Ȃ�
				if( Sound->BufferPlayStateBackupFlagValid[ i ] )
					continue ;

				// �T�E���h�o�b�t�@�̍Đ���Ԃ�ۑ�
				{
					int IsPlay ;

					IsPlay = SoundBuffer_CheckPlay( &Sound->Buffer[ i ] ) ;
					if( IsPlay != -1 )
					{
						Sound->BufferPlayStateBackupFlagValid[ i ] = TRUE ;

						if( IsPlay )
						{
							Sound->BufferPlayStateBackupFlag[ i ] = TRUE ;

							// �Đ�����Ă�����Đ����~�߂�
							SoundBuffer_Stop( &Sound->Buffer[ i ], TRUE ) ;
						}
						else
						{
							Sound->BufferPlayStateBackupFlag[ i ] = FALSE ;
						}
					}
					else
					{
						Sound->BufferPlayStateBackupFlagValid[ i ] = FALSE ;
					}
				}
			}
		}
	}
	else
	{
		// �ĊJ����ꍇ

		for( List = HandleManageArray[ DX_HANDLETYPE_SOUND ].ListFirst.Next ; List->Next != NULL ; List = List->Next )
		{
			Sound = ( SOUND * )List->Data ;

			for( i = 0 ; i < MAX_SOUNDBUFFER_NUM ; i ++ )
			{
				if( Sound->Buffer[ i ].Valid == FALSE || SoundBuffer_CheckEnable( &Sound->Buffer[ i ] ) == FALSE ) continue ;

				// �T�E���h�o�b�t�@�̍Đ���Ԃ��L���ŁA���Đ����Ă����ꍇ�͍Đ����ĊJ����
				if( Sound->BufferPlayStateBackupFlagValid[ i ] &&
					Sound->BufferPlayStateBackupFlag[ i ] )
				{
					SoundBuffer_Play( &Sound->Buffer[ i ], Sound->Buffer[ i ].Loop ) ;
				}

				Sound->BufferPlayStateBackupFlagValid[ i ] = FALSE ;
			}
		}
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �I��
	return 0 ;
}

// �X�g���[���T�E���h�̍Đ������֐�
extern int NS_ProcessStreamSoundMem( int SoundHandle )
{
	return ProcessStreamSoundMem_UseGParam( SoundHandle, FALSE ) ;
}

// �L���ȃX�g���[���T�E���h�̂��ׂčĐ������֐��ɂ�����
extern int NS_ProcessStreamSoundMemAll( void )
{
	HANDLELIST *List ;
	SOUND *Sound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &SoundSysData.StreamSoundListCriticalSection ) ;

	for( List = SoundSysData.StreamSoundListFirst.Next ; List->Next != NULL ; List = List->Next )
	{
		// �Đ��������������Ă��Ȃ������牽����������
		Sound = ( SOUND * )List->Data ;
		if( Sound->Stream.StartSetupCompFlag == FALSE ) continue ;

		NS_ProcessStreamSoundMem( List->Handle ) ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &SoundSysData.StreamSoundListCriticalSection ) ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �I��
	return 0 ;
}

// �Đ����̂��ׂĂ̂R�c�T�E���h�̃p�����[�^���X�V����
extern int Refresh3DSoundParamAll()
{
	HANDLELIST *List ;
	SOUND *Sound ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &SoundSysData._3DSoundListCriticalSection ) ;

	for( List = SoundSysData._3DSoundListFirst.Next ; List->Next != NULL ; List = List->Next )
	{
		Sound = ( SOUND * )List->Data ;

		// �o�b�t�@�[�̐������J��Ԃ�
		for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
		{
			// �p�����[�^�X�V�\��
			Sound->Buffer[ i ].EmitterDataChangeFlag = TRUE ;

			// �Đ����������瑦���ɍX�V
			if( SoundBuffer_CheckPlay( &Sound->Buffer[ i ] ) )
			{
				SoundBuffer_Refresh3DSoundParam( &Sound->Buffer[ i ] ) ;
			}
		}
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &SoundSysData._3DSoundListCriticalSection ) ;

	// �I��
	return 0 ;
}

// �Đ����I��������T�E���h�n���h�����폜����T�E���h�̏������s��
extern int ProcessPlayFinishDeleteSoundMemAll( void )
{
	HANDLELIST *List ;
//	SOUND *Sound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

LOOPSTART:

	for( List = SoundSysData.PlayFinishDeleteSoundListFirst.Next ; List->Next != NULL ; List = List->Next )
	{
		// �Đ����ł͂Ȃ�������폜
		if( NS_CheckSoundMem( List->Handle ) == 0 )
		{
			NS_DeleteSoundMem( List->Handle ) ;
			goto LOOPSTART ;
		}
	}

	// �I��
	return 0 ;
}

// �R�c�T�E���h���Đ����Ă���T�E���h�n���h���ɑ΂��鏈�����s��
extern int ProcessPlay3DSoundMemAll( void )
{
	HANDLELIST *List ;
	SOUND *Sound ;
	int i ;
	int Valid ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &SoundSysData.Play3DSoundListCriticalSection ) ;

LOOPSTART:
	for( List = SoundSysData.Play3DSoundListFirst.Next ; List->Next != NULL ; List = List->Next )
	{
		Sound = ( SOUND * )List->Data ;

		Valid = FALSE ;
		for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
		{
			if( Sound->Buffer[ i ].Valid == 0 )
			{
				continue ;
			}

			if( SoundBuffer_CycleProcess( &Sound->Buffer[ i ] ) == 0 )
			{
				Valid = TRUE ;
			}
		}

		if( Valid == FALSE )
		{
			Sound->AddPlay3DSoundList = FALSE ;
			SubHandleList( &Sound->Play3DSoundList ) ;
			goto LOOPSTART ;
		}
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &SoundSysData.Play3DSoundListCriticalSection ) ;

	// �I��
	return 0 ;
}

// LoadSoundMem2 �̎������֐�
extern int LoadSoundMem2_Static(
	LOADSOUND_GPARAM *GParam,
	int SoundHandle,
	const wchar_t *WaveName1,
	const wchar_t *WaveName2,
	int ASyncThread
)
{
	if( CheckSoundSystem_Initialize_PF() == FALSE )
		return -1 ;

	// ��ڂ̃T�E���h�f�[�^�̒ǉ�
	if( AddStreamSoundMemToFile_UseGParam( GParam, WaveName1, 0,  SoundHandle, GParam->CreateSoundDataType, -1, ASyncThread ) == -1 ) goto ERR ;

	// �Q�ڂ̃T�E���h�f�[�^�̒ǉ�
	if( AddStreamSoundMemToFile_UseGParam( GParam, WaveName2, -1, SoundHandle, GParam->CreateSoundDataType, -1, ASyncThread ) == -1 ) goto ERR ;

	// �Đ�����
	SetupStreamSoundMem_UseGParam( SoundHandle, ASyncThread ) ;

	// ����I��
	return 0 ;

ERR :
	// �G���[�I��
	return -1 ;
}

#ifndef DX_NON_ASYNCLOAD

// LoadSoundMem2 �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void LoadSoundMem2_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	LOADSOUND_GPARAM *GParam ;
	int SoundHandle ;
	const wchar_t *WaveName1 ;
	const wchar_t *WaveName2 ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	GParam = ( LOADSOUND_GPARAM * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	SoundHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	WaveName1 = GetASyncLoadParamString( AParam->Data, &Addr ) ;
	WaveName2 = GetASyncLoadParamString( AParam->Data, &Addr ) ;

	Result = LoadSoundMem2_Static( GParam, SoundHandle, WaveName1, WaveName2, TRUE ) ;
	DecASyncLoadCount( SoundHandle ) ;
	if( Result < 0 )
	{
		SubHandle( SoundHandle ) ;
	}
}

#endif // DX_NON_ASYNCLOAD

// LoadSoundMem2 �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int LoadSoundMem2_UseGParam(
	LOADSOUND_GPARAM *GParam,
	const wchar_t *WaveName1,
	const wchar_t *WaveName2,
	int ASyncLoadFlag
)
{
	int SoundHandle = -1 ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
		return -1 ;

	SoundHandle = _CreateSoundHandle( GParam->Create3DSoundFlag, GParam->NotInitSoundMemDelete, FALSE ) ;
	if( SoundHandle == -1 )
		goto ERR ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;
		wchar_t FullPath1[ 1024 ] ;
		wchar_t FullPath2[ 1024 ] ;

		ConvertFullPathW_( WaveName1, FullPath1, sizeof( FullPath1 ) ) ;
		ConvertFullPathW_( WaveName2, FullPath2, sizeof( FullPath2 ) ) ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamStruct( NULL, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( NULL, &Addr, SoundHandle ) ;
		AddASyncLoadParamString( NULL, &Addr, FullPath1 ) ; 
		AddASyncLoadParamString( NULL, &Addr, FullPath2 ) ; 

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = LoadSoundMem2_ASync ;
		Addr = 0 ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SoundHandle ) ;
		AddASyncLoadParamString( AParam->Data, &Addr, FullPath1 ) ; 
		AddASyncLoadParamString( AParam->Data, &Addr, FullPath2 ) ; 

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( SoundHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( LoadSoundMem2_Static( GParam, SoundHandle, WaveName1, WaveName2, FALSE ) < 0 )
			goto ERR ;
	}

	// �n���h����Ԃ�
	return SoundHandle ;

ERR :
	SubHandle( SoundHandle ) ;
	SoundHandle = -1 ;

	// �I��
	return -1 ;
}

// �O�t���ƃ��[�v���ɕ����ꂽ�T�E���h�f�[�^�̍쐬
extern int NS_LoadSoundMem2( const TCHAR *WaveName1 , const TCHAR *WaveName2 )
{
#ifdef UNICODE
	return LoadSoundMem2_WCHAR_T(
		WaveName1 , WaveName2
	) ;
#else
	int Result = -1 ;

	TCHAR_TO_WCHAR_T_STRING_BEGIN( WaveName1 )
	TCHAR_TO_WCHAR_T_STRING_BEGIN( WaveName2 )

	TCHAR_TO_WCHAR_T_STRING_SETUP( WaveName1, goto ERR )
	TCHAR_TO_WCHAR_T_STRING_SETUP( WaveName2, goto ERR )

	Result = LoadSoundMem2_WCHAR_T(
		UseWaveName1Buffer , UseWaveName2Buffer
	) ;

ERR :

	TCHAR_TO_WCHAR_T_STRING_END( WaveName1 )
	TCHAR_TO_WCHAR_T_STRING_END( WaveName2 )

	return Result ;
#endif
}

// �O�t���ƃ��[�v���ɕ����ꂽ�T�E���h�f�[�^�̍쐬
extern int LoadSoundMem2_WCHAR_T( const wchar_t *WaveName1 , const wchar_t *WaveName2 )
{
	LOADSOUND_GPARAM GParam ;

	InitLoadSoundGParam( &GParam ) ;

	return LoadSoundMem2_UseGParam( &GParam, WaveName1, WaveName2, GetASyncLoadFlag() ) ;
}

// ��ɂa�f�l��ǂݍ��ނ̂ɓK�����֐�
extern int NS_LoadBGM( const TCHAR *WaveName )
{
#ifdef UNICODE
	return LoadBGM_WCHAR_T(
		WaveName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( WaveName, return -1 )

	Result = LoadBGM_WCHAR_T(
		UseWaveNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( WaveName )

	return Result ;
#endif
}

// ��ɂa�f�l��ǂݍ��ނ̂ɓK�����֐�
extern int LoadBGM_WCHAR_T( const wchar_t *WaveName )
{
	int Type = SoundSysData.CreateSoundDataType, SoundHandle ;

	if( _WCSICMP( WaveName + _WCSLEN( WaveName ) - 3, L"wav" ) == 0 )
	{
		NS_SetCreateSoundDataType( DX_SOUNDDATATYPE_FILE ) ;
	}
	else
	{
		NS_SetCreateSoundDataType( DX_SOUNDDATATYPE_MEMPRESS ) ;
	}

	SoundHandle = LoadSoundMem_WCHAR_T( WaveName, 1 ) ;
	NS_SetCreateSoundDataType( Type ) ;

	return SoundHandle ;
}

// LoadSoundMem2ByMemImage �̎������֐�
static int LoadSoundMem2ByMemImage_Static( 
	LOADSOUND_GPARAM *GParam,
	int SoundHandle,
	const void *FileImageBuffer1,
	int ImageSize1,
	const void *FileImageBuffer2,
	int ImageSize2,
	int ASyncThread
)
{
	if( CheckSoundSystem_Initialize_PF() == FALSE )
		return -1 ;

	// ��ڂ̃T�E���h�f�[�^�̒ǉ�
	if( AddStreamSoundMemToMem_UseGParam( GParam, FileImageBuffer1, ImageSize1,  0, SoundHandle, GParam->CreateSoundDataType, -1, ASyncThread ) == -1 ) goto ERR ;

	// �Q�ڂ̃T�E���h�f�[�^�̒ǉ�
	if( AddStreamSoundMemToMem_UseGParam( GParam, FileImageBuffer2, ImageSize2, -1, SoundHandle, GParam->CreateSoundDataType, -1, ASyncThread ) == -1 ) goto ERR ;

	// �Đ�����
	SetupStreamSoundMem_UseGParam( SoundHandle, ASyncThread ) ;

	// ����I��
	return 0 ;

ERR :
	// �G���[�I��
	return -1 ;
}

#ifndef DX_NON_ASYNCLOAD
// LoadSoundMem2ByMemImage �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void LoadSoundMem2ByMemImage_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	LOADSOUND_GPARAM *GParam ;
	int SoundHandle ;
	const void *FileImageBuffer1 ;
	int ImageSize1 ;
	const void *FileImageBuffer2 ;
	int ImageSize2 ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	GParam = ( LOADSOUND_GPARAM * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	SoundHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	FileImageBuffer1 = GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	ImageSize1 = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	FileImageBuffer2 = GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	ImageSize2 = GetASyncLoadParamInt( AParam->Data, &Addr ) ;

	Result = LoadSoundMem2ByMemImage_Static( GParam, SoundHandle, FileImageBuffer1, ImageSize1, FileImageBuffer2, ImageSize2, TRUE ) ;
	DecASyncLoadCount( SoundHandle ) ;
	if( Result < 0 )
	{
		SubHandle( SoundHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// LoadSoundMem2ByMemImage �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int LoadSoundMem2ByMemImage_UseGParam(
	LOADSOUND_GPARAM *GParam,
	const void *FileImageBuffer1,
	int ImageSize1,
	const void *FileImageBuffer2,
	int ImageSize2,
	int ASyncLoadFlag
)
{
	int SoundHandle = -1 ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
		return -1 ;

	SoundHandle = _CreateSoundHandle( GParam->Create3DSoundFlag, GParam->NotInitSoundMemDelete, FALSE ) ;
	if( SoundHandle == -1 )
		goto ERR ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamStruct( NULL, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( NULL, &Addr, SoundHandle ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, FileImageBuffer1 ) ; 
		AddASyncLoadParamInt( NULL, &Addr, ImageSize1 ) ; 
		AddASyncLoadParamConstVoidP( NULL, &Addr, FileImageBuffer2 ) ; 
		AddASyncLoadParamInt( NULL, &Addr, ImageSize2 ) ; 

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = LoadSoundMem2ByMemImage_ASync ;
		Addr = 0 ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SoundHandle ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, FileImageBuffer1 ) ; 
		AddASyncLoadParamInt( AParam->Data, &Addr, ImageSize1 ) ; 
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, FileImageBuffer2 ) ; 
		AddASyncLoadParamInt( AParam->Data, &Addr, ImageSize2 ) ; 

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( SoundHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( LoadSoundMem2ByMemImage_Static( GParam, SoundHandle, FileImageBuffer1, ImageSize1, FileImageBuffer2, ImageSize2, FALSE ) < 0 )
			goto ERR ;
	}

	// �n���h����Ԃ�
	return SoundHandle ;

ERR :
	SubHandle( SoundHandle ) ;
	SoundHandle = -1 ;

	// �I��
	return -1 ;
}

// �O�t���ƃ��[�v���ɕ����ꂽ�T�E���h�f�[�^�̍쐬
extern int NS_LoadSoundMem2ByMemImage( const void *FileImageBuffer1, int ImageSize1, const void *FileImageBuffer2, int ImageSize2 )
{
	LOADSOUND_GPARAM GParam ;

	InitLoadSoundGParam( &GParam ) ;

	return LoadSoundMem2ByMemImage_UseGParam( &GParam, FileImageBuffer1, ImageSize1, FileImageBuffer2, ImageSize2, GetASyncLoadFlag() ) ;
}


	
// LOADSOUND_GPARAM �̃f�[�^���Z�b�g����
extern void InitLoadSoundGParam( LOADSOUND_GPARAM *GParam )
{
	GParam->NotInitSoundMemDelete = FALSE ;																// InitSoundMem �ō폜���Ȃ����ǂ����̃t���O( TRUE:InitSoundMem�ł͍폜���Ȃ�  FALSE:InitSoundMem�ō폜���� )
	GParam->Create3DSoundFlag					= SoundSysData.Create3DSoundFlag ;						// 3D�T�E���h���쐬���邩�ǂ����̃t���O( TRUE:�R�c�T�E���h���쐬����  FALSE:�R�c�T�E���h���쐬���Ȃ� )
	GParam->CreateSoundDataType					= SoundSysData.CreateSoundDataType ;					// �쐬����T�E���h�f�[�^�̃f�[�^�^�C�v
	GParam->CreateSoundPitchRateEnable			= SoundSysData.CreateSoundPitchRateEnable ;				// �쐬����T�E���h�f�[�^�̃s�b�`���[�g���L�����ǂ���
	GParam->CreateSoundPitchRate				= SoundSysData.CreateSoundPitchRate ;					// �쐬����T�E���h�f�[�^�̃s�b�`���[�g
	GParam->CreateSoundTimeStretchRateEnable	= SoundSysData.CreateSoundTimeStretchRateEnable ;		// �쐬����T�E���h�f�[�^�̃^�C���X�g���b�`���[�g���L�����ǂ���
	GParam->CreateSoundTimeStretchRate			= SoundSysData.CreateSoundTimeStretchRate ;				// �쐬����T�E���h�f�[�^�̃^�C���X�g���b�`���[�g
	GParam->CreateSoundLoopStartTimePosition	= SoundSysData.CreateSoundLoopStartTimePosition ;		// �쐬����T�E���h�f�[�^�̃��[�v�͈͂̐�[( �~���b )
	GParam->CreateSoundLoopStartSamplePosition	= SoundSysData.CreateSoundLoopStartSamplePosition ;		// �쐬����T�E���h�f�[�^�̃��[�v�͈͂̐�[( �T���v�� )
	GParam->CreateSoundLoopEndTimePosition		= SoundSysData.CreateSoundLoopEndTimePosition ;			// �쐬����T�E���h�f�[�^�̃��[�v�͈͂̏I�[( �~���b )
	GParam->CreateSoundLoopEndSamplePosition	= SoundSysData.CreateSoundLoopEndSamplePosition ;		// �쐬����T�E���h�f�[�^�̃��[�v�͈͂̏I�[( �T���v�� )

	// ���[�v�ʒu�̎w�肪����ꍇ�̓X�g���[���T�E���h�Ƃ��ēǂݍ���
	if( GParam->CreateSoundDataType == DX_SOUNDDATATYPE_MEMNOPRESS &&
		( GParam->CreateSoundLoopStartTimePosition   != GParam->CreateSoundLoopEndTimePosition ||
		  GParam->CreateSoundLoopStartSamplePosition != GParam->CreateSoundLoopEndSamplePosition ) )
	{
		GParam->CreateSoundDataType = DX_SOUNDDATATYPE_MEMPRESS ;
	}

	GParam->DisableReadSoundFunctionMask = SoundSysData.DisableReadSoundFunctionMask ;		// �g�p���Ȃ��ǂݍ��ݏ����̃}�X�N
#ifndef DX_NON_OGGVORBIS
	GParam->OggVorbisBitDepth = SoundSysData.OggVorbisBitDepth ;							// �n�����u�����������g�p���̃r�b�g�[�x(1:8bit 2:16bit)
	GParam->OggVorbisFromTheoraFile = SoundSysData.OggVorbisFromTheoraFile ;				// Ogg Theora �t�@�C������ Vorbis �f�[�^���Q�Ƃ��邩�ǂ����̃t���O( TRUE:Theora �t�@�C������ Vorbis �f�[�^���Q�Ƃ��� )
#endif
}

// �s�b�`�ύX��^�C���X�g���b�`���̐L�k�ύX�T�E���h�n���h�����Z�b�g�A�b�v����w���p�[�֐�
static int SetupSoundPitchRateTimeStretchRateChangeHandle( int SoundHandle, LOADSOUND_GPARAM *GParam, int SSSHandleI, int BufferNum, int ASyncThread )
{
	int SSSHandle = -1 ;
	int DSSHandle = -1 ;
	void *WaveImage = NULL ;
	int WaveSize ;
	int DSSSampleNum ;
	SOFTSOUND * SSSound = NULL ;
	LOADSOUND_GPARAM TempGParam ;
	int ErrorPos = -1 ;

	SSND_MASKHCHK( SSSHandleI, SSSound ) ;

	// float�^�̃\�t�g�E�G�A�T�E���h���쐬
	SSSHandle = NS_MakeSoftSoundCustom( SSSound->BufferFormat.nChannels, 32, SSSound->BufferFormat.nSamplesPerSec, SSSound->Wave.BufferSampleNum, TRUE ) ;
	if( SSSHandle < 0 )
	{
		ErrorPos = -1 ;
		goto ERR ;
	}

	// ����float�^�̃\�t�g�E�G�A�T�E���h�ł͂Ȃ�������float�^�̃\�t�g�E�G�A�T�E���h�ɕϊ�
	if( SSSound->BufferFormat.wFormatTag != WAVE_FORMAT_IEEE_FLOAT )
	{
		ConvertIntToFloatSoftSound( SSSHandleI, SSSHandle ) ;
	}
	else
	{
		// ����float�^�̃\�t�g�E�G�A�T�E���h�̏ꍇ�̓R�s�[��p��
		CopySoftSound( SSSHandleI, SSSHandle ) ;
	}

	// �s�b�`�ύX������ꍇ
	if( GParam->CreateSoundPitchRateEnable )
	{
		// �Q�{�ȏ�Ɏ��g����������ꍇ�͂Q�{�ȉ��ɂȂ�܂łQ�{�P�ʂŕϊ����s��
		float PitchRate = GParam->CreateSoundPitchRate ;

		if( PitchRate > 2.0f )
		{
			DSSSampleNum = _FTOL( NS_GetSoftSoundSampleNum( SSSHandle ) * 2.0f ) ;
			DSSHandle = NS_MakeSoftSound( SSSHandle, DSSSampleNum ) ;
			if( DSSHandle < 0 )
			{
				ErrorPos = -2 ;
				goto ERR ;
			}

			while( PitchRate > 2.0f )
			{
				NS_WriteTimeStretchSoftSoundData( SSSHandle, DSSHandle ) ;
				NS_WritePitchShiftSoftSoundData(  DSSHandle, SSSHandle ) ;

				PitchRate /= 2.0f ;
			}

			NS_DeleteSoftSound( DSSHandle ) ;
			DSSHandle = -1 ;
		}

		DSSSampleNum = _FTOL( NS_GetSoftSoundSampleNum( SSSHandle ) * PitchRate ) ;
		DSSHandle = NS_MakeSoftSound( SSSHandle, DSSSampleNum ) ;
		if( DSSHandle < 0 )
		{
			ErrorPos = -3 ;
			goto ERR ;
		}

		NS_WriteTimeStretchSoftSoundData( SSSHandle, DSSHandle ) ;
		NS_WritePitchShiftSoftSoundData(  DSSHandle, SSSHandle ) ;
	}

	// �^�C���X�g���b�`������ꍇ
	if( GParam->CreateSoundTimeStretchRateEnable )
	{
		int Temp ;

		DSSSampleNum = _FTOL( NS_GetSoftSoundSampleNum( SSSHandle ) * GParam->CreateSoundTimeStretchRate ) ;
		DSSHandle = NS_MakeSoftSound( SSSHandle, DSSSampleNum ) ;
		if( DSSHandle < 0 )
		{
			ErrorPos = -4 ;
			goto ERR ;
		}

		NS_WriteTimeStretchSoftSoundData( SSSHandle, DSSHandle ) ;

		Temp = SSSHandle ;
		SSSHandle = DSSHandle ;
		DSSHandle = Temp ;
	}

	if( SSND_MASKHCHK( SSSHandle, SSSound ) )
	{
		ErrorPos = -5 ;
		goto ERR ;
	}

	// �v�`�u�d�t�@�C�����ł����グ��
	if( CreateWaveFileImage(
			&WaveImage,
			&WaveSize,
			&SSSound->BufferFormat,
			sizeof( WAVEFORMATEX ),
			SSSound->Wave.Buffer,
			SSSound->Wave.BufferSampleNum * SSSound->BufferFormat.nBlockAlign ) < 0 )
	{
		ErrorPos = -6 ;
		goto ERR ;
	}

	TempGParam = *GParam ;
	TempGParam.CreateSoundDataType				= DX_SOUNDDATATYPE_MEMNOPRESS ;
	TempGParam.CreateSoundPitchRateEnable		= FALSE ;
	TempGParam.CreateSoundPitchRate				= 1.0f ;
	TempGParam.CreateSoundTimeStretchRateEnable = FALSE ;
	TempGParam.CreateSoundTimeStretchRate       = 1.0f ;
	if( LoadSoundMemByMemImageBase_UseGParam( &TempGParam, FALSE, SoundHandle, WaveImage, WaveSize, BufferNum, -1, FALSE, ASyncThread ) < 0 )
	{
		ErrorPos = -7 ;
		goto ERR ;
	}

	// �������̉��
	DXFREE( WaveImage ) ;
	WaveImage = NULL ;

	NS_DeleteSoftSound( SSSHandle ) ;
	NS_DeleteSoftSound( DSSHandle ) ;
	SSSHandle = -1 ;
	DSSHandle = -1 ;

	// ����I��
	return 0 ;

	// �G���[�I��
ERR :
	if( WaveImage != NULL )
	{
		DXFREE( WaveImage ) ;
		WaveImage = NULL ;
	}

	if( SSSHandle >= 0 )
	{
		NS_DeleteSoftSound( SSSHandle ) ;
		SSSHandle = -1 ;
	}

	if( DSSHandle >= 0 )
	{
		NS_DeleteSoftSound( DSSHandle ) ;
		DSSHandle = -1 ;
	}

	return ErrorPos ;
}

// ���[�v�͈͎w��p�e�L�X�g�t�@�C���̃t�@�C���p�X���쐬����
static int CreateSoundLoopAreaTxtFilePath( const wchar_t *Path, wchar_t *Dest, size_t BufferBytes )
{
	int   i, j ;
	int   LastPoint ;
	DWORD CharCode ;
	int   CharBytes ;

	// ��Ԍ��� . �̈ʒu�𒲂ׂ�
	LastPoint = -1 ;
	i = 0 ;
	for(;;)
	{
		CharCode = GetCharCode( ( const char * )&( ( BYTE * )Path )[ i ], WCHAR_T_CHARCODEFORMAT, &CharBytes ) ;
		if( CharCode == '\0' )
		{
			break ;
		}

		if( CharCode == '.' )
		{
			LastPoint = i ;
		}

		i += CharBytes ;
	}

	// ���[�v�͈͎w��p�e�L�X�g�t�@�C���l�[���̍쐬
	i = 0 ;
	for(;;)
	{
		CharCode = GetCharCode( ( const char * )&( ( BYTE * )Path )[ i ], WCHAR_T_CHARCODEFORMAT, &CharBytes ) ;
		if( CharCode == '\0' || i == LastPoint )
		{
			break ;
		}

		PutCharCode( CharCode, WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ i ], BufferBytes - i ) ;
		i += CharBytes ;
	}

	j = i ;
	j += PutCharCode( '_',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;
	j += PutCharCode( 'l',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;
	j += PutCharCode( 'o',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;
	j += PutCharCode( 'o',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;
	j += PutCharCode( 'p',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;
	j += PutCharCode( '.',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;
	j += PutCharCode( 't',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;
	j += PutCharCode( 'x',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;
	j += PutCharCode( 't',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;
	j += PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ j ], BufferBytes - j ) ;

	return 0 ;
}

// ���[�v�͈͎w��p�e�L�X�g�t�@�C����ǂݍ���
static int LoadSoundLoopAreaTxtFile( const wchar_t *LoopFilePath, int *LoopStart, int *LoopEnd )
{
	BYTE *FileImage ;
	int FileSize ;
	int Result = -1 ;
	wchar_t WCharStr[ 256 ] = { 0 } ;
	wchar_t *p, *p2 ;
	int LoopStartPos ;
	int LoopEndPos ;
	int IsLoopLength ;

	// �t�@�C�����ۂ��Ɠǂݍ���
	if( FileFullRead( LoopFilePath, ( void ** )&FileImage, &FileSize, FALSE ) < 0 )
	{
		return -1 ;
	}

	if( FileSize < 3 )
		goto END ;

	// UTF-16LE UTF-16BE UTF-8 �� Ascii �R�[�h�ɕϊ�
	{
		int Count = 0 ;

		if( FileSize > Count + 0 && FileImage[ Count + 0 ] == 0xff &&
			FileSize > Count + 1 && FileImage[ Count + 1 ] == 0xfe )
		{
			Count += 2 ;
			ConvString( ( char * )&FileImage[ Count ], DX_CHARCODEFORMAT_UTF16LE, ( char * )WCharStr, sizeof( WCharStr ) - 32, WCHAR_T_CHARCODEFORMAT ) ;
		}
		else
		if( FileSize > Count + 0 && FileImage[ Count + 0 ] == 0xfe &&
			FileSize > Count + 1 && FileImage[ Count + 1 ] == 0xff )
		{
			Count += 2 ;
			ConvString( ( char * )&FileImage[ Count ], DX_CHARCODEFORMAT_UTF16BE, ( char * )WCharStr, sizeof( WCharStr ) - 32, WCHAR_T_CHARCODEFORMAT ) ;
		}
		else
		if( FileSize > Count + 0 && FileImage[ Count + 0 ] == 0xef &&
			FileSize > Count + 1 && FileImage[ Count + 1 ] == 0xbb &&
			FileSize > Count + 2 && FileImage[ Count + 2 ] == 0xbf )
		{
			Count += 3 ;
			ConvString( ( char * )&FileImage[ Count ], DX_CHARCODEFORMAT_UTF8, ( char * )WCharStr, sizeof( WCharStr ) - 32, WCHAR_T_CHARCODEFORMAT ) ;
		}
		else
		{
			ConvString( ( char * )&FileImage[ Count ], DX_CHARCODEFORMAT_SHIFTJIS, ( char * )WCharStr, sizeof( WCharStr ) - 32, WCHAR_T_CHARCODEFORMAT ) ;
		}
	}

	// LOOPSTART ��ǂݍ���
	{
		for( p = WCharStr ; *p != L'\0' && *p != L'L' ; p++ ){}
		if( CL_strncmp( WCHAR_T_CHARCODEFORMAT, ( char * )p, ( char * )L"LOOPSTART", 9 ) != 0 )
		{
			goto END ;
		}
		p += 9 ;

		for( ; *p != L'\0' && *p != L'=' ; p++ ){}
		if( *p == L'\0' )
		{
			goto END ;
		}
		for( ; *p != L'\0' && ( *p < L'0' || *p > L'9' ) ; p++ ){}
		if( *p == L'\0' )
		{
			goto END ;
		}

		for( p2 = p ; *p2 >= L'0' && *p2 <= L'9' ; p2++ ){}
		if( *p2 == L'\0' )
		{
			goto END ;
		}
		*p2 = L'\0' ;
		LoopStartPos = CL_atoi( WCHAR_T_CHARCODEFORMAT, ( char * )p ) ;
	}

	// LOOPEND �܂��� LOOPLENGTH ��ǂݍ���
	{
		for( p = p2 + 1 ; *p != L'\0' && *p != L'L' ; p++ ){}
		if( CL_strncmp( WCHAR_T_CHARCODEFORMAT, ( char * )p, ( char * )L"LOOPEND", 7 ) == 0 )
		{
			IsLoopLength = FALSE ;
			p += 7 ;
		}
		else
		if( CL_strncmp( WCHAR_T_CHARCODEFORMAT, ( char * )p, ( char * )L"LOOPLENGTH", 10 ) == 0 )
		{
			IsLoopLength = TRUE ;
			p += 10 ;
		}
		else
		{
			goto END ;
		}

		for( ; *p != L'\0' && *p != L'=' ; p++ ){}
		if( *p == L'\0' )
		{
			goto END ;
		}
		for( ; *p != L'\0' && ( *p < L'0' || *p > L'9' ) ; p++ ){}
		if( *p == L'\0' )
		{
			goto END ;
		}

		for( p2 = p ; *p2 >= L'0' && *p2 <= L'9' ; p2++ ){}
		*p2 = '\0' ;
		if( IsLoopLength )
		{
			LoopEndPos = LoopStartPos + CL_atoi( WCHAR_T_CHARCODEFORMAT, ( char * )p ) ;
		}
		else
		{
			LoopEndPos = CL_atoi( WCHAR_T_CHARCODEFORMAT, ( char * )p ) ;
		}
	}

	// �p�����[�^�̃`�F�b�N
	if( LoopStartPos >= LoopEndPos )
	{
		goto END ;
	}

	// �l�̕ۑ�
	if( LoopStart ) *LoopStart = LoopStartPos ;
	if( LoopEnd   ) *LoopEnd   = LoopEndPos ;

	// ����I���l���Z�b�g
	Result = 0 ;

END :

	// �������̉��
	DXFREE( FileImage ) ;
	FileImage = NULL ;

	// �I��
	return Result ;
}

// LoadSoundMemBase �̎������֐�
static int LoadSoundMemBase_Static(
	LOADSOUND_GPARAM *GParam,
	int SoundHandle,
	const wchar_t *WaveName,
	int BufferNum,
	int UnionHandle,
	int ASyncThread
)
{
	void *SrcBuffer = NULL ;
	int SrcSize ;
	int SSSHandleI = -1 ;
	int SetLoopAreaCancel = FALSE ;
	wchar_t LoopPosTxtName[ FILEPATH_MAX ] ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
		return -1 ;

	if( BufferNum > MAX_SOUNDBUFFER_NUM ) BufferNum = MAX_SOUNDBUFFER_NUM ;
	
	// ���[�v�͈͏��e�L�X�g�t�@�C���̓ǂݍ���
	if( ( GParam->CreateSoundLoopStartTimePosition   == GParam->CreateSoundLoopEndTimePosition   ) &&
		( GParam->CreateSoundLoopStartSamplePosition == GParam->CreateSoundLoopEndSamplePosition ) )
	{
		int LoopStartPos ;
		int LoopEndPos ;

		CreateSoundLoopAreaTxtFilePath( WaveName, LoopPosTxtName, sizeof( LoopPosTxtName ) ) ;
		if( LoadSoundLoopAreaTxtFile( LoopPosTxtName, &LoopStartPos, &LoopEndPos ) >= 0 )
		{
			GParam->CreateSoundLoopStartSamplePosition = LoopStartPos ;
			GParam->CreateSoundLoopEndSamplePosition   = LoopEndPos ;

			// ���[�v�ʒu�̎w�肪�������ꍇ�̓X�g���[���T�E���h�Ƃ��ēǂݍ���
			if( GParam->CreateSoundDataType == DX_SOUNDDATATYPE_MEMNOPRESS )
			{
				GParam->CreateSoundDataType = DX_SOUNDDATATYPE_MEMPRESS ;
			}
		}
	}

	// �s�b�`�ύX���^�C���X�g���b�`������ꍇ�͐L�k���ēǂݍ���
	if( GParam->CreateSoundPitchRateEnable || GParam->CreateSoundTimeStretchRateEnable )
	{
		int Result ;

		SSSHandleI = LoadSoftSound_WCHAR_T( WaveName ) ;
		if( SSSHandleI < 0 )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Sound File Load Error 2 : %s" , WaveName )) ;
			goto ERR ;
		}

		Result = SetupSoundPitchRateTimeStretchRateChangeHandle( SoundHandle, GParam, SSSHandleI, BufferNum, ASyncThread ) ;
		if( Result < 0 )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Sound File Setup Error %d : %s" , -Result, WaveName )) ;
			goto ERR ;
		}

		NS_DeleteSoftSound( SSSHandleI ) ;
		SSSHandleI = -1 ;
	}
	else
	{
		// �f�[�^�^�C�v���������ɓW�J����ȊO�̏ꍇ�A�X�g���[���f�[�^�Ƃ��ăT�E���h�f�[�^���쐬����
		if( GParam->CreateSoundDataType != DX_SOUNDDATATYPE_MEMNOPRESS )
		{
			// �X�g���[���n�̃T�E���h�f�[�^�̒ǉ�
			if( AddStreamSoundMemToFile_UseGParam( GParam, WaveName, 0, SoundHandle, GParam->CreateSoundDataType, UnionHandle, ASyncThread ) == -1 )
				goto ERR ;

			// �Đ�����
			SetupStreamSoundMem_UseGParam( SoundHandle, ASyncThread ) ;
		}
		else
		{
			// �ہX�������ɓǂݍ���
			if( FileFullRead( WaveName, &SrcBuffer, &SrcSize ) < 0 )
			{
				DXST_ERRORLOGFMT_ADDW(( L"Sound File Load Error : %s", WaveName )) ;
				goto ERR ;
			}

			// ����������ǂݍ��ފ֐��ɓn��
			if( LoadSoundMemByMemImageBase_UseGParam( GParam, FALSE, SoundHandle, SrcBuffer, SrcSize, BufferNum, -1, FALSE, ASyncThread ) < 0 )
			{
				DXST_ERRORLOGFMT_ADDW(( L"Sound File Setup Error : %s" , WaveName )) ;
				goto ERR ;
			}

			// LoadSoundMemByMemImageBase_UseGParam �̒��Ń��[�v�ʒu�w�������̂ŁA
			// LoadSoundMemByMemImageBase_UseGParam �����s�����ꍇ�̓��[�v�ʒu�w�菈���̓L�����Z������
			SetLoopAreaCancel = TRUE ;

			// �������̉��
			DXFREE( SrcBuffer ) ;
			SrcBuffer = NULL ;
		}
	}

	// ���[�v�ʒu�̎w�肪����ꍇ�̓��[�v�ʒu��ݒ肷��
	if( SetLoopAreaCancel == FALSE )
	{
		if( GParam->CreateSoundLoopStartTimePosition != GParam->CreateSoundLoopEndTimePosition )
		{
			SetLoopAreaTimePosSoundMem_UseGParam( 
				GParam->CreateSoundLoopStartTimePosition,
				GParam->CreateSoundLoopEndTimePosition,
				SoundHandle,
				ASyncThread
			) ;
		}
		else
		if( GParam->CreateSoundLoopStartSamplePosition != GParam->CreateSoundLoopEndSamplePosition )
		{
			SetLoopAreaSamplePosSoundMem_UseGParam( 
				GParam->CreateSoundLoopStartSamplePosition,
				GParam->CreateSoundLoopEndSamplePosition,
				SoundHandle,
				ASyncThread
			) ;
		}
	}

	// ����I��
	return 0 ;

ERR :
	if( SrcBuffer != NULL )
	{
		DXFREE( SrcBuffer ) ;
		SrcBuffer = NULL ;
	}

	if( SSSHandleI >= 0 )
	{
		NS_DeleteSoftSound( SSSHandleI ) ;
		SSSHandleI = -1 ;
	}

	return -1 ;
}


#ifndef DX_NON_ASYNCLOAD
// LoadSoundMemBase �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void LoadSoundMemBase_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	LOADSOUND_GPARAM *GParam ;
	int SoundHandle ;
	const wchar_t *WaveName ;
	int BufferNum ;
	int UnionHandle ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	GParam = ( LOADSOUND_GPARAM * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	SoundHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	WaveName = GetASyncLoadParamString( AParam->Data, &Addr ) ;
	BufferNum = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	UnionHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;

	Result = LoadSoundMemBase_Static( GParam, SoundHandle, WaveName, BufferNum, UnionHandle, TRUE ) ;
	DecASyncLoadCount( SoundHandle ) ;
	if( Result < 0 )
	{
		SubHandle( SoundHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// LoadSoundMemBase �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int LoadSoundMemBase_UseGParam(
	LOADSOUND_GPARAM *GParam,
	const wchar_t *WaveName,
	int BufferNum,
	int UnionHandle,
	int ASyncLoadFlag,
	int ASyncThread 
)
{
	int SoundHandle = -1 ;
	wchar_t FullPath[ 1024 ] ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
		return -1 ;

	SoundHandle = _CreateSoundHandle( GParam->Create3DSoundFlag, GParam->NotInitSoundMemDelete, ASyncThread ) ;
	if( SoundHandle == -1 )
		goto ERR ;

	ConvertFullPathW_( WaveName, FullPath, sizeof( FullPath ) ) ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag && ASyncThread == FALSE )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamStruct( NULL, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( NULL, &Addr, SoundHandle ) ;
		AddASyncLoadParamString( NULL, &Addr, FullPath ) ; 
		AddASyncLoadParamInt( NULL, &Addr, BufferNum ) ;
		AddASyncLoadParamInt( NULL, &Addr, UnionHandle ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = LoadSoundMemBase_ASync ;
		Addr = 0 ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SoundHandle ) ;
		AddASyncLoadParamString( AParam->Data, &Addr, FullPath ) ; 
		AddASyncLoadParamInt( AParam->Data, &Addr, BufferNum ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, UnionHandle ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( SoundHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( LoadSoundMemBase_Static( GParam, SoundHandle, FullPath, BufferNum, UnionHandle, ASyncThread ) < 0 )
			goto ERR ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( SoundHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	// �n���h����Ԃ�
	return SoundHandle ;

ERR :
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( SoundHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	SubHandle( SoundHandle ) ;
	SoundHandle = -1 ;

	// �I��
	return -1 ;
}

// �T�E���h�f�[�^��ǉ�����
extern int NS_LoadSoundMemBase( const TCHAR *WaveName, int BufferNum, int UnionHandle )
{
#ifdef UNICODE
	return LoadSoundMemBase_WCHAR_T(
		WaveName, BufferNum, UnionHandle
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( WaveName, return -1 )

	Result = LoadSoundMemBase_WCHAR_T(
		UseWaveNameBuffer, BufferNum, UnionHandle
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( WaveName )

	return Result ;
#endif
}

// �T�E���h�f�[�^��ǉ�����
extern int LoadSoundMemBase_WCHAR_T( const wchar_t *WaveName, int BufferNum, int UnionHandle )
{
	LOADSOUND_GPARAM GParam ;

	InitLoadSoundGParam( &GParam ) ;

	return LoadSoundMemBase_UseGParam( &GParam, WaveName, BufferNum, UnionHandle, GetASyncLoadFlag() ) ;
}

// �T�E���h�f�[�^��ǉ�����
extern int NS_LoadSoundMem( const TCHAR *WaveName, int BufferNum, int UnionHandle )
{
#ifdef UNICODE
	return LoadSoundMem_WCHAR_T(
		WaveName, BufferNum, UnionHandle
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( WaveName, return -1 )

	Result = LoadSoundMem_WCHAR_T(
		UseWaveNameBuffer, BufferNum, UnionHandle
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( WaveName )

	return Result ;
#endif
}

// �T�E���h�f�[�^��ǉ�����
extern int LoadSoundMem_WCHAR_T( const wchar_t *WaveName, int BufferNum, int UnionHandle )
{
	return LoadSoundMemBase_WCHAR_T( WaveName, BufferNum, UnionHandle ) ;
}

// �����Đ����w��^�T�E���h�ǉ��֐�
extern int NS_LoadSoundMemToBufNumSitei( const TCHAR *WaveName, int BufferNum )
{
#ifdef UNICODE
	return LoadSoundMemToBufNumSitei_WCHAR_T(
		WaveName, BufferNum
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( WaveName, return -1 )

	Result = LoadSoundMemToBufNumSitei_WCHAR_T(
		UseWaveNameBuffer, BufferNum
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( WaveName )

	return Result ;
#endif
}

// �����Đ����w��^�T�E���h�ǉ��֐�
extern int LoadSoundMemToBufNumSitei_WCHAR_T( const wchar_t *WaveName, int BufferNum )
{
	return LoadSoundMemBase_WCHAR_T( WaveName, BufferNum, -1 ) ;
}

// �����T�E���h�f�[�^���g�p����T�E���h�n���h�����쐬����
extern int NS_DuplicateSoundMem( int SrcSoundHandle, int BufferNum )
{
	int i ;
	int Handle = -1 ;
	int hr ;
	void *write1,  *write2 ;
	DWORD length1, length2 ;
	SOUND *Sound, *SrcSound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
		return -1 ;

	// �G���[����
	if( SOUNDHCHK( SrcSoundHandle, SrcSound ) )
		goto ERR ;

	// �T�E���h�^�C�v���X�g���[�����T�E���h�������ꍇ�͕����ł��Ȃ�
	if( SrcSound->Type == DX_SOUNDTYPE_STREAMSTYLE )
		goto ERR ;

	if( BufferNum > MAX_SOUNDBUFFER_NUM ) BufferNum = MAX_SOUNDBUFFER_NUM ;

	// �V�����T�E���h�f�[�^�̒ǉ�
	Handle = _CreateSoundHandle( SrcSound->Is3DSound, SrcSound->NotInitSoundMemDelete, FALSE ) ;
	if( Handle < 0 )
	{
		goto ERR ;
	}

	Sound = ( SOUND * )HandleManageArray[ DX_HANDLETYPE_SOUND ].Handle[ Handle & DX_HANDLEINDEX_MASK ] ;

	// �t�H�[�}�b�g��ۑ�
	Sound->BufferFormat = SrcSound->BufferFormat ;

	// WaveData �̃|�C���^��ۑ�
	Sound->Normal.WaveData = SrcSound->Normal.WaveData ;
	Sound->Normal.WaveSize = SrcSound->Normal.WaveSize ;

	// WaveData �̎Q�Ɛ��𑝂₷
	Sound->Normal.WaveDataUseCount = SrcSound->Normal.WaveDataUseCount ;
	*Sound->Normal.WaveDataUseCount = *Sound->Normal.WaveDataUseCount + 1 ;

	// �R�c�T�E���h�̏ꍇ�̓o�b�t�@���͂P
	if( Sound->Is3DSound )
		BufferNum = 1 ;

	// �T�E���h�o�b�t�@���쐬
	if( ( CreateSoundBuffer( &Sound->BufferFormat, ( DWORD )Sound->Normal.WaveSize, DX_SOUNDTYPE_NORMAL, BufferNum, Handle, SrcSoundHandle, FALSE ) ) == -1 )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x07\x89\xfd\x88\xb5\x30\xa6\x30\xf3\x30\xc9\x30\x6e\x30\x20\x00\x44\x00\x69\x00\x72\x00\x65\x00\x63\x00\x74\x00\x53\x00\x6f\x00\x75\x00\x6e\x00\x64\x00\x42\x00\x75\x00\x66\x00\x66\x00\x65\x00\x72\x00\x20\x00\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�����T�E���h�̃T�E���h�o�b�t�@�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
		goto ERR ;
	}
	Sound->BaseFrequency   = ( int )Sound->BufferFormat.nSamplesPerSec ;

	Sound->PitchRateEnable = SrcSound->PitchRateEnable ;
	Sound->PitchRate       = SrcSound->PitchRate ;

	// �T�E���h�f�[�^���T�E���h�o�b�t�@�ɃR�s�[����
	for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
	{
		// �ʂ̃T�E���h�o�b�t�@�̕����������ꍇ�͓]����Ƃ̕K�v�������̂œ]�����Ȃ�
		// ( ����ɓ��삵�Ȃ��������������߂Ƃ肠�����t���O�͖��� )
//		if( Sound->Normal.BufferDuplicateFlag[ i ] == TRUE ) continue ;

		hr = SoundBuffer_Lock( &Sound->Buffer[ i ], 0, ( DWORD )Sound->Normal.WaveSize,
								&write1, &length1,
								&write2, &length2 ) ;
		if( hr != 0 )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xe2\x8e\x01\x90\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5f\x00\x31\x00\x00"/*@ L"�T�E���h�f�[�^�̓]���Ɏ��s���܂���_1" @*/ ) ;
			goto ERR ;
		}

		_MEMCPY( write1, Sound->Normal.WaveData, length1 ) ;
		if( write2 != 0 ) _MEMCPY( write2, (BYTE *)Sound->Normal.WaveData + length1, length2 ) ;

		hr = SoundBuffer_Unlock( &Sound->Buffer[ i ], write1, length1, write2, length2 ) ;
		if( hr != 0 )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xe2\x8e\x01\x90\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5f\x00\x32\x00\x00"/*@ L"�T�E���h�f�[�^�̓]���Ɏ��s���܂���_2" @*/ ) ;
			goto ERR ;
		}
	}

	// �I���A�T�E���h�f�[�^�n���h����Ԃ�
	return Handle ;

ERR :
	if( Handle != -1 )
	{
		NS_DeleteSoundMem( Handle ) ;
	}
	
	return -1 ;
}

// LoadSoundMemByMemImageBase �̎������֐�
static int LoadSoundMemByMemImageBase_Static(
	LOADSOUND_GPARAM *GParam,
	int SoundHandle,
	const void *FileImageBuffer,
	int ImageSize,
	int BufferNum,
	int UnionHandle,
	int ASyncThread
)
{
	int i ;
	WAVEFORMATEX Format ;
	void *SoundBuffer ;
	int SoundSize ;
	int hr ;
	void *write1, *write2 ;
	DWORD length1, length2 ;
	SOUND * Sound ;
	SOUNDCONV ConvData ;
	STREAMDATA Stream ;
	int SSSHandleI = -1 ;

	if( CheckSoundSystem_Initialize_PF() == FALSE || BufferNum > MAX_SOUNDBUFFER_NUM )
		return -1 ;

//	_MEMSET( &ConvData, 0, sizeof( ConvData ) );
	ConvData.InitializeFlag = FALSE ;
	Stream.DataPoint = NULL ;
	SoundBuffer = NULL ;

	Sound = ( SOUND * )HandleManageArray[ DX_HANDLETYPE_SOUND ].Handle[ SoundHandle & DX_HANDLEINDEX_MASK ] ;

//	// �R�c�T�E���h�̏ꍇ�̓o�b�t�@�̐��͈����
//	if( Sound->Is3DSound != FALSE )
//		BufferNum = 1 ;

	// �s�b�`�ύX���^�C���X�g���b�`������ꍇ�͐L�k���ēǂݍ���
	if( GParam->CreateSoundPitchRateEnable || GParam->CreateSoundTimeStretchRateEnable )
	{
		int Result ;

		SSSHandleI = LoadSoftSoundBase_UseGParam( GParam, NULL, FileImageBuffer, ImageSize, ASyncThread ) ;
		if( SSSHandleI < 0 )
		{
			DXST_ERRORLOGFMT_ADDW(( L"LoadSoundMemByMemImageBase Error 1" )) ;
			goto ERR ;
		}

		Result = SetupSoundPitchRateTimeStretchRateChangeHandle( SoundHandle, GParam, SSSHandleI, BufferNum, ASyncThread ) ;
		if( Result < 0 )
		{
			DXST_ERRORLOGFMT_ADDW(( L"LoadSoundMemByMemImageBase Error %d" , -Result )) ;
			goto ERR ;
		}

		NS_DeleteSoftSound( SSSHandleI ) ;
		SSSHandleI = -1 ;

		// ����I��
		return 0 ;
	}

	// �f�[�^�^�C�v���������ɓW�J����ȊO�̏ꍇ�A�X�g���[���f�[�^�Ƃ��ăT�E���h�f�[�^���쐬����
	if( GParam->CreateSoundDataType != DX_SOUNDDATATYPE_MEMNOPRESS )
	{
STREAM_TYPE :
		if( AddStreamSoundMemToMem_UseGParam(
				GParam,
				FileImageBuffer,
				ImageSize,
				0,
				SoundHandle,
				GParam->CreateSoundDataType,
				UnionHandle,
				ASyncThread
			) == -1 )
			goto ERR ;

		// �Đ�����
		SetupStreamSoundMem_UseGParam( SoundHandle, ASyncThread ) ;
	}
	else
	{
		int LoopStartPos = -1, LoopEndPos = -1 ;

		// �o�b�l�`���ɕϊ�
		{
			Stream.DataPoint = MemStreamOpen( FileImageBuffer, ( size_t )ImageSize ) ;
			Stream.ReadShred = *GetMemStreamDataShredStruct() ;
			if( SetupSoundConvert( &ConvData, &Stream, GParam->DisableReadSoundFunctionMask
#ifndef DX_NON_OGGVORBIS
								,GParam->OggVorbisBitDepth, GParam->OggVorbisFromTheoraFile
#endif
								) < 0 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xf3\x97\xf0\x58\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\x30\xff\x23\xff\x2d\xff\x78\x30\x6e\x30\x09\x59\xdb\x63\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�����t�@�C���̂o�b�l�ւ̕ϊ��Ɏ��s���܂���" @*/ )) ;
				goto ERR ;
			}
			if( SoundConvertFast( &ConvData, &Format, &SoundBuffer, &SoundSize ) < 0 )
			{
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xf3\x97\xf0\x58\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�����t�@�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���" @*/ )) ;
				goto ERR ;
			}

			// ���[�v���̎擾
			if( ( GParam->CreateSoundLoopStartTimePosition   == GParam->CreateSoundLoopEndTimePosition   ) &&
				( GParam->CreateSoundLoopStartSamplePosition == GParam->CreateSoundLoopEndSamplePosition ) )
			{
				GetSoundConvertLoopAreaInfo( &ConvData, &LoopStartPos, &LoopEndPos ) ;
			}

			TerminateSoundConvert( &ConvData ) ;
			MemStreamClose( Stream.DataPoint ) ;
			Stream.DataPoint = NULL ;
		}

		// ���[�v��񂪂���ꍇ�̓Z�b�g���ăX�g���[���`���ōĐ�
		if( LoopStartPos >= 0 && LoopEndPos >= 0 )
		{
			GParam->CreateSoundLoopStartSamplePosition = LoopStartPos ;
			GParam->CreateSoundLoopEndSamplePosition   = LoopEndPos ;

			if( SoundBuffer != NULL )
			{
				DXFREE( SoundBuffer ) ;
				SoundBuffer = NULL ;
			}

			GParam->CreateSoundDataType = DX_SOUNDDATATYPE_MEMPRESS ;
			goto STREAM_TYPE ;
		}

		// 3���ȏ�̃T�E���h�f�[�^�̓X�g���[���`���ōĐ�����
		if( SoundSize / Format.nAvgBytesPerSec > 60 * 3 )
		{
			if( SoundBuffer != NULL )
			{
				DXFREE( SoundBuffer ) ;
				SoundBuffer = NULL ;
			}

			GParam->CreateSoundDataType = DX_SOUNDDATATYPE_MEMPRESS ;
			goto STREAM_TYPE ;
		}

		// �T�E���h�o�b�t�@���쐬
//		DXST_ERRORLOGFMT_ADDW(( L"format:%d  channel:%d  samplespersec:%d  AvgBytes:%d  blockalign:%d  BitsPerSample:%d  cbsize:%d\n",
//				Format.wFormatTag, Format.nChannels, Format.nSamplesPerSec, Format.nAvgBytesPerSec, Format.nBlockAlign, Format.wBitsPerSample, Format.cbSize )) ;
		if( ( CreateSoundBuffer( &Format, ( DWORD )SoundSize, DX_SOUNDTYPE_NORMAL, BufferNum, SoundHandle, -1, ASyncThread ) ) == -1 )
		{
			// �T�E���h�o�b�t�@�̍쐬�Ɏ��s������X�g���[���`���ŊJ�����Ƃ��Ă݂�
			if( SoundBuffer != NULL )
			{
				DXFREE( SoundBuffer ) ;
				SoundBuffer = NULL ;
			}

			GParam->CreateSoundDataType = DX_SOUNDDATATYPE_MEMPRESS ;
			goto STREAM_TYPE ;
		}
		Sound->BaseFrequency = ( int )Format.nSamplesPerSec ;
//		Sound->PitchRateEnable = GParam->CreateSoundPitchRateEnable ;
//		Sound->PitchRate       = GParam->CreateSoundPitchRate ;

		// �Q�Ɛ���ۑ����郁�����̊m��
		Sound->Normal.WaveDataUseCount = ( int * )DXALLOC( sizeof( int ) ) ;
		if( Sound->Normal.WaveDataUseCount == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xc2\x53\x67\x71\x70\x65\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�T�E���h�f�[�^�̎Q�Ɛ���ۑ����郁�����̈�̊m�ۂɎ��s���܂���" @*/ )) ;
			goto ERR ;
		}

		Sound->BufferFormat = Format ;			// �t�H�[�}�b�g��ۑ�
		Sound->Normal.WaveData = SoundBuffer ;	// �f�[�^�|�C���^��ۑ�
		Sound->Normal.WaveSize = SoundSize ;	// �f�[�^�T�C�Y��ۑ�
		*Sound->Normal.WaveDataUseCount = 1 ;
		SoundBuffer = NULL ;

		// �T�E���h�f�[�^���T�E���h�o�b�t�@�ɃR�s�[����
		{
			for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
			{
				// �ʂ̃T�E���h�o�b�t�@�̕����������ꍇ�͓]����Ƃ̕K�v�������̂œ]�����Ȃ�
				if( Sound->Normal.BufferDuplicateFlag[ i ] == TRUE ) continue ;
			
				hr = SoundBuffer_Lock( &Sound->Buffer[ i ], 0, ( DWORD )SoundSize,
										&write1, &length1,
										&write2, &length2 ) ;
				if( hr != 0 )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xe2\x8e\x01\x90\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5f\x00\x31\x00\x00"/*@ L"�T�E���h�f�[�^�̓]���Ɏ��s���܂���_1" @*/ ) ;
					goto ERR ;
				}

				_MEMCPY( write1, Sound->Normal.WaveData, length1 ) ;
				if( write2 != 0 ) _MEMCPY( write2, (BYTE *)Sound->Normal.WaveData + length1, length2 ) ;

				hr = SoundBuffer_Unlock( &Sound->Buffer[ i ], write1, length1, write2, length2 ) ;
				if( hr != 0 )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xe2\x8e\x01\x90\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5f\x00\x32\x00\x00"/*@ L"�T�E���h�f�[�^�̓]���Ɏ��s���܂���_2" @*/ ) ;
					goto ERR ;
				}
			}
		}
	}

	// ���[�v�ʒu�̎w�肪����ꍇ�̓��[�v�ʒu��ݒ肷��
	{
		if( GParam->CreateSoundLoopStartTimePosition != GParam->CreateSoundLoopEndTimePosition )
		{
			SetLoopAreaTimePosSoundMem_UseGParam( 
				GParam->CreateSoundLoopStartTimePosition,
				GParam->CreateSoundLoopEndTimePosition,
				SoundHandle,
				ASyncThread
			) ;
		}
		else
		if( GParam->CreateSoundLoopStartSamplePosition != GParam->CreateSoundLoopEndSamplePosition )
		{
			SetLoopAreaSamplePosSoundMem_UseGParam( 
				GParam->CreateSoundLoopStartSamplePosition,
				GParam->CreateSoundLoopEndSamplePosition,
				SoundHandle,
				ASyncThread
			) ;
		}
	}
	
	// ����I��
	return 0 ;

ERR :
	if( SoundBuffer != NULL )
	{
		DXFREE( SoundBuffer ) ;
	}

	if( Stream.DataPoint != NULL )
	{
		MemStreamClose( Stream.DataPoint ) ;
	}

	if( SSSHandleI >= 0 )
	{
		NS_DeleteSoftSound( SSSHandleI ) ;
		SSSHandleI = -1 ;
	}

	TerminateSoundConvert( &ConvData ) ;
	
	return -1 ;
}

#ifndef DX_NON_ASYNCLOAD
// LoadSoundMemByMemImageBase �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void LoadSoundMemByMemImageBase_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	LOADSOUND_GPARAM *GParam ;
	int CreateSoundHandle ;
	int SoundHandle ;
	const void *FileImageBuffer ;
	int ImageSize ;
	int BufferNum ;
	int UnionHandle ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	GParam = ( LOADSOUND_GPARAM * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	CreateSoundHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	SoundHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	FileImageBuffer = GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	ImageSize = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	BufferNum = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	UnionHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;

	Result = LoadSoundMemByMemImageBase_Static( GParam, SoundHandle, FileImageBuffer, ImageSize, BufferNum, UnionHandle, TRUE ) ;
	DecASyncLoadCount( SoundHandle ) ;
	if( Result < 0 )
	{
		if( CreateSoundHandle )
		{
			SubHandle( SoundHandle ) ;
		}
	}
}
#endif // DX_NON_ASYNCLOAD

// LoadSoundMemByMemImageBase �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int LoadSoundMemByMemImageBase_UseGParam(
	LOADSOUND_GPARAM *GParam,
	int CreateSoundHandle,
	int SoundHandle,
	const void *FileImageBuffer,
	int ImageSize,
	int BufferNum,
	int UnionHandle,
	int ASyncLoadFlag,
	int ASyncThread
)
{
	if( CheckSoundSystem_Initialize_PF() == FALSE )
		return -1 ;

	if( CreateSoundHandle )
	{
		SoundHandle = _CreateSoundHandle( GParam->Create3DSoundFlag, GParam->NotInitSoundMemDelete, ASyncThread ) ;
		if( SoundHandle == -1 )
		{
			goto ERR ;
		}
	}
	else
	{
#ifndef DX_NON_ASYNCLOAD
		if( ASyncThread )
		{
			IncASyncLoadCount( SoundHandle, -1 ) ;
		}
#endif // DX_NON_ASYNCLOAD
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag && ASyncThread == FALSE )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamStruct( NULL, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( NULL, &Addr, CreateSoundHandle ) ;
		AddASyncLoadParamInt( NULL, &Addr, SoundHandle ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, FileImageBuffer ) ; 
		AddASyncLoadParamInt( NULL, &Addr, ImageSize ) ;
		AddASyncLoadParamInt( NULL, &Addr, BufferNum ) ;
		AddASyncLoadParamInt( NULL, &Addr, UnionHandle ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = LoadSoundMemByMemImageBase_ASync ;
		Addr = 0 ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, CreateSoundHandle ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SoundHandle ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, FileImageBuffer ) ; 
		AddASyncLoadParamInt( AParam->Data, &Addr, ImageSize ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, BufferNum ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, UnionHandle ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( SoundHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( LoadSoundMemByMemImageBase_Static( GParam, SoundHandle, FileImageBuffer, ImageSize, BufferNum, UnionHandle, ASyncThread ) < 0 )
			goto ERR ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( SoundHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	// �n���h����Ԃ�
	return CreateSoundHandle ? SoundHandle : 0 ;

ERR :
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( SoundHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	if( CreateSoundHandle )
	{
		SubHandle( SoundHandle ) ;
		SoundHandle = -1 ;
	}

	// �I��
	return -1 ;
}

// ��������ɓW�J���ꂽ�t�@�C���C���[�W����n���h�����쐬����(�x�[�X�֐�)
extern int NS_LoadSoundMemByMemImageBase( const void *FileImageBuffer, int ImageSize, int BufferNum, int UnionHandle )
{
	LOADSOUND_GPARAM GParam ;

	InitLoadSoundGParam( &GParam ) ;

	return LoadSoundMemByMemImageBase_UseGParam( &GParam, TRUE, -1, FileImageBuffer, ImageSize, BufferNum, UnionHandle, GetASyncLoadFlag(), FALSE ) ;
}

// ��������ɓW�J���ꂽ�t�@�C���C���[�W����n���h�����쐬����
extern int NS_LoadSoundMemByMemImage( const void *FileImageBuffer , int ImageSize, int UnionHandle ) 
{
	return NS_LoadSoundMemByMemImageBase( FileImageBuffer, ImageSize, 1, UnionHandle ) ;
}

// ��������ɓW�J���ꂽ�t�@�C���C���[�W����n���h�����쐬����2
extern int NS_LoadSoundMemByMemImage2( const void *UData, int UDataSize, const WAVEFORMATEX *UFormat, int UHeaderSize )
{
	void *WaveImage ;
	int WaveImageSize ;
	int res, Handle ;
	
	res = CreateWaveFileImage( &WaveImage, &WaveImageSize,
								 UFormat, UHeaderSize,
								 UData, UDataSize ) ;
	if( res < 0 ) return -1 ;
	
	Handle = NS_LoadSoundMemByMemImageBase( WaveImage, WaveImageSize, 1 ) ;
	
	DXFREE( WaveImage ) ;
	
	return Handle ;
}

// ��������ɓW�J���ꂽ�t�@�C���C���[�W����n���h�����쐬����(�x�[�X�֐�)
extern int NS_LoadSoundMemByMemImageToBufNumSitei( const void *FileImageBuffer, int ImageSize, int BufferNum )
{
	return NS_LoadSoundMemByMemImageBase( FileImageBuffer, ImageSize, BufferNum ) ;
}







// �������ɓǂݍ���WAVE�f�[�^���폜����
extern int NS_DeleteSoundMem( int SoundHandle, int )
{
	return SubHandle( SoundHandle ) ;
}

// �T�E���h�n���h���̍Đ��������s��( -1:�G���[ 0:����I�� 1:�Đ�����K�v�Ȃ� )
static	int _PlaySetupSoundMem( SOUND * Sound, int TopPositionFlag )
{
	ULONGLONG Position ;
	int IsPlay ;
	int i ;
	int j ;
//	D_XAUDIO2_VOICE_STATE XAState ;
	LONG Volume[ SOUNDBUFFER_MAX_CHANNEL_NUM ] ;
	LONG Pan ;
	DWORD Frequency ;

	// �O��̍Đ��ʒu���擾����
	{
		DWORD Temp ;

//		Sound->Buffer[ Sound->Normal.BackPlayBufferNo ]->GetCurrentPosition( &Position, NULL ) ;
		SoundBuffer_GetCurrentPosition( &Sound->Buffer[ Sound->Normal.BackPlayBufferNo ], &Temp, NULL ) ;
		Position = Temp ;
	}

	// �Đ�����T�E���h�o�b�t�@�̃Z�b�g
	i = Sound->Normal.BackPlayBufferNo ;

	// ����Đ�����T�E���h�o�b�t�@�̃Z�b�g
	i ++ ;
	if( i >= Sound->ValidBufferNum )
		i = 0 ;

	// �O��̍Đ��ʒu�̏����X�V����
	Sound->Normal.BackPlayBufferNo = i ;

	// �Đ����������ꍇ�͎~�߂�
	{
		IsPlay = SoundBuffer_CheckPlay( &Sound->Buffer[ i ] ) ;
		if( IsPlay == -1 )
		{
			return -1 ;
		}

		if( IsPlay )
		{
			// �擪����̍Đ��w��ł͂Ȃ��ꍇ�͉��������I��
			if( TopPositionFlag == FALSE )
				return 1 ;

			// �Đ���~
			SoundBuffer_Stop( &Sound->Buffer[ i ] ) ;
		}

		// �Đ��ʒu��擪�ɂ���
		if( TopPositionFlag == TRUE )	SoundBuffer_SetCurrentPosition( &Sound->Buffer[i], 0 ) ;
		else							SoundBuffer_SetCurrentPosition( &Sound->Buffer[i], ( DWORD )Position ) ;
	}

	// �{�����[�����Z�b�g����
	for( j = 0 ; j < SOUNDBUFFER_MAX_CHANNEL_NUM ; j ++ )
	{
		if( Sound->ValidNextPlayVolume[ j ] )
		{
			Volume[ j ] = ( LONG )Sound->NextPlayVolume[ j ] ;
			Sound->ValidNextPlayVolume[ j ] = 0 ;
		}
		else
		{
			Volume[ j ] = ( LONG )Sound->BaseVolume[ j ] ;
		}
		SoundBuffer_SetVolume( &Sound->Buffer[ i ], j, Volume[ j ] ) ;
	}

	// �p�����Z�b�g����
	{
		if( Sound->ValidNextPlayPan )
		{
			Pan = ( LONG )Sound->NextPlayPan ;
			Sound->ValidNextPlayPan = 0 ;
		}
		else
		{
			Pan = ( LONG )Sound->BasePan ;
		}
		SoundBuffer_SetPan(       &Sound->Buffer[ i ], Pan ) ;
	}

	// ���g�����Z�b�g����
	{
		if( Sound->ValidNextPlayFrequency )
		{
			Frequency = ( DWORD )Sound->NextPlayFrequency ;
			Sound->ValidNextPlayFrequency = 0 ;
		}
		else
		{
			Frequency = ( DWORD )Sound->BaseFrequency ;
		}

		if( Sound->PitchRateEnable )
		{
			Frequency = ( DWORD )_FTOL( Frequency * Sound->PitchRate ) ;
		}

		SoundBuffer_SetFrequency( &Sound->Buffer[ i ], Frequency ) ;
	}

	// �R�c�T�E���h�̏ꍇ�͂R�c�T�E���h�p�����[�^�̍X�V���s��
	if( Sound->Is3DSound != FALSE )
	{
		VECTOR *Position ;
		float Radius ;
		VECTOR *Velocity ;

		if( Sound->ValidNextPlay3DPosition )
		{
			Position = &Sound->NextPlay3DPosition ;
			Sound->ValidNextPlay3DPosition = 0 ;
		}
		else
		{
			Position = &Sound->Base3DPosition ;
		}

		if( Sound->ValidNextPlay3DRadius )
		{
			Radius = Sound->NextPlay3DRadius ;
			Sound->ValidNextPlay3DRadius = 0 ;
		}
		else
		{
			Radius = Sound->Base3DRadius ;
		}

		if( Sound->ValidNextPlay3DVelocity )
		{
			Velocity = &Sound->NextPlay3DVelocity ;
			Sound->ValidNextPlay3DVelocity = 0 ;
		}
		else
		{
			Velocity = &Sound->Base3DVelocity ;
		}
		SoundBuffer_Set3DPosition( &Sound->Buffer[ i ], Position ) ;
		SoundBuffer_Set3DRadius(   &Sound->Buffer[ i ], Radius ) ;
		SoundBuffer_Set3DVelocity( &Sound->Buffer[ i ], Velocity ) ;

		SoundBuffer_Refresh3DSoundParam( &Sound->Buffer[ i ] ) ;
	}

	// ����I��
	return 0 ;
}

// �������ɓǂ݂���WAVE�f�[�^���Đ�����
extern int NS_PlaySoundMem( int SoundHandle , int PlayType, int TopPositionFlag )
{
	SOUND * Sound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	// �T�E���h�^�C�v���X�g���[�����T�E���h�������ꍇ�͂������ɏ������ڂ�
	if( Sound->Type == DX_SOUNDTYPE_STREAMSTYLE )
	{
		return NS_PlayStreamSoundMem( SoundHandle , PlayType, TopPositionFlag ) ;
	}

	// �Đ��������s��
	if( _PlaySetupSoundMem( Sound, TopPositionFlag ) == 1 )
		return 0 ;

	// �Đ�
	if( SoundBuffer_Play( &Sound->Buffer[ Sound->Normal.BackPlayBufferNo ], PlayType == DX_PLAYTYPE_LOOP ) != 0 )
	{
		return -1 ;
	}

	// �R�c�T�E���h�̏ꍇ�͍Đ����̂R�c�T�E���h���X�g�ɒǉ�����
	if( Sound->Is3DSound != FALSE )
	{
		if( Sound->AddPlay3DSoundList == FALSE )
		{
			Sound->AddPlay3DSoundList = TRUE ;

			// �N���e�B�J���Z�N�V�����̎擾
			CRITICALSECTION_LOCK( &SoundSysData.Play3DSoundListCriticalSection ) ;

			AddHandleList( &SoundSysData.Play3DSoundListFirst, &Sound->Play3DSoundList, SoundHandle, Sound ) ;

			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &SoundSysData.Play3DSoundListCriticalSection ) ;
		}
	}

	// �Đ��X�e�[�^�X�ɂ���Ă͍Đ��I����҂�
	if( PlayType == DX_PLAYTYPE_NORMAL )
	{
		while( NS_ProcessMessage() == 0 && NS_CheckSoundMem( SoundHandle ) == 1 ){ Thread_Sleep( 1 ) ; }
	}

	// �I��
	return 0 ;
}


// �������ɓǂݍ��񂾕�����WAVE�f�[�^���Ȃ�ׂ������ɍĐ�����
extern int NS_MultiPlaySoundMem( const int *SoundHandleList, int SoundHandleNum, int PlayType, int TopPositionFlag )
{
	SOUND **SoundList, *SoundBuf[ 256 ], **SoundTempBuf = NULL, *Sound ;
	SOUNDBUFFER **SBufferList, *SBufferBuf[ 256 ], **SBufferTempBuf = NULL, *SBuffer ;
	int i, Loop ;

	if( SoundSysData.InitializeFlag == FALSE )
	{
		return -1 ;
	}
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �T�E���h�|�C���^���X�g������Ȃ������烁�������m�ۂ���
	if( SoundHandleNum > 256 )
	{
		SoundTempBuf = ( SOUND ** )DXALLOC( ( sizeof( SOUND * ) + sizeof( SOUNDBUFFER * ) ) * SoundHandleNum ) ;
		if( SoundTempBuf == NULL )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x25\x00\x64\x00\x20\x00\x0b\x50\x6e\x30\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xc7\x30\xfc\x30\xbf\x30\x6e\x30\xdd\x30\xa4\x30\xf3\x30\xbf\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"%d �̃T�E���h�f�[�^�̃|�C���^���i�[���郁�����̈�̊m�ۂɎ��s���܂���" @*/ , SoundHandleNum )) ;
			goto END ;
		}
		SBufferTempBuf = ( SOUNDBUFFER ** )( SoundTempBuf + SoundHandleNum ) ;

		SBufferList = SBufferTempBuf ;
		SoundList = SoundTempBuf ;
	}
	else
	{
		SBufferList = SBufferBuf ;
		SoundList = SoundBuf ;
	}

	// ���[�v���ǂ������擾���Ă���
	Loop = PlayType == DX_PLAYTYPE_LOOP ;

	// �Đ��̏���
	for( i = 0 ; i < SoundHandleNum ; i ++ )
	{
		// �G���[����
		if( SOUNDHCHK( SoundHandleList[ i ], Sound ) )
		{
			SoundList[ i ] = NULL ;
		}
		else
		{
			SoundList[ i ] = Sound ;

			if( Sound->Type == DX_SOUNDTYPE_STREAMSTYLE )
			{
				_PlaySetupStreamSoundMem( SoundHandleList[ i ], Sound, PlayType, TopPositionFlag, &SBufferList[ i ] ) ;
			}
			else
			{
				_PlaySetupSoundMem( Sound, TopPositionFlag ) ;
				SBufferList[ i ] = &Sound->Buffer[ Sound->Normal.BackPlayBufferNo ] ;
			}
		}
	}

	// ��ĂɍĐ�
	if( SoundSysData.EnableSoundCaptureFlag )
	{
		for( i = 0 ; i < SoundHandleNum ; i ++ )
		{
			Sound = SoundList[ i ] ;
			if( Sound == NULL ) continue ;
			SBuffer = SBufferList[ i ] ;
			SBuffer->State = TRUE ;
			SBuffer->Loop = ( Loop || Sound->Type == DX_SOUNDTYPE_STREAMSTYLE ) ? 1 : 0 ;
		}
	}
	else
	{
		for( i = 0 ; i < SoundHandleNum ; i ++ )
		{
			Sound = SoundList[ i ] ;
			if( Sound == NULL )
			{
				continue ;
			}

			SBuffer = SBufferList[ i ] ;
			SoundBuffer_Play( SBuffer, Loop || Sound->Type == DX_SOUNDTYPE_STREAMSTYLE ? TRUE : FALSE ) ;
		}
	}

	// �Đ��X�e�[�^�X�ɂ���Ă͍Đ��I����҂�
	if( PlayType == DX_PLAYTYPE_NORMAL )
	{
		while( NS_ProcessMessage() == 0 )
		{
			for( i = 0 ; i < SoundHandleNum ; i ++ )
			{
				if( SoundList[ i ] == NULL ) continue ;
				if( NS_CheckSoundMem( SoundHandleList[ i ] ) == 1 ) break ;
			}
			if( i == SoundHandleNum ) break ;
			Thread_Sleep( 1 ) ;
		}
	}

END :

	// �������̉��
	if( SoundTempBuf )
	{
		DXFREE( SoundTempBuf ) ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �I��
	return 0 ;
}


// �������ɓǂݍ���WAVE�f�[�^�̍Đ����~�߂�
extern int NS_StopSoundMem( int SoundHandle )
{
	SOUND * Sound ;
	int IsPlay ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	// �T�E���h�^�C�v���X�g���[�����T�E���h�������ꍇ�͂������ɏ������ڂ�
	if( Sound->Type == DX_SOUNDTYPE_STREAMSTYLE )
	{
		return NS_StopStreamSoundMem( SoundHandle ) ;
	}

	// �Đ���~
	for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
	{
		Sound->BufferPlayStateBackupFlagValid[ i ] = FALSE ;
		Sound->BufferPlayStateBackupFlag[ i ] = FALSE ;

		IsPlay = SoundBuffer_CheckPlay( &Sound->Buffer[ i ] ) ;
		if( IsPlay == -1 )
		{
			return -1 ;
		}

		if( IsPlay )
		{
			SoundBuffer_Stop( &Sound->Buffer[ i ], TRUE ) ;
		}
	}

	// �I��
	return 0 ;
}

// �������ɓǂ݂���WAVE�f�[�^���Đ��������ׂ�
extern int NS_CheckSoundMem( int SoundHandle )
{
	SOUND * Sound ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	// �T�E���h�^�C�v���X�g���[�����T�E���h�������ꍇ�͂������ɏ������ڂ�
	if( Sound->Type == DX_SOUNDTYPE_STREAMSTYLE )
	{
		return NS_CheckStreamSoundMem( SoundHandle ) ;
	}
	
	// �X�e�[�^�X���擾���Ē��ׂ�
	for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
	{
		// �V�X�e�����Ŏ~�߂Ă���ꍇ�͎~�߂�O�̏�Ԃ�Ԃ�
		if( Sound->BufferPlayStateBackupFlagValid[ i ] )
		{
			return Sound->BufferPlayStateBackupFlag[ i ] ? 1 : 0 ;
		}
		else
		{
			if( SoundBuffer_CheckPlay( &Sound->Buffer[ i ] ) )
			{
				return 1 ;
			}
		}
	}

	// �����܂ł��Ă���΍Đ��͂���Ă��Ȃ�
	return 0 ;
}

// �������ɓǂ݂���WAVE�f�[�^�̍Đ��Ƀp����ݒ肷��
extern int NS_SetPanSoundMem( int PanPal , int SoundHandle )
{
	int i ;
	SOUND * Sound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	// �Đ�������Ă��Ȃ��ꍇ�� SetNextPlayPanSoundMem ���Ă�
//	if( NS_CheckSoundMem( SoundHandle ) == 0 )
//	{
//		return NS_SetNextPlayPanSoundMem( PanPal, SoundHandle ) ;
//	}

	if( PanPal >  10000 ) PanPal =  10000 ;
	if( PanPal < -10000 ) PanPal = -10000 ;

	// �p�����Z�b�g����
	for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
	{
//		Sound->Buffer[ i ]->SetPan( ( LONG )PanPal ) ;
		SoundBuffer_SetPan( &Sound->Buffer[ i ], ( LONG )PanPal ) ;
	}
//	SoundBuffer_SetPan( &Sound->Buffer[ Sound->Normal.BackPlayBufferNo ], ( LONG )PanPal ) ;
	Sound->BasePan = PanPal ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̃p����ݒ肷��( 0 �` 255 )
extern int NS_ChangePanSoundMem( int PanPal, int SoundHandle )
{
	int i ;
	int pan ;
	SOUND * Sound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	if( PanPal >  255 ) PanPal =  255 ;
	if( PanPal < -255 ) PanPal = -255 ;

	if( PanPal == 0 )
	{
		pan = 0 ;
	}
	else
	{
		if( PanPal > 0 )
		{
			if( PanPal == 255 )
			{
				pan = 10000 ;
			}
			else
			{
				if( SoundSysData.OldVolumeTypeFlag )
				{
					pan = -(LONG)( _DTOL( _LOG10( ( 255 - PanPal ) / 255.0 ) * 10.0 * 100.0 ) ) ;
				}
				else
				{
					pan = -(LONG)( _DTOL( _LOG10( ( 255 - PanPal ) / 255.0 ) * 50.0 * 100.0 ) ) ;
				}
				if( pan > 10000 )
				{
					pan = 10000 ;
				}
			}
		}
		else
		{
			if( PanPal == -255 )
			{
				pan = -10000 ;
			}
			else
			{
				if( SoundSysData.OldVolumeTypeFlag )
				{
					pan = (LONG)( _DTOL( _LOG10( ( 255 + PanPal ) / 255.0 ) * 10.0 * 100.0 ) ) ;
				}
				else
				{
					pan = (LONG)( _DTOL( _LOG10( ( 255 + PanPal ) / 255.0 ) * 50.0 * 100.0 ) ) ;
				}
				if( pan < -10000 )
				{
					pan = -10000 ;
				}
			}
		}
	}

	// �p�����Z�b�g����
	for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
	{
		SoundBuffer_SetPan( &Sound->Buffer[ i ], ( LONG )pan ) ;
	}

	Sound->BasePan = pan ;

	// �I��
	return 0 ;
}

// �������ɓǂ݂���WAVE�f�[�^�̃p�����擾����
extern int NS_GetPanSoundMem( int SoundHandle )
{
	SOUND * Sound ;
	LONG Result ;
	int hr ;
	SOUNDBUFFER *SBuffer ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	// �T�E���h�^�C�v���X�g���[�����T�E���h�������ꍇ�͂������ɏ������ڂ�
	if( Sound->Type == DX_SOUNDTYPE_STREAMSTYLE )
	{
		// �g�p���������ꍇ�̓G���[
		if( GetSoundBuffer( SoundHandle, Sound, &SBuffer, false ) == -1 ) return -1 ;
	
		// �p�����擾����
		hr = SoundBuffer_GetPan( SBuffer, &Result ) ;
	}
	else
	{
		hr = SoundBuffer_GetPan( &Sound->Buffer[ Sound->Normal.BackPlayBufferNo ], &Result ) ;
	}

	// �I��
	return Result ; 
}

// �������ɓǂ݂���WAVE�f�[�^�̍Đ��Ƀ{�����[����ݒ肷��
extern int NS_SetVolumeSoundMem( int VolumePal , int SoundHandle )
{
	SOUND * Sound ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	// �Đ�������Ă��Ȃ��ꍇ�� SetNextPlayVolumeSoundMem ���Ă�
//	if( NS_CheckSoundMem( SoundHandle ) == 0 )
//	{
//		return NS_SetNextPlayVolumeSoundMem( VolumePal, SoundHandle ) ;
//	}

	VolumePal -= 10000 ;
//	if( VolumePal >  0 ) VolumePal =  0 ;
	if( VolumePal < -10000 ) VolumePal = -10000 ;

	// �{�����[�����Z�b�g����
	for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
	{
//		Sound->Buffer[ i ]->SetVolume( ( LONG )VolumePal ) ;
		SoundBuffer_SetVolumeAll( &Sound->Buffer[ i ], ( LONG )VolumePal ) ;
	}
//	SoundBuffer_SetVolumeAll( &Sound->Buffer[ Sound->Normal.BackPlayBufferNo ], ( LONG )VolumePal ) ;

	for( i = 0 ; i < SOUNDBUFFER_MAX_CHANNEL_NUM ; i ++ )
	{
		Sound->BaseVolume[ i ] = VolumePal ;
	}

	// �I��
	return 0 ;
}

// �������ɓǂ݂���WAVE�f�[�^�̍Đ��Ƀ{�����[����ݒ肷��
extern int NS_ChangeVolumeSoundMem( int VolumePal, int SoundHandle )
{
	SOUND * Sound ;
	int i ;
	LONG vol ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

//	     if( VolumePal > 255 ) VolumePal = 255 ;
//	else if( VolumePal < 0   ) VolumePal = 0   ;
	if( VolumePal < 0   ) VolumePal = 0   ;

	if( VolumePal == 0 )
	{
		vol = DX_DSBVOLUME_MIN ;
	}
	else
	{
		if( SoundSysData.OldVolumeTypeFlag )
		{
			vol = (LONG)( _DTOL( _LOG10( VolumePal / 255.0 ) * 10.0 * 100.0 ) ) ;
		}
		else
		{
			vol = (LONG)( _DTOL( _LOG10( VolumePal / 255.0 ) * 50.0 * 100.0 ) ) ;
		}
		if( vol < DX_DSBVOLUME_MIN ) vol = DX_DSBVOLUME_MIN ;
	}

	// �{�����[�����Z�b�g����
	for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
	{
//		Sound->Buffer[ i ]->SetVolume( vol ) ;
		SoundBuffer_SetVolumeAll( &Sound->Buffer[ i ], vol ) ;
	}
//	SoundBuffer_SetVolumeAll( &Sound->Buffer[ Sound->Normal.BackPlayBufferNo ], vol ) ;

	for( i = 0 ; i < SOUNDBUFFER_MAX_CHANNEL_NUM ; i ++ )
	{
		Sound->BaseVolume[ i ] = vol ;
	}

	// �I��
	return 0 ;
}

// �������ɓǂ݂���WAVE�f�[�^�̍Đ��̃{�����[�����擾����
extern int NS_GetVolumeSoundMem( int SoundHandle )
{
	return NS_GetChannelVolumeSoundMem( 0, SoundHandle ) ;
}


// �T�E���h�n���h���̎w��̃`�����l���̃{�����[����ݒ肷��( 100����1�f�V�x���P�� 0 �` 10000 )
extern int NS_SetChannelVolumeSoundMem( int Channel, int VolumePal, int SoundHandle )
{
	SOUND * Sound ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	VolumePal -= 10000 ;
//	if( VolumePal >  0 ) VolumePal =  0 ;
	if( VolumePal < -10000 ) VolumePal = -10000 ;

	// �{�����[�����Z�b�g����
	for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
	{
		if( SoundBuffer_SetVolume( &Sound->Buffer[ i ], Channel, ( LONG )VolumePal ) < 0 )
		{
			return -1 ;
		}
	}
	Sound->BaseVolume[ Channel ] = VolumePal ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̎w��̃`�����l���̃{�����[����ݒ肷��( 0 �` 255 )
extern int NS_ChangeChannelVolumeSoundMem( int Channel, int VolumePal, int SoundHandle )
{
	SOUND * Sound ;
	int i ;
	LONG vol ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

//	     if( VolumePal > 255 ) VolumePal = 255 ;
//	else if( VolumePal < 0   ) VolumePal = 0   ;
	if( VolumePal < 0   ) VolumePal = 0   ;

	if( VolumePal == 0 )
	{
		vol = DX_DSBVOLUME_MIN ;
	}
	else
	{
		if( SoundSysData.OldVolumeTypeFlag )
		{
			vol = (LONG)( _DTOL( _LOG10( VolumePal / 255.0 ) * 10.0 * 100.0 ) ) ;
		}
		else
		{
			vol = (LONG)( _DTOL( _LOG10( VolumePal / 255.0 ) * 50.0 * 100.0 ) ) ;
		}
		if( vol < DX_DSBVOLUME_MIN ) vol = DX_DSBVOLUME_MIN ;
	}

	// �{�����[�����Z�b�g����
	for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
	{
		if( SoundBuffer_SetVolume( &Sound->Buffer[ i ], Channel, vol ) < 0 )
		{
			return -1 ;
		}
	}

	Sound->BaseVolume[ Channel ] = vol ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̎w��̃`�����l���̃{�����[�����擾����
extern int NS_GetChannelVolumeSoundMem( int Channel, int SoundHandle )
{
	SOUND * Sound ;
	LONG Result ;
	SOUNDBUFFER *SBuffer ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	// �T�E���h�^�C�v���X�g���[�����T�E���h�������ꍇ�͂������ɏ������ڂ�
	if( Sound->Type == DX_SOUNDTYPE_STREAMSTYLE )
	{
		// �g�p���������ꍇ�̓G���[
		if( GetSoundBuffer( SoundHandle, Sound, &SBuffer, false ) == -1 ) return -1 ;
	
		// �{�����[�����擾����
		if( SoundBuffer_GetVolume( SBuffer, Channel, &Result ) < 0 )
		{
			return -1 ;
		}
	}
	else
	{
		if( SoundBuffer_GetVolume( &Sound->Buffer[ Sound->Normal.BackPlayBufferNo ], Channel, &Result ) < 0 )
		{
			return -1 ;
		}
	}

	// �I��
	return Result + 10000 ; 
}

// �������ɓǂݍ���WAVE�f�[�^�̍Đ����g����ݒ肷��
extern int NS_SetFrequencySoundMem( int FrequencyPal , int SoundHandle )
{
	SOUND * Sound ;
	int TempFrequency ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	if( FrequencyPal == -1 )
	{
		FrequencyPal = 0 ;
	}

	// ���g�����Z�b�g����
	TempFrequency = FrequencyPal ;
	if( Sound->PitchRateEnable )
	{
		TempFrequency = _FTOL( TempFrequency * Sound->PitchRate ) ;
	}
	for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
	{
		SoundBuffer_SetFrequency( &Sound->Buffer[ i ], ( DWORD )TempFrequency ) ;
	}

	Sound->BaseFrequency = FrequencyPal ;

	// �I��
	return 0 ;
}

// �������ɓǂݍ���WAVE�f�[�^�̍Đ����g�����擾����
extern int NS_GetFrequencySoundMem( int SoundHandle )
{
	SOUND * Sound ;
	SOUNDBUFFER *SBuffer ;
	int hr ;
	DWORD Result ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	// ���g����Ԃ�
//	return (int)Sound->BufferFormat.nSamplesPerSec ;

	// �T�E���h�^�C�v���X�g���[�����T�E���h�������ꍇ�͂������ɏ������ڂ�
	if( Sound->Type == DX_SOUNDTYPE_STREAMSTYLE )
	{
		// �g�p���������ꍇ�̓G���[
		if( GetSoundBuffer( SoundHandle, Sound, &SBuffer, false ) == -1 ) return -1 ;
	
		// �p�����擾����
		hr = SoundBuffer_GetFrequency( SBuffer, &Result ) ;
	}
	else
	{
		hr = SoundBuffer_GetFrequency( &Sound->Buffer[ Sound->Normal.BackPlayBufferNo ], &Result ) ;
	}

	// �I��
	return ( int )Result ; 
}

// �������ɓǂݍ���WAVE�f�[�^�̍Đ����g����ǂݍ��ݒ���̏�Ԃɖ߂�
extern	int NS_ResetFrequencySoundMem( int SoundHandle )
{
	SOUND * Sound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	// ���g����������Ԃɖ߂�
	return NS_SetFrequencySoundMem( (int)Sound->BufferFormat.nSamplesPerSec, SoundHandle ) ;
}


// �������ɓǂ݂���WAVE�f�[�^�̍Đ��Ƀp����ݒ肷��
extern	int	NS_SetNextPlayPanSoundMem( int PanPal, int SoundHandle )
{
	SOUND * Sound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	if( PanPal >  10000 ) PanPal =  10000 ;
	if( PanPal < -10000 ) PanPal = -10000 ;

	Sound->NextPlayPan = PanPal ;
	Sound->ValidNextPlayPan = 1 ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̎��̍Đ��ɂ̂ݎg�p����p����ݒ肷��( -255 �` 255 )
extern int NS_ChangeNextPlayPanSoundMem( int PanPal, int SoundHandle )
{
	SOUND * Sound ;
	int pan ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	if( PanPal >  255 ) PanPal =  255 ;
	if( PanPal < -255 ) PanPal = -255 ;

	if( PanPal == 0 )
	{
		pan = 0 ;
	}
	else
	{
		if( PanPal > 0 )
		{
			if( PanPal == 255 )
			{
				pan = 10000 ;
			}
			else
			{
				if( SoundSysData.OldVolumeTypeFlag )
				{
					pan = -(LONG)( _DTOL( _LOG10( ( 255 - PanPal ) / 255.0 ) * 10.0 * 100.0 ) ) ;
				}
				else
				{
					pan = -(LONG)( _DTOL( _LOG10( ( 255 - PanPal ) / 255.0 ) * 50.0 * 100.0 ) ) ;
				}
				if( pan > 10000 )
				{
					pan = 10000 ;
				}
			}
		}
		else
		{
			if( PanPal == -255 )
			{
				pan = -10000 ;
			}
			else
			{
				if( SoundSysData.OldVolumeTypeFlag )
				{
					pan = (LONG)( _DTOL( _LOG10( ( 255 + PanPal ) / 255.0 ) * 10.0 * 100.0 ) ) ;
				}
				else
				{
					pan = (LONG)( _DTOL( _LOG10( ( 255 + PanPal ) / 255.0 ) * 50.0 * 100.0 ) ) ;
				}
				if( pan < -10000 )
				{
					pan = -10000 ;
				}
			}
		}
	}

	Sound->NextPlayPan = pan ;
	Sound->ValidNextPlayPan = 1 ;

	// �I��
	return 0 ;
}

// �������ɓǂ݂���WAVE�f�[�^�̍Đ��Ƀ{�����[����ݒ肷��( 100����1�f�V�x���P�� )
extern	int	NS_SetNextPlayVolumeSoundMem( int VolumePal, int SoundHandle )
{
	SOUND * Sound ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	VolumePal -= 10000 ;
	if( VolumePal >  0 ) VolumePal =  0 ;
	if( VolumePal < -10000 ) VolumePal = -10000 ;

	for( i = 0 ; i < SOUNDBUFFER_MAX_CHANNEL_NUM ; i ++ )
	{
		Sound->NextPlayVolume[ i ] = VolumePal ;
		Sound->ValidNextPlayVolume[ i ] = 1 ;
	}

	// �I��
	return 0 ;
}

// �������ɓǂ݂���WAVE�f�[�^�̍Đ��Ƀ{�����[����ݒ肷��( �p�[�Z���g�w�� )
extern	int	NS_ChangeNextPlayVolumeSoundMem( int VolumePal, int SoundHandle )
{
	SOUND * Sound ;
	LONG vol ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	     if( VolumePal > 255 ) VolumePal = 255 ;
	else if( VolumePal < 0   ) VolumePal = 0   ;

	if( VolumePal == 0 )
	{
		vol = DX_DSBVOLUME_MIN ;
	}
	else
	{
		if( SoundSysData.OldVolumeTypeFlag )
		{
			vol = (LONG)( _DTOL( _LOG10( VolumePal / 255.0 ) * 10.0 * 100.0 ) ) ;
		}
		else
		{
			vol = (LONG)( _DTOL( _LOG10( VolumePal / 255.0 ) * 50.0 * 100.0 ) ) ;
		}
		if( vol < DX_DSBVOLUME_MIN ) vol = DX_DSBVOLUME_MIN ;
	}

	for( i = 0 ; i < SOUNDBUFFER_MAX_CHANNEL_NUM ; i ++ )
	{
		Sound->NextPlayVolume[ i ] = vol ;
		Sound->ValidNextPlayVolume[ i ] = 1 ;
	}

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̎��̍Đ��ɂ̂ݎg�p����`�����l���̃{�����[����ݒ肷��( 100����1�f�V�x���P�� 0 �` 10000 )
extern int NS_SetNextPlayChannelVolumeSoundMem( int Channel, int VolumePal, int SoundHandle )
{
	SOUND * Sound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	VolumePal -= 10000 ;
	if( VolumePal >  0 ) VolumePal =  0 ;
	if( VolumePal < -10000 ) VolumePal = -10000 ;

	Sound->NextPlayVolume[ Channel ] = VolumePal ;
	Sound->ValidNextPlayVolume[ Channel ] = 1 ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̎��̍Đ��ɂ̂ݎg�p����`�����l���̃{�����[����ݒ肷��( 0 �` 255 )
extern int NS_ChangeNextPlayChannelVolumeSoundMem( int Channel, int VolumePal, int SoundHandle )
{
	SOUND * Sound ;
	LONG vol ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	     if( VolumePal > 255 ) VolumePal = 255 ;
	else if( VolumePal < 0   ) VolumePal = 0   ;

	if( VolumePal == 0 )
	{
		vol = DX_DSBVOLUME_MIN ;
	}
	else
	{
		if( SoundSysData.OldVolumeTypeFlag )
		{
			vol = (LONG)( _DTOL( _LOG10( VolumePal / 255.0 ) * 10.0 * 100.0 ) ) ;
		}
		else
		{
			vol = (LONG)( _DTOL( _LOG10( VolumePal / 255.0 ) * 50.0 * 100.0 ) ) ;
		}
		if( vol < DX_DSBVOLUME_MIN ) vol = DX_DSBVOLUME_MIN ;
	}

	Sound->NextPlayVolume[ Channel ] = vol ;
	Sound->ValidNextPlayVolume[ Channel ] = 1 ;

	// �I��
	return 0 ;
}

// �������ɓǂݍ���WAVE�f�[�^�̍Đ����g����ݒ肷��
extern	int	NS_SetNextPlayFrequencySoundMem( int FrequencyPal, int SoundHandle )
{
	SOUND * Sound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	if( FrequencyPal == -1 )
	{
		FrequencyPal = 0 ;
	}

	Sound->NextPlayFrequency = FrequencyPal ;
	Sound->ValidNextPlayFrequency = 1 ;

	// �I��
	return 0 ;
}


// �T�E���h�n���h���̍Đ��ʒu���T���v���P�ʂŕύX����(�Đ����~�܂��Ă��鎞�̂ݗL��)
extern int NS_SetCurrentPositionSoundMem( int SamplePosition, int SoundHandle )
{
	SOUND * sd ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �ύX
	return NS_SetSoundCurrentPosition( sd->BufferFormat.nBlockAlign * SamplePosition, SoundHandle ) ;
}

// �T�E���h�n���h���̍Đ��ʒu���T���v���P�ʂŎ擾����
extern int NS_GetCurrentPositionSoundMem( int SoundHandle )
{
	SOUND * sd ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �擾
	return NS_GetSoundCurrentPosition( SoundHandle ) / sd->BufferFormat.nBlockAlign ;
}

// �T�E���h�n���h���̍Đ��ʒu���o�C�g�P�ʂŕύX����(�Đ����~�܂��Ă��鎞�̂ݗL��)
extern int NS_SetSoundCurrentPosition( int Byte, int SoundHandle )
{
	SOUND * sd ;
//	int i ;
//	DWORD State ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �T�E���h�^�C�v���X�g���[�����T�E���h�������ꍇ�͂������ɏ������ڂ�
	if( sd->Type == DX_SOUNDTYPE_STREAMSTYLE )
	{
		return NS_SetStreamSoundCurrentPosition( Byte, SoundHandle ) ;
	}

	// �Đ��ʒu��ύX����
//	sd->Buffer[ i ]->SetCurrentPosition( Byte ) ;
//	SoundBuffer_SetCurrentPosition( &sd->Buffer[ i ], Byte ) ;
	SoundBuffer_SetCurrentPosition( &sd->Buffer[ sd->Normal.BackPlayBufferNo ], ( DWORD )Byte ) ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̍Đ��ʒu���o�C�g�P�ʂŎ擾����
extern int NS_GetSoundCurrentPosition( int SoundHandle )
{
	SOUND * sd ;
	DWORD pos ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �T�E���h�^�C�v���X�g���[�����T�E���h�������ꍇ�͂������ɏ������ڂ�
	if( sd->Type == DX_SOUNDTYPE_STREAMSTYLE )
	{
		return NS_GetStreamSoundCurrentPosition( SoundHandle ) ;
	}

	// �Đ����̃o�b�t�@�̍Đ��ʒu��Ԃ�
//	sd->Buffer[ Sound->Normal.BackPlayBufferNo ]->GetCurrentPosition( &pos, NULL ) ;
	SoundBuffer_GetCurrentPosition( &sd->Buffer[ sd->Normal.BackPlayBufferNo ], &pos, NULL ) ;

	return (int)pos ;
}

// �T�E���h�n���h���̍Đ��ʒu���~���b�P�ʂŐݒ肷��(���k�`���̏ꍇ�͐������ݒ肳��Ȃ��ꍇ������)
extern int NS_SetSoundCurrentTime( int Time, int SoundHandle )
{
	SOUND * sd ;
	int /*i,*/ time ;
//	DWORD State ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �T�E���h�^�C�v���X�g���[�����T�E���h�������ꍇ�͂������ɏ������ڂ�
	if( sd->Type == DX_SOUNDTYPE_STREAMSTYLE )
	{
		return NS_SetStreamSoundCurrentTime( Time, SoundHandle ) ;
	}

	// �Đ��ʒu��ύX����
//	time = _DTOL( ( (double)sd->BufferFormat.nSamplesPerSec * Time / 1000 ) * sd->BufferFormat.nBlockAlign ) ;
//	for( i = 0 ; i < sd->ValidBufferNum ; i ++ )
	time = MilliSecPositionToSamplePosition( ( int )sd->BufferFormat.nSamplesPerSec, Time ) * sd->BufferFormat.nBlockAlign ;

//	sd->Buffer[ i ]->SetCurrentPosition( time ) ;
//	SoundBuffer_SetCurrentPosition( &sd->Buffer[ i ], time ) ;
	SoundBuffer_SetCurrentPosition( &sd->Buffer[ sd->Normal.BackPlayBufferNo ], ( DWORD )time ) ;

	// �I��
	return 0 ;
}

// GetSoundTotalSample �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int GetSoundTotalSample_UseGParam( int SoundHandle, int ASyncThread )
{
	SOUND * sd ;
	int sample = -1 ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, sd ) )
			return -1 ;
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, sd ) )
			return -1 ;
	}

	// �T�E���h�^�C�v�ɂ���ď����𕪊�
	switch( sd->Type )
	{
	case DX_SOUNDTYPE_STREAMSTYLE : // �X�g���[���^�C�v�̏ꍇ
		sample = sd->Stream.TotalSample ;
		break ;
		
	case DX_SOUNDTYPE_NORMAL :		// �m�[�}���^�C�v�̏ꍇ
		sample = sd->Normal.WaveSize / sd->BufferFormat.nBlockAlign ;
		break ;
	}
	
	// �I��
	return sample ;
}

// �T�E���h�n���h���̉��̑����Ԃ��擾����(�P�ʂ̓T���v��)
extern int NS_GetSoundTotalSample( int SoundHandle )
{
	return GetSoundTotalSample_UseGParam( SoundHandle, FALSE ) ;
}

// �T�E���h�n���h���̉��̑����Ԃ��擾����(�P�ʂ̓~���b)
extern int NS_GetSoundTotalTime( int SoundHandle )
{
	SOUND * sd ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;
	
	// �~���b�ɕϊ����ĕԂ�
	return SamplePositionToMilliSecPosition( ( int )sd->BufferFormat.nSamplesPerSec, NS_GetSoundTotalSample( SoundHandle ) ) ;
}

// �T�E���h�n���h���̍Đ��ʒu���~���b�P�ʂŎ擾����(���k�`���̏ꍇ�͐������l���Ԃ��Ă��Ȃ��ꍇ������)
extern int NS_GetSoundCurrentTime( int SoundHandle )
{
	SOUND * sd ;
	DWORD time ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �T�E���h�^�C�v���X�g���[�����T�E���h�������ꍇ�͂������ɏ������ڂ�
	if( sd->Type == DX_SOUNDTYPE_STREAMSTYLE )
	{
		return NS_GetStreamSoundCurrentTime( SoundHandle ) ;
	}

	// �T�E���h�o�b�t�@�O�̍Đ��ʒu���擾����
//	sd->Buffer[ 0 ]->GetCurrentPosition( &time, NULL ) ;
	SoundBuffer_GetCurrentPosition( &sd->Buffer[ sd->Normal.BackPlayBufferNo ], &time, NULL ) ;
//	return _DTOL( (double)time * 1000 / ( sd->BufferFormat.nSamplesPerSec * sd->BufferFormat.nBlockAlign ) ) ;
	return SamplePositionToMilliSecPosition( ( int )sd->BufferFormat.nSamplesPerSec, ( int )( time / sd->BufferFormat.nBlockAlign ) ) ;
}


// �T�E���h�n���h���Ƀ��[�v�ʒu��ݒ肷��
extern int NS_SetLoopPosSoundMem( int LoopTime, int SoundHandle )
{
	SOUND * sd ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// ���[�v�T���v���ʒu���Z�b�g
	NS_SetLoopSamplePosSoundMem( LoopTime == -1 ? -1 : MilliSecPositionToSamplePosition( ( int )sd->BufferFormat.nSamplesPerSec, LoopTime ), SoundHandle ) ;

	// �I��
	return 0 ;
}

// SetLoopTimePosSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int SetLoopTimePosSoundMem_UseGParam( int LoopTime, int SoundHandle, int ASyncThread )
{
	SOUND * sd ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, sd ) )
			return -1 ;
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, sd ) )
			return -1 ;
	}
	
	// ���[�v�T���v���ʒu���Z�b�g
	SetLoopSamplePosSoundMem_UseGParam(
		LoopTime == -1 ? -1 : MilliSecPositionToSamplePosition( ( int )sd->BufferFormat.nSamplesPerSec, LoopTime ),
		SoundHandle, ASyncThread
	) ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���Ƀ��[�v�ʒu��ݒ肷��
extern int NS_SetLoopTimePosSoundMem( int LoopTime, int SoundHandle )
{
	return SetLoopTimePosSoundMem_UseGParam( LoopTime, SoundHandle, FALSE ) ;
}

// SetLoopSamplePosSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int SetLoopSamplePosSoundMem_UseGParam( int LoopSamplePosition, int SoundHandle, int ASyncThread )
{
	SOUND * sd ;
	STREAMFILEDATA * pl ;

	if( SoundSysData.InitializeFlag == FALSE )
	{
		return -1 ;
	}
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �G���[����
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, sd ) )
			goto ERR ;
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, sd ) )
			goto ERR ;
	}

	// �w��̈ʒu�������t�@�C���S�̂��������ꍇ�̓G���[
	if( GetSoundTotalSample_UseGParam( SoundHandle, ASyncThread ) <= LoopSamplePosition )
		goto ERR ;

	// ���[�v�ʒu�� -1 �������ꍇ�̓��[�v�ʒu�ݒ����������
	if( LoopSamplePosition == -1 )
	{
		pl = &sd->Stream.File[0] ;
		pl->LoopSamplePosition = 0 ;
		pl->LoopNum = 0 ;
		goto END ;
	}

	// �T�E���h�n���h�����X�g���[���Đ��ł͂Ȃ�������X�g���[���Đ��̃n���h���ɕύX����
	if( sd->Type == DX_SOUNDTYPE_NORMAL )
	{
		// �񓯊��X���b�h�̏ꍇ�̓G���[
		if( ASyncThread )
			goto ERR ;

		if( SoundTypeChangeToStream( SoundHandle ) < 0 )
			goto ERR ;

		// �G���[����
		if( SOUNDHCHK( SoundHandle, sd ) )
			goto ERR ;
	}

	// ���̏��X�g���[���f�[�^���Q�ȏ゠�����疳��
	if( sd->Stream.FileNum > 1 )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x8c\x4e\x64\x30\xe5\x4e\x0a\x4e\x6e\x30\xf3\x97\xf0\x58\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x4b\x30\x89\x30\x5c\x4f\x10\x62\x55\x30\x8c\x30\x5f\x30\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6b\x30\xeb\x30\xfc\x30\xd7\x30\xdd\x30\xa4\x30\xf3\x30\xc8\x30\x92\x30\x07\x63\x9a\x5b\x59\x30\x8b\x30\x8b\x4e\x6f\x30\xfa\x51\x65\x67\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"��ȏ�̉����t�@�C������쐬���ꂽ�T�E���h�n���h���Ƀ��[�v�|�C���g���w�肷�鎖�͏o���܂���\n" @*/ ) ;
		goto ERR ;
	}

	pl = &sd->Stream.File[0] ;

/*	// �����k wave �� OGG �ȊO�͖���
	if( pl->DataType != DX_SOUNDDATATYPE_MEMNOPRESS &&
#ifndef DX_NON_OGGVORBIS
		pl->OggFlag == FALSE && 
#endif
		( pl->DataType == DX_SOUNDDATATYPE_MEMPRESS ||
			( pl->DataType == DX_SOUNDDATATYPE_FILE && pl->FileData.FormatMatchFlag == FALSE ) ) )
	{
		DXST_ERRORLOG_ADDUTF16LE( L"�����k�v�`�u�d�t�@�C���Ⴕ���͂n�f�f�t�@�C���ȊO�̓��[�v�|�C���g���w�肷�鎖�͏o���܂���\n" ) ;

		
		
		return -1 ;
	}
*/
	// ���[�v�ʒu��ݒ�
	pl->LoopSamplePosition = LoopSamplePosition ;
	SetupSeekPosSoundConvert( &pl->ConvData, LoopSamplePosition ) ;

	// �������[�v�ɐݒ�
	pl->LoopNum = -1 ;

END :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �I��
	return 0 ;

ERR :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �G���[�I��
	return -1 ;
}

// �T�E���h�n���h���Ƀ��[�v�ʒu��ݒ肷��
extern int NS_SetLoopSamplePosSoundMem( int LoopSamplePosition, int SoundHandle )
{
	return SetLoopSamplePosSoundMem_UseGParam( LoopSamplePosition, SoundHandle, FALSE ) ;
}

// SetLoopStartTimePosSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int SetLoopStartTimePosSoundMem_UseGParam( int LoopStartTime, int SoundHandle, int ASyncThread )
{
	SOUND * sd ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, sd ) )
			return -1 ;
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, sd ) )
			return -1 ;
	}

	// ���[�v�T���v���ʒu���Z�b�g
	SetLoopStartSamplePosSoundMem_UseGParam(
		LoopStartTime == -1 ? -1 : MilliSecPositionToSamplePosition( ( int )sd->BufferFormat.nSamplesPerSec, LoopStartTime ),
		SoundHandle, ASyncThread
	) ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���Ƀ��[�v�J�n�ʒu��ݒ肷��
extern int NS_SetLoopStartTimePosSoundMem( int LoopStartTime, int SoundHandle )
{
	return SetLoopStartTimePosSoundMem_UseGParam( LoopStartTime, SoundHandle, FALSE ) ;
}

// SetLoopStartTimePosSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int SetLoopStartSamplePosSoundMem_UseGParam( int LoopStartSamplePosition, int SoundHandle, int ASyncThread )
{
	SOUND * sd ;
	STREAMFILEDATA * pl ;

	if( SoundSysData.InitializeFlag == FALSE )
	{
		return -1 ;
	}
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �G���[����
	if( ASyncThread )
	{
		if( SOUNDHCHK_ASYNC( SoundHandle, sd ) )
			goto ERR ;
	}
	else
	{
		if( SOUNDHCHK( SoundHandle, sd ) )
			goto ERR ;
	}

	// �w��̈ʒu�������t�@�C���S�̂��������ꍇ�̓G���[
	if( GetSoundTotalSample_UseGParam( SoundHandle, ASyncThread ) <= LoopStartSamplePosition )
		goto ERR ;

	// ���[�v�ʒu�� -1 �������ꍇ�̓��[�v�ʒu�ݒ����������
	if( LoopStartSamplePosition == -1 )
	{
		pl = &sd->Stream.File[0] ;
		pl->LoopStartSamplePosition = -1 ;
		goto END ;
	}

	// �T�E���h�n���h�����X�g���[���Đ��ł͂Ȃ�������X�g���[���Đ��̃n���h���ɕύX����
	if( sd->Type == DX_SOUNDTYPE_NORMAL )
	{
		// �񓯊��X���b�h�̏ꍇ�̓G���[
		if( ASyncThread )
			goto ERR ;

		if( SoundTypeChangeToStream( SoundHandle ) < 0 )
			goto ERR ;

		// �G���[����
		if( SOUNDHCHK( SoundHandle, sd ) )
			goto ERR ;
	}

	// ���̏��X�g���[���f�[�^���Q�ȏ゠�����疳��
	if( sd->Stream.FileNum > 1 )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x8c\x4e\x64\x30\xe5\x4e\x0a\x4e\x6e\x30\xf3\x97\xf0\x58\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x4b\x30\x89\x30\x5c\x4f\x10\x62\x55\x30\x8c\x30\x5f\x30\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6b\x30\xeb\x30\xfc\x30\xd7\x30\x8b\x95\xcb\x59\x4d\x4f\x6e\x7f\x92\x30\x07\x63\x9a\x5b\x59\x30\x8b\x30\x8b\x4e\x6f\x30\xfa\x51\x65\x67\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"��ȏ�̉����t�@�C������쐬���ꂽ�T�E���h�n���h���Ƀ��[�v�J�n�ʒu���w�肷�鎖�͏o���܂���\n" @*/ ) ;
		goto ERR ;
	}

	pl = &sd->Stream.File[0] ;

	// ���[�v�ʒu��ݒ�
	pl->LoopStartSamplePosition = LoopStartSamplePosition ;

	// �]�������ʒu�����Ƀ��[�v�J�n�ʒu���z���Ă��܂��Ă�����X�g���[���T�E���h�f�[�^�̃Z�b�g�A�b�v����蒼��
	if( sd->Stream.FileCompCopyLength > pl->LoopStartSamplePosition )
	{
		RefreshStreamSoundPlayCompLength( SoundHandle ) ;
		SetStreamSoundCurrentPosition_UseGParam( ( int )sd->Stream.CompPlayWaveLength, SoundHandle, ASyncThread ) ;
	}

END :
	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �I��
	return 0 ;

ERR :
	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

	// �G���[�I��
	return -1 ;
}

// �T�E���h�n���h���Ƀ��[�v�J�n�ʒu��ݒ肷��
extern int NS_SetLoopStartSamplePosSoundMem( int LoopStartSamplePosition, int SoundHandle )
{
	return SetLoopStartSamplePosSoundMem_UseGParam( LoopStartSamplePosition, SoundHandle, FALSE ) ;
}

// SetLoopAreaTimePosSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int SetLoopAreaTimePosSoundMem_UseGParam( int LoopStartTime, int LoopEndTime, int SoundHandle, int ASyncThread )
{
	// �p�����[�^�`�F�b�N
	if( LoopStartTime >= LoopEndTime )
	{
		return -1 ;
	}

	// ���[�v�͈͂̐�[���Z�b�g
	if( SetLoopTimePosSoundMem_UseGParam( LoopStartTime, SoundHandle, ASyncThread ) < 0 )
	{
		return -1 ;
	}

	// ���[�v�͈͂̏I�[���Z�b�g
	if( SetLoopStartTimePosSoundMem_UseGParam( LoopEndTime, SoundHandle, ASyncThread ) < 0 )
	{
		return -1 ;
	}

	// ����I��
	return 0 ;
}

// �T�E���h�n���h���Ƀ��[�v�͈͂�ݒ肷��(�~���b�P��)
extern int NS_SetLoopAreaTimePosSoundMem( int LoopStartTime, int LoopEndTime, int SoundHandle )
{
	return SetLoopAreaTimePosSoundMem_UseGParam( LoopStartTime, LoopEndTime, SoundHandle, FALSE ) ;
}

// �T�E���h�n���h���Ƀ��[�v�͈͂��擾����(�~���b�P��)
extern int NS_GetLoopAreaTimePosSoundMem( int *LoopStartTime, int *LoopEndTime, int SoundHandle )
{
	SOUND * sd ;
	int LoopStartSamplePosition ;
	int LoopEndSamplePosition ;
	int Result ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;
	
	// ���[�v�T���v���ʒu���擾
	Result = NS_GetLoopAreaSamplePosSoundMem( &LoopStartSamplePosition, &LoopEndSamplePosition, SoundHandle ) ;
	if( Result < 0 )
	{
		return -1 ;
	}

	if( LoopStartTime != NULL ) *LoopStartTime = SamplePositionToMilliSecPosition( ( int )sd->BufferFormat.nSamplesPerSec, LoopStartSamplePosition ) ;
	if( LoopEndTime   != NULL ) *LoopEndTime   = SamplePositionToMilliSecPosition( ( int )sd->BufferFormat.nSamplesPerSec, LoopEndSamplePosition   ) ;

	// �I��
	return 0 ;
}

// SetLoopAreaSamplePosSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int SetLoopAreaSamplePosSoundMem_UseGParam( int LoopStartSamplePosition, int LoopEndSamplePosition, int SoundHandle, int ASyncThread )
{
	// �p�����[�^�`�F�b�N
	if( LoopStartSamplePosition >= LoopEndSamplePosition )
	{
		return -1 ;
	}

	// ���[�v�͈͂̐�[���Z�b�g
	if( SetLoopSamplePosSoundMem_UseGParam( LoopStartSamplePosition, SoundHandle, ASyncThread ) < 0 )
	{
		return -1 ;
	}

	// ���[�v�͈͂̏I�[���Z�b�g
	if( SetLoopStartSamplePosSoundMem_UseGParam( LoopEndSamplePosition, SoundHandle, ASyncThread ) < 0 )
	{
		return -1 ;
	}

	// ����I��
	return 0 ;
}

// �T�E���h�n���h���Ƀ��[�v�͈͂�ݒ肷��(�T���v���P��)
extern int NS_SetLoopAreaSamplePosSoundMem( int LoopStartSamplePosition, int LoopEndSamplePosition, int SoundHandle )
{
	return SetLoopAreaSamplePosSoundMem_UseGParam( LoopStartSamplePosition, LoopEndSamplePosition, SoundHandle, FALSE ) ;
}

// �T�E���h�n���h���Ƀ��[�v�͈͂��擾����(�T���v���P��)
extern int NS_GetLoopAreaSamplePosSoundMem( int *LoopStartSamplePosition, int *LoopEndSamplePosition, int SoundHandle )
{
	SOUND * sd ;
	STREAMFILEDATA * pl ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;
	
	pl = &sd->Stream.File[0] ;

	if( LoopStartSamplePosition != NULL ) *LoopStartSamplePosition = pl->LoopSamplePosition ;
	if( LoopEndSamplePosition   != NULL ) *LoopEndSamplePosition   = pl->LoopStartSamplePosition ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̍Đ����I�������玩���I�Ƀn���h�����폜���邩�ǂ�����ݒ肷��
extern int NS_SetPlayFinishDeleteSoundMem( int DeleteFlag, int SoundHandle )
{
	SOUND *sd ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// ���܂łƃt���O�������ꍇ�͉������Ȃ�
	if( sd->PlayFinishDeleteFlag == DeleteFlag )
	{
		return 0 ;
	}

	// �t���O��ۑ�
	sd->PlayFinishDeleteFlag = DeleteFlag ;

	// �t���O�����Ă���ꍇ�̓��X�g�ɒǉ�����A�|�����ꍇ�̓��X�g����O��
	if( DeleteFlag )
	{
		AddHandleList( &SoundSysData.PlayFinishDeleteSoundListFirst, &sd->PlayFinishDeleteSoundList, SoundHandle, sd ) ;
	}
	else
	{
		SubHandleList( &sd->PlayFinishDeleteSoundList ) ;
	}

	// ����I��
	return 0 ;
}

// �T�E���h�n���h���̂R�c�T�E���h�p�̃��o�[�u�p�����[�^��ݒ肷��
extern int NS_Set3DReverbParamSoundMem( const SOUND3D_REVERB_PARAM *Param, int SoundHandle )
{
	SOUND *sd ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( sd->Is3DSound == FALSE )
		return -1 ;

	// �p�����[�^��ۑ�
	sd->PresetReverbParam = -1 ;
	_MEMCPY( &sd->ReverbParam, Param, sizeof( sd->ReverbParam ) ) ;

	// �o�b�t�@�̐������J��Ԃ�
	for( i = 0 ; i < sd->ValidBufferNum ; i ++ )
	{
		// �p�����[�^�̍X�V
		SoundBuffer_SetReverbParam( &sd->Buffer[ i ], &sd->ReverbParam ) ;
	}

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̂R�c�T�E���h�p�̃��o�[�u�p�����[�^���v���Z�b�g���g�p���Đݒ肷��
extern int NS_Set3DPresetReverbParamSoundMem( int PresetNo /* DX_REVERB_PRESET_DEFAULT �� */ , int SoundHandle )
{
	SOUND *sd ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;
	if( PresetNo < 0 || PresetNo >= DX_REVERB_PRESET_NUM )
		return -1 ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( sd->Is3DSound == FALSE )
		return -1 ;

	// �p�����[�^��ۑ�
	sd->PresetReverbParam = PresetNo ;

	// �o�b�t�@�̐������J��Ԃ�
	for( i = 0 ; i < sd->ValidBufferNum ; i ++ )
	{
		// �p�����[�^�̍X�V
		SoundBuffer_SetPresetReverbParam( &sd->Buffer[ i ], PresetNo ) ;
	}

	// �I��
	return 0 ;
}

// �S�Ă̂R�c�T�E���h�̃T�E���h�n���h���Ƀ��o�[�u�p�����[�^��ݒ肷��
extern int NS_Set3DReverbParamSoundMemAll( const SOUND3D_REVERB_PARAM *Param, int PlaySoundOnly )
{
	HANDLELIST *List ;
	SOUND *Sound ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	if( PlaySoundOnly )
	{
		// �N���e�B�J���Z�N�V�����̎擾
		CRITICALSECTION_LOCK( &SoundSysData.Play3DSoundListCriticalSection ) ;

		List = SoundSysData.Play3DSoundListFirst.Next ;
	}
	else
	{
		// �N���e�B�J���Z�N�V�����̎擾
		CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOUND ].CriticalSection ) ;

		List = SoundSysData._3DSoundListFirst.Next ;
	}
	for( ; List->Next != NULL ; List = List->Next )
	{
		Sound = ( SOUND * )List->Data ;

		// �p�����[�^��ۑ�
		Sound->PresetReverbParam = -1 ;
		_MEMCPY( &Sound->ReverbParam, Param, sizeof( Sound->ReverbParam ) ) ;

		// �o�b�t�@�̐������J��Ԃ�
		for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
		{
			// �p�����[�^�̍X�V
			SoundBuffer_SetReverbParam( &Sound->Buffer[ i ], &Sound->ReverbParam ) ;
		}
	}

	if( PlaySoundOnly )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &SoundSysData.Play3DSoundListCriticalSection ) ;
	}
	else
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &SoundSysData._3DSoundListCriticalSection ) ;
	}

	// �I��
	return 0 ;
}

// �S�Ă̂R�c�T�E���h�̃T�E���h�n���h���Ƀ��o�[�u�p�����[�^���v���Z�b�g���g�p���Đݒ肷��
extern int NS_Set3DPresetReverbParamSoundMemAll( int PresetNo /* DX_REVERB_PRESET_DEFAULT �� */, int PlaySoundOnly )
{
	HANDLELIST *List ;
	SOUND *Sound ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	if( PresetNo < 0 || PresetNo >= DX_REVERB_PRESET_NUM )
		return -1 ;

	if( PlaySoundOnly )
	{
		// �N���e�B�J���Z�N�V�����̎擾
		CRITICALSECTION_LOCK( &SoundSysData.Play3DSoundListCriticalSection ) ;

		List = SoundSysData.Play3DSoundListFirst.Next ;
	}
	else
	{
		// �N���e�B�J���Z�N�V�����̎擾
		CRITICALSECTION_LOCK( &SoundSysData._3DSoundListCriticalSection ) ;

		List = SoundSysData._3DSoundListFirst.Next ;
	}
	for( ; List->Next != NULL ; List = List->Next )
	{
		Sound = ( SOUND * )List->Data ;

		// �p�����[�^��ۑ�
		Sound->PresetReverbParam = PresetNo ;

		// �o�b�t�@�̐������J��Ԃ�
		for( i = 0 ; i < Sound->ValidBufferNum ; i ++ )
		{
			// �p�����[�^�̍X�V
			SoundBuffer_SetPresetReverbParam( &Sound->Buffer[ i ], PresetNo ) ;
		}
	}

	if( PlaySoundOnly )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &SoundSysData.Play3DSoundListCriticalSection ) ;
	}
	else
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &SoundSysData._3DSoundListCriticalSection ) ;
	}

	// �I��
	return 0 ;
}

// �T�E���h�n���h���ɐݒ肳��Ă���R�c�T�E���h�p�̃��o�[�u�p�����[�^���擾����
extern int NS_Get3DReverbParamSoundMem( SOUND3D_REVERB_PARAM *ParamBuffer, int SoundHandle )
{
	SOUND *sd ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( sd->Is3DSound == FALSE )
		return -1 ;

	// �p�����[�^���R�s�[
	if( ParamBuffer != NULL ) *ParamBuffer = sd->ReverbParam ;

	// ����I��
	return 0 ;
}

// �v���Z�b�g�̂R�c�T�E���h�p�̃��o�[�u�p�����[�^���擾����
extern int NS_Get3DPresetReverbParamSoundMem( SOUND3D_REVERB_PARAM *ParamBuffer, int PresetNo /* DX_REVERB_PRESET_DEFAULT �� */ )
{
	// �G���[����
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	if( PresetNo < 0 || PresetNo >= DX_REVERB_PRESET_NUM )
	{
		return -1 ;
	}

	// ���ˑ�����
	if( Get3DPresetReverbParamSoundMem_PF( ParamBuffer, PresetNo ) < 0 )
	{
		return -1 ;
	}

	// ����I��
	return 0 ;
}

// �T�E���h�n���h���̂R�c�T�E���h�p�̍Đ��ʒu��ݒ肷��
extern int NS_Set3DPositionSoundMem( VECTOR Position, int SoundHandle )
{
	SOUND *sd ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( sd->Is3DSound == FALSE )
		return -1 ;

	// 1.0f = �P���[�g���̒P�ʂɕϊ�����
	VectorScale( &Position, &Position, 1.0f / SoundSysData._3DSoundOneMetre ) ;

	// �o�b�t�@�̐������J��Ԃ�
	for( i = 0 ; i < sd->ValidBufferNum ; i ++ )
	{
		// �p�����[�^�̍X�V
		SoundBuffer_Set3DPosition( &sd->Buffer[ i ], &Position ) ;
	}
	sd->Base3DPosition = Position ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̂R�c�T�E���h�p�̉����������鋗����ݒ肷��
extern int NS_Set3DRadiusSoundMem( float Radius, int SoundHandle )
{
	SOUND *sd ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( sd->Is3DSound == FALSE )
		return -1 ;

	Radius /= SoundSysData._3DSoundOneMetre ;

	// �o�b�t�@�̐������J��Ԃ�
	for( i = 0 ; i < sd->ValidBufferNum ; i ++ )
	{
		// �p�����[�^�̍X�V
		SoundBuffer_Set3DRadius( &sd->Buffer[ i ], Radius ) ;
	}
	sd->Base3DRadius = Radius ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̂R�c�T�E���h�p�̉��ʂP�O�O���ŕ������鋗����ݒ肷��
extern int NS_Set3DInnerRadiusSoundMem( float Radius, int SoundHandle )
{
	SOUND *sd ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( sd->Is3DSound == FALSE )
		return -1 ;

	Radius /= SoundSysData._3DSoundOneMetre ;

	for( i = 0 ; i < sd->ValidBufferNum ; i ++ )
	{
		// �p�����[�^�̍X�V
		SoundBuffer_Set3DInnerRadius( &sd->Buffer[ i ], Radius ) ;
	}

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̂R�c�T�E���h�p�̈ړ����x��ݒ肷��
extern int NS_Set3DVelocitySoundMem( VECTOR Velocity, int SoundHandle )
{
	SOUND *sd ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( sd->Is3DSound == FALSE )
		return -1 ;

	// 1.0f = �P���[�g���̒P�ʂɕϊ�����
	VectorScale( &Velocity, &Velocity, 1.0f / SoundSysData._3DSoundOneMetre ) ;

	// �o�b�t�@�̐������J��Ԃ�
	for( i = 0 ; i < sd->ValidBufferNum ; i ++ )
	{
		// �p�����[�^�̍X�V
		SoundBuffer_Set3DVelocity( &sd->Buffer[ i ], &Velocity ) ;
	}
	sd->Base3DVelocity = Velocity ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̂R�c�T�E���h�p�̕��������肷�邽�߂̑O�����W�Ə������ݒ肷��
extern int NS_Set3DFrontPositionSoundMem( VECTOR FrontPosition, VECTOR UpVector, int SoundHandle )
{
	SOUND *sd ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( sd->Is3DSound == FALSE )
		return -1 ;

	// �o�b�t�@�̐������J��Ԃ�
	for( i = 0 ; i < sd->ValidBufferNum ; i ++ )
	{
		// �p�����[�^�̍X�V
		SoundBuffer_Set3DFrontPosition( &sd->Buffer[ i ], &FrontPosition, &UpVector ) ;
	}

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̂R�c�T�E���h�p�̕��������肷�邽�߂̑O�����W��ݒ肷��( �������Y�� )
extern int NS_Set3DFrontPosition_UpVecYSoundMem( VECTOR FrontPosition, int SoundHandle )
{
	VECTOR UpVec = { 0.0f, 1.0f, 0.0f } ;

	return NS_Set3DFrontPositionSoundMem( FrontPosition, UpVec, SoundHandle ) ;
}

// �T�E���h�n���h���̂R�c�T�E���h�p�̕����̊p�x�͈͂�ݒ肷��
extern int NS_Set3DConeAngleSoundMem( float InnerAngle, float OuterAngle, int SoundHandle )
{
	SOUND *sd ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( sd->Is3DSound == FALSE )
		return -1 ;

	if( OuterAngle < 0.0f )
	{
		OuterAngle = 0.0f ;
	}
	else
	if( OuterAngle > DX_PI_F * 2.0f )
	{
		OuterAngle = DX_PI_F * 2.0f ;
	}

	if( InnerAngle < 0.0f )
	{
		InnerAngle = 0.0f ;
	}
	else
	if( InnerAngle > DX_PI_F * 2.0f )
	{
		InnerAngle = DX_PI_F * 2.0f ;
	}

	if( OuterAngle < InnerAngle )
	{
		float Temp ;
		Temp = InnerAngle ;
		InnerAngle = OuterAngle ;
		OuterAngle = Temp ;
	}

	for( i = 0 ; i < sd->ValidBufferNum ; i ++ )
	{
		// �p�����[�^�̍X�V
		SoundBuffer_Set3DConeAngle( &sd->Buffer[ i ], InnerAngle, OuterAngle ) ;
	}

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̂R�c�T�E���h�p�̕����̊p�x�͈͂̉��ʔ{����ݒ肷��
extern int NS_Set3DConeVolumeSoundMem( float InnerAngleVolume, float OuterAngleVolume, int SoundHandle )
{
	SOUND *sd ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( sd->Is3DSound == FALSE )
		return -1 ;

	// �o�b�t�@�[�̐������J��Ԃ�
	for( i = 0 ; i < sd->ValidBufferNum ; i ++ )
	{
		// �p�����[�^�̍X�V
		SoundBuffer_Set3DConeVolume( &sd->Buffer[ i ], InnerAngleVolume, OuterAngleVolume ) ;
	}

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̎��̍Đ��݂̂Ɏg�p����R�c�T�E���h�p�̍Đ��ʒu��ݒ肷��
extern int NS_SetNextPlay3DPositionSoundMem( VECTOR Position, int SoundHandle )
{
	SOUND * Sound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( Sound->Is3DSound == FALSE )
		return -1 ;

	// 1.0f = �P���[�g���̒P�ʂɕϊ�����
	VectorScale( &Position, &Position, 1.0f / SoundSysData._3DSoundOneMetre ) ;

	Sound->NextPlay3DPosition = Position ;
	Sound->ValidNextPlay3DPosition = 1 ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̎��̍Đ��݂̂Ɏg�p����R�c�T�E���h�p�̉����������鋗����ݒ肷��
extern int NS_SetNextPlay3DRadiusSoundMem( float Radius, int SoundHandle )
{
	SOUND * Sound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( Sound->Is3DSound == FALSE )
		return -1 ;

	Radius /= SoundSysData._3DSoundOneMetre ;

	Sound->NextPlay3DRadius = Radius ;
	Sound->ValidNextPlay3DRadius = 1 ;

	// �I��
	return 0 ;
}

// �T�E���h�n���h���̎��̍Đ��݂̂Ɏg�p����R�c�T�E���h�p�̈ړ����x��ݒ肷��
extern int NS_SetNextPlay3DVelocitySoundMem( VECTOR Velocity, int SoundHandle )
{
	SOUND * Sound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, Sound ) )
		return -1 ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( Sound->Is3DSound == FALSE )
		return -1 ;

	// 1.0f = �P���[�g���̒P�ʂɕϊ�����
	VectorScale( &Velocity, &Velocity, 1.0f / SoundSysData._3DSoundOneMetre ) ;

	Sound->NextPlay3DVelocity = Velocity ;
	Sound->ValidNextPlay3DVelocity = 1 ;

	// �I��
	return 0 ;
}










// �ݒ�֌W�֐�

// �쐬����T�E���h�̃f�[�^�`����ݒ肷��
extern int NS_SetCreateSoundDataType( int SoundDataType )
{
	// �l���͈͊O�̃f�[�^�`�����ǂ������ׂ�
	if( SoundDataType >= DX_SOUNDDATATYPE_MEMNOPRESS && SoundDataType <= DX_SOUNDDATATYPE_FILE )
	{
		// ���� DX_SOUNDDATATYPE_MEMNOPRESS_PLUS �͔�Ή�
		if( SoundDataType == DX_SOUNDDATATYPE_MEMNOPRESS_PLUS )
		{
			SoundSysData.CreateSoundDataType = DX_SOUNDDATATYPE_MEMNOPRESS ;
		}
		else
		{
			SoundSysData.CreateSoundDataType = SoundDataType ;
		}
	}
	else
	{
		return -1 ; 
	}

	// �I��
	return 0 ;
}

// �쐬����T�E���h�̃f�[�^�`�����擾����( DX_SOUNDDATATYPE_MEMNOPRESS �� )
extern	int NS_GetCreateSoundDataType( void )
{
	return SoundSysData.CreateSoundDataType ;
}

// �쐬����T�E���h�n���h���̃s�b�`���[�g��ݒ肷��( �P�ʂ̓Z���g( 100.0f�Ŕ����A1200.0f�łP�I�N�^�[�� )�A�v���X�̒l�ŉ����������A�}�C�i�X�̒l�ŉ������Ⴍ�Ȃ�܂� )
extern	int NS_SetCreateSoundPitchRate( float Cents )
{
//	if( PitchRate < 0.25f || PitchRate > 2.0f )
//	{
//		return -1 ;
//	}

	if( Cents > -0.0000001 && Cents < 0.0000001f )
	{
		SoundSysData.CreateSoundPitchRateEnable = FALSE ;
		SoundSysData.CreateSoundPitchRate = 1.0f ;
		SoundSysData.CreateSoundPitchRate_Cents = 0.0f ;
	}
	else
	{
		SoundSysData.CreateSoundPitchRateEnable = TRUE ;
		SoundSysData.CreateSoundPitchRate = _POW( 2.0f, Cents / 1200.0f ) ;
		SoundSysData.CreateSoundPitchRate_Cents = Cents ;
	}

	// ����I��
	return 0 ;
}

// �쐬����T�E���h�n���h���̃s�b�`���[�g���擾����( �P�ʂ̓Z���g( 100.0f�Ŕ����A1200.0f�łP�I�N�^�[�� )�A�v���X�̒l�ŉ����������A�}�C�i�X�̒l�ŉ������Ⴍ�Ȃ�܂� )
extern	float NS_GetCreateSoundPitchRate( void )
{
	return SoundSysData.CreateSoundPitchRate_Cents ;
}

// �쐬����T�E���h�n���h���̃^�C���X�g���b�`( ������ς����ɉ��̒�����ύX���� )���[�g��ݒ肷��( �P�ʂ͔{���A2.0f �ŉ��̒������Q�{�ɁA0.5f �ŉ��̒����������ɂȂ�܂� )
extern int NS_SetCreateSoundTimeStretchRate( float Rate )
{
	if( Rate > 0.9999999f && Rate < 1.0000001f )
	{
		SoundSysData.CreateSoundTimeStretchRateEnable = FALSE ;
		SoundSysData.CreateSoundTimeStretchRate = 1.0f ;
	}
	else
	{
		SoundSysData.CreateSoundTimeStretchRateEnable = TRUE ;
		SoundSysData.CreateSoundTimeStretchRate = Rate ;
	}

	// ����I��
	return 0 ;
}

// �쐬����T�E���h�n���h���̃^�C���X�g���b�`( ������ς����ɉ��̒�����ύX���� )���[�g���擾����( �P�ʂ͔{���A2.0f �ŉ��̒������Q�{�ɁA0.5f �ŉ��̒����������ɂȂ�܂� )
extern float NS_GetCreateSoundTimeStretchRate( void )
{
	return SoundSysData.CreateSoundTimeStretchRate ;
}

// �쐬����T�E���h�n���h���̃��[�v�͈͂�ݒ肷��( �~���b�P�� )
extern int NS_SetCreateSoundLoopAreaTimePos( int  LoopStartTime, int  LoopEndTime )
{
	// �T���v���P�ʂ̃p�����[�^�͖�����
	SoundSysData.CreateSoundLoopStartSamplePosition = 0 ;
	SoundSysData.CreateSoundLoopEndSamplePosition   = 0 ;

	// �X�^�[�g�ʒu�ƃG���h�ʒu���������A�X�^�[�g�ʒu�̂ق����G���h�ʒu���l���傫���ꍇ�̓p�����[�^�𖳌���
	if( LoopStartTime >= LoopEndTime )
	{
		SoundSysData.CreateSoundLoopStartTimePosition = 0 ;
		SoundSysData.CreateSoundLoopEndTimePosition   = 0 ;
	}
	else
	{
		// �L���Ȓl�̏ꍇ�̂ݕۑ�
		SoundSysData.CreateSoundLoopStartTimePosition = LoopStartTime ;
		SoundSysData.CreateSoundLoopEndTimePosition   = LoopEndTime ;
	}

	// ����I��
	return 0 ;
}

// �쐬����T�E���h�n���h���̃��[�v�͈͂��擾����( �~���b�P�� )
extern int NS_GetCreateSoundLoopAreaTimePos( int *LoopStartTime, int *LoopEndTime )
{
	if( LoopStartTime != NULL ) *LoopStartTime = SoundSysData.CreateSoundLoopStartTimePosition ;
	if( LoopEndTime   != NULL ) *LoopEndTime   = SoundSysData.CreateSoundLoopEndTimePosition ;

	// ����I��
	return 0 ;
}

// �쐬����T�E���h�n���h���̃��[�v�͈͂�ݒ肷��( �T���v���P�� )
extern int NS_SetCreateSoundLoopAreaSamplePos( int  LoopStartSamplePosition, int  LoopEndSamplePosition )
{
	// �~���b�P�ʂ̃p�����[�^�͖�����
	SoundSysData.CreateSoundLoopStartTimePosition = 0 ;
	SoundSysData.CreateSoundLoopEndTimePosition   = 0 ;

	// �X�^�[�g�ʒu�ƃG���h�ʒu���������A�X�^�[�g�ʒu�̂ق����G���h�ʒu���l���傫���ꍇ�̓p�����[�^�𖳌���
	if( LoopStartSamplePosition >= LoopEndSamplePosition )
	{
		SoundSysData.CreateSoundLoopStartSamplePosition = 0 ;
		SoundSysData.CreateSoundLoopEndSamplePosition   = 0 ;
	}
	else
	{
		// �L���Ȓl�̏ꍇ�̂ݕۑ�
		SoundSysData.CreateSoundLoopStartSamplePosition = LoopStartSamplePosition ;
		SoundSysData.CreateSoundLoopEndSamplePosition   = LoopEndSamplePosition ;
	}

	// ����I��
	return 0 ;
}

// �쐬����T�E���h�n���h���̃��[�v�͈͂��擾����( �T���v���P�� )
extern int NS_GetCreateSoundLoopAreaSamplePos( int *LoopStartSamplePosition, int *LoopEndSamplePosition )
{
	if( LoopStartSamplePosition != NULL ) *LoopStartSamplePosition = SoundSysData.CreateSoundLoopStartSamplePosition ;
	if( LoopEndSamplePosition   != NULL ) *LoopEndSamplePosition   = SoundSysData.CreateSoundLoopEndSamplePosition ;

	// ����I��
	return 0 ;
}

// �g�p���Ȃ��T�E���h�f�[�^�ǂݍ��ݏ����̃}�X�N��ݒ肷��
extern	int NS_SetDisableReadSoundFunctionMask( int Mask )
{
	SoundSysData.DisableReadSoundFunctionMask = Mask ;

	// �I��
	return 0 ;
}

// �g�p���Ȃ��T�E���h�f�[�^�ǂݍ��ݏ����̃}�X�N���擾����
extern	int	NS_GetDisableReadSoundFunctionMask( void )
{
	return SoundSysData.DisableReadSoundFunctionMask ;
}

// �T�E���h�L���v�`����O��Ƃ�����������邩�ǂ�����ݒ肷��
extern	int NS_SetEnableSoundCaptureFlag( int Flag )
{
	// �t���O�������ꍇ�͉������Ȃ�
	if( SoundSysData.EnableSoundCaptureFlag == Flag ) return 0 ;

	// �S�ẴT�E���h�n���h�����폜����
	NS_InitSoundMem() ;
	
	// �t���O���Z�b�g����
	SoundSysData.EnableSoundCaptureFlag = Flag ;
	
	// �I��
	return 0 ;
}

// ChangeVolumeSoundMem, ChangeNextPlayVolumeSoundMem, ChangeMovieVolumeToGraph �̉��ʌv�Z���� Ver3.10c�ȑO�̂��̂��g�p���邩�ǂ�����ݒ肷��( TRUE:Ver3.10c�ȑO�̌v�Z�����g�p  FALSE:3.10d�ȍ~�̌v�Z�����g�p( �f�t�H���g ) )
extern int NS_SetUseOldVolumeCalcFlag( int Flag )
{
	// �t���O��ۑ�����
	SoundSysData.OldVolumeTypeFlag = Flag ;
	
	// �I��
	return 0 ;
}

// ���ɍ쐬����T�E���h���R�c�T�E���h�p�ɂ��邩�ǂ�����ݒ肷��( TRUE:�R�c�T�E���h�p�ɂ���  FALSE:�R�c�T�E���h�p�ɂ��Ȃ�( �f�t�H���g ) )
extern int NS_SetCreate3DSoundFlag( int Flag )
{
	// �t���O��ۑ�����
	SoundSysData.Create3DSoundFlag = Flag != FALSE ? TRUE : FALSE ;

	// �I��
	return 0 ;
}

// �R�c��Ԃ̂P���[�g���ɓ��鋗����ݒ肷��( �f�t�H���g:1.0f )
extern int NS_Set3DSoundOneMetre( float Distance )
{
	if( CheckSoundSystem_Initialize_PF() ) return -1 ;

	if( Distance <= 0.0f )
	{
		SoundSysData._3DSoundOneMetreEnable = FALSE ;
		SoundSysData._3DSoundOneMetre = 1.0f ;
	}
	else
	{
		SoundSysData._3DSoundOneMetreEnable = TRUE ;
		SoundSysData._3DSoundOneMetre = Distance ;
	}

	// �I��
	return 0 ;
}

// �R�c�T�E���h�̃��X�i�[�̈ʒu�ƃ��X�i�[�̑O���ʒu��ݒ肷��( ���X�i�[�̏�����ʒu�͂x���Œ� )
extern int NS_Set3DSoundListenerPosAndFrontPos_UpVecY( VECTOR Position, VECTOR FrontPosition )
{
	VECTOR UpVec = { 0.0f, 1.0f, 0.0f } ;

	return NS_Set3DSoundListenerPosAndFrontPosAndUpVec( Position, FrontPosition, UpVec ) ;
}

// �R�c�T�E���h�̃��X�i�[�̈ʒu�ƃ��X�i�[�̑O���ʒu�ƃ��X�i�[�̏�����ʒu��ݒ肷��
extern int NS_Set3DSoundListenerPosAndFrontPosAndUpVec( VECTOR Position, VECTOR FrontPosition, VECTOR UpVector )
{
	VECTOR SideVec ;
	VECTOR DirVec ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	VectorSub( &DirVec, &FrontPosition, &Position ) ;
	VectorNormalize( &DirVec, &DirVec ) ;

	VectorScale( &Position, &Position, 1.0f / SoundSysData._3DSoundOneMetre ) ;

	VectorOuterProduct( &SideVec, &DirVec,  &UpVector ) ;
	VectorOuterProduct( &UpVector, &SideVec, &DirVec ) ;
	VectorNormalize( &UpVector, &UpVector ) ;
	VectorNormalize( &SideVec, &SideVec ) ;

	// �l���قƂ�Ǖω����Ȃ��ꍇ�ꍇ�͉������Ȃ�
	if( _FABS( SoundSysData.ListenerInfo.Position.x       - Position.x ) < 0.001f &&
		_FABS( SoundSysData.ListenerInfo.Position.y       - Position.y ) < 0.001f &&
		_FABS( SoundSysData.ListenerInfo.Position.z       - Position.z ) < 0.001f &&
		_FABS( SoundSysData.ListenerInfo.FrontDirection.x - DirVec.x   ) < 0.001f &&
		_FABS( SoundSysData.ListenerInfo.FrontDirection.y - DirVec.y   ) < 0.001f &&
		_FABS( SoundSysData.ListenerInfo.FrontDirection.z - DirVec.z   ) < 0.001f &&
		_FABS( SoundSysData.ListenerInfo.UpDirection.x    - UpVector.x ) < 0.001f &&
		_FABS( SoundSysData.ListenerInfo.UpDirection.y    - UpVector.y ) < 0.001f &&
		_FABS( SoundSysData.ListenerInfo.UpDirection.z    - UpVector.z ) < 0.001f )
		return 0 ;

	SoundSysData.ListenerInfo.Position       = Position ;
	SoundSysData.ListenerInfo.FrontDirection = DirVec ;
	SoundSysData.ListenerInfo.UpDirection    = UpVector ;
	SoundSysData.ListenerSideDirection       = SideVec ;

	// ���ˑ�����
	if( Set3DSoundListenerPosAndFrontPosAndUpVec_PF( Position, FrontPosition, UpVector ) < 0 )
	{
		return -1 ;
	}

	// �Đ����̂R�c�T�E���h�̃p�����[�^���X�V����
	Refresh3DSoundParamAll() ;

	// �I��
	return 0 ;
}

// �R�c�T�E���h�̃��X�i�[�̈ړ����x��ݒ肷��
extern int NS_Set3DSoundListenerVelocity( VECTOR Velocity )
{
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	VectorScale( &Velocity, &Velocity, 1.0f / SoundSysData._3DSoundOneMetre ) ;

	// �l���قƂ�Ǖω����Ȃ��ꍇ�ꍇ�͉������Ȃ�
	if( _FABS( SoundSysData.ListenerInfo.Velocity.x - Velocity.x ) < 0.001f &&
		_FABS( SoundSysData.ListenerInfo.Velocity.y - Velocity.y ) < 0.001f &&
		_FABS( SoundSysData.ListenerInfo.Velocity.z - Velocity.z ) < 0.001f )
	{
		return 0 ;
	}

	// ���ˑ�����
	if( Set3DSoundListenerVelocity_PF( Velocity ) < 0 )
	{
		return -1 ;
	}

	SoundSysData.ListenerInfo.Velocity = Velocity ;


	// �Đ����̂R�c�T�E���h�̃p�����[�^���X�V����
	Refresh3DSoundParamAll() ;

	// �I��
	return 0 ;
}

// �R�c�T�E���h�̃��X�i�[�̉��p�x�͈͂�ݒ肷��
extern int NS_Set3DSoundListenerConeAngle( float InnerAngle, float OuterAngle )
{
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	if( OuterAngle < 0.0f )
	{
		OuterAngle = 0.0f ;
	}
	else
	if( OuterAngle > DX_PI_F * 2.0f )
	{
		OuterAngle = DX_PI_F * 2.0f ;
	}

	if( InnerAngle < 0.0f )
	{
		InnerAngle = 0.0f ;
	}
	else
	if( InnerAngle > DX_PI_F * 2.0f )
	{
		InnerAngle = DX_PI_F * 2.0f ;
	}

	if( OuterAngle < InnerAngle )
	{
		float Temp ;
		Temp = InnerAngle ;
		InnerAngle = OuterAngle ;
		OuterAngle = Temp ;
	}

	// �l���قƂ�Ǖω����Ȃ��ꍇ�ꍇ�͉������Ȃ�
	if( _FABS( SoundSysData.ListenerInfo.InnerAngle - InnerAngle ) < 0.001f &&
		_FABS( SoundSysData.ListenerInfo.OuterAngle - OuterAngle ) < 0.001f )
	{
		return 0 ;
	}

	SoundSysData.ListenerInfo.InnerAngle = InnerAngle ;
	SoundSysData.ListenerInfo.OuterAngle = OuterAngle ;

	// ���ˑ�����
	if( Set3DSoundListenerConeAngle_PF( InnerAngle, OuterAngle ) < 0 )
	{
		return -1 ;
	}

	// �Đ����̂R�c�T�E���h�̃p�����[�^���X�V����
	Refresh3DSoundParamAll() ;

	// �I��
	return 0 ;
}

// �R�c�T�E���h�̃��X�i�[�̉��p�x�͈͂̉��ʔ{����ݒ肷��
extern int NS_Set3DSoundListenerConeVolume( float InnerAngleVolume, float OuterAngleVolume )
{
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �l���قƂ�Ǖω����Ȃ��ꍇ�ꍇ�͉������Ȃ�
	if( _FABS( SoundSysData.ListenerInfo.InnerVolume - InnerAngleVolume ) < 0.001f &&
		_FABS( SoundSysData.ListenerInfo.OuterVolume - OuterAngleVolume ) < 0.001f )
	{
		return 0 ;
	}

	SoundSysData.ListenerInfo.InnerVolume = InnerAngleVolume ;
	SoundSysData.ListenerInfo.OuterVolume = OuterAngleVolume ;

	// ���ˑ�����
	if( Set3DSoundListenerConeVolume_PF( InnerAngleVolume, OuterAngleVolume ) < 0 )
	{
		return -1 ;
	}

	// �Đ����̂R�c�T�E���h�̃p�����[�^���X�V����
	Refresh3DSoundParamAll() ;

	// �I��
	return 0 ;
}










// �⏕�n�֐�

// �T�C�Y�␳
static	int AdjustSoundDataBlock( int Length, SOUND * Sound ) 
{
	return Length / Sound->BufferFormat.nBlockAlign * Sound->BufferFormat.nBlockAlign  ;
}


// �g�`�f�[�^�p
extern WAVEDATA *AllocWaveData( int Size, int UseDoubleSizeBuffer )
{
	WAVEDATA *Data ;
	unsigned int AllocSize ;

	AllocSize = sizeof( WAVEDATA ) + 16 + Size ;
	if( UseDoubleSizeBuffer )
	{
		AllocSize += Size * 2 + 32 ;
	}

	Data = ( WAVEDATA * )DXALLOC( AllocSize ) ;
	if( Data == NULL )
		return NULL ;
	Data->Buffer = ( void * )( ( ( DWORD_PTR )Data + sizeof( WAVEDATA ) + 15 ) / 16 * 16 ) ;
	Data->Bytes = Size ;
	Data->RefCount = 1 ;

	if( UseDoubleSizeBuffer )
	{
		Data->DoubleSizeBuffer = ( void * )( ( ( DWORD_PTR )Data->Buffer + Size + 15 ) / 16 * 16 ) ;
	}
	else
	{
		Data->DoubleSizeBuffer = NULL ;
	}

	return Data ;
}

extern int ReleaseWaveData( WAVEDATA *Data )
{
	Data->RefCount -- ;
	if( Data->RefCount > 0 )
		return 0 ;

	if( Data->RefCount < 0 )
		return -1 ;

	DXFREE( Data ) ;

	return 0 ;
}

extern WAVEDATA *DuplicateWaveData( WAVEDATA *Data )
{
	Data->RefCount ++ ;
	return Data ;
}




//�T�E���h�o�b�t�@�p
extern int SoundBuffer_Initialize( SOUNDBUFFER *Buffer, DWORD Bytes, WAVEFORMATEX *Format, SOUNDBUFFER *Src, int UseGetCurrentPosition, int IsStream, int Is3DSound )
{
	int i ;

	// �������ς݂̏ꍇ�̓G���[
	if( Buffer->Valid )
	{
		return -1 ;
	}

	Buffer->Wave					= NULL ;
	Buffer->StopTimeState			= 0 ;
	Buffer->StopTime				= 0 ;
	Buffer->Is3DSound				= Is3DSound ;
	Buffer->UseGetCurrentPosition	= UseGetCurrentPosition ;
	Buffer->IsStream				= IsStream ;

	if( SoundSysData.EnableSoundCaptureFlag )
	{
		if( Src != NULL )
		{
			Buffer->Wave = DuplicateWaveData( Src->Wave ) ;
		}
		else
		{
			Buffer->Wave = AllocWaveData( ( int )Bytes ) ;
		}
		if( Buffer->Wave == NULL )
			goto ERR ;
	}
	else
	{
		// ���ˑ�����
		if( SoundBuffer_Initialize_Timing0_PF( Buffer, Bytes, Format, Src, Is3DSound ) < 0 )
		{
			goto ERR ;
		}
	}

	Buffer->State	= FALSE ;
	Buffer->Pos		= 0 ;
	Buffer->CompPos	= 0 ;
	Buffer->Loop	= FALSE ;

	if( Src != NULL )
	{
		Buffer->Pan = Src->Pan ;
		for( i = 0 ; i < SOUNDBUFFER_MAX_CHANNEL_NUM ; i ++ )
		{
			Buffer->Volume[ i ] = Src->Volume[ i ] ;
		}
		Buffer->Frequency = Src->Frequency ;
		Buffer->SampleNum = Src->SampleNum ;
		Buffer->Format    = Src->Format ;
	}
	else
	{
		Buffer->Pan = 0 ;
		for( i = 0 ; i < SOUNDBUFFER_MAX_CHANNEL_NUM ; i ++ )
		{
			Buffer->Volume[ i ] = 0 ;
		}
		Buffer->Frequency	= -1 ;
		Buffer->SampleNum	= ( int )( Bytes / Format->nBlockAlign ) ;
		Buffer->Format		= *Format ;
	}

	// �R�c�T�E���h�̏ꍇ�͂R�c�T�E���h�p�����[�^��������
	if( Is3DSound != FALSE )
	{
		// �ŏ��ɃZ�b�g�A�b�v���s�����߂ɕύX�����t���O�𗧂Ă�
		Buffer->EmitterDataChangeFlag = TRUE ;

		if( SoundSysData.EnableSoundCaptureFlag == FALSE )
		{
			// ���ˑ�����
			if( SoundBuffer_Initialize_Timing1_PF( Buffer, Src, Is3DSound ) < 0 )
			{
				goto ERR ;
			}
		}

		// �G�~�b�^�[�̊�{�I�ȏ����Z�b�g����
		if( Src != NULL )
		{
			Buffer->EmitterInfo					= Src->EmitterInfo ;
			Buffer->EmitterRadius				= Src->EmitterRadius ;
			Buffer->EmitterInnerRadius			= Src->EmitterInnerRadius ;
		}
		else
		{
			Buffer->EmitterInfo.Position		= VGet( 0.0f, 0.0f, 0.0f ) ;
			Buffer->EmitterInfo.FrontDirection	= VGet( 0.0f, 0.0f, 1.0f ) ;
			Buffer->EmitterInfo.UpDirection		= VGet( 0.0f, 1.0f, 0.0f ) ;
			Buffer->EmitterInfo.Velocity		= VGet( 0.0f, 0.0f, 0.0f ) ;
		}
	}
	
	Buffer->Valid = TRUE ;
	
	return 0 ;

ERR :
	if( Buffer->Wave != NULL )
	{
		ReleaseWaveData( Buffer->Wave ) ;
		Buffer->Wave = NULL ;
	}

	return -1 ;
}

extern int SoundBuffer_Duplicate( SOUNDBUFFER *Buffer, SOUNDBUFFER *Src, int Is3DSound )
{
	return SoundBuffer_Initialize( Buffer, 0, NULL, Src, Src->UseGetCurrentPosition, Src->IsStream, Is3DSound ) ;
}


extern int SoundBuffer_Terminate(          SOUNDBUFFER *Buffer )
{
	if( Buffer->Valid == FALSE )
	{
		return -1 ;
	}

	// ���ˑ�����
	SoundBuffer_Terminate_PF( Buffer ) ;

	if( Buffer->Wave != NULL )
	{
		ReleaseWaveData( Buffer->Wave ) ;
		Buffer->Wave = NULL ;
	}
	
	Buffer->Valid = FALSE ;
	
	return 0 ;
}

extern int SoundBuffer_CheckEnable(         SOUNDBUFFER *Buffer )
{
	if( Buffer->Valid == FALSE )
	{
		return FALSE ;
	}

	return SoundBuffer_CheckEnable_PF( Buffer ) ;
}

extern int SoundBuffer_Play(               SOUNDBUFFER *Buffer, int Loop )
{
	if( Buffer->Valid == FALSE ) return -1 ;
	
	Buffer->Loop = Loop ;
	if( SoundSysData.EnableSoundCaptureFlag )
	{
		Buffer->State = TRUE ;
	}
	else
	{
		// ���ˑ�����
		if( SoundBuffer_Play_PF( Buffer, Loop ) < 0 )
		{
			return -1 ;
		}
	}
	
	return 0 ;
}

extern int SoundBuffer_Stop(               SOUNDBUFFER *Buffer, int EffectStop )
{
	if( Buffer->Valid == FALSE ) return -1 ;
	
	if( SoundSysData.EnableSoundCaptureFlag )
	{
		Buffer->State = FALSE ;
	}
	else
	{
		// ���ˑ�����
		if( SoundBuffer_Stop_PF( Buffer, EffectStop ) < 0 )
		{
			return -1 ;
		}
	}

	return 0 ;
}

extern int SoundBuffer_CheckPlay(          SOUNDBUFFER *Buffer )
{
	if( Buffer->Valid == FALSE ) return -1 ;
	
	if( SoundSysData.EnableSoundCaptureFlag )
	{
		return Buffer->State ;
	}
	else
	{
		// ���ˑ�����
		return SoundBuffer_CheckPlay_PF( Buffer ) ;
	}
}

extern int SoundBuffer_Lock(               SOUNDBUFFER *Buffer, DWORD WritePos , DWORD WriteSize, void **LockPos1, DWORD *LockSize1, void **LockPos2, DWORD *LockSize2 )
{
	if( Buffer->Valid == FALSE )
	{
		return -1 ;
	}
	
	if( SoundSysData.EnableSoundCaptureFlag )
	{
DEFAULTPROCESS :
		DWORD pos, sample, sample1 ;

		pos    = WritePos  / Buffer->Format.nBlockAlign ;
		sample = WriteSize / Buffer->Format.nBlockAlign ;
		if( pos + sample > ( DWORD )Buffer->SampleNum )
		{
			sample1 = Buffer->SampleNum - pos ;
			*LockPos1  = (BYTE *)Buffer->Wave->Buffer + WritePos ;
			*LockSize1 = sample1 * Buffer->Format.nBlockAlign ;
			*LockPos2  = (BYTE *)Buffer->Wave->Buffer ;
			*LockSize2 = ( sample - sample1 ) * Buffer->Format.nBlockAlign ;
		}
		else
		{
			*LockPos1  = (BYTE *)Buffer->Wave->Buffer + WritePos ;
			*LockSize1 = WriteSize ;
			*LockPos2  = NULL ;
			*LockSize2 = 0 ;
		}
	}
	else
	{
		int Result ;

		// ���ˑ�����
		Result = SoundBuffer_Lock_PF( Buffer, WritePos, WriteSize, LockPos1, LockSize1, LockPos2, LockSize2 ) ;
		if( Result == 2 )
		{
			goto DEFAULTPROCESS ;
		}
		if( Result == -1 )
		{
			return -1 ;
		}
	}
	
	return 0 ;
}

extern int SoundBuffer_Unlock(             SOUNDBUFFER *Buffer, void *LockPos1, DWORD LockSize1, void *LockPos2, DWORD LockSize2 )
{
	if( Buffer->Valid == FALSE ) return -1 ;
	
	if( SoundSysData.EnableSoundCaptureFlag )
	{
		// ���ɉ������Ȃ�
	}
	else
	{
		// ���ˑ�����
		if( SoundBuffer_Unlock_PF( Buffer, LockPos1, LockSize1, LockPos2, LockSize2 ) < 0 )
		{
			return -1 ;
		}
	}
	
	return 0 ;
}


extern int SoundBuffer_SetFrequency(       SOUNDBUFFER *Buffer, DWORD Frequency )
{
	if( Buffer->Valid == FALSE ) return -1 ;
	
	if( Frequency == 0 ) Buffer->Frequency = -1 ;
	else                 Buffer->Frequency = ( int )Frequency ;

	if( SoundSysData.EnableSoundCaptureFlag )
	{
	}
	else
	{
		// ���ˑ�����
		if( SoundBuffer_SetFrequency_PF( Buffer, Frequency ) < 0 )
		{
			return -1 ;
		}
	}

	return 0 ;
}

extern int SoundBuffer_GetFrequency(             SOUNDBUFFER *Buffer, DWORD * Frequency )
{
	if( Buffer->Valid == FALSE ) return -1 ;
	
	if( SoundSysData.EnableSoundCaptureFlag )
	{
DEFAULTPROCESS :
		if( Buffer->Frequency < 0 )
		{
			*Frequency = Buffer->Format.nSamplesPerSec ;
		}
		else
		{
			*Frequency = ( DWORD )Buffer->Frequency ;
		}
	}
	else
	{
		int Result ;

		Result = SoundBuffer_GetFrequency_PF( Buffer, Frequency ) ;
		if( Result == 2 )
		{
			goto DEFAULTPROCESS ;
		}
		if( Result < 0 )
		{
			return -1 ;
		}
	}
	
	return 0 ;
}

extern int SoundBuffer_SetPan(             SOUNDBUFFER *Buffer, LONG Pan )
{
	if( Buffer->Valid == FALSE ) return -1 ;

	if( Pan < -10000 )
	{
		Pan = -10000 ;
	}
	else
	if( Pan > 10000 ) 
	{
		Pan = 10000 ;
	}
	Buffer->Pan = Pan ;

	return SoundBuffer_RefreshVolume( Buffer ) ;
}

extern int SoundBuffer_GetPan(             SOUNDBUFFER *Buffer, LPLONG Pan )
{
	if( Buffer->Valid == FALSE ) return -1 ;

	if( Pan != NULL )
	{
		*Pan = Buffer->Pan ;
	}

	return 0 ;
}

extern int SoundBuffer_RefreshVolume( SOUNDBUFFER *Buffer )
{
	if( Buffer->Valid == FALSE ) return -1 ;

	if( SoundSysData.EnableSoundCaptureFlag )
	{
		if( Buffer->Volume[ 0 ] <= -10000 )
		{
			Buffer->CalcVolume = 0 ;
		}
		else
		if( Buffer->Volume[ 0 ] >= 0 )
		{
			Buffer->CalcVolume = 256 ;
		}
		else
		{
			const double Min = 0.0000000001 ;
			const double Max = 1.0 ;
			
			Buffer->CalcVolume = _DTOL( ( _POW( (float)10, Buffer->Volume[ 0 ] / 10.0f / 100.0f ) / ( Max - Min ) ) * 256 ) ;
		}

		if( Buffer->Pan == -10000 )
		{
			Buffer->CalcPan = -256 ;
		}
		else
		if( Buffer->Pan == 10000 ) 
		{
			Buffer->CalcPan = 256 ;
		}
		else
		if( Buffer->Pan == 0 )
		{
			Buffer->CalcPan = 0 ;
		}
		else
		{
			const double Min = 0.0000000001 ;
			const double Max = 1.0 ;
			int temp ;
			
			temp = _DTOL( ( _POW( (float)10, -( _ABS( Buffer->Pan ) ) / 10.0f / 100.0f ) / ( Max - Min ) ) * 256 ) ;
			Buffer->CalcPan = Buffer->Pan < 0 ? -temp : temp ;
		}
	}
	else
	{
		// ���ˑ�����
		if( SoundBuffer_RefreshVolume_PF( Buffer ) < 0 )
		{
			return -1 ;
		}
	}
	
	return 0 ;
}

extern int SoundBuffer_SetVolumeAll( SOUNDBUFFER *Buffer, LONG Volume )
{
	int i ;

	if( Buffer->Valid == FALSE ) return -1 ;

	for( i = 0 ; i < SOUNDBUFFER_MAX_CHANNEL_NUM ; i ++ )
	{
		Buffer->Volume[ i ] = Volume ;
	}

	return SoundBuffer_RefreshVolume( Buffer ) ;
}

extern int SoundBuffer_SetVolume( SOUNDBUFFER *Buffer, int Channel, LONG Volume )
{
	if( Buffer->Valid == FALSE ) return -1 ;

	if( Channel < 0 || Channel >= SOUNDBUFFER_MAX_CHANNEL_NUM ) return -1 ;

	Buffer->Volume[ Channel ] = Volume ;

	return SoundBuffer_RefreshVolume( Buffer ) ;
}

extern int SoundBuffer_GetVolume( SOUNDBUFFER *Buffer, int Channel, LPLONG Volume )
{
	if( Buffer->Valid == FALSE ) return -1 ;

	if( Channel < 0 || Channel >= SOUNDBUFFER_MAX_CHANNEL_NUM ) return -1 ;

	if( Volume )
	{
		*Volume = Buffer->Volume[ Channel ] ;
	}

	return 0 ;
}

extern int SoundBuffer_GetCurrentPosition( SOUNDBUFFER *Buffer, DWORD *PlayPos, DWORD *WritePos )
{
	if( Buffer->Valid == FALSE ) return -1 ;
	
	if( SoundSysData.EnableSoundCaptureFlag )
	{
DEFAULTPROCESS :
		if( PlayPos  ) *PlayPos  = (DWORD)Buffer->Pos * Buffer->Format.nBlockAlign ;
		if( WritePos ) *WritePos = (DWORD)Buffer->Pos * Buffer->Format.nBlockAlign ;
	}
	else
	{
		int Result ;

		// ���ˑ�����
		Result = SoundBuffer_GetCurrentPosition_PF( Buffer, PlayPos, WritePos ) ;
		if( Result == 2 )
		{
			goto DEFAULTPROCESS ;
		}

		if( Result < 0 )
		{
			return -1 ;
		}
	}
	
	return 0 ;
}

extern int SoundBuffer_SetCurrentPosition( SOUNDBUFFER *Buffer, DWORD NewPos )
{
	if( Buffer->Valid == FALSE ) return -1 ;

	if( SoundSysData.EnableSoundCaptureFlag )
	{
		Buffer->Pos     = ( int )( NewPos / Buffer->Format.nBlockAlign ) ;
		Buffer->CompPos = Buffer->Pos ;
	}
	else
	{
		// ���ˑ�����
		if( SoundBuffer_SetCurrentPosition_PF( Buffer, NewPos ) < 0 )
		{
			return -1 ;
		}
	}
	
	return 0 ;
}

extern int SoundBuffer_CycleProcess( SOUNDBUFFER *Buffer )
{
	if( SoundSysData.EnableSoundCaptureFlag )
	{
		return -1 ;
	}
	else
	{
		// ���ˑ�����
		return SoundBuffer_CycleProcess_PF( Buffer ) ;
	}
}


#define DESTADD	\
{									\
	if( vol != 255 )				\
	{								\
		d1 = ( d1 * vol ) >> 8 ;	\
		d2 = ( d2 * vol ) >> 8 ;	\
	}								\
	if( pan != 0 )					\
	{								\
		if( pan > 0 ) d2 = ( d2 * ( 256 -   pan  ) ) >> 8 ;		\
		else          d1 = ( d1 * ( 256 - (-pan) ) ) >> 8 ;		\
	}								\
									\
		 if( DestBuf[0] + d1 >  32767 ) DestBuf[0] =  32767 ;				\
	else if( DestBuf[0] + d1 < -32768 ) DestBuf[0] = -32768 ;				\
	else                                DestBuf[0] += ( short )d1 ; 		\
									\
		 if( DestBuf[1] + d2 >  32767 ) DestBuf[1] =  32767 ;				\
	else if( DestBuf[1] + d2 < -32768 ) DestBuf[1] = -32768 ;				\
	else                                DestBuf[1] += ( short )d2 ; 		\
}

#define CNV( S )		((int)((S) * 65535 / 255) - 32768)

extern int SoundBuffer_FrameProcess(       SOUNDBUFFER *Buffer, int Sample, short *DestBuf )
{
	if( Buffer->Valid == FALSE ) return -1 ;

	if( SoundSysData.EnableSoundCaptureFlag )
	{
		int i, pos, d1, d2, b, ch, rate, bit, vol, pan ;
		short *s ;
		BYTE *sb ;
		
		ch   = Buffer->Format.nChannels ;
		rate = ( int )Buffer->Format.nSamplesPerSec ;
		bit  = Buffer->Format.wBitsPerSample ;
		vol  = Buffer->Volume[ 0 ] ;
		pan  = Buffer->Pan ;
		s    = (short *)( (BYTE *)Buffer->Wave->Buffer + Buffer->Pos * Buffer->Format.nBlockAlign ) ;
		sb   = (BYTE *)s ;

		if( DestBuf != NULL &&
			( rate == 44100 || rate == 22050 ) &&
			( bit  == 16    || bit  == 8     ) &&
			( ch   == 2     || ch   == 1     ) )
		{
			if( rate == 44100 )
			{
				pos = Buffer->Pos ;
				for( i = 0 ; i < Sample ; i ++, DestBuf += 2 )
				{
					if( bit == 16 )
					{
						if( ch == 2 ){      d1 = s[0]; d2 = s[1]; s += 2 ; }
						else         { d2 = d1 = s[0];            s += 1 ; }
					}
					else
					{
						if( ch == 2 ){      d1 = CNV(sb[0]); d2 = CNV(sb[1]); sb += 2 ; }
						else         { d2 = d1 = CNV(sb[0]);                  sb += 1 ; }
					}
					
					DESTADD
					pos ++ ;
					if( pos >= Buffer->SampleNum )
					{
						if( Buffer->Loop == FALSE ) break ;
						
						s = (short *)Buffer->Wave->Buffer ;
						sb = (BYTE *)s ;
						pos = 0 ;
					}
				}
			}
			else
			if( rate == 22050 )
			{
				pos = Buffer->Pos ;
				b = Buffer->Pos & 1;
				for( i = 0 ; i < Sample ; i ++, DestBuf += 2 )
				{
					if( b )
					{
						if( pos + 1 >= Buffer->SampleNum )
						{
							if( Buffer->Loop == FALSE )
							{
								if( bit == 16 )
								{
									if( ch == 2 )
									{
										d1=s[0]+(s[0]-s[-2])/2;
										d2=s[1]+(s[1]-s[-3])/2;
									}
									else
									{
										d2 = d1=s[0]+(s[0]-s[-1])/2;
									}
								}
								else
								{
									if( ch == 2 )
									{
										d1=CNV(sb[0])+(CNV(sb[0])-CNV(sb[-2]))/2;
										d2=CNV(sb[1])+(CNV(sb[1])-CNV(sb[-3]))/2;
									}
									else
									{
										d2 = d1=CNV(sb[0])+(CNV(sb[0])-CNV(sb[-1]))/2;
									}
								}
								break;
							}
							else
							{
								if( bit == 16 )
								{
									if( ch == 2 )
									{
										d1 = s[0] ;
										d2 = s[1] ;
										s = (short *)Buffer->Wave->Buffer ;
										d1=d1+(s[0]-d1)/2;
										d2=d2+(s[1]-d2)/2;
									}
									else
									{
										d1 = s[0] ;
										s = (short *)Buffer->Wave->Buffer ;
										d2 = d1=d1+(s[0]-d1)/2;
									}
								}
								else
								{
									if( ch == 2 )
									{
										d1 = CNV(sb[0]) ;
										d2 = CNV(sb[1]) ;
										sb = (BYTE *)Buffer->Wave->Buffer ;
										d1=d1+(CNV(sb[0])-d1)/2;
										d2=d2+(CNV(sb[1])-d2)/2;
									}
									else
									{
										d1 = CNV(sb[0]) ;
										sb = (BYTE *)Buffer->Wave->Buffer ;
										d2 = d1=d1+(CNV(sb[0])-d1)/2;
									}
								}

								pos = 0 ;
								b = 0;
							}
						}
						else
						{
							if( bit == 16 )
							{
								if( ch == 2 )
								{
									d1=s[0]+(s[2]-s[0])/2;
									d2=s[1]+(s[3]-s[1])/2;
									s += 2 ;
								}
								else
								{
									d2 = d1=s[0]+(s[1]-s[0])/2;
									s += 1 ;
								}
							}
							else
							{
								if( ch == 2 )
								{
									d1=CNV(sb[0]);d1+=(CNV(sb[2])-d1)/2;
									d2=CNV(sb[1]);d2+=(CNV(sb[3])-d2)/2;
									sb += 2 ;
								}
								else
								{
									d1=CNV(sb[0]);d1+=(CNV(sb[1])-d1)/2;
									d2 = d1;
									sb += 1 ;
								}
							}
							pos ++ ;
							b = b ? 0 : 1;
						}
						DESTADD
					}
					else
					{
						b = b ? 0 : 1;
						if( bit == 16 )
						{
							if( ch == 2 ){      d1 = s[0]; d2 = s[1]; }
							else         { d2 = d1 = s[0];            }
						}
						else
						{
							if( ch == 2 ){      d1 = CNV(sb[0]); d2 = CNV(sb[1]); }
							else         { d2 = d1 = CNV(sb[0]);	              }
						}
						DESTADD
					}
				}
				Sample >>= 1;
			}
		}

		if( Buffer->Pos + Sample >= Buffer->SampleNum )
		{
			if( Buffer->Loop == TRUE )
			{
				Buffer->Pos = ( Buffer->Pos + Sample ) - Buffer->SampleNum ;
			}
			else
			{
				Buffer->Pos = Buffer->SampleNum ;
				Buffer->State = FALSE ;
			}
		}
		else
		{
			Buffer->Pos += Sample ;
		}
		Buffer->CompPos = Buffer->Pos ;
	}
	else
	{
	}
	
	return 0 ;
}

extern int SoundBuffer_Set3DPosition( SOUNDBUFFER *Buffer, VECTOR *Position )
{
	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( Buffer->Is3DSound == FALSE )
		return -1 ;

	// �l���قڕω����Ȃ��ꍇ�͉������Ȃ�
	if( _FABS( Buffer->EmitterInfo.Position.x - Position->x ) < 0.001f &&
		_FABS( Buffer->EmitterInfo.Position.y - Position->y ) < 0.001f &&
		_FABS( Buffer->EmitterInfo.Position.z - Position->z ) < 0.001f )
	{
		return 0 ;
	}

	// �l��ۑ�
	Buffer->EmitterInfo.Position = *Position ;

	// ���ˑ�����
	if( SoundBuffer_Set3DPosition_PF( Buffer, Position ) < 0 )
	{
		return -1 ;
	}

	// �f�[�^���ύX���ꂽ�t���O�𗧂Ă�
	Buffer->EmitterDataChangeFlag = TRUE ;

	// �Đ����������ꍇ�̓p�����[�^���X�V
	if( SoundBuffer_CheckPlay( Buffer ) )
	{
		SoundBuffer_Refresh3DSoundParam( Buffer ) ;
	}

	return 0 ;
}

extern int SoundBuffer_Set3DRadius( SOUNDBUFFER *Buffer, float Radius )
{
	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( Buffer->Is3DSound == FALSE )
		return -1 ;

	// �l���قڕω����Ȃ��ꍇ�͉������Ȃ�
	if( _FABS( Buffer->EmitterRadius - Radius ) < 0.001f )
	{
		return 0 ;
	}

	// �������鋗����ۑ�
	Buffer->EmitterRadius = Radius ;

	// ���ˑ�����
	if( SoundBuffer_Set3DRadius_PF( Buffer, Radius ) < 0 )
	{
		return -1 ;
	}

	// �f�[�^���ύX���ꂽ�t���O�𗧂Ă�
	Buffer->EmitterDataChangeFlag = TRUE ;

	// �Đ����������ꍇ�̓p�����[�^���X�V
	if( SoundBuffer_CheckPlay( Buffer ) )
	{
		SoundBuffer_Refresh3DSoundParam( Buffer ) ;
	}

	return 0 ;
}

extern int SoundBuffer_Set3DInnerRadius( SOUNDBUFFER *Buffer, float Radius )
{
	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( Buffer->Is3DSound == FALSE )
		return -1 ;

	// �l���قڕω����Ȃ��ꍇ�͉������Ȃ�
	if( _FABS( Buffer->EmitterInnerRadius - Radius ) < 0.001f )
	{
		return 0 ;
	}

	// �P�O�O���̉��ʂŉ����������鋗����ۑ�
	Buffer->EmitterInnerRadius = Radius ;

	// ���ˑ�����
	if( SoundBuffer_Set3DInnerRadius_PF( Buffer, Radius ) < 0 )
	{
		return -1 ;
	}

	// �f�[�^���ύX���ꂽ�t���O�𗧂Ă�
	Buffer->EmitterDataChangeFlag = TRUE ;

	// �Đ����������ꍇ�̓p�����[�^���X�V
	if( SoundBuffer_CheckPlay( Buffer ) )
	{
		SoundBuffer_Refresh3DSoundParam( Buffer ) ;
	}

	return 0 ;
}

extern int SoundBuffer_Set3DVelocity( SOUNDBUFFER *Buffer, VECTOR *Velocity )
{
	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( Buffer->Is3DSound == FALSE )
		return -1 ;

	// �l���قƂ�Ǖω����Ȃ��ꍇ�ꍇ�͉������Ȃ�
	if( _FABS( Buffer->EmitterInfo.Velocity.x - Velocity->x ) < 0.001f &&
		_FABS( Buffer->EmitterInfo.Velocity.y - Velocity->y ) < 0.001f &&
		_FABS( Buffer->EmitterInfo.Velocity.z - Velocity->z ) < 0.001f )
	{
		return 0 ;
	}

	// �l��ۑ�
	Buffer->EmitterInfo.Velocity = *Velocity ;

	// ���ˑ�����
	if( SoundBuffer_Set3DVelocity_PF( Buffer, Velocity ) < 0 )
	{
		return -1 ;
	}

	// �f�[�^���ύX���ꂽ�t���O�𗧂Ă�
	Buffer->EmitterDataChangeFlag = TRUE ;

	// �Đ����������ꍇ�̓p�����[�^���X�V
	if( SoundBuffer_CheckPlay( Buffer ) )
	{
		SoundBuffer_Refresh3DSoundParam( Buffer ) ;
	}

	return 0 ;
}

extern int SoundBuffer_Set3DFrontPosition( SOUNDBUFFER *Buffer, VECTOR *FrontPosition, VECTOR *UpVector )
{
	VECTOR SideVec ;
	VECTOR DirVec ;
	VECTOR Position ;
	VECTOR UpVectorT ;

	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( Buffer->Is3DSound == FALSE )
		return -1 ;

	Position.x = Buffer->EmitterInfo.Position.x * SoundSysData._3DSoundOneMetre ;
	Position.y = Buffer->EmitterInfo.Position.y * SoundSysData._3DSoundOneMetre ;
	Position.z = Buffer->EmitterInfo.Position.z * SoundSysData._3DSoundOneMetre ;

	VectorSub( &DirVec, FrontPosition, &Position ) ;
	VectorNormalize( &DirVec, &DirVec ) ;

	VectorOuterProduct( &SideVec, &DirVec,  UpVector ) ;
	VectorOuterProduct( &UpVectorT, &SideVec, &DirVec ) ;
	VectorNormalize( &UpVectorT, &UpVectorT ) ;

	// �l���قƂ�Ǖω����Ȃ��ꍇ�ꍇ�͉������Ȃ�
	if( _FABS( Buffer->EmitterInfo.FrontDirection.x - DirVec.x ) < 0.0001f &&
		_FABS( Buffer->EmitterInfo.FrontDirection.y - DirVec.y ) < 0.0001f &&
		_FABS( Buffer->EmitterInfo.FrontDirection.z - DirVec.z ) < 0.0001f &&
		_FABS( Buffer->EmitterInfo.UpDirection.x - UpVectorT.x ) < 0.0001f &&
		_FABS( Buffer->EmitterInfo.UpDirection.y - UpVectorT.y ) < 0.0001f &&
		_FABS( Buffer->EmitterInfo.UpDirection.z - UpVectorT.z ) < 0.0001f )
	{
		return 0 ;
	}

	// �l��ۑ�
	Buffer->EmitterInfo.FrontDirection = DirVec ;
	Buffer->EmitterInfo.UpDirection = UpVectorT ;

	// ���ˑ�����
	if( SoundBuffer_Set3DFrontPosition_PF( Buffer, FrontPosition, UpVector ) < 0 )
	{
		return -1 ;
	}

	// �f�[�^���ύX���ꂽ�t���O�𗧂Ă�
	Buffer->EmitterDataChangeFlag = TRUE ;

	// �Đ����������ꍇ�̓p�����[�^���X�V
	if( SoundBuffer_CheckPlay( Buffer ) )
	{
		SoundBuffer_Refresh3DSoundParam( Buffer ) ;
	}

	return 0 ;
}

extern int SoundBuffer_Set3DConeAngle( SOUNDBUFFER *Buffer, float InnerAngle, float OuterAngle )
{
	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( Buffer->Is3DSound == FALSE )
		return -1 ;

	// �l���قڕω����Ȃ��ꍇ�͉������Ȃ�
	if( _FABS( Buffer->EmitterInfo.InnerAngle - InnerAngle ) < 0.001f &&
		_FABS( Buffer->EmitterInfo.OuterAngle - OuterAngle ) < 0.001f )
	{
		return 0 ;
	}

	Buffer->EmitterInfo.InnerAngle = InnerAngle ;
	Buffer->EmitterInfo.OuterAngle = OuterAngle ;

	// ���ˑ�����
	if( SoundBuffer_Set3DConeAngle_PF( Buffer, InnerAngle, OuterAngle ) < 0 )
	{
		return -1 ;
	}

	// �f�[�^���ύX���ꂽ�t���O�𗧂Ă�
	Buffer->EmitterDataChangeFlag = TRUE ;

	// �Đ����������ꍇ�̓p�����[�^���X�V
	if( SoundBuffer_CheckPlay( Buffer ) )
	{
		SoundBuffer_Refresh3DSoundParam( Buffer ) ;
	}

	return 0 ;
}

extern int SoundBuffer_Set3DConeVolume( SOUNDBUFFER *Buffer, float InnerAngleVolume, float OuterAngleVolume )
{
	// �R�c�T�E���h�ł͂Ȃ��ꍇ�͉������Ȃ�
	if( Buffer->Is3DSound == FALSE )
		return -1 ;

	// �l���قڕω����Ȃ��ꍇ�͉������Ȃ�
	if( _FABS( Buffer->EmitterInfo.InnerVolume - InnerAngleVolume ) < 0.001f &&
		_FABS( Buffer->EmitterInfo.OuterVolume - OuterAngleVolume ) < 0.001f )
	{
		return 0 ;
	}

	Buffer->EmitterInfo.InnerVolume = InnerAngleVolume ;
	Buffer->EmitterInfo.OuterVolume = OuterAngleVolume ;

	// ���ˑ�����
	if( SoundBuffer_Set3DConeVolume_PF( Buffer, InnerAngleVolume, OuterAngleVolume ) < 0 )
	{
		return -1 ;
	}

	// �f�[�^���ύX���ꂽ�t���O�𗧂Ă�
	Buffer->EmitterDataChangeFlag = TRUE ;

	// �Đ����������ꍇ�̓p�����[�^���X�V
	if( SoundBuffer_CheckPlay( Buffer ) )
	{
		SoundBuffer_Refresh3DSoundParam( Buffer ) ;
	}

	return 0 ;
}


extern int SoundBuffer_Refresh3DSoundParam( SOUNDBUFFER *Buffer, int AlwaysFlag )
{
	if( Buffer->Is3DSound == FALSE || Buffer->Valid == FALSE )
	{
		return -1 ;
	}

	// �K�����s����t���O���|��Ă��āA�f�[�^���ύX���ꂽ�t���O���|��Ă����牽�����Ȃ�
	if( AlwaysFlag == FALSE && Buffer->EmitterDataChangeFlag == FALSE )
	{
		return 0 ;
	}

	if( SoundSysData.EnableSoundCaptureFlag )
	{
	}
	else
	{
		// ���ˑ�����
		if( SoundBuffer_Refresh3DSoundParam_PF( Buffer, AlwaysFlag ) < 0 )
		{
			return -1 ;
		}
	}

	// �f�[�^���ύX���ꂽ�t���O��|��
	Buffer->EmitterDataChangeFlag = FALSE ;

	// �I��
	return 0 ;
}

extern int SoundBuffer_SetReverbParam( SOUNDBUFFER *Buffer, SOUND3D_REVERB_PARAM *Param )
{
	if( Buffer->Is3DSound == FALSE || Buffer->Valid == FALSE ) return -1 ;

	if( SoundSysData.EnableSoundCaptureFlag )
	{
	}
	else
	{
		// ���ˑ�����
		if( SoundBuffer_SetReverbParam_PF( Buffer, Param ) < 0 )
		{
			return -1 ;
		}
	}

	// �I��
	return 0 ;
}

extern int SoundBuffer_SetPresetReverbParam( SOUNDBUFFER *Buffer, int PresetNo )
{
	if( Buffer->Is3DSound == FALSE || Buffer->Valid == FALSE ) return -1 ;

	if( SoundSysData.EnableSoundCaptureFlag )
	{
	}
	else
	{
		// ���ˑ�����
		if( SoundBuffer_SetPresetReverbParam_PF( Buffer, PresetNo ) < 0 )
		{
			return 0 ;
		}
	}

	// �I��
	return 0 ;
}















// ���b�p�[�֐�

// PlaySoundFile �̋�����
extern int NS_PlaySound( const TCHAR *FileName, int PlayType )
{
	return NS_PlaySoundFile( FileName, PlayType ) ;
}

// PlaySoundFile �̋�����
extern int PlaySound_WCHAR_T( const wchar_t *FileName, int PlayType )
{
	return PlaySoundFile_WCHAR_T( FileName, PlayType ) ;
}

// CheckSoundFile �̋�����
extern int NS_CheckSound( void )
{
	return NS_CheckSoundFile() ;
}

// StopSoundFile �̋�����
extern int NS_StopSound( void )
{
	return NS_StopSoundFile() ;
}

// SetVolumeSound �̋�����
extern int NS_SetVolumeSound( int VolumePal )
{
	return NS_SetVolumeSoundFile( VolumePal ) ;
}

// WAVE�t�@�C�����Đ�����
extern int NS_PlaySoundFile( const TCHAR *FileName , int PlayType )
{
#ifdef UNICODE
	return PlaySoundFile_WCHAR_T(
		FileName, PlayType
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = PlaySoundFile_WCHAR_T(
		UseFileNameBuffer, PlayType
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// WAVE�t�@�C�����Đ�����
extern int PlaySoundFile_WCHAR_T( const wchar_t *FileName , int PlayType )
{
	LOADSOUND_GPARAM GParam ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �ȑO�Đ����������f�[�^���~�߂�
	if( SoundSysData.PlayWavSoundHandle != -1 )
	{
		NS_DeleteSoundMem( SoundSysData.PlayWavSoundHandle ) ;
	}

	// �T�E���h�f�[�^��ǂݍ���
	InitLoadSoundGParam( &GParam ) ;
	SoundSysData.PlayWavSoundHandle = LoadSoundMemBase_UseGParam( &GParam, FileName, 1, -1, FALSE, FALSE ) ;
	if( SoundSysData.PlayWavSoundHandle == -1 )
	{
		return -1 ;
	}

	// �T�E���h���Đ�����
	NS_PlaySoundMem( SoundSysData.PlayWavSoundHandle , PlayType ) ;

	// �I��
	return 0 ;
}

// WAVE�t�@�C�����Đ��������ׂ�
extern int NS_CheckSoundFile( void )
{
	int Result ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}
	if( SoundSysData.PlayWavSoundHandle == -1 )
	{
		return 0 ;
	}

	Result = NS_CheckSoundMem( SoundSysData.PlayWavSoundHandle ) ;

	return Result ;
}

// WAVE�t�@�C���̍Đ����~�߂�
extern int NS_StopSoundFile( void )
{
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}
	if( SoundSysData.PlayWavSoundHandle == -1 ) return 0 ;

	return NS_StopSoundMem( SoundSysData.PlayWavSoundHandle ) ;
}

// WAVE�t�@�C���̉��ʂ��Z�b�g����
extern int NS_SetVolumeSoundFile( int VolumePal )
{
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}
	if( SoundSysData.PlayWavSoundHandle == -1 ) return 0 ;

	return NS_SetVolumeSoundMem( VolumePal, SoundSysData.PlayWavSoundHandle ) ;
}

// �T�E���h�L���v�`���̊J�n
extern	int StartSoundCapture( const wchar_t *SaveFilePath )
{
#ifdef DX_NON_SAVEFUNCTION

	return -1 ;

#else // DX_NON_SAVEFUNCTION

	BYTE temp[NORMALWAVE_HEADERSIZE] ;
	
	// �T�E���h�L���v�`���������ȏꍇ�͉��������I��
	if( SoundSysData.EnableSoundCaptureFlag == FALSE ) return -1 ;

	// ���ɃL���v�`�����J�n���Ă���ꍇ�͉��������I��
	if( SoundSysData.SoundCaptureFlag == TRUE ) return -1 ;
	SoundSysData.SoundCaptureFlag = TRUE;
	
	// �ۑ��p�̃t�@�C�����J��
	SoundSysData.SoundCaptureFileHandle = WriteOnlyFileAccessOpen( SaveFilePath ) ;
	if( SoundSysData.SoundCaptureFileHandle == 0 )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xad\x30\xe3\x30\xd7\x30\xc1\x30\xe3\x30\xdd\x4f\x58\x5b\x28\x75\x6e\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x4c\x30\x8b\x95\x51\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x02\x30\x00"/*@ L"�T�E���h�L���v�`���ۑ��p�̃t�@�C�����J���܂���ł����B" @*/ ) ;
		return -1 ;
	}
	
	// �w�b�_�[���̋�f�[�^�������o��
	_MEMSET( temp, 0, sizeof( temp ) ) ;
	WriteOnlyFileAccessWrite( SoundSysData.SoundCaptureFileHandle, temp, NORMALWAVE_HEADERSIZE ) ;
	
	// �T�E���h�L���v�`���̃t���O�𗧂Ă�
	SoundSysData.SoundCaptureFlag = TRUE ;
	
	// �L���v�`�������T���v�������O�ɂ���
	SoundSysData.SoundCaptureSample = 0 ;
	
	// �I��
	return 0 ;
#endif // DX_NON_SAVEFUNCTION
}

// �T�E���h�L���v�`���̎����I����
extern	int SoundCaptureProcess( int CaptureSample )
{
	int i, j, num, k ;
	short *Temp = NULL ;
	SOUND *sound ;

	if( SoundSysData.EnableSoundCaptureFlag == FALSE ) return -1 ;
	
#ifndef DX_NON_SAVEFUNCTION
	// �L���v�`�����s���ꍇ�̓������̊m��
	if( SoundSysData.SoundCaptureFlag == TRUE )
	{
		Temp = (short *)DXALLOC( ( size_t )( CaptureSample * 4 ) ) ;
		_MEMSET( Temp, 0, ( size_t )( CaptureSample * 4 ) ) ;
	}
#endif // DX_NON_SAVEFUNCTION
	
	// �T�E���h�o�b�t�@�̐i�s�������s��
	num = HandleManageArray[ DX_HANDLETYPE_SOUND ].Num ;
	for( i = 0, j = HandleManageArray[ DX_HANDLETYPE_SOUND ].AreaMin ; i < num ; j ++ )
	{
		if( HandleManageArray[ DX_HANDLETYPE_SOUND ].Handle[ j ] == NULL ) continue ;
		i ++ ;
		
		sound = ( SOUND * )HandleManageArray[ DX_HANDLETYPE_SOUND ].Handle[ j ] ;
		switch( sound->Type )
		{
		case DX_SOUNDTYPE_NORMAL :
			for( k = 0 ; k < MAX_SOUNDBUFFER_NUM ; k ++ )
			{
				if( sound->Buffer[ k ].Valid == FALSE || sound->Buffer[ k ].State == FALSE ) continue ;
				SoundBuffer_FrameProcess( &sound->Buffer[ k ], CaptureSample, Temp ) ;
			}
			break ;
		
		case DX_SOUNDTYPE_STREAMSTYLE :
			if( sound->Buffer[ 0 ].Valid && sound->Buffer[ 0 ].State )
				SoundBuffer_FrameProcess( &sound->Buffer[ 0 ], CaptureSample, Temp ) ;
			break ;
		}
	}

#ifndef DX_NON_SAVEFUNCTION
	
	// �L���v�`���p�̃f�[�^�������o��
	if( SoundSysData.SoundCaptureFlag == TRUE )
	{
		WriteOnlyFileAccessWrite( SoundSysData.SoundCaptureFileHandle, Temp, ( DWORD )( CaptureSample * 4 ) ) ;
		SoundSysData.SoundCaptureSample += CaptureSample ;
		
		// �������̉��
		DXFREE( Temp ) ;
	}

#endif // DX_NON_SAVEFUNCTION

	// �I��
	return 0 ;
}

// �T�E���h�L���v�`���̏I��
extern	int EndSoundCapture( void )
{
#ifdef DX_NON_SAVEFUNCTION

	return 0 ;

#else // DX_NON_SAVEFUNCTION

	BYTE Header[NORMALWAVE_HEADERSIZE], *p ;
	WAVEFORMATEX *format;

	// �T�E���h�L���v�`�������s���Ă��Ȃ������ꍇ�͉��������I��
	if( SoundSysData.SoundCaptureFlag == FALSE ) return -1 ;
	
	// �t�H�[�}�b�g���Z�b�g
	format = (WAVEFORMATEX *)&Header[20]; 
	format->wFormatTag      = WAVE_FORMAT_PCM ;
	format->nChannels       = 2 ;
	format->nSamplesPerSec  = 44100 ;
	format->wBitsPerSample  = 16 ;
	format->nBlockAlign     = ( WORD )( format->wBitsPerSample / 8 * format->nChannels ) ;
	format->nAvgBytesPerSec = format->nSamplesPerSec * format->nBlockAlign ;
	format->cbSize          = 0 ;
	
	// �w�b�_�������o���ăt�@�C�������
	p = Header;
	_MEMCPY( (char *)p, "RIFF", 4 ) ;												p += 4 ;
	*((DWORD *)p) = ( DWORD )( SoundSysData.SoundCaptureSample * format->nBlockAlign + NORMALWAVE_HEADERSIZE - 8 ) ;	p += 4 ;
	_MEMCPY( (char *)p, "WAVE", 4 ) ;												p += 4 ;

	_MEMCPY( (char *)p, "fmt ", 4 ) ;												p += 4 ;
	*((DWORD *)p) = NORMALWAVE_FORMATSIZE ;											p += 4 + NORMALWAVE_FORMATSIZE ;

	_MEMCPY( (char *)p, "data", 4 ) ;												p += 4 ;
	*((DWORD *)p) = ( DWORD )( SoundSysData.SoundCaptureSample * format->nBlockAlign ) ;	p += 4 ;
	
	WriteOnlyFileAccessSeek( SoundSysData.SoundCaptureFileHandle, 0, SEEK_SET ) ;
	WriteOnlyFileAccessWrite( SoundSysData.SoundCaptureFileHandle, Header, NORMALWAVE_HEADERSIZE ) ;
	WriteOnlyFileAccessClose( SoundSysData.SoundCaptureFileHandle ) ;
	SoundSysData.SoundCaptureFileHandle = 0 ;
	
	// �L���v�`���I��
	SoundSysData.SoundCaptureFlag = FALSE ;
	
	// �I��
	return 0 ;

#endif // DX_NON_SAVEFUNCTION
}



















// �\�t�g�E�G�A����T�E���h�n�֐�

// �\�t�g�T�E���h�n���h�����Z�b�g�A�b�v����
extern int SetupSoftSoundHandle(
	int SoftSoundHandle,
	int IsPlayer,
	int Channels,
	int BitsPerSample,
	int SamplesPerSec,
	int IsFloatType,
	int SampleNum
)
{
	SOFTSOUND * SSound ;

	if( SoundSysData.InitializeFlag == FALSE ) return -1 ;

	// �G���[����
	if( SSND_MASKHCHK_ASYNC( SoftSoundHandle, SSound ) )
		return -1 ;

	// �p�����[�^�̏�����
	SSound->IsPlayer = IsPlayer ;
	if( IsFloatType )
	{
		SSound->BufferFormat.wFormatTag  = WAVE_FORMAT_IEEE_FLOAT ;
		SSound->BufferFormat.wBitsPerSample  = ( WORD )32 ;
	}
	else
	{
		SSound->BufferFormat.wFormatTag  = WAVE_FORMAT_PCM ;
		SSound->BufferFormat.wBitsPerSample  = ( WORD )BitsPerSample ;
	}
	SSound->BufferFormat.nChannels       = ( WORD )Channels ;
	SSound->BufferFormat.nSamplesPerSec  = ( DWORD )SamplesPerSec ;
	SSound->BufferFormat.nBlockAlign     = ( WORD )( SSound->BufferFormat.wBitsPerSample / 8 * SSound->BufferFormat.nChannels ) ;
	SSound->BufferFormat.nAvgBytesPerSec = SSound->BufferFormat.nSamplesPerSec * SSound->BufferFormat.nBlockAlign ;
	SSound->BufferFormat.cbSize = 0 ;

	// �v���C���[���ǂ����ŏ����𕪊�
	if( IsPlayer )
	{
		DWORD BufferSize ;

		// �v���C���[�̏ꍇ

		// �����O�o�b�t�@�̏�����
		RingBufInitialize( &SSound->Player.StockSample ) ;
		SSound->Player.StockSampleNum = 0 ;

		// �Đ��p�T�E���h�o�b�t�@�̍쐬
		BufferSize = SOUNDSIZE( SSND_PLAYER_STRM_BUFSEC * SSound->BufferFormat.nAvgBytesPerSec / SSND_PLAYER_SEC_DIVNUM, SSound->BufferFormat.nBlockAlign ) ;
		if( SoundBuffer_Initialize( &SSound->Player.SoundBuffer, BufferSize, &SSound->BufferFormat, NULL, TRUE, TRUE, FALSE ) != 0 )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xbd\x30\xd5\x30\xc8\x30\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xd7\x30\xec\x30\xa4\x30\xe4\x30\xfc\x30\x28\x75\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�\�t�g�T�E���h�v���C���[�p�T�E���h�o�b�t�@�̍쐬�Ɏ��s���܂���" @*/ ) ;
			return -1 ;
		}
		SSound->Player.SoundBufferSize   = ( int )BufferSize ;
		SSound->Player.DataSetCompOffset = 0 ;

		SSound->Player.NoneDataSetCompOffset           = -1 ;
		SSound->Player.NoneDataPlayCheckBackPlayOffset = 0 ;
		SSound->Player.NoneDataPlayStartFlag           = FALSE ;

		SSound->Player.IsPlayFlag = FALSE ;

		// ��s�o�b�t�@�T�C�Y���Z�b�g
		SSound->Player.MaxDataSetSize = ( int )SOUNDSIZE( SSND_PLAYER_STRM_SAKICOPYSEC    * SSound->BufferFormat.nAvgBytesPerSec / SSND_PLAYER_SEC_DIVNUM, SSound->BufferFormat.nBlockAlign ) ;
		SSound->Player.MinDataSetSize = ( int )SOUNDSIZE( SSND_PLAYER_STRM_MINSAKICOPYSEC * SSound->BufferFormat.nAvgBytesPerSec / SSND_PLAYER_SEC_DIVNUM, SSound->BufferFormat.nBlockAlign ) ;

		// �N���e�B�J���Z�N�V�����̎擾
		CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

		// �n���h�����X�g�ɒǉ�
		AddHandleList( &SoundSysData.SoftSoundPlayerListFirst, &SSound->Player.SoftSoundPlayerList, -1, SSound ) ;

		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;
	}
	else
	{
		// �T�E���h�f�[�^�̏ꍇ

		// �f�[�^���i�[���郁�����̈���m��
		SSound->Wave.BufferSampleNum = SampleNum ;
		SSound->Wave.Buffer          = DXALLOC( ( size_t )( SampleNum * SSound->BufferFormat.nBlockAlign ) ) ;
		if( SSound->Wave.Buffer == NULL )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xbd\x30\xd5\x30\xc8\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\xe2\x6c\x62\x5f\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x41\x00\x64\x00\x64\x00\x53\x00\x6f\x00\x66\x00\x74\x00\x53\x00\x6f\x00\x75\x00\x6e\x00\x64\x00\x44\x00\x61\x00\x74\x00\x61\x00\x00"/*@ L"�\�t�g�n���h���̔g�`���i�[���郁�����̈�̊m�ۂɎ��s���܂��� in AddSoftSoundData" @*/ ) ;
			return -1 ;
		}
	}

	// ����I��
	return 0 ;
}

// �\�t�g�T�E���h�n���h���̏�����
extern int InitializeSoftSoundHandle( HANDLEINFO * )
{
	// ���ɉ������Ȃ�
	return 0 ;
}

// �\�t�g�T�E���h�n���h���̌�n��
extern int TerminateSoftSoundHandle( HANDLEINFO *HandleInfo )
{
	SOFTSOUND *SSound = ( SOFTSOUND * )HandleInfo ;

	// �v���C���[���ǂ����ŏ����𕪊�
	if( SSound->IsPlayer == TRUE )
	{
		// �����O�o�b�t�@�̉��
		RingBufTerminate( &SSound->Player.StockSample ) ;

		// �T�E���h�o�b�t�@�[�̉��
		SoundBuffer_Terminate( &SSound->Player.SoundBuffer ) ;

		// �\�t�g�T�E���h�v���C���[���X�g����O��
		SubHandleList( &SSound->Player.SoftSoundPlayerList ) ;
	}
	else
	{
		// �T�E���h�f�[�^���i�[���Ă����������̈���J��
		if( SSound->Wave.Buffer )
		{
			DXFREE( SSound->Wave.Buffer ) ;
			SSound->Wave.Buffer = NULL ;
		}
	}

	// �I��
	return 0 ;
}

// �\�t�g�T�E���h�n���h�����폜���邩�ǂ������`�F�b�N����֐�
extern int DeleteCancelCheckSoftSoundFunction( HANDLEINFO *HandleInfo )
{
	SOFTSOUND *SSound = ( SOFTSOUND * )HandleInfo ;

	return SSound->IsPlayer != FALSE ;
}

// �\�t�g�T�E���h�v���[���[�n���h�����폜���邩�ǂ������`�F�b�N����֐�
extern int DeleteCancelCheckSoftSoundPlayerFunction( HANDLEINFO *HandleInfo )
{
	SOFTSOUND *SSound = ( SOFTSOUND * )HandleInfo ;

	return SSound->IsPlayer == FALSE ;
}


// �\�t�g�E�G�A�ň����g�`�f�[�^�����ׂĉ������
extern int NS_InitSoftSound( void )
{
	if( SoundSysData.InitializeFlag == FALSE )
		return -1 ;

	return AllHandleSub( DX_HANDLETYPE_SOFTSOUND, DeleteCancelCheckSoftSoundFunction );
}

// LoadSoftSoundBase �̎������֐�
static int LoadSoftSoundBase_Static(
	LOADSOUND_GPARAM *GParam,
	int SoftSoundHandle,
	const wchar_t *FileName,
	const void *FileImage,
	int FileImageSize,
	int /*ASyncThread*/
)
{
	SOFTSOUND * SSound ;
	STREAMDATA Stream ;
	SOUNDCONV ConvData ;
	WAVEFORMATEX Format ;
	int SampleNum ;
	void *SrcBuffer = NULL ;
	int SoundSize ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
		return -1 ;

	if( SSND_MASKHCHK_ASYNC( SoftSoundHandle, SSound ) )
		return -1 ;

	// �t�@�C������ NULL �ł͂Ȃ��ꍇ�̓t�@�C������ǂݍ���
	if( FileName != NULL )
	{
		// �t�@�C�����J��
		Stream.DataPoint = (void *)DX_FOPEN( FileName ) ;
		if( Stream.DataPoint == NULL ) return -1 ;
		Stream.ReadShred = *GetFileStreamDataShredStruct() ;
	}
	else
	// ����ȊO�̏ꍇ�̓���������ǂݍ���
	{
		Stream.DataPoint = MemStreamOpen( ( void *)FileImage, ( size_t )FileImageSize ) ;
		Stream.ReadShred = *GetMemStreamDataShredStruct() ;
	}

	// �o�b�l�`���ɕϊ�
	{
		_MEMSET( &ConvData, 0, sizeof( ConvData ) ) ;
		if( SetupSoundConvert( &ConvData, &Stream, GParam->DisableReadSoundFunctionMask
#ifndef DX_NON_OGGVORBIS
								,GParam->OggVorbisBitDepth, GParam->OggVorbisFromTheoraFile
#endif
								) < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xbd\x30\xd5\x30\xc8\x30\xb5\x30\xa6\x30\xf3\x30\xc9\x30\x28\x75\x6e\x30\xf3\x97\xf0\x58\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\x30\xff\x23\xff\x2d\xff\x78\x30\x6e\x30\x09\x59\xdb\x63\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�\�t�g�T�E���h�p�̉����t�@�C���̂o�b�l�ւ̕ϊ��Ɏ��s���܂���" @*/ )) ;
			goto ERR ;
		}
		if( SoundConvertFast( &ConvData, &Format, &SrcBuffer, &SoundSize ) < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xbd\x30\xd5\x30\xc8\x30\xb5\x30\xa6\x30\xf3\x30\xc9\x30\x28\x75\x6e\x30\xf3\x97\xf0\x58\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�\�t�g�T�E���h�p�̉����t�@�C�����i�[���郁�����̈�̊m�ۂɎ��s���܂���" @*/ )) ;
			goto ERR ;
		}

		TerminateSoundConvert( &ConvData ) ;
	}

	// �t�@�C�������
	if( FileName != NULL )
	{
		DX_FCLOSE( ( DWORD_PTR )Stream.DataPoint ) ;
	}
	else
	{
		MemStreamClose( Stream.DataPoint ) ;
	}
	Stream.DataPoint = NULL ;

	// �T���v���̐����Z�o����
	SampleNum = SoundSize / Format.nBlockAlign ;

	// �n���h���̃Z�b�g�A�b�v
	if( SetupSoftSoundHandle(
			SoftSoundHandle,
			FALSE,
			( int )Format.nChannels,
			( int )Format.wBitsPerSample,
			( int )Format.nSamplesPerSec,
			Format.wFormatTag == WAVE_FORMAT_IEEE_FLOAT ? TRUE : FALSE,
			SampleNum ) < 0 )
		goto ERR ;

	// �T�E���h�f�[�^�̃R�s�[
	_MEMCPY( SSound->Wave.Buffer, SrcBuffer, ( size_t )SoundSize ) ;

	// �T�E���h�f�[�^�̉��
	if( SrcBuffer )
	{
		DXFREE( SrcBuffer ) ;
	}

	// ����I��
	return 0 ;

ERR :
	if( Stream.DataPoint )
	{
		if( FileName != NULL )
		{
			DX_FCLOSE( ( DWORD_PTR )Stream.DataPoint ) ;
		}
		else
		{
			MemStreamClose( Stream.DataPoint ) ;
		}
	}
	TerminateSoundConvert( &ConvData ) ;
	if( SrcBuffer )
	{
		DXFREE( SrcBuffer ) ;
	}

	// �G���[�I��
	return -1 ;
}

#ifndef DX_NON_ASYNCLOAD
// LoadSoftSoundBase �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void LoadSoftSoundBase_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	LOADSOUND_GPARAM *GParam ;
	int SoftSoundHandle ;
	const wchar_t *FileName ;
	const void *FileImage ;
	int FileImageSize ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	GParam = ( LOADSOUND_GPARAM * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	SoftSoundHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	FileName = GetASyncLoadParamString( AParam->Data, &Addr ) ;
	FileImage = GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	FileImageSize = GetASyncLoadParamInt( AParam->Data, &Addr ) ;

	Result = LoadSoftSoundBase_Static( GParam, SoftSoundHandle, FileName, FileImage, FileImageSize, TRUE ) ;

	DecASyncLoadCount( SoftSoundHandle ) ;
	if( Result < 0 )
	{
		SubHandle( SoftSoundHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// �\�t�g�E�G�A�ň����g�`�f�[�^���t�@�C���܂��̓�������ɓW�J���ꂽ�t�@�C���C���[�W����쐬����
extern int LoadSoftSoundBase_UseGParam(
	LOADSOUND_GPARAM *GParam,
	const wchar_t *FileName,
	const void *FileImage,
	int FileImageSize,
	int ASyncLoadFlag
)
{
	int SoftSoundHandle ;

	if( SoundSysData.InitializeFlag == FALSE )
		return -1 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �n���h���̍쐬
	SoftSoundHandle = AddHandle( DX_HANDLETYPE_SOFTSOUND, FALSE, -1 ) ;
	if( SoftSoundHandle == -1 ) return -1 ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;
		wchar_t FullPath[ 1024 ] ;

		ConvertFullPathW_( FileName, FullPath, sizeof( FullPath ) ) ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamStruct( NULL, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( NULL, &Addr, SoftSoundHandle ) ;
		AddASyncLoadParamString( NULL, &Addr, FullPath ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, ( void * )FileImage ) ;
		AddASyncLoadParamInt( NULL, &Addr, FileImageSize ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = LoadSoftSoundBase_ASync ;
		Addr = 0 ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, GParam, sizeof( *GParam ) ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SoftSoundHandle ) ;
		AddASyncLoadParamString( AParam->Data, &Addr, FullPath ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, ( void * )FileImage ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, FileImageSize ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( SoftSoundHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( LoadSoftSoundBase_Static( GParam, SoftSoundHandle, FileName, FileImage, FileImageSize, FALSE ) < 0 )
			goto ERR ;
	}

	// �I��
	return SoftSoundHandle ;

ERR :
	SubHandle( SoftSoundHandle ) ;

	return -1 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^���t�@�C������쐬����
extern	int NS_LoadSoftSound( const TCHAR *FileName )
{
#ifdef UNICODE
	return LoadSoftSound_WCHAR_T(
		FileName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = LoadSoftSound_WCHAR_T(
		UseFileNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �\�t�g�E�G�A�ň����g�`�f�[�^���t�@�C������쐬����
extern	int LoadSoftSound_WCHAR_T( const wchar_t *FileName )
{
	LOADSOUND_GPARAM GParam ;

	InitLoadSoundGParam( &GParam ) ;
	return LoadSoftSoundBase_UseGParam( &GParam, FileName, NULL, 0, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^����������ɓW�J���ꂽ�t�@�C���C���[�W����쐬����
extern	int	NS_LoadSoftSoundFromMemImage( const void *FileImageBuffer, int FileImageSize )
{
	LOADSOUND_GPARAM GParam ;

	InitLoadSoundGParam( &GParam ) ;
	return LoadSoftSoundBase_UseGParam( &GParam, NULL, FileImageBuffer, FileImageSize, GetASyncLoadFlag() ) ;
}

// MakeSoftSoundBase �̎������֐�
static int MakeSoftSoundBase_Static(
	int SoftSoundHandle,
	int IsPlayer,
	int Channels,
	int BitsPerSample,
	int SamplesPerSec,
	int SampleNum,
	int IsFloatType,
	int UseFormat_SoftSoundHandle,
	int /*ASyncThread*/
)
{
	SOFTSOUND * SSound ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
		return -1 ;

	if( SSND_MASKHCHK_ASYNC( SoftSoundHandle, SSound ) )
		return -1 ;

	if( UseFormat_SoftSoundHandle >= 0 )
	{
		if( NS_GetSoftSoundFormat( UseFormat_SoftSoundHandle, &Channels, &BitsPerSample, &SamplesPerSec, &IsFloatType ) < 0 )
			return -1 ;
	}

	if( SetupSoftSoundHandle( SoftSoundHandle, IsPlayer, Channels, BitsPerSample, SamplesPerSec, IsFloatType, SampleNum ) < 0 )
		return -1 ;

	// ����I��
	return 0 ;
}

#ifndef DX_NON_ASYNCLOAD
// MakeSoftSoundBase �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void MakeSoftSoundBase_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	int SoftSoundHandle ;
	int IsPlayer ;
	int Channels ;
	int BitsPerSample ;
	int SamplesPerSec ;
	int SampleNum ;
	int IsFloatType ;
	int UseFormat_SoftSoundHandle ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	SoftSoundHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	IsPlayer = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	Channels = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	BitsPerSample = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	SamplesPerSec = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	SampleNum = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	IsFloatType = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	UseFormat_SoftSoundHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;

	Result = MakeSoftSoundBase_Static( SoftSoundHandle, IsPlayer, Channels, BitsPerSample, SamplesPerSec, SampleNum, IsFloatType, UseFormat_SoftSoundHandle, TRUE ) ;

	DecASyncLoadCount( SoftSoundHandle ) ;
	if( Result < 0 )
	{
		SubHandle( SoftSoundHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// �\�t�g�E�G�A�Ő��䂷��T�E���h�f�[�^�n���h���̍쐬
extern int MakeSoftSoundBase_UseGParam(
	int IsPlayer,
	int Channels,
	int BitsPerSample,
	int SamplesPerSec,
	int SampleNum,
	int IsFloatType,
	int UseFormat_SoftSoundHandle,
	int ASyncLoadFlag
)
{
	int SoftSoundHandle ;

	if( SoundSysData.InitializeFlag == FALSE )
		return -1 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �n���h���̍쐬
	SoftSoundHandle = AddHandle( DX_HANDLETYPE_SOFTSOUND, FALSE, -1 ) ;
	if( SoftSoundHandle == -1 ) return -1 ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt( NULL, &Addr, SoftSoundHandle ) ;
		AddASyncLoadParamInt( NULL, &Addr, IsPlayer ) ;
		AddASyncLoadParamInt( NULL, &Addr, Channels ) ;
		AddASyncLoadParamInt( NULL, &Addr, BitsPerSample ) ;
		AddASyncLoadParamInt( NULL, &Addr, SamplesPerSec ) ;
		AddASyncLoadParamInt( NULL, &Addr, SampleNum ) ;
		AddASyncLoadParamInt( NULL, &Addr, IsFloatType ) ;
		AddASyncLoadParamInt( NULL, &Addr, UseFormat_SoftSoundHandle ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = MakeSoftSoundBase_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SoftSoundHandle ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, IsPlayer ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, Channels ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, BitsPerSample ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SamplesPerSec ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SampleNum ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, IsFloatType ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, UseFormat_SoftSoundHandle ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( SoftSoundHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( MakeSoftSoundBase_Static( SoftSoundHandle, IsPlayer, Channels, BitsPerSample, SamplesPerSec, SampleNum, IsFloatType, UseFormat_SoftSoundHandle, FALSE ) < 0 )
			goto ERR ;
	}

	// �I��
	return SoftSoundHandle ;

ERR :
	SubHandle( SoftSoundHandle ) ;

	return -1 ;
}

// �\�t�g�E�G�A�ň�����̔g�`�f�[�^���쐬����( �t�H�[�}�b�g�͈����̃\�t�g�E�G�A�T�E���h�n���h���Ɠ������̂ɂ��� )
extern	int NS_MakeSoftSound( int UseFormat_SoftSoundHandle, int SampleNum )
{
	if( UseFormat_SoftSoundHandle < 0 )
		return -1 ;

	return MakeSoftSoundBase_UseGParam( FALSE, 0, 0, 0, SampleNum, FALSE, UseFormat_SoftSoundHandle, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň�����̔g�`�f�[�^���쐬����( �`�����l����:2 �ʎq���r�b�g��:16bit �T���v�����O���g��:44KHz )
extern	int NS_MakeSoftSound2Ch16Bit44KHz( int SampleNum )
{
	return MakeSoftSoundBase_UseGParam( FALSE, 2, 16, 44100, SampleNum, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň�����̔g�`�f�[�^���쐬����( �`�����l����:2 �ʎq���r�b�g��:16bit �T���v�����O���g��:22KHz )
extern	int NS_MakeSoftSound2Ch16Bit22KHz( int SampleNum )
{
	return MakeSoftSoundBase_UseGParam( FALSE, 2, 16, 22050, SampleNum, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň�����̔g�`�f�[�^���쐬����( �`�����l����:2 �ʎq���r�b�g��:8bit �T���v�����O���g��:44KHz )
extern	int NS_MakeSoftSound2Ch8Bit44KHz( int SampleNum )
{
	return MakeSoftSoundBase_UseGParam( FALSE, 2, 8, 44100, SampleNum, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň�����̔g�`�f�[�^���쐬����( �`�����l����:2 �ʎq���r�b�g��:8bit �T���v�����O���g��:22KHz )
extern	int NS_MakeSoftSound2Ch8Bit22KHz( int SampleNum )
{
	return MakeSoftSoundBase_UseGParam( FALSE, 2, 8, 22050, SampleNum, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň�����̔g�`�f�[�^���쐬����( �`�����l����:1 �ʎq���r�b�g��:16bit �T���v�����O���g��:44KHz )
extern	int NS_MakeSoftSound1Ch16Bit44KHz( int SampleNum )
{
	return MakeSoftSoundBase_UseGParam( FALSE, 1, 16, 44100, SampleNum, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň�����̔g�`�f�[�^���쐬����( �`�����l����:1 �ʎq���r�b�g��:16bit �T���v�����O���g��:22KHz )
extern	int NS_MakeSoftSound1Ch16Bit22KHz( int SampleNum )
{
	return MakeSoftSoundBase_UseGParam( FALSE, 1, 16, 22050, SampleNum, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň�����̔g�`�f�[�^���쐬����( �`�����l����:1 �ʎq���r�b�g��:8bit �T���v�����O���g��:44KHz )
extern	int NS_MakeSoftSound1Ch8Bit44KHz( int SampleNum )
{
	return MakeSoftSoundBase_UseGParam( FALSE, 1, 8, 44100, SampleNum, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň�����̔g�`�f�[�^���쐬����( �`�����l����:1 �ʎq���r�b�g��:8bit �T���v�����O���g��:22KHz )
extern	int NS_MakeSoftSound1Ch8Bit22KHz( int SampleNum )
{
	return MakeSoftSoundBase_UseGParam( FALSE, 1, 8, 22050, SampleNum, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň�����̔g�`�f�[�^���쐬����
extern	int NS_MakeSoftSoundCustom( int ChannelNum, int BitsPerSample, int SamplesPerSec, int SampleNum, int IsFloatType )
{
	return MakeSoftSoundBase_UseGParam( FALSE, ChannelNum, BitsPerSample, SamplesPerSec, SampleNum, IsFloatType, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^���������
extern	int NS_DeleteSoftSound( int SoftSoundHandle )
{
	SOFTSOUND *SSound ;

	if( SSND_MASKHCHK_ASYNC( SoftSoundHandle, SSound ) ) return -1 ;

	// �v���C���[���ǂ����̃`�F�b�N
	if( SSound->IsPlayer != FALSE )
	{
		// �������G���[
		return -1 ;
	}

	return SubHandle( SoftSoundHandle ) ;
}

#ifndef DX_NON_SAVEFUNCTION

// �\�t�g�E�G�A�ň����g�`�f�[�^�𖳈��kWav�`���ŕۑ�����
extern int NS_SaveSoftSound( int SoftSoundHandle, const TCHAR *FileName )
{
#ifdef UNICODE
	return SaveSoftSound_WCHAR_T(
		SoftSoundHandle, FileName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = SaveSoftSound_WCHAR_T(
		SoftSoundHandle, UseFileNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�𖳈��kWav�`���ŕۑ�����
extern int SaveSoftSound_WCHAR_T( int SoftSoundHandle, const wchar_t *FileName )
{
	DWORD_PTR fp ;
	BYTE Header[NORMALWAVE_HEADERSIZE], *p ;
	WAVEFORMATEX *format;
	SOFTSOUND * SSound ;

	// �G���[����
	if( SSND_MASKHCHK( SoftSoundHandle, SSound ) ) return -1 ;
	if( SSound->IsPlayer == 1 ) return -1 ;

	fp = WriteOnlyFileAccessOpen( FileName ) ;

	// �t�H�[�}�b�g���Z�b�g
	format = (WAVEFORMATEX *)&Header[20]; 
	format->wFormatTag      = SSound->BufferFormat.wFormatTag ;
	format->nChannels       = SSound->BufferFormat.nChannels ;
	format->nSamplesPerSec  = SSound->BufferFormat.nSamplesPerSec ;
	format->wBitsPerSample  = SSound->BufferFormat.wBitsPerSample ;
	format->nBlockAlign     = ( WORD )( format->wBitsPerSample / 8 * format->nChannels ) ;
	format->nAvgBytesPerSec = format->nSamplesPerSec * format->nBlockAlign ;
	format->cbSize          = 0 ;
	
	// �w�b�_�������o���ăt�@�C�������
	p = Header;
	_MEMCPY( (char *)p, "RIFF", 4 ) ;																	p += 4 ;
	*((DWORD *)p) = ( DWORD )( SSound->Wave.BufferSampleNum * format->nBlockAlign + NORMALWAVE_HEADERSIZE - 8 ) ;	p += 4 ;
	_MEMCPY( (char *)p, "WAVE", 4 ) ;																	p += 4 ;

	_MEMCPY( (char *)p, "fmt ", 4 ) ;																	p += 4 ;
	*((DWORD *)p) = NORMALWAVE_FORMATSIZE ;																p += 4 + NORMALWAVE_FORMATSIZE ;

	_MEMCPY( (char *)p, "data", 4 ) ;																	p += 4 ;
	*((DWORD *)p) = ( DWORD )( SSound->Wave.BufferSampleNum * format->nBlockAlign ) ;					p += 4 ;
	
	WriteOnlyFileAccessWrite( fp, Header, NORMALWAVE_HEADERSIZE ) ;
	WriteOnlyFileAccessWrite( fp, SSound->Wave.Buffer, ( size_t )( SSound->Wave.BufferSampleNum * format->nBlockAlign ) ) ;
	WriteOnlyFileAccessClose( fp ) ;

	// �I��
	return 0 ;
}

#endif // DX_NON_SAVEFUNCTION

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃T���v�������擾����
extern	int NS_GetSoftSoundSampleNum( int SoftSoundHandle )
{
	SOFTSOUND * SSound ;

	// �G���[����
	if( SSND_MASKHCHK( SoftSoundHandle, SSound ) ) return -1 ;
	if( SSound->IsPlayer == 1 ) return -1 ;

	// �T���v������Ԃ�
	return SSound->Wave.BufferSampleNum ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃t�H�[�}�b�g���擾����
extern	int NS_GetSoftSoundFormat( int SoftSoundHandle, int *Channels, int *BitsPerSample, int *SamplesPerSec, int *IsFloatType )
{
	SOFTSOUND * SSound ;

	// �G���[����
	if( SSND_MASKHCHK( SoftSoundHandle, SSound ) ) return -1 ;
	if( SSound->IsPlayer == 1 ) return -1 ;

	// �f�[�^���Z�b�g
	if( Channels      ) *Channels      = SSound->BufferFormat.nChannels ;
	if( BitsPerSample ) *BitsPerSample = SSound->BufferFormat.wBitsPerSample ;
	if( SamplesPerSec ) *SamplesPerSec = ( int )SSound->BufferFormat.nSamplesPerSec ;
	if( IsFloatType   ) *IsFloatType   = SSound->BufferFormat.wFormatTag == WAVE_FORMAT_IEEE_FLOAT ? TRUE : FALSE ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃T���v����ǂݎ��
extern	int NS_ReadSoftSoundData( int SoftSoundHandle, int SamplePosition, int *Channel1, int *Channel2 )
{
	SOFTSOUND * SSound ;
	BYTE *Src ;

	// �G���[����
	if( SSND_MASKHCHK( SoftSoundHandle, SSound ) ) return -1 ;
	if( SSound->IsPlayer == 1 ) return -1 ;

	// �T���v���͈̔͊O�������ꍇ���G���[
	if( SSound->Wave.BufferSampleNum <= SamplePosition || SamplePosition < 0 )
		return -1 ;

	// �T���v���̈ʒu���Z�o
	Src = ( BYTE * )SSound->Wave.Buffer + SSound->BufferFormat.nBlockAlign * SamplePosition ;

	// �T���v����Ԃ�
	if( SSound->BufferFormat.wFormatTag == WAVE_FORMAT_IEEE_FLOAT )
	{
		switch( SSound->BufferFormat.wBitsPerSample )
		{
		case 32 :
			if( Channel1 )
			{
				*Channel1 = _FTOL( ( ( float * )Src )[ 0 ] * 32768.0f ) ;
			}

			if( Channel2 )
			{
				if( SSound->BufferFormat.nChannels == 1 )
				{
					*Channel2 = 0 ;
				}
				else
				{
					*Channel2 = _FTOL( ( ( float * )Src )[ 1 ] * 32768.0f ) ;
				}
			}
			break ;
		}
	}
	else
	{
		switch( SSound->BufferFormat.wBitsPerSample )
		{
		case 8 :
			if( Channel1 )
			{
				*Channel1 = ( int )Src[ 0 ] ;
			}

			if( Channel2 )
			{
				if( SSound->BufferFormat.nChannels == 1 )
				{
					*Channel2 = 127 ;
				}
				else
				{
					*Channel2 = ( int )Src[ 1 ] ;
				}
			}
			break ;

		case 16 :
			if( Channel1 )
			{
				*Channel1 = ( ( short * )Src )[ 0 ] ;
			}

			if( Channel2 )
			{
				if( SSound->BufferFormat.nChannels == 1 )
				{
					*Channel2 = 0 ;
				}
				else
				{
					*Channel2 = ( ( short * )Src )[ 1 ] ;
				}
			}
			break ;
		}
	}

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�n���h���̃T���v����ǂݎ��( float�^�� )
extern	int NS_ReadSoftSoundDataF( int SoftSoundHandle, int SamplePosition, float *Channel1, float *Channel2 )
{
	SOFTSOUND * SSound ;
	BYTE *Src ;

	// �G���[����
	if( SSND_MASKHCHK( SoftSoundHandle, SSound ) ) return -1 ;
	if( SSound->IsPlayer == 1 ) return -1 ;

	// �T���v���͈̔͊O�������ꍇ���G���[
	if( SSound->Wave.BufferSampleNum <= SamplePosition || SamplePosition < 0 )
		return -1 ;

	// �T���v���̈ʒu���Z�o
	Src = ( BYTE * )SSound->Wave.Buffer + SSound->BufferFormat.nBlockAlign * SamplePosition ;

	// �T���v����Ԃ�
	if( SSound->BufferFormat.wFormatTag == WAVE_FORMAT_IEEE_FLOAT )
	{
		switch( SSound->BufferFormat.wBitsPerSample )
		{
		case 32 :
			if( Channel1 )
			{
				*Channel1 = ( ( float * )Src )[ 0 ] ;
			}

			if( Channel2 )
			{
				if( SSound->BufferFormat.nChannels == 1 )
				{
					*Channel2 = 0.0f ;
				}
				else
				{
					*Channel2 = ( ( float * )Src )[ 1 ] ;
				}
			}
			break ;
		}
	}
	else
	{
		switch( SSound->BufferFormat.wBitsPerSample )
		{
		case 8 :
			if( Channel1 )
			{
				*Channel1 = ( float )( ( int )Src[ 0 ] - 127 ) / 128.0f ;
			}

			if( Channel2 )
			{
				if( SSound->BufferFormat.nChannels == 1 )
				{
					*Channel2 = 0.0f ;
				}
				else
				{
					*Channel2 = ( float )( ( int )Src[ 1 ] - 127 ) / 128.0f ;
				}
			}
			break ;

		case 16 :
			if( Channel1 )
			{
				*Channel1 = ( float )( ( short * )Src )[ 0 ] / 32768.0f ;
			}

			if( Channel2 )
			{
				if( SSound->BufferFormat.nChannels == 1 )
				{
					*Channel2 = 0.0f ;
				}
				else
				{
					*Channel2 = ( float )( ( short * )Src )[ 1 ] / 32768.0f ;
				}
			}
			break ;
		}
	}

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃T���v������������
extern	int NS_WriteSoftSoundData( int SoftSoundHandle, int SamplePosition, int Channel1, int Channel2 )
{
	SOFTSOUND * SSound ;
	BYTE *Dest ;

	// �G���[����
	if( SSND_MASKHCHK( SoftSoundHandle, SSound ) ) return -1 ;
	if( SSound->IsPlayer == 1 ) return -1 ;

	// �T���v���͈̔͊O�������ꍇ���G���[
	if( SSound->Wave.BufferSampleNum <= SamplePosition || SamplePosition < 0 )
		return -1 ;

	// �T���v���̈ʒu���Z�o
	Dest = ( BYTE * )SSound->Wave.Buffer + SSound->BufferFormat.nBlockAlign * SamplePosition ;

	// �T���v������������
	if( SSound->BufferFormat.wFormatTag == WAVE_FORMAT_IEEE_FLOAT )
	{
		switch( SSound->BufferFormat.wBitsPerSample )
		{
		case 32 :
			( ( float * )Dest )[ 0 ] = ( float )Channel1 / 32768.0f ;

			if( SSound->BufferFormat.nChannels != 1 )
			{
				( ( float * )Dest )[ 1 ] = ( float )Channel2 / 32768.0f ;
			}
			break ;
		}
	}
	else
	{
		switch( SSound->BufferFormat.wBitsPerSample )
		{
		case 8 :
				 if( Channel1 > 255 ) Channel1 = 255 ;
			else if( Channel1 <   0 ) Channel1 =   0 ;
			Dest[ 0 ] = ( BYTE )Channel1 ;

			if( SSound->BufferFormat.nChannels != 1 )
			{
					 if( Channel2 > 255 ) Channel2 = 255 ;
				else if( Channel2 <   0 ) Channel2 =   0 ;
				Dest[ 1 ] = ( BYTE )Channel2 ;
			}
			break ;

		case 16 :
				 if( Channel1 >  32767 ) Channel1 =  32767 ;
			else if( Channel1 < -32768 ) Channel1 = -32768 ;
			( ( short * )Dest )[ 0 ] = ( short )Channel1 ;

			if( SSound->BufferFormat.nChannels != 1 )
			{
					 if( Channel2 >  32767 ) Channel2 =  32767 ;
				else if( Channel2 < -32768 ) Channel2 = -32768 ;
				( ( short * )Dest )[ 1 ] = ( short )Channel2 ;
			}
			break ;
		}
	}

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�n���h���̃T���v������������( float�^�� )
extern int NS_WriteSoftSoundDataF( int SoftSoundHandle, int SamplePosition, float Channel1, float Channel2 )
{
	SOFTSOUND * SSound ;
	BYTE *Dest ;
	int ChannelI ;

	// �G���[����
	if( SSND_MASKHCHK( SoftSoundHandle, SSound ) ) return -1 ;
	if( SSound->IsPlayer == 1 ) return -1 ;

	// �T���v���͈̔͊O�������ꍇ���G���[
	if( SSound->Wave.BufferSampleNum <= SamplePosition || SamplePosition < 0 )
		return -1 ;

	// �T���v���̈ʒu���Z�o
	Dest = ( BYTE * )SSound->Wave.Buffer + SSound->BufferFormat.nBlockAlign * SamplePosition ;

	// �T���v������������
	if( SSound->BufferFormat.wFormatTag == WAVE_FORMAT_IEEE_FLOAT )
	{
		switch( SSound->BufferFormat.wBitsPerSample )
		{
		case 32 :
			( ( float * )Dest )[ 0 ] = Channel1 ;

			if( SSound->BufferFormat.nChannels != 1 )
			{
				( ( float * )Dest )[ 1 ] = Channel2 ;
			}
			break ;
		}
	}
	else
	{
		switch( SSound->BufferFormat.wBitsPerSample )
		{
		case 8 :
			ChannelI = _FTOL( Channel1 * 128.0f + 127.0f  ) ;
				 if( ChannelI > 255 ) ChannelI = 255 ;
			else if( ChannelI <   0 ) ChannelI =   0 ;
			Dest[ 0 ] = ( BYTE )ChannelI ;

			if( SSound->BufferFormat.nChannels != 1 )
			{
				ChannelI = _FTOL( Channel2 * 128.0f + 127.0f  ) ;
					 if( Channel2 > 255 ) Channel2 = 255 ;
				else if( Channel2 <   0 ) Channel2 =   0 ;
				Dest[ 1 ] = ( BYTE )Channel2 ;
			}
			break ;

		case 16 :
			ChannelI = _FTOL( Channel1 * 32768.0f  ) ;
				 if( ChannelI >  32767 ) ChannelI =  32767 ;
			else if( ChannelI < -32768 ) ChannelI = -32768 ;
			( ( short * )Dest )[ 0 ] = ( short )ChannelI ;

			if( SSound->BufferFormat.nChannels != 1 )
			{
				ChannelI = _FTOL( Channel2 * 32768.0f  ) ;
					 if( ChannelI >  32767 ) ChannelI =  32767 ;
				else if( ChannelI < -32768 ) ChannelI = -32768 ;
				( ( short * )Dest )[ 1 ] = ( short )ChannelI ;
			}
			break ;
		}
	}

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�n���h���̔g�`�f�[�^��������ς����Ƀf�[�^�̒�����ύX����
extern int NS_WriteTimeStretchSoftSoundData( int SrcSoftSoundHandle, int DestSoftSoundHandle )
{
	SOFTSOUND * SSSound ;
	SOFTSOUND * DSSound ;

	// �G���[����
	if( SSND_MASKHCHK_ASYNC( SrcSoftSoundHandle, SSSound ) ) return -1 ;
	if( SSSound->IsPlayer == 1 ) return -1 ;

	if( SSND_MASKHCHK_ASYNC( DestSoftSoundHandle, DSSound ) ) return -1 ;
	if( DSSound->IsPlayer == 1 ) return -1 ;

	// �T���v�������ς��Ȃ��ꍇ�̓G���[
	if( SSSound->Wave.BufferSampleNum == DSSound->Wave.BufferSampleNum )
	{
		return -1 ;
	}

	// �t�H�[�}�b�g���قȂ�ꍇ�̓G���[
	if( SSSound->BufferFormat.wFormatTag     != DSSound->BufferFormat.wFormatTag ||
		SSSound->BufferFormat.nChannels      != DSSound->BufferFormat.nChannels  ||
		SSSound->BufferFormat.nSamplesPerSec != DSSound->BufferFormat.nSamplesPerSec ||
		SSSound->BufferFormat.wBitsPerSample != DSSound->BufferFormat.wBitsPerSample )
	{
		return -1 ;
	}

	// ���������Ȃ�̂��Z���Ȃ�̂��ŏ����𕪊�
	if( SSSound->Wave.BufferSampleNum < DSSound->Wave.BufferSampleNum )
	{
		int n, m, template_size, pmin, pmax, p, q, offset0, offset1 ;
		double rate ;
		int rateI ;
		int SSampleNum ;
		int DSampleNum ;

		rate = ( double )SSSound->Wave.BufferSampleNum / DSSound->Wave.BufferSampleNum ; // rate�͈̔͂�0.5<=rate<1.0
		rateI = _DTOL( rate * 512.0 ) ;
		rate = rateI / 512.0 ;

//		// 8�{�ȏ�ɒ����Ȃ�ꍇ�̓G���[
//		if( rate < 0.125 )
//		{
//			return -1 ;
//		}

		SSampleNum = SSSound->Wave.BufferSampleNum ;
		DSampleNum = DSSound->Wave.BufferSampleNum ;

		template_size = _DTOL( SSSound->BufferFormat.nSamplesPerSec * 0.01  ) ; // 10ms
		pmin          = _DTOL( SSSound->BufferFormat.nSamplesPerSec * 0.005 ) ; // 5ms
		pmax          = _DTOL( SSSound->BufferFormat.nSamplesPerSec * 0.02  ) ; // 20ms

		switch( SSSound->BufferFormat.wFormatTag )
		{
		case WAVE_FORMAT_PCM :

			switch( SSSound->BufferFormat.wBitsPerSample )
			{
			case 16 :
				{
					short *SBuffer ;
					short *DBuffer ;
					int r ;
					int max_of_r;

					SBuffer = ( short * )SSSound->Wave.Buffer ;
					DBuffer = ( short * )DSSound->Wave.Buffer ;

					switch( SSSound->BufferFormat.nChannels )
					{
					case 2 :
						offset0 = 0 ;
						offset1 = 0 ;
						while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								short *sp1 ;
								short *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									r += ( sp1[ 0 ] * sp2[ 0 ] ) / 32768 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								short *sp1, *dp1 ;
								short *sp2, *dp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 ] ;
								dp2 = &DBuffer[ ( offset1 + p ) * 2 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2, dp2 += 2 )
								{
									dp1[ 0 ] =   sp1[ 0 ] ;
									dp2[ 0 ] = ( sp2[ 0 ] * ( p - n ) + sp1[ 0 ] * n ) / p ;
								}

								if( offset1 + p * 2 < DSampleNum &&
									offset0 + p     < SSampleNum )
								{
									q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p     ) * 2 ] ;
									dp1 = &DBuffer[ ( offset1 + p * 2 ) * 2 ] ;
									movnum = q - p ;
									if( offset0 + p + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p ) ;
										q = p + movnum ;
									}
									if( offset1 + p * 2 + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p * 2 ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										dp1[ 0 ] = sp1[ 0 ] ;
									}
								}
							}

							offset0 += q ;		// offset0�̍X�V
							offset1 += p + q ;	// offset1�̍X�V
						}

						while( offset1 < DSampleNum && offset0 < SSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								short *sp1 ;
								short *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									int sp1p ;
									int sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									sp2p = offset0 + m + n >= SSampleNum ? 0 : sp2[ 0 ] ;
									r += ( sp1p * sp2p ) / 32768 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								short *sp1, *dp1 ;
								short *sp2, *dp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 ] ;
								dp2 = &DBuffer[ ( offset1 + p ) * 2 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2, dp2 += 2 )
								{
									short sp1p ;
									short sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									sp2p = offset0 + p + n >= SSampleNum ? 0 : sp2[ 0 ] ;
									if( offset1 + n < DSampleNum )
									{
										dp1[ 0 ] =   sp1p ;
									}
									if( offset1 + p + n < DSampleNum )
									{
										dp2[ 0 ] = ( sp2p * ( p - n ) + sp1p * n ) / p ;
									}
								}

								if( offset1 + p * 2 < DSampleNum &&
									offset0 + p     < SSampleNum )
								{
									q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p     ) * 2 ] ;
									dp1 = &DBuffer[ ( offset1 + p * 2 ) * 2 ] ;
									movnum = q - p ;
									if( offset0 + p + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p ) ;
										q = p + movnum ;
									}
									if( offset1 + p * 2 + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p * 2 ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										short sp1p ;

										sp1p = offset0 + p + n >= SSampleNum ? 0 : sp1[ 0 ] ;
										dp1[ 0 ] = sp1p ;
									}
								}
							}

							offset0 += q ;		// offset0�̍X�V
							offset1 += p + q ;	// offset1�̍X�V
						}
						if( offset1 < DSampleNum )
						{
							short *dp ;
							int loopnum ;

							loopnum = DSampleNum - offset1 ;
							dp = &DBuffer[ offset1 * 2 ] ;
							for( n = 0 ; n < loopnum ; n ++, dp += 2 )
							{
								*dp = 0 ;
							}
						}



						offset0 = 0 ;
						offset1 = 0 ;
						while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								short *sp1 ;
								short *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 + 1 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									r += ( sp1[ 0 ] * sp2[ 0 ] ) / 32768 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								short *sp1, *dp1 ;
								short *sp2, *dp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 + 1 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 + 1 ] ;
								dp2 = &DBuffer[ ( offset1 + p ) * 2 + 1 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2, dp2 += 2 )
								{
									dp1[ 0 ] =   sp1[ 0 ] ;
									dp2[ 0 ] = ( sp2[ 0 ] * ( p - n ) + sp1[ 0 ] * n ) / p ;
								}

								if( offset1 + p * 2 < DSampleNum &&
									offset0 + p     < SSampleNum )
								{
									q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p     ) * 2 + 1 ] ;
									dp1 = &DBuffer[ ( offset1 + p * 2 ) * 2 + 1 ] ;
									movnum = q - p ;
									if( offset0 + p + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p ) ;
										q = p + movnum ;
									}
									if( offset1 + p * 2 + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p * 2 ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										dp1[ 0 ] = sp1[ 0 ] ;
									}
								}
							}

							offset0 += q ;		// offset0�̍X�V
							offset1 += p + q ;	// offset1�̍X�V
						}

						while( offset1 < DSampleNum && offset0 < SSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								short *sp1 ;
								short *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 + 1 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									int sp1p ;
									int sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									sp2p = offset0 + m + n >= SSampleNum ? 0 : sp2[ 0 ] ;
									r += ( sp1p * sp2p ) / 32768 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								short *sp1, *dp1 ;
								short *sp2, *dp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 + 1 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 + 1 ] ;
								dp2 = &DBuffer[ ( offset1 + p ) * 2 + 1 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2, dp2 += 2 )
								{
									short sp1p ;
									short sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									sp2p = offset0 + p + n >= SSampleNum ? 0 : sp2[ 0 ] ;
									if( offset1 + n < DSampleNum )
									{
										dp1[ 0 ] =   sp1p ;
									}
									if( offset1 + p + n < DSampleNum )
									{
										dp2[ 0 ] = ( sp2p * ( p - n ) + sp1p * n ) / p ;
									}
								}

								if( offset1 + p * 2 < DSampleNum &&
									offset0 + p     < SSampleNum )
								{
									q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p     ) * 2 + 1 ] ;
									dp1 = &DBuffer[ ( offset1 + p * 2 ) * 2 + 1 ] ;
									movnum = q - p ;
									if( offset0 + p + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p ) ;
										q = p + movnum ;
									}
									if( offset1 + p * 2 + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p * 2 ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										short sp1p ;

										sp1p = offset0 + p + n >= SSampleNum ? 0 : sp1[ 0 ] ;
										dp1[ 0 ] = sp1p ;
									}
								}
							}

							offset0 += q ;		// offset0�̍X�V
							offset1 += p + q ;	// offset1�̍X�V
						}
						if( offset1 < DSampleNum )
						{
							short *dp ;
							int loopnum ;

							loopnum = DSampleNum - offset1 ;
							dp = &DBuffer[ offset1 * 2 + 1 ] ;
							for( n = 0 ; n < loopnum ; n ++, dp += 2 )
							{
								*dp = 0 ;
							}
						}
						break ;

					case 1 :
						offset0 = 0 ;
						offset1 = 0 ;
						while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								short *sp1 ;
								short *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + m ] ;
								for( n = 0 ; n < template_size ; n++, sp1 ++, sp2 ++ )
								{
									r += ( sp1[ 0 ] * sp2[ 0 ] ) / 32768 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								short *sp1, *dp1 ;
								short *sp2, *dp2 ;
								int movnum ;

								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + p ] ;
								dp1 = &DBuffer[ offset1     ] ;
								dp2 = &DBuffer[ offset1 + p ] ;
								for( n = 0; n < p ; n++, sp1 ++, sp2 ++, dp1 ++, dp2 ++ )
								{
									dp1[ 0 ] =   sp1[ 0 ] ;
									dp2[ 0 ] = ( sp2[ 0 ] * ( p - n ) + sp1[ 0 ] * n ) / p ;
								}

								if( offset1 + p * 2 < DSampleNum &&
									offset0 + p     < SSampleNum )
								{
									q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
									sp1 = &SBuffer[ offset0 + p     ] ;
									dp1 = &DBuffer[ offset1 + p * 2 ] ;
									movnum = q - p ;
									if( offset0 + p + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p ) ;
										q = p + movnum ;
									}
									if( offset1 + p * 2 + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p * 2 ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 ++, dp1 ++ )
									{
										dp1[ 0 ] = sp1[ 0 ] ;
									}
								}
							}

							offset0 += q ;		// offset0�̍X�V
							offset1 += p + q ;	// offset1�̍X�V
						}

						while( offset1 < DSampleNum && offset0 < SSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								short *sp1 ;
								short *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + m ] ;
								for( n = 0 ; n < template_size ; n++, sp1 ++, sp2 ++ )
								{
									int sp1p ;
									int sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									sp2p = offset0 + m + n >= SSampleNum ? 0 : sp2[ 0 ] ;
									r += ( sp1p * sp2p ) / 32768 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								short *sp1, *dp1 ;
								short *sp2, *dp2 ;
								int movnum ;

								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + p ] ;
								dp1 = &DBuffer[ offset1     ] ;
								dp2 = &DBuffer[ offset1 + p ] ;
								for( n = 0; n < p ; n++, sp1 ++, sp2 ++, dp1 ++, dp2 ++ )
								{
									short sp1p ;
									short sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									sp2p = offset0 + p + n >= SSampleNum ? 0 : sp2[ 0 ] ;
									if( offset1 + n < DSampleNum )
									{
										dp1[ 0 ] =   sp1p ;
									}
									if( offset1 + p + n < DSampleNum )
									{
										dp2[ 0 ] = ( sp2p * ( p - n ) + sp1p * n ) / p ;
									}
								}

								if( offset1 + p * 2 < DSampleNum &&
									offset0 + p     < SSampleNum )
								{
									q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
									sp1 = &SBuffer[ offset0 + p     ] ;
									dp1 = &DBuffer[ offset1 + p * 2 ] ;
									movnum = q - p ;
									if( offset0 + p + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p ) ;
										q = p + movnum ;
									}
									if( offset1 + p * 2 + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p * 2 ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 ++, dp1 ++ )
									{
										short sp1p ;

										sp1p = offset0 + p + n >= SSampleNum ? 0 : sp1[ 0 ] ;
										dp1[ 0 ] = sp1p ;
									}
								}
							}

							offset0 += q ;		// offset0�̍X�V
							offset1 += p + q ;	// offset1�̍X�V
						}
						if( offset1 < DSampleNum )
						{
							short *dp ;
							int loopnum ;

							loopnum = DSampleNum - offset1 ;
							dp = &DBuffer[ offset1 ] ;
							for( n = 0 ; n < loopnum ; n ++, dp ++ )
							{
								*dp = 0 ;
							}
						}
						break ;
					}
				}
				break ;

			case 8 :
				{
					BYTE *SBuffer ;
					BYTE *DBuffer ;
					int r ;
					int max_of_r;

					SBuffer = ( BYTE * )SSSound->Wave.Buffer ;
					DBuffer = ( BYTE * )DSSound->Wave.Buffer ;

					switch( SSSound->BufferFormat.nChannels )
					{
					case 2 :
						offset0 = 0 ;
						offset1 = 0 ;
						while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								BYTE *sp1 ;
								BYTE *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									r += ( ( ( int )sp1[ 0 ] - 128 ) * ( int )( sp2[ 0 ] - 128 ) ) / 128 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								BYTE *sp1, *dp1 ;
								BYTE *sp2, *dp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 ] ;
								dp2 = &DBuffer[ ( offset1 + p ) * 2 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2, dp2 += 2 )
								{
									dp1[ 0 ] =   sp1[ 0 ] ;
									dp2[ 0 ] = ( sp2[ 0 ] * ( p - n ) + sp1[ 0 ] * n ) / p ;
								}

								if( offset1 + p * 2 < DSampleNum &&
									offset0 + p     < SSampleNum )
								{
									q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p     ) * 2 ] ;
									dp1 = &DBuffer[ ( offset1 + p * 2 ) * 2 ] ;
									movnum = q - p ;
									if( offset0 + p + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p ) ;
										q = p + movnum ;
									}
									if( offset1 + p * 2 + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p * 2 ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										dp1[ 0 ] = sp1[ 0 ] ;
									}
								}
							}

							offset0 += q ;		// offset0�̍X�V
							offset1 += p + q ;	// offset1�̍X�V
						}

						while( offset1 < DSampleNum && offset0 < SSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								BYTE *sp1 ;
								BYTE *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									int sp1p ;
									int sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : ( int )sp1[ 0 ] - 128 ;
									sp2p = offset0 + m + n >= SSampleNum ? 0 : ( int )sp2[ 0 ] - 128 ;
									r += ( sp1p * sp2p ) / 128 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								BYTE *sp1, *dp1 ;
								BYTE *sp2, *dp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 ] ;
								dp2 = &DBuffer[ ( offset1 + p ) * 2 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2, dp2 += 2 )
								{
									BYTE sp1p ;
									BYTE sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 128 : sp1[ 0 ] ;
									sp2p = offset0 + p + n >= SSampleNum ? 128 : sp2[ 0 ] ;
									if( offset1 + n < DSampleNum )
									{
										dp1[ 0 ] =   sp1p ;
									}
									if( offset1 + p + n < DSampleNum )
									{
										dp2[ 0 ] = ( sp2p * ( p - n ) + sp1p * n ) / p ;
									}
								}

								if( offset1 + p * 2 < DSampleNum &&
									offset0 + p     < SSampleNum )
								{
									q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p     ) * 2 ] ;
									dp1 = &DBuffer[ ( offset1 + p * 2 ) * 2 ] ;
									movnum = q - p ;
									if( offset0 + p + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p ) ;
										q = p + movnum ;
									}
									if( offset1 + p * 2 + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p * 2 ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										BYTE sp1p ;

										sp1p = offset0 + p + n >= SSampleNum ? 128 : sp1[ 0 ] ;
										dp1[ 0 ] = sp1p ;
									}
								}
							}

							offset0 += q ;		// offset0�̍X�V
							offset1 += p + q ;	// offset1�̍X�V
						}
						if( offset1 < DSampleNum )
						{
							BYTE *dp ;
							int loopnum ;

							loopnum = DSampleNum - offset1 ;
							dp = &DBuffer[ offset1 * 2 ] ;
							for( n = 0 ; n < loopnum ; n ++, dp += 2 )
							{
								*dp = 128 ;
							}
						}



						offset0 = 0 ;
						offset1 = 0 ;
						while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								BYTE *sp1 ;
								BYTE *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 + 1 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									r += ( ( ( int )sp1[ 0 ] - 128 ) * ( int )( sp2[ 0 ] - 128 ) ) / 128 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								BYTE *sp1, *dp1 ;
								BYTE *sp2, *dp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 + 1 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 + 1 ] ;
								dp2 = &DBuffer[ ( offset1 + p ) * 2 + 1 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2, dp2 += 2 )
								{
									dp1[ 0 ] =   sp1[ 0 ] ;
									dp2[ 0 ] = ( sp2[ 0 ] * ( p - n ) + sp1[ 0 ] * n ) / p ;
								}

								if( offset1 + p * 2 < DSampleNum &&
									offset0 + p     < SSampleNum )
								{
									q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p     ) * 2 + 1 ] ;
									dp1 = &DBuffer[ ( offset1 + p * 2 ) * 2 + 1 ] ;
									movnum = q - p ;
									if( offset0 + p + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p ) ;
										q = p + movnum ;
									}
									if( offset1 + p * 2 + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p * 2 ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										dp1[ 0 ] = sp1[ 0 ] ;
									}
								}
							}

							offset0 += q ;		// offset0�̍X�V
							offset1 += p + q ;	// offset1�̍X�V
						}

						while( offset1 < DSampleNum && offset0 < SSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								BYTE *sp1 ;
								BYTE *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 + 1 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									int sp1p ;
									int sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : ( int )sp1[ 0 ] - 128 ;
									sp2p = offset0 + m + n >= SSampleNum ? 0 : ( int )sp2[ 0 ] - 128 ;
									r += ( sp1p * sp2p ) / 128 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								BYTE *sp1, *dp1 ;
								BYTE *sp2, *dp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 + 1 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 + 1 ] ;
								dp2 = &DBuffer[ ( offset1 + p ) * 2 + 1 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2, dp2 += 2 )
								{
									BYTE sp1p ;
									BYTE sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 128 : sp1[ 0 ] ;
									sp2p = offset0 + p + n >= SSampleNum ? 128 : sp2[ 0 ] ;
									if( offset1 + n < DSampleNum )
									{
										dp1[ 0 ] =   sp1p ;
									}
									if( offset1 + p + n < DSampleNum )
									{
										dp2[ 0 ] = ( sp2p * ( p - n ) + sp1p * n ) / p ;
									}
								}

								if( offset1 + p * 2 < DSampleNum &&
									offset0 + p     < SSampleNum )
								{
									q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p     ) * 2 + 1 ] ;
									dp1 = &DBuffer[ ( offset1 + p * 2 ) * 2 + 1 ] ;
									movnum = q - p ;
									if( offset0 + p + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p ) ;
										q = p + movnum ;
									}
									if( offset1 + p * 2 + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p * 2 ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										BYTE sp1p ;

										sp1p = offset0 + p + n >= SSampleNum ? 128 : sp1[ 0 ] ;
										dp1[ 0 ] = sp1p ;
									}
								}
							}

							offset0 += q ;		// offset0�̍X�V
							offset1 += p + q ;	// offset1�̍X�V
						}
						if( offset1 < DSampleNum )
						{
							BYTE *dp ;
							int loopnum ;

							loopnum = DSampleNum - offset1 ;
							dp = &DBuffer[ offset1 * 2 + 1 ] ;
							for( n = 0 ; n < loopnum ; n ++, dp += 2 )
							{
								*dp = 128 ;
							}
						}
						break ;

					case 1 :
						offset0 = 0 ;
						offset1 = 0 ;
						while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								BYTE *sp1 ;
								BYTE *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + m ] ;
								for( n = 0 ; n < template_size ; n++, sp1 ++, sp2 ++ )
								{
									r += ( ( ( int )sp1[ 0 ] - 128 ) * ( int )( sp2[ 0 ] - 128 ) ) / 128 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								BYTE *sp1, *dp1 ;
								BYTE *sp2, *dp2 ;
								int movnum ;

								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + p ] ;
								dp1 = &DBuffer[ offset1     ] ;
								dp2 = &DBuffer[ offset1 + p ] ;
								for( n = 0; n < p ; n++, sp1 ++, sp2 ++, dp1 ++, dp2 ++ )
								{
									dp1[ 0 ] =   sp1[ 0 ] ;
									dp2[ 0 ] = ( sp2[ 0 ] * ( p - n ) + sp1[ 0 ] * n ) / p ;
								}

								if( offset1 + p * 2 < DSampleNum &&
									offset0 + p     < SSampleNum )
								{
									q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
									sp1 = &SBuffer[ offset0 + p     ] ;
									dp1 = &DBuffer[ offset1 + p * 2 ] ;
									movnum = q - p ;
									if( offset0 + p + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p ) ;
										q = p + movnum ;
									}
									if( offset1 + p * 2 + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p * 2 ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 ++, dp1 ++ )
									{
										dp1[ 0 ] = sp1[ 0 ] ;
									}
								}
							}

							offset0 += q ;		// offset0�̍X�V
							offset1 += p + q ;	// offset1�̍X�V
						}

						while( offset1 < DSampleNum && offset0 < SSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								BYTE *sp1 ;
								BYTE *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + m ] ;
								for( n = 0 ; n < template_size ; n++, sp1 ++, sp2 ++ )
								{
									int sp1p ;
									int sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : ( int )sp1[ 0 ] - 128 ;
									sp2p = offset0 + m + n >= SSampleNum ? 0 : ( int )sp2[ 0 ] - 128 ;
									r += ( sp1p * sp2p ) / 128 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								BYTE *sp1, *dp1 ;
								BYTE *sp2, *dp2 ;
								int movnum ;

								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + p ] ;
								dp1 = &DBuffer[ offset1     ] ;
								dp2 = &DBuffer[ offset1 + p ] ;
								for( n = 0; n < p ; n++, sp1 ++, sp2 ++, dp1 ++, dp2 ++ )
								{
									BYTE sp1p ;
									BYTE sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 128 : sp1[ 0 ] ;
									sp2p = offset0 + p + n >= SSampleNum ? 128 : sp2[ 0 ] ;
									if( offset1 + n < DSampleNum )
									{
										dp1[ 0 ] =   sp1p ;
									}
									if( offset1 + p + n < DSampleNum )
									{
										dp2[ 0 ] = ( sp2p * ( p - n ) + sp1p * n ) / p ;
									}
								}

								if( offset1 + p * 2 < DSampleNum &&
									offset0 + p     < SSampleNum )
								{
									q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
									sp1 = &SBuffer[ offset0 + p     ] ;
									dp1 = &DBuffer[ offset1 + p * 2 ] ;
									movnum = q - p ;
									if( offset0 + p + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p ) ;
										q = p + movnum ;
									}
									if( offset1 + p * 2 + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p * 2 ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 ++, dp1 ++ )
									{
										BYTE sp1p ;

										sp1p = offset0 + p + n >= SSampleNum ? 128 : sp1[ 0 ] ;
										dp1[ 0 ] = sp1p ;
									}
								}
							}

							offset0 += q ;		// offset0�̍X�V
							offset1 += p + q ;	// offset1�̍X�V
						}
						if( offset1 < DSampleNum )
						{
							BYTE *dp ;
							int loopnum ;

							loopnum = DSampleNum - offset1 ;
							dp = &DBuffer[ offset1 ] ;
							for( n = 0 ; n < loopnum ; n ++, dp ++ )
							{
								*dp = 128 ;
							}
						}
						break ;
					}
				}
				break ;
			}
			break ;

		case WAVE_FORMAT_IEEE_FLOAT :
			{
				float *SBuffer ;
				float *DBuffer ;
				float r ;
				float max_of_r;

				SBuffer = ( float * )SSSound->Wave.Buffer ;
				DBuffer = ( float * )DSSound->Wave.Buffer ;

				switch( SSSound->BufferFormat.nChannels )
				{
				case 2 :
					offset0 = 0 ;
					offset1 = 0 ;
					while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
					{
						max_of_r = 0.0f ;
						p = pmin ;
						for( m = pmin ; m <= pmax ; m++ )
						{
							float *sp1 ;
							float *sp2 ;

							r = 0.0f ;
							sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
							sp2 = &SBuffer[ ( offset0 + m ) * 2 ] ;
							for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
							{
								r += sp1[ 0 ] * sp2[ 0 ] ; // ���֊֐�
							}
							if( r > max_of_r )
							{
								max_of_r = r ;	// ���֊֐��̃s�[�N
								p = m ;			// ���f�[�^�̊�{����
							}
						}

						{
							float *sp1, *dp1 ;
							float *sp2, *dp2 ;
							int movnum ;

							sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
							sp2 = &SBuffer[ ( offset0 + p ) * 2 ] ;
							dp1 = &DBuffer[ ( offset1     ) * 2 ] ;
							dp2 = &DBuffer[ ( offset1 + p ) * 2 ] ;
							for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2, dp2 += 2 )
							{
								dp1[ 0 ] =   sp1[ 0 ] ;
								dp2[ 0 ] = ( sp2[ 0 ] * ( p - n ) + sp1[ 0 ] * n ) / p ;
							}

							if( offset1 + p * 2 < DSampleNum &&
								offset0 + p     < SSampleNum )
							{
								q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
								sp1 = &SBuffer[ ( offset0 + p     ) * 2 ] ;
								dp1 = &DBuffer[ ( offset1 + p * 2 ) * 2 ] ;
								movnum = q - p ;
								if( offset0 + p + movnum > SSampleNum )
								{
									movnum = SSampleNum - ( offset0 + p ) ;
									q = p + movnum ;
								}
								if( offset1 + p * 2 + movnum > DSampleNum )
								{
									movnum = DSampleNum - ( offset1 + p * 2 ) ;
									q = p + movnum ;
								}
								for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
								{
									dp1[ 0 ] = sp1[ 0 ] ;
								}
							}
						}

						offset0 += q ;		// offset0�̍X�V
						offset1 += p + q ;	// offset1�̍X�V
					}

					while( offset1 < DSampleNum && offset0 < SSampleNum )
					{
						max_of_r = 0.0f ;
						p = pmin ;
						for( m = pmin ; m <= pmax ; m++ )
						{
							float *sp1 ;
							float *sp2 ;

							r = 0.0f ;
							sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
							sp2 = &SBuffer[ ( offset0 + m ) * 2 ] ;
							for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
							{
								float sp1p ;
								float sp2p ;

								sp1p = offset0     + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
								sp2p = offset0 + m + n >= SSampleNum ? 0.0f : sp2[ 0 ] ;
								r += sp1p * sp2p ; // ���֊֐�
							}
							if( r > max_of_r )
							{
								max_of_r = r ;	// ���֊֐��̃s�[�N
								p = m ;			// ���f�[�^�̊�{����
							}
						}

						{
							float *sp1, *dp1 ;
							float *sp2, *dp2 ;
							int movnum ;

							sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
							sp2 = &SBuffer[ ( offset0 + p ) * 2 ] ;
							dp1 = &DBuffer[ ( offset1     ) * 2 ] ;
							dp2 = &DBuffer[ ( offset1 + p ) * 2 ] ;
							for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2, dp2 += 2 )
							{
								float sp1p ;
								float sp2p ;

								sp1p = offset0     + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
								sp2p = offset0 + p + n >= SSampleNum ? 0.0f : sp2[ 0 ] ;
								if( offset1 + n < DSampleNum )
								{
									dp1[ 0 ] =   sp1p ;
								}
								if( offset1 + p + n < DSampleNum )
								{
									dp2[ 0 ] = ( sp2p * ( p - n ) + sp1p * n ) / p ;
								}
							}

							if( offset1 + p * 2 < DSampleNum &&
								offset0 + p     < SSampleNum )
							{
								q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
								sp1 = &SBuffer[ ( offset0 + p     ) * 2 ] ;
								dp1 = &DBuffer[ ( offset1 + p * 2 ) * 2 ] ;
								movnum = q - p ;
								if( offset0 + p + movnum > SSampleNum )
								{
									movnum = SSampleNum - ( offset0 + p ) ;
									q = p + movnum ;
								}
								if( offset1 + p * 2 + movnum > DSampleNum )
								{
									movnum = DSampleNum - ( offset1 + p * 2 ) ;
									q = p + movnum ;
								}
								for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
								{
									float sp1p ;

									sp1p = offset0 + p + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
									dp1[ 0 ] = sp1p ;
								}
							}
						}

						offset0 += q ;		// offset0�̍X�V
						offset1 += p + q ;	// offset1�̍X�V
					}
					if( offset1 < DSampleNum )
					{
						float *dp ;
						int loopnum ;

						loopnum = DSampleNum - offset1 ;
						dp = &DBuffer[ offset1 * 2 ] ;
						for( n = 0 ; n < loopnum ; n ++, dp += 2 )
						{
							*dp = 0.0f ;
						}
					}



					offset0 = 0 ;
					offset1 = 0 ;
					while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
					{
						max_of_r = 0.0f ;
						p = pmin ;
						for( m = pmin ; m <= pmax ; m++ )
						{
							float *sp1 ;
							float *sp2 ;

							r = 0.0f ;
							sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
							sp2 = &SBuffer[ ( offset0 + m ) * 2 + 1 ] ;
							for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
							{
								r += sp1[ 0 ] * sp2[ 0 ] ; // ���֊֐�
							}
							if( r > max_of_r )
							{
								max_of_r = r ;	// ���֊֐��̃s�[�N
								p = m ;			// ���f�[�^�̊�{����
							}
						}

						{
							float *sp1, *dp1 ;
							float *sp2, *dp2 ;
							int movnum ;

							sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
							sp2 = &SBuffer[ ( offset0 + p ) * 2 + 1 ] ;
							dp1 = &DBuffer[ ( offset1     ) * 2 + 1 ] ;
							dp2 = &DBuffer[ ( offset1 + p ) * 2 + 1 ] ;
							for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2, dp2 += 2 )
							{
								dp1[ 0 ] =   sp1[ 0 ] ;
								dp2[ 0 ] = ( sp2[ 0 ] * ( p - n ) + sp1[ 0 ] * n ) / p ;
							}

							if( offset1 + p * 2 < DSampleNum &&
								offset0 + p     < SSampleNum )
							{
								q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
								sp1 = &SBuffer[ ( offset0 + p     ) * 2 + 1 ] ;
								dp1 = &DBuffer[ ( offset1 + p * 2 ) * 2 + 1 ] ;
								movnum = q - p ;
								if( offset0 + p + movnum > SSampleNum )
								{
									movnum = SSampleNum - ( offset0 + p ) ;
									q = p + movnum ;
								}
								if( offset1 + p * 2 + movnum > DSampleNum )
								{
									movnum = DSampleNum - ( offset1 + p * 2 ) ;
									q = p + movnum ;
								}
								for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
								{
									dp1[ 0 ] = sp1[ 0 ] ;
								}
							}
						}

						offset0 += q ;		// offset0�̍X�V
						offset1 += p + q ;	// offset1�̍X�V
					}

					while( offset1 < DSampleNum && offset0 < SSampleNum )
					{
						max_of_r = 0.0f ;
						p = pmin ;
						for( m = pmin ; m <= pmax ; m++ )
						{
							float *sp1 ;
							float *sp2 ;

							r = 0.0f ;
							sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
							sp2 = &SBuffer[ ( offset0 + m ) * 2 + 1 ] ;
							for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
							{
								float sp1p ;
								float sp2p ;

								sp1p = offset0     + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
								sp2p = offset0 + m + n >= SSampleNum ? 0.0f : sp2[ 0 ] ;
								r += sp1p * sp2p ; // ���֊֐�
							}
							if( r > max_of_r )
							{
								max_of_r = r ;	// ���֊֐��̃s�[�N
								p = m ;			// ���f�[�^�̊�{����
							}
						}

						{
							float *sp1, *dp1 ;
							float *sp2, *dp2 ;
							int movnum ;

							sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
							sp2 = &SBuffer[ ( offset0 + p ) * 2 + 1 ] ;
							dp1 = &DBuffer[ ( offset1     ) * 2 + 1 ] ;
							dp2 = &DBuffer[ ( offset1 + p ) * 2 + 1 ] ;
							for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2, dp2 += 2 )
							{
								float sp1p ;
								float sp2p ;

								sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
								sp2p = offset0 + p + n >= SSampleNum ? 0 : sp2[ 0 ] ;
								if( offset1 + n < DSampleNum )
								{
									dp1[ 0 ] =   sp1p ;
								}
								if( offset1 + p + n < DSampleNum )
								{
									dp2[ 0 ] = ( sp2p * ( p - n ) + sp1p * n ) / p ;
								}
							}

							if( offset1 + p * 2 < DSampleNum &&
								offset0 + p     < SSampleNum )
							{
								q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
								sp1 = &SBuffer[ ( offset0 + p     ) * 2 + 1 ] ;
								dp1 = &DBuffer[ ( offset1 + p * 2 ) * 2 + 1 ] ;
								movnum = q - p ;
								if( offset0 + p + movnum > SSampleNum )
								{
									movnum = SSampleNum - ( offset0 + p ) ;
									q = p + movnum ;
								}
								if( offset1 + p * 2 + movnum > DSampleNum )
								{
									movnum = DSampleNum - ( offset1 + p * 2 ) ;
									q = p + movnum ;
								}
								for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
								{
									float sp1p ;

									sp1p = offset0 + p + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									dp1[ 0 ] = sp1p ;
								}
							}
						}

						offset0 += q ;		// offset0�̍X�V
						offset1 += p + q ;	// offset1�̍X�V
					}
					if( offset1 < DSampleNum )
					{
						float *dp ;
						int loopnum ;

						loopnum = DSampleNum - offset1 ;
						dp = &DBuffer[ offset1 * 2 + 1 ] ;
						for( n = 0 ; n < loopnum ; n ++, dp += 2 )
						{
							*dp = 0.0f ;
						}
					}
					break ;

				case 1 :
					offset0 = 0 ;
					offset1 = 0 ;
					while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
					{
						max_of_r = 0.0f ;
						p = pmin ;
						for( m = pmin ; m <= pmax ; m++ )
						{
							float *sp1 ;
							float *sp2 ;

							r = 0.0f ;
							sp1 = &SBuffer[ offset0     ] ;
							sp2 = &SBuffer[ offset0 + m ] ;
							for( n = 0 ; n < template_size ; n++, sp1 ++, sp2 ++ )
							{
								r += sp1[ 0 ] * sp2[ 0 ] ; // ���֊֐�
							}
							if( r > max_of_r )
							{
								max_of_r = r ;	// ���֊֐��̃s�[�N
								p = m ;			// ���f�[�^�̊�{����
							}
						}

						{
							float *sp1, *dp1 ;
							float *sp2, *dp2 ;
							int movnum ;

							sp1 = &SBuffer[ offset0     ] ;
							sp2 = &SBuffer[ offset0 + p ] ;
							dp1 = &DBuffer[ offset1     ] ;
							dp2 = &DBuffer[ offset1 + p ] ;
							for( n = 0; n < p ; n++, sp1 ++, sp2 ++, dp1 ++, dp2 ++ )
							{
								dp1[ 0 ] =   sp1[ 0 ] ;
								dp2[ 0 ] = ( sp2[ 0 ] * ( p - n ) + sp1[ 0 ] * n ) / p ;
							}

							if( offset1 + p * 2 < DSampleNum &&
								offset0 + p     < SSampleNum )
							{
								q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
								sp1 = &SBuffer[ offset0 + p     ] ;
								dp1 = &DBuffer[ offset1 + p * 2 ] ;
								movnum = q - p ;
								if( offset0 + p + movnum > SSampleNum )
								{
									movnum = SSampleNum - ( offset0 + p ) ;
									q = p + movnum ;
								}
								if( offset1 + p * 2 + movnum > DSampleNum )
								{
									movnum = DSampleNum - ( offset1 + p * 2 ) ;
									q = p + movnum ;
								}
								for( n = 0 ; n < movnum ; n++, sp1 ++, dp1 ++ )
								{
									dp1[ 0 ] = sp1[ 0 ] ;
								}
							}
						}

						offset0 += q ;		// offset0�̍X�V
						offset1 += p + q ;	// offset1�̍X�V
					}

					while( offset1 < DSampleNum && offset0 < SSampleNum )
					{
						max_of_r = 0.0f ;
						p = pmin ;
						for( m = pmin ; m <= pmax ; m++ )
						{
							float *sp1 ;
							float *sp2 ;

							r = 0.0f ;
							sp1 = &SBuffer[ offset0     ] ;
							sp2 = &SBuffer[ offset0 + m ] ;
							for( n = 0 ; n < template_size ; n++, sp1 ++, sp2 ++ )
							{
								float sp1p ;
								float sp2p ;

								sp1p = offset0     + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
								sp2p = offset0 + m + n >= SSampleNum ? 0.0f : sp2[ 0 ] ;
								r += sp1p * sp2p ; // ���֊֐�
							}
							if( r > max_of_r )
							{
								max_of_r = r ;	// ���֊֐��̃s�[�N
								p = m ;			// ���f�[�^�̊�{����
							}
						}

						{
							float *sp1, *dp1 ;
							float *sp2, *dp2 ;
							int movnum ;

							sp1 = &SBuffer[ offset0     ] ;
							sp2 = &SBuffer[ offset0 + p ] ;
							dp1 = &DBuffer[ offset1     ] ;
							dp2 = &DBuffer[ offset1 + p ] ;
							for( n = 0; n < p ; n++, sp1 ++, sp2 ++, dp1 ++, dp2 ++ )
							{
								float sp1p ;
								float sp2p ;

								sp1p = offset0     + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
								sp2p = offset0 + p + n >= SSampleNum ? 0.0f : sp2[ 0 ] ;
								if( offset1 + n < DSampleNum )
								{
									dp1[ 0 ] =   sp1p ;
								}
								if( offset1 + p + n < DSampleNum )
								{
									dp2[ 0 ] = ( sp2p * ( p - n ) + sp1p * n ) / p ;
								}
							}

							if( offset1 + p * 2 < DSampleNum &&
								offset0 + p     < SSampleNum )
							{
								q = ( p * rateI * 512 / ( 512 - rateI ) + 256 ) / 512 ;
								sp1 = &SBuffer[ offset0 + p     ] ;
								dp1 = &DBuffer[ offset1 + p * 2 ] ;
								movnum = q - p ;
								if( offset0 + p + movnum > SSampleNum )
								{
									movnum = SSampleNum - ( offset0 + p ) ;
									q = p + movnum ;
								}
								if( offset1 + p * 2 + movnum > DSampleNum )
								{
									movnum = DSampleNum - ( offset1 + p * 2 ) ;
									q = p + movnum ;
								}
								for( n = 0 ; n < movnum ; n++, sp1 ++, dp1 ++ )
								{
									float sp1p ;

									sp1p = offset0 + p + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
									dp1[ 0 ] = sp1p ;
								}
							}
						}

						offset0 += q ;		// offset0�̍X�V
						offset1 += p + q ;	// offset1�̍X�V
					}
					if( offset1 < DSampleNum )
					{
						float *dp ;
						int loopnum ;

						loopnum = DSampleNum - offset1 ;
						dp = &DBuffer[ offset1 ] ;
						for( n = 0 ; n < loopnum ; n ++, dp ++ )
						{
							*dp = 0.0f ;
						}
					}
					break ;
				}
			}
			break ;
		}
	}
	else
	{
		int n, m, template_size, pmin, pmax, p, q, offset0, offset1 ;
		double rate ;
		int rateI ;
		int SSampleNum ;
		int DSampleNum ;

		rate = ( double )SSSound->Wave.BufferSampleNum / DSSound->Wave.BufferSampleNum ; // rate�͈̔͂�0.5<=rate<1.0
		rateI = _DTOL( rate * 512.0 ) ;
		rate = rateI / 512.0 ;

		SSampleNum = SSSound->Wave.BufferSampleNum ;
		DSampleNum = DSSound->Wave.BufferSampleNum ;

		template_size = _DTOL( SSSound->BufferFormat.nSamplesPerSec * 0.01  ) ; // 10ms
		pmin          = _DTOL( SSSound->BufferFormat.nSamplesPerSec * 0.005 ) ; // 5ms
		pmax          = _DTOL( SSSound->BufferFormat.nSamplesPerSec * 0.02  ) ; // 20ms

		switch( SSSound->BufferFormat.wFormatTag )
		{
		case WAVE_FORMAT_PCM :

			switch( SSSound->BufferFormat.wBitsPerSample )
			{
			case 16 :
				{
					short *SBuffer ;
					short *DBuffer ;
					int r ;
					int max_of_r;

					SBuffer = ( short * )SSSound->Wave.Buffer ;
					DBuffer = ( short * )DSSound->Wave.Buffer ;

					switch( SSSound->BufferFormat.nChannels )
					{
					case 2 :
						offset0 = 0 ;
						offset1 = 0 ;
						while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								short *sp1 ;
								short *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									r += ( sp1[ 0 ] * sp2[ 0 ] ) / 32768 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								short *sp1, *dp1 ;
								short *sp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2 )
								{
									dp1[ 0 ] = ( sp1[ 0 ] * ( p - n ) + sp2[ 0 ] * n ) / p ;
								}

								if( offset1 + p     < DSampleNum &&
									offset0 + p * 2 < SSampleNum )
								{
									q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p * 2 ) * 2 ] ;
									dp1 = &DBuffer[ ( offset1 + p     ) * 2 ] ;
									movnum = q - p ;
									if( offset0 + p * 2 + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p * 2 ) ;
										q = p + movnum ;
									}
									if( offset1 + p + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										dp1[ 0 ] = sp1[ 0 ] ;
									}
								}
							}

							offset0 += p + q ;		// offset0�̍X�V
							offset1 += q ;	// offset1�̍X�V
						}

						while( offset1 < DSampleNum && offset0 < SSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								short *sp1 ;
								short *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									int sp1p ;
									int sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									sp2p = offset0 + m + n >= SSampleNum ? 0 : sp2[ 0 ] ;
									r += ( sp1p * sp2p ) / 32768 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								short *sp1, *dp1 ;
								short *sp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2 )
								{
									short sp1p ;
									short sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									sp2p = offset0 + p + n >= SSampleNum ? 0 : sp2[ 0 ] ;
									if( offset1 + n < DSampleNum )
									{
										dp1[ 0 ] = ( sp1p * ( p - n ) + sp2p * n ) / p ;
									}
								}

								if( offset1 + p     < DSampleNum &&
									offset0 + p * 2 < SSampleNum )
								{
									q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p * 2 ) * 2 ] ;
									dp1 = &DBuffer[ ( offset1 + p     ) * 2 ] ;
									movnum = q - p ;
									if( offset0 + p * 2 + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p * 2 ) ;
										q = p + movnum ;
									}
									if( offset1 + p + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										short sp1p ;

										sp1p = offset0 + p + n >= SSampleNum ? 0 : sp1[ 0 ] ;
										dp1[ 0 ] = sp1p ;
									}
								}
							}

							offset0 += p + q ;		// offset0�̍X�V
							offset1 += q ;	// offset1�̍X�V
						}
						if( offset1 < DSampleNum )
						{
							short *dp ;
							int loopnum ;

							loopnum = DSampleNum - offset1 ;
							dp = &DBuffer[ offset1 * 2 ] ;
							for( n = 0 ; n < loopnum ; n ++, dp += 2 )
							{
								*dp = 0 ;
							}
						}



						offset0 = 0 ;
						offset1 = 0 ;
						while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								short *sp1 ;
								short *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 + 1 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									r += ( sp1[ 0 ] * sp2[ 0 ] ) / 32768 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								short *sp1, *dp1 ;
								short *sp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 + 1 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 + 1 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2 )
								{
									dp1[ 0 ] = ( sp1[ 0 ] * ( p - n ) + sp2[ 0 ] * n ) / p ;
								}

								if( offset1 + p     < DSampleNum &&
									offset0 + p * 2 < SSampleNum )
								{
									q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p * 2 ) * 2 + 1 ] ;
									dp1 = &DBuffer[ ( offset1 + p     ) * 2 + 1 ] ;
									movnum = q - p ;
									if( offset0 + p * 2 + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p * 2 ) ;
										q = p + movnum ;
									}
									if( offset1 + p + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										dp1[ 0 ] = sp1[ 0 ] ;
									}
								}
							}

							offset0 += p + q ;	// offset0�̍X�V
							offset1 += q ;		// offset1�̍X�V
						}

						while( offset1 < DSampleNum && offset0 < SSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								short *sp1 ;
								short *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 + 1 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									int sp1p ;
									int sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									sp2p = offset0 + m + n >= SSampleNum ? 0 : sp2[ 0 ] ;
									r += ( sp1p * sp2p ) / 32768 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								short *sp1, *dp1 ;
								short *sp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 + 1 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 + 1 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2 )
								{
									short sp1p ;
									short sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									sp2p = offset0 + p + n >= SSampleNum ? 0 : sp2[ 0 ] ;
									if( offset1 + n < DSampleNum )
									{
										dp1[ 0 ] = ( sp1p * ( p - n ) + sp2p * n ) / p ;
									}
								}

								if( offset1 + p     < DSampleNum &&
									offset0 + p * 2 < SSampleNum )
								{
									q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p * 2 ) * 2 + 1 ] ;
									dp1 = &DBuffer[ ( offset1 + p     ) * 2 + 1 ] ;
									movnum = q - p ;
									if( offset0 + p * 2 + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p * 2 ) ;
										q = p + movnum ;
									}
									if( offset1 + p + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										short sp1p ;

										sp1p = offset0 + p + n >= SSampleNum ? 0 : sp1[ 0 ] ;
										dp1[ 0 ] = sp1p ;
									}
								}
							}

							offset0 += p + q ;	// offset0�̍X�V
							offset1 += q ;		// offset1�̍X�V
						}
						if( offset1 < DSampleNum )
						{
							short *dp ;
							int loopnum ;

							loopnum = DSampleNum - offset1 ;
							dp = &DBuffer[ offset1 * 2 + 1 ] ;
							for( n = 0 ; n < loopnum ; n ++, dp += 2 )
							{
								*dp = 0 ;
							}
						}
						break ;

					case 1 :
						offset0 = 0 ;
						offset1 = 0 ;
						while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								short *sp1 ;
								short *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + m ] ;
								for( n = 0 ; n < template_size ; n++, sp1 ++, sp2 ++ )
								{
									r += ( sp1[ 0 ] * sp2[ 0 ] ) / 32768 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								short *sp1, *dp1 ;
								short *sp2 ;
								int movnum ;

								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + p ] ;
								dp1 = &DBuffer[ offset1     ] ;
								for( n = 0; n < p ; n++, sp1 ++, sp2 ++, dp1 ++ )
								{
									dp1[ 0 ] = ( sp1[ 0 ] * ( p - n ) + sp2[ 0 ] * n ) / p ;
								}

								if( offset1 + p     < DSampleNum &&
									offset0 + p * 2 < SSampleNum )
								{
									q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
									sp1 = &SBuffer[ offset0 + p * 2 ] ;
									dp1 = &DBuffer[ offset1 + p     ] ;
									movnum = q - p ;
									if( offset0 + p * 2 + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p * 2 ) ;
										q = p + movnum ;
									}
									if( offset1 + p + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 ++, dp1 ++ )
									{
										dp1[ 0 ] = sp1[ 0 ] ;
									}
								}
							}

							offset0 += p + q ;	// offset0�̍X�V
							offset1 += q ;		// offset1�̍X�V
						}

						while( offset1 < DSampleNum && offset0 < SSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								short *sp1 ;
								short *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + m ] ;
								for( n = 0 ; n < template_size ; n++, sp1 ++, sp2 ++ )
								{
									int sp1p ;
									int sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									sp2p = offset0 + m + n >= SSampleNum ? 0 : sp2[ 0 ] ;
									r += ( sp1p * sp2p ) / 32768 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								short *sp1, *dp1 ;
								short *sp2 ;
								int movnum ;

								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + p ] ;
								dp1 = &DBuffer[ offset1     ] ;
								for( n = 0; n < p ; n++, sp1 ++, sp2 ++, dp1 ++ )
								{
									short sp1p ;
									short sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									sp2p = offset0 + p + n >= SSampleNum ? 0 : sp2[ 0 ] ;
									if( offset1 + n < DSampleNum )
									{
										dp1[ 0 ] = ( sp1p * ( p - n ) + sp2p * n ) / p ;
									}
								}

								if( offset1 + p     < DSampleNum &&
									offset0 + p * 2 < SSampleNum )
								{
									q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
									sp1 = &SBuffer[ offset0 + p * 2 ] ;
									dp1 = &DBuffer[ offset1 + p     ] ;
									movnum = q - p ;
									if( offset0 + p * 2 + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p * 2 ) ;
										q = p + movnum ;
									}
									if( offset1 + p + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 ++, dp1 ++ )
									{
										short sp1p ;

										sp1p = offset0 + p + n >= SSampleNum ? 0 : sp1[ 0 ] ;
										dp1[ 0 ] = sp1p ;
									}
								}
							}

							offset0 += p + q ;	// offset0�̍X�V
							offset1 += q ;		// offset1�̍X�V
						}
						if( offset1 < DSampleNum )
						{
							short *dp ;
							int loopnum ;

							loopnum = DSampleNum - offset1 ;
							dp = &DBuffer[ offset1 ] ;
							for( n = 0 ; n < loopnum ; n ++, dp ++ )
							{
								*dp = 0 ;
							}
						}
						break ;
					}
				}
				break ;

			case 8 :
				{
					BYTE *SBuffer ;
					BYTE *DBuffer ;
					int r ;
					int max_of_r;

					SBuffer = ( BYTE * )SSSound->Wave.Buffer ;
					DBuffer = ( BYTE * )DSSound->Wave.Buffer ;

					switch( SSSound->BufferFormat.nChannels )
					{
					case 2 :
						offset0 = 0 ;
						offset1 = 0 ;
						while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								BYTE *sp1 ;
								BYTE *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									r += ( ( ( int )sp1[ 0 ] - 128 ) * ( int )( sp2[ 0 ] - 128 ) ) / 128 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								BYTE *sp1, *dp1 ;
								BYTE *sp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2 )
								{
									dp1[ 0 ] = ( sp1[ 0 ] * ( p - n ) + sp2[ 0 ] * n ) / p ;
								}

								if( offset1 + p     < DSampleNum &&
									offset0 + p * 2 < SSampleNum )
								{
									q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p * 2 ) * 2 ] ;
									dp1 = &DBuffer[ ( offset1 + p     ) * 2 ] ;
									movnum = q - p ;
									if( offset0 + p * 2 + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p * 2 ) ;
										q = p + movnum ;
									}
									if( offset1 + p + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										dp1[ 0 ] = sp1[ 0 ] ;
									}
								}
							}

							offset0 += p + q ;	// offset0�̍X�V
							offset1 += q ;		// offset1�̍X�V
						}

						while( offset1 < DSampleNum && offset0 < SSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								BYTE *sp1 ;
								BYTE *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									int sp1p ;
									int sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : ( int )sp1[ 0 ] - 128 ;
									sp2p = offset0 + m + n >= SSampleNum ? 0 : ( int )sp2[ 0 ] - 128 ;
									r += ( sp1p * sp2p ) / 128 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								BYTE *sp1, *dp1 ;
								BYTE *sp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2 )
								{
									BYTE sp1p ;
									BYTE sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 128 : sp1[ 0 ] ;
									sp2p = offset0 + p + n >= SSampleNum ? 128 : sp2[ 0 ] ;
									if( offset1 + n < DSampleNum )
									{
										dp1[ 0 ] = ( sp1p * ( p - n ) + sp2p * n ) / p ;
									}
								}

								if( offset1 + p     < DSampleNum &&
									offset0 + p * 2 < SSampleNum )
								{
									q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p * 2 ) * 2 ] ;
									dp1 = &DBuffer[ ( offset1 + p     ) * 2 ] ;
									movnum = q - p ;
									if( offset0 + p * 2 + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p * 2 ) ;
										q = p + movnum ;
									}
									if( offset1 + p + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										BYTE sp1p ;

										sp1p = offset0 + p + n >= SSampleNum ? 128 : sp1[ 0 ] ;
										dp1[ 0 ] = sp1p ;
									}
								}
							}

							offset0 += p + q ;	// offset0�̍X�V
							offset1 += q ;		// offset1�̍X�V
						}
						if( offset1 < DSampleNum )
						{
							BYTE *dp ;
							int loopnum ;

							loopnum = DSampleNum - offset1 ;
							dp = &DBuffer[ offset1 * 2 ] ;
							for( n = 0 ; n < loopnum ; n ++, dp += 2 )
							{
								*dp = 128 ;
							}
						}



						offset0 = 0 ;
						offset1 = 0 ;
						while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								BYTE *sp1 ;
								BYTE *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 + 1 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									r += ( ( ( int )sp1[ 0 ] - 128 ) * ( int )( sp2[ 0 ] - 128 ) ) / 128 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								BYTE *sp1, *dp1 ;
								BYTE *sp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 + 1 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 + 1 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2 )
								{
									dp1[ 0 ] = ( sp1[ 0 ] * ( p - n ) + sp2[ 0 ] * n ) / p ;
								}

								if( offset1 + p     < DSampleNum &&
									offset0 + p * 2 < SSampleNum )
								{
									q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p * 2 ) * 2 + 1 ] ;
									dp1 = &DBuffer[ ( offset1 + p     ) * 2 + 1 ] ;
									movnum = q - p ;
									if( offset0 + p * 2 + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p * 2 ) ;
										q = p + movnum ;
									}
									if( offset1 + p + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										dp1[ 0 ] = sp1[ 0 ] ;
									}
								}
							}

							offset0 += p + q ;	// offset0�̍X�V
							offset1 += q ;		// offset1�̍X�V
						}

						while( offset1 < DSampleNum && offset0 < SSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								BYTE *sp1 ;
								BYTE *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + m ) * 2 + 1 ] ;
								for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
								{
									int sp1p ;
									int sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : ( int )sp1[ 0 ] - 128 ;
									sp2p = offset0 + m + n >= SSampleNum ? 0 : ( int )sp2[ 0 ] - 128 ;
									r += ( sp1p * sp2p ) / 128 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								BYTE *sp1, *dp1 ;
								BYTE *sp2 ;
								int movnum ;

								sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
								sp2 = &SBuffer[ ( offset0 + p ) * 2 + 1 ] ;
								dp1 = &DBuffer[ ( offset1     ) * 2 + 1 ] ;
								for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2 )
								{
									BYTE sp1p ;
									BYTE sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 128 : sp1[ 0 ] ;
									sp2p = offset0 + p + n >= SSampleNum ? 128 : sp2[ 0 ] ;
									if( offset1 + n < DSampleNum )
									{
										dp1[ 0 ] = ( sp1p * ( p - n ) + sp2p * n ) / p ;
									}
								}

								if( offset1 + p     < DSampleNum &&
									offset0 + p * 2 < SSampleNum )
								{
									q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
									sp1 = &SBuffer[ ( offset0 + p * 2 ) * 2 + 1 ] ;
									dp1 = &DBuffer[ ( offset1 + p     ) * 2 + 1 ] ;
									movnum = q - p ;
									if( offset0 + p * 2 + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p * 2 ) ;
										q = p + movnum ;
									}
									if( offset1 + p + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
									{
										BYTE sp1p ;

										sp1p = offset0 + p + n >= SSampleNum ? 128 : sp1[ 0 ] ;
										dp1[ 0 ] = sp1p ;
									}
								}
							}

							offset0 += p + q ;	// offset0�̍X�V
							offset1 += q ;		// offset1�̍X�V
						}
						if( offset1 < DSampleNum )
						{
							BYTE *dp ;
							int loopnum ;

							loopnum = DSampleNum - offset1 ;
							dp = &DBuffer[ ( offset1 * 2 ) + 1 ] ;
							for( n = 0 ; n < loopnum ; n ++, dp += 2 )
							{
								*dp = 128 ;
							}
						}
						break ;

					case 1 :
						offset0 = 0 ;
						offset1 = 0 ;
						while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								BYTE *sp1 ;
								BYTE *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + m ] ;
								for( n = 0 ; n < template_size ; n++, sp1 ++, sp2 ++ )
								{
									r += ( ( ( int )sp1[ 0 ] - 128 ) * ( int )( sp2[ 0 ] - 128 ) ) / 128 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								BYTE *sp1, *dp1 ;
								BYTE *sp2 ;
								int movnum ;

								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + p ] ;
								dp1 = &DBuffer[ offset1     ] ;
								for( n = 0; n < p ; n++, sp1 ++, sp2 ++, dp1 ++ )
								{
									dp1[ 0 ] = ( sp1[ 0 ] * ( p - n ) + sp2[ 0 ] * n ) / p ;
								}

								if( offset1 + p     < DSampleNum &&
									offset0 + p * 2 < SSampleNum )
								{
									q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
									sp1 = &SBuffer[ offset0 + p * 2 ] ;
									dp1 = &DBuffer[ offset1 + p     ] ;
									movnum = q - p ;
									if( offset0 + p * 2 + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p * 2 ) ;
										q = p + movnum ;
									}
									if( offset1 + p + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 ++, dp1 ++ )
									{
										dp1[ 0 ] = sp1[ 0 ] ;
									}
								}
							}

							offset0 += p + q ;	// offset0�̍X�V
							offset1 += q ;		// offset1�̍X�V
						}

						while( offset1 < DSampleNum && offset0 < SSampleNum )
						{
							max_of_r = 0 ;
							p = pmin ;
							for( m = pmin ; m <= pmax ; m++ )
							{
								BYTE *sp1 ;
								BYTE *sp2 ;

								r = 0 ;
								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + m ] ;
								for( n = 0 ; n < template_size ; n++, sp1 ++, sp2 ++ )
								{
									int sp1p ;
									int sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 0 : ( int )sp1[ 0 ] - 128 ;
									sp2p = offset0 + m + n >= SSampleNum ? 0 : ( int )sp2[ 0 ] - 128 ;
									r += ( sp1p * sp2p ) / 128 ; // ���֊֐�
								}
								if( r > max_of_r )
								{
									max_of_r = r ;	// ���֊֐��̃s�[�N
									p = m ;			// ���f�[�^�̊�{����
								}
							}

							{
								BYTE *sp1, *dp1 ;
								BYTE *sp2 ;
								int movnum ;

								sp1 = &SBuffer[ offset0     ] ;
								sp2 = &SBuffer[ offset0 + p ] ;
								dp1 = &DBuffer[ offset1     ] ;
								for( n = 0; n < p ; n++, sp1 ++, sp2 ++, dp1 ++ )
								{
									BYTE sp1p ;
									BYTE sp2p ;

									sp1p = offset0     + n >= SSampleNum ? 128 : sp1[ 0 ] ;
									sp2p = offset0 + p + n >= SSampleNum ? 128 : sp2[ 0 ] ;
									if( offset1 + n < DSampleNum )
									{
										dp1[ 0 ] = ( sp1p * ( p - n ) + sp2p * n ) / p ;
									}
								}

								if( offset1 + p     < DSampleNum &&
									offset0 + p * 2 < SSampleNum )
								{
									q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
									sp1 = &SBuffer[ offset0 + p * 2 ] ;
									dp1 = &DBuffer[ offset1 + p     ] ;
									movnum = q - p ;
									if( offset0 + p * 2 + movnum > SSampleNum )
									{
										movnum = SSampleNum - ( offset0 + p * 2 ) ;
										q = p + movnum ;
									}
									if( offset1 + p + movnum > DSampleNum )
									{
										movnum = DSampleNum - ( offset1 + p ) ;
										q = p + movnum ;
									}
									for( n = 0 ; n < movnum ; n++, sp1 ++, dp1 ++ )
									{
										BYTE sp1p ;

										sp1p = offset0 + p + n >= SSampleNum ? 128 : sp1[ 0 ] ;
										dp1[ 0 ] = sp1p ;
									}
								}
							}

							offset0 += p + q ;	// offset0�̍X�V
							offset1 += q ;		// offset1�̍X�V
						}
						if( offset1 < DSampleNum )
						{
							BYTE *dp ;
							int loopnum ;

							loopnum = DSampleNum - offset1 ;
							dp = &DBuffer[ offset1 ] ;
							for( n = 0 ; n < loopnum ; n ++, dp ++ )
							{
								*dp = 128 ;
							}
						}
						break ;
					}
				}
				break ;
			}
			break ;

		case WAVE_FORMAT_IEEE_FLOAT :
			{
				float *SBuffer ;
				float *DBuffer ;
				float r ;
				float max_of_r;

				SBuffer = ( float * )SSSound->Wave.Buffer ;
				DBuffer = ( float * )DSSound->Wave.Buffer ;

				switch( SSSound->BufferFormat.nChannels )
				{
				case 2 :
					offset0 = 0 ;
					offset1 = 0 ;
					while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
					{
						max_of_r = 0.0f ;
						p = pmin ;
						for( m = pmin ; m <= pmax ; m++ )
						{
							float *sp1 ;
							float *sp2 ;

							r = 0.0f ;
							sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
							sp2 = &SBuffer[ ( offset0 + m ) * 2 ] ;
							for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
							{
								r += sp1[ 0 ] * sp2[ 0 ] ; // ���֊֐�
							}
							if( r > max_of_r )
							{
								max_of_r = r ;	// ���֊֐��̃s�[�N
								p = m ;			// ���f�[�^�̊�{����
							}
						}

						{
							float *sp1, *dp1 ;
							float *sp2 ;
							int movnum ;

							sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
							sp2 = &SBuffer[ ( offset0 + p ) * 2 ] ;
							dp1 = &DBuffer[ ( offset1     ) * 2 ] ;
							for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2 )
							{
								dp1[ 0 ] = ( sp1[ 0 ] * ( p - n ) + sp2[ 0 ] * n ) / p ;
							}

							if( offset1 + p     < DSampleNum &&
								offset0 + p * 2 < SSampleNum )
							{
								q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
								sp1 = &SBuffer[ ( offset0 + p * 2 ) * 2 ] ;
								dp1 = &DBuffer[ ( offset1 + p     ) * 2 ] ;
								movnum = q - p ;
								if( offset0 + p * 2 + movnum > SSampleNum )
								{
									movnum = SSampleNum - ( offset0 + p * 2 ) ;
									q = p + movnum ;
								}
								if( offset1 + p + movnum > DSampleNum )
								{
									movnum = DSampleNum - ( offset1 + p ) ;
									q = p + movnum ;
								}
								for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
								{
									dp1[ 0 ] = sp1[ 0 ] ;
								}
							}
						}

						offset0 += p + q ;	// offset0�̍X�V
						offset1 += q ;		// offset1�̍X�V
					}

					while( offset1 < DSampleNum && offset0 < SSampleNum )
					{
						max_of_r = 0.0f ;
						p = pmin ;
						for( m = pmin ; m <= pmax ; m++ )
						{
							float *sp1 ;
							float *sp2 ;

							r = 0.0f ;
							sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
							sp2 = &SBuffer[ ( offset0 + m ) * 2 ] ;
							for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
							{
								float sp1p ;
								float sp2p ;

								sp1p = offset0     + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
								sp2p = offset0 + m + n >= SSampleNum ? 0.0f : sp2[ 0 ] ;
								r += sp1p * sp2p ; // ���֊֐�
							}
							if( r > max_of_r )
							{
								max_of_r = r ;	// ���֊֐��̃s�[�N
								p = m ;			// ���f�[�^�̊�{����
							}
						}

						{
							float *sp1, *dp1 ;
							float *sp2 ;
							int movnum ;

							sp1 = &SBuffer[ ( offset0     ) * 2 ] ;
							sp2 = &SBuffer[ ( offset0 + p ) * 2 ] ;
							dp1 = &DBuffer[ ( offset1     ) * 2 ] ;
							for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2 )
							{
								float sp1p ;
								float sp2p ;

								sp1p = offset0     + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
								sp2p = offset0 + p + n >= SSampleNum ? 0.0f : sp2[ 0 ] ;
								if( offset1 + n < DSampleNum )
								{
									dp1[ 0 ] = ( sp1p * ( p - n ) + sp2p * n ) / p ;
								}
							}

							if( offset1 + p     < DSampleNum &&
								offset0 + p * 2 < SSampleNum )
							{
								q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
								sp1 = &SBuffer[ ( offset0 + p * 2 ) * 2 ] ;
								dp1 = &DBuffer[ ( offset1 + p     ) * 2 ] ;
								movnum = q - p ;
								if( offset0 + p * 2 + movnum > SSampleNum )
								{
									movnum = SSampleNum - ( offset0 + p * 2 ) ;
									q = p + movnum ;
								}
								if( offset1 + p + movnum > DSampleNum )
								{
									movnum = DSampleNum - ( offset1 + p ) ;
									q = p + movnum ;
								}
								for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
								{
									float sp1p ;

									sp1p = offset0 + p + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
									dp1[ 0 ] = sp1p ;
								}
							}
						}

						offset0 += p + q ;	// offset0�̍X�V
						offset1 += q ;		// offset1�̍X�V
					}
					if( offset1 < DSampleNum )
					{
						float *dp ;
						int loopnum ;

						loopnum = DSampleNum - offset1 ;
						dp = &DBuffer[ offset1 * 2 ] ;
						for( n = 0 ; n < loopnum ; n ++, dp += 2 )
						{
							*dp = 0.0f ;
						}
					}



					offset0 = 0 ;
					offset1 = 0 ;
					while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
					{
						max_of_r = 0.0f ;
						p = pmin ;
						for( m = pmin ; m <= pmax ; m++ )
						{
							float *sp1 ;
							float *sp2 ;

							r = 0.0f ;
							sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
							sp2 = &SBuffer[ ( offset0 + m ) * 2 + 1 ] ;
							for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
							{
								r += sp1[ 0 ] * sp2[ 0 ] ; // ���֊֐�
							}
							if( r > max_of_r )
							{
								max_of_r = r ;	// ���֊֐��̃s�[�N
								p = m ;			// ���f�[�^�̊�{����
							}
						}

						{
							float *sp1, *dp1 ;
							float *sp2 ;
							int movnum ;

							sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
							sp2 = &SBuffer[ ( offset0 + p ) * 2 + 1 ] ;
							dp1 = &DBuffer[ ( offset1     ) * 2 + 1 ] ;
							for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2 )
							{
								dp1[ 0 ] = ( sp1[ 0 ] * ( p - n ) + sp2[ 0 ] * n ) / p ;
							}

							if( offset1 + p     < DSampleNum &&
								offset0 + p * 2 < SSampleNum )
							{
								q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
								sp1 = &SBuffer[ ( offset0 + p * 2 ) * 2 + 1 ] ;
								dp1 = &DBuffer[ ( offset1 + p     ) * 2 + 1 ] ;
								movnum = q - p ;
								if( offset0 + p * 2 + movnum > SSampleNum )
								{
									movnum = SSampleNum - ( offset0 + p * 2 ) ;
									q = p + movnum ;
								}
								if( offset1 + p + movnum > DSampleNum )
								{
									movnum = DSampleNum - ( offset1 + p ) ;
									q = p + movnum ;
								}
								for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
								{
									dp1[ 0 ] = sp1[ 0 ] ;
								}
							}
						}

						offset0 += p + q ;	// offset0�̍X�V
						offset1 += q ;		// offset1�̍X�V
					}

					while( offset1 < DSampleNum && offset0 < SSampleNum )
					{
						max_of_r = 0.0f ;
						p = pmin ;
						for( m = pmin ; m <= pmax ; m++ )
						{
							float *sp1 ;
							float *sp2 ;

							r = 0.0f ;
							sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
							sp2 = &SBuffer[ ( offset0 + m ) * 2 + 1 ] ;
							for( n = 0 ; n < template_size ; n++, sp1 += 2, sp2 += 2 )
							{
								float sp1p ;
								float sp2p ;

								sp1p = offset0     + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
								sp2p = offset0 + m + n >= SSampleNum ? 0.0f : sp2[ 0 ] ;
								r += sp1p * sp2p ; // ���֊֐�
							}
							if( r > max_of_r )
							{
								max_of_r = r ;	// ���֊֐��̃s�[�N
								p = m ;			// ���f�[�^�̊�{����
							}
						}

						{
							float *sp1, *dp1 ;
							float *sp2 ;
							int movnum ;

							sp1 = &SBuffer[ ( offset0     ) * 2 + 1 ] ;
							sp2 = &SBuffer[ ( offset0 + p ) * 2 + 1 ] ;
							dp1 = &DBuffer[ ( offset1     ) * 2 + 1 ] ;
							for( n = 0; n < p ; n++, sp1 += 2, sp2 += 2, dp1 += 2 )
							{
								float sp1p ;
								float sp2p ;

								sp1p = offset0     + n >= SSampleNum ? 0 : sp1[ 0 ] ;
								sp2p = offset0 + p + n >= SSampleNum ? 0 : sp2[ 0 ] ;
								if( offset1 + n < DSampleNum )
								{
									dp1[ 0 ] = ( sp1p * ( p - n ) + sp2p * n ) / p ;
								}
							}

							if( offset1 + p     < DSampleNum &&
								offset0 + p * 2 < SSampleNum )
							{
								q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
								sp1 = &SBuffer[ ( offset0 + p * 2 ) * 2 + 1 ] ;
								dp1 = &DBuffer[ ( offset1 + p     ) * 2 + 1 ] ;
								movnum = q - p ;
								if( offset0 + p * 2 + movnum > SSampleNum )
								{
									movnum = SSampleNum - ( offset0 + p * 2 ) ;
									q = p + movnum ;
								}
								if( offset1 + p + movnum > DSampleNum )
								{
									movnum = DSampleNum - ( offset1 + p ) ;
									q = p + movnum ;
								}
								for( n = 0 ; n < movnum ; n++, sp1 += 2, dp1 += 2 )
								{
									float sp1p ;

									sp1p = offset0 + p + n >= SSampleNum ? 0 : sp1[ 0 ] ;
									dp1[ 0 ] = sp1p ;
								}
							}
						}

						offset0 += p + q ;	// offset0�̍X�V
						offset1 += q ;		// offset1�̍X�V
					}
					if( offset1 < DSampleNum )
					{
						float *dp ;
						int loopnum ;

						loopnum = DSampleNum - offset1 ;
						dp = &DBuffer[ offset1 * 2 + 1 ] ;
						for( n = 0 ; n < loopnum ; n ++, dp += 2 )
						{
							*dp = 0.0f ;
						}
					}
					break ;

				case 1 :
					offset0 = 0 ;
					offset1 = 0 ;
					while( offset0 + pmax * 2 < SSampleNum && offset1 + pmax * 2 < DSampleNum )
					{
						max_of_r = 0.0f ;
						p = pmin ;
						for( m = pmin ; m <= pmax ; m++ )
						{
							float *sp1 ;
							float *sp2 ;

							r = 0.0f ;
							sp1 = &SBuffer[ offset0     ] ;
							sp2 = &SBuffer[ offset0 + m ] ;
							for( n = 0 ; n < template_size ; n++, sp1 ++, sp2 ++ )
							{
								r += sp1[ 0 ] * sp2[ 0 ] ; // ���֊֐�
							}
							if( r > max_of_r )
							{
								max_of_r = r ;	// ���֊֐��̃s�[�N
								p = m ;			// ���f�[�^�̊�{����
							}
						}

						{
							float *sp1, *dp1 ;
							float *sp2 ;
							int movnum ;

							sp1 = &SBuffer[ offset0     ] ;
							sp2 = &SBuffer[ offset0 + p ] ;
							dp1 = &DBuffer[ offset1     ] ;
							for( n = 0; n < p ; n++, sp1 ++, sp2 ++, dp1 ++ )
							{
								dp1[ 0 ] = ( sp1[ 0 ] * ( p - n ) + sp2[ 0 ] * n ) / p ;
							}

							if( offset1 + p     < DSampleNum &&
								offset0 + p * 2 < SSampleNum )
							{
								q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
								sp1 = &SBuffer[ offset0 + p * 2 ] ;
								dp1 = &DBuffer[ offset1 + p     ] ;
								movnum = q - p ;
								if( offset0 + p * 2 + movnum > SSampleNum )
								{
									movnum = SSampleNum - ( offset0 + p * 2 ) ;
									q = p + movnum ;
								}
								if( offset1 + p + movnum > DSampleNum )
								{
									movnum = DSampleNum - ( offset1 + p ) ;
									q = p + movnum ;
								}
								for( n = 0 ; n < movnum ; n++, sp1 ++, dp1 ++ )
								{
									dp1[ 0 ] = sp1[ 0 ] ;
								}
							}
						}

						offset0 += p + q ;	// offset0�̍X�V
						offset1 += q ;		// offset1�̍X�V
					}

					while( offset1 < DSampleNum && offset0 < SSampleNum )
					{
						max_of_r = 0.0f ;
						p = pmin ;
						for( m = pmin ; m <= pmax ; m++ )
						{
							float *sp1 ;
							float *sp2 ;

							r = 0.0f ;
							sp1 = &SBuffer[ offset0     ] ;
							sp2 = &SBuffer[ offset0 + m ] ;
							for( n = 0 ; n < template_size ; n++, sp1 ++, sp2 ++ )
							{
								float sp1p ;
								float sp2p ;

								sp1p = offset0     + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
								sp2p = offset0 + m + n >= SSampleNum ? 0.0f : sp2[ 0 ] ;
								r += sp1p * sp2p ; // ���֊֐�
							}
							if( r > max_of_r )
							{
								max_of_r = r ;	// ���֊֐��̃s�[�N
								p = m ;			// ���f�[�^�̊�{����
							}
						}

						{
							float *sp1, *dp1 ;
							float *sp2 ;
							int movnum ;

							sp1 = &SBuffer[ offset0     ] ;
							sp2 = &SBuffer[ offset0 + p ] ;
							dp1 = &DBuffer[ offset1     ] ;
							for( n = 0; n < p ; n++, sp1 ++, sp2 ++, dp1 ++ )
							{
								float sp1p ;
								float sp2p ;

								sp1p = offset0     + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
								sp2p = offset0 + p + n >= SSampleNum ? 0.0f : sp2[ 0 ] ;
								if( offset1 + n < DSampleNum )
								{
									dp1[ 0 ] = ( sp1p * ( p - n ) + sp2p * n ) / p ;
								}
							}

							if( offset1 + p     < DSampleNum &&
								offset0 + p * 2 < SSampleNum )
							{
								q = ( p * 512 * 512 / ( rateI - 512 ) + 256 ) / 512 ;
								sp1 = &SBuffer[ offset0 + p * 2 ] ;
								dp1 = &DBuffer[ offset1 + p     ] ;
								movnum = q - p ;
								if( offset0 + p * 2 + movnum > SSampleNum )
								{
									movnum = SSampleNum - ( offset0 + p * 2 ) ;
									q = p + movnum ;
								}
								if( offset1 + p + movnum > DSampleNum )
								{
									movnum = DSampleNum - ( offset1 + p ) ;
									q = p + movnum ;
								}
								for( n = 0 ; n < movnum ; n++, sp1 ++, dp1 ++ )
								{
									float sp1p ;

									sp1p = offset0 + p + n >= SSampleNum ? 0.0f : sp1[ 0 ] ;
									dp1[ 0 ] = sp1p ;
								}
							}
						}

						offset0 += p + q ;	// offset0�̍X�V
						offset1 += q ;		// offset1�̍X�V
					}
					if( offset1 < DSampleNum )
					{
						float *dp ;
						int loopnum ;

						loopnum = DSampleNum - offset1 ;
						dp = &DBuffer[ offset1 ] ;
						for( n = 0 ; n < loopnum ; n ++, dp ++ )
						{
							*dp = 0.0f ;
						}
					}
					break ;
				}
			}
			break ;
		}
	}


	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�n���h���̔g�`�f�[�^�̒�����ύX����
#define PITCHSHIFT_WIN_N			(128)
#define PITCHSHIFT_ANGLE_DIV_NUM	( ( PITCHSHIFT_WIN_N * 2 + 1 ) * 128 )
extern int NS_WritePitchShiftSoftSoundData( int SrcSoftSoundHandle, int DestSoftSoundHandle )
{
	SOFTSOUND * SSSound ;
	SOFTSOUND * DSSound ;
	int SSampleNum ;
	int DSampleNum ;
	int n, m, ta, tb ;
	int Start ;
	int End ;
	int T_M_I ;
	int mt, st ;
	int AngleI, SinCI, SinCosI, CosI ;

	// �G���[����
	if( SSND_MASKHCHK_ASYNC( SrcSoftSoundHandle, SSSound ) ) return -1 ;
	if( SSSound->IsPlayer == 1 ) return -1 ;

	if( SSND_MASKHCHK_ASYNC( DestSoftSoundHandle, DSSound ) ) return -1 ;
	if( DSSound->IsPlayer == 1 ) return -1 ;

	// �T���v�������ς��Ȃ��ꍇ�̓G���[
	if( SSSound->Wave.BufferSampleNum == DSSound->Wave.BufferSampleNum )
	{
		return -1 ;
	}

	// �t�H�[�}�b�g���قȂ�ꍇ�̓G���[
	if( SSSound->BufferFormat.wFormatTag     != DSSound->BufferFormat.wFormatTag ||
		SSSound->BufferFormat.nChannels      != DSSound->BufferFormat.nChannels  ||
		SSSound->BufferFormat.nSamplesPerSec != DSSound->BufferFormat.nSamplesPerSec ||
		SSSound->BufferFormat.wBitsPerSample != DSSound->BufferFormat.wBitsPerSample )
	{
		return -1 ;
	}

	SSampleNum = SSSound->Wave.BufferSampleNum ;
	DSampleNum = DSSound->Wave.BufferSampleNum ;

	mt = 0 ;
	st = 0 ;

	switch( SSSound->BufferFormat.wFormatTag )
	{
	case WAVE_FORMAT_PCM :
#if 1
		{
			// Float �^�C�v�ɕϊ����Ď��s����
			int TempFloatSSSHandle = -1 ;
			int TempFloatDSSHandle = -1 ;

			TempFloatSSSHandle = NS_MakeSoftSoundCustom( SSSound->BufferFormat.nChannels, 32, SSSound->BufferFormat.nSamplesPerSec, SSSound->Wave.BufferSampleNum, TRUE ) ;
			if( TempFloatSSSHandle < 0 )
			{
				return -1 ;
			}
			TempFloatDSSHandle = NS_MakeSoftSoundCustom( DSSound->BufferFormat.nChannels, 32, DSSound->BufferFormat.nSamplesPerSec, DSSound->Wave.BufferSampleNum, TRUE ) ;
			if( TempFloatDSSHandle < 0 )
			{
				NS_DeleteSoftSound( TempFloatSSSHandle ) ;
				TempFloatSSSHandle = -1 ;
				return -1 ;
			}

			ConvertIntToFloatSoftSound( SrcSoftSoundHandle, TempFloatSSSHandle ) ;
			NS_WritePitchShiftSoftSoundData( TempFloatSSSHandle, TempFloatDSSHandle ) ;
			ConvertFloatToIntSoftSound( TempFloatDSSHandle, DestSoftSoundHandle ) ;

			NS_DeleteSoftSound( TempFloatSSSHandle ) ;
			NS_DeleteSoftSound( TempFloatDSSHandle ) ;
			TempFloatSSSHandle = -1 ;
			TempFloatDSSHandle = -1 ;
		}
#else
		int DestInt ;
		switch( SSSound->BufferFormat.wBitsPerSample )
		{
		case 16 :
			{
				short *SBuffer ;
				short *DBuffer ;

				SBuffer = ( short * )SSSound->Wave.Buffer ;
				DBuffer = ( short * )DSSound->Wave.Buffer ;

				_MEMSET( DBuffer, 0, sizeof( short ) * SSSound->BufferFormat.nChannels * DSampleNum ) ;

				switch( SSSound->BufferFormat.nChannels )
				{
				case 1 :
					for( n = 0; n < DSampleNum ; n++ )
					{
						ta = mt ;
						tb = st == 0 ? ta : ta + 1 ;

						Start = tb - PITCHSHIFT_WIN_N / 2 ;
						End   = ta + PITCHSHIFT_WIN_N / 2 ;
						if( Start <           0 ) Start = 0 ;
						if( Start >= SSampleNum ) Start = SSampleNum - 1 ;
						if( End   <           0 ) End   = 0 ;
						if( End   >= SSampleNum ) End   = SSampleNum - 1 ;

						st += SSampleNum ;
						while( st >= DSampleNum )
						{
							st -= DSampleNum;
							mt++ ;
						}

						T_M_I = ( mt * 128 + st * 128 / DSampleNum ) - Start * 128 ;
						for( m = Start; m <= End; m++, T_M_I -= 128 )
						{
							AngleI = ( int )( DX_PI_F * 128.0f ) * T_M_I ;
							if( AngleI == 0 )
							{
								SinCI = 16384 ;
							}
							else
							{
								SinCI = SoundSysData.SinTable[ ( ( SOUND_SINTABLE_DIV / 2 ) * T_M_I / 128 ) & ( SOUND_SINTABLE_DIV - 1 ) ] * 16384 / AngleI ;
							}
							CosI = SoundSysData.SinTable[ ( ( SOUND_SINTABLE_DIV * T_M_I / PITCHSHIFT_ANGLE_DIV_NUM ) + SOUND_SINTABLE_DIV / 4 ) & ( SOUND_SINTABLE_DIV - 1 ) ] ;

							SinCosI = ( SinCI * ( 64 * 128 + ( 64 * 128 * CosI ) / 16384 ) ) / 16384 ;
							DestInt  = ( int )DBuffer[ n ] + ( ( int )SBuffer[ m ] * SinCosI ) / 16384 ;

							     if( DestInt < -32768 )	DBuffer[ n ] = -32768 ;
							else if( DestInt >  32767 )	DBuffer[ n ] =  32767 ;
							else						DBuffer[ n ] = ( short )DestInt ;
						}
					}
					break ;

				case 2 :
					for( n = 0; n < DSampleNum ; n++ )
					{
						ta = mt ;
						tb = st == 0 ? ta : ta + 1 ;

						Start = tb - PITCHSHIFT_WIN_N / 2 ;
						End   = ta + PITCHSHIFT_WIN_N / 2 ;
						if( Start <           0 ) Start = 0 ;
						if( Start >= SSampleNum ) Start = SSampleNum - 1 ;
						if( End   <           0 ) End   = 0 ;
						if( End   >= SSampleNum ) End   = SSampleNum - 1 ;

						st += SSampleNum ;
						while( st >= DSampleNum )
						{
							st -= DSampleNum;
							mt++ ;
						}

						T_M_I = ( mt * 128 + st * 128 / DSampleNum ) - Start * 128 ;
						for( m = Start; m <= End; m++, T_M_I -= 128 )
						{
							AngleI = ( int )( DX_PI_F * 128.0f ) * T_M_I ;
							if( AngleI == 0 )
							{
								SinCI = 16384 ;
							}
							else
							{
								SinCI = SoundSysData.SinTable[ ( ( SOUND_SINTABLE_DIV / 2 ) * T_M_I / 128 ) & ( SOUND_SINTABLE_DIV - 1 ) ] * 16384 / AngleI ;
							}
							CosI = SoundSysData.SinTable[ ( ( SOUND_SINTABLE_DIV * T_M_I / PITCHSHIFT_ANGLE_DIV_NUM ) + SOUND_SINTABLE_DIV / 4 ) & ( SOUND_SINTABLE_DIV - 1 ) ] ;

							SinCosI = ( SinCI * ( 64 * 128 + ( 64 * 128 * CosI ) / 16384 ) ) / 16384 ;

							DestInt  = ( int )DBuffer[ n * 2     ] + ( ( int )SBuffer[ m * 2     ] * SinCosI ) / 16384 ;
							     if( DestInt < -32768 )	DBuffer[ n * 2 ] = -32768 ;
							else if( DestInt >  32767 )	DBuffer[ n * 2 ] =  32767 ;
							else						DBuffer[ n * 2 ] = ( short )DestInt ;

							DestInt  = ( int )DBuffer[ n * 2 + 1 ] + ( ( int )SBuffer[ m * 2 + 1 ] * SinCosI ) / 16384 ;
							     if( DestInt < -32768 )	DBuffer[ n * 2 + 1 ] = -32768 ;
							else if( DestInt >  32767 )	DBuffer[ n * 2 + 1 ] =  32767 ;
							else						DBuffer[ n * 2 + 1 ] = ( short )DestInt ;
						}
					}
					break ;
				}
			}
			break ;

		case 8 :
			{
				BYTE *SBuffer ;
				BYTE *DBuffer ;

				SBuffer = ( BYTE * )SSSound->Wave.Buffer ;
				DBuffer = ( BYTE * )DSSound->Wave.Buffer ;

				_MEMSET( DBuffer, 127, sizeof( BYTE ) * DSSound->BufferFormat.nChannels * DSampleNum ) ;

				switch( SSSound->BufferFormat.nChannels )
				{
				case 1 :
					for( n = 0; n < DSampleNum ; n++ )
					{
						ta = mt ;
						tb = st == 0 ? ta : ta + 1 ;

						Start = tb - PITCHSHIFT_WIN_N / 2 ;
						End   = ta + PITCHSHIFT_WIN_N / 2 ;
						if( Start <           0 ) Start = 0 ;
						if( Start >= SSampleNum ) Start = SSampleNum - 1 ;
						if( End   <           0 ) End   = 0 ;
						if( End   >= SSampleNum ) End   = SSampleNum - 1 ;

						st += SSampleNum ;
						while( st >= DSampleNum )
						{
							st -= DSampleNum;
							mt++ ;
						}

						T_M_I = ( mt * 128 + st * 128 / DSampleNum ) - Start * 128 ;
						for( m = Start; m <= End; m++, T_M_I -= 128 )
						{
							AngleI = ( int )( DX_PI_F * 128.0f ) * T_M_I ;
							if( AngleI == 0 )
							{
								SinCI = 16384 ;
							}
							else
							{
								SinCI = SoundSysData.SinTable[ ( ( SOUND_SINTABLE_DIV / 2 ) * T_M_I / 128 ) & ( SOUND_SINTABLE_DIV - 1 ) ] * 16384 / AngleI ;
							}
							CosI = SoundSysData.SinTable[ ( ( SOUND_SINTABLE_DIV * T_M_I / PITCHSHIFT_ANGLE_DIV_NUM ) + SOUND_SINTABLE_DIV / 4 ) & ( SOUND_SINTABLE_DIV - 1 ) ] ;

							SinCosI = ( SinCI * ( 64 * 128 + ( 64 * 128 * CosI ) / 16384 ) ) / 16384 ;
							DestInt  = ( int )DBuffer[ n ] + ( ( ( int )SBuffer[ m ] - 127 ) * SinCosI ) / 16384 ;

							     if( DestInt <   0 )	DBuffer[ n ] = 0 ;
							else if( DestInt > 255 )	DBuffer[ n ] = 255 ;
							else						DBuffer[ n ] = ( BYTE )DestInt ;
						}
					}
					break ;

				case 2 :
					for( n = 0; n < DSampleNum ; n++ )
					{
						ta = mt ;
						tb = st == 0 ? ta : ta + 1 ;

						Start = tb - PITCHSHIFT_WIN_N / 2 ;
						End   = ta + PITCHSHIFT_WIN_N / 2 ;
						if( Start <           0 ) Start = 0 ;
						if( Start >= SSampleNum ) Start = SSampleNum - 1 ;
						if( End   <           0 ) End   = 0 ;
						if( End   >= SSampleNum ) End   = SSampleNum - 1 ;

						st += SSampleNum ;
						while( st >= DSampleNum )
						{
							st -= DSampleNum;
							mt++ ;
						}

						T_M_I = ( mt * 128 + st * 128 / DSampleNum ) - Start * 128 ;
						for( m = Start; m <= End; m++, T_M_I -= 128 )
						{
							AngleI = ( int )( DX_PI_F * 128.0f ) * T_M_I ;
							if( AngleI == 0 )
							{
								SinCI = 16384 ;
							}
							else
							{
								SinCI = SoundSysData.SinTable[ ( ( SOUND_SINTABLE_DIV / 2 ) * T_M_I / 128 ) & ( SOUND_SINTABLE_DIV - 1 ) ] * 16384 / AngleI ;
							}
							CosI = SoundSysData.SinTable[ ( ( SOUND_SINTABLE_DIV * T_M_I / PITCHSHIFT_ANGLE_DIV_NUM ) + SOUND_SINTABLE_DIV / 4 ) & ( SOUND_SINTABLE_DIV - 1 ) ] ;

							SinCosI = ( SinCI * ( 64 * 128 + ( 64 * 128 * CosI ) / 16384 ) ) / 16384 ;

							DestInt  = ( int )DBuffer[ n * 2     ] + ( ( ( int )SBuffer[ m * 2     ] - 127 ) * SinCosI ) / 16384 ;
							     if( DestInt <   0 )	DBuffer[ n * 2 ] = 0 ;
							else if( DestInt > 255 )	DBuffer[ n * 2 ] = 255 ;
							else						DBuffer[ n * 2 ] = ( BYTE )DestInt ;

							DestInt  = ( int )DBuffer[ n * 2 + 1 ] + ( ( ( int )SBuffer[ m * 2 + 1 ] - 127 ) * SinCosI ) / 16384 ;
							     if( DestInt <   0 )	DBuffer[ n * 2 + 1 ] = 0 ;
							else if( DestInt > 255 )	DBuffer[ n * 2 + 1 ] = 255 ;
							else						DBuffer[ n * 2 + 1 ] = ( BYTE )DestInt ;
						}
					}
					break ;
				}
			}
			break ;
		}
#endif
		break ;

	case WAVE_FORMAT_IEEE_FLOAT :
		{
			float *SBuffer ;
			float *DBuffer ;

			SBuffer = ( float * )SSSound->Wave.Buffer ;
			DBuffer = ( float * )DSSound->Wave.Buffer ;

			_MEMSET( DBuffer, 0, sizeof( float ) * SSSound->BufferFormat.nChannels * DSampleNum ) ;

			switch( SSSound->BufferFormat.nChannels )
			{
			case 1 :
				for( n = 0; n < DSampleNum ; n++ )
				{
					ta = mt ;
					tb = st == 0 ? ta : ta + 1 ;

					Start = tb - PITCHSHIFT_WIN_N / 2 ;
					End   = ta + PITCHSHIFT_WIN_N / 2 ;
					if( Start <           0 ) Start = 0 ;
					if( Start >= SSampleNum ) Start = SSampleNum - 1 ;
					if( End   <           0 ) End   = 0 ;
					if( End   >= SSampleNum ) End   = SSampleNum - 1 ;

					st += SSampleNum ;
					while( st >= DSampleNum )
					{
						st -= DSampleNum;
						mt++ ;
					}

					T_M_I = ( mt * 128 + st * 128 / DSampleNum ) - Start * 128 ;
					for( m = Start; m <= End; m++, T_M_I -= 128 )
					{
						AngleI = ( int )( DX_PI_F * 128.0f ) * T_M_I ;
						if( AngleI == 0 )
						{
							SinCI = 16384 ;
						}
						else
						{
							SinCI = SoundSysData.SinTable[ ( ( SOUND_SINTABLE_DIV / 2 ) * T_M_I / 128 ) & ( SOUND_SINTABLE_DIV - 1 ) ] * 16384 / AngleI ;
						}
						CosI = SoundSysData.SinTable[ ( ( SOUND_SINTABLE_DIV * T_M_I / PITCHSHIFT_ANGLE_DIV_NUM ) + SOUND_SINTABLE_DIV / 4 ) & ( SOUND_SINTABLE_DIV - 1 ) ] ;

						SinCosI = ( SinCI * ( 64 * 128 + ( 64 * 128 * CosI ) / 16384 ) ) / 16384 ;
						DBuffer[ n ] += ( SBuffer[ m ] * SinCosI ) / 16384.0f ;
					}
				}
				break ;

			case 2 :
				for( n = 0; n < DSampleNum ; n++ )
				{
					ta = mt ;
					tb = st == 0 ? ta : ta + 1 ;

					Start = tb - PITCHSHIFT_WIN_N / 2 ;
					End   = ta + PITCHSHIFT_WIN_N / 2 ;
					if( Start <           0 ) Start = 0 ;
					if( Start >= SSampleNum ) Start = SSampleNum - 1 ;
					if( End   <           0 ) End   = 0 ;
					if( End   >= SSampleNum ) End   = SSampleNum - 1 ;

					st += SSampleNum ;
					while( st >= DSampleNum )
					{
						st -= DSampleNum;
						mt++ ;
					}

					T_M_I = ( mt * 128 + st * 128 / DSampleNum ) - Start * 128 ;
					for( m = Start; m <= End; m++, T_M_I -= 128 )
					{
						AngleI = ( int )( DX_PI_F * 128.0f ) * T_M_I ;
						if( AngleI == 0 )
						{
							SinCI = 16384 ;
						}
						else
						{
							SinCI = SoundSysData.SinTable[ ( ( SOUND_SINTABLE_DIV / 2 ) * T_M_I / 128 ) & ( SOUND_SINTABLE_DIV - 1 ) ] * 16384 / AngleI ;
						}
						CosI = SoundSysData.SinTable[ ( ( SOUND_SINTABLE_DIV * T_M_I / PITCHSHIFT_ANGLE_DIV_NUM ) + SOUND_SINTABLE_DIV / 4 ) & ( SOUND_SINTABLE_DIV - 1 ) ] ;

						SinCosI = ( SinCI * ( 64 * 128 + ( 64 * 128 * CosI ) / 16384 ) ) / 16384 ;

						DBuffer[ n * 2     ] += ( SBuffer[ m * 2     ] * SinCosI ) / 16384.0f ;
						DBuffer[ n * 2 + 1 ] += ( SBuffer[ m * 2 + 1 ] * SinCosI ) / 16384.0f ;
					}
				}
				break ;
			}
		}
		break ;
	}

	// ����I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̔g�`�C���[�W���i�[����Ă��郁�����A�h���X���擾����
extern	void *NS_GetSoftSoundDataImage( int SoftSoundHandle )
{
	SOFTSOUND * SSound ;

	// �G���[����
	if( SSND_MASKHCHK_ASYNC( SoftSoundHandle, SSound ) ) return NULL ;
	if( SSound->IsPlayer == 1 ) return NULL ;

	// �I��
	return SSound->Wave.Buffer ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�n���h���̎w��͈̔͂������t�[���G�ϊ����s���A�e���g����̐U�����擾����( SampleNum �� 256, 512, 1024, 2048, 4096, 8192, 16384 �̉��ꂩ�ł���K�v������܂��AChannel �� -1 �ɂ���Ɠ�̃`�����l���������������ʂɂȂ�܂� )
extern int NS_GetFFTVibrationSoftSound( int SoftSoundHandle, int Channel, int SamplePosition, int SampleNum, float *Buffer, int BufferLength )
{
	SOFTSOUND * SSound ;
	float ( *Real_Imag )[ 2 ] ;
	float *TempBuffer ;
	int i, stage, number_of_stage, *index = NULL ;

	// �G���[����
	if( SSND_MASKHCHK_ASYNC( SoftSoundHandle, SSound ) ) return -1 ;
	if( SSound->IsPlayer == 1 ) return -1 ;

	// �z���������
	_MEMSET( Buffer, 0, sizeof( float ) * BufferLength ) ;

	// �����`�F�b�N
	if( SampleNum != 256 &&
		SampleNum != 512 &&
		SampleNum != 1024 &&
		SampleNum != 2048 &&
		SampleNum != 4096 &&
		SampleNum != 8192 &&
		SampleNum != 16384 )
	{
		return -1 ;
	}
	if( SamplePosition >= SSound->Wave.BufferSampleNum )
	{
		return -1 ;
	}
	if( SamplePosition < 0 )
	{
		SamplePosition = 0 ;
	}
	if( SamplePosition + SampleNum > SSound->Wave.BufferSampleNum )
	{
		while( SampleNum > 256 && SamplePosition + SampleNum > SSound->Wave.BufferSampleNum )
		{
			SampleNum /= 2 ;
		}
		if( SampleNum <= 256 && SamplePosition + SampleNum > SSound->Wave.BufferSampleNum )
		{
			return -1 ;
		}
	}
	if( Channel > 1 )
	{
		return -1 ;
	}
	if( SSound->BufferFormat.nChannels == 1 && Channel < 0 )
	{
		Channel = 0 ;
	}

	// ���������m��
	Real_Imag = ( float (*)[ 2 ] )DXALLOC( SampleNum * 2 * sizeof( float ) + SampleNum / 2 * sizeof( float ) ) ;
	if( Real_Imag == NULL )
	{
		return -1 ;
	}
	TempBuffer = ( float * )Real_Imag + SampleNum * 2 ;
	_MEMSET( Real_Imag, 0, sizeof( float ) * SampleNum * 2 ) ;

	// �ϊ����f�[�^�̏���
	{
		BYTE *Src ;

		Src = ( BYTE * )SSound->Wave.Buffer + SSound->BufferFormat.nBlockAlign * SamplePosition ;
		if( Channel < 0 )
		{
			switch( SSound->BufferFormat.wBitsPerSample )
			{
			case 8 :
				for( i = 0 ; i < SampleNum ; i ++, Src += SSound->BufferFormat.nBlockAlign )
				{
					Real_Imag[ i ][ 0 ] = ( ( ( float )Src[ 0 ] - 127.0f ) + ( ( float )Src[ 1 ] - 127.0f ) ) / 256.0f ;
				}
				break ;

			case 16 :
				for( i = 0 ; i < SampleNum ; i ++, Src += SSound->BufferFormat.nBlockAlign )
				{
					Real_Imag[ i ][ 0 ] = ( ( float )( ( ( short * )Src )[ 0 ] ) + ( float )( ( ( short * )Src )[ 1 ] ) ) / 65536.0f ;
				}
				break ;
			}
		}
		else
		{
			switch( SSound->BufferFormat.wBitsPerSample )
			{
			case 8 :
				for( i = 0 ; i < SampleNum ; i ++, Src += SSound->BufferFormat.nBlockAlign )
				{
					Real_Imag[ i ][ 0 ] = ( ( float )Src[ Channel ] - 127.0f ) / 128.0f ;
				}
				break ;

			case 16 :
				for( i = 0 ; i < SampleNum ; i ++, Src += SSound->BufferFormat.nBlockAlign )
				{
					Real_Imag[ i ][ 0 ] = ( float )( ( ( short * )Src )[ Channel ] ) / 32768.0f ;
				}
				break ;
			}
		}
	}

	// FFT�̒i���Z�o
	{
		int x = SampleNum ;

		for( number_of_stage = 0 ; x > 1 ; number_of_stage ++ )
		{
			x >>= 1 ;
		}
	}

	// �o�^�t���C�v�Z
	for( stage = 1 ; stage <= number_of_stage ; stage++ )
	{
		int loopnum1 ;
		int baseindex_m ;
		int baseindex_r ;

		baseindex_m = 1 << ( number_of_stage - stage ) ;
		baseindex_r = 1 << ( stage - 1 ) ;

		loopnum1 = 1 << ( stage - 1 ) ;
		for( i = 0 ; i < loopnum1 ; i++ )
		{
			int loopnum2 ;
			int baseindex_n ;
			float a_real, a_imag, b_real, b_imag ;
			int j, n, m, r ;

			baseindex_n = ( 1 << ( number_of_stage - stage + 1 ) ) * i ;

			loopnum2 = 1 << ( number_of_stage - stage ) ;
			if( stage < number_of_stage )
			{
				for( j = 0 ; j < loopnum2 ; j++ )
				{
					float Sin, Cos ;
					float c_real, c_imag ;

					n = baseindex_n + j ;
					m = baseindex_m + n ;
					r = baseindex_r * j ;

					a_real = Real_Imag[ n ][ 0 ] ;
					a_imag = Real_Imag[ n ][ 1 ] ;
					b_real = Real_Imag[ m ][ 0 ] ;
					b_imag = Real_Imag[ m ][ 1 ] ;

					_SINCOS_PLATFORM( ( 2.0f * DX_PI_F * r ) / SampleNum, &Sin, &Cos ) ;
					c_real =  Cos ;
					c_imag = -Sin ;

					Real_Imag[ n ][ 0 ] = a_real + b_real ;
					Real_Imag[ n ][ 1 ] = a_imag + b_imag ;
					Real_Imag[ m ][ 0 ] = ( a_real - b_real ) * c_real - ( a_imag - b_imag ) * c_imag ;
					Real_Imag[ m ][ 1 ] = ( a_imag - b_imag ) * c_real + ( a_real - b_real ) * c_imag ;
				}
			}
			else
			{
				for( j = 0 ; j < loopnum2 ; j++ )
				{
					n = baseindex_n + j ;
					m = baseindex_m + n ;

					a_real = Real_Imag[ n ][ 0 ] ;
					a_imag = Real_Imag[ n ][ 1 ] ;
					b_real = Real_Imag[ m ][ 0 ] ;
					b_imag = Real_Imag[ m ][ 1 ] ;

					Real_Imag[ n ][ 0 ] = a_real + b_real ;
					Real_Imag[ n ][ 1 ] = a_imag + b_imag ;
					Real_Imag[ m ][ 0 ] = a_real - b_real ;
					Real_Imag[ m ][ 1 ] = a_imag - b_imag ;
				}
			}
		}
	}

	// �Q�Ɨp�C���f�b�N�X���X�g���쐬
	{
		static volatile int SetupIndexTableFlag[ 15 ] ;
		static int IndexTable14[ 16384 ] ;
		static int IndexTable13[ 8192 ] ;
		static int IndexTable12[ 4096 ] ;
		static int IndexTable11[ 2048 ] ;
		static int IndexTable10[ 1024 ] ;
		static int IndexTable9[ 512 ] ;
		static int IndexTable8[ 256 ] ;
		switch( number_of_stage )
		{
		case 8 :	index = IndexTable8 ;	break ;
		case 9 :	index = IndexTable9 ;	break ;
		case 10 :	index = IndexTable10 ;	break ;
		case 11 :	index = IndexTable11 ;	break ;
		case 12 :	index = IndexTable12 ;	break ;
		case 13 :	index = IndexTable13 ;	break ;
		case 14 :	index = IndexTable14 ;	break ;
		}
		if( SetupIndexTableFlag[ number_of_stage ] == 0 )
		{
			for( stage = 1 ; stage <= number_of_stage ; stage ++ )
			{
				int loopnum ;
				int offset ;

				loopnum = 1 << ( stage - 1 ) ;
				offset  = 1 << ( number_of_stage - stage ) ;
				for( i = 0; i < loopnum ; i++ )
				{
					index[ loopnum + i ] = index[ i ] + offset ;
				}
			}

			SetupIndexTableFlag[ number_of_stage ] = 1 ;
		}
	}

	// �U���l�ɕϊ�
	{
		int loopnum ;
		float DivNum ;
		float RealT ;
		float ImagT ;

		loopnum = SampleNum / 2 ;
		DivNum = ( float )SampleNum / 2 ;
		for( i = 0; i < loopnum ; i++ )
		{
			RealT = Real_Imag[ index[ i ] ][ 0 ] / DivNum ;
			ImagT = Real_Imag[ index[ i ] ][ 1 ] / DivNum ;
			TempBuffer[ i ] = _SQRT( RealT * RealT + ImagT * ImagT ) ;
		}
	}

	// �o��
	if( SampleNum / 2 == BufferLength )
	{
		for( i = 0 ; i < BufferLength ; i ++ )
		{
			Buffer[ i ] = TempBuffer[ i ] ;
		}
	}
	else
	{
		float OneBufferNum ;
		int SrcOffset ;
		float Src ;
		float SrcRate ;
		float SetBufferNum ;

		OneBufferNum = ( float )( SampleNum / 2 ) / BufferLength ;

		SrcOffset = 0 ;
		Src = 0.0f ;
		SrcRate = 0.0f ;
		for( i = 0 ; i < BufferLength ; i ++ )
		{
			SetBufferNum = OneBufferNum ;
			while( SetBufferNum > 0.0000001f )
			{
				if( SrcRate < 0.0000001f )
				{
					Src = TempBuffer[ SrcOffset ] ;
					SrcRate += 1.0f ;
					SrcOffset ++ ;
				}

				if( SetBufferNum >= SrcRate )
				{
					Buffer[ i ] += Src * SrcRate ;
					SetBufferNum -= SrcRate ;
					SrcRate = 0.0f ;
				}
				else
				{
					Buffer[ i ] += Src * SetBufferNum ;
					SrcRate -= SetBufferNum ;
					SetBufferNum = 0.0f ;
				}
			}
			Buffer[ i ] /= OneBufferNum ;
		}
	}

	// �������̊J��
	DXFREE( Real_Imag ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�����ׂĉ������
extern	int NS_InitSoftSoundPlayer( void )
{
	if( SoundSysData.InitializeFlag == FALSE )
		return -1 ;

	return AllHandleSub( DX_HANDLETYPE_SOFTSOUND, DeleteCancelCheckSoftSoundPlayerFunction );
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[���쐬����( �t�H�[�}�b�g�͈����̃\�t�g�E�G�A�T�E���h�n���h���Ɠ������̂ɂ��� )
extern int NS_MakeSoftSoundPlayer( int UseFormat_SoftSoundHandle )
{
	if( UseFormat_SoftSoundHandle < 0 )
		return -1 ;

	return MakeSoftSoundBase_UseGParam( TRUE, 0, 0, 0, 0, FALSE, UseFormat_SoftSoundHandle, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[���쐬����( �`�����l����:2 �ʎq���r�b�g��:16bit �T���v�����O���g��:44KHz )
extern	int NS_MakeSoftSoundPlayer2Ch16Bit44KHz( void )
{
	return MakeSoftSoundBase_UseGParam( TRUE, 2, 16, 44100, 0, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[���쐬����( �`�����l����:2 �ʎq���r�b�g��:16bit �T���v�����O���g��:22KHz )
extern	int NS_MakeSoftSoundPlayer2Ch16Bit22KHz( void )
{
	return MakeSoftSoundBase_UseGParam( TRUE, 2, 16, 22050, 0, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[���쐬����( �`�����l����:2 �ʎq���r�b�g��:8bit �T���v�����O���g��:44KHz )
extern	int NS_MakeSoftSoundPlayer2Ch8Bit44KHz( void )
{
	return MakeSoftSoundBase_UseGParam( TRUE, 2, 8, 44100, 0, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[���쐬����( �`�����l����:2 �ʎq���r�b�g��:8bit �T���v�����O���g��:22KHz )
extern	int NS_MakeSoftSoundPlayer2Ch8Bit22KHz( void )
{
	return MakeSoftSoundBase_UseGParam( TRUE, 2, 8, 22050, 0, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[���쐬����( �`�����l����:1 �ʎq���r�b�g��:16bit �T���v�����O���g��:44KHz )
extern	int NS_MakeSoftSoundPlayer1Ch16Bit44KHz( void )
{
	return MakeSoftSoundBase_UseGParam( TRUE, 1, 16, 44100, 0, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[���쐬����( �`�����l����:1 �ʎq���r�b�g��:16bit �T���v�����O���g��:22KHz )
extern	int NS_MakeSoftSoundPlayer1Ch16Bit22KHz( void )
{
	return MakeSoftSoundBase_UseGParam( TRUE, 1, 16, 22050, 0, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[���쐬����( �`�����l����:1 �ʎq���r�b�g��:8bit �T���v�����O���g��:44KHz )
extern	int NS_MakeSoftSoundPlayer1Ch8Bit44KHz( void )
{
	return MakeSoftSoundBase_UseGParam( TRUE, 1, 8, 44100, 0, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[���쐬����( �`�����l����:1 �ʎq���r�b�g��:8bit �T���v�����O���g��:22KHz )
extern	int NS_MakeSoftSoundPlayer1Ch8Bit22KHz( void )
{
	return MakeSoftSoundBase_UseGParam( TRUE, 1, 8, 22050, 0, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[���쐬����
extern	int NS_MakeSoftSoundPlayerCustom( int ChannelNum, int BitsPerSample, int SamplesPerSec )
{
	return MakeSoftSoundBase_UseGParam( TRUE, ChannelNum, BitsPerSample, SamplesPerSec, 0, FALSE, -1, GetASyncLoadFlag() ) ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[���폜����
extern	int NS_DeleteSoftSoundPlayer( int SSoundPlayerHandle )
{
	SOFTSOUND *SSound ;

	if( SSND_MASKHCHK_ASYNC( SSoundPlayerHandle, SSound ) ) return -1 ;

	// �v���C���[���ǂ����̃`�F�b�N
	if( SSound->IsPlayer != TRUE )
	{
		// �������G���[
		return -1 ;
	}

	return SubHandle( SSoundPlayerHandle ) ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�ɔg�`�f�[�^��ǉ�����( �t�H�[�}�b�g�������ł͂Ȃ��ꍇ�̓G���[ )
extern	int NS_AddDataSoftSoundPlayer( int SSoundPlayerHandle, int SoftSoundHandle, int AddSamplePosition, int AddSampleNum )
{
	SOFTSOUND *SPlayer, *SSound ;
	void *Src ;

	if( SoundSysData.InitializeFlag == FALSE ) return -1 ;

	// �G���[����
	if( SSND_MASKHCHK( SoftSoundHandle, SSound ) ) return -1 ;
	if( SSound->IsPlayer == 1 ) return -1 ;

	if( SSND_MASKHCHK( SSoundPlayerHandle, SPlayer ) ) return -1 ;
	if( SPlayer->IsPlayer == 0 ) return -1 ;

	// �t�H�[�}�b�g���������G���[
	if( SSound->BufferFormat.nChannels != SPlayer->BufferFormat.nChannels ||
		SSound->BufferFormat.wBitsPerSample != SPlayer->BufferFormat.wBitsPerSample ||
		SSound->BufferFormat.nSamplesPerSec != SPlayer->BufferFormat.nSamplesPerSec )
		return -1 ;

	// �T���v���ʒu�̎w�肪�Ԉ���Ă�����G���[
	if( AddSampleNum == 0 || SSound->Wave.BufferSampleNum > AddSamplePosition + AddSampleNum )
		return -1 ; 

	// �]���ʒu�̃A�h���X���Z�o
	Src = ( BYTE * )SSound->Wave.Buffer + SSound->BufferFormat.nBlockAlign * AddSamplePosition ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �����O�o�b�t�@�Ƀf�[�^��ǉ�
	if( RingBufDataAdd( &SPlayer->Player.StockSample, Src, AddSampleNum * SSound->BufferFormat.nBlockAlign ) < 0 )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;
		return -1 ;
	}

	// �ǉ��������T���v�����𑝂₷
	SPlayer->Player.StockSampleNum += AddSampleNum ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�Ƀv���C���[���Ή������t�H�[�}�b�g�̐��g�`�f�[�^��ǉ�����
extern	int	NS_AddDirectDataSoftSoundPlayer( int SSoundPlayerHandle, const void *SoundData, int AddSampleNum )
{
	SOFTSOUND * SPlayer ;

	if( SoundSysData.InitializeFlag == FALSE ) return -1 ;

	// �G���[����
	if( SSND_MASKHCHK( SSoundPlayerHandle, SPlayer ) ) return -1 ;
	if( SPlayer->IsPlayer == 0 ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �����O�o�b�t�@�Ƀf�[�^��ǉ�
	if( RingBufDataAdd( &SPlayer->Player.StockSample, SoundData, AddSampleNum * SPlayer->BufferFormat.nBlockAlign ) < 0 )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;
		return -1 ;
	}

	// �ǉ��������T���v�����𑝂₷
	SPlayer->Player.StockSampleNum += AddSampleNum ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�ɔg�`�f�[�^����ǉ�����
extern int NS_AddOneDataSoftSoundPlayer( int SSoundPlayerHandle, int Channel1, int Channel2 )
{
	SOFTSOUND * SPlayer ;
	BYTE Dest[ 16 ] ;

	if( SoundSysData.InitializeFlag == FALSE ) return -1 ;

	// �G���[����
	if( SSND_MASKHCHK( SSoundPlayerHandle, SPlayer ) ) return -1 ;
	if( SPlayer->IsPlayer == 0 ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �f�[�^�̏���
	switch( SPlayer->BufferFormat.wBitsPerSample )
	{
	case 8 :
		     if( Channel1 > 255 ) Channel1 = 255 ;
		else if( Channel1 <   0 ) Channel1 =   0 ;
		Dest[ 0 ] = ( BYTE )Channel1 ;

		if( SPlayer->BufferFormat.nChannels != 1 )
		{
				 if( Channel2 > 255 ) Channel2 = 255 ;
			else if( Channel2 <   0 ) Channel2 =   0 ;
			Dest[ 1 ] = ( BYTE )Channel2 ;
		}
		break ;

	case 16 :
			 if( Channel1 >  32767 ) Channel1 =  32767 ;
		else if( Channel1 < -32768 ) Channel1 = -32768 ;
		( ( short * )Dest )[ 0 ] = ( short )Channel1 ;

		if( SPlayer->BufferFormat.nChannels != 1 )
		{
				 if( Channel2 >  32767 ) Channel2 =  32767 ;
			else if( Channel2 < -32768 ) Channel2 = -32768 ;
			( ( short * )Dest )[ 1 ] = ( short )Channel2 ;
		}
		break ;
	}

	// �����O�o�b�t�@�Ƀf�[�^��ǉ�
	if( RingBufDataAdd( &SPlayer->Player.StockSample, Dest, SPlayer->BufferFormat.nBlockAlign ) < 0 )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;
		return -1 ;
	}

	// �T���v����������₷
	SPlayer->Player.StockSampleNum ++ ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �I��
	return 0 ;
}


// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�ɍĐ��p�T�E���h�o�b�t�@�ɓ]�����Ă��Ȃ��g�`�f�[�^�������A�Đ��p�T�E���h�o�b�t�@�ɂ������f�[�^�ȊO�������ǂ������擾����( TRUE:�����f�[�^�ȊO����  FALSE:�L���f�[�^������ )
extern	int	NS_CheckSoftSoundPlayerNoneData( int SSoundPlayerHandle )
{
	SOFTSOUND * SPlayer ;
	int Result = -1 ;

	if( SoundSysData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �G���[����
	if( SSND_MASKHCHK( SSoundPlayerHandle, SPlayer ) || SPlayer->IsPlayer == 0 )
		goto END ;

	// �X�V�������s��
	_SoftSoundPlayerProcess( SPlayer ) ;

	// �����f�[�^�Đ������ǂ����̃t���O���Z�b�g
	Result = SPlayer->Player.NoneDataPlayStartFlag ;

	// ���f�[�^�̃X�g�b�N������ꍇ�͖����f�[�^�Đ����ł͂Ȃ�
	if( SPlayer->Player.StockSampleNum != 0 )
		Result = FALSE ;

END :
	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// ��Ԃ�Ԃ�
	return Result ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�ɒǉ������g�`�f�[�^�ł܂��Đ��p�T�E���h�o�b�t�@�ɓ]������Ă��Ȃ��g�`�f�[�^�̃T���v�������擾����
extern	int	NS_GetStockDataLengthSoftSoundPlayer( int SSoundPlayerHandle )
{
	SOFTSOUND * SPlayer ;

	// �G���[����
	if( SSND_MASKHCHK( SSoundPlayerHandle, SPlayer ) || SPlayer->IsPlayer == 0 )
		return -1 ;

	// �����O�o�b�t�@�ɂ��関�]�����̃T���v����Ԃ�
	return SPlayer->Player.StockSampleNum ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�������f�[�^�t�H�[�}�b�g���擾����
extern	int	NS_GetSoftSoundPlayerFormat( int SSoundPlayerHandle, int *Channels, int *BitsPerSample, int *SamplesPerSec )
{
	SOFTSOUND * SPlayer ;

	// �G���[����
	if( SSND_MASKHCHK( SSoundPlayerHandle, SPlayer ) ) return -1 ;
	if( SPlayer->IsPlayer == 0 ) return -1 ;

	// �f�[�^���Z�b�g
	if( Channels      ) *Channels      = SPlayer->BufferFormat.nChannels ;
	if( BitsPerSample ) *BitsPerSample = SPlayer->BufferFormat.wBitsPerSample ;
	if( SamplesPerSec ) *SamplesPerSec = ( int )SPlayer->BufferFormat.nSamplesPerSec ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�̍Đ��������J�n����
extern	int NS_StartSoftSoundPlayer( int SSoundPlayerHandle )
{
	SOFTSOUND * SPlayer ;

	if( SoundSysData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �G���[����
	if( SSND_MASKHCHK( SSoundPlayerHandle, SPlayer ) || SPlayer->IsPlayer == 0 )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

		return -1 ;
	}

	// ���ɍĐ�����Ă����牽�����Ȃ�
	if( SPlayer->Player.IsPlayFlag )
		goto END ;

	// �Đ���Ԃɂ���
	SPlayer->Player.IsPlayFlag = TRUE ;

	// �Đ��p�f�[�^���T�E���h�o�b�t�@�ɃZ�b�g����
	_SoftSoundPlayerProcess( SPlayer ) ;

	// �T�E���h�o�b�t�@�̍Đ��J�n
	SoundBuffer_Play( &SPlayer->Player.SoundBuffer, TRUE ) ;

END :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�̍Đ��������J�n����Ă��邩�擾����( TRUE:�J�n���Ă���  FALSE:��~���Ă��� )
extern	int	NS_CheckStartSoftSoundPlayer( int SSoundPlayerHandle )
{
	SOFTSOUND * SPlayer ;

	// �G���[����
	if( SSND_MASKHCHK( SSoundPlayerHandle, SPlayer ) || SPlayer->IsPlayer == 0 )
		return -1 ;

	// �Đ����t���O��Ԃ�
	return SPlayer->Player.IsPlayFlag ;
}


// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�̍Đ��������~����
extern	int NS_StopSoftSoundPlayer( int SSoundPlayerHandle )
{
	SOFTSOUND * SPlayer ;

	if( SoundSysData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �G���[����
	if( SSND_MASKHCHK( SSoundPlayerHandle, SPlayer ) || SPlayer->IsPlayer == 0 )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

		return -1 ;
	}

	// �Đ�����Ă��Ȃ������牽�����Ȃ�
	if( SPlayer->Player.IsPlayFlag == FALSE )
		goto END ;

	// �Đ����~�߂�
	SoundBuffer_Stop( &SPlayer->Player.SoundBuffer, TRUE ) ;

	// �Đ����t���O��|��
	SPlayer->Player.IsPlayFlag = FALSE ;

END :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�̏�Ԃ�������Ԃɖ߂�( �ǉ����ꂽ�g�`�f�[�^�͍폜����A�Đ���Ԃ������ꍇ�͒�~���� )
extern	int NS_ResetSoftSoundPlayer( int SSoundPlayerHandle )
{
	SOFTSOUND * SPlayer ;

	if( SoundSysData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �G���[����
	if( SSND_MASKHCHK( SSoundPlayerHandle, SPlayer ) || SPlayer->IsPlayer == 0 )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

		return -1 ;
	}

	// �Đ����~�߂�
	SoundBuffer_Stop( &SPlayer->Player.SoundBuffer, TRUE ) ;

	// �Đ��ʒu��擪�ɖ߂�
	SoundBuffer_SetCurrentPosition( &SPlayer->Player.SoundBuffer, 0 ) ;

	// �����O�o�b�t�@���ď���������
	RingBufTerminate( &SPlayer->Player.StockSample ) ;
	RingBufInitialize( &SPlayer->Player.StockSample ) ;
	SPlayer->Player.StockSampleNum = 0 ;

	// ���̑���������������
	SPlayer->Player.DataSetCompOffset = 0 ;
	SPlayer->Player.NoneDataSetCompOffset = 0 ;
	SPlayer->Player.NoneDataPlayCheckBackPlayOffset = 0 ;
	SPlayer->Player.NoneDataPlayStartFlag = FALSE ;
	SPlayer->Player.IsPlayFlag = FALSE ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �I��
	return 0 ;
}

// �S�Ẵ\�t�g�E�G�A�T�E���h�̈ꎞ��~��Ԃ�ύX����
extern int PauseSoftSoundAll( int PauseFlag )
{
	HANDLELIST *List ;
	SOFTSOUND * SPlayer ;

	if( SoundSysData.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// ��~���邩�J�n���邩�ŏ����𕪊�
	if( PauseFlag )
	{
		// ��~����ꍇ

		for( List = SoundSysData.SoftSoundPlayerListFirst.Next ; List->Next != NULL ; List = List->Next )
		{
			SPlayer = ( SOFTSOUND * )List->Data ;

			if( SPlayer->Player.SoundBuffer.Valid == FALSE || SoundBuffer_CheckEnable( &SPlayer->Player.SoundBuffer ) == FALSE )
				continue ;

			// ���ɏ�ԕۑ��ς݂̏ꍇ�͉������Ȃ�
			if( SPlayer->Player.SoundBufferPlayStateBackupFlagValid )
				continue ;

			// �T�E���h�o�b�t�@�̍Đ���Ԃ�ۑ�
			{
				int IsPlay ;

				IsPlay = SoundBuffer_CheckPlay( &SPlayer->Player.SoundBuffer ) ;
				if( IsPlay != -1 )
				{
					SPlayer->Player.SoundBufferPlayStateBackupFlagValid = TRUE ;

					if( IsPlay )
					{
						SPlayer->Player.SoundBufferPlayStateBackupFlag = TRUE ;

						// �Đ�����Ă�����Đ����~�߂�
						SoundBuffer_Stop( &SPlayer->Player.SoundBuffer, TRUE ) ;
					}
					else
					{
						SPlayer->Player.SoundBufferPlayStateBackupFlag = FALSE ;
					}
				}
				else
				{
					SPlayer->Player.SoundBufferPlayStateBackupFlagValid = FALSE ;
				}
			}
		}
	}
	else
	{
		// �ĊJ����ꍇ

		for( List = SoundSysData.SoftSoundPlayerListFirst.Next ; List->Next != NULL ; List = List->Next )
		{
			SPlayer = ( SOFTSOUND * )List->Data ;

			// �Đ����ł͂Ȃ��ꍇ�̂ݏ���
			if( SPlayer->Player.IsPlayFlag )
			{
				if( SPlayer->Player.SoundBuffer.Valid == FALSE || SoundBuffer_CheckEnable( &SPlayer->Player.SoundBuffer ) == FALSE ) continue ;

				// �T�E���h�o�b�t�@�̍Đ���Ԃ��L���ŁA���Đ����Ă����ꍇ�͍Đ����ĊJ����
				if( SPlayer->Player.SoundBufferPlayStateBackupFlagValid &&
					SPlayer->Player.SoundBufferPlayStateBackupFlag )
				{
					SoundBuffer_Play( &SPlayer->Player.SoundBuffer, SPlayer->Player.SoundBuffer.Loop ) ;
				}
			}

			SPlayer->Player.SoundBufferPlayStateBackupFlagValid = FALSE ;
		}
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �I��
	return 0 ;
}

// �T�E���h�f�[�^��ʂ̃T�E���h�f�[�^�ɃR�s�[����
extern int CopySoftSound( int SrcSoftSoundHandle, int DestSoftSoundHandle )
{
	SOFTSOUND * SSSound ;
	SOFTSOUND * DSSound ;

	// �G���[����
	if( SSND_MASKHCHK_ASYNC( SrcSoftSoundHandle, SSSound ) ) return -1 ;
	if( SSSound->IsPlayer == 1 ) return -1 ;

	if( SSND_MASKHCHK_ASYNC( DestSoftSoundHandle, DSSound ) ) return -1 ;
	if( DSSound->IsPlayer == 1 ) return -1 ;

	// ���f�[�^�Ɠ]����̃T���v�������`�����l�������r�b�g�����قȂ�ꍇ�̓G���[
	if( SSSound->Wave.BufferSampleNum        != DSSound->Wave.BufferSampleNum   ||
		SSSound->BufferFormat.nChannels      != DSSound->BufferFormat.nChannels ||
		SSSound->BufferFormat.wBitsPerSample != DSSound->BufferFormat.wBitsPerSample )
	{
		return -1 ;
	}

	// ���f�[�^�Ɠ]����̃t�H�[�}�b�g���قȂ�ꍇ�̓G���[
	if( SSSound->BufferFormat.wFormatTag != DSSound->BufferFormat.wFormatTag )
	{
		return -1 ;
	}

	// �f�[�^���R�s�[
	_MEMCPY( DSSound->Wave.Buffer, SSSound->Wave.Buffer, SSSound->Wave.BufferSampleNum * SSSound->BufferFormat.nBlockAlign ) ;

	// ����I��
	return 0 ;
}

// int�^�̃T�E���h�f�[�^���� float�^�̃T�E���h�f�[�^���Z�b�g�A�b�v����
extern int ConvertIntToFloatSoftSound( int SrcSoftSoundHandle, int DestSoftSoundHandle )
{
	SOFTSOUND * SSSound ;
	SOFTSOUND * DSSound ;
	int i ;
	int SrcSampleNum ;
	float *Dest ;

	// �G���[����
	if( SSND_MASKHCHK_ASYNC( SrcSoftSoundHandle, SSSound ) ) return -1 ;
	if( SSSound->IsPlayer == 1 ) return -1 ;

	if( SSND_MASKHCHK_ASYNC( DestSoftSoundHandle, DSSound ) ) return -1 ;
	if( DSSound->IsPlayer == 1 ) return -1 ;

	// ���f�[�^�Ɠ]����̃T���v�������`�����l�������قȂ�ꍇ�̓G���[
	if( SSSound->Wave.BufferSampleNum   != DSSound->Wave.BufferSampleNum ||
		SSSound->BufferFormat.nChannels != DSSound->BufferFormat.nChannels )
	{
		return -1 ;
	}

	// ���f�[�^�� float �^�̏ꍇ�̓G���[
	if( SSSound->BufferFormat.wFormatTag == WAVE_FORMAT_IEEE_FLOAT )
	{
		return -1 ;
	}

	// �]���悪 float �^�ł͂Ȃ��ꍇ�̓G���[
	if( DSSound->BufferFormat.wFormatTag != WAVE_FORMAT_IEEE_FLOAT )
	{
		return -1 ;
	}

	SrcSampleNum = SSSound->Wave.BufferSampleNum ;
	Dest = ( float * )DSSound->Wave.Buffer ;
	switch( SSSound->BufferFormat.wBitsPerSample )
	{
	case 8 :
		{
			BYTE *Src ;

			Src = ( BYTE * )SSSound->Wave.Buffer ;
			if( SSSound->BufferFormat.nChannels == 1 )
			{
				for( i = 0 ; i < SrcSampleNum ; i ++ )
				{
					Dest[ i ] = ( float )( ( int )Src[ i ] - 127 ) / 128.0f ;
				}
			}
			else
			{
				for( i = 0 ; i < SrcSampleNum ; i ++, Src += 2, Dest += 2 )
				{
					Dest[ 0 ] = ( float )( ( int )Src[ 0 ] - 127 ) / 128.0f ;
					Dest[ 1 ] = ( float )( ( int )Src[ 1 ] - 127 ) / 128.0f ;
				}
			}
		}
		break ;

	case 16 :
		{
			short *Src ;

			Src = ( short * )SSSound->Wave.Buffer ;
			if( SSSound->BufferFormat.nChannels == 1 )
			{
				for( i = 0 ; i < SrcSampleNum ; i ++ )
				{
					Dest[ i ] = ( float )Src[ i ] / 32768.0f ;
				}
			}
			else
			{
				for( i = 0 ; i < SrcSampleNum ; i ++, Src += 2, Dest += 2 )
				{
					Dest[ 0 ] = ( float )Src[ 0 ] / 32768.0f ;
					Dest[ 1 ] = ( float )Src[ 1 ] / 32768.0f ;
				}
			}
		}
		break ;
	}

	// ����I��
	return 0 ;
}

// float�^�̃T�E���h�f�[�^���� int�^�̃T�E���h�f�[�^���Z�b�g�A�b�v����
extern int ConvertFloatToIntSoftSound( int SrcSoftSoundHandle, int DestSoftSoundHandle )
{
	SOFTSOUND * SSSound ;
	SOFTSOUND * DSSound ;
	int i ;
	int SrcSampleNum ;
	float *Src ;
	int ChannelI ;

	// �G���[����
	if( SSND_MASKHCHK_ASYNC( SrcSoftSoundHandle, SSSound ) ) return -1 ;
	if( SSSound->IsPlayer == 1 ) return -1 ;

	if( SSND_MASKHCHK_ASYNC( DestSoftSoundHandle, DSSound ) ) return -1 ;
	if( DSSound->IsPlayer == 1 ) return -1 ;

	// ���f�[�^�Ɠ]����̃T���v�������`�����l�������قȂ�ꍇ�̓G���[
	if( SSSound->Wave.BufferSampleNum   != DSSound->Wave.BufferSampleNum ||
		SSSound->BufferFormat.nChannels != DSSound->BufferFormat.nChannels )
	{
		return -1 ;
	}

	// ���f�[�^�� int �^�̏ꍇ�̓G���[
	if( SSSound->BufferFormat.wFormatTag == WAVE_FORMAT_PCM )
	{
		return -1 ;
	}

	// �]���悪 int �^�ł͂Ȃ��ꍇ�̓G���[
	if( DSSound->BufferFormat.wFormatTag != WAVE_FORMAT_PCM )
	{
		return -1 ;
	}

	SrcSampleNum = SSSound->Wave.BufferSampleNum ;
	Src = ( float * )SSSound->Wave.Buffer ;
	switch( DSSound->BufferFormat.wBitsPerSample )
	{
	case 8 :
		{
			BYTE *Dest ;

			Dest = ( BYTE * )DSSound->Wave.Buffer ;
			if( SSSound->BufferFormat.nChannels == 1 )
			{
				for( i = 0 ; i < SrcSampleNum ; i ++ )
				{
					ChannelI = _FTOL( Src[ i ] * 128.0f + 127.0f  ) ;
						 if( ChannelI > 255 ) ChannelI = 255 ;
					else if( ChannelI <   0 ) ChannelI =   0 ;
					Dest[ i ] = ( BYTE )ChannelI ;
				}
			}
			else
			{
				for( i = 0 ; i < SrcSampleNum ; i ++, Src += 2, Dest += 2 )
				{
					ChannelI = _FTOL( Src[ 0 ] * 128.0f + 127.0f  ) ;
						 if( ChannelI > 255 ) ChannelI = 255 ;
					else if( ChannelI <   0 ) ChannelI =   0 ;
					Dest[ 0 ] = ( BYTE )ChannelI ;

					ChannelI = _FTOL( Src[ 1 ] * 128.0f + 127.0f  ) ;
						 if( ChannelI > 255 ) ChannelI = 255 ;
					else if( ChannelI <   0 ) ChannelI =   0 ;
					Dest[ 1 ] = ( BYTE )ChannelI ;
				}
			}
		}
		break ;

	case 16 :
		{
			short *Dest ;

			Dest = ( short * )DSSound->Wave.Buffer ;
			if( SSSound->BufferFormat.nChannels == 1 )
			{
				for( i = 0 ; i < SrcSampleNum ; i ++ )
				{
					ChannelI = _FTOL( Src[ i ] * 32768.0f  ) ;
						 if( ChannelI >  32767 ) ChannelI =  32767 ;
					else if( ChannelI < -32768 ) ChannelI = -32768 ;
					Dest[ i ] = ( short )ChannelI ;
				}
			}
			else
			{
				for( i = 0 ; i < SrcSampleNum ; i ++, Src += 2, Dest += 2 )
				{
					ChannelI = _FTOL( Src[ 0 ] * 32768.0f  ) ;
						 if( ChannelI >  32767 ) ChannelI =  32767 ;
					else if( ChannelI < -32768 ) ChannelI = -32768 ;
					Dest[ 0 ] = ( short )ChannelI ;

					ChannelI = _FTOL( Src[ 1 ] * 32768.0f  ) ;
						 if( ChannelI >  32767 ) ChannelI =  32767 ;
					else if( ChannelI < -32768 ) ChannelI = -32768 ;
					Dest[ 1 ] = ( short )ChannelI ;
				}
			}
		}
		break ;
	}

	// ����I��
	return 0 ;
}

// �\�t�g�E�G�A�Ő��䂷��S�ẴT�E���h�f�[�^�v���C���[�̒���������s��
extern	int ST_SoftSoundPlayerProcessAll( void )
{
	HANDLELIST *List ;
	SOFTSOUND * SPlayer ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	for( List = SoundSysData.SoftSoundPlayerListFirst.Next ; List->Next != NULL ; List = List->Next )
	{
		SPlayer = ( SOFTSOUND * )List->Data ;

		// �Đ����ł͂Ȃ��ꍇ�͉�����������
		if( SPlayer->Player.IsPlayFlag == FALSE ) continue ;

		_SoftSoundPlayerProcess( SPlayer ) ;
	}

	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�Ő��䂷��T�E���h�f�[�^�v���C���[�̒������
static int _SoftSoundPlayerProcess( SOFTSOUND * SPlayer )
{
	int WriteSize, NoneWriteSize, WriteStartPos ;
	DWORD PlayPos, WritePos ;
	int hr ;
	int Result = -1 ;
	DWORD MoveSize, MoveTempSize ;
	DWORD C, S, P, N ;
	SOUNDBUFFERLOCKDATA LockData ;

	if( SoundSysData.InitializeFlag == FALSE ) return -1 ;

	// �Đ���Ԃł͂Ȃ��ꍇ�͉������Ȃ�
	if( SPlayer->Player.IsPlayFlag == FALSE ) return 0 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �Đ��ʒu�̎擾
	SoundBuffer_GetCurrentPosition( &SPlayer->Player.SoundBuffer, &PlayPos, &WritePos ) ;

	// �����f�[�^�Đ��`�F�b�N
	if( SPlayer->Player.NoneDataSetCompOffset != -1 && SPlayer->Player.NoneDataPlayStartFlag == FALSE )
	{
		P = ( DWORD )SPlayer->Player.NoneDataPlayCheckBackPlayOffset ;
		N = PlayPos ;
		if( ( N > P && ( P <= ( DWORD )SPlayer->Player.NoneDataSetCompOffset && N >= ( DWORD )SPlayer->Player.NoneDataSetCompOffset ) ) ||
			( N < P && ( P <= ( DWORD )SPlayer->Player.NoneDataSetCompOffset || N >= ( DWORD )SPlayer->Player.NoneDataSetCompOffset ) ) )
		{
			SPlayer->Player.NoneDataPlayStartFlag = TRUE ;
		}
		else
		{
			SPlayer->Player.NoneDataPlayCheckBackPlayOffset = ( int )N ;
		}
	}

	// �]������K�v�����邩�ǂ����𒲂ׂ�
	WriteStartPos = ( int )SPlayer->Player.DataSetCompOffset - ( int )SPlayer->Player.MinDataSetSize ;
	if( WriteStartPos < 0 )
	{
		WriteStartPos += SPlayer->Player.SoundBufferSize ;
	}
	C = ( DWORD )SPlayer->Player.DataSetCompOffset ;
	S = ( DWORD )WriteStartPos ;
	if( ( S > C && ( S > PlayPos && C < PlayPos ) ) ||
		( S < C && ( S > PlayPos || C < PlayPos ) ) )
	{
		Result = 0 ;
		goto END ;
	}

	// �]������T�C�Y���Z�o����
	WriteSize = 0 ;
	if( S > C )
	{
		if( S < PlayPos )
		{
			WriteSize = ( int )( PlayPos - S ) ;
		}
		else
		{
			WriteSize = ( int )( ( SPlayer->Player.SoundBufferSize - S ) + PlayPos ) ;
		}
	}
	else
	{
		WriteSize = ( int )( PlayPos - S ) ;
	}
	WriteSize += SPlayer->Player.MinDataSetSize ;
	WriteSize /= SPlayer->BufferFormat.nBlockAlign ;
	if( WriteSize < 0 )
	{
		Result = 0 ;
		goto END ;
	}

	// �T�E���h�o�b�t�@�ɓ]������L���f�[�^�̃T�C�Y�Ɩ����f�[�^�̃T�C�Y���Z�o
	NoneWriteSize = 0 ;
	if( WriteSize > SPlayer->Player.StockSampleNum )
	{
		if( SPlayer->Player.StockSampleNum <= 0 )
		{
			NoneWriteSize = WriteSize ;
			WriteSize = 0 ;
		}
		else
		{
			WriteSize = SPlayer->Player.StockSampleNum ;
		}
	}

	// �T�E���h�o�b�t�@�����b�N����
	MoveSize = ( DWORD )( ( NoneWriteSize + WriteSize ) * SPlayer->BufferFormat.nBlockAlign ) ;
	hr = SoundBuffer_Lock(
		&SPlayer->Player.SoundBuffer, ( DWORD )SPlayer->Player.DataSetCompOffset,
		MoveSize,
		( void ** )&LockData.WriteP,  &LockData.Length,
		( void ** )&LockData.WriteP2, &LockData.Length2 ) ;
	if( hr != 0 )
		goto END ;

	// �����f�[�^���Z�b�g����ꍇ�͖����f�[�^�̃Z�b�g���J�n�����I�t�Z�b�g��ۑ�����
	if( NoneWriteSize != 0 && SPlayer->Player.NoneDataSetCompOffset == -1 )
	{
		SPlayer->Player.NoneDataSetCompOffset = SPlayer->Player.DataSetCompOffset + WriteSize * SPlayer->BufferFormat.nBlockAlign ;
		if( SPlayer->Player.NoneDataSetCompOffset > SPlayer->Player.SoundBufferSize )
		{
			SPlayer->Player.NoneDataSetCompOffset -= SPlayer->Player.SoundBufferSize ;
		}
		SPlayer->Player.NoneDataPlayCheckBackPlayOffset = ( int )PlayPos ;
		SPlayer->Player.NoneDataPlayStartFlag           = FALSE ;
	}

	// �����f�[�^�Đ����ɗL���f�[�^��]������ꍇ�͖����f�[�^�Đ��t���O��|��
	if( SPlayer->Player.NoneDataPlayStartFlag && WriteSize != 0 )
	{
		SPlayer->Player.NoneDataPlayStartFlag = FALSE ;
		SPlayer->Player.NoneDataPlayCheckBackPlayOffset = 0 ;
		SPlayer->Player.NoneDataSetCompOffset = -1 ;
	}

	// �c�T���v���������炷
	SPlayer->Player.StockSampleNum -= WriteSize ;

	// ���b�N�����Z�b�g
	LockData.StartOffst = ( DWORD )SPlayer->Player.DataSetCompOffset ;
	LockData.Offset     = 0 ;
	LockData.Offset2    = 0 ;
	LockData.Valid      = LockData.Length ;
	LockData.Valid2     = LockData.Length2 ;

	// �L���f�[�^��]��
	if( LockData.Valid != 0 && WriteSize != 0 )
	{
		MoveTempSize = ( DWORD )( WriteSize * SPlayer->BufferFormat.nBlockAlign ) ;
		if( MoveTempSize > LockData.Valid )
			MoveTempSize = LockData.Valid ;

		RingBufDataGet( &SPlayer->Player.StockSample, LockData.WriteP + LockData.Offset, ( int )MoveTempSize, FALSE ) ;
		LockData.Offset += MoveTempSize ;
		LockData.Valid -= MoveTempSize ;
		WriteSize -= MoveTempSize / SPlayer->BufferFormat.nBlockAlign ;
	}
	if( LockData.Valid2 != 0 && WriteSize != 0 )
	{
		MoveTempSize = ( DWORD )( WriteSize * SPlayer->BufferFormat.nBlockAlign ) ;
		if( MoveTempSize > LockData.Valid2 )
			MoveTempSize = LockData.Valid2 ;

		RingBufDataGet( &SPlayer->Player.StockSample, LockData.WriteP2 + LockData.Offset2, ( int )MoveTempSize, FALSE ) ;
		LockData.Offset2 += MoveTempSize ;
		LockData.Valid2 -= MoveTempSize ;
		WriteSize -= MoveTempSize / SPlayer->BufferFormat.nBlockAlign ;
	}

	// �����f�[�^��]��
	if( LockData.Valid != 0 && NoneWriteSize != 0 )
	{
		MoveTempSize = ( DWORD )( NoneWriteSize * SPlayer->BufferFormat.nBlockAlign ) ;
		if( MoveTempSize > LockData.Valid )
			MoveTempSize = LockData.Valid ;

		switch( SPlayer->BufferFormat.wBitsPerSample )
		{
		case 8  : _MEMSET( LockData.WriteP + LockData.Offset, 127, MoveTempSize ) ; break ;
		case 16 : _MEMSET( LockData.WriteP + LockData.Offset,   0, MoveTempSize ) ; break ;
		}

		LockData.Offset += MoveTempSize ;
		LockData.Valid -= MoveTempSize ;
		NoneWriteSize -= MoveTempSize / SPlayer->BufferFormat.nBlockAlign ;
	}
	if( LockData.Valid2 != 0 && NoneWriteSize != 0 )
	{
		MoveTempSize = ( DWORD )( NoneWriteSize * SPlayer->BufferFormat.nBlockAlign ) ;
		if( MoveTempSize > LockData.Valid2 )
			MoveTempSize = LockData.Valid2 ;

		switch( SPlayer->BufferFormat.wBitsPerSample )
		{
		case 8  : _MEMSET( LockData.WriteP2 + LockData.Offset2, 127, MoveTempSize ) ; break ;
		case 16 : _MEMSET( LockData.WriteP2 + LockData.Offset2,   0, MoveTempSize ) ; break ;
		}

		LockData.Offset2 += MoveTempSize ;
		LockData.Valid2 -= MoveTempSize ;
		NoneWriteSize -= MoveTempSize / SPlayer->BufferFormat.nBlockAlign ;
	}

	// �T�E���h�o�b�t�@�̃��b�N����������
	SoundBuffer_Unlock(
		&SPlayer->Player.SoundBuffer,
		LockData.WriteP,  LockData.Length,
		LockData.WriteP2, LockData.Length2 ) ;

	// �f�[�^�Z�b�g�����������ʒu��ύX����
	SPlayer->Player.DataSetCompOffset += MoveSize ;
	if( SPlayer->Player.DataSetCompOffset >= SPlayer->Player.SoundBufferSize )
		SPlayer->Player.DataSetCompOffset -= SPlayer->Player.SoundBufferSize ;

	Result = 0 ;
END :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_SOFTSOUND ].CriticalSection ) ;

	// �I��
	return Result ;
}





































// �l�h�c�h����֐�

// �l�h�c�h�n���h����������������֐�
extern int InitializeMidiHandle( HANDLEINFO *HandleInfo )
{
	MIDIHANDLEDATA *MusicData = ( MIDIHANDLEDATA * )HandleInfo ;

	// ���ʂ��Z�b�g
	MusicData->Volume = 255 ;

	// �I��
	return 0 ;
}

// �l�h�c�h�n���h���̌�n�����s���֐�
extern int TerminateMidiHandle( HANDLEINFO *HandleInfo )
{
	MIDIHANDLEDATA *MusicData = ( MIDIHANDLEDATA * )HandleInfo ;

	// ���̍Đ����~�߂�
	if( NS_CheckMusicMem( HandleInfo->Handle ) == TRUE )
		NS_StopMusicMem( HandleInfo->Handle ) ;

	// ���ˑ�����
	if( TerminateMidiHandle_PF( MusicData ) < 0 )
	{
		return -1 ;
	}

	// �l�h�c�h�f�[�^�̉��
	if( MusicData->DataImage != NULL )
	{
		_MEMSET( MusicData->DataImage, 0, ( size_t )MusicData->DataSize );
		DXFREE( MusicData->DataImage ) ;
		MusicData->DataImage = NULL ;
	}

	// �I��
	return 0 ;
}

// �V�����l�h�c�h�n���h�����擾����
extern int NS_AddMusicData( void )
{
	int NewHandle ;

	NewHandle = AddHandle( DX_HANDLETYPE_MUSIC, FALSE, -1 ) ;

	return NewHandle ;
}

// �l�h�c�h�n���h�����폜����
extern int NS_DeleteMusicMem( int MusicHandle )
{
	return SubHandle( MusicHandle ) ;
}

// LoadMusicMemByMemImage �̎������֐�
extern int LoadMusicMemByMemImage_Static(
	int MusicHandle,
	const void *FileImage,
	int FileImageSize,
	int ASyncThread
)
{
	MIDIHANDLEDATA *MusicData ;

	if( ASyncThread )
	{
		if( MIDI_MASKHCHK_ASYNC( MusicHandle, MusicData ) )
			return -1 ;
	}
	else
	{
		if( MIDI_MASKHCHK( MusicHandle, MusicData ) )
			return -1 ;
	}

	// �C���[�W�̃R�s�[���쐬
	MusicData->DataImage = DXALLOC( ( size_t )FileImageSize ) ;
	if( MusicData->DataImage == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xdf\x30\xe5\x30\xfc\x30\xb8\x30\xc3\x30\xaf\x30\xc7\x30\xfc\x30\xbf\x30\x92\x30\x00\x4e\x42\x66\x84\x76\x6b\x30\xdd\x4f\x58\x5b\x57\x30\x66\x30\x4a\x30\x4f\x30\xe1\x30\xe2\x30\xea\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�~���[�W�b�N�f�[�^���ꎞ�I�ɕۑ����Ă����������̈�̊m�ۂɎ��s���܂���\n" @*/ ) ;
		return -1 ;
	}
	_MEMCPY( MusicData->DataImage, FileImage, ( size_t )FileImageSize ) ;
	MusicData->DataSize = FileImageSize ;

	// ���ˑ�����
	if( LoadMusicMemByMemImage_Static_PF( MusicData, ASyncThread ) < 0 )
	{
		return -1 ;
	}

	// �Đ����t���O��|��
	MusicData->PlayFlag = FALSE ;

	// �I��
	return 0 ;
}

#ifndef DX_NON_ASYNCLOAD
// LoadMusicMemByMemImage �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void LoadMusicMemByMemImage_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	int MusicHandle ;
	const void *FileImage ;
	int FileImageSize ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	MusicHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	FileImage = GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	FileImageSize = GetASyncLoadParamInt( AParam->Data, &Addr ) ;

	Result = LoadMusicMemByMemImage_Static( MusicHandle, FileImage, FileImageSize, TRUE ) ;

	DecASyncLoadCount( MusicHandle ) ;
	if( Result < 0 )
	{
		SubHandle( MusicHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// LoadMusicMemByMemImage �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int LoadMusicMemByMemImage_UseGParam(
	const void *FileImage,
	int FileImageSize,
	int ASyncLoadFlag
)
{
	int MusicHandle ;

	if( SoundSysData.InitializeFlag == FALSE )
		return -1 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �n���h���̍쐬
	MusicHandle = AddHandle( DX_HANDLETYPE_MUSIC, FALSE, -1 ) ;
	if( MusicHandle == -1 ) return -1 ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt( NULL, &Addr, MusicHandle ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, FileImage ) ;
		AddASyncLoadParamInt( NULL, &Addr, FileImageSize ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = LoadMusicMemByMemImage_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt( AParam->Data, &Addr, MusicHandle ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, FileImage ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, FileImageSize ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( MusicHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( LoadMusicMemByMemImage_Static( MusicHandle, FileImage, FileImageSize, FALSE ) < 0 )
			goto ERR ;
	}

	// �I��
	return MusicHandle ;

ERR :
	SubHandle( MusicHandle ) ;

	return -1 ;
}

// ��������ɓW�J���ꂽ�l�h�c�h�t�@�C����ǂݍ���
extern int NS_LoadMusicMemByMemImage( const void *FileImageBuffer, int FileImageSize )
{
	return LoadMusicMemByMemImage_UseGParam( FileImageBuffer, FileImageSize, GetASyncLoadFlag() ) ;
}

// LoadMusicMem �̎������֐�
static int LoadMusicMem_Static(
	int MusicHandle,
	const wchar_t *FileName,
	int ASyncThread
)
{
	MIDIHANDLEDATA * MusicData ;
	DWORD_PTR fp = 0 ;
	void *Buffer = NULL ;
	size_t FileSize ;
	int Ret ;

	if( ASyncThread )
	{
		if( MIDI_MASKHCHK_ASYNC( MusicHandle, MusicData ) )
			return -1 ;
	}
	else
	{
		if( MIDI_MASKHCHK( MusicHandle, MusicData ) )
			return -1 ;
	}

	// �t�@�C���̓ǂݍ���
	{
		fp = DX_FOPEN( FileName ) ;
		if( fp == 0 )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Music File Open Error : %s", FileName )) ;
			goto ERR ;
		}
		DX_FSEEK( fp, 0L, SEEK_END ) ;
		FileSize = ( size_t )DX_FTELL( fp ) ;
		DX_FSEEK( fp, 0L, SEEK_SET ) ;
		Buffer = DXALLOC( FileSize ) ;
		if( Buffer == NULL )
		{
			DXST_ERRORLOGFMT_ADDW(( L"Music File Memory Alloc Error : %s", FileName )) ;
			goto ERR ;
		}
		DX_FREAD( Buffer, FileSize, 1, fp ) ;
		DX_FCLOSE( fp ) ;
		fp = 0 ;
	}

	// �n���h���̃Z�b�g�A�b�v
	Ret = LoadMusicMemByMemImage_Static( MusicHandle, Buffer, ( int )FileSize, ASyncThread ) ;

	// �������̉��
	DXFREE( Buffer ) ;

	// ���ʂ�Ԃ�
	return Ret ;

ERR :
	if( fp != 0 ) DX_FCLOSE( fp ) ;
	if( Buffer != NULL ) DXFREE( Buffer ) ;

	return -1 ;
}

#ifndef DX_NON_ASYNCLOAD
// LoadMusicMem �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void LoadMusicMem_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	int MusicHandle ;
	const wchar_t *FileName ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	MusicHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	FileName = GetASyncLoadParamString( AParam->Data, &Addr ) ;

	Result = LoadMusicMem_Static( MusicHandle, FileName, TRUE ) ;

	DecASyncLoadCount( MusicHandle ) ;
	if( Result < 0 )
	{
		SubHandle( MusicHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// LoadMusicMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int LoadMusicMem_UseGParam(
	const wchar_t *FileName,
	int ASyncLoadFlag
)
{
	int MusicHandle ;

	if( SoundSysData.InitializeFlag == FALSE )
		return -1 ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �n���h���̍쐬
	MusicHandle = AddHandle( DX_HANDLETYPE_MUSIC, FALSE, -1 ) ;
	if( MusicHandle == -1 ) return -1 ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;
		wchar_t FullPath[ 1024 ] ;

		ConvertFullPathW_( FileName, FullPath, sizeof( FullPath ) ) ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt( NULL, &Addr, MusicHandle ) ;
		AddASyncLoadParamString( NULL, &Addr, FullPath ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = LoadMusicMem_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt( AParam->Data, &Addr, MusicHandle ) ;
		AddASyncLoadParamString( AParam->Data, &Addr, FullPath ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( MusicHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( LoadMusicMem_Static( MusicHandle, FileName, FALSE ) < 0 )
			goto ERR ;
	}

	// �I��
	return MusicHandle ;

ERR :
	SubHandle( MusicHandle ) ;

	return -1 ;
}

// �l�h�c�h�t�@�C����ǂݍ���
extern int NS_LoadMusicMem( const TCHAR *FileName )
{
#ifdef UNICODE
	return LoadMusicMem_WCHAR_T(
		FileName
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = LoadMusicMem_WCHAR_T(
		UseFileNameBuffer
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}

// �l�h�c�h�t�@�C����ǂݍ���
extern int LoadMusicMem_WCHAR_T( const wchar_t *FileName )
{
	return LoadMusicMem_UseGParam( FileName, GetASyncLoadFlag() ) ;
}

// �ǂݍ��񂾂l�h�c�h�f�[�^�̉��t���J�n����
extern int NS_PlayMusicMem( int MusicHandle, int PlayType )
{
	MIDIHANDLEDATA * MusicData ;
	int IsDefaultHandle ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �f�t�H���g�n���h�����ǂ������`�F�b�N
	IsDefaultHandle = ( MusicHandle != 0 && MusicHandle == MidiSystemData.DefaultHandle ) ? TRUE : FALSE ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;

	// �G���[����
	if( MIDI_MASKHCHK( MusicHandle, MusicData ) )
		return -1 ;

	// ���t���~����
	NS_StopMusicMem( MidiSystemData.PlayHandle ) ;

	// ���ˑ�����
	if( PlayMusicMem_PF( MusicData, PlayType ) < 0 )
	{
		return -1 ;
	}

	MidiSystemData.PlayFlag		= TRUE ;								// ���t�t���O�𗧂Ă�
	MidiSystemData.PlayHandle	= MusicHandle ;							// ���t���Ă���n���h���̍X�V
	MidiSystemData.LoopFlag		= PlayType == DX_PLAYTYPE_LOOP ;		// ���[�v�t���O���Z�b�g����

	if( MIDI_MASKHCHK( MusicHandle, MusicData ) )
	{
		MusicData = NULL ;
	}
	else
	{
		MusicData->PlayFlag			= TRUE ;								// ��Ԃ��Đ����ɂ���
		MusicData->PlayStartFlag	= FALSE ;								// ���t���J�n���ꂽ���t���O��|��
	}

	// ���t�I���܂ő҂w��̏ꍇ�͂����ő҂�
	if( PlayType == DX_PLAYTYPE_NORMAL )
	{
		// �Đ��I���܂ő҂�
		while( NS_ProcessMessage() == 0 )
		{
			if( IsDefaultHandle )
			{
				if( NS_CheckMusic() == FALSE )
				{
					break ;
				}
			}
			else
			{
				if( NS_CheckMusicMem( MusicHandle ) == FALSE )
				{
					break ;
				}
			}
		}

		// ��~�������s��
		if( IsDefaultHandle )
		{
			NS_StopMusic() ;
		}
		else
		{
			NS_StopMusicMem( MusicHandle ) ;
		}

		// �e���|�����t�@�C������Đ�����Ă����ꍇ�͍폜����
//		if( MidiSystemData.MemImagePlayFlag == TRUE )
//		{
//			DeleteFileW( MidiSystemData.FileName ) ;
//		}
//		MidiSystemData.MemImagePlayFlag = FALSE ;
	}

	// �I��
	return 0 ;
}

// �l�h�c�h�f�[�^�̉��t���~����
extern int NS_StopMusicMem( int MusicHandle )
{
	MIDIHANDLEDATA * MusicData ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( MIDI_MASKHCHK( MusicHandle, MusicData ) )
		return -1 ;

	// �T�E���h�n���h���̍Đ��̏ꍇ�̓T�E���h���~�߂�
	if( MidiSystemData.DefaultHandle != 0 && MidiSystemData.DefaultHandleToSoundHandleFlag == TRUE )
	{
		NS_StopSoundMem( MidiSystemData.DefaultHandle ) ;
		NS_DeleteSoundMem( MidiSystemData.DefaultHandle ) ;

		MidiSystemData.DefaultHandle = 0 ;
		return 0 ;
	}

	// ���[�v�t���O��|��
	MidiSystemData.LoopFlag = FALSE ;

	// ���ˑ�����
	if( StopMusicMem_PF( MusicData ) < 0 )
	{
		return -1 ;
	}

	// ��Ԃ��~���ɂ���
	MusicData->PlayFlag = FALSE ;

	// ���t���J�n���ꂽ���t���O��|��
	MusicData->PlayStartFlag = FALSE ;

	// �I��
	return 0 ;
}

// �l�h�c�h�f�[�^�����t�����ǂ������擾����( TRUE:���t��  FALSE:��~�� )
extern int NS_CheckMusicMem( int MusicHandle )
{
	MIDIHANDLEDATA * MusicData ;
	int Result = -1 ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( MIDI_MASKHCHK( MusicHandle, MusicData ) )
		return -1 ;

	// �T�E���h�n���h���̍Đ��̏ꍇ�̓T�E���h�̍Đ���Ԃ�Ԃ�
	if( MidiSystemData.DefaultHandle != 0 && MidiSystemData.DefaultHandleToSoundHandleFlag == TRUE )
	{
		return NS_CheckSoundMem( MidiSystemData.DefaultHandle ) ;
	}

	// ���ˑ�����
	Result = CheckMusicMem_PF( MusicData ) ;

	return Result ;
}

// �l�h�c�h�f�[�^�̍Đ����ʂ��Z�b�g����
extern int NS_SetVolumeMusicMem( int Volume, int MusicHandle )
{
	MIDIHANDLEDATA * MusicData ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( MIDI_MASKHCHK( MusicHandle, MusicData ) )
		return -1 ;

	// ���ʂ��Z�b�g
	MusicData->Volume = Volume ;

	// �T�E���h�n���h���̍Đ��̏ꍇ�̓T�E���h�̉��ʂ�ύX����
	if( MidiSystemData.DefaultHandle != 0 && MidiSystemData.DefaultHandleToSoundHandleFlag == TRUE )
	{
		return NS_ChangeVolumeSoundMem( Volume, MidiSystemData.DefaultHandle );
	}

	return NS_SetVolumeMusic( Volume )  ;
}

// �l�h�c�h�f�[�^�n���h�������ׂč폜����
extern int NS_InitMusicMem( void )
{
	return AllHandleSub( DX_HANDLETYPE_MUSIC ) ;
}

// �l�h�c�h�f�[�^�̎����I����
extern int NS_ProcessMusicMem( void )
{
	MIDIHANDLEDATA * MusicData ;
	int i ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
		return -1 ;

	if( HandleManageArray[ DX_HANDLETYPE_MUSIC ].InitializeFlag == FALSE )
		return -1 ;

	for( i = HandleManageArray[ DX_HANDLETYPE_MUSIC ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_MUSIC ].AreaMax ; i ++ )
	{
		MusicData = ( MIDIHANDLEDATA * )HandleManageArray[ DX_HANDLETYPE_MUSIC ].Handle[ i ] ;
		if( MusicData == NULL ) continue ;

		if( MusicData->PlayFlag == FALSE ) continue ;

		if( ProcessMusicMem_PF( MusicData ) < 0 )
		{
			return -1 ;
		}
	}

	// �I��
	return 0;
}

// �l�h�c�h�f�[�^�̌��݂̍Đ��ʒu���擾����
extern int NS_GetMusicMemPosition( int MusicHandle )
{
	MIDIHANDLEDATA * MusicData ;
	int Result = -1 ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( MIDI_MASKHCHK( MusicHandle, MusicData ) )
		return -1 ;

	// ���ˑ�����
	Result = GetMusicMemPosition_PF( MusicData ) ;

	return Result ;
}



// �l�h�c�h�t�@�C�����Đ�����
extern int NS_PlayMusic( const TCHAR *FileName , int PlayType )
{
#ifdef UNICODE
	return PlayMusic_WCHAR_T(
		FileName, PlayType
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( FileName, return -1 )

	Result = PlayMusic_WCHAR_T(
		UseFileNameBuffer, PlayType
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( FileName )

	return Result ;
#endif
}


// �l�h�c�h�t�@�C�����Đ�����
extern int PlayMusic_WCHAR_T( const wchar_t *FileName , int PlayType )
{
	// �������t����������~�߂�
	if( MidiSystemData.DefaultHandle != 0 )
	{
		if( MidiSystemData.DefaultHandleToSoundHandleFlag == TRUE )
		{
			NS_DeleteSoundMem( MidiSystemData.DefaultHandle ) ;
		}
		else
		{
			NS_DeleteMusicMem( MidiSystemData.DefaultHandle ) ;
		}
		MidiSystemData.DefaultHandle = 0 ;
	}

	// �ǂݍ���
	MidiSystemData.DefaultHandle = LoadMusicMem_UseGParam( FileName, FALSE ) ;
	if( MidiSystemData.DefaultHandle == -1 )
	{
		MidiSystemData.DefaultHandle = 0;
		return -1 ;
	}
	MidiSystemData.DefaultHandleToSoundHandleFlag = FALSE ;

	// ���t
	if( NS_PlayMusicMem( MidiSystemData.DefaultHandle, PlayType ) == -1 )
	{
		LOADSOUND_GPARAM GParam ;

		// ���t�Ɏ��s�����畁�ʂ̃T�E���h�t�@�C���̉\��������
		NS_DeleteMusicMem( MidiSystemData.DefaultHandle ) ;

		InitLoadSoundGParam( &GParam ) ;
		GParam.CreateSoundDataType = DX_SOUNDDATATYPE_MEMPRESS ;
		MidiSystemData.DefaultHandle = LoadSoundMemBase_UseGParam( &GParam, FileName, 1, -1, FALSE, FALSE ) ;
		if( MidiSystemData.DefaultHandle == -1 )
		{
			// ����ł����s������t�@�C�����Ȃ��Ƃ�������
			MidiSystemData.DefaultHandle = 0;
			return -1 ;
		}

		// �Đ��J�n
		NS_PlaySoundMem( MidiSystemData.DefaultHandle, PlayType ) ;
		MidiSystemData.DefaultHandleToSoundHandleFlag = TRUE ;
	}

	return 0;
}





// ��������ɓW�J����Ă���l�h�c�h�t�@�C�������t����
extern int NS_PlayMusicByMemImage( const void *FileImageBuffer, int FileImageSize, int PlayType )
{
	// �������t����������~�߂�
	if( MidiSystemData.DefaultHandle != 0 )
	{
		if( MidiSystemData.DefaultHandleToSoundHandleFlag == TRUE )
		{
			NS_DeleteSoundMem( MidiSystemData.DefaultHandle ) ;
		}
		else
		{
			NS_DeleteMusicMem( MidiSystemData.DefaultHandle ) ;
		}
		MidiSystemData.DefaultHandle = 0 ;
	}

	// �ǂݍ���
	MidiSystemData.DefaultHandle = LoadMusicMemByMemImage_UseGParam( FileImageBuffer, FileImageSize, FALSE ) ;
	if( MidiSystemData.DefaultHandle == -1 )
	{
		LOADSOUND_GPARAM GParam ;

		// �ǂݍ��݂Ɏ��s�����特���Ƃ��čĐ�����
		InitLoadSoundGParam( &GParam ) ;
		MidiSystemData.DefaultHandle = LoadSoundMemByMemImageBase_UseGParam( &GParam, TRUE, -1, FileImageBuffer, FileImageSize, 1, -1, FALSE, FALSE ) ;
		if( MidiSystemData.DefaultHandle == -1 )
		{
			// ����ł����s������f�[�^�����Ă���Ƃ�������
			MidiSystemData.DefaultHandle = 0;
			return -1 ;
		}
		MidiSystemData.DefaultHandleToSoundHandleFlag = TRUE ;
	}
	else
	{
		MidiSystemData.DefaultHandleToSoundHandleFlag = FALSE ;
	}

	// ���t
	NS_PlayMusicMem( MidiSystemData.DefaultHandle, PlayType ) ;

	return 0;
}

// �l�h�c�h�t�@�C���̉��t��~
extern int NS_StopMusic( void )
{
	if( MidiSystemData.DefaultHandle == 0 ) return 0 ;

	if( MidiSystemData.DefaultHandleToSoundHandleFlag == TRUE )
	{
		NS_StopSoundMem( MidiSystemData.DefaultHandle ) ;
		NS_DeleteSoundMem( MidiSystemData.DefaultHandle ) ;
	}
	else
	{
		NS_StopMusicMem( MidiSystemData.DefaultHandle ) ;
		NS_DeleteMusicMem( MidiSystemData.DefaultHandle ) ;
	}

	MidiSystemData.DefaultHandle = 0 ;

	return 0 ;
}



// �l�h�c�h�t�@�C�������t�����ۂ������擾����
extern int NS_CheckMusic( void )
{
	if( MidiSystemData.DefaultHandle == 0 ) return 0 ;

	if( MidiSystemData.DefaultHandleToSoundHandleFlag == TRUE )
	{
		return NS_CheckSoundMem( MidiSystemData.DefaultHandle ) ;
	}
	else
	{
		return NS_CheckMusicMem( MidiSystemData.DefaultHandle ) ;
	}
}


// �l�h�c�h�̍Đ��`�����Z�b�g����
extern int NS_SelectMidiMode( int Mode )
{
	// �l�̗L�����`�F�b�N
	if( Mode < 0 || Mode >= DX_MIDIMODE_NUM )
	{
		return -1 ;
	}

	// �������[�h�����܂łƓ����ꍇ�͂Ȃɂ������I��
	if( Mode == SoundSysData.SoundMode ) return 0 ;

	// �Đ����[�h���Z�b�g
	SoundSysData.SoundMode = Mode ;

	if( CheckSoundSystem_Initialize_PF() == FALSE ) return 0 ;

	// �T�E���h�V�X�e���̌�n��
	TerminateSoundSystem() ;

	// �T�E���h�V�X�e��������
	return InitializeSoundSystem() ;
}

// �l�h�c�h�̍Đ����ʂ��Z�b�g����
extern int NS_SetVolumeMusic( int Volume )
{
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	if( MidiSystemData.DefaultHandleToSoundHandleFlag == FALSE )
	{
		// ���ˑ�����
		SetVolumeMusic_PF( Volume ) ;
	}
	else
	{
		NS_ChangeVolumeSoundMem( Volume, MidiSystemData.DefaultHandle );
	}

	// �I��
	return 0 ;
}

// �l�h�c�h�̌��݂̍Đ��ʒu���擾����
extern int NS_GetMusicPosition( void )
{
	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �Đ����ł͂Ȃ������牽�����Ȃ�
	if( NS_CheckMusic() == FALSE ) return -1 ;

	// ���ˑ�����
	return GetMusicPosition_PF() ;
}
	





// �⏕�n

// �����t�H�[�}�b�g�Ɣg�`�C���[�W����v�`�u�d�t�@�C���C���[�W���쐬����
extern	int CreateWaveFileImage( 	void **DestBufferP, int *DestBufferSizeP,
									const WAVEFORMATEX *Format, int FormatSize,
									const void *WaveData, int WaveDataSize )
{
	int BufferSize ;
	void *Buffer ;
	BYTE *p ;
	
	// �o�b�t�@���m��
	BufferSize = FormatSize + WaveDataSize
					+ 12/*"RIFF" + �t�@�C���T�C�Y + "WAVE"*/
					+ 8 * 2/*"fmt "�`�����N + "data"�`�����N*/ ;
	Buffer = DXALLOC( ( size_t )BufferSize ) ;
	if( Buffer == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xee\x4e\x57\x00\x41\x00\x56\x00\x45\x00\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xa4\x30\xe1\x30\xfc\x30\xb8\x30\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"��WAVE�t�@�C���C���[�W�p�̃������m�ۂɎ��s���܂���\n" @*/ ) ;
		return -1 ;
	}
	
	// �f�[�^�̃Z�b�g
	p = (BYTE *)Buffer ;
	_MEMCPY( (char *)p, "RIFF", 4 ) ;					p += 4 ;
	*((DWORD *)p) = ( DWORD )( BufferSize - 8 ) ;		p += 4 ;
	_MEMCPY( (char *)p, "WAVE", 4 ) ;					p += 4 ;

	_MEMCPY( (char *)p, "fmt ", 4 ) ;					p += 4 ;
	*((DWORD *)p) = ( DWORD )FormatSize ;				p += 4 ;
	_MEMCPY( p, Format, ( size_t )FormatSize ) ;		p += FormatSize ;

	_MEMCPY( (char *)p, "data", 4 ) ;					p += 4 ;
	*((DWORD *)p) = ( DWORD )WaveDataSize ;				p += 4 ;
	_MEMCPY( p, WaveData, ( size_t )WaveDataSize ) ;	p += WaveDataSize ;

	// ����ۑ�
	*DestBufferP = Buffer ;
	*DestBufferSizeP = BufferSize ;

	// �I��
	return 0 ;
}

// �t�@�C�����ۂ܂郁�����ɓǂݍ���
static	int FileFullRead( const wchar_t *FileName, void **BufferP, int *SizeP, int OutputFileOpenErrorLog )
{
	DWORD_PTR fp = 0 ;
	size_t Size ;
	void *Buffer ;
	
	fp = DX_FOPEN( FileName ) ;
	if( fp == 0 )
	{
		if( OutputFileOpenErrorLog )
		{
			DXST_ERRORLOGFMT_ADDW(( L"File Open Error : %s", FileName )) ;
		}
		goto ERR ;
	}

	DX_FSEEK( fp, 0, SEEK_END ) ;
	Size = ( size_t )DX_FTELL( fp ) ;
	DX_FSEEK( fp, 0, SEEK_SET ) ;
	
	Buffer = DXALLOC( Size ) ;
	if( Buffer == NULL )
	{
		DXST_ERRORLOGFMT_ADDW(( L"File Load : Memory Alloc Error : %s", FileName )) ;
		goto ERR ;
	}
	
	DX_FREAD( Buffer, Size, 1, fp ) ;
	DX_FCLOSE( fp ) ;
	
	*BufferP = Buffer ;
	*SizeP = ( int )Size ;
	
	// �I��
	return 0 ;
	
ERR :
	if( fp != 0 ) DX_FCLOSE( fp ) ;
	
	return -1 ;
}

// �X�g���[���f�[�^���ۂ܂郁�����ɓǂݍ���
extern	int StreamFullRead( STREAMDATA *Stream, void **BufferP, int *SizeP )
{
	size_t Size ;
	void *Buffer ;
	
	Stream->ReadShred.Seek( Stream->DataPoint, 0, SEEK_END ) ;
	Size = ( size_t )Stream->ReadShred.Tell( Stream->DataPoint ) ;
	Stream->ReadShred.Seek( Stream->DataPoint, 0, SEEK_SET ) ;
	
	Buffer = DXALLOC( Size ) ;
	if( Buffer == NULL )
	{
		return DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb9\x30\xc8\x30\xea\x30\xfc\x30\xe0\x30\x6e\x30\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\x6b\x30\xc5\x5f\x81\x89\x6a\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�X�g���[���̓ǂݍ��݂ɕK�v�ȃ������̊m�ۂɎ��s���܂���" @*/ )) ;
	}

	Stream->ReadShred.Read( Buffer, Size, 1, Stream->DataPoint ) ;
	
	*BufferP = Buffer ;
	*SizeP = ( int )Size ;
	
	// �I��
	return 0 ;
}

// �\�t�g�E�G�A�ň����g�`�f�[�^����T�E���h�n���h�����쐬����
extern int NS_LoadSoundMemFromSoftSound( int SoftSoundHandle, int BufferNum )
{
	SOFTSOUND * SSound ;
	int NewHandle ;
	void *WaveImage ;
	int WaveSize ;
	LOADSOUND_GPARAM GParam ;

	// �G���[����
	if( SSND_MASKHCHK( SoftSoundHandle, SSound ) ) return -1 ;
	if( SSound->IsPlayer == TRUE ) return -1 ;

	// �v�`�u�d�t�@�C�����ł����グ��
	if( CreateWaveFileImage( &WaveImage, &WaveSize,
						 &SSound->BufferFormat, sizeof( WAVEFORMATEX ),
						 SSound->Wave.Buffer, SSound->Wave.BufferSampleNum * SSound->BufferFormat.nBlockAlign ) < 0 )
		 return -1 ;

	// �n���h���̍쐬
	InitLoadSoundGParam( &GParam ) ;
	if( GParam.CreateSoundDataType == DX_SOUNDDATATYPE_FILE )
		GParam.CreateSoundDataType = DX_SOUNDDATATYPE_MEMNOPRESS ;
	NewHandle = LoadSoundMemByMemImageBase_UseGParam( &GParam, TRUE, -1, WaveImage, WaveSize, BufferNum, -1, FALSE, FALSE ) ;

	// �������̉��
	DXFREE( WaveImage ) ;
	WaveImage = NULL ;

	// �I��
	return NewHandle ;
}

// �T�E���h�n���h���̃^�C�v���X�g���[���n���h���ł͂Ȃ��ꍇ�ɃX�g���[���n���h���ɕύX����
static int SoundTypeChangeToStream( int SoundHandle )
{
	SOUND * sd ;
	void *WaveImage ;
	int WaveSize ;
	int Time, Volume, Frequency, Pan ;

	if( CheckSoundSystem_Initialize_PF() == FALSE )
	{
		return -1 ;
	}

	// �G���[����
	if( SOUNDHCHK( SoundHandle, sd ) )
		return -1 ;

	// ���ɃX�g���[���n���h���������牽�������ɏI��
	if( sd->Type == DX_SOUNDTYPE_STREAMSTYLE )
		return 0 ;

	// �v�`�u�d�t�@�C�����ł����グ��
	if( CreateWaveFileImage( &WaveImage, &WaveSize,
						 &sd->BufferFormat, sizeof( WAVEFORMATEX ),
						 sd->Normal.WaveData, sd->Normal.WaveSize ) < 0 )
		return -1 ;

	// �����n���h���̐ݒ���擾���Ă���
	Volume = NS_GetVolumeSoundMem( SoundHandle ) ;
	Time = NS_GetSoundCurrentPosition( SoundHandle ) ;
	Frequency = NS_GetFrequencySoundMem( SoundHandle ) ;
	Pan = NS_GetPanSoundMem( SoundHandle ) ;

	// �����n���h���̍폜
	{
		NS_StopSoundMem( SoundHandle ) ;					// �Đ����������Ƃ��̂��Ƃ��l���Ď~�߂Ă���
		NS_DeleteSoundMem( SoundHandle ) ;					// �n���h�����폜
	}

	// �X�g���[���Đ��`���̃n���h���Ƃ��č�蒼��
	{
		int f, res ;
		STREAMDATA Stream ;
		
		Stream.DataPoint = MemStreamOpen( WaveImage, ( size_t )WaveSize ) ;
		Stream.ReadShred = *GetMemStreamDataShredStruct() ;

		SoundHandle = AddHandle( DX_HANDLETYPE_SOUND, FALSE, SoundHandle ) ;
		res = NS_AddStreamSoundMem( &Stream, 0, SoundHandle, DX_SOUNDDATATYPE_MEMNOPRESS, &f ) ;
		if( res == -1 )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xb9\x30\xc8\x30\xea\x30\xfc\x30\xe0\x30\xb5\x30\xa6\x30\xf3\x30\xc9\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�X�g���[���T�E���h�n���h���̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}

		Stream.ReadShred.Close( Stream.DataPoint ) ;
	}
	
	// �������̉��
	DXFREE( WaveImage ) ;

	// ���̃n���h���̐ݒ���Z�b�g����
	NS_SetVolumeSoundMem( Volume, SoundHandle ) ;
	NS_SetSoundCurrentPosition( Time, SoundHandle ) ;
	NS_SetFrequencySoundMem( Frequency, SoundHandle ) ;
	NS_SetPanSoundMem( Pan, SoundHandle ) ;

	// ����I��
	return 0 ;
}

// �~���b�P�ʂ̐��l���T���v���P�ʂ̐��l�ɕϊ�����
static	int MilliSecPositionToSamplePosition( int SamplesPerSec, int MilliSecTime )
{
	LONGLONG TempValue1, TempValue2 ;
	unsigned int Temp[ 4 ] ;

	TempValue1 = SamplesPerSec ;
	TempValue2 = MilliSecTime ;
	_MUL128_1( ( DWORD * )&TempValue1, ( DWORD * )&TempValue2, ( DWORD * )Temp ) ;
	TempValue2 = 1000 ;
	_DIV128_1( ( DWORD * )Temp, ( DWORD * )&TempValue2, ( DWORD * )&TempValue1 ) ;
	return ( int )TempValue1 ;
}

// �T���v���P�ʂ̐��l���~���b�P�ʂ̐��l�ɕϊ�����
static	int SamplePositionToMilliSecPosition( int SamplesPerSec, int SampleTime )
{
	LONGLONG TempValue1, TempValue2 ;
	unsigned int Temp[ 4 ] ;

	TempValue1 = SampleTime ;
	TempValue2 = 1000 ;
	_MUL128_1( ( DWORD * )&TempValue1, ( DWORD * )&TempValue2, ( DWORD * )Temp ) ;
	TempValue2 = SamplesPerSec ;
	_DIV128_1( ( DWORD * )Temp, ( DWORD * )&TempValue2, ( DWORD * )&TempValue1 ) ;
	return ( int )TempValue1 ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_SOUND







