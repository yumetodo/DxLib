//-----------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		Windows用ファイル関係プログラム
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// ＤＸライブラリ作成時用定義
#define __DX_MAKE

// インクルード ---------------------------------------------------------------
#include "DxWindow.h"
#include "../DxFile.h"
#include "../DxLog.h"
#include "../DxChar.h"
#include "../DxBaseFunc.h"
#include <windows.h>

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 -----------------------------------------------------------------

// 構造体宣言 -----------------------------------------------------------------

// データ定義 -----------------------------------------------------------------

int g_CancelCurrentDirNameNum = 0 ;					// パスの始まりが / や \\ でもカレントディレクトリに落とさない文字列の数
char * g_CancelCurrentDirName[ 16 ] ;		// パスの始まりが / や \\ でもカレントディレクトリに落とさない文字列

// 関数宣言 -------------------------------------------------------------------

DWORD	WINAPI		FileAccessThreadFunction( void *FileAccessThreadData ) ;					// ファイルアクセス専用スレッド用関数
int					SetFilePointer64( HANDLE FileHandle, ULONGLONG Position ) ;					// ファイルのアクセス位置を設定する( 0:成功  -1:失敗 )

// プログラム -----------------------------------------------------------------

// ファイルのアクセス位置を設定する
int SetFilePointer64( HANDLE FileHandle, ULONGLONG Position )
{
	DWORD High, Low ;
	DWORD Result ;

	Low = ( DWORD )Position ;
	High = ( DWORD )( Position >> 32 ) ;
	Result = SetFilePointer( FileHandle, ( LONG )Low, ( LONG * )&High, FILE_BEGIN ) ;
	if( Result == 0xffffffff && GetLastError() != NO_ERROR )
	{
		return -1 ;
	}

	return 0 ;
}

// テンポラリファイルを作成する
extern HANDLE CreateTemporaryFile( wchar_t *TempFileNameBuffer, size_t BufferBytes )
{
	wchar_t String1[FILEPATH_MAX], String2[FILEPATH_MAX] ;
	HANDLE FileHandle ;
	int Length ;

	// テンポラリファイルのディレクトリパスを取得する
	if( GetTempPathW( sizeof( String1 ) / sizeof( wchar_t ), String1 ) == 0 ) return NULL ;

	// 文字列の最後に￥マークをつける
	Length = ( int )_WCSLEN( String1 ) ;
	if( String1[ Length - 1 ] != L'\\' )
	{
		_WCSCAT_S( String1, sizeof( String1 ), L"\\" ) ;
	}

	// テンポラリファイルのファイル名を作成する
	if( GetTempFileNameW( String1, L"tmp", 0, String2 ) == 0 ) return NULL ;

	// フルパスに変換
	ConvertFullPathW_( String2, String1, sizeof( String1 ) ) ;

	// テンポラリファイルを開く
	DeleteFileW( String1 ) ;
	FileHandle = CreateFileW( String1, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if( FileHandle == NULL )
	{
		return NULL ;
	}

	// テンポラリファイル名を保存
	if( TempFileNameBuffer != NULL )
	{
		_WCSCPY_S( TempFileNameBuffer, BufferBytes, String1 ) ;
	}

	// ハンドルを返す
	return FileHandle ;
}



// ファイルアクセス専用スレッド用関数
DWORD WINAPI FileAccessThreadFunction( void *FileAccessThreadData )
{
	FILEACCESSTHREAD *dat = (FILEACCESSTHREAD *)FileAccessThreadData ;
	DWORD res, ReadSize ;

	for(;;)
	{
		for(;;)
		{
			// キャッシュを使用すかどうかで処理を分岐
			if( dat->CacheBuffer )
			{
				// 指令が来るまでちょっと待つ
				res = WaitForSingleObject( dat->FuncEvent, 100 ) ;

				// 指令が来てい無い場合でファイルが開いている場合はキャッシング処理を行う
				if( res == WAIT_TIMEOUT && dat->Handle != NULL )
				{
					// もしキャッシュが一杯だったら何もしない
					if( dat->CacheSize != FILEACCESSTHREAD_DEFAULT_CACHESIZE )
					{
						// 読み込み開始位置セット
						SetFilePointer64( dat->Handle, dat->CachePosition + dat->CacheSize ) ;

						// 読み込み
						ReadFile( dat->Handle, &dat->CacheBuffer[dat->CacheSize], ( DWORD )( FILEACCESSTHREAD_DEFAULT_CACHESIZE - dat->CacheSize ), &ReadSize, NULL ) ;
						
						// 有効なサイズを増やす
						dat->CacheSize += ( LONGLONG )ReadSize ;
					}
				}
				else
				{
					break ;
				}
			}
			else
			{
				// 指令が来るまで待つ
				res = WaitForSingleObject( dat->FuncEvent, INFINITE ) ;
				if( res == WAIT_TIMEOUT && dat->Handle != NULL ) continue;
				break;
			}
		}

//		WaitForSingleObject( dat->FuncEvent, INFINITE ) ;

		// イベントのシグナル状態を解除する
		ResetEvent( dat->FuncEvent ) ;
		ResetEvent( dat->CompEvent ) ;

		// 指令が来たら判断する
		switch( dat->Function )
		{
		case FILEACCESSTHREAD_FUNCTION_OPEN :
			dat->Handle = CreateFileW( dat->FilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
			if( dat->Handle == INVALID_HANDLE_VALUE )
			{
				dat->ErrorFlag = TRUE ;
				goto END ;
			}
			break ;

		case FILEACCESSTHREAD_FUNCTION_CLOSE :
			CloseHandle( dat->Handle ) ;
			dat->Handle = NULL ;
			break ;

		case FILEACCESSTHREAD_FUNCTION_READ :
			// キャッシュと読み込み位置が一致している場合はキャッシュからデータを転送する
			if( dat->CacheBuffer && dat->ReadPosition == dat->CachePosition && dat->CacheSize != 0 )
			{
				DWORD MoveSize ;

				// 転送するサイズを調整
				MoveSize = dat->ReadSize ;
				if( MoveSize > dat->CacheSize ) MoveSize = ( DWORD )dat->CacheSize ;

				// 転送
				_MEMCPY( dat->ReadBuffer, dat->CacheBuffer, MoveSize ) ;

				// 読み込みサイズと読み込み位置を移動する
				dat->ReadBuffer = (void *)( (BYTE *)dat->ReadBuffer + MoveSize ) ;
				dat->ReadPosition += MoveSize ;
				dat->ReadSize -= MoveSize ;
				
				// キャッシュの情報も更新
				dat->CachePosition += MoveSize ;
				dat->CacheSize     -= MoveSize ;
				if( dat->CacheSize != 0 ) _MEMMOVE( &dat->CacheBuffer[0], &dat->CacheBuffer[MoveSize], ( size_t )dat->CacheSize ) ;
			}

			// 希望のデータが全て読めていない場合は更にファイルから読み込む
			if( dat->ReadSize != 0 )
			{
				SetFilePointer64( dat->Handle, dat->ReadPosition ) ;
				ReadFile( dat->Handle, dat->ReadBuffer, dat->ReadSize, &dat->ReadSize, NULL ) ;

				// キャッシュを初期化する
				if( dat->CacheBuffer )
				{
					dat->CachePosition = dat->ReadPosition + dat->ReadSize ;
					dat->CacheSize = 0 ;
				}
			}
			break ;

		case FILEACCESSTHREAD_FUNCTION_SEEK :
			SetFilePointer64( dat->Handle, dat->SeekPoint ) ;

			// キャッシュを初期化する
			if( dat->CacheBuffer )
			{
				dat->CachePosition = (DWORD)dat->SeekPoint ;
				dat->CacheSize = 0 ;
			}
			break ;

		case FILEACCESSTHREAD_FUNCTION_EXIT :
			if( dat->Handle != NULL ) CloseHandle( dat->Handle ) ;
			dat->Handle = NULL ;
			goto END ;
		}

		// 指令が完了したら完了イベントをシグナル状態にする
		SetEvent( dat->CompEvent ) ;
	}

END:
	// エラー時の為に完了イベントをシグナル状態にする
	SetEvent( dat->CompEvent ) ;
	dat->EndFlag = TRUE ;
	ExitThread( 1 ) ;

	return 0 ;
}



// ファイルアクセス処理の初期化・終了関数

// ファイルアクセス処理の初期化関数の環境依存の処理を行う関数
extern int InitializeFile_PF( void )
{
	return 0 ;
}

// ファイルアクセス処理の後始末関数の環境依存の処理を行う関数
extern int TerminateFile_PF( void )
{
	return 0 ;
}


// ファイルアクセス関数
extern int ReadOnlyFileAccessOpen_PF( FILEACCESS *FileAccess, const wchar_t *Path, int UseCacheFlag, int BlockReadFlag )
{
	DWORD Code ;
	DWORD High ;

//	UseCacheFlag = UseCacheFlag ;
	BlockReadFlag = BlockReadFlag ;

//	// キャッシュを使用するかどうかをスレッドを使用するかどうかにしてしまう
//	FileAccess->PF.UseThread = UseCacheFlag ;

	// キャッシュを使用するかどうかのフラグをセット
	FileAccess->PF.UseCacheFlag = UseCacheFlag ;
	FileAccess->PF.ThreadData.CacheBuffer = NULL;

	// キャッシュ、若しくは非同期読み書きを行う場合はスレッドを使用する
	FileAccess->PF.UseThread = FileAccess->PF.UseCacheFlag || FileAccess->UseASyncReadFlag ;

	// スレッドを使用する場合としない場合で処理を分岐
	if( FileAccess->PF.UseThread == TRUE )
	{
		// スレッドを使用する場合はファイルアクセス専用スレッドを立てる

		// 最初にファイルを開けるかどうか確かめておく
		FileAccess->PF.Handle = CreateFileW( Path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
		if( FileAccess->PF.Handle == INVALID_HANDLE_VALUE )
		{
			return -1 ;
		}
		FileAccess->Size = GetFileSize( FileAccess->PF.Handle, &High ) ;
		FileAccess->Size |= ( ( ULONGLONG )High ) << 32 ;
		CloseHandle( FileAccess->PF.Handle ) ;
		FileAccess->PF.Handle = NULL ;

		// キャッシュ用メモリの確保
		if( FileAccess->PF.UseCacheFlag )
		{
			FileAccess->PF.ThreadData.CacheBuffer = (BYTE *)DXALLOC( FILEACCESSTHREAD_DEFAULT_CACHESIZE );
			if( FileAccess->PF.ThreadData.CacheBuffer == NULL )
			{
				DXFREE( FileAccess->PF.ThreadData.CacheBuffer ) ;
				DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\xad\x30\xe3\x30\xc3\x30\xb7\x30\xe5\x30\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ファイル読み込みキャッシュ用メモリの確保に失敗しました\n" @*/ ) ;
				return -1 ;
			}
		}

		// 専用スレッドデータを初期化する
		FileAccess->PF.ThreadData.Handle = NULL ;
		FileAccess->PF.ThreadData.ThreadHandle = NULL ;
		FileAccess->PF.ThreadData.FuncEvent = CreateEvent( NULL, TRUE, FALSE, NULL ) ;
		FileAccess->PF.ThreadData.CompEvent = CreateEvent( NULL, TRUE, TRUE, NULL ) ;

		FileAccess->PF.ThreadData.ThreadHandle = CreateThread(
												NULL,
												0,
												(LPTHREAD_START_ROUTINE)FileAccessThreadFunction, 
												&FileAccess->PF.ThreadData,
												0,
												&FileAccess->PF.ThreadData.ThreadID ) ;
		if( FileAccess->PF.ThreadData.ThreadHandle == NULL )
		{
			if( FileAccess->PF.ThreadData.CacheBuffer ) DXFREE( FileAccess->PF.ThreadData.CacheBuffer ) ;
			CloseHandle( FileAccess->PF.ThreadData.FuncEvent ) ;
			CloseHandle( FileAccess->PF.ThreadData.CompEvent ) ;
			DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xa2\x30\xaf\x30\xbb\x30\xb9\x30\x02\x5c\x28\x75\xb9\x30\xec\x30\xc3\x30\xc9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ファイルアクセス専用スレッドの作成に失敗しました\n" @*/ ) ;
			return -1 ;
		}
		SetThreadPriority( FileAccess->PF.ThreadData.ThreadHandle, THREAD_PRIORITY_LOWEST ) ;

		// ファイルオープン指令はここで完了してしまう
		FileAccess->PF.ThreadData.Function = FILEACCESSTHREAD_FUNCTION_OPEN ;
		_WCSCPY_S( FileAccess->PF.ThreadData.FilePath, sizeof( FileAccess->PF.ThreadData.FilePath ), Path ) ;

		ResetEvent( FileAccess->PF.ThreadData.CompEvent ) ;
		SetEvent( FileAccess->PF.ThreadData.FuncEvent ) ;

		// 指令が終了するまで待つ
		WaitForSingleObject( FileAccess->PF.ThreadData.CompEvent, INFINITE ) ;
		if( FileAccess->PF.ThreadData.ErrorFlag == TRUE )
		{
			if( FileAccess->PF.ThreadData.CacheBuffer ) DXFREE( FileAccess->PF.ThreadData.CacheBuffer ) ;
			CloseHandle( FileAccess->PF.ThreadData.FuncEvent ) ;
			CloseHandle( FileAccess->PF.ThreadData.CompEvent ) ;
			do
			{
				Sleep( 0 ) ;
				GetExitCodeThread( FileAccess->PF.ThreadData.ThreadHandle, &Code ) ;
			}while( Code == STILL_ACTIVE ) ;
			CloseHandle( FileAccess->PF.ThreadData.ThreadHandle ) ;
			DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\xaa\x30\xfc\x30\xd7\x30\xf3\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"ファイルのオープンに失敗しました\n" @*/ ) ;
			return -1 ;
		}
	}
	else
	{
		// スレッドを使用しない場合はこの場でファイルを開く
		FileAccess->PF.Handle = CreateFileW( Path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
		if( FileAccess->PF.Handle == INVALID_HANDLE_VALUE )
		{
			return -1 ;
		}
		FileAccess->Size = GetFileSize( FileAccess->PF.Handle, &High ) ;
		FileAccess->Size |= ( ( ULONGLONG )High ) << 32 ;
	}

	return 0 ;
}

extern int ReadOnlyFileAccessClose_PF( FILEACCESS *FileAccess )
{
	BOOL Result;
	DWORD Code ;

	// スレッドを使用する場合としない場合で処理を分岐
	if( FileAccess->PF.UseThread == TRUE )
	{
		// これ以前の指令が出ていた場合の為に指令完了イベントがシグナル状態になるまで待つ
		WaitForSingleObject( FileAccess->PF.ThreadData.CompEvent, INFINITE ) ;

		// スレッドに終了指令を出す
		FileAccess->PF.ThreadData.Function = FILEACCESSTHREAD_FUNCTION_EXIT ;
		ResetEvent( FileAccess->PF.ThreadData.CompEvent ) ;
		SetEvent( FileAccess->PF.ThreadData.FuncEvent ) ;

		// 指令が終了するまで待つ
		WaitForSingleObject( FileAccess->PF.ThreadData.CompEvent, INFINITE ) ;

		// スレッドが終了するのを待つ
		do
		{
			Sleep(1);
			GetExitCodeThread( FileAccess->PF.ThreadData.ThreadHandle, &Code );
		}while( Code == STILL_ACTIVE );

		// キャッシュを使用していた場合はキャッシュ用メモリを開放する
		if( FileAccess->PF.ThreadData.CacheBuffer )
			DXFREE( FileAccess->PF.ThreadData.CacheBuffer ) ;

		// イベントやスレッドを閉じる
		CloseHandle( FileAccess->PF.ThreadData.ThreadHandle ) ;
		CloseHandle( FileAccess->PF.ThreadData.CompEvent ) ;
		CloseHandle( FileAccess->PF.ThreadData.FuncEvent ) ;
		Result = 0 ;
	}
	else
	{
		// 使用していない場合はこの場でハンドルを閉じて終了
		Result = CloseHandle( FileAccess->PF.Handle ) ;
	}

	return Result != 0 ? 0 : -1/*EOF*/ ;
}

extern int ReadOnlyFileAccessSeek_PF( FILEACCESS *FileAccess, LONGLONG SeekPoint )
{
	// スレッドを使用しているかどうかで処理を分岐
	if( FileAccess->PF.UseThread == TRUE )
	{
		// これ以前の指令が出ていた場合の為に指令完了イベントがシグナル状態になるまで待つ
		WaitForSingleObject( FileAccess->PF.ThreadData.CompEvent, INFINITE ) ;

		// スレッドにファイル位置変更指令を出す
		FileAccess->PF.ThreadData.Function = FILEACCESSTHREAD_FUNCTION_SEEK ;
		FileAccess->PF.ThreadData.SeekPoint = ( ULONGLONG )SeekPoint ;
		ResetEvent( FileAccess->PF.ThreadData.CompEvent ) ;
		SetEvent( FileAccess->PF.ThreadData.FuncEvent ) ;
	}
	else
	{
		// ファイルアクセス位置を変更する
		if( SetFilePointer64( FileAccess->PF.Handle, ( ULONGLONG )SeekPoint ) == -1 )
		{
			return -1 ;
		}
	}

	// 終了
	return 0 ;
}

extern	size_t ReadOnlyFileAccessRead_PF( void *Buffer, size_t BlockSize, size_t DataNum, FILEACCESS *FileAccess )
{
	DWORD BytesRead ;

	// スレッドを使用しているかどうかで処理を分岐
	if( FileAccess->PF.UseThread == TRUE )
	{
		// これ以前の指令が出ていた場合の為に指令完了イベントがシグナル状態になるまで待つ
		WaitForSingleObject( FileAccess->PF.ThreadData.CompEvent, INFINITE ) ;

		// スレッドにファイル読み込み指令を出す
		FileAccess->PF.ThreadData.Function = FILEACCESSTHREAD_FUNCTION_READ ;
		FileAccess->PF.ThreadData.ReadBuffer = Buffer ;
		FileAccess->PF.ThreadData.ReadPosition = FileAccess->Position ;
		FileAccess->PF.ThreadData.ReadSize = ( DWORD )( BlockSize * DataNum ) ;
		ResetEvent( FileAccess->PF.ThreadData.CompEvent ) ;
		SetEvent( FileAccess->PF.ThreadData.FuncEvent ) ;

		// 非同期かどうかで処理を分岐
		if( FileAccess->UseASyncReadFlag == FALSE )
		{
			// 同期読み込みの場合は指令が完了するまで待つ
			WaitForSingleObject( FileAccess->PF.ThreadData.CompEvent, INFINITE ) ;
		}

		BytesRead = ( DWORD )( BlockSize * DataNum ) ;
	}
	else
	{
		ReadFile( FileAccess->PF.Handle, Buffer, ( DWORD )( BlockSize * DataNum ), &BytesRead, NULL ) ;
	}

	return BytesRead ;
}

extern int ReadOnlyFileAccessIdleCheck_PF( FILEACCESS *FileAccess )
{
	if( FileAccess->PF.UseThread == TRUE )
	{
		return WaitForSingleObject( FileAccess->PF.ThreadData.CompEvent, 0 ) == WAIT_TIMEOUT ? FALSE : TRUE ;
	}
	else
	{
		return TRUE ;
	}
}

extern int ReadOnlyFileAccessChDir_PF( const wchar_t *Path )
{
	return SetCurrentDirectoryW( Path ) ;
}

extern int ReadOnlyFileAccessGetDirS_PF( wchar_t *Buffer, size_t BufferBytes )
{
	return ( int )GetCurrentDirectoryW( ( DWORD )( BufferBytes / sizeof( wchar_t ) ), Buffer ) ;
}

static void _WIN32_FIND_DATA_To_FILEINFO( WIN32_FIND_DATAW *FindData, FILEINFOW *FileInfo )
{
	// ファイル名のコピー
	_WCSCPY_S( FileInfo->Name, sizeof( FileInfo->Name ), FindData->cFileName );

	// ディレクトリかどうかのフラグをセット
	FileInfo->DirFlag = (FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 ? TRUE : FALSE;

	// ファイルのサイズをセット
	FileInfo->Size = ( LONGLONG )( ( ( ( ULONGLONG )FindData->nFileSizeHigh ) << 32 ) + FindData->nFileSizeLow ) ;

	// ファイルタイムを保存
	_FileTimeToLocalDateData( &FindData->ftCreationTime, &FileInfo->CreationTime );
	_FileTimeToLocalDateData( &FindData->ftLastWriteTime, &FileInfo->LastWriteTime );
}

extern int ReadOnlyFileAccessFindFirst_PF( FINDINFO *FindInfo, const wchar_t *FilePath, FILEINFOW *Buffer )
{
	WIN32_FIND_DATAW FindData ;

	FindInfo->PF.FindHandle = FindFirstFileW( FilePath, &FindData ) ;
	if( FindInfo->PF.FindHandle == INVALID_HANDLE_VALUE )
	{
		return -1;
	}

	if( Buffer )
	{
		_WIN32_FIND_DATA_To_FILEINFO( &FindData, Buffer ) ;
	}

	return 0 ;
}

extern int ReadOnlyFileAccessFindNext_PF( FINDINFO *FindInfo, FILEINFOW *Buffer )
{
	WIN32_FIND_DATAW FindData ;

	if( FindNextFileW( FindInfo->PF.FindHandle, &FindData ) == 0 )
	{
		return -1 ;
	}

	if( Buffer )
	{
		_WIN32_FIND_DATA_To_FILEINFO( &FindData, Buffer ) ;
	}

	return 0 ;
}

extern int ReadOnlyFileAccessFindClose_PF( FINDINFO *FindInfo )
{
	// ０以外が返ってきたら成功
	return FindClose( FindInfo->PF.FindHandle ) != 0 ? 0 : -1 ;
}





// 書き込み専用ファイルアクセス関数
extern	int			WriteOnlyFileAccessDelete_PF( const wchar_t *Path )
{
	DeleteFileW( Path ) ;

	return 0 ;
}

extern	DWORD_PTR	WriteOnlyFileAccessOpen_PF(  const wchar_t *Path )
{
	DeleteFileW( Path ) ;
	HANDLE Result = CreateFileW( Path, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ) ;

	return Result == INVALID_HANDLE_VALUE ? NULL : ( DWORD_PTR )Result ;
}

extern	int			WriteOnlyFileAccessClose_PF( DWORD_PTR Handle )
{
	return CloseHandle( ( HANDLE )Handle ) ;
}

extern	int			WriteOnlyFileAccessSeek_PF(  DWORD_PTR Handle, LONGLONG SeekPoint, int SeekType )
{
	DWORD MoveMethod = FILE_BEGIN ;

	switch( SeekType )
	{
	case SEEK_SET :
		MoveMethod = FILE_BEGIN ;
		break ;

	case SEEK_END :
		MoveMethod = FILE_END ;
		break ;

	case SEEK_CUR :
		MoveMethod = FILE_CURRENT ;
		break ;
	}

	SetFilePointer( ( HANDLE )Handle, ( LONG )SeekPoint, NULL, MoveMethod ) ;

	return 0 ;
}

extern	int			WriteOnlyFileAccessWrite_PF( DWORD_PTR Handle, void *Buffer, size_t WriteSize, size_t *GetWriteSize )
{
	DWORD lWriteSize ;

	WriteFile( ( HANDLE )Handle, Buffer, ( DWORD )WriteSize, &lWriteSize, NULL ) ;

	if( GetWriteSize )
	{
		*GetWriteSize = lWriteSize ;
	}

	return 0 ;
}








#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

