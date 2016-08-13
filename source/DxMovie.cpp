// ----------------------------------------------------------------------------
//
//		�c�w���C�u�����@���[�r�[�Đ������p�v���O����
//
//				Ver 3.16d
//
// ----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "DxMovie.h"

#ifndef DX_NON_MOVIE

// �C���N���[�h----------------------------------------------------------------
#include "DxLib.h"
#include "DxStatic.h"
#include "DxBaseFunc.h"
#include "DxMemory.h"
#include "DxSound.h"
#include "DxUseCLib.h"
#include "DxLog.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����`------------------------------------------------------------------

// ���[�r�[���X�V����Ԋu
#define MOVIEREFRESHWAIT		(17)

// ���[�r�[�n���h���̗L�����`�F�b�N
#define MOVIEHCHK( HAND, MPOINT )		HANDLECHK(       DX_HANDLETYPE_MOVIE, HAND, *( ( HANDLEINFO ** )&MPOINT ) )
#define MOVIEHCHK_ASYNC( HAND, MPOINT )	HANDLECHK_ASYNC( DX_HANDLETYPE_MOVIE, HAND, *( ( HANDLEINFO ** )&MPOINT ) )

#define VFW_E_NOT_FOUND                  ((HRESULT)0x80040216L)

#define MOVIE MovieData

// �\���̌^�錾----------------------------------------------------------------

// �f�[�^�錾------------------------------------------------------------------

MOVIEGRAPHMANAGE MovieData ;								// ����֘A�f�[�^

// �N���X�錾 -----------------------------------------------------------------

// �֐��v���g�^�C�v�錾--------------------------------------------------------

// �v���O�����R�[�h------------------------------------------------------------

// ���[�r�[�֘A�̊Ǘ������̏�����
extern int InitializeMovieManage( void )
{
	if( MOVIE.InitializeFlag == TRUE )
		return -1 ;

	// �n���h���Ǘ����̏�����
	InitializeHandleManage( DX_HANDLETYPE_MOVIE, sizeof( MOVIEGRAPH ), MAX_MOVIE_NUM, InitializeMovieHandle, TerminateMovieHandle, L"Movie" ) ;

	// ���ˑ��̏������������s��
	if( InitializeMovieManage_PF() < 0 )
	{
		return -1 ;
	}

	// �������t���O���Ă�
	MOVIE.InitializeFlag = TRUE ;

	// �I��
	return 0 ;
}

// ���[�r�[�֘A�̊Ǘ������̌�n��
extern int TerminateMovieManage( void )
{
	if( MOVIE.InitializeFlag == FALSE )
		return -1 ;

	// ���ˑ��̌�n���������s��
	if( TerminateMovieManage_PF() < 0 )
	{
		return -1 ;
	}

	// �n���h���Ǘ����̌�n��
	TerminateHandleManage( DX_HANDLETYPE_MOVIE ) ;

	// �������t���O�|��
	MOVIE.InitializeFlag = FALSE ;

	// �I��
	return 0 ;
}

// OPENMOVIE_GPARAM �̃f�[�^���Z�b�g����
extern void Graphics_Image_InitOpenMovieGParam( OPENMOVIE_GPARAM *GParam )
{
	GParam->RightAlphaFlag = MOVIE.RightAlphaFlag ;
	GParam->A8R8G8B8Flag = MOVIE.A8R8G8B8Flag ;
	GParam->NotUseYUVFormatSurfaceFlag = MOVIE.NotUseYUVFormatSurfaceFlag ;
}

// ���[�r�[�t�@�C�����I�[�v������
extern int OpenMovie( const wchar_t *FileName, int *Width, int *Height, int SurfaceMode )
{
	OPENMOVIE_GPARAM GParam ;

	Graphics_Image_InitOpenMovieGParam( &GParam ) ;

	return OpenMovie_UseGParam( &GParam, FileName, NULL, 0, Width, Height, SurfaceMode ) ;
}

// ���[�r�[�n���h����������������֐�
extern int InitializeMovieHandle( HANDLEINFO *HandleInfo )
{
	MOVIEGRAPH *Movie = ( MOVIEGRAPH * )HandleInfo ;

	Movie->YGrHandle  = -1 ;
	Movie->UVGrHandle = -1 ;

	// �I��
	return 0 ;
}

// ���[�r�[�n���h���̌�n�����s���֐�
extern int TerminateMovieHandle( HANDLEINFO *HandleInfo )
{
	MOVIEGRAPH *Movie = ( MOVIEGRAPH * )HandleInfo ;

	if( Movie->YGrHandle >= 0 )
	{
		NS_DeleteGraph( Movie->YGrHandle ) ;
		Movie->YGrHandle = -1 ;
	}

	if( Movie->UVGrHandle >= 0 )
	{
		NS_DeleteGraph( Movie->UVGrHandle ) ;
		Movie->UVGrHandle = -1 ;
	}

	// ���ˑ�����
	TerminateMovieHandle_PF( HandleInfo ) ;

#ifndef DX_NON_OGGTHEORA
	// ���� Theora ���g�p���Ă����ꍇ�͂��̊J���������s��
	if( Movie->TheoraFlag )
	{
		TheoraDecode_Terminate( Movie->TheoraHandle ) ;

		if( Movie->TheoraStreamData )
		{
			if( Movie->TheoraStreamDataIsFileImage == FALSE )
			{
				DX_FCLOSE( Movie->TheoraStreamData ) ;
			}
			else
			{
				MemStreamClose( Movie->TheoraStreamData ) ;
			}
			Movie->TheoraStreamData = 0 ;
		}

#ifndef DX_NON_SOUND
		NS_DeleteSoundMem( Movie->TheoraVorbisHandle ) ;
		Movie->TheoraVorbisHandle = 0 ;
#endif // DX_NON_SOUND
		Movie->TheoraHandle = 0 ;
		Movie->TheoraFlag = 0 ;
	}
#endif // DX_NON_OGGTHEORA

	if( Movie->NowImage.GraphData != NULL )
	{
		if( Movie->NowImageGraphOutAlloc == FALSE )
		{
			DXFREE( Movie->NowImage.GraphData ) ;
		}
		Movie->NowImage.GraphData = NULL ;
	}

	// ����I��
	return 0 ;
}

// OpenMovie �̃O���[�o���ϐ��ɃA�N�Z�X���Ȃ��o�[�W����
extern int OpenMovie_UseGParam(
	OPENMOVIE_GPARAM *GParam,
	const wchar_t *FileName,
	const void *FileImage,
	size_t FileImageSize,
	int *Width,
	int *Height,
	int SurfaceMode,
	int ASyncThread
)
{
	int NewHandle ;
	MOVIEGRAPH * Movie ;

	// �n���h���̍쐬
	NewHandle = AddHandle( DX_HANDLETYPE_MOVIE, ASyncThread, -1 ) ;
	if( NewHandle < 0 )
	{
		return -1 ;
	}

	if( MOVIEHCHK_ASYNC( NewHandle, Movie ) )
	{
		return -1 ;
	}

	// �E�����A���t�@�Ƃ��Ĉ������t���O���Z�b�g����
	Movie->RightAlpha = GParam->RightAlphaFlag ;

	// 32bit�J���[�t�H�[�}�b�g�̓���� A8R8G8B8 �`���Ƃ��Ĉ������ǂ����̃t���O���Z�b�g����
	Movie->A8R8G8B8Flag = GParam->A8R8G8B8Flag ;

	// YUV�t�H�[�}�b�g�̃T�[�t�F�X���g�p���Ȃ����ǂ����̃t���O���Z�b�g����
	Movie->NotUseYUVFormatSurfaceFlag = GParam->NotUseYUVFormatSurfaceFlag ;

#ifndef DX_NON_OGGTHEORA
	STREAMDATASHRED *UseStreamDataShred ;
	// Ogg Theora �Ƃ��ăI�[�v�����悤�Ƃ��Ă݂�
	if( FileName != NULL )
	{
		Movie->TheoraStreamData = ( void * )DX_FOPEN( FileName ) ;
		UseStreamDataShred = GetFileStreamDataShredStruct() ;
		Movie->TheoraStreamDataIsFileImage = FALSE ;
	}
	else
	{
		Movie->TheoraStreamData = MemStreamOpen( ( void *)FileImage, FileImageSize ) ;
		UseStreamDataShred = GetMemStreamDataShredStruct() ;
		Movie->TheoraStreamDataIsFileImage = TRUE ;
	}
	Movie->TheoraHandle = TheoraDecode_InitializeStream(
		UseStreamDataShred,
		Movie->TheoraStreamData,
		20,
		Movie->NotUseYUVFormatSurfaceFlag,
		FALSE,
		ASyncThread
	) ;
	if( Movie->TheoraHandle == 0 )
	{
		Movie->TheoraSetupGraphHandleImage = FALSE ;
		if( Movie->TheoraStreamDataIsFileImage == FALSE )
		{
			DX_FCLOSE( Movie->TheoraStreamData ) ;
		}
		else
		{
			MemStreamClose( Movie->TheoraStreamData ) ;
		}
		Movie->TheoraStreamData = 0 ;
	}
	if( Movie->TheoraHandle != 0 )
	{
		THEORADECODE_INFO Info ;

		// ���𖄂߂�
		TheoraDecode_GetInfo( Movie->TheoraHandle, &Info ) ;
		Movie->Width  = Info.Width ;
		Movie->Height = Info.Height ;
		if( Width  ) *Width  = Info.Width ;
		if( Height ) *Height = Info.Height ;
		Movie->TheoraFrameRate = Info.FrameRate ;
		Movie->TheoraTotalPlayTime = _DTOL( 1000.0 / Info.FrameRate * Info.TotalFrame ) ;

#ifndef DX_NON_SOUND
		LOADSOUND_GPARAM GParam ;

		InitLoadSoundGParam( &GParam ) ;

		// �J������ Thera �p�̃Z�b�g�A�b�v���s��

		// �T�E���h�Đ��p�ɃT�E���h�f�[�^�Ƃ��Ă��ǂݍ���
		GParam.NotInitSoundMemDelete = TRUE ;
		GParam.OggVorbisFromTheoraFile = TRUE ;
		GParam.DisableReadSoundFunctionMask = ~DX_READSOUNDFUNCTION_OGG ;
		if( FileName != NULL )
		{
			GParam.CreateSoundDataType = DX_SOUNDDATATYPE_FILE ;
			Movie->TheoraVorbisHandle = LoadSoundMemBase_UseGParam( &GParam, FileName, 1, -1, FALSE, ASyncThread ) ;
		}
		else
		{
			GParam.CreateSoundDataType = DX_SOUNDDATATYPE_MEMPRESS ;
			Movie->TheoraVorbisHandle = LoadSoundMemByMemImageBase_UseGParam( &GParam, TRUE, -1, FileImage, ( int )FileImageSize, 1, -1, FALSE, ASyncThread ) ;
		}
		Movie->TheoraVorbisTotalTime = NS_GetSoundTotalTime( Movie->TheoraVorbisHandle ) ;
		Movie->TheoraVorbisFrequency = NS_GetFrequencySoundMem( Movie->TheoraVorbisHandle ) ;

		// ���[�v�^�C�v�̌���( �����ق�����ɂ��� )
		Movie->TheoraLoopType = Movie->TheoraVorbisTotalTime > Movie->TheoraTotalPlayTime ? 1 : 0 ;

#else // DX_NON_SOUND
		// ���[�v�^�C�v�͓���f�[�^���킹
		Movie->TheoraLoopType = 0 ;
#endif // DX_NON_SOUND

		// �Đ����x�̏�����
		Movie->TheoraPlaySpeedRate = 1.0 ;

		// Theora ���g�p���Ă���t���O�𗧂Ă�
		Movie->TheoraFlag = 1 ;

		// �g�p���ׂ� BASEIMAGE ���Z�b�g
		Movie->UseNowImage = ( BASEIMAGE * )TheoraDecode_GetBaseImage( Movie->TheoraHandle ) ;

		// �T�[�t�F�X���[�h�̓m�[�}���Ƃ������Ƃ�
		Movie->SurfaceMode = DX_MOVIESURFACE_NORMAL ;
	}
	else
#endif // DX_NON_OGGTHEORA
	{
		// Ogg Theora �ȊO�̓���̓���������̍Đ��͂ł��Ȃ�
		if( FileName == NULL )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xe0\x30\xfc\x30\xd3\x30\xfc\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xaa\x30\xfc\x30\xd7\x30\xf3\x30\xe6\x51\x06\x74\x42\x66\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"���[�r�[�t�@�C���I�[�v���������Ɏ��s���܂���" @*/ ) ;
			goto ERR ;
		}

		// �I�[�v���ł��Ȃ���������ˑ��̓���t�@�C���I�[�v�������݂�
		if( OpenMovie_UseGParam_PF( Movie, GParam, FileName, Width, Height, SurfaceMode, ASyncThread ) == -1 )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xe0\x30\xfc\x30\xd3\x30\xfc\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xaa\x30\xfc\x30\xd7\x30\xf3\x30\xe6\x51\x06\x74\x42\x66\x6b\x30\xa8\x30\xe9\x30\xfc\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"���[�r�[�t�@�C���I�[�v���������ɃG���[���������܂���" @*/ ) ;
			goto ERR ;
		}
		Movie->Width  = Movie->NowImage.Width  ;
		Movie->Height = Movie->NowImage.Height ;

		// �g�p���ׂ� BASEIMAGE ���Z�b�g
		Movie->UseNowImage = &Movie->NowImage ;
	}

	// �摜���X�V���ꂽ�t���O��|��
	Movie->NowImageUpdateFlag = FALSE ;

	// �����ꎞ��~�t���O�𗧂Ă�
	Movie->SysPauseFlag = 1 ;

	// �Đ����t���O��|��
	Movie->PlayFlag = FALSE ;

	// �Đ��^�C�v�̓o�b�N�O���E���h�ɂ��Ă���
	Movie->PlayType = DX_PLAYTYPE_BACK ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( NewHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	// �n���h����Ԃ�
	return NewHandle ;

ERR :
#ifndef DX_NON_ASYNCLOAD
	if( ASyncThread )
	{
		DecASyncLoadCount( NewHandle ) ;
	}
#endif // DX_NON_ASYNCLOAD

	CloseMovie( NewHandle ) ;

	// �G���[�I��
	return -1 ;
}

