// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		����v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMOVIE_H__
#define __DXMOVIE_H__

#include "DxCompileConfig.h"

#ifndef DX_NON_MOVIE

// �C���N���[�h ------------------------------------------------------------------
#include "DxHandle.h"
#include "DxBaseImage.h"
#include "DxFile.h"

#ifdef __WINDOWS__
	#include "Windows/DxMovieWin.h"
#endif // __WINDOWS__

#ifdef __psp2__
	#include "PSVita/DxMoviePSVita.h"
#endif // __psp2__

#ifdef __ORBIS__
	#include "PS4/DxMoviePS4.h"
#endif // __ORBIS__

#ifdef __ANDROID__
	#include "Android/DxMovieAndroid.h"
#endif // __ANDROID__

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

// ���[�r�[�t�@�C���̃I�[�v���ɕK�v�ȃO���[�o���f�[�^��Z�߂�����
struct OPENMOVIE_GPARAM
{
	int							RightAlphaFlag;					// ���摜�̉E�����A���t�@�Ƃ݂Ȃ�����n���h�����쐬���邩(TRUE:�쐬����)
	int							A8R8G8B8Flag ;					// 32bit�J���[�t�H�[�}�b�g�̓���� A8R8G8B8 �`���Ƃ��Ĉ������ǂ����̃t���O
	int							NotUseYUVFormatSurfaceFlag ;	// �x�t�u�t�H�[�}�b�g�̃T�[�t�F�X���g�p���Ȃ����ǂ���
} ;

// Ogg Theora �̃f�R�[�h���̏��
struct THEORADECODE_INFO
{
	double						FrameRate ;						// �P�b�ԕӂ�̃R�}��
	int							TotalFrame ;					// ���t���[����
	int							Width, Height ;					// ���ƍ���
} ;

// ���[�r�[�O���t�B�b�N�f�[�^�^
struct MOVIEGRAPH
{
	HANDLEINFO				HandleInfo ;					// �n���h�����ʃf�[�^

	int						PlayType ;						// �Đ��^�C�v

#ifndef DX_NON_OGGTHEORA
	int						TheoraFlag ;					// Theora ���g�p���Ă��邩�ǂ����̃t���O( 1:�g�p���Ă���  0:�g�p���Ă��Ȃ� )
	DWORD_PTR				TheoraHandle ;					// Theora �t�@�C���n���h��
//	LONGLONG				TheoraPlayTime ;				// �Đ��J�n����
	LONGLONG				TheoraPrevTimeCount ;			// �O��̌v������
	LONGLONG				TheoraPlayNowTime ;				// �Đ�����
	int						TheoraTotalPlayTime ;			// �Đ�������( �~���b )
	void *					TheoraStreamData ;				// �X�g���[�������p�f�[�^
	int						TheoraStreamDataIsFileImage ;	// �X�g���[�������p�f�[�^���t�@�C�����������ɓǂݍ��񂾂��̂��ǂ���
	double					TheoraFrameRate ;				// �t���[�����[�g
	int						TheoraLoopType ;				// ���[�v�^�C�v( 0:����f�[�^�ɍ��킹�ă��[�v  1:�T�E���h�f�[�^�ɍ��킹�ă��[�v )
	int						TheoraSetupGraphHandleImage ;	// �摜�\�z�̌�A�O���t�B�b�N�n���h���̃Z�b�g�A�b�v���I���Ă��邩�ǂ���( TRUE:�I���Ă���  FALSE:�I���Ă��Ȃ� )
#ifndef DX_NON_SOUND
	int						TheoraVorbisHandle ;			// Vorbis�p�T�E���h�n���h��
	int						TheoraVorbisFrequency ;			// Vorbis�p�T�E���h�̎��g��
	int						TheoraVorbisTotalTime ;			// Vorbis�T�E���h�f�[�^�̍Đ�������( �~���b )
#endif // DX_NON_SOUND
	double					TheoraPlaySpeedRate ;			// Theora�p�Đ����x
#endif // DX_NON_OGGTHEORA

	RECT					SrcRect ;						// �u���b�N�]������̓]������`

