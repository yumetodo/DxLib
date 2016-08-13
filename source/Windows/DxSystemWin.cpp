// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Windows�p�V�X�e���v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "DxSystemWin.h"

// �C���N���[�h ------------------------------------------------------------------
#include "DxWindow.h"
#include "DxGraphicsWin.h"
#include "../DxLib.h"
#include "../DxSystem.h"
#include "../DxArchive_.h"
#include "../DxGraphics.h"
#include "../DxModel.h"
#include "../DxFile.h"
#include "../DxNetwork.h"
#include "../DxInputString.h"
#include "../DxInput.h"
#include "../DxSound.h"
#include "../DxSoundConvert.h"
#include "../DxBaseImage.h"
#include "../DxSoftImage.h"
#include "../DxMovie.h"
#include "../DxFont.h"
#include "../DxLog.h"
#include "../DxASyncLoad.h"
#include "../DxUseCLib.h"


#include "../DxBaseFunc.h"
#include "../DxMemory.h"
#include "DxWindow.h"
#include "DxWinAPI.h"
#include "DxGuid.h"


#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

//#define MAX_EVENTPROCESS_NUM						(5)					// ��x�ɏ�������C�x���g�̍ő吔

// �\���̒�` --------------------------------------------------------------------

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �v���O���� --------------------------------------------------------------------

// ���C�u�����������֐�
extern int NS_DxLib_Init( void )
{
	// ���ɏ������ς݂̏ꍇ�͉��������I��
	if( DxSysData.DxLib_InitializeFlag == TRUE ) return 0 ;

	DXST_ERRORLOGFMT_ADDUTF16LE(( "\x24\xff\x38\xff\xe9\x30\xa4\x30\xd6\x30\xe9\x30\xea\x30\x6e\x30\x1d\x52\x1f\x67\x16\x53\xe6\x51\x06\x74\x8b\x95\xcb\x59\x00"/*@ L"�c�w���C�u�����̏����������J�n" @*/ )) ;
	DXST_ERRORLOG_TABADD ;

	// ���������t���O�𗧂Ă�
	DxSysData.DxLib_RunInitializeFlag = TRUE ;

	// QuitMessage ���΂����t���O��|��
	WinData.QuitMessageFlag = FALSE ;

	// DxSysData �̋��ʏ���������
	DxLib_SysInit() ;

	// �L�����N�^�[�R�[�h�֌W�̏��������s��
	InitCharCode() ;

	// DxBaseFunc �̏�����
	_INIT_BASEFUNC() ;

	// �g�p���镶���Z�b�g���Z�b�g
	_SET_DEFAULT_CHARCODEFORMAT() ;

	// API ��ǂݍ���
	LoadWinAPI() ;

#ifndef DX_NON_OGGTHEORA
	// Theora �p�̏�����
	TheoraDecode_GrobalInitialize() ;
#endif

	// �A�[�J�C�u�t�@�C���A�N�Z�X�p�̃f�[�^��������
#ifndef DX_NON_DXA
	DXA_DIR_Initialize() ;
#endif

	// �X�g���[���f�[�^�ǂݍ��ݐ���p�|�C���^�\���̂̃f�t�H���g�l���Z�b�g
	NS_ChangeStreamFunction( NULL ) ;

	// �J�����g�f�B���N�g���ۑ�
	DX_FGETDIR( WinData.CurrentDirectory, sizeof( WinData.CurrentDirectory ) ) ; 

#ifndef DX_NON_LOG
	// �G���[���O�r�o�̏�����
	ErrorLogInitialize() ;
#endif

#ifndef DX_NON_GRAPHICS

	// �t���X�N���[�����[�h�̏ꍇ�͎w��̉�ʉ𑜓x���g�p�\���ǂ����𒲂ׂ�
	if( WinData.WindowModeFlag == FALSE )
	{
		int Num, i, Width, Height, ColorBitDepth ;
		int DisplayIndex ;
		DISPLAYMODEDATA Mode ;

SCREENMODECHECK:
		ColorBitDepth = NS_GetColorBitDepth() ;
		NS_GetDrawScreenSize( &Width, &Height ) ; 

		DXST_ERRORLOGFMT_ADDUTF16LE(( "\x2d\x8a\x9a\x5b\x55\x30\x8c\x30\x66\x30\x44\x30\x8b\x30\x3b\x75\x62\x97\x2d\x8a\x9a\x5b\x20\x00\x25\x00\x64\x00\x78\x00\x25\x00\x64\x00\x20\x00\x25\x00\x64\x00\x62\x00\x69\x00\x74\x00\x20\x00\x63\x00\x6f\x00\x6c\x00\x6f\x00\x72\x00\x00"/*@ L"�ݒ肳��Ă����ʐݒ� %dx%d %dbit color" @*/, Width, Height, ColorBitDepth ) ) ;

		DisplayIndex = GSYS.Screen.ValidUseDisplayIndex ? GSYS.Screen.UseDisplayIndex : 0 ;

		Num = NS_GetDisplayModeNum( DisplayIndex ) ;
		for( i = 0 ; i < Num ; i ++ )
		{
			Mode = NS_GetDisplayMode( i, DisplayIndex ) ;
//			DXST_ERRORLOGFMT_ADDUTF16LE(( L"%dx%d %dbit color", Mode.Width, Mode.Height, Mode.ColorBitDepth ) ) ;
			if( Mode.Width == Width && Mode.Height == Height && Mode.ColorBitDepth == ColorBitDepth )
				break ;
		}
		if( i == Num )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xfe\x5b\xdc\x5f\x57\x30\x66\x30\x44\x30\x8b\x30\x3b\x75\x62\x97\xe2\x30\xfc\x30\xc9\x30\x6a\x30\x57\x30\x00"/*@ L"�Ή����Ă����ʃ��[�h�Ȃ�" @*/ )) ;

			// 640x480 16bit�����߂������ꍇ�� 32bit�������Ă݂�
			if( Width == 640 && Height == 480 && ColorBitDepth == 16 )
			{
				NS_SetGraphMode( Width, Height, 32, GSYS.Screen.MainScreenRefreshRate ) ;
				goto SCREENMODECHECK ;
			}
		}
	}

	// 24�r�b�g�J���[�`�F�b�N
	if( DxSysData.NotWinFlag == FALSE )
	{
		int CrBitNum ;

		// ���݂̉�ʃ��[�h�ł̐F�̐����擾
		{
			HDC hdc ;
			hdc = GetDC( NULL ) ;
			CrBitNum = GetDeviceCaps( hdc, PLANES ) * GetDeviceCaps( hdc, BITSPIXEL ) ;
			ReleaseDC( NULL, hdc ) ;
		}

		if( ( CrBitNum == 24 && WinData.WindowModeFlag == TRUE ) ||
			( NS_GetColorBitDepth() == 24 ) )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x24\xff\x38\xff\xe9\x30\xa4\x30\xd6\x30\xe9\x30\xea\x30\x6f\x30\x12\xff\x14\xff\xd3\x30\xc3\x30\xc8\x30\xab\x30\xe9\x30\xfc\x30\xe2\x30\xfc\x30\xc9\x30\x6b\x30\xfe\x5b\xdc\x5f\x57\x30\x66\x30\x44\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"�c�w���C�u�����͂Q�S�r�b�g�J���[���[�h�ɑΉ����Ă��܂���\n" @*/ ) ;

			DXST_ERRORLOG_TABSUB ;
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x24\xff\x38\xff\xe9\x30\xa4\x30\xd6\x30\xe9\x30\xea\x30\x6e\x30\x1d\x52\x1f\x67\x16\x53\xe6\x51\x06\x74\x31\x59\x57\x65\x00"/*@ L"�c�w���C�u�����̏������������s" @*/ ) ) ;
			return -1 ;
		}
	}

#endif // DX_NON_GRAPHICS

	// �p�t�H�[�}���X�J�E���^�[���̏�����
	{
		// �p�t�H�[�}���X�J�E���^�[�̎��g�����擾����
		QueryPerformanceFrequency( ( LARGE_INTEGER * )&WinData.PerformanceClock ) ;

		// �g���邩�ǂ�����ۑ����Ă���
		WinData.PerformanceTimerFlag = WinData.PerformanceClock != 0 ;
	}

	// �V�X�e�����O���o��
	OutSystemInfo() ;

#ifndef DX_NON_GRAPHICS
	// �f�t�H���g�̃O���t�B�b�N�����֐���o�^
	NS_SetRestoreGraphCallback( NULL ) ;
#endif // DX_NON_GRAPHICS

	// �e�����n�̏�����
	InitializeCom() ;											// �b�n�l�̏�����
	if( InitializeWindow() == -1 ) goto ERROR_DX ;				// �E�C���h�E�֌W�̏�����
#ifndef DX_NON_NETWORK
#if 0
	if( HTTP_Initialize() == -1 ) goto ERROR_DX ;				// HTTP �֌W�̏����̏�����
#endif
#endif

	if( DxSysData.NotInputFlag == FALSE )
	{
#ifndef DX_NON_INPUT
		if( InitializeInputSystem() == -1 ) goto ERROR_DX ;			// ���̓V�X�e���̏�����
#endif // DX_NON_INPUT
	}

	if( DxSysData.NotSoundFlag == FALSE )
	{
#ifndef DX_NON_SOUND
		InitializeSoundConvert() ;									// �T�E���h�ϊ������̏�����
		InitializeSoundSystem() ;									// �T�E���h�V�X�e���̂̏�����
#endif // DX_NON_SOUND
	}
	if( DxSysData.NotDrawFlag == FALSE )
	{
		InitializeBaseImageManage() ;
#ifndef DX_NON_SOFTIMAGE
		InitializeSoftImageManage() ;
#endif // DX_NON_SOFTIMAGE
#ifndef DX_NON_MOVIE
		InitializeMovieManage() ;
#endif

#ifndef DX_NON_GRAPHICS
		if( Graphics_Initialize() < 0 ) goto ERROR_DX ;
#endif // DX_NON_GRAPHICS
	}
#ifndef DX_NON_INPUTSTRING
	InitializeInputCharBuf() ;									// �����R�[�h�o�b�t�@�̏�����
#endif // DX_NON_INPUTSTRING

	// �c�w���C�u���������������t���O�����Ă�
	DxSysData.DxLib_InitializeFlag = TRUE ;

	// �u�r�x�m�b�҂�������
//	NS_SetWaitVSyncFlag( TRUE ) ;

	// �t���X�N���[�����[�h�������ꍇ�E�C���h�E���őO�ʂɎ����Ă���
	if( WinData.WindowModeFlag == FALSE && WinData.UserWindowFlag == FALSE )
		SetAbsoluteForegroundWindow( WinData.MainWindow ) ;
	
#if !defined( DX_NON_LOG ) && !defined( DX_NON_PRINTF_DX )
	// ���O�o�͏����̏��������s��
	InitializeLog() ;
#endif

#ifndef DX_NON_GRAPHICS
	// �`���̕ύX
	NS_SetDrawScreen( DX_SCREEN_BACK ) ;
	NS_SetDrawScreen( DX_SCREEN_FRONT ) ;
#endif // DX_NON_GRAPHICS

	if( DxSysData.NotDrawFlag == FALSE )
	{
#ifndef DX_NON_MODEL
		// ���f���o�[�W�����P�̏�����
		if( MV1Initialize() < 0 )
		{
			goto ERROR_DX ;
		}
#endif
	}

	// ���������t���O��|��
	DxSysData.DxLib_RunInitializeFlag = FALSE ;

	// �E�C���h�E�̃T�C�Y���L�����ǂ����̃t���O��|�����N�G�X�g������Ă�����t���O��|��
	if( WinData.WindowSizeValidResetRequest == TRUE )
	{
		WinData.WindowSizeValid = FALSE ;
	}

	DXST_ERRORLOG_TABSUB ;
	DXST_ERRORLOGFMT_ADDUTF16LE(( "\x24\xff\x38\xff\xe9\x30\xa4\x30\xd6\x30\xe9\x30\xea\x30\x6e\x30\x1d\x52\x1f\x67\x16\x53\xe6\x51\x06\x74\x42\x7d\x86\x4e\x00"/*@ L"�c�w���C�u�����̏����������I��" @*/ ) ) ;

	// �I��
	return 0 ;

ERROR_DX:
	NS_DxLib_End() ;

	DXST_ERRORLOG_TABSUB ;
	DXST_ERRORLOGFMT_ADDUTF16LE(( "\x24\xff\x38\xff\xe9\x30\xa4\x30\xd6\x30\xe9\x30\xea\x30\x6e\x30\x1d\x52\x1f\x67\x16\x53\xe6\x51\x06\x74\x31\x59\x57\x65\x00"/*@ L"�c�w���C�u�����̏������������s" @*/ ) ) ;

	// ���������t���O��|��
	DxSysData.DxLib_RunInitializeFlag = FALSE ;

	return -1 ;
} 

// ���C�u�����g�p�̏I���֐�
extern int NS_DxLib_End( void )
{
	// ���ɏI���������s���Ă��邩�A������������������Ă��Ȃ��ꍇ�͉������Ȃ�
	if( DxSysData.DxLib_InitializeFlag == FALSE ) return 0;

	WinData.AltF4_EndFlag = 1 ;

#ifndef DX_NON_SOFTIMAGE
	// �o�^�����S�Ẵ\�t�g�C���[�W���폜
	InitSoftImage() ;
#endif // DX_NON_SOFTIMAGE

	// �e�����n�̏I��
#if !defined( DX_NON_LOG ) && !defined( DX_NON_PRINTF_DX )
	TerminateLog() ;			// ���O�����̌�n��
#endif

#ifndef DX_NON_NETWORK
#if 0
	HTTP_Terminate() ;			// HTTP �֌W�̏����̌�n�����s��
#endif
	TerminateNetWork() ;		// �v�����r�������������֌W�̏I��
#endif

#ifndef DX_NON_SOUND
	NS_StopMusic() ;			// �l�h�c�h�����t����Ă����Ԃ̏ꍇ������~�߂�
#endif // DX_NON_SOUND

#ifndef DX_NON_MODEL
	MV1Terminate() ;			// ���f���o�[�W�����P�̌�n��
#if 0
	NS_InitModel() ;			// ���f���f�[�^�̍폜
#endif
#endif

#ifndef DX_NON_GRAPHICS
	Graphics_Terminate() ;
#endif // DX_NON_GRAPHICS

	TerminateBaseImageManage() ;

#ifndef DX_NON_SOFTIMAGE
	TerminateSoftImageManage() ;
#endif // DX_NON_SOFTIMAGE

#ifndef DX_NON_MOVIE
	TerminateMovieManage() ;
#endif

#ifndef DX_NON_INPUT
	TerminateInputSystem() ;	// ���̓V�X�e���̏I��
#endif // DX_NON_INPUT

#ifndef DX_NON_SOUND
	TerminateSoundSystem() ;	// �T�E���h�V�X�e���̌�n��
	TerminateSoundConvert() ;	// �T�E���h�ϊ������̏I��
#endif // DX_NON_SOUND

	TerminateWindow() ;			// �v�����������֌W�̏I������

	TerminateCom() ;			// �b�n�l�̏I��

#ifndef DX_NON_LOG
	// �G���[���O�r�o�̏I��
	ErrorLogTerminate() ;
#endif

	// �c�w���C�u���������������t���O��|��
	DxSysData.DxLib_InitializeFlag = FALSE ;

	// �p�t�h�s���b�Z�[�W������܂Ń��[�v
	if( DxSysData.NotWinFlag == FALSE )
		while( NS_ProcessMessage() == 0 && WinData.QuitMessageFlag == FALSE && WinData.DestroyMessageCatchFlag == FALSE ){}

	// �E�C���h�E�N���X�̓o�^�𖕏�
	UnregisterClassW( WinData.ClassName, WinData.Instance ) ;

	// �E�C���h�E���҂�
	while( DxSysData.NotWinFlag == FALSE && FindWindowW( WinData.ClassName, NULL ) == WinData.MainWindow )
	{
		DestroyWindow( WinData.MainWindow ) ;
		Sleep( 100 ) ;
	}

	// �J�����g�f�B���N�g�������ɖ߂�
	DX_FSETDIR( WinData.CurrentDirectory ) ; 

	// �A�[�J�C�u�t�@�C���A�N�Z�X�p�̃f�[�^�̌�n��
#ifndef DX_NON_DXA
	DXA_DIR_Terminate() ;
#endif

#ifdef DX_USE_DXLIB_MEM_DUMP
	// �������_���v���s��
	NS_DxDumpAlloc() ;
#endif

	// �������̌�n�����s��
	MemoryTerminate() ;

	// WinAPI ���������
	ReleaseWinAPI() ;

	// �I��
	return 0 ;
}

// ���C�u�����̓����Ŏg�p���Ă���\���̂��[�����������āADxLib_Init �̑O�ɍs�����ݒ�𖳌�������( DxLib_Init �̑O�ł̂ݗL�� )
extern int NS_DxLib_GlobalStructInitialize( void )
{
//	_MEMSET( &GRA2, 0, sizeof( GRA2 ) ) ;
#ifndef DX_NON_GRAPHICS
	_MEMSET( &GraphicsSysData, 0, sizeof( GraphicsSysData ) );
#endif // DX_NON_GRAPHICS

	_MEMSET( &BaseImageManage, 0, sizeof( BaseImageManage ) ) ;
	_MEMSET( &WinData, 0, sizeof( WinData ) );
#ifndef DX_NON_SOUND
	_MEMSET( &SoundSysData, 0, sizeof( SoundSysData ) );
#endif // DX_NON_SOUND

	return 0;
}

// �E�C���h�E�Y�̃��b�Z�[�W���[�v�ɑ��鏈�����s��
extern int NS_ProcessMessage( void )
{
	int	EventCon ;
	int LoopCount ;
	MSG msg = { 0 } ;
	int Flag = FALSE, hr, hr2, StopCheckFlag ;
	static int EndFlag = FALSE ;

	// �����t���O�������Ă�����Ȃɂ������I��
	if( EndFlag )
	{
		return 0 ;
	}

	// ProcessMessage �����s���t���O�𗧂Ă�
	WinData.ProcessMessageFlag = TRUE ;

	// �N���C�A���g�̈悩��o�Ȃ��悤�ɂ���ݒ�ŁA���N���C�A���g�̈���ɋ�����o��Ȃ��悤�ɂ���
	if( WinData.NotMoveMousePointerOutClientAreaFlag && WinData.ActiveFlag && WinData.WindowMinSizeFlag == FALSE && WinData.WindowModeFlag && WinData.SetClipCursorFlag == FALSE )
	{
		POINT MousePos ;

		GetCursorPos( &MousePos ) ;
		if( WinData.WindowRect.left < MousePos.x && MousePos.x <= WinData.WindowRect.right &&
			WinData.WindowRect.top  < MousePos.y && MousePos.y <= WinData.WindowRect.bottom )
		{
			WinData.SetClipCursorFlag = TRUE ;
			ClipCursor( &WinData.WindowRect ) ;
		}
	}

	// DxSysData.NotWinFlag �������Ă���ꍇ�̓E�C���h�E�֘A�̏����͍s��Ȃ�
	if( DxSysData.NotWinFlag == FALSE )
	{
		// �E�C���h�E���쐬����Ă��Ȃ������牽�������I��
		if( WinData.MainWindow == NULL ) goto END ;

		// �c�w���C�u�������쐬�����E�C���h�E���g�p���Ă���ꍇ�̂݃��b�Z�[�W�������s��
		if( WinData.UserWindowFlag == FALSE )
		{
			// �����E�C���h�E���[�h�ύX�t���O�������Ă�����E�C���h�E���[�h��ύX���鏈�����s��
			if( WinData.ChangeWindowModeFlag == TRUE && WinData.UseChangeWindowModeFlag == TRUE && WinData.MainThreadID == GetCurrentThreadId() )
			{
				// �t���O��|��
				WinData.ChangeWindowModeFlag = FALSE ;

				// �E�C���h�E���[�h��ύX����
				NS_ChangeWindowMode( WinData.WindowModeFlag == TRUE ? FALSE : TRUE ) ;

				// �R�[���o�b�N�֐����Ă�
				if( WinData.ChangeWindowModeCallBackFunction != NULL ) 
					WinData.ChangeWindowModeCallBackFunction( WinData.ChangeWindowModeCallBackFunctionData ) ;
			}

			// ���b�Z�[�W�������Ȃ����������ꍇ��ү���ނ̏������I���܂Ń��[�v����i��������j
			EventCon = 0 ;
			LoopCount = 0 ;
			StopCheckFlag = FALSE ;
			while(
				(
				  WinData.CloseMessagePostFlag == FALSE &&
				  WinData.WM_ACTIVATE_StockNum == 0 &&
				  WinData.NonActiveRunFlag     == FALSE &&
				  (
				    WinData.ActiveFlag == FALSE ||
				    WinData.WindowMinSizeFlag == TRUE
				  ) 
				) ||
				(
				  Flag == FALSE &&
				  WinData.QuitMessageFlag == FALSE
				)
			)
			{
				// �\�t�g����A�N�e�B�u�ɂȂ��~�����ꍇ�͎�����ۑ����Ă���
				if( StopCheckFlag == FALSE && ( WinData.ActiveFlag == FALSE || WinData.WindowMinSizeFlag == TRUE ) && WinData.NonActiveRunFlag == FALSE )
				{
					StopCheckFlag = TRUE ;
					WinData.WaitTime = NS_GetNowCount() ;
					WinData.WaitTimeValidFlag = TRUE ;
				}

				Flag = TRUE ;

				if( ( WinData.ActiveFlag == FALSE || WinData.WindowMinSizeFlag == TRUE ) && WinData.NonActiveRunFlag == FALSE )
				{
					Sleep( 1 ) ;

					// ScreenCopy �̃��N�G�X�g�t���O�������Ă������莞�Ԃ��o�߂��Ă����� ScreenCopy ���s��
					if( WinData.ScreenCopyRequestFlag == TRUE && NS_GetNowCount() - WinData.ScreenCopyRequestStartTime > 1000 / 60 * 4 )
					{
#ifndef DX_NON_GRAPHICS
						// ��A�N�e�B�u�̏ꍇ�͔�A�N�e�B�u���ɕ\�������p�̉摜������ꍇ�͗���ʂ̃R�s�[�͍s��Ȃ�
						if( WinData.PauseGraph.GraphData == NULL )
						{
							NS_ScreenCopy() ;
						}
#endif // DX_NON_GRAPHICS
						WinData.ScreenCopyRequestStartTime = NS_GetNowCount() ;
						WinData.ScreenCopyRequestFlag = FALSE ;
					}
				}

				hr  = 1 ;
				hr2 = 0 ;
				while( hr )
				{
					MSG msg2 ;
					if(
//						( hr2 = ( PeekMessageW( &msg2, NULL, 0, 0, PM_NOREMOVE ) != 0 && msg2.message == WM_SYSCOMMAND && msg2.wParam == SC_SCREENSAVE ) ) ||
						( hr2 = ( PeekMessageW( &msg2,       NULL, 0, 0, PM_NOREMOVE ) != 0 /*&& msg2.message == WM_SYSCOMMAND && msg2.wParam == SC_SCREENSAVE*/ ) ) || // ��ATOK�̏ꍇ�X�N���[���Z�[�o�[���b�Z�[�W�Ɍ��肷��Ɣ��p�^�S�p�L�[�œ��{����̓��[�h�Ɉڍs�ł��Ȃ������̂ŁA�������ɁB������肪�������炻�̂Ƃ��l���悤�E�E�E
						( hr  = ( PeekMessageW( &msg,  ( HWND )-1, 0, 0, PM_REMOVE   ) || PeekMessageW( &msg, WinData.MainWindow, 0, 0, PM_REMOVE ) ) )
					  )
					{
						if( hr2 != 0 )
						{
							PeekMessageW( &msg2, NULL, 0, 0, PM_REMOVE ) ;
							msg = msg2 ;
						}

						// �_�C�A���O�{�b�N�X�p�̃��b�Z�[�W�����ׂ�
						if( WinData.DialogBoxHandle == NULL || IsDialogMessageW( WinData.DialogBoxHandle, &msg ) == 0 )
						{
							// �_�C�A���O�{�b�N�X�̃��b�Z�[�W�ł͂Ȃ������畁�ʂɏ���
							// (�E�C���h�E���[�h�̎��̂�)
							if( msg.message == WM_ACTIVATEAPP && WinData.WindowModeFlag == TRUE )
							{
								// �A�N�e�B�u���ǂ�����ۑ�
								WinData.ActiveFlag = LOWORD( msg.wParam ) ;
							}

							if( WinData.QuitMessageFlag ) goto R1 ;
							if( msg.message == WM_QUIT ) 
							{
								WinData.QuitMessageFlag = TRUE ;
								DxLib_EndRequest() ;
							}

							// �A�N�Z�����[�^���L���ȏꍇ�̓A�N�Z�����[�^�L�[����������
							if( TranslateAcceleratorW( WinData.MainWindow, WinData.Accel, &msg ) == 0 )
							{
//								DXST_ERRORLOGFMT_ADDUTF16LE(( L"DispatchMessage\n" )) ;

								// �A�N�Z�����[�^�L�[���b�Z�[�W�ł͂Ȃ������畁�ʂɏ�������
								TranslateMessage( &msg );
								DispatchMessageW( &msg );
							}
							EventCon ++ ;
//							if ( EventCon >= MAX_EVENTPROCESS_NUM )
//							{
//								EventCon = 0 ;
//								Flag = TRUE ;
//
//								goto R2;
//							}

							Flag = FALSE ;
						}
						else
						{
							Flag = FALSE ;
						}
					}

					// �_�C�A���O�{�b�N�X�̃��b�Z�[�W����
					if( WinData.DialogBoxHandle != NULL )
					{
						if( PeekMessageW( &msg, WinData.DialogBoxHandle, 0, 0, PM_REMOVE ) )
						{
							IsDialogMessageW( WinData.DialogBoxHandle, &msg );
						}
					}

					// �q�E�C���h�E�̃��b�Z�[�W����
					{
						int i ;

						for( i = 0 ; i < WinData.MesTakeOverWindowNum ; i ++ )
						{
							if( PeekMessageW( &msg, WinData.MesTakeOverWindow[i], 0, 0, PM_REMOVE ) )
							{
								TranslateMessage( &msg );
								DispatchMessageW( &msg );
							}
						}
					}
				}

//		R2 :
				if( ( WinData.ActiveFlag == FALSE || WinData.WindowMinSizeFlag == TRUE ) && WinData.NonActiveRunFlag == FALSE )
				{
					// �������֌W�̎����I�������s��
					MemoryProcess() ;

#ifndef DX_NON_NETWORK
					// �ʐM�֌W�̃��b�Z�[�W�������s��
					NS_ProcessNetMessage( TRUE ) ;

#if 0
					// HTTP �������s��
					HTTP_ProcessAll() ;
#endif
#endif

#ifndef DX_NON_SOUND

#ifndef DX_NON_MULTITHREAD
					if( WinData.ProcessorNum <= 1 )
#endif // DX_NON_MULTITHREAD
					{
						// �T�E���h�̎����I�������s��
						NS_ProcessStreamSoundMemAll() ;
						ST_SoftSoundPlayerProcessAll() ;
						ProcessPlay3DSoundMemAll() ;
					}
					ProcessPlayFinishDeleteSoundMemAll() ;
#endif // DX_NON_SOUND

#ifndef DX_NON_GRAPHICS
					// �`�揈���̃��b�Z�[�W���[�v���ɍs���ׂ����������s����
					Graphics_Win_MessageLoop_Process() ;
#endif // DX_NON_GRAPHICS

#ifndef DX_NON_ASYNCLOAD
					// ���C���X���b�h����������񓯊��ǂݍ��݂̏������s��
					ProcessASyncLoadRequestMainThread() ;
#endif // DX_NON_ASYNCLOAD

					Sleep( 24 ) ;
				}

				if( WinData.QuitMessageFlag ) goto R1 ;

				// ���C���X���b�h�ł͂Ȃ������璼��������
				if( WinData.MainThreadID != GetCurrentThreadId() ) break ;

				// DxLib_End ���Ă΂�Ă����烋�[�v�P�O��Ŕ�����
				LoopCount ++ ;
				if( WinData.AltF4_EndFlag && LoopCount >= 10 )
					break ;
			}

		R1 :
			// ���j���[���L���ɂȂ��Ă���Ƃ��̏���
			if( WinData.MainThreadID == GetCurrentThreadId() )
			{
				// ���j���[�������I�ɕ\���������\���ɂ����肷��
				// �t���O�������Ă���ꍇ�͂��̏������s��
				if( WinData.NotMenuAutoDisplayFlag == FALSE )
					MenuAutoDisplayProcess() ;

				// �L�[�{�[�h�̓��͏�Ԃ��X�V
	//			UpdateKeyboardInputState() ;
			}

			// ���j���[���L���ɂȂ��Ă���Ƃ��̏���
		/*	{
				// ���Ƀ��j���[�������s���Ă��邩�A���j���[�������������ꍇ�͉������Ȃ�
				if( DxSysData.DxLib_InitializeFlag == TRUE && WinData.MenuUseFlag == TRUE && WinData.MenuShredRunFlag == FALSE )
				{
					int Key ;

					// �G�X�P�[�v�L�[���w��̃L�[�������ꂽ�烁�j���[�������J�n
					Key = ( WinData.MenuStartKey == 0 ) ? KEY_INPUT_ESCAPE : WinData.MenuStartKey ;
					if( CheckHitKey( Key ) == 1 )
					{
						MenuProcess() ;
					}
				}
			}
		*/
			// �������b�N�w�肪�������烍�b�N����
		/*	if( WinData.SysCommandOffFlag == TRUE )
			{
				if( WinData.LockInitializeFlag == TRUE )
				{
					LockSetForegroundWindow( LSFW_LOCK ) ;
					WinData.LockInitializeFlag = FALSE ;
				}
			}
		*/
		}

		// WM_ACTIVATE ���b�Z�[�W�����̃��N�G�X�g������ꍇ�͂����ŏ�������
		if( WinData.CloseMessagePostFlag == FALSE )
		{
			while( WinData.WM_ACTIVATE_StockNum != 0 )
			{
				WPARAM wParam ;
				LPARAM lParam ;
				int APPMes ;

				wParam = WinData.WM_ACTIVATE_wParam[ WinData.WM_ACTIVATE_StartIndex ] ;
				lParam = WinData.WM_ACTIVATE_lParam[ WinData.WM_ACTIVATE_StartIndex ] ;
				APPMes = WinData.WM_ACTIVATE_APPMes[ WinData.WM_ACTIVATE_StartIndex ] ;
				WinData.WM_ACTIVATE_StartIndex = ( WinData.WM_ACTIVATE_StartIndex + 1 ) % 512 ;
				WinData.WM_ACTIVATE_StockNum -- ;
				WM_ACTIVATEProcess( wParam, lParam, APPMes ) ;
			}
		}
	}

	// �t�@�C�������̎����I�������s��
//	ReadOnlyFileAccessProcessAll() ;

#ifndef DX_NON_SOUND

#ifndef DX_NON_MULTITHREAD
	if( WinData.ProcessorNum <= 1 )
#endif // DX_NON_MULTITHREAD
	{
		// �T�E���h�̎����I�������s��
		NS_ProcessStreamSoundMemAll() ;
		ST_SoftSoundPlayerProcessAll() ;
		ProcessPlay3DSoundMemAll() ;
	}
	ProcessPlayFinishDeleteSoundMemAll() ;
#endif // DX_NON_SOUND

#ifndef DX_NON_GRAPHICS
	// �`�揈���̃��b�Z�[�W���[�v���ɍs���ׂ����������s����
	Graphics_Win_MessageLoop_Process() ;
#endif // DX_NON_GRAPHICS

#ifndef DX_NON_ASYNCLOAD
	// ���C���X���b�h����������񓯊��ǂݍ��݂̏������s��
	if( WinData.QuitMessageFlag == FALSE )
	{
		ProcessASyncLoadRequestMainThread() ;
	}
#endif // DX_NON_ASYNCLOAD

	// ���t�̎����I�������s��
#ifndef DX_NON_SOUND
	NS_ProcessMusicMem() ;
#endif // DX_NON_SOUND

#ifndef DX_NON_INPUT
	// �L�[�{�[�h�̎����I�������s��
	//KeyboradBufferProcess() ;

	// �L�[�{�[�h���͂̍X�V�������s��
	UpdateKeyboardInputState( FALSE ) ;

	// �p�b�h�̎����I�������s��
	JoypadEffectProcess() ;
#endif // DX_NON_INPUT

#ifndef DX_NON_NETWORK
	// �ʐM�֌W�̃��b�Z�[�W�������s��
	NS_ProcessNetMessage( TRUE ) ;

#if 0
	// HTTP �������s��
	HTTP_ProcessAll() ;
#endif
#endif

	// �������֌W�̎����I�������s��
	MemoryProcess() ;

#ifndef DX_NON_KEYEX
	// �L�[���͏������s��
	{
		// �t���O�����Ă�
		EndFlag = TRUE ;

		NS_ProcessActKeyInput() ;

		// �t���O��|��
		EndFlag = FALSE ;
	}
#endif
END :

	// ProcessMessage �����s���t���O��|��
	WinData.ProcessMessageFlag = FALSE ;

	return WinData.QuitMessageFlag == TRUE ? -1 : 0 ;
}










// �G���[�����֐�

// �G���[����
extern int DxLib_Error( const wchar_t *ErrorStr )
{
	// �G���[���O�̔r�o
	DXST_ERRORLOG_ADDW( ErrorStr ) ;
	DXST_ERRORLOG_ADDW( L"\n" ) ;

	// �e�����n�̏I��
	NS_DxLib_End() ;

#if defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )
	// �N���e�B�J���Z�N�V�����ƃC�x���g�n���h�����������
	if( WinData.DxConflictWaitThreadIDInitializeFlag == TRUE )
	{
		int i ;

		// �������t���O��|��
		WinData.DxConflictWaitThreadIDInitializeFlag = FALSE ;

		// �Փˎ��Ɏg�p����C�x���g�̉��
		for( i = 0 ; i < MAX_THREADWAIT_NUM ; i ++ )
		{
			if( WinData.DxConflictWaitThreadID[i][1] != 0 )
				CloseHandle( (HANDLE)WinData.DxConflictWaitThreadID[i][1] ) ;
			WinData.DxConflictWaitThreadID[i][1] = 0 ;
		}

		// �N���e�B�J���Z�N�V�������폜����
		CriticalSection_Delete( &WinData.DxConflictCheckCriticalSection ) ;
	}
#endif // defined( DX_THREAD_SAFE ) || defined( DX_THREAD_SAFE_NETWORK_ONLY )

//	while( WinData.QuitMessageFlag == FALSE && NS_ProcessMessage() == 0 ){}

//	DXST_ERRORLOG_ADDW( L"�\�t�g���G���[�I�����܂�" ) ;
	ExitProcess( (DWORD)-1 ) ;

	return -1 ;
}

// ���C�u�����̃G���[�������s��( UTF16LE�� )
extern int DxLib_ErrorUTF16LE( const char *ErrorStr )
{
	int Result ;

	CHAR_TO_WCHAR_T_STRING_BEGIN( ErrorStr )
	CHAR_TO_WCHAR_T_STRING_SETUP( ErrorStr, return -1, DX_CHARCODEFORMAT_UTF16LE )

	Result = DxLib_Error( UseErrorStrBuffer ) ;

	CHAR_TO_WCHAR_T_STRING_END( ErrorStr )

	return Result ;
}

































// �J�E���^�y�ю����擾�n�֐�

// �~���b�P�ʂ̐��x�����J�E���^�̌��ݒl�𓾂�
extern int NS_GetNowCount( int /*UseRDTSCFlag*/ )
{
#if 0
#ifndef DX_NON_INLINE_ASM
	// ���� RDTSC �͎g�p���Ȃ�(��b�ԕӂ�̃J�E���g���𐳊m�Ɏ擾����ɂ͎��Ԃ��|����߂��āA�N������
	// �̑���Ɍq�����)
//	if( WinData.UseRDTSCFlag == TRUE && UseRDTSCFlag == TRUE && DxSysData.DxLib_InitializeFlag == TRUE )
	if( FALSE )
	{
		LARGE_INTEGER Clock ;

#ifdef __BCC
		unsigned int low, high ;
		unsigned int lowP, highP ;
		
		lowP = (unsigned int)&low ;
		highP = (unsigned int)&high ;

		__asm
		{
			db		0x0f
			db		0x31
			MOV		EDI, lowP
			MOV		[EDI], EAX
			MOV		EDI, highP
			MOV		[EDI], EDX
//			MOV		low , EAX
//			MOV		high , EDX
		}
		
		Clock.LowPart = low ;
		Clock.HighPart = high ;
#else
		__asm
		{
//			db		0fh
//			db		031h
		RDTSC
			MOV		Clock.LowPart , EAX
			MOV		Clock.HighPart , EDX
		}
#endif
		return ( int )( _DTOL( ( double )Clock.QuadPart * 1000.0 / WinData.OneSecCount ) & 0x7fffffff ) ;
	}
	else
#endif // DX_NON_INLINE_ASM
#endif // 0
	{
		if( WinAPIData.Win32Func.WinMMDLL == NULL )
		{
			LoadWinAPI() ;
			if( WinAPIData.Win32Func.WinMMDLL == NULL )
			{
				return -1 ;
			}
		}

		return ( int )( WinAPIData.Win32Func.timeGetTimeFunc() & 0x7fffffff ) ;
	}
}

// GetNowTime�̍����x�o�[�W����
extern LONGLONG NS_GetNowHiPerformanceCount( int /*UseRDTSCFlag*/ )
{
	LONGLONG Result, MulNum ;
	unsigned int /*FPUStatus,*/ Temp[4] ;

#if 0
#ifndef DX_NON_INLINE_ASM
//	FPUStatus = 0x00020000 ;
	// ���� RDTSC �͎g�p���Ȃ�(��b�ԕӂ�̃J�E���g���𐳊m�Ɏ擾����ɂ͎��Ԃ��|����߂��āA�N������
	// �̑���Ɍq�����)
//	if( WinData.UseRDTSCFlag == TRUE && UseRDTSCFlag == TRUE && DxSysData.DxLib_InitializeFlag == TRUE )
	if( FALSE )
	{
		LARGE_INTEGER Clock ;

#ifdef __BCC
		unsigned int low, high ;
		unsigned int lowP, highP ;
		
		lowP = (unsigned int)&low ;
		highP = (unsigned int)&high ;

		__asm
		{
			db		0x0f
			db		0x31
			MOV		EDI, lowP
			MOV		[EDI], EAX
			MOV		EDI, highP
			MOV		[EDI], EDX
//			MOV		low , EAX
//			MOV		high , EDX
		}

		Clock.LowPart = low ;
		Clock.HighPart = high ;
#else
		__asm
		{
		RDTSC
			MOV		Clock.LowPart, EAX
			MOV		Clock.HighPart, EDX
		}
#endif
		// ���x�ݒ���グ��
//		FPUStatus = _control87( 0x00000000, 0x00030000 ) ;
		MulNum = 1000000;
		_MUL128_1( (DWORD *)&MulNum, (DWORD *)&Clock.QuadPart, (DWORD *)Temp );
		_DIV128_1( (DWORD *)Temp, (DWORD *)&WinData.OneSecCount, (DWORD *)&Result );
//		Result = _DTOL64( ( double )Clock.QuadPart * 1000000.0 / WinData.OneSecCount ) ;
	}
	else
#endif  // DX_NON_INLINE_ASM
#endif // 0
	{
		if( WinData.PerformanceTimerFlag )
		{
			LONGLONG NowTime ;

			// �p�t�H�[�}���X�J�E���^����J�E���g�𓾂�
			QueryPerformanceCounter( ( LARGE_INTEGER * )&NowTime ) ;

//			// ���x�ݒ���グ��
//			FPUStatus = _control87( 0x00000000, 0x00030000 ) ;
			MulNum = 1000000;
			_MUL128_1( (DWORD *)&MulNum, (DWORD *)&NowTime, (DWORD *)Temp );
			_DIV128_1( (DWORD *)Temp, (DWORD *)&WinData.PerformanceClock, (DWORD *)&Result );
//			Result = ( LONGLONG )_DTOL64( ( double )NowTime / ( double )WinData.PerformanceClock * 1000000.0 ) ;
		}
		else
		{
			if( WinAPIData.Win32Func.WinMMDLL == NULL )
				return -1 ;

			// �p�t�H�[�}���X�J�E���^���Ȃ��ꍇ�͒ʏ�̃J�E���^�ɂP�O�O�O���|�����l��Ԃ�
//			Result = ( LONGLONG )_DTOL64( (double)WinAPIData.Win32Func.timeGetTimeFunc() * 1000 ) ;
			MulNum = 1000;
			((DWORD *)&Result)[0] = (DWORD)WinAPIData.Win32Func.timeGetTimeFunc();
			((DWORD *)&Result)[1] = 0;
			_MUL128_1( (DWORD *)&MulNum, (DWORD *)&Result, (DWORD *)Temp );
			((DWORD *)&Result)[0] = Temp[0];
			((DWORD *)&Result)[1] = Temp[1];
		}
	}

	// ���x�ݒ�����ɖ߂�
//	_control87( FPUStatus, 0x00030000 ) ;

	return Result ;
}

// ���ݎ������擾����
extern int NS_GetDateTime( DATEDATA *DateBuf )
{
	SYSTEMTIME LocalTime ;

	// ���[�J�������𓾂�	
	GetLocalTime( &LocalTime ) ;
	
	// ���[�J�������f�[�^�����ɐ�p�̃f�[�^�^�f�[�^�Ɏ�����ɉh������
	DateBuf->Year	= LocalTime.wYear ;
	DateBuf->Mon	= LocalTime.wMonth ;
	DateBuf->Day	= LocalTime.wDay ;
	DateBuf->Hour	= LocalTime.wHour ;
	DateBuf->Min	= LocalTime.wMinute ;
	DateBuf->Sec	= LocalTime.wSecond ;

	// �I��
	return 0 ;
}







// �����擾

#ifndef DX_NON_MERSENNE_TWISTER

// �����̏����l��ݒ肷��
extern int NS_SRand( int Seed )
{
	// �����l�Z�b�g
	srandMT( ( unsigned long )Seed ) ;

	// �I��
	return 0 ;
}

// �������擾����( RandMax : �Ԃ��ė���l�̍ő�l )
extern int NS_GetRand( int RandMax )
{
	int Result ;
	LONGLONG RandMaxLL ;

	RandMaxLL = RandMax ;
	RandMaxLL ++ ;
	Result = ( int )( ( ( LONGLONG )randMT() * RandMaxLL ) >> 32 ) ;

	return Result ;
}

#else // DX_NON_MERSENNE_TWISTER

// �����̏����l��ݒ肷��
extern int NS_SRand( int Seed )
{
	// �����l�Z�b�g
	srand( Seed ) ;

	// �I��
	return 0 ;
}

// �������擾����( RandMax : �Ԃ��ė���l�̍ő�l )
extern int NS_GetRand( int RandMax )
{
	int Result ;
	LONGLONG RandMaxLL ;

	RandMaxLL = RandMax ;
	RandMaxLL ++ ;
	Result = ( int )( ( ( LONGLONG )rand() * RandMaxLL ) / ( ( LONGLONG )RAND_MAX + 1 ) ) ;

	return Result ;
}

#endif // DX_NON_MERSENNE_TWISTER





#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE





