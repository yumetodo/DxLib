// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�ʐM�֘A�v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "DxNetwork.h"

#ifndef DX_NON_NETWORK

// �C���N���[�h ------------------------------------------------------------------
#include "DxStatic.h"
#include "DxBaseFunc.h"
#include "DxSystem.h"
#include "DxMemory.h"
#include "DxMath.h"
#include "DxUseCLib.h"
#include "DxASyncLoad.h"
#include "DxLog.h"
#include "Windows/DxWinAPI.h"
#include "Windows/DxWindow.h"

#ifndef DX_GCC_COMPILE
#include <winnls32.h>
#endif

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// �l�b�g���[�N�n���h���̗L�����`�F�b�N
#define NETHCHK( HAND, NPOINT )			HANDLECHK(       DX_HANDLETYPE_NETWORK, HAND, *( ( HANDLEINFO ** )&NPOINT ) )
#define NETHCHK_ASYNC( HAND, NPOINT )	HANDLECHK_ASYNC( DX_HANDLETYPE_NETWORK, HAND, *( ( HANDLEINFO ** )&NPOINT ) )

// �l�b�g���[�N�n���h���̗L�����`�F�b�N( �s�b�o )
#define TCPNETHCHK( HAND, NPOINT )			( ( NETHCHK( HAND, NPOINT ) ) || (NPOINT)->IsUDP != 0 )
#define TCPNETHCHK_ASYNC( HAND, NPOINT )	( ( NETHCHK_ASYNC( HAND, NPOINT ) ) || (NPOINT)->IsUDP != 0 )

// �l�b�g���[�N�n���h���̗L�����`�F�b�N( �t�c�o )
#define UDPNETHCHK( HAND, NPOINT )			( ( NETHCHK( HAND, NPOINT ) ) || (NPOINT)->IsUDP == 0 )
#define UDPNETHCHK_ASYNC( HAND, NPOINT )	( ( NETHCHK_ASYNC( HAND, NPOINT ) ) || (NPOINT)->IsUDP == 0 )


// WinSock�p��`
#define WSA_DEFAULTPORT				(10001)
#define WSA_TIMEOUTWAIT				(2000)

#define	WSA_ACCEPT					(WM_USER + 256)
#define	WSA_CONNECT					(WM_USER + 257)
#define	WSA_READ					(WM_USER + 258)
#define	WSA_WRITE					(WM_USER + 259)
#define	WSA_CLOSE					(WM_USER + 260)

#define WINFILEACCESS_BUFFERSIZE	(64 * 1024)			// �t�@�C���̈ꎞ�L���o�b�t�@�̃T�C�Y

// �h�d�̃v���L�V�T�[�o�A�h���X���i�[����Ă��郌�W�X�g���A�h���X
#define IEPROXY						"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"

// �\���̒�` --------------------------------------------------------------------

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

WINSOCKDATA SockData ;									// �v�����r�������������f�[�^
HTTPSYS HttpData ;										// HTTP �֌W�����p�f�[�^

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �ʐM�֌W
static	int			ConnectNetWorkBase( IPDATA *IPData_IPv4, IPDATA_IPv6 *IPData_IPv6, int Port, int ASync ) ;	// ConnectNetWork �̏������s���֐�
static	int			PreparationListenNetWork_Base( int IsIPv6, int Port ) ;					// �ڑ����󂯂����Ԃɂ���
static	int			MakeUDPSocketBase( int IsIPv6, int RecvPort ) ;							// UDP���g�p�����ʐM���s���\�P�b�g�n���h�����쐬����( RecvPort �� -1 �ɂ���Ƒ��M��p�̃\�P�b�g�n���h���ɂȂ�܂� )

static	int			RecvSocket( int NetHandle ) ;											// �f�[�^�̎�M����
static	int			SendSocket( int NetHandle ) ;											// ���܂����f�[�^�̑��M����

static	int			ErrorNetWork( const char *ErrorStr, ... ) ;								// �ʐM�G���[�����֐�
static	int			ErrorNetLogAdd( const char *String ) ;									// �ʐM���b�Z�[�W�o�͊֐�
static	int			ErrorNetLogTabAdd( void ) ;												// �ʐM���b�Z�[�W�Ƀ^�u��ǉ�����֐�
static	int			ErrorNetLogTabSub( void ) ;												// �ʐM���b�Z�[�W�̃^�u�����炷�֐�

extern	LRESULT		CALLBACK WinSockWindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) ;	// �v�����r��������p�̃E�C���h�E�̃��b�Z�[�W�R�[���o�b�N�֐�

DWORD	WINAPI		ProcessNetMessageThreadFunction( LPVOID ) ;									// ProcessNetMessage ���Ђ�����Ăё�����X���b�h

#if 0

static	int			HTTP_Initialize( void ) ;													// HTTP �֌W�̏����̏��������s��
static	int			HTTP_Terminate( void ) ;													// HTTP �֌W�̏����̌�n�����s��

static	int			HTTP_GetResult( const char *ResStr ) ;										// HTTP ���b�Z�[�W�̌��ʂ𓾂�

static	int			HTTP_GetConnectInfo( const char *URL, int UseProxy, char *HostBuf,
											char *PathBuf, char *FileNameBuf, int *PortBuf ) ;	// �w��̂t�q�k����z�X�g�p�X�A�z�X�g�̃A�N�Z�X�p�X�A�A�N�Z�X�|�[�g���擾����
static	int			HTTP_ConnectHost( const char *URL, int UseProxy, IPDATA *HostIPBuf = NULL,
									 char *HostBuf = NULL, char *PathBuf = NULL, char *FileNameBuf = NULL, int *PortBuf = NULL ) ;	// �w��� HTTP �z�X�g�ɐڑ�����( �߂�l: 0�ȏ� = �l�b�g���[�N�n���h��  -1 = �G���[ )
static	int			HTTP_AddHandle( void ) ;													// �V����HTTP�n���h����ǉ�����( �߂�l: -1 = �G���[  0�ȏ� = HTTP�n���h�� )
static	int			HTTP_DelHandle( int HttpHandle ) ;											// HTTP�n���h���̌�n�����s��
static	int			HTTP_ReleaseResource( int HttpHandle, int Result = HTTP_RES_COMPLETE, int Error = HTTP_ERR_NONE ) ;		// HTTP �̏����Ɏg�p���Ă��郊�\�[�X���������
static	int			HTTP_Process( int HttpHandle ) ;											// HTTP �̏������s��
static	int			HTTP_ProcessAll( void ) ;													// HTTP_Proess ��S�Ẵn���h���ɑ΂��čs��
static	int			HTTP_ProcessForGET( int HttpHandle ) ;										// HTTP �� GET ���\�b�h�̏���������
static	int			HTTP_ProcessForPOST( int HttpHandle ) ;										// HTTP �� POST ���\�b�h�̏���������
static	int			HTTP_ProcessForHEAD( int HttpHandle ) ;										// HTTP �� HEAD ���\�b�h�̏���������

static	char		GetDataSizeString( int ByteSize, char *SizeString ) ;						// �T�C�Y�Ɍ��������P�ʂŕ\����������Ƃ��̒P�ʂ�\���ꕶ����Ԃ�
static	int			GetTimeLengthString( int MillSec, char *LengthString ) ;					// �~���b�̎��Ԓ����玞�Ԓ��ɍ������P�ʂ̕�����ɕϊ�����

#endif

extern	int			HTTP_FileDownload( const char *FileURL, const char *SavePath = NULL,
										 void **SaveBufferP = NULL, int *FileSize = NULL,
										 char **ParamList = NULL ) ;						// HTTP ���g�p���ăl�b�g���[�N��̃t�@�C�����_�E�����[�h����
extern	int			HTTP_GetFileSize( const char *FileURL ) ;								// HTTP ���g�p���ăl�b�g���[�N��̃t�@�C���̃T�C�Y�𓾂�

extern	int			HTTP_StartFileDownload( const char *FileURL, const char *SavePath, void **SaveBufferP = NULL, char **ParamList = NULL ) ;	// HTTP ���g�p�����l�b�g���[�N��̃t�@�C�����_�E�����[�h���鏈�����J�n����
extern	int			HTTP_StartGetFileSize( const char *FileURL ) ;							// HTTP ���g�p�����l�b�g���[�N��̃t�@�C���̃T�C�Y�𓾂鏈�����J�n����
extern	int			HTTP_Close( int HttpHandle ) ;											// HTTP �̏������I�����A�n���h�����������
extern	int			HTTP_CloseAll( void ) ;													// �S�Ẵn���h���ɑ΂��� HTTP_Close ���s��
extern	int			HTTP_GetState( int HttpHandle ) ;										// HTTP �����̌��݂̏�Ԃ𓾂�( NET_RES_COMPLETE �� )
extern	int			HTTP_GetError( int HttpHandle ) ;										// HTTP �����ŃG���[�����������ꍇ�A�G���[�̓��e�𓾂�( HTTP_ERR_NONE �� )
extern	int			HTTP_GetDownloadFileSize( int HttpHandle ) ;							// HTTP �����őΏۂƂȂ��Ă���t�@�C���̃T�C�Y�𓾂�( �߂�l: -1 = �G���[�E�Ⴕ���͂܂��t�@�C���̃T�C�Y���擾���Ă��Ȃ�  0�ȏ� = �t�@�C���̃T�C�Y )
extern	int			HTTP_GetDownloadedFileSize( int HttpHandle ) ;							// HTTP �����Ŋ��Ƀ_�E�����[�h�����t�@�C���̃T�C�Y���擾����

extern	int			fgetsForNetHandle( int NetHandle, char *strbuffer ) ;					// fgets �̃l�b�g���[�N�n���h����( -1:�擾�ł��� 0:�擾�ł��� )
extern	int			URLAnalys( const char *URL, char *HostBuf = NULL, char *PathBuf = NULL,
												 char *FileNameBuf = NULL, int *PortBuf = NULL ) ;	// �t�q�k����͂���
extern	int			URLConvert( char *URL, int ParamConvert = TRUE, int NonConvert = FALSE ) ;	// HTTP �ɓn���Ȃ��L�����g��ꂽ�������n����悤�ȕ�����ɕϊ�����( �߂�l: -1 = �G���[  0�ȏ� = �ϊ���̕�����̃T�C�Y )
extern	int			URLParamAnalysis( char **ParamList, char **ParamStringP ) ;				// HTTP �p�p�����[�^���X�g�����̃p�����[�^��������쐬����( �߂�l:  -1 = �G���[  0�ȏ� = �p�����[�^�̕�����̒��� )

// �v���O���� --------------------------------------------------------------------

// �ʐM�֌W

// �ʐM�G���[�����֐�
static	int ErrorNetWork( const char *ErrorStr, ... )
{
	va_list VaList ;
	char String[ 1024 ] ;
	int ErrorNum ;

	// ���O�o�͗p�̃��X�g���Z�b�g����
	va_start( VaList, ErrorStr ) ;

	// �ҏW��̕�������擾����
	CL_vsnprintf( DX_CHARCODEFORMAT_UTF16LE, TRUE, CHAR_CHARCODEFORMAT, WCHAR_T_CHARCODEFORMAT, String, sizeof( String ) / 2, ErrorStr, VaList ) ;
	
	// �ϒ����X�g�̃|�C���^�����Z�b�g����
	va_end( VaList ) ;

//	if( SockData.MessageWindow != NULL ) return -1 ;

	ErrorNum = WinAPIData.WinSockFunc.WSAGetLastErrorFunc() ;
	DXST_ERRORLOGFMT_ADDUTF16LE(( "\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xed\x30\xb0\x30\x02\x30\x25\x00\x73\x00\x5b\x00\x25\x00\x64\x00\x5d\x00\x00"/*@ L"�l�b�g���[�N���O�B%s[%d]" @*/, String, ErrorNum )) ;

	// �I��
	return -1 ;
}


// �ʐM���b�Z�[�W�o�͊֐�
static int ErrorNetLogAdd( const char *String )
{
//	if( SockData.MessageWindow != NULL ) return 0 ;

	return ErrorNetWork( String ) ;
}

// �ʐM���b�Z�[�W�Ƀ^�u��ǉ�����֐�
static int ErrorNetLogTabAdd( void )
{
//	if( SockData.MessageWindow != NULL ) return 0 ;

	return DXST_ERRORLOG_TABADD ;
}

// �ʐM���b�Z�[�W�̃^�u�����炷�֐�
static int ErrorNetLogTabSub( void )
{
//	if( SockData.MessageWindow != NULL ) return 0 ;

	return DXST_ERRORLOG_TABSUB ;
}