// ���[�r�[�O���t�B�b�N���I������
extern int CloseMovie( int MovieHandle )
{
	return SubHandle( MovieHandle ) ;
}

// ���[�r�[�̍Đ����J�n����
extern int PlayMovie_( int MovieHandle, int PlayType, int SysPlay )
{
	MOVIEGRAPH * Movie ;

	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

	// PlayType �� -1 �̏ꍇ�͕ύX���Ȃ�
	if( PlayType == -1 ) PlayType = Movie->PlayType ;

	// PlayType �� DX_PLAYTYPE_NORMAL ���w�肵���ꍇ�� DX_PLAYTYPE_BACK �ɂȂ�
	if( PlayType == DX_PLAYTYPE_NORMAL ) PlayType = DX_PLAYTYPE_BACK ;

	// �����ꎞ��~�t���O���|��Ă����牽�����Ȃ�
	if( Movie->SysPauseFlag == 0 ) return 0 ;

	if( SysPlay == 1 && Movie->PlayFlag == FALSE ) return 0 ;

	// �摜���X�V���ꂽ�t���O��|��
	Movie->NowImageUpdateFlag = FALSE ;

	// �����ꎞ��~�t���O��|��
	Movie->SysPauseFlag = 0 ;

	// �Đ��^�C�v��ۑ�����
	Movie->PlayType = PlayType ;

	// �Đ����t���O�𗧂Ă�
	Movie->PlayFlag = TRUE ;

#ifndef DX_NON_OGGTHEORA
	// Theora ���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( Movie->TheoraFlag )
	{
		int CurFrame ;
		THEORADECODE_INFO Info ;

		// �Đ��J�n���̎��Ԃ��擾
//		Movie->TheoraPlayTime = NS_GetNowHiPerformanceCount() ;

		// �Đ��J�n���̎��Ԃ��擾
		Movie->TheoraPrevTimeCount = NS_GetNowHiPerformanceCount() ;

		// �Đ����Ԃ��Z�b�g
		TheoraDecode_GetInfo( Movie->TheoraHandle, &Info ) ;
		CurFrame = TheoraDecode_GetCurrentFrame( Movie->TheoraHandle ) ;
		Movie->TheoraPlayNowTime = _DTOL( 1000000.0 / Movie->TheoraFrameRate * CurFrame ) ;

		// ���ɍĐ��ς݂̃t���[�����������O�ɍĐ��������Ƃɂ���
//		TheoraDecode_GetInfo( Movie->TheoraHandle, &Info ) ;
//		CurFrame = TheoraDecode_GetCurrentFrame( Movie->TheoraHandle ) ;
//		Movie->TheoraPlayTime -= _DTOL( 1000000.0 / Movie->TheoraFrameRate * CurFrame ) ;

		// Vorbis �̍Đ����J�n����
#ifndef DX_NON_SOUND
//		NS_SetSoundCurrentTime( _DTOL( 1000.0 / Movie->TheoraFrameRate * CurFrame ), Movie->TheoraVorbisHandle ) ; 
		NS_PlaySoundMem( Movie->TheoraVorbisHandle, Movie->TheoraLoopType == 1 ? PlayType : DX_PLAYTYPE_BACK, FALSE ) ;
#endif // DX_NON_SOUND
	}
	else
#endif // DX_NON_OGGTHEORA
	{
		// ���ˑ��̍Đ��������s��
		PlayMovie__PF( Movie, PlayType, SysPlay ) ;
	}

	// �I��
	return 0 ;
}

// ���[�r�[�̍Đ����X�g�b�v����
extern int PauseMovie( int MovieHandle, int SysPause )
{
	MOVIEGRAPH * Movie ;

	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

	// �����ꎞ��~�t���O�������Ă�����Ȃɂ����Ȃ�
	if( Movie->SysPauseFlag == 1 ) return 0 ;

#ifndef DX_NON_OGGTHEORA
	// Theora ���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( Movie->TheoraFlag )
	{
		// Theora ���g�p���Ă���ꍇ

		// Vorbis �̍Đ����~����
#ifndef DX_NON_SOUND
		NS_StopSoundMem( Movie->TheoraVorbisHandle ) ;
#endif // DX_NON_SOUND

		// ���݂̍Đ����ԕ��܂Ńt���[����i�߂Ă���
		UpdateMovie( MovieHandle ) ;
	}
	else
#endif // DX_NON_OGGTHEORA
	{
		// ���ˑ��̍Đ���~����
		if( PauseMovie_PF( Movie, SysPause ) < 0 )
		{
			return 0 ;
		}
	}

	// �����ꎞ��~�t���O�𗧂Ă�
	Movie->SysPauseFlag = 1 ;

	// �Đ����t���O��|��
	if( SysPause == 0 )
	{
		Movie->PlayFlag = FALSE ;
	}

	// �I��
	return 0 ;
}

// ���[�r�[�̃t���[����i�߂�A�߂����Ƃ͏o���Ȃ�( ���[�r�[����~��ԂŁA���� Ogg Theora �̂ݗL�� )
extern int AddMovieFrame( int MovieHandle, unsigned int FrameNum )
{
	MOVIEGRAPH * Movie ;

	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

	// �摜���X�V���ꂽ�t���O��|��
	Movie->NowImageUpdateFlag = FALSE ;

#ifndef DX_NON_OGGTHEORA
	// Ogg Theora �ł͂Ȃ�������ύX�ł��Ȃ�
	if( Movie->TheoraFlag == FALSE )
		return -1 ;

	// �Đ�����������ύX�ł��Ȃ�
	if( GetMovieState( MovieHandle ) == TRUE )
		return -1 ;

	// �Đ��ʒu��ύX
	TheoraDecode_IncToFrame( Movie->TheoraHandle, ( int )FrameNum ) ;

	// �I��
	return 0 ;
#else // DX_NON_OGGTHEORA
	return -1 ;
#endif // DX_NON_OGGTHEORA
}

// ���[�r�[�̍Đ��ʒu��ݒ肷��(�~���b�P��)
extern int SeekMovie( int MovieHandle, int Time )
{
	MOVIEGRAPH * Movie ;

	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

	// �Đ����~�߂�
	PauseMovie( MovieHandle ) ;

	// �摜���X�V���ꂽ�t���O��|��
	Movie->NowImageUpdateFlag = FALSE ;

#ifndef DX_NON_OGGTHEORA
	// Theora ���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( Movie->TheoraFlag )
	{
		int CurFrame ;
		THEORADECODE_INFO Info ;

		// �R���o�[�g�ʒu��ύX����
		TheoraDecode_SeekToTime( Movie->TheoraHandle, Time * 1000 ) ;

		// �Đ��J�n�^�C����ύX����
//		Movie->TheoraPlayTime = NS_GetNowHiPerformanceCount() ;
		TheoraDecode_GetInfo( Movie->TheoraHandle, &Info ) ;
		CurFrame = TheoraDecode_GetCurrentFrame( Movie->TheoraHandle ) ;
		Movie->TheoraPlayNowTime = _DTOL( 1000000.0 / Movie->TheoraFrameRate * CurFrame ) ;
//		Movie->TheoraPlayTime -= _DTOL( 1000000.0 / Movie->TheoraFrameRate * CurFrame ) ;
	
		// �Đ��ʒu��ύX����
#ifndef DX_NON_SOUND
		NS_SetSoundCurrentTime( Time, Movie->TheoraVorbisHandle ) ;
#endif // DX_NON_SOUND
	}
	else
#endif // DX_NON_OGGTHEORA
	{
		// ���ˑ�����
		if( SeekMovie_PF( Movie, Time ) < 0 )
		{
			return 0 ;
		}
	}

	// �I��
	return 0 ;
}

// ���[�r�[�̍Đ����x��ݒ肷��( 1.0 = ���{��  2.0 = �Q�{�� )�A�ꕔ�̃t�@�C���t�H�[�}�b�g�݂̂ŗL���ȋ@�\�ł�
extern int SetPlaySpeedRateMovie( int MovieHandle, double SpeedRate )
{
	MOVIEGRAPH * Movie ;

	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

#ifndef DX_NON_OGGTHEORA
	// Theora ���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( Movie->TheoraFlag )
	{
		Movie->TheoraPlaySpeedRate = SpeedRate ;
#ifndef DX_NON_SOUND
		if( Movie->TheoraVorbisHandle != -1 )
		{
			NS_SetFrequencySoundMem( _DTOL( Movie->TheoraVorbisFrequency * Movie->TheoraPlaySpeedRate ), Movie->TheoraVorbisHandle ) ;
		}
		else
#endif // DX_NON_SOUND
		{
		}
	}
	else
#endif // DX_NON_OGGTHEORA
	{
		// ���ˑ�����
		if( SetPlaySpeedRateMovie_PF( Movie, SpeedRate ) < 0 )
		{
			return 0 ;
		}
	}

	// �I��
	return 0 ;
}

// ���[�r�[�̍Đ���Ԃ𓾂�
extern int GetMovieState( int MovieHandle )
{
	MOVIEGRAPH * Movie ;

	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
	{
		return -1 ;
	}

#ifndef DX_NON_OGGTHEORA
	// Theora���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( Movie->TheoraFlag )
	{
		return Movie->PlayFlag ;
	}
	else
#endif // DX_NON_OGGTHEORA
	{
		// ���ˑ�����
		return GetMovieState_PF( Movie ) ;
	}
}

// ���[�r�[�̃{�����[�����Z�b�g����(0�`10000)
extern int SetMovieVolume( int Volume, int MovieHandle )
{
	MOVIEGRAPH * Movie ;

	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

#ifndef DX_NON_OGGTHEORA
	// Theora ���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( Movie->TheoraFlag )
	{
		// Vorbis �T�E���h�̉��ʂ��Z�b�g
#ifndef DX_NON_SOUND
		NS_SetVolumeSoundMem( Volume, Movie->TheoraVorbisHandle ) ;
#endif // DX_NON_SOUND
	}
	else
#endif // DX_NON_OGGTHEORA
	{
		// ���ˑ�����
		if( SetMovieVolume_PF( Movie, Volume ) < 0 )
		{
			return 0 ;
		}
	}

	// �I��
	return 0 ;
}

// ���[�r�[�̊�{�C���[�W�f�[�^���擾����
extern BASEIMAGE *GetMovieBaseImage( int MovieHandle, int *ImageUpdateFlag, int ImageUpdateFlagSetOnly )
{
	MOVIEGRAPH * Movie ;

	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return NULL ;

#ifndef DX_NON_OGGTHEORA
	// Theora ���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( Movie->TheoraFlag )
	{
		// ���[�r�[�̃t���[�����X�V
		UpdateMovie( MovieHandle ) ;

		// NowImage �̓��e���X�V���ꂽ���ǂ����̃t���O��������
		if( ImageUpdateFlag != NULL )
		{
			*ImageUpdateFlag = Movie->NowImageUpdateFlag ;
		}
		Movie->NowImageUpdateFlag = FALSE ;

		// Theora �̃C���[�W��Ԃ�
		return ImageUpdateFlagSetOnly ? NULL : ( BASEIMAGE * )TheoraDecode_GetBaseImage( Movie->TheoraHandle ) ;
	}
#endif // DX_NON_OGGTHEORA

	// �g�p���Ă��Ȃ��ꍇ�͊��ˑ��̏������s��
	return GetMovieBaseImage_PF( Movie, ImageUpdateFlag, ImageUpdateFlagSetOnly ) ;
}

// ���[�r�[�̑��t���[�����𓾂�( Ogg Theora �ł̂ݗL�� )
extern int GetMovieTotalFrame( int MovieHandle )
{
	MOVIEGRAPH * Movie ;

	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

#ifndef DX_NON_OGGTHEORA
	THEORADECODE_INFO info ;

	// Ogg Theora �ł͂Ȃ�������ύX�ł��Ȃ�
	if( Movie->TheoraFlag == FALSE )
		return -1 ;

	// ���̎擾
	TheoraDecode_GetInfo( Movie->TheoraHandle, &info ) ;

	// ���t���[������Ԃ�
	return info.TotalFrame ;
#else // DX_NON_OGGTHEORA
	return -1 ;
#endif // DX_NON_OGGTHEORA
}

// ���[�r�[�̍Đ��ʒu���擾����(�~���b�P��)
extern int TellMovie( int MovieHandle )
{
	MOVIEGRAPH * Movie ;
	
	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

#ifndef DX_NON_OGGTHEORA
	// Theora ���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( Movie->TheoraFlag )
	{
		THEORADECODE_INFO Info ;

		TheoraDecode_GetInfo( Movie->TheoraHandle, &Info ) ;

		// �t���[�����X�V
		UpdateMovie( MovieHandle ) ;

		// �t���[������Đ����Ԃ�����o��
		return _DTOL( TheoraDecode_GetCurrentFrame( Movie->TheoraHandle ) * 1000 / Movie->TheoraFrameRate ) ;
	}
#endif // DX_NON_OGGTHEORA

	// �g�p���Ă��Ȃ��ꍇ�͊��ˑ��̏������s��
	return TellMovie_PF( Movie ) ;
}
 
// ���[�r�[�̍Đ��ʒu���擾����(�t���[���P��)
extern int TellMovieToFrame( int MovieHandle )
{
	MOVIEGRAPH * Movie ;
	
	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

#ifndef DX_NON_OGGTHEORA
	// Theora ���ǂ����ŏ����𕪊�
	if( Movie->TheoraFlag )
	{
		THEORADECODE_INFO Info ;

		TheoraDecode_GetInfo( Movie->TheoraHandle, &Info ) ;

		// �t���[�����X�V
		UpdateMovie( MovieHandle ) ;

		// ���݂̃t���[����Ԃ�
		return TheoraDecode_GetCurrentFrame( Movie->TheoraHandle ) ;
	}
#endif // DX_NON_OGGTHEORA

	// �g�p���Ă��Ȃ��ꍇ�͊��ˑ��̏������s��
	return TellMovieToFrame_PF( Movie ) ;
}

// ���[�r�[�̍Đ��ʒu��ݒ肷��(�t���[���P��)
extern int SeekMovieToFrame( int MovieHandle, int Frame )
{
	MOVIEGRAPH * Movie ;
//	D_OAFilterState State ;
	
	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

	// �摜���X�V���ꂽ�t���O��|��
	Movie->NowImageUpdateFlag = FALSE ;

#ifndef DX_NON_OGGTHEORA
	// Theora ���ǂ����ŏ����𕪊�
	if( Movie->TheoraFlag )
	{
		int CurFrame ;
		THEORADECODE_INFO Info ;

		// �R���o�[�g�ʒu��ύX����
		TheoraDecode_SeekToFrame( Movie->TheoraHandle, Frame ) ;

		// �Đ��J�n�^�C����ύX����
//		Movie->TheoraPlayTime = NS_GetNowHiPerformanceCount() ;
		TheoraDecode_GetInfo( Movie->TheoraHandle, &Info ) ;
		CurFrame = TheoraDecode_GetCurrentFrame( Movie->TheoraHandle ) ;
		Movie->TheoraPlayNowTime = _DTOL( 1000000.0 / Movie->TheoraFrameRate * CurFrame ) ;
//		Movie->TheoraPlayTime -= _DTOL( 1000000.0 / Movie->TheoraFrameRate * CurFrame ) ;
	
		// �Đ��ʒu��ύX����
#ifndef DX_NON_SOUND
		NS_SetSoundCurrentTime( _DTOL( Frame * 1000 / Movie->TheoraFrameRate ), Movie->TheoraVorbisHandle ) ;
#endif // DX_NON_SOUND
	}
	else
#endif // DX_NON_OGGTHEORA
	{
		// ���ˑ��̏������s��
		SeekMovieToFrame_PF( Movie, Frame ) ;
	}

	// �I��
	return 0 ;
}

// ���[�r�[�̂P�t���[��������̎��Ԃ𓾂�
extern LONGLONG GetOneFrameTimeMovie( int MovieHandle )
{
	MOVIEGRAPH * Movie ;
	
	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

#ifndef DX_NON_OGGTHEORA
	// Theora ���ǂ����ŏ����𕪊�
	if( Movie->TheoraFlag )
	{
		THEORADECODE_INFO Info ;

		TheoraDecode_GetInfo( Movie->TheoraHandle, &Info ) ;
		return _DTOL( 1000000 / Movie->TheoraFrameRate ) ;
	}
#endif // DX_NON_OGGTHEORA

	// ���ˑ��̏������s��
	return GetOneFrameTimeMovie_PF( Movie ) ;
}

// ���[�r�[�O���t�B�b�N�̃f�[�^���擾����
extern MOVIEGRAPH * GetMovieData( int MovieHandle )
{
	MOVIEGRAPH * Movie ;
	
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return NULL ;

	return Movie ;
}


// ���[�r�[�Ŏg�p���Ă���T�[�t�F�X�̉�����s��
extern int ReleaseMovieSurface( int MovieHandle )
{
	MOVIEGRAPH * Movie ;
	
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

#ifndef DX_NON_OGGTHEORA
	if( Movie->TheoraFlag )
	{
		TheoraDecode_SurfaceTerminate( Movie->TheoraHandle ) ;
	}
	else
#endif
	{
	}

	// �I��
	return 0 ;
}

// ���[�r�[�̍X�V���s��
extern int UpdateMovie( int MovieHandle, int AlwaysFlag )
{
	MOVIEGRAPH * Movie ;
//	LONGLONG NowFrame, OldFrame ;
	
	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

#ifndef DX_NON_OGGTHEORA
	// Theora ���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( Movie->TheoraFlag )
	{
		// Theora ���g�p���Ă���ꍇ
		int NowFrame, CurFrame, AddFrame ;
		LONGLONG NowTime ;
		THEORADECODE_INFO Info ;

		// Theora �̏����擾
		TheoraDecode_GetInfo( Movie->TheoraHandle, &Info ) ;

		// ���̍Đ����ԕ��t���[����i�߂�
		if( Movie->SysPauseFlag == 0 )
		{
			// �Đ����Ԃ͉������L���ȏꍇ�͉�������擾����
#ifndef DX_NON_SOUND
/*			if( Movie->TheoraVorbisHandle != -1 && NS_CheckSoundMem( Movie->TheoraVorbisHandle ) == 1 )
			{
				NowTime = NS_GetSoundCurrentTime( Movie->TheoraVorbisHandle ) * 1000 ;
				NowFrame = _DTOL( ( double )( NowTime * Movie->TheoraFrameRate ) / 1000000 ) ;

				// �������特���擾�����ꍇ�́A�f���̍ő�t���[���𒴂��Ă��܂����ꍇ�͌��݃t���[�����f���̍ő�t���[���ɂ���
				if( NowFrame >= Info.TotalFrame )
				{
					NowFrame = Info.TotalFrame - 1 ;
				}
			}
			else*/
#endif // DX_NON_SOUND
			{
				LONGLONG Temp64_1, Temp64_2 ;
				DWORD Temp128[ 4 ] ;

				NowTime = NS_GetNowHiPerformanceCount() ;

				// �Đ��ςݎ��Ԃ�i�߂�
				if( Movie->TheoraPlaySpeedRate < 0.999999999 || Movie->TheoraPlaySpeedRate > 1.0000000001 )
				{
					Temp64_1 = NowTime - Movie->TheoraPrevTimeCount ;
					Temp64_2 = _DTOL( Movie->TheoraPlaySpeedRate * 0x10000 ) ;
					_MUL128_1( ( DWORD * )&Temp64_1, ( DWORD * )&Temp64_2, Temp128 ) ;

					Temp64_2 = 0x10000 ;
					_DIV128_1( Temp128, ( DWORD * )&Temp64_2, ( DWORD * )&Temp64_1 ) ;

					Movie->TheoraPlayNowTime += Temp64_1 ;
				}
				else
				{
					Movie->TheoraPlayNowTime += NowTime - Movie->TheoraPrevTimeCount ;
				}

				Movie->TheoraPrevTimeCount = NowTime ;

				// ���݂̃t���[���̎Z�o
				{
					Temp64_1 = Movie->TheoraPlayNowTime ;
					Temp64_2 = _DTOL( Movie->TheoraFrameRate * 0x10000 ) ;
					_MUL128_1( ( DWORD * )&Temp64_1, ( DWORD * )&Temp64_2, Temp128 ) ;

#if defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
					Temp64_2 = 65536000000LL /* 1000000 * 0x10000 */ ;
#else
					Temp64_2 = 65536000000   /* 1000000 * 0x10000 */ ;
#endif
					_DIV128_1( Temp128, ( DWORD * )&Temp64_2, ( DWORD * )&Temp64_1 ) ;

					NowFrame = ( int )Temp64_1 ;
//					NowFrame = _DTOL( ( double )( ( NowTime - Movie->TheoraPlayTime ) * Movie->TheoraFrameRate ) / 1000000 ) ;
				}

				// ���[�v�w�肪���邩�ǂ����ő��t���[�����𒴂��Ă���ꍇ�̏����𕪊򂷂�
				if( Info.TotalFrame <= NowFrame )
				{
					if( Movie->PlayType & DX_PLAYTYPE_LOOPBIT )
					{
						// ���[�v����ꍇ�͑��t���[�����Ŋ������]����o��
						NowFrame %= Info.TotalFrame ;
					}
					else
					{
						// ���[�v���Ȃ��ꍇ�͍ŏI�t���[���Ŏ~�܂�
						NowFrame = Info.TotalFrame - 1 ;
					}
				}
			}

			// ���[�r�[�̍Đ��t���[�����擾����
			CurFrame = TheoraDecode_GetCurrentFrame( Movie->TheoraHandle ) ;

			// ��]�̃t���[�������݃o�b�t�@�Ɋi�[����Ă���t���[���ƈႤ�ꍇ�̓o�b�t�@���X�V����
			if( CurFrame != NowFrame )
			{
				Movie->NowImageUpdateFlag = TRUE ;

				// ���Z����t���[�������Z�o
				if( NowFrame < CurFrame )
				{
					// ���[�v����ꍇ
					AddFrame = Info.TotalFrame - CurFrame + NowFrame ;

#ifndef DX_NON_SOUND
					// ���[�v����ꍇ�ŁA�Đ��^�C�v�������̏ꍇ�̓T�E���h���ēx�Đ����J�n����
					if( Movie->TheoraLoopType == 0 )
					{
						NS_PlaySoundMem( Movie->TheoraVorbisHandle, DX_PLAYTYPE_BACK ) ;
					}
#endif // DX_NON_SOUND
				}
				else
				{
					AddFrame = NowFrame - CurFrame ;
				}

				// �t���[����i�߂�
				TheoraDecode_IncToFrame( Movie->TheoraHandle, AddFrame ) ;

				// �ŏI�X�V���Ԃ�ۑ�
				Movie->RefreshTime = ( int )( NowTime / 1000 ) ;
			}
			else
			{
				// ��]�̃t���[�������t���[�����I�[�ɒB���Ă��Ċ����[�v�w��ł������T�E���h�������ꍇ�͂����ōĐ��I��
				if( NowFrame == Info.TotalFrame - 1 &&
					( Movie->PlayType & DX_PLAYTYPE_LOOPBIT ) == 0
#ifndef DX_NON_SOUND
					&& ( Movie->TheoraVorbisHandle == -1 || NS_CheckSoundMem( Movie->TheoraVorbisHandle ) != 1 )
#endif // DX_NON_SOUND
					)
				{
					Movie->PlayFlag = FALSE ;

					// �����ꎞ��~�t���O�𗧂Ă�
					Movie->SysPauseFlag = 1 ;
				}
			}
		}

		// �C���[�W�̍\�z
		if( Movie->UpdateFunction )
		{
			Movie->UpdateFunction( Movie, Movie->UpdateFunctionData ) ;

			// �ŏ��̍X�V���s��ꂽ�t���O���Z�b�g����
			Movie->FirstUpdateFlag = TRUE ;
		}
	}
	else
#endif
	{
		// ���ˑ��������s��
		UpdateMovie_PF( Movie, AlwaysFlag ) ;
	}

	// �I��
	return 0 ;
}

// ���[�r�[�̍X�V���ɌĂяo���R�[���o�b�N�֐���o�^����
extern int SetCallbackMovie( int MovieHandle, void ( *Callback )( MOVIEGRAPH *Movie, void *Data ), void *Data )
{
	MOVIEGRAPH *Movie ;

	// ���[�r�[�f�[�^�n���h�����擾
	if( MOVIEHCHK( MovieHandle, Movie ) )
		return -1 ;

	// �A�h���X���Z�b�g
	Movie->UpdateFunction = Callback ;
	Movie->UpdateFunctionData = Data ;

	// �I��
	return 0 ;
}

// ���[�r�[�̍Đ���Ԃ��~����
extern int DisableMovieAll( void )
{
	// ���o�[�W�����ł͓��ɉ������Ȃ�
	return 0 ;

#if 0
	MOVIEGRAPH *Movie ;
	int i ;

	if( MOVIE.InitializeFlag == FALSE )
		return -1 ;

	// ���ׂẴ��[�r�[�ɂ��ď������s��
	for( i = HandleManageArray[ DX_HANDLETYPE_MOVIE ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_MOVIE ].AreaMax ; i ++ )
	{
		Movie = ( MOVIEGRAPH * )HandleManageArray[ DX_HANDLETYPE_MOVIE ].Handle[ i ] ;
		if( Movie == NULL ) continue ;

#ifndef DX_NON_OGGTHEORA
		// Theora ���g�p���Ă��邩�ǂ����ŏ����𕪊�
		if( Movie->TheoraFlag )
		{
			// �������Ȃ�
/*
			// �����ꎞ��~�t���O���|��Ă����珈��
			if( Movie->SysPauseFlag == 0 )
			{
				// Vorbis �̍Đ����~����
				NS_StopSoundMem( Movie->TheoraVorbisHandle ) ;

				// ���݂̍Đ����ԕ��܂Ńt���[����i�߂Ă���
				{
					int NowFrame, CurFrame ;
					LONGLONG NowTime ;
					THEORADECODE_INFO Info ;

					// Theora �̏����擾
					TheoraDecode_GetInfo( Movie->TheoraHandle, &Info ) ;

					// ���̍Đ����ԕ��t���[����i�߂�
					NowTime = NS_GetNowHiPerformanceCount() ;
					NowFrame = _DTOL( ( double )( ( NowTime - Movie->TheoraPlayTime ) * Movie->TheoraFrameRate ) / 1000000 ) ;
					CurFrame = TheoraDecode_GetCurrentFrame( Movie->TheoraHandle ) ;
					if( CurFrame != NowFrame )
					{
						if( NowFrame < CurFrame )
						{
							TheoraDecode_SeekToFrame( Movie->TheoraHandle, NowFrame ) ;
						}
						else
						{
							TheoraDecode_IncToFrame( Movie->TheoraHandle, NowFrame - CurFrame ) ;
						}
					}

					// �C���[�W�̍\�z
					TheoraDecode_SetupImage( Movie->TheoraHandle ) ;
				}

				// �����ꎞ��~�t���O�𗧂Ă�
				Movie->SysPauseFlag = 1 ;
			}
*/		}
		else
#endif
#ifndef DX_NON_DSHOW_MOVIE
		{
/*			if( Movie )
			{
				// ���݂̍Đ����Ԃ�ۑ�
				Movie->pMediaSeeking->GetCurrentPosition( &Movie->BackUpTime ) ;

				// �e��b�n�l�I�u�W�F�N�g���I������

				if( Movie->pMediaControl		){ Movie->pMediaControl->Release() ;		Movie->pMediaControl = NULL ;		}
				if( Movie->pMediaSeeking		){ Movie->pMediaSeeking->Release() ;		Movie->pMediaSeeking = NULL ;		}
				if( Movie->pBasicAudio			){ Movie->pBasicAudio->Release() ;			Movie->pBasicAudio = NULL ;			}
				if( Movie->pMovieImage			){ Movie->pMovieImage->Release() ;			Movie->pMovieImage = NULL ;			}
				if( Movie->pFilter				){ Movie->pFilter->Release() ;				Movie->pFilter = NULL ;				}
				if( Movie->pGraph				){ Movie->pGraph->Release() ;				Movie->pGraph = NULL ;				}
			}
*/		}
#else  // DX_NON_DSHOW_MOVIE
		{
		}
#endif
	}

	// �I��
	return 0 ;
#endif
}


// ���[�r�[�̍Đ���Ԃ𕜌�����
extern int RestoreMovieAll( void )
{
	// ���o�[�W�����ł͓��ɉ������Ȃ�
	return 0 ;

#if 0

	MOVIEGRAPH *Movie ;
	int i ;

	if( MOVIE.InitializeFlag == FALSE )
		return -1 ;

	// ���ׂẴ��[�r�[�ɂ��ď������s��
	for( i = HandleManageArray[ DX_HANDLETYPE_MOVIE ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_MOVIE ].AreaMax ; i ++ )
	{
		Movie = ( MOVIEGRAPH * )HandleManageArray[ DX_HANDLETYPE_MOVIE ].Handle[ i ] ;
		if( Movie == NULL ) continue ;

#ifndef DX_NON_OGGTHEORA
		// Theora ���ǂ����ŏ����𕪊�
		if( Movie->TheoraFlag )
		{
			// �������Ȃ�
/*
			// �Đ����t���O�������Ă���ꍇ�̂ݏ���
			if( Movie->PlayFlag )
			{
				// �����ꎞ��~�t���O�������Ă����珈��
				if( Movie->SysPauseFlag == 1 )
				{
					int CurFrame ;
					THEORADECODE_INFO Info ;

					// �Đ��J�n���̎��Ԃ��擾
					Movie->TheoraPlayTime = NS_GetNowHiPerformanceCount() ;

					// ���ɍĐ��ς݂̃t���[�����������O�ɍĐ��������Ƃɂ���
					TheoraDecode_GetInfo( Movie->TheoraHandle, &Info ) ;
					CurFrame = TheoraDecode_GetCurrentFrame( Movie->TheoraHandle ) ;
					Movie->TheoraPlayTime -= _DTOL( 1000000.0 / Movie->TheoraFrameRate * CurFrame ) ;

					// Vorbis �̍Đ����J�n����
					NS_PlaySoundMem( Movie->TheoraVorbisHandle, DX_PLAYTYPE_BACK, FALSE ) ;

					// �����ꎞ��~�t���O��|��
					Movie->SysPauseFlag = 0 ;
				}
			}
*/
		}
		else
#endif
#ifndef DX_NON_DSHOW_MOVIE
		{
		}
#else // DX_NON_DSHOW_MOVIE
		{
		}
#endif
	}

	// �I��
	return 0 ;
#endif  
}

// ���ׂẴ��[�r�[�O���t�B�b�N���X�^�[�g
extern int PlayMovieAll( void )
{
	MOVIEGRAPH *Movie ;
	int i ;

	if( MOVIE.InitializeFlag == FALSE )
		return -1 ;

	for( i = HandleManageArray[ DX_HANDLETYPE_MOVIE ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_MOVIE ].AreaMax ; i ++ )
	{
		Movie = ( MOVIEGRAPH * )HandleManageArray[ DX_HANDLETYPE_MOVIE ].Handle[ i ] ;
		if( Movie == NULL ) continue ; 

		PlayMovie_( Movie->HandleInfo.Handle, -1, 1 ) ;
	}

	// �I��
	return 0 ;
}

// ���ׂẴ��[�r�[�O���t�B�b�N���X�g�b�v
extern int PauseMovieAll( void )
{
	MOVIEGRAPH *Movie ;
	int i ;

	if( MOVIE.InitializeFlag == FALSE )
		return -1 ;

	for( i = HandleManageArray[ DX_HANDLETYPE_MOVIE ].AreaMin ; i <= HandleManageArray[ DX_HANDLETYPE_MOVIE ].AreaMax ; i ++ )
	{
		Movie = ( MOVIEGRAPH * )HandleManageArray[ DX_HANDLETYPE_MOVIE ].Handle[ i ] ;
		if( Movie == NULL ) continue ; 

		PauseMovie( Movie->HandleInfo.Handle, 1 ) ;
	}

	// �I��
	return 0 ;
}

// ����t�@�C���̉E�������A���t�@���Ƃ��Ĉ������ǂ������Z�b�g����( TRUE:�A���t�@���Ƃ��Ĉ��� )
extern int NS_SetMovieRightImageAlphaFlag( int Flag )
{
	MOVIE.RightAlphaFlag = Flag;
	return TRUE;
}

// �ǂݍ��ޓ���t�@�C����32bit�J���[�������ꍇ�AA8R8G8B8 �`���Ƃ��Ĉ������ǂ������Z�b�g����A
// 32bit�J���[�ł͂Ȃ�����t�@�C���ɑ΂��Ă͖���( Flag  TRUE:A8R8G8B8�Ƃ��Ĉ���  FALSE:X8R8G8B8�Ƃ��Ĉ���( �f�t�H���g ) )
extern int NS_SetMovieColorA8R8G8B8Flag( int Flag )
{
	MOVIE.A8R8G8B8Flag = Flag;
	return TRUE;
}

// �x�t�u�t�H�[�}�b�g�̃T�[�t�F�X���g�p�ł���ꍇ�͂x�t�u�t�H�[�}�b�g�̃T�[�t�F�X���g�p���邩�ǂ�����ݒ肷��( TRUE:�g�p����( �f�t�H���g ) FALSE:�q�f�a�t�H�[�}�b�g�̃T�[�t�F�X���g�p���� )
extern int NS_SetMovieUseYUVFormatSurfaceFlag( int Flag )
{
	MOVIE.NotUseYUVFormatSurfaceFlag = Flag ? FALSE : TRUE ;
	return TRUE;
}


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif
