// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�E�C���h�E�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXWINDOW_H__
#define __DXWINDOW_H__

// �C���N���[�h ------------------------------------------------------------------
#include "../DxCompileConfig.h"
#include "../DxLib.h"
#include "../DxMemory.h"
#include "../DxFile.h"
#include "../DxThread.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

#define WIN_MOUSEINPUT_LOG_NUM		(32)				// �ێ�����}�E�X�̃N���b�N���

#define MAX_MENUITEMSELECT_NUM		(64)				// �ێ����Ă�����I�����ꂽ���j���[���ڂ̍ő吔
#define MAX_MENUITEM_NUM			(128)				// ���j���[���ڂ̍ő吔
#define MAX_ACCELERATOR_NUM			(64)				// �A�N�Z�����[�^�̍ő吔
#define MAX_TOOLBARITEM_NUM			(64)				// �c�[���o�[�ɒu����A�C�e���̍ő吔

#define MAX_MESTAKEOVERWIN_NUM		(100)				// ���b�Z�[�W�v���Z�X�������肷��E�C���h�E�̍ő吔
#define MAX_DRAGFILE_NUM			(1000)				// �h���b�O&�h���b�v���ꂽ�t�@�C������ۑ�����ō���
#define MAX_THREADWAIT_NUM			(4096)				// �����ɑ҂��Ƃ��o����X���b�h�̍ő吔

typedef HHOOK 						( *MSGFUNC )( HWND MainWindow, HHOOK *pKeyboardHookHandle, int Enable ) ;		// ���b�Z�[�W�t�b�N�ݒ�֐�

#define WSA_WINSOCKMESSAGE 			(WM_USER + 261)

// �\���̒�` --------------------------------------------------------------------

// ���j���[���ڂ̏��
struct WINMENUITEMINFO
{
	HMENU					Menu ;								// ���j���[
	short					Index ;								// �i���o�[
	unsigned short			ID ;								// �h�c
	wchar_t					Name[128] ;							// ���O
} ;

// �c�[���o�[���ڂ̏��
struct WINTOOLBARITEMINFO
{
	int						ID ;								// ID
	int						ImageIndex ;						// �摜�̃C���f�b�N�X
	int						Type ;								// �^�C�v
	int						State ;								// ���
	int						Click ;								// �N���b�N���ꂽ��
} ;

// �o�b�̏��
struct PCINFO
{
	wchar_t					OSString[256] ;						// �n�r�̋L�q
	wchar_t					DirectXString[256] ;				// �c�����������w�̋L�q
	wchar_t					CPUString[256] ;					// �b�o�t�̋L�q
	int						CPUSpeed ;							// �b�o�t�̑��x(�P��MHz)
	LONGLONG				FreeMemorySize ;					// �󂫃������T�C�Y(�P��byte)
	LONGLONG				TotalMemorySize ;					// ���������T�C�Y(�P��byte)
	wchar_t					VideoDriverFileName[256] ;			// �r�f�I�J�[�h�h���C�o�t�@�C����
	wchar_t					VideoDriverString[256] ;			// �r�f�I�J�[�h�h���C�o�̋L�q
	unsigned int			VideoFreeMemorySize ;				// �󂫂u�q�`�l�T�C�Y(�P��byte)
	unsigned int			VideoTotalMemorySize ;				// �u�q�`�l�̑��T�C�Y(�P��byte)
} ;

#ifndef DX_NON_INPUT
// �}�E�X�̓��͏��
struct WINMOUSEINPUT
{
	int						Button ;							// �N���b�N�����{�^��
	int						ClickX, ClickY ;					// �N���b�N�����N���C�A���g���W
} ;
#endif // DX_NON_INPUT

// �E�C���h�E�n�f�[�^�\����
struct WINDATA
{
	PCINFO					PcInfo ;							// �o�b�̏��
	HINSTANCE				Instance ;							// �\�t�g�̃C���X�^���X�n���h��
	HWND					MainWindow ;						// ���C���E�C���h�E�n���h��
	HWND					UserChildWindow ;					// �\���p�q�E�C���h�E�n���h��

	int						ProcessorNum ;						// �_���b�o�t�R�A�̐�
	int						AeroDisableFlag ;					// Aero�𖳌��ɂ��Ă��邩�ǂ����̃t���O
	int						SystembarHeight ;					// �V�X�e���o�[�̍���

#ifdef DX_THREAD_SAFE
	HANDLE					ProcessMessageThreadHandle ;		// ProcessMessage ���Ђ�����ĂтÂ���v���Z�X�̃n���h��
	DWORD					ProcessMessageThreadID ;			// ProcessMessage ���Ђ�����ĂтÂ���v���Z�X�̂h�c
	DWORD					ProcessMessageThreadExitFlag ;		// ProcessMessage ���Ђ�����ĂтÂ���v���Z�X���I�����ɗ��Ă�t���O
#endif

	HRGN					WindowRgn ;							// ���C���E�C���h�E�̃��[�W����
	int						WindowStyle ;						// �E�C���h�E�̃X�^�C���^�C�v�C���f�b�N�X�l
	int						WindowZType ;						// �E�C���h�E�̂y�I�[�_�[�^�C�v( DX_WIN_ZTYPE_NORMAL �Ȃ� )
	int						UserWindowFlag ;					// MainWindow �͂c�w���C�u�������쐬�����E�C���h�E�ł͂Ȃ����A�t���O(�s�q�t�d�F���[�U�[����n���ꂽ�E�C���h�E  �e�`�k�r�d�F�c�w���C�u�������쐬�����E�C���h�E)
	int						NotUserWindowMessageProcessDXLibFlag ;	// UserWindowFlag �������Ă���ꍇ�A�E�C���h�E�̃��b�Z�[�W�������c�w���C�u�������s��Ȃ����ǂ����A�t���O(�s�q�t�d�F�c�w���C�u�����͉����������[�U�[�����b�Z�[�W�������s���@�e�`�k�r�d�F�c�w���C�u�������s��)
	WNDPROC					DefaultUserWindowProc ;				// �c�w���C�u�����̃v���V�[�W����ݒ肷��O�Ƀ��[�U�[�̃E�C���h�E�ɐݒ肳��Ă����v���V�[�W��
	DWORD					MainThreadID ;						// InitializeWindow ���Ăяo�����X���b�h�̂h�c


	RECT					WindowEdgeRect ;					// �E�C���h�E���[�h�œ��삵�Ă��鎞�̃E�C���h�E�̋�`�̈�
	RECT					WindowRect ;						// �E�C���h�E���[�h�œ��삵�Ă��鎞�̃E�C���h�E�̃N���C�A���g�̈�
	int						WindowModeFlag ;					// �E�C���h�E���[�h�œ��삵�Ă��邩�A�̃t���O
	int						ChangeWindodwFlag ;					// �E�C���h�E���[�h�ɕύX�����A�t���O
	int						UseChangeWindowModeFlag ;			// ALT+ENTER �ɂ��t���X�N���[�������E�C���h�E�̕ύX�@�\���L���ł��邩�t���O
	void					(*ChangeWindowModeCallBackFunction)(void *) ; // �t���X�N���[�������E�C���h�E�̕ύX���N�����Ƃ��ɌĂԊ֐�
	void					*ChangeWindowModeCallBackFunctionData ;	// �R�[���o�b�N�֐��ɓn���f�[�^

	wchar_t					InputSysChara ;						// ���͂��ꂽ�V�X�e�������R�[�h

	int						QuitMessageFlag ;					// WM_QUIT���b�Z�[�W�������Ă������ǂ����̃t���O�ϐ�
	int						CloseMessagePostFlag ;				// WM_CLOSE���b�Z�[�W�𑗂������ɂs�q�t�d�ɂȂ�t���O�ϐ�
	int						DestroyMessageCatchFlag ;			// WM_DESTROY ���b�Z�[�W���������ɂs�q�t�d�ɂȂ�t���O�ϐ�
	int						DisplayChangeMessageFlag ;			// WM_DISPLAYCHANGE ���b�Z�[�W���������t���O
	
	int						ScreenCopyRequestFlag ;				// ScreenCopy �����ė~�������ǂ����̃t���O
	int						ScreenCopyRequestStartTime ;		// ScreenCopy �����ė~�������ǂ����̃t���O���������Ƃ��̎���
	int						ActiveFlag ;						// ���s���̃\�t�g���A�N�e�B�u��Ԃ��i�s�q�t�d�F�A�N�e�B�u �e�`�k�r�d�F��A�N�e�B�u�j
	int						WindowMinSizeFlag ;					// �ŏ�������Ă��邩�ǂ����̃t���O
	int						WaitTimeValidFlag ;					// WaitTime ���L�����ǂ����̃t���O
	int						WaitTime ;							// ���̃\�t�g���A�N�e�B�u�ɂȂ�A�\�t�g����~���n�߂����̃J�E���g
	int						StopFlag ;							// �ꎞ�I�ɃI�u�W�F�N�g�������[�X���Ă��邩�t���O
	int						ChangeWindowModeFlag ;				// �E�C���h�E���[�h�̕ύX���s���w�肪�N��������s�q�t�d�ɂȂ�t���O
	int						NonUserCloseEnableFlag ;			// ���[�U�[���~�{�^�������������Ƀ��C�u�������ŃE�C���h�E����邩�ǂ����̃t���O
	int						NonDxLibEndPostQuitMessageFlag ;	//�c�w���C�u�����̏I������ PostQuitMessage ���Ă΂Ȃ����ǂ����̃t���O
	int						AltF4_EndFlag ;						// �����I�����A�t���O
	int						WM_PAINTFlag ;						// WM_PAINT ���������A�t���O
	int						RecvWM_PAINTFlag;					// WM_PAINT ���������ǂ����A�t���O
	int						NotUseDxLibWM_PAINTProcess ;		// �c�w���C�u������ WM_PAINT �̏��������s���Ȃ����̃t���O
	int						UseFPUPreserve ;					// FPU���x�𗎂Ƃ��Ȃ��ݒ���g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )

	int						MouseDispFlag ;						// �}�E�X�̕\���t���O
	int						MouseDispState ;					// �}�E�X�̕\�����
	int						MouseMoveZ ;						// �}�E�X�̃z�C�[���̈ړ���
	int						MouseMoveHZ ;						// �}�E�X�̐����z�C�[���̈ړ���
#ifndef DX_NON_INPUT
	int						MouseInputNum ;						// �}�E�X�̓��͏��̐�
	WINMOUSEINPUT			MouseInput[ WIN_MOUSEINPUT_LOG_NUM ] ;	// �}�E�X�̓��͏��
#endif // DX_NON_INPUT

	int						WM_ACTIVATE_StockNum ;				// WM_ACTIVATE ���b�Z�[�W�X�g�b�N�̐�
	int						WM_ACTIVATE_StartIndex ;			// WM_ACTIVATE ���b�Z�[�W�����O�o�b�t�@�̊J�n�C���f�b�N�X
	int						WM_ACTIVATE_EndIndex ;				// WM_ACTIVATE ���b�Z�[�W�����O�o�b�t�@�̏I���C���f�b�N�X
	WPARAM					WM_ACTIVATE_wParam[ 512 ] ;			// WM_ACTIVATE �Ƀ��b�Z�[�W�������Ղ� wParam
	LPARAM					WM_ACTIVATE_lParam[ 512 ] ;			// WM_ACTIVATE �Ƀ��b�Z�[�W�������Ղ� lParam
	int						WM_ACTIVATE_APPMes[ 512 ] ;			// WM_ACTIVATEAPP �̌����肩
	int						WM_ACTIVATE_Dummy[ 512 ] ;			// WM_ACTIVATE �̃_�~�[���b�Z�[�W���ǂ���

	int						PerformanceTimerFlag ;				// �p�t�H�[�}���X�J�E���^�[���g���邩�t���O
	LONGLONG				PerformanceClock ;					// �p�t�H�[�}���X�J�E���^�[�̎��g��

//	SIZE					DefaultScreenSize ;					// �N�����̃f�X�N�g�b�v�̉�ʃT�C�Y
//	int						DefaultColorBitCount ;				// �N�����̃f�X�N�g�b�v�̃J���[�r�b�g��
//	int						DefaultRefreshRate ;				// �N�����̃f�X�N�g�b�v�̃��t���b�V�����[�g

	int						ComInitializeFlag ;					// �b�n�l���������������A�̃t���O
	int						WindowCreateFlag ;					// �E�C���h�E�쐬�����A�t���O
	int						ProcessMessageFlag ;				// ProcessMessage �����s�����A�t���O
	int						DoubleStartValidFlag ;				// �Q�d�N�����������ǂ����̃t���O(TRUE:���� FALSE:�����Ȃ�)

	BASEIMAGE				PauseGraph ;						// �A�N�e�B�u�����Ɉڂ��Ă���Ƃ��ɕ`�悷��摜
	int						PauseGraphHandle ;					// �A�N�e�B�u�����Ɉڂ��Ă���Ƃ��ɕ`�悷��摜�̃O���t�B�b�N�n���h��
	int						(*ActiveStateChangeCallBackFunction)( int ActiveState, void *UserData ) ;	// �E�C���h�E�̃A�N�e�B�u��Ԃɕω����������Ƃ��ɌĂ΂��֐�
	void					*ActiveStateChangeCallBackFunctionData ;	// ActiveStateChangeCallBackFunction �ɓn���f�[�^�A�h���X

	wchar_t					CurrentDirectory[ FILEPATH_MAX ] ;	// �N�����̃J�����g�f�B���N�g��
	int						EnableWindowText ;					// WindowText ���L�����ǂ���
	wchar_t					WindowText[ 256 ] ;					// ���C���E�C���h�E�e�L�X�g
	wchar_t					ClassName[ 256 ] ;					// ���C���E�C���h�E�̃N���X��

	int						NonActiveRunFlag ;					// �E�C���h�E���A�N�e�B�u�ł͂Ȃ��Ă����������s���邩�ǂ����̃t���O
	int						DrawBackGraphFlag ;					// DrawBackGraph �֐������s�����ǂ���( TRUE:���s��  FALSE:���s���ł͂Ȃ� )

	int						IconID ;							// �g�p����A�C�R���̂h�c
	HICON					IconHandle ;						// �g�p����A�C�R���̃n���h��

	int						UseAccelFlag ;						// �A�N�Z�����[�^�[���g�p���邩�ǂ����t���O
	HACCEL					Accel ;								// �A�N�Z�����[�^�[

	HMENU					Menu ;								// �g�p���Ă��郁�j���[�̃n���h��
	int						MenuSetupFlag ;						// ���j���[���Z�b�g���Ă��邩�ǂ����t���O( TRUE:�Z�b�g���Ă���  FALSE:���ĂȂ� )
	int						MenuUseFlag ;						// ���j���[���g�p���Ă��邩�t���O
	int						MenuResourceID ;					// ���j���[�̃��\�[�X�h�c
	int						(*MenuProc)( WORD ID ) ;			// ���j���[���b�Z�[�W�̃R�[���o�b�N�֐�
	void					(*MenuCallBackFunction)( const TCHAR *ItemName, int ItemID ) ;	// ���j���[���b�Z�[�W�̃R�[���o�b�N�֐�
	int						NotMenuDisplayFlag ;				// ���j���[�\���t���O
	int						MenuDisplayState ;					// ���j���[���\������Ă��邩�ǂ����t���O
	int						NotMenuAutoDisplayFlag ;			// ���j���[�������ŕ\�������肵�Ȃ������肷��
	int						MousePosInMenuBarFlag ;				// ���j���[�o�[�̗̈�Ƀ}�E�X�|�C���^�������Ă��邩�A�t���O
	int						SelectMenuItem[MAX_MENUITEMSELECT_NUM] ; // �I�����ꂽ���j���[���ڂ̃��X�g
	int						SelectMenuItemNum ;					// �I�����ꂽ���j���[���ڂ̐�
	WINMENUITEMINFO			MenuItemInfo[MAX_MENUITEM_NUM] ;	// ���j���[�̑I�����ڂ̏��
	int						MenuItemInfoNum ;					// ���j���[�̑I�����ڂ̐�

	int						ToolBarUseFlag ;					// �c�[���o�[���g�p���邩�ǂ����̏��
	HWND					ToolBarHandle ;						// �c�[���o�[�̃E�C���h�E�n���h��
	HBITMAP					ToolBarButtonImage ;				// �c�[���o�[�̃{�^���̃r�b�g�}�b�v
	int						ToolBarItemNum ;					// �c�[���o�[�̃A�C�e���̐�
	WINTOOLBARITEMINFO		ToolBarItem[MAX_TOOLBARITEM_NUM] ;	// �c�[���o�[�̃{�^���̏��

	int						WindowSizeChangeEnable ;			// �E�C���h�E�̃T�C�Y��ύX�ł��邩�t���O
	double					WindowSizeExRateX, WindowSizeExRateY ;	// �`���ʂ̃T�C�Y�ɑ΂���E�C���h�E�T�C�Y�̔䗦
	int						ScreenNotFitWindowSize ;			// �E�C���h�E�̃N���C�A���g�̈�ɉ�ʂ��t�B�b�g�����Ȃ����ǂ����t���O( TRUE:�t�B�b�g�����Ȃ�  FALSE:�t�B�b�g���� )
	int						WindowWidth, WindowHeight ;			// �E�C���h�E�̃T�C�Y( ���[�U�[�w�� )
	int						WindowSizeValid ;					// �E�C���h�E�̃T�C�Y���L�����ǂ���( TRUE:�L��  FALSE:���� )
	int						WindowSizeValidResetRequest ;		// �E�C���h�E�̃T�C�Y���L�����ǂ����̃t���O�����Z�b�g���郊�N�G�X�g�t���O
	int						WindowX, WindowY ;					// �E�C���h�E�̈ʒu( ���[�U�[�w�� )
	int						WindowPosValid ;					// �E�C���h�E�̈ʒu���L�����ǂ���( TRUE:�L��  FALSE:���� )
	int						WindowMaxWidth, WindowMaxHeight ;	// �E�C���h�E�̍ő�T�C�Y( ���[�U�[�w�� )
	int						WindowMaxSizeValid ;				// �E�C���h�E�̍ő�T�C�Y���L�����ǂ���( TRUE:�L��  FALSE:���� )
	int						WindowMinWidth, WindowMinHeight ;	// �E�C���h�E�̍ŏ��T�C�Y( ���[�U�[�w�� )
	int						WindowMinSizeValid ;				// �E�C���h�E�̍ŏ��T�C�Y���L�����ǂ���( TRUE:�L��  FALSE:���� )

	int						VisibleFlag ;
	int						NotWindowVisibleFlag ;				// �E�C���h�E��\�����Ȃ��t���O
	int						WindowMinimizeFlag ;				// �E�C���h�E���ŏ�����Ԃɂ��邩�ǂ����̃t���O
	int						WindowMaximizeFlag ;				// �E�C���h�E���ő剻��Ԃɂ��邩�ǂ����̃t���O
	RECT					WindowMaximizedClientRect ;			// �E�C���h�E�ő剻��Ԃł̃N���C�A���g��`
	RECT					WindowMaximizedRect ;				// �E�C���h�E�ő剻��Ԃł̃E�C���h�E��`
	RECT					WindowMaximizedAdjustRect ;			// �E�C���h�E�ő剻��Ԃł̃E�C���h�E��`���c�w���C�u�������ŕ␳��������
	int						ValidFirstWindowMaximizedRect ;		// ��ԍŏ��̍ő剻��Ԃł̃E�C���h�E�̋�`���L�����ǂ���( TRUE:�L��  FALSE:���� )
	RECT					FirstWindowMaximizedRect ;			// ��ԍŏ��̍ő剻��Ԃł̃E�C���h�E�̋�`
	int						NotMoveMousePointerOutClientAreaFlag ;	// �E�C���h�E�̃N���C�A���g�G���A�O�Ƀ}�E�X�|�C���^���ړ��ł��Ȃ��悤�ɂ��邩�ǂ����̃t���O
	int						NotActive_WindowMoveOrSystemMenu ;	// WM_ENTERSIZEMOVE �� WM_ENTERMENULOOP �ɂ���Ĕ�A�N�e�B�u�ɂȂ��Ă���
	int						SetClipCursorFlag ;					// ClipCursor �̐ݒ肪�L���ɂȂ��Ă��邩�ǂ����t���O
	int						SysCommandOffFlag ;					// �^�X�N�X�C�b�`��}�����鏈�����s�����t���O
	HHOOK					TaskHookHandle ;					// �t�b�N�n���h��
	HHOOK					GetMessageHookHandle ;				// WH_GETMESSAGE �t�b�N�n���h��
	HHOOK					KeyboardHookHandle ;				// WH_KEYBOARD_LL �t�b�N�n���h��
	int						LockInitializeFlag ;				// ���b�N��������Ƃ����t���O
	wchar_t					HookDLLFilePath[FILEPATH_MAX] ;		// �t�b�N����������c�k�k�t�@�C���ւ̃p�X
	int						NotUseUserHookDllFlag ;				// ���[�U�[�w��̂c�k�k���g�p���Ă��Ȃ����ǂ����t���O
	HMODULE					LoadResourModule ;					// ���\�[�X����ǂݍ��ތn�Ŏg�p���郂�W���[��( NULL �̏ꍇ�� GetModuleHandle( NULL ) ���g�p )
	int						WM_PAINTMessageFlag ;				// WM_PAINT ���b�Z�[�W�������ۂ� TRUE �ɂȂ�t���O

	int						BackBufferTransColorFlag ;			// �o�b�N�o�b�t�@�̓��ߐF�̕����𓧉߂����邩�ǂ����̃t���O( TRUE:���߂�����  FALSE:���߂����Ȃ� )
	int						UseUpdateLayerdWindowFlag ;			// UpdateLayerdWindow ���g�p���邩�ǂ����̃t���O( TRUE;�g�p����  FALSE:�g�p���Ȃ� )
	HBITMAP					BackBufferTransBitmap ;				// �o�b�N�o�b�t�@�̓��ߐF�̕����𓧉߂����邽�߂̃r�b�g�}�b�v
	void					*BackBufferTransBitmapImage ;		// BackBufferTransBitmap �̃C���[�W�̐擪�A�h���X
	SIZE					BackBufferTransBitmapSize ;			// BackBufferTransBitmap �̃T�C�Y
	COLORDATA				BackBufferTransBitmapColorData ;	// BackBufferTransBitmap �̃J���[�t�H�[�}�b�g

	HINSTANCE				MessageHookDLL ;					// ���b�Z�[�W�t�b�N�c�k�k�̃n���h��
	MSGFUNC					MessageHookCallBadk ;				// ���b�Z�[�W�t�b�N�̃R�[���o�b�N�֐�
	DWORD					MessageHookThredID ;				// ���b�Z�[�W�t�b�N�c�k�k�̃X���b�h�h�c

	int						DirectXVersion ;					// DirectX�̃o�[�W����
	int						WindowsVersion ;					// Windows�̃o�[�W����
	int						UseMMXFlag ;						// MMX���g���邩�t���O
	int						UseSSEFlag ;						// SSE���g���邩�ǂ����t���O
	int						UseSSE2Flag ;						// SSE2���g���邩�ǂ����t���O
	int						UseRDTSCFlag ;						// RDTSC ���߂��g���邩�A�t���O
	LONGLONG				OneSecCount ;						// ��b�Ԃ�����̃N���b�N��

	WNDPROC					UserWindowProc ;					// ���[�U�[��`�̃E�C���h�E�v���[�W��
	int						UseUserWindowProcReturnValue ;		// ���[�U�[��`�̃E�C���h�E�v���[�W���̖߂�l���g�p���邩�ǂ���( TRUE:�g�p����  FALSE:�g�p���Ȃ� )

	HWND					MesTakeOverWindow[MAX_MESTAKEOVERWIN_NUM] ;	// ���b�Z�[�W�����������肷��E�C���h�E�̃��X�g
	int						MesTakeOverWindowNum ;				// ���b�Z�[�W�����������肷��E�C���h�E�̐�
	
	POINT					InitializeWindowPos ;				// �����E�C���h�E�̈ʒu
	int						InitializeWindowPosValidFlag ;		// �����E�C���h�E�̈ʒu�̍��W�̗L���t���O
	
	HWND					DialogBoxHandle ;					// �_�C�A���O�{�b�N�X�̃n���h��
	
	int						DragFileValidFlag ;					// �t�@�C���̃h���b�O���h���b�v���������t���O
	wchar_t					*DragFileName[MAX_DRAGFILE_NUM] ;	// �h���b�O���h���b�v���ꂽ�t�@�C����
	int						DragFileNum ;						// �h���b�O���h���b�v���ꂽ�t�@�C���̐�

//	WINFILEACCESS			*ReadOnlyFileAccessFirst ;			// �t�@�C���A�N�Z�X�\���̃��X�g�̐擪�ւ̃|�C���^
//	int						ReadOnlyFileAccessLockFlag ;		// �t�@�C���A�N�Z�X�\���̃��X�g�ւ̃A�N�Z�X���֎~���Ă��邩�A�t���O

#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )
//	int						DxConflictCheckFlag ;				// �c�w���C�u�����̊֐��������̃^�X�N���瓯���Ɏ��s����Ȃ��悤�ɂ��邽�߂̃J�E���^���̂P
	int						DxConflictCheckCounter ;			// �c�w���C�u�����̊֐��������̃^�X�N���瓯���Ɏ��s����Ȃ��悤�ɂ��邽�߂̃J�E���^���̂Q
	int						DxUseThreadFlag ;					// �c�w���C�u�������g�p���Ă���X���b�h�����邩�ǂ����A�t���O( FALSE:�Ȃ�  TRUE:���� )
	DWORD					DxUseThreadID ;						// �c�w���C�u�������g�p���Ă���X���b�h�̂h�c
	DWORD_PTR				DxConflictWaitThreadID[MAX_THREADWAIT_NUM][2] ;	// 0:�҂��X���b�h��ID  1:�C�x���g�n���h��
	int						DxConflictWaitThreadIDInitializeFlag ;	// DxConflictWaitThreadID �����������Ă��邩�ǂ����̃t���O
	int						DxConflictWaitThreadNum ;			// �҂��X���b�h�̐�
	DX_CRITICAL_SECTION		DxConflictCheckCriticalSection ;	// �Փ˖h�~�p�N���e�B�J���Z�N�V����
#endif
} ;

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �E�C���h�E�̃f�[�^
extern WINDATA WinData ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �b�n�l�������A�I���֌W�֐�
extern	int			InitializeCom( void ) ;													// �b�n�l�C���^�[�t�F�[�X������������
extern	int			TerminateCom( void ) ;													// �b�n�l�C���^�[�t�F�[�X���I������

// �������I���n�֐�
extern int			InitializeWindow( void ) ;												// �E�C���h�E�֌W�̏������֐�
extern int			TerminateWindow( void ) ;												// �E�C���h�E�֌W�̏����I���֐�

// ���o�͌n
extern	int			OutSystemInfo( void ) ;													// �n�r��c�����������w�̃o�[�W�������擾����

// �Փ˖h�~�p�֐�
//extern	void	PreparationDxFunction( void ) ;											// �c�w���C�u���������Ō��J�֐����g�p����O�ɌĂԊ֐�
extern	void		CheckConflictAndWaitDxFunction( void ) ;								// �����ɂc�w���C�u�����̊֐����Ă΂�Ă��Ȃ����`�F�b�N���āA�����ɌĂ΂�Ă�����҂֐�
extern	void		PostConflictProcessDxFunction( void ) ;									// CheckConfictAndWaitDxFunction ���g�p�����c�w���C�u�����̊֐��� return ����O�ɌĂԂׂ��֐�

// �\�t�g�̃E�C���h�E�Ƀt�H�[�J�X���ڂ�
extern	void		SetAbsoluteForegroundWindow( HWND hWnd, int Flag = TRUE ) ;


// wchar_t�Ŋ֐�
extern	int			GetResourceInfo_WCHAR_T(			const wchar_t *ResourceName , const wchar_t *ResourceType , void **DataPointerP , int *DataSizeP ) ;
extern	int			GetPcInfo_WCHAR_T(					wchar_t *OSString , wchar_t *DirectXString , wchar_t *CPUString , int *CPUSpeed /* �P��MHz */ , double *FreeMemorySize /* �P��MByte */ , double *TotalMemorySize , wchar_t *VideoDriverFileName , wchar_t *VideoDriverString , double *FreeVideoMemorySize /* �P��MByte */ , double *TotalVideoMemorySize ) ;
extern	wchar_t		GetInputSystemChar_WCHAR_T(			int DeleteFlag ) ;
extern	int			LoadPauseGraph_WCHAR_T(				const wchar_t *FileName ) ;
extern	int			SetMainWindowText_WCHAR_T(			const wchar_t *WindowText ) ;
extern	int			SetMainWindowClassName_WCHAR_T(		const wchar_t *ClassName ) ;
extern	int			SetSysCommandOffFlag_WCHAR_T(		int Flag , const wchar_t *HookDllPath = NULL ) ;
extern	int			GetClipboardText_WCHAR_T(			wchar_t *DestBuffer ) ;
extern	int			SetClipboardText_WCHAR_T(			const wchar_t *Text ) ;
extern	int			GetDragFilePath_WCHAR_T(			wchar_t *FilePathBuffer ) ;
extern	int			SetWindowRgnGraph_WCHAR_T(			const wchar_t *FileName ) ;
extern	int			SetupToolBar_WCHAR_T(				const wchar_t *BitmapName , int DivNum , int ResourceID = -1 ) ;
extern	int			AddKeyAccel_WCHAR_T(				const wchar_t *ItemName , int ItemID , int KeyCode , int CtrlFlag , int AltFlag , int ShiftFlag ) ;
extern	int			AddKeyAccel_Name_WCHAR_T(			const wchar_t *ItemName , int KeyCode , int CtrlFlag , int AltFlag , int ShiftFlag ) ;
extern	int			AddMenuItem_WCHAR_T(				int AddType /* MENUITEM_ADD_CHILD�� */ , const wchar_t *ItemName, int ItemID, int SeparatorFlag, const wchar_t *NewItemName = NULL , int NewItemID = -1 ) ;
extern	int			DeleteMenuItem_WCHAR_T(				const wchar_t *ItemName, int ItemID ) ;
extern	int			CheckMenuItemSelect_WCHAR_T(		const wchar_t *ItemName, int ItemID ) ;
extern	int			SetMenuItemEnable_WCHAR_T(			const wchar_t *ItemName, int ItemID, int EnableFlag ) ;
extern	int			SetMenuItemMark_WCHAR_T(			const wchar_t *ItemName, int ItemID, int Mark ) ;
extern	int			AddMenuItem_Name_WCHAR_T(			const wchar_t *ParentItemName, const wchar_t *NewItemName ) ;
extern	int			AddMenuLine_Name_WCHAR_T(			const wchar_t *ParentItemName ) ;
extern	int			InsertMenuItem_Name_WCHAR_T(		const wchar_t *ItemName, const wchar_t *NewItemName ) ;
extern	int			InsertMenuLine_Name_WCHAR_T(		const wchar_t *ItemName ) ;
extern	int			DeleteMenuItem_Name_WCHAR_T(		const wchar_t *ItemName ) ;
extern	int			CheckMenuItemSelect_Name_WCHAR_T(	const wchar_t *ItemName ) ;
extern	int			SetMenuItemEnable_Name_WCHAR_T(		const wchar_t *ItemName, int EnableFlag ) ;
extern	int			SetMenuItemMark_Name_WCHAR_T(		const wchar_t *ItemName, int Mark ) ;
extern	int			AddMenuItem_ID_WCHAR_T(				int ParentItemID, const wchar_t *NewItemName, int NewItemID = -1 ) ;
extern	int			GetMenuItemID_WCHAR_T(				const wchar_t *ItemName ) ;
extern	int			GetMenuItemName_WCHAR_T(			int ItemID, wchar_t *NameBuffer ) ;


/*
extern __inline void PreparationDxFunction( void )
{
#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )
	// ���C���J�E���^���C���N�������g����
	WinData.DxConflictCheckCounter ++ ;
#endif
}
*/


// �ݒ�n�֐�
extern	int			SetWindowModeFlag( int Flag ) ;											// �E�C���h�E���[�h�ŋN�����邩���Z�b�g
extern	int			SetWindowStyle( int CenterPosition = TRUE ) ;							// �E�C���h�E�̃X�^�C�����Z�b�g����
extern	int			RefreshDragFileValidFlag( void ) ;										// �t�@�C���̃h���b�O���h���b�v�@�\��L���ɂ��邩�ǂ����̐ݒ���Đݒ肷��


// ���擾�x����
extern	PCINFO		*GetPcInfoStructP( void ) ;												// �o�b���\���̂̃A�h���X�𓾂�
extern	HWND		GetDisplayWindowHandle( void ) ;										// �\���Ɏg�p����E�C���h�E�̃n���h�����擾����


// �֗��֐�
//extern	int			CheckConflict( int *Check_Inc_Counter ) ;									// �}���`�X���b�h���̏Փ˂�h�����߂̊֐�
//extern	int			CheckConflict2( int *Check_NonInc_Counter, int *NonCheck_Inc_Counter ) ;	// �}���`�X���b�h���̏Փ˂�h�����߂̊֐�2
//extern	int			CheckConflict3( int *Check_NonInc_Counter, int *Check_Inc_Counter ) ;		// �}���`�X���b�h���̏Փ˂�h�����߂̊֐�3

// ���b�Z�[�W�����֐�
extern	void		DxActiveWait( void ) ;													// �A�N�e�B�u�ɂȂ�܂ŉ������Ȃ�
extern	int			CheckActiveWait( void ) ;												// ��A�N�e�B�u���ǂ������`�F�b�N����
extern	int			GetQuitMessageFlag( void ) ;											// WM_QUIT �����s����Ă��邩�ǂ������擾����
extern	int			WM_ACTIVATEProcessUseStock( WPARAM wParam, LPARAM lParam, int APPMes = FALSE, int Dummy = FALSE ) ;
extern	int			WM_ACTIVATEProcess(         WPARAM wParam, LPARAM lParam, int APPMes = FALSE, int Dummy = FALSE ) ;

// Aero �̗L���A�����ݒ�
extern	int			SetEnableAero( int Flag ) ;

// �c�[���o�[�֌W
extern	int			GetToolBarHeight( void ) ;													// �c�[���o�[�̍����𓾂�

// ���j���[�֌W
extern	int			MenuAutoDisplayProcess( void ) ;																// ���j���[�������I�ɕ\���������\���ɂ����肷�鏈�����s��

// �⏕�֐�
extern	void		_FileTimeToLocalDateData( FILETIME *FileTime, DATEDATA *DateData ) ;
extern	int			UpdateBackBufferTransColorWindow( const BASEIMAGE *SrcImage, const RECT *SrcImageRect = NULL, HDC Direct3DDC = NULL, int NotColorKey = FALSE, int PreMultipliedAlphaImage = FALSE ) ;	// �w��̊�{�C���[�W���g�p���� UpdateLayeredWindow ���s��

// ���\�[�X�֌W
extern	int			GetBmpImageToResource( int ResourceID, BITMAPINFO **BmpInfoP, void **GraphDataP ) ;					// �a�l�o���\�[�X���� BITMAPINFO �Ɖ摜�C���[�W���\�z����

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXWINDOW_H__