// �ʐM���b�Z�[�W�̏���������֐�
extern int NS_ProcessNetMessage( int RunReleaseProcess )
{
	// �ʐM�֌W������������Ă��Ȃ��ꍇ�͉��������I��
	if( SockData.MessageWindow == NULL || SockData.InitializeFlag == FALSE ) return 0 ;

	// �N���e�B�J���Z�N�V�����̎擾
	if( HandleManageArray[ DX_HANDLETYPE_NETWORK ].InitializeFlag != FALSE )
		CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �n���h���������������w�肪����ꍇ�̓n���h������������s��
	if( RunReleaseProcess == TRUE )
	{
		SOCKETDATA * Sock ;
		HANDLELIST *List, *temp ;

		// �ؒf����ɉ������t���O�������Ă���
		// ���肩��ڑ����A���肩��ؒf���ꂽ�ꍇ�A�����I�Ƀn���h���̉�����s��
		// �Ⴕ���͏d��ȃG���[���������Ă���ꍇ�������I�Ƀn���h�����������
		for( List = HandleManageArray[ DX_HANDLETYPE_NETWORK ].ListFirst.Next ; List->Next != NULL ; List = temp )
		{
			temp = List->Next ;
			Sock = (SOCKETDATA *)List->Data ;
			if( Sock->UseFlag == TRUE &&					// �n���h�����L����
				( ( 
					Sock->ConnectionFlag == FALSE &&			// �ڑ����f����Ă���
					Sock->ConnectionLostFlag == TRUE &&			// �ڑ����f���ꂽ���Ƃ��m�F�ς݂�
//					Sock->AcceptFlag == TRUE &&					// ���肩��ڑ�����Ă���
					Sock->CloseAfterLostFlag == TRUE ) ||		// �����I�ɉ������t���O�������Ă���ꍇ
				  Sock->ErrorFlag == TRUE ) )					// �������͏d��ȃG���[���������Ă���ꍇ
			{
				// �n���h�������
				NS_CloseNetWork( List->Handle ) ;
			}
		}
	}

	// �ʐM�p�E�C���h�E���b�Z�[�W����������
	{
		MSG msg;

		// ���b�Z�[�W�������Ȃ����������ꍇ��ү���ނ̏������I���܂Ń��[�v����i��������j
		while( PeekMessageW( &msg, SockData.MessageWindow, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessageW( &msg );
		}
	}

	// �N���e�B�J���Z�N�V�����̉��
	if( HandleManageArray[ DX_HANDLETYPE_NETWORK ].InitializeFlag != FALSE )
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return 0 ;
}


// �ʐM�@�\������������
extern 	int InitializeNetWork( HWND WindowHandle )
{
	WSADATA wsaData;

	if( SockData.InitializeFlag ) return 0 ;

	DXERRORNETLOG_ADD( "\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xa2\x95\x23\x90\x92\x30\x1d\x52\x1f\x67\x16\x53\x57\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"�l�b�g���[�N�֘A�����������܂�\n" @*/ ) ;
	ErrorNetLogTabAdd() ;

	// �n���h���Ǘ����̏�����
	InitializeHandleManage( DX_HANDLETYPE_NETWORK, sizeof( SOCKETDATA ), MAX_SOCKET_NUM, InitializeNetworkHandle, TerminateNetworkHandle, L"Network" ) ;

	// WinSockets������
	if( WinAPIData.WinSockFunc.WSAStartupFunc( MAKEWORD( 2 , 2 ), &wsaData ) != 0 ) 
		return DXERRORNETLOG_ADD( "\x77\x00\x73\x00\x32\x00\x5f\x00\x33\x00\x32\x00\x2e\x00\x64\x00\x6c\x00\x6c\x00\x4c\x30\x0e\x5c\x65\x51\x55\x30\x8c\x30\x66\x30\x44\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"ws2_32.dll����������Ă��܂���\n" @*/ ) ;

	// �w��o�[�W�����ŏ���������Ȃ������ꍇ�͏I��
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 )
	{
		WinAPIData.WinSockFunc.WSACleanupFunc();

		DXERRORNETLOG_ADD( "\x77\x00\x73\x00\x32\x00\x5f\x00\x33\x00\x32\x00\x2e\x00\x64\x00\x6c\x00\x6c\x00\x6e\x30\xd0\x30\xfc\x30\xb8\x30\xe7\x30\xf3\x30\x4c\x30\x55\x90\x44\x30\x7e\x30\x59\x30\x0a\x00\x00"/*@ L"ws2_32.dll�̃o�[�W�������Ⴂ�܂�\n" @*/ ) ;
		return -1;
	}

	// �h�o�A�h���X���擾����
	{
		char IPStr[ 1024 ] ;
		HOSTENT *Host ;

//		_MEMSET( &SockData.MyIP, 0, sizeof( IPDATA ) ) ;
		if( WinAPIData.WinSockFunc.gethostnameFunc( IPStr, 256 ) == 0 )
		{
			Host = WinAPIData.WinSockFunc.gethostbynameFunc( IPStr ) ;
			if( Host != NULL )
			{
				// �h�o�A�h���X�̐��𐔂���
				for( SockData.MyIPNum = 0 ; Host->h_addr_list[ SockData.MyIPNum ] != NULL ; SockData.MyIPNum ++ ){}

				// �O�������ꍇ�� NULL ���Z�b�g���ďI��
				if( SockData.MyIPNum == 0 )
				{
					SockData.MyIP = NULL ;
				}
				else
				{
					int i ;

					// �h�o�A�h���X��ۑ����郁�������m�ۂ���
					SockData.MyIP = ( IPDATA * )DXALLOC( sizeof( IPDATA ) * SockData.MyIPNum ) ;
					if( SockData.MyIP == NULL )
					{
						WinAPIData.WinSockFunc.WSACleanupFunc() ;

						DXERRORNETLOG_ADD( "\x49\x00\x50\x00\xa2\x30\xc9\x30\xec\x30\xb9\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"IP�A�h���X��ۑ����郁�����̊m�ۂɎ��s���܂���\n" @*/ ) ;
						return -1 ;
					}

					// �h�o�A�h���X��ۑ�
					for( i = 0 ; i < SockData.MyIPNum ; i ++ )
					{
						SockData.MyIP[ i ].d1 = ( unsigned char )Host->h_addr_list[ i ][ 0 ] ;
						SockData.MyIP[ i ].d2 = ( unsigned char )Host->h_addr_list[ i ][ 1 ] ;
						SockData.MyIP[ i ].d3 = ( unsigned char )Host->h_addr_list[ i ][ 2 ] ;
						SockData.MyIP[ i ].d4 = ( unsigned char )Host->h_addr_list[ i ][ 3 ] ;
					}
				}
			}
		}
	}

	// �\�P�b�g�f�[�^�[��������
//	_MEMSET( SockData.CSocket, 0, sizeof( SockData.CSocket ) ) ;

	// �ڑ��ő吔���Z�b�g����A�������t���O�𗧂Ă�
//	SockData.MaxSockets = MAX_SOCKET_NUM ;
	SockData.InitializeFlag = TRUE ;

	// �E�C���h�E�n���h����ۑ�����
	SockData.ParentWindow = WindowHandle ;

	// ���b�Z�[�W�����p�q�E�C���h�E���쐬����
	{
		WNDCLASSEXW wc ;
		HWND ParentWindow ;
		HINSTANCE hInst = GetModuleHandle( NULL ) ;
		wchar_t *Name = L"WinSockProc" ;

		// �q�E�C���h�E�̃E�C���h�E�N���X��o�^
		_MEMSET( &wc, 0, sizeof( wc ) ) ;
		{
			wc.style			= 0 ;
			wc.lpfnWndProc		= WinSockWindowProc ;
			wc.cbClsExtra		= 0 ;
			wc.cbWndExtra		= 0 ;
			wc.hInstance		= hInst ;
			wc.hIcon			= NULL ;
			wc.hCursor			= LoadCursorW( NULL , ( LPCWSTR )IDC_ARROW ) ;
			wc.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);
			wc.lpszMenuName		= NULL ;
			wc.lpszClassName	= Name ;
			wc.cbSize			= sizeof( WNDCLASSEX );
			wc.hIconSm			= NULL ;

			if( !RegisterClassExW( &wc ) )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\x50\x5b\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\xaf\x30\xe9\x30\xb9\x30\x6e\x30\x7b\x76\x32\x93\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�l�b�g���[�N�����p�̎q�E�C���h�E�N���X�̓o�^�Ɏ��s���܂���\n" @*/ ) ;
				return -1 ;
			}
		}

		// �e�E�C���h�E�̊m��
		ParentWindow = SockData.ParentWindow != NULL ? SockData.ParentWindow : WinData.MainWindow ;

		// �q�E�C���h�E�𐶐�
		SockData.MessageWindow = 
			WinAPIData.Win32Func.CreateWindowExWFunc(
				WS_EX_TRANSPARENT,
				Name,
				Name ,
				( DWORD )( ( ParentWindow ? WS_CHILD : 0 ) | WS_MINIMIZE ),
				0, 0, 0, 0,
				ParentWindow,
				NULL,
				hInst,
				NULL );
		if( SockData.MessageWindow == NULL )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xe6\x51\x06\x74\x28\x75\x6e\x30\x50\x5b\xa6\x30\xa4\x30\xf3\x30\xc9\x30\xa6\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�l�b�g���[�N�����p�̎q�E�C���h�E�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}
		SockData.DestroyFlag = FALSE ;
	}

	// �n���h�����X�g�̏�����
//	InitializeHandleList( &SockData.SocketListFirst, &SockData.SocketListLast ) ;

	// ProcessNetMessage ���Ђ�����ĂԃX���b�h�𗧂Ă�
	SockData.ProcessNetMessageThreadHandle = CreateThread( NULL, 0, ProcessNetMessageThreadFunction, NULL, CREATE_SUSPENDED, &SockData.ProcessNetMessageThreadID ) ;
	ResumeThread( SockData.ProcessNetMessageThreadHandle ) ;

	ErrorNetLogTabSub() ;
	DXERRORNETLOG_ADD( "\x1d\x52\x1f\x67\x16\x53\x92\x30\x8c\x5b\x86\x4e\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x0a\x00\x00"/*@ L"���������������܂��� \n" @*/ ) ;

	// �I��
	return 0 ;
}

// �ʐM�@�\���I������
extern int TerminateNetWork( void )
{
//	int i, Handle ;

	if( !SockData.InitializeFlag ) return 0 ;

	DXERRORNETLOG_ADD( "\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xa2\x95\x23\x90\x6e\x30\x42\x7d\x86\x4e\xe6\x51\x06\x74\x92\x30\x4c\x88\x44\x30\x7e\x30\x59\x30\x20\x00\x0a\x00\x00"/*@ L"�l�b�g���[�N�֘A�̏I���������s���܂� \n" @*/ ) ;

	// ProcessNetMessage ���Ђ�����ĂтÂ���X���b�h�����
	if( SockData.ProcessNetMessageThreadHandle != NULL )
	{
		// �X���b�h���I������܂ő҂�
		SockData.ProcessNetMessageThreadEndRequest = TRUE ;
		while( SockData.ProcessNetMessageThreadExitFlag == 0 )
		{
			Thread_Sleep( 1 ) ;
		}

		// �X���b�h�̃n���h�������
		CloseHandle( SockData.ProcessNetMessageThreadHandle ) ;
		SockData.ProcessNetMessageThreadHandle = NULL ;
	}

	// �n���h���Ǘ����̌�n��
	TerminateHandleManage( DX_HANDLETYPE_NETWORK ) ;
/*
	// �I������
	for( i = 0 ; i < MAX_SOCKET_NUM + 1 ; i ++ )
	{
		if( SockData.CSocket[i] != NULL )
		{
			Handle = i | DX_HANDLETYPE_MASK_NETWORK | ( SockData.CSocket[i]->ID << DX_HANDLECHECK_ADDRESS ) ;
			if( SockData.CSocket[i]->IsUDP )
			{
				NS_DeleteUDPSocket( Handle ) ;
			}
			else
			{
				NS_CloseNetWork( Handle ) ;
			}
		}
	}
*/
	// �h�o�A�h���X��ۑ����邽�߂Ƀ��������m�ۂ��Ă�����������
	if( SockData.MyIP != NULL )
	{
		DXFREE( SockData.MyIP ) ;
		SockData.MyIP = NULL ;
	}

	// �E�C���h�E���폜����
	if( SockData.MessageWindow != NULL )
	{
		PostMessageW( SockData.MessageWindow, WM_CLOSE, 0, 0 );
		while( SockData.DestroyFlag == FALSE )
		{
			if( NS_ProcessNetMessage() <= 0 ) break ;
		}
	}

	// �[���N���A
	_MEMSET( &SockData, 0, sizeof( SockData ) ) ;

	// WinSockets�̎g�p���I������
	WinAPIData.WinSockFunc.WSACleanupFunc() ;

	DXERRORNETLOG_ADD( "\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xa2\x95\x23\x90\x6e\x30\x42\x7d\x86\x4e\xe6\x51\x06\x74\x6f\x30\x8c\x5b\x86\x4e\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�l�b�g���[�N�֘A�̏I�������͊������܂���\n" @*/ ) ;

	// �I��
	return 0 ;
}

// �v�����r���������b�Z�[�W�̃R�[���o�b�N�֐�
extern int WinSockProc( HWND /*hWnd*/, UINT /*message*/, WPARAM wParam, LPARAM lParam )
{
	WORD _event , error ;
	int NetHandle ;
	SOCKET Sock ;
	SOCKETDATA * SockD ;
	int ReturnValue = TRUE ;
	HANDLELIST *List ;

	// �N���e�B�J���Z�N�V�����̎擾
	if( HandleManageArray[ DX_HANDLETYPE_NETWORK ].InitializeFlag != FALSE )
		CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �ǂ̃n���h���ւ̃��b�Z�[�W�Ȃ̂����擾
	Sock = ( SOCKET )wParam ;
	for( List = HandleManageArray[ DX_HANDLETYPE_NETWORK ].ListFirst.Next ;
		List->Next != NULL &&
		((SOCKETDATA *)List->Data)->Socket != ( unsigned int )Sock ;
		List = List->Next ){}
	SockD = (SOCKETDATA *)List->Data ;
	if( List->Next == NULL )
	{
		// �N���e�B�J���Z�N�V�����̉��
		if( HandleManageArray[ DX_HANDLETYPE_NETWORK ].InitializeFlag != FALSE )
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

		DXST_ERRORLOG_ADDUTF16LE( "\x1a\x90\xe1\x4f\xa8\x30\xe9\x30\xfc\x30\x20\x00\x4e\x00\x6f\x00\x2e\x00\x31\x00\x0a\x00\x00"/*@ L"�ʐM�G���[ No.1\n" @*/ ) ;
		return FALSE ;
	}
	NetHandle = SockD->HandleInfo.Handle ;

	// ���b�Z�[�W�̎擾
	_event = WSAGETSELECTEVENT( (LPARAM) lParam ) ;
	error = WSAGETSELECTERROR( (LPARAM) lParam ) ;

	// UDP���ǂ����ŏ����𕪊�
	if( SockD->IsUDP )
	{
		switch( _event )
		{
		// �f�[�^��M
		case FD_READ:
			// ��M�f�[�^���݃t���O�𗧂Ă�
			SockD->UDPReadFlag = TRUE ;
			break ;

		// �f�[�^���M��������
		case FD_WRITE:
			// ���M�\�t���O�𗧂Ă�
			SockD->UDPWriteFlag = TRUE ;
			break;
		}
	}
	else
	{
		switch( _event )
		{
		// �R�l�N�g���b�Z�[�W
		case FD_CONNECT :
			// �G���[������������R�l�N�g���s
			if( error != 0 ) break ;
			
			// �ڑ��t���O�𗧂Ă�
			SockD->ConnectionFlag = TRUE ;

			// �ڑ��O�t���O��|��
			SockD->PreConnectionFlag = FALSE ;
			break ;

		// �A�N�Z�v�g���b�Z�[�W
		case FD_ACCEPT :
			// �A�N�Z�v�g����
			ReturnValue = AcceptNetWork() ;
			break ;

		// �f�[�^��M
		case FD_READ:
			// �����f�[�^��M�������s��
			RecvSocket( NetHandle ) ;
			break ;


		// �f�[�^���M��������
		case FD_WRITE:
			// �������M�����֐������s����
			SendSocket( NetHandle ) ;
			break;

		// �N���[�Y���b�Z�[�W
		case FD_CLOSE:
			// �ڑ��t���O�Ɛؒf�m�F�t���O��|��
			SockD->ConnectionFlag = FALSE ;
			SockD->ConnectionLostFlag = FALSE ;
			break ;
		}
	}

	// �N���e�B�J���Z�N�V�����̉��
	if( HandleManageArray[ DX_HANDLETYPE_NETWORK ].InitializeFlag != FALSE )
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return ReturnValue ;
}

// �v�����r��������p�̃E�C���h�E�̃��b�Z�[�W�R�[���o�b�N�֐�
extern LRESULT CALLBACK WinSockWindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int Ret ;

	// �N���e�B�J���Z�N�V�����̎擾
	if( HandleManageArray[ DX_HANDLETYPE_NETWORK ].InitializeFlag != FALSE )
		CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	switch( message )
	{
	case WM_CREATE :
		hWnd = hWnd ;
		break ;

	case WM_CLOSE :
		DestroyWindow( hWnd ) ;
		break ;

	case WM_DESTROY :
		SockData.DestroyFlag = TRUE ;
		break ;

	case WSA_WINSOCKMESSAGE :
		Ret = WinSockProc( hWnd , message , wParam , lParam ) ;

		// �N���e�B�J���Z�N�V�����̉��
		if( HandleManageArray[ DX_HANDLETYPE_NETWORK ].InitializeFlag != FALSE )
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

		return Ret ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	if( HandleManageArray[ DX_HANDLETYPE_NETWORK ].InitializeFlag != FALSE )
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	return DefWindowProcW( hWnd , message , wParam , lParam ) ;
}
	


// �c�m�r�T�[�o�[���g���ăz�X�g������h�o�A�h���X���擾����
extern int NS_GetHostIPbyName( const TCHAR *HostName, IPDATA *IPDataBuf )
{
	hostent *HostInfo ;
	int Ret = 0 ;
	unsigned int ip ;
	const char *UseHostName ;
#ifdef UNICODE
	char TempHostName[ 1024 ] ;

	ConvString( ( const char * )HostName, WCHAR_T_CHARCODEFORMAT, TempHostName, sizeof( TempHostName ), DX_CHARCODEFORMAT_ASCII ) ;
	UseHostName = TempHostName ;
#else
	UseHostName = HostName ;
#endif

	if( SockData.InitializeFlag == FALSE )
	{
		if( WinData.CloseMessagePostFlag != TRUE ) InitializeNetWork() ;
		else return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �h�o�𐔒l������ɂ����������������̂��߂̏���
	// ��������Ε���
	ip = WinAPIData.WinSockFunc.inet_addrFunc( UseHostName ) ;
	if( ip != INADDR_NONE )
	{
		// �A�h���X�f�[�^����z�X�g�̏��𓾂�
		HostInfo = WinAPIData.WinSockFunc.gethostbyaddrFunc( ( const char * )&ip, 4, AF_INET ) ;
	}
	else
	{
		// �z�X�g������z�X�g�̏��𓾂�
		HostInfo = WinAPIData.WinSockFunc.gethostbynameFunc( UseHostName ) ;
	}
	if( HostInfo == NULL )
	{
		Ret = -1 ;
		goto ENDLABEL ;
	}

	// �z�X�g�̏�񂩂�h�o�A�h���X�𓾂�
	if( HostInfo->h_addrtype != AF_INET || HostInfo->h_length != 4 )
	{
		Ret = -2 ;
		goto ENDLABEL ;
	}
		
	IPDataBuf->d1 = ( unsigned char )HostInfo->h_addr_list[0][0] ;
	IPDataBuf->d2 = ( unsigned char )HostInfo->h_addr_list[0][1] ;
	IPDataBuf->d3 = ( unsigned char )HostInfo->h_addr_list[0][2] ;
	IPDataBuf->d4 = ( unsigned char )HostInfo->h_addr_list[0][3] ;

ENDLABEL :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return Ret ;
}

// �c�m�r�T�[�o�[���g���ăz�X�g������h�o�A�h���X���擾����( IPv6�� )
extern int NS_GetHostIPbyName_IPv6( const TCHAR *HostName, IPDATA_IPv6 *IPDataBuf )
{
	const char *UseHostName ;
	_addrinfo hints ;
	_addrinfo *ainfo ;
	_sockaddr_in6 *addrv6 ;
	int i ;
	int Ret = 0 ;
#ifdef UNICODE
	char TempHostName[ 1024 ] ;

	ConvString( ( const char * )HostName, WCHAR_T_CHARCODEFORMAT, TempHostName, sizeof( TempHostName ), DX_CHARCODEFORMAT_ASCII ) ;
	UseHostName = TempHostName ;
#else
	UseHostName = HostName ;
#endif

	if( SockData.InitializeFlag == FALSE )
	{
		if( WinData.CloseMessagePostFlag != TRUE ) InitializeNetWork() ;
		else return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �z�X�g������h�o�����擾
    _MEMSET( &hints, 0, sizeof( hints ) ) ;
    hints.ai_family = 23/*AF_INET6 �� 23*/ ;
	if( WinAPIData.WinSockFunc.getaddrinfoFunc( UseHostName, NULL, &hints, &ainfo ) != 0 )
	{
		Ret = -1 ;
		goto ENDLABEL ;
	}

	if( ainfo->ai_family != 23/*AF_INET6 �� 23*/ || ainfo->ai_addr == NULL )
	{
		Ret = -2 ;
		goto ENDLABEL ;
	}

	// �A�h���X���Z�b�g
	addrv6 = ( _sockaddr_in6 * )ainfo->ai_addr ;
	for( i = 0 ; i < 8 ; i ++ )
	{
		IPDataBuf->Word[ i ] = addrv6->sin6_addr.Word[ i ] ;
	}

ENDLABEL :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return Ret ;
}

// �l�b�g���[�N�n���h��������������
extern int InitializeNetworkHandle( HANDLEINFO * /*HandleInfo*/ )
{
	// �I��
	return 0 ;
}

// �l�b�g���[�N�n���h���̌�n�����s��
extern int TerminateNetworkHandle( HANDLEINFO *HandleInfo )
{
	SOCKETDATA *Sock = ( SOCKETDATA * )HandleInfo ;

	// UDP���ǂ����ŏ����𕪊�
	if( Sock->IsUDP )
	{
		// �\�P�b�g���L���������ꍇ����
		if( Sock->Socket != NULL && Sock->Socket != INVALID_SOCKET )
		{
			// ��u���b�L���O����
			WinAPIData.WinSockFunc.WSAAsyncSelectFunc( Sock->Socket, SockData.MessageWindow, 0, 0 );

			// �\�P�b�g�����
			WinAPIData.WinSockFunc.closesocketFunc( Sock->Socket );
			Sock->Socket = NULL ;
		}
	}
	else
	{
		// TCP

		// �ڑ��҂��p�n���h�����ǂ����ŏ����𕪊�
		if( SockData.ListenHandle == HandleInfo->Handle )
		{
			// �ڑ��҂��p�n���h��

			if( Sock->Socket && Sock->Socket != INVALID_SOCKET )
			{
				// �ʐM�֌W����
				NS_ProcessNetMessage() ;

				// ��u���b�L���O����
				WinAPIData.WinSockFunc.WSAAsyncSelectFunc( Sock->Socket, SockData.MessageWindow, 0, 0 );

				// �f�[�^����M�V���b�g�_�E��
				WinAPIData.WinSockFunc.shutdownFunc( Sock->Socket , 2 ) ;

				// �\�P�b�g�����
				WinAPIData.WinSockFunc.closesocketFunc( Sock->Socket );
				Sock->Socket = NULL ;
			}
			
			// �g�p���t���O��|��
			Sock->UseFlag = FALSE ;

			// �ڑ���t�p�̃n���h���𖳌��ɂ���
			SockData.ListenHandle = 0 ;
		}
		else
		{
			// �L���ȃ\�P�b�g�f�[�^�ł͂Ȃ������牽�������I��
			if( Sock->UseFlag == FALSE )
				return 0 ;

			// �ʐM�֌W����
			NS_ProcessNetMessage() ;

			// �c��̃f�[�^���o�b�t�@�����������
			{
				int time ;
				
				time = NS_GetNowCount() ;
				while( NS_GetNowCount() - time < 1000 && Sock->RecvComDataVol != 0 )
				{
					if( NS_ProcessNetMessage() != 0 ) break ;
					RecvSocket( HandleInfo->Handle ) ;
				}

				time = NS_GetNowCount() ;
				while( NS_GetNowCount() - time < 1000 && Sock->SendBufferR.DataLength != 0 )
				{
					if( NS_ProcessNetMessage() != 0 ) break ;
					SendSocket( HandleInfo->Handle ) ;
				}
			}

			// �\�P�b�g���̌�n�����s��
			if( Sock->Socket && Sock->Socket != INVALID_SOCKET )
			{
				// ��u���b�L���O����
				WinAPIData.WinSockFunc.WSAAsyncSelectFunc( Sock->Socket, SockData.MessageWindow, 0, 0 );

				// �\�P�b�g�����
				WinAPIData.WinSockFunc.closesocketFunc( Sock->Socket );
				Sock->Socket = NULL ;
			}

			// ����M�p�o�b�t�@�̉��
			RingBufTerminate( &Sock->RecvBufferToUserR ) ;
			RingBufTerminate( &Sock->SendBufferR ) ;

			// �g�p���t���O��|��
			Sock->UseFlag = FALSE ;
			
			// ���̑��K���Ƀt���O���Z�b�g
			Sock->ConnectionFlag = FALSE ;
			Sock->ConnectionLostFlag = TRUE ;
			Sock->AccepteConfirFlag = FALSE ;
		}
	}

	// �I��
	return 0 ;
}

// CONNECTNETWORK_GPARAM ������������
extern void InitConnectNetWorkBaseGParam( CONNECTNETWORK_GPARAM *GParam )
{
	GParam->NonCloseAfterLostFlag = SockData.NonCloseAfterLostFlag ;
	GParam->NonUseDXProtocolFlag = SockData.NonUseDXProtocolFlag ;
}

// ConnectNetWorkBase �̎������֐�
static int ConnectNetWorkBase_Static(
	CONNECTNETWORK_GPARAM *GParam,
	int NetHandle,
	IPDATA *IPData_IPv4,
	IPDATA_IPv6 *IPData_IPv6,
	int Port,
	int ASync,
	int ASyncThread
)
{
	_sockaddr_in6 con_v6 ;
	SOCKADDR_IN con;
	SOCKETDATA *Sock ;
	unsigned int pt ;
	int i ;
	int IsIPv6 ;

	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	if( ASyncThread )
	{
		if( NETHCHK_ASYNC( NetHandle, Sock) )
			goto ERR ;
	}
	else
	{
		if( NETHCHK( NetHandle, Sock) )
			goto ERR ;
	}

	// IPv6 ���ǂ������Z�b�g
	IsIPv6 = IPData_IPv6 != NULL ? TRUE : FALSE ;

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;
	
	// �|�[�g�̒l������
	pt = Port == -1 ? WinAPIData.WinSockFunc.htonsFunc( WSA_DEFAULTPORT ) : WinAPIData.WinSockFunc.htonsFunc( ( unsigned short )Port ) ;

	// �f�[�^��������
	Sock->IsUDP					= FALSE ;							// �t�c�o�͎g�p���Ȃ�
	Sock->IsIPv6				= IsIPv6 ;							// �h�o���U���ǂ�����ۑ�
	Sock->ErrorFlag 			= FALSE ;							// �G���[�����������t���O��|��
	Sock->UseFlag				= TRUE ;							// �f�[�^���g�p���ɂ���
	Sock->PreConnectionFlag		= TRUE ;							// �ڑ��O�t���O���Z�b�g����
	Sock->ConnectionFlag		= FALSE ;							// �ڑ��������t���O��ڑ����Ă��Ȃ���Ԃ�
	Sock->ConnectionLostFlag 	= FALSE ;							// �ؒf�m�F�t���O�͂Ƃ肠�����|���Ă���
	Sock->AccepteConfirFlag 	= TRUE ;							// �ڑ��m�F�t���O�͎���ڑ����Ă���̂Ŕc���ς݂Ƃ������Ƃɂ��Ă���
	Sock->AcceptFlag			= FALSE ;							// ���肩��ڑ����ꂽ�̂��t���O��|��
	Sock->DXProtocolFlag 		= !GParam->NonUseDXProtocolFlag ;	// �c�w���C�u�����Ǝ��̒ʐM�������g�����ǂ������Z�b�g
	Sock->CloseAfterLostFlag	= !GParam->NonCloseAfterLostFlag ;	// �ؒf����Ƀn���h����������邩�ǂ����̃t���O���Z�b�g
	if( IsIPv6 )
	{
		Sock->AcceptedIP_IPv6	= *IPData_IPv6 ;					// �ڑ���̂h�o��ۑ�
	}
	else
	{
		Sock->AcceptedIP 		= *IPData_IPv4 ;					// �ڑ���̂h�o��ۑ�
	}
	Sock->Port 					= pt ;								// �ڑ���̃|�[�g��ۑ�

	// �c�w���C�u�����Ǝ��̕��������ꍇ�͑���M�p�o�b�t�@��������
	if( Sock->DXProtocolFlag == TRUE )
	{
		// ���[�U�[�ɓn���Ƃ��Ɏg�������O�o�b�t�@�̏�����
		RingBufInitialize( &Sock->RecvBufferToUserR ) ;
		Sock->RecvComDataVol = 0 ;
		Sock->RecvComDataOriginalVol = 0 ;
		Sock->RecvBufferToUserOpenSize = 0 ;

		// ���M�o�b�t�@�֘A�̏�����
		RingBufInitialize( &Sock->SendBufferR ) ;
		Sock->SendComDataVol = 0 ;
		Sock->SendComDataComVol = 0 ;
	}

	// �\�P�b�g�̍쐬
	Sock->Socket = WinAPIData.WinSockFunc.socketFunc( IsIPv6 ? 23/*AF_INET6 �� 23*/ : AF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
	if( Sock->Socket == INVALID_SOCKET )
	{
		DXERRORNETWORK(( "\x77\x00\x73\x00\x32\x00\x5f\x00\x33\x00\x32\x00\x2e\x00\x64\x00\x6c\x00\x6c\x00\x20\x00\x4c\x30\xd5\x52\x5c\x4f\x57\x30\x66\x30\x44\x30\x7e\x30\x5b\x30\x93\x30\x01\x30\xbd\x30\xb1\x30\xc3\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5f\x00\x32\x00\x00"/*@ L"ws2_32.dll �����삵�Ă��܂���A�\�P�b�g�̍쐬�Ɏ��s���܂���_2" @*/ )) ;
		goto ERR ;
	}

	// WinSock ���b�Z�[�W�󂯎��ݒ�
	if( WinAPIData.WinSockFunc.WSAAsyncSelectFunc(
			Sock->Socket,
			SockData.MessageWindow,
			WSA_WINSOCKMESSAGE,
			FD_CONNECT | FD_WRITE | FD_READ | FD_CLOSE ) == SOCKET_ERROR )
	{
		DXERRORNETWORK(( "\x57\x00\x53\x00\x41\x00\x41\x00\x73\x00\x79\x00\x6e\x00\x63\x00\x53\x00\x65\x00\x6c\x00\x65\x00\x63\x00\x74\x00\x67\x30\xa8\x30\xe9\x30\xfc\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"WSAAsyncSelect�ŃG���[���������܂���" @*/ )) ;
		goto ERR ;
	}

	// �ڑ�
	if( IsIPv6 )
	{
		// �ڑ��p�����[�^�Z�b�g
		_MEMSET( &con_v6, 0, sizeof( con_v6 ) ) ;
		con_v6.sin6_family = 23/*AF_INET6 �� 23*/ ;
		con_v6.sin6_port = ( unsigned short )Sock->Port ;
		for( i = 0 ; i < 8 ; i ++ )
			con_v6.sin6_addr.Word[ i ] = IPData_IPv6->Word[ i ] ;

		// �ڑ�
		if( WinAPIData.WinSockFunc.connectFunc( Sock->Socket , (PSOCKADDR) &con_v6, sizeof( _sockaddr_in6 ) ) == SOCKET_ERROR )
		{
			if( WinAPIData.WinSockFunc.WSAGetLastErrorFunc() != WSAEWOULDBLOCK )
				goto ERR ;
		}
	}
	else
	{

		// �ڑ��p�����[�^�Z�b�g
		_MEMSET( &con, 0, sizeof( con ) ) ;
		con.sin_family = AF_INET;
		con.sin_addr = *(( in_addr *)IPData_IPv4 ) ;
		con.sin_port = ( u_short )Sock->Port ;

		// �ڑ�
		if( WinAPIData.WinSockFunc.connectFunc( Sock->Socket , (PSOCKADDR) &con, sizeof( SOCKADDR_IN ) ) == SOCKET_ERROR )
		{
			if( WinAPIData.WinSockFunc.WSAGetLastErrorFunc() != WSAEWOULDBLOCK )
				goto ERR ;
		}
	}

	// �����ڑ��w��̏ꍇ�͂����Őڑ�����莞�Ԑڑ���҂�
	if( ASyncThread == FALSE && ASync == FALSE )
	{
		int StTime ;
	
		StTime = NS_GetNowCount() ;
		if( SockData.TimeOutWait == 0 ) SockData.TimeOutWait = WSA_TIMEOUTWAIT ;
		while( ( ( NS_GetNowCount() - StTime ) < SockData.TimeOutWait ) && ( Sock->ConnectionFlag == FALSE ) )
		{
			if( NS_ProcessNetMessage() != 0 ) break ;
			Thread_Sleep( 1 ) ;
		}
		if( Sock->ConnectionFlag == FALSE )
			goto ERR ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// ����I��
	return 0 ;

ERR :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �G���[�I��
	return -1 ;
}

#ifndef DX_NON_ASYNCLOAD

// ConnectNetWorkBase �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void ConnectNetWorkBase_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	CONNECTNETWORK_GPARAM *GParam ;
	int NetHandle ;
	IPDATA *IPData_IPv4 ;
	IPDATA_IPv6 *IPData_IPv6 ;
	int Port ;
	int ASync ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	GParam = ( CONNECTNETWORK_GPARAM * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	NetHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	IPData_IPv4 = ( IPDATA * )GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	IPData_IPv6 = ( IPDATA_IPv6 * )GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	Port = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	ASync = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	if( IPData_IPv4 != NULL )
	{
		IPData_IPv4 = ( IPDATA * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	}
	if( IPData_IPv6 != NULL )
	{
		IPData_IPv6 = ( IPDATA_IPv6 * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	}

	Result = ConnectNetWorkBase_Static( GParam, NetHandle, IPData_IPv4, IPData_IPv6, Port, ASync, TRUE ) ;

	DecASyncLoadCount( NetHandle ) ;
	if( Result < 0 )
	{
		SubHandle( NetHandle ) ;
	}
}

#endif // DX_NON_ASYNCLOAD

// ConnectNetWorkBase �̃O���[�o���f�[�^�ɃA�N�Z�X���Ȃ���
extern int ConnectNetWorkBase_UseGParam(
	CONNECTNETWORK_GPARAM *GParam,
	IPDATA *IPData_IPv4,
	IPDATA_IPv6 *IPData_IPv6,
	int Port,
	int ASync,
	int ASyncLoadFlag
)
{
	int NetHandle ;
	int IsIPv6 ;
	HANDLELIST *List ;
	SOCKETDATA *Sock ;
	unsigned int pt ;
	int i ;

	// �ʐM�֌W������������Ă��Ȃ������珉����
	if( SockData.InitializeFlag == FALSE )
	{
		if( WinData.CloseMessagePostFlag != TRUE )	InitializeNetWork() ;
		else										return -1 ;
	}
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	if( WinData.ActiveFlag == FALSE )
		DxActiveWait() ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// IPv6 ���ǂ������Z�b�g
	IsIPv6 = IPData_IPv6 != NULL ? TRUE : FALSE ;

	// �|�[�g�̒l������
	pt = Port == -1 ? WinAPIData.WinSockFunc.htonsFunc( WSA_DEFAULTPORT ) : WinAPIData.WinSockFunc.htonsFunc( ( unsigned short )Port ) ;

	// ���ݐڑ����Ă��钆�Ɏw��̂h�o�����邩�m�F�A�������炻�̃n���h����Ԃ�
	for( List = HandleManageArray[ DX_HANDLETYPE_NETWORK ].ListFirst.Next ; List->Next != NULL ; List = List->Next )
	{
		Sock = ( SOCKETDATA * )List->Data ;
		if( Sock->UseFlag == FALSE ||
			Sock->IsIPv6 != IsIPv6 ||
			Sock->IsUDP != FALSE ||
			Sock->Port != pt ||
			Sock->ConnectionFlag == FALSE )
			continue ;

		if( IsIPv6 )
		{
			for( i = 0 ; i < 8 ; i ++ )
			{
				if( Sock->AcceptedIP_IPv6.Word[ i ] != IPData_IPv6->Word[ i ] )
					continue ;
			}
		}
		else
		{
			if( Sock->AcceptedIP.d1 != IPData_IPv4->d1 ||
				Sock->AcceptedIP.d2 != IPData_IPv4->d2 ||
				Sock->AcceptedIP.d3 != IPData_IPv4->d3 ||
				Sock->AcceptedIP.d4 != IPData_IPv4->d4 )
				continue ;
		}
		NetHandle = Sock->HandleInfo.Handle ;

		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

		// �n���h����Ԃ�
		return NetHandle ;
	}

	// �n���h���̍쐬
	NetHandle = AddHandle( DX_HANDLETYPE_NETWORK, FALSE, -1 ) ;
	if( NetHandle == -1 )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

		return -1 ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamStruct( NULL, &Addr, GParam, sizeof( CONNECTNETWORK_GPARAM ) ) ;
		AddASyncLoadParamInt( NULL, &Addr, NetHandle ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, IPData_IPv4 ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, IPData_IPv6 ) ;
		AddASyncLoadParamInt( NULL, &Addr, Port ) ;
		AddASyncLoadParamInt( NULL, &Addr, ASync ) ;
		if( IPData_IPv4 != NULL )
		{
			AddASyncLoadParamStruct( NULL, &Addr, IPData_IPv4, sizeof( IPDATA ) ) ;
		}
		if( IPData_IPv6 != NULL )
		{
			AddASyncLoadParamStruct( NULL, &Addr, IPData_IPv6, sizeof( IPDATA_IPv6 ) ) ;
		}

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = ConnectNetWorkBase_ASync ;
		Addr = 0 ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, GParam, sizeof( CONNECTNETWORK_GPARAM ) ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, NetHandle ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, IPData_IPv4 ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, IPData_IPv6 ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, Port ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, ASync ) ;
		if( IPData_IPv4 != NULL )
		{
			AddASyncLoadParamStruct( AParam->Data, &Addr, IPData_IPv4, sizeof( IPDATA ) ) ;
		}
		if( IPData_IPv6 != NULL )
		{
			AddASyncLoadParamStruct( AParam->Data, &Addr, IPData_IPv6, sizeof( IPDATA_IPv6 ) ) ;
		}

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( NetHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( ConnectNetWorkBase_Static( GParam, NetHandle, IPData_IPv4, IPData_IPv6, Port, ASync, FALSE ) < 0 )
			goto ERR ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return NetHandle ;

ERR :
	SubHandle( NetHandle ) ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	return -1 ;
}

// ConnectNetWork �̏������s���֐�
static int ConnectNetWorkBase( IPDATA *IPData_IPv4, IPDATA_IPv6 *IPData_IPv6, int Port, int ASync )
{
	CONNECTNETWORK_GPARAM GParam ;
	int Result ;

	InitConnectNetWorkBaseGParam( &GParam ) ;

	Result = ConnectNetWorkBase_UseGParam( &GParam, IPData_IPv4, IPData_IPv6, Port, ASync, GetASyncLoadFlag() ) ;
	if( Result < 0 )
	{
		if( IPData_IPv6 != NULL )
		{
			DXERRORNETWORK(( "\x25\x00\x78\x00\x3a\x00\x25\x00\x78\x00\x3a\x00\x25\x00\x78\x00\x3a\x00\x25\x00\x78\x00\x3a\x00\x25\x00\x78\x00\x3a\x00\x25\x00\x78\x00\x3a\x00\x25\x00\x78\x00\x3a\x00\x25\x00\x78\x00\x20\x00\x78\x30\x6e\x30\xa5\x63\x9a\x7d\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x02\x30\x0a\x00\x00"/*@ L"%x:%x:%x:%x:%x:%x:%x:%x �ւ̐ڑ��Ɏ��s���܂����B\n" @*/,
				IPData_IPv6->Word[ 0 ], IPData_IPv6->Word[ 1 ], IPData_IPv6->Word[ 2 ], IPData_IPv6->Word[ 3 ],
				IPData_IPv6->Word[ 4 ], IPData_IPv6->Word[ 5 ], IPData_IPv6->Word[ 6 ], IPData_IPv6->Word[ 7 ] ) ) ;
		}
		else
		{
			DXERRORNETWORK(( "\x25\x00\x64\x00\x2e\x00\x25\x00\x64\x00\x2e\x00\x25\x00\x64\x00\x2e\x00\x25\x00\x64\x00\x20\x00\x78\x30\x6e\x30\xa5\x63\x9a\x7d\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x02\x30\x0a\x00\x00"/*@ L"%d.%d.%d.%d �ւ̐ڑ��Ɏ��s���܂����B\n" @*/,IPData_IPv4->d1 , IPData_IPv4->d2 , IPData_IPv4->d3 , IPData_IPv4->d4) ) ;
		}
	}

	return Result ;
}

// ���}�V���ɐڑ�����
extern int NS_ConnectNetWork_IPv6( IPDATA_IPv6 IPData, int Port )
{
	return ConnectNetWorkBase( NULL, &IPData, Port, FALSE ) ;
}

// ���}�V���ɐڑ�����
extern int NS_ConnectNetWork( IPDATA IPData, int Port )
{
	return ConnectNetWorkBase( &IPData, NULL, Port, FALSE ) ;
}

// ���}�V���ɐڑ�����
extern int NS_ConnectNetWork_IPv6_ASync( IPDATA_IPv6 IPData, int Port )
{
	return ConnectNetWorkBase( NULL, &IPData, Port, TRUE ) ;
}

// ���}�V���ɐڑ�����
extern int NS_ConnectNetWork_ASync( IPDATA IPData, int Port )
{
	return ConnectNetWorkBase( &IPData, NULL, Port, TRUE ) ;
}

// �ڑ����󂯂����Ԃɂ���
static int PreparationListenNetWork_Base( int IsIPv6, int Port )
{
	int ReturnValue = 0 ;
	SOCKETDATA *ListenSock ;

	// �ʐM�֌W������������Ă��Ȃ������珉����
	if( SockData.InitializeFlag == FALSE )
	{
		if( WinData.CloseMessagePostFlag != TRUE ) InitializeNetWork() ;
		else										return -1 ;
	}
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// ���Ɏ�t���������牽�������I��	
//	if( SockData.CSocket[ MAX_SOCKET_NUM ] != NULL )
	if( !NETHCHK( SockData.ListenHandle, ListenSock ) )
	{
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �ڑ���t�҂��p�̃n���h����ǉ�
	SockData.ListenHandle = AddHandle( DX_HANDLETYPE_NETWORK, FALSE, -1 ) ;
	if( NETHCHK_ASYNC( SockData.ListenHandle, ListenSock ) )
	{
		DXERRORNETWORK(( "\xa5\x63\x9a\x7d\x85\x5f\x61\x30\x28\x75\xcd\x30\xc3\x30\xc8\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�ڑ��҂��p�l�b�g�n���h���̍쐬�Ɏ��s���܂���" @*/ )) ;
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �g�p����f�[�^����������
	ListenSock->ErrorFlag 			= FALSE ;			// �G���[�����������t���O��|��
	ListenSock->UseFlag				= TRUE ;			// �f�[�^���g�p���ɂ���
	ListenSock->IsIPv6				= IsIPv6 ;			// IPv6 ���ǂ�����ۑ�

	// �A�N�Z�v�g�p�\�P�b�g�̍쐬
	ListenSock->Socket = WinAPIData.WinSockFunc.socketFunc( IsIPv6 ? 23/*AF_INET6 �� 23*/ : AF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
	if( ListenSock->Socket == INVALID_SOCKET )
	{
		DXERRORNETWORK(( "\xbd\x30\xb1\x30\xc3\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x02\x30\x77\x00\x73\x00\x32\x00\x5f\x00\x33\x00\x32\x00\x2e\x00\x64\x00\x6c\x00\x6c\x00\x4c\x30\x5c\x4f\xd5\x52\x57\x30\x66\x30\x44\x30\x7e\x30\x5b\x30\x93\x30\x00"/*@ L"�\�P�b�g�̍쐬�Ɏ��s���܂����Bws2_32.dll���쓮���Ă��܂���" @*/ )) ;
		goto ERR ;
	}

	// WinSock ���b�Z�[�W�󂯎��ݒ�
	if( WinAPIData.WinSockFunc.WSAAsyncSelectFunc( ListenSock->Socket, SockData.MessageWindow, WSA_WINSOCKMESSAGE , FD_READ | FD_WRITE | FD_CLOSE | FD_ACCEPT ) == SOCKET_ERROR )
	{
		DXERRORNETWORK(( "\x57\x00\x53\x00\x41\x00\x73\x00\x79\x00\x6e\x00\x63\x00\x53\x00\x65\x00\x6c\x00\x65\x00\x63\x00\x74\x00\x67\x30\xa8\x30\xe9\x30\xfc\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"WSAsyncSelect�ŃG���[���������܂���" @*/ )) ;
		goto ERR ;
	}

	// �쐬�����\�P�b�g�ƃ|�[�g�ԍ��̊֘A�t�����s��
	if( IsIPv6 )
	{
		_sockaddr_in6 local_v6;
	
		_MEMSET( &local_v6, 0, sizeof( local_v6 ) ) ;
		local_v6.sin6_family = 23/*AF_INET6 �� 23*/ ;
		local_v6.sin6_port = Port == -1 ? WinAPIData.WinSockFunc.htonsFunc( WSA_DEFAULTPORT ) : WinAPIData.WinSockFunc.htonsFunc( ( unsigned short )Port ) ;

		if( WinAPIData.WinSockFunc.bindFunc( ListenSock->Socket , (struct sockaddr FAR *) &local_v6, sizeof(local_v6) ) == SOCKET_ERROR )
		{
			if( WinAPIData.WinSockFunc.WSAGetLastErrorFunc() != WSAEWOULDBLOCK )
			{
				DXERRORNETWORK(( "\x62\x00\x69\x00\x6e\x00\x64\x00\xfa\x51\x65\x67\x7e\x30\x5b\x30\x93\x30\x00"/*@ L"bind�o���܂���" @*/ )) ;
				goto ERR ;
			}
		}
	}
	else
	{
		SOCKADDR_IN	local;
	
		_MEMSET( &local, 0, sizeof( local ) ) ;
		local.sin_family = AF_INET;
		local.sin_port = Port == -1 ? WinAPIData.WinSockFunc.htonsFunc( WSA_DEFAULTPORT ) : WinAPIData.WinSockFunc.htonsFunc( ( unsigned short )Port ) ;
		local.sin_addr.s_addr = INADDR_ANY;

		if( WinAPIData.WinSockFunc.bindFunc( ListenSock->Socket , (struct sockaddr FAR *) &local, sizeof(local) ) == SOCKET_ERROR )
		{
			if( WinAPIData.WinSockFunc.WSAGetLastErrorFunc() != WSAEWOULDBLOCK )
			{
				DXERRORNETWORK(( "\x62\x00\x69\x00\x6e\x00\x64\x00\xfa\x51\x65\x67\x7e\x30\x5b\x30\x93\x30\x00"/*@ L"bind�o���܂���" @*/ )) ;
				goto ERR ;
			}
		}
	}

	// �ڑ��҂��p�\�P�b�g��ҋ@��Ԃɂ���
	if( WinAPIData.WinSockFunc.listenFunc( ListenSock->Socket, SOMAXCONN ) == SOCKET_ERROR )
	{
		DXERRORNETWORK(( "\x6c\x00\x69\x00\x73\x00\x74\x00\x65\x00\x6e\x00\x92\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"listen�����s���܂���" @*/ )) ;
		goto ERR ;
	}

FUNCTIONEND :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return ReturnValue ;
	
	// �G���[����
ERR :
	// �n���h���̍폜
	SubHandle( SockData.ListenHandle ) ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	return -1 ;
}

// �ڑ����󂯂����Ԃɂ���
extern int NS_PreparationListenNetWork_IPv6( int Port )
{
	return PreparationListenNetWork_Base( TRUE, Port ) ;
}

// �ڑ����󂯂����Ԃɂ���
extern int NS_PreparationListenNetWork( int Port )
{
	return PreparationListenNetWork_Base( FALSE, Port ) ;
}

// �A�N�Z�v�g����
extern int AcceptNetWork( void )
{
	int ReturnValue = TRUE ;
//	SOCKETDATA * ListenSock = SockData.CSocket[ MAX_SOCKET_NUM ] ;
	SOCKETDATA *ListenSock = NULL ;
	SOCKETDATA *Sock = NULL ;
	int NewNetHandle ;

	// �ʐM�֌W������������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �ʐM�֌W�̏������s��
	NS_ProcessNetMessage() ;

	// �ڑ��҂������Ă��Ȃ��ꍇ�͉��������I��
//	if( ListenSock == NULL )
	if( NETHCHK( SockData.ListenHandle, ListenSock ) )
	{
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �V�����n���h����ǉ�
	NewNetHandle = AddHandle( DX_HANDLETYPE_NETWORK, FALSE, -1 ) ;
	if( NETHCHK_ASYNC( NewNetHandle, Sock ) )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x61\x00\x63\x00\x63\x00\x65\x00\x70\x00\x74\x00\x20\x00\x28\x75\x6e\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x0a\x00\x00"/*@ L"accept �p�̃n���h���쐬�Ɏ��s���܂��� \n" @*/ ) ;
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �f�[�^������
	Sock->IsUDP					= FALSE ;							// �ڑ����ꂽ�ꍇ�͕K���s�b�o
	Sock->IsIPv6				= ListenSock->IsIPv6 ;				// �h�o���U���ǂ�����ۑ�
	Sock->ErrorFlag 			= FALSE ;							// �G���[�����������t���O��|��
	Sock->UseFlag				= TRUE ;							// �f�[�^���g�p���ɂ���
	Sock->PreConnectionFlag		= FALSE ;							// �ڑ��O�t���O��|��
	Sock->ConnectionFlag		= TRUE ;							// �ڑ��������t���O��ڑ����Ă����Ԃ�
	Sock->ConnectionLostFlag 	= FALSE ;							// �ؒf�m�F�t���O�͂Ƃ肠�����|���Ă���
	Sock->AccepteConfirFlag 	= FALSE ;							// �ڑ��m�F�t���O�͑��肩�珟��ɐڑ����ꂽ�̂Ŋm�F���Ă��Ȃ���Ԃɂ��Ă���
	Sock->AcceptFlag			= TRUE ;							// ���肩��ڑ����ꂽ�̂��t���O�𗧂Ă�
	Sock->DXProtocolFlag 		= !SockData.NonUseDXProtocolFlag ;	// �c�w���C�u�����Ǝ��̒ʐM�������g�����ǂ������Z�b�g
	Sock->CloseAfterLostFlag	= !SockData.NonCloseAfterLostFlag ;	// �ؒf����Ƀn���h����������邩�ǂ����̃t���O���Z�b�g

	// �c�w���C�u�����Ǝ��̕��������ꍇ�͑���M�p�o�b�t�@��������
	if( Sock->DXProtocolFlag == TRUE )
	{
		// ���[�U�[�ɓn���Ƃ��Ɏg�������O�o�b�t�@�̏�����
		RingBufInitialize( &Sock->RecvBufferToUserR ) ;
		Sock->RecvComDataVol = 0 ;
		Sock->RecvComDataOriginalVol = 0 ;
		Sock->RecvBufferToUserOpenSize = 0 ;

		// ���M�o�b�t�@�֘A�̏�����
		RingBufInitialize( &Sock->SendBufferR ) ;
		Sock->SendComDataVol = 0 ;
		Sock->SendComDataComVol = 0 ;
	}
	
	// �ڑ��҂������Ă���ڑ���̏����擾����
	{
		int Len ;
		if( Sock->IsIPv6 )
		{
			int i ;
			_sockaddr_in6 AcIP_v6 ;

			// �ڑ��m��
			Len = sizeof( _sockaddr_in6 ) ;
			if( ( Sock->Socket = WinAPIData.WinSockFunc.acceptFunc( ListenSock->Socket , ( SOCKADDR * )&AcIP_v6 , &Len ) ) == INVALID_SOCKET )
			{
				DXERRORNETWORK(( "\x61\x00\x63\x00\x63\x00\x65\x00\x70\x00\x74\x00\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"accept�Ɏ��s���܂���" @*/ )) ;
				goto ERR ;
			}

			// �h�o�̕ۑ�
			for( i = 0 ; i < 8 ; i ++ )
				Sock->AcceptedIP_IPv6.Word[ i ] = AcIP_v6.sin6_addr.Word[ i ] ;
			
			// �|�[�g�̕ۑ�
			Sock->Port = AcIP_v6.sin6_port ;
		}
		else
		{
			SOCKADDR AcIP ;
			SOCKADDR_IN *AcIP_IN ;

			// �ڑ��m��
			Len = sizeof( SOCKADDR_IN ) ;
			if( ( Sock->Socket = WinAPIData.WinSockFunc.acceptFunc( ListenSock->Socket , &AcIP , &Len ) ) == INVALID_SOCKET )
			{
				DXERRORNETWORK(( "\x61\x00\x63\x00\x63\x00\x65\x00\x70\x00\x74\x00\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"accept�Ɏ��s���܂���" @*/ )) ;
				goto ERR ;
			}

			// �h�o�̕ۑ�
			AcIP_IN = ( SOCKADDR_IN * )&AcIP ;
			Sock->AcceptedIP.d1 = AcIP_IN->sin_addr.S_un.S_un_b.s_b1 ;
			Sock->AcceptedIP.d2 = AcIP_IN->sin_addr.S_un.S_un_b.s_b2 ;
			Sock->AcceptedIP.d3 = AcIP_IN->sin_addr.S_un.S_un_b.s_b3 ;
			Sock->AcceptedIP.d4 = AcIP_IN->sin_addr.S_un.S_un_b.s_b4 ;
			
			// �|�[�g�̕ۑ�
			Sock->Port = AcIP_IN->sin_port ;
		}
	}

	// WinSock ���b�Z�[�W�󂯎��ݒ�
	if( WinAPIData.WinSockFunc.WSAAsyncSelectFunc( Sock->Socket , SockData.MessageWindow, WSA_WINSOCKMESSAGE , FD_WRITE | FD_READ | FD_CLOSE ) == SOCKET_ERROR )
	{
		DXERRORNETWORK(( "\x57\x00\x53\x00\x41\x00\x41\x00\x73\x00\x79\x00\x6e\x00\x63\x00\x53\x00\x65\x00\x6c\x00\x65\x00\x63\x00\x74\x00\x6e\x30\xa8\x30\xe9\x30\xfc\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x41\x00\x63\x00\x63\x00\x65\x00\x70\x00\x74\x00\x4e\x00\x65\x00\x74\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x00"/*@ L"WSAAsyncSelect�̃G���[ in AcceptNetWork" @*/ )) ;
		goto ERR ;
	}

FUNCTIONEND :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return ReturnValue ;
	
	// �G���[����	
ERR :

	// �n���h�����폜
	SubHandle( NewNetHandle ) ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	return -1 ;
}

// �ڑ����󂯂���Ԃ̉���
extern int NS_StopListenNetWork( void )
{
	if( SockData.InitializeFlag == FALSE )
		return -1 ;

	// �ڑ��󂯕t���p�̃n���h�����폜
	return SubHandle( SockData.ListenHandle ) ;
/*
	int ReturnValue = 0 ;
	SOCKETDATA * ListenSock = SockData.CSocket[ MAX_SOCKET_NUM ] ;

	// �ʐM�֌W������������Ă��Ȃ������珉����
	if( SockData.InitializeFlag == FALSE )
	{
		if( WinData.CloseMessagePostFlag != TRUE ) InitializeNetWork() ;
		else										return -1 ;
	}
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// �ڑ���t��Ԃł͖��������牽�������I��
	if( ListenSock == NULL )
	{
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// ��u���b�L���O����
	WinAPIData.WinSockFunc.WSAAsyncSelectFunc( ListenSock->Socket, SockData.MessageWindow, 0, 0 );

	// �f�[�^����M�V���b�g�_�E��
	WinAPIData.WinSockFunc.shutdownFunc( ListenSock->Socket , 2 ) ;

	// �\�P�b�g�����
	WinAPIData.WinSockFunc.closesocketFunc( ListenSock->Socket );
	ListenSock->Socket = 0 ;
	
	// �g�p���t���O��|��
	ListenSock->UseFlag = FALSE ;

	// ���X�g����O��
	SubHandleList( &ListenSock->List ) ;

	// �������̉��
	DXFREE( ListenSock ) ;
	SockData.CSocket[ MAX_SOCKET_NUM ] = NULL ;

	// �\�P�b�g�̑������f�N�������g
	SockData.SocketNum -- ;

FUNCTIONEND :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return ReturnValue ;
*/
}


// �ڑ����I������
extern int NS_CloseNetWork( int NetHandle )
{
	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE )
		return -1 ;

	// �n���h�����폜
	return SubHandle( NetHandle ) ;
/*
	int ReturnValue = 0 ;
	SOCKETDATA * Sock ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �L���ȃn���h���ł͂Ȃ�������G���[
	if( TCPNETHCHK( NetHandle, Sock ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

		return -1 ;
	}

	// �L���ȃ\�P�b�g�f�[�^�ł͂Ȃ������牽�������I��
	if( Sock->UseFlag == FALSE )
	{
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// �c��̃f�[�^���o�b�t�@�����������
	{
		int time ;
		
		time = NS_GetNowCount() ;
		while( NS_GetNowCount() - time < 1000 && Sock->RecvComDataVol != 0 )
		{
			if( NS_ProcessNetMessage() != 0 ) break ;
			RecvSocket( NetHandle ) ;
		}

		time = NS_GetNowCount() ;
		while( NS_GetNowCount() - time < 1000 && Sock->SendBufferR.DataLength != 0 )
		{
			if( NS_ProcessNetMessage() != 0 ) break ;
			SendSocket( NetHandle ) ;
		}
	}

	// �\�P�b�g���̌�n�����s��
	{
		// ��u���b�L���O����
		WinAPIData.WinSockFunc.WSAAsyncSelectFunc( Sock->Socket, SockData.MessageWindow, 0, 0 );

		// �\�P�b�g�����
		WinAPIData.WinSockFunc.closesocketFunc( Sock->Socket );
		Sock->Socket = 0 ;
	}

	// ����M�p�o�b�t�@�̉��
	RingBufTerminate( &Sock->RecvBufferToUserR ) ;
	RingBufTerminate( &Sock->SendBufferR ) ;

	// �g�p���t���O��|��
	Sock->UseFlag = FALSE ;
	
	// ���̑��K���Ƀt���O���Z�b�g
	Sock->ConnectionFlag = FALSE ;
	Sock->ConnectionLostFlag = TRUE ;
	Sock->AccepteConfirFlag = FALSE ;

	// ���X�g����O��
	SubHandleList( &Sock->List ) ;

	// �������̉��
	DXFREE( Sock ) ;
	SockData.CSocket[ NetHandle & DX_HANDLEINDEX_MASK ] = NULL ;

	// �\�P�b�g�̑������f�N�������g
	SockData.SocketNum -- ;

FUNCTIONEND :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return ReturnValue ;
*/
}




// �f�[�^�̎�M����
static int RecvSocket( int NetHandle )
{
	static int ProcessRunFlag = FALSE ;
	int ReturnValue = 0 ;
	SOCKETDATA * Sock ;
	int RecvLen ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �L���ȃn���h���ł͂Ȃ�������G���[
	if( TCPNETHCHK( NetHandle, Sock ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// ���ɏ��������s����Ă����牽�������I��
	if( ProcessRunFlag == TRUE )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �u���������s���v�t���O�𗧂Ă�
	ProcessRunFlag = TRUE ;


	// �ʐM�֌W����
	NS_ProcessNetMessage() ;
	
	// �L���ȃ\�P�b�g�f�[�^�ł͂Ȃ������牽�������I��
	if( Sock->UseFlag == FALSE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x09\x67\xb9\x52\x6a\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x67\x30\x6f\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x52\x00\x65\x00\x63\x00\x76\x00\x53\x00\x6f\x00\x63\x00\x6b\x00\x65\x00\x74\x00\x0a\x00\x00"/*@ L"�L���ȃl�b�g���[�N�n���h���ł͂���܂��� in RecvSocket\n" @*/ ) ; 
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}


	// �d��ȃG���[���������Ă���ꍇ�͉��������I��
	if( Sock->ErrorFlag == TRUE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x53\x30\x6e\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6b\x30\x6f\x30\xa8\x30\xe9\x30\xfc\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x52\x00\x65\x00\x63\x00\x76\x00\x53\x00\x6f\x00\x63\x00\x6b\x00\x65\x00\x74\x00\x0a\x00\x00"/*@ L"���̃l�b�g���[�N�n���h���ɂ̓G���[���������Ă��܂� in RecvSocket\n" @*/ ) ;
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �c�w���C�u�����Ǝ��̕������ǂ����ŏ����𕪊�
	if( Sock->DXProtocolFlag == TRUE )
	{
		do{ 
			// ��M����f�[�^�̗ʂ��Z�b�g
			if( Sock->RecvComDataVol == 0 )
			{
				// ��M����f�[�^�̗ʂ��擾�ł��邩���ׂ�
				RecvLen = WinAPIData.WinSockFunc.recvFunc( Sock->Socket , ( char * )&Sock->RecvComDataVol , sizeof( int ) , MSG_PEEK ) ;
				if( RecvLen < 0 )
					break ;

				// ��M����f�[�^�ʂ�ۑ��o����f�[�^�ʂ������Ă��Ȃ������牽�������I��
				if( RecvLen < 4 )
					break ;

				// ��M����f�[�^�̗ʂ𐳎��Ɏ擾
				RecvLen = WinAPIData.WinSockFunc.recvFunc( Sock->Socket , ( char * )&Sock->RecvComDataVol , sizeof( int ) , 0 ) ;
				if( RecvLen < 4 )
					break ;

				// ��M����f�[�^�T�C�Y��ۑ�
				Sock->RecvComDataOriginalVol  = Sock->RecvComDataVol ;

				// �V���Ɏ�M����f�[�^�����܂�悤�Ƀo�b�t�@�����T�C�Y
				if( RingBufReSize( &Sock->RecvBufferToUserR, Sock->RecvComDataVol ) < 0 )
				{
					DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd7\x53\xe1\x4f\xc7\x30\xfc\x30\xbf\x30\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xea\x30\xf3\x30\xb0\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\xea\x30\xb5\x30\xa4\x30\xba\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\xc7\x30\xfc\x30\xbf\x30\xb5\x30\xa4\x30\xba\x30\x20\x00\x25\x00\x64\x00\x20\x00\x62\x00\x79\x00\x74\x00\x65\x00\x29\x00\x00"/*@ L"��M�f�[�^��ۑ����郊���O�o�b�t�@�̃��T�C�Y�Ɏ��s���܂���(�f�[�^�T�C�Y %d byte)" @*/, Sock->RecvComDataVol )) ;

					// ���s������d��ȃG���[
					Sock->ErrorFlag = TRUE ;

					ReturnValue = -1 ;
					goto FUNCTIONEND ;
				}
			}

			// �f�[�^�̎�M
			{
				RINGBUF *RingBuf = &Sock->RecvBufferToUserR ;

				// �A�h���X�`�F�b�N
				RingBufAddressCheck( RingBuf ) ;

				// �Q��ɕ����Ċi�[���Ȃ���΂Ȃ�Ȃ����ǂ����ŏ����𕪊�
				if( RingBuf->End + Sock->RecvComDataVol > RingBuf->BufferSize )
				{
					int AllRecvLen ;
				
					// �Q��ɕʂ��Ċi�[����ꍇ�̏���

					// �ŏ��̃f�[�^���擾
					AllRecvLen = 0 ;
					RecvLen = WinAPIData.WinSockFunc.recvFunc( Sock->Socket,(char *)RingBuf->DataBuffer + RingBuf->End,
															 RingBuf->BufferSize - RingBuf->End, 0 ) ;
					if( RecvLen < 0 )
						break ;

					// �w�肵���f�[�^�ʂ������Ǝ擾�ł��������ׂ�
					if( RecvLen < RingBuf->BufferSize - RingBuf->End )
					{
						RingBuf->End += RecvLen ;
					}
					else
					{
						// �擾�����f�[�^�ʂ̕ۑ�
						AllRecvLen = RecvLen ;

						// ���̃f�[�^���擾
						RecvLen = WinAPIData.WinSockFunc.recvFunc( Sock->Socket, (char *)RingBuf->DataBuffer, Sock->RecvComDataVol - AllRecvLen, 0 ) ;

						// �G���[���N���ĉ����f�[�^����M�ł��Ȃ�������ŏ��Ɏ擾�����f�[�^�������擾�������Ƃɂ���
						if( RecvLen < 0 )
						{
							RecvLen = AllRecvLen ;
							RingBuf->End = 0  ;
						}
						else
						{
							// �Ō�܂ł�������f�[�^���擾�ł����ꍇ
						
							// �A�h���X�̐���
							RingBuf->End = RecvLen  ;

							// �擾�����f�[�^�̑��ʂ��Z�b�g
							RecvLen += AllRecvLen ;
						}
					}
				}
				else
				{
					// �P��Ŋi�[����ꍇ�̏���
					
					// ��x�Ɏ擾
					RecvLen = WinAPIData.WinSockFunc.recvFunc( Sock->Socket, (char *)RingBuf->DataBuffer + RingBuf->End, Sock->RecvComDataVol, 0 ) ;
					if( RecvLen < 0 )
						break ;

					// �A�h���X�̐���
					RingBuf->End += RecvLen ;
				}

				// �擾�����f�[�^�̗ʂ����L���f�[�^�ʂ𑝂₷
				RingBuf->DataLength += RecvLen ;
			}

			// ��M���ׂ��f�[�^�ʂ���M�����f�[�^�ʕ��������炷
			Sock->RecvComDataVol -= RecvLen ;

			// ��M���ׂ��f�[�^�ʂ��O�ɂȂ����ꍇ�͗��p�҂Ɍ��J����f�[�^�ʂɍ����M�����f�[�^�̗ʂ����Z����
			if( Sock->RecvComDataVol == 0 )
				Sock->RecvBufferToUserOpenSize += Sock->RecvComDataOriginalVol ;

			if( NS_ProcessNetMessage() != 0 ) break ;
		
		}// �G���[���N���Ă��Ȃ��Ă܂��擾���ׂ��f�[�^������ꍇ�̓��[�v
		while( RecvLen > 0 && Sock->RecvComDataVol == 0 ) ;
	}

FUNCTIONEND :

	// �u���������s���v�t���O��|��
	ProcessRunFlag = FALSE ; 

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return ReturnValue ;
}

// ���܂����f�[�^�̑��M����
extern int SendSocket( int NetHandle )
{
	static int ProcessRunFlag = FALSE ;
	int ReturnValue = 0 ;
	SOCKETDATA * Sock ;
	int SendVol ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �L���ȃn���h���ł͂Ȃ�������G���[
	if( TCPNETHCHK( NetHandle, Sock ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// ���ɏ��������s����Ă����牽�������I��
	if( ProcessRunFlag == TRUE )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �u���������s���v�t���O�𗧂Ă�
	ProcessRunFlag = TRUE ;

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// �L���ȃ\�P�b�g�f�[�^�ł͂Ȃ������牽�������I��
	if( Sock->UseFlag == FALSE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x09\x67\xb9\x52\x6a\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x67\x30\x6f\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x53\x00\x65\x00\x6e\x00\x64\x00\x53\x00\x6f\x00\x63\x00\x6b\x00\x65\x00\x74\x00\x0a\x00\x00"/*@ L"�L���ȃl�b�g���[�N�n���h���ł͂���܂��� in SendSocket\n" @*/ ) ; 
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �d��ȃG���[���������Ă���ꍇ�͉��������I��
	if( Sock->ErrorFlag == TRUE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x53\x30\x6e\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6b\x30\x6f\x30\xa8\x30\xe9\x30\xfc\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x53\x00\x65\x00\x6e\x00\x64\x00\x53\x00\x6f\x00\x63\x00\x6b\x00\x65\x00\x74\x00\x0a\x00\x00"/*@ L"���̃l�b�g���[�N�n���h���ɂ̓G���[���������Ă��܂� in SendSocket\n" @*/ ) ;
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// ���M���ׂ��f�[�^�e�ʂ��Ȃ��ꍇ�͏I��
	if( Sock->SendBufferR.DataLength == 0 )
	{
		ReturnValue = 0 ;
		goto FUNCTIONEND ;
	}

	// �c�w���C�u�����Ǝ��̕������ǂ����ŏ����𕪊�
	if( Sock->DXProtocolFlag == TRUE )
	{
		do{
			// �܂����ɑ��M����f�[�^�ʂ𑗐M���Ă��Ȃ��ꍇ�͑��M����
			if( Sock->SendComDataComVol != 0 )
			{
				SendVol = WinAPIData.WinSockFunc.sendFunc( Sock->Socket , ( char * )&Sock->SendComDataVol + 4 - Sock->SendComDataComVol , Sock->SendComDataComVol , 0 ) ;
				if( SendVol < 0 )
					break ;

				// ���M�ł����������Z
				Sock->SendComDataComVol -= SendVol ;
				
				// ���M������Ȃ������炱���ŏI��
				if( Sock->SendComDataComVol != 0 )
					break ;
			}

			// ���M���f�[�^���Ȃ��ꍇ�͎��Ȃ�f�[�^�̑��M�ɂ�����
			if( Sock->SendComDataVol == 0 )
			{
				// ���M����f�[�^�ʂ𑗐M����
				Sock->SendComDataVol = Sock->SendBufferR.DataLength ;
				SendVol = WinAPIData.WinSockFunc.sendFunc( Sock->Socket , ( char * )&Sock->SendComDataVol , 4 , 0 ) ;
				
				// �S�����M�ł��Ȃ������玟��ɂ�������
				if( SendVol < 0 )
				{
					Sock->SendComDataVol = 0 ;
					break ;
				}

				// ���M������Ȃ������玟�񑱂��𑗐M����
				if( SendVol < 4 )
				{
					Sock->SendComDataComVol = 4 - SendVol ;
					break ;
				}
			}

			// �f�[�^�̑��M
			{
				RINGBUF *RingBuf = &Sock->SendBufferR ;
				
				// �A�h���X�`�F�b�N
				RingBufAddressCheck( RingBuf ) ;

				// �Q��ɕʂ��Ȃ���΂����Ȃ����ǂ����ŏ����𕪊�
				if( RingBuf->Start + Sock->SendComDataVol > RingBuf->BufferSize )
				{
					int AllSendVol ;
				
					// �Q��ɕʂ���ꍇ�̏���

					// �ŏ��̃f�[�^�𑗐M
					AllSendVol = 0 ;
					SendVol = WinAPIData.WinSockFunc.sendFunc( Sock->Socket , (char *)RingBuf->DataBuffer + RingBuf->Start, RingBuf->BufferSize - RingBuf->Start, 0 ) ;
					if( SendVol < 0 )
						break ;

					// ���M�ł����f�[�^�ʂ��w��������Ȃ������炱���ŏI��
					if( SendVol < RingBuf->BufferSize - RingBuf->Start )
					{
						// �A�h���X�̒���
						RingBuf->Start += SendVol ;
					}
					else
					{
						// �܂����M�ł���ꍇ�͑��M

						// ���M�ł����f�[�^�ʂ�ۑ�
						AllSendVol = SendVol ;

						// ���̃f�[�^�𑗐M
						SendVol = WinAPIData.WinSockFunc.sendFunc( Sock->Socket, RingBuf->DataBuffer, Sock->SendComDataVol - AllSendVol, 0 ) ;
						
						// �G���[���N�����ꍇ�͈�O�ɑ��M�ł����������𑗐M�������Ƃɂ��Ē���
						if( SendVol < 0 )
						{
							// �A�h���X�̒���
							RingBuf->Start = 0 ;

							// �擾�����f�[�^�̗ʂ��Z�b�g
							SendVol = AllSendVol ;
						}
						else
						if( SendVol == Sock->SendComDataVol - AllSendVol )
						{
							// ��������S�����M�ł������̏���

							// �A�h���X�̒���
							RingBuf->Start = SendVol ;
							
							// �擾�����f�[�^�ʂ̃Z�b�g
							SendVol += AllSendVol ;
						}
					}
				}
				else
				{
					// �P��Ŋi�[����ꍇ�̏���
					
					// ��C�ɑ��M
					SendVol = WinAPIData.WinSockFunc.sendFunc( Sock->Socket , (char *)RingBuf->DataBuffer + RingBuf->Start, Sock->SendComDataVol, 0 ) ;
					if( SendVol < 0 )
						break ;

					// �A�h���X�̒���
					RingBuf->Start += SendVol ;
				}

				// ���M�o�����f�[�^�ʕ��������M�҂��������Ă���f�[�^�ʂ����炷
				RingBuf->DataLength -= SendVol ;

				// ���M�ł����f�[�^�ʕ��������񑗐M���ׂ��f�[�^�ʂ����炷
				Sock->SendComDataVol -= SendVol ;
			}

			if( NS_ProcessNetMessage() != 0 ) break ;
		
		}// �G���[���N���Ă��Ȃ��Ă܂����M���ׂ��f�[�^������ꍇ�̓��[�v
		while( SendVol != 0 && Sock->SendComDataVol == 0 && Sock->SendBufferR.DataLength != 0 );
	}

FUNCTIONEND :

	// �u���������s���v�t���O��|��
	ProcessRunFlag = FALSE ; 

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return ReturnValue ;
}



// �ڑ���Ԃ��擾����
extern int NS_GetNetWorkAcceptState( int NetHandle )
{
	SOCKETDATA * Sock ;
	int Result = -1 ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// �L���ȃn���h���ł͂Ȃ�������G���[
	if( TCPNETHCHK( NetHandle, Sock ) )
		goto ENDLABEL ;

	// �L���ȃ\�P�b�g�f�[�^�ł͂Ȃ������牽�������I��
	if( Sock->UseFlag == FALSE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x09\x67\xb9\x52\x6a\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x67\x30\x6f\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x47\x00\x65\x00\x74\x00\x4e\x00\x65\x00\x74\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x41\x00\x63\x00\x63\x00\x65\x00\x70\x00\x74\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x0a\x00\x00"/*@ L"�L���ȃl�b�g���[�N�n���h���ł͂���܂��� in GetNetWorkAcceptState\n" @*/ ) ; 
		goto ENDLABEL ;
	}

	// �d��ȃG���[���������Ă���ꍇ�͉��������I��
	if( Sock->ErrorFlag == TRUE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x53\x30\x6e\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6b\x30\x6f\x30\xa8\x30\xe9\x30\xfc\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x47\x00\x65\x00\x74\x00\x4e\x00\x65\x00\x74\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x41\x00\x63\x00\x63\x00\x65\x00\x70\x00\x74\x00\x53\x00\x74\x00\x61\x00\x74\x00\x65\x00\x68\x00\x0a\x00\x00"/*@ L"���̃l�b�g���[�N�n���h���ɂ̓G���[���������Ă��܂� in GetNetWorkAcceptStateh\n" @*/ ) ;
		goto ENDLABEL ;
	}
	
	// �����ؒf����Ă��Ă��ꂪ�܂��m�F����Ă��Ȃ�������m�F���ꂽ�t���O�𗧂Ă�
	if( Sock->PreConnectionFlag  == FALSE &&
		Sock->ConnectionFlag     == FALSE &&
		Sock->ConnectionLostFlag == FALSE )
	{
		Sock->ConnectionLostFlag = TRUE ;
	}

	// �ڑ�����Ă��邩�ǂ�����Ԃ�
	Result = Sock->ConnectionFlag == TRUE ? 1 : 0 ;

ENDLABEL :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return Result ;
}

// ��M�f�[�^�̗ʂ𓾂�
extern int NS_GetNetWorkDataLength( int NetHandle )
{
	SOCKETDATA * Sock ;
	int temp ;
	int Result = -1 ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// �����f�[�^����M�������s��
	RecvSocket( NetHandle ) ;
	SendSocket( NetHandle ) ;

	// �L���ȃn���h���ł͂Ȃ�������G���[
	if( TCPNETHCHK( NetHandle, Sock ) )
		goto ENDLABEL ;

	// �L���ȃ\�P�b�g�f�[�^�ł͂Ȃ������牽�������I��
	if( Sock->UseFlag == FALSE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x09\x67\xb9\x52\x6a\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x67\x30\x6f\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x47\x00\x65\x00\x74\x00\x4e\x00\x65\x00\x74\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x44\x00\x61\x00\x74\x00\x61\x00\x4c\x00\x65\x00\x6e\x00\x67\x00\x74\x00\x68\x00\x0a\x00\x00"/*@ L"�L���ȃl�b�g���[�N�n���h���ł͂���܂��� in GetNetWorkDataLength\n" @*/ ) ; 
		goto ENDLABEL ;
	}

	// �d��ȃG���[���������Ă���ꍇ�͉��������I��
	if( Sock->ErrorFlag == TRUE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x53\x30\x6e\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6b\x30\x6f\x30\xa8\x30\xe9\x30\xfc\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x47\x00\x65\x00\x74\x00\x4e\x00\x65\x00\x74\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x44\x00\x61\x00\x74\x00\x61\x00\x4c\x00\x65\x00\x6e\x00\x67\x00\x74\x00\x68\x00\x0a\x00\x00"/*@ L"���̃l�b�g���[�N�n���h���ɂ̓G���[���������Ă��܂� in GetNetWorkDataLength\n" @*/ ) ;
		goto ENDLABEL ;
	}

	// �c�w���C�u�����Ǝ��̕������Ƃ邩�ǂ����ŏ����𕪊�
	if( Sock->DXProtocolFlag == FALSE )
	{
		// �c�w���C�u�����Ǝ��̕����ł͂Ȃ��ꍇ��M�ʂ̎擾�͏o���Ȃ��̂�
		// �f�[�^�����Ă��邩�ǂ��������Ԃ�
		Result = NS_NetWorkRecvToPeek( NetHandle, &temp, 1 ) ;
		Result = Result < 0 ? Result : ( Result == 1 ? TRUE : FALSE ) ;
	}
	else
	{
//		// ��M�o�b�t�@�ɗ��܂��Ă���f�[�^�ʂ�Ԃ�
//		Result = RingBufGetDataLength( &Sock->RecvBufferToUserR ) ;

		// ��M�o�b�t�@�Ɏ�M���������Ă���f�[�^�ʂ�Ԃ�
		Result = Sock->RecvBufferToUserOpenSize ;
	}

ENDLABEL :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return Result ;
}

// �����M�̃f�[�^�̗ʂ𓾂� 
extern int NS_GetNetWorkSendDataLength( int NetHandle )
{
	SOCKETDATA * Sock ;
	int Result = -1 ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// �L���ȃn���h���ł͂Ȃ�������G���[
	if( TCPNETHCHK( NetHandle, Sock ) )
		goto ENDLABEL ;

	// �L���ȃ\�P�b�g�f�[�^�ł͂Ȃ������牽�������I��
	if( Sock->UseFlag == FALSE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x09\x67\xb9\x52\x6a\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x67\x30\x6f\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x47\x00\x65\x00\x74\x00\x4e\x00\x65\x00\x74\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x53\x00\x65\x00\x6e\x00\x64\x00\x44\x00\x61\x00\x74\x00\x61\x00\x4c\x00\x65\x00\x6e\x00\x67\x00\x74\x00\x68\x00\x0a\x00\x00"/*@ L"�L���ȃl�b�g���[�N�n���h���ł͂���܂��� in GetNetWorkSendDataLength\n" @*/ ) ; 
		goto ENDLABEL ;
	}

	// �d��ȃG���[���������Ă���ꍇ�͉��������I��
	if( Sock->ErrorFlag == TRUE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x53\x30\x6e\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6b\x30\x6f\x30\xa8\x30\xe9\x30\xfc\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x47\x00\x65\x00\x74\x00\x4e\x00\x65\x00\x74\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x53\x00\x65\x00\x6e\x00\x64\x00\x44\x00\x61\x00\x74\x00\x61\x00\x4c\x00\x65\x00\x6e\x00\x67\x00\x74\x00\x68\x00\x0a\x00\x00"/*@ L"���̃l�b�g���[�N�n���h���ɂ̓G���[���������Ă��܂� in GetNetWorkSendDataLength\n" @*/ ) ;
		goto ENDLABEL ;
	}

	// �c�w���C�u�����Ǝ��̕������Ƃ��Ă��邩�ǂ����ŏ����𕪊�
	if( Sock->DXProtocolFlag == FALSE )
	{
		// �c�w���C�u�����Ǝ��̕������Ƃ��Ă��Ȃ��ꍇ�����M���̃f�[�^�͒~�����Ȃ��̂ŏ�ɂO
		Result = 0 ;
	}
	else
	{
		// ���M�o�b�t�@�ɗ��܂����f�[�^�ʂ�Ԃ�
		Result = RingBufGetDataLength( &Sock->SendBufferR ) ;
	}

ENDLABEL :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return Result ;
}




// �V���ɐڑ������ʐM����𓾂�
extern int NS_GetNewAcceptNetWork( void )
{
	int Result = 0 ;
	SOCKETDATA * Sock ;
	HANDLELIST *List ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// �V���ɐڑ������ڑ��悪����ꍇ�͂��̐ڑ��n���h����Ԃ�
	for( List = HandleManageArray[ DX_HANDLETYPE_NETWORK ].ListFirst.Next ; List->Next != NULL ; List = List->Next )
	{
		Sock = (SOCKETDATA *)List->Data ;

		// �V���ɐڑ����ꂽ�n���h��������
		if( Sock->UseFlag == TRUE &&			// �n���h�����L���ł��邩����
			Sock->ConnectionFlag == TRUE &&		// ���������ڑ�����Ă��邩����
			Sock->AccepteConfirFlag == FALSE )	// �ڑ�����Ă��邱�Ƃ��܂��m�F����Ă��Ȃ�������
		{
			// ����Őڑ����ꂽ���Ƃ͊m���Ɋm�F�����̂�
			// �ڑ����ꂽ���Ƃ��m�F���ꂽ���Ƃɂ���
			Sock->AccepteConfirFlag = TRUE ;

			// ���������m�F�ڑ���̃n���h����Ԃ�
			Result = Sock->HandleInfo.Handle ;
			goto FUNCTIONEND ;
		}
	}

	// �����ɗ�����V���ɐڑ����ꂽ�ڑ���͂Ȃ��Ƃ������ƂȂ̂� -1 ��Ԃ�
	Result = -1 ;

FUNCTIONEND :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return Result ;
}

// �ڑ���ؒf���ꂽ�ʐM����𓾂�
extern int NS_GetLostNetWork( void )
{
	int ReturnValue = 0 ;
	SOCKETDATA * Sock ;
	HANDLELIST *List ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// �܂����C�u�������[�U�[�����m�F�̐ؒf���ꂽ�ڑ����T��
	for( List = HandleManageArray[ DX_HANDLETYPE_NETWORK ].ListFirst.Next ; List->Next != NULL ; List = List->Next )
	{
		Sock = (SOCKETDATA *)List->Data ;

		// ����
		if( Sock->UseFlag == TRUE &&								// �f�[�^�͗L���ł��邩�̔���
			Sock->HandleInfo.Handle  != SockData.ListenHandle &&	// �ڑ��҂��\�P�b�g�ł͂Ȃ�������
			Sock->PreConnectionFlag  == FALSE &&					// �ڑ��O�ł͂Ȃ�������
			Sock->ConnectionFlag     == FALSE &&					// �ڑ���ؒf����Ă��邩�̔���
			Sock->ConnectionLostFlag == FALSE )						// �ؒf���ꂽ���Ƃ��܂��m�F����Ă��Ȃ����̔���
		{
			// �ؒf���m�F�����t���O�𗧂Ă�
			Sock->ConnectionLostFlag = TRUE ;

			// ���炽�Ɋm�F���ꂽ�ڑ��n���h����Ԃ�
			ReturnValue = Sock->HandleInfo.Handle ;
			goto FUNCTIONEND ;
		}
	}

	// �����ɂ�����V���ɐؒf���ꂽ�ڑ��͂Ȃ������ƌ������ƂȂ̂� -1 ��Ԃ�
	ReturnValue = -1 ;

FUNCTIONEND :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return ReturnValue ;
}

// �ڑ���̂h�o�𓾂�( �ؒf����Ă���ڑ���ł� OK )
extern int NS_GetNetWorkIP_IPv6( int NetHandle , IPDATA_IPv6 *IpBuf )
{
	SOCKETDATA * Sock ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �L���ȃn���h���ł͂Ȃ�������G���[
	if( TCPNETHCHK( NetHandle, Sock ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// IPv6 �ł͖���������G���[
	if( Sock->IsIPv6 == FALSE )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �h�o��������
	*IpBuf = Sock->AcceptedIP_IPv6 ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return 0 ;
}

// �ڑ���̂h�o�𓾂�( �ؒf����Ă���ڑ���ł� OK )
extern int NS_GetNetWorkIP( int NetHandle , IPDATA *IpBuf )
{
	SOCKETDATA * Sock ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �L���ȃn���h���ł͂Ȃ�������G���[
	if( TCPNETHCHK( NetHandle, Sock ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// IPv4 �ł͖���������G���[
	if( Sock->IsIPv6 == TRUE )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �h�o��������
	*IpBuf = Sock->AcceptedIP ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return 0 ;
}

// �����̂h�o�𓾂�
extern int NS_GetMyIPAddress( IPDATA *IpBuf, int IpBufLength, int *IpNum )
{
	// �ʐM�֌W�̏��������܂��s���Ă��Ȃ��ꍇ�͏���������
	if( !SockData.InitializeFlag )
	{
		if( WinData.CloseMessagePostFlag != TRUE ) InitializeNetWork() ;
		else										return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �h�o����������
	if( SockData.MyIP != NULL && IpBuf != NULL )
	{
		int i ;

		for( i = 0 ; i < IpBufLength && i < SockData.MyIPNum ; i ++ )
		{
			IpBuf[ i ] = SockData.MyIP[ i ] ;
		}
	}

	// �h�o�A�h���X�̐����Z�b�g����
	if( IpNum != NULL )
	{
		*IpNum = SockData.MyIPNum ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return 0 ;
}

// �ڑ��̃^�C���A�E�g�܂ł̎��Ԃ�ݒ肷��
extern int NS_SetConnectTimeOutWait( int Time )
{
	// �N���e�B�J���Z�N�V�����̎擾
	if( SockData.InitializeFlag )
	{
		CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
	}

	SockData.TimeOutWait = Time ;

	// �N���e�B�J���Z�N�V�����̉��
	if( DxSysData.DxLib_InitializeFlag == TRUE )
	{
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
	}

	// �I��
	return 0 ;
}

// �c�w���C�u�����̒ʐM�`�Ԃ��g�����ǂ������Z�b�g����
extern int NS_SetUseDXNetWorkProtocol( int Flag )
{
	// �N���e�B�J���Z�N�V�����̎擾
	if( SockData.InitializeFlag )
	{
		CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
	}

	SockData.NonUseDXProtocolFlag = !Flag ;

	// �N���e�B�J���Z�N�V�����̉��
	if( SockData.InitializeFlag )
	{
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
	}

	// �I��
	return 0 ;
}

// �c�w���C�u�����̒ʐM�`�Ԃ��g�����ǂ������擾����
extern int NS_GetUseDXNetWorkProtocol( void )
{
	return SockData.NonUseDXProtocolFlag ;
}

// SetUseDXNetWorkProtocol �̕ʖ�
extern int NS_SetUseDXProtocol( int Flag )
{
	return NS_SetUseDXNetWorkProtocol( Flag ) ;
}

// GetUseDXNetWorkProtocol �̕ʖ�
extern int NS_GetUseDXProtocol( void )
{
	return NS_GetUseDXNetWorkProtocol() ;
}


// �ڑ����ؒf���ꂽ����ɐڑ��n���h����������邩�ǂ����̃t���O���Z�b�g����
extern int NS_SetNetWorkCloseAfterLostFlag( int Flag )
{
	// �N���e�B�J���Z�N�V�����̎擾
	if( SockData.InitializeFlag )
	{
		CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
	}

	// �t���O��ۑ�����
	SockData.NonCloseAfterLostFlag = !Flag ;

	// �N���e�B�J���Z�N�V�����̉��
	if( SockData.InitializeFlag )
	{
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
	}

	// �I��
	return 0 ;
}

// �ڑ����ؒf���ꂽ����ɐڑ��n���h����������邩�ǂ����̃t���O���擾����
extern int NS_GetNetWorkCloseAfterLostFlag( void )
{
	// �t���O��Ԃ�
	return SockData.NonCloseAfterLostFlag ;
}

/*
// �h�d�Őݒ肳��Ă���v���L�V���g�p���邩�ǂ����̃t���O���Z�b�g����
extern	int			SetUseIEProxySettingFlag( int Flag )
{
	HttpData.NonUseIEProxy = !Flag ;
	
	return 0 ;
}

// �h�d�Őݒ肳��Ă���v���L�V���g�p���邩�ǂ����̃t���O���擾����
extern	int			GetUseIEProxySettingFlag( void )
{
	return !HttpData.NonUseIEProxy ;
}

// �g�s�s�o�ʐM�Ŏg�p����v���L�V�ݒ���s��
extern int NS_SetProxySetting( int UseFlag, const char *Address, int Port )
{
	HttpData.UseProxy = UseFlag ;
	_STRCPY( HttpData.ProxyHost, Address ) ;
	HttpData.ProxyPort = Port == -1 ? 80 : Port ;
	
	return 0 ;
}

// �g�s�s�o�ʐM�Ŏg�p����v���L�V�ݒ���擾����
extern int NS_GetProxySetting( int *UseFlagBuffer, char *AddressBuffer, int *PortBuffer )
{
	if( UseFlagBuffer != NULL ) *UseFlagBuffer = HttpData.UseProxy ;
	if( AddressBuffer != NULL ) _STRCPY( AddressBuffer, HttpData.ProxyHost ) ;
	if( PortBuffer != NULL ) *PortBuffer = HttpData.ProxyPort ;

	return 0 ;
}

// �h�d�̃v���L�V�ݒ��K������
extern int NS_SetIEProxySetting( void ) 
{
	char str[256] ;
	DWORD size ;
	DWORD type ;
	char *p ;
	DWORD hr ;
	HKEY key ;
	int OpenFlag ;

	// ���W�X�g������v���L�V�̏��𓾂�
	_MEMSET( str, 0, 256 ) ;
	type = REG_SZ ;
	size = 256 ;
	OpenFlag = FALSE ;
	
	// ���W�X�g�����J��
	hr = RegOpenKeyEx( HKEY_CURRENT_USER, IEPROXY, 0, KEY_ALL_ACCESS, &key ) ; 
	if( hr != ERROR_SUCCESS ) goto ERR ;
	OpenFlag = TRUE ;

	// ���W�X�g�����i�[����̂ɕK�v�ȃf�[�^�̃T�C�Y�𓾂�
	hr = RegQueryValueEx( key, "ProxyServer", NULL, &type, NULL, &size ) ;
	if( hr != ERROR_SUCCESS ) goto ERR ;

	// ���W�X�g���̓��e�𓾂�
	hr = RegQueryValueEx( key, "ProxyServer", NULL, &type, ( unsigned char * )str, &size ) ;
	if( hr != ERROR_SUCCESS ) goto ERR ;

	// ���W�X�g�������
	RegCloseKey( key ) ;
	OpenFlag = FALSE ;
						
	// �g�p���Ă���v���L�V�T�[�o�[�̕��������͂���
	p = _STRCHR( str, ':' ) ;
	_STRCPY( HttpData.ProxyHost, str ) ;
	if( p == NULL )
	{
		HttpData.ProxyPort = 80 ;
	}
	else
	{
		HttpData.ProxyHost[p-str] = '\0' ;
		HttpData.ProxyPort = _ATOI( p + 1 ) ;
		if( HttpData.ProxyPort <= 0 ) HttpData.ProxyPort = -1 ;
	}

	// �I��
	return 0 ;
	
	// �G���[����
ERR :
	if( OpenFlag == TRUE ) 	RegCloseKey( key ) ;

	return -1 ;
}
*/

// NetWorkRecv �̎������֐�
static int NetWorkRecv_Static(
	int NetHandle,
	void *Buffer,
	int Length,
	int Peek, 
	int ASyncThread
)
{
	int ReturnValue = 0 ;
	SOCKETDATA * Sock ;

	// �擾����f�[�^���̒l���s���ȏꍇ�͉��������I��
	if( Length < 0 ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( ASyncThread )
	{
		if( TCPNETHCHK_ASYNC( NetHandle, Sock ) )
		{
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
			return -1 ;
		}
	}
	else
	{
		if( TCPNETHCHK( NetHandle, Sock ) )
		{
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
			return -1 ;
		}
	}

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// �L���ȃ\�P�b�g�f�[�^�ł͂Ȃ������牽�������I��
	if( Sock->UseFlag == FALSE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x09\x67\xb9\x52\x6a\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x67\x30\x6f\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x4e\x00\x65\x00\x74\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x52\x00\x65\x00\x63\x00\x76\x00\x0a\x00\x00"/*@ L"�L���ȃl�b�g���[�N�n���h���ł͂���܂��� in NetWorkRecv\n" @*/ ) ; 
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �d��ȃG���[���������Ă���ꍇ�͉��������I��
	if( Sock->ErrorFlag == TRUE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x53\x30\x6e\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6b\x30\x6f\x30\xa8\x30\xe9\x30\xfc\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x4e\x00\x65\x00\x74\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x52\x00\x65\x00\x63\x00\x76\x00\x0a\x00\x00"/*@ L"���̃l�b�g���[�N�n���h���ɂ̓G���[���������Ă��܂� in NetWorkRecv\n" @*/ ) ;
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// ������M�f�[�^�擾�������s��
	if( RecvSocket( NetHandle ) < 0 )
	{
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �c�w���C�u�����Ǝ��̕������g�p���邩�ǂ����ŏ����𕪊�
	if( Sock->DXProtocolFlag == FALSE )
	{
		// WinSock ���璼�ړǂݍ���
		int RecvVol ;

		// ��M�ł���ꍇ�͎�M�A��M�����f�[�^�ʂ�Ԃ�
		RecvVol = WinAPIData.WinSockFunc.recvFunc( Sock->Socket, (char *)Buffer, Length, Peek ? MSG_PEEK : 0 ) ;

		// �T�C�Y��Ԃ�
		ReturnValue = RecvVol ;
		goto FUNCTIONEND ;
	}
	else
	{
		// ��M�����f�[�^�e�ʂ��v���e�ʂ������Ȃ��ꍇ�͎��s
		if( RingBufGetDataLength( &Sock->RecvBufferToUserR ) < Length )
		{
			ReturnValue = -1 ;
			goto FUNCTIONEND ;
		}

		// �f�[�^�̎擾
		if( Peek )
		{
			RingBufDataGet( &Sock->RecvBufferToUserR, Buffer, Length, TRUE ) ;
		}
		else
		{
			RingBufDataGet( &Sock->RecvBufferToUserR, Buffer, Length, FALSE ) ;
			Sock->RecvBufferToUserOpenSize -= Length ;
		}

		// ������M�f�[�^�擾�������s��
		RecvSocket( NetHandle ) ;
	}

FUNCTIONEND :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return ReturnValue ;
}

#ifndef DX_NON_ASYNCLOAD
// NetWorkRecv �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void NetWorkRecv_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	SOCKETDATA * Sock ;
	int NetHandle ;
	void *Buffer ;
	int Length ;
	int Peek ;
	int Addr ;

	Addr = 0 ;
	NetHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	Buffer = GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	Length = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	Peek = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	if( !NETHCHK_ASYNC( NetHandle, Sock ) )
	{
		Sock->HandleInfo.ASyncLoadResult = NetWorkRecv_Static( NetHandle, Buffer, Length, Peek, TRUE ) ;
	}

	DecASyncLoadCount( NetHandle ) ;
}
#endif // DX_NON_ASYNCLOAD

// NetWorkRecv �̃O���[�o���f�[�^�ɃA�N�Z�X���Ȃ��o�[�W����
extern int NetWorkRecv_UseGParam(
	int NetHandle,
	void *Buffer,
	int Length,
	int Peek, 
	int ASyncLoadFlag
)
{
	int Result = -1 ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	if( WinData.ActiveFlag == FALSE )
		DxActiveWait() ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt( NULL, &Addr, NetHandle ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, Buffer ) ;
		AddASyncLoadParamInt( NULL, &Addr, Length ) ;
		AddASyncLoadParamInt( NULL, &Addr, Peek ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto END ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = NetWorkRecv_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt( AParam->Data, &Addr, NetHandle ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, Buffer ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, Length ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, Peek ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto END ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( NetHandle, AParam->Index ) ;

		Result = 0 ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		Result = NetWorkRecv_Static( NetHandle, Buffer, Length, Peek, FALSE ) ;
	}

#ifndef DX_NON_ASYNCLOAD
END :
#endif

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return Result ;
}

// ��M�����f�[�^��ǂݍ���
extern int NS_NetWorkRecv( int NetHandle, void *Buffer, int Length )
{
	return NetWorkRecv_UseGParam( NetHandle, Buffer, Length, FALSE, GetASyncLoadFlag() ) ;
/*
	int ReturnValue = 0 ;
	SOCKETDATA * Sock ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �擾����f�[�^���̒l���s���ȏꍇ�͉��������I��
	if( Length < 0 ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( TCPNETHCHK( NetHandle, Sock ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// �L���ȃ\�P�b�g�f�[�^�ł͂Ȃ������牽�������I��
	if( Sock->UseFlag == FALSE )
	{
		DXST_ERRORLOG_ADDUTF16LE( L"�L���ȃl�b�g���[�N�n���h���ł͂���܂��� in NetWorkRecv\n" ) ; 
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �d��ȃG���[���������Ă���ꍇ�͉��������I��
	if( Sock->ErrorFlag == TRUE )
	{
		DXST_ERRORLOG_ADDUTF16LE( L"���̃l�b�g���[�N�n���h���ɂ̓G���[���������Ă��܂� in NetWorkRecv\n" ) ;
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// ������M�f�[�^�擾�������s��
	if( RecvSocket( NetHandle ) < 0 )
	{
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �c�w���C�u�����Ǝ��̕������g�p���邩�ǂ����ŏ����𕪊�
	if( Sock->DXProtocolFlag == FALSE )
	{
		// WinSock ���璼�ړǂݍ���
		int RecvVol ;

		// ��M�ł���ꍇ�͎�M�A��M�����f�[�^�ʂ�Ԃ�
		RecvVol = WinAPIData.WinSockFunc.recvFunc( Sock->Socket, (char *)Buffer, Length, 0 ) ;

		// �T�C�Y��Ԃ�
		ReturnValue = RecvVol ;
		goto FUNCTIONEND ;
	}
	else
	{
		// ��M�����f�[�^�e�ʂ��v���e�ʂ������Ȃ��ꍇ�͎��s
		if( RingBufGetDataLength( &Sock->RecvBufferToUserR ) < Length )
		{
			ReturnValue = -1 ;
			goto FUNCTIONEND ;
		}

		// �f�[�^�̎擾
		RingBufDataGet( &Sock->RecvBufferToUserR, Buffer, Length, FALSE ) ;
		Sock->RecvBufferToUserOpenSize -= Length ;

		// ������M�f�[�^�擾�������s��
		RecvSocket( NetHandle ) ;
	}

FUNCTIONEND :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return ReturnValue ;
*/
}

// ��M�����f�[�^��ǂݍ��ށA�ǂݍ��񂾃f�[�^�̓o�b�t�@����폜����Ȃ�
extern int NS_NetWorkRecvToPeek( int NetHandle , void *Buffer , int Length )
{
	return NetWorkRecv_UseGParam( NetHandle, Buffer, Length, TRUE, GetASyncLoadFlag() ) ;
/*
	int ReturnValue = 0 ;
	SOCKETDATA * Sock ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( TCPNETHCHK( NetHandle, Sock ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// �L���ȃ\�P�b�g�f�[�^�ł͂Ȃ������牽�������I��
	if( Sock->UseFlag == FALSE )
	{
		DXST_ERRORLOG_ADDUTF16LE( L"�L���ȃl�b�g���[�N�n���h���ł͂���܂��� in NetWorkRecvToPeek\n" ) ; 
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �d��ȃG���[���������Ă���ꍇ�͉��������I��
	if( Sock->ErrorFlag == TRUE )
	{
		DXST_ERRORLOG_ADDUTF16LE( L"���̃l�b�g���[�N�n���h���ɂ̓G���[���������Ă��܂� in NetWorkRecvToPeek\n" ) ;
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// ������M�f�[�^�擾�������s��
	if( RecvSocket( NetHandle ) < 0 )
	{
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}
	
	// �c�w���C�u�����Ǝ��̕������ǂ����ŏ����𕪊�
	if( Sock->DXProtocolFlag == FALSE )
	{
		int RecvVol ;

		// ��M�ł���ꍇ�͎�M�A��M�����f�[�^�ʂ�Ԃ�
		RecvVol = WinAPIData.WinSockFunc.recvFunc( Sock->Socket, (char *)Buffer, Length, MSG_PEEK ) ;

		// �T�C�Y��Ԃ�
		ReturnValue = RecvVol ;
		goto FUNCTIONEND ;
	}
	else
	{
		// ��M�����f�[�^�e�ʂ��v���e�ʂ������Ȃ��ꍇ�͎��s
		if( RingBufGetDataLength( &Sock->RecvBufferToUserR ) < Length )
		{
			ReturnValue = -1 ;
			goto FUNCTIONEND ;
		}

		// �f�[�^�̎擾
		RingBufDataGet( &Sock->RecvBufferToUserR, Buffer, Length, TRUE ) ;

		// ������M�f�[�^�擾�������s��
		RecvSocket( NetHandle ) ;
	}

FUNCTIONEND :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return ReturnValue ;
*/
}

// ��M�����f�[�^���N���A����
extern int NS_NetWorkRecvBufferClear( int NetHandle )
{
	int ReturnValue = 0 ;
	SOCKETDATA * Sock ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( TCPNETHCHK( NetHandle, Sock ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// �L���ȃ\�P�b�g�f�[�^�ł͂Ȃ������牽�������I��
	if( Sock->UseFlag == FALSE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x09\x67\xb9\x52\x6a\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x67\x30\x6f\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x4e\x00\x65\x00\x74\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x52\x00\x65\x00\x63\x00\x76\x00\x42\x00\x75\x00\x66\x00\x66\x00\x65\x00\x72\x00\x43\x00\x6c\x00\x65\x00\x61\x00\x72\x00\x0a\x00\x00"/*@ L"�L���ȃl�b�g���[�N�n���h���ł͂���܂��� in NetWorkRecvBufferClear\n" @*/ ) ; 
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �d��ȃG���[���������Ă���ꍇ�͉��������I��
	if( Sock->ErrorFlag == TRUE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x53\x30\x6e\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6b\x30\x6f\x30\xa8\x30\xe9\x30\xfc\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x4e\x00\x65\x00\x74\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x52\x00\x65\x00\x63\x00\x76\x00\x42\x00\x75\x00\x66\x00\x66\x00\x65\x00\x72\x00\x43\x00\x6c\x00\x65\x00\x61\x00\x72\x00\x0a\x00\x00"/*@ L"���̃l�b�g���[�N�n���h���ɂ̓G���[���������Ă��܂� in NetWorkRecvBufferClear\n" @*/ ) ;
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// ������M�f�[�^�擾�������s��
	if( RecvSocket( NetHandle ) < 0 )
	{
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}
	
	// �c�w���C�u�����Ǝ��̕������ǂ����ŏ����𕪊�
	if( Sock->DXProtocolFlag == FALSE )
	{
		void *Buffer;

		// �ꎞ�I�Ȏ�M�o�b�t�@���m�ۂ���
		Buffer = DXALLOC( 256 * 1024 ) ;

		// ��M�ł����T�C�Y���o�b�t�@��菬�����Ȃ�܂ŌJ��Ԃ�
		while( WinAPIData.WinSockFunc.recvFunc( Sock->Socket, (char *)Buffer, 256 * 1024, 0 ) < 256 * 1024 ){}

		// �ꎞ�I�Ȏ�M�o�b�t�@���J������
		DXFREE( Buffer ) ;
	}
	else
	{
		// �����O�o�b�t�@���ď���������
		RingBufTerminate( &Sock->RecvBufferToUserR );
		RingBufInitialize( &Sock->RecvBufferToUserR );
		Sock->RecvComDataOriginalVol = 0 ;
		Sock->RecvBufferToUserOpenSize = 0 ;
	}

FUNCTIONEND :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return ReturnValue ;
}

// NetWorkSend �̎������֐�
static int NetWorkSend_Static(
	int NetHandle,
	const void *Buffer,
	int Length,
	int ASyncThread
)
{
	int ReturnValue = 0 ;
	SOCKETDATA * Sock ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( ASyncThread )
	{
		if( TCPNETHCHK_ASYNC( NetHandle, Sock ) )
		{
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
			return -1 ;
		}
	}
	else
	{
		if( TCPNETHCHK( NetHandle, Sock ) )
		{
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
			return -1 ;
		}
	}

	// �ʐM�֌W����
	NS_ProcessNetMessage() ;

	// �L���ȃ\�P�b�g�f�[�^�ł͂Ȃ������牽�������I��
	if( Sock->UseFlag == FALSE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x09\x67\xb9\x52\x6a\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x67\x30\x6f\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x4e\x00\x65\x00\x74\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x53\x00\x65\x00\x6e\x00\x64\x00\x0a\x00\x00"/*@ L"�L���ȃl�b�g���[�N�n���h���ł͂���܂��� in NetWorkSend\n" @*/ ) ; 
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �d��ȃG���[���������Ă���ꍇ�͉��������I��
	if( Sock->ErrorFlag == TRUE )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x53\x30\x6e\x30\xcd\x30\xc3\x30\xc8\x30\xef\x30\xfc\x30\xaf\x30\xcf\x30\xf3\x30\xc9\x30\xeb\x30\x6b\x30\x6f\x30\xa8\x30\xe9\x30\xfc\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x66\x30\x44\x30\x7e\x30\x59\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x4e\x00\x65\x00\x74\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x53\x00\x65\x00\x6e\x00\x64\x00\x0a\x00\x00"/*@ L"���̃l�b�g���[�N�n���h���ɂ̓G���[���������Ă��܂� in NetWorkSend\n" @*/ ) ;
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}
	
	// �ڑ����f����Ă���ꍇ�����������I��
	if( Sock->ConnectionFlag == FALSE )
	{
		ReturnValue = -1 ;
		goto FUNCTIONEND ;
	}

	// �������M�v���Z�X�����s����
	SendSocket( NetHandle ) ;

	// �c�w���C�u�����Ǝ��̕������ǂ����ŏ����𕪊�
	if( Sock->DXProtocolFlag == FALSE )
	{
		int SendVol ;
	
		SendVol = WinAPIData.WinSockFunc.sendFunc( Sock->Socket, (char *)Buffer, Length, 0 ) ;

		// �I��
		ReturnValue = SendVol ;
		goto FUNCTIONEND ;
	}
	else
	{
		// ���M�o�b�t�@�Ƀf�[�^�ǉ�
		if( RingBufDataAdd( &Sock->SendBufferR, Buffer, Length ) < 0 )
		{
			DXST_ERRORLOGFMT_ADDUTF16LE(( "\x01\x90\xe1\x4f\xc7\x30\xfc\x30\xbf\x30\x20\x00\x25\x00\x64\x00\x20\x00\x62\x00\x79\x00\x74\x00\x65\x00\x20\x00\x6e\x30\xfd\x8f\xa0\x52\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x69\x00\x6e\x00\x20\x00\x4e\x00\x65\x00\x77\x00\x57\x00\x6f\x00\x72\x00\x6b\x00\x53\x00\x65\x00\x6e\x00\x64\x00\x00"/*@ L"���M�f�[�^ %d byte �̒ǉ��Ɏ��s���܂��� in NewWorkSend" @*/, Length )) ;
			ReturnValue = -1 ;
			goto FUNCTIONEND ;
		}

		// �������M�v���Z�X�����s����
		SendSocket( NetHandle ) ;
	}

FUNCTIONEND :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return ReturnValue ;
}

#ifndef DX_NON_ASYNCLOAD
// NetWorkSend �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void NetWorkSend_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	SOCKETDATA * Sock ;
	int NetHandle ;
	void *Buffer ;
	int Length ;
	int Addr ;

	Addr = 0 ;
	NetHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	Buffer = GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	Length = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	if( !NETHCHK_ASYNC( NetHandle, Sock ) )
	{
		Sock->HandleInfo.ASyncLoadResult = NetWorkSend_Static( NetHandle, Buffer, Length, TRUE ) ;
	}

	DecASyncLoadCount( NetHandle ) ;
}
#endif // DX_NON_ASYNCLOAD

// NetWorkSend �̃O���[�o���f�[�^�ɃA�N�Z�X���Ȃ��o�[�W����
extern int NetWorkSend_UseGParam(
	int NetHandle,
	const void *Buffer,
	int Length,
	int ASyncLoadFlag
)
{
	int Result = -1 ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	if( WinData.ActiveFlag == FALSE )
		DxActiveWait() ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt( NULL, &Addr, NetHandle ) ;
		AddASyncLoadParamStruct( NULL, &Addr, Buffer, Length ) ;
		AddASyncLoadParamInt( NULL, &Addr, Length ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto END ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = NetWorkSend_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt( AParam->Data, &Addr, NetHandle ) ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, Buffer, Length ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, Length ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto END ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( NetHandle, AParam->Index ) ;

		Result = 0 ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		Result = NetWorkSend_Static( NetHandle, Buffer, Length, FALSE ) ;
	}

#ifndef DX_NON_ASYNCLOAD
END :
#endif

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return Result ;
}

// �f�[�^�𑗐M����
extern int NS_NetWorkSend( int NetHandle, const void *Buffer, int Length )
{
	return NetWorkSend_UseGParam( NetHandle, Buffer, Length, GetASyncLoadFlag() ) ;
}

// MakeUDPSocketBase �̎������֐�
static int MakeUDPSocketBase_Static(
	int NetHandle,
	int IsIPv6,
	int RecvPort,
	int ASyncThread
)
{
	int ReturnValue = 0 ;
	SOCKETDATA *Sock ;
	unsigned int pt ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	if( ASyncThread )
	{
		if( NETHCHK_ASYNC( NetHandle, Sock) )
			return -1 ;
	}
	else
	{
		if( NETHCHK( NetHandle, Sock) )
			return -1 ;
	}

	// �|�[�g�̒l������
	pt = ( unsigned int )( RecvPort == -1 ? -1 : WinAPIData.WinSockFunc.htonsFunc( ( unsigned short )RecvPort ) ) ;

	// �f�[�^��������
	Sock->IsUDP					= TRUE ;							// �t�c�o���g�p����
	Sock->IsIPv6				= IsIPv6 ? TRUE : FALSE ;			// IPv6���g�p���邩�ǂ�����ۑ�����
	Sock->Port 					= pt ;								// �ڑ���̃|�[�g��ۑ�
	Sock->UDPReadFlag			= FALSE ;							// ��M�f�[�^�����݂��邩�A�t���O��|��
	Sock->UDPWriteFlag			= TRUE ;							// ���M�\�ȏ�Ԃ��A�t���O�𗧂Ă�

	// �\�P�b�g�̍쐬
	Sock->Socket = WinAPIData.WinSockFunc.socketFunc( IsIPv6 ? 23/*AF_INET6 �� 23*/ : AF_INET , SOCK_DGRAM , IPPROTO_UDP ) ;
	if( Sock->Socket == INVALID_SOCKET )
	{
		DXERRORNETWORK(( "\x77\x00\x73\x00\x32\x00\x5f\x00\x33\x00\x32\x00\x2e\x00\x64\x00\x6c\x00\x6c\x00\x4c\x30\xd5\x52\x5c\x4f\x57\x30\x66\x30\x44\x30\x7e\x30\x5b\x30\x93\x30\x01\x30\xbd\x30\xb1\x30\xc3\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x5f\x00\x31\x00\x00"/*@ L"ws2_32.dll�����삵�Ă��܂���A�\�P�b�g�̍쐬�Ɏ��s���܂���_1" @*/ )) ;
		goto ERR ;
	}

	// RecvPort �� -1 �ȊO�̏ꍇ�͍쐬�����\�P�b�g�ƃ|�[�g�ԍ��̊֘A�t�����s��
	if( RecvPort != -1 )
	{
		if( IsIPv6 )
		{
			_sockaddr_in6 local_v6;
		
			_MEMSET( &local_v6, 0, sizeof( local_v6 ) ) ;
			local_v6.sin6_family = 23/*AF_INET6 �� 23*/;
			local_v6.sin6_port = ( unsigned short )pt ;

			if( WinAPIData.WinSockFunc.bindFunc( Sock->Socket , (struct sockaddr FAR *) &local_v6, sizeof( local_v6 ) ) == SOCKET_ERROR )
			{
				if( WinAPIData.WinSockFunc.WSAGetLastErrorFunc() != WSAEWOULDBLOCK )
				{
					DXERRORNETWORK(( "\x62\x00\x69\x00\x6e\x00\x64\x00\xfa\x51\x65\x67\x7e\x30\x5b\x30\x93\x30\x00"/*@ L"bind�o���܂���" @*/ )) ;
					goto ERR ;
				}
			}
		}
		else
		{
			SOCKADDR_IN	local;
		
			_MEMSET( &local, 0, sizeof( local ) ) ;
			local.sin_family = AF_INET;
			local.sin_port = ( u_short )pt ;
			local.sin_addr.s_addr = INADDR_ANY;

			if( WinAPIData.WinSockFunc.bindFunc( Sock->Socket , (struct sockaddr FAR *) &local, sizeof( local ) ) == SOCKET_ERROR )
			{
				if( WinAPIData.WinSockFunc.WSAGetLastErrorFunc() != WSAEWOULDBLOCK )
				{
					DXERRORNETWORK(( "\x62\x00\x69\x00\x6e\x00\x64\x00\xfa\x51\x65\x67\x7e\x30\x5b\x30\x93\x30\x00"/*@ L"bind�o���܂���" @*/ )) ;
					goto ERR ;
				}
			}
		}
	}

	// WinSock ���b�Z�[�W�󂯎��ݒ�
	if( WinAPIData.WinSockFunc.WSAAsyncSelectFunc(
			Sock->Socket,
			SockData.MessageWindow,
			WSA_WINSOCKMESSAGE,
			FD_WRITE | FD_READ ) == SOCKET_ERROR )
	{
		DXERRORNETWORK(( "\x57\x00\x53\x00\x41\x00\x41\x00\x73\x00\x79\x00\x6e\x00\x63\x00\x53\x00\x65\x00\x6c\x00\x65\x00\x63\x00\x74\x00\x67\x30\xa8\x30\xe9\x30\xfc\x30\x4c\x30\x7a\x76\x1f\x75\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"WSAAsyncSelect�ŃG���[���������܂���" @*/ )) ;
		goto ERR ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	return ReturnValue ;

	// �G���[����	
ERR :
	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	return -1 ;
}

#ifndef DX_NON_ASYNCLOAD
// MakeUDPSocketBase �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void MakeUDPSocketBase_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	int NetHandle ;
	int IsIPv6 ;
	int RecvPort ;
	int Addr ;
	int Result ;

	Addr = 0 ;
	NetHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	IsIPv6 = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	RecvPort = GetASyncLoadParamInt( AParam->Data, &Addr ) ;

	Result = MakeUDPSocketBase_Static( NetHandle, IsIPv6, RecvPort, TRUE ) ;

	DecASyncLoadCount( NetHandle ) ;
	if( Result < 0 )
	{
		SubHandle( NetHandle ) ;
	}
}
#endif // DX_NON_ASYNCLOAD

// MakeUDPSocketBase �̃O���[�o���f�[�^�ɃA�N�Z�X���Ȃ��o�[�W����
extern int MakeUDPSocketBase_UseGParam(
	int IsIPv6,
	int RecvPort,
	int ASyncLoadFlag
)
{
	int NetHandle ;

	// �ʐM�֌W������������Ă��Ȃ������珉����
	if( SockData.InitializeFlag == FALSE )
	{
		if( WinData.CloseMessagePostFlag != TRUE )	InitializeNetWork() ;
		else										return -1 ;
	}
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	if( WinData.ActiveFlag == FALSE )
		DxActiveWait() ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �n���h���̍쐬
	NetHandle = AddHandle( DX_HANDLETYPE_NETWORK, FALSE, -1 ) ;
	if( NetHandle == -1 )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt( NULL, &Addr, NetHandle ) ;
		AddASyncLoadParamInt( NULL, &Addr, IsIPv6 ) ;
		AddASyncLoadParamInt( NULL, &Addr, RecvPort ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto ERR ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = MakeUDPSocketBase_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt( AParam->Data, &Addr, NetHandle ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, IsIPv6 ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, RecvPort ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto ERR ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( NetHandle, AParam->Index ) ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		if( MakeUDPSocketBase_Static( NetHandle, IsIPv6, RecvPort, FALSE ) < 0 )
			goto ERR ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return NetHandle ;

ERR :
	SubHandle( NetHandle ) ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	return -1 ;
}

// UDP���g�p�����ʐM���s���\�P�b�g�n���h�����쐬����( RecvPort �� -1 �ɂ���Ƒ��M��p�̃\�P�b�g�n���h���ɂȂ�܂� )
static int MakeUDPSocketBase( int IsIPv6, int RecvPort )
{
	return MakeUDPSocketBase_UseGParam( IsIPv6, RecvPort, GetASyncLoadFlag() ) ;
}

// UDP���g�p�����ʐM���s���\�P�b�g�n���h�����쐬����( RecvPort �� -1 �ɂ���Ƒ��M��p�̃\�P�b�g�n���h���ɂȂ�܂� )
extern int NS_MakeUDPSocket( int RecvPort )
{
	return MakeUDPSocketBase( FALSE, RecvPort ) ;
}

// UDP���g�p�����ʐM���s���\�P�b�g�n���h�����쐬����( RecvPort �� -1 �ɂ���Ƒ��M��p�̃\�P�b�g�n���h���ɂȂ�܂� )( IPv6�� )
extern int NS_MakeUDPSocket_IPv6( int RecvPort )
{
	return MakeUDPSocketBase( TRUE, RecvPort ) ;
}

// UDP���g�p�����ʐM���s���\�P�b�g�n���h�����폜����
extern int NS_DeleteUDPSocket( int NetUDPHandle )
{
	return SubHandle( NetUDPHandle ) ;
}

// NetWorkSendUDP �̎������֐�
static int NetWorkSendUDP_Static(
	int NetUDPHandle,
	IPDATA SendIP_IPv4,
	IPDATA_IPv6 SendIP_IPv6,
	int SendPort,
	const void *Buffer,
	int Length,
	int ASyncThread
)
{
	SOCKETDATA * Sock = NULL ;
	int SendVol ;
	int Result ;
	int i ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( ( ASyncThread          && UDPNETHCHK_ASYNC( NetUDPHandle, Sock ) ) ||
		( ASyncThread == FALSE && UDPNETHCHK_ASYNC( NetUDPHandle, Sock ) ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}
	if( SendPort == -1 && Sock->Port == -1 )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �f�[�^�̑��M
	if( Sock->IsIPv6 )
	{
		_sockaddr_in6 addr ;

		_MEMSET( &addr, 0, sizeof( addr ) ) ;
		addr.sin6_family = 23/*AF_INET6 �� 23*/ ;
		for( i = 0 ; i < 8 ; i ++ )
			addr.sin6_addr.Word[ i ] = SendIP_IPv6.Word[ i ] ;
		addr.sin6_port = SendPort == -1 ? ( unsigned short )Sock->Port : WinAPIData.WinSockFunc.htonsFunc( ( unsigned short )SendPort ) ;
		SendVol = WinAPIData.WinSockFunc.sendtoFunc( Sock->Socket, (char *)Buffer, Length, 0, ( struct sockaddr FAR * )&addr, sizeof( addr ) ) ;
	}
	else
	{
		SOCKADDR_IN addr ;

		_MEMSET( &addr, 0, sizeof( addr ) ) ;
		addr.sin_family = AF_INET ;
		addr.sin_addr = *(( in_addr *)&SendIP_IPv4 ) ;
		addr.sin_port = SendPort == -1 ? ( u_short )Sock->Port : WinAPIData.WinSockFunc.htonsFunc( ( unsigned short )SendPort ) ;
		SendVol = WinAPIData.WinSockFunc.sendtoFunc( Sock->Socket, (char *)Buffer, Length, 0, ( struct sockaddr FAR * )&addr, sizeof( addr ) ) ;
	}
	if( SendVol < 0 )
	{
		int hr ;
		hr = WinAPIData.WinSockFunc.WSAGetLastErrorFunc() ;
		switch( hr )
		{
		case WSAEMSGSIZE :
			Result = -2 ;
			goto ENDLABEL ;

		case WSAEWOULDBLOCK :
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

			NS_ProcessMessage() ;
			return -3 ;

		default :
			Result = -1 ;
			goto ENDLABEL ;
		}
	}
	else
	{
		Result = SendVol ;
	}

	// ���M�\�t���O��|��
	Sock->UDPWriteFlag = FALSE ;

ENDLABEL :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return Result ;
}

#ifndef DX_NON_ASYNCLOAD
// NetWorkSendUDP �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void NetWorkSendUDP_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	SOCKETDATA *Sock ;
	int NetHandle ;
	IPDATA *SendIP_IPv4 ;
	IPDATA_IPv6 *SendIP_IPv6 ;
	int SendPort ;
	const void *Buffer ;
	int Length ;
	int Addr ;

	Addr = 0 ;
	NetHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	SendIP_IPv4 = ( IPDATA * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	SendIP_IPv6 = ( IPDATA_IPv6 * )GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	SendPort = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	Buffer = GetASyncLoadParamStruct( AParam->Data, &Addr ) ;
	Length = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	if( !NETHCHK_ASYNC( NetHandle, Sock ) )
	{
		Sock->HandleInfo.ASyncLoadResult = NetWorkSendUDP_Static( NetHandle, *SendIP_IPv4, *SendIP_IPv6, SendPort, Buffer, Length, TRUE ) ;
	}

	DecASyncLoadCount( NetHandle ) ;
}
#endif // DX_NON_ASYNCLOAD

// NetWorkSendUDP �̃O���[�o���f�[�^�ɃA�N�Z�X���Ȃ��o�[�W����
extern int NetWorkSendUDP_UseGParam(
	int NetUDPHandle,
	IPDATA SendIP_IPv4,
	IPDATA_IPv6 SendIP_IPv6,
	int SendPort,
	const void *Buffer,
	int Length,
	int ASyncLoadFlag
)
{
	int Result = -1 ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	if( WinData.ActiveFlag == FALSE )
		DxActiveWait() ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt( NULL, &Addr, NetUDPHandle ) ;
		AddASyncLoadParamStruct( NULL, &Addr, &SendIP_IPv4, sizeof( IPDATA ) ) ;
		AddASyncLoadParamStruct( NULL, &Addr, &SendIP_IPv6, sizeof( IPDATA_IPv6 ) ) ;
		AddASyncLoadParamInt( NULL, &Addr, SendPort ) ;
		AddASyncLoadParamStruct( NULL, &Addr, Buffer, Length ) ;
		AddASyncLoadParamInt( NULL, &Addr, Length ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto END ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = NetWorkSendUDP_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt( AParam->Data, &Addr, NetUDPHandle ) ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, &SendIP_IPv4, sizeof( IPDATA ) ) ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, &SendIP_IPv6, sizeof( IPDATA_IPv6 ) ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, SendPort ) ;
		AddASyncLoadParamStruct( AParam->Data, &Addr, Buffer, Length ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, Length ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto END ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( NetUDPHandle, AParam->Index ) ;

		Result = 0 ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		Result = NetWorkSendUDP_Static( NetUDPHandle, SendIP_IPv4, SendIP_IPv6, SendPort, Buffer, Length, FALSE ) ;
	}

#ifndef DX_NON_ASYNCLOAD
END :
#endif

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return Result ;
}

// UDP���g�p�����ʐM�Ŏw��̂h�o�Ƀf�[�^�𑗐M����ALength �͍ő�65507�ASendPort �� -1 �ɂ���� MakeUDPSocket �� RecvPort �œn�����|�[�g���g�p����܂�( �߂�l  0�ȏ�;���M�ł����f�[�^�T�C�Y  -1:�G���[   -2:���M�f�[�^���傫������  -3:���M�������ł��Ă��Ȃ� )
extern int NS_NetWorkSendUDP( int NetUDPHandle, IPDATA SendIP, int SendPort, const void *Buffer, int Length )
{
	SOCKETDATA *Sock ;
	IPDATA_IPv6 SendIP_IPv6 ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( UDPNETHCHK( NetUDPHandle, Sock ) ||
		Sock->IsIPv6 == TRUE )
		return -1 ;

	_MEMSET( &SendIP_IPv6, 0, sizeof( SendIP_IPv6 ) ) ;
	return NetWorkSendUDP_UseGParam( NetUDPHandle, SendIP, SendIP_IPv6, SendPort, Buffer, Length, GetASyncLoadFlag() ) ;
/*
	SOCKETDATA * Sock ;
	SOCKADDR_IN addr ;
	int SendVol ;
	int Result ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( UDPNETHCHK( NetUDPHandle, Sock ) ||
	    ( SendPort == -1 && Sock->Port == -1 ) ||
	    Sock->IsIPv6 == TRUE )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �f�[�^�̑��M
	_MEMSET( &addr, 0, sizeof( addr ) ) ;
	addr.sin_family = AF_INET ;
	addr.sin_addr = *(( in_addr *)&SendIP ) ;
	addr.sin_port = SendPort == -1 ? Sock->Port : WinAPIData.WinSockFunc.htonsFunc( ( unsigned short )SendPort ) ;
	SendVol = WinAPIData.WinSockFunc.sendtoFunc( Sock->Socket, (char *)Buffer, Length, 0, ( struct sockaddr FAR * )&addr, sizeof( addr ) ) ;
	if( SendVol < 0 )
	{
		int hr ;
		hr = WinAPIData.WinSockFunc.WSAGetLastErrorFunc() ;
		switch( hr )
		{
		case WSAEMSGSIZE :
			Result = -2 ;
			goto ENDLABEL ;

		case WSAEWOULDBLOCK :
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

			NS_ProcessMessage() ;
			return -3 ;

		default :
			Result = -1 ;
			goto ENDLABEL ;
		}
	}
	else
	{
		Result = SendVol ;
	}

	// ���M�\�t���O��|��
	Sock->UDPWriteFlag = FALSE ;

ENDLABEL :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return Result ;
*/
}

// UDP���g�p�����ʐM�Ŏw��̂h�o�Ƀf�[�^�𑗐M����ALength �͍ő�65507�ASendPort �� -1 �ɂ���� MakeUDPSocket �� RecvPort �œn�����|�[�g���g�p����܂�( �߂�l  0�ȏ�;���M�ł����f�[�^�T�C�Y  -1:�G���[   -2:���M�f�[�^���傫������  -3:���M�������ł��Ă��Ȃ� )( IPv6�� )
extern int NS_NetWorkSendUDP_IPv6( int NetUDPHandle, IPDATA_IPv6 SendIP, int SendPort, const void *Buffer, int Length )
{
	SOCKETDATA *Sock ;
	IPDATA SendIP_IPv4 ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( UDPNETHCHK( NetUDPHandle, Sock ) ||
		Sock->IsIPv6 == FALSE )
		return -1 ;

	_MEMSET( &SendIP_IPv4, 0, sizeof( SendIP_IPv4 ) ) ;
	return NetWorkSendUDP_UseGParam( NetUDPHandle, SendIP_IPv4, SendIP, SendPort, Buffer, Length, GetASyncLoadFlag() ) ;
/*
	SOCKETDATA * Sock ;
	_sockaddr_in6 addr ;
	int SendVol ;
	int Result ;
	int i ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( UDPNETHCHK( NetUDPHandle, Sock ) ||
	    ( SendPort == -1 && Sock->Port == -1 ) ||
	    Sock->IsIPv6 == FALSE )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �f�[�^�̑��M
	_MEMSET( &addr, 0, sizeof( addr ) ) ;
	// AF_INET6 �� 23
	addr.sin6_family = 23 ;
	for( i = 0 ; i < 8 ; i ++ )
		addr.sin6_addr.Word[ i ] = SendIP.Word[ i ] ;
	addr.sin6_port = SendPort == -1 ? Sock->Port : WinAPIData.WinSockFunc.htonsFunc( ( unsigned short )SendPort ) ;
	SendVol = WinAPIData.WinSockFunc.sendtoFunc( Sock->Socket, (char *)Buffer, Length, 0, ( struct sockaddr FAR * )&addr, sizeof( addr ) ) ;
	if( SendVol < 0 )
	{
		int hr ;
		hr = WinAPIData.WinSockFunc.WSAGetLastErrorFunc() ;
		switch( hr )
		{
		case WSAEMSGSIZE :
			Result = -2 ;
			goto ENDLABEL ;

		case WSAEWOULDBLOCK :
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

			NS_ProcessMessage() ;
			return -3 ;

		default :
			Result = -1 ;
			goto ENDLABEL ;
		}
	}
	else
	{
		Result = SendVol ;
	}

	// ���M�\�t���O��|��
	Sock->UDPWriteFlag = FALSE ;

ENDLABEL :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return Result ;
*/
}

// NetWorkRecvUDP �̎������֐�
static int NetWorkRecvUDP_Static(
	int NetUDPHandle,
	void *RecvIP,
	int *RecvPort,
	void *Buffer,
	int Length,
	int Peek,
	int ASyncThread
)
{
	SOCKETDATA *Sock = NULL ;
	SOCKADDR_IN addr_ipv4 = { 0 } ;
	_sockaddr_in6 addr_ipv6 = { 0 } ;
	int addrsize ;
	int RecvVol ;
	int Result ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( ( ASyncThread          && UDPNETHCHK_ASYNC( NetUDPHandle, Sock ) ) ||
		( ASyncThread == FALSE && UDPNETHCHK_ASYNC( NetUDPHandle, Sock ) ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �f�[�^�̎�M
	if( Sock->IsIPv6 )
	{
		_MEMSET( &addr_ipv6, 0, sizeof( addr_ipv6 ) ) ;
		addrsize = sizeof( addr_ipv6 ) ;
		RecvVol = WinAPIData.WinSockFunc.recvfromFunc(
					Sock->Socket,
					(char *)Buffer,
					Length,
					Peek ? MSG_PEEK : 0,
					( struct sockaddr FAR * )&addr_ipv6,
					&addrsize ) ;
	}
	else
	{
		_MEMSET( &addr_ipv4, 0, sizeof( addr_ipv4 ) ) ;
		addrsize = sizeof( addr_ipv4 ) ;
		RecvVol = WinAPIData.WinSockFunc.recvfromFunc(
					Sock->Socket,
					(char *)Buffer,
					Length,
					Peek ? MSG_PEEK : 0,
					( struct sockaddr FAR * )&addr_ipv4,
					&addrsize ) ;
	}

	// ��M�f�[�^����t���O��|��
	Sock->UDPReadFlag = FALSE ;

	if( RecvVol < 0 )
	{
		int hr ;

		hr = WinAPIData.WinSockFunc.WSAGetLastErrorFunc() ;
		switch( hr )
		{
		case WSAEMSGSIZE :
			Result = -2 ;
			goto ENDLABEL ;

		case WSAEWOULDBLOCK :
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

			NS_ProcessMessage() ;
			return -3 ;

		default :
			Result = -1 ;
			goto ENDLABEL ;
		}
	}
	else
	{
		Result = RecvVol ;
	}

	if( RecvIP )
	{
		if( Sock->IsIPv6 )
		{
			int i ;
			for( i = 0 ; i < 8 ; i ++ )
				( ( IPDATA_IPv6 * )RecvIP )->Word[ i ] = addr_ipv6.sin6_addr.Word[ i ] ;
		}
		else
		{
			( ( IPDATA * )RecvIP )->d1 = addr_ipv4.sin_addr.S_un.S_un_b.s_b1;
			( ( IPDATA * )RecvIP )->d2 = addr_ipv4.sin_addr.S_un.S_un_b.s_b2;
			( ( IPDATA * )RecvIP )->d3 = addr_ipv4.sin_addr.S_un.S_un_b.s_b3;
			( ( IPDATA * )RecvIP )->d4 = addr_ipv4.sin_addr.S_un.S_un_b.s_b4;
		}
	}

	if( RecvPort )
	{
		if( Sock->IsIPv6 )
		{
			*RecvPort = WinAPIData.WinSockFunc.ntohsFunc( addr_ipv6.sin6_port ) ;
		}
		else
		{
			*RecvPort = WinAPIData.WinSockFunc.ntohsFunc( addr_ipv4.sin_port ) ;
		}
	}

ENDLABEL :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	return Result ;
}

#ifndef DX_NON_ASYNCLOAD
// NetWorkRecvUDP �̔񓯊��ǂݍ��݃X���b�h����Ă΂��֐�
static void NetWorkRecvUDP_ASync( ASYNCLOADDATA_COMMON *AParam )
{
	SOCKETDATA *Sock ;
	int NetHandle ;
	void *RecvIP ;
	int *RecvPort ;
	void *Buffer ;
	int Length ;
	int Peek ;
	int Addr ;

	Addr = 0 ;
	NetHandle = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	RecvIP = GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	RecvPort = ( int * )GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	Buffer = GetASyncLoadParamVoidP( AParam->Data, &Addr ) ;
	Length = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	Peek = GetASyncLoadParamInt( AParam->Data, &Addr ) ;
	if( !NETHCHK_ASYNC( NetHandle, Sock ) )
	{
		Sock->HandleInfo.ASyncLoadResult = NetWorkRecvUDP_Static( NetHandle, RecvIP, RecvPort, Buffer, Length, Peek, TRUE ) ;
	}

	DecASyncLoadCount( NetHandle ) ;
}
#endif // DX_NON_ASYNCLOAD

// NetWorkRecvUDP �̃O���[�o���f�[�^�ɃA�N�Z�X���Ȃ��o�[�W����
extern int NetWorkRecvUDP_UseGParam(
	int NetUDPHandle,
	void *RecvIP,
	int *RecvPort,
	void *Buffer,
	int Length,
	int Peek,
	int ASyncLoadFlag
)
{
	int Result = -1 ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	if( WinData.ActiveFlag == FALSE )
		DxActiveWait() ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

#ifndef DX_NON_ASYNCLOAD
	if( ASyncLoadFlag )
	{
		ASYNCLOADDATA_COMMON *AParam = NULL ;
		int Addr ;

		// �p�����[�^�ɕK�v�ȃ������̃T�C�Y���Z�o
		Addr = 0 ;
		AddASyncLoadParamInt( NULL, &Addr, NetUDPHandle ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, RecvIP ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, RecvPort ) ;
		AddASyncLoadParamConstVoidP( NULL, &Addr, Buffer ) ;
		AddASyncLoadParamInt( NULL, &Addr, Length ) ;
		AddASyncLoadParamInt( NULL, &Addr, Peek ) ;

		// �������̊m��
		AParam = AllocASyncLoadDataMemory( Addr ) ;
		if( AParam == NULL )
			goto END ;

		// �����ɕK�v�ȏ����Z�b�g
		AParam->ProcessFunction = NetWorkRecvUDP_ASync ;
		Addr = 0 ;
		AddASyncLoadParamInt( AParam->Data, &Addr, NetUDPHandle ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, RecvIP ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, RecvPort ) ;
		AddASyncLoadParamConstVoidP( AParam->Data, &Addr, Buffer ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, Length ) ;
		AddASyncLoadParamInt( AParam->Data, &Addr, Peek ) ;

		// �f�[�^��ǉ�
		if( AddASyncLoadData( AParam ) < 0 )
		{
			DXFREE( AParam ) ;
			AParam = NULL ;
			goto END ;
		}

		// �񓯊��ǂݍ��݃J�E���g���C���N�������g
		IncASyncLoadCount( NetUDPHandle, AParam->Index ) ;

		Result = 0 ;
	}
	else
#endif // DX_NON_ASYNCLOAD
	{
		Result = NetWorkRecvUDP_Static( NetUDPHandle, RecvIP, RecvPort, Buffer, Length, Peek, FALSE ) ;
	}

#ifndef DX_NON_ASYNCLOAD
END :
#endif

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �I��
	return Result ;
}

// UDP���g�p�����ʐM�Ńf�[�^����M����APeek �� TRUE ��n���Ǝ�M�ɐ������Ă��f�[�^����M�L���[����폜���܂���( �߂�l  0�ȏ�:��M�����f�[�^�̃T�C�Y  -1:�G���[  -2:�o�b�t�@�̃T�C�Y������Ȃ�  -3:��M�f�[�^���Ȃ� )
extern int NS_NetWorkRecvUDP( int NetUDPHandle, IPDATA *RecvIP, int *RecvPort, void *Buffer, int Length, int Peek )
{
	SOCKETDATA * Sock ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( UDPNETHCHK( NetUDPHandle, Sock ) ||
		Sock->IsIPv6 == TRUE )
		return -1 ;

	return NetWorkRecvUDP_UseGParam( NetUDPHandle, RecvIP, RecvPort, Buffer, Length, Peek, GetASyncLoadFlag() ) ;
/*
	SOCKETDATA * Sock ;
	SOCKADDR_IN addr ;
	int addrsize ;
	int RecvVol ;
	int Result ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( UDPNETHCHK( NetUDPHandle, Sock ) ||
	    Sock->IsIPv6 == TRUE )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �f�[�^�̎�M
	_MEMSET( &addr, 0, sizeof( addr ) ) ;
	addrsize = sizeof( addr ) ;
	RecvVol = WinAPIData.WinSockFunc.recvfromFunc(
				Sock->Socket,
				(char *)Buffer,
				Length,
				Peek ? MSG_PEEK : 0,
				( struct sockaddr FAR * )&addr,
				&addrsize ) ;

	// ��M�f�[�^����t���O��|��
	Sock->UDPReadFlag = FALSE ;

	if( RecvVol < 0 )
	{
		int hr ;

		hr = WinAPIData.WinSockFunc.WSAGetLastErrorFunc() ;
		switch( hr )
		{
		case WSAEMSGSIZE :
			Result = -2 ;
			goto ENDLABEL ;

		case WSAEWOULDBLOCK :
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

			NS_ProcessMessage() ;
			return -3 ;

		default :
			Result = -1 ;
			goto ENDLABEL ;
		}
	}
	else
	{
		Result = RecvVol ;
	}

	if( RecvIP )
	{
		RecvIP->d1 = addr.sin_addr.S_un.S_un_b.s_b1;
		RecvIP->d2 = addr.sin_addr.S_un.S_un_b.s_b2;
		RecvIP->d3 = addr.sin_addr.S_un.S_un_b.s_b3;
		RecvIP->d4 = addr.sin_addr.S_un.S_un_b.s_b4;
	}

	if( RecvPort )
	{
		*RecvPort = WinAPIData.WinSockFunc.ntohsFunc( addr.sin_port ) ;
	}

ENDLABEL :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	return Result ;
*/
}

// UDP���g�p�����ʐM�Ńf�[�^����M����APeek �� TRUE ��n���Ǝ�M�ɐ������Ă��f�[�^����M�L���[����폜���܂���( �߂�l  0�ȏ�:��M�����f�[�^�̃T�C�Y  -1:�G���[  -2:�o�b�t�@�̃T�C�Y������Ȃ�  -3:��M�f�[�^���Ȃ� )( IPv6�� )
extern int NS_NetWorkRecvUDP_IPv6( int NetUDPHandle, IPDATA_IPv6 *RecvIP, int *RecvPort, void *Buffer, int Length, int Peek )
{
	SOCKETDATA * Sock ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( UDPNETHCHK( NetUDPHandle, Sock ) ||
		Sock->IsIPv6 == FALSE )
		return -1 ;

	return NetWorkRecvUDP_UseGParam( NetUDPHandle, RecvIP, RecvPort, Buffer, Length, Peek, GetASyncLoadFlag() ) ;
/*
	SOCKETDATA * Sock ;
	_sockaddr_in6 addr ;
	int addrsize ;
	int RecvVol ;
	int Result ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( UDPNETHCHK( NetUDPHandle, Sock ) ||
	    Sock->IsIPv6 == FALSE )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �f�[�^�̎�M
	_MEMSET( &addr, 0, sizeof( addr ) ) ;
	addrsize = sizeof( addr ) ;
	RecvVol = WinAPIData.WinSockFunc.recvfromFunc(
				Sock->Socket,
				(char *)Buffer,
				Length,
				Peek ? MSG_PEEK : 0,
				( struct sockaddr FAR * )&addr,
				&addrsize ) ;

	// ��M�f�[�^����t���O��|��
	Sock->UDPReadFlag = FALSE ;

	if( RecvVol < 0 )
	{
		int hr ;

		hr = WinAPIData.WinSockFunc.WSAGetLastErrorFunc() ;
		switch( hr )
		{
		case WSAEMSGSIZE :
			Result = -2 ;
			goto ENDLABEL ;

		case WSAEWOULDBLOCK :
			// �N���e�B�J���Z�N�V�����̉��
			CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

			NS_ProcessMessage() ;
			return -3 ;

		default :
			Result = -1 ;
			goto ENDLABEL ;
		}
	}
	else
	{
		Result = RecvVol ;
	}

	if( RecvIP )
	{
		int i ;
		for( i = 0 ; i < 8 ; i ++ )
			RecvIP->Word[ i ] = addr.sin6_addr.Word[ i ] ;
	}

	if( RecvPort )
	{
		*RecvPort = WinAPIData.WinSockFunc.ntohsFunc( addr.sin6_port ) ;
	}

ENDLABEL :

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	return Result ;
*/
}

// UDP���g�p�����ʐM�Ńf�[�^�����M�ł����Ԃ��ǂ����𒲂ׂ�( �߂�l  -1:�G���[  TRUE:���M�\  FALSE:���M�s�\ )
extern int NS_CheckNetWorkSendUDP( int NetUDPHandle )
{
	SOCKETDATA * Sock ;
	int Result ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( UDPNETHCHK( NetUDPHandle, Sock ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// �f�[�^���M�\�t���O���Z�b�g
	Result = Sock->UDPWriteFlag ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �f�[�^���M�\�t���O��Ԃ�
	return Result ;
}

// UDP���g�p�����ʐM�ŐV���Ȏ�M�f�[�^�����݂��邩�ǂ����𒲂ׂ�( �߂�l  -1:�G���[  TRUE:��M�f�[�^����  FALSE:��M�f�[�^�Ȃ� )
extern int NS_CheckNetWorkRecvUDP( int NetUDPHandle )
{
	SOCKETDATA * Sock ;
	int addrsize ;
	int RecvVol ;
	BYTE TempBuffer[ 16 ] ;

	// �ʐM�֌W�̏�����������Ă��Ȃ������牽�������I��
	if( SockData.InitializeFlag == FALSE ) return -1 ;

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	// �l�b�g���[�N�n���h���̃`�F�b�N
	if( UDPNETHCHK( NetUDPHandle, Sock ) )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return -1 ;
	}

	// ��M�f�[�^������ꍇ�� TRUE ��Ԃ�
	if( Sock->UDPReadFlag )
	{
		Sock->UDPReadFlag = FALSE ;

		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return TRUE ;
	}

	// �f�[�^�̎�M�`�F�b�N
	if( Sock->IsIPv6 )
	{
		_sockaddr_in6 addr_v6 ;
		_MEMSET( &addr_v6, 0, sizeof( addr_v6 ) ) ;
		addrsize = sizeof( addr_v6 ) ;
		RecvVol = WinAPIData.WinSockFunc.recvfromFunc(
					Sock->Socket,
					(char *)TempBuffer,
					sizeof( TempBuffer ),
					MSG_PEEK,
					( struct sockaddr FAR * )&addr_v6,
					&addrsize ) ;
	}
	else
	{
		SOCKADDR_IN addr ;
		_MEMSET( &addr, 0, sizeof( addr ) ) ;
		addrsize = sizeof( addr ) ;
		RecvVol = WinAPIData.WinSockFunc.recvfromFunc(
					Sock->Socket,
					(char *)TempBuffer,
					sizeof( TempBuffer ),
					MSG_PEEK,
					( struct sockaddr FAR * )&addr,
					&addrsize ) ;
	}

	if( RecvVol >= 0 || WinAPIData.WinSockFunc.WSAGetLastErrorFunc() == WSAEMSGSIZE )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;
		return TRUE ;
	}

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &HandleManageArray[ DX_HANDLETYPE_NETWORK ].CriticalSection ) ;

	NS_ProcessMessage() ;

	// �I��
	return FALSE ;
}












// ProcessNetMessage ���Ђ�����Ăё�����X���b�h
DWORD WINAPI ProcessNetMessageThreadFunction( LPVOID )
{
	for(;;)
	{
		if( SockData.ProcessNetMessageThreadEndRequest != FALSE ) break ;
		if( SockData.InitializeFlag == FALSE ) break ;

		// �l�b�g���b�Z�[�W�v���Z�X����
		NS_ProcessNetMessage() ;

		// �҂�
		Thread_Sleep( 8 ) ;
	}

	// �X���b�h�I��
	SockData.ProcessNetMessageThreadExitFlag = TRUE ;
	ExitThread( 0 ) ;

	return 0 ;
}









#if 0

// HTTP �֌W�̏����̏��������s��
static int HTTP_Initialize( void )
{
	// �돉����
	_MEMSET( &HttpData, 0, sizeof( HttpData ) ) ;
	
	// �I��
	return 0 ;
}

// HTTP �֌W�̏����̌�n�����s��
static int HTTP_Terminate( void )
{
	// �S�Ẵn���h�������
	NS_HTTP_CloseAll() ;
	
	// �I��
	return 0 ;
}

// HTTP ���b�Z�[�W�̌��ʂ𓾂�
static int HTTP_GetResult( const char *ResStr )
{
	const char *p ;

	p = _STRCHR( ResStr, ' ' ) ;
	if( p == NULL ) return -1 ;

	return _ATOI( p ) ;
}

// HTTP ���g�p���ăl�b�g���[�N��̃t�@�C�����_�E�����[�h����
extern int NS_HTTP_FileDownload( const char *FileURL, const char *SavePath, void **SaveBufferP,
										int *FileSize, char **ParamList )
{
	int DHandle = -1, BackUp = -1, Screen = -1, Font = -1, DrawScreen, BlendMode, BlendPal ;
	int width, rate, ScW, ScH, CX, CY ;
	int StartTime ;
	char FileName[64] ;
	const int CommentY = -32, MeterX = -15, MeterY = 32, MeterH = 8, MeterW = 110, SpeedX = 30, SpeedY = 8 ;
	const int DownSizeX = 10, DownSizeY = 8, SecY = -10, FontSize = 10, BlackW = 320, BlackH = 120 ;
	const char *Message1 = "\x90\xda\x91\xb1\x92\x86\x82\xc5\x82\xb7"/*@ "�ڑ����ł�" @*/ ;
	const char *Message2 = "%s \x82\xf0\x83\x5f\x83\x45\x83\x93\x83\x8d\x81\x5b\x83\x68\x92\x86\x82\xc5\x82\xb7"/*@ "%s ���_�E�����[�h���ł�" @*/ ;
	const char *Message3 = "\x82\xa0\x82\xc6 %s"/*@ "���� %s" @*/ ;
	const char *Message4 = "%s %cbyte/sec" ;
	const char *Message5 = "%s %cbyte/%s %cbyte" ;

	// �p�X�̎w�肪�Ȃ��ꍇ�̓J�����g�t�H���_�Ɋi�[����	
	if( SaveBufferP == NULL && SavePath == NULL ) SavePath = "" ;
	
	// �t�@�C�����𓾂�
	NS_URLAnalys( FileURL, NULL, NULL, FileName, NULL ) ;
//	ErrorLogFmtAdd( "FileName:%s\n", FileName ) ;

	// �`����\��ʂɂ���
	DrawScreen = GetActiveGraph() ;
	SetDrawScreen( DX_SCREEN_FRONT ) ;

	// �`��u�����h���[�h��ۑ�
	GetDrawBlendMode( &BlendMode, &BlendPal ) ;

	// ���b�Z�[�W��`�悷��ׂ̃t�H���g���쐬
	{
		int FontTexFlag = GetUseSystemMemGraphCreateFlag() ;
		int CacheNum = GetFontCacheCharNum() ;
		SetFontCacheToTextureFlag( FALSE ) ;
		SetFontCacheCharNum( 60 ) ;
		Font = CreateFontToHandle( NULL, FontSize, -1, DX_FONTTYPE_ANTIALIASING ) ;
		if( Font < 0 ) goto ERR ;
		SetFontCacheCharNum( CacheNum ) ;
		SetFontCacheToTextureFlag( FontTexFlag ) ;
	}

	// �`��p��ʂ̍쐬�Ɖ�ʂ̃R�s�[�����
	{
		int UseSystemMem ;

		GetDrawScreenSize( &ScW, &ScH ) ;
		UseSystemMem = GetUseSystemMemGraphCreateFlag() ;
		SetUseSystemMemGraphCreateFlag( TRUE ) ;
		BackUp = MakeGraph( BlackW, BlackH, TRUE ) ;
		if( BackUp < 0 ) goto ERR ;
		Screen = MakeGraph( BlackW, BlackH, TRUE ) ;
		if( Screen < 0 ) goto ERR ;
		GetDrawScreenGraph( ScW / 2 - BlackW / 2, ScH / 2 - BlackH / 2,
							ScW / 2 + BlackW / 2, ScH / 2 + BlackH / 2, BackUp ) ;
		SetUseSystemMemGraphCreateFlag( UseSystemMem ) ;

		CX = BlackW / 2 ;
		CY = BlackH / 2 ;
	}

	// �ڑ��҂��\��
	if( GetActiveFlag() == TRUE )
	{
		SetDrawScreen( Screen ) ;
		DrawBox( 0, 0, BlackW, BlackH, GetColor( 0,0,0 ), TRUE ) ;

		width = GetDrawStringWidthToHandle( Message1, lstrlenA( Message1 ), Font ) ;
		DrawStringToHandle( CX - width / 2, CY + CommentY, Message1, GetColor( 255,255,255 ), Font ) ;

		SetDrawScreen( DX_SCREEN_FRONT ) ;
		DrawGraph( ScW / 2 - BlackW / 2, ScH / 2 - BlackH / 2, Screen, FALSE ) ;
	}

	// �_�E�����[�h�����J�n
	DHandle = HTTP_StartFileDownload( FileURL, SavePath, SaveBufferP, ParamList ) ;
	if( DHandle == -1 )
	{
		ErrorLogFmtAdd( "\x83\x74\x83\x40\x83\x43\x83\x8b %s \x82\xcc\x83\x5f\x83\x45\x83\x93\x83\x8d\x81\x5b\x83\x68\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd"/*@ "�t�@�C�� %s �̃_�E�����[�h�Ɏ��s���܂���" @*/, FileName ) ;
		goto ERR ;
	}

	// ���x�v�Z�̏���
	StartTime = GetNowCount() ;

	// �_�E�����[�h�����܂ő҂�
	while( ProcessMessage() == 0 && HTTP_GetState( DHandle ) == HTTP_RES_NOW )
	{
		// ���������҂�
		WaitVSync(1) ;

		// �_�E�����[�h�������̕\��
		if( ProcessMessage() == 0 && GetActiveFlag() == TRUE && HTTP_GetDownloadedFileSize( DHandle ) != -1 )
		{
			SetDrawScreen( Screen ) ;
			DrawBox( 0, 0, BlackW, BlackH, GetColor( 0,0,0 ), TRUE ) ;

			// �����̕\��
			width = GetDrawFormatStringWidthToHandle( Font, Message2, FileName ) ;
			DrawFormatStringToHandle( CX - width / 2, CY + CommentY, GetColor( 255,255,255 ), Font, Message2, FileName ) ;

			// �_�E�����[�h����\��
			if( GetNowCount() - StartTime > 1000 )
			{
				int RecvSize, FileSize, LapsTime ;
				char tempstr[128], timestr[32], numstr1[32], numstr2[32], unit1, unit2 ;
			
				RecvSize = HTTP_GetDownloadedFileSize( DHandle ) ;
				FileSize = HTTP_GetDownloadFileSize( DHandle ) ;
				LapsTime = GetNowCount() - StartTime ;

				// �_�E�����[�h���[�^�[�̕`��
				{
					rate = _DTOL( (double)HTTP_GetDownloadedFileSize( DHandle ) * MeterW / HTTP_GetDownloadFileSize( DHandle ) ) ;
					DrawBox( CX - MeterW / 2 + MeterX, CY + MeterY, CX - MeterW / 2 + MeterX + rate, CY + MeterY + MeterH, GetColor( 255,0,0 ), TRUE ) ;
					DrawBox( CX - MeterW / 2 + MeterX + rate, CY + MeterY, CX + MeterX - MeterW / 2 + MeterW, CY + MeterY + MeterH, GetColor( 80,80,80 ), TRUE ) ;
					_SNPRINTF( tempstr, sizeof( tempstr ), "%.1f%%", ( double )HTTP_GetDownloadedFileSize( DHandle ) * 100 / HTTP_GetDownloadFileSize( DHandle ) ) ;
					if( _STRCMP( tempstr, "-0.0" ) == 0 ) _STRCPY( tempstr, "0.0" ) ;
					DrawStringToHandle( CX - MeterW / 2 + MeterW, CY + MeterY + MeterH - FontSize, tempstr, GetColor( 255,255,255 ), Font ) ;
				}

				// �c�莞�Ԃ̕`��
				if( RecvSize != 0 )
				{
					GetTimeLengthString( _DTOL( ( double )( FileSize - RecvSize ) / RecvSize * LapsTime ), timestr ) ;
					_SNPRINTF( tempstr, sizeof( tempstr ), Message3, timestr ) ;
					width = GetDrawStringWidthToHandle( tempstr, lstrlenA( tempstr ), Font )  ;
					DrawStringToHandle( CX - width / 2, CY + SecY, tempstr, GetColor( 255,255,255 ), Font ) ;
				}
				
				// �_�E�����[�h���x�̕`��
				{
					unit1 = GetDataSizeString( _DTOL( ( double )RecvSize / ( ( double )LapsTime / 1000.0 ) ), numstr1 ) ;
					_SNPRINTF( tempstr, sizeof( tempstr ), Message4, numstr1, unit1 ) ;
					width = GetDrawStringWidthToHandle( tempstr, lstrlenA( tempstr ), Font ) ;
					DrawStringToHandle( CX + SpeedX, CY + SpeedY, tempstr, GetColor( 255,255,255 ), Font ) ;
				}
				
				// �_�E�����[�h�����T�C�Y�̕`��
				{
					unit1 = GetDataSizeString( RecvSize, numstr1 ) ;
					unit2 = GetDataSizeString( FileSize, numstr2 ) ;
					_SNPRINTF( tempstr, sizeof( tempstr ), Message5, numstr1, unit1, numstr2, unit2 ) ;
					width = GetDrawStringWidthToHandle( tempstr, lstrlenA( tempstr ), Font ) ;
					DrawStringToHandle( CX - width + DownSizeX, CY + DownSizeY, tempstr, GetColor( 255,255,255 ), Font ) ;
				}
			}
			
			// �\��ʂɔ��f
			SetDrawScreen( DX_SCREEN_FRONT ) ;
			DrawGraph( ScW / 2 - BlackW / 2, ScH / 2 - BlackH / 2, Screen, FALSE ) ;
		}
	}
	if( HTTP_GetState( DHandle ) != HTTP_RES_COMPLETE )
	{
		switch( HTTP_GetState( DHandle ) )
		{
		case HTTP_RES_STOP : ErrorLogFmtAdd( "\x83\x74\x83\x40\x83\x43\x83\x8b %s \x82\xcc\x83\x5f\x83\x45\x83\x93\x83\x8d\x81\x5b\x83\x68\x82\xcd\x92\x86\x8e\x7e\x82\xb3\x82\xea\x82\xdc\x82\xb5\x82\xbd"/*@ "�t�@�C�� %s �̃_�E�����[�h�͒��~����܂���" @*/, FileName ) ; break ;
		case HTTP_RES_ERROR : ErrorLogFmtAdd( "\x83\x74\x83\x40\x83\x43\x83\x8b %s \x82\xcc\x83\x5f\x83\x45\x83\x93\x83\x8d\x81\x5b\x83\x68\x82\xc5\x83\x47\x83\x89\x81\x5b\x82\xaa\x94\xad\x90\xb6\x82\xb5\x82\xdc\x82\xb5\x82\xbd"/*@ "�t�@�C�� %s �̃_�E�����[�h�ŃG���[���������܂���" @*/, FileName ) ; break ;
		}
		goto ERR ;
	}
	
	// �t�@�C���T�C�Y�𓾂�
	if( FileSize != NULL ) *FileSize = HTTP_GetDownloadFileSize( DHandle ) ;

	// HTTP �n���h�������
	HTTP_Close( DHandle ) ;

	// �t�H���g�n���h���̍폜
	DeleteFontToHandle( Font ) ;

	// �F�X���ɖ߂�
	if( ProcessMessage() == 0 && GetActiveFlag() == TRUE )
	{
		// �o�b�N�A�b�v���Ă����摜�𕜌�
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 ) ;
		DrawGraph( ScW / 2 - BlackW / 2, ScH / 2 - BlackH / 2, BackUp, FALSE ) ;
		DeleteGraph( BackUp ) ;
		
		// �`���Ɏg�p���Ă�����ʂ��폜
		DeleteGraph( Screen ) ;

		SetDrawScreen( DrawScreen ) ;				// �`�������ɖ߂�
		SetDrawBlendMode( BlendMode, BlendPal ) ;	// �`��u�����h���[�h�����ɖ߂�
	}
	
	
	// �I��
	return 0 ;

	// �G���[����
ERR :
	if( BackUp != -1 )
	{
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 ) ;
		if( ProcessMessage() == 0 && GetActiveFlag() == TRUE ) DrawGraph( ScW / 2 - BlackW / 2, ScH / 2 - BlackH / 2, BackUp, FALSE ) ;
		DeleteGraph( BackUp ) ;
	}
	if( Screen != -1 ) DeleteGraph( Screen ) ;
	SetDrawScreen( DrawScreen ) ;
	SetDrawBlendMode( BlendMode, BlendPal ) ;

	if( DHandle != -1 ) HTTP_Close( DHandle ) ;
	if( Font != -1 ) DeleteFontToHandle( Font ) ;

	return -1 ;
}

// HTTP ���g�p���ăl�b�g���[�N��̃t�@�C���̃T�C�Y�𓾂�
extern int NS_HTTP_GetFileSize( const char *FileURL )
{
	int DHandle, FileSize ;

	// �t�@�C���T�C�Y�擾�J�n
	DHandle = HTTP_StartGetFileSize( FileURL ) ;
	if( DHandle == -1 ) return -1 ;
	
	// �_�E�����[�h�����܂ő҂�
	while( ProcessMessage() == 0 && HTTP_GetState( DHandle ) == HTTP_RES_NOW ){}
	if( HTTP_GetState( DHandle ) != HTTP_RES_COMPLETE )
	{
		// ���s�����獟���ŏI��
		HTTP_Close( DHandle ) ;
		return -1 ;
	}
	
	// �t�@�C���T�C�Y�𓾂�
	FileSize = HTTP_GetDownloadFileSize( DHandle ) ;

	// HTTP �n���h�������
	HTTP_Close( DHandle ) ;
	
	// �t�@�C���T�C�Y��Ԃ�
	return FileSize ;
}

// �w��̂t�q�k����z�X�g�p�X�A�z�X�g�̃A�N�Z�X�p�X�A�A�N�Z�X�|�[�g���擾����
static int HTTP_GetConnectInfo( const char *URL, int UseProxy, char *HostBuf,
								char *PathBuf, char *FileNameBuf, int *PortBuf )
{
	// �h�d�̃v���L�V���g��Ȃ��ꍇ�Ǝg���ꍇ�ŏ����𕪊�
	HostBuf[0] = '\0' ;
	if( UseProxy == TRUE )
	{
		char temppath[128] ;
		int tempport ;

		// �t�q�k�����
		if( NS_URLAnalys( URL, HostBuf, temppath, FileNameBuf, &tempport ) == -1 ) return -1 ;

		// �g���Ȃ��L�����g����L���ɕϊ�
		NS_URLConvert( temppath, FALSE ) ;

		// �p�X���\�z
		_STRCPY( PathBuf, "http://" ) ;
		_STRCAT( PathBuf, HostBuf ) ;
		if( tempport != -1 ) _SNPRINTF( PathBuf + lstrlenA( PathBuf ), sizeof( PathBuf ) - lstrlenA( PathBuf ), ":%d", tempport ) ;
		_STRCAT( PathBuf, temppath ) ;
		
		// �v���L�V�̏��𓾂�
		_STRCPY( HostBuf, HttpData.ProxyHost ) ;
		*PortBuf = HttpData.ProxyPort ;
	}

	// �v���L�V���L���ł͂Ȃ������ꍇ
	if( HostBuf[0] == '\0' )
	{
		// �t�q�k�����
		if( NS_URLAnalys( URL, HostBuf, PathBuf, FileNameBuf, PortBuf ) == -1 ) return -1 ;
		if( *PortBuf == -1 ) *PortBuf = 80 ;

		// �g���Ȃ��L�����g����L���ɕϊ�
		NS_URLConvert( PathBuf, FALSE ) ;
	}
	
	// �I��
	return 0 ;
}

// �w��� HTTP �z�X�g�ɐڑ�����( �߂�l: 0�ȏ� = �l�b�g���[�N�n���h��  -1 = �G���[ )
static int HTTP_ConnectHost( const char *URL, int UseProxy, IPDATA *HostIPBuf,
							 char *HostBuf, char *PathBuf, char *FileNameBuf, int *PortBuf )
{
	char Host[128], Path[128], FileName[128] ;
	int Port, NetHandle ;
	IPDATA HostIP ;

	// �ڑ���z�X�g�A�z�X�g�ւ̃A�N�Z�X�p�X�A�A�N�Z�X�|�[�g�𓾂�
	if( HTTP_GetConnectInfo( URL, UseProxy, Host, Path, FileName, &Port ) < 0 )
	{
		DXST_ERRORLOGFMT_ADDA(( "\x82\x74\x82\x71\x82\x6b %s \x82\xcc\x89\xf0\x90\xcd\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n"/*@ "�t�q�k %s �̉�͂Ɏ��s���܂���\n" @*/, URL )) ;
		return -1 ;
	}

	// �w��̃z�X�g�����݂��邩����
	if( NS_GetHostIPbyName( Host, &HostIP ) == -1 )
	{
		DXST_ERRORLOGFMT_ADDA(( "\x83\x5f\x83\x45\x83\x93\x83\x8d\x81\x5b\x83\x68\x90\xe6\x83\x7a\x83\x58\x83\x67 %s \x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd"/*@ "�_�E�����[�h��z�X�g %s ��������܂���ł���" @*/, Host )) ;
		return -1 ;
	}

	// �z�X�g�ɐڑ�����
	{
		int DXProtocol, CloseAfterLost ;

		// �t���O��ޔ�		
		DXProtocol = NS_GetUseDXNetWorkProtocol() ;
		CloseAfterLost = NS_GetNetWorkCloseAfterLostFlag() ;

		// �t���O���Z�b�g
		NS_SetUseDXNetWorkProtocol( FALSE ) ;
		NS_SetNetWorkCloseAfterLostFlag( FALSE )  ;

		// �ڑ�
		NetHandle = NS_ConnectNetWork( HostIP, Port ) ;

		// �t���O�𕜋A
		NS_SetUseDXNetWorkProtocol( DXProtocol ) ;
		NS_SetNetWorkCloseAfterLostFlag( CloseAfterLost )  ;
		
		// ���s�̔���
		if( NetHandle == -1 )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x83\x5f\x83\x45\x83\x93\x83\x8d\x81\x5b\x83\x68\x90\xe6\x83\x7a\x83\x58\x83\x67 %s \x82\xd6\x82\xcc\x90\xda\x91\xb1\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd"/*@ "�_�E�����[�h��z�X�g %s �ւ̐ڑ��Ɏ��s���܂���" @*/, Host )) ;
			return -1 ;
		}
	}
	
	// �o�b�t�@�̃|�C���^���n����Ă���ꍇ�����R�s�[����
	if( HostBuf != NULL ) _STRCPY( HostBuf, Host ) ;
	if( PathBuf != NULL ) _STRCPY( PathBuf, Path ) ;
	if( FileNameBuf != NULL ) _STRCPY( FileNameBuf, FileName ) ;
	if( PortBuf != NULL ) *PortBuf = Port ;
	if( HostIPBuf != NULL ) *HostIPBuf = HostIP ;

	// �l�b�g�n���h����Ԃ�
	return NetHandle ;
}

// �V����HTTP�n���h����ǉ�����( �߂�l: -1 = �G���[  0�ȏ� = HTTP�n���h�� )
static	int			HTTP_AddHandle( void )
{
	int NewHandle ;
	HTTPDATA *http ;

	// ����HTTP�n���h������t�̏ꍇ�̓G���[
	if( HttpData.Num == MAX_HTTPHANDLE_NUM )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x0c\x54\x42\x66\x6b\x30\xc0\x30\xa6\x30\xf3\x30\xed\x30\xfc\x30\xc9\x30\x57\x30\x66\x30\x44\x30\x8b\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x4c\x30\x50\x96\x4c\x75\x6b\x30\x54\x90\x57\x30\x66\x30\x44\x30\x5f\x30\x5f\x30\x81\x30\xc0\x30\xa6\x30\xf3\x30\xed\x30\xfc\x30\xc9\x30\x92\x30\x8b\x95\xcb\x59\x67\x30\x4d\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�����Ƀ_�E�����[�h���Ă���t�@�C�������E�ɒB���Ă������߃_�E�����[�h���J�n�ł��܂���ł���\n" @*/ ) ;
		return -1 ;
	}
	
	// �g���Ă��Ȃ�HTTP�n���h����T��
	for( NewHandle = 0 ; HttpData.Data[NewHandle] != NULL ; NewHandle ++ ){}

	// �f�[�^���i�[���郁�����̈���m��
	HttpData.Data[NewHandle] = http = ( HTTPDATA * )DXALLOC( sizeof( HTTPDATA ) ) ;
	if( http == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xc0\x30\xa6\x30\xf3\x30\xed\x30\xfc\x30\xc9\x30\xe6\x51\x06\x74\x28\x75\xc7\x30\xfc\x30\xbf\x30\x18\x98\xdf\x57\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�_�E�����[�h�����p�f�[�^�̈�̊m�ۂɎ��s���܂���\n" @*/ ) ;
		return -1 ;
	}
	
	// �돉����
	_MEMSET( http, 0, sizeof( HTTPDATA ) ) ;

	// �n���h���̐��𑝂₷
	HttpData.Num ++ ;

	// �n���h����Ԃ�
	return NewHandle ;
}

// HTTP�n���h���̌�n�����s��
static	int			HTTP_DelHandle( int HttpHandle )
{
	HTTPDATA *http = HttpData.Data[HttpHandle] ;
	
	// ���ɉ������Ă����牽�������I��
	if( http == NULL ) return 0 ;
	
	// �������̉��
	DXFREE( http ) ;
	HttpData.Data[HttpHandle] = NULL ;
	
	// �n���h���̐������炷
	HttpData.Num -- ;
	
	// �I��
	return 0 ;
}

// HTTP ���g�p�����l�b�g���[�N��̃t�@�C�����_�E�����[�h���鏈�����J�n����
extern int NS_HTTP_StartFileDownload( const char *FileURL, const char *SavePath, void **SaveBufferP, char **ParamList )
{
	HTTPDATA *http = NULL ;
	int NewHandle = -1 ;

	// �ۑ���̎w�肪�s���ȏꍇ�̓G���[
	if( SavePath == NULL && SaveBufferP == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xdd\x4f\x58\x5b\x48\x51\x6e\x30\x07\x63\x9a\x5b\x4c\x30\x0d\x4e\x63\x6b\x6a\x30\x5f\x30\x81\x30\xc0\x30\xa6\x30\xf3\x30\xed\x30\xfc\x30\xc9\x30\xfa\x51\x65\x67\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"�ۑ���̎w�肪�s���Ȃ��߃_�E�����[�h�o���܂���\n" @*/ ) ;
		return -1 ;
	}

	// HTTP �����p�n���h����ǉ�����
	NewHandle = HTTP_AddHandle() ;
	if( NewHandle < 0 ) goto ERR ;
	http = HttpData.Data[NewHandle] ;

	// �z�X�g�ɐڑ�����
	http->NetHandle = HTTP_ConnectHost( FileURL, HttpData.UseProxy,
								&http->HostIP, http->Host, http->Path, http->FileName, &http->Port ) ;
	if( http->NetHandle < 0 )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x48\x00\x54\x00\x54\x00\x50\x00\x20\x00\xdb\x30\xb9\x30\xc8\x30\x78\x30\x6e\x30\xa5\x63\x9a\x7d\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"HTTP �z�X�g�ւ̐ڑ��Ɏ��s���܂���\n" @*/ ) ;
		goto ERR ;
	}

	// �p�����[�^����͂���
	if( ParamList != NULL )
	{
		http->ParamLength = NS_URLParamAnalysis( ParamList, &http->Param ) ;
		if( http->ParamLength < 0 )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x48\x00\x54\x00\x54\x00\x50\x00\x20\x00\x6e\x30\xd1\x30\xe9\x30\xe1\x30\xfc\x30\xbf\x30\xe3\x89\x90\x67\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"HTTP �̃p�����[�^��͂Ɏ��s���܂���\n" @*/ ) ;
			goto ERR ;
		}
	}
	else
	{
		http->Param = NULL ;
		http->ParamLength = 0 ;
	}

	// �f�[�^�̏�����
	http->Process 		= http->Param != NULL ? HTTP_PR_POST : HTTP_PR_GET ;
	http->State 		= HTTP_ST_CONNECT ;
	http->Step			= 0 ;
	http->Counter		= 0 ;
	http->RecvFileSize 	= 0 ;
	http->FileSize 		= -1 ;
	http->Result		= HTTP_RES_NOW ;
	http->Error			= HTTP_ERR_NONE ;

	// �ۑ�������Z�b�g�A�b�v
	if( SaveBufferP != NULL )
	{
		// �������Ɋi�[
		http->SavePlace = HTTP_SV_MEM ;
		http->BufferPoint = SaveBufferP ;
		*http->BufferPoint = NULL ;
	}
	else
	{
		// �t�@�C���ɕۑ�
		DeleteFile( http->FileName ) ;
		http->FilePoint = CreateFile( http->FileName, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ) ;
		if( http->FilePoint == NULL )
		{
			DXST_ERRORLOGFMT_ADDA(( "\x83\x5f\x83\x45\x83\x93\x83\x8d\x81\x5b\x83\x68\x82\xb5\x82\xbd\x83\x74\x83\x40\x83\x43\x83\x8b\x82\xf0\x95\xdb\x91\xb6\x82\xb7\x82\xe9\x83\x74\x83\x40\x83\x43\x83\x8b %s \x82\xf0\x8a\x4a\x82\xaf\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\n"/*@ "�_�E�����[�h�����t�@�C����ۑ�����t�@�C�� %s ���J���܂���ł���\n" @*/, http->FileName )) ;
			goto ERR ;
		}
	}

	// HTTP ��������񕪍s��
	if( HTTP_Process( NewHandle ) == -1 )
	{
		NS_HTTP_Close( NewHandle ) ;
		return -1 ;
	}

	// �I��
	return NewHandle ;

	// �G���[����	
ERR :
	if( NewHandle != -1 )
	{
		if( http->Param != NULL ) DXFREE( http->Param ) ;
		if( http->NetHandle != -1 )
		{
			NS_CloseNetWork( http->NetHandle ) ;
		}
		if( http->FilePoint != NULL ) CloseHandle( http->FilePoint ) ;
		HTTP_DelHandle( NewHandle ) ;
	}

	return -1 ; 
}

// HTTP ���g�p�����l�b�g���[�N��̃t�@�C���̃T�C�Y�𓾂鏈�����J�n����
extern int NS_HTTP_StartGetFileSize( const char *FileURL )
{
	HTTPDATA *http = NULL ;
	int NewHandle = -1 ;

	// HTTP �����p�n���h����ǉ�����
	NewHandle = HTTP_AddHandle() ;
	if( NewHandle < 0 ) goto ERR ;
	http = HttpData.Data[NewHandle] ;

	// �z�X�g�ɐڑ�����
	http->NetHandle = HTTP_ConnectHost( FileURL, HttpData.UseProxy,
								&http->HostIP, http->Host, http->Path, http->FileName, &http->Port ) ;
	if( http->NetHandle < 0 ) goto ERR ;
	
	// �f�[�^�̏�����
	http->Process 		= HTTP_PR_HEAD ;
	http->State 		= HTTP_ST_CONNECT ;
	http->Step			= 0 ;
	http->Counter		= 0 ;
	http->FileSize 		= -1 ;
	http->Result		= HTTP_RES_NOW ;
	http->Param			= NULL ;
	http->Error			= HTTP_ERR_NONE ;

	// HTTP ��������񕪍s��
	if( HTTP_Process( NewHandle ) == -1 )
	{
		NS_HTTP_Close( NewHandle ) ;
		return -1 ;
	}

	// �I��
	return NewHandle ;

	// �G���[����	
ERR :
	if( NewHandle != -1 )
	{
		if( http->Param != NULL ) DXFREE( http->Param ) ;
		if( http->NetHandle != -1 )
		{
			NS_CloseNetWork( http->NetHandle ) ;
		}
		HTTP_DelHandle( NewHandle ) ;
	}

	return -1 ; 
}

// HTTP �̏����Ɏg�p���Ă��郊�\�[�X���������
static int HTTP_ReleaseResource( int HttpHandle, int Result, int Error )
{
	HTTPDATA *http = HttpData.Data[HttpHandle] ;

	if( http == NULL ) return -1 ;

	// �l�b�g�n���h�������
	if( http->NetHandle != -1 )
	{
		NS_CloseNetWork( http->NetHandle ) ;
	}
	http->NetHandle = -1 ;
	
	// POST ���\�b�h�p�̒���������̂��߂ɕ�������m�ۂ��Ă����ꍇ�͉������
	if( http->Param != NULL ) DXFREE( http->Param ) ;
	http->Param = NULL ;

	// �t�@�C�������o���������ꍇ�A�t�@�C���n���h�����������
	if( http->FilePoint != NULL ) CloseHandle( http->FilePoint ) ;
	http->FilePoint = NULL ;

	// �������ʂ��Z�b�g����
	if( http->Result == -1 ) http->Result = Result ;
	if( http->Error == -1 ) http->Error = Error ;

	// �������ǂݍ��݂������ꍇ�œǂݍ��݂��������Ă��Ȃ��ꍇ�A�m�ۂ������������������
	if( http->BufferPoint != NULL && http->Result != HTTP_RES_COMPLETE )
	{
		DXFREE( *http->BufferPoint ) ;
		*http->BufferPoint = NULL ;
	}

	// �I��
	return 0 ;
}

// HTTP �̏������I�����A�n���h�����������
extern int NS_HTTP_Close( int HttpHandle )
{
	HTTPDATA *http ;
	
	if( HttpHandle >= MAX_HTTPHANDLE_NUM ) return -1 ;
	http = HttpData.Data[HttpHandle] ;
	if( http == NULL ) return -1 ;

	// HTTP �̏����Ɏg�p�������\�[�X���������
	HTTP_ReleaseResource( HttpHandle, HTTP_RES_COMPLETE ) ;

	// HTTP �n���h���̉��
	HTTP_DelHandle( HttpHandle ) ;

	// �I��
	return 0 ;	
}

// �S�Ẵn���h���ɑ΂��� HTTP_Close ���s��
extern int NS_HTTP_CloseAll( void )
{
	HTTPDATA **http ;
	int Num, i, j ;

	// HTTP �n���h����������������牽�����Ȃ�
	if( HttpData.Num == 0 ) return 0 ;

	// �S�Ă� HTTP �n���h�����������
	http = HttpData.Data ;
	Num = HttpData.Num ;
	for( i = 0, j = 0 ; i < Num ; j ++ )
	{
		if( http[j] == NULL ) continue ;
		NS_HTTP_Close( j ) ;
		i ++ ;
	}

	// �I��
	return 0 ;
}

// HTTP �̏������s��
static int HTTP_Process( int HttpHandle )
{
	HTTPDATA *http = HttpData.Data[HttpHandle] ;
	int res1, res2 ;
	
	if( http == NULL ) return -1 ;
	
	// HTTP �̏��������炩�̌`�ŏI�����Ă����牽�������I��
	if( http->Result != HTTP_RES_NOW ) return 0;

	// �ؒf����Ă����璆�~
	res1 = NS_GetNetWorkAcceptState( http->NetHandle ) ;
	res2 = NS_GetNetWorkDataLength( http->NetHandle ) ;
	if( res1 == 0 && res2 == 0 )
	{
		HTTP_ReleaseResource( HttpHandle, HTTP_RES_ERROR, HTTP_ERR_LOST ) ;
		return 0 ;
	}
	
	// �s�������ɂ���ď����𕪊�
	switch( http->Process )
	{
	case HTTP_PR_GET :	// GET ���\�b�h�̏���
		return HTTP_ProcessForGET( HttpHandle ) ;
		
	case HTTP_PR_POST :	// POST ���\�b�h�̏���
		return HTTP_ProcessForPOST( HttpHandle ) ;
		
	case HTTP_PR_HEAD :	// HEAD ���\�b�h�̏���
		return HTTP_ProcessForHEAD( HttpHandle ) ;
	}

	// �I��
	return 0 ;
}

// HTTP_Proess ��S�Ẵn���h���ɑ΂��čs��
static int HTTP_ProcessAll( void )
{
	HTTPDATA **http ;
	int i, j, Num ;

	// HTTP �̏����������s���Ă��Ȃ������牽�����Ȃ�
	if( HttpData.Num == 0 ) return 0 ;

	// �S�Ă� HTTP �n���h������������
	http = HttpData.Data ;
	Num = HttpData.Num ;
	for( i = 0, j = 0 ; i < Num ; j ++ )
	{
		if( http[j] == NULL ) continue ;
		HTTP_Process( j ) ;
		i ++ ;
	}

	// �I��
	return 0 ;
}

// HTTP �� GET ���\�b�h�̏���������
static	int			HTTP_ProcessForGET( int HttpHandle )
{
	HTTPDATA *http = HttpData.Data[HttpHandle] ;
	char str[512]/*, buf[128]*/ ;
	int i ;
	
	if( http == NULL ) return -1 ;
	
	// ��Ԃɂ���ď����𕪊�
	switch( http->Step ) 
	{
	case 0 :		// GET ���b�Z�[�W���z�X�g�ɑ���
		_SNPRINTF( str, sizeof( str ), "GET %s HTTP/1.0\r\nHost: %s:%d\r\nConnection: close\r\n\r\n", http->Path, http->Host, http->Port ) ;
		NS_NetWorkSend( http->NetHandle, str, lstrlenA( str ) ) ;
		
		http->Step ++ ;

		
	case 1 :		// �ڑ��ɐ������Ȃ�������I��
		if( NS_fgetsForNetHandle( http->NetHandle, str ) == -1 ) break ;
//		sscanf( str, "%s%d", buf, &i ) ;
		i = HTTP_GetResult( str ) ;
		if( i != 200 )
		{
			int err ;
		
			switch( i )
			{
			case 404 : err = HTTP_ERR_NOTFOUND ; break ;
			case 500 : err = HTTP_ERR_SERVER ; break ;
			default : err = HTTP_ERR_SERVER ; break ;
			}
			
			DXST_ERRORLOGFMT_ADDA(( "\x90\xda\x91\xb1\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd (\x96\xdf\x82\xe8\x92\x6c:%d)"/*@ "�ڑ��Ɏ��s���܂��� (�߂�l:%d)" @*/, i )) ;
			
			HTTP_ReleaseResource( HttpHandle, HTTP_RES_ERROR, err ) ;
			break ;
		}

		http->Step ++ ;
		
	case 2 :		// �w�b�_����͂���
		if( NS_fgetsForNetHandle( http->NetHandle, str ) == -1 ) break ;

		// �w�b�_�I���������玟��
		if( _STRCHR( str, ':' ) == NULL )
		{
			http->Step ++ ;
		}
		else
		{
			// �t�@�C���T�C�Y�̏�񂾂�����T�C�Y���擾����
//			sscanf( str, "%s%d", buf, &i ) ;
			i = HTTP_GetResult( str ) ;
//			if( lstrcmp( buf, "Content-Length:" ) != 0 ) break ;
			if( _STRNCMP( str, "Content-Length:", lstrlenA( "Content-Length:" ) ) != 0 ) break ;
			http->FileSize = i ;

			// �������ɓǂݍ��ޏꍇ�̓������̊m�ۂ��s��
			if( http->SavePlace == HTTP_SV_MEM )
			{
				*http->BufferPoint = DXALLOC( http->FileSize ) ;
				if( *http->BufferPoint == NULL )
				{
					HTTP_ReleaseResource( HttpHandle, HTTP_RES_ERROR, HTTP_ERR_MEMORY ) ;
					break ;
				}
			}
			break ;
		}

	case 3 :		// �f�[�^���擾����
		{
			int GetSize ;
			int res1, res2 ;
			
	GETLOOP :

			// �ۑ���ɂ���ď����𕪊�
			switch( http->SavePlace )
			{
			case HTTP_SV_FILE :		// �t�@�C���̏ꍇ
				{
					static char buf[4096] ;
					DWORD WriteSize ;
					
					// �f�[�^���擾
					GetSize = NS_NetWorkRecv( http->NetHandle, buf, 4096 ) ;
					if( GetSize < 0 ) break ;
					
					// �f�[�^�̏����o��
					WriteFile( http->FilePoint, buf, GetSize, &WriteSize, NULL ) ;
				}
				break ;
				
			case HTTP_SV_MEM :		// �������̏ꍇ
				{
					// �f�[�^���擾
					GetSize = NS_NetWorkRecv( http->NetHandle, ( char * )*http->BufferPoint + http->RecvFileSize, http->FileSize - http->RecvFileSize ) ;
					if( GetSize < 0 ) break ;
				}
				break ;
			}
			if( GetSize < 0 ) break ;
			
			// �擾�������Z
			http->RecvFileSize += GetSize ;

			// �_�E�����[�h�I�����m
			res1 = NS_GetNetWorkAcceptState( http->NetHandle ) ;
			res2 = NS_GetNetWorkDataLength( http->NetHandle ) ;
			if( ( http->FileSize == -1 && ( res1 != 0 || res2 != 0 ) ) || 
				( http->FileSize != -1 && http->RecvFileSize < http->FileSize ) )
			{
				if( GetSize != 0 ) goto GETLOOP ;
				break ;
			}
		}
		http->Step ++ ;

	case 4 :	// �I��
		HTTP_ReleaseResource( HttpHandle, HTTP_RES_COMPLETE ) ;
		break ;
	}
	
	// �I��
	return 0 ;
}

// HTTP �� POST ���\�b�h�̏���������
static	int			HTTP_ProcessForPOST( int HttpHandle )
{
	HTTPDATA *http = HttpData.Data[HttpHandle] ;
	char str[256]/*, buf[128]*/ ;
	int i ;
	
	if( http == NULL ) return -1 ;
	
	// ��Ԃɂ���ď����𕪊�
	switch( http->Step ) 
	{
	case 0 :		// POST ���b�Z�[�W���z�X�g�ɑ���

		_SNPRINTF( str, sizeof( str ), "POST %s HTTP/1.0\nHost: %s:%d\nContent-Length: %d\n\n", http->Path, http->Host, http->Port, http->ParamLength ) ;
		NS_NetWorkSend( http->NetHandle, str, lstrlenA( str ) ) ;
		NS_NetWorkSend( http->NetHandle, http->Param, http->ParamLength ) ;

//		NS_NetWorkSend( http->NetHandle, "\n", 1 ) ;
/*
		_SNPRINTF( str, sizeof( str ), "GET %s?%s HTTP/1.0\nHost: %s:%d\n\n", http->Path, http->Param, http->Host, http->Port ) ;
		DXST_ERRORLOG_ADDA( str ) ;
		NS_NetWorkSend( http->NetHandle, str, lstrlenA( str ) ) ;
*/		
		http->Step ++ ;

		
	case 1 :		// �ڑ��ɐ������Ȃ�������I��
		if( NS_fgetsForNetHandle( http->NetHandle, str ) == -1 ) break ;
//		sscanf( str, "%s%d", buf, &i ) ;
		i = HTTP_GetResult( str ) ;
		if( i != 200 )
		{
			int err ;
		
			switch( i )
			{
			case 404 : err = HTTP_ERR_NOTFOUND ; break ;
			case 500 : err = HTTP_ERR_SERVER ; break ;
			default : err = HTTP_ERR_SERVER ; break ;
			}
			HTTP_ReleaseResource( HttpHandle, HTTP_RES_ERROR, err ) ;
			break ;
		}

		http->Step ++ ;
		
	case 2 :		// �w�b�_����͂���
		if( NS_fgetsForNetHandle( http->NetHandle, str ) == -1 ) break ;

		// �w�b�_�I���������玟��
		if( _STRCHR( str, ':' ) == NULL )
		{
			http->Step ++ ;
		}
		else
		{
			// �t�@�C���T�C�Y�̏�񂾂�����T�C�Y���擾����
//			sscanf( str, "%s%d", buf, &i ) ;
			i = HTTP_GetResult( str ) ;
//			if( lstrcmp( buf, "Content-Length:" ) != 0 ) break ;
			if( _STRNCMP( str, "Content-Length:", lstrlenA( "Content-Length:" ) ) != 0 ) break ;
			http->FileSize = i ;

			// �������ɓǂݍ��ޏꍇ�̓������̊m�ۂ��s��
			if( http->SavePlace == HTTP_SV_MEM )
			{
				*http->BufferPoint = DXALLOC( http->FileSize ) ;
				if( *http->BufferPoint == NULL )
				{
					HTTP_ReleaseResource( HttpHandle, HTTP_RES_ERROR, HTTP_ERR_MEMORY ) ;
					break ;
				}
			}
			break ;
		}

	case 3 :		// �f�[�^���擾����
		{
			int GetSize ;
			int res1, res2 ;
			
	GETLOOP :

			// �ۑ���ɂ���ď����𕪊�
			switch( http->SavePlace )
			{
			case HTTP_SV_FILE :		// �t�@�C���̏ꍇ
				{
					static char buf[4096] ;
					DWORD WriteSize ;
					
					// �f�[�^���擾
					GetSize = NS_NetWorkRecv( http->NetHandle, buf, 4096 ) ;
					if( GetSize < 0 ) break ;
					
					// �f�[�^�̏����o��
					WriteFile( http->FilePoint, buf, GetSize, &WriteSize, NULL ) ;
				}
				break ;
				
			case HTTP_SV_MEM :		// �������̏ꍇ
				{
					// �f�[�^���擾
					GetSize = NS_NetWorkRecv( http->NetHandle, ( char * )*http->BufferPoint + http->RecvFileSize, http->FileSize - http->RecvFileSize ) ;
					if( GetSize < 0 ) break ;
				}
				break ;
			}
			if( GetSize < 0 ) break ;
			
			// �擾�������Z
			http->RecvFileSize += GetSize ;

			// �_�E�����[�h�I�����m
			res1 = NS_GetNetWorkAcceptState( http->NetHandle ) ;
			res2 = NS_GetNetWorkDataLength( http->NetHandle ) ;
			if( ( http->FileSize == -1 && ( res1 != 0 || res2 != 0 ) ) || 
				( http->FileSize != -1 && http->RecvFileSize < http->FileSize ) )
			{
				if( http->FileSize != -1 && http->RecvFileSize < http->FileSize && GetSize != 0 ) goto GETLOOP ;
				break ;
			}
		}
		http->Step ++ ;

	case 4 :	// �I��
		HTTP_ReleaseResource( HttpHandle, HTTP_RES_COMPLETE ) ;
		break ;
	}
	
	// �I��
	return 0 ;
}

// HTTP �� HEAD ���\�b�h�̏���������
static	int			HTTP_ProcessForHEAD( int HttpHandle )
{
	HTTPDATA *http = HttpData.Data[HttpHandle] ;
	char str[256]/*, buf[128]*/ ;
	int i ;
	
	if( http == NULL ) return -1 ;

	// ��Ԃɂ���ď����𕪊�
	switch( http->Step ) 
	{
	case 0 :		// HEAD ���b�Z�[�W���z�X�g�ɑ���
		_SNPRINTF( str, sizeof( str ), "HEAD %s HTTP/1.0\n\n", http->Path ) ;
		NS_NetWorkSend( http->NetHandle, str, lstrlenA( str ) ) ;
		
		http->Step ++ ;
		
	case 1 :		// �ڑ��ɐ������Ȃ�������I��
		if( NS_fgetsForNetHandle( http->NetHandle, str ) == -1 ) break ;
		i = HTTP_GetResult( str ) ;
//		sscanf( str, "%s%d", buf, &i ) ;
		if( i != 200 )
		{
			int err ;
		
			switch( i )
			{
			case 404 : err = HTTP_ERR_NOTFOUND ; break ;
			case 500 : err = HTTP_ERR_SERVER ; break ;
			default : err = HTTP_ERR_SERVER ; break ;
			}
			HTTP_ReleaseResource( HttpHandle, HTTP_RES_ERROR, err ) ;
			break ;
		}

		http->Step ++ ;
		
	case 2 :		// �w�b�_����͂���
		// �����񂪗��Ă��Ȃ������牽�������I��
		if( NS_fgetsForNetHandle( http->NetHandle, str ) == -1 ) break ;

		// �t�@�C���T�C�Y�̏�񂾂�����T�C�Y���擾����
//		sscanf( str, "%s%d", buf, &i ) ;
//		if( lstrcmp( buf, "Content-Length:" ) != 0 ) break ;
		i = HTTP_GetResult( str ) ;
		if( _STRNCMP( str, "Content-Length:", lstrlenA( "Content-Length:" ) ) != 0 ) break ;
		http->FileSize = i ;

		// �t�@�C���T�C�Y�̎擾���o������v���Z�X����
		http->Step ++ ;

	case 3 :	// �I��
		HTTP_ReleaseResource( HttpHandle, HTTP_RES_COMPLETE ) ;
		break ;
	}
	
	// �I��
	return 0 ;
}

// HTTP �����̌��݂̏�Ԃ𓾂�( HTTP_RES_COMPLETE �� )
extern int NS_HTTP_GetState( int HttpHandle )
{
	HTTPDATA *http ;
	
	if( HttpHandle >= MAX_HTTPHANDLE_NUM ) return -1 ;
	http = HttpData.Data[HttpHandle] ;
	if( http == NULL ) return -1 ;

	// ��Ԃ�Ԃ�
	return http->Result ;
}

// HTTP �����ŃG���[�����������ꍇ�A�G���[�̓��e�𓾂�( HTTP_ERR_NONE �� )
extern int NS_HTTP_GetError( int HttpHandle )
{
	HTTPDATA *http ;
	
	if( HttpHandle >= MAX_HTTPHANDLE_NUM ) return -1 ;
	http = HttpData.Data[HttpHandle] ;
	if( http == NULL ) return -1 ;

	// �G���[��Ԃ�Ԃ�
	return http->Error ;
}


// HTTP �����őΏۂƂȂ��Ă���t�@�C���̃T�C�Y�𓾂�( �߂�l: -1 = �G���[�E�Ⴕ���͂܂��t�@�C���̃T�C�Y���擾���Ă��Ȃ�  0�ȏ� = �t�@�C���̃T�C�Y )
extern int NS_HTTP_GetDownloadFileSize( int HttpHandle )
{
	HTTPDATA *http ;
	
	if( HttpHandle >= MAX_HTTPHANDLE_NUM ) return -1 ;
	http = HttpData.Data[HttpHandle] ;
	if( http == NULL ) return -1 ;

	return http->FileSize ;
}

// HTTP �����Ŋ��Ƀ_�E�����[�h�����t�@�C���̃T�C�Y���擾����
extern int NS_HTTP_GetDownloadedFileSize( int HttpHandle ) 
{
	HTTPDATA *http ;
	
	if( HttpHandle >= MAX_HTTPHANDLE_NUM ) return -1 ;
	http = HttpData.Data[HttpHandle] ;
	if( http == NULL ) return -1 ;

	return http->RecvFileSize ;
}

// fgets �̃l�b�g���[�N�n���h����( -1:�擾�ł��� 0:�擾�ł��� )
extern int NS_fgetsForNetHandle( int NetHandle, char *strbuffer )
{
	char str[256], *p ;
	int length ;

	// ���ݎ擾���Ă���f�[�^���擾����
	_MEMSET( str, 0, 255 ) ;
	length = NS_NetWorkRecvToPeek( NetHandle, str, 255 ) ;
	if( length <= 0 ) return -1 ;
	str[length] = '\0' ;
	
	// ���s�������܂܂�Ă��Ȃ������牽�������I��
	p = _STRCHR( str, '\n' ) ;
	if( p == NULL ) return -1 ;
	
	// �܂܂�Ă����炻���܂ł̕������Ԃ�
	*p = '\0' ;
	_STRCPY( strbuffer, str ) ;
	
	// �擾�����������o�b�t�@����f�[�^������
	NS_NetWorkRecv( NetHandle, str, ( p - str ) + 1 ) ;

	// �I��
	return 0 ;
}

// �t�q�k����͂���
extern int NS_URLAnalys( const char *URL, char *HostBuf, char *PathBuf, char *FileNameBuf, int *PortBuf )
{
	char tempHost[128], tempPath[128], tempFileName[128] ;
	int tempPort ;
	char *hp, *p, *pt ;
	
	if( HostBuf == NULL ) HostBuf = tempHost ;
	if( PathBuf == NULL ) PathBuf = tempPath ;
	if( FileNameBuf == NULL ) FileNameBuf = tempFileName ;
	if( PortBuf == NULL ) PortBuf = &tempPort ;
	
	// �v���g�R�����ȍ~�̕������擾����
	// �Ȃ�������G���[
	hp = _STRSTR( ( char * )URL, "//" ) ;
	if( hp == NULL ) return -1 ;
	hp += 2 ;
	
	// �p�X�̐擪���擾����
	p = _STRCHR( hp, '/' ) ;
	if( p == NULL )
	{
		// �p�X���Ȃ��ꍇ�̏���
		_STRCPY( HostBuf, hp ) ;
		_STRCPY( PathBuf, "/" ) ;
	}
	else
	{
		// �p�X������ꍇ�̏���
		_MEMCPY( HostBuf, hp, p - hp ) ;
		HostBuf[p - hp] = '\0' ;
		_STRCPY( PathBuf, p ) ;
	}
	
	// �|�[�g�̎擾
	pt = _STRCHR( HostBuf, ':' ) ;
	if( pt == NULL )
	{
		*PortBuf = -1 ;
	}
	else
	{
		*PortBuf = _ATOI( pt + 1 ) ;
		if( *PortBuf <= 0 ) *PortBuf = -1 ;
		*pt = '\0' ;
	}

	// �t�@�C�����̎擾
	{
		char *p ;

		p = _STRRCHR( PathBuf, '/' ) ;
		if( p[1] == '\0' )
		{
			_STRCPY( FileNameBuf, "index.htm" ) ;
		}
		else
		{
			_STRCPY( FileNameBuf, p + 1 ) ;
		}
	}

	// ����I��
	return 0 ;
}


// HTTP �ɓn���Ȃ��L�����g��ꂽ�������n����悤�ȕ�����ɕϊ�����
extern int NS_URLConvert( char *URL, int ParamConvert, int NonConvert )
{
	int len, count ;
	char *Number = "0123456789ABCDEF" ;
	unsigned char code ;

	count = 0 ;
	len = lstrlenA( URL ) + 1 ;
	while( *URL != '\0' )
	{
		code = (unsigned char)*URL ;
		if( code == ' ' )
		{
			if( NonConvert == FALSE ) *URL = '+' ;
			URL ++ ;
			count ++ ;
		}
		else
		if( ( ( code < 'a' || code > 'z' ) && ( code < 'A' || code > 'Z' ) && ( code < '0' || code > '9' ) &&
			  code != '.' && code != '_' && code != '-' && code != '*' && code != '/' && code != ':' && code != '%' ) || 
			( ParamConvert == TRUE && ( code == '/' || code == ':' || code == '%' ) ) )
		{
			if( NonConvert == FALSE )
			{
				if( ParamConvert == FALSE && /*_mbclen( (unsigned char *)URL ) == 2*/ CheckMultiByteChar( *URL, _GET_CHARSET() ) == TRUE )
				{
					_MEMMOVE( URL + 6, URL + 2, len - 2 ) ;
					code = URL[0] ;
					URL[0] = '%' ;
					URL[1] = Number[code >> 4] ;
					URL[2] = Number[code & 0xf] ;
					
					URL += 3 ;
					
					code = URL[0] ;
					URL[0] = '%' ;
					URL[1] = Number[code >> 4] ;
					URL[2] = Number[code & 0xf] ;
					
					URL += 3 ;
				}
				else
				{
					_MEMMOVE( URL + 3, URL + 1, len - 1 ) ;
					code = URL[0] ;
					URL[0] = '%' ;
					URL[1] = Number[code >> 4] ;
					URL[2] = Number[code & 0xf] ;
					
					URL += 3 ;
				}
			}
			else
			{
				URL ++ ;
			}

			count += 3 ;
		}
		else
		{
			URL ++ ;
			count ++ ;
		}
		len -- ;
	}
	
	// �I��
	return count ;
}

// HTTP �p�p�����[�^���X�g�����̃p�����[�^��������쐬����( �߂�l:  -1 = �G���[  0�ȏ� = �p�����[�^�̕�����̒��� )
extern int NS_URLParamAnalysis( char **ParamList, char **ParamStringP )
{
	int i, len ;
	char *ParamString, *p ;

	if( ParamList == NULL ) return -1 ;

	// �S�Ẵp�����[�^��ϊ�������̕����񒷂𓾂�
	len = 0 ;
	for( i = 0 ; ParamList[i] != NULL ; i ++ )
	{
		p = _STRCHR( ParamList[i], '=' ) ;
		len += p - ParamList[i] + 1 ;
		if( p == NULL ) return -1 ;
		len += NS_URLConvert( p + 1, TRUE, TRUE ) + 1 ;
	}
	len -- ;

	// �p�����[�^������Ȃ������ꍇ�̏���
	if( len == 0 )
	{
		if( ParamStringP != NULL ) *ParamStringP = NULL ;
		return 0 ;
	}

	// �������̊m��
	ParamString = ( char * )DXALLOC( len + 1 ) ;
	if( ParamString == NULL ) return -1 ;
	_MEMSET( ParamString, 0, len + 1 ) ;

	// �S�Ẵp�����[�^��ϊ�����������𓾂�
	p = ParamString ;
	for( i = 0 ; ParamList[i] != NULL ; i ++ )
	{
		_STRCPY( p, ParamList[i] ) ;
		p = _STRCHR( p, '=' ) + 1 ;
		p += NS_URLConvert( p, TRUE, FALSE ) ;
		*p = '&' ;
		p ++ ;
	}
	p -- ;
	*p = '\0' ;

	// �|�C���^��ۑ�
	if( ParamStringP != NULL ) *ParamStringP = ParamString ;

	// �I��
	return len ;
}


// �T�C�Y�Ɍ��������P�ʂŕ\����������Ƃ��̒P�ʂ�\���ꕶ����Ԃ�
static	char		GetDataSizeString( int ByteSize, char *SizeString )
{
	const int KByte = 1024, MByte = KByte * 1024, GByte = MByte * 1024 ;

	if( ( double )ByteSize > GByte * 0.8 )
	{
		_SPRINTF( SizeString, "%.1f", ( double )ByteSize / GByte ) ;
		return 'G' ;
	}
	else
	if( ( double )ByteSize > MByte * 0.8 )
	{
		_SPRINTF( SizeString, "%.1f", ( double )ByteSize / MByte ) ;
		return 'M' ;
	}
	else
	{
		_SPRINTF( SizeString, "%.1f", ( double )ByteSize / KByte ) ;
		if( _STRCMP( SizeString, "-0.0" ) == 0 ) _STRCPY( SizeString, "0.0" ) ;
		return 'k' ;
	}

	// �����ɗ��邱�Ƃ͖���	
	return 0 ;
}

// �~���b�̎��Ԓ����玞�Ԓ��ɍ������P�ʂ̕�����ɕϊ�����
static	int			GetTimeLengthString( int MillSec, char *LengthString )
{
	int Sec = 1000, Min = Sec * 60, Hour = Min * 60, Day = Hour * 24 ;
	int DayFlag = FALSE, HourFlag = FALSE, MinFlag = FALSE ;
	
	LengthString[0] = '\0' ;
	if( MillSec >= Day )
	{
		_SPRINTF( LengthString, "%d\x93\xfa "/*@ "%d�� " @*/, MillSec / Day ) ;
		MillSec %= Day ;
		LengthString += lstrlenA( LengthString ) ;
		
		DayFlag = TRUE ;
	}

	if( MillSec >= Hour )
	{
		_SPRINTF( LengthString, "%d\x8e\x9e\x8a\xd4 "/*@ "%d���� " @*/, MillSec / Hour ) ;
		MillSec %= Hour ;
		LengthString += lstrlenA( LengthString ) ;
		
		HourFlag = TRUE ;
	}

	if( MillSec >= Min && DayFlag == FALSE )
	{
		_SPRINTF( LengthString, "%d\x95\xaa "/*@ "%d�� " @*/, MillSec / Min ) ;
		MillSec %= Hour ;
		LengthString += lstrlenA( LengthString ) ;
		
		MinFlag = TRUE ;
	}

	if( DayFlag == FALSE && HourFlag == FALSE )
	{
		_SPRINTF( LengthString, "%d\x95\x62"/*@ "%d�b" @*/, MillSec / Sec ) ;
		MillSec %= Sec ;
		LengthString += lstrlenA( LengthString ) ;
	}
	
	// �I��
	return 0 ;
}

#endif

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_NETWORK
