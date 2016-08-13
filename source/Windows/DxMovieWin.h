// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		WindowsOS�p����v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMOVIEWIN_H__
#define __DXMOVIEWIN_H__

#include "../DxCompileConfig.h"

#ifndef DX_NON_MOVIE

// �C���N���[�h ------------------------------------------------------------------
#include "DxUseCStrmBaseFilter.h"
#include "DxGuid.h"
#include "../DxLib.h"
#include "../DxFile.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

// ���[�r�[�O���t�B�b�N���ˑ��f�[�^�^
struct MOVIEGRAPH_PF
{
	int						Dummy ;

#ifndef DX_NON_DSHOW_MOVIE
	D_IGraphBuilder			*pGraph ;						// �t�B���^�O���t�}�l�[�W��
	D_IMediaControl			*pMediaControl ;				// ���f�B�A�R���g���[��
	D_IMediaSeeking			*pMediaSeeking ;				// ���f�B�A�V�[�L���O
	D_IBasicAudio			*pBasicAudio ;					// BasicAudio �C���^�[�t�F�C�X

//	D_ISampleGrabber		*SampleGrabber ;				// �T���v���O���b�o�I�u�W�F�N�g

	D_CMovieRender			*pMovieImage ;					// ����̃C���[�W

	D_STREAM_TIME			FrameTime ;						// �P�t���[��������̎���
	D_STREAM_TIME			OldTime ;						// �O��`�掞�̎���
	D_STREAM_TIME			StartTime ;						// �Đ��J�n����

	D_STREAM_TIME			BackUpTime ;					// �Đ����~�߂�����
	int						UseTemporaryFile ;				// �e���|�����t�@�C�����g�p���Ă��邩�ǂ����A�t���O
	wchar_t					FileName[ FILEPATH_MAX ] ;		// �t�@�C���l�[��
#endif // DX_NON_DSHOW_MOVIE
} ;

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MOVIE

#endif // __DXMOVIEWIN_H__
