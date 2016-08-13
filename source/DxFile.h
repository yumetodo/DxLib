// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�t�@�C���A�N�Z�X�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXFILE_H__
#define __DXFILE_H__

// �C���N���[�h ------------------------------------------------------------------
#include "DxCompileConfig.h"
#include "DxLib.h"
#include "DxStatic.h"
#include "DxHandle.h"


// �}�N����` --------------------------------------------------------------------

// ���C�u���������Ŏg�p����t�@�C���A�N�Z�X�p�֐�
#define DX_FSYNC( handle )							{while( DX_FIDLECHK( handle ) == FALSE ) Thread_Sleep(0);}
#define DX_FOPEN( path )							StreamOpen( (path), FALSE, TRUE, FALSE )
#define DX_FOPENT( path )							StreamOpenT( (path), FALSE, TRUE, FALSE )
#define DX_FOPEN_CACHE( path )						StreamOpen( (path),  TRUE, TRUE, FALSE )
#define DX_FOPEN_ASYNC( path )						StreamOpen( (path), FALSE, TRUE,  TRUE )
#define DX_FCLOSE( handle )							StreamClose( (DWORD_PTR)(handle) )
#define DX_FREAD( buf, length, num, handle )		StreamRead( (void *)(buf), (size_t)(length), (size_t)(num), (DWORD_PTR)(handle) )
#define DX_FSEEK( handle, pos, mode )				StreamSeek( (DWORD_PTR)(handle), (LONGLONG)(pos), (int)(mode) )
#define DX_FTELL( handle )							StreamTell( (DWORD_PTR)(handle) )
#define DX_FEOF( handle )							StreamEof( (DWORD_PTR)(handle) )
#define DX_FIDLECHK( handle )						StreamIdleCheck( (DWORD_PTR)(handle) )
#define DX_FSETDIR( path )							StreamChDir( (path) )
#define DX_FGETDIR( buffer, bufferbytes )			StreamGetDirS( (buffer), (bufferbytes) )
#define DX_FFINDFIRST( path, buffer )				StreamFindFirst( (path), (buffer) )
#define DX_FFINDNEXT( handle, buffer )				StreamFindNext( (DWORD_PTR)(handle), (buffer) )
#define DX_FFINDCLOSE( handle )						StreamFindClose( (DWORD_PTR)(handle) )

// �t�@�C���A�N�Z�X��p�X���b�h�ւ̎w��
#define FILEACCESSTHREAD_FUNCTION_OPEN		(0)				// �t�@�C�����J��
#define FILEACCESSTHREAD_FUNCTION_CLOSE		(1)				// �t�@�C�������
#define FILEACCESSTHREAD_FUNCTION_READ		(2)				// �t�@�C����ǂݍ���
#define FILEACCESSTHREAD_FUNCTION_SEEK		(3)				// �t�@�C���|�C���^���ړ�����
#define FILEACCESSTHREAD_FUNCTION_EXIT		(4)				// �I������

#define FILEACCESSTHREAD_DEFAULT_CACHESIZE	(128 * 1024)	// �f�t�H���g�̃L���b�V���T�C�Y

// �n���h���^�C�v
#define FILEHANDLETYPE_NORMAL				(0)				// �ʏ�̃t�@�C���n���h��
#define FILEHANDLETYPE_FULLYLOAD			(1)				// �t�@�C���̓��e���������ɓǂݍ��񂾏����p�̃n���h��

// �t�@�C���p�X�̍ő咷
#define FILEPATH_MAX						(512 * 3)


// ���ˑ���`�w�b�_�t�@�C���̃C���N���[�h --------------------------------------

#ifdef __WINDOWS__
#include "Windows/DxFileWin.h"
#endif // __WINDOWS__

#ifdef __psp2__
#include "PSVita/DxFilePSVita.h"
#endif // __psp2__

#ifdef __ORBIS__
#include "PS4/DxFilePS4.h"
#endif // __ORBIS__

#ifdef __ANDROID__
#include "Android/DxFileAndroid.h"
#endif // __ANDROID__


#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �\���̒�` --------------------------------------------------------------------

// �t�@�C���A�N�Z�X�n���h�����
struct FILEACCESSINFO
{
	HANDLEINFO				HandleInfo ;						// �n���h�����ʃf�[�^
	int						HandleType ;						// �n���h���^�C�v( FILEHANDLETYPE_NORMAL �Ȃ� )

	STREAMDATA				StreamData ;						// �ǂݍ��ݏ����p�f�[�^

	// FILEHANDLETYPE_NORMAL �ł̂ݎg�p
	DWORD_PTR				FilePointer ;						// �t�@�C���|�C���^

	// FILEHANDLETYPE_FULLYLOAD �ł̂ݎg�p
	int						AllocFileImage ;					// �t�@�C����ǂݍ��ރ������̈���m�ۂ������ǂ���( TRUE:�m�ۂ���  FALSE:�m�ۂ��Ă��Ȃ� )
	void					*FileImage ;						// �ǂݍ��񂾃t�@�C�����i�[�����������̈�ւ̃|�C���^
	LONGLONG				FileSize ;							// �t�@�C���T�C�Y
} ;

// �t�@�C���A�N�Z�X�����p�\����
struct FILEACCESS
{
	int						EofFlag ;							// �I�[�`�F�b�N�t���O
	ULONGLONG				Position ;							// �A�N�Z�X�ʒu
	ULONGLONG				Size ;								// �T�C�Y
	int						UseASyncReadFlag ;					// �񓯊��ǂݍ��݂��s�����ǂ���

	FILEACCESS_PF			PF ;								// ���ˑ����
} ;

// �t�@�C�����������p�\����
struct FINDINFO
{
	FINDINFO_PF				PF ;								// ���ˑ����
} ;

// �������ϐ��錾 --------------------------------------------------------------

// �f�t�H���g�X�g���[���t�@���N�V����
extern STREAMDATASHREDTYPE2W StreamFunctionW ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �t�@�C���A�N�Z�X�����̏������E�I���֐�
extern	int			InitializeFile( void ) ;														// �t�@�C���A�N�Z�X�����̏�����
extern	int			TerminateFile( void ) ;															// �t�@�C���A�N�Z�X�����̌�n��

// �ǂݍ��ݐ�p�t�@�C���A�N�Z�X�֐�
extern	DWORD_PTR	ReadOnlyFileAccessOpen( const wchar_t *Path, int UseCacheFlag, int BlockReadFlag, int UseASyncReadFlag  ) ;
extern	int			ReadOnlyFileAccessClose( DWORD_PTR Handle ) ;
extern	LONGLONG	ReadOnlyFileAccessTell( DWORD_PTR Handle ) ;
extern	int			ReadOnlyFileAccessSeek( DWORD_PTR Handle, LONGLONG SeekPoint, int SeekType ) ;
extern	size_t		ReadOnlyFileAccessRead( void *Buffer, size_t BlockSize, size_t DataNum, DWORD_PTR Handle ) ;
extern	int			ReadOnlyFileAccessEof( DWORD_PTR Handle ) ;
extern	int			ReadOnlyFileAccessIdleCheck( DWORD_PTR Handle ) ;
extern	int			ReadOnlyFileAccessChDir( const wchar_t *Path ) ;
extern	int			ReadOnlyFileAccessGetDir( wchar_t *Buffer ) ;
extern	int			ReadOnlyFileAccessGetDirS( wchar_t *Buffer, size_t BufferBytes ) ;
extern	DWORD_PTR	ReadOnlyFileAccessFindFirst( const wchar_t *FilePath, FILEINFOW *Buffer ) ;				// �߂�l: -1=�G���[  -1�ȊO=FindHandle
extern	int			ReadOnlyFileAccessFindNext( DWORD_PTR FindHandle, FILEINFOW *Buffer ) ;					// �߂�l: -1=�G���[  0=����
extern	int			ReadOnlyFileAccessFindClose( DWORD_PTR FindHandle ) ;									// �߂�l: -1=�G���[  0=����

// �������ݐ�p�t�@�C���A�N�Z�X�֐�
extern	int			WriteOnlyFileAccessDelete( const wchar_t *Path ) ;
extern	DWORD_PTR	WriteOnlyFileAccessOpen( const wchar_t *Path ) ;
extern	DWORD_PTR	WriteOnlyFileAccessOpenWCHAR( const char *Path ) ; // Visual C++ 6.0 �p�Ɉ����� char �^�ɂ��������̂���
extern	int			WriteOnlyFileAccessClose( DWORD_PTR Handle ) ;
extern	int			WriteOnlyFileAccessSeek( DWORD_PTR Handle, LONGLONG SeekPoint, int SeekType ) ;
extern	int			WriteOnlyFileAccessWrite( DWORD_PTR Handle, void *Buffer, size_t WriteSize, size_t *GetWriteSize = NULL ) ;
extern	int			WriteOnlyFileAccessPrintf( DWORD_PTR Handle, const char *FormatString, ... ) ;

// �X�g���[���f�[�^�A�N�Z�X�֐�
extern	DWORD_PTR	StreamOpen(  const wchar_t *Path, int UseCacheFlag, int BlockFlag, int UseASyncReadFlag ) ;
extern	DWORD_PTR	StreamOpenT( const TCHAR   *Path, int UseCacheFlag, int BlockFlag, int UseASyncReadFlag ) ;
extern	int			StreamClose( DWORD_PTR Handle ) ;
extern	LONGLONG	StreamTell( DWORD_PTR Handle ) ;
extern	int			StreamSeek( DWORD_PTR Handle, LONGLONG SeekPoint, int SeekType ) ;
extern	size_t		StreamRead( void *Buffer, size_t BlockSize, size_t DataNum, DWORD_PTR Handle ) ;
extern	int			StreamEof( DWORD_PTR Handle ) ;
extern	int			StreamIdleCheck( DWORD_PTR Handle ) ;
extern	int			StreamChDir(  const wchar_t *Path ) ;
extern	int			StreamChDirT( const TCHAR   *Path ) ;
extern	int			StreamGetDirS(  wchar_t *Buffer, size_t BufferBytes ) ;
extern	int			StreamGetDirTS( TCHAR   *Buffer, size_t BufferBytes ) ;
extern	DWORD_PTR	StreamFindFirst(  const wchar_t *FilePath, FILEINFOW *Buffer ) ;		// �߂�l: -1=�G���[  -1�ȊO=FindHandle
extern	DWORD_PTR	StreamFindFirstT( const TCHAR   *FilePath, FILEINFO  *Buffer ) ;		// �߂�l: -1=�G���[  -1�ȊO=FindHandle
extern	int			StreamFindNext(  DWORD_PTR FindHandle, FILEINFOW *Buffer ) ;			// �߂�l: -1=�G���[  0=����
extern	int			StreamFindNextT( DWORD_PTR FindHandle, FILEINFO  *Buffer ) ;			// �߂�l: -1=�G���[  0=����
extern	int			StreamFindClose( DWORD_PTR FindHandle ) ;								// �߂�l: -1=�G���[  0=����
extern	const STREAMDATASHREDTYPE2W *StreamGetStruct( void ) ;

// �t�@�C������ wchar_t �ł� TCHAR �ł̃f�[�^�ϊ��֐�
extern	int			ConvFileIntoToFileInfoW( FILEINFO  *Src, FILEINFOW *Dest ) ;
extern	int			ConvFileIntoWToFileInfo( FILEINFOW *Src, FILEINFO  *Dest ) ;

// �X�g���[���f�[�^�A�N�Z�X�p�֐��\���̊֌W
extern	STREAMDATASHRED *GetFileStreamDataShredStruct( void ) ;								// �X�g���[���f�[�^�ǂ݂��ݐ���p�֐��|�C���^�\���̂̃t�@�C���p�\���̂𓾂�
extern	STREAMDATASHRED *GetMemStreamDataShredStruct( void ) ;								// �X�g���[���f�[�^�ǂ݂��ݐ���p�֐��|�C���^�\���̂���ؗp�\���̂𓾂�




// �t�@�C���n���h���֐�
extern	int			FileRead_open_UseGParam( const wchar_t *FilePath, int ASync, int ASyncLoadFlag = FALSE ) ;				// �t�@�C�����J��
extern	int			FileRead_seek_UseGParam( int FileHandle, LONGLONG Offset, int Origin, int ASyncLoadFlag = FALSE ) ;		// �t�@�C���|�C���^�̈ʒu��ύX����
extern	int			FileRead_read_UseGParam( int FileHandle, void *Buffer, int ReadSize, int ASyncLoadFlag = FALSE ) ;		// �t�@�C������f�[�^��ǂݍ���
extern	int			FileRead_fullyLoad_UseGParam( const wchar_t *FilePath, int ASyncLoadFlag = FALSE ) ;					// �w��̃t�@�C���̓��e��S�ă������ɓǂݍ��݁A���̏��̃A�N�Z�X�ɕK�v�ȃn���h����Ԃ�( �߂�l  -1:�G���[  -1�ȊO:�n���h�� )�A�g���I�������n���h���� FileRead_fullyLoad_delete �ō폜����K�v������܂�
extern	int			FileRead_scanf_base(        int         FileHandle, const void  *Format,        va_list Param ) ;	// �t�@�C�����珑�������ꂽ�f�[�^��ǂݏo��
extern	int			FileRead_scanf_baseCHAR(    STREAMDATA *StreamData, const char  *Format,        va_list Param ) ;	// �t�@�C�����珑�������ꂽ�f�[�^��ǂݏo��
extern	int			FileRead_scanf_baseUTF16LE( STREAMDATA *StreamData, const WORD  *FormatUTF16LE, va_list Param ) ;	// �t�@�C�����珑�������ꂽ�f�[�^��ǂݏo��



// �������ɒu���ꂽ�f�[�^���t�@�C���̃f�[�^�ɗႦ�Ă������߂̊֐�
extern	void*		MemStreamOpen( const void *DataBuffer, size_t DataSize ) ;
extern	int			MemStreamClose( void *StreamDataPoint ) ;



// �t�@�C���p�X�֌W

// �t���p�X�ł͂Ȃ��p�X��������t���p�X�ɕϊ�����
// ( CurrentDir �̓t���p�X�ł���K�v������(����Ɂw\�x�������Ă������Ă��ǂ�) )
// ( CurrentDir �� NULL �̏ꍇ�͌��݂̃J�����g�f�B���N�g�����g�p���� )
//extern int			ConvertFullPath_( const char *Src, char *Dest, const char *CurrentDir = NULL ) ; 
extern int			ConvertFullPathW_( const wchar_t *Src, wchar_t *Dest, size_t BufferBytes, const wchar_t *CurrentDir = NULL ) ; 
extern int			ConvertFullPathT_( const TCHAR   *Src, TCHAR   *Dest, size_t BufferBytes, const TCHAR   *CurrentDir = NULL ) ; 

// �w��̃t�@�C���p�X���w��̃t�H���_�p�X���瑊�΃A�N�Z�X���邽�߂̑��΃p�X���쐬����
// ( FilePath �� StartFolderPath ���t���p�X�ł͂Ȃ������ꍇ�͊֐����Ńt���p�X������܂� )
// StartFolderPath �̖��[�� / or \ �������Ă���肠��܂���
//extern int			CreateRelativePath_( const char *FilePath, const char *StartFolderPath, char *Dest ) ;
extern int			CreateRelativePathW_( const wchar_t *FilePath, const wchar_t *StartFolderPath, wchar_t *Dest, size_t BufferBytes ) ;
extern int			CreateRelativePathT_( const TCHAR   *FilePath, const TCHAR   *StartFolderPath, TCHAR   *Dest, size_t BufferBytes ) ;

// ����̃p�X�����񂩂�F�X�ȏ����擾����
// ( CurrentDir �̓t���p�X�ł���K�v������(����Ɂw\�x�������Ă������Ă��ǂ�) )
// ( CurrentDir �� 0 �̏ꍇ�͎��ۂ̃J�����g�f�B���N�g�����g�p���� )
// DirPath �̏I�[�ɂ� \ �͕t���Ȃ�
extern int AnalyseFilePathW_(
	const wchar_t *Src,
	wchar_t *FullPath,	size_t FullPathBytes,
	wchar_t *DirPath,	size_t DirPathBytes,
	wchar_t *FileName,	size_t FileNameBytes,
	wchar_t *Name,		size_t NameBytes,
	wchar_t *ExeName,	size_t ExeNameBytes,
	const wchar_t *CurrentDir = 0
) ;
extern int AnalyseFilePathT_(
	const TCHAR *Src,
	TCHAR   *FullPath,	size_t FullPathBytes,
	TCHAR   *DirPath,	size_t DirPathBytes,
	TCHAR   *FileName,	size_t FileNameBytes,
	TCHAR   *Name,		size_t NameBytes,
	TCHAR   *ExeName,	size_t ExeNameBytes,
	const TCHAR *CurrentDir = 0
) ;

// �t�@�C�������ꏏ�ɂȂ��Ă���ƕ������Ă���p�X������t�@�C�����ƃf�B���N�g���p�X�𕪊�����
// �t���p�X�ł���K�v�͖����A�t�@�C���������ł��ǂ�
// DirPath �̏I�[�� �� �}�[�N�͕t���Ȃ�
//extern int		AnalysisFileNameAndDirPath_( const char *Src, char *FileName = 0, char *DirPath = 0 ) ;
extern int			AnalysisFileNameAndDirPathW_( const wchar_t *Src, wchar_t *FileName, size_t FileNameBytes, wchar_t *DirPath, size_t DirPathBytes ) ;
extern int			AnalysisFileNameAndDirPathT_( const TCHAR   *Src, TCHAR   *FileName, size_t FileNameBytes, TCHAR   *DirPath, size_t DirPathBytes ) ;

// �t�@�C���p�X����t�@�C�����Ɗg���q���擾����
//extern int		AnalysisFileNameAndExeName_( const char *Src, char *Name = 0, char *ExeName = 0 ) ;
extern int			AnalysisFileNameAndExeNameW_( const wchar_t *Src, wchar_t *Name, size_t NameBytes, wchar_t *ExeName, size_t ExeNameBytes ) ;
extern int			AnalysisFileNameAndExeNameT_( const TCHAR   *Src, TCHAR   *Name, size_t NameBytes, TCHAR   *ExeName, size_t ExeNameBytes ) ;

// �t�@�C���p�X�̊g���q��ς���������𓾂�
//extern int		GetChangeExeNamePath_( const char *Src, char *Dest, const char *ExeName ) ;

extern void			SetEnMarkT_( TCHAR   *PathBuf, size_t BufferBytes ) ;	// ����Ɂw\�x�����Ă��Ȃ��ꍇ�͕t����
extern void			SetEnMarkW_( wchar_t *PathBuf, size_t BufferBytes ) ;	// ����Ɂw\�x�����Ă��Ȃ��ꍇ�͕t����

extern void			ChangeEnMarkToSlashT_( TCHAR   *PathBuf ) ;				// �p�X�����񒆂́w\�x���w/�x�ɕϊ�����
extern void			ChangeEnMarkToSlashW_( wchar_t *PathBuf ) ;				// �p�X�����񒆂́w\�x���w/�x�ɕϊ�����

// �n���ꂽ��������t���p�X������Ƃ��Ĉ����A�h���C�u��( :\ or :/ �̑O�܂� )
// ���̓l�b�g���[�N�t�H���_��( \ or / �̑O�܂� )���擾����
// �l�b�g���[�N�t�H���_�������ꍇ�͍ŏ��� \\ ���܂߂�
// �߂�l�͎擾����������̒���( �l�b�g���[�N�t�H���_�̏ꍇ�� \\ ��������ʂɊ܂܂�܂� )
// Src �̓t���p�X�ł���K�v������܂��A���΃p�X�ł͐���ɓ��삵�܂���
//extern int		AnalysisDriveName_( const char *Src, char *Dest ) ;
extern int			AnalysisDriveNameW_( const wchar_t *Src, wchar_t *Dest, size_t BufferBytes ) ;

// �n���ꂽ��������t�H���_�p�X������Ƃ��Ĉ����A�t�H���_��( \ or / �̑O�܂� )���擾���܂�
// �n�������񂪃t���p�X�ŁA�ŏ��Ƀh���C�u����������Ă����琳��Ȍ��ʂ������܂���
// ../ ���̉��ʃt�H���_�ɍ~��镶���񂪂������ꍇ�� .. �����o�͂���܂�
// �߂�l�͎擾����������̒����ł�
//extern int		AnalysisDirectoryName_( const char *Src, char *Dest ) ;
extern int			AnalysisDirectoryNameW_( const wchar_t *Src, wchar_t *Dest, size_t BufferBytes ) ;





// ���ˑ������p�֐�

// �t�@�C���A�N�Z�X�����̏������E�I���֐�
extern	int			InitializeFile_PF( void ) ;														// �t�@�C���A�N�Z�X�����̏������֐��̊��ˑ��̏������s���֐�
extern	int			TerminateFile_PF( void ) ;														// �t�@�C���A�N�Z�X�����̌�n���֐��̊��ˑ��̏������s���֐�

// �t�@�C���A�N�Z�X�֐�
extern	int			ReadOnlyFileAccessOpen_PF(		FILEACCESS *FileAccess, const wchar_t *Path, int UseCacheFlag, int BlockReadFlag ) ;
extern	int			ReadOnlyFileAccessClose_PF(		FILEACCESS *FileAccess ) ;
extern	LONGLONG	ReadOnlyFileAccessTell_PF(		FILEACCESS *FileAccess ) ;
extern	int			ReadOnlyFileAccessSeek_PF(		FILEACCESS *FileAccess, LONGLONG SeekPoint ) ;
extern	size_t		ReadOnlyFileAccessRead_PF(		void *Buffer, size_t BlockSize, size_t DataNum, FILEACCESS *FileAccess ) ;
extern	int			ReadOnlyFileAccessIdleCheck_PF(	FILEACCESS *FileAccess ) ;
extern	int			ReadOnlyFileAccessChDir_PF(		const wchar_t *Path ) ;
extern	int			ReadOnlyFileAccessGetDirS_PF(	wchar_t *Buffer, size_t BufferBytes ) ;
extern	int			ReadOnlyFileAccessFindFirst_PF(	FINDINFO *FindInfo, const wchar_t *FilePath, FILEINFOW *Buffer ) ;
extern	int			ReadOnlyFileAccessFindNext_PF(	FINDINFO *FindInfo, FILEINFOW *Buffer ) ;
extern	int			ReadOnlyFileAccessFindClose_PF(	FINDINFO *FindInfo ) ;

// �������ݐ�p�t�@�C���A�N�Z�X�֐�
extern	int			WriteOnlyFileAccessDelete_PF( const wchar_t *Path ) ;
extern	DWORD_PTR	WriteOnlyFileAccessOpen_PF(  const wchar_t *Path ) ;
extern	int			WriteOnlyFileAccessClose_PF( DWORD_PTR Handle ) ;
extern	int			WriteOnlyFileAccessSeek_PF(  DWORD_PTR Handle, LONGLONG SeekPoint, int SeekType ) ;
extern	int			WriteOnlyFileAccessWrite_PF( DWORD_PTR Handle, void *Buffer, size_t WriteSize, size_t *GetWriteSize = NULL ) ;


#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXFILE_H__

