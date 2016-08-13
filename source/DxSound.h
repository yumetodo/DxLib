// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�T�E���h�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXSOUND_H__
#define __DXSOUND_H__

#include "DxCompileConfig.h"

#ifndef DX_NON_SOUND

// �C���N���[�h ------------------------------------------------------------------
#include "DxLib.h"
#include "DxStatic.h"
#include "DxRingBuffer.h"
#include "DxSoundConvert.h"
#include "DxHandle.h"
#include "DxFile.h"

#ifdef __WINDOWS__
#include "Windows/DxSoundWin.h"
#endif // __WINDOWS__

#ifdef __psp2__
#include "PSVita/DxSoundPSVita.h"
#endif // __psp2__

#ifdef __ORBIS__
#include "PS4/DxSoundPS4.h"
#endif // __ORBIS__

#ifdef __ANDROID__
#include "Android/DxSoundAndroid.h"
#endif // __ANDROID__

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

#define MAX_SOUNDBUFFER_NUM				(10)			// DIRECTSOUNDBUFFER �o�b�t�@�̍ő吔
#define STS_SETSOUNDNUM					(4)				// �X�g���[�����T�E���h�Đ��ɓo�^�ł���T�E���h�̐�

#define SOUNDBUFFER_MAX_CHANNEL_NUM		(8)				// �Ή�����T�E���h�`�����l���̍ő吔

#define SOUND_SINTABLE_DIV				(65536)			// �T�C���e�[�u���̐��x


// �T�E���h�n���h���G���[�`�F�b�N�p�}�N��
#define SOUNDHCHK( HAND, SPOINT )			HANDLECHK(       DX_HANDLETYPE_SOUND, HAND, *( ( HANDLEINFO ** )&SPOINT ) )
#define SOUNDHCHK_ASYNC( HAND, SPOINT )		HANDLECHK_ASYNC( DX_HANDLETYPE_SOUND, HAND, *( ( HANDLEINFO ** )&SPOINT ) )

// �\�t�g�T�E���h�n���h���G���[�`�F�b�N�p�}�N��
#define SSND_MASKHCHK( HAND, SPOINT )		HANDLECHK(       DX_HANDLETYPE_SOFTSOUND, HAND, *( ( HANDLEINFO ** )&SPOINT ) )
#define SSND_MASKHCHK_ASYNC( HAND, SPOINT )	HANDLECHK_ASYNC( DX_HANDLETYPE_SOFTSOUND, HAND, *( ( HANDLEINFO ** )&SPOINT ) )

// �~���[�W�b�N�n���h���G���[�`�F�b�N�p�}�N��
#define MIDI_MASKHCHK( HAND, SPOINT )		HANDLECHK(       DX_HANDLETYPE_MUSIC, HAND, *( ( HANDLEINFO ** )&SPOINT ) )
#define MIDI_MASKHCHK_ASYNC( HAND, SPOINT )	HANDLECHK_ASYNC( DX_HANDLETYPE_MUSIC, HAND, *( ( HANDLEINFO ** )&SPOINT ) )


// �\���̒�` --------------------------------------------------------------------

// �t�@�C������T�E���h�n���h�����쐬���鏈���ɕK�v�ȃO���[�o���f�[�^��Z�߂�����
struct LOADSOUND_GPARAM
{
	int							NotInitSoundMemDelete ;					// InitSoundMem �ō폜���Ȃ����ǂ����̃t���O( TRUE:InitSoundMem�ł͍폜���Ȃ�  FALSE:InitSoundMem�ō폜���� )
	int							Create3DSoundFlag ;						// 3D�T�E���h���쐬���邩�ǂ����̃t���O( TRUE:�R�c�T�E���h���쐬����  FALSE:�R�c�T�E���h���쐬���Ȃ� )
	int							CreateSoundDataType ;					// �쐬����T�E���h�f�[�^�̃f�[�^�^�C�v
	int							CreateSoundPitchRateEnable ;			// �쐬����T�E���h�f�[�^�̃s�b�`���[�g���L�����ǂ���
	float						CreateSoundPitchRate ;					// �쐬����T�E���h�f�[�^�̃s�b�`���[�g
	int							CreateSoundTimeStretchRateEnable ;		// �쐬����T�E���h�f�[�^�̃^�C���X�g���b�`���[�g���L�����ǂ���
	float						CreateSoundTimeStretchRate ;			// �쐬����T�E���h�f�[�^�̃^�C���X�g���b�`���[�g
	int							CreateSoundLoopStartTimePosition ;		// �쐬����T�E���h�f�[�^�̃��[�v�͈͂̐�[( �~���b )
	int							CreateSoundLoopStartSamplePosition ;	// �쐬����T�E���h�f�[�^�̃��[�v�͈͂̐�[( �T���v�� )
	int							CreateSoundLoopEndTimePosition ;		// �쐬����T�E���h�f�[�^�̃��[�v�͈͂̏I�[( �~���b )
	int							CreateSoundLoopEndSamplePosition ;		// �쐬����T�E���h�f�[�^�̃��[�v�͈͂̏I�[( �T���v�� )
	int							DisableReadSoundFunctionMask ;			// �g�p���Ȃ��ǂݍ��ݏ����̃}�X�N
#ifndef DX_NON_OGGVORBIS
	int							OggVorbisBitDepth ;						// �n�����u�����������g�p���̃r�b�g�[�x(1:8bit 2:16bit)
	int							OggVorbisFromTheoraFile ;				// Ogg Theora �t�@�C������ Vorbis �f�[�^���Q�Ƃ��邩�ǂ����̃t���O( TRUE:Theora �t�@�C������ Vorbis �f�[�^���Q�Ƃ��� )
#endif
} ;

// �T�E���h�o�b�t�@�����b�N�����ۂɎg�p����f�[�^�^
struct SOUNDBUFFERLOCKDATA
{
	DWORD						StartOffst ;
	BYTE						*WriteP, *WriteP2 ;
	DWORD						Length, Length2 ;
	DWORD						Offset, Offset2 ;
	DWORD						Valid, Valid2 ;
} ;

// �g�`�f�[�^�\����
struct WAVEDATA
{
	void						*Buffer ;				// �g�`�f�[�^
	void						*DoubleSizeBuffer ;		// �Q�{�e�ʂ̔g�`�f�[�^
	int							Bytes ;					// �T�C�Y
	int							RefCount ;				// �Q�Ɛ�
} ;

// �R�c�T�E���h�f�[�^�̊�{�I�ȏ��
struct _3DSOUNDINFO
{
	VECTOR						Position ;				// �ʒu
	VECTOR						FrontDirection ;		// �O������
	VECTOR						UpDirection ;			// �����
	VECTOR						Velocity ;				// �ړ����x
	float						InnerAngle ;			// �����p�x
	float						OuterAngle ;			// ���O�p�x
	float						InnerVolume ;			// �����p�x�ł̉��ʃX�P�[��
	float						OuterVolume ;			// ���O�p�x�ł̉��ʃX�P�[��
} ;

// �T�E���h�o�b�t�@�\����
struct SOUNDBUFFER
{
	int							Valid ;					// �f�[�^���L�����ۂ�(TRUE:�L�� FALSE:����)

	int							Pan ;					// �p��( 10000:��100%�E0%  0:���E100%  -100000:�E100% )
	int							Volume[ SOUNDBUFFER_MAX_CHANNEL_NUM ] ;	// �{�����[��( 10000:0%  0:100% )
	int							Frequency ;				// �Đ����g��( -1:�f�t�H���g )
	int							SampleNum ;				// �T���v���̐�

	WAVEFORMATEX				Format ;				// �o�b�t�@�̃t�H�[�}�b�g

	int							UseGetCurrentPosition ;	// GetCurrentPosition ���g�p���邩�ǂ���
	int							IsStream ;				// �X�g���[���Đ��p�̃T�E���h�o�b�t�@���ǂ���

	// DirectSound �ȊO�̏ꍇ�Ɏg�p
	WAVEDATA					*Wave ;					// �g�`�f�[�^�o�b�t�@
	volatile int				State ;					// �Đ����( TRUE:�Đ���  FALSE:��~�� )
	volatile int				Pos ;					// �Đ��ʒu
	volatile int				CompPos ;				// �]�������������ʒu
	int							Loop ;					// ���[�v���邩�ǂ���( TRUE:���[�v����  FALSE:���[�v���Ȃ� )
	int							CalcPan ;				// �L���v�`���p�Ɍv�Z������̃p��( 256:��100%�E0%  0:���E100%  -256:�E100% )
	int							CalcVolume ;			// �L���v�`���p�Ɍv�Z������̃{�����[��( 256:100%  0:0% )
	volatile int				StopTimeState ;			// StopTime �̏��( 0:���얳�� 1:�Đ��I���҂� 2:���Ԍo�ߑ҂� )
	volatile int				StopTime ;				// �Đ�����~��������

	// �R�c�T�E���h�Đ��p�f�[�^
	int							Is3DSound ;												// �R�c�T�E���h���ǂ���
	_3DSOUNDINFO				EmitterInfo ;											// �R�c�T�E���h�p�̏��
	float						EmitterRadius ;											// �R�c�T�E���h�p�����̉����͂�����
	float						EmitterInnerRadius ;									// �R�c�T�E���h�p�����̉������ʂP�O�O���ŕ������鋗��
	int							EmitterDataChangeFlag ;									// �R�c�T�E���h�p������񂪕ύX���ꂽ���ǂ���( TRUE:�ύX���ꂽ  FALSE:�ύX����Ă��Ȃ� )
	float						DSound_Calc3DVolume ;									// DirectSound�p�̂R�c�v�Z��̃{�����[��
	float						DSound_Calc3DPan ;										// DirectSound�p�̂R�c�v�Z��̃p��

	SOUNDBUFFER_PF				PF ;					// ���ˑ����
} ;

// �X�g���[�����T�E���h�v���C�p�t�@�C���f�[�^
struct STREAMFILEDATA
{
	char						LoopNum ;					// ���[�v��( -1:�������[�v )
	int							DataType ;					// �f�[�^�̃^�C�v( DX_SOUNDDATATYPE_MEMNOPRESS �� )
	void						*FileImage ;				// ��������ɓW�J�����t�@�C���C���[�W( DX_SOUNDDATATYPE_FILE �ȊO�Ŏg�p )
	int							FileImageSize ;				// ��������ɓW�J�����t�@�C���C���[�W�̃T�C�Y( DX_SOUNDDATATYPE_FILE �ȊO�Ŏg�p )
	STREAMDATA					MemStream ;					// �������X�g���[�������p�f�[�^( DX_SOUNDDATATYPE_FILE �ȊO�Ŏg�p )
	int							LoopSamplePosition ;		// ���[�v��̍Đ��ʒu(�T���v���P�ʁAOGG �� �����kWAV �̂ݗL�� )
	int							LoopStartSamplePosition ;	// ���[�v���J�n����Đ��ʒu(�T���v���P�ʁAOGG �� �����kWAV �̂ݗL�� )
	
	SOUNDCONV					ConvData ;					// �t�H�[�}�b�g�ϊ������p�f�[�^
} ;

// �X�g���[�����T�E���h�v���C�p�f�[�^
struct STREAMPLAYDATA
{
	int							BufferBorrowSoundHandle ;								// DirectSoundBuffer ��݂�Ă���T�E���h�n���h��(-1:�؂�Ă��Ȃ�)
	int							BufferBorrowSoundHandleCount ;							// DirectSoundBuffer ��݂��Ă��鐔(0:�݂��Ă��Ȃ�)
	int							BufferUseSoundHandle ;									// DirectSoundBuffer �����ݎg�p���Ă���T�E���h�n���h��(-1:�N�����͎g���Ă��Ȃ�)
	int							DeleteWaitFlag ;										// DirectSoundBuffer ��݂��Ă��邩��폜�ł��Ȃ��ŋ���̂��A�t���O
	
	BYTE						*SoundBuffer ;											// �T�E���h�o�b�t�@( XAudio2�p )
	DWORD						SoundBufferSize ;										// DirectSoundBuffer �̃T�C�Y( �P�ʂ̓o�C�g )
	int							SoundBufferCompCopyOffset ;								// �O��̃T�E���h�f�[�^�R�s�[�œ]�������������T�E���h�f�[�^�̈�̖��[�I�t�Z�b�g
	int							SoundBufferCopyStartOffset ;							// �w�����܂ł����玟�̃T�E���h�f�[�^�R�s�[���J�n����x�Ƃ����f�[�^�I�t�Z�b�g

	STREAMFILEDATA				File[ STS_SETSOUNDNUM ] ;								// �X�g���[���t�@�C���f�[�^
	BYTE						FileLibraryLoadFlag[ STS_SETSOUNDNUM ] ;				// ���C�u���������[�h�����f�[�^���ǂ���
	int							FileNum ;												// �X�g���[���t�@�C���f�[�^�̐�
	int							FileActive ;											// �A�N�e�B�u�ȃX�g���[���t�@�C���f�[�^�ԍ�
	int							FileLoopCount ;											// ���݃A�N�e�B�u�ȃX�g���[���f�[�^�����[�v������
	int							FileCompCopyLength ;									// ���݃A�N�e�B�u�ȃX�g���[���f�[�^���̃T�E���h�o�b�t�@�ւ̓]�������������o�C�g��
	int							TotalSample ;											// �S�X�g���[���̃T���v������

	DWORD						CompPlayWaveLength ;									// �Đ����I������g�`�f�[�^�̒���
	DWORD						PrevCheckPlayPosition ;									// �O��`�F�b�N�����Đ��ʒu

	BYTE						StartSetupCompFlag ;									// �Đ����������t���O
	BYTE						EndWaitFlag ;											// �I���҂����t���O
	BYTE						LoopPositionValidFlag ;									// ���[�v�ʒu���L���t���O
	BYTE						AllPlayFlag ;											// �S�̂�ʂ��Ĉ�x�ł��Ō�ł��Đ��������t���O
	DWORD						LoopAfterCompPlayWaveLength ;							// ���[�v��� CompPlayWaveLength �ɉ��Z����l
	
	DWORD						EndOffset ;												// �I���I�t�Z�b�g
	DWORD						EndStartOffset ;										// �I�������J�n�I�t�Z�b�g
	DWORD						LoopPosition ;											// ���[�v�ʒu(�Đ��o�b�t�@���̃A�h���X)

	HANDLELIST					StreamSoundList ;										// �X�g���[���T�E���h���X�g�����p�\����
} ;

// �m�[�}���T�E���h�v���C�p�f�[�^
struct NORMALPLAYDATA
{
	int							WaveSize ;												// �g�`�f�[�^�̃T�C�Y
	void						*WaveData ;												// �g�`�f�[�^
	int							*WaveDataUseCount ;										// �g�`�f�[�^�Q�Ɛ�
	int							BufferDuplicateFlag[ MAX_SOUNDBUFFER_NUM ] ;			// �������ꂽ�T�E���h�o�b�t�@���ǂ����̃t���O
	int							BackPlayBufferNo ;										// ��O�ɍĐ������T�E���h�o�b�t�@�̔ԍ�(�Đ��p�o�b�t�@����������ꍇ�p)
} ;

// �T�E���h�f�[�^
struct SOUND
{
	HANDLEINFO					HandleInfo ;											// �n���h�����ʏ��

	HANDLELIST					_3DSoundList ;											// �R�c�T�E���h���X�g�����p�\����
	int							Is3DSound ;												// �R�c�T�E���h���ǂ���( TRUE:�R�c�T�E���h  FALSE:��R�c�T�E���h )
	int							AddPlay3DSoundList ;									// Play3DSoundList �����X�g�ɒǉ�����Ă��邩�ǂ���( TRUE:�ǉ�����Ă���  FALSE:�ǉ�����Ă��Ȃ� )
	HANDLELIST					Play3DSoundList ;										// �Đ����̂R�c�T�E���h�̃��X�g�����p�\����

	int							PlayFinishDeleteFlag ;									// �T�E���h�̍Đ����I��������폜���邩�ǂ���( TRUE:�폜����  FALSE:�폜���Ȃ� )
	HANDLELIST					PlayFinishDeleteSoundList ;								// �T�E���h�̍Đ����I��������폜����T�E���h�̃��X�g�����p�\����

	int							NotInitSoundMemDelete ;									// InitSoundMem �ō폜���Ȃ����ǂ����̃t���O( TRUE:InitSoundMem�ł͍폜���Ȃ�  FALSE:InitSoundMem�ō폜���� )

//	char						FilePath[ 256 ] ;										// �f�B���N�g��
//	char						SoundName[ 256 ] ;										// �T�E���h�t�@�C���l�[��

	int							ValidBufferNum ;										// �L���ȃo�b�t�@�̐�
	int							BufferPlayStateBackupFlagValid[ MAX_SOUNDBUFFER_NUM ] ;	// BufferPlayStateBackupFlag ���L�����ǂ����̃t���O( TRUE:�L��  FALSE:���� )
	int							BufferPlayStateBackupFlag[ MAX_SOUNDBUFFER_NUM ] ;		// �T�E���h�o�b�t�@���ꎞ��~����Ƃ��̍Đ���ԃt���O
	SOUNDBUFFER					Buffer[ MAX_SOUNDBUFFER_NUM ] ;							// �T�E���h�o�b�t�@
	WAVEFORMATEX				BufferFormat ;											// �T�E���h�o�b�t�@�̃t�H�[�}�b�g

	int							Type ;													// �T�E���h�̃^�C�v
	int							PlayType ;												// �Đ��^�C�v

	STREAMPLAYDATA				Stream ;												// �X�g���[�����T�E���h�v���C�p�f�[�^
	NORMALPLAYDATA				Normal ;												// �m�[�}���T�E���h�v���C�p�f�[�^

	int							PresetReverbParam ;										// �R�c�T�E���h���ɐݒ肷��v���Z�b�g���o�[�u�p�����[�^�ԍ�( DX_REVERB_PRESET_DEFAULT �� )
	SOUND3D_REVERB_PARAM		ReverbParam ;											// �R�c�T�E���h���ɐݒ肷�郊�o�[�u�p�����[�^( PresetReverbParam �� -1 �̍ۂɎg�p )

	int							BaseVolume[ SOUNDBUFFER_MAX_CHANNEL_NUM ] ;				// ��{�{�����[��( -1:�f�t�H���g )
	int							BasePan ;												// ��{�p��( -1:�f�t�H���g )
	int							BaseFrequency ;											// ��{�Đ����g��( -1:�f�t�H���g )
	VECTOR						Base3DPosition ;										// ��{�Đ��ʒu
	float						Base3DRadius ;											// ��{�������鋗��
	VECTOR						Base3DVelocity ;										// ��{���̑��x
	int							PitchRateEnable ;										// �s�b�`���[�g���L�����ǂ���( TRUE:�L��  FALSE:���� )
	float						PitchRate ;												// �s�b�`���[�g

	BYTE						ValidNextPlayVolume[ SOUNDBUFFER_MAX_CHANNEL_NUM ] ;	// NextPlayVolume ���L�����ǂ���( 1:�L�� 0:���� )
	BYTE						ValidNextPlayPan ;										// NextPlayPan ���L�����ǂ���( 1:�L�� 0:���� )
	BYTE						ValidNextPlayFrequency ;								// NextPlayFrequency ���L�����ǂ���( 1:�L�� 0:���� )
	BYTE						ValidNextPlay3DPosition ;								// NextPlay3DPosition ���L�����ǂ���( 1:�L�� 0:���� )
	BYTE						ValidNextPlay3DRadius ;									// NextPlay3DRadius ���L�����ǂ���( 1:�L�� 0:���� )
	BYTE						ValidNextPlay3DVelocity ;								// NextPlay3DVelocity ���L�����ǂ���( 1:�L�� 0:���� )
	int							NextPlayVolume[ SOUNDBUFFER_MAX_CHANNEL_NUM ] ;			// ���ɖ炷���̃{�����[��
	int							NextPlayPan ;											// ���ɖ炷���̃p��
	int							NextPlayFrequency ;										// ���ɖ炷���̎��g��
	VECTOR						NextPlay3DPosition ;									// ���ɖ炷���̈ʒu
	float						NextPlay3DRadius ;										// ���ɖ炷���̕������鋗��
	VECTOR						NextPlay3DVelocity ;									// ���ɖ炷���̈ړ����x
} ;

// �\�t�g�E�G�A�ň����g�`�f�[�^�̔g�`�f�[�^�\����
struct SOFTSOUND_WAVE
{
	int							BufferSampleNum ;										// �o�b�t�@�̃T�C�Y( �T���v���� )
	void *						Buffer ;												// �f�[�^�o�b�t�@
} ;

// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�\����
struct SOFTSOUND_PLAYER
{
	HANDLELIST					SoftSoundPlayerList ;									// �\�t�g�E�G�A�T�E���h�v���C���[���X�g�����p�\����

	int							StockSampleNum ;										// �X�g�b�N���Ă���T���v���̐�
	RINGBUF						StockSample ;											// �T���v���X�g�b�N�p�����O�o�b�t�@

