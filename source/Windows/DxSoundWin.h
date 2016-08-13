// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		WindowsOS�p�T�E���h�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXSOUNDWIN_H__
#define __DXSOUNDWIN_H__

#include "../DxCompileConfig.h"

#ifndef DX_NON_SOUND

// �C���N���[�h ------------------------------------------------------------------
#include "../DxLib.h"
#include "DxDirectX.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

#define D_X3DAUDIO_INPUTCHANNELS		(8)				// X3DAudio �̌v�Z�Ŏg�p����ő���̓`�����l����

// �\���̒�` --------------------------------------------------------------------

// XAudio2�p�R�[���o�b�N�����p�\����
struct SOUNDBUFFER_CALLBACK : public D_IXAudio2VoiceCallback
{
	virtual void    __stdcall OnVoiceProcessingPassStart( DWORD BytesRequired ) ;
	virtual void    __stdcall OnVoiceProcessingPassEnd	() ;
	virtual void    __stdcall OnStreamEnd				() ;
	virtual void    __stdcall OnBufferStart				( void* pBufferContext ) ;
	virtual void    __stdcall OnBufferEnd				( void* pBufferContext ) ;
	virtual void    __stdcall OnLoopEnd					( void* pBufferContext ) ;
	virtual void    __stdcall OnVoiceError				( void* pBufferContext, HRESULT Error ) ;

	struct SOUNDBUFFER			*Buffer ;
} ;

// �T�E���h�o�b�t�@���ˑ����\����
struct SOUNDBUFFER_PF
{
	union
	{
		D_IDirectSoundBuffer 		*DSBuffer ;				// DirectSoundBuffer
		D_IXAudio2SourceVoice		*XA2SourceVoice ;		// XAudio2SourceVoice
		D_IXAudio2_8SourceVoice		*XA2_8SourceVoice ;		// XAudio2_8SourceVoice
	} ;
	union
	{
		D_IXAudio2SubmixVoice		*XA2SubmixVoice ;		// XAudio2SubmixVoice
		D_IXAudio2_8SubmixVoice		*XA2_8SubmixVoice ;		// XAudio2_8SubmixVoice
	} ;
	IUnknown					*XA2ReverbEffect ;		// ���o�[�u�G�t�F�N�g
	SOUNDBUFFER_CALLBACK		*XA2Callback ;			// XAudio2�p�̃R�[���o�b�N

	union
	{
		D_XAUDIO2FX_REVERB_PARAMETERS    XAudio2ReverbParameter ;	// ���o�[�u�v�Z�p�p�����[�^
		D_XAUDIO2FX_REVERB_PARAMETERS2_8 XAudio2_8ReverbParameter ;	// ���o�[�u�v�Z�p�p�����[�^XAudio2.8�p
	} ;

	int							XAudioChannels ;		// XAudio ���ł̃`�����l����

	// �R�c�T�E���h�Đ��p�f�[�^
	D_X3DAUDIO_EMITTER			X3DAudioEmitterData ;									// �R�c�T�E���h�p�������
	D_X3DAUDIO_CONE				X3DAudioEmitterConeData ;								// �R�c�T�E���h�p�������Ŏg�p����R�[�����
	float						X3DAudioEmitterChannelAzimuths[ D_X3DAUDIO_INPUTCHANNELS ] ;	// �R�c�T�E���h�p�������Ŏg�p����`�����l���ʒu�e�[�u��
} ;

// �T�E���h�V�X�e���p���ˑ��f�[�^�\����
struct SOUNDSYSTEMDATA_PF
{
	D_IDirectMusicLoader8		*DirectMusicLoaderObject ;		// DirectMusicLoader8 �I�u�W�F�N�g
	D_IDirectMusicPerformance8	*DirectMusicPerformanceObject ;	// DirectMusicPerformance8 �I�u�W�F�N�g

	HMODULE						XAudio2_8DLL ;					// XAudio2_8.dll
	HRESULT						( WINAPI *XAudio2CreateFunc )( D_IXAudio2_8 ** ppXAudio2, DWORD Flags, D_XAUDIO2_PROCESSOR XAudio2Processor );
	HRESULT						( WINAPI *CreateAudioVolumeMeterFunc )( IUnknown** ppApo ) ;
	HRESULT						( WINAPI *CreateAudioReverbFunc )( IUnknown** ppApo ) ;

	HMODULE						X3DAudioDLL ;					// X3DAudio.dll
	void						( __cdecl *X3DAudioInitializeFunc )( DWORD SpeakerChannelMask, float SpeedOfSound, D_X3DAUDIO_HANDLE Instance ) ;
	void						( __cdecl *X3DAudioCalculateFunc )( const D_X3DAUDIO_HANDLE Instance, const D_X3DAUDIO_LISTENER* pListener, const D_X3DAUDIO_EMITTER* pEmitter, DWORD Flags, D_X3DAUDIO_DSP_SETTINGS* pDSPSettings ) ;

	union
	{
		D_IXAudio2				*XAudio2Object ;				// XAudio2�I�u�W�F�N�g
		D_IXAudio2_8			*XAudio2_8Object ;				// XAudio2_8�I�u�W�F�N�g
		D_IDirectSound			*DirectSoundObject ;			// �c�����������r���������I�u�W�F�N�g
	} ;

	D_XAUDIO2FX_REVERB_PARAMETERS    XAudio2ReverbParameters[ D_XAUDIO2FX_PRESET_NUM ] ;	// ���o�[�u�v�Z�p�p�����[�^
	D_XAUDIO2FX_REVERB_PARAMETERS2_8 XAudio2_8ReverbParameters[ D_XAUDIO2FX_PRESET_NUM ] ;	// ���o�[�u�v�Z�p�p�����[�^XAudio2.8�p
	D_IXAudio2MasteringVoice	*XAudio2MasteringVoiceObject ;	// XAudio2MasteringVoice�I�u�W�F�N�g
	D_IXAudio2_8MasteringVoice	*XAudio2_8MasteringVoiceObject ;// XAudio2_8MasteringVoice�I�u�W�F�N�g
	D_XAUDIO2_EFFECT_CHAIN		XAudio2MasteringEffectChain ;	// �}�X�^�[�{�C�X�p�̃G�t�F�N�g�`�F�C��

	DWORD						XAudio2OutputChannelMask ;		// �o�̓`�����l���}�X�N

	D_X3DAUDIO_HANDLE			X3DAudioInstance ;				// �R�c�T�E���h�v�Z�p�C���X�^���X�n���h��
	D_X3DAUDIO_CONE				X3DAudioListenerConeData ;		// �R�c�T�E���h�̃��X�i�[���Ɏg�p����R�[�����
	D_X3DAUDIO_LISTENER			X3DAudioListenerData ;			// �R�c�T�E���h�̃��X�i�[���

	D_IDirectSoundBuffer		*PrimarySoundBuffer ;			// �v���C�}���T�E���h�o�b�t�@
	D_IDirectSoundBuffer		*BeepSoundBuffer ;				// �r�[�v���p�o�b�t�@

	D_IDirectSoundBuffer		*NoSoundBuffer ;				// �����o�b�t�@

	int							EnableXAudioFlag ;				// XAudio���g�p���邩�ǂ����̃t���O( TRUE:�g�p����  FALSE:�g�p���Ȃ� ) 
	int							UseSoftwareMixing ;				// �\�t�g�E�G�A�~�L�V���O���s�����ǂ����̃t���O( TRUE:�\�t�g�E�G�A  FALSE:�n�[�h�E�G�A )

	HANDLE						StreamSoundThreadHandle ;		// �X�g���[���T�E���h���Đ����鏈�����s���X���b�h�̃n���h��
	DWORD						StreamSoundThreadID ;			// �X�g���[���T�E���h���Đ����鏈�����s���X���b�h�̂h�c
	DWORD						StreamSoundThreadEndFlag ;		// �X�g���[���T�E���h���Đ����鏈�����I�����邩�ǂ����̃t���O
} ;

// �l�h�c�h�f�[�^���ˑ����
struct MIDIHANDLEDATA_PF
{
	D_IDirectMusicSegment8		*DirectMusicSegmentObject ;		// DirectMusicSegment8 �I�u�W�F�N�g
} ;

// �l�h�c�h�V�X�e���p���ˑ��f�[�^�\����
struct MIDISYSTEMDATA_PF
{
	UINT						MidiDeviceID ;					// �l�h�c�h���t���̃f�o�C�X�h�c
} ;

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

extern	int		MidiCallBackProcess( void ) ;																			// �l�h�c�h���t�I�����Ă΂��R�[���o�b�N�֐�


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_SOUND

#endif // __DXSOUNDWIN_H__
