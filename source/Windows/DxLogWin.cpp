//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Windows�p���O�v���O����
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "../DxCompileConfig.h"

// �C���N���[�h ---------------------------------------------------------------
#include "DxLogWin.h"

#ifndef DX_NON_LOG

#include "../DxStatic.h"
#include "../DxLib.h"
#include "../DxLog.h"
#include "../DxBaseFunc.h"
#include "../DxMemory.h"
#include "../DxGraphics.h"
#include "DxWindow.h"
#include "DxWinAPI.h"
#include "DxGraphicsWin.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` -----------------------------------------------------------------

// �\���̐錾 -----------------------------------------------------------------

// �f�[�^��` -----------------------------------------------------------------

// �֐��錾 -------------------------------------------------------------------

// �v���O���� -----------------------------------------------------------------

// �G���[�����o���t�@�C�������������鏈���̊��ˑ�����
extern int ErrorLogInitialize_PF( const wchar_t *LogFilePath )
{
	HANDLE fp ;

	// �G���[���O�t�@�C�����č쐬����
	DeleteFileW( LogFilePath ) ;
	fp = CreateFileW( LogFilePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ) ;

	BYTE BOM[ 2 ] = { 0xff, 0xfe } ;
	DWORD WriteSize ;
	WriteFile( fp, &BOM, 2, &WriteSize, NULL ) ;

	if( fp != NULL )
	{
		CloseHandle( fp ) ;
	}

	// �I��
	return 0 ;
}

// �G���[�����������o�������̊��ˑ�����
extern int ErrorLogAdd_WCHAR_T_PF( const wchar_t *LogFilePath, const wchar_t *ErrorStr )
{
	HANDLE	fp ;
	DWORD	WriteSize ;

	if( WinAPIData.Win32Func.WinMMDLL == NULL )
	{
		LoadWinAPI() ;
		if( WinAPIData.Win32Func.WinMMDLL == NULL )
		{
			return -1 ;
		}
	}

	// �G���[���O�t�@�C�����J��
	fp = CreateFileW( LogFilePath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if( fp != NULL )
	{
		// �t�@�C���A�N�Z�X�ʒu���t�@�C���̖��[�Ɉړ�
		SetFilePointer( fp, 0, NULL, FILE_END ) ;

		// �G���[���O�t�@�C���ɏ����o��
		WriteFile( fp, ErrorStr, ( DWORD )( _WCSLEN( ErrorStr ) * sizeof( wchar_t ) ), &WriteSize, NULL ) ;

		// �G���[���O���A�E�g�v�b�g�ɏ����o��
		OutputDebugStringW( ErrorStr ) ;

		// �t�@�C�������
		CloseHandle( fp ) ;
	}
	
	// �I��
	return 0 ;
}

// ���O�@�\�̏��������s�����ǂ������擾����
extern int IsInitializeLog( void )
{
	if( WinData.MainWindow == NULL )
	{
		return FALSE ;
	}

#ifndef DX_NON_GRAPHICS
	// �n�[�h�E�G�A�@�\���g���Ă��邩�ǂ����ŏ����𕪊�
	if( GSYS.Setting.ValidHardware )
	{
		switch( GRAWIN.Setting.UseGraphicsAPI )
		{
#ifndef DX_NON_DIRECT3D11
		case GRAPHICS_API_DIRECT3D11_WIN32 :
			return D3D11Device_IsValid() ? TRUE : FALSE ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
		case GRAPHICS_API_DIRECT3D9_WIN32 :
			return NS_GetUseDDrawObj() == NULL ? FALSE : TRUE ;
#endif // DX_NON_DIRECT3D9

		default :
			return FALSE ;
		}
	}
	else
	{
		// �c�w���C�u�������I����Ԃɓ����Ă����珉�������Ȃ�
		return NS_GetUseDDrawObj() == NULL || WinData.MainWindow == NULL ? FALSE : TRUE ;
	}
#else // DX_NON_GRAPHICS
	// �c�w���C�u�������I����Ԃɓ����Ă����珉�������Ȃ�
	return WinData.MainWindow == NULL ? FALSE : TRUE ;
#endif // DX_NON_GRAPHICS

	return TRUE ;
}


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_LOG

