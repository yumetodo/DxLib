// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�W���b���C�u�����g�p�R�[�h�@Ogg�֌W�w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXUSECLIBOGG_H__
#define __DXUSECLIBOGG_H__

// �C���N���[�h ------------------------------------------------------------------
#include "DxCompileConfig.h"

#ifndef DX_NON_OGGTHEORA
#include "DxMovie.h"
#endif

#if !defined( DX_NON_OPUS ) || !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_MOVIE )
#include "DxSoundConvert.h"
#endif

#include "DxFile.h"
#include "DxBaseFunc.h"

#if defined( DX_NON_OGGVORBIS ) && defined( DX_NON_OGGTHEORA )
#else
    #include "vorbis/codec.h"
    #include "vorbis/vorbisfile.h"
    
    #include "os.h"
    #include "misc.h"
#endif

#ifndef DX_NON_OGGVORBIS
	//#include "codec.h"
	#include "vorbis/vorbisfile.h"
	
    #include "vorbis/codec.h"
#endif

#ifndef DX_NON_OGGTHEORA
	//#include "codec.h"
	#include "vorbis/vorbisfile.h"
	
    #include "theora/theora.h"
#endif

#ifndef DX_NON_OPUS
	#include "opusfile.h"
#endif

//namespace DxLib
//{

// �}�N����` --------------------------------------------------------------------

// Theora �f�R�[�h�X���b�h�̏��
#define THEORAT_STATE_IDLE		(0)				// �������Ă��Ȃ�
#define THEORAT_STATE_DECODE	(1)				// �t���[���f�R�[�h��
#define THEORAT_STATE_SEEK		(2)				// �V�[�N��
#define THEORAT_STATE_EXIT		(3)				// �X���b�h�I��
#define THEORAT_STATE_STOP		(4)				// �X���b�h�X�g�b�v��

// �\���̒�` --------------------------------------------------------------------

#ifndef DX_NON_OGGTHEORA
// Ogg �y�[�W�w�b�_( �w�b�_�̃T�C�Y�̋��ߕ��� Segments + 27 )
typedef struct tagOGG_HEADER
{
	char					ID[ 4 ] ;					// ����ID "OggS"
	BYTE					Version ;					// �o�[�W����
	BYTE					HeadType ;					// �w�b�_�[�^�C�v
	BYTE					GranulePosition[ 8 ] ;		// �A�h���X( ���g���G���f�B�A�� )
	BYTE					SerialNumber[ 4 ] ;			// �V���A���i���o�[
	BYTE					PageNumber[ 4 ] ;			// �y�[�W�i���o�[
	BYTE					CRCCehckSum[ 4 ] ;			// CRC�`�F�b�N�T��
	BYTE					Segments ;					// �Z�O�����g��
	BYTE					SegmentTable[ 1 ] ;			// �Z�O�����g�e�[�u��
} OGG_HEADER ;

// �y�[�W�̏��
typedef struct tagTHEORA_PAGEINFO
{
	int						StreamAddres ;				// �X�g���[����̃A�h���X
	int						FrameAddres ;				// Theora ���撆�̃t���[��
	int						KeyFrame ;					// �L�[�t���[�����܂܂�Ă��邩�ǂ���
	int						FrameNum ;					// �y�[�W�Ɋ܂܂��t���[����
} THEORA_PAGEINFO ;

// �X�g�b�N�t���[���̏��
typedef struct tagTHEORA_STOCKFRAME
{
	volatile int			UseFlag ;					// �g�p���Ă��邩�ǂ���( 1:�g�p���Ă���  0:�g�p���Ă��Ȃ� )
	volatile int			FrameNumber ;				// �ۑ����Ă���摜�̃t���[��

	void					*YBuffer ;					// �x�C���[�W�ւ̃A�h���X
	void					*UVBuffer ;					// �t�u�C���[�W�ւ̃A�h���X
	int						YWidth, YHeight ;			// �x�C���[�W�̕��ƍ���
	int						YStride ;					// �x�o�b�t�@�̃s�b�`
	int						UVWidth, UVHeight ;			// �t�u�C���[�W�̕��ƍ���
	int						UVStride ;					// �t�u�o�b�t�@�̃s�b�`
} THEORA_STOCKFRAME ;

// Ogg Theora �f�R�[�h�����p�f�[�^�\����
typedef struct tagDECODE_THEORA
{
	DX_CRITICAL_SECTION		CriticalSection ;			// �����X���b�h�ł̋����h�~�p�̃N���e�B�J���Z�N�V����

	volatile int			ThreadState ;				// �X���b�h�̏��( THEORAT_STATE_IDLE �� )
	volatile int			ThreadStopRequest ;			// �X���b�h�Ɏ~�܂��Ăق����Ƃ��� 1 �ɂ���
	volatile int			ThreadExitRequest ;			// �X���b�h�ɏI�����Ăق����Ƃ��� 1 �ɂ���
	volatile int			ThreadSeekRequest ;			// �X���b�h�ɃV�[�N���ė~�����Ƃ��� 1 �ɂ���
	volatile int			ThreadEnableCacheRead ;		// �X���b�h�ɓ��Ƀ^�X�N���Ȃ��ꍇ�ɐ�ǂ݂������邩�ǂ���( 1:����  0:�����Ȃ� )
	volatile int			ThreadSeekFrame  ;			// �X���b�h�ɃV�[�N���ĖႤ�Ƃ��̃V�[�N��t���[��
	volatile int			ThreadPacketEnd ;			// �f�[�^��S�ēǂݏI������t���O
	volatile int			ThreadStandbyTime ;			// �ҋ@��Ԃ��ێ�����ڈ��Ƃ��鎞��
//	volatile HANDLE			DecodeThreadHandle ;		// �f�R�[�h�����X���b�h
//	volatile DWORD			DecodeThreadID ;			// �f�R�[�h�����X���b�h�̂h�c 
	THREAD_INFO				DecodeThreadInfo ;			// �f�R�[�h�X���b�h���

	STREAMDATASHRED			StreamShred ;				// �X�g���[���֐�
	void *					StreamData ;				// �X�g���[���f�[�^

	ogg_sync_state			OggSyncState ;				// Ogg �x�[�X�f�[�^
	ogg_page				OggPage ;					// Ogg �y�[�W
	ogg_stream_state		OggTheoraStream ;			// Ogg �X�g���[��
	ogg_packet				OggPacket ;					// Ogg �p�P�b�g

	theora_info				TheoraInfo ;				// Theora ���
	theora_comment			TheoraComment ;				// Theora Comment
	theora_state			TheoraState ;				// Theora State
	int						TheoraSerialNo ;			// Theora �f�[�^�̃V���A���i���o�[

	THEORA_PAGEINFO			*TheoraPageInfo ;			// �y�[�W�̏��
	int						TheoraPageInfoNum ;			// �y�[�W�̏��̐�
	int						*TheoraTimeFrameToDataFrame ;	// �Đ����ԃt���[���ɑΉ�����f�[�^�t���[���Ƃ̑Ή��z��( �Đ����ԃt���[���̉ӏ��Ƀf�[�^�t���[���ԍ��������Ă��� )
	unsigned char			*TheoraKeyFrameInfo ;		// �L�[�t���[�����z��( �L�[�t���[���̔z��v�f�� 1 �ɂȂ��Ă��� )
	ogg_int64_t				TheoraTotalDataFrame ;		// ���f�[�^�t���[����
	int						TheoraTotalTimeFrame ;		// ���Đ����ԃt���[����
	volatile int			TheoraCurrentTimeFrame ;	// �ΊO�I�ȃJ�����g�^�C���t���[��
	volatile int			TheoraCurrentDataFrame ;	// �ΊO�I�ȃJ�����g�f�[�^�t���[��
	volatile int			TheoraInCurrentDataFrame ;	// ���ۂ̃J�����g�f�[�^�t���[��

	int						NumOfTheoraHeader ;			// Theora �̃w�b�_�̐�

	volatile THEORA_STOCKFRAME *StockFrame ;			// �X�g�b�N�t���[��
	volatile int			StockFrameMaxNum ;			// �X�g�b�N�t���[���̍ő吔

	BASEIMAGE				BaseImage ;					// �J�����g�t���[�����i�[���ꂽ�t���[���X�^�b�N���̃C���[�W�̃R�s�[
	volatile int			BaseImageSetup ;			// �J�����g�t���[���� RGB �C���[�W���\�z����Ă��邩�ǂ���( 1:����Ă���  0:����Ă��Ȃ� )

	volatile int			NotUseYUVFormatSurface ;	// �x�t�u�t�H�[�}�b�g�̃T�[�t�F�X���g�p���Ȃ����ǂ���
	volatile int			ValidYUVSurface ;			// �x�t�u�T�[�t�F�X���L�����ǂ���( TRUE:�L��  FALSE:���� )
	volatile int			YUVSurfaceReleaseRequest ;	// �x�t�u�t�H�[�}�b�g�T�[�t�F�X�̉���˗��t���O
	volatile DWORD			YUVImageFourCC ;			// �x�t�u�T�[�t�F�X�Ɏg�p����Ă��� FourCC �t�H�[�}�b�g
	volatile int			YUVImageSetup ;				// �J�����g�t���[���� YUV �C���[�W���\�z����Ă��邩�ǂ���( 1:����Ă���  0:����Ă��Ȃ� )

	volatile int			NotUseYUVGrHandle ;			// YUV�J���[�̃O���t�B�b�N�n���h�����g�p���Ȃ����ǂ���
	volatile int			YGrHandle ;					// YUV�J���[��Y�����݂̂̃O���t�B�b�N�n���h��
	volatile int			UVGrHandle ;				// YUV�J���[��UV�����݂̂̃O���t�B�b�N�n���h��
	volatile int			YUVGrHandleSetup ;			// YUV�J���[�̃O���t�B�b�N�n���h���̃Z�b�g�A�b�v���������Ă��邩�ǂ���( 1:�������Ă���  0:�������Ă��Ȃ� )

	volatile struct DECODE_THEORA_PF *PF ;						// ���ˑ��f�[�^
} DECODE_THEORA ;

#endif

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------


#ifndef DX_NON_OGGTHEORA

extern	int		TheoraDecode_Thread_LoopProcess( DECODE_THEORA *DT ) ;														// Ogg Theora �̃f�R�[�h�X���b�h�ōs�����[�v�������s��( �߂�l 2 �ŏI�� )

extern	void	TheoraDecode_CreateSurface( DECODE_THEORA *DT, int ASyncThread ) ;

// ���ˑ��֐�
extern	int		TheoraDecode_CreateSurface_PF(  DECODE_THEORA *DT ) ;														// �n�[�h�E�G�A�@�\���L���ȏꍇ�� YUV �t�H�[�}�b�g�̈ꎞ�ۑ��p�e�N�X�`�����쐬����
extern	void	TheoraDecode_ReleaseSurface_PF(	DECODE_THEORA *DT ) ;
extern	int		TheoraDecode_SetupImage_PF( DECODE_THEORA *DT, volatile THEORA_STOCKFRAME *Stock, int ASyncThread ) ;		// �J�����g�t���[����RGB�摜���쐬����( �߂�l  1:�쐬���ꂽ  0:����Ȃ����� )
extern	const void * TheoraDecode_GetYUVImage_PF( DECODE_THEORA *DT ) ;														// �ꎞ�o�b�t�@�� YUV �t�H�[�}�b�g�̃e�N�X�`���𓾂�
extern	int		TheoraDecode_InitializeStream_PF( DECODE_THEORA *DT ) ;														// Ogg Theora �̓ǂݍ��ݏ����̏������s�������̊��ˑ��������s���֐�

#endif

//}

#endif // __DXUSECLIBOGG_H__
