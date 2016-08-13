// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Windows�p�t�@�C���֌W�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXFILEWIN_H__
#define __DXFILEWIN_H__

// �C���N���[�h ------------------------------------------------------------------
#include "../DxCompileConfig.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

// �t�@�C���A�N�Z�X��p�X���b�h�p�\����
struct FILEACCESSTHREAD
{
	DWORD					ThreadID ;							// �X���b�h�h�c
	HANDLE					ThreadHandle ;						// �X���b�h�n���h��
	HANDLE					Handle ;							// �t�@�C���A�N�Z�X�p�n���h��
	HANDLE					FuncEvent ;							// �w�ߎ󂯎��p�n���h��
	HANDLE					CompEvent ;							// �w�ߊ����p�n���h��
	int						Function ;							// �w��( FILEACCESSTHREAD_FUNCTION_OPEN �� )

	int						EndFlag ;							// �I���������A�t���O
	int						ErrorFlag ;							// �G���[�����t���O

	wchar_t					FilePath[FILEPATH_MAX] ;			// �t�@�C���p�X
	void					*ReadBuffer ;						// �ǂݍ��ރf�[�^���i�[����o�b�t�@�[
	ULONGLONG				ReadPosition ;						// �ǂݍ��ރf�[�^�̈ʒu
	DWORD					ReadSize ;							// �ǂݍ��ރf�[�^�̃T�C�Y(�ǂݍ��߂��f�[�^�̃T�C�Y)
	ULONGLONG				SeekPoint ;							// �t�@�C���|�C���^���ړ�����ʒu 

	BYTE					*CacheBuffer ;						// �L���b�V���o�b�t�@�ւ̃|�C���^
	ULONGLONG				CachePosition ;						// �L���b�V�����Ă���t�@�C���ʒu
	ULONGLONG				CacheSize ;							// �L���b�V���o�b�t�@�̗L���f�[�^�T�C�Y
} ;

// �E�C���h�E�Y�p�t�@�C���A�N�Z�X�����p�\����
struct FILEACCESS_PF
{
	HANDLE					Handle ;							// �t�@�C���A�N�Z�X�p�n���h��
	int						UseThread ;							// �X���b�h���g�p���邩�ǂ���
	int						UseCacheFlag ;						// �L���b�V�����g�p���邩�ǂ���

	FILEACCESSTHREAD		ThreadData ;						// �ǂݍ��ݐ�p�X���b�h�̃f�[�^
} ;

// �E�C���h�E�Y�p�t�@�C�����������p�\����
struct FINDINFO_PF
{
	HANDLE					FindHandle ;						// �t�@�C�������p�n���h��
} ;

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

extern	HANDLE		CreateTemporaryFile( wchar_t *TempFileNameBuffer, size_t BufferBytes ) ;								// �e���|�����t�@�C�����쐬����

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXFILEWIN_H__
