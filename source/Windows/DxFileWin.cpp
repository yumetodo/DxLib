//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Windows�p�t�@�C���֌W�v���O����
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

// �C���N���[�h ---------------------------------------------------------------
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

// �}�N����` -----------------------------------------------------------------

// �\���̐錾 -----------------------------------------------------------------

// �f�[�^��` -----------------------------------------------------------------

int g_CancelCurrentDirNameNum = 0 ;					// �p�X�̎n�܂肪 / �� \\ �ł��J�����g�f�B���N�g���ɗ��Ƃ��Ȃ�������̐�
char * g_CancelCurrentDirName[ 16 ] ;		// �p�X�̎n�܂肪 / �� \\ �ł��J�����g�f�B���N�g���ɗ��Ƃ��Ȃ�������

// �֐��錾 -------------------------------------------------------------------

DWORD	WINAPI		FileAccessThreadFunction( void *FileAccessThreadData ) ;					// �t�@�C���A�N�Z�X��p�X���b�h�p�֐�
int					SetFilePointer64( HANDLE FileHandle, ULONGLONG Position ) ;					// �t�@�C���̃A�N�Z�X�ʒu��ݒ肷��( 0:����  -1:���s )

// �v���O���� -----------------------------------------------------------------

// �t�@�C���̃A�N�Z�X�ʒu��ݒ肷��
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

// �e���|�����t�@�C�����쐬����
extern HANDLE CreateTemporaryFile( wchar_t *TempFileNameBuffer, size_t BufferBytes )
{
	wchar_t String1[FILEPATH_MAX], String2[FILEPATH_MAX] ;
	HANDLE FileHandle ;
	int Length ;

	// �e���|�����t�@�C���̃f�B���N�g���p�X���擾����
	if( GetTempPathW( sizeof( String1 ) / sizeof( wchar_t ), String1 ) == 0 ) return NULL ;

	// ������̍Ō�Ɂ��}�[�N������
	Length = ( int )_WCSLEN( String1 ) ;
	if( String1[ Length - 1 ] != L'\\' )
	{
		_WCSCAT_S( String1, sizeof( String1 ), L"\\" ) ;
	}

	// �e���|�����t�@�C���̃t�@�C�������쐬����
	if( GetTempFileNameW( String1, L"tmp", 0, String2 ) == 0 ) return NULL ;

	// �t���p�X�ɕϊ�
	ConvertFullPathW_( String2, String1, sizeof( String1 ) ) ;

	// �e���|�����t�@�C�����J��
	DeleteFileW( String1 ) ;
	FileHandle = CreateFileW( String1, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if( FileHandle == NULL )
	{
		return NULL ;
	}

	// �e���|�����t�@�C������ۑ�
	if( TempFileNameBuffer != NULL )
	{
		_WCSCPY_S( TempFileNameBuffer, BufferBytes, String1 ) ;
	}

	// �n���h����Ԃ�
	return FileHandle ;
}



// �t�@�C���A�N�Z�X��p�X���b�h�p�֐�
DWORD WINAPI FileAccessThreadFunction( void *FileAccessThreadData )
{
	FILEACCESSTHREAD *dat = (FILEACCESSTHREAD *)FileAccessThreadData ;
	DWORD res, ReadSize ;

	for(;;)
	{
		for(;;)
		{
			// �L���b�V�����g�p�����ǂ����ŏ����𕪊�
			if( dat->CacheBuffer )
			{
				// �w�߂�����܂ł�����Ƒ҂�
				res = WaitForSingleObject( dat->FuncEvent, 100 ) ;

				// �w�߂����Ă������ꍇ�Ńt�@�C�����J���Ă���ꍇ�̓L���b�V���O�������s��
				if( res == WAIT_TIMEOUT && dat->Handle != NULL )
				{
					// �����L���b�V������t�������牽�����Ȃ�
					if( dat->CacheSize != FILEACCESSTHREAD_DEFAULT_CACHESIZE )
					{
						// �ǂݍ��݊J�n�ʒu�Z�b�g
						SetFilePointer64( dat->Handle, dat->CachePosition + dat->CacheSize ) ;

						// �ǂݍ���
						ReadFile( dat->Handle, &dat->CacheBuffer[dat->CacheSize], ( DWORD )( FILEACCESSTHREAD_DEFAULT_CACHESIZE - dat->CacheSize ), &ReadSize, NULL ) ;
						
						// �L���ȃT�C�Y�𑝂₷
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
				// �w�߂�����܂ő҂�
				res = WaitForSingleObject( dat->FuncEvent, INFINITE ) ;
				if( res == WAIT_TIMEOUT && dat->Handle != NULL ) continue;
				break;
			}
		}

//		WaitForSingleObject( dat->FuncEvent, INFINITE ) ;

		// �C�x���g�̃V�O�i����Ԃ���������
		ResetEvent( dat->FuncEvent ) ;
		ResetEvent( dat->CompEvent ) ;

		// �w�߂������画�f����
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
			// �L���b�V���Ɠǂݍ��݈ʒu����v���Ă���ꍇ�̓L���b�V������f�[�^��]������
			if( dat->CacheBuffer && dat->ReadPosition == dat->CachePosition && dat->CacheSize != 0 )
			{
				DWORD MoveSize ;

				// �]������T�C�Y�𒲐�
				MoveSize = dat->ReadSize ;
				if( MoveSize > dat->CacheSize ) MoveSize = ( DWORD )dat->CacheSize ;

				// �]��
				_MEMCPY( dat->ReadBuffer, dat->CacheBuffer, MoveSize ) ;

				// �ǂݍ��݃T�C�Y�Ɠǂݍ��݈ʒu���ړ�����
				dat->ReadBuffer = (void *)( (BYTE *)dat->ReadBuffer + MoveSize ) ;
				dat->ReadPosition += MoveSize ;
				dat->ReadSize -= MoveSize ;
				
				// �L���b�V���̏����X�V
				dat->CachePosition += MoveSize ;
				dat->CacheSize     -= MoveSize ;
				if( dat->CacheSize != 0 ) _MEMMOVE( &dat->CacheBuffer[0], &dat->CacheBuffer[MoveSize], ( size_t )dat->CacheSize ) ;
			}

			// ��]�̃f�[�^���S�ēǂ߂Ă��Ȃ��ꍇ�͍X�Ƀt�@�C������ǂݍ���
			if( dat->ReadSize != 0 )
			{
				SetFilePointer64( dat->Handle, dat->ReadPosition ) ;
				ReadFile( dat->Handle, dat->ReadBuffer, dat->ReadSize, &dat->ReadSize, NULL ) ;

				// �L���b�V��������������
				if( dat->CacheBuffer )
				{
					dat->CachePosition = dat->ReadPosition + dat->ReadSize ;
					dat->CacheSize = 0 ;
				}
			}
			break ;

		case FILEACCESSTHREAD_FUNCTION_SEEK :
			SetFilePointer64( dat->Handle, dat->SeekPoint ) ;

			// �L���b�V��������������
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

		// �w�߂����������犮���C�x���g���V�O�i����Ԃɂ���
		SetEvent( dat->CompEvent ) ;
	}

END:
	// �G���[���ׂ̈Ɋ����C�x���g���V�O�i����Ԃɂ���
	SetEvent( dat->CompEvent ) ;
	dat->EndFlag = TRUE ;
	ExitThread( 1 ) ;

	return 0 ;
}



// �t�@�C���A�N�Z�X�����̏������E�I���֐�

// �t�@�C���A�N�Z�X�����̏������֐��̊��ˑ��̏������s���֐�
extern int InitializeFile_PF( void )
{
	return 0 ;
}

// �t�@�C���A�N�Z�X�����̌�n���֐��̊��ˑ��̏������s���֐�
extern int TerminateFile_PF( void )
{
	return 0 ;
}


// �t�@�C���A�N�Z�X�֐�
extern int ReadOnlyFileAccessOpen_PF( FILEACCESS *FileAccess, const wchar_t *Path, int UseCacheFlag, int BlockReadFlag )
{
	DWORD Code ;
	DWORD High ;

//	UseCacheFlag = UseCacheFlag ;
	BlockReadFlag = BlockReadFlag ;

//	// �L���b�V�����g�p���邩�ǂ������X���b�h���g�p���邩�ǂ����ɂ��Ă��܂�
//	FileAccess->PF.UseThread = UseCacheFlag ;

	// �L���b�V�����g�p���邩�ǂ����̃t���O���Z�b�g
	FileAccess->PF.UseCacheFlag = UseCacheFlag ;
	FileAccess->PF.ThreadData.CacheBuffer = NULL;

	// �L���b�V���A�Ⴕ���͔񓯊��ǂݏ������s���ꍇ�̓X���b�h���g�p����
	FileAccess->PF.UseThread = FileAccess->PF.UseCacheFlag || FileAccess->UseASyncReadFlag ;

	// �X���b�h���g�p����ꍇ�Ƃ��Ȃ��ꍇ�ŏ����𕪊�
	if( FileAccess->PF.UseThread == TRUE )
	{
		// �X���b�h���g�p����ꍇ�̓t�@�C���A�N�Z�X��p�X���b�h�𗧂Ă�

		// �ŏ��Ƀt�@�C�����J���邩�ǂ����m���߂Ă���
		FileAccess->PF.Handle = CreateFileW( Path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
		if( FileAccess->PF.Handle == INVALID_HANDLE_VALUE )
		{
			return -1 ;
		}
		FileAccess->Size = GetFileSize( FileAccess->PF.Handle, &High ) ;
		FileAccess->Size |= ( ( ULONGLONG )High ) << 32 ;
		CloseHandle( FileAccess->PF.Handle ) ;
		FileAccess->PF.Handle = NULL ;

		// �L���b�V���p�������̊m��
		if( FileAccess->PF.UseCacheFlag )
		{
			FileAccess->PF.ThreadData.CacheBuffer = (BYTE *)DXALLOC( FILEACCESSTHREAD_DEFAULT_CACHESIZE );
			if( FileAccess->PF.ThreadData.CacheBuffer == NULL )
			{
				DXFREE( FileAccess->PF.ThreadData.CacheBuffer ) ;
				DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\xad\x30\xe3\x30\xc3\x30\xb7\x30\xe5\x30\x28\x75\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�t�@�C���ǂݍ��݃L���b�V���p�������̊m�ۂɎ��s���܂���\n" @*/ ) ;
				return -1 ;
			}
		}

		// ��p�X���b�h�f�[�^������������
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
			DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xa1\x30\xa4\x30\xeb\x30\xa2\x30\xaf\x30\xbb\x30\xb9\x30\x02\x5c\x28\x75\xb9\x30\xec\x30\xc3\x30\xc9\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�t�@�C���A�N�Z�X��p�X���b�h�̍쐬�Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}
		SetThreadPriority( FileAccess->PF.ThreadData.ThreadHandle, THREAD_PRIORITY_LOWEST ) ;

		// �t�@�C���I�[�v���w�߂͂����Ŋ������Ă��܂�
		FileAccess->PF.ThreadData.Function = FILEACCESSTHREAD_FUNCTION_OPEN ;
		_WCSCPY_S( FileAccess->PF.ThreadData.FilePath, sizeof( FileAccess->PF.ThreadData.FilePath ), Path ) ;

		ResetEvent( FileAccess->PF.ThreadData.CompEvent ) ;
		SetEvent( FileAccess->PF.ThreadData.FuncEvent ) ;

		// �w�߂��I������܂ő҂�
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
			DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\xaa\x30\xfc\x30\xd7\x30\xf3\x30\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�t�@�C���̃I�[�v���Ɏ��s���܂���\n" @*/ ) ;
			return -1 ;
		}
	}
	else
	{
		// �X���b�h���g�p���Ȃ��ꍇ�͂��̏�Ńt�@�C�����J��
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

	// �X���b�h���g�p����ꍇ�Ƃ��Ȃ��ꍇ�ŏ����𕪊�
	if( FileAccess->PF.UseThread == TRUE )
	{
		// ����ȑO�̎w�߂��o�Ă����ꍇ�ׂ̈Ɏw�ߊ����C�x���g���V�O�i����ԂɂȂ�܂ő҂�
		WaitForSingleObject( FileAccess->PF.ThreadData.CompEvent, INFINITE ) ;

		// �X���b�h�ɏI���w�߂��o��
		FileAccess->PF.ThreadData.Function = FILEACCESSTHREAD_FUNCTION_EXIT ;
		ResetEvent( FileAccess->PF.ThreadData.CompEvent ) ;
		SetEvent( FileAccess->PF.ThreadData.FuncEvent ) ;

		// �w�߂��I������܂ő҂�
		WaitForSingleObject( FileAccess->PF.ThreadData.CompEvent, INFINITE ) ;

		// �X���b�h���I������̂�҂�
		do
		{
			Sleep(1);
			GetExitCodeThread( FileAccess->PF.ThreadData.ThreadHandle, &Code );
		}while( Code == STILL_ACTIVE );

		// �L���b�V�����g�p���Ă����ꍇ�̓L���b�V���p���������J������
		if( FileAccess->PF.ThreadData.CacheBuffer )
			DXFREE( FileAccess->PF.ThreadData.CacheBuffer ) ;

		// �C�x���g��X���b�h�����
		CloseHandle( FileAccess->PF.ThreadData.ThreadHandle ) ;
		CloseHandle( FileAccess->PF.ThreadData.CompEvent ) ;
		CloseHandle( FileAccess->PF.ThreadData.FuncEvent ) ;
		Result = 0 ;
	}
	else
	{
		// �g�p���Ă��Ȃ��ꍇ�͂��̏�Ńn���h������ďI��
		Result = CloseHandle( FileAccess->PF.Handle ) ;
	}

	return Result != 0 ? 0 : -1/*EOF*/ ;
}

extern int ReadOnlyFileAccessSeek_PF( FILEACCESS *FileAccess, LONGLONG SeekPoint )
{
	// �X���b�h���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( FileAccess->PF.UseThread == TRUE )
	{
		// ����ȑO�̎w�߂��o�Ă����ꍇ�ׂ̈Ɏw�ߊ����C�x���g���V�O�i����ԂɂȂ�܂ő҂�
		WaitForSingleObject( FileAccess->PF.ThreadData.CompEvent, INFINITE ) ;

		// �X���b�h�Ƀt�@�C���ʒu�ύX�w�߂��o��
		FileAccess->PF.ThreadData.Function = FILEACCESSTHREAD_FUNCTION_SEEK ;
		FileAccess->PF.ThreadData.SeekPoint = ( ULONGLONG )SeekPoint ;
		ResetEvent( FileAccess->PF.ThreadData.CompEvent ) ;
		SetEvent( FileAccess->PF.ThreadData.FuncEvent ) ;
	}
	else
	{
		// �t�@�C���A�N�Z�X�ʒu��ύX����
		if( SetFilePointer64( FileAccess->PF.Handle, ( ULONGLONG )SeekPoint ) == -1 )
		{
			return -1 ;
		}
	}

	// �I��
	return 0 ;
}

extern	size_t ReadOnlyFileAccessRead_PF( void *Buffer, size_t BlockSize, size_t DataNum, FILEACCESS *FileAccess )
{
	DWORD BytesRead ;

	// �X���b�h���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( FileAccess->PF.UseThread == TRUE )
	{
		// ����ȑO�̎w�߂��o�Ă����ꍇ�ׂ̈Ɏw�ߊ����C�x���g���V�O�i����ԂɂȂ�܂ő҂�
		WaitForSingleObject( FileAccess->PF.ThreadData.CompEvent, INFINITE ) ;

		// �X���b�h�Ƀt�@�C���ǂݍ��ݎw�߂��o��
		FileAccess->PF.ThreadData.Function = FILEACCESSTHREAD_FUNCTION_READ ;
		FileAccess->PF.ThreadData.ReadBuffer = Buffer ;
		FileAccess->PF.ThreadData.ReadPosition = FileAccess->Position ;
		FileAccess->PF.ThreadData.ReadSize = ( DWORD )( BlockSize * DataNum ) ;
		ResetEvent( FileAccess->PF.ThreadData.CompEvent ) ;
		SetEvent( FileAccess->PF.ThreadData.FuncEvent ) ;

		// �񓯊����ǂ����ŏ����𕪊�
		if( FileAccess->UseASyncReadFlag == FALSE )
		{
			// �����ǂݍ��݂̏ꍇ�͎w�߂���������܂ő҂�
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
	// �t�@�C�����̃R�s�[
	_WCSCPY_S( FileInfo->Name, sizeof( FileInfo->Name ), FindData->cFileName );

	// �f�B���N�g�����ǂ����̃t���O���Z�b�g
	FileInfo->DirFlag = (FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 ? TRUE : FALSE;

	// �t�@�C���̃T�C�Y���Z�b�g
	FileInfo->Size = ( LONGLONG )( ( ( ( ULONGLONG )FindData->nFileSizeHigh ) << 32 ) + FindData->nFileSizeLow ) ;

	// �t�@�C���^�C����ۑ�
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
	// �O�ȊO���Ԃ��Ă����琬��
	return FindClose( FindInfo->PF.FindHandle ) != 0 ? 0 : -1 ;
}





// �������ݐ�p�t�@�C���A�N�Z�X�֐�
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