	int							SoundBufferPlayStateBackupFlagValid ;					// SoundBufferPlayStateBackupFlag ���L�����ǂ����̃t���O( TRUE:�L��  FALSE:���� )
	int							SoundBufferPlayStateBackupFlag ;						// �T�E���h�o�b�t�@���ꎞ��~����Ƃ��̍Đ���ԃt���O
	SOUNDBUFFER					SoundBuffer ;											// �Đ��p�T�E���h�o�b�t�@
	int							SoundBufferSize ;										// �Đ��p�T�E���h�o�b�t�@�̃T�C�Y( �o�C�g�P�� )
//	int							BackCheckPlayOffset ;									// ��O�Ƀ`�F�b�N�����Đ��ʒu
	int							DataSetCompOffset ;										// �f�[�^�Z�b�g���ς�ł���T�E���h�o�b�t�@��̈ʒu( -1:�܂��Z�b�g���Ă��Ȃ� )

	int							NoneDataSetCompOffset ;									// �����f�[�^�̃Z�b�g���J�n�����T�E���h�o�b�t�@��̈ʒu( -1:�܂��Z�b�g���Ă��Ȃ� )
	int							NoneDataPlayCheckBackPlayOffset ;						// �����f�[�^�̍Đ����J�n�����������o���邽�߂̑O��`�F�b�N���̍Đ��ʒu
	int							NoneDataPlayStartFlag ;									// �����f�[�^�̍Đ����J�n���Ă��邩�ǂ����t���O( TRUE:���Ă���  FALSE:���Ă��Ȃ� )

	int							MaxDataSetSize ;										// �Đ��ʒu�ɐ�s���ăZ�b�g����ő�f�[�^�T�C�Y�A�o�C�g�P��( ��s���ăZ�b�g���Ă���f�[�^�̃T�C�Y�����̃T�C�Y�ȏ�ɂȂ�Ȃ��悤�ɂ��� )
	int							MinDataSetSize ;										// �Đ��ʒu�ɐ�s���ăZ�b�g����ŏ��f�[�^�T�C�Y�A�o�C�g�P��( ��s���ăZ�b�g���Ă���f�[�^�̃T�C�Y�����̃T�C�Y�ȉ��ɂȂ�Ȃ��悤�ɂ��� )

	int							IsPlayFlag ;											// �Đ������ǂ����̃t���O( TRUE:�Đ���   FALSE:��~�� )
} ;

// �\�t�g�E�G�A�ň����g�`�f�[�^
struct SOFTSOUND
{
	HANDLEINFO					HandleInfo ;											// �n���h�����ʏ��

	int							IsPlayer ;												// �v���C���[���ǂ����̃t���O( TRUE:�v���C���[  FALSE:�T�E���h�f�[�^ )
	WAVEFORMATEX				BufferFormat ;											// �T�E���h�̃t�H�[�}�b�g

	union
	{
		SOFTSOUND_WAVE			Wave ;													// �g�`�f�[�^
		SOFTSOUND_PLAYER		Player ;												// �v���C���[
	} ;
} ;


// �T�E���h�V�X�e���p�f�[�^�\����
struct SOUNDSYSTEMDATA
{
	int							InitializeFlag ;				// �������t���O

	DWORD						OutputChannels ;				// �o�̓`�����l����
	DWORD						OutputSmaplesPerSec ;			// �o�̓T���v�����O���[�g

	int							_3DSoundOneMetreEnable ;		// _3DSoundOneMetre ���L�����ǂ���( TRUE:�L��  FALSE:���� )
	float						_3DSoundOneMetre ;				// �R�c�T�E���h�����p�̂P���[�g��
	_3DSOUNDINFO				ListenerInfo ;					// �R�c�T�E���h�̃��X�i�[�̏��
	VECTOR						ListenerSideDirection ;			// �R�c�T�E���h�̃��X�i�[�̉�����

	HANDLELIST					_3DSoundListFirst ;				// �R�c�T�E���h�n���h�����X�g�̐擪
	HANDLELIST					_3DSoundListLast ;				// �R�c�T�E���h�n���h�����X�g�̏I�[
	DX_CRITICAL_SECTION			_3DSoundListCriticalSection ;	// �R�c�T�E���h���X�g�A�N�Z�X���p�N���e�B�J���Z�N�V����

	HANDLELIST					StreamSoundListFirst ;			// �X�g���[���T�E���h�n���h�����X�g�̐擪
	HANDLELIST					StreamSoundListLast ;			// �X�g���[���T�E���h�n���h�����X�g�̏I�[
	DX_CRITICAL_SECTION			StreamSoundListCriticalSection ;// �X�g���[���T�E���h�n���h�����X�g�A�N�Z�X���p�N���e�B�J���Z�N�V����

	HANDLELIST					SoftSoundPlayerListFirst ;		// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�̃n���h�����X�g�̐擪
	HANDLELIST					SoftSoundPlayerListLast ;		// �\�t�g�E�G�A�ň����g�`�f�[�^�̃v���C���[�̃n���h�����X�g�̏I�[

	HANDLELIST					PlayFinishDeleteSoundListFirst ; // �Đ��I�����ɍ폜����T�E���h�n���h�����X�g�̐擪
	HANDLELIST					PlayFinishDeleteSoundListLast ;	 // �Đ��I�����ɍ폜����T�E���h�n���h�����X�g�̏I�[

	HANDLELIST					Play3DSoundListFirst ;			// �Đ����Ă���R�c�T�E���h���X�g�̐擪
	HANDLELIST					Play3DSoundListLast ;			// �Đ����Ă���R�c�T�E���h���X�g�̏I�[
	DX_CRITICAL_SECTION			Play3DSoundListCriticalSection ;// �Đ����Ă���R�c�T�E���h���X�g�A�N�Z�X���p�N���e�B�J���Z�N�V����

	int							PlayWavSoundHandle ;			// PlayWav�֐��Ŗ炳��Ă���WAVE�f�[�^�̃n���h��

	int							Create3DSoundFlag ;				// 3D�T�E���h���쐬���邩�ǂ����̃t���O( TRUE:�R�c�T�E���h���쐬����  FALSE:�R�c�T�E���h���쐬���Ȃ� )
	int							OldVolumeTypeFlag ;				// Ver3.10c�ȑO�̉��ʌv�Z�����g�p���邩�ǂ����̃t���O( TRUE:�Â��v�Z�����g�p����  FALSE:�V�����v�Z�����g�p���� )
	int							SoundMode ;						// �Đ��`��

	int							MaxVolume ;						// �ő剹��

	int							EnableSoundCaptureFlag ;		// �T�E���h�L���v�`����O��Ƃ������������
#ifndef DX_NON_SAVEFUNCTION
	int							SoundCaptureFlag ;				// �T�E���h�L���v�`�������s���Ă���Œ����ǂ����̃t���O(TRUE:�Œ� FASLE:�Ⴄ)
	int							SoundCaptureSample ;			// �L���v�`�������T���v���̐�(44.1KHz���Z)
	DWORD_PTR					SoundCaptureFileHandle ;		// �L���v�`�������T�E���h��ۑ����Ă���t�@�C��
#endif // DX_NON_SAVEFUNCTION

	int							CreateSoundDataType ;					// �쐬����T�E���h�f�[�^�̃f�[�^�^�C�v
	int							CreateSoundPitchRateEnable ;			// �쐬����T�E���h�f�[�^�̃s�b�`���[�g���L�����ǂ���
	float						CreateSoundPitchRate ;					// �쐬����T�E���h�f�[�^�̃s�b�`���[�g
	float						CreateSoundPitchRate_Cents ;			// �쐬����T�E���h�f�[�^�̃s�b�`���[�g( �Z���g�P�� )
	int							CreateSoundTimeStretchRateEnable ;		// �쐬����T�E���h�f�[�^�̃^�C���X�g���b�`���[�g���L�����ǂ���
	float						CreateSoundTimeStretchRate ;			// �쐬����T�E���h�f�[�^�̃^�C���X�g���b�`���[�g
	int							CreateSoundLoopStartTimePosition ;		// �쐬����T�E���h�f�[�^�̃��[�v�͈͂̐�[( �~���b )
	int							CreateSoundLoopStartSamplePosition ;	// �쐬����T�E���h�f�[�^�̃��[�v�͈͂̐�[( �T���v�� )
	int							CreateSoundLoopEndTimePosition ;		// �쐬����T�E���h�f�[�^�̃��[�v�͈͂̏I�[( �~���b )
	int							CreateSoundLoopEndSamplePosition ;		// �쐬����T�E���h�f�[�^�̃��[�v�͈͂̏I�[( �T���v�� )
	int							DisableReadSoundFunctionMask ;			// �g�p���Ȃ��ǂݍ��ݏ����̃}�X�N
#ifndef DX_NON_OGGVORBIS
	int							OggVorbisBitDepth ;				// �n�����u�����������g�p���̃r�b�g�[�x(1:8bit 2:16bit)
	int							OggVorbisFromTheoraFile ;		// Ogg Theora �t�@�C������ Vorbis �f�[�^���Q�Ƃ��邩�ǂ����̃t���O( TRUE:Theora �t�@�C������ Vorbis �f�[�^���Q�Ƃ��� )
#endif

	short						SinTable[ SOUND_SINTABLE_DIV ] ;	// �T�C���e�[�u��

	SOUNDSYSTEMDATA_PF			PF ;							// ���ˑ����
} ;

// �l�h�c�h�f�[�^
struct MIDIHANDLEDATA
{
	HANDLEINFO					HandleInfo ;					// �n���h�����ʃf�[�^

	int							PlayFlag ;						// �Đ������A�t���O
	int							PlayStartFlag ;					// �Đ����J�n���ꂽ���A�t���O
	int							Volume ;						// ����

	void						*DataImage ;					// �l�h�c�h�t�@�C���̃f�[�^���i�[����Ă���A�h���X( DSOUND.SoundMode �� DX_MIDIMODE_MCI �̎��̂ݗL�� )
	int							DataSize ;						// �l�h�c�h�t�@�C���̃f�[�^�T�C�Y( DSOUND.SoundMode �� DX_MIDIMODE_MCI �̎��̂ݗL�� )

	int							StartTime ;						// �Đ��J�n����

	MIDIHANDLEDATA_PF			PF ;							// ���ˑ����
} ;

// �l�h�c�h�V�X�e���p�f�[�^�\����
struct MIDISYSTEMDATA
{
	int							PlayFlag ;						// ���t���t���O
	int							PlayHandle ;					// ���t���̃n���h��

	wchar_t						FileName[ FILEPATH_MAX ] ;		// �t�@�C���l�[��

	int							LoopFlag ;						// ���[�v���t���A�t���O

	int							StartTime ;						// �Đ��J�n����

	int							MemImagePlayFlag ;				// ��������̃C���[�W���g�p���čĐ����Ă��邩�A�t���O( TRUE:�������C���[�W  FALSE:�t�@�C�� )

	int							DefaultHandle ;					// �W���̍Đ��֐��Ŏg�p����MIDI�n���h��
	int							DefaultHandleToSoundHandleFlag ;	// �W���̍Đ��֐��ōĐ����Ă���n���h���͉����n���h�����A�t���O

	MIDISYSTEMDATA_PF			PF ;							// ���ˑ����
} ;

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �T�E���h�V�X�e���f�[�^
extern SOUNDSYSTEMDATA SoundSysData ;

// �l�h�c�h�Ǘ��p�f�[�^
extern MIDISYSTEMDATA MidiSystemData ;	

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

extern	int		InitializeSoundSystem( void ) ;																			// �T�E���h�V�X�e��������������
extern	int		TerminateSoundSystem( void ) ;																			// �T�E���h�V�X�e���̌�n��������

extern	int		RefreshStreamSoundPlayCompLength( int SoundHandle, int CurrentPosition = -1, int ASyncThread = FALSE ) ;	// �T�E���h�n���h���̍Đ��������Ԃ��X�V����
extern	int		ST_SoftSoundPlayerProcessAll( void ) ;																	// �\�t�g�E�G�A�Ő��䂷��S�ẴT�E���h�f�[�^�v���C���[�̒���������s��

extern	int		StartSoundCapture( const wchar_t *SaveFilePath ) ;															// �T�E���h�L���v�`���̊J�n
extern	int		SoundCaptureProcess( int CaptureSample ) ;																// �T�E���h�L���v�`���̎����I����
extern	int		EndSoundCapture( void ) ;																				// �T�E���h�L���v�`���̏I��

extern	void	InitLoadSoundGParam( LOADSOUND_GPARAM *GParam ) ;														// LOADSOUND_GPARAM �̃f�[�^���Z�b�g����

extern	int		InitializeSoundHandle( HANDLEINFO *HandleInfo ) ;														// �T�E���h�n���h���̏�����
extern	int		TerminateSoundHandle( HANDLEINFO *HandleInfo ) ;														// �T�E���h�n���h���̌�n��

extern	int		LoadSoundMemBase_UseGParam(              LOADSOUND_GPARAM *GParam, const wchar_t *WaveName, int BufferNum, int UnionHandle, int ASyncLoadFlag = FALSE, int ASyncThread = FALSE ) ;																		// LoadSoundMemBase �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		LoadSoundMemByMemImageBase_UseGParam(    LOADSOUND_GPARAM *GParam, int CreateSoundHandle, int SoundHandle, const void *FileImageBuffer, int ImageSize, int BufferNum, int UnionHandle = -1, int ASyncLoadFlag = FALSE, int ASyncThread = FALSE ) ;		// LoadSoundMemByMemImageBase �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		LoadSoundMem2_UseGParam(                 LOADSOUND_GPARAM *GParam, const wchar_t *WaveName1 , const wchar_t *WaveName2, int ASyncLoadFlag = FALSE ) ;																									// LoadSoundMem2 �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		LoadSoundMem2ByMemImage_UseGParam(       LOADSOUND_GPARAM *GParam, const void *FileImageBuffer1, int ImageSize1, const void *FileImageBuffer2, int ImageSize2, int ASyncLoadFlag = FALSE ) ;															// LoadSoundMem2ByMemImage �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����

extern	int		AddStreamSoundMem_UseGParam(       LOADSOUND_GPARAM *GParam, STREAMDATA *Stream, int LoopNum,  int SoundHandle, int StreamDataType, int *CanStreamCloseFlag, int UnionHandle = -1, int ASyncThread = FALSE ) ;											// AddStreamSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		AddStreamSoundMemToFile_UseGParam( LOADSOUND_GPARAM *GParam, const wchar_t *WaveFile, int LoopNum,  int SoundHandle, int StreamDataType, int UnionHandle = -1, int ASyncThread = FALSE ) ;																// AddStreamSoundMemToFile �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		AddStreamSoundMemToMem_UseGParam(  LOADSOUND_GPARAM *GParam, void *FileImageBuffer, int ImageSize, int LoopNum,  int SoundHandle, int StreamDataType, int UnionHandle = -1, int ASyncThread = FALSE ) ;													// AddStreamSoundMemToMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		SetupStreamSoundMem_UseGParam(     int SoundHandle, int ASyncThread = FALSE ) ;																																											// SetupStreamSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		ProcessStreamSoundMem_UseGParam(   int SoundHandle, int ASyncThread = FALSE ) ;																																											// ProcessStreamSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����

extern	int		SetLoopAreaTimePosSoundMem_UseGParam(    int LoopStartTime,           int LoopEndTime,           int SoundHandle, int ASyncThread = FALSE ) ;																											// SetLoopAreaTimePosSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		SetLoopAreaSamplePosSoundMem_UseGParam(  int LoopStartSamplePosition, int LoopEndSamplePosition, int SoundHandle, int ASyncThread = FALSE ) ;																											// SetLoopAreaSamplePosSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		SetLoopTimePosSoundMem_UseGParam(        int LoopTime,                int SoundHandle, int ASyncThread = FALSE ) ;																																		// SetLoopTimePosSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		SetLoopSamplePosSoundMem_UseGParam(      int LoopSamplePosition,      int SoundHandle, int ASyncThread = FALSE ) ;																																		// SetLoopSamplePosSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		SetLoopStartTimePosSoundMem_UseGParam(   int LoopStartTime,           int SoundHandle, int ASyncThread = FALSE ) ;																																		// SetLoopStartTimePosSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		SetLoopStartSamplePosSoundMem_UseGParam( int LoopStartSamplePosition, int SoundHandle, int ASyncThread = FALSE ) ;																																		// SetLoopStartTimePosSoundMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����

extern	int		GetSoundTotalSample_UseGParam( int SoundHandle, int ASyncThread = FALSE ) ;																																												// GetSoundTotalSample �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		SetStreamSoundCurrentPosition_UseGParam( int Byte, int SoundHandle, int ASyncThread = FALSE ) ;																																							// SetStreamSoundCurrentPosition �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����

extern	int		Refresh3DSoundParamAll() ;																				// �Đ����̂��ׂĂ̂R�c�T�E���h�̃p�����[�^���X�V����

extern	int		ProcessPlayFinishDeleteSoundMemAll( void ) ;															// �Đ����I��������T�E���h�n���h�����폜����T�E���h�̏������s��
extern	int		ProcessPlay3DSoundMemAll( void ) ;																		// �R�c�T�E���h���Đ����Ă���T�E���h�n���h���ɑ΂��鏈�����s��

extern	int		SetupSoftSoundHandle( int SoftSoundHandle, int IsPlayer, int Channels, int BitsPerSample, int SamplesPerSec, int IsFloatType, int SampleNum ) ;	// �\�t�g�T�E���h�n���h�����Z�b�g�A�b�v

extern	int		InitializeSoftSoundHandle( HANDLEINFO *HandleInfo ) ;													// �\�t�g�T�E���h�n���h���̏�����
extern	int		TerminateSoftSoundHandle( HANDLEINFO *HandleInfo ) ;													// �\�t�g�T�E���h�n���h���̌�n��

extern	int		DeleteCancelCheckSoftSoundFunction( HANDLEINFO *HandleInfo ) ;											// �\�t�g�T�E���h�n���h�����폜���邩�ǂ������`�F�b�N����֐�
extern	int		DeleteCancelCheckSoftSoundPlayerFunction( HANDLEINFO *HandleInfo ) ;									// �\�t�g�T�E���h�v���[���[�n���h�����폜���邩�ǂ������`�F�b�N����֐�

extern	int		LoadSoftSoundBase_UseGParam( LOADSOUND_GPARAM *GParam, const wchar_t *FileName, const void *FileImage, int FileImageSize, int ASyncLoadFlag = FALSE ) ;		// �\�t�g�E�G�A�ň����g�`�f�[�^���t�@�C���܂��̓�������ɓW�J���ꂽ�t�@�C���C���[�W����쐬����
extern	int		MakeSoftSoundBase_UseGParam( int IsPlayer, int Channels, int BitsPerSample, int SamplesPerSec, int SampleNum, int IsFloatType, int UseFormat_SoftSoundHandle = -1, int ASyncLoadFlag = FALSE ) ;	// �\�t�g�E�G�A�Ő��䂷��T�E���h�f�[�^�n���h���̍쐬

extern	int		AddMusicData( void ) ;																					// �V�����l�h�c�h�n���h�����擾����

extern	int		InitializeMidiHandle( HANDLEINFO *HandleInfo ) ;														// �l�h�c�h�n���h����������������֐�
extern	int		TerminateMidiHandle( HANDLEINFO *HandleInfo ) ;															// �l�h�c�h�n���h���̌�n�����s���֐�

extern	int		LoadMusicMemByMemImage_UseGParam( void *FileImage, int FileImageSize, int ASyncLoadFlag = FALSE ) ;		// LoadMusicMemByMemImage �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		LoadMusicMem_UseGParam( const wchar_t *FileName, int ASyncLoadFlag = FALSE ) ;							// LoadMusicMem �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		LoadMusicMemByResource_UseGParam( const wchar_t *ResourceName, const wchar_t *ResourceType, int ASyncLoadFlag = FALSE ) ;	// LoadMusicMemByResource �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		LoadMusicMemByMemImage_Static( int MusicHandle, const void *FileImage, int FileImageSize, int ASyncThread ) ;		// LoadMusicMemByMemImage �̎������֐�

extern	int		PauseSoundMemAll( int PauseFlag ) ;																		// �S�Ẳ��̈ꎞ��~��Ԃ�ύX����
extern	int		PauseSoftSoundAll( int PauseFlag ) ;																	// �S�Ẵ\�t�g�E�G�A�T�E���h�̈ꎞ��~��Ԃ�ύX����

extern	int		CopySoftSound( int SrcSoftSoundHandle, int DestSoftSoundHandle ) ;										// �T�E���h�f�[�^��ʂ̃T�E���h�f�[�^�ɃR�s�[����
extern	int		ConvertIntToFloatSoftSound( int SrcSoftSoundHandle, int DestSoftSoundHandle ) ;							// int�^�̃T�E���h�f�[�^���� float�^�̃T�E���h�f�[�^���Z�b�g�A�b�v����
extern	int		ConvertFloatToIntSoftSound( int SrcSoftSoundHandle, int DestSoftSoundHandle ) ;							// float�^�̃T�E���h�f�[�^���� int�^�̃T�E���h�f�[�^���Z�b�g�A�b�v����





// wchar_t�Ŋ֐�
extern	int		AddStreamSoundMemToFile_WCHAR_T(	const wchar_t *WaveFile, int LoopNum,  int SoundHandle, int StreamDataType, int UnionHandle = -1 ) ;
extern	int		LoadSoundMem2_WCHAR_T(				const wchar_t *FileName1, const wchar_t *FileName2 ) ;

extern	int		LoadBGM_WCHAR_T(					const wchar_t *FileName ) ;

extern	int		LoadSoundMemBase_WCHAR_T(			const wchar_t *FileName, int BufferNum,      int UnionHandle = -1 ) ;
extern	int		LoadSoundMem_WCHAR_T(				const wchar_t *FileName, int BufferNum = 3 , int UnionHandle = -1 ) ;
extern	int		LoadSoundMemToBufNumSitei_WCHAR_T(	const wchar_t *FileName, int BufferNum ) ;
extern	int		LoadSoundMemByResource_WCHAR_T(		const wchar_t *ResourceName, const wchar_t *ResourceType, int BufferNum = 1 ) ;

extern	int		PlaySoundFile_WCHAR_T(				const wchar_t *FileName, int PlayType ) ;
extern	int		PlaySound_WCHAR_T(					const wchar_t *FileName, int PlayType ) ;

extern	int		LoadSoftSound_WCHAR_T(				const wchar_t *FileName ) ;

#ifndef DX_NON_SAVEFUNCTION
extern	int		SaveSoftSound_WCHAR_T(				int SoftSoundHandle, const wchar_t *FileName ) ;
#endif // DX_NON_SAVEFUNCTION

extern	int		LoadMusicMem_WCHAR_T(				const wchar_t *FileName ) ;
extern	int		LoadMusicMemByResource_WCHAR_T(		const wchar_t *ResourceName, const wchar_t *ResourceType ) ;

extern	int		PlayMusic_WCHAR_T(					const wchar_t *FileName, int PlayType ) ;

extern	int		PlayMusicByResource_WCHAR_T(		const wchar_t *ResourceName, const wchar_t *ResourceType, int PlayType ) ;





//�T�E���h�o�b�t�@�p
extern	int		SoundBuffer_Initialize(          SOUNDBUFFER *Buffer, DWORD Bytes, WAVEFORMATEX *Format, SOUNDBUFFER *Src, int UseGetCurrentPosition, int IsStream, int Is3DSound ) ;
extern	int		SoundBuffer_Duplicate(           SOUNDBUFFER *Buffer, SOUNDBUFFER *Src, int Is3DSound ) ;
extern	int		SoundBuffer_Terminate(           SOUNDBUFFER *Buffer ) ;
extern	int		SoundBuffer_CheckEnable(         SOUNDBUFFER *Buffer ) ;
extern	int		SoundBuffer_Play(                SOUNDBUFFER *Buffer, int Loop ) ;
extern	int		SoundBuffer_Stop(                SOUNDBUFFER *Buffer, int EffectStop = FALSE ) ;
extern	int		SoundBuffer_CheckPlay(           SOUNDBUFFER *Buffer ) ;
extern	int		SoundBuffer_Lock(                SOUNDBUFFER *Buffer, DWORD  WritePos, DWORD WriteSize, void **LockPos1, DWORD *LockSize1, void **LockPos2, DWORD *LockSize2 ) ;
extern	int		SoundBuffer_Unlock(              SOUNDBUFFER *Buffer, void  *LockPos1, DWORD LockSize1, void  *LockPos2, DWORD LockSize2 ) ;
extern	int		SoundBuffer_SetFrequency(        SOUNDBUFFER *Buffer, DWORD Frequency ) ;
extern	int		SoundBuffer_GetFrequency(        SOUNDBUFFER *Buffer, DWORD * Frequency ) ;
extern	int		SoundBuffer_SetPan(              SOUNDBUFFER *Buffer, LONG Pan ) ;
extern	int		SoundBuffer_GetPan(              SOUNDBUFFER *Buffer, LPLONG Pan ) ;
extern	int		SoundBuffer_RefreshVolume(       SOUNDBUFFER *Buffer ) ;
extern	int		SoundBuffer_SetVolumeAll(        SOUNDBUFFER *Buffer, LONG Volume ) ;
extern	int		SoundBuffer_SetVolume(           SOUNDBUFFER *Buffer, int Channel, LONG Volume ) ;
extern	int		SoundBuffer_GetVolume(           SOUNDBUFFER *Buffer, int Channel, LPLONG Volume ) ;
extern	int		SoundBuffer_GetCurrentPosition(  SOUNDBUFFER *Buffer, DWORD * PlayPos, DWORD * WritePos ) ;
extern	int		SoundBuffer_SetCurrentPosition(  SOUNDBUFFER *Buffer, DWORD NewPos ) ;
extern	int		SoundBuffer_CycleProcess(        SOUNDBUFFER *Buffer ) ;
extern	int		SoundBuffer_FrameProcess(        SOUNDBUFFER *Buffer, int Sample, short *DestBuf ) ;
extern	int		SoundBuffer_Set3DPosition(       SOUNDBUFFER *Buffer, VECTOR *Position ) ;
extern	int		SoundBuffer_Set3DRadius(         SOUNDBUFFER *Buffer, float Radius ) ;
extern	int		SoundBuffer_Set3DInnerRadius(    SOUNDBUFFER *Buffer, float Radius ) ;
extern	int		SoundBuffer_Set3DVelocity(       SOUNDBUFFER *Buffer, VECTOR *Velocity ) ;
extern	int		SoundBuffer_Set3DFrontPosition(  SOUNDBUFFER *Buffer, VECTOR *FrontPosition, VECTOR *UpVector ) ;
extern	int		SoundBuffer_Set3DConeAngle(      SOUNDBUFFER *Buffer, float InnerAngle, float OuterAngle ) ;
extern	int		SoundBuffer_Set3DConeVolume(     SOUNDBUFFER *Buffer, float InnerAngleVolume, float OuterAngleVolume ) ;
extern	int		SoundBuffer_Refresh3DSoundParam( SOUNDBUFFER *Buffer, int AlwaysFlag = FALSE ) ;
extern	int		SoundBuffer_SetReverbParam(      SOUNDBUFFER *Buffer, SOUND3D_REVERB_PARAM *Param ) ;
extern	int		SoundBuffer_SetPresetReverbParam( SOUNDBUFFER *Buffer, int PresetNo ) ;


// �g�`�f�[�^�p
extern	WAVEDATA *	AllocWaveData( int Size, int UseDoubleSizeBuffer = FALSE ) ;
extern	int			ReleaseWaveData( WAVEDATA *Data ) ;
extern	WAVEDATA *	DuplicateWaveData( WAVEDATA *Data ) ;







// ���ˑ��֐�
extern	int		InitializeSoundSystem_PF_Timing0( void ) ;											// �T�E���h�V�X�e��������������֐��̊��ˑ��������s���֐�( ���s�ӏ���ʂO )
extern	int		InitializeSoundSystem_PF_Timing1( void ) ;											// �T�E���h�V�X�e��������������֐��̊��ˑ��������s���֐�( ���s�ӏ���ʂP )

extern	int		TerminateSoundSystem_PF_Timing0( void ) ;											// �T�E���h�V�X�e���̌�n��������֐��̊��ˑ��������s���֐�( ���s�ӏ���ʂO )
extern	int		TerminateSoundSystem_PF_Timing1( void ) ;											// �T�E���h�V�X�e���̌�n��������֐��̊��ˑ��������s���֐�( ���s�ӏ���ʂP )

extern	int		CheckSoundSystem_Initialize_PF( void ) ;											// �T�E���h�V�X�e���̏������`�F�b�N�̊��ˑ��������s���֐�( TRUE:����������Ă���  FALSE:����������Ă��Ȃ� )

extern	int		TerminateMidiHandle_PF( MIDIHANDLEDATA *MusicData ) ;								// �l�h�c�h�n���h���̌�n�����s���֐��̊��ˑ�����

extern	int		Get3DPresetReverbParamSoundMem_PF( SOUND3D_REVERB_PARAM *ParamBuffer, int PresetNo /* DX_REVERB_PRESET_DEFAULT �� */ ) ;		// �v���Z�b�g�̂R�c�T�E���h�p�̃��o�[�u�p�����[�^���擾���鏈���̊��ˑ��������s���֐�
extern	int		Set3DSoundListenerPosAndFrontPosAndUpVec_PF( VECTOR Position, VECTOR FrontPosition, VECTOR UpVector ) ;							// �R�c�T�E���h�̃��X�i�[�̈ʒu�ƃ��X�i�[�̑O���ʒu�ƃ��X�i�[�̏�����ʒu��ݒ肷�鏈���̊��ˑ��������s���֐�
extern	int		Set3DSoundListenerVelocity_PF( VECTOR Velocity ) ;																				// �R�c�T�E���h�̃��X�i�[�̈ړ����x��ݒ肷�鏈���̊��ˑ��������s���֐�
extern	int		Set3DSoundListenerConeAngle_PF( float InnerAngle, float OuterAngle ) ;															// �R�c�T�E���h�̃��X�i�[�̉��p�x�͈͂�ݒ肷�鏈���̊��ˑ��������s���֐�
extern	int		Set3DSoundListenerConeVolume_PF( float InnerAngleVolume, float OuterAngleVolume ) ;												// �R�c�T�E���h�̃��X�i�[�̉��p�x�͈͂̉��ʔ{����ݒ肷�鏈���̊��ˑ��������s���֐�

extern	int		LoadMusicMemByMemImage_Static_PF( MIDIHANDLEDATA *MusicData, int ASyncThread ) ;												// LoadMusicMemByMemImage �̎������֐��̊��ˑ��������s���֐�
extern	int		PlayMusicMem_PF( MIDIHANDLEDATA *MusicData, int PlayType ) ;																	// �ǂݍ��񂾂l�h�c�h�f�[�^�̉��t���J�n���鏈���̊��ˑ��������s���֐�
extern	int		StopMusicMem_PF( MIDIHANDLEDATA *MusicData ) ;																					// �l�h�c�h�f�[�^�̉��t���~���鏈���̊��ˑ��������s��
extern	int		CheckMusicMem_PF( MIDIHANDLEDATA *MusicData ) ;																					// �l�h�c�h�f�[�^�����t�����ǂ������擾����( TRUE:���t��  FALSE:��~�� )�����̊��ˑ��������s���֐�
extern	int		ProcessMusicMem_PF( MIDIHANDLEDATA *MusicData ) ;																				// �l�h�c�h�f�[�^�̎����I�����̊��ˑ��������s���֐�
extern	int		GetMusicMemPosition_PF( MIDIHANDLEDATA *MusicData ) ;																			// �l�h�c�h�f�[�^�̌��݂̍Đ��ʒu���擾���鏈���̊��ˑ��������s���֐�
extern	int		SetVolumeMusic_PF( int Volume ) ;																								// �l�h�c�h�̍Đ����ʂ��Z�b�g���鏈���̊��ˑ��������s���֐�
extern	int		GetMusicPosition_PF( void ) ;																									// �l�h�c�h�̌��݂̍Đ��ʒu���擾���鏈���̊��ˑ��������s���֐�




extern	int		SoundBuffer_Initialize_Timing0_PF(	SOUNDBUFFER *Buffer, DWORD Bytes, WAVEFORMATEX *Format, SOUNDBUFFER *Src, int Is3DSound ) ;
extern	int		SoundBuffer_Initialize_Timing1_PF(	SOUNDBUFFER *Buffer, SOUNDBUFFER *Src, int Is3DSound ) ;
extern	int		SoundBuffer_Terminate_PF(           SOUNDBUFFER *Buffer ) ;
extern	int		SoundBuffer_CheckEnable_PF(         SOUNDBUFFER *Buffer ) ;
extern	int		SoundBuffer_Play_PF(                SOUNDBUFFER *Buffer, int Loop ) ;
extern	int		SoundBuffer_Stop_PF(				SOUNDBUFFER *Buffer, int EffectStop ) ;
extern	int		SoundBuffer_CheckPlay_PF(           SOUNDBUFFER *Buffer ) ;
extern	int		SoundBuffer_Lock_PF(                SOUNDBUFFER *Buffer, DWORD WritePos , DWORD WriteSize, void **LockPos1, DWORD *LockSize1, void **LockPos2, DWORD *LockSize2 ) ;
extern	int		SoundBuffer_Unlock_PF(              SOUNDBUFFER *Buffer, void *LockPos1, DWORD LockSize1, void *LockPos2, DWORD LockSize2 ) ;
extern	int		SoundBuffer_SetFrequency_PF(        SOUNDBUFFER *Buffer, DWORD Frequency ) ;
extern	int		SoundBuffer_GetFrequency_PF(        SOUNDBUFFER *Buffer, DWORD * Frequency ) ;
extern	int		SoundBuffer_RefreshVolume_PF(		SOUNDBUFFER *Buffer ) ;
extern	int		SoundBuffer_GetCurrentPosition_PF(	SOUNDBUFFER *Buffer, DWORD * PlayPos, DWORD * WritePos ) ;
extern	int		SoundBuffer_SetCurrentPosition_PF(	SOUNDBUFFER *Buffer, DWORD NewPos ) ;
extern	int		SoundBuffer_CycleProcess_PF(		SOUNDBUFFER *Buffer ) ;
extern	int		SoundBuffer_Set3DPosition_PF(		SOUNDBUFFER *Buffer, VECTOR *Position ) ;
extern	int		SoundBuffer_Set3DRadius_PF(			SOUNDBUFFER *Buffer, float Radius ) ;
extern	int		SoundBuffer_Set3DInnerRadius_PF(	SOUNDBUFFER *Buffer, float Radius ) ;
extern	int		SoundBuffer_Set3DVelocity_PF(		SOUNDBUFFER *Buffer, VECTOR *Velocity ) ;
extern	int		SoundBuffer_Set3DFrontPosition_PF(	SOUNDBUFFER *Buffer, VECTOR *FrontPosition, VECTOR *UpVector ) ;
extern	int		SoundBuffer_Set3DConeAngle_PF(		SOUNDBUFFER *Buffer, float InnerAngle, float OuterAngle ) ;
extern	int		SoundBuffer_Set3DConeVolume_PF(		SOUNDBUFFER *Buffer, float InnerAngleVolume, float OuterAngleVolume ) ;
extern	int		SoundBuffer_Refresh3DSoundParam_PF(	SOUNDBUFFER *Buffer, int AlwaysFlag ) ;
extern	int		SoundBuffer_SetReverbParam_PF(		SOUNDBUFFER *Buffer, SOUND3D_REVERB_PARAM *Param ) ;
extern	int		SoundBuffer_SetPresetReverbParam_PF( SOUNDBUFFER *Buffer, int PresetNo ) ;








#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_SOUND

#endif // __DXSOUND_H__
