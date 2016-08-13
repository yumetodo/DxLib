// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�ʐM�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXNETWORK_H__
#define __DXNETWORK_H__

#include "DxCompileConfig.h"

#ifndef DX_NON_NETWORK

// �C���N���[�h ------------------------------------------------------------------
#include "DxLib.h"
#include "DxHandle.h"
#include "DxRingBuffer.h"
#include "DxThread.h"
#include "DxFile.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

#define MAX_HTTPHANDLE_NUM			(100)				// �����Ɏg�p�ł��� HTTP �n���h���̐�

// HTTP �̏�Ԉꗗ
enum HTTP_STATE
{
	HTTP_ST_CONNECT = 0,				// �ڑ���
	HTTP_ST_GET = 1,					// GET �Ńt�@�C����M��
	HTTP_ST_HEAD = 2,					// HEAD �Ńw�b�_����M��
	HTTP_ST_POST = 3,					// POST �Ŏ�ɂb�f�h�Ȃǂ���o�͎�M��
} ;

// HTTP �̏����ꗗ
enum HTTP_PROC
{
	HTTP_PR_GET,						// GET �Ńt�@�C���擾
	HTTP_PR_POST,						// POST �Ńt�@�C���擾
	HTTP_PR_HEAD,						// HEAD �Ńw�b�_�擾
} ;

// �ۑ���ꗗ
enum HTTP_SAVEPLACE
{
	HTTP_SV_FILE = 0,					// �ۑ���̓t�@�C��
	HTTP_SV_MEM = 1,					// �ۑ���̓�����
} ;

// FTP �̏�Ԉꗗ
enum FTP_STATE
{
	FTP_ST_CONNECT = 0,					// �ڑ���
	FTP_ST_LOGIN_NAME = 1,				// ���O�C����(���[�U�[�l�[�����͒�)
	FTP_ST_LOGIN_PASS = 2,				// ���O�C����(�p�X���[�h���͒�)
	FTP_ST_TYPE_A = 3,					// TYPE A ���M��
	FTP_ST_TYPE_B = 4,					// TYPE B ���M��
	FTP_ST_SIZE = 5,					// �T�C�Y�擾��
	FTP_ST_QUIT = 6,					// �I����
	FTP_ST_PASV = 7,					// Passive ���[�h�ȍ~��
	FTP_ST_RETR = 8,					// �擾�������t�@�C�����𑗐M��
	FTP_ST_STOR = 9,					// ���M�������t�@�C�����𑗐M��
	FTP_ST_DATA_GET = 10,				// �f�[�^��M��
	FTP_ST_DATA_SET = 11,				// �f�[�^���M��
	FTP_ST_CWD = 12,					// �f�B���N�g���ύX��
} ;

// FTP �̏����ꗗ
enum FTP_PROC
{
	FTP_PR_LOGIN = 0,					// ���O�C��
	FTP_PR_IDLE = 1,					// �ҋ@���
	FTP_PR_CHDIR = 2,					// �f�B���N�g���ύX
	FTP_PR_FILE_SET = 3,				// �t�@�C�����M
	FTP_PR_FILE_GET = 4,				// �t�@�C���擾
} ;

// �\���̒�` --------------------------------------------------------------------

// WinSock2.0 �Ŏg�p�����`�ƍ\����

struct _addrinfo
{
	int						ai_flags ;							// AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST
	int						ai_family ;							// PF_xxx
	int						ai_socktype ;						// SOCK_xxx
	int						ai_protocol ;						// 0 or IPPROTO_xxx for IPv4 and IPv6
	size_t					ai_addrlen ;						// Length of ai_addr
	char					*ai_canonname ;						// Canonical name for nodename
	struct sockaddr			*ai_addr ;							// Binary address
	struct _addrinfo		*ai_next ;							// Next structure in linked list
} ;

struct _sockaddr_in6 
{
	short					sin6_family ;						// AF_INET6
	unsigned short			sin6_port ;							// Transport level port number
	unsigned long			sin6_flowinfo ;						// IPv6 flow information
	IPDATA_IPv6				sin6_addr ;							// IPv6 address
	unsigned long			sin6_scope_id ;						// set of interfaces for a scope
} ;

