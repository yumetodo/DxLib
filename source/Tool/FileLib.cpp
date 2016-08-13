// ============================================================================
//
//		�t�@�C���֌W�̃��C�u����
//
//		Creator			: �R�c�@�I
//		Creation Data	: 09/17/2004
//
// ============================================================================

// include --------------------------------------
#include <string.h>
#include <stdio.h>
#include <mbstring.h>
#include <windows.h>
#include "FileLib.h"

// define ---------------------------------------

#define PATH_LENGTH		(256)			// �p�X�o�b�t�@�̒���
#define TEXTCHECKSIZE	(0x2000)		// �e�L�X�g�f�[�^�����ׂ�T�C�Y

// data type ------------------------------------

// data -----------------------------------------

// function proto type --------------------------

// �t�@�C���I�u�W�F�N�g�񋓗p�֐�( -1:�G���[  0�ȏ�:�t�@�C���̐� )
// flist �� NULL �ł��ǂ�
static int __EnumObject( char *Path, char *CurrentPath, FILE_INFOLIST *FileList,
							int OmitDirectory, int SubDirectory,
							char **OmitName, char **OmitExName, char **ValidExName ) ;

// function code --------------------------------

// �t�@�C���I�u�W�F�N�g�񋓗p�֐�( -1:�G���[  0�ȏ�:�t�@�C���̐� )
// flist �� NULL �ł��ǂ�
static int __EnumObject( char *Path, char *CurrentPath, FILE_INFOLIST *FileList,
							int OmitDirectory, int SubDirectory,
							char **OmitName, char **OmitExName, char **ValidExName )
{
	WIN32_FIND_DATA FindData ;
	HANDLE FindHandle = INVALID_HANDLE_VALUE ;
	int FileNum, IsDirectory ;
	char RelDir[PATH_LENGTH] ;
	char *AbsDir ;
	int RelDirLen, AbsDirLen, StartNum ;

	// �f�B���N�g���p�X�̍쐬
	AbsDir = Path ;
	AbsDirLen = strlen( AbsDir ) ;
	strcpy( RelDir, AbsDir + strlen( CurrentPath ) ) ;
	RelDirLen = strlen( RelDir ) ;

	// �t�@�C���̗񋓊J�n
	{
		char temp[PATH_LENGTH] ;

		strcpy( temp, AbsDir ) ;
		strcat( temp, "*" ) ;
		FindHandle = FindFirstFile( temp, &FindData ) ;
		if( FindHandle == INVALID_HANDLE_VALUE )
			return -1 ;
	}

	// �t�@�C���̏���񋓂���
	FileNum = 0 ;
	if( FileList != NULL ) StartNum = FileList->Num ;
	do
	{
		// ��̃t�H���_�ɖ߂����肷�邽�߂̃p�X�͖�������
		if( strcmp( FindData.cFileName, "." ) == 0 || strcmp( FindData.cFileName, ".." ) == 0 ) continue ;

		// �f�B���N�g�����ǂ����𓾂�
		IsDirectory = ( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ? 1 : 0 ;

		// �L���g���q�w�肪����A�L���g���q�ł͂Ȃ��ꍇ�͖�������
		if( ValidExName != NULL && IsDirectory == 0 )
		{
			int i ;
			char *name ;

			name = strchr( FindData.cFileName, '.' ) ;
			if( name == NULL ) name = "" ;
			else name ++ ;

			for( i = 0 ; ValidExName[i] != NULL && stricmp( name, ValidExName[i] ) != 0 ; i ++ ){}
			if( ValidExName[i] == NULL ) continue ;
		}

		// ���O�w�肪���镶����͖�������
		if( OmitName != NULL )
		{
			int i ;

			for( i = 0 ; OmitName[i] != NULL && strcmp( FindData.cFileName, OmitName[i] ) != 0 ; i ++ ){}
			if( OmitName[i] != NULL ) continue ;
		}

		// ���O�w�肪����g���q�����t�@�C���͖�������
		if( OmitExName != NULL && IsDirectory == 0 )
		{
			int i ;
			char *name ;

			name = strchr( FindData.cFileName, '.' ) ;
			if( name == NULL ) name = "" ;
			else name ++ ;

			for( i = 0 ; OmitExName[i] != NULL && strcmp( name, OmitExName[i] ) != 0 ; i ++ ){}
			if( OmitExName[i] != NULL ) continue ;
		}

		// �f�B���N�g���̏ꍇ�ŃT�u�f�B���N�g�������ׂ�w�肪����ꍇ�̓T�u�f�B���N�g�������񋓂���
		if( IsDirectory == 1 && SubDirectory == 1 )
		{
			int res ;
			char tempAbs[PATH_LENGTH], tempRel[PATH_LENGTH] ;

			// ��΃p�X�̍쐬
			strcpy( tempAbs, AbsDir ) ;
			strcat( tempAbs, FindData.cFileName ) ;
			strcat( tempAbs, "\\" ) ;

			// ���΃p�X�̍쐬
			strcpy( tempRel, RelDir ) ;
			strcat( tempRel, FindData.cFileName ) ;
			strcat( tempRel, "\\" ) ;
			
			// ��
			res = __EnumObject( tempAbs, CurrentPath, FileList, OmitDirectory, SubDirectory, OmitName, OmitExName, ValidExName ) ;
			if( res < 0 )
				goto ERR ;
			FileNum += res ;
		}

		// �f�[�^���i�[���邱�Ƃ��o����ꍇ�̓f�[�^���i�[����
		if( FileList != NULL &&
			( IsDirectory == 0 || ( IsDirectory == 1 && OmitDirectory == 0 ) ) )
		{
			FILE_INFO *info ;
			int FileNameLen ;

			info = &FileList->List[FileList->Num] ;
			memset( info, 0, sizeof( FILE_INFO ) ) ;

			// ������ۑ�
			info->Date.Create		= ( ( ( LONGLONG )FindData.ftCreationTime	.dwHighDateTime ) << 32 ) + FindData.ftCreationTime		.dwLowDateTime ;
			info->Date.LastAccess	= ( ( ( LONGLONG )FindData.ftLastAccessTime	.dwHighDateTime ) << 32 ) + FindData.ftLastAccessTime	.dwLowDateTime ;
			info->Date.LastWrite	= ( ( ( LONGLONG )FindData.ftLastWriteTime	.dwHighDateTime ) << 32 ) + FindData.ftLastWriteTime	.dwLowDateTime ;

			info->Size			= FindData.nFileSizeLow ;		// �T�C�Y��ۑ�
			info->Attributes	= FindData.dwFileAttributes ;	// ������ۑ�
			info->IsDirectory	= (u8)IsDirectory ;

			// �p�X�n�̕ۑ�
			{
				// �p�X�n��ۑ����郁�����̈�̊m��
				FileNameLen = strlen( FindData.cFileName ) ;
				info->FileName = ( char * )malloc( ( FileNameLen + 1 ) + ( AbsDirLen + 1 ) + ( RelDirLen + 1 ) ) ;
				if( info->FileName == NULL )
					goto ERR ;
				info->RelDirectoryPath = info->FileName + FileNameLen + 1 ;
				info->AbsDirectoryPath = info->RelDirectoryPath + RelDirLen + 1 ;

				// �R�s�[
				strcpy( info->FileName, FindData.cFileName ) ;
				strcpy( info->RelDirectoryPath, RelDir ) ;
				strcpy( info->AbsDirectoryPath, AbsDir ) ;
			}

			// �t�@�C���̐��𑝂₷
			FileList->Num ++ ;
		}

		// �t�@�C���̐��𑝂₷
		FileNum ++ ;
	}
	while( FindNextFile( FindHandle, &FindData ) != 0 ) ;

	// �񋓏I��
	FindClose( FindHandle ) ;
	FindHandle = INVALID_HANDLE_VALUE ;

	// �I��
	return FileNum ;

	// �G���[����
ERR :
	if( FindHandle != INVALID_HANDLE_VALUE ) FindClose( FindHandle ) ;

	// ���Ɋm�ۂ��Ă��܂����������̉������
	if( FileList != NULL )
	{
		int i ;

		for( i = StartNum ; i < FileList->Num ; i ++ )
			if( FileList->List[i].FileName != NULL ) free( FileList->List[i].FileName ) ;
	}

	// �G���[�I��
	return -1 ;
}

// �f�[�^���ȈՈÍ�������֐�
extern void EasyEncode( void *Data, unsigned int Size )
{
	if( Size == 0 ) return ;

#ifdef _WIN64
	unsigned int i ;
	BYTE *p = ( BYTE * )Data ;
	for( i = 0 ; i < Size ; i ++ )
	{
		p[ i ] = ( ~p[ i ] << 4 ) | ( ~p[ i ] >> 4 ) ;
	}
#else
	unsigned int tempSize, tempData ;

	tempData = ( unsigned int )Data ;
	tempSize = ( unsigned int )Size ;
	__asm
	{
		// ����
		mov edi, tempData
		mov ecx, tempSize

		// �l�𔽓]�A��]4�r�b�g���ď����߂�
LOOP1:
		mov al, [edi]
		not al
		rol al, 4
		mov [edi], al
		inc edi

		// ���[�v����
		dec ecx
		jnz LOOP1
	} ;
#endif
}

// �f�[�^���ȈՈÍ������ăt�@�C���ɏ����o���֐�
extern void EasyEncodeFileWrite( void *Data, int Size, FILE *fp )
{
	EasyEncode( Data, Size ) ;
	fwrite( Data, Size, 1, fp ) ;
	EasyEncode( Data, Size ) ;
}

// �f�[�^���ȈՈÍ������ăt�@�C������ǂݍ��ފ֐�
extern void EasyEncodeFileRead( void *Data, int Size, FILE *fp )
{
	fread( Data, Size, 1, fp ) ;
	EasyEncode( Data, Size ) ;
}

// �t�@�C���̓��e���������ɓǂݍ���( 0:����  -1:���s )
extern int LoadFileMem( const char *Path, void **DataBuf, int *Size )
{
	FILE *fp = NULL ;
	void *buf = NULL ;
	int size ;

	fp = fopen( Path, "rb" ) ;
	if( fp == NULL ) goto ERR ;

	// �T�C�Y�𓾂�
	fseek( fp, 0L, SEEK_END ) ;
	size = ftell( fp ) ;
	fseek( fp, 0L, SEEK_SET ) ;

	// �������̊m��
	buf = malloc( size ) ;
	if( buf == NULL ) goto ERR ;

	// �ǂݍ���
	fread( buf, size, 1, fp ) ;
	fclose( fp ) ;

	// �Z�b�g
	if( DataBuf != NULL ) *DataBuf = buf ;
	if( Size != NULL ) *Size = size ;

	// �I��
	return 0 ;

ERR :
	if( buf != NULL ) free( buf ) ;
	if( fp != NULL ) fclose( fp ) ;

	return -1 ;
}

// �t�@�C���̓��e���������ɓǂݍ���( 0:����  -1:���s )
extern int LoadFileMem( const char *Path, void *DataBuf, int *Size )
{
	FILE *fp = NULL ;
	int size ;

	fp = fopen( Path, "rb" ) ;
	if( fp == NULL ) return -1;

	// �T�C�Y�𓾂�
	fseek( fp, 0L, SEEK_END ) ;
	size = ftell( fp ) ;
	fseek( fp, 0L, SEEK_SET ) ;

	// �ǂݍ���
	if( DataBuf )
		fread( DataBuf, size, 1, fp ) ;
	fclose( fp ) ;

	// �Z�b�g
	if( Size != NULL ) *Size = size ;

	// �I��
	return 0 ;
}

// �������̓��e���t�@�C���ɏ����o�� 
extern int SaveFileMem( const char *Path, void *Data, int Size )
{
	FILE *fp ;

	fp = fopen( Path, "wb" ) ;
	if( fp == NULL ) return -1 ;
	fwrite( Data, Size, 1, fp ) ;
	fclose( fp ) ;

	return 0 ;
}


// �w��̃f�B���N�g�����쐬����A���Ԃ̃f�B���N�g�������݂��Ȃ��ꍇ�͍쐬����
// �Ō���� '\' �������Ă���������
// �h���C�u���̌�� '\' ���Ȃ��ꍇ�͐���ɓ��삵�Ȃ�
extern int __CreateDirectory( const char *Path )
{
	char dir[MAX_PATH] ;

	// �t���p�X�𓾂�
	ConvertFullPath__( Path, dir, NULL ) ;
	SetEnMark( dir ) ;

	// �w��̃f�B���N�g�������݂����牽�������I��
	{
		WIN32_FIND_DATA FindData ;
		HANDLE FindHandle ;

		FindHandle = FindFirstFile( dir, &FindData ) ;
		if( FindHandle != INVALID_HANDLE_VALUE )
		{
			FindClose( FindHandle ) ;
			return 0 ;
		}
	}

	// �f�B���N�g�����쐬���郋�[�v
	{
		char *p ;
		p = strchr( dir, '\\' ) ;
		while( p != NULL )
		{
			*p = '\0' ;
			CreateDirectory( dir, NULL ) ;
			*p = '\\' ;

			p = strchr( p + 1, '\\' ) ;
		}
	}

	// �I��
	return 0 ;
}


// �w��̃p�X�������Ă�����̂��f�B���N�g�����ǂ����𓾂�
extern int IsDirectory( const char *Path )
{
	WIN32_FIND_DATA FindData ;
	HANDLE FindHandle ;
	
	// �t�@�C���̏��𓾂�
	FindHandle = FindFirstFile( Path, &FindData ) ;
	if( FindHandle == INVALID_HANDLE_VALUE ) return -1 ;
	FindClose( FindHandle ) ;
	
	// �f�B���N�g�����ǂ�����Ԃ�
	return ( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ? 1 : 0 ;
}

// �w��̃p�X�̏��𓾂�
extern int CreateFileInfo( const char *Path, FILE_INFO *FileInfoBuffer )
{
	FILE_INFO *info ;
	WIN32_FIND_DATA FindData ;
	HANDLE FindHandle = INVALID_HANDLE_VALUE ;
	int AbsDirLen, RelDirLen ;
	char RelDir[1] ;
	char AbsDir[PATH_LENGTH] ;
	
	info = FileInfoBuffer ;
	
	// �f�B���N�g���p�X�̍쐬
	{
		ConvertFullPath__( Path, AbsDir ) ;
		DelChr( AbsDir, '\\' ) ;
		strrchr( AbsDir, '\\' )[1] = '\0' ;
		AbsDirLen = strlen( AbsDir ) ;
		RelDir[0] = '\0' ;
		RelDirLen = 0 ;
	}

	// �t�@�C���̏��𓾂�
	FindHandle = FindFirstFile( Path, &FindData ) ;
	if( FindHandle == INVALID_HANDLE_VALUE ) return -1 ;
	FindClose( FindHandle ) ;
	
	// �Ƃ肠�����돉����
	memset( info, 0, sizeof( FILE_INFO ) ) ;

	// ������ۑ�
	info->Date.Create		= ( ( ( LONGLONG )FindData.ftCreationTime	.dwHighDateTime ) << 32 ) + FindData.ftCreationTime		.dwLowDateTime ;
	info->Date.LastAccess	= ( ( ( LONGLONG )FindData.ftLastAccessTime	.dwHighDateTime ) << 32 ) + FindData.ftLastAccessTime	.dwLowDateTime ;
	info->Date.LastWrite	= ( ( ( LONGLONG )FindData.ftLastWriteTime	.dwHighDateTime ) << 32 ) + FindData.ftLastWriteTime	.dwLowDateTime ;

	info->Size			= FindData.nFileSizeLow ;		// �T�C�Y��ۑ�
	info->Attributes	= FindData.dwFileAttributes ;	// ������ۑ�
	info->IsDirectory	= (u8)(( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ? 1 : 0) ;	// �f�B���N�g�����ǂ�����ۑ�

	// �p�X�n�̕ۑ�
	{
		int FileNameLen ;
	
		// �p�X�n��ۑ����郁�����̈�̊m��
		FileNameLen = strlen( FindData.cFileName ) ;
		info->FileName = ( char * )malloc( ( FileNameLen + 1 ) + ( AbsDirLen + 1 ) + ( RelDirLen + 1 ) ) ;
		if( info->FileName == NULL ) return -1 ;
		info->RelDirectoryPath = info->FileName + FileNameLen + 1 ;
		info->AbsDirectoryPath = info->RelDirectoryPath + RelDirLen + 1 ;

		// �R�s�[
		strcpy( info->FileName, FindData.cFileName ) ;
		strcpy( info->RelDirectoryPath, RelDir ) ;
		strcpy( info->AbsDirectoryPath, AbsDir ) ;
	}
	
	// �I��
	return 0 ;
}

// �p�X���̌�n�����s��
extern int ReleaseFileInfo( FILE_INFO *FileInfo )
{
	if( FileInfo->FileName != NULL ) free( FileInfo->FileName ) ;
	FileInfo->FileName = NULL ;
	
	// �I��
	return 0 ;
}

// �w��̃p�X�̃t�@�C���̃^�C���X�^���v�� FileInfo �Ɋi�[����Ă���^�C���X�^���v�ɂ���
extern int SetFileTimeStamp( const char *Path, FILE_INFO *FileInfo )
{
	HANDLE HFile ;
	FILETIME CreateTime, LastAccessTime, LastWriteTime ;

	HFile = CreateFile( Path,
						GENERIC_WRITE, 0, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) ;
	if( HFile == INVALID_HANDLE_VALUE )
	{
		HFile = HFile ;
	}

	CreateTime.dwHighDateTime     = ( u32 )( FileInfo->Date.Create     >> 32        ) ;
	CreateTime.dwLowDateTime      = ( u32 )( FileInfo->Date.Create     & 0xffffffff ) ;
	LastAccessTime.dwHighDateTime = ( u32 )( FileInfo->Date.LastAccess >> 32        ) ;
	LastAccessTime.dwLowDateTime  = ( u32 )( FileInfo->Date.LastAccess & 0xffffffff ) ;
	LastWriteTime.dwHighDateTime  = ( u32 )( FileInfo->Date.LastWrite  >> 32        ) ;
	LastWriteTime.dwLowDateTime   = ( u32 )( FileInfo->Date.LastWrite  & 0xffffffff ) ;
	SetFileTime( HFile, &CreateTime, &LastAccessTime, &LastWriteTime ) ;
	CloseHandle( HFile ) ;

	// �I��
	return 0;
}

// ��̃t�@�C���̃^�C���X�^���v����v���Ă��邩�ǂ����𓾂�(0:��v���Ă��� -1:��v���Ă��Ȃ�)
extern int CmpFileTimeStamp( FILE_INFO *FileInfo1, FILE_INFO *FileInfo2, bool Create, bool LastAccess, bool LastWrite )
{
	if( Create     && FileInfo1->Date.Create     != FileInfo2->Date.Create     ) return -1;
	if( LastAccess && FileInfo1->Date.LastAccess != FileInfo2->Date.LastAccess ) return -1;
	if( LastWrite  && FileInfo1->Date.LastWrite  != FileInfo2->Date.LastWrite  ) return -1;
	return 0;
}

// �w��̃f�B���N�g���̃t�@�C�����X�g���擾����
// �p�X������̍Ō�Ɂw\�x�������Ă���ɍŌ�̕�������f�B���N�g���Ɣ��f����
// �t���p�X�ł͂Ȃ��ꍇ�͌��݂̃J�����g�f�B���N�g������̑��΃p�X�ƂȂ�
// FileListInfo �� 0 �ł͂Ȃ��ꍇ�g�p�� ReleaseFileObjectList ���ĂԕK�v������
// SubDirectory �� 1 �ɂ���ƃT�u�f�B���N�g�����̃t�@�C�����S�ė񋓂���
// NULL �A�������͕���������Ȃ��������n�����ꍇ�J�����g�f�B���N�g���̃t�@�C�����񋓂����
// OmitDirectory �� 1 �ɂ���ƃf�B���N�g���͗񋓂��Ȃ�
// OmitName �ɏ��O�������t�@�C������n���Ƃ��̕���������O���Ă����( ';'�ŕ����̕������A���\ )
// OmitExName �ɏ��O�������g���q��n���Ƃ��̊g���q�����t�@�C�������O���Ă����( ';'�ŕ����̕������A���\ )
// ValidExName �ɗL���ɂ������g���q��n���Ƃ��̊g���q�����t�@�C���̂ݗ񋓂��Ă����( Omit�n�����D��x������, ';'�ŕ����̕������A���\ )
// �߂�l : -1 = �G���[  0�ȏ� = �񋓂����t�@�C���̐�
extern int CreateFileList( const char *DirectoryPath, FILE_INFOLIST *FileListInfo,
							int OmitDirectory, int SubDirectory,
							const char *OmitName, const char *OmitExName, const char *ValidExName )
{
	char DirPath[PATH_LENGTH] ;
	FILE_INFOLIST FileList ;
	char *OmitStrBuf = NULL, *OmitExStrBuf = NULL, *ValidExStrBuf = NULL ;
	char *OmitStr[50], *OmitExStr[50], *ValidExStr[100] ;
	
	// �t���p�X�𓾂�
	ConvertFullPath__( DirectoryPath, DirPath, NULL ) ;
	
	// �f�B���N�g���ł͂Ȃ��t�@�C���������獟���ŏ������Ă��܂�
	{
		int res ;
		
		DelChr( DirPath, '\\' ) ;
		
		res = IsDirectory( DirPath ) ;
		if( res == -1 ) return -1 ;
		
		if( IsDirectory( DirPath ) == 0 )
		{
			FileListInfo->Num = 1 ;
			
			FileListInfo->List = ( FILE_INFO * )malloc( sizeof( FILE_INFO ) ) ;
			CreateFileInfo( DirectoryPath, FileListInfo->List ) ;
			
			return 1 ;
		}

		SetEnMark( DirPath ) ;
	}

	// ������
	FileList.Num = 0 ;
	FileList.List = NULL ;
	OmitStrBuf = ( char * )malloc( 1024 ) ;
	if( OmitStrBuf == NULL ) goto ERR ;
	OmitExStrBuf = ( char * )malloc( 1024 ) ;
	if( OmitExStrBuf == NULL ) goto ERR ;
	ValidExStrBuf = ( char * )malloc( 1024 ) ;
	if( ValidExStrBuf == NULL ) goto ERR ;

	// ���O�����񃊃X�g�̍쐬
	if( OmitName != NULL )
	{
		int i ;
		char *p ;

		strcpy( OmitStrBuf, OmitName ) ;
		i = 0 ;
		p = OmitStrBuf ;
		OmitStr[i] = p ;
		i ++ ;
		while( ( p = strchr( p, ';' ) ) != NULL )
		{
			*p = '\0' ;
			p ++ ;
			OmitStr[i] = p ;
			i ++ ;
		}
		OmitStr[i] = NULL ;
	}
	else OmitStr[0] = NULL ;

	// ���O�g���q���X�g�̍쐬
	if( OmitExName != NULL )
	{
		int i ;
		char *p ;

		strcpy( OmitExStrBuf, OmitExName ) ;
		i = 0 ;
		p = OmitExStrBuf ;
		OmitExStr[i] = p ;
		i ++ ;
		while( ( p = strchr( p, ';' ) ) != NULL )
		{
			*p = '\0' ;
			p ++ ;
			OmitExStr[i] = p ;
			i ++ ;
		}
		OmitExStr[i] = NULL ;
	}
	else OmitExStr[0] = NULL ;

	// �L���g���q���X�g�̍쐬
	if( ValidExName != NULL )
	{
		int i ;
		char *p ;

		strcpy( ValidExStrBuf, ValidExName ) ;
		i = 0 ;
		p = ValidExStrBuf ;
		ValidExStr[i] = p ;
		i ++ ;
		while( ( p = strchr( p, ';' ) ) != NULL )
		{
			*p = '\0' ;
			p ++ ;
			ValidExStr[i] = p ;
			i ++ ;
		}
		ValidExStr[i] = NULL ;
	}
	else ValidExStr[0] = NULL ;

	// �񋓗p�֐��ɓ����ăt�@�C���̐��𓾂�
	FileList.Num = __EnumObject( DirPath, DirPath, NULL, OmitDirectory, SubDirectory, OmitStr, OmitExStr, ValidExStr[0] != NULL ? ValidExStr : NULL ) ;
	if( FileList.Num < 0 )
		goto ERR ;

	// �t�@�C�����X�g�̃f�[�^���i�[�ł��郁�����̈���m��
	FileList.List = ( FILE_INFO * )malloc( FileList.Num * sizeof( FILE_INFO ) ) ;
	if( FileList.List == NULL )
		goto ERR ;
	memset( FileList.List, 0, FileList.Num * sizeof( FILE_INFO ) ) ;

	// �t�@�C�����X�g�Ƀf�[�^���i�[����
	FileList.Num = 0 ;
	if( __EnumObject( DirPath, DirPath, &FileList, OmitDirectory, SubDirectory, OmitStr, OmitExStr, ValidExStr[0] != NULL ? ValidExStr : NULL ) < 0 )
		goto ERR ;

	// �f�[�^���i�[
	if( FileListInfo != NULL ) *FileListInfo = FileList ;

	// ���O������o�b�t�@���
	free( OmitStrBuf ) ;

	// ���O�g���q�o�b�t�@���
	free( OmitExStrBuf ) ;

	// �L���g���q�o�b�t�@���
	free( ValidExStrBuf ) ;

	// �I��
	return FileList.Num ;

	// �G���[����
ERR :
	ReleaseFileList( &FileList ) ;
	if( OmitStrBuf != NULL ) free( OmitStrBuf ) ;
	if( OmitExStrBuf != NULL ) free( OmitExStrBuf ) ;
	if( ValidExStrBuf != NULL ) free( ValidExStrBuf ) ;
	
	return -1 ;
}

extern int ReleaseFileList( FILE_INFOLIST *DirectoryInfo )
{
	// �������̉��
	if( DirectoryInfo->List != NULL )
	{
		int i ;
		FILE_INFO *finfo ;

		finfo = DirectoryInfo->List ;
		for( i = 0 ; i < DirectoryInfo->Num ; i ++, finfo ++ )
			if( finfo->FileName != NULL ) free( finfo->FileName ) ;

		free( DirectoryInfo->List ) ;
		DirectoryInfo->List = NULL ;
	}

	// �[��������
	memset( DirectoryInfo, 0, sizeof( FILE_INFOLIST ) ) ;

	// �I��
	return 0 ;
}

// ����̃p�X�����񂩂�F�X�ȏ����擾����
// ( CurrentDir �̓t���p�X�ł���K�v������(����Ɂw\�x�������Ă������Ă��ǂ�) )
// ( CurrentDir �� 0 �̏ꍇ�͎��ۂ̃J�����g�f�B���N�g�����g�p���� )
extern int AnalyseFilePath(
	const char *Src,
	char *FullPath, char *DirPath, char *FileName, char *Name, char *ExeName, const char *CurrentDir )
{
	char full[256], dir[256], fname[256], name[256], exename[256];

	ConvertFullPath__( Src, full, CurrentDir );
	AnalysisFileNameAndDirPath( full, fname, dir );
	AnalysisFileNameAndExeName( fname, name, exename );
	if( FullPath ) strcpy( FullPath, full    );
	if( DirPath  ) strcpy( DirPath,  dir     );
	if( FileName ) strcpy( FileName, fname   );
	if( Name     ) strcpy( Name,     name    );
	if( ExeName  ) strcpy( ExeName,  exename );

	return 0;
}

// �t���p�X�ł͂Ȃ��p�X��������t���p�X�ɕϊ�����
// ( CurrentDir �̓t���p�X�ł���K�v������(����Ɂw\�x�������Ă������Ă��ǂ�) )
// ( CurrentDir �� NULL �̏ꍇ�͎��ۂ̃J�����g�f�B���N�g�����g�p���� )
extern int ConvertFullPath__( const char *Src, char *Dest, const char *CurrentDir )
{
	int i, j, k ;
	char iden[256], cur[MAX_PATH] ;

	if( CurrentDir == NULL )
	{
		GetCurrentDirectory( MAX_PATH, cur ) ;
		CurrentDir = cur ;
	}

	if( Src == NULL )
	{
		strcpy( Dest, CurrentDir ) ;
		goto END ;
	}

	i = 0 ;
	j = 0 ;
	k = 0 ;
	
	// �ŏ��Ɂw\�x���́w/�x���Q��A���ő����Ă���ꍇ�̓l�b�g���[�N����Ă���Ɣ��f
	if( ( Src[0] == '\\' && Src[1] == '\\' ) ||
		( Src[0] == '/'  && Src[1] == '/'  ) )
	{
		Dest[0] = '\\';
		Dest[1] = '\0';

		i += 2;
		j ++ ;
	}
	else
	// �ŏ����w\�x���́w/�x�̏ꍇ�̓J�����g�h���C�u�̃��[�g�f�B���N�g���܂ŗ�����
	if( Src[0] == '\\' )
	{
		Dest[0] = CurrentDir[0] ;
		Dest[1] = CurrentDir[1] ;
		Dest[2] = '\0' ;

		i ++ ;
		j = 2 ;
	}
	else
	// �h���C�u����������Ă����炻�̃h���C�u��
	if( Src[1] == ':' )
	{
		Dest[0] = Src[0] ;
		Dest[1] = Src[1] ;
		Dest[2] = '\0' ;

		i = 2 ;
		j = 2 ;

		if( Src[i] == '\\' ) i ++ ;
	}
	else
	// ����ȊO�̏ꍇ�̓J�����g�f�B���N�g��
	{
		strcpy( Dest, CurrentDir ) ;
		j = strlen( Dest ) ;
		if( Dest[j-1] == '\\' || Dest[j-1] == '/' )
		{
			Dest[j-1] = '\0' ;
			j -- ;
		}
	}

	for(;;)
	{
		switch( Src[i] )
		{
		case '\0' :
			if( k != 0 )
			{
				Dest[j] = '\\' ; j ++ ;
				strcpy( &Dest[j], iden ) ;
				j += k ;
				k = 0 ;
			}
			goto END ;

		case '\\' :
		case '/' :
			// �����񂪖���������X�L�b�v
			if( k == 0 )
			{
				i ++ ;
				break;
			}
			if( strcmp( iden, "." ) == 0 )
			{
				// �Ȃɂ����Ȃ�
			}
			else
			if( strcmp( iden, ".." ) == 0 )
			{
				// ����̃f�B���N�g����
				j -- ;
				while( Dest[j] != '\\' && Dest[j] != '/' && Dest[j] != ':' ) j -- ;
				if( Dest[j] != ':' ) Dest[j] = '\0' ;
				else j ++ ;
			}
			else
			{
				Dest[j] = '\\' ; j ++ ;
				strcpy( &Dest[j], iden ) ;
				j += k ;
			}

			k = 0 ;
			i ++ ;
			break ;
		
		default :
			if( _mbsbtype( ( const unsigned char * )&Src[i], 0 ) == 0/*(_MBC_SINGLE)*/ )
			{
				iden[k] = Src[i] ;
				iden[k+1] = 0 ; 
				k ++ ;
				i ++ ;
			}
			else
			{
				*(( unsigned short * )&iden[k] ) = *(( unsigned short * )&Src[i] ) ;
				iden[k+2] = '\0' ;
				k += 2 ;
				i += 2 ;
			}
			break ;
		}
	}
	
END :
	// ����I��
	return 0 ;
}

// �t�@�C�������ꏏ�ɂȂ��Ă���ƕ������Ă���p�X������t�@�C�����ƃf�B���N�g���p�X�𕪊�����
// �t���p�X�ł���K�v�͖����A�t�@�C���������ł��ǂ�
extern int AnalysisFileNameAndDirPath( const char *Src, char *FileName, char *DirPath )
{
	int i, Last ;
	
	// �t�@�C�����𔲂��o��
	i = 0 ;
	Last = -1 ;
	while( Src[i] != '\0' )
	{
		if( _mbsbtype( ( const unsigned char * )&Src[i], 0 ) == 0/*(_MBC_SINGLE)*/ )
		{
			if( Src[i] == '\\' || Src[i] == '/' || Src[i] == '\0' || Src[i] == ':' ) Last = i ;
			i ++ ;
		}
		else
		{
			i += 2 ;
		}
	}
	if( FileName != NULL )
	{
		if( Last != -1 ) strcpy( FileName, &Src[Last+1] ) ;
		else strcpy( FileName, Src ) ;
	}
	
	// �f�B���N�g���p�X�𔲂��o��
	if( DirPath != NULL )
	{
		if( Last != -1 )
		{
			strncpy( DirPath, Src, Last ) ;
			DirPath[Last] = '\0' ;
		}
		else
		{
			DirPath[0] = '\0' ;
		}
	}
	
	// �I��
	return 0 ;
}

// �t�@�C���p�X����t�@�C�����Ɗg���q���擾����
extern int AnalysisFileNameAndExeName( const char *Src, char *Name, char *ExeName )
{
	char FileName[256], *p, ename[128], name[128] ;

	// �t�@�C�����݂̂��擾
	AnalysisFileNameAndDirPath( Src, FileName, 0 ) ;

	// �w.�x�����邩�ǂ����ŏ����𕪊�
	if( ( p = strrchr( FileName, '.' ) ) == NULL )
	{
		strcpy( name, FileName ) ;
		ename[0] = '\0' ;
	}
	else
	{
		strncpy( name, FileName, p - FileName ) ;
		name[p - FileName] = '\0' ;
		strcpy( ename, p + 1 ) ;
	}

	if( Name != NULL ) strcpy( Name, name ) ;
	if( ExeName != NULL ) strcpy( ExeName, ename ) ;

	// �I��
	return 0 ;
}

// �t�@�C���p�X�̊g���q��ς���������𓾂�
extern int GetChangeExeNamePath( const char *Src, char *Dest, const char *ExeName )
{
	char DirPath[256], FileName[128] ;

	AnalysisFileNameAndDirPath( Src, NULL, DirPath ) ;
	AnalysisFileNameAndExeName( Src, FileName, 0 ) ;
	SetEnMark( DirPath ) ;
	sprintf( Dest, "%s%s.%s", DirPath, FileName, ExeName ) ;
	
	// �I��
	return 0 ;
}


// ����Ɂw\�x�����Ă��Ȃ��ꍇ�͕t����
extern void SetEnMark( char *PathBuf )
{
	int Len = ( int )strlen( PathBuf ) ;
	
	if( PathBuf[Len-1] != '\\' )
	{
		PathBuf[Len] = '\\' ;
		PathBuf[Len+1] = '\0' ;
	}
}

// ����Ɏw��̕������Ȃ��ꍇ�͂���
extern void SetChr( char *PathBuf, char chr )
{
	int Len = ( int )strlen( PathBuf ) ;
	
	if( PathBuf[Len-1] != chr )
	{
		PathBuf[Len] = chr ;
		PathBuf[Len+1] = '\0' ;
	}
}

// ����Ɏw��̕���������ꍇ�͍폜����
extern void DelChr( char *PathBuf, char chr )
{
	int Len = ( int )strlen( PathBuf ) ;

	if( PathBuf[Len-1] == chr )
	{
		PathBuf[Len-1] = '\0' ;
	}
}


// �g���q�𓾂�
extern int GetExName( const char *Path, char *ExNameBuf )
{
	char *p ;
	
	// ��ԍŌ�� '.' ���o�镔�����擾����
	p = strrchr( ( char * )Path, '.' ) ;
	if( p == NULL ) ExNameBuf[0] = '\0' ;
	else strcpy( ExNameBuf, p + 1 ) ;

	// �I��
	return 0 ;
}

// �g���q��ύX����
extern int SetExName( const char *Path, char *ExName, char *DestBuf )
{
	char *p ;
	char tempstr[256] ;
	
	strcpy( tempstr, Path ) ;

	// ��ԍŌ�� '.' ���o�镔�����擾����
	p = strrchr( tempstr, '.' ) ;
	if( p == NULL )
	{
		sprintf( DestBuf, "%s.%s", tempstr, ExName ) ;
	}
	else
	{
		strncpy( DestBuf, tempstr, p - tempstr + 1 ) ;
		strcpy( DestBuf + ( p - tempstr + 1 ), ExName ) ;
	}

	// �I��
	return 0 ;
}


// �e�L�X�g�f�[�^���ǂ����𔻒肷��( 1:�e�L�X�g�f�[�^  0:�o�C�i���f�[�^ )
extern int CheckTextData( void *buffer, int size )
{
	unsigned char *p ;
	int len, saerch ;
	
	// �擪�W�j�a�̂݌���
	saerch = size > TEXTCHECKSIZE ? TEXTCHECKSIZE : size ;

	// �I�[�������Q��ȏ�o�Ă�����o�C�i���f�[�^
	{
		int con ;
	
		p = ( unsigned char * )buffer ;
		con = 0 ;
		for( len = saerch ; len > 0 ; len --, p ++ )
		{
			if( *p == '\0' )
			{
				con ++ ;
				if( con >= 2 ) return 0 ;
			}
		}
	}
	
	// ���s�A�^�u�ȊO�̃R���g���[���������o�Ă�����o�C�i���f�[�^
	{
		int con ;
		
		p = ( unsigned char * )buffer ;
		con = 0 ;
		for( len = saerch ; len > 0 ; len --, p ++ )
		{
			if( *p < 0x20 && *p != '\r' && *p != '\n' && *p != '\t' )
			{
				con ++ ;
				if( con >= 1 ) return 0 ;
			}
		}
	}
	
	// �����ł͂Ȃ��R�[�h���o�Ă�����o�C�i���f�[�^
	{
		p = ( unsigned char * )buffer ;
		for( len = saerch ; len > 0 ; len --, p ++ )
		{
			if( ( *p >= 0x81 && *p <= 0x9f ) || ( *p >= 0xe0 && *p <= 0xfc ) )
			{
				len -- ;
				p ++ ;
				if( len == 0 && size <= saerch ) return 0 ;
				
				if( !( ( *p >= 0x40 && *p <= 0x7e ) || ( *p >= 0x80 && *p <= 0xfc ) ) )
					return 0 ;
				
				if( len == 0 ) return 1 ;
			}
		}
	}
	
	// �����ɗ�����e�L�X�g�f�[�^
	return 1 ;
}

// �e�L�X�g�t�@�C�����ǂ����𔻒肷��( 1:�e�L�X�g�f�[�^  0:�o�C�i���f�[�^ )
extern int CheckTextFile( const char *Path )
{
	void *buf ;
	int size, res ;
	FILE *fp ;

	fp = fopen( Path, "rb" ) ;
	if( fp == NULL ) return -1 ;
	
	fseek( fp, 0L, SEEK_END ) ;
	size = ftell( fp ) ;
	fseek( fp, 0L, SEEK_SET ) ;
	
	size = size > TEXTCHECKSIZE ? TEXTCHECKSIZE : size ;
	
	buf = malloc( size ) ;
	if( buf == NULL )
	{
		fclose( fp ) ;
		return -1 ;
	}
	
	fread( buf, size, 1, fp ) ;
	fclose( fp ) ;
	
	res = CheckTextData( buf, size ) ;
	
	free( buf ) ;

	return res ;
}

// �Q�o�C�g���������ׂ�( TRUE:�Q�o�C�g���� FALSE:�P�o�C�g���� )
extern int CheckMultiByteChar( char *Buf )
{
	return  ( (unsigned char)*Buf >= 0x81 && (unsigned char)*Buf <= 0x9F ) || ( (unsigned char)*Buf >= 0xE0 && (unsigned char)*Buf <= 0xFC ) ;
}







