// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		WindowsOS�p���͏��v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXINPUTWIN_H__
#define __DXINPUTWIN_H__

// �C���N���[�h ------------------------------------------------------------------
#include "../DxCompileConfig.h"
#include "../DxLib.h"
#include "DxDirectX.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �\���̒�` --------------------------------------------------------------------

// �U���̊��ˑ����
struct INPUTVIBRATIONDATA_PF
{
	D_IDirectInputEffect	*DIEffect ;							// �p�b�h�f�o�C�X�G�t�F�N�g�I�u�W�F�N�g
} ;

// �Q�[���p�b�h�̊��ˑ����
struct INPUTPADDATA_PF
{
	int						XInputDeviceNo ;					// XInput�ŃA�N�Z�X����f�o�C�X�̏ꍇ�͂O�ȏ�̒l������( DirectInput �ň����ꍇ�� -1 )

	D_IDirectInputDevice7	*Device ;							// �p�b�h�f�o�C�X�I�u�W�F�N�g
	HANDLE					Event ;								// �p�b�h�̃C�x���g�n���h��
	int						RightStickFlag ;					// �E�X�e�B�b�N�����邩�ǂ���

	wchar_t					InstanceName[ MAX_PATH ];			// �f�o�C�X�̓o�^��
	wchar_t					ProductName[ MAX_PATH ];			// �f�o�C�X�̐��i�o�^��
} ;

// ���̓V�X�e���p���ˑ��f�[�^�\���̌^
struct INPUTSYSTEMDATA_PF
{
	HMODULE					XInputDLL ;							// �w�h���������̂c�k�k

	DWORD					( WINAPI * XInputGetStateFunc )( DWORD dwUserIndex, D_XINPUT_STATE* pState ) ;
	DWORD					( WINAPI * XInputSetStateFunc )( DWORD dwUserIndex, D_XINPUT_VIBRATION* pVibration ) ;

	int						NoUseXInputFlag ;					// �w�h�����������g�p���Ȃ����ǂ����̃t���O
	int						NoUseDirectInputFlag ;				// �c�����������h�����������g�p���Ȃ����ǂ����̃t���O
	int						UseDirectInputFlag ;				// �c�����������h�����������g�p���邩�ǂ����̃t���O( NoUseDirectInputFlag ��⏕������� )
	int						UseDirectInput8Flag ;				// �c�����������h���������W���g�p���邩�ǂ����̃t���O
	D_IDirectInput7			*DirectInputObject ;				// �c�����������h���������I�u�W�F�N�g

	D_IDirectInputDevice7	*MouseDeviceObject ;				// �}�E�X�f�o�C�X�I�u�W�F�N�g
	D_IDirectInputDevice7	*KeyboardDeviceObject ;				// �L�[�{�[�h�f�o�C�X�I�u�W�F�N�g
	HANDLE					KeyEvent ;							// �L�[�{�[�h�̃C�x���g�n���h��

	int						KeyExclusiveCooperativeLevelFlag ;	// �L�[�{�[�h�̋������x�����r�����x���ɂȂ��Ă��邩�ǂ����t���O
	int						KeyboardNotUseDirectInputFlag ;		// �L�[�{�[�h�f�o�C�X�̃A�N�Z�X�� DirectInput ���g�p���Ȃ����ǂ����̃t���O
} ;

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

extern	int	KeyboradBufferProcess( void ) ;											// �L�[�{�[�h�̃o�b�t�@����f�[�^���擾���鏈��

extern	int RefreshInputDeviceAcquireState( void ) ;								// �c�����������h���������f�o�C�X�̎擾��Ԃ��X�V����

extern	int CheckUseDirectInputMouse( void ) ;										// �}�E�X�̏��擾�ɂc�����������h�����������g�p���Ă��邩�ǂ������擾����( �߂�l  TRUE:DirectInput���g�p���Ă���  FALSE:DirectInput���g�p���Ă��Ȃ� )
extern	int GetDirectInputMouseMoveZ( int CounterReset = TRUE ) ;					// �c�����������h�����������g�p�����}�E�X�z�C�[���̈ړ��l���擾����
extern	float GetDirectInputMouseMoveZF( int CounterReset = TRUE ) ;				// �c�����������h�����������g�p�����}�E�X�z�C�[���̈ړ��l���擾����( float�� )

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXINPUTWIN_H__