// ConnectNetWorkBase �Ŏg�p����O���[�o���f�[�^��Z�߂�����
struct CONNECTNETWORK_GPARAM
{
	int						NonUseDXProtocolFlag ;				// �c�w���C�u�����̃v���g�R�����g�p���Ȃ����ǂ����̃t���O
	int						NonCloseAfterLostFlag ;				// �ڑ����؂ꂽ���Ɏ����I�Ƀn���h����������Ȃ����ǂ����̃t���O( TRUE:����ɉ���͂��Ȃ�  FALSE:����ɉ������ )
} ;

// �\�P�b�g�P�̃f�[�^�\����
struct SOCKETDATA
{
	HANDLEINFO				HandleInfo ;						// �n���h�����ʃf�[�^

	int						IsUDP ;								// �t�c�o�ڑ����ǂ���
	int						UseFlag ;							// �f�[�^���g�p�����A�t���O
	int						IsIPv6 ;							// IPv6���g�p���Ă��邩�ǂ���

	int						ErrorFlag ;							// �d��ȃG���[�������������t���O( TRUE:��������  FALSE:���Ă��Ȃ� )
	int						PreConnectionFlag ;					// �ڑ��O�t���O( TRUE:�ڑ��O  FALSE:�ڑ��� )
	int						ConnectionFlag ;					// �ڑ����Ă��邩�t���O( TRUE:���Ă���  FALSE:���Ă��Ȃ� )
	int						ConnectionLostFlag ;				// ���C�u�������[�U�[���ؒf���m�F�������t���O�AAcceptedFlag�� FALSE �̎��̂ݗL��( TRUE:����  FALSE:���Ă��Ȃ� )
	int						AccepteConfirFlag ;					// ���C�u�������[�U�[���ڑ����m�F�������t���O( TRUE:����  FALSE:���Ă��Ȃ� )
	int						AcceptFlag ;						// �ڑ����ꂽ�̂��A�����̂��A�t���O( TRUE:���ꂽ  FALSE:���� )

	int						DXProtocolFlag  ;					// �c�w���C�u�����̃v���g�R���ŒʐM���邩�ǂ����̃t���O( TRUE:�T�C�Y�w�b�_��t���� FALSE:WinSock�����̂܂܎g�� )
	int						CloseAfterLostFlag ;				// �ؒf����Ƀn���h����������邩�ǂ����̃t���O( TRUE:����ɉ��  FALSE:���[�U�[���� CloseNetWork �����܂ŉ�����Ȃ� )

//	int						ID ;								// �G���[�`�F�b�N�p�h�c
	SOCKET					Socket ;							// �\�P�b�g���ʔԍ�

	int						RecvComDataVol ;					// �����o�b�t�@�ɂv�����r�������o�b�t�@�̓��e�����ׂē���Ȃ��ꍇ�A�ǂ܂Ȃ��Ă͂Ȃ�Ȃ��c��e��
	RINGBUF					RecvBufferToUserR ;					// ���C�u�������p�҂Ɍ��J�����M�f�[�^�̃����O�o�b�t�@
	int						RecvComDataOriginalVol ;			// ��x�̎�M�ő��M����Ă����f�[�^�e�ʁARecvComDataVol �� 0 �ɂȂ������� RecvBufferToUserOpenSize �ɉ��Z�����
	int						RecvBufferToUserOpenSize ;			// ���C�u�������p�҂Ɍ��J�����M�f�[�^�̃T�C�Y

	RINGBUF					SendBufferR ;						// ���M�o�b�t�@�p�����O�f�[�^
	int						SendComDataVol ;					// ��x�ɑ��M�ł��Ȃ������ꍇ�̎c��f�[�^��
	int						SendComDataComVol ;					// ��x�ɑ��M����f�[�^�ʂ̑��M����x�ɑ��M�ł��Ȃ������ꍇ�̎c��e��

	IPDATA					AcceptedIP ;						// �ڑ���̂h�o(�ؒf����L��)
	IPDATA_IPv6				AcceptedIP_IPv6 ;					// �ڑ���̂h�o(�ؒf����L��)
	unsigned int			Port ;								// �ڑ���̃|�[�g

	int						UDPReadFlag ;						// ��M�f�[�^�����݂��邩�A�t���O( TRUE:���݂���  FALSE:���Ȃ� )
	int						UDPWriteFlag ;						// �f�[�^���M���\�ȏ�Ԃ��A�t���O( TRUE:�\  FALSE:�s�\ )

//	HANDLELIST				List ;								// ���X�g�����p�f�[�^
} ;

// Winsockets�֌W�\����
struct WINSOCKDATA
{
	int						ListenHandle ;						// �ڑ��҂������p�n���h��
/*
	SOCKETDATA				*CSocket[ MAX_SOCKET_NUM + 1 ] ;	// �R�l�N�g�p�\�P�b�g�f�[�^
	HANDLELIST				SocketListFirst ;					// �\�P�b�g���X�g�̐擪
	HANDLELIST				SocketListLast ;					// �\�P�b�g���X�g�̏I�[
*/
//	int						SocketNum ;							// �\�P�b�g�̐�
//	int						HandleID ;							// �n���h���Ɋ��蓖�Ă�h�c
	int						InitializeFlag ;					// �������t���O
//	int						MaxSockets ;						// �ő�ڑ���
	int						MyIPNum ;							// �����̂h�o�A�h���X�̐�
	IPDATA					*MyIP ;								// �����̂h�o�A�h���X
	int						TimeOutWait ;						// �^�C���A�E�g����܂ł̎���(�~���b�P��)
	HWND					ParentWindow ;						// �e�E�C���h�E
	HWND					MessageWindow ;						// �E�C���h�E�n���h��
	int						NonUseDXProtocolFlag ;				// �c�w���C�u�����̃v���g�R�����g�p���Ȃ����ǂ����̃t���O
	int						NonCloseAfterLostFlag ;				// �ڑ����؂ꂽ���Ɏ����I�Ƀn���h����������Ȃ����ǂ����̃t���O( TRUE:����ɉ���͂��Ȃ�  FALSE:����ɉ������ )
	int						DestroyFlag ;						// ���b�Z�[�W�E�C���h�E�� WM_DESTROY ���󂯂����ǂ����̃t���O

//	DX_CRITICAL_SECTION		CriticalSection ;					// �f�[�^�A�N�Z�X���p�N���e�B�J���Z�N�V����

	HANDLE					ProcessNetMessageThreadHandle ;		// ProcessNetMessage ���Ђ�����ĂтÂ���v���Z�X�̃n���h��
	DWORD					ProcessNetMessageThreadID ;			// ProcessNetMessage ���Ђ�����ĂтÂ���v���Z�X�̂h�c
	DWORD					ProcessNetMessageThreadExitFlag ;	// ProcessNetMessage ���Ђ�����ĂтÂ���v���Z�X���I�����ɗ��Ă�t���O
	DWORD					ProcessNetMessageThreadEndRequest ;	// ProcessNetMessage ���Ђ�����ĂтÂ���v���Z�X�̏I�����N�G�X�g�t���O
} ;

// HTTP�ʐM�֌W�\����
struct HTTPDATA
{
	HTTP_PROC				Process ;							// ����
	HTTP_STATE				State ;								// ���
	int						Step ;								// ���ڏ��
	int 					Counter ;							// �ėp�J�E���^
	int						Result ;							// ��������
	int						Error ;								// �G���[�̏ꍇ�̃G���[���e

	int						NetHandle ;							// HTTP�T�[�o�Ƃ̒ʐM�Ɏg�p����l�b�g���[�N�n���h��

	HTTP_SAVEPLACE			SavePlace ;							// �ۑ��ꏊ
	HANDLE					FilePoint ;							// ��M�悪�t�@�C���̎��Ɏg�p����t�@�C���|�C���^
	void					**BufferPoint ;						// ��M�悪�������̎��Ɏg�p����|�C���^�̃|�C���^

	int						FileSize ;							// �t�@�C���T�C�Y
	int						RecvFileSize ;						// ��M�����f�[�^�̃T�C�Y
	char					FileName[128] ;						// ��M����t�@�C����
	
	char					Host[128] ;							// �ڑ���̃z�X�g��
	char 					Path[256] ;							// ��M����t�@�C���̃z�X�g��̃p�X
	char					*Param ;							// POST ���\�b�h�Œ����p�����[�^��n���ۂ̕����񂪊i�[����Ă���|�C���^
	int						ParamLength ;						// �p�����[�^������̒���
	IPDATA					HostIP ;							// �ڑ���z�X�g�̂h�o
	int						Port ;								// �ڑ���|�[�g
} ;

// HTTP �֌W�Ǘ��f�[�^
struct HTTPSYS
{
	HTTPDATA				*Data[MAX_HTTPHANDLE_NUM] ;			// �����p�f�[�^�ւ̃|�C���^�̔z��
	int						Num ;								// �����p�f�[�^�̐�

	int						UseProxy ;							// �v���L�V���g�p���邩�ǂ���
	char					ProxyHost[256] ;					// �v���L�V�̃z�X�g��
	int						ProxyPort ;							// �v���L�V�̃|�[�g
//	int						NonUseIEProxy ;						// �h�d�̃v���L�V�T�[�o�[�ݒ���g�p���Ȃ����ǂ����̃t���O
} ;

// FTP�ʐM�֌W�\����
struct FTPDATA
{
	int						UseFlag ;							// �g�p���t���O
	
	FTP_PROC				Process ;							// ����
	FTP_STATE				State ;								// ���

	int						ComNetHandle ;						// �R�}���h�̑���M�Ɏg�p����l�b�g���[�N�n���h��
	int						DatNetHandle ;						// �f�[�^�̑���M�Ɏg�p����l�b�g���[�N�n���h��

	DWORD					Fp ;								// ����M�Ɏg�p����t�@�C���|�C���^
	int						FileSize ;							// �t�@�C���T�C�Y
	int						FilePoint ;							// ����M�����t�@�C���̃T�C�Y
	char					FileName[FILEPATH_MAX] ;			// ����M����t�@�C����
	char					FileDir[FILEPATH_MAX] ;				// ����M����t�@�C�������݂���p�X
} ;

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

extern WINSOCKDATA SockData ;									// �v�����r�������������f�[�^
extern HTTPSYS HttpData ;										// HTTP �֌W�����p�f�[�^

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

extern 	int			InitializeNetWork( HWND WindowHandle = NULL ) ;							// �ʐM�@�\������������
extern 	int			TerminateNetWork( void ) ;												// �ʐM�@�\���I������

extern	int			WinSockProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) ;	// �v�����r���������b�Z�[�W�̃R�[���o�b�N�֐�

extern	int			AcceptNetWork( void ) ;													// �A�N�Z�v�g����

extern	int			InitializeNetworkHandle( HANDLEINFO *HandleInfo ) ;						// �l�b�g���[�N�n���h��������������
extern	int			TerminateNetworkHandle( HANDLEINFO *HandleInfo ) ;						// �l�b�g���[�N�n���h���̌�n�����s��

extern	void		InitConnectNetWorkBaseGParam( CONNECTNETWORK_GPARAM *GParam ) ;			// CONNECTNETWORK_GPARAM ������������
extern	int			ConnectNetWorkBase_UseGParam( CONNECTNETWORK_GPARAM *GParam, IPDATA *IPData_IPv4, IPDATA_IPv6 *IPData_IPv6, int Port, int ASync, int ASyncLoadFlag = FALSE ) ;		// ConnectNetWorkBase �̃O���[�o���f�[�^�ɃA�N�Z�X���Ȃ���
extern	int			NetWorkRecv_UseGParam( int NetHandle,       void *Buffer, int Length, int Peek, int ASyncLoadFlag = FALSE ) ;		// NetWorkRecv �̃O���[�o���f�[�^�ɃA�N�Z�X���Ȃ��o�[�W����
extern	int			NetWorkSend_UseGParam( int NetHandle, const void *Buffer, int Length, int ASyncLoadFlag = FALSE ) ;					// NetWorkSend �̃O���[�o���f�[�^�ɃA�N�Z�X���Ȃ��o�[�W����
extern	int			MakeUDPSocketBase_UseGParam( int IsIPv6, int RecvPort, int ASyncLoadFlag = FALSE ) ;								// MakeUDPSocketBase �̃O���[�o���f�[�^�ɃA�N�Z�X���Ȃ��o�[�W����
extern	int			NetWorkSendUDP_UseGParam( int NetUDPHandle, IPDATA SendIP_IPv4, IPDATA_IPv6 SendIP_IPv6, int SendPort, const void *Buffer, int Length, int ASyncLoadFlag = FALSE ) ;	// NetWorkSendUDP �̃O���[�o���f�[�^�ɃA�N�Z�X���Ȃ��o�[�W����
extern	int			NetWorkRecvUDP_UseGParam( int NetUDPHandle, void *RecvIP, int *RecvPort, void *Buffer, int Length, int Peek, int ASyncLoadFlag = FALSE ) ;						// NetWorkRecvUDP �̃O���[�o���f�[�^�ɃA�N�Z�X���Ȃ��o�[�W����

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_NETWORK

#endif // __DXNETWORK_H__