	LONGLONG				RefreshFrame ;					// �O��X�V�����t���[��
	int						RefreshTime ;					// �O��X�V��������

	LONGLONG				StopTime ;						// �I���^�C��
	int						SufIsSystemMemory ;				// �T�[�t�F�X���V�X�e����������ɂ���ꍇ�s�q�t�d
	int						Width, Height ;					// ���[�r�[�O���t�B�b�N�̃T�C�Y
	int						RightAlpha ;					// ���[�r�[�̉E�����A���t�@�`�����l���Ƃ��Ĉ�����(TRUE:����)
	int						A8R8G8B8Flag ;					// 32bit�J���[�t�H�[�}�b�g�̓���� A8R8G8B8 �`���Ƃ��Ĉ������ǂ����̃t���O
	int						NotUseYUVFormatSurfaceFlag ;	// �x�t�u�t�H�[�}�b�g�̃T�[�t�F�X���g�p���Ȃ����ǂ���
	int						PlayFlag ;						// �Đ����t���O
	int						SysPauseFlag ;					// �����ꎞ��~�t���O
	int						FirstUpdateFlag ;				// �ŏ��̃A�b�v�f�[�g���s��ꂽ���ǂ���( TRUE:�s��ꂽ  FALSE:�܂� )

	int						YUVFlag ;						// �x�t�u�`�����g�p���Ă��邩�ǂ���( TRUE:���Ă���  FALSE:���Ă��Ȃ� )
	int						SurfaceMode ;					// �g�p���Ă���T�[�t�F�X�^�C�v

	BASEIMAGE				NowImage ;
	int						NowImageGraphOutAlloc ;			// NowImage �� GraphData ���O���Ŋm�ۂ��ꂽ�o�b�t�@���g�p���Ă��邩�ǂ���
	int						NowImageUpdateFlag ;			// NowImage �̓��e���X�V���ꂽ���ǂ����̃t���O( GetMovieBaseImageToGraph �p )
	BASEIMAGE				*UseNowImage ;					// Theora ���� BASEIMAGE ���g�p���Ă���\��������̂ŊO���̃v���O�����͂�������g���ׂ�
	int						YGrHandle ;						// YUV�J���[��Y�����݂̂̃O���t�B�b�N�n���h��
	int						UVGrHandle ;					// YUV�J���[��UV�����݂̂̃O���t�B�b�N�n���h��
	int						OverlayDestX, OverlayDestY ;	// �Ō�ɐݒ肳�ꂽ�I�[�o�[���C�T�[�t�F�X�̏o�͈ʒu
	int						OverlayDestExRate ;				// �Ō�ɐݒ肳�ꂽ�I�[�o�[���C�T�[�t�F�X�̏o�͔{��
	int						OverlayDispFlag ;				// �Ō�ɐݒ肳�ꂽ�I�[�o�[���C�T�[�t�F�X�̕\���t���O
	RECT					OverlaySrcRect, OverlayDestRect ;// �Ō�ɃZ�b�g���ꂽ�I�[�o�[���C�\�[�X��`�Əo�͋�`

	void					( *UpdateFunction )( struct MOVIEGRAPH *Movie, void *Data ) ;		// ����X�V���ɌĂԃR�[���o�b�N�֐�
	void					*UpdateFunctionData ;												// �R�[���o�b�N�֐��ɓn���|�C���^

	MOVIEGRAPH_PF			PF ;							// ���ˑ����
} ;

// ���[�r�[�f�[�^�Ǘ��\����
struct MOVIEGRAPHMANAGE
{
	int						InitializeFlag ;				// �������t���O

	int						RightAlphaFlag ;				// ���摜�̉E�����A���t�@�Ƃ݂Ȃ�����n���h�����쐬���邩(TRUE:�쐬����)
	int						A8R8G8B8Flag ;					// 32bit�J���[�t�H�[�}�b�g�� A8R8G8B8 �`���Ƃ��Ĉ������ǂ����̃t���O
	int						NotUseYUVFormatSurfaceFlag ;	// �x�t�u�t�H�[�}�b�g�̃T�[�t�F�X���g�p���Ȃ����ǂ���
} ;

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

extern	int		InitializeMovieManage( void ) ;																		// ���[�r�[�֘A�̊Ǘ������̏�����
extern	int		TerminateMovieManage( void ) ;																		// ���[�r�[�֘A�̊Ǘ������̌�n��

// OpenMovie �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern	int		OpenMovie_UseGParam(
					OPENMOVIE_GPARAM *GParam,
					const wchar_t *FileName,
					const void *FileImage,
					size_t FileImageSize,
					int *Width,
					int *Height,
					int SurfaceMode,
					int ASyncThread = FALSE ) ;

extern	int		OpenMovie( const wchar_t *FileName, int *Width, int *Height, int SurfaceMode ) ;						// ���[�r�[���J��
extern	int		CloseMovie( int MovieHandle ) ;																		// ���[�r�[�����
extern 	int		PlayMovie_( int MovieHandle, int PlayType = DX_PLAYTYPE_BACK, int SysPlay = 0 ) ;					// ���[�r�[�̍Đ����J�n����
extern 	int		PauseMovie( int MovieHandle, int SysPause = 0 ) ;													// ���[�r�[�̍Đ����X�g�b�v����
extern	int		AddMovieFrame( int MovieHandle, unsigned int FrameNum ) ;											// ���[�r�[�̃t���[����i�߂�A�߂����Ƃ͏o���Ȃ�( ���[�r�[����~��ԂŁA���� Ogg Theora �̂ݗL�� )
extern	int		SeekMovie( int MovieHandle, int Time ) ;															// ���[�r�[�̍Đ��ʒu��ݒ肷��(�~���b�P��)
extern	int		SetPlaySpeedRateMovie( int MovieHandle, double SpeedRate ) ;										// ���[�r�[�̍Đ����x��ݒ肷��( 1.0 = ���{��  2.0 = �Q�{�� )�A�ꕔ�̃t�@�C���t�H�[�}�b�g�݂̂ŗL���ȋ@�\�ł�
extern 	int		GetMovieState( int MovieHandle ) ;																	// ���[�r�[�̍Đ���Ԃ𓾂�
extern	int		SetMovieVolume( int Volume, int MovieHandle ) ;														// ���[�r�[�̃{�����[�����Z�b�g����(0�`10000)
extern	BASEIMAGE *GetMovieBaseImage( int MovieHandle, int *ImageUpdateFlag, int ImageUpdateFlagSetOnly ) ;			// ���[�r�[�̊�{�C���[�W�f�[�^���擾����
extern	int		GetMovieTotalFrame( int MovieHandle ) ;																// ���[�r�[�̑��t���[�����𓾂�( Ogg Theora �ł̂ݗL�� )
extern	int		TellMovie( int MovieHandle ) ;																		// ���[�r�[�̍Đ��ʒu���擾����(�~���b�P��)
extern	int		TellMovieToFrame( int MovieHandle ) ;																// ���[�r�[�̍Đ��ʒu���擾����(�t���[���P��)
extern	int		SeekMovieToFrame( int MovieHandle, int Frame ) ;													// ���[�r�[�̍Đ��ʒu��ݒ肷��(�t���[���P��)
extern	LONGLONG GetOneFrameTimeMovie( int MovieHandle ) ;															// ���[�r�[�̂P�t���[��������̎��Ԃ𓾂�
extern	MOVIEGRAPH *GetMovieData( int MovieHandle ) ;																// ���[�r�[�O���t�B�b�N�̃f�[�^���擾����

extern	int		SetCallbackMovie( int MovieHandle, void ( *Callback )( MOVIEGRAPH *Movie, void *Data ), void *Data );	// ���[�r�[�̍X�V���ɌĂяo���R�[���o�b�N�֐���o�^����
extern	int		UpdateMovie( int MovieHandle, int AlwaysFlag = FALSE ) ;											// ���[�r�[�̍X�V���s��
extern	int		ReleaseMovieSurface( int MovieHandle ) ;															// ���[�r�[�Ŏg�p���Ă���T�[�t�F�X�̉�����s��

extern	int		DisableMovieAll( void ) ;																			// ���[�r�[�̍Đ���Ԃ��~����
extern	int		RestoreMovieAll( void ) ;																			// ���[�r�[�̍Đ���Ԃ��Đ�����

extern 	int		PlayMovieAll( void ) ;																				// ���ׂẴ��[�r�[�O���t�B�b�N���X�^�[�g
extern 	int		PauseMovieAll( void ) ;																				// ���ׂẴ��[�r�[�O���t�B�b�N���X�g�b�v

extern	int		InitializeMovieHandle( HANDLEINFO *HandleInfo ) ;													// ���[�r�[�n���h����������������֐�
extern	int		TerminateMovieHandle( HANDLEINFO *HandleInfo ) ;													// ���[�r�[�n���h���̌�n�����s���֐�


// ���ˑ��֐�

extern	int		InitializeMovieManage_PF( void ) ;																	// ���[�r�[�֘A�̊Ǘ������̏������̊��ˑ�����
extern	int		TerminateMovieManage_PF( void ) ;																	// ���[�r�[�֘A�̊Ǘ������̌�n���̊��ˑ�����

extern	int		TerminateMovieHandle_PF( HANDLEINFO *HandleInfo ) ;													// ���[�r�[�n���h���̌�n�����s��
extern	int		OpenMovie_UseGParam_PF( MOVIEGRAPH * Movie, OPENMOVIE_GPARAM *GParam, const wchar_t *FileName, int *Width, int *Height, int SurfaceMode, int ASyncThread = FALSE ) ;	// OpenMovie �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W�����̊��ˑ�����
extern 	int		PlayMovie__PF( MOVIEGRAPH * Movie, int PlayType = DX_PLAYTYPE_BACK, int SysPlay = 0 ) ;				// ���[�r�[�̍Đ����J�n���鏈���̊��ˑ�����
extern 	int		PauseMovie_PF( MOVIEGRAPH * Movie, int SysPause = 0 ) ;												// ���[�r�[�̍Đ����X�g�b�v���鏈���̊��ˑ�����
extern	int		SeekMovie_PF( MOVIEGRAPH * Movie, int Time ) ;														// ���[�r�[�̍Đ��ʒu��ݒ肷��(�~���b�P��)�����̊��ˑ�����
extern	int		SetPlaySpeedRateMovie_PF( MOVIEGRAPH * Movie, double SpeedRate ) ;									// ���[�r�[�̍Đ����x��ݒ肷��( 1.0 = ���{��  2.0 = �Q�{�� )�����̊��ˑ�����
extern 	int		GetMovieState_PF( MOVIEGRAPH * Movie ) ;															// ���[�r�[�̍Đ���Ԃ𓾂鏈���̊��ˑ�����
extern	int		SetMovieVolume_PF( MOVIEGRAPH * Movie, int Volume ) ;												// ���[�r�[�̃{�����[�����Z�b�g����(0�`10000)�����̊��ˑ�����
extern	BASEIMAGE *GetMovieBaseImage_PF( MOVIEGRAPH * Movie, int *ImageUpdateFlag, int ImageUpdateFlagSetOnly ) ;		// ���[�r�[�̊�{�C���[�W�f�[�^���擾���鏈���̊��ˑ�����
extern	int		TellMovie_PF( MOVIEGRAPH * Movie ) ;																// ���[�r�[�̍Đ��ʒu���擾����(�~���b�P��)�����̊��ˑ�����
extern	int		TellMovieToFrame_PF( MOVIEGRAPH * Movie ) ;															// ���[�r�[�̍Đ��ʒu���擾����(�t���[���P��)�����̊��ˑ�����
extern	int		SeekMovieToFrame_PF( MOVIEGRAPH * Movie, int Frame ) ;												// ���[�r�[�̍Đ��ʒu��ݒ肷��(�t���[���P��)�����̊��ˑ�����
extern	LONGLONG GetOneFrameTimeMovie_PF( MOVIEGRAPH * Movie ) ;													// ���[�r�[�̂P�t���[��������̎��Ԃ𓾂鏈���̊��ˑ�����

extern	int		UpdateMovie_PF( MOVIEGRAPH * Movie, int AlwaysFlag = FALSE ) ;										// ���[�r�[�̍X�V���s�������̊��ˑ�����



#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MOVIE

#endif // __DXMOVIE_H__
