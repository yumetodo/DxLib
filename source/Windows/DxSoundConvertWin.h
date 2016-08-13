// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		WindowsOS�p�T�E���h�f�[�^�ϊ��v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXSOUNDCONVERTWIN_H__
#define __DXSOUNDCONVERTWIN_H__

#include "../DxCompileConfig.h"

// �C���N���[�h ------------------------------------------------------------------
#include "../DxLib.h"

#include "DxUseCStrmBaseFilter.h"

#ifndef DX_NON_ACM
	#ifndef DX_GCC_COMPILE
		#include <mmreg.h>
	#endif // DX_GCC_COMPILE
	#ifdef DX_GCC_COMPILE_4_9_2
		#include <mmreg.h>
	#endif // DX_GCC_COMPILE_4_9_2
	#include <msacm.h>
#endif // DX_NON_ACM

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

#define SOUND_METHODTYPE_ACM				(SOUND_METHODTYPE_DEFAULT_NUM + 0)		// �`�b�l���g�p
#define SOUND_METHODTYPE_DSMP3				(SOUND_METHODTYPE_DEFAULT_NUM + 1)		// �c�����������r���������g�p�����l�o�R�Đ�

// �\���̒�` --------------------------------------------------------------------

#ifndef DX_NON_ACM

// #ifdef DX_GCC_COMPILE
struct D_HACMSTREAM__
{
	int unused ;
} ;

typedef struct D_HACMSTREAM__ *D_HACMSTREAM ;
typedef        D_HACMSTREAM   *D_LPHACMSTREAM ;

struct D_HACMOBJ__
{
	int unused ;
} ;
typedef struct D_HACMOBJ__     *D_HACMOBJ ;

typedef struct D_wavefilter_tag
{
	DWORD   cbStruct;           /* Size of the filter in bytes */
	DWORD   dwFilterTag;        /* filter type */
	DWORD   fdwFilter;          /* Flags for the filter (Universal Dfns) */
	DWORD   dwReserved[5];      /* Reserved for system use */
} D_WAVEFILTER;
typedef D_WAVEFILTER FAR  *D_LPWAVEFILTER;

#define D_MPEGLAYER3_WFX_EXTRA_BYTES			12
#define D_WAVE_FORMAT_MPEGLAYER3				0x0055 /* ISO/MPEG Layer3 Format Tag */

#define D_MPEGLAYER3_ID_UNKNOWN					0
#define D_MPEGLAYER3_ID_MPEG					1
#define D_MPEGLAYER3_ID_CONSTANTFRAMESIZE		2

typedef struct D_mpeglayer3waveformat_tag
{
	WAVEFORMATEX  wfx;
	WORD          wID;
	DWORD         fdwFlags;
	WORD          nBlockSize;
	WORD          nFramesPerBlock;
	WORD          nCodecDelay;
} D_MPEGLAYER3WAVEFORMAT ;

#define D_ACM_STREAMSIZEF_SOURCE				0x00000000L
#define D_ACM_STREAMSIZEF_DESTINATION			0x00000001L
#define D_ACM_STREAMSIZEF_QUERYMASK				0x0000000FL

#define D_ACM_METRIC_MAX_SIZE_FORMAT			50

#define D_ACM_STREAMCONVERTF_BLOCKALIGN			0x00000004
#define D_ACM_STREAMCONVERTF_START				0x00000010
#define D_ACM_STREAMCONVERTF_END				0x00000020

#ifdef  _WIN64
#define D__DRVRESERVED    15
#else
#define D__DRVRESERVED    10
#endif  // _WIN64

typedef struct D_tACMSTREAMHEADER
{
    DWORD           cbStruct;               // sizeof(D_ACMSTREAMHEADER)
    DWORD           fdwStatus;              // ACMSTREAMHEADER_STATUSF_*
    DWORD_PTR       dwUser;                 // user instance data for hdr
    LPBYTE          pbSrc;
    DWORD           cbSrcLength;
    DWORD           cbSrcLengthUsed;
    DWORD_PTR       dwSrcUser;              // user instance data for src
    LPBYTE          pbDst;
    DWORD           cbDstLength;
    DWORD           cbDstLengthUsed;
    DWORD_PTR       dwDstUser;              // user instance data for dst
    DWORD           dwReservedDriver[D__DRVRESERVED];   // driver reserved work space

} D_ACMSTREAMHEADER, *D_PACMSTREAMHEADER, FAR *D_LPACMSTREAMHEADER;
// #endif

// �`�b�l�f�[�^�ϊ��p�\����
struct SOUNDCONV_ACM
{
	WAVEFORMATEX				*Format ;				// �ϊ����̃t�H�[�}�b�g
	D_HACMSTREAM				AcmStreamHandle[2]	;	// ���k���ꂽ�f�[�^�������ۂɎg���`�b�l�̃n���h��

	void						*SrcData ;				// �ϊ����̃f�[�^���ꎞ�I�ɕۑ����Ă����������̈�
	int							SrcDataSize ;			// �ϊ����̃f�[�^���ꎞ�I�ɕۑ����Ă����������̈�̃T�C�Y
//	int							SrcDataValidSize ;		// �ϊ����̃f�[�^�̗L���ȃT�C�Y
	int							SrcDataPosition ;		// �ϊ����̃f�[�^�̕ϊ������������T�C�Y
	int							DestDataSampleNum ;		// �ϊ���̃f�[�^�̃T���v����( -1:�ȊO�̏ꍇ�̂ݗL�� )

	int							BeginSeekCompSrcSize ;	// �V�[�N�\��ʒu�܂ŃT�uACM�n���h���ŕϊ����Ă��������ŁA���ɕϊ����������Ă��鈳�k�f�[�^�̃T�C�Y
	int							BeginSeekPosition ;		// �V�[�N�\��ʒu�܂ŃT�uACM�n���h���ŕϊ����Ă��������ŁA���ɕϊ����������Ă���W�J��f�[�^�̃T�C�Y(�V�[�N�\��ʒu�� SOUNDCONV �֐��� SeekLockPosition �����o�[)
	void						*BeginSeekBuffer ;		// �V�[�N�\��ʒu�܂ŃT�uACM�n���h���ŕϊ����Ă��������Ŏg�p����o�b�t�@
} ;

#endif // DX_NON_ACM

#ifndef DX_NON_MOVIE
#ifndef DX_NON_DSHOW_MOVIE

// �c�����������r���������g�����l�o�R�f�[�^�ϊ������Ŏg�p����o�b�t�@�N���X
class SOUNDCONV_DSMP3_BUF : public D_ISampleGrabberCB
{
public :
	struct SOUNDCONV			*SoundConvData ;		// �����f�[�^�ϊ������p�\���̂ւ̃|�C���^

	ULONG __stdcall				AddRef() ;
	ULONG __stdcall				Release() ;

	HRESULT __stdcall			QueryInterface( REFIID riid, void ** ppv ) ;
	HRESULT __stdcall			SampleCB( double SampleTime, D_IMediaSample *pSample ) ;
	HRESULT __stdcall			BufferCB( double SampleTime, BYTE *pBuffer, long BufferLen ) ;
} ;

// �c�����������r���������g�����l�o�R�f�[�^�ϊ��p�\����
struct SOUNDCONV_DSMP3
{
	void						*PCMBuffer ;			// �o�b�l�ɕϊ����ꂽ�f�[�^���i�[���郁�����̈�
	DWORD						PCMBufferSize ;			// �o�b�l�o�b�t�@�̃T�C�Y
	DWORD						PCMValidDataSize ;		// �L���Ȃo�b�l�̃T�C�Y
	DWORD						PCMDestCopySize ;		// �{�o�b�t�@�ւ̃R�s�[�����������T�C�Y
} ;

#endif  // DX_NON_DSHOW_MOVIE
#endif	// DX_NON_MOVIE

// �T�E���h�ϊ������S�̂Ŏg�p����f�[�^�\���̂� Windows �Ɉˑ����Ă�����̍\����
struct SOUNDCONVERTDATA_WIN
{
	int							Dummy ;
#ifndef DX_NON_ACM
	HMODULE						msacm32DLL ;					// msacm32DLL
	MMRESULT					( WINAPI *acmStreamOpenFunc            )( D_LPHACMSTREAM phas, HACMDRIVER had, LPWAVEFORMATEX pwfxSrc, LPWAVEFORMATEX pwfxDst, D_LPWAVEFILTER pwfltr, DWORD dwCallback, DWORD dwInstance, DWORD fdwOpen );
	MMRESULT					( WINAPI *acmFormatSuggestFunc         )( HACMDRIVER had, LPWAVEFORMATEX pwfxSrc, LPWAVEFORMATEX pwfxDst, DWORD cbwfxDst, DWORD fdwSuggest );
	MMRESULT					( WINAPI *acmStreamCloseFunc           )( D_HACMSTREAM has, DWORD fdwClose );
	MMRESULT					( WINAPI *acmMetricsFunc               )( D_HACMOBJ hao, UINT uMetric, LPVOID pMetric );
	MMRESULT					( WINAPI *acmStreamPrepareHeaderFunc   )( D_HACMSTREAM has, D_LPACMSTREAMHEADER pash, DWORD fdwPrepare );
	MMRESULT					( WINAPI *acmStreamConvertFunc         )( D_HACMSTREAM has, D_LPACMSTREAMHEADER pash, DWORD fdwConvert );
	MMRESULT					( WINAPI *acmStreamUnprepareHeaderFunc )( D_HACMSTREAM has, D_LPACMSTREAMHEADER pash, DWORD fdwUnprepare );
	MMRESULT					( WINAPI *acmStreamSizeFunc            )( D_HACMSTREAM has, DWORD cbInput, LPDWORD pdwOutputBytes, DWORD fdwSize );
#endif
} ;

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

#ifndef DX_NON_ACM
extern	int SetupSoundConvert_ACM(            struct SOUNDCONV *SoundConv ) ;								// �`�b�l���g�p�����t�@�C���̃Z�b�g�A�b�v�������s��( [��] -1:�G���[ )
extern	int SetupSoundConvert_MP3(            struct SOUNDCONV *SoundConv ) ;								// �l�o�R�t�@�C���̃Z�b�g�A�b�v�������s��( [��] -1:�G���[ )
extern	int TerminateSoundConvert_ACM(        struct SOUNDCONV *SoundConv ) ;								// �`�b�l���g�p�����t�@�C���̌�n���������s��
extern	int ConvertProcessSoundConvert_ACM(   struct SOUNDCONV *SoundConv ) ;								// �ϊ���̃o�b�t�@�Ƀf�[�^���[����
//extern	int SetTimeSoundConvert_ACM(      struct SOUNDCONV *SoundConv, int Time ) ;					// �ϊ��������ʒu��ύX����( �~���b�P�� )
extern	int SetSampleTimeSoundConvert_ACM(    struct SOUNDCONV *SoundConv, int SampleTime ) ;				// �ϊ������̈ʒu��ύX����( �T���v���P�� )
extern	int GetSoundConvertDestSize_Fast_ACM( struct SOUNDCONV *SoundConv ) ;								// �ϊ���̑�}�̃f�[�^�T�C�Y�𓾂�
#endif

#ifndef DX_NON_MOVIE
#ifndef DX_NON_DSHOW_MOVIE
#ifndef DX_NON_DSHOW_MP3
extern	int TerminateSoundConvert_DSMP3(        struct SOUNDCONV *SoundConv ) ;							// MP3���g�p�����t�@�C���̌�n���������s��
extern	int ConvertProcessSoundConvert_DSMP3(   struct SOUNDCONV *SoundConv ) ;							// �ϊ���̃o�b�t�@�Ƀf�[�^���[����
//extern	int SetTimeSoundConvert_DSMP3(      struct SOUNDCONV *SoundConv, int Time ) ;					// �ϊ��������ʒu��ύX����( �~���b�P�� )
extern	int SetSampleTimeSoundConvert_DSMP3(    struct SOUNDCONV *SoundConv, int SampleTime ) ;			// �ϊ������̈ʒu��ύX����( �T���v���P�� )
extern	int GetSoundConvertDestSize_Fast_DSMP3( struct SOUNDCONV *SoundConv ) ;							// �ϊ���̑�}�̃f�[�^�T�C�Y�𓾂�
#endif
#endif
#endif

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXSOUNDCONVERTWIN_H__
