// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		��������̓v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "DxInputString.h"

#ifndef DX_NON_INPUTSTRING

// �C���N���[�h ------------------------------------------------------------------
#include "DxLib.h"
#include "DxStatic.h"
#include "DxBaseFunc.h"
#include "DxSystem.h"
#include "DxGraphics.h"
#include "DxFont.h"
#include "DxUseCLib.h"
#include "DxLog.h"
#include "DxMath.h"
#include "Windows/DxWindow.h"
#include "Windows/DxWinAPI.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// ��������̓n���h���̗L�����`�F�b�N
#define KEYHCHK( HAND, KPOINT )																			\
	  ( ( ( (HAND) & DX_HANDLEERROR_MASK ) != 0 ) ||													\
		( ( (HAND) & DX_HANDLETYPE_MASK ) != DX_HANDLETYPE_MASK_KEYINPUT ) ||								\
		( ( (HAND) & DX_HANDLEINDEX_MASK ) >= MAX_INPUT_NUM ) ||										\
		( ( KPOINT = &CharBuf.InputData[ (HAND) & DX_HANDLEINDEX_MASK ] )->UseFlag == FALSE ) ||		\
		( ( (KPOINT)->ID << DX_HANDLECHECK_ADDRESS ) != ( (HAND) & DX_HANDLECHECK_MASK ) ) )


#define CURSORBRINKWAIT				(500)				// �J�[�\���_�ł̊Ԋu

#ifndef WM_IME_REQUEST
#define WM_IME_REQUEST				(0x0288)
#endif
#ifndef IMR_RECONVERTSTRING
#define IMR_RECONVERTSTRING			(0x0004)
#endif
#ifndef SCS_SETRECONVERTSTRING
#define SCS_SETRECONVERTSTRING		(0x00010000)
#endif
#ifndef SCS_QUERYRECONVERTSTRING
#define SCS_QUERYRECONVERTSTRING	(0x00020000)
#endif

// �\���̒�` --------------------------------------------------------------------

typedef struct tagD_RECONVERTSTRING
{
	DWORD dwSize ;
	DWORD dwVersion ;
	DWORD dwStrLen ;
	DWORD dwStrOffset ;
	DWORD dwCompStrLen ;
	DWORD dwCompStrOffset ;
	DWORD dwTargetStrLen ;
	DWORD dwTargetStrOffset ;
} D_RECONVERTSTRING ;

// �e�[�u��-----------------------------------------------------------------------

// ���z�L�[�R�[�h���R���g���[�������R�[�h�ɕϊ����邽�߂̃e�[�u��
char CtrlCode[ 10 ][ 2 ] =
{
	VK_DELETE	,	CTRL_CODE_DEL		,	// �c�d�k�L�[

	VK_LEFT		,	CTRL_CODE_LEFT		,	// ���L�[
	VK_RIGHT	,	CTRL_CODE_RIGHT		,	// ���L�[
	VK_UP		,	CTRL_CODE_UP		,	// ���L�[
	VK_DOWN		,	CTRL_CODE_DOWN		,	// ���L�[

	VK_HOME		,	CTRL_CODE_HOME		,	// �g�n�l�d�L�[
	VK_END		,	CTRL_CODE_END		,	// �d�m�c�L�[
	VK_PRIOR	,	CTRL_CODE_PAGE_UP	,	// �o�`�f�d�@�t�o�L�[
	VK_NEXT		,	CTRL_CODE_PAGE_DOWN	,	// �o�`�f�d�@�c�n�v�m�L�[
	0			,	0
} ;

// �������ϐ��錾 --------------------------------------------------------------

CHARBUFFER CharBuf ;										// �����R�[�h�o�b�t�@

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �����R�[�h�o�b�t�@����֌W
#ifndef DX_NON_KEYEX
static	int			ResetKeyInputCursorBrinkCount( void ) ;										// �L�[���͎��̃J�[�\���_�ŏ����̃J�E���^�����Z�b�g
static	int			SetIMEOpenState( int OpenFlag ) ;											// �h�l�d���g�p��Ԃ�ύX����
static	int			KeyInputSelectAreaDelete( INPUTDATA *Input ) ;								// �L�[���͂̑I��͈͂��폜����
#endif

// �v���O���� --------------------------------------------------------------------

// �����R�[�h�o�b�t�@����֌W

// �����R�[�h�o�b�t�@�֌W�̏�����
extern int InitializeInputCharBuf( void )
{
	int IMEUseFlag ;
	int IMEUseFlag_OSSet ;

	DXST_ERRORLOG_ADDUTF16LE( "\x87\x65\x57\x5b\xb3\x30\xfc\x30\xc9\x30\xd0\x30\xc3\x30\xd5\x30\xa1\x30\x6e\x30\x1d\x52\x1f\x67\x16\x53\x92\x30\x4c\x88\x44\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�����R�[�h�o�b�t�@�̏��������s���܂�... " @*/ ) ; 

	// �[��������
	IMEUseFlag               = CharBuf.IMEUseFlag ;
	IMEUseFlag_OSSet         = CharBuf.IMEUseFlag_OSSet ;
	_MEMSET( &CharBuf, 0, sizeof( CharBuf ) ) ;
	CharBuf.IMEUseFlag       = IMEUseFlag ;
	CharBuf.IMEUseFlag_OSSet = IMEUseFlag_OSSet ;

#ifndef DX_NON_KEYEX
	// �f�t�H���g�F���Z�b�g
	{
		int i ;
		for( i = 0 ; i < DX_KEYINPSTRCOLOR_NUM ; i ++ )
		{
			NS_ResetKeyInputStringColor2( i ) ;
		}
	}
#endif // DX_NON_KEYEX

	// �`��Ɏg�p����t�H���g�̓f�t�H���g�t�H���g
	CharBuf.UseFontHandle = -1 ;

	// ���̓A�N�e�B�u�n���h����������
	CharBuf.ActiveInputHandle = -1 ;
	
	// �J�[�\���_�ŏ����֌W�̏�����
	CharBuf.CBrinkFlag = FALSE ;
	CharBuf.CBrinkCount = 0 ;
	CharBuf.CBrinkWait = CURSORBRINKWAIT ;
	CharBuf.CBrinkDrawFlag = TRUE ;

	DXST_ERRORLOG_ADDUTF16LE( "\x8c\x5b\x86\x4e\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�������܂���\n" @*/ ) ;

	// �I��
	return 0 ;
}

// �o�b�t�@�ɃR�[�h���X�g�b�N����
extern int NS_StockInputChar( TCHAR CharCode )
{
#ifdef UNICODE
	return StockInputChar_WCHAR_T( CharCode ) ;
#else
	int CharBytes ;

	// �X�g�b�N�ɑ��
	CharBuf.TempStock[ CharBuf.TempStockNum     ] = CharCode ;
	CharBuf.TempStock[ CharBuf.TempStockNum + 1 ] = 0 ;
	CharBuf.TempStockNum ++ ;

	// �����̃o�C�g�����擾
	CharBytes = GetCharBytes( CharBuf.TempStock, _TCHARCODEFORMAT ) ;

	// �P�o�C�g�����̏ꍇ�͒����� wchar_t�Ŋ֐��ɓn��
	if( CharBytes == 1 )
	{
		StockInputChar_WCHAR_T( ( wchar_t )CharCode ) ;
		CharBuf.TempStockNum = 0 ;
	}
	else
	// �Q�o�C�g�ȏ�̕����̏ꍇ�͕����̃o�C�g���ɒB������ wchar_t ������ɕϊ����� wchar_t�Ŋ֐��ɓn��
	if( CharBuf.TempStockNum >= CharBytes )
	{
		wchar_t WCharString[ 16 ] ;
		int StrCharNum ;
		int i ;

		StrCharNum = ConvString( CharBuf.TempStock, _TCHARCODEFORMAT, ( char * )WCharString, sizeof( WCharString ), WCHAR_T_CHARCODEFORMAT ) / sizeof( wchar_t ) - 1 ;
		for( i = 0 ; i < StrCharNum ; i ++ )
		{
			StockInputChar_WCHAR_T( WCharString[ i ] ) ;
		}

		CharBuf.TempStockNum = 0 ;
	}

	return 0 ;
#endif
}


// �o�b�t�@�ɃR�[�h���X�g�b�N����
extern int StockInputChar_WCHAR_T( wchar_t CharCode )
{
	// �o�b�t�@����t�̏ꍇ�͂Ȃɂ����Ȃ�
	if( ( CharBuf.EdPoint + 1 == CharBuf.StPoint ) ||
		( CharBuf.StPoint == 0 && CharBuf.EdPoint == CHARBUFFER_SIZE ) )
	{
		return -1 ;
	}

	// �o�b�t�@�ɕ����R�[�h����
	CharBuf.CharBuffer[ CharBuf.EdPoint ] = CharCode ;
	CharBuf.EdPoint ++ ;
	if( CharBuf.EdPoint == CHARBUFFER_SIZE + 1 ) CharBuf.EdPoint = 0 ;

	// �I��
	return 0 ;
}

// �����R�[�h�o�b�t�@���N���A����
extern int NS_ClearInputCharBuf( void )
{
	while( NS_ProcessMessage() == 0 )
	{
		if( NS_GetInputChar( TRUE ) == 0 ) break ;
	} 

#ifndef UNICODE
	// �}���`�o�C�g�����Z�b�g�ł̏ꍇ�̓X�g�b�N������������
	CharBuf.TempStockNum    = 0 ;
	CharBuf.TempGetStockNum = 0 ;
#endif // UNICODE

	// �X�^�[�g�|�C���^�ƃG���h�|�C���^��������
	CharBuf.EdPoint = CharBuf.StPoint = 0 ;

#ifndef DX_NON_KEYEX
	// IME���͂̏�Ԃ����Z�b�g����
	if( WinData.MainWindow )
	{
		if( CharBuf.IMEUseFlag_OSSet == TRUE && CharBuf.IMESwitch == TRUE )
		{
			HIMC Imc;

			Imc = WinAPIData.ImmFunc.ImmGetContextFunc( WinData.MainWindow ) ;
			if( Imc )
			{
				_MEMSET( CharBuf.InputString, 0, CHARBUFFER_SIZE );
				WinAPIData.ImmFunc.ImmNotifyIMEFunc( Imc , NI_COMPOSITIONSTR ,  CPS_CANCEL , 0  );

				WinAPIData.ImmFunc.ImmReleaseContextFunc( WinData.MainWindow , Imc );
			}
		}
	}
#endif

	// �I��
	return 0 ;
}

// �����R�[�h�o�b�t�@�ɗ��܂����f�[�^����P�o�C�g���擾����
extern TCHAR NS_GetInputChar( int DeleteFlag )
{
#ifdef UNICODE
	return GetInputChar_WCHAR_T( DeleteFlag ) ;
#else
	char Result = 0 ;

	// �X�g�b�N�������ꍇ�� wchar_t �����񂩂當�����擾����
	if( CharBuf.TempGetStockNum == 0 )
	{
		wchar_t WCharString[ 16 ] ;

		do
		{
			// wchar_t �̃X�g�b�N����P�����擾
			WCharString[ 0 ] = GetInputChar_WCHAR_T( TRUE ) ;
			WCharString[ 1 ] = L'\0' ;

			// wchar_t �̃X�g�b�N������������ 0 ��Ԃ�
			if( WCharString[ 0 ] == 0 )
			{
				return 0 ;
			}

			// �X�g�b�N����������T���Q�[�g�y�A�`�F�b�N
			if( CHECK_WCHAR_T_DOUBLE( WCharString[ 0 ] ) )
			{
				// �T���Q�[�g�y�A��������X�ɂP�����擾
				WCharString[ 1 ] = GetInputChar_WCHAR_T( TRUE ) ;
				WCharString[ 2 ] = L'\0' ;
			}

			// �}���`�o�C�g������ɕϊ�
			CharBuf.TempGetStockNum = ConvString( ( const char * )WCharString, WCHAR_T_CHARCODEFORMAT, CharBuf.TempGetStock, sizeof( CharBuf.TempGetStock ), _TCHARCODEFORMAT ) ;

			// �}���`�o�C�g������ɕϊ��ł����烋�[�v�𔲂���
		}while( CharBuf.TempGetStockNum > 0 ) ;
	}

	// �X�g�b�N����P�����Ԃ�
	Result = CharBuf.TempGetStock[ 0 ] ;
	if( DeleteFlag )
	{
		CharBuf.TempGetStockNum -- ;
		if( CharBuf.TempGetStockNum > 0 )
		{
			_MEMMOVE( &CharBuf.TempGetStock[ 0 ], &CharBuf.TempGetStock[ 1 ], sizeof( char ) * CharBuf.TempGetStockNum ) ;
		}
	}

	return Result ;
#endif
}

// �����R�[�h�o�b�t�@�ɗ��܂����f�[�^����P�o�C�g���擾����
extern wchar_t GetInputChar_WCHAR_T( int DeleteFlag )
{
	wchar_t RetChar ;

	// �o�b�t�@�ɕ������Ȃ������ꍇ�͂O��Ԃ�
	if( CharBuf.EdPoint == CharBuf.StPoint )
	{
		return 0 ;
	}

	// �Ԃ������R�[�h���Z�b�g
	RetChar = CharBuf.CharBuffer[ CharBuf.StPoint ] ;

	// �L���f�[�^�X�^�[�g�|�C���^���C���N�������g
	if( DeleteFlag )
	{
		CharBuf.StPoint ++ ;
		if( CharBuf.StPoint == CHARBUFFER_SIZE + 1 ) CharBuf.StPoint = 0 ;
	}

	if( RetChar == -1 ) 
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x87\x65\x57\x5b\xd0\x30\xc3\x30\xd5\x30\xa1\x30\xa8\x30\xe9\x30\xfc\x30\x0a\x00\x00"/*@ L"�����o�b�t�@�G���[\n" @*/ ) ;
		return ( wchar_t )-1 ;
	}

	// �����R�[�h��Ԃ�
	return RetChar ;
}

// �����R�[�h�o�b�t�@�ɗ��܂����f�[�^����P�������擾����
extern int NS_GetOneChar( TCHAR *CharBuffer , int DeleteFlag )
{
#ifdef UNICODE
	return GetOneChar_WCHAR_T( CharBuffer, DeleteFlag ) ;
#else
	int CharBytes ;
	int Result = 0 ;

	// 1�o�C�g�ڂ��擾
	CharBuffer[ 0 ] = NS_GetInputChar( FALSE ) ;

	// �������������������� 0 ��Ԃ�
	if( CharBuffer[ 0 ] == 0 )
	{
		return 0 ;
	}

	// �����o�C�g�������ǂ����ŏ����𕪊�
	CharBytes = GetCharBytes( CharBuffer, _TCHARCODEFORMAT ) ;
	if( CharBytes == 1 )
	{
		// �o�b�t�@�폜�w�肪����ꍇ�͍폜
		if( DeleteFlag )
		{
			NS_GetInputChar( TRUE ) ;
		}

		// 1 ��Ԃ�
		return 1 ;
	}

	// �����o�C�g�̕������R�s�[
	_MEMCPY( CharBuffer, CharBuf.TempGetStock, CharBuf.TempGetStockNum ) ;
	Result = CharBuf.TempGetStockNum ;

	// �o�b�t�@�폜�w�肪����ꍇ�̓X�g�b�N�� 0 �ɂ���
	if( DeleteFlag )
	{
		CharBuf.TempGetStockNum = 0 ;
	}

	// �����̃o�C�g����Ԃ�
	return Result ;
#endif
}

// �����R�[�h�o�b�t�@�ɗ��܂����f�[�^����P�������擾����
extern int GetOneChar_WCHAR_T( wchar_t *CharBuffer , int DeleteFlag )
{
	int Ret ;

	CharBuffer[ 0 ] = 0 ;
	CharBuffer[ 1 ] = 0 ;

	Ret = 0 ;

	// �o�b�t�@�ɕ������Ȃ������ꍇ�͂O��Ԃ�
	if( CharBuf.EdPoint == CharBuf.StPoint )
	{
		return 0 ;
	}

	// �P�����ڂ��擾
	CharBuffer[ 0 ] = CharBuf.CharBuffer[ CharBuf.StPoint ] ;
	Ret ++ ;

	// �L���f�[�^�X�^�[�g�|�C���^���C���N�������g
	CharBuf.StPoint ++ ;
	if( CharBuf.StPoint == CHARBUFFER_SIZE + 1 )
	{
		CharBuf.StPoint = 0 ;
	}

	// �T���Q�[�g�y�A���ǂ������`�F�b�N
	if( GetCtrlCodeCmp_WCHAR_T( CharBuffer[ 0 ] ) == 0 && CHECK_WCHAR_T_DOUBLE( *CharBuffer ) )
	{
		// �o�b�t�@�ɕ������Ȃ������ꍇ�͂O��Ԃ�	
		if( CharBuf.EdPoint == CharBuf.StPoint )
		{
			// 1 wchar_t ���f�[�^�X�^�[�g�|�C���^��߂�
			if( CharBuf.StPoint == 0 )
			{
				CharBuf.StPoint = CHARBUFFER_SIZE ;
			}
			else
			{
				CharBuf.StPoint -- ;
			}
			
			return 0 ;
		}

		// 2 wchar_t �ڂ��擾
		CharBuffer[ 1 ] = CharBuf.CharBuffer[ CharBuf.StPoint ] ;
		Ret ++ ;

		// �L���f�[�^�X�^�[�g�|�C���^���C���N�������g
		if( DeleteFlag )
		{
			CharBuf.StPoint ++ ;
			if( CharBuf.StPoint == CHARBUFFER_SIZE + 1 )
			{
				CharBuf.StPoint = 0 ;
			}
		}
	}

	// �����o�b�t�@�폜�t���O���������Ă��Ȃ���΂P�o�C�g���߂�
	if( !DeleteFlag )
	{
		if( CharBuf.StPoint == 0 )
		{
			CharBuf.StPoint = CHARBUFFER_SIZE ;
		}
		else
		{
			CharBuf.StPoint -- ;
		}
	}

	// �擾�����o�C�g����Ԃ�
	return Ret ;
}

// �����R�[�h�o�b�t�@�ɗ��܂����f�[�^����P�o�C�g���擾����A�o�b�t�@�ɂȂɂ������R�[�h���Ȃ��ꍇ�̓L�[���������܂ő҂�
extern TCHAR NS_GetInputCharWait( int DeleteFlag )
{
#ifdef UNICODE
	return GetInputCharWait_WCHAR_T( DeleteFlag ) ;
#else
	TCHAR RetChar = 0 ;

	// �L�[�o�b�t�@�ɕ����R�[�h�����܂�܂ő҂�
	while( NS_ProcessMessage() == 0 )
	{
		RetChar = NS_GetInputChar( DeleteFlag ) ;
		if( RetChar != 0 )
		{
			break ;
		}
	}

	// �L�[�R�[�h��Ԃ�
	return RetChar ;
#endif
}

// �����R�[�h�o�b�t�@�ɗ��܂����f�[�^����P�o�C�g���擾����A�o�b�t�@�ɂȂɂ������R�[�h���Ȃ��ꍇ�̓L�[���������܂ő҂�
extern wchar_t GetInputCharWait_WCHAR_T( int DeleteFlag )
{
	wchar_t RetChar = 0 ;

	// �L�[�o�b�t�@�ɕ����R�[�h�����܂�܂ő҂�
	while( NS_ProcessMessage() == 0 )
	{
		RetChar = GetInputChar_WCHAR_T( DeleteFlag ) ;
		if( RetChar != 0 )
		{
			break ;
		}
	}

	// �L�[�R�[�h��Ԃ�
	return RetChar ;
}

// �����R�[�h�o�b�t�@�ɗ��܂����f�[�^����P�������擾����A�o�b�t�@�ɉ��������R�[�h���Ȃ��ꍇ�̓L�[���������܂ő҂�
extern int NS_GetOneCharWait( TCHAR *CharBuffer , int DeleteFlag ) 
{
#ifdef UNICODE
	return GetOneCharWait_WCHAR_T( CharBuffer, DeleteFlag ) ;
#else
	int Ret = 0 ;

	// �L�[�o�b�t�@�ɕ����R�[�h�����܂�܂ő҂�
	while( NS_ProcessMessage() == 0 )
	{
		Ret = NS_GetOneChar( CharBuffer , DeleteFlag ) ;
		if( Ret != 0 )
		{
			break ;
		}
	}

	// �擾�����o�C�g����Ԃ�
	return Ret ;
#endif
}

// �����R�[�h�o�b�t�@�ɗ��܂����f�[�^����P�������擾����A�o�b�t�@�ɉ��������R�[�h���Ȃ��ꍇ�̓L�[���������܂ő҂�
extern int GetOneCharWait_WCHAR_T( wchar_t *CharBuffer , int DeleteFlag ) 
{
	int Ret = 0 ;

	// �L�[�o�b�t�@�ɕ����R�[�h�����܂�܂ő҂�
	while( NS_ProcessMessage() == 0 )
	{
		Ret = GetOneChar_WCHAR_T( CharBuffer , DeleteFlag ) ;
		if( Ret != 0 )
		{
			break ;
		}
	}

	// �擾�����o�C�g����Ԃ�
	return Ret ;
}


#ifndef DX_NON_KEYEX

// �h�l�d���b�Z�[�W�̃R�[���o�b�N�֐�
extern LRESULT IMEProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	HIMC Imc ;
	static int Flag = FALSE ;
	INPUTDATA * Input ;

	// �h�l�d������Ԃ������ꍇ�͂Ȃɂ������I��
/*	if( CharBuf.IMEUseFlag == FALSE )
	{
//		DefWindowProcW( hWnd , message , wParam , lParam ) ;
		return 0;
	}
*/
	if( KEYHCHK( CharBuf.ActiveInputHandle, Input ) )
	{
		Input = NULL ;
	}

	// �ω����������t���O�Z�b�g
	CharBuf.ChangeFlag = TRUE ;

	// �J�[�\���_�ŏ����̃J�E���^�����Z�b�g
	ResetKeyInputCursorBrinkCount() ;

	// �X�C�b�`
	switch( message )
	{
	// �h�l�d���N�G�X�g
	case WM_IME_REQUEST :
		if( Input != NULL && wParam == IMR_RECONVERTSTRING )
		{
			int smin = 0 ;
			int smax ;
			wchar_t *TargetStr = NULL ;
			int TargetStrLength ;
			D_RECONVERTSTRING *ReConvString = ( D_RECONVERTSTRING * )lParam ;

			// �ĕϊ��Ώۂ̕��������擾
			if( Input->SelectStart != -1 )
			{
				if( Input->SelectStart > Input->SelectEnd )
				{
					smin = Input->SelectEnd ;
					smax = Input->SelectStart ;
				}
				else
				{
					smin = Input->SelectStart ;
					smax = Input->SelectEnd ;
				}

				TargetStrLength = smax - smin ;
			}
			else
			{
				TargetStrLength = ( int )_WCSLEN( Input->Buffer ) ;
			}

			// �ĕϊ����̐ݒ肪�v������Ă��Ȃ��ꍇ�͕K�v�f�[�^�T�C�Y��Ԃ�
			if( ReConvString == NULL )
			{
				return ( LRESULT )( sizeof( D_RECONVERTSTRING ) + ( TargetStrLength + 1 ) * sizeof( wchar_t ) ) ;
			}

			// �ĕϊ����̐ݒ���s��
			TargetStr = ( wchar_t * )( ( BYTE * )ReConvString + sizeof( D_RECONVERTSTRING ) ) ;
			if( Input != NULL && Input->SelectStart != -1 )
			{
				_MEMCPY( TargetStr, &Input->Buffer[ smin ], TargetStrLength * sizeof( wchar_t ) ) ;
				TargetStr[ TargetStrLength ] = _T( '\0' ) ;
			}
			else
			{
				_WCSCPY( TargetStr, Input->Buffer ) ;
			}

			ReConvString->dwStrLen          = ( DWORD )TargetStrLength ;
			ReConvString->dwStrOffset       = sizeof( D_RECONVERTSTRING ) ;
			ReConvString->dwTargetStrLen    = 0 ;
			ReConvString->dwTargetStrOffset = 0 ;

			Imc = WinAPIData.ImmFunc.ImmGetContextFunc( hWnd ) ;
			if( Imc != NULL )
			{
				// �ĕϊ��Ώە�����̈ʒu�ƕ��������Z�b�g
				if( Input->SelectStart != -1 )
				{
					// �ĕϊ��̑ΏۂƂȂ镔���́A�n����������̐擪���炷�ׂ�
					ReConvString->dwCompStrOffset = 0 ;
					ReConvString->dwCompStrLen    = ( DWORD )TargetStrLength ;
				}
				else
				{
					// �͈͑I���������Ƃ��͓��͕�����S�̂�n���A�J�[�\���ʒu���w�肷��
					ReConvString->dwCompStrOffset = Input->Point * sizeof( wchar_t ) ;
					ReConvString->dwCompStrLen    = ( DWORD )_WCSLEN( Input->Buffer ) ;
				}

				// �ĕϊ��̏���
				WinAPIData.ImmFunc.ImmSetCompositionStringFunc( Imc, SCS_QUERYRECONVERTSTRING, ReConvString, ReConvString->dwSize, NULL, 0 ) ;

				// �ĕϊ��̎��s
				if( WinAPIData.ImmFunc.ImmSetCompositionStringFunc( Imc, SCS_SETRECONVERTSTRING, ReConvString, ReConvString->dwSize, NULL, 0 ) != 0 )
				{
					// �ĕϊ��ΏۂƂȂ����ӏ����폜����
					if( Input->SelectStart == -1 )
					{
						Input->SelectStart = ( int )( ReConvString->dwCompStrOffset / sizeof( wchar_t ) ) ;
						Input->SelectEnd   = ( int )( Input->SelectStart + ReConvString->dwCompStrLen ) ;
						Input->Point       = Input->SelectStart ;
					}
					KeyInputSelectAreaDelete( Input ) ;
				}

				WinAPIData.ImmFunc.ImmReleaseContextFunc( hWnd, Imc );
			}
		}
		else
		{
			DefWindowProcW( hWnd , message , wParam , lParam ) ;
		}
		break ;

	// �h�l�d�E�C���h�E�A�N�e�B�u��ԕω�
	case WM_IME_SETCONTEXT :
//		lParam &= ~ISC_SHOWUICANDIDATEWINDOW ;
		lParam = 0;
		return DefWindowProcW( hWnd , message , wParam , lParam ) ;

	// �h�l�d���͊J�n���b�Z�[�W
	case WM_IME_STARTCOMPOSITION :

		CharBuf.InputPoint = 0 ;
		CharBuf.IMEInputFlag = TRUE ;

		// �h�l�d�̎g�p��Ԃ𓾂�
		if( CharBuf.IMESwitch == FALSE && WinData.WindowsVersion >= DX_WINDOWSVERSION_VISTA )
		{
			CharBuf.IMESwitch = TRUE ;
		}
		return 0;

	// �h�l�d���͏I�����b�Z�[�W
	case WM_IME_ENDCOMPOSITION :
		_MEMSET( CharBuf.InputString, 0, CHARBUFFER_SIZE );
		CharBuf.IMEInputFlag = FALSE ;

		// �h�l�d�̎g�p��Ԃ𓾂�
		if( CharBuf.IMESwitch == TRUE && WinData.WindowsVersion >= DX_WINDOWSVERSION_VISTA )
		{
			CharBuf.IMESwitch = FALSE ;
		}
		break ;


	// �h�l�d�����ҏW�ω����b�Z�[�W
	case WM_IME_COMPOSITION :
		{
//			HWND DefHwnd = ImmGetDefaultIMEWnd( WinData.MainWindow ) ;
			HWND DefHwnd = hWnd;

			// ���̓R���e�L�X�g���擾
//			SendMessage( DefHwnd , WM_CLOSE , 0 , 0 ) ;
			Imc = WinAPIData.ImmFunc.ImmGetContextFunc( DefHwnd ) ;
//			Imc = WinAPIData.ImmFunc.ImmGetContextFunc( hWnd ) ;

			// �ҏW�����ω����̏���
			if( lParam & GCS_COMPSTR )
			{
				// �ҏW��������̎擾
				_MEMSET( CharBuf.InputString, 0, CHARBUFFER_SIZE );
				WinAPIData.ImmFunc.ImmGetCompositionStringFunc( Imc , GCS_COMPSTR , CharBuf.InputString , CHARBUFFER_SIZE );
			}

			// �ҏW���������莞�̏���
			if( lParam & GCS_RESULTSTR )
			{
				_MEMSET( CharBuf.InputString, 0, CHARBUFFER_SIZE );
				WinAPIData.ImmFunc.ImmGetCompositionStringFunc( Imc , GCS_RESULTSTR , CharBuf.InputString , CHARBUFFER_SIZE );
				// �o�b�t�@�ɃX�g�b�N
				{
					int StrLen , i ;

					StrLen = ( int )_WCSLEN( CharBuf.InputString ) ;
					for( i = 0 ; i < StrLen ; i ++ )
					{
						StockInputChar_WCHAR_T( CharBuf.InputString[ i ] ) ;
					}
				}

				// �ҏW�������񏉊���
				_MEMSET( CharBuf.InputString, 0, sizeof( CharBuf.InputString ) );
				WinAPIData.ImmFunc.ImmGetCompositionStringFunc( Imc , GCS_COMPSTR , CharBuf.InputString , CHARBUFFER_SIZE );

				// �ϊ����̏������Z�b�g
				if( CharBuf.CandidateList )
				{
					DXFREE( CharBuf.CandidateList ) ;
					CharBuf.CandidateList = NULL ;
					CharBuf.CandidateListSize = 0 ;
				}
			}

			// �J�[�\���ʒu�ω����̏���
			if( lParam & GCS_CURSORPOS )
			{
				int OlgPoint = CharBuf.InputPoint ;
				CharBuf.InputPoint = GetStringPoint2_WCHAR_T( CharBuf.InputString , WinAPIData.ImmFunc.ImmGetCompositionStringFunc( Imc , GCS_CURSORPOS , NULL , 0 ) ) ;

				// �ϊ����̏������Z�b�g
				if( CharBuf.CandidateList && OlgPoint != CharBuf.InputPoint )
				{
					DXFREE( CharBuf.CandidateList ) ;
					CharBuf.CandidateList = NULL ;
					CharBuf.CandidateListSize = 0 ;
				}
			}

			// ���ߏ��ω���
			if( lParam & GCS_COMPCLAUSE )
			{
				// ���ߏ��̎擾
				_MEMSET( CharBuf.ClauseData, 0, sizeof( CharBuf.ClauseData ) ) ;
				CharBuf.ClauseNum = WinAPIData.ImmFunc.ImmGetCompositionStringFunc( Imc , GCS_COMPCLAUSE , ( void * )CharBuf.ClauseData , 1024 * sizeof( int ) ) / 4 ;
			}

			// ���͕��������ω���
			if( lParam & GCS_COMPATTR )
			{
				_MEMSET( CharBuf.CharAttr, 0, sizeof( CharBuf.CharAttr ) ) ;
				CharBuf.CharAttrNum = WinAPIData.ImmFunc.ImmGetCompositionStringFunc( Imc , GCS_COMPATTR , ( void * )CharBuf.CharAttr , 1024 * sizeof( BYTE ) ) ;
			}

			// ���̓R���e�L�X�g�̍폜
			WinAPIData.ImmFunc.ImmReleaseContextFunc( DefHwnd , Imc );
//			WinAPIData.ImmFunc.ImmReleaseContextFunc( hWnd , Imc ) ;
		}
		break;

	// �h�l�d��ԕω����b�Z�[�W
	case WM_IME_NOTIFY :

		switch( wParam )
		{
		// �h�l�d�̂n�m�A�n�e�e�ω�
		case IMN_SETOPENSTATUS :

			if( Flag ) break ;

			Flag = TRUE ;

			DefWindowProcW( hWnd , message , wParam , lParam ) ;

			// �L�[��������Ă���Ԃ����Ŏ~�܂�
			{
				__int64 Time = NS_GetNowHiPerformanceCount() ;

				while( NS_ProcessMessage() == 0 && NS_GetNowHiPerformanceCount() - Time < 2000 ){}
			}

			Flag = FALSE ;

			// �h�l�d�̎g�p��Ԃ𓾂�
			{
				HWND DefHwnd = hWnd;
//				HWND DefHwnd = ImmGetDefaultIMEWnd( hWnd ) ;
				Imc = WinAPIData.ImmFunc.ImmGetContextFunc( DefHwnd ) ;

//				SendMessage( DefHwnd , WM_CLOSE , 0 , 0 ) ;
//				Imc = WinAPIData.ImmFunc.ImmGetContextFunc( hWnd ) ;

				CharBuf.IMESwitch = WinAPIData.ImmFunc.ImmGetOpenStatusFunc( Imc ) ;
				WinAPIData.ImmFunc.ImmReleaseContextFunc( DefHwnd , Imc );
//				WinAPIData.ImmFunc.ImmReleaseContextFunc( hWnd , Imc );
			}

			break ;


		// ���ω����̏���
		case IMN_CHANGECANDIDATE:
		case IMN_OPENCANDIDATE :
		case IMN_SETCANDIDATEPOS :
			{
				DWORD BufSize ;
//				HWND DefHwnd = ImmGetDefaultIMEWnd( hWnd ) ;
				HWND DefHwnd = hWnd;
//				int Result ;

				// ���̓R���e�L�X�g���擾
				Imc = WinAPIData.ImmFunc.ImmGetContextFunc( DefHwnd ) ;
//				Imc = WinAPIData.ImmFunc.ImmGetContextFunc( hWnd ) ;

				// �o�b�t�@�T�C�Y�擾
//				BufSize = ImmGetCandidateListCount( Imc , &ListSize ) ;
				BufSize = WinAPIData.ImmFunc.ImmGetCandidateListFunc( Imc , 0 , NULL , 0 ) ;
				if( BufSize != 0 )
				{
					CharBuf.CandidateListSize = ( int )BufSize ;
					if( BufSize == 0 )
					{
						WinAPIData.ImmFunc.ImmReleaseContextFunc( DefHwnd , Imc );
						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x22\x6f\x57\x5b\x09\x59\xdb\x63\x19\x50\xdc\x88\x6e\x30\xc7\x30\xfc\x30\xbf\x30\xb5\x30\xa4\x30\xba\x30\x4c\x30\x10\xff\x67\x30\x57\x30\x5f\x30\x20\x00\x20\x00\x44\x00\x65\x00\x66\x00\x48\x00\x57\x00\x4e\x00\x44\x00\x3a\x00\x25\x00\x78\x00\x20\x00\x49\x00\x4d\x00\x43\x00\x3a\x00\x25\x00\x78\x00\x0a\x00\x00"/*@ L"�����ϊ����̃f�[�^�T�C�Y���O�ł���  DefHWND:%x IMC:%x\n" @*/, DefHwnd, Imc )) ;
						return 0 ;
					}

					// �o�b�t�@�p�������̊m��
					if( CharBuf.CandidateList != NULL )
							DXFREE( CharBuf.CandidateList ) ;
					if( ( CharBuf.CandidateList = ( CANDIDATELIST * )DXALLOC( BufSize ) ) == NULL )
					{
						WinAPIData.ImmFunc.ImmReleaseContextFunc( DefHwnd , Imc );
#ifndef DX_NON_LITERAL_STRING
						return DxLib_FmtErrorUTF16LE( "\x22\x6f\x57\x5b\x09\x59\xdb\x63\x19\x50\xdc\x88\x92\x30\xdd\x4f\x58\x5b\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x28\x00\x20\x00\xba\x78\xdd\x4f\x57\x30\x88\x30\x46\x30\x68\x30\x57\x30\x5f\x30\xb5\x30\xa4\x30\xba\x30\x20\x00\x25\x00\x64\x00\x20\x00\xd0\x30\xa4\x30\xc8\x30\x20\x00\x29\x00\x00"/*@ L"�����ϊ�����ۑ����郁�����̊m�ۂɎ��s���܂���( �m�ۂ��悤�Ƃ����T�C�Y %d �o�C�g )" @*/, BufSize ) ;
#else
						return DxLib_FmtError( "" ) ;
#endif
					}

					// �f�[�^�̎擾
					if( WinAPIData.ImmFunc.ImmGetCandidateListFunc( Imc , 0 , CharBuf.CandidateList , BufSize ) == 0 )
					{
						DXFREE( CharBuf.CandidateList ) ;
						CharBuf.CandidateList = NULL ;
						WinAPIData.ImmFunc.ImmReleaseContextFunc( DefHwnd , Imc );

						DXST_ERRORLOGFMT_ADDUTF16LE(( "\x22\x6f\x57\x5b\x09\x59\xdb\x63\x19\x50\xdc\x88\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x20\x00\x20\x00\x42\x00\x75\x00\x66\x00\x53\x00\x69\x00\x7a\x00\x65\x00\x3a\x00\x25\x00\x64\x00\x0a\x00\x00"/*@ L"�����ϊ����̎擾�Ɏ��s���܂���  BufSize:%d\n" @*/, BufSize )) ;
						return 0 ;
					}
				}

				// ���̓R���e�L�X�g�̍폜
				WinAPIData.ImmFunc.ImmReleaseContextFunc( DefHwnd , Imc );
//				WinAPIData.ImmFunc.ImmReleaseContextFunc( hWnd , Imc );
			}
			break ;

		// ���͌��E�C���h�E����悤�Ƃ��Ă��鎞�̏���
		case IMN_CLOSECANDIDATE :

			// �������̉��
			if( CharBuf.CandidateList )
				DXFREE( CharBuf.CandidateList ) ;
			CharBuf.CandidateList = NULL ;
			CharBuf.CandidateListSize = 0 ;
			break ;

		default :
//			return 0;
			return DefWindowProcW( hWnd , message , wParam , lParam ) ;
		}
		break;
	}

	return 0 ;
}



// ��ʏ�ɓ��͒��̕������`�悷��
extern int NS_DrawIMEInputString( int x , int y , int SelectStringNum )
{
	int StrLen , FontSize , FontHandle ;
	int PointX , PointY ;
	int CPointX , CPointY ;
	int StrWidth , Width ;
	RECT DrawRect ;
	int Use3DFlag ;
	int i ;
	int IsSelect ;
	int Mult ;
	int Point ;
	static wchar_t StringBuf[2048] ;

	if( CharBuf.IMEUseFlag_OSSet == FALSE || CharBuf.IMESwitch == FALSE ) return -1 ;

	// �g�p����t�H���g�̃n���h�����Z�b�g����
	FontHandle = CharBuf.UseFontHandle == -1 ? NS_GetDefaultFontHandle() : CharBuf.UseFontHandle ;

	// �t�H���g�T�C�Y�𓾂�
	FontSize = NS_GetFontSizeToHandle( FontHandle ) ;

	// �R�c�L���t���O�𓾂�
	Use3DFlag = NS_GetUse3DFlag() ;
	NS_SetUse3DFlag( FALSE ) ;

	// �`��\�̈�𓾂�
	NS_GetDrawArea( &DrawRect ) ;

	// ���͕����񂪂Ȃ��ꍇ�͂����ŏI��
	if( CharBuf.InputString[ 0 ] == 0 )
	{
		// �R�c�L���t���O�����ɖ߂�
		NS_SetUse3DFlag( Use3DFlag ) ;
		return 0 ;
	}

	// �`�敶����̒����𓾂�
	StrLen   = ( int )_WCSLEN( CharBuf.InputString ) ;
	StrWidth = GetDrawStringWidthToHandle_WCHAR_T( CharBuf.InputString , StrLen , FontHandle ) ;

	// �`��ʒu��␳
	if( x < DrawRect.left ) x = DrawRect.left ;
	if( y < DrawRect.top  ) y = DrawRect.top ;

	// ���͕������`��
	{
		// ���͒�������𕢂���`��`��
		for( i = 0 ; i < StrLen ; i ++ )
		{
			Point = GetDrawStringWidthToHandle_WCHAR_T( CharBuf.InputString     , i , FontHandle ) ;
			Width = GetDrawStringWidthToHandle_WCHAR_T( CharBuf.InputString + i , 1 , FontHandle ) ;

			if( i < CharBuf.CharAttrNum && CharBuf.CharAttr[ i ] == ATTR_TARGET_CONVERTED )
			{
				IsSelect = TRUE ;
			}
			else
			{
				IsSelect = FALSE ;
			}

			NS_DrawObtainsBox(
				x + Point ,
				y ,
				x + Point + Width ,
				y + FontSize ,
				FontSize + FontSize / 10 * 3 ,
				IsSelect ? CharBuf.IMESelectStrBackColor : CharBuf.IMEStrBackColor ,
				TRUE
			) ;

			if( CHECK_WCHAR_T_DOUBLE( CharBuf.InputString[ i ] ) )
			{
				i ++ ;
			}
		}

		// ���͒��������`��
		for( i = 0 ; i < StrLen ; i ++ )
		{
			Point = GetDrawStringWidthToHandle_WCHAR_T( CharBuf.InputString     , i , FontHandle ) ;
			Width = GetDrawStringWidthToHandle_WCHAR_T( CharBuf.InputString + i , 1 , FontHandle ) ;

			if( i < CharBuf.CharAttrNum && CharBuf.CharAttr[ i ] == ATTR_TARGET_CONVERTED )
			{
				IsSelect = TRUE ;
			}
			else
			{
				IsSelect = FALSE ;
			}

			Mult = CHECK_WCHAR_T_DOUBLE( CharBuf.InputString[ i ] ) ;

			StringBuf[ 0 ] = CharBuf.InputString[ i ] ;
			if( Mult )
			{
				StringBuf[ 1 ] = CharBuf.InputString[ i + 1 ] ;
				StringBuf[ 2 ] = L'\0' ;
			}
			else
			{
				StringBuf[ 1 ] = L'\0' ;
			}

			DrawObtainsString_WCHAR_T(
				x + Point ,
				y ,
				FontSize + FontSize / 10 * 3 ,
				StringBuf ,
				-1,
				IsSelect ? CharBuf.IMESelectStrColor : CharBuf.IMEStrColor ,
				IsSelect ? ( CharBuf.IMESelectStrEdgeColorEnable ? CharBuf.IMESelectStrEdgeColor : CharBuf.IMESelectStrBackColor ) :
				           ( CharBuf.IMEStrEdgeColorEnable       ? CharBuf.IMEStrEdgeColor       : CharBuf.IMEStrBackColor       ),
				FontHandle
			) ;

			if( Mult )
			{
				i ++ ;
			}
		}
	}

	// �J�[�\���̂���ʒu���Z�o
	{
		StrLen = GetStringPoint_WCHAR_T( CharBuf.InputString , CharBuf.InputPoint ) ;

		// �擪�����ʒu�܂ł̕����Z�o
		if( StrLen == 0 )
		{
			CPointX = 0 ;
		}
		else
		{
			// �������𓾂�
			CPointX = GetDrawStringWidthToHandle_WCHAR_T( CharBuf.InputString , StrLen , FontHandle ) ;
		}
		CPointX += x ;
		CPointY = y ;
	}

	// �J�[�\����`��
	{
		// �I�𕶎��񂪑��݂���ꍇ�̂ݕ`��
		if( !CharBuf.CandidateList && CharBuf.CBrinkDrawFlag == TRUE )
		{
			// �J�[�\���̕`��
			NS_DrawObtainsBox( CPointX , CPointY , CPointX + 2 , CPointY + FontSize , FontSize + FontSize / 10 * 3  , CharBuf.IMECursorColor , TRUE ) ;
		}
	}

	// ������`��
	{
		int i , StrNum , StrWidth , PointX , PointY , LinePX, LinePY ;

		StrNum = 0 ;
		PointX = x ;
		PointY = y ;
		LinePX = FontSize / 20 ;
		if( LinePX < 1 )
		{
			LinePX = 1 ;
		}
		LinePY = FontSize / 10 ;
		for( i = 0 ; i < CharBuf.ClauseNum ; i ++ )
		{
			StrWidth = GetDrawStringWidthToHandle_WCHAR_T( &CharBuf.InputString[ StrNum ] , CharBuf.ClauseData[ i ] - StrNum , FontHandle ) ;
			if( StrWidth > LinePX * 4 )
			{
				NS_DrawObtainsBox(
					PointX + LinePX * 2				, PointY + FontSize + LinePY , 
					PointX + StrWidth - LinePX * 2	, PointY + FontSize + LinePY * 2 ,
					FontSize + FontSize / 10 * 3 ,
					CharBuf.IMELineColor , TRUE
				) ;
			}
			PointX += StrWidth ;
			StrNum = CharBuf.ClauseData[ i ] ;
		}
	}

	// ��⃊�X�g���o�Ă���ꍇ�͂��̕`��
	if( CharBuf.CandidateList )
	{
		DWORD i ;
		RECT SelectRect ;
		int j ,h ;
		int MaxWidth ;
		int SelectionNumWidth ;

		// �I�����̍ő吔��\������ƕ`��͈͂𒴂��Ă��܂��ꍇ�͕`�悷����̐��𐧌�����
		h = FontSize / 3 ;
		if( FontSize * ( SelectStringNum + 1 ) + h * 2 > DrawRect.bottom - DrawRect.top )
		{
			SelectStringNum = ( DrawRect.bottom - DrawRect.top - ( h * 2 ) ) / FontSize - 1 ;
			if( SelectStringNum <= 0 )
			{
				SelectStringNum = 1 ;
			}
		}

		// �Œ��I�����𒲂ׂ�
		MaxWidth = 0 ;
		j = 0 ;
		for( i = ( CharBuf.CandidateList->dwSelection / SelectStringNum ) * SelectStringNum ; j < SelectStringNum ; i ++ , j ++ )
		{
			if( i == CharBuf.CandidateList->dwCount ) break ;

			_SWNPRINTF( StringBuf, sizeof( StringBuf ) / 2, L"%d:%s", j + 1, ( wchar_t * )( ( BYTE * )CharBuf.CandidateList + CharBuf.CandidateList->dwOffset[i] ) ) ;
			Width = GetDrawStringWidthToHandle_WCHAR_T( StringBuf , ( int )_WCSLEN( StringBuf ) , FontHandle ) ;
			if( Width > MaxWidth )
			{
				MaxWidth = Width ;
			}
		}
		_SWNPRINTF( StringBuf, sizeof( StringBuf ) / 2, L"%d/%d", CharBuf.CandidateList->dwSelection + 1, CharBuf.CandidateList->dwCount ) ;
		SelectionNumWidth = GetDrawStringWidthToHandle_WCHAR_T( StringBuf , ( int )_WCSLEN( StringBuf ) , FontHandle ) ;
		if( SelectionNumWidth > MaxWidth )
		{
			MaxWidth = SelectionNumWidth ;
		}
		j ++ ;

		// �`��͈͂��Z�b�g
		{
			SETRECT(
				SelectRect,
				CPointX,					CPointY + FontSize + FontSize / 2 ,
				CPointX + MaxWidth + h * 2,	CPointY + FontSize + FontSize / 2 + FontSize * j + h * 2
			) ;

			// �C���v�b�g�����`��ʒu��艺�̈ʒu�ɔz�u�ł��邩����
			if( SelectRect.bottom > DrawRect.bottom )
			{
				// �o���Ȃ��ꍇ�̓C���v�b�g�����`��ʒu����̈ʒu�ɔz�u�����݂�
				if( y - FontSize * j - FontSize / 2 - h * 2 < DrawRect.top )
				{	
					// ����ł��ʖڂȏꍇ�͖�����艺�̈ʒu�ɕ`�悷��
					j = SelectRect.bottom - DrawRect.bottom ;
				}
				else
				{
					// ��̈ʒu���N�_�ɔz�u
					j = SelectRect.top - ( y - FontSize * j - FontSize / 2 - h * 2 ) ;
				}

				// ������␳
				SelectRect.bottom -= j ;
				SelectRect.top -= j ;
			}

			// �C���v�b�g�����`�敝�̂����ŕ`��\�̈�E�[�ɓ��B���Ă����ꍇ�͕␳
			if( SelectRect.right > DrawRect.right )
			{
				j = SelectRect.right - DrawRect.right ;	

				SelectRect.left -= j ;
				SelectRect.right -= j ;
			}
		}

		// �`��͈͂����œh��Ԃ�
		NS_DrawBox( SelectRect.left , SelectRect.top , SelectRect.right , SelectRect.bottom , CharBuf.IMEConvWinBackColor , TRUE ) ;
		NS_DrawBox( SelectRect.left , SelectRect.top , SelectRect.right , SelectRect.bottom , CharBuf.IMEConvWinEdgeColor , FALSE ) ;

		// ����`��
		PointX = SelectRect.left + h ;
		PointY = SelectRect.top + h ;
		j = 0 ;
		for( i = ( CharBuf.CandidateList->dwSelection / SelectStringNum ) * SelectStringNum ; j < SelectStringNum ; i ++ , j ++ )
		{
			if( i == CharBuf.CandidateList->dwCount ) break ;

			IsSelect = ( i == CharBuf.CandidateList->dwSelection ) ? TRUE : FALSE ;
			if( IsSelect && CharBuf.IMEConvWinSelectStrBackColorEnable )
			{
				NS_DrawBox( SelectRect.left + 1, PointY + j * FontSize , SelectRect.right - 1, PointY + ( j + 1 ) * FontSize, CharBuf.IMEConvWinSelectStrBackColor, TRUE ) ;
			}

			_SWNPRINTF( StringBuf, sizeof( StringBuf ) / 2, L"%d:%s", j + 1, ( wchar_t * )( ( BYTE * )CharBuf.CandidateList + CharBuf.CandidateList->dwOffset[i] ) ) ;
			DrawStringToHandle_WCHAR_T( PointX , PointY + j * FontSize , StringBuf,
				IsSelect ? CharBuf.IMEConvWinSelectStrColor     : CharBuf.IMEConvWinStrColor , FontHandle ,
				IsSelect ? CharBuf.IMEConvWinSelectStrEdgeColor : CharBuf.IMEConvWinStrEdgeColor ) ;
		}
 
		// ��⑍���̓����Ԗڂ��A�̏���`�悷��
		_SWNPRINTF( StringBuf, sizeof( StringBuf ) / 2, L"%d/%d", CharBuf.CandidateList->dwSelection + 1, CharBuf.CandidateList->dwCount ) ;
		DrawStringToHandle_WCHAR_T( PointX + MaxWidth - SelectionNumWidth, PointY + j * FontSize , StringBuf, CharBuf.IMEConvWinStrColor , FontHandle , CharBuf.IMEConvWinStrEdgeColor ) ;

		// ��⃊�X�g�̊J�n�C���f�b�N�X�ƌ�⃊�X�g�̃y�[�W�T�C�Y��ύX
		{
			HWND DefHwnd = WinData.MainWindow;
			HIMC Imc ;

			// ���̓R���e�L�X�g���擾
			Imc = WinAPIData.ImmFunc.ImmGetContextFunc( DefHwnd ) ;

			// ��⃊�X�g�̃y�[�W�T�C�Y��ύX
			WinAPIData.ImmFunc.ImmNotifyIMEFunc( Imc, NI_SETCANDIDATE_PAGESIZE, 0, ( DWORD )SelectStringNum ) ;

			// ��⃊�X�g�̊J�n�C���f�b�N�X�̕ύX���b�Z�[�W�𑗂�
			WinAPIData.ImmFunc.ImmNotifyIMEFunc( Imc, NI_SETCANDIDATE_PAGESTART, 0, ( CharBuf.CandidateList->dwSelection / SelectStringNum ) * SelectStringNum ) ;

			// ���̓R���e�L�X�g�̍폜
			WinAPIData.ImmFunc.ImmReleaseContextFunc( DefHwnd , Imc );
		}
	}

	// �R�c�L���t���O�����ɖ߂�
	NS_SetUse3DFlag( Use3DFlag ) ;

	// �I��
	return 0 ;
}

// �h�l�d���g�p���邩�ǂ����̏�Ԃ��X�V����
extern void RefreshIMEFlag( int Always )
{
	int NewUseFlag ;

	NewUseFlag = CharBuf.IMEUseFlag || CharBuf.IMEUseFlag_System ;

	// �t���O���ȑO�Ɠ����ꍇ�͉��������I��
	if( NewUseFlag == CharBuf.IMEUseFlag_OSSet && Always == FALSE ) return ;

	// �L���t���O���Z�b�g
	if( WinAPIData.Win32Func.WINNLSEnableIME_Func )
		WinAPIData.Win32Func.WINNLSEnableIME_Func( WinData.MainWindow , NewUseFlag ) ;

	// �t���O�ۑ�
	CharBuf.IMEUseFlag_OSSet = NewUseFlag ;

	// ����FALSE�������ꍇ�͊e�탁�������������
	if( NewUseFlag == FALSE ) 
	{
		DXFREE( CharBuf.CandidateList ) ;
		CharBuf.CandidateList = NULL ;
		CharBuf.CandidateListSize = 0 ;

		_MEMSET( CharBuf.InputString, 0, CHARBUFFER_SIZE ) ;

		// ���͒����A�t���O���|��
		CharBuf.IMEInputFlag = FALSE ;
	}

	// �I��
	return ;
}

// �h�l�d���g�p���邩�ǂ������Z�b�g����
extern int NS_SetUseIMEFlag( int UseFlag )
{
	// �t���O�ۑ�
	CharBuf.IMEUseFlag = UseFlag ;

	// ��Ԃ��X�V
	RefreshIMEFlag() ;

	// �I��
	return 0 ;
}


// �h�l�d�œ��͂ł���ő啶������ MakeKeyInput �̐ݒ�ɍ��킹�邩�ǂ������Z�b�g����( TRUE:���킹��  FALSE:���킹�Ȃ�(�f�t�H���g) )
extern int NS_SetInputStringMaxLengthIMESync( int Flag )
{
	CharBuf.IMEInputStringMaxLengthIMESync = Flag ;

	// �I��
	return 0 ;
}

// �h�l�d�ň�x�ɓ��͂ł���ő啶������ݒ肷��( 0:�����Ȃ�  1�ȏ�:�w��̕������Ő��� )
extern int NS_SetIMEInputStringMaxLength( int Length )
{
	CharBuf.IMEInputMaxLength = Length ;

	// �I��
	return 0 ;
}

// �h�l�d���g�p��Ԃ�ύX����
static int SetIMEOpenState( int OpenFlag ) 
{
	HIMC Imc ;
	HWND DefHwnd = WinData.MainWindow;

	// ���̓R���e�L�X�g���擾
	Imc = WinAPIData.ImmFunc.ImmGetContextFunc( DefHwnd ) ;

	// �g�p��Ԃ�ύX����
	WinAPIData.ImmFunc.ImmSetOpenStatusFunc( Imc, OpenFlag == TRUE ? true : false ) ;

	// ���̓R���e�L�X�g�̍폜
	WinAPIData.ImmFunc.ImmReleaseContextFunc( DefHwnd , Imc );

	// �I��
	return 0 ;
}

// �L�[���͂̑I��͈͂��폜����
static int KeyInputSelectAreaDelete( INPUTDATA * Input )
{
	int smin, smax, DelNum ;

	if( Input->SelectStart == -1 ) return 0 ;

	if( Input->SelectStart > Input->SelectEnd )
	{
		smin = Input->SelectEnd ;
		smax = Input->SelectStart ;
	}
	else
	{
		smin = Input->SelectStart ;
		smax = Input->SelectEnd ;
	}
	DelNum = smax - smin ;
	Input->Point = smin ;

	if( smax != Input->StrLength )
	{
		_MEMMOVE( &Input->Buffer[ smin ], &Input->Buffer[ smax ], ( Input->StrLength - smax ) * sizeof( wchar_t ) ) ;
	}
	Input->StrLength -= DelNum ;
	Input->Buffer[ Input->StrLength ] = _T( '\0' ) ;
	Input->SelectStart = -1 ;
	Input->SelectEnd = -1 ;
	if( Input->StrLength < Input->DrawStartPos ) Input->DrawStartPos = Input->StrLength ;

	// �I��
	return 0 ;
}


#endif

// �S�p�����A���p�������藐��钆����w��̑S�����ݕ������ł̔��p�������𓾂�
extern int NS_GetStringPoint( const TCHAR *String , int Point )
{
#ifdef UNICODE
	return GetStringPoint_WCHAR_T( String, Point ) ;
#else
	int i , p ;
	DWORD CharCode ;
	int CharBytes ;

	p = 0 ;
	for( i = 0 ; i < Point ; i ++ )
	{
		CharCode = GetCharCode( String + p, _TCHARCODEFORMAT, &CharBytes ) ;
		if( CharCode == 0 )
		{
			break ;
		}

		p += CharBytes / sizeof( char ) ;
	}

	return p ;
#endif
}

// �S�p�����A���p�������藐��钆����w��̑S�����ݕ������ł̔��p�������𓾂�
extern int GetStringPoint_WCHAR_T( const wchar_t *String , int Point )
{
	int i , p ;

	p = 0 ;
	for( i = 0 ; i < Point ; i ++ )
	{
		if( *( String + p ) == L'\0' )
		{
			break ; 
		}

		if( CHECK_WCHAR_T_DOUBLE( String[ p ] ) == FALSE )
		{
			p ++ ;
		}
		else
		{
			p += 2 ;
		}
	}

	return p ;
}

// �S�p�����A���p�������藐��钆����w��̔��p�������ł̕������𓾂�
extern int NS_GetStringPoint2( const TCHAR *String , int Point )
{
#ifdef UNICODE
	return GetStringPoint2_WCHAR_T( String, Point ) ;
#else
	int i , p ;
	DWORD CharCode ;
	int CharBytes ;

	p = 0 ;
	i = 0 ;
	while( i < Point )
	{
		CharCode = GetCharCode( String + i, _TCHARCODEFORMAT, &CharBytes ) ;
		if( CharCode == 0 )
		{
			break ;
		}

		i += CharBytes / sizeof( char ) ;
		p ++ ;
	}

	return p ;
#endif
}

// �S�p�����A���p�������藐��钆����w��̔��p�������ł̑S�p�������𓾂�
extern int GetStringPoint2_WCHAR_T( const wchar_t *String , int Point )
{
	int i , p ;

	p = 0 ;
	i = 0 ;
	while( i < Point )
	{
		if( *( String + i ) == L'\0' )
		{
			break ; 
		}

		if( CHECK_WCHAR_T_DOUBLE( String[ i ] ) == FALSE )
		{
			i ++ ;
		}
		else
		{
			i += 2 ;
		}

		p ++ ;
	}

	return p ;
}

// �S�p�����A���p�������藐��钆���當�������擾����
extern int NS_GetStringLength( const TCHAR *String )
{
#ifdef UNICODE
	return GetStringLength_WCHAR_T( String ) ;
#else
	int i , p ;
	DWORD CharCode ;
	int CharBytes ;

	p = 0 ;
	i = 0 ;
	for(;;)
	{
		CharCode = GetCharCode( String + i, _TCHARCODEFORMAT, &CharBytes ) ;
		if( CharCode == 0 )
		{
			break ;
		}

		i += CharBytes / sizeof( char ) ;
		p ++ ;
	}

	return p ;
#endif
}

// �S�p�����A���p�������藐��钆���當�������擾����
extern int GetStringLength_WCHAR_T( const wchar_t *String )
{
	int i , p ;

	p = 0 ;
	i = 0 ;
	while( *( String + i ) != L'\0' )
	{
		if( CHECK_WCHAR_T_DOUBLE( String[ i ] ) == FALSE )
		{
			i ++ ;
		}
		else
		{
			i += 2 ;
		}

		p ++ ;
	}

	return p ;
}

#ifndef DX_NON_FONT

// �K��̈�Ɏ��߂��������ŕ������`��
extern int NS_DrawObtainsString(
	int x , int y ,
	int AddY ,
	const TCHAR *String ,
	unsigned int StrColor ,
	unsigned int StrEdgeColor ,
	int FontHandle,
	unsigned int SelectBackColor ,
	unsigned int SelectStrColor ,
	unsigned int SelectStrEdgeColor ,
	int SelectStart ,
	int SelectEnd )
{
#ifdef UNICODE
	return DrawObtainsString_WCHAR_T(
		x , y ,
		AddY ,
		String ,
		-1,
		StrColor ,
		StrEdgeColor ,
		FontHandle,
		SelectBackColor ,
		SelectStrColor ,
		SelectStrEdgeColor ,
		SelectStart ,
		SelectEnd,
		TRUE,
		NULL,
		NULL
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( String, return -1 )

	Result = DrawObtainsString_WCHAR_T(
		x , y ,
		AddY ,
		UseStringBuffer ,
		-1,
		StrColor ,
		StrEdgeColor ,
		FontHandle,
		SelectBackColor ,
		SelectStrColor ,
		SelectStrEdgeColor ,
		SelectStart ,
		SelectEnd,
		TRUE,
		NULL,
		NULL
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( String )

	return Result ;
#endif
}

// �K��̈�Ɏ��߂��������ŕ������`��
extern int DrawObtainsString_WCHAR_T(
	int x , int y ,
	int AddY ,
	const wchar_t *String ,
	int StrLen,
	unsigned int StrColor ,
	unsigned int StrEdgeColor ,
	int FontHandle,
	unsigned int SelectBackColor ,
	unsigned int SelectStrColor ,
	unsigned int SelectStrEdgeColor ,
	int SelectStart ,
	int SelectEnd,
	int DrawFlag,
	int *PosX,
	int *PosY
)
{
#ifndef DX_NON_GRAPHICS
	int PointX , PointY ;
//	int Width ;
	int StrWidth ;
	int Use3DFlag ;
	RECT DrawRect ;
//	RECT Rect ;

	// FontHandle �� -1 �̏ꍇ�̓f�t�H���g�̃t�H���g���g�p����
	if( FontHandle == -1 ) FontHandle = NS_GetDefaultFontHandle() ;

	if( DrawFlag )
	{
		// �R�c�L���t���O�𓾂�
		Use3DFlag = NS_GetUse3DFlag() ;
		NS_SetUse3DFlag( FALSE ) ;
	}

	// �`��\�̈�𓾂�
	NS_GetDrawArea( &DrawRect ) ;

	// �����l���Z�b�g
	if( PosX )
	{
		*PosX = 0 ;
	}
	if( PosY )
	{
		*PosY = 0 ;
	}

	// �`��ʒu��␳
	if( x < DrawRect.left ) x = DrawRect.left ;
	if( y < DrawRect.top  ) y = DrawRect.top ;

	while( x > DrawRect.right )
	{
		x -= DrawRect.right - DrawRect.left ;
		y += AddY ;
	}

	// ���͕������`��
//	if( SelectStart == -1 )
//	{
//		// �`�敶����̒����𓾂�
//		StrWidth = GetDrawStringWidthToHandle_WCHAR_T( String , ( int )_WCSLEN( String ) , FontHandle ) ;
//
//		Width = StrWidth ;
//		PointX = x ;
//		PointY = y ;
//		for(;;)
//		{
//			DrawStringToHandle_WCHAR_T( PointX , PointY , String , StrColor , FontHandle , StrEdgeColor ) ;
//
//			SETRECT( Rect , PointX , PointY , PointX + StrWidth , PointY + AddY ) ;
//			RectClipping_Inline( &Rect , &DrawRect ) ;
//
//			Width -= Rect.right - Rect.left ;
//
//			if( Width > 0 )
//			{
//				PointX = DrawRect.left - ( StrWidth - Width ) ;
//				PointY += AddY ;
//			}
//			else break ;
//		}
//	}
//	else
	{
		int i, Num, smin = 0, smax = 0 ;
		wchar_t TempBuf[ 3 ] ;

		if( SelectStart != -1 )
		{
			if( SelectStart < SelectEnd )
			{
				smax = SelectEnd ;
				smin = SelectStart ;
			}
			else
			{
				smax = SelectStart ;
				smin = SelectEnd ;
			}
		}

		if( StrLen < 0 )
		{
			StrLen = ( int )_WCSLEN( String ) ;
		}
		PointX = x ;
		PointY = y ;
		for( i = 0 ; i < StrLen ; )
		{
			if( CHECK_WCHAR_T_DOUBLE( String[ i ] ) )
			{
				TempBuf[ 0 ] = String[ i ] ;
				TempBuf[ 1 ] = String[ i + 1 ] ;
				TempBuf[ 2 ] = 0 ;
				Num = 2 ;
			}
			else
			{
				if( String[ i ] == L'\n' )
				{
					PointX = x ;
					PointY += AddY ;
					i ++ ;
					continue ;
				}
				else
				{
					TempBuf[ 0 ] = String[ i ] ;
					TempBuf[ 1 ] = 0 ;
					Num = 1 ;
				}
			}

			StrWidth = GetDrawStringWidthToHandle_WCHAR_T( TempBuf, ( int )_WCSLEN( TempBuf ), FontHandle ) ;
			if( DrawFlag )
			{
				if( SelectStart != -1 && i >= smin && i < smax )
				{
					NS_DrawBox( PointX, PointY, PointX + StrWidth, PointY + NS_GetFontSizeToHandle( FontHandle ), SelectBackColor, TRUE ) ; 
					DrawStringToHandle_WCHAR_T( PointX, PointY, TempBuf, SelectStrColor, FontHandle, SelectStrEdgeColor ) ;
				}
				else
				{
					DrawStringToHandle_WCHAR_T( PointX, PointY, TempBuf, StrColor, FontHandle, StrEdgeColor ) ;
				}
			}

			if( PointX + StrWidth > DrawRect.right )
			{
				PointX = DrawRect.left - ( DrawRect.right - PointX ) ;
				PointY += AddY ;
			}
			else
			{
				PointX += StrWidth ;
				i += Num ;
			}
		}
	}

	if( DrawFlag )
	{
		// �R�c�`��t���O�����ɖ߂�
		NS_SetUse3DFlag( Use3DFlag ) ;
	}

	// ���W���Z�b�g
	if( PosX )
	{
		*PosX = PointX ;
	}
	if( PosY )
	{
		*PosY = PointY ;
	}

	// �I��
	return 0 ;
#else // DX_NON_GRAPHICS
	return -1 ;
#endif // DX_NON_GRAPHICS
}

// �`��\�̈�Ɏ��܂�悤�ɉ��s���Ȃ��當�����`��( �N���b�v�������P�� )
extern int NS_DrawObtainsString_CharClip(
	int x, int y,
	int AddY,
	const TCHAR *String,
	unsigned int StrColor,
	unsigned int StrEdgeColor,
	int FontHandle,
	unsigned int SelectBackColor,
	unsigned int SelectStrColor,
	unsigned int SelectStrEdgeColor,
	int SelectStart,
	int SelectEnd
)
{
#ifdef UNICODE
	return DrawObtainsString_CharClip_WCHAR_T(
		x, y,
		AddY,
		String,
		-1,
		StrColor,
		StrEdgeColor,
		FontHandle,
		SelectBackColor,
		SelectStrColor,
		SelectStrEdgeColor,
		SelectStart,
		SelectEnd,
		TRUE,
		NULL,
		NULL
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( String, return -1 )

	Result = DrawObtainsString_CharClip_WCHAR_T(
		x, y,
		AddY,
		UseStringBuffer,
		-1,
		StrColor,
		StrEdgeColor,
		FontHandle,
		SelectBackColor,
		SelectStrColor,
		SelectStrEdgeColor,
		SelectStart,
		SelectEnd,
		TRUE,
		NULL,
		NULL
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( String )

	return Result ;
#endif
}

// �`��\�̈�Ɏ��܂�悤�ɉ��s���Ȃ��當�����`��( �N���b�v�������P�� )
extern int DrawObtainsString_CharClip_WCHAR_T(
	int x, int y,
	int AddY,
	const wchar_t *String,
	int StrLen,
	unsigned int StrColor,
	unsigned int StrEdgeColor,
	int FontHandle,
	unsigned int SelectBackColor,
	unsigned int SelectStrColor,
	unsigned int SelectStrEdgeColor,
	int SelectStart,
	int SelectEnd,
	int DrawFlag,
	int *PosX,
	int *PosY
)
{
#ifndef DX_NON_GRAPHICS
	int PointX , PointY ;
	int StrWidth ;
	int Use3DFlag ;
	RECT DrawRect ;
	int i, Num ;
	wchar_t TempBuf[ 3 ] ;

	// FontHandle �� -1 �̏ꍇ�̓f�t�H���g�̃t�H���g���g�p����
	if( FontHandle == -1 )
	{
		FontHandle = NS_GetDefaultFontHandle() ;
	}

	if( DrawFlag )
	{
		// �R�c�L���t���O�𓾂�
		Use3DFlag = NS_GetUse3DFlag() ;
		NS_SetUse3DFlag( FALSE ) ;
	}

	// �`��\�̈�𓾂�
	NS_GetDrawArea( &DrawRect ) ;

	// �����l���Z�b�g
	if( PosX )
	{
		*PosX = 0 ;
	}
	if( PosY )
	{
		*PosY = 0 ;
	}

	// �`��͈͂ɕ��������ꍇ�͉��������ɏI��
	if( DrawRect.left == DrawRect.right ||
		DrawRect.top  == DrawRect.bottom )
	{
		return 0 ;
	}

	// �`��ʒu��␳
	if( x < DrawRect.left ) x = DrawRect.left ;
	if( y < DrawRect.top  ) y = DrawRect.top ;

	while( x > DrawRect.right )
	{
		x -= DrawRect.right - DrawRect.left ;
		y += AddY ;
	}

	if( StrLen < 0 )
	{
		StrLen = ( int )_WCSLEN( String ) ;
	}

	// ���͕������`��
//	if( SelectStart == -1 )
//	{
//		PointX = x ;
//		PointY = y ;
//		for( i = 0 ; i < StrLen ; )
//		{
//			if( CHECK_WCHAR_T_DOUBLE( String[ i ] ) )
//			{
//				TempBuf[ 0 ] = String[ i ] ;
//				TempBuf[ 1 ] = String[ i + 1 ] ;
//				TempBuf[ 2 ] = 0 ;
//				Num = 2 ;
//			}
//			else
//			{
//				TempBuf[ 0 ] = String[ i ] ;
//				TempBuf[ 1 ] = 0 ;
//				Num = 1 ;
//			}
//
//			StrWidth = GetDrawStringWidthToHandle_WCHAR_T( TempBuf, Num, FontHandle ) ;
//			if( PointX + StrWidth > DrawRect.right )
//			{
//				PointX = x ;
//				PointY += AddY ;
//			}
//
//			DrawStringToHandle_WCHAR_T( PointX, PointY, TempBuf, StrColor, FontHandle, StrEdgeColor ) ;
//
//			PointX += StrWidth ;
//			i += Num ;
//		}
//	}
//	else
	{
		int smin = 0, smax = 0;

		if( SelectStart != -1 )
		{
			if( SelectStart < SelectEnd )
			{
				smax = SelectEnd ;
				smin = SelectStart ;
			}
			else
			{
				smax = SelectStart ;
				smin = SelectEnd ;
			}
		}

		PointX = x ;
		PointY = y ;
		for( i = 0 ; i < StrLen ; )
		{
			if( CHECK_WCHAR_T_DOUBLE( String[ i ] ) )
			{
				TempBuf[ 0 ] = String[ i ] ;
				TempBuf[ 1 ] = String[ i + 1 ] ;
				TempBuf[ 2 ] = 0 ;
				Num = 2 ;
			}
			else
			{
				if( String[ i ] == L'\n' )
				{
					PointX = x ;
					PointY += AddY ;
					i ++ ;
					continue ;
				}
				else
				{
					TempBuf[ 0 ] = String[ i ] ;
					TempBuf[ 1 ] = 0 ;
					Num = 1 ;
				}
			}

			StrWidth = GetDrawStringWidthToHandle_WCHAR_T( TempBuf, Num, FontHandle ) ;
			if( PointX + StrWidth > DrawRect.right )
			{
				PointX = x ;
				PointY += AddY ;
			}

			if( DrawFlag )
			{
				if( SelectStart != -1 && i >= smin && i < smax )
				{
					NS_DrawBox( PointX, PointY, PointX + StrWidth, PointY + NS_GetFontSizeToHandle( FontHandle ), SelectBackColor, TRUE ) ; 
					DrawStringToHandle_WCHAR_T( PointX, PointY, TempBuf, SelectStrColor, FontHandle, SelectStrEdgeColor ) ;
				}
				else
				{
					DrawStringToHandle_WCHAR_T( PointX, PointY, TempBuf, StrColor, FontHandle, StrEdgeColor ) ;
				}
			}

			PointX += StrWidth ;
			i += Num ;
		}
	}

	if( DrawFlag )
	{
		// �R�c�`��t���O�����ɖ߂�
		NS_SetUse3DFlag( Use3DFlag ) ;
	}

	// ���W���Z�b�g
	if( PosX )
	{
		*PosX = PointX ;
	}
	if( PosY )
	{
		*PosY = PointY ;
	}

	// �I��
	return 0 ;
#else // DX_NON_GRAPHICS
	return -1 ;
#endif // DX_NON_GRAPHICS
}







// �`��\�̈�Ɏ��܂�悤�ɉ��s���Ȃ��當�����`�悵���ꍇ�̕�����̖��[�̍��W���擾����
extern int NS_GetObtainsStringCharPosition(	int x, int y, int AddY, const TCHAR *String, int StrLen, int *PosX, int *PosY, int FontHandle )
{
#ifdef UNICODE
	return DrawObtainsString_WCHAR_T(
		x , y ,
		AddY ,
		String ,
		StrLen,
		0 ,
		0 ,
		FontHandle,
		0 ,
		0 ,
		0 ,
		0 ,
		0 ,
		FALSE,
		PosX,
		PosY
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( String, return -1 )

	Result = DrawObtainsString_WCHAR_T(
		x , y ,
		AddY ,
		UseStringBuffer ,
		StrLen,
		0 ,
		0 ,
		FontHandle,
		0 ,
		0 ,
		0 ,
		0 ,
		0 ,
		FALSE ,
		PosX,
		PosY
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( String )

	return Result ;
#endif
}

// �`��\�̈�Ɏ��܂�悤�ɉ��s���Ȃ��當�����`�悵���ꍇ�̕�����̖��[�̍��W���擾����( �N���b�v�������P�� )
extern int NS_GetObtainsStringCharPosition_CharClip( int x, int y, int AddY, const TCHAR *String, int StrLen, int *PosX, int *PosY, int FontHandle )
{
#ifdef UNICODE
	return DrawObtainsString_CharClip_WCHAR_T(
		x, y,
		AddY,
		String,
		StrLen,
		0,
		0,
		FontHandle,
		0,
		0,
		0,
		0,
		0,
		FALSE,
		PosX,
		PosY
	) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( String, return -1 )

	Result = DrawObtainsString_CharClip_WCHAR_T(
		x, y,
		AddY,
		UseStringBuffer,
		StrLen,
		0,
		0,
		FontHandle,
		0,
		0,
		0,
		0,
		0,
		FALSE,
		PosX,
		PosY
	) ;

	TCHAR_TO_WCHAR_T_STRING_END( String )

	return Result ;
#endif
}





#endif // DX_NON_FONT

// �K��̈�Ɏ��߂��������ŋ�`��`�� 
extern int NS_DrawObtainsBox( int x1 , int y1 , int x2 , int y2 , int AddY , unsigned int Color , int FillFlag )
{
#ifndef DX_NON_GRAPHICS
	int AllWidth , PointX , PointY ;
	int Width , Height ;
	int Use3DFlag ;
	RECT DrawRect , Rect ;

	// �R�c�L���t���O�𓾂�
	Use3DFlag = NS_GetUse3DFlag() ;
	NS_SetUse3DFlag( FALSE ) ;

	// �`�悷�钷���ƍ����𓾂�
	AllWidth = x2 - x1 ;
	Height = y2 - y1 ;

	// �`��\�̈�𓾂�
	NS_GetDrawArea( &DrawRect ) ;

	// �`��ʒu��␳
	if( x1 < DrawRect.left ){ x2 += DrawRect.left - x1 ; x1 = DrawRect.left ; }
	if( y1 < DrawRect.top  ){ y2 += DrawRect.top  - y1 ; y1 = DrawRect.top  ; }

	while( x1 > DrawRect.right )
	{
		x1 -= DrawRect.right - DrawRect.left ;
		y1 += AddY ;
	}

	// ���͕������`��
	Width = AllWidth ;
	PointX = x1 ;
	PointY = y1 ;
	for(;;)
	{
		NS_DrawBox( PointX , PointY , PointX + AllWidth , PointY + Height , Color , FillFlag ) ;

		SETRECT( Rect , PointX , PointY , PointX + AllWidth , PointY + AddY ) ;
		RectClipping_Inline( &Rect , &DrawRect ) ;

		Width -= Rect.right - Rect.left ;

		if( Width > 0 )
		{
			PointX = DrawRect.left - ( AllWidth - Width ) ;
			PointY += AddY ;
		}
		else break ;
	}

	// �R�c�`��t���O�����ɖ߂�
	NS_SetUse3DFlag( Use3DFlag ) ;

	// �I��
	return 0 ;
#else // DX_NON_GRAPHICS
	return -1 ;
#endif // DX_NON_GRAPHICS
}



// �A�X�L�[�R���g���[���R�[�h�����ׂ�
extern int NS_GetCtrlCodeCmp( TCHAR Char ) 
{
	return GetCtrlCodeCmp_WCHAR_T( ( wchar_t )Char ) ;
}

extern int GetCtrlCodeCmp_WCHAR_T( wchar_t Char ) 
{
	int i ;

	for( i = 0 ; CtrlCode[ i ][ 0 ] != 0  &&  ( wchar_t )CtrlCode[ i ][ 1 ] != Char ; i ++ ){}
	
	// ���ʂ�Ԃ�
	return CtrlCode[ i ][ 0 ] != 0 ;
}

// �h�l�d�ɕω������������A�t���O�̎擾
extern int GetIMEChangeFlag( void )
{
	int Flag = CharBuf.ChangeFlag ;

	CharBuf.ChangeFlag = FALSE ;
	return Flag ;
}



#ifndef DX_NON_KEYEX

// ������̓��͎擾
extern int NS_InputStringToCustom(
	int x , int y ,
	size_t BufLength ,
	TCHAR *StrBuffer ,
	int CancelValidFlag ,
	int SingleCharOnlyFlag ,
	int NumCharOnlyFlag,
	int DoubleCharOnlyFlag
)
{
#ifdef UNICODE
	return InputStringToCustom_WCHAR_T(
		x , y ,
		BufLength ,
		StrBuffer ,
		CancelValidFlag ,
		SingleCharOnlyFlag ,
		NumCharOnlyFlag,
		DoubleCharOnlyFlag
	) ;
#else
	int Result ;
	wchar_t TempBuffer[ 512 ] ;
	wchar_t *UseTempBuffer ;
	wchar_t *AllocTempBuffer = NULL ;

	if( BufLength + 1 > sizeof( TempBuffer ) / sizeof( wchar_t ) )
	{
		AllocTempBuffer = ( wchar_t * )DXALLOC( ( BufLength + 1 ) * sizeof( wchar_t ) ) ;
		if( AllocTempBuffer == NULL )
		{
			return -1 ;
		}

		UseTempBuffer = AllocTempBuffer ;
	}
	else
	{
		UseTempBuffer = TempBuffer ;
	}

	Result = InputStringToCustom_WCHAR_T(
		x , y ,
		BufLength ,
		UseTempBuffer ,
		CancelValidFlag ,
		SingleCharOnlyFlag ,
		NumCharOnlyFlag,
		DoubleCharOnlyFlag
	) ;

	// �L�����Z������Ă��Ȃ��ꍇ�̂ݕ������ϊ�
	if( Result == 1 )
	{
		ConvString( ( const char * )UseTempBuffer, WCHAR_T_CHARCODEFORMAT, StrBuffer, BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;
	}

	if( AllocTempBuffer != NULL )
	{
		DXFREE( AllocTempBuffer ) ;
		AllocTempBuffer = NULL ;
	}

	return Result ;
#endif
}

// ������̓��͎擾
extern int InputStringToCustom_WCHAR_T(
	int x , int y ,
	size_t BufLength ,
	wchar_t *StrBuffer ,
	int CancelValidFlag ,
	int SingleCharOnlyFlag ,
	int NumCharOnlyFlag,
	int DoubleCharOnlyFlag
)
{
	int ScreenGraphFront, ScreenGraphBack ;
	int EndFlag = 0 ;
	RECT DrawRect ;
	int FontSize ;
	int DrawWidth , DrawHeight ;
//	wchar_t *Buffer ;
	int Red , Green , Blue ;
	int InputHandle ;
	int ScreenWidth, ScreenHeight ;
	SCREENDRAWSETTINGINFO ScreenDrawSettingInfo ;

	// �e��`��ݒ�����擾
	Graphics_DrawSetting_GetScreenDrawSettingInfo( &ScreenDrawSettingInfo ) ;
	GetTransColor( &Red , &Green , &Blue ) ;
	
	// ��ʂ̑傫���𓾂�
	GetDrawScreenSize( &ScreenWidth, &ScreenHeight ) ;

	// �`��̈�𓾂�
	GetDrawArea( &DrawRect ) ;

	// �u�q�`�l�̎g�p�A�R�c�`��@�\�̎g�p�ύX
	SetUseSystemMemGraphCreateFlag( CheckFontChacheToTextureFlag( GetDefaultFontHandle() ) == TRUE ? FALSE : TRUE ) ;
	SetUse3DFlag( FALSE ) ;

	// ���ߐF��^�����ɂ���
	SetTransColor( 0 , 0 , 0 ) ;

	// �h�l�d��L���ɂ���
	CharBuf.IMEUseFlag_System = TRUE ;
	RefreshIMEFlag() ;

	// ��ʂ�ۑ����邽�߂̃��������m��
	DrawWidth			= DrawRect.right  - DrawRect.left ;
	DrawHeight			= DrawRect.bottom - DrawRect.top ; 
	ScreenGraphFront	= MakeGraph( ScreenWidth, ScreenHeight ) ;
	ScreenGraphBack		= MakeGraph( ScreenWidth, ScreenHeight ) ;
	SetDrawScreen( DX_SCREEN_FRONT ) ;
	SetDrawArea( 0, 0, ScreenWidth, ScreenHeight ) ;
	GetDrawScreenGraph( 0, 0, ScreenWidth, ScreenHeight, ScreenGraphFront ) ;
	SetDrawScreen( DX_SCREEN_BACK ) ;
	SetDrawArea( 0, 0, ScreenWidth, ScreenHeight ) ;
	GetDrawScreenGraph( 0, 0, ScreenWidth, ScreenHeight, ScreenGraphBack ) ;

	// �������͗p�o�b�t�@�̊m��
//	if( ( Buffer = ( wchar_t * )DXCALLOC( BufLength + 1 ) ) == NULL ) return -1 ;

	// ���̓n���h���𓾂�
	InputHandle = MakeKeyInput( BufLength , CancelValidFlag , SingleCharOnlyFlag , NumCharOnlyFlag, DoubleCharOnlyFlag ) ;
	if( InputHandle != -1 )
	{
		// ���͂��A�N�e�B�u�ɂ���
		SetActiveKeyInput( InputHandle ) ;

		// ���͕��������O�ɂ���
		FontSize = GetFontSize() ;

		// ���W�̕␳
//		x -= DrawRect.left ;
//		y -= DrawRect.top ;

		// �P��`��
		DrawKeyInputString( x , y , InputHandle ) ;

		// �`���𗠉�ʂ�
		SetDrawScreen( DX_SCREEN_BACK ) ;
		while( ProcessMessage() == 0 )
		{
			// ���͂��I�����Ă���ꍇ�͏I��
			EndFlag = CheckKeyInput( InputHandle ) ;
			if( EndFlag ) break ;

			// ���͕����̍X�V
			{
				// ���Ɖ�ʂ̕`��
				DrawGraph( 0 , 0 , ScreenGraphFront , FALSE ) ;

				// �h�l�d���̓��[�h�̕`��
				DrawKeyInputModeString( DrawRect.right , DrawRect.bottom ) ; 

				// ���͏�Ԃ̕`��
				SetDrawArea( DrawRect.left, DrawRect.top, DrawRect.right, DrawRect.bottom ) ;
				DrawKeyInputString( x , y , InputHandle ) ;
				SetDrawArea( 0, 0, ScreenWidth, ScreenHeight ) ;

				// �\��ʂɓ]��
				ScreenFlip() ;
			}
		}
	}

	// �E�C���h�E������ꂽ�ꍇ�́|�P��Ԃ�
	if( ProcessMessage() == -1 ) return -1;

	// �L�����Z������Ă��Ȃ��ꍇ�͕�������擾
	if( EndFlag == 1 )
	{
		GetKeyInputString_WCHAR_T( StrBuffer , InputHandle ) ;
	}

	// ��ʂ����ɂ��ǂ�
	SetDrawArea( 0, 0, ScreenWidth, ScreenHeight ) ;
	DrawGraph( 0, 0, ScreenGraphBack, FALSE ) ;
	SetDrawScreen( DX_SCREEN_FRONT ) ;
	DrawGraph( 0, 0, ScreenGraphFront, FALSE ) ;

	// �O���t�B�b�N�f�[�^������
	DeleteKeyInput( InputHandle ) ;
	DeleteGraph( ScreenGraphFront ) ;
	DeleteGraph( ScreenGraphBack ) ;

	// �`��ݒ�������ɖ߂�
	Graphics_DrawSetting_SetScreenDrawSettingInfo( &ScreenDrawSettingInfo ) ;
	SetTransColor( Red , Green , Blue ) ;

	// �I��
	return EndFlag ;
}


// ������̓��͎擾
extern int NS_KeyInputString( int x , int y , size_t CharMaxLength , TCHAR *StrBuffer , int CancelValidFlag )
{
	return NS_InputStringToCustom( x , y , CharMaxLength , StrBuffer , CancelValidFlag , FALSE , FALSE, FALSE ) ;
}

// ������̓��͎擾
extern int KeyInputString_WCHAR_T( int x , int y , size_t CharMaxLength , wchar_t *StrBuffer , int CancelValidFlag )
{
	return InputStringToCustom_WCHAR_T( x , y , CharMaxLength , StrBuffer , CancelValidFlag , FALSE , FALSE, FALSE ) ;
}

// ���p������݂̂̓��͎擾
extern int NS_KeyInputSingleCharString( int x , int y , size_t CharMaxLength , TCHAR *StrBuffer , int CancelValidFlag )
{
	return NS_InputStringToCustom( x , y, CharMaxLength , StrBuffer , CancelValidFlag , TRUE , FALSE, FALSE ) ;
}

// ���p������݂̂̓��͎擾
extern int KeyInputSingleCharString_WCHAR_T( int x , int y , size_t CharMaxLength , wchar_t *StrBuffer , int CancelValidFlag )
{
	return InputStringToCustom_WCHAR_T( x , y, CharMaxLength , StrBuffer , CancelValidFlag , TRUE , FALSE, FALSE ) ;
}

// ���l�̓���
extern int NS_KeyInputNumber( int x , int y , int MaxNum , int MinNum , int CancelValidFlag )
{
	int Num ;
	wchar_t Buffer[ 50 ] ;
	int StrLen , hr ;

	// �ő啶���񕝂��擾
	{
		int MaxLen , MinLen ;

		_SWNPRINTF( Buffer, sizeof( Buffer ) / 2, L"%d", MaxNum ) ;
		MaxLen = ( int )_WCSLEN( Buffer ) ;

		_SWNPRINTF( Buffer, sizeof( Buffer ) / 2, L"%d", MinNum ) ;
		MinLen = ( int )_WCSLEN( Buffer ) ;

		StrLen = MaxLen > MinLen ? MaxLen : MinLen ;
	}

	// �������擾
	hr = InputStringToCustom_WCHAR_T( x , y , StrLen , Buffer , CancelValidFlag , TRUE , TRUE ) ;
	if( hr == -1 )
	{
		return MinNum - 1 ;
	}
	if( hr == 2 )
	{
		return MaxNum + 1 ;
	}

	// ���l�ɕϊ�
	Num = _ATOIW( Buffer ) ;

	// �K��͈͂̒��Ɏ擾�������l�����Ă͂܂�Ȃ��Ԃ͌J��Ԃ�
	while( ProcessMessage() == 0 && ( Num > MaxNum || Num < MinNum ) )
	{
		hr = InputStringToCustom_WCHAR_T( x , y , StrLen , Buffer , CancelValidFlag , TRUE , TRUE ) ;
		if( hr == -1 )
		{
			return MinNum - 1 ;
		}
		if( hr == 2 )
		{
			return MaxNum + 1 ;
		}

		// ���l�ɕϊ�
		Num = _ATOIW( Buffer ) ;
	}

	// �I��
	return Num ;
}

// IME�̓��̓��[�h������̎擾
extern int NS_GetIMEInputModeStr( TCHAR *GetBuffer ) 
{
#ifdef UNICODE
	return GetIMEInputModeStr_WCHAR_T( GetBuffer ) ;
#else
	wchar_t TempBuffer[ 128 ] ;
	int Result ;

	Result = GetIMEInputModeStr_WCHAR_T( TempBuffer ) ;

	ConvString( ( const char * )TempBuffer, WCHAR_T_CHARCODEFORMAT, GetBuffer, BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;

	return Result ;
#endif
}

// IME�̓��̓��[�h������̎擾
extern int GetIMEInputModeStr_WCHAR_T( wchar_t *GetBuffer ) 
{
	HIMC	Imc ;
	DWORD	InputState , SentenceState , Buf ;
	wchar_t	*SelectStr ;
	static  int StringSetup = FALSE ;
	static  wchar_t ZenkakuKanaString[ 32 ] ;
	static  wchar_t HankakuKanaString[ 32 ] ;
	static  wchar_t ZenkakuHiraString[ 32 ] ;
	static  wchar_t ZenkakuEisuString[ 32 ] ;
	static  wchar_t HankakuEisuString[ 32 ] ;
	static  wchar_t FumeiString[ 32 ] ;

	// �h�l�d���g���Ă��Ȃ��Ƃ���-1��Ԃ�
	if( CharBuf.IMESwitch == FALSE ) return -1 ;

	// ������̏������ł��Ă��Ȃ��ꍇ�͏������s��
	if( StringSetup == FALSE )
	{
		ConvString( "\x68\x51\xd2\x89\xab\x30\xbf\x30\xab\x30\xca\x30\x00"/*@ L"�S�p�J�^�J�i" @*/, DX_CHARCODEFORMAT_UTF16LE, ( char * )ZenkakuKanaString, sizeof( ZenkakuKanaString ), WCHAR_T_CHARCODEFORMAT ) ;
		ConvString( "\x4a\x53\xd2\x89\xab\x30\xbf\x30\xab\x30\xca\x30\x00"/*@ L"���p�J�^�J�i" @*/, DX_CHARCODEFORMAT_UTF16LE, ( char * )HankakuKanaString, sizeof( HankakuKanaString ), WCHAR_T_CHARCODEFORMAT ) ;
		ConvString( "\x68\x51\xd2\x89\x72\x30\x89\x30\x4c\x30\x6a\x30\x00"/*@ L"�S�p�Ђ炪��" @*/, DX_CHARCODEFORMAT_UTF16LE, ( char * )ZenkakuHiraString, sizeof( ZenkakuHiraString ), WCHAR_T_CHARCODEFORMAT ) ;
		ConvString( "\x68\x51\xd2\x89\xf1\x82\x70\x65\x00"/*@ L"�S�p�p��" @*/, DX_CHARCODEFORMAT_UTF16LE, ( char * )ZenkakuEisuString, sizeof( ZenkakuEisuString ), WCHAR_T_CHARCODEFORMAT ) ;
		ConvString( "\x4a\x53\xd2\x89\xf1\x82\x70\x65\x00"/*@ L"���p�p��" @*/, DX_CHARCODEFORMAT_UTF16LE, ( char * )HankakuEisuString, sizeof( HankakuEisuString ), WCHAR_T_CHARCODEFORMAT ) ;
		ConvString( "\x0d\x4e\x0e\x66\x6a\x30\x65\x51\x9b\x52\xe2\x30\xfc\x30\xc9\x30\x00"/*@ L"�s���ȓ��̓��[�h" @*/, DX_CHARCODEFORMAT_UTF16LE, ( char * )FumeiString, sizeof( FumeiString ), WCHAR_T_CHARCODEFORMAT ) ;

		StringSetup = TRUE ;
	}

	// ���̓R���e�L�X�g���擾
	HWND DefHwnd = WinData.MainWindow;
	Imc = WinAPIData.ImmFunc.ImmGetContextFunc( DefHwnd ) ;

	// ���̓��[�h�𓾂�
	WinAPIData.ImmFunc.ImmGetConversionStatusFunc( Imc , &InputState , &SentenceState ) ;

	// ���f
	Buf = InputState & ( IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE | IME_CMODE_KATAKANA ) ;
	if( Buf == ( IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE | IME_CMODE_KATAKANA ) )
	{
		SelectStr = ZenkakuKanaString ;
	}
	else
	{
		Buf = InputState & ( IME_CMODE_NATIVE | IME_CMODE_KATAKANA ) ;
		if( Buf == ( IME_CMODE_NATIVE | IME_CMODE_KATAKANA ) )
		{
			SelectStr = HankakuKanaString ;
		}
		else
		{
			Buf = InputState & ( IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE ) ;
			if( Buf == ( IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE ) )
			{
				SelectStr = ZenkakuHiraString ;
			}
			else
			{
				Buf = InputState & IME_CMODE_FULLSHAPE ;
				if( Buf == IME_CMODE_FULLSHAPE )
				{
					SelectStr = ZenkakuEisuString ;
				}
				else
				{
					Buf = InputState & IME_CMODE_ALPHANUMERIC ;
					if( Buf == IME_CMODE_ALPHANUMERIC )
					{
						SelectStr = HankakuEisuString ;
					}
					else
					{
						SelectStr = FumeiString ;
					}
				}
			}
		}
	}

	_WCSCPY( GetBuffer , SelectStr ) ;

	// ���̓R���e�L�X�g�̍폜
	WinAPIData.ImmFunc.ImmReleaseContextFunc( DefHwnd , Imc );

	// �I��
	return 0 ;
}

// InputString�֐��g�p���̕����̊e�F��ύX����
extern int NS_SetKeyInputStringColor(
	ULONGLONG NmlStr ,				// ���͕�����̐F
	ULONGLONG NmlCur ,				// �h�l�d��g�p���̃J�[�\���̐F
	ULONGLONG IMEStrBack ,			// �h�l�d�g�p���̓��͕�����̔w�i�̐F
	ULONGLONG IMECur ,				// �h�l�d�g�p���̃J�[�\���̐F
	ULONGLONG IMELine ,				// �h�l�d�g�p���̕ϊ�������̉���
	ULONGLONG IMESelectStr ,		// �h�l�d�g�p���̑I��Ώۂ̕ϊ���╶����̐F
	ULONGLONG IMEModeStr  ,			// �h�l�d�g�p���̓��̓��[�h������̐F(�u�S�p�Ђ炪�ȁv��)
	ULONGLONG NmlStrE ,				// ���͕�����̉��̐F
	ULONGLONG IMESelectStrE ,		// �h�l�d�g�p���̑I��Ώۂ̕ϊ���╶����̉��̐F
	ULONGLONG IMEModeStrE	,		// �h�l�d�g�p���̓��̓��[�h������̉��̐F
	ULONGLONG IMESelectWinE ,		// �h�l�d�g�p���̕ϊ����E�C���h�E�̉��̐F
	ULONGLONG IMESelectWinF	,		// �h�l�d�g�p���̕ϊ����E�C���h�E�̉��n�̐F
	ULONGLONG SelectStrBackColor ,	// �I�����ꂽ���͕�����̔w�i�̐F
	ULONGLONG SelectStrColor ,		// �I�����ꂽ���͕�����̐F
	ULONGLONG SelectStrEdgeColor,	// �I�����ꂽ���͕�����̉��̐F
	ULONGLONG IMEStr,				// �h�l�d�g�p���̓��͕�����̐F
	ULONGLONG IMEStrE				// �h�l�d�g�p���̓��͕�����̉��̐F
	)
{
	if( NmlStr != -1 )
	{
		*((DWORD *)&CharBuf.NormalStrColor)     = *((DWORD *)&NmlStr) ;
		*((DWORD *)&CharBuf.IMEConvWinStrColor) = *((DWORD *)&NmlStr) ;
	}

	if( NmlCur != -1 )
	{
		*((DWORD *)&CharBuf.NormalCursorColor) = *((DWORD *)&NmlCur) ;
	}

	if( IMEStrBack != -1 )
	{
		*((DWORD *)&CharBuf.IMEStrBackColor) = *((DWORD *)&IMEStrBack) ;
	}

	if( IMECur != -1 )
	{
		*((DWORD *)&CharBuf.IMECursorColor) = *((DWORD *)&IMECur) ;
	}

	if( IMELine != -1 )
	{
		*((DWORD *)&CharBuf.IMELineColor) = *((DWORD *)&IMELine) ;
	}

	if( IMESelectStr != -1 )
	{
		*((DWORD *)&CharBuf.IMESelectStrBackColor)    = *((DWORD *)&IMESelectStr) ;
		*((DWORD *)&CharBuf.IMEConvWinSelectStrColor) = *((DWORD *)&IMESelectStr) ;
	}

	if( IMEModeStr != -1 )
	{
		*((DWORD *)&CharBuf.IMEModeStrColor) = *((DWORD *)&IMEModeStr) ;
	}

	if( NmlStrE != -1 )
	{
		*((DWORD *)&CharBuf.NormalStrEdgeColor)     = *((DWORD *)&NmlStrE) ;
		*((DWORD *)&CharBuf.IMEConvWinStrEdgeColor) = *((DWORD *)&NmlStrE) ;
	}

	if( IMESelectStrE != -1 )
	{
		*((DWORD *)&CharBuf.IMEConvWinSelectStrEdgeColor) = *((DWORD *)&IMESelectStrE) ;
	}

	if( IMEModeStrE != -1 )
	{
		*((DWORD *)&CharBuf.IMEModeStrEdgeColor) = *((DWORD *)&IMEModeStrE) ;
	}

	if( IMESelectWinE != -1 )
	{
		*((DWORD *)&CharBuf.IMEConvWinEdgeColor) = *((DWORD *)&IMESelectWinE) ;
	}
	else
	{
		if( IMEStrBack != -1 )
		{
			CharBuf.IMEConvWinEdgeColor = CharBuf.IMEStrBackColor;
		}
	}

	if( IMESelectWinF != -1 )
	{
		*((DWORD *)&CharBuf.IMEConvWinBackColor) = *((DWORD *)&IMESelectWinF) ;
	}
	else
	{
		CharBuf.IMEConvWinBackColor = NS_GetColor( 0, 0, 0 );
	}

	if( SelectStrBackColor != -1 )
	{
		*((DWORD *)&CharBuf.SelectStrBackColor) = *((DWORD *)&SelectStrBackColor) ;
	}
	else
	{
		CharBuf.SelectStrBackColor = NS_GetColor( 255, 255, 255 );
	}

	if( SelectStrColor != -1 )
	{
		*((DWORD *)&CharBuf.SelectStrColor) = *((DWORD *)&SelectStrColor) ;
	}
	else
	{
		CharBuf.SelectStrColor = NS_GetColor( 0, 0, 0 );
	}

	if( SelectStrEdgeColor != -1 )
	{
		*((DWORD *)&CharBuf.SelectStrEdgeColor) = *((DWORD *)&SelectStrEdgeColor) ;
	}
	else
	{
		CharBuf.SelectStrEdgeColor = NS_GetColor( 128, 128, 128 );
	}

	if( IMEStr != -1 )
	{
		*((DWORD *)&CharBuf.IMEStrColor) = *((DWORD *)&IMEStr) ;
	}

	if( IMEStrE != -1 )
	{
		CharBuf.IMEStrEdgeColorEnable = TRUE ;
		*((DWORD *)&CharBuf.IMEStrEdgeColor) = *((DWORD *)&IMEStrE) ;
	}

	// �I��
	return 0 ;
}

// InputString�֐��g�p���̕����̊e�F��ύX����
extern int NS_SetKeyInputStringColor2(	int TargetColor /* DX_KEYINPSTRCOLOR_NORMAL_STR �� */, unsigned int Color )
{
	switch( TargetColor )
	{
	default :
		return -1 ;

	case DX_KEYINPSTRCOLOR_NORMAL_STR :
		CharBuf.NormalStrColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_NORMAL_STR_EDGE :
		CharBuf.NormalStrEdgeColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_NORMAL_CURSOR :
		CharBuf.NormalCursorColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_SELECT_STR :
		CharBuf.SelectStrColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_SELECT_STR_EDGE :
		CharBuf.SelectStrEdgeColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_SELECT_STR_BACK :
		CharBuf.SelectStrBackColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_STR :
		CharBuf.IMEStrColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_STR_EDGE :
		CharBuf.IMEStrEdgeColorEnable = TRUE ;
		CharBuf.IMEStrEdgeColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_STR_BACK :
		CharBuf.IMEStrBackColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CURSOR :
		CharBuf.IMECursorColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_LINE :
		CharBuf.IMELineColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_SELECT_STR :
		CharBuf.IMESelectStrColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_SELECT_STR_EDGE :
		CharBuf.IMESelectStrEdgeColorEnable = TRUE ;
		CharBuf.IMESelectStrEdgeColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_SELECT_STR_BACK :
		CharBuf.IMESelectStrBackColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_STR :
		CharBuf.IMEConvWinStrColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_STR_EDGE :
		CharBuf.IMEConvWinStrEdgeColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_SELECT_STR :
		CharBuf.IMEConvWinSelectStrColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_SELECT_STR_EDGE :
		CharBuf.IMEConvWinSelectStrEdgeColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_SELECT_STR_BACK :
		CharBuf.IMEConvWinSelectStrBackColorEnable = TRUE ;
		CharBuf.IMEConvWinSelectStrBackColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_EDGE :
		CharBuf.IMEConvWinEdgeColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_BACK :
		CharBuf.IMEConvWinBackColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_MODE_STR :
		CharBuf.IMEModeStrColor = Color ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_MODE_STR_EDGE :
		CharBuf.IMEModeStrEdgeColor = Color ;
		break ;
	}

	// ����I��
	return 0 ;
}

// SetKeyInputStringColor2 �Őݒ肵���F���f�t�H���g�ɖ߂�
extern int NS_ResetKeyInputStringColor2(	int TargetColor /* DX_KEYINPSTRCOLOR_NORMAL_STR �� */ )
{
	switch( TargetColor )
	{
	default :
		return -1 ;

	case DX_KEYINPSTRCOLOR_NORMAL_STR :
		CharBuf.NormalStrColor = NS_GetColor( 255 , 255 , 255 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_NORMAL_STR_EDGE :
		CharBuf.NormalStrEdgeColor = NS_GetColor(   0 ,   0 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_NORMAL_CURSOR :
		CharBuf.NormalCursorColor = NS_GetColor( 255 , 255 , 255 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_SELECT_STR :
		CharBuf.SelectStrColor = NS_GetColor(   0 ,   0 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_SELECT_STR_EDGE :
		CharBuf.SelectStrEdgeColor = NS_GetColor( 128 , 128 , 128 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_SELECT_STR_BACK :
		CharBuf.SelectStrBackColor = NS_GetColor( 255 , 255 , 255 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_STR :
		CharBuf.IMEStrColor = NS_GetColor(   0 ,   0 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_STR_EDGE :
		CharBuf.IMEStrEdgeColorEnable = FALSE ;
		CharBuf.IMEStrEdgeColor = NS_GetColor(   0 ,   0 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_STR_BACK :
		CharBuf.IMEStrBackColor = NS_GetColor( 255 , 255 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CURSOR :
		CharBuf.IMECursorColor = NS_GetColor(   0 , 255 , 255 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_LINE :
		CharBuf.IMELineColor = NS_GetColor( 255 , 255 , 255 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_SELECT_STR :
		CharBuf.IMESelectStrColor = NS_GetColor( 0 ,   0 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_SELECT_STR_EDGE :
		CharBuf.IMESelectStrEdgeColorEnable = FALSE ;
		CharBuf.IMESelectStrEdgeColor = NS_GetColor(   0 ,   0 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_SELECT_STR_BACK :
		CharBuf.IMESelectStrBackColor = NS_GetColor( 255 ,   0 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_STR :
		CharBuf.IMEConvWinStrColor = NS_GetColor( 255 , 255 , 255 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_STR_EDGE :
		CharBuf.IMEConvWinStrEdgeColor = NS_GetColor(   0 ,   0 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_SELECT_STR :
		CharBuf.IMEConvWinSelectStrColor = NS_GetColor( 255 ,   0 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_SELECT_STR_EDGE :
		CharBuf.IMEConvWinSelectStrEdgeColor = NS_GetColor(   0 ,   0 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_SELECT_STR_BACK :
		CharBuf.IMEConvWinSelectStrBackColorEnable = FALSE ;
		CharBuf.IMEConvWinSelectStrBackColor = NS_GetColor( 0 ,   0 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_EDGE :
		CharBuf.IMEConvWinEdgeColor = NS_GetColor( 255 , 255 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_CONV_WIN_BACK :
		CharBuf.IMEConvWinBackColor = NS_GetColor(   0 ,   0 ,   0 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_MODE_STR :
		CharBuf.IMEModeStrColor = NS_GetColor( 255 , 255 ,  20 ) ;
		break ;

	case DX_KEYINPSTRCOLOR_IME_MODE_STR_EDGE :
		CharBuf.IMEModeStrEdgeColor = NS_GetColor(   0 ,   0 ,   0 ) ;
		break ;
	}

	// ����I��
	return 0 ;
}

// �L�[���͕�����`��֘A�Ŏg�p����t�H���g�̃n���h����ύX����(-1�Ńf�t�H���g�̃t�H���g�n���h��)
extern int NS_SetKeyInputStringFont( int FontHandle )
{
	CharBuf.UseFontHandle = FontHandle ;

	// �I��
	return 0 ;
}

// �L�[���͕����񏈗��̓��͕����������E�ɒB���Ă����ԂŁA������̖��[�����œ��͂��s��ꂽ�ꍇ�̏������[�h��ύX����
extern int NS_SetKeyInputStringEndCharaMode( int EndCharaMode /* DX_KEYINPSTR_ENDCHARAMODE_OVERWRITE �� */ )
{
	CharBuf.EndCharaMode = EndCharaMode ;

	// �I��
	return 0 ;
}

// ���̓��[�h�������`�悷��
extern int NS_DrawKeyInputModeString( int x , int y )
{
	RECT DrawRect ;
	int FontSize, FontHandle ;

	FontHandle = CharBuf.UseFontHandle == -1 ? NS_GetDefaultFontHandle() : CharBuf.UseFontHandle ;
	FontSize = NS_GetFontSizeToHandle( FontHandle ) ;
	NS_GetDrawArea( &DrawRect ) ;

	// ���̓��[�h��`��
	if( CharBuf.IMESwitch )
	{
		wchar_t InputModeStr[ 20 ] ;
		int Width ;

		// ������擾
		if( GetIMEInputModeStr_WCHAR_T( InputModeStr ) != -1 )
		{
			// ���擾
			Width = GetDrawStringWidthToHandle_WCHAR_T( InputModeStr , ( int )_WCSLEN( InputModeStr ) , FontHandle ) ;

			if( Width    + x > DrawRect.right  ) x = DrawRect.right  - Width ;
			if( FontSize + y > DrawRect.bottom ) y = DrawRect.bottom - ( FontSize + 2 ) ;
			if( x < DrawRect.left ) x = DrawRect.left ;
			if( y < DrawRect.top  ) y = DrawRect.top ;

			// �`��
			DrawStringToHandle_WCHAR_T( x , y , InputModeStr , CharBuf.IMEModeStrColor , FontHandle , CharBuf.IMEModeStrEdgeColor )  ;
		}
	}

	// �I��
	return 0 ;
}

// �L�[���̓f�[�^������
extern int NS_InitKeyInput( void )
{
	int i ;
	INPUTDATA * Input ;

	Input = CharBuf.InputData ;
	for( i = 0 ; i < MAX_INPUT_NUM ; i ++, Input ++ )
	{
		if( Input->UseFlag == TRUE )
		{
			NS_DeleteKeyInput( i | DX_HANDLETYPE_MASK_KEYINPUT | ( Input->ID << DX_HANDLECHECK_ADDRESS ) ) ;
		}
	}

	// �h�l�d�̓��͏��擾�p�Ɋm�ۂ��Ă��郁����������ꍇ�͉��
	if( CharBuf.IMEInputData )
	{
		DXFREE( CharBuf.IMEInputData ) ;
		CharBuf.IMEInputData = NULL ;
	}

	// �A�N�e�B�u�n���h����-1�ɂ���
	CharBuf.ActiveInputHandle = -1 ;

	// �I��
	return 0 ;
}

// �V�����L�[���̓f�[�^�̍쐬
extern int NS_MakeKeyInput( size_t MaxStrLength , int CancelValidFlag , int SingleCharOnlyFlag , int NumCharOnlyFlag, int DoubleCharOnlyFlag, int EnableNewLineFlag )
{
	int i, Result ;
	INPUTDATA * Input ;

	// �g���Ă��Ȃ��L�[���̓f�[�^��T��
	for( i = 0 ; i != MAX_INPUT_NUM && CharBuf.InputData[ i ].UseFlag ; i ++ ){}
	if( i == MAX_INPUT_NUM ) return -1 ;
	Input = &CharBuf.InputData[ i ] ;

	// �f�[�^�̏�����
	_MEMSET( Input, 0, sizeof( *Input ) ) ;
#ifdef UNICODE
	Input->Buffer = ( wchar_t * )DXCALLOC( ( MaxStrLength + 1 ) * sizeof( wchar_t ) ) ;
#else // UNICODE
	Input->TempBufferBytes = ( MaxStrLength + 1 ) * 2 ;
	Input->Buffer = ( wchar_t * )DXCALLOC( ( MaxStrLength + 1 ) * sizeof( wchar_t ) + ( MaxStrLength + 1 ) * 2 ) ;
	Input->TempBuffer = ( char * )( Input->Buffer + MaxStrLength + 1 ) ;
#endif // UNICODE
	if( Input->Buffer == NULL )
	{
		return -1 ;
	}
	Input->MaxStrLength			= ( int )MaxStrLength ;
	Input->CancelValidFlag		= CancelValidFlag ;
	Input->SingleCharOnlyFlag	= SingleCharOnlyFlag ;
	Input->NumCharOnlyFlag		= NumCharOnlyFlag ;
	Input->DoubleCharOnlyFlag	= DoubleCharOnlyFlag ;
	Input->EnableNewLineFlag	= EnableNewLineFlag ;
	Input->UseFlag				= TRUE ;
	Input->ID					= CharBuf.HandleID ;
	Input->SelectStart			= -1 ;
	Input->SelectEnd			= -1 ;
	CharBuf.HandleID ++ ;
	if( CharBuf.HandleID >= ( DX_HANDLECHECK_MASK >> DX_HANDLECHECK_ADDRESS ) )
	{
		CharBuf.HandleID = 0 ;
	}

	// �h�l�d��L���ɂ���
	CharBuf.IMEUseFlag_System = TRUE ;
	RefreshIMEFlag() ;

	// �����R�[�h�o�b�t�@���N���A����
	NS_ClearInputCharBuf() ;

	// �n���h���l�̍쐬
	Result = i | DX_HANDLETYPE_MASK_KEYINPUT | ( Input->ID << DX_HANDLECHECK_ADDRESS ) ;

	// �I��
	return Result ;
}

// �L�[���̓f�[�^�̍폜
extern int NS_DeleteKeyInput( int InputHandle )
{
	INPUTDATA * Input ;
	int i ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	// �m�ۂ��Ă����������̉��
	if( Input->Buffer != NULL ) DXFREE( Input->Buffer ) ;

	// �f�[�^�̏�����
	_MEMSET( &CharBuf.InputData[ InputHandle & DX_HANDLEINDEX_MASK ], 0, sizeof( INPUTDATA ) ) ;
	
	if( CharBuf.ActiveInputHandle == InputHandle )
	{
		CharBuf.ActiveInputHandle = -1 ;
	}

	// ���ׂẴL�[���͂������ɂȂ��Ă���΂h�l�d�͖����ɂ���
	for( i = 0 ; i != MAX_INPUT_NUM && !CharBuf.InputData[ i & DX_HANDLEINDEX_MASK ].UseFlag ; i ++ ){}
	if( i == MAX_INPUT_NUM )
	{
		CharBuf.IMEUseFlag_System = FALSE ;
		RefreshIMEFlag() ;
	}

	// �I��
	return 0 ;
}

// �w��̃L�[���͂��A�N�e�B�u�ɂ���
extern int NS_SetActiveKeyInput( int InputHandle )
{
	INPUTDATA * Input ;
	INPUTDATA * ActInput ;

	// �n���h���`�F�b�N
	Input = NULL ;
	if( InputHandle > 0 )
	{
		if( KEYHCHK( InputHandle, Input ) ) return -1 ;
	}

	// �J�[�\���_�ŏ����̃J�E���^�����Z�b�g
	ResetKeyInputCursorBrinkCount() ;

	// ���܂łƓ����ꍇ�͉������Ȃ�
	if( InputHandle == CharBuf.ActiveInputHandle &&
		( ( Input != NULL && Input->EndFlag == FALSE ) || ( Input == NULL && InputHandle >= 0 ) ) ) return 0 ;

	// ���܂ŃA�N�e�B�u�������n���h���őI��̈悪����ꍇ�͉�������
	if( CharBuf.ActiveInputHandle != -1 )
	{
		if( KEYHCHK( CharBuf.ActiveInputHandle, ActInput ) ) return -1 ;

		ActInput->SelectStart = -1 ;
		ActInput->SelectEnd = -1 ;
	}

	if( InputHandle < 0 )
	{
		CharBuf.ActiveInputHandle = -1 ;
	}
	else
	{
		// �A�N�e�B�u�ȓ��̓n���h����ύX����O�ɓ��͕�������N���A���Ă���
		NS_ClearInputCharBuf() ;

		CharBuf.ActiveInputHandle = InputHandle ;

		// �G���h�t���O�̗ނ�����������
		Input->EndFlag = FALSE ;
		Input->CancellFlag = FALSE ;
	}

	// �I��
	return 0 ;
}

// ���݃A�N�e�B�u�ɂȂ��Ă���L�[���̓n���h�����擾����
extern int NS_GetActiveKeyInput( void )
{
	return CharBuf.ActiveInputHandle ;
}

// ���͂��I�����Ă��邩�擾����
extern int NS_CheckKeyInput( int InputHandle )
{
	INPUTDATA * Input ;
	int Result ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	Result = Input->CancellFlag ? 2 : ( Input->EndFlag ? 1 : 0 ) ;

	return Result ;
}

// ���͂����������L�[���͂��ēx�ҏW��Ԃɖ߂�
extern int NS_ReStartKeyInput( int InputHandle )
{
	INPUTDATA * Input ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	Input->EndFlag = FALSE ;

	// �I��
	return 0 ;
}

// �w��̕�����̎w��̈ʒu�̕����̃^�C�v��Ԃ�( 0:wchar_t��Ŋ������Ă��镶��  1:�T���Q�[�g�y�A��wchar_t���  2:�T���Q�[�g�y�A��wchar_t��� )
static int CheckWCharType( const wchar_t *String, int CharPosition )
{
	int i ;

	for( i = 0 ; String[ i ] != L'\0' ; )
	{
		if( CHECK_WCHAR_T_DOUBLE( String[ i ] ) == TRUE )
		{
			if( i     == CharPosition ) return 1 ;
			if( i + 1 == CharPosition ) return 2 ;
			i += 2 ;
		}
		else
		{
			if( i == CharPosition ) return 0 ;
			i ++ ;
		}
	}

	return -1 ;
}

// �L�[���͏����֐�
extern int NS_ProcessActKeyInput( void )
{
	INPUTDATA * Input ;
	wchar_t C[ 3 ] = { 0 , 0 , 0 } ;
	wchar_t TempString[ CHARBUFFER_SIZE + 1 ] ;
#ifndef UNICODE
	char TempString2[ CHARBUFFER_SIZE * 2 + 1 ] ;
#endif // UNICODE
	int CharLen, len, DelNum ;
	HIMC Imc;

	// �h�l�d�̃��t���b�V������
	{
		HWND DefHwnd;
		static wchar_t str[256];

		switch( CharBuf.IMERefreshStep )
		{
		case 1:
//			DefHwnd = ImmGetDefaultIMEWnd( WinData.MainWindow ) ;
			DefHwnd = WinData.MainWindow;
			if( DefHwnd )
			{
				Imc = WinAPIData.ImmFunc.ImmGetContextFunc( DefHwnd ) ;
				if( Imc )
				{
					_MEMSET( CharBuf.InputString, 0, CHARBUFFER_SIZE );
					_MEMSET( CharBuf.InputTempString, 0, CHARBUFFER_SIZE );
					WinAPIData.ImmFunc.ImmGetCompositionStringFunc( Imc , GCS_COMPSTR , CharBuf.InputTempString , CHARBUFFER_SIZE );
					WinAPIData.ImmFunc.ImmNotifyIMEFunc( Imc , NI_COMPOSITIONSTR ,  CPS_CANCEL , 0  );

					WinAPIData.ImmFunc.ImmReleaseContextFunc( DefHwnd , Imc );
				}
			}
			CharBuf.IMERefreshStep ++ ;
//			break;

		case 2:
			SetIMEOpenState( FALSE );
			CharBuf.IMERefreshStep ++ ;
//			break;

		case 3:
			SetIMEOpenState( TRUE );
			CharBuf.IMERefreshStep ++ ;
//			break;

		case 4:
//			DefHwnd = ImmGetDefaultIMEWnd( WinData.MainWindow ) ;
			DefHwnd = WinData.MainWindow;
			if( DefHwnd )
			{
				Imc = WinAPIData.ImmFunc.ImmGetContextFunc( DefHwnd ) ;
				if( Imc )
				{
					WinAPIData.ImmFunc.ImmSetOpenStatusFunc( Imc, TRUE );
					WinAPIData.ImmFunc.ImmSetCompositionStringFunc( Imc, SCS_SETSTR, CharBuf.InputTempString, ( DWORD )_WCSLEN( CharBuf.InputTempString ), 0, 0 );
					WinAPIData.ImmFunc.ImmReleaseContextFunc( DefHwnd , Imc );
				}
			}
			CharBuf.IMERefreshStep = 0 ;
//			break;
		}
	}

	if( CharBuf.ActiveInputHandle == -1 )
	{
		return 0 ;
	}

	if( KEYHCHK( CharBuf.ActiveInputHandle, Input ) )
	{
		return -1 ;
	}

	if( Input->EndFlag )
	{
		return 0 ;
	}

	// �h�l�d�̓��͕�������������
	if( CharBuf.IMEInputStringMaxLengthIMESync || CharBuf.IMEInputMaxLength )
	{
		int i, maxlen ;
		int flag ;
		int InputStrLength ;
		int StrLength ;

		// ���������w��ő�o�C�g���𒴂��Ă����狭���I�ɕ����������炷
		flag = FALSE ;
#ifdef UNICODE
		InputStrLength = ( int )_WCSLEN( CharBuf.InputString ) ;
		StrLength      = Input->StrLength ;
#else // UNICODE
		// �}���`�o�C�g������Ƃ��Ă̒������擾����
		{
			ConvString( ( const char * )Input->Buffer, WCHAR_T_CHARCODEFORMAT, Input->TempBuffer, Input->TempBufferBytes, CHAR_CHARCODEFORMAT ) ;
			StrLength = ( int )CL_strlen( CHAR_CHARCODEFORMAT, Input->TempBuffer ) ;

			ConvString( ( const char * )CharBuf.InputString, WCHAR_T_CHARCODEFORMAT, TempString2, sizeof( TempString2 ), CHAR_CHARCODEFORMAT ) ;
			InputStrLength = ( int )CL_strlen( CHAR_CHARCODEFORMAT, TempString2 ) ;
		}
#endif // UNICODE
		maxlen = 65536 ;
		if( CharBuf.IMEInputStringMaxLengthIMESync && StrLength + InputStrLength > Input->MaxStrLength )
		{
			flag = TRUE ;
			maxlen = Input->MaxStrLength - StrLength ;
		}

		if( CharBuf.IMEInputMaxLength && InputStrLength > CharBuf.IMEInputMaxLength )
		{
			flag = TRUE ;
			if( maxlen > CharBuf.IMEInputMaxLength )
			{
				maxlen = CharBuf.IMEInputMaxLength ;
			}
		}

		if( flag )
		{
			// ���̓R���e�L�X�g�̎擾
			Imc = WinAPIData.ImmFunc.ImmGetContextFunc( WinData.MainWindow ) ;
			if( Imc )
			{
				int len ;

				// ���o�b�t�@�ɂ��镶����������Ȃ��Ă��邩���ׂ�
				_MEMSET( TempString, 0, sizeof( TempString ) );
				WinAPIData.ImmFunc.ImmGetCompositionStringFunc( Imc , GCS_COMPSTR , TempString , CHARBUFFER_SIZE );
#ifdef UNICODE
				len = ( int )_WCSLEN( TempString ) ;
#else // UNICODE
				// �}���`�o�C�g������ɕϊ�
				ConvString( ( const char * )TempString, WCHAR_T_CHARCODEFORMAT, TempString2, sizeof( TempString2 ), CHAR_CHARCODEFORMAT ) ;

				// ������̒������擾
				len = ( int )CL_strlen( CHAR_CHARCODEFORMAT, TempString2 ) ;
#endif // UNICODE
				if( len > maxlen )
				{
					// �������Z������
#ifdef UNICODE
					for( i = 0; i < len ; )
					{
						if( CHECK_WCHAR_T_DOUBLE( TempString[ i ] ) )
						{
							if( i + 2 > maxlen ) break;
							i += 2 ;
						}
						else
						{
							if( i + 1 > maxlen ) break;
							i ++ ;
						}
					}
					TempString[ i ] = L'\0' ;
#else // UNICODE
					for( i = 0; i < len ; )
					{
						if( GetCharBytes( &TempString2[ i ], CHAR_CHARCODEFORMAT ) == 2 )
						{
							if( i + 2 > maxlen ) break;
							i += 2 ;
						}
						else
						{
							if( i + 1 > maxlen ) break;
							i ++ ;
						}
					}
					TempString2[ i ] = '\0' ;

					// wchar_t������ɕϊ�
					ConvString( TempString2, CHAR_CHARCODEFORMAT, ( char * )TempString, sizeof( TempString ), WCHAR_T_CHARCODEFORMAT ) ;
#endif // UNICODE
					// �ݒ��������
					WinAPIData.ImmFunc.ImmNotifyIMEFunc( Imc , NI_COMPOSITIONSTR ,  CPS_CANCEL , 0  );
					WinAPIData.ImmFunc.ImmSetCompositionStringFunc( Imc, SCS_SETSTR, TempString, ( DWORD )_WCSLEN( TempString ), 0, 0 );
				}

				// ���̓R���e�L�X�g�̍폜
				WinAPIData.ImmFunc.ImmReleaseContextFunc( WinData.MainWindow , Imc );
			}
		}
	}

	// �L�[�R�[�h�o�b�t�@�̑������菈��
	while( Input->EndFlag == FALSE )
	{
		CharLen = GetOneChar_WCHAR_T( C , TRUE ) ;
		if( CharLen == 0 )
		{
			break ;
		}

		// �J�[�\���_�ŏ����̃J�E���^�����Z�b�g
		ResetKeyInputCursorBrinkCount() ;
		
		switch( C[0] )
		{
			// �㉺�^�u�L�[�͖���
		case CTRL_CODE_TAB :
			break ;

			// �z�[���{�^��
		case CTRL_CODE_HOME :
			if( NS_CheckHitKey( KEY_INPUT_LCONTROL ) || NS_CheckHitKey( KEY_INPUT_RCONTROL ) )
			{
				if( NS_CheckHitKey( KEY_INPUT_LSHIFT ) || NS_CheckHitKey( KEY_INPUT_RSHIFT ) )
				{
					if( Input->SelectStart == -1 )
					{
						Input->SelectStart = Input->Point ;
						Input->SelectEnd   = 0 ;
					}
					else
					{
						Input->SelectEnd   = 0 ;
					}
				}
				else
				{
					Input->SelectStart = -1 ;
					Input->SelectEnd = -1 ;
				}
				Input->Point = 0 ;
			}
			else
			if( Input->Point > 0 )
			{
				int OldPos ;
				int CharNum ;

				OldPos = Input->Point ;

				// �s�̐擪�Ɉړ�
				do
				{
					CharNum = 1 ;
					if( Input->Point > 1 )
					{
						CharNum = CheckWCharType( Input->Buffer, Input->Point - 1 ) == 2 ? 2 : 1 ;
					}

					Input->Point -= CharNum ;
				}while( Input->Point > 0 && Input->Buffer[ Input->Point ] != L'\n' ) ;
				if( Input->Buffer[ Input->Point ] == L'\n' )
				{
					Input->Point ++ ;
				}

				if( NS_CheckHitKey( KEY_INPUT_LSHIFT ) || NS_CheckHitKey( KEY_INPUT_RSHIFT ) )
				{
					if( Input->SelectStart == -1 )
					{
						Input->SelectStart = OldPos ;
						Input->SelectEnd   = Input->Point ;
					}
					else
					{
						Input->SelectEnd   = Input->Point ;
					}
				}
				else
				{
					Input->SelectStart = -1 ;
					Input->SelectEnd = -1 ;
				}
			}
			break ;

			// �G���h�{�^��
		case CTRL_CODE_END :
			if( NS_CheckHitKey( KEY_INPUT_LCONTROL ) || NS_CheckHitKey( KEY_INPUT_RCONTROL ) )
			{
				if( NS_CheckHitKey( KEY_INPUT_LSHIFT ) || NS_CheckHitKey( KEY_INPUT_RSHIFT ) )
				{
					if( Input->SelectStart == -1 )
					{
						Input->SelectStart = Input->Point ;
						Input->SelectEnd   = Input->StrLength ;
					}
					else
					{
						Input->SelectEnd   = Input->StrLength ;
					}
				}
				else
				{
					Input->SelectStart = -1 ;
					Input->SelectEnd = -1 ;
				}
				Input->Point = Input->StrLength ;
			}
			else
			{
				int OldPos ;

				OldPos = Input->Point ;

				// �s�̏I�[�Ɉړ�
				while( Input->Point < Input->StrLength && Input->Buffer[ Input->Point ] != L'\n' )
				{
					len = 1 ;
					if( CHECK_WCHAR_T_DOUBLE( Input->Buffer[ Input->Point ] ) == TRUE )
					{
						len = 2 ;
					}

					Input->Point += len ;
				}

				if( NS_CheckHitKey( KEY_INPUT_LSHIFT ) || NS_CheckHitKey( KEY_INPUT_RSHIFT ) )
				{
					if( Input->SelectStart == -1 )
					{
						Input->SelectStart = OldPos ;
						Input->SelectEnd   = Input->Point ;
					}
					else
					{
						Input->SelectEnd   = Input->Point ;
					}
				}
				else
				{
					Input->SelectStart = -1 ;
					Input->SelectEnd = -1 ;
				}
			}
			break ;

			// �J�b�g
		case CTRL_CODE_CUT :
CUT:
			if( Input->SelectStart != -1 )
			{
				wchar_t *Buffer ;
				int smin, smax, size ;

				if( Input->SelectStart > Input->SelectEnd )
				{
					smin = Input->SelectEnd ;
					smax = Input->SelectStart ;
				}
				else
				{
					smin = Input->SelectStart ;
					smax = Input->SelectEnd ;
				}
				size = smax - smin ;
				Buffer = ( wchar_t * )DXALLOC( ( size + 1 ) * sizeof( wchar_t ) ) ;
				if( Buffer )
				{
					_MEMCPY( Buffer, &Input->Buffer[ smin ], size * sizeof( wchar_t ) ) ;
					Buffer[ size ] = L'\0' ;
					SetClipboardText_WCHAR_T( Buffer ) ;
					DXFREE( Buffer ) ;
				}
				KeyInputSelectAreaDelete( Input ) ;
			}
			break ;

			// �R�s�[
		case CTRL_CODE_COPY :
			if( Input->SelectStart != -1 )
			{
				wchar_t *Buffer ;
				int smin, smax, size ;

				if( Input->SelectStart > Input->SelectEnd )
				{
					smin = Input->SelectEnd ;
					smax = Input->SelectStart ;
				}
				else
				{
					smin = Input->SelectStart ;
					smax = Input->SelectEnd ;
				}
				size = smax - smin ;
				Buffer = ( wchar_t * )DXALLOC( ( size + 1 ) * sizeof( wchar_t ) ) ;
				if( Buffer )
				{
					_MEMCPY( Buffer, &Input->Buffer[ smin ], size * sizeof( wchar_t ) ) ;
					Buffer[ size ] = L'\0' ;
					SetClipboardText_WCHAR_T( Buffer ) ;
					DXFREE( Buffer ) ;
				}
			}
			break ;

			// �y�[�X�g
		case CTRL_CODE_PASTE :
			if( GetClipboardText_WCHAR_T( NULL ) != -1 )
			{
				int Size ;
				wchar_t *Buffer, *p ;

				Size = GetClipboardText_WCHAR_T( NULL ) ;
				Buffer = ( wchar_t * )DXALLOC( ( size_t )Size ) ;
				if( Buffer )
				{
					GetClipboardText_WCHAR_T( Buffer ) ;
					for( p = Buffer ; *p ; p++ )
					{
						if( CHECK_WCHAR_T_DOUBLE( *p ) == TRUE )
						{
							StockInputChar_WCHAR_T( *p ) ;
							p++ ;
							StockInputChar_WCHAR_T( *p ) ;
						}
						else
						{
							if( Input->EnableNewLineFlag || ( *p != L'\r' && *p != L'\n' ) )
							{
								StockInputChar_WCHAR_T( *p ) ;
							}
						}
					}
					DXFREE( Buffer ) ;
				}
			}
			break ;

			// �G���^�[�����������ꍇ
		case CTRL_CODE_CR :
			// ���s��������Ă���ꍇ�͉��s������ǉ�����
			if( Input->EnableNewLineFlag )
			{
				C[ 0 ] = '\n' ;
				goto DEFAULTCHARADD ;
			}
			else
			{
				// �����łȂ��ꍇ�͉��s�ŏI��
				Input->EndFlag = TRUE ;
				CharBuf.ActiveInputHandle = -1 ;
			}
			break ;

			// �G�X�P�[�v�L�[�������ꍇ�̓L�����Z��
		case CTRL_CODE_ESC :
			if( Input->CancelValidFlag )
			{
				Input->CancellFlag = TRUE ;
				Input->EndFlag = TRUE ;
				CharBuf.ActiveInputHandle = -1 ;
			}
			break ;

			// �f���[�g�L�[�������ꍇ�͕����̍폜
		case CTRL_CODE_DEL :
			if( Input->SelectStart == -1 )
			{
				if( Input->Point != Input->StrLength )
				{
					DelNum = CHECK_WCHAR_T_DOUBLE( Input->Buffer[Input->Point] ) == TRUE ? 2 : 1 ;  
					_MEMMOVE( &Input->Buffer[ Input->Point ] , &Input->Buffer[ Input->Point + DelNum ] , ( Input->StrLength - Input->Point ) * sizeof( wchar_t ) ) ;
					Input->StrLength -= DelNum ;
				}
			}
			else
			{
				if( NS_CheckHitKey( KEY_INPUT_LSHIFT ) || NS_CheckHitKey( KEY_INPUT_RSHIFT ) )
				{
					goto CUT ;
				}
				KeyInputSelectAreaDelete( Input ) ;
			}
			break ;

			// �o�b�N�X�y�[�X�L�[�������ꍇ�͈�O�̕������폜
		case CTRL_CODE_BS :
			if( Input->SelectStart == -1 )
			{
				if( Input->Point > 0 )
				{
					int DelNum ;
					int DelNewLine ;

					DelNum = 1 ;
					if( Input->Point > 1 )
					{
						DelNum = CheckWCharType( Input->Buffer, Input->Point - 1 ) == 2 ? 2 : 1 ;
					}

					DelNewLine = Input->Buffer[ Input->Point - DelNum ] == L'\n' ? TRUE : FALSE ;
		
					Input->Point -= DelNum ;
					_MEMMOVE( &Input->Buffer[ Input->Point ] , &Input->Buffer[ Input->Point + DelNum ] , ( Input->StrLength - Input->Point ) * sizeof( wchar_t ) ) ;
					Input->StrLength -= DelNum ;

					// �������s�������폜�����ꍇ�͕`��J�n�ʒu��ύX���邩�`�F�b�N����
					if( DelNewLine )
					{
						if( Input->Point <= Input->DrawStartPos )
						{
							int CharNum ;

							// �s�̐擪�ɕ`��J�n�ʒu���ړ�����
							while( Input->DrawStartPos > 0 )
							{
								CharNum = 1 ;
								if( Input->DrawStartPos > 1 )
								{
									CharNum = CheckWCharType( Input->Buffer, Input->DrawStartPos - 1 ) == 2 ? 2 : 1 ;
								}

								if( CharNum == 1 )
								{
									if( Input->Buffer[ Input->DrawStartPos - CharNum ] == L'\n' )
									{
										break ;
									}
								}

								Input->DrawStartPos -= CharNum ;
							}
						}
					}
				}
			}
			else
			{
				KeyInputSelectAreaDelete( Input ) ;
			}
			break ;

			// ���E�L�[�̏ꍇ�͈ꕶ���ړ�
		case CTRL_CODE_LEFT :
			if( Input->Point > 0 )
			{
				int MoveNewLine ;

				len = 1 ;
				if( Input->Point > 1 && CheckWCharType( Input->Buffer, Input->Point - 1 ) == 2 )
				{
					len = 2 ;
				}

				MoveNewLine = Input->Buffer[ Input->Point - len ] == L'\n' ? TRUE : FALSE ;

				if( NS_CheckHitKey( KEY_INPUT_LSHIFT ) || NS_CheckHitKey( KEY_INPUT_RSHIFT ) )
				{
					if( Input->SelectStart == -1 )
					{
						Input->SelectStart = Input->Point ;
						Input->SelectEnd   = Input->Point - len ;
					}
					else
					{
						Input->SelectEnd   = Input->Point - len ;
					}
				}
				else
				{
					Input->SelectStart = -1 ;
					Input->SelectEnd = -1 ;
				}
				Input->Point -= len ;

				// �������s�������ړ������ꍇ�͕`��J�n�ʒu��ύX���邩�`�F�b�N����
				if( MoveNewLine )
				{
					if( Input->Point <= Input->DrawStartPos )
					{
						int CharNum ;

						// �s�̐擪�ɕ`��J�n�ʒu���ړ�����
						Input->DrawStartPos = Input->Point ;
						while( Input->DrawStartPos > 0 )
						{
							CharNum = 1 ;
							if( Input->DrawStartPos > 1 )
							{
								CharNum = CheckWCharType( Input->Buffer, Input->DrawStartPos - 1 ) == 2 ? 2 : 1 ;
							}

							if( CharNum == 1 )
							{
								if( Input->Buffer[ Input->DrawStartPos - CharNum ] == L'\n' )
								{
									break ;
								}
							}

							Input->DrawStartPos -= CharNum ;
						}
					}
				}
			}
			break ;

		case CTRL_CODE_RIGHT :
			if( Input->Point < Input->StrLength )
			{
				len = 1 ;
				if( CHECK_WCHAR_T_DOUBLE( Input->Buffer[Input->Point] ) == TRUE )
				{
					len = 2 ;
				}

				if( NS_CheckHitKey( KEY_INPUT_LSHIFT ) || NS_CheckHitKey( KEY_INPUT_RSHIFT ) )
				{
					if( Input->SelectStart == -1 )
					{
						Input->SelectStart = Input->Point ;
						Input->SelectEnd   = Input->Point + len ;
					}
					else
					{
						Input->SelectEnd   = Input->Point + len ;
					}
				}
				else
				{
					Input->SelectStart = -1 ;
					Input->SelectEnd = -1 ;
				}
				Input->Point += len ;
			}
			break ;

			// �㉺�L�[�͍s���ړ�
		case CTRL_CODE_UP :
			if( Input->Point > 0 )
			{
				int CharPos ;
				int CharNum ;
				int MoveLength ;
				int i ;
				int OldPos ;

				OldPos = Input->Point ;

				// �s�̐擪�Ɉړ�
				CharPos = 0 ;
				MoveLength = 0 ;
				do
				{
					CharNum = 1 ;
					if( Input->Point - MoveLength > 1 )
					{
						CharNum = CheckWCharType( Input->Buffer, Input->Point - MoveLength - 1 ) == 2 ? 2 : 1 ;
					}

					MoveLength += CharNum ;
					CharPos ++ ;
				}while( Input->Point - MoveLength > 0 && Input->Buffer[ Input->Point - MoveLength ] != L'\n' ) ;

				// ��ԏ�̍s�������ꍇ�͉������Ȃ�
				if( Input->Point - MoveLength == 0 )
				{
					// �I�����Ă���������������s��
					if( NS_CheckHitKey( KEY_INPUT_LSHIFT ) == 0 && NS_CheckHitKey( KEY_INPUT_RSHIFT ) == 0 )
					{
						Input->SelectStart = -1 ;
						Input->SelectEnd = -1 ;
					}

					break ;
				}

				// �ЂƂ�̍s�̐擪�Ɉړ�
				Input->Point -= MoveLength ;
				do
				{
					CharNum = 1 ;
					if( Input->Point > 1 )
					{
						CharNum = CheckWCharType( Input->Buffer, Input->Point - 1 ) == 2 ? 2 : 1 ;
					}

					Input->Point -= CharNum ;
				}while( Input->Point > 0 && Input->Buffer[ Input->Point ] != L'\n' ) ;

				// �ړ��O�̍s�Ɠ����������̉ӏ��Ɉړ�
				if( Input->Point != 0 )
				{
					Input->Point ++ ;
				}
				if( Input->DrawStartPos > Input->Point )
				{
					Input->DrawStartPos = Input->Point ;
				}
				CharPos -- ;
				for( i = 0 ; i < CharPos && Input->Buffer[ Input->Point ] != L'\n' ; i ++ )
				{
					len = 1 ;
					if( CHECK_WCHAR_T_DOUBLE( Input->Buffer[ Input->Point ] ) == TRUE )
					{
						len = 2 ;
					}

					Input->Point += len ;
				}

				if( NS_CheckHitKey( KEY_INPUT_LSHIFT ) || NS_CheckHitKey( KEY_INPUT_RSHIFT ) )
				{
					if( Input->SelectStart == -1 )
					{
						Input->SelectStart = OldPos ;
						Input->SelectEnd   = Input->Point ;
					}
					else
					{
						Input->SelectEnd   = Input->Point ;
					}
				}
				else
				{
					Input->SelectStart = -1 ;
					Input->SelectEnd = -1 ;
				}
			}
			break ;

		case CTRL_CODE_DOWN :
			if( Input->Point < Input->StrLength )
			{
				int CharPos ;
				int CharNum ;
				int MoveLength ;
				int i ;
				int OldPos ;

				OldPos = Input->Point ;

				// �s�̐擪�܂ł̕������𐔂���
				CharPos = 0 ;
				MoveLength = 0 ;
				do
				{
					CharNum = 1 ;
					if( Input->Point - MoveLength > 1 )
					{
						CharNum = CheckWCharType( Input->Buffer, Input->Point - MoveLength - 1 ) == 2 ? 2 : 1 ;
					}

					MoveLength += CharNum ;
					CharPos ++ ;
				}while( Input->Point - MoveLength > 0 && Input->Buffer[ Input->Point - MoveLength ] != L'\n' ) ;

				if( Input->Point - MoveLength == 0 )
				{
					CharPos ++ ;
				}

				// �ЂƂ��̍s�Ɉړ�
				MoveLength = 0 ;
				while( Input->Point + MoveLength < Input->StrLength && Input->Buffer[ Input->Point + MoveLength ] != L'\n' )
				{
					len = 1 ;
					if( CHECK_WCHAR_T_DOUBLE( Input->Buffer[ Input->Point + MoveLength ] ) == TRUE )
					{
						len = 2 ;
					}

					MoveLength += len ;
				}

				// �ЂƂ��̍s���Ȃ��ꍇ�͉��������ɏI��
				if( Input->Point + MoveLength == Input->StrLength )
				{
					// �I�����Ă���������������s��
					if( NS_CheckHitKey( KEY_INPUT_LSHIFT ) == 0 && NS_CheckHitKey( KEY_INPUT_RSHIFT ) == 0 )
					{
						Input->SelectStart = -1 ;
						Input->SelectEnd = -1 ;
					}

					break ;
				}

				// ���݂̍s�̖��[�Ɉړ�
				Input->Point += MoveLength ;

				// �ЂƂ��̍s�́A�ړ��O�̍s�Ɠ����������̉ӏ��Ɉړ�
				Input->Point ++ ;
				CharPos -- ;
				for( i = 0 ; i < CharPos && Input->Point < Input->StrLength && Input->Buffer[ Input->Point ] != L'\n' ; i ++ )
				{
					len = 1 ;
					if( CHECK_WCHAR_T_DOUBLE( Input->Buffer[ Input->Point ] ) == TRUE )
					{
						len = 2 ;
					}

					Input->Point += len ;
				}

				if( NS_CheckHitKey( KEY_INPUT_LSHIFT ) || NS_CheckHitKey( KEY_INPUT_RSHIFT ) )
				{
					if( Input->SelectStart == -1 )
					{
						Input->SelectStart = OldPos ;
						Input->SelectEnd   = Input->Point ;
					}
					else
					{
						Input->SelectEnd   = Input->Point ;
					}
				}
				else
				{
					Input->SelectStart = -1 ;
					Input->SelectEnd = -1 ;
				}
			}
			break ;

		default:
DEFAULTCHARADD:
			KeyInputSelectAreaDelete( Input ) ;

			// ���l�����̂ݎ��̏���
			if( Input->NumCharOnlyFlag )
			{
				// ���l�����łȂ���΃L�����Z��
				if( C[ 0 ] < L'0' || C[ 0 ] > L'9' )
				{
					// �}�C�i�X�����ŃJ�[�\���ʒu���O�������ꍇ�͓��͉\�@ 
					if( C[ 0 ] != L'-' || Input->Point != 0 )
					{
						// �s���I�h����܂łn�j
						if( ( C[ 0 ] == L'.' && _WCSCHR( Input->Buffer, L'.' ) != NULL ) || C[ 0 ] != L'.' ) break ;
					}
				}
			}

			// ������̒ǉ�
			{
				int CharLen2 ;
				int CharLen3 ;
				int StrLength2 ;
#ifdef UNICODE
				char TempCharBuf[ 32 ] ;

				CharLen2   = CharLen ;
				StrLength2 = Input->StrLength ;

				ConvString( ( const char * )C, WCHAR_T_CHARCODEFORMAT, TempCharBuf, sizeof( TempCharBuf ), CHAR_CHARCODEFORMAT ) ;
				CharLen3   = ( int )CL_strlen( CHAR_CHARCODEFORMAT, TempCharBuf ) ;

#else // UNICODE

				ConvString( ( const char * )C, WCHAR_T_CHARCODEFORMAT, TempString2, sizeof( TempString2 ), CHAR_CHARCODEFORMAT ) ;
				CharLen2   = ( int )CL_strlen( CHAR_CHARCODEFORMAT, TempString2 ) ;
				CharLen3   = CharLen2 ;

				ConvString( ( const char * )Input->Buffer, WCHAR_T_CHARCODEFORMAT, Input->TempBuffer, Input->TempBufferBytes, CHAR_CHARCODEFORMAT ) ;
				StrLength2 = ( int )CL_strlen( CHAR_CHARCODEFORMAT, Input->TempBuffer ) ;
#endif // UNICODE
				// �P�o�C�g�����̂ݎ��̏���
				if( Input->SingleCharOnlyFlag )
				{
					if( CharLen3 != 1 ) break ;
				}

				// �Q�o�C�g�����̂ݎ��̏���
				if( Input->DoubleCharOnlyFlag )
				{
					if( CharLen3 == 1 ) break ;
				}

				// �J�[�\���̈ʒu��������̖��[�ŁA�����[�����������[�h��
				// DX_KEYINPSTR_ENDCHARAMODE_NOTCHANGE �������牽�������I��
				if( CharBuf.EndCharaMode != DX_KEYINPSTR_ENDCHARAMODE_NOTCHANGE ||
					StrLength2   + CharLen2 <= Input->MaxStrLength ||
					Input->Point + CharLen  <= Input->StrLength )
				{
					// �o�b�t�@�ɋ󂫂��Ȃ����̏���
					while( StrLength2 + CharLen2 > Input->MaxStrLength )
					{
						int /*DelNum,*/ Pos, CLen ;

						// �Ō�̕��������
						Pos = 0 ;
						while( Pos < Input->StrLength )
						{
							if( CHECK_WCHAR_T_DOUBLE( Input->Buffer[Pos] ) == TRUE )
							{
								CLen = 2 ;
							}
							else
							{
								CLen = 1 ;
							}
							Pos += CLen ;
						}
#ifdef UNICODE
						StrLength2 -= CLen ;
#else // UNICODE
						{
							int i ;
							int CLen2 ;

							for( i = 0 ; i < CLen ; i ++ )
							{
								TempString[ i ] = Input->Buffer[ Pos - CLen + i ] ;
							}
							TempString[ CLen ] = L'\0' ;
							ConvString( ( const char * )TempString, WCHAR_T_CHARCODEFORMAT, TempString2, sizeof( TempString2 ), CHAR_CHARCODEFORMAT ) ;
							CLen2 = ( int )CL_strlen( CHAR_CHARCODEFORMAT, TempString2 ) ;

							StrLength2 -= CLen2 ;
						}
#endif // UNICODE
						Input->Buffer[ Pos - CLen ] = L'\0' ;
						Input->StrLength -= CLen ;

						// �J�[�\�����ړ�����
						if( Input->Point == Pos )
						{
							Input->Point -= CLen ;
						}
					}
					
					_MEMMOVE( &Input->Buffer[ Input->Point + CharLen ] , &Input->Buffer[ Input->Point ] , ( Input->StrLength - Input->Point + 1 ) * sizeof( wchar_t ) ) ;
					_MEMMOVE( &Input->Buffer[ Input->Point ] , C , CharLen * sizeof( wchar_t ) ) ;

					Input->Point     += CharLen ;
					Input->StrLength += CharLen ;
				}
				break ;
			}
		}
	}

	// �I��
	return 0 ;
}

// �L�[���̓n���h���̓��͒��������`�悷��ۂ̕`��͈͂�ݒ肷��
extern int NS_SetKeyInputDrawArea( int x1, int y1, int x2, int y2, int InputHandle )
{
	INPUTDATA * Input ;

	if( KEYHCHK( InputHandle, Input ) )
	{
		return -1 ;
	}

	Input->DrawAreaValidFlag = TRUE ;
	Input->DrawArea.left = x1 ;
	Input->DrawArea.top = y1 ;
	Input->DrawArea.right = x2 ;
	Input->DrawArea.bottom = y2 ;

	// �I��
	return 0 ;
}

// �L�[���͒��f�[�^�̕`��
extern int NS_DrawKeyInputString( int x , int y , int InputHandle )
{
	INPUTDATA * Input ;
	int Use3DFlag , FontSize , FontHandle ;
	int PointX ;
	int PointY ;
	int AddY ;
//	int StrWidth ;
	RECT DrawRect ;
	RECT DrawRectBackup ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;
	if( Input->EndFlag ) return -1 ;

	// �e��f�[�^�ۑ�
	Use3DFlag = NS_GetUse3DFlag() ;

	// �R�c�`��@�\�̎g�p�ύX
	NS_SetUse3DFlag( FALSE ) ;

	// �g�p����t�H���g�̃n���h�����Z�b�g����
	FontHandle = CharBuf.UseFontHandle == -1 ? NS_GetDefaultFontHandle() : CharBuf.UseFontHandle ;

	// �t�H���g�T�C�Y�̎擾
	FontSize = NS_GetFontSizeToHandle( FontHandle ) ;
	
	// �J�[�\���_�ŏ���
	if( CharBuf.CBrinkFlag == TRUE )
	{
		if( NS_GetNowCount() - CharBuf.CBrinkCount > CharBuf.CBrinkWait )
		{
			CharBuf.CBrinkDrawFlag = CharBuf.CBrinkDrawFlag == TRUE ? FALSE : TRUE ;
			CharBuf.CBrinkCount = NS_GetNowCount() ;
		}
	}
	else
	{
		CharBuf.CBrinkDrawFlag = TRUE ;
	}

	// �`��͈͂̐ݒ�
	NS_GetDrawArea( &DrawRectBackup ) ;
	if( Input->DrawAreaValidFlag )
	{
		DrawRect = Input->DrawArea ;
		NS_SetDrawArea( Input->DrawArea.left, Input->DrawArea.top, Input->DrawArea.right, Input->DrawArea.bottom ) ;
	}
	else
	{
		DrawRect = DrawRectBackup ;
	}

	// �`��J�n�����ʒu�̌���
	{
		int DrawWidth, DrawHNum ;
		int tx, ty ;
//		int w ;
//		int SizeX, SizeY, LineCount ;
		int PosX, PosY ;

		AddY = FontSize + FontSize / 10 * 3 ;

		if( DrawRect.left == DrawRect.right ||
			DrawRect.bottom == DrawRect.top )
			return -1 ;

		tx = x ;
		ty = y ;
		if( tx < DrawRect.left ) tx = DrawRect.left ;
		if( ty < DrawRect.top  ) ty = DrawRect.top ;

		while( tx > DrawRect.right )
		{
			tx -= DrawRect.right - DrawRect.left ;
			ty += AddY ;
		}

		DrawHNum = ( DrawRect.bottom - ty ) / AddY ;
		if( DrawHNum == 0 )
		{
			DrawWidth = DrawRect.right - tx ;
			DrawHNum = 1 ;
		}
		else
		{
			DrawWidth = ( DrawRect.right - tx ) + ( DrawHNum - 1 ) * ( DrawRect.right - DrawRect.left ) ;
		}

		DrawObtainsString_WCHAR_T(
			x, y,
			AddY,
			Input->Buffer,
			-1,
			0, 0, FontHandle, 0, 0, 0, 0, 0, FALSE, &PosX, &PosY ) ;
		if( PosY < DrawRect.bottom - AddY )
		{
			Input->DrawStartPos = 0 ;
		}
		else
		{
			if( Input->DrawStartPos > Input->Point )
			{
				Input->DrawStartPos = Input->Point ;
			}
			else
			{
				for(;;)
				{
					DrawObtainsString_WCHAR_T(
						x, y,
						AddY,
						Input->Buffer + Input->DrawStartPos,
						Input->Point  - Input->DrawStartPos,
						0, 0, FontHandle, 0, 0, 0, 0, 0, FALSE, &PosX, &PosY ) ;

					if( Input->DrawStartPos >= Input->Point )
					{
						break ;
					}

					if( DrawHNum == 1 )
					{
						if( PosY == y )
						{
							break ;
						}
					}
					else
					{
						if( PosY <= DrawRect.bottom - AddY )
						{
							break ;
						}
					}

					if( CHECK_WCHAR_T_DOUBLE( Input->Buffer[ Input->DrawStartPos ] ) )
					{
						Input->DrawStartPos += 2 ;
					}
					else
					{
						Input->DrawStartPos ++ ;
					}
				}
			}
		}
	}

	// �J�[�\���̕`��
	{
		DrawObtainsString_WCHAR_T(
			x,
			y,
			AddY,
			Input->Buffer + Input->DrawStartPos,
			Input->Point - Input->DrawStartPos,
			0,
			0,
			FontHandle,
			0,
			0,
			0,
			0,
			0,
			FALSE,
			&PointX,
			&PointY
		) ;

		if( CharBuf.CBrinkDrawFlag == TRUE && InputHandle == CharBuf.ActiveInputHandle )
	//		|| ( InputHandle == CharBuf.ActiveInputHandle && CharBuf.IMESwitch == TRUE ) )
		{
			NS_DrawObtainsBox( PointX , PointY , PointX + 2 , PointY + FontSize , FontSize + FontSize / 10 * 3 , CharBuf.NormalCursorColor , TRUE ) ;
		}
	}

	// ������̕`��
	DrawObtainsString_WCHAR_T(
		x,
		y,
		AddY,
		Input->Buffer + Input->DrawStartPos,
		-1,
		CharBuf.NormalStrColor,
		CharBuf.NormalStrEdgeColor,
		FontHandle,
		CharBuf.SelectStrBackColor,
		CharBuf.SelectStrColor,
		CharBuf.SelectStrEdgeColor,
		Input->SelectStart - Input->DrawStartPos,
		Input->SelectEnd   - Input->DrawStartPos
	) ;

	// �`��͈͂�ύX���Ă����ꍇ�͌��ɖ߂�
	if( Input->DrawAreaValidFlag )
	{
		NS_SetDrawArea( DrawRectBackup.left, DrawRectBackup.top, DrawRectBackup.right, DrawRectBackup.bottom ) ;
	}

	// ���͒�������̕`��
	if( InputHandle == CharBuf.ActiveInputHandle )
	{
		DrawIMEInputString( PointX , PointY , 5 ) ; 
	}

	// �f�[�^�����ɖ߂�
	NS_SetUse3DFlag( Use3DFlag ) ;

	// �I��
	return 0 ;
}

// �L�[���͎��̃J�[�\���̓_�ł��鑁�����Z�b�g����
extern int NS_SetKeyInputCursorBrinkTime( int Time )
{
	CharBuf.CBrinkWait = Time ;

	// �J�[�\���_�ŏ����̃J�E���^�����Z�b�g
	ResetKeyInputCursorBrinkCount() ;

	// �I��
	return 0 ;
}

// �L�[���̓f�[�^�̎w��̗̈��I����Ԃɂ���( SelectStart �� SelectEnd �� -1 ���w�肷��ƑI����Ԃ���������܂� )
extern int NS_SetKeyInputSelectArea( int SelectStart, int SelectEnd, int InputHandle )
{
	INPUTDATA * Input ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	if( SelectStart == -1 && SelectEnd == -1 )
	{
		Input->SelectStart = -1 ;
		Input->SelectEnd   = -1 ;
	}
	else
	{
		if( SelectStart < 0                ) SelectStart = 0 ;
		if( SelectStart > Input->StrLength ) SelectStart = Input->StrLength ;
		if( SelectEnd   < 0                ) SelectEnd   = 0 ;
		if( SelectEnd   > Input->StrLength ) SelectEnd   = Input->StrLength ;

		Input->SelectStart = SelectStart ;
		Input->SelectEnd   = SelectEnd ;
	}

	// �I��
	return 0 ;
}

// �L�[���̓f�[�^�̑I��̈���擾����
extern int NS_GetKeyInputSelectArea( int *SelectStart, int *SelectEnd, int InputHandle )
{
	INPUTDATA * Input ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	if( SelectStart ) *SelectStart = Input->SelectStart ;
	if( SelectEnd   ) *SelectEnd   = Input->SelectEnd ;

	// �I��
	return 0 ;
}

// �L�[���͂̕`��J�n�����ʒu���擾����
extern int NS_GetKeyInputDrawStartPos( int InputHandle )
{
	INPUTDATA * Input ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	return Input->DrawStartPos ;
}

// �L�[���͂̕`��J�n�����ʒu��ݒ肷��
extern int NS_SetKeyInputDrawStartPos( int DrawStartPos, int InputHandle )
{
	INPUTDATA * Input ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	if( DrawStartPos < 0                ) DrawStartPos = 0 ;
	if( DrawStartPos > Input->StrLength ) DrawStartPos = Input->StrLength ;

	Input->DrawStartPos = DrawStartPos ;

	return Input->DrawStartPos ;
}

// �L�[���͎��̃J�[�\����_�ł����邩�ǂ������Z�b�g����
extern int NS_SetKeyInputCursorBrinkFlag( int Flag )
{
	CharBuf.CBrinkFlag = Flag ;

	// �J�[�\���_�ŏ����̃J�E���^�����Z�b�g
	ResetKeyInputCursorBrinkCount() ;
	
	// �I��
	return 0 ;
}

// �L�[���͎��̃J�[�\���_�ŏ����̃J�E���^�����Z�b�g
static int ResetKeyInputCursorBrinkCount( void )
{
	CharBuf.CBrinkCount = NS_GetNowCount() ;
	CharBuf.CBrinkDrawFlag = TRUE ;
	
	// �I��
	return 0 ;
}
	


// �L�[���̓f�[�^�Ɏw��̕�������Z�b�g����
extern int NS_SetKeyInputString( const TCHAR *String , int InputHandle )
{
#ifdef UNICODE
	return SetKeyInputString_WCHAR_T( String, InputHandle ) ;
#else
	int Result ;

	TCHAR_TO_WCHAR_T_STRING_ONE_BEGIN( String, return -1 )

	Result = SetKeyInputString_WCHAR_T( UseStringBuffer, InputHandle ) ;

	TCHAR_TO_WCHAR_T_STRING_END( String )

	return Result ;
#endif
}

// �L�[���̓f�[�^�Ɏw��̕�������Z�b�g����
extern int SetKeyInputString_WCHAR_T( const wchar_t *String , int InputHandle )
{
	INPUTDATA * Input ;
	int StrLen ;

	if( KEYHCHK( InputHandle, Input ) )
	{
		return -1 ;
	}

	StrLen = ( int )_WCSLEN( String ) ;
	if( StrLen > Input->MaxStrLength )
	{
		StrLen = Input->MaxStrLength ;
	}

	_WCSNCPY_S( Input->Buffer, Input->MaxStrLength * sizeof( wchar_t ), String, StrLen ) ;
	Input->Buffer[ StrLen ]	= L'\0' ;
	Input->StrLength		= StrLen ;
	Input->Point			= StrLen ;
	Input->SelectEnd		= -1 ;
	Input->SelectStart		= -1 ;
	if( Input->StrLength < Input->DrawStartPos )
	{
		Input->DrawStartPos = Input->StrLength ;
	}

	// �J�[�\���_�ŏ����̃J�E���^�����Z�b�g
	ResetKeyInputCursorBrinkCount() ;
	
	// �I��
	return 0 ;
}

// �L�[���̓f�[�^�Ɏw��̐��l�𕶎��ɒu�������ăZ�b�g����
extern int NS_SetKeyInputNumber( int Number , int InputHandle )
{
	INPUTDATA * Input ;
	wchar_t StrBuf[ 512 ] ;
	int StrLen ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	// �����ɒu��������
	_ITOAW_S( Number, StrBuf, sizeof( StrBuf ), 10 ) ;

	StrLen = ( int )_WCSLEN( StrBuf ) ;
	if( StrLen > Input->MaxStrLength )
	{
		StrLen = Input->MaxStrLength ;
	}

	_WCSNCPY_S( Input->Buffer, Input->MaxStrLength * sizeof( wchar_t ), StrBuf ,StrLen ) ;
	Input->Buffer[ StrLen ] = L'\0' ;
	Input->StrLength		= StrLen ;
	Input->Point			= StrLen ;
	Input->SelectEnd		= -1 ;
	Input->SelectStart		= -1 ;
	if( Input->StrLength < Input->DrawStartPos )
	{
		Input->DrawStartPos = Input->StrLength ;
	}

	// �J�[�\���_�ŏ����̃J�E���^�����Z�b�g
	ResetKeyInputCursorBrinkCount() ;

	// �I��
	return 0 ;
}

// �L�[���̓f�[�^�Ɏw��̕��������_�l�𕶎��ɒu�������ăZ�b�g����
extern int NS_SetKeyInputNumberToFloat( float Number, int InputHandle )
{
	INPUTDATA * Input ;
	wchar_t StrBuf[ 256 ] ;
	int StrLen ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	// �����ɒu��������
	_SWNPRINTF( StrBuf, sizeof( StrBuf ) / 2, L"%f", Number ) ;

	StrLen = ( int )_WCSLEN( StrBuf ) ;
	if( StrLen > Input->MaxStrLength )
	{
		StrLen = Input->MaxStrLength ;
	}

	_WCSNCPY_S( Input->Buffer, Input->MaxStrLength * sizeof( wchar_t ), StrBuf, StrLen ) ;
	Input->Buffer[ StrLen ] = L'\0' ;
	Input->StrLength		= StrLen ;
	Input->Point			= StrLen ;
	Input->SelectEnd		= -1 ;
	Input->SelectStart		= -1 ;
	if( Input->StrLength < Input->DrawStartPos )
	{
		Input->DrawStartPos = Input->StrLength ;
	}

	// �J�[�\���_�ŏ����̃J�E���^�����Z�b�g
	ResetKeyInputCursorBrinkCount() ;

	// �I��
	return 0 ;
}

// ���͒��̕�����f�[�^���擾����
extern int NS_GetKeyInputString( TCHAR *StrBuffer , int InputHandle )
{
#ifdef UNICODE
	return GetKeyInputString_WCHAR_T( StrBuffer, InputHandle ) ;
#else
	INPUTDATA * Input ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	ConvString( ( const char * )Input->Buffer, WCHAR_T_CHARCODEFORMAT, StrBuffer, BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;

	// �I��
	return 0 ;
#endif
}

// ���͒��̕�����f�[�^���擾����
extern int GetKeyInputString_WCHAR_T( wchar_t *StrBuffer , int InputHandle )
{
	INPUTDATA * Input ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	_WCSCPY( StrBuffer , Input->Buffer ) ;

	// �I��
	return 0 ;
}

// ���̓f�[�^�̕�����𐮐��l�Ƃ��Ď擾����
extern int NS_GetKeyInputNumber( int InputHandle )
{
	INPUTDATA * Input ;
	int Number ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	// ���l�ɕϊ�
	if( _WCSCHR( Input->Buffer, L'.' ) )
	{
		Number = _FTOL( ( float )_ATOFW( Input->Buffer ) ) ;
	}
	else
	{
		Number = _ATOIW( Input->Buffer ) ;
	}

	// �I��
	return Number ;
}

// ���̓f�[�^�̕�����𕂓������_�l�Ƃ��Ď擾����
extern float NS_GetKeyInputNumberToFloat( int InputHandle )
{
	INPUTDATA * Input ;
	float Number ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	// ���l�ɕϊ�
	if( _WCSCHR( Input->Buffer, L'.' ) )
	{
		Number = ( float )_ATOFW( Input->Buffer ) ;
	}
	else
	{
		Number = ( float )_ATOIW( Input->Buffer ) ;
	}

	// �I��
	return Number ;
}

// �L�[���͂̌��݂̃J�[�\���ʒu��ݒ肷��
extern int NS_SetKeyInputCursorPosition( int Position, int InputHandle )
{
	INPUTDATA * Input ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	if( Position < 0                ) Position = 0 ;
	if( Position > Input->StrLength ) Position = Input->StrLength ;

	Input->Point = Position ;

	// �I��
	return 0 ;
}

// �L�[���͂̌��݂̃J�[�\���ʒu���擾����
extern int NS_GetKeyInputCursorPosition( int InputHandle )
{
	INPUTDATA * Input ;

	if( KEYHCHK( InputHandle, Input ) ) return -1 ;

	// �I��
	return Input->Point ;
}

// �h�l�d�œ��͒��̕�����̏����擾����
extern const IMEINPUTDATA *NS_GetIMEInputData( void )
{
	LPIMEINPUTDATA IMEInput ;
	int TotalSize, i, Len ;
	LPIMEINPUTCLAUSEDATA IMEClause ;

	// ���Ɋm�ۂ���Ă�������
	if( CharBuf.IMEInputData )
	{
		DXFREE( CharBuf.IMEInputData ) ;
		CharBuf.IMEInputData = NULL ;
	}

	if( CharBuf.IMEUseFlag_OSSet == FALSE || CharBuf.IMESwitch == FALSE /* || CharBuf.ClauseNum == 0 */ )
	{
		return NULL ;
	}

	if( CharBuf.InputString[ 0 ] == 0 )
	{
		return NULL ;
	}

	// �m�ۂ��ׂ��������e�ʂ��v�Z
	Len = ( int )_WCSLEN( CharBuf.InputString ) ;
	TotalSize = sizeof( IMEINPUTDATA ) ;
	TotalSize += ( Len + 1 ) * sizeof( TCHAR ) * 8 ;
	if( CharBuf.ClauseNum > 1 )
	{
		TotalSize += ( CharBuf.ClauseNum - 1 ) * sizeof( IMEINPUTCLAUSEDATA ) ;
	}
	if( CharBuf.CandidateList != NULL )
	{
		TotalSize += CharBuf.CandidateList->dwCount * sizeof( wchar_t ** ) + CharBuf.CandidateListSize * 8 ;
	}

	// �������̊m��
	IMEInput = ( LPIMEINPUTDATA )DXALLOC( ( size_t )TotalSize ) ;
	if( IMEInput == NULL )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x29\xff\x2d\xff\x25\xff\x65\x51\x9b\x52\xc5\x60\x31\x58\x28\x75\x6e\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�h�l�d���͏��p�̃������̊m�ۂɎ��s���܂���\n" @*/ ) ;
		return NULL ;
	}
	CharBuf.IMEInputData = IMEInput ;

	// ���̃Z�b�g
	{
		const void *NextP ;
		int TLen ;

		// ��������̃Z�b�g
		IMEInput->InputString = ( TCHAR * )( IMEInput + 1 ) ;
		ConvString( ( const char * )CharBuf.InputString, WCHAR_T_CHARCODEFORMAT, ( char * )IMEInput->InputString, BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;
		TLen = ( int )_TSTRLEN( IMEInput->InputString ) ;

		// �J�[�\���ʒu�̃Z�b�g
		IMEInput->CursorPosition = NS_GetStringPoint( IMEInput->InputString , CharBuf.InputPoint ) ;

		// ���ߏ��̃Z�b�g
		if( CharBuf.ClauseNum > 1 )
		{
			IMEInput->ClauseNum		= CharBuf.ClauseNum - 1 ;
			IMEClause				= ( LPIMEINPUTCLAUSEDATA )( IMEInput->InputString + TLen + 1 ) ;
			IMEInput->ClauseData	= IMEClause ;
			IMEInput->SelectClause	= -1 ;
			for( i = 0 ; i < IMEInput->ClauseNum ; i ++ , IMEClause ++ )
			{
#ifdef UNICODE
				IMEClause->Position = CharBuf.ClauseData[ i ] ;
				IMEClause->Length   = CharBuf.ClauseData[ i + 1 ] - IMEClause->Position ;
#else
				int CharPosition0 ;
				int CharPosition1 ;
				int CharBytePosition0 ;
				int CharBytePosition1 ;

				CharPosition0 = GetStringPoint2_WCHAR_T( CharBuf.InputString, CharBuf.ClauseData[ i     ] ) ;
				CharPosition1 = GetStringPoint2_WCHAR_T( CharBuf.InputString, CharBuf.ClauseData[ i + 1 ] ) ;

				CharBytePosition0 = NS_GetStringPoint( IMEInput->InputString, CharPosition0 ) * sizeof( char ) ;
				CharBytePosition1 = NS_GetStringPoint( IMEInput->InputString, CharPosition1 ) * sizeof( char ) ;

				IMEClause->Position = CharBytePosition0 ;
				IMEClause->Length   = CharBytePosition1 - IMEClause->Position ;
#endif

				if( IMEClause->Position == IMEInput->CursorPosition )
				{
					IMEInput->SelectClause = i ;
				}
			}

			NextP = IMEInput->ClauseData + IMEInput->ClauseNum ;
		}
		else
		{
			IMEInput->ClauseNum		= 0 ;
			IMEInput->ClauseData	= NULL ;
			IMEInput->SelectClause	= -1 ;

			NextP = IMEInput->InputString + TLen + 1 ;
		}

		// �ϊ������̃Z�b�g
		if( CharBuf.CandidateList != NULL )
		{
			TCHAR **CandidateList ;

			CandidateList			= ( TCHAR ** )NextP ;
			IMEInput->CandidateList = ( const TCHAR ** )CandidateList ;
			IMEInput->CandidateNum  = ( int )CharBuf.CandidateList->dwCount ;
#ifdef UNICODE
			PCANDIDATELIST CandidateListBase ;
			CandidateListBase       = ( PCANDIDATELIST )( IMEInput->CandidateList + IMEInput->CandidateNum ) ;
			_MEMCPY( CandidateListBase, CharBuf.CandidateList, ( size_t )CharBuf.CandidateListSize ) ;
			for( i = 0 ; i < IMEInput->CandidateNum ; i ++ )
			{
				CandidateList[ i ] = ( wchar_t * )( ( BYTE * )CandidateListBase + CandidateListBase->dwOffset[ i ] ) ;
			}
#else
			wchar_t *CandidateListWCharString ;
			TCHAR   *TCharBuffer ;
			int     StringBytes ;

			TCharBuffer = ( TCHAR * )( IMEInput->CandidateList + IMEInput->CandidateNum ) ;
			for( i = 0 ; i < IMEInput->CandidateNum ; i ++ )
			{
				CandidateListWCharString = ( wchar_t * )( ( BYTE * )CharBuf.CandidateList + CharBuf.CandidateList->dwOffset[ i ] ) ;
				StringBytes = ConvString( ( const char * )CandidateListWCharString, WCHAR_T_CHARCODEFORMAT, TCharBuffer, BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;
				CandidateList[ i ] = TCharBuffer ;
				TCharBuffer += StringBytes / sizeof( char ) ;
			}
#endif
			IMEInput->SelectCandidate = ( int )CharBuf.CandidateList->dwSelection ;
		}
		else
		{
			IMEInput->CandidateList   = NULL ;
			IMEInput->CandidateNum    = 0 ;
			IMEInput->SelectCandidate = 0 ;
		}

		// ������ϊ������ǂ������Z�b�g
		if( CharBuf.CharAttrNum == 0 )
		{
			IMEInput->ConvertFlag = FALSE ;
		}
		else
		{
			for( i = 0 ; i < CharBuf.CharAttrNum && CharBuf.CharAttr[ i ] != ATTR_INPUT ; i ++ ){}
			IMEInput->ConvertFlag = i == CharBuf.CharAttrNum ? TRUE : FALSE ;
		}
	}

	// �쐬��������Ԃ�
	return CharBuf.IMEInputData ;
}

/*
// IME�ŕ������ϊ����̏ꍇ�ɕϊ����̒��őI���������ύX����
extern int NS_SetIMESelectCandidate( int CandidateIndex )
{
	HIMC Imc ;

	// �h�l�d���g���Ă��Ȃ��Ƃ��� -1 ��Ԃ�
	if( CharBuf.IMESwitch == FALSE )
	{
		return -1 ;
	}

	// ������ϊ����ł͖����ꍇ�� -1 ��Ԃ�
	if( CharBuf.CandidateList == NULL )
	{
		return -1 ;
	}

	// ���ԍ����s���ȏꍇ�� -1 ��Ԃ�
	if( CandidateIndex < 0 || ( DWORD )CandidateIndex >= CharBuf.CandidateList->dwCount )
	{
		return -1 ;
	}

	// �h�l�b�̎擾
	Imc = WinAPIData.ImmFunc.ImmGetContextFunc( WinData.MainWindow ) ;
	if( Imc == NULL )
	{
		return -1 ;
	}

	// ���̕ύX
	WinAPIData.ImmFunc.ImmNotifyIMEFunc( Imc , NI_SELECTCANDIDATESTR, 0, CandidateIndex );

	// �h�l�b�����
	WinAPIData.ImmFunc.ImmReleaseContextFunc( WinData.MainWindow, Imc );

	// �I��
	return 0 ;
}
*/

#endif

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_INPUTSTRING

