// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Windows�p�������֌W�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMEMORYWIN_H__
#define __DXMEMORYWIN_H__

// �C���N���[�h ------------------------------------------------------------------
#include "../DxCompileConfig.h"
#include <windows.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

#define DXMEMORY_SMALLHEAP_UNITSIZE		( 4 * 1024 * 1024)			// �������������m�ۗp�̃q�[�v�̒P�ʊm�ۃT�C�Y
#define DXMEMORY_BIGHEAP_UNITSIZE		(32 * 1024 * 1024)			// �傫���������m�ۗp�̃q�[�v�̒P�ʊm�ۃT�C�Y
#define DXMEMORY_SMALL_MAX_SIZE			(64 * 1024)					// �������������m�ۗp�̃q�[�v���g�p����ő�T�C�Y

// �\���̒�` --------------------------------------------------------------------

// Windows�p�q�[�v�̏��
struct PLATFORMHEAP_WIN
{
	void *					AllocAddress ;						// �m�ۂ����������̈�̃A�h���X
	size_t					AllocSize ;							// �m�ۂ����������̈�̃T�C�Y
} ;

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXMEMORYWIN_H__
