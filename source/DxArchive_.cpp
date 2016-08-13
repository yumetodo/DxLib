// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�A�[�J�C�u����v���O����
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------


// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

// �C���N���[�h-------------------------------------------------------------------
#include "DxArchive_.h"
#include "DxBaseFunc.h"
#include "DxMemory.h"
#include "DxFile.h"
#include "DxFont.h"
#include "DxLog.h"
#include "DxChar.h"
#include "DxThread.h"
#include "DxSystem.h"
#include <stdio.h>


#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

#ifndef DX_NON_DXA

// �}�N����` --------------------------------------------------------------------

#define DXARCD						DX_ArchiveDirData
//#define CHECKMULTIBYTECHAR(CP)		(( (unsigned char)*(CP) >= 0x81 && (unsigned char)*(CP) <= 0x9F ) || ( (unsigned char)*(CP) >= 0xE0 && (unsigned char)*(CP) <= 0xFC ))	// TRUE:�Q�o�C�g����  FALSE:�P�o�C�g����
#define CHARUP(C)					( (C) >= 'a' && (C) <= 'z' ? (C) - 'a' + 'A' : (C))

#define DXARC_ID_AND_VERSION_SIZE	(sizeof( WORD ) * 2)

#define DXARC_HEAD_VER3_SIZE		(24)
#define DXARC_HEAD_VER4_SIZE		(28)
#define DXARC_HEAD_VER6_SIZE		(48)

#define DXARC_FILEHEAD_VER1_SIZE	(40)			// Ver0x0001 �� DXARC_FILEHEAD �\���̂̃T�C�Y
#define DXARC_FILEHEAD_VER2_SIZE	(44)			// Ver0x0002 �� DXARC_FILEHEAD �\���̂̃T�C�Y
#define DXARC_FILEHEAD_VER6_SIZE	(64)			// Ver0x0006 �� DXARC_FILEHEAD �\���̂̃T�C�Y

#if defined( DX_GCC_COMPILE ) || defined( __ANDROID__ )
#define NONE_PAL		(0xffffffffffffffffULL)
#else
#define NONE_PAL		(0xffffffffffffffff)
#endif

// �\���̒�` --------------------------------------------------------------------

// DXA_DIR_FindFirst ���̏����Ŏg�p����\����
struct DXA_DIR_FINDDATA
{
	int							UseArchiveFlag;					// �A�[�J�C�u�t�@�C�����g�p���Ă��邩�t���O
	int							UseArchiveIndex;				// �A�[�J�C�u���g�p���Ă���ꍇ�A�g�p���Ă���A�[�J�C�u�t�@�C���f�[�^�̃C���f�b�N�X
	DWORD_PTR					FindHandle;						// �t�@�C�������p�n���h��
} ;

// DXA_FindFirst ���̏����Ŏg�p����\����
struct DXA_FINDDATA
{
	DXARC						*Container;						// �����Ώۂ̂c�w�`�t�@�C��
	BYTE						SearchStr[ FILEPATH_MAX ] ;		// ����������
	union
	{
		DXARC_DIRECTORY			*Directory;						// �����Ώۂ̃f�B���N�g��
		DXARC_DIRECTORY_VER5	*DirectoryV5;					// �����Ώۂ̃f�B���N�g��(Ver5�ȑO�p)
	};
	DWORD						ObjectCount;					// ���ɓn���f�B���N�g�����I�u�W�F�N�g�̃C���f�b�N�X
} ;

// �������ϐ��錾 --------------------------------------------------------------

static unsigned char BinToChar128Table[ 128 ] =
{
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40,
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60,
	0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
	0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0xA1, 0xA2,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
	0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2,
	0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2,
} ;

static unsigned char Char128ToBinTable[ 256 ] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e,
	0x5f, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e,
	0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
	0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
	0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x4e, 0x4f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c,
	0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b, 0x7c,
	0x7d, 0x7e, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
} ;

BYTE Ascii_DotStr[ 2 ]       = { '.',       0 } ;
BYTE Ascii_DoubleDotStr[ 3 ] = { '.',  '.', 0 } ;
BYTE Ascii_EnStr[ 2 ]        = { '\\',      0 } ;
BYTE Ascii_SlashStr[ 2 ]     = { '/',       0 } ;

// �A�[�J�C�u���f�B���N�g���Ɍ����Ă�ׂ̃f�[�^
DXARC_DIR DX_ArchiveDirData ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

static DXARC_FILEHEAD_VER5 *DXA_GetFileHeaderV5(	DXARC *DXA, const BYTE *FilePath ) ;													// �t�@�C���̏��𓾂�
static DXARC_FILEHEAD      *DXA_GetFileHeader(		DXARC *DXA, const BYTE *FilePath ) ;													// �t�@�C���̏��𓾂�
static int		DXA_ConvSearchData(					DXARC *DXA, DXARC_SEARCHDATA *Dest, const BYTE *Src, int *Length ) ;					// ������������p�̃f�[�^�ɕϊ�( �k�������� \ ����������I�� )
static int		DXA_ChangeCurrentDirectoryFast(		DXARC *DXA, DXARC_SEARCHDATA *SearchData ) ;											// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
static int		DXA_ChangeCurrentDirectoryBase(		DXARC *DXA, const BYTE *DirectoryPath, bool ErrorIsDirectoryReset, DXARC_SEARCHDATA *LastSearchData = NULL ) ;		// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
//static int	DXA_Decode(							void *Src, void *Dest ) ;																// �f�[�^���𓀂���( �߂�l:�𓀌�̃f�[�^�T�C�Y )
static void		DXA_KeyCreate(						const char *Source, unsigned char *Key ) ;												// ����������쐬
static void		DXA_KeyConv(						void *Data, LONGLONG  SizeLL, LONGLONG  PositionLL,  unsigned char *Key ) ;				// ����������g�p���� Xor ���Z( Key �͕K�� DXA_KEYSTR_LENGTH �̒������Ȃ���΂Ȃ�Ȃ� )
static void		DXA_KeyConvFileRead(				void *Data, ULONGLONG Size,   DWORD_PTR FilePointer, unsigned char *Key, LONGLONG Position = -1 ) ;	// �t�@�C������ǂݍ��񂾃f�[�^������������g�p���� Xor ���Z����֐�( Key �͕K�� DXA_KEYSTR_LENGTH �̒������Ȃ���΂Ȃ�Ȃ� )
static int		DXA_FindProcess(					DXA_FINDDATA *FindData, FILEINFOW *FileInfo );											// �����ɓK������I�u�W�F�N�g����������(�����Ώۂ� ObjectCount ���C���f�b�N�X�Ƃ����Ƃ��납��)(�߂�l -1:�G���[ 0:����)

static int		DXA_DIR_OpenArchive(				const wchar_t *FilePath, void *FileImage = NULL, int FileSize = -1, int FileImageCopyFlag = FALSE, int FileImageReadOnly = FALSE, int ArchiveIndex = -1, int OnMemory = FALSE, int ASyncThread = FALSE ) ;	// �A�[�J�C�u�t�@�C�����J��
static int		DXA_DIR_GetArchive(					const wchar_t *FilePath, void *FileImage = NULL ) ;										// ���ɊJ����Ă���A�[�J�C�u�̃n���h�����擾����( �߂�l: -1=�������� 0�ȏ�:�n���h�� )
static int		DXA_DIR_CloseArchive(				int ArchiveHandle ) ;																	// �A�[�J�C�u�t�@�C�������
static void		DXA_DIR_CloseWaitArchive(			void ) ;																				// �g�p�����̂�҂��Ă���A�[�J�C�u�t�@�C����S�ĕ���
static int		DXA_DIR_ConvertFullPath(			const wchar_t *Src, wchar_t *Dest, size_t BufferBytes ) ;								// �S�Ẳp����������啶���ɂ��Ȃ���A�t���p�X�ɕϊ�����
static int		DXA_DIR_AnalysisFileNameAndDirPath( DXARC *DXA, const BYTE *Src, BYTE *FileName = 0, size_t FileNameBytes = 0, BYTE *DirPath = 0, size_t DirPathBytes = 0 ) ;					// �t�@�C�������ꏏ�ɂȂ��Ă���ƕ������Ă���p�X������t�@�C�����ƃf�B���N�g���p�X�𕪊�����B�t���p�X�ł���K�v�͖����A�t�@�C���������ł��ǂ��ADirPath �̏I�[�� �� �}�[�N�͕t���Ȃ�
static int		DXA_DIR_FileNameCmp(				DXARC *DXA, const BYTE *Src, const BYTE *CmpStr );										// CmpStr �̏����� Src ���K�����邩�ǂ����𒲂ׂ�( 0:�K������  -1:�K�����Ȃ� )
static int		DXA_DIR_OpenTest(					const wchar_t *FilePath, int *ArchiveIndex, BYTE *ArchiveFilePath, size_t BufferBytes ) ;	// �A�[�J�C�u�t�@�C�����t�H���_�Ɍ����Ăăt�@�C�����J�����̏��𓾂�( -1:�A�[�J�C�u�Ƃ��Ă͑��݂��Ȃ�����  0:���݂��� )

static int		DXA_DirectoryKeyConv(				DXARC *DXA, DXARC_DIRECTORY *Dir ) ;													// �w��̃f�B���N�g���f�[�^�̈Í�������������( �ۂ��ƃ������ɓǂݍ��񂾏ꍇ�p )
static int		DXA_DirectoryKeyConvV5(				DXARC *DXA, DXARC_DIRECTORY_VER5 *Dir ) ;												// �w��̃f�B���N�g���f�[�^�̈Í�������������( �ۂ��ƃ������ɓǂݍ��񂾏ꍇ�p )

// �v���O���� --------------------------------------------------------------------

// �t�@�C���̏��𓾂�
static DXARC_FILEHEAD_VER5 *DXA_GetFileHeaderV5( DXARC *DXA, const BYTE *FilePath )
{
	DXARC_DIRECTORY_VER5 *OldDir ;
	DXARC_FILEHEAD_VER5 *FileH ;
	DWORD FileHeadSize ;
	BYTE *NameData ;
	int i, j, k, Num ;
	DXARC_SEARCHDATA SearchData ;

	// ���̃f�B���N�g����ۑ����Ă���
	OldDir = DXA->CurrentDirectoryV5 ;

	// �t�@�C���p�X�� \ or / ���܂܂�Ă���ꍇ�A�f�B���N�g���ύX���s��
	if( CL_strchr( DXA->CharCodeFormat, ( const char * )FilePath, '\\' ) != NULL ||
		CL_strchr( DXA->CharCodeFormat, ( const char * )FilePath, '/'  ) != NULL )
	{
		// �J�����g�f�B���N�g����ړI�̃t�@�C��������f�B���N�g���ɕύX����
		if( DXA_ChangeCurrentDirectoryBase( DXA, FilePath, false, &SearchData ) >= 0 )
		{
			// �G���[���N���Ȃ������ꍇ�̓t�@�C�������f�B���N�g�����������ƂɂȂ�̂ŃG���[
			goto ERR ;
		}
	}
	else
	{
		// �t�@�C�����������p�f�[�^�ɕϊ�����
		DXA_ConvSearchData( DXA, &SearchData, FilePath, NULL ) ;
	}

	// �����̃t�@�C����T��
	FileHeadSize = ( DWORD )( DXA->HeadV5.Version >= 0x0002 ? DXARC_FILEHEAD_VER2_SIZE : DXARC_FILEHEAD_VER1_SIZE ) ;
	FileH        = ( DXARC_FILEHEAD_VER5 * )( DXA->Table.FileTable + DXA->CurrentDirectoryV5->FileHeadAddress ) ;
	Num          = ( int )DXA->CurrentDirectoryV5->FileHeadNum ;
	for( i = 0 ; i < Num ; i ++, FileH = (DXARC_FILEHEAD_VER5 *)( (BYTE *)FileH + FileHeadSize ) )
	{
		// �f�B���N�g���`�F�b�N
		if( ( FileH->Attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
		{
			continue ;
		}

		// �����񐔂ƃp���e�B�`�F�b�N
		NameData = DXA->Table.NameTable + FileH->NameAddress ;
		if( SearchData.PackNum != ( ( WORD * )NameData )[ 0 ] ||
			SearchData.Parity  != ( ( WORD * )NameData )[ 1 ] )
		{
			continue ;
		}

		// ������`�F�b�N
		NameData += 4 ;
		for( j = 0, k = 0 ; j < SearchData.PackNum ; j ++, k += 4 )
		{
			if( *( ( DWORD * )&SearchData.FileName[ k ]) != *( ( DWORD * )&NameData[ k ] ) )
			{
				break ;
			}
		}

		// �K�������t�@�C�����������炱���ŏI��
		if( SearchData.PackNum == j )
		{
			break ;
		}
	}

	// ����������G���[
	if( i == Num )
	{
		goto ERR ;
	}
	
	// �f�B���N�g�������ɖ߂�
	DXA->CurrentDirectoryV5 = OldDir ;
	
	// �ړI�̃t�@�C���̃A�h���X��Ԃ�
	return FileH ;
	
ERR :
	// �f�B���N�g�������ɖ߂�
	DXA->CurrentDirectoryV5 = OldDir ;
	
	// �G���[�I��
	return NULL ;
}

// �t�@�C���̏��𓾂�
static DXARC_FILEHEAD *DXA_GetFileHeader( DXARC *DXA, const BYTE *FilePath )
{
	DXARC_DIRECTORY *OldDir ;
	DXARC_FILEHEAD *FileH ;
	DWORD FileHeadSize ;
	BYTE *NameData ;
	int i, j, k, Num ;
	DXARC_SEARCHDATA SearchData ;

	// ���̃f�B���N�g����ۑ����Ă���
	OldDir = DXA->CurrentDirectory ;

	// �t�@�C���p�X�� \ or / ���܂܂�Ă���ꍇ�A�f�B���N�g���ύX���s��
	if( CL_strchr( DXA->CharCodeFormat, ( const char * )FilePath, '\\' ) != NULL ||
		CL_strchr( DXA->CharCodeFormat, ( const char * )FilePath, '/'  ) != NULL )
	{
		// �J�����g�f�B���N�g����ړI�̃t�@�C��������f�B���N�g���ɕύX����
		if( DXA_ChangeCurrentDirectoryBase( DXA, FilePath, false, &SearchData ) >= 0 )
		{
			// �G���[���N���Ȃ������ꍇ�̓t�@�C�������f�B���N�g�����������ƂɂȂ�̂ŃG���[
			goto ERR ;
		}
	}
	else
	{
		// �t�@�C�����������p�f�[�^�ɕϊ�����
		DXA_ConvSearchData( DXA, &SearchData, FilePath, NULL ) ;
	}

	// �����̃t�@�C����T��
	FileHeadSize = DXARC_FILEHEAD_VER6_SIZE ;
	FileH        = ( DXARC_FILEHEAD * )( DXA->Table.FileTable + DXA->CurrentDirectory->FileHeadAddress ) ;
	Num          = ( int )DXA->CurrentDirectory->FileHeadNum ;
	for( i = 0 ; i < Num ; i ++, FileH = (DXARC_FILEHEAD *)( (BYTE *)FileH + FileHeadSize ) )
	{
		// �f�B���N�g���`�F�b�N
		if( ( FileH->Attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
		{
			continue ;
		}

		// �����񐔂ƃp���e�B�`�F�b�N
		NameData = DXA->Table.NameTable + FileH->NameAddress ;
		if( SearchData.PackNum != ( ( WORD * )NameData )[ 0 ] ||
			SearchData.Parity  != ( ( WORD * )NameData )[ 1 ] )
		{
			continue ;
		}

		// ������`�F�b�N
		NameData += 4 ;
		for( j = 0, k = 0 ; j < SearchData.PackNum ; j ++, k += 4 )
		{
			if( *( ( DWORD * )&SearchData.FileName[ k ] ) != *( ( DWORD * )&NameData[ k ] ) )
			{
				break ;
			}
		}

		// �K�������t�@�C�����������炱���ŏI��
		if( SearchData.PackNum == j )
		{
			break ;
		}
	}

	// ����������G���[
	if( i == Num )
	{
		goto ERR ;
	}
	
	// �f�B���N�g�������ɖ߂�
	DXA->CurrentDirectory = OldDir ;
	
	// �ړI�̃t�@�C���̃A�h���X��Ԃ�
	return FileH ;
	
ERR :
	// �f�B���N�g�������ɖ߂�
	DXA->CurrentDirectory = OldDir ;
	
	// �G���[�I��
	return NULL ;
}


// ������������p�̃f�[�^�ɕϊ�( �k�������� \ ����������I�� )
static int DXA_ConvSearchData( DXARC *DXA, DXARC_SEARCHDATA *Dest, const BYTE *Src, int *Length )
{
	int i, j ;
	int StringLength ;
	WORD  ParityData ;
	DWORD CharCode ;
	int CharBytes ;

	ParityData = 0 ;
	i          = 0 ;
	for(;;)
	{
		CharCode = GetCharCode( ( const char * )&Src[ i ], DXA->CharCodeFormat, &CharBytes ) ;
		if( CharCode == '\0' || CharCode == '\\' || CharCode == '/' )
		{
			break ;
		}

		// �������̏ꍇ�͑啶���ɕϊ�
		if( CharCode >= 'a' && CharCode <= 'z' )
		{
			CharCode = CharCode - 'a' + 'A' ;
			PutCharCode( CharCode, DXA->CharCodeFormat, ( char * )&Dest->FileName[ i ], sizeof( Dest->FileName ) - i ) ;

			switch( CharBytes )
			{
			case 1 :
				ParityData += Dest->FileName[ i ] ;
				break ;

			case 2 :
				ParityData += Dest->FileName[ i ] + Dest->FileName[ i + 1 ] ;
				break ;

			case 4 :
				ParityData += Dest->FileName[ i ] + Dest->FileName[ i + 1 ] + Dest->FileName[ i + 2 ] + Dest->FileName[ i + 3 ] ;
				break ;

			default :
				for( j = 0 ; j < CharBytes ; j ++ )
				{
					ParityData += Dest->FileName[ i + j ] ;
				}
				break ;
			}
		}
		else
		{
			switch( CharBytes )
			{
			case 1 :
				Dest->FileName[ i ] = Src[ i ] ;
				ParityData += Src[ i ] ;
				break ;

			case 2 :
				*( ( WORD * )&Dest->FileName[ i ] ) = *( ( WORD * )&Src[ i ] ) ;
				ParityData += Src[ i ] + Src[ i + 1 ] ;
				break ;

			case 4 :
				*( ( DWORD * )&Dest->FileName[ i ] ) = *( ( DWORD * )&Src[ i ] ) ;
				ParityData += Src[ i ] + Src[ i + 1 ] + Src[ i + 2 ] + Src[ i + 3 ] ;
				break ;

			default :
				for( j = 0 ; j < CharBytes ; j ++ )
				{
					Dest->FileName[ i + j ] = Src[ i + j ] ;
					ParityData += Src[ i + j ] ;
				}
				break ;
			}
		}

		i += CharBytes ;
	}

	// ������̒�����ۑ�
	if( Length != NULL )
	{
		*Length = i ;
	}

	// �S�̔{���̈ʒu�܂łO����
	StringLength = ( ( i + 1 ) + 3 ) / 4 * 4 ;
	_MEMSET( &Dest->FileName[ i ], 0, ( size_t )( StringLength - i ) ) ;

	// �p���e�B�f�[�^�̕ۑ�
	Dest->Parity = ParityData ;

	// �p�b�N�f�[�^���̕ۑ�
	Dest->PackNum = ( WORD )( StringLength / 4 ) ;

	// ����I��
	return 0 ;
}






// ����������쐬
void DXA_KeyCreate( const char *Source, unsigned char *Key )
{
	size_t Len ;

	if( Source == NULL )
	{
		_MEMSET( Key, 0xaa, DXA_KEYSTR_LENGTH ) ;
	}
	else
	{
		Len = CL_strlen( DX_CHARCODEFORMAT_ASCII, Source ) ;
		if( Len > DXA_KEYSTR_LENGTH )
		{
			_MEMCPY( Key, Source, DXA_KEYSTR_LENGTH ) ;
		}
		else
		{
			// �������� DXA_KEYSTR_LENGTH ���Z�������烋�[�v����
			size_t i ;

			for( i = 0 ; i + Len <= DXA_KEYSTR_LENGTH ; i += Len )
			{
				_MEMCPY( Key + i, Source, Len ) ;
			}

			if( i < DXA_KEYSTR_LENGTH )
			{
				_MEMCPY( Key + i, Source, DXA_KEYSTR_LENGTH - i ) ;
			}
		}
	}

	Key[ 0] = ( unsigned char )( ~Key[0] ) ;
	Key[ 1] = ( unsigned char )( ( Key[1] >> 4 ) | ( Key[1] << 4 ) ) ;
	Key[ 2] = ( unsigned char )( Key[2] ^ 0x8a ) ;
	Key[ 3] = ( unsigned char )( ~( ( Key[3] >> 4 ) | ( Key[3] << 4 ) ) ) ;
	Key[ 4] = ( unsigned char )( ~Key[4] ) ;
	Key[ 5] = ( unsigned char )( Key[5] ^ 0xac ) ;
	Key[ 6] = ( unsigned char )( ~Key[6] ) ;
	Key[ 7] = ( unsigned char )( ~( ( Key[7] >> 3 ) | ( Key[7] << 5 ) ) ) ;
	Key[ 8] = ( unsigned char )( ( Key[8] >> 5 ) | ( Key[8] << 3 ) ) ;
	Key[ 9] = ( unsigned char )( Key[9] ^ 0x7f ) ;
	Key[10] = ( unsigned char )( ( ( Key[10] >> 4 ) | ( Key[10] << 4 ) ) ^ 0xd6 ) ;
	Key[11] = ( unsigned char )( Key[11] ^ 0xcc ) ;
}

// ����������g�p���� Xor ���Z( Key �͕K�� DXA_KEYSTR_LENGTH �̒������Ȃ���΂Ȃ�Ȃ� )
void DXA_KeyConv( void *Data, LONGLONG SizeLL, LONGLONG PositionLL, unsigned char *Key )
{
	int Position ;

	Position = ( int )( PositionLL % DXA_KEYSTR_LENGTH ) ;

	if( SizeLL <= 0xffffffff )
	{
		DWORD SizeT ;
		SizeT = ( DWORD )SizeLL ;

#ifndef DX_NON_INLINE_ASM
		DWORD DataT ;
		DataT = (DWORD)Data ;

		__asm
		{
			MOV EDI, DataT
			MOV ESI, Key

			MOV EAX, SizeT
			CMP EAX, DXA_KEYSTR_LENGTH
			JB LABEL2


			MOV EAX, Position
			CMP EAX, 0
			JE LABEL1


			MOV EDX, SizeT
	LOOP1:
			MOV BL, [ESI+EAX]
			XOR [EDI], BL
			INC EAX
			INC EDI
			DEC EDX
			CMP EAX, DXA_KEYSTR_LENGTH
			JB LOOP1
			XOR ECX, ECX
			MOV Position, ECX

			MOV SizeT, EDX
			CMP EDX, DXA_KEYSTR_LENGTH
			JB LABEL2


	LABEL1:
			MOV EAX, SizeT
			XOR EDX, EDX
			MOV ECX, DXA_KEYSTR_LENGTH
			DIV ECX
			MOV SizeT, EDX
			MOV ECX, EAX

			MOV EAX, [ESI]
			MOV EBX, [ESI+4]
			MOV EDX, [ESI+8]
	LOOP2:
			XOR [EDI],    EAX
			XOR [EDI+4],  EBX
			XOR [EDI+8],  EDX
			ADD EDI, DXA_KEYSTR_LENGTH
			DEC ECX
			JNZ LOOP2


	LABEL2:
			MOV EDX, SizeT
			CMP EDX, 0
			JE LABEL3


			MOV EAX, Position
	LOOP3:
			MOV BL, [ESI+EAX]
			XOR [EDI], BL
			INC EAX
			CMP EAX, DXA_KEYSTR_LENGTH
			JNE LABEL4
			XOR EAX, EAX
	LABEL4:
			INC EDI
			DEC EDX
			JNZ LOOP3
	LABEL3:
		} ;

#else // DX_NON_INLINE_ASM

		BYTE *DataBP ;
		BYTE *KeyBP ;

		DataBP = ( BYTE * )Data ;
		KeyBP = ( BYTE * )Key ;
		if( SizeT >= DXA_KEYSTR_LENGTH )
		{
			if( Position != 0 )
			{
				do
				{
					*DataBP ^= KeyBP[ Position ] ;
					Position ++ ;
					DataBP ++ ;
					SizeT -- ;
				}while( Position < DXA_KEYSTR_LENGTH ) ;
				Position = 0 ;
			}

			if( SizeT >= DXA_KEYSTR_LENGTH )
			{
				DWORD SetNum ;
				DWORD Key1, Key2, Key3 ;

				SetNum = SizeT / DXA_KEYSTR_LENGTH ;
				SizeT -= SetNum * DXA_KEYSTR_LENGTH ;
				Key1 = ( ( DWORD * )KeyBP )[ 0 ] ;
				Key2 = ( ( DWORD * )KeyBP )[ 1 ] ;
				Key3 = ( ( DWORD * )KeyBP )[ 2 ] ;

				do
				{
					( ( DWORD * )DataBP )[ 0 ] ^= Key1 ;
					( ( DWORD * )DataBP )[ 1 ] ^= Key2 ;
					( ( DWORD * )DataBP )[ 2 ] ^= Key3 ;
					DataBP += DXA_KEYSTR_LENGTH ;
					SetNum -- ;
				}while( SetNum > 0 ) ;
			}
		}

		if( SizeT > 0 )
		{
			do
			{
				*DataBP ^= KeyBP[ Position ] ;
				DataBP ++ ;
				Position ++ ;
				if( Position == DXA_KEYSTR_LENGTH )
					Position = 0 ;
				SizeT -- ;
			}while( SizeT > 0 ) ;
		}
#endif // DX_NON_INLINE_ASM
	}
	else
	{
		BYTE *DataBP ;
		BYTE *KeyBP ;

		DataBP = ( BYTE * )Data ;
		KeyBP = ( BYTE * )Key ;
		if( SizeLL >= DXA_KEYSTR_LENGTH )
		{
			if( Position != 0 )
			{
				do
				{
					*DataBP ^= KeyBP[ Position ] ;
					Position ++ ;
					DataBP ++ ;
					SizeLL -- ;
				}while( Position < DXA_KEYSTR_LENGTH ) ;
				Position = 0 ;
			}

			if( SizeLL >= DXA_KEYSTR_LENGTH )
			{
				DWORD SetNum ;
				DWORD Key1, Key2, Key3 ;

				SetNum = ( DWORD )( SizeLL / DXA_KEYSTR_LENGTH ) ;
				SizeLL -= SetNum * DXA_KEYSTR_LENGTH ;
				Key1 = ( ( DWORD * )KeyBP )[ 0 ] ;
				Key2 = ( ( DWORD * )KeyBP )[ 1 ] ;
				Key3 = ( ( DWORD * )KeyBP )[ 2 ] ;

				do
				{
					( ( DWORD * )DataBP )[ 0 ] ^= Key1 ;
					( ( DWORD * )DataBP )[ 1 ] ^= Key2 ;
					( ( DWORD * )DataBP )[ 2 ] ^= Key3 ;
					DataBP += DXA_KEYSTR_LENGTH ;
					SetNum -- ;
				}while( SetNum > 0 ) ;
			}
		}

		if( SizeLL > 0 )
		{
			do
			{
				*DataBP ^= KeyBP[ Position ] ;
				DataBP ++ ;
				Position ++ ;
				if( Position == DXA_KEYSTR_LENGTH )
					Position = 0 ;
				SizeLL -- ;
			}while( SizeLL > 0 ) ;
		}
	}
}

// �t�@�C������ǂݍ��񂾃f�[�^������������g�p���� Xor ���Z����֐�( Key �͕K�� DXA_KEYSTR_LENGTH �̒������Ȃ���΂Ȃ�Ȃ� )
void DXA_KeyConvFileRead( void *Data, ULONGLONG  Size, DWORD_PTR FilePointer, unsigned char *Key, LONGLONG Position )
{
	LONGLONG pos ;

	// �t�@�C���̈ʒu���擾���Ă���
	if( Position == -1 )
	{
		pos = ReadOnlyFileAccessTell( FilePointer ) ;
	}
	else
	{
		pos = Position ;
	}

	// �ǂݍ���
	ReadOnlyFileAccessRead( Data, ( size_t )Size, 1, FilePointer ) ;
	while( ReadOnlyFileAccessIdleCheck( FilePointer ) == FALSE )
	{
		Thread_Sleep( 1 ) ;
	}

	// �f�[�^������������g���� Xor ���Z
	DXA_KeyConv( Data, ( LONGLONG )Size, pos, Key ) ;
}

// �����ɓK������I�u�W�F�N�g����������(�����Ώۂ� ObjectCount ���C���f�b�N�X�Ƃ����Ƃ��납��)(�߂�l -1:�G���[ 0:����)
static int DXA_FindProcess( DXA_FINDDATA *FindData, FILEINFOW *FileInfo )
{
	BYTE  *nameTable ;
	DXARC *DXA ;
	BYTE  *str ;
	BYTE  *name ;
	DWORD fileHeadSize ;
	BYTE  TempName[ 2048 ] ;
	BYTE  DotStr[ 16 ] ;
	BYTE  DoubleDotStr[ 16 ] ;

	DXA       = FindData->Container;
	nameTable = DXA->Table.NameTable;
	str       = FindData->SearchStr;

	ConvString( ( const char * )Ascii_DotStr,       DX_CHARCODEFORMAT_ASCII, ( char * )DotStr,       sizeof( DotStr       ), DXA->CharCodeFormat ) ;
	ConvString( ( const char * )Ascii_DoubleDotStr, DX_CHARCODEFORMAT_ASCII, ( char * )DoubleDotStr, sizeof( DoubleDotStr ), DXA->CharCodeFormat ) ;

	if( DXA->V5Flag )
	{
		int i, num, addnum ;

		DXARC_DIRECTORY_VER5 *dir ;
		DXARC_FILEHEAD_VER5  *file ;

		dir          = FindData->DirectoryV5 ;
		num          = ( int )dir->FileHeadNum ;
		addnum       = dir->ParentDirectoryAddress == 0xffffffff ? 1 : 2 ;
		fileHeadSize = ( DWORD )( DXA->HeadV5.Version >= 0x0002 ? DXARC_FILEHEAD_VER2_SIZE : DXARC_FILEHEAD_VER1_SIZE ) ;

		if( FindData->ObjectCount == ( DWORD )( num + addnum ) )
		{
			return -1 ;
		}

		file = ( DXARC_FILEHEAD_VER5 * )( DXA->Table.FileTable + dir->FileHeadAddress + fileHeadSize * FindData->ObjectCount ) ;
		for( i = ( int )FindData->ObjectCount; i < num + addnum; i ++ )
		{
			if( i >= num )
			{
				if( i - num == 0 )
				{
					if( DXA_DIR_FileNameCmp( DXA, DotStr, str ) == 0 )
					{
						break ;
					}
				}
				else
				if( i - num == 1 )
				{
					if( DXA_DIR_FileNameCmp( DXA, DoubleDotStr, str ) == 0 )
					{
						break ;
					}
				}
			}
			else
			{
				name = ( BYTE * )( nameTable + file->NameAddress + 4 ) ;
				if( DXA_DIR_FileNameCmp( DXA, name, str ) == 0 )
				{
					break ;
				}

				file = (DXARC_FILEHEAD_VER5 *)( (BYTE *)file + fileHeadSize ) ;
			}
		}
		FindData->ObjectCount = ( DWORD )i ;
		if( i == num + addnum )
		{
			return -1 ;
		}

		if( FileInfo )
		{
			if( i >= num )
			{
				switch( i - num )
				{
				default :
				case 0 :
					CL_strcpy_s( DXA->CharCodeFormat, ( char * )TempName, sizeof( TempName ), ( const char * )DotStr  ) ;
					break ;

				case 1 :
					CL_strcpy_s( DXA->CharCodeFormat, ( char * )TempName, sizeof( TempName ), ( const char * )DoubleDotStr ) ;
					break ;
				}
				FileInfo->DirFlag = 1 ;
				FileInfo->Size    = 0 ;
				_MEMSET( &FileInfo->CreationTime,  0, sizeof( FileInfo->CreationTime  ) ) ;
				_MEMSET( &FileInfo->LastWriteTime, 0, sizeof( FileInfo->LastWriteTime ) ) ;
			}
			else
			{
				name = ( BYTE * )( nameTable + file->NameAddress ) ;
				CL_strcpy_s( DXA->CharCodeFormat, ( char * )TempName, sizeof( TempName ), ( const char * )( name + ( ( WORD * )name )[ 0 ] * 4 + 4 ) ) ;
				FileInfo->DirFlag = ( file->Attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ? TRUE : FALSE ;
				FileInfo->Size    = ( LONGLONG )file->DataSize ;
#ifdef __WINDOWS__
				_FileTimeToLocalDateData( ( FILETIME * )&file->Time.Create,    &FileInfo->CreationTime  ) ;
				_FileTimeToLocalDateData( ( FILETIME * )&file->Time.LastWrite, &FileInfo->LastWriteTime ) ;
#else // __WINDOWS__
				_MEMSET( &FileInfo->CreationTime,  0, sizeof( FileInfo->CreationTime  ) ) ;
				_MEMSET( &FileInfo->LastWriteTime, 0, sizeof( FileInfo->LastWriteTime ) ) ;
#endif // __WINDOWS__
			}

			ConvString( ( const char * )TempName, DXA->CharCodeFormat, ( char * )FileInfo->Name, sizeof( FileInfo->Name ), WCHAR_T_CHARCODEFORMAT ) ;
		}
	}
	else
	{
		ULONGLONG i, num, addnum ;
		DXARC_DIRECTORY *dir ;
		DXARC_FILEHEAD *file ;

		dir          = FindData->Directory ;
		num          = dir->FileHeadNum ;
		addnum       = ( ULONGLONG )( dir->ParentDirectoryAddress == NONE_PAL ? 1 : 2 ) ;
		fileHeadSize = DXARC_FILEHEAD_VER6_SIZE ;

		if( FindData->ObjectCount == ( DWORD )( num + addnum ) )
		{
			return -1 ;
		}

		file = ( DXARC_FILEHEAD * )( DXA->Table.FileTable + dir->FileHeadAddress + fileHeadSize * FindData->ObjectCount ) ;
		for( i = FindData->ObjectCount; i < num + addnum; i ++ )
		{
			if( i >= num )
			{
				if( i - num == 0 )
				{
					if( DXA_DIR_FileNameCmp( DXA, DotStr,       str ) == 0 )
					{
						break ;
					}
				}
				else
				if( i - num == 1 )
				{
					if( DXA_DIR_FileNameCmp( DXA, DoubleDotStr, str ) == 0 )
					{
						break;
					}
				}
			}
			else
			{
				name = ( BYTE * )( nameTable + file->NameAddress + 4 ) ;
				if( DXA_DIR_FileNameCmp( DXA, name, str ) == 0 )
				{
					break ;
				}

				file = (DXARC_FILEHEAD *)( (BYTE *)file + fileHeadSize ) ;
			}
		}
		FindData->ObjectCount = ( DWORD )i ;
		if( i == num + addnum )
		{
			return -1 ;
		}

		if( FileInfo )
		{
			if( i >= num )
			{
				switch( i - num )
				{
				default :
				case 0:
					CL_strcpy_s( DXA->CharCodeFormat, ( char * )TempName, sizeof( TempName ), ( const char * )DotStr       ) ;
					break ;

				case 1 :
					CL_strcpy_s( DXA->CharCodeFormat, ( char * )TempName, sizeof( TempName ), ( const char * )DoubleDotStr ) ;
					break ;
				}
				FileInfo->DirFlag = 1 ;
				FileInfo->Size    = 0 ;
				_MEMSET( &FileInfo->CreationTime,  0, sizeof( FileInfo->CreationTime  ) ) ;
				_MEMSET( &FileInfo->LastWriteTime, 0, sizeof( FileInfo->LastWriteTime ) ) ;
			}
			else
			{
				name = ( BYTE * )( nameTable + file->NameAddress ) ;
				CL_strcpy_s( DXA->CharCodeFormat, ( char * )TempName, sizeof( TempName ), ( const char * )( name + ( ( WORD * )name )[ 0 ] * 4 + 4 ) ) ;
				FileInfo->DirFlag = ( file->Attributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 ? TRUE : FALSE ;
				FileInfo->Size    = ( LONGLONG)file->DataSize ;
#ifdef __WINDOWS__
				_FileTimeToLocalDateData( ( FILETIME * )&file->Time.Create,    &FileInfo->CreationTime  ) ;
				_FileTimeToLocalDateData( ( FILETIME * )&file->Time.LastWrite, &FileInfo->LastWriteTime ) ;
#else // __WINDOWS__
				_MEMSET( &FileInfo->CreationTime,  0, sizeof( FileInfo->CreationTime  ) ) ;
				_MEMSET( &FileInfo->LastWriteTime, 0, sizeof( FileInfo->LastWriteTime ) ) ;
#endif // __WINDOWS__
			}

			ConvString( ( const char * )TempName, DXA->CharCodeFormat, ( char * )FileInfo->Name, sizeof( FileInfo->Name ), WCHAR_T_CHARCODEFORMAT ) ;
		}
	}

	return 0 ;
}



// �A�[�J�C�u�t�@�C���������ׂ̍\���̂�����������
extern	int	DXA_Initialize( DXARC *DXA )
{
	_MEMSET( DXA, 0, sizeof( DXARC ) ) ;

	// �I��
	return 0 ;
}

// �A�[�J�C�u�t�@�C���������ׂ̍\���̂̌�n��������
extern int DXA_Terminate( DXARC *DXA )
{
	DXA_CloseArchive( DXA ) ;

	_MEMSET( DXA, 0, sizeof( DXARC ) ) ;

	// �I��
	return 0 ;
}

// �w��̃f�B���N�g���f�[�^�̈Í�������������( �ۂ��ƃ������ɓǂݍ��񂾏ꍇ�p )
static int DXA_DirectoryKeyConv( DXARC *DXA, DXARC_DIRECTORY *Dir )
{
	// �������C���[�W�ł͂Ȃ��ꍇ�̓G���[
	if( DXA->MemoryOpenFlag == FALSE )
		return -1 ;

	// �o�[�W���� 0x0006 ���O�ł͉������Ȃ�
	if( DXA->V5Flag )
		return 0 ;

	// �Í������������J�n
	{
		DWORD i, FileHeadSize ;
		DXARC_FILEHEAD *File ;

		// �i�[����Ă���t�@�C���̐������J��Ԃ�
		FileHeadSize = DXARC_FILEHEAD_VER6_SIZE ;
		File = ( DXARC_FILEHEAD * )( DXA->Table.FileTable + Dir->FileHeadAddress ) ;
		for( i = 0 ; i < Dir->FileHeadNum ; i ++, File = ( DXARC_FILEHEAD * )( ( BYTE * )File + FileHeadSize ) )
		{
			// �f�B���N�g�����ǂ����ŏ����𕪊�
			if( File->Attributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// �f�B���N�g���̏ꍇ�͍ċA��������
				DXA_DirectoryKeyConv( DXA, ( DXARC_DIRECTORY * )( DXA->Table.DirectoryTable + File->DataAddress ) ) ;
			}
			else
			{
				BYTE *DataP ;

				// �t�@�C���̏ꍇ�͈Í�������������
				
				// �f�[�^������ꍇ�̂ݏ���
				if( File->DataSize != 0 )
				{
					// �f�[�^�ʒu���Z�b�g����
					DataP = ( BYTE * )DXA->MemoryImage + DXA->Head.DataStartAddress + File->DataAddress ;

					// �f�[�^�����k����Ă��邩�ǂ����ŏ����𕪊�
					if( File->PressDataSize != NONE_PAL )
					{
						// ���k����Ă���ꍇ
						DXA_KeyConv( DataP, ( LONGLONG )File->PressDataSize, ( LONGLONG )File->DataSize, DXA->Key ) ;
					}
					else
					{
						// ���k����Ă��Ȃ��ꍇ
						DXA_KeyConv( DataP, ( LONGLONG )File->DataSize, ( LONGLONG )File->DataSize, DXA->Key ) ;
					}
				}
			}
		}
	}

	// �I��
	return 0 ;
}

// �w��̃f�B���N�g���f�[�^�̈Í�������������( �ۂ��ƃ������ɓǂݍ��񂾏ꍇ�p )
static int DXA_DirectoryKeyConvV5( DXARC *DXA, DXARC_DIRECTORY_VER5 *Dir )
{
	// �������C���[�W�ł͂Ȃ��ꍇ�̓G���[
	if( DXA->MemoryOpenFlag == FALSE )
	{
		return -1 ;
	}

	// �o�[�W���� 0x0005 ���O�ł͉������Ȃ�
	if( DXA->HeadV5.Version < 0x0005 )
	{
		return 0 ;
	}
	
	// �Í������������J�n
	{
		DWORD i, FileHeadSize ;
		DXARC_FILEHEAD_VER5 *File ;

		// �i�[����Ă���t�@�C���̐������J��Ԃ�
		FileHeadSize = ( DWORD )( DXA->HeadV5.Version >= 0x0002 ? DXARC_FILEHEAD_VER2_SIZE : DXARC_FILEHEAD_VER1_SIZE ) ;
		File = ( DXARC_FILEHEAD_VER5 * )( DXA->Table.FileTable + Dir->FileHeadAddress ) ;
		for( i = 0 ; i < Dir->FileHeadNum ; i ++, File = ( DXARC_FILEHEAD_VER5 * )( ( BYTE * )File + FileHeadSize ) )
		{
			// �f�B���N�g�����ǂ����ŏ����𕪊�
			if( File->Attributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// �f�B���N�g���̏ꍇ�͍ċA��������
				DXA_DirectoryKeyConvV5( DXA, ( DXARC_DIRECTORY_VER5 * )( DXA->Table.DirectoryTable + File->DataAddress ) ) ;
			}
			else
			{
				BYTE *DataP ;

				// �t�@�C���̏ꍇ�͈Í�������������
				
				// �f�[�^������ꍇ�̂ݏ���
				if( File->DataSize != 0 )
				{
					// �f�[�^�ʒu���Z�b�g����
					DataP = ( BYTE * )DXA->MemoryImage + DXA->HeadV5.DataStartAddress + File->DataAddress ;

					// �f�[�^�����k����Ă��邩�ǂ����ŏ����𕪊�
					if( DXA->HeadV5.Version >= 0x0002 && File->PressDataSize != 0xffffffff )
					{
						// ���k����Ă���ꍇ
						DXA_KeyConv( DataP, File->PressDataSize, File->DataSize, DXA->Key ) ;
					}
					else
					{
						// ���k����Ă��Ȃ��ꍇ
						DXA_KeyConv( DataP, File->DataSize, File->DataSize, DXA->Key ) ;
					}
				}
			}
		}
	}

	// �I��
	return 0 ;
}

// ��������ɂ���A�[�J�C�u�t�@�C���C���[�W���J��( 0:����  -1:���s )
extern int DXA_OpenArchiveFromMem( DXARC *DXA, void *ArchiveImage, int ArchiveSize, int ArchiveImageCopyFlag, int ArchiveImageReadOnlyFlag, const char *KeyString, const wchar_t *EmulateArchivePath )
{
	BYTE *datp ;

	// �������C���[�W���R�s�[���Ďg�p����ꍇ�͓ǂݎ���p�ɂ͂��Ȃ�
	if( ArchiveImageCopyFlag )
	{
		ArchiveImageReadOnlyFlag = FALSE ;
	}

	// ���ɂȂ�炩�̃A�[�J�C�u���J���Ă����ꍇ�̓G���[
	if( DXA->WinFilePointer__ != 0 || DXA->MemoryImage != NULL ) return -1 ;

	// ���̍쐬
	DXA_KeyCreate( KeyString, DXA->Key ) ;

	// �t�@�C���p�X��ۑ�
	if( EmulateArchivePath != NULL )
	{
		_WCSCPY_S( DXA->FilePath, sizeof( DXA->FilePath ), EmulateArchivePath ) ;
	}
	else
	{
		_MEMSET( DXA->FilePath, 0, sizeof( DXA->FilePath ) ) ;
	}

	DXA->Table.Top   = NULL ;
	DXA->MemoryImage = NULL ;
	if( ArchiveImageCopyFlag )
	{
		// �C���[�W���R�s�[����t���O�������Ă���ꍇ�̓R�s�[��̃������̈���m��
		DXA->MemoryImage = DXALLOC( ( size_t )ArchiveSize ) ;
		if( DXA->MemoryImage == NULL )
		{
			return -1 ;
		}

		// �A�[�J�C�u�t�@�C���C���[�W�̓��e���m�ۂ����������ɃR�s�[
		_MEMCPY( DXA->MemoryImage, ArchiveImage, ( size_t )ArchiveSize ) ;

		// �R�s�[�����������C���[�W���g�p����
		DXA->MemoryImageOriginal = ArchiveImage ;
		ArchiveImage             = DXA->MemoryImage ;
	}
	else
	{
		// �|�C���^��ۑ�
		DXA->MemoryImage         = ArchiveImage ;
		DXA->MemoryImageOriginal = NULL ;
	}

	DXA->V5Flag = FALSE ;

	// �ŏ��Ƀw�b�_�̕����𔽓]����
	_MEMCPY( &DXA->Head, ArchiveImage, DXARC_ID_AND_VERSION_SIZE ) ;
	DXA_KeyConv( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, 0, DXA->Key ) ;

	// �h�c���Ⴄ�ꍇ�̓o�[�W�����Q�ȑO�����ׂ�
	if( DXA->Head.Head != DXAHEAD )
	{
		// �o�[�W�����Q�ȑO�����ׂ�
		_MEMSET( DXA->Key, 0xff, DXA_KEYSTR_LENGTH ) ;

		_MEMCPY( &DXA->Head, ArchiveImage, DXARC_ID_AND_VERSION_SIZE ) ;
		DXA_KeyConv( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, 0, DXA->Key ) ;

		// �o�[�W�����Q�ȑO�ł��Ȃ��ꍇ�̓G���[
		if( DXA->Head.Head != DXAHEAD )
		{
			goto ERR ;
		}
	}

	// �o�[�W����6�ȍ~���ǂ����ŏ����𕪊�
	if( DXA->Head.Version >= 0x0006 )
	{
		DXA->V5Flag = FALSE ;

		DXA->Head.CharCodeFormat = 0 ;

		// �w�b�_����͂���
		{
			_MEMCPY( &DXA->Head, ArchiveImage, DXARC_HEAD_VER6_SIZE ) ;
			DXA_KeyConv( &DXA->Head, DXARC_HEAD_VER6_SIZE, 0, DXA->Key ) ;

			// �h�c�̌���
			if( DXA->Head.Head != DXAHEAD )
			{
				goto ERR ;
			}
			
			// �o�[�W��������
			if( DXA->Head.Version > DXAVER )
			{
				goto ERR ;
			}

			// �����R�[�h�`�����Z�b�g
			switch( DXA->Head.CharCodeFormat )
			{
			case DX_CHARCODEFORMAT_UHC :
			case DX_CHARCODEFORMAT_BIG5 :
			case DX_CHARCODEFORMAT_GB2312 :
			case DX_CHARCODEFORMAT_SHIFTJIS :
			case DX_CHARCODEFORMAT_UTF16LE :
			case DX_CHARCODEFORMAT_UTF16BE :
			case DX_CHARCODEFORMAT_WINDOWS_1252 :
			case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
			case DX_CHARCODEFORMAT_UTF8 :
			case DX_CHARCODEFORMAT_UTF32LE :
			case DX_CHARCODEFORMAT_UTF32BE :
				DXA->CharCodeFormat = ( int )DXA->Head.CharCodeFormat ;
				break ;

			default :
				DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
				break ;
			}

			// �ǂݎ���p�̏ꍇ�͏��e�[�u���̃T�C�Y���̃��������m�ۂ���
			if( ArchiveImageReadOnlyFlag )
			{
				DXA->Table.Top = ( BYTE * )DXALLOC( DXA->Head.HeadSize ) ;
				if( DXA->Table.Top == NULL )
				{
					goto ERR ;
				}

				_MEMCPY( DXA->Table.Top,  (BYTE *)DXA->MemoryImage + DXA->Head.FileNameTableStartAddress, DXA->Head.HeadSize ) ;
			}
			else
			{
				// ���e�[�u���̃A�h���X���Z�b�g����
				DXA->Table.Top          = (BYTE *)DXA->MemoryImage + DXA->Head.FileNameTableStartAddress ;
			}
			DXA_KeyConv( DXA->Table.Top, DXA->Head.HeadSize, 0, DXA->Key ) ;

			DXA->Table.NameTable		= DXA->Table.Top ;
			DXA->Table.FileTable		= DXA->Table.NameTable + DXA->Head.FileTableStartAddress ;
			DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->Head.DirectoryTableStartAddress ;
		}

		// �J�����g�f�B���N�g���̃Z�b�g
		DXA->CurrentDirectory = ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ;

		DXA->MemoryOpenFlag					= TRUE ;						// �������C���[�W����J���Ă���t���O�𗧂Ă�
		DXA->UserMemoryImageFlag			= TRUE ;						// ���[�U�[�̃C���[�W����J�����t���O�𗧂Ă�
		DXA->MemoryImageSize				= ArchiveSize ;					// �T�C�Y���ۑ����Ă���
		DXA->MemoryImageCopyFlag			= ArchiveImageCopyFlag ;		// �������C���[�W���R�s�[���Ďg�p���Ă��邩�ǂ����̃t���O��ۑ�
		DXA->MemoryImageReadOnlyFlag        = ArchiveImageReadOnlyFlag ;	// �������C���[�W��ǂݎ���p�ɂ��邩�ǂ����̃t���O��ۑ�

		// �S�Ẵt�@�C���f�[�^�̈Í�������������
		if( ArchiveImageReadOnlyFlag == FALSE )
		{
			DXA_DirectoryKeyConv( DXA, ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ) ;
		}
	}
	else
	{
		DXA->V5Flag = TRUE ;

		// �o�[�W��������
		if( DXA->HeadV5.Version > DXAVER_VER5 )
		{
			goto ERR ;
		}

		// ��������ǂݎ���p�Ƃ��邩�ǂ����ŏ����𕪊�
		if( ArchiveImageReadOnlyFlag )
		{
			// �o�[�W�������S�ȏォ�ǂ����œǂݍ��ގc��̃w�b�_�T�C�Y��ύX
			if( DXA->HeadV5.Version >= 0x0004 )
			{
				_MEMCPY( &DXA->HeadV5, ArchiveImage, DXARC_HEAD_VER4_SIZE ) ;
				DXA_KeyConv( &DXA->HeadV5, DXARC_HEAD_VER4_SIZE, 0, DXA->Key ) ;
				switch( DXA->HeadV5.CharCodeFormat )
				{
				case DX_CHARCODEFORMAT_UHC :
				case DX_CHARCODEFORMAT_BIG5 :
				case DX_CHARCODEFORMAT_GB2312 :
				case DX_CHARCODEFORMAT_SHIFTJIS :
				case DX_CHARCODEFORMAT_UTF16LE :
				case DX_CHARCODEFORMAT_UTF16BE :
				case DX_CHARCODEFORMAT_WINDOWS_1252 :
				case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
				case DX_CHARCODEFORMAT_UTF8 :
				case DX_CHARCODEFORMAT_UTF32LE :
				case DX_CHARCODEFORMAT_UTF32BE :
					DXA->CharCodeFormat = ( int )DXA->HeadV5.CharCodeFormat ;
					break ;

				default :
					DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
					break ;
				}
			}
			else
			{
				_MEMCPY( &DXA->HeadV5, ArchiveImage, DXARC_HEAD_VER3_SIZE ) ;
				DXA_KeyConv( &DXA->HeadV5, DXARC_HEAD_VER3_SIZE, 0, DXA->Key ) ;

				DXA->HeadV5.CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
				DXA->CharCodeFormat        = DX_CHARCODEFORMAT_SHIFTJIS ;
			}

			// ���e�[�u���̃T�C�Y���̃��������m�ۂ���
			DXA->Table.Top = ( BYTE * )DXALLOC( DXA->HeadV5.HeadSize ) ;
			if( DXA->Table.Top == NULL )
			{
				goto ERR ;
			}
			
			// ���e�[�u�����������ɓǂݍ���
			_MEMCPY( DXA->Table.Top, ( BYTE * )DXA->MemoryImage + DXA->HeadV5.FileNameTableStartAddress, DXA->HeadV5.HeadSize ) ;
			if( DXA->HeadV5.Version >= 0x0005 )
			{
				DXA_KeyConv( DXA->Table.Top, DXA->HeadV5.HeadSize,                                     0, DXA->Key ) ;
			}
			else
			{
				DXA_KeyConv( DXA->Table.Top, DXA->HeadV5.HeadSize, DXA->HeadV5.FileNameTableStartAddress, DXA->Key ) ;
			}
		}
		else
		{
			// ���ׂẴf�[�^�𔽓]����
			if( DXA->HeadV5.Version < 0x0005 )
			{
				DXA_KeyConv( ArchiveImage, ArchiveSize, 0, DXA->Key ) ;
			}

			datp = (BYTE *)ArchiveImage ;

			// �w�b�_����͂���
			{
				if( DXA->HeadV5.Version >= 0x0005 )
				{
					_MEMCPY( &DXA->HeadV5, datp, DXARC_HEAD_VER3_SIZE ) ;
					DXA_KeyConv( &DXA->HeadV5, DXARC_HEAD_VER3_SIZE, 0, DXA->Key ) ;
				}
				else
				{
					_MEMCPY( &DXA->HeadV5, datp, DXARC_HEAD_VER3_SIZE ) ;
				}
				datp += DXARC_HEAD_VER3_SIZE ;

				// �h�c�̌���
				if( DXA->HeadV5.Head != DXAHEAD )
				{
					goto ERR ;
				}
				
				// �o�[�W��������
				if( DXA->HeadV5.Version > DXAVER_VER5 )
				{
					goto ERR ;
				}

				// �o�[�W������ 4�ȏゾ�����當���R�[�h�`����ǂݍ���
				if( DXA->HeadV5.Version >= 0x0004 )
				{
					DXA->HeadV5.CharCodeFormat = *( ( DWORD * )datp ) ;
					if( DXA->HeadV5.Version >= 0x0005 )
					{
						DXA_KeyConv( &DXA->HeadV5.CharCodeFormat, 4, DXARC_HEAD_VER3_SIZE, DXA->Key ) ;
					}
					switch( DXA->HeadV5.CharCodeFormat )
					{
					case DX_CHARCODEFORMAT_UHC :
					case DX_CHARCODEFORMAT_BIG5 :
					case DX_CHARCODEFORMAT_GB2312 :
					case DX_CHARCODEFORMAT_SHIFTJIS :
					case DX_CHARCODEFORMAT_UTF16LE :
					case DX_CHARCODEFORMAT_UTF16BE :
					case DX_CHARCODEFORMAT_WINDOWS_1252 :
					case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
					case DX_CHARCODEFORMAT_UTF8 :
					case DX_CHARCODEFORMAT_UTF32LE :
					case DX_CHARCODEFORMAT_UTF32BE :
						DXA->CharCodeFormat = ( int )DXA->HeadV5.CharCodeFormat ;
						break ;

					default :
						DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
						break ;
					}
				}
				else
				{
					DXA->HeadV5.CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
					DXA->CharCodeFormat        = DX_CHARCODEFORMAT_SHIFTJIS ;
				}

				// ���e�[�u���̃A�h���X���Z�b�g����
				DXA->Table.Top = (BYTE *)DXA->MemoryImage + DXA->HeadV5.FileNameTableStartAddress ;
				if( DXA->HeadV5.Version >= 0x0005 )
				{
					DXA_KeyConv( DXA->Table.Top, DXA->HeadV5.HeadSize, 0, DXA->Key ) ;
				}
			}
		}

		// ���e�[�u���̃A�h���X���Z�b�g����
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->HeadV5.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->HeadV5.DirectoryTableStartAddress ;

		// �J�����g�f�B���N�g���̃Z�b�g
		DXA->CurrentDirectoryV5 = ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ;

		DXA->MemoryOpenFlag					= TRUE ;						// �������C���[�W����J���Ă���t���O�𗧂Ă�
		DXA->UserMemoryImageFlag			= TRUE ;						// ���[�U�[�̃C���[�W����J�����t���O�𗧂Ă�
		DXA->MemoryImageSize				= ArchiveSize ;					// �T�C�Y���ۑ����Ă���
		DXA->MemoryImageCopyFlag			= ArchiveImageCopyFlag ;		// �������C���[�W���R�s�[���Ďg�p���Ă��邩�ǂ����̃t���O��ۑ�
		DXA->MemoryImageReadOnlyFlag        = ArchiveImageReadOnlyFlag ;	// �������C���[�W��ǂݎ���p�ɂ��邩�ǂ����̃t���O��ۑ�

		if( ArchiveImageReadOnlyFlag == FALSE )
		{
			// �S�Ẵt�@�C���f�[�^�̈Í�������������
			if( DXA->HeadV5.Version >= 0x0005 )
			{
				DXA_DirectoryKeyConvV5( DXA, ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ) ;
			}
		}
	}

	// �I��
	return 0 ;

ERR :
	if( ArchiveImageCopyFlag )
	{
		if( DXA->MemoryImage != NULL )
		{
			DXFREE( DXA->MemoryImage ) ;
			DXA->MemoryImage = NULL ;
		}
	}
	else
	if( ArchiveImageReadOnlyFlag )
	{
		if( DXA->Table.Top != NULL )
		{
			DXFREE( DXA->Table.Top ) ;
			DXA->Table.Top = NULL ;
		}
	}
	else
	{
		if( DXA->V5Flag )
		{
			// ���]�����f�[�^�����ɖ߂�
			if( DXA->HeadV5.Version < 0x0005 )
			{
				DXA_KeyConv( ArchiveImage, ArchiveSize, 0, DXA->Key ) ;
			}
		}
	}
	
	// �I��
	return -1 ;
}

// �A�[�J�C�u�t�@�C�����������������������𓾂�( TRUE:�����Ă���  FALSE:�����Ă��Ȃ� )
extern int DXA_CheckIdle( DXARC *DXA )
{
	// �񓯊��I�[�v�����ł͂Ȃ���Γ��ɂ�邱�Ƃ͂Ȃ�
	if( DXA->ASyncOpenFlag == FALSE )
	{
		return TRUE ;
	}

	// �t�@�C���ǂݍ��݂��������Ă��邩���ׂ�
	if( ReadOnlyFileAccessIdleCheck( DXA->ASyncOpenFilePointer ) == FALSE )
	{
		return FALSE ;
	}

	// �t�@�C�������
	ReadOnlyFileAccessClose( DXA->ASyncOpenFilePointer ) ;
	DXA->ASyncOpenFilePointer = 0;

	// ���ׂẴf�[�^�𔽓]����
	if( DXA->V5Flag == FALSE )
	{
		DXA_DirectoryKeyConv( DXA, ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ) ;
	}
	else
	{
		if( DXA->HeadV5.Version >= 0x0005 )
		{
			DXA_DirectoryKeyConvV5( DXA, ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ) ;
		}
		else
		{
			DXA_KeyConv( DXA->MemoryImage, DXA->MemoryImageSize, 0, DXA->Key ) ;
		}
	}

	// �񓯊��I�[�v�����t���O��|��
	DXA->ASyncOpenFlag = FALSE ;

	// �I��
	return TRUE ;
}

// �A�[�J�C�u�t�@�C�����J���ŏ��ɂ��ׂă�������ɓǂݍ���ł��珈������( 0:����  -1:���s )
extern int DXA_OpenArchiveFromFileUseMem( DXARC *DXA, const wchar_t *ArchivePath, const char *KeyString , int ASyncThread )
{
	// ���ɂȂ�炩�̃A�[�J�C�u���J���Ă����ꍇ�̓G���[
	if( DXA->WinFilePointer__ != 0 || DXA->MemoryImage )
	{
		return -1 ;
	}

	// ���̍쐬
	DXA_KeyCreate( KeyString, DXA->Key ) ;

	// �w�b�_����������ɓǂݍ���
	DXA->ASyncOpenFilePointer = 0 ;
	DXA->MemoryImage          = NULL ;
	DXA->ASyncOpenFilePointer = ReadOnlyFileAccessOpen( ArchivePath, FALSE, TRUE, FALSE ) ;
	if( DXA->ASyncOpenFilePointer == 0 )
	{
		return -1 ;
	}

	// �t�@�C���p�X��ۑ�
	_WCSCPY_S( DXA->FilePath, sizeof( DXA->FilePath ), ArchivePath ) ;

	// �t�@�C���̃T�C�Y���擾����
	ReadOnlyFileAccessSeek( DXA->ASyncOpenFilePointer, 0L, SEEK_END ) ;
	DXA->MemoryImageSize = ReadOnlyFileAccessTell( DXA->ASyncOpenFilePointer ) ;
	ReadOnlyFileAccessSeek( DXA->ASyncOpenFilePointer, 0L, SEEK_SET ) ;

	// �t�@�C���̓��e��S�ă������ɓǂݍ��ވׂ̃������̈�̊m��
	DXA->MemoryImage = DXALLOC( ( size_t )DXA->MemoryImageSize ) ;

	// ID�ƃo�[�W������ǂݍ���
	DXA_KeyConvFileRead( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, DXA->ASyncOpenFilePointer, DXA->Key, 0 ) ;

	// �h�c���Ⴄ�ꍇ�̓o�[�W�����Q�ȑO�����ׂ�
	if( DXA->Head.Head != DXAHEAD )
	{
		// �o�[�W�����Q�ȑO�����ׂ�
		_MEMSET( DXA->Key, 0xff, DXA_KEYSTR_LENGTH ) ;

		ReadOnlyFileAccessSeek( DXA->ASyncOpenFilePointer, 0L, SEEK_SET ) ;
		DXA_KeyConvFileRead( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, DXA->ASyncOpenFilePointer, DXA->Key, 0 ) ;

		// �o�[�W�����Q�ȑO�ł��Ȃ��ꍇ�̓G���[
		if( DXA->Head.Head != DXAHEAD )
		{
			goto ERR ;
		}
	}

	// �o�[�W����6�ȍ~���ǂ����ŏ����𕪊�
	if( DXA->Head.Version >= 0x0006 )
	{
		DXA->V5Flag = FALSE ;

		// �o�[�W��������
		if( DXA->Head.Version > DXAVER )
		{
			goto ERR ;
		}

		// �o�[�W�������S�ȏォ�ǂ����œǂݍ��ގc��̃w�b�_�T�C�Y��ύX
		DXA_KeyConvFileRead( ( BYTE * )&DXA->Head + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER6_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->ASyncOpenFilePointer, DXA->Key, DXARC_ID_AND_VERSION_SIZE ) ;
		switch( DXA->Head.CharCodeFormat )
		{
		case DX_CHARCODEFORMAT_UHC :
		case DX_CHARCODEFORMAT_BIG5 :
		case DX_CHARCODEFORMAT_GB2312 :
		case DX_CHARCODEFORMAT_SHIFTJIS :
		case DX_CHARCODEFORMAT_UTF16LE :
		case DX_CHARCODEFORMAT_UTF16BE :
		case DX_CHARCODEFORMAT_WINDOWS_1252 :
		case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
		case DX_CHARCODEFORMAT_UTF8 :
		case DX_CHARCODEFORMAT_UTF32LE :
		case DX_CHARCODEFORMAT_UTF32BE :
			DXA->CharCodeFormat = ( int )DXA->Head.CharCodeFormat ;
			break ;

		default :
			DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
			break ;
		}
		
		// ���e�[�u���̃T�C�Y���̃��������m�ۂ���
		DXA->Table.Top = ( BYTE * )DXALLOC( DXA->Head.HeadSize ) ;
		if( DXA->Table.Top == NULL )
		{
			goto ERR ;
		}
		
		// ���e�[�u�����������ɓǂݍ���
		ReadOnlyFileAccessSeek( DXA->ASyncOpenFilePointer, ( LONGLONG )DXA->Head.FileNameTableStartAddress, SEEK_SET ) ;
		DXA_KeyConvFileRead( DXA->Table.Top, DXA->Head.HeadSize, DXA->ASyncOpenFilePointer, DXA->Key, 0 ) ;

		// ���e�[�u���̃A�h���X���Z�b�g����
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->Head.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->Head.DirectoryTableStartAddress ;

		// �J�����g�f�B���N�g���̃Z�b�g
		DXA->CurrentDirectory = ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ;
	}
	else
	{
		DXA->V5Flag = TRUE ;

		// �o�[�W��������
		if( DXA->HeadV5.Version > DXAVER_VER5 )
		{
			goto ERR ;
		}

		// �o�[�W�������S�ȏォ�ǂ����œǂݍ��ގc��̃w�b�_�T�C�Y��ύX
		if( DXA->HeadV5.Version >= 0x0004 )
		{
			DXA_KeyConvFileRead( ( BYTE * )&DXA->HeadV5 + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER4_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->ASyncOpenFilePointer, DXA->Key, DXARC_ID_AND_VERSION_SIZE ) ;
			switch( DXA->HeadV5.CharCodeFormat )
			{
			case DX_CHARCODEFORMAT_UHC :
			case DX_CHARCODEFORMAT_BIG5 :
			case DX_CHARCODEFORMAT_GB2312 :
			case DX_CHARCODEFORMAT_SHIFTJIS :
			case DX_CHARCODEFORMAT_UTF16LE :
			case DX_CHARCODEFORMAT_UTF16BE :
			case DX_CHARCODEFORMAT_WINDOWS_1252 :
			case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
			case DX_CHARCODEFORMAT_UTF8 :
			case DX_CHARCODEFORMAT_UTF32LE :
			case DX_CHARCODEFORMAT_UTF32BE :
				DXA->CharCodeFormat = ( int )DXA->HeadV5.CharCodeFormat ;
				break ;

			default :
				DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
				break ;
			}
		}
		else
		{
			DXA_KeyConvFileRead( ( BYTE * )&DXA->HeadV5 + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER3_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->ASyncOpenFilePointer, DXA->Key, DXARC_ID_AND_VERSION_SIZE ) ;
			DXA->HeadV5.CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
			DXA->CharCodeFormat        = DX_CHARCODEFORMAT_SHIFTJIS ;
		}
		
		// ���e�[�u���̃T�C�Y���̃��������m�ۂ���
		DXA->Table.Top = ( BYTE * )DXALLOC( DXA->HeadV5.HeadSize ) ;
		if( DXA->Table.Top == NULL )
		{
			goto ERR ;
		}
		
		// ���e�[�u�����������ɓǂݍ���
		ReadOnlyFileAccessSeek( DXA->ASyncOpenFilePointer, DXA->HeadV5.FileNameTableStartAddress, SEEK_SET ) ;
		if( DXA->HeadV5.Version >= 0x0005 )
		{
			DXA_KeyConvFileRead( DXA->Table.Top, DXA->HeadV5.HeadSize, DXA->ASyncOpenFilePointer, DXA->Key, 0 ) ;
		}
		else
		{
			DXA_KeyConvFileRead( DXA->Table.Top, DXA->HeadV5.HeadSize, DXA->ASyncOpenFilePointer, DXA->Key ) ;
		}

		// ���e�[�u���̃A�h���X���Z�b�g����
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->HeadV5.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->HeadV5.DirectoryTableStartAddress ;

		// �J�����g�f�B���N�g���̃Z�b�g
		DXA->CurrentDirectoryV5 = ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ;
	}

	// ���߂ăt�@�C�����ۂ��Ɠǂݍ���
	ReadOnlyFileAccessSeek( DXA->ASyncOpenFilePointer, 0L, SEEK_SET ) ;
	ReadOnlyFileAccessRead( DXA->MemoryImage, ( size_t )DXA->MemoryImageSize, 1, DXA->ASyncOpenFilePointer );

	// �t�@�C���񓯊��I�[�v�������Ƃ������Ƃ��Z�b�g���Ă���
	DXA->ASyncOpenFlag = TRUE ;

	DXA->MemoryOpenFlag					= TRUE ;			// �������C���[�W����J���Ă���t���O�𗧂Ă�
	DXA->UserMemoryImageFlag			= FALSE ;			// ���[�U�[�̃C���[�W����J�����킯�ł͂Ȃ��̂Ńt���O��|��
	DXA->MemoryImageCopyFlag			= FALSE ;			// �������C���[�W���R�s�[���Ďg�p���Ă���t���O��|��
	DXA->MemoryImageReadOnlyFlag        = FALSE ;			// �������C���[�W��ǂݎ���p�ɂ��邩�ǂ����̃t���O��|��

	// �����I�[�v���̏ꍇ�͂����ŊJ���I���̂�҂�
	if( ASyncThread == FALSE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	// �I��
	return 0 ;

ERR :
	if( DXA->ASyncOpenFilePointer )
	{
		ReadOnlyFileAccessClose( DXA->ASyncOpenFilePointer );
		DXA->ASyncOpenFilePointer = 0;
	}

	if( DXA->MemoryImage )
	{
		DXFREE( DXA->MemoryImage );
		DXA->MemoryImage = 0;
	}

	DXA->ASyncOpenFlag = FALSE ;

	// �I��
	return -1 ;
}

// �A�[�J�C�u�t�@�C�����J��( 0:����  -1:���s )
extern int DXA_OpenArchiveFromFile( DXARC *DXA, const wchar_t *ArchivePath, const char *KeyString )
{
	// ���ɂȂ�炩�̃A�[�J�C�u���J���Ă����ꍇ�̓G���[
	if( DXA->WinFilePointer__ != 0 || DXA->MemoryImage )
	{
		return -1 ;
	}

	// �w�b�_�̏�����
	_MEMSET( &DXA->Head, 0, sizeof( DXA->Head ) ) ;

	// �A�[�J�C�u�t�@�C�����J�����Ǝ��݂�
	DXA->WinFilePointer__ = ReadOnlyFileAccessOpen( ArchivePath, FALSE, TRUE, FALSE ) ;
	if( DXA->WinFilePointer__ == 0 )
	{
		return -1 ;
	}

	// �t�@�C���p�X��ۑ�
	_WCSCPY_S( DXA->FilePath, sizeof( DXA->FilePath ), ArchivePath ) ;

	// ��������̍쐬
	DXA_KeyCreate( KeyString, DXA->Key ) ;

	// �h�c�ƃo�[�W�����ԍ�����������ǂݍ���
	DXA_KeyConvFileRead( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, DXA->WinFilePointer__, DXA->Key, 0 ) ;

	// �h�c�̌���
	if( DXA->Head.Head != DXAHEAD )
	{
		// �o�[�W�����Q�ȑO�����ׂ�
		_MEMSET( DXA->Key, 0xff, DXA_KEYSTR_LENGTH ) ;

		ReadOnlyFileAccessSeek( DXA->WinFilePointer__, 0L, SEEK_SET ) ;
		DXA_KeyConvFileRead( &DXA->Head, DXARC_ID_AND_VERSION_SIZE, DXA->WinFilePointer__, DXA->Key, 0 ) ;

		// �o�[�W�����Q�ȑO�ł��Ȃ��ꍇ�̓G���[
		if( DXA->Head.Head != DXAHEAD )
		{
			goto ERR ;
		}
	}

	// �o�[�W����6�ȍ~���ǂ����ŏ����𕪊�
	if( DXA->Head.Version >= 0x0006 )
	{
		DXA->V5Flag = FALSE ;

		// �o�[�W��������
		if( DXA->Head.Version > DXAVER )
		{
			goto ERR ;
		}

		// �o�[�W�������S�ȏォ�ǂ����œǂݍ��ގc��̃w�b�_�T�C�Y��ύX
		DXA_KeyConvFileRead( ( BYTE * )&DXA->Head + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER6_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->WinFilePointer__, DXA->Key, DXARC_ID_AND_VERSION_SIZE ) ;
		switch( DXA->Head.CharCodeFormat )
		{
		case DX_CHARCODEFORMAT_UHC :
		case DX_CHARCODEFORMAT_BIG5 :
		case DX_CHARCODEFORMAT_GB2312 :
		case DX_CHARCODEFORMAT_SHIFTJIS :
		case DX_CHARCODEFORMAT_UTF16LE :
		case DX_CHARCODEFORMAT_UTF16BE :
		case DX_CHARCODEFORMAT_WINDOWS_1252 :
		case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
		case DX_CHARCODEFORMAT_UTF8 :
		case DX_CHARCODEFORMAT_UTF32LE :
		case DX_CHARCODEFORMAT_UTF32BE :
			DXA->CharCodeFormat = ( int )DXA->Head.CharCodeFormat ;
			break ;

		default :
			DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
			break ;
		}

		// ���e�[�u���̃T�C�Y���̃��������m�ۂ���
		DXA->Table.Top = ( BYTE * )DXALLOC( DXA->Head.HeadSize ) ;
		if( DXA->Table.Top == NULL )
		{
			goto ERR ;
		}
		
		// ���e�[�u�����������ɓǂݍ���
		ReadOnlyFileAccessSeek( DXA->WinFilePointer__, ( LONGLONG )DXA->Head.FileNameTableStartAddress, SEEK_SET ) ;
		DXA_KeyConvFileRead( DXA->Table.Top, DXA->Head.HeadSize, DXA->WinFilePointer__, DXA->Key, 0 ) ;

		// ���e�[�u���̃A�h���X���Z�b�g����
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->Head.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->Head.DirectoryTableStartAddress ;

		// �J�����g�f�B���N�g���̃Z�b�g
		DXA->CurrentDirectory = ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ;
	}
	else
	{
		DXA->V5Flag = TRUE ;

		// �o�[�W��������
		if( DXA->HeadV5.Version > DXAVER_VER5 )
		{
			goto ERR ;
		}

		// �o�[�W�������S�ȏォ�ǂ����œǂݍ��ގc��̃w�b�_�T�C�Y��ύX
		if( DXA->HeadV5.Version >= 0x0004 )
		{
			DXA_KeyConvFileRead( ( BYTE * )&DXA->HeadV5 + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER4_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->WinFilePointer__, DXA->Key, DXARC_ID_AND_VERSION_SIZE ) ;
			switch( DXA->HeadV5.CharCodeFormat )
			{
			case DX_CHARCODEFORMAT_UHC :
			case DX_CHARCODEFORMAT_BIG5 :
			case DX_CHARCODEFORMAT_GB2312 :
			case DX_CHARCODEFORMAT_SHIFTJIS :
			case DX_CHARCODEFORMAT_UTF16LE :
			case DX_CHARCODEFORMAT_UTF16BE :
			case DX_CHARCODEFORMAT_WINDOWS_1252 :
			case DX_CHARCODEFORMAT_ISO_IEC_8859_15 :
			case DX_CHARCODEFORMAT_UTF8 :
			case DX_CHARCODEFORMAT_UTF32LE :
			case DX_CHARCODEFORMAT_UTF32BE :
				DXA->CharCodeFormat = ( int )DXA->HeadV5.CharCodeFormat ;
				break ;

			default :
				DXA->CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
				break ;
			}
		}
		else
		{
			DXA_KeyConvFileRead( ( BYTE * )&DXA->HeadV5 + DXARC_ID_AND_VERSION_SIZE, DXARC_HEAD_VER3_SIZE - DXARC_ID_AND_VERSION_SIZE, DXA->WinFilePointer__, DXA->Key, DXARC_ID_AND_VERSION_SIZE ) ;
			DXA->HeadV5.CharCodeFormat = DX_CHARCODEFORMAT_SHIFTJIS ;
			DXA->CharCodeFormat        = DX_CHARCODEFORMAT_SHIFTJIS ;
		}

		// ���e�[�u���̃T�C�Y���̃��������m�ۂ���
		DXA->Table.Top = ( BYTE * )DXALLOC( DXA->HeadV5.HeadSize ) ;
		if( DXA->Table.Top == NULL )
		{
			goto ERR ;
		}
		
		// ���e�[�u�����������ɓǂݍ���
		ReadOnlyFileAccessSeek( DXA->WinFilePointer__, DXA->HeadV5.FileNameTableStartAddress, SEEK_SET ) ;
		if( DXA->HeadV5.Version >= 0x0005 )
		{
			DXA_KeyConvFileRead( DXA->Table.Top, DXA->HeadV5.HeadSize, DXA->WinFilePointer__, DXA->Key, 0 ) ;
		}
		else
		{
			DXA_KeyConvFileRead( DXA->Table.Top, DXA->HeadV5.HeadSize, DXA->WinFilePointer__, DXA->Key ) ;
		}

		// ���e�[�u���̃A�h���X���Z�b�g����
		DXA->Table.NameTable		= DXA->Table.Top ;
		DXA->Table.FileTable		= DXA->Table.NameTable + DXA->HeadV5.FileTableStartAddress ;
		DXA->Table.DirectoryTable	= DXA->Table.NameTable + DXA->HeadV5.DirectoryTableStartAddress ;

		// �J�����g�f�B���N�g���̃Z�b�g
		DXA->CurrentDirectoryV5 = ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ;
	}

	DXA->MemoryOpenFlag					= FALSE ;			// �������C���[�W����J���Ă���t���O��|��
	DXA->UserMemoryImageFlag			= FALSE ;			// ���[�U�[�̃C���[�W����J�����킯�ł͂Ȃ��̂Ńt���O��|��
	DXA->MemoryImageCopyFlag			= FALSE ;			// �������C���[�W���R�s�[���Ďg�p���Ă���t���O��|��
	DXA->MemoryImageReadOnlyFlag        = FALSE ;			// �������C���[�W��ǂݎ���p�ɂ��邩�ǂ����̃t���O��|��

	// �I��
	return 0 ;

ERR :
	if( DXA->WinFilePointer__ != 0 )
	{
		ReadOnlyFileAccessClose( DXA->WinFilePointer__ ) ;
		DXA->WinFilePointer__ = 0 ;
	}

	if( DXA->Table.Top != NULL )
	{
		DXFREE( DXA->Table.Top ) ;
		DXA->Table.Top = NULL ;
	}
	
	// �I��
	return -1 ;
}

// �A�[�J�C�u�t�@�C�������
extern int DXA_CloseArchive( DXARC *DXA )
{
	// ���ɕ��Ă����牽�������I��
	if( DXA->WinFilePointer__ == 0 && DXA->MemoryImage == NULL )
	{
		return 0 ;
	}

	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	// ����������J���Ă��邩�ǂ����ŏ����𕪊�
	if( DXA->MemoryOpenFlag == TRUE )
	{
		// �A�[�J�C�u�v���O�������������ɓǂݍ��񂾏ꍇ�Ƃ����łȂ��ꍇ�ŏ����𕪊�
		if( DXA->UserMemoryImageFlag == TRUE )
		{
			// ���[�U�[����n���ꂽ�f�[�^�������ꍇ

			// �A�[�J�C�u�C���[�W���R�s�[���Ďg�p���Ă����ꍇ�͒P���ɉ������
			if( DXA->MemoryImageCopyFlag )
			{
				if( DXA->MemoryImage != NULL )
				{
					DXFREE( DXA->MemoryImage ) ;
					DXA->MemoryImage = NULL ;
				}
			}
			else
			{
				if( DXA->MemoryImageReadOnlyFlag )
				{
					// �ǂݎ���p�̏ꍇ�͏��e�[�u���p�Ɋm�ۂ��Ă��������������
					if( DXA->Table.Top != NULL )
					{
						DXFREE( DXA->Table.Top ) ;
						DXA->Table.Top = NULL ;
					}
				}
				else
				{
					// �n���ꂽ�������A�h���X��̓��e�𒼐ڎg�p���Ă����ꍇ��
					// ���]�����f�[�^�����ɖ߂�
					if( DXA->V5Flag == FALSE )
					{
						DXA_DirectoryKeyConv( DXA, ( DXARC_DIRECTORY * )DXA->Table.DirectoryTable ) ;
						DXA_KeyConv( DXA->Table.Top, DXA->Head.HeadSize, 0, DXA->Key ) ;
					}
					else
					{
						if( DXA->HeadV5.Version >= 0x0005 )
						{
							DXA_DirectoryKeyConvV5( DXA, ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ) ;
							DXA_KeyConv( DXA->Table.Top, DXA->HeadV5.HeadSize, 0, DXA->Key ) ;
						}
						else
						{
							DXA_KeyConv( DXA->MemoryImage, DXA->MemoryImageSize, 0, DXA->Key ) ;
						}
					}
				}
			}
		}
		else
		{
			// �A�[�J�C�u�v���O�������������ɓǂݍ��񂾏ꍇ

			// �m�ۂ��Ă������������J������
			DXFREE( DXA->Table.Top ) ;
			DXFREE( DXA->MemoryImage ) ;
		}
	}
	else
	{
		// �A�[�J�C�u�t�@�C�������
		ReadOnlyFileAccessClose( DXA->WinFilePointer__ ) ;

		// ���e�[�u�����i�[���Ă����������̈�����
		DXFREE( DXA->Table.Top ) ;
	}

	// ������
	_MEMSET( DXA, 0, sizeof( DXARC ) ) ;

	// �I��
	return 0 ;
}

// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
static int DXA_ChangeCurrentDirectoryFast( DXARC *DXA, DXARC_SEARCHDATA *SearchData )
{
	int i, j, k, Num ;
	BYTE *NameData, *PathData ;
	WORD PackNum, Parity ;
	DWORD FileHeadSize ;

	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	PackNum  = SearchData->PackNum ;
	Parity   = SearchData->Parity ;
	PathData = SearchData->FileName ;

	// �J�����g�f�B���N�g�����瓯���̃f�B���N�g����T��
	if( DXA->V5Flag )
	{
		DXARC_FILEHEAD_VER5 *FileH ;

		FileH        = ( DXARC_FILEHEAD_VER5 * )( DXA->Table.FileTable + DXA->CurrentDirectoryV5->FileHeadAddress ) ;
		Num          = ( int )DXA->CurrentDirectoryV5->FileHeadNum ;
		FileHeadSize = ( DWORD )( DXA->HeadV5.Version >= 0x0002 ? DXARC_FILEHEAD_VER2_SIZE : DXARC_FILEHEAD_VER1_SIZE ) ;
		for( i = 0 ; i < Num ; i ++, FileH = (DXARC_FILEHEAD_VER5 *)( (BYTE *)FileH + FileHeadSize ) )
		{
			// �f�B���N�g���`�F�b�N
			if( ( FileH->Attributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
			{
				continue ;
			}

			// �����񐔂ƃp���e�B�`�F�b�N
			NameData = DXA->Table.NameTable + FileH->NameAddress ;
			if( PackNum != ( ( WORD * )NameData )[ 0 ] ||
				Parity  != ( ( WORD * )NameData )[ 1 ] )
			{
				continue ;
			}

			// ������`�F�b�N
			NameData += 4 ;
			for( j = 0, k = 0 ; j < PackNum ; j ++, k += 4 )
			{
				if( *( ( DWORD * )&PathData[ k ] ) != *( ( DWORD * )&NameData[ k ] ) )
				{
					break ;
				}
			}

			// �K�������f�B���N�g�����������炱���ŏI��
			if( PackNum == j )
			{
				break ;
			}
		}

		// ����������G���[
		if( i == Num )
		{
			return -1 ;
		}

		// �݂�����J�����g�f�B���N�g����ύX
		DXA->CurrentDirectoryV5 = ( DXARC_DIRECTORY_VER5 * )( DXA->Table.DirectoryTable + FileH->DataAddress ) ;
	}
	else
	{
		DXARC_FILEHEAD *FileH ;

		FileH        = ( DXARC_FILEHEAD * )( DXA->Table.FileTable + DXA->CurrentDirectory->FileHeadAddress ) ;
		Num          = ( int )DXA->CurrentDirectory->FileHeadNum ;
		FileHeadSize = DXARC_FILEHEAD_VER6_SIZE ;
		for( i = 0 ; i < Num ; i ++, FileH = (DXARC_FILEHEAD *)( (BYTE *)FileH + FileHeadSize ) )
		{
			// �f�B���N�g���`�F�b�N
			if( ( FileH->Attributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
			{
				continue ;
			}

			// �����񐔂ƃp���e�B�`�F�b�N
			NameData = DXA->Table.NameTable + FileH->NameAddress ;
			if( PackNum != ( ( WORD * )NameData )[ 0 ] ||
				Parity  != ( ( WORD * )NameData )[ 1 ] )
			{
				continue ;
			}

			// ������`�F�b�N
			NameData += 4 ;
			for( j = 0, k = 0 ; j < PackNum ; j ++, k += 4 )
			{
				if( *( ( DWORD * )&PathData[ k ] ) != *( ( DWORD * )&NameData[ k ] ) )
				{
					break ;
				}
			}

			// �K�������f�B���N�g�����������炱���ŏI��
			if( PackNum == j )
			{
				break ;
			}
		}

		// ����������G���[
		if( i == Num )
		{
			return -1 ;
		}

		// �݂�����J�����g�f�B���N�g����ύX
		DXA->CurrentDirectory = ( DXARC_DIRECTORY * )( DXA->Table.DirectoryTable + FileH->DataAddress ) ;
	}

	// ����I��
	return 0 ;
}

// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
extern int DXA_ChangeCurrentDir( DXARC *DXA, int CharCodeFormat, const char *DirPath )
{
	BYTE TempBuffer[ 4096 ] ;
	const BYTE *DirPathB ;

	// �����R�[�h�`�����قȂ�ꍇ�͕ϊ�����
	if( CharCodeFormat != DXA->CharCodeFormat )
	{
		ConvString( DirPath, CharCodeFormat, ( char * )TempBuffer, sizeof( TempBuffer ), DXA->CharCodeFormat ) ;
		DirPathB = TempBuffer ;
	}
	else
	{
		DirPathB = ( const BYTE * )DirPath ;
	}

	return DXA_ChangeCurrentDirectoryBase( DXA, DirPathB, true ) ;
}

// �A�[�J�C�u���̃f�B���N�g���p�X��ύX����( 0:����  -1:���s )
static int DXA_ChangeCurrentDirectoryBase( DXARC *DXA, const BYTE *DirectoryPath, bool ErrorIsDirectoryReset, DXARC_SEARCHDATA *LastSearchData )
{
	DXARC_DIRECTORY *OldDir ;
	DXARC_SEARCHDATA SearchData ;

	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	// �����ɗ��܂�p�X�������疳��
	if( CL_strcmp_str2_ascii( DXA->CharCodeFormat, ( const char * )DirectoryPath, ( const char * )Ascii_DotStr ) == 0 )
	{
		return 0 ;
	}

	// �w\ or /�x�����̏ꍇ�̓��[�g�f�B���N�g���ɖ߂�
	if( CL_strcmp_str2_ascii( DXA->CharCodeFormat, ( const char * )DirectoryPath, ( const char * )Ascii_EnStr    ) == 0 ||
		CL_strcmp_str2_ascii( DXA->CharCodeFormat, ( const char * )DirectoryPath, ( const char * )Ascii_SlashStr ) == 0 )
	{
		if( DXA->V5Flag )
		{
			DXA->CurrentDirectoryV5 = ( DXARC_DIRECTORY_VER5 * )DXA->Table.DirectoryTable ;
		}
		else
		{
			DXA->CurrentDirectory   = ( DXARC_DIRECTORY      * )DXA->Table.DirectoryTable ;
		}
		return 0 ;
	}

	// ���Ɉ������p�X�������珈���𕪊�
	if( CL_strcmp_str2_ascii( DXA->CharCodeFormat, ( const char * )DirectoryPath, ( const char * )Ascii_DoubleDotStr ) == 0 )
	{
		if( DXA->V5Flag )
		{
			// ���[�g�f�B���N�g���ɋ�����G���[
			if( DXA->CurrentDirectoryV5->ParentDirectoryAddress == 0xffffffff ) return -1 ;
			
			// �e�f�B���N�g�����������炻����Ɉڂ�
			DXA->CurrentDirectoryV5 = ( DXARC_DIRECTORY_VER5 * )( DXA->Table.DirectoryTable + DXA->CurrentDirectoryV5->ParentDirectoryAddress ) ;
		}
		else
		{
			// ���[�g�f�B���N�g���ɋ�����G���[
			if( DXA->CurrentDirectory->ParentDirectoryAddress == NONE_PAL ) return -1 ;
			
			// �e�f�B���N�g�����������炻����Ɉڂ�
			DXA->CurrentDirectory   = ( DXARC_DIRECTORY      * )( DXA->Table.DirectoryTable + DXA->CurrentDirectory->ParentDirectoryAddress ) ;
		}
		return 0 ;
	}

	// ����ȊO�̏ꍇ�͎w��̖��O�̃f�B���N�g����T��
	
	// �ύX�ȑO�̃f�B���N�g����ۑ����Ă���
	OldDir = DXA->CurrentDirectory ;

	// �p�X���Ɂw\�x�����邩�ǂ����ŏ����𕪊�
	if( CL_strchr( DXA->CharCodeFormat, ( const char * )DirectoryPath, '\\' ) == NULL &&
		CL_strchr( DXA->CharCodeFormat, ( const char * )DirectoryPath, '/'  ) == NULL )
	{
		// �t�@�C������������p�̌`���ɕϊ�����
		DXA_ConvSearchData( DXA, &SearchData, DirectoryPath, NULL ) ;

		// �f�B���N�g����ύX
		if( DXA_ChangeCurrentDirectoryFast( DXA, &SearchData ) < 0 ) goto ERR ;
	}
	else
	{
		// \ or / ������ꍇ�͌q�������f�B���N�g������ÂύX���Ă䂭
		int i ;
		int StrLength ;
		int CharBytes ;
		int CharBytes2 ;
		DWORD CharCode ;
		DWORD CharCode2 ;

		i = 0 ;

		// ���[�v
		for(;;)
		{
			// ��������擾����
			DXA_ConvSearchData( DXA, &SearchData, &DirectoryPath[ i ], &StrLength ) ;
			i += StrLength ;

			// ���������[�� \ or / �������ꍇ�̓��[�g�f�B���N�g���ɗ��Ƃ�
			CharCode = GetCharCode( ( const char * )&DirectoryPath[ i ], DXA->CharCodeFormat, &CharBytes ) ;
			if( StrLength == 0 && ( CharCode == '\\' || CharCode == '/' ) )
			{
				BYTE EnStr[ 16 ] ;

				ConvString( ( const char * )Ascii_EnStr, DX_CHARCODEFORMAT_ASCII, ( char * )EnStr, sizeof( EnStr ), DXA->CharCodeFormat ) ;
				DXA_ChangeCurrentDirectoryBase( DXA, EnStr, false ) ;
			}
			else
			{
				// ����ȊO�̏ꍇ�͕��ʂɃf�B���N�g���ύX
				if( DXA_ChangeCurrentDirectoryFast( DXA, &SearchData ) < 0 )
				{
					// �G���[���N���āA�X�ɃG���[���N�������Ɍ��̃f�B���N�g���ɖ߂���
					// �t���O�������Ă���ꍇ�͌��̃f�B���N�g���ɖ߂�
					if( ErrorIsDirectoryReset == true )
					{
						DXA->CurrentDirectory = OldDir ;
					}

					// �G���[�I��
					goto ERR ;
				}
			}

			// �����I�[�����ŏI�������ꍇ�̓��[�v���甲����
			// ���͂��� \ or / �����Ȃ��ꍇ�����[�v���甲����
			if( CharCode == '\0' )
			{
				break ;
			}
			else
			{
				CharCode2 = GetCharCode( ( const char * )&DirectoryPath[ i + CharBytes ], DXA->CharCodeFormat, &CharBytes2 ) ;
				if( ( CharCode == '\\' && CharCode2 == '\0' ) ||
					( CharCode == '/'  && CharCode2 == '\0' ) )
				{
					break ;
				}
			}
			i += CharBytes ;
		}
	}

	if( LastSearchData != NULL )
	{
		_MEMCPY( LastSearchData->FileName, SearchData.FileName, ( size_t )( SearchData.PackNum * 4 ) ) ;
		LastSearchData->Parity  = SearchData.Parity ;
		LastSearchData->PackNum = SearchData.PackNum ;
	}

	// ����I��
	return 0 ;

ERR:
	if( LastSearchData != NULL )
	{
		_MEMCPY( LastSearchData->FileName, SearchData.FileName, ( size_t )( SearchData.PackNum * 4 ) ) ;
		LastSearchData->Parity  = SearchData.Parity ;
		LastSearchData->PackNum = SearchData.PackNum ;
	}

	// �G���[�I��
	return -1 ;
}
		
// �A�[�J�C�u���̃J�����g�f�B���N�g���p�X���擾����
//extern int DXA_GetCurrentDir( DXARC *DXA, char *DirPathBuffer, int BufferSize )
//{
//	char DirPath[FILEPATH_MAX] ;
//	int Depth, i ;
//
//	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
//	if( DXA->ASyncOpenFlag == TRUE )
//	{
//		while( DXA_CheckIdle( DXA ) == FALSE )
//		{
//			Thread_Sleep( 0 ) ;
//		}
//	}
//
//	if( DXA->V5Flag )
//	{
//		DXARC_DIRECTORY_VER5 *Dir[200], *DirTempP ;
//
//		// ���[�g�f�B���N�g���ɒ����܂Ō�������
//		Depth = 0 ;
//		DirTempP = DXA->CurrentDirectoryV5 ;
//		while( DirTempP->DirectoryAddress != 0xffffffff && DirTempP->DirectoryAddress != 0 )
//		{
//			Dir[Depth] = DirTempP ;
//			DirTempP = ( DXARC_DIRECTORY_VER5 * )( DXA->Table.DirectoryTable + DirTempP->ParentDirectoryAddress ) ;
//			Depth ++ ;
//		}
//		
//		// �p�X����A������
//		DirPath[0] = '\0' ;
//		for( i = Depth - 1 ; i >= 0 ; i -- )
//		{
//			_STRCAT( DirPath, "\\" ) ;
//			_STRCAT( DirPath, (char *)DXA->Table.NameTable + ((DXARC_FILEHEAD_VER5 *)( DXA->Table.FileTable + Dir[i]->DirectoryAddress ))->NameAddress ) ;
//		}
//	}
//	else
//	{
//		DXARC_DIRECTORY *Dir[200], *DirTempP ;
//
//		// ���[�g�f�B���N�g���ɒ����܂Ō�������
//		Depth = 0 ;
//		DirTempP = DXA->CurrentDirectory ;
//		while( DirTempP->DirectoryAddress != 0xffffffff && DirTempP->DirectoryAddress != 0 )
//		{
//			Dir[Depth] = DirTempP ;
//			DirTempP = ( DXARC_DIRECTORY * )( DXA->Table.DirectoryTable + DirTempP->ParentDirectoryAddress ) ;
//			Depth ++ ;
//		}
//		
//		// �p�X����A������
//		DirPath[0] = '\0' ;
//		for( i = Depth - 1 ; i >= 0 ; i -- )
//		{
//			_STRCAT( DirPath, "\\" ) ;
//			_STRCAT( DirPath, (char *)DXA->Table.NameTable + ((DXARC_FILEHEAD *)( DXA->Table.FileTable + Dir[i]->DirectoryAddress ))->NameAddress ) ;
//		}
//	}
//
//	// �o�b�t�@�̒������O���A����������Ȃ��Ƃ��̓f�B���N�g�����̒�����Ԃ�
//	if( BufferSize == 0 || BufferSize < (int)_STRLEN( DirPath ) )
//	{
//		return _STRLEN( DirPath ) + 1 ;
//	}
//	else
//	{
//		// �f�B���N�g�������o�b�t�@�ɓ]������
//		_STRCPY( DirPathBuffer, DirPath ) ;
//	}
//
//	// �I��
//	return 0 ;
//}

// �A�[�J�C�u���̃I�u�W�F�N�g����������( -1:�G���[ -1�ȊO:DXA�����n���h�� )
extern DWORD_PTR DXA_FindFirst( DXARC *DXA, const BYTE *FilePath, FILEINFOW *Buffer )
{
	DXA_FINDDATA *find ;
	BYTE Dir[  FILEPATH_MAX ] ;
	BYTE Name[ FILEPATH_MAX ] ;
	int CharBytes ;

	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	// �������̊m��
	find = ( DXA_FINDDATA * )DXALLOC( sizeof( *find ) ) ;
	if( find == NULL )
	{
		return ( DWORD_PTR )-1 ;
	}

	find->Container = DXA;
	DXA_DIR_AnalysisFileNameAndDirPath( DXA, FilePath, Name, sizeof( Name ), Dir, sizeof( Dir ) );

	// �S�đ啶���ɂ���
	CL_strupr( DXA->CharCodeFormat, ( char * )Dir  ) ;
	CL_strupr( DXA->CharCodeFormat, ( char * )Name ) ;

	// �����Ώۂ̃f�B���N�g�����擾
	if( GetCharCode( ( const char * )Dir, DXA->CharCodeFormat, &CharBytes ) == '\0' )
	{
		find->Directory = DXA->CurrentDirectory ;
	}
	else
	{
		DXARC_DIRECTORY *OldDir;

		OldDir = DXA->CurrentDirectory;

		// �w��̃f�B���N�g���������ꍇ�̓G���[
		if( DXA_ChangeCurrentDirectoryBase( DXA, Dir, false ) == -1 )
		{
			DXFREE( find ) ;
			return ( DWORD_PTR )-1 ;
		}

		find->Directory       = DXA->CurrentDirectory ;
		DXA->CurrentDirectory = OldDir ;
	}

	find->ObjectCount = 0;
	CL_strcpy_s( DXA->CharCodeFormat, ( char * )find->SearchStr, sizeof( find->SearchStr ), ( const char * )Name ) ;

	// �K������ŏ��̃t�@�C������������
	if( DXA_FindProcess( find, Buffer ) == -1 )
	{
		DXFREE( find );
		return ( DWORD_PTR )-1 ;
	}
	find->ObjectCount ++ ;

	// �n���h����Ԃ�
	return ( DWORD_PTR )find ;
}

// �A�[�J�C�u���̃I�u�W�F�N�g����������( -1:�G���[ 0:���� )
extern int DXA_FindNext( DWORD_PTR DxaFindHandle, FILEINFOW *Buffer )
{
	DXA_FINDDATA *find;

	find = ( DXA_FINDDATA * )DxaFindHandle ;
	if( DXA_FindProcess( find, Buffer ) == -1 )
	{
		return -1 ;
	}
	find->ObjectCount ++ ;

	return 0;
}

// �A�[�J�C�u���̃I�u�W�F�N�g�������I������
extern int DXA_FindClose( DWORD_PTR DxaFindHandle )
{
	DXA_FINDDATA *find ;

	find = ( DXA_FINDDATA * )DxaFindHandle ;
	DXFREE( find ) ;

	return 0 ;
}

// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C�����������ɓǂݍ���( -1:�G���[ 0�ȏ�:�t�@�C���T�C�Y )
//extern int DXA_LoadFile( DXARC *DXA, const char *FilePath, void *Buffer, ULONGLONG BufferSize )
//{
//	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
//	if( DXA->ASyncOpenFlag == TRUE )
//	{
//		while( DXA_CheckIdle( DXA ) == FALSE )
//		{
//			Thread_Sleep( 0 ) ;
//		}
//	}
//
//	if( DXA->V5Flag )
//	{
//		DXARC_FILEHEAD_VER5 *FileH ;
//
//		// �w��̃t�@�C���̏��𓾂�
//		FileH = DXA_GetFileHeaderV5( DXA, FilePath ) ;
//		if( FileH == NULL ) return -1 ;
//
//		// �t�@�C���T�C�Y������Ă��邩���ׂ�A����Ă��Ȃ����A�o�b�t�@�A���̓T�C�Y���O��������T�C�Y��Ԃ�
//		if( BufferSize < FileH->DataSize || BufferSize == 0 || Buffer == NULL )
//		{
//			return ( int )FileH->DataSize ;
//		}
//		
//		// ����Ă���ꍇ�̓o�b�t�@�[�ɓǂݍ���
//
//		// �t�@�C�������k����Ă��邩�ǂ����ŏ����𕪊�
//		if( DXA->HeadV5.Version >= 0x0002 && FileH->PressDataSize != 0xffffffff )
//		{
//			// ���k����Ă���ꍇ
//
//			// ��������ɓǂݍ���ł��邩�ǂ����ŏ����𕪊�
//			if( DXA->MemoryOpenFlag == TRUE )
//			{
//				if( DXA->MemoryImageReadOnlyFlag )
//				{
//					void *temp ;
//
//					// ���k�f�[�^���������ɓǂݍ���ł���𓀂���
//
//					// ���k�f�[�^�����܂郁�����̈�̊m��
//					temp = DXALLOC( FileH->PressDataSize ) ;
//
//					// ���k�f�[�^�̓]��
//					_MEMCPY( temp, (BYTE *)DXA->MemoryImage + DXA->HeadV5.DataStartAddress + FileH->DataAddress, FileH->DataSize ) ;
//					if( DXA->HeadV5.Version >= 0x0005 )
//					{
//						DXA_KeyConv( temp, FileH->PressDataSize,                                   FileH->DataSize, DXA->Key ) ;
//					}
//					else
//					{
//						DXA_KeyConv( temp, FileH->PressDataSize, DXA->HeadV5.DataStartAddress + FileH->DataAddress, DXA->Key ) ;
//					}
//					
//					// ��
//					DXA_Decode( temp, Buffer ) ;
//					
//					// �������̉��
//					DXFREE( temp ) ;
//				}
//				else
//				{
//					// ��������̈��k�f�[�^���𓀂���
//					DXA_Decode( (BYTE *)DXA->MemoryImage + DXA->HeadV5.DataStartAddress + FileH->DataAddress, Buffer ) ;
//				}
//			}
//			else
//			{
//				void *temp ;
//
//				// ���k�f�[�^���������ɓǂݍ���ł���𓀂���
//
//				// ���k�f�[�^�����܂郁�����̈�̊m��
//				temp = DXALLOC( FileH->PressDataSize ) ;
//
//				// ���k�f�[�^�̓ǂݍ���
//				ReadOnlyFileAccessSeek( DXA->WinFilePointer__, DXA->HeadV5.DataStartAddress + FileH->DataAddress, SEEK_SET ) ;
//				if( DXA->HeadV5.Version >= 0x0005 )
//				{
//					DXA_KeyConvFileRead( temp, FileH->PressDataSize, DXA->WinFilePointer__, DXA->Key, FileH->DataSize ) ;
//				}
//				else
//				{
//					DXA_KeyConvFileRead( temp, FileH->PressDataSize, DXA->WinFilePointer__, DXA->Key ) ;
//				}
//				
//				// ��
//				DXA_Decode( temp, Buffer ) ;
//				
//				// �������̉��
//				DXFREE( temp ) ;
//			}
//		}
//		else
//		{
//			if( DXA->MemoryOpenFlag == TRUE )
//			{
//				if( DXA->MemoryImageReadOnlyFlag )
//				{
//					// �t�@�C���|�C���^���ړ�
//					_MEMCPY( Buffer, (BYTE *)DXA->MemoryImage + DXA->HeadV5.DataStartAddress + FileH->DataAddress, FileH->DataSize ) ;
//
//					// �ǂݍ���
//					if( DXA->HeadV5.Version >= 0x0005 )
//					{
//						DXA_KeyConv( Buffer, FileH->DataSize,                                   FileH->DataSize, DXA->Key ) ;
//					}
//					else
//					{
//						DXA_KeyConv( Buffer, FileH->DataSize, DXA->HeadV5.DataStartAddress + FileH->DataAddress, DXA->Key ) ;
//					}
//				}
//				else
//				{
//					// �R�s�[
//					_MEMCPY( Buffer, (BYTE *)DXA->MemoryImage + DXA->HeadV5.DataStartAddress + FileH->DataAddress, FileH->DataSize ) ;
//				}
//			}
//			else
//			{
//				// �t�@�C���|�C���^���ړ�
//				ReadOnlyFileAccessSeek( DXA->WinFilePointer__, DXA->HeadV5.DataStartAddress + FileH->DataAddress, SEEK_SET ) ;
//
//				// �ǂݍ���
//				if( DXA->HeadV5.Version >= 0x0005 )
//				{
//					DXA_KeyConvFileRead( Buffer, FileH->DataSize, DXA->WinFilePointer__, DXA->Key, FileH->DataSize ) ;
//				}
//				else
//				{
//					DXA_KeyConvFileRead( Buffer, FileH->DataSize, DXA->WinFilePointer__, DXA->Key ) ;
//				}
//			}
//		}
//	}
//	else
//	{
//		DXARC_FILEHEAD *FileH ;
//
//		// �w��̃t�@�C���̏��𓾂�
//		FileH = DXA_GetFileHeader( DXA, FilePath ) ;
//		if( FileH == NULL ) return -1 ;
//
//		// �t�@�C���T�C�Y������Ă��邩���ׂ�A����Ă��Ȃ����A�o�b�t�@�A���̓T�C�Y���O��������T�C�Y��Ԃ�
//		if( BufferSize < FileH->DataSize || BufferSize == 0 || Buffer == NULL )
//		{
//			return ( int )FileH->DataSize ;
//		}
//		
//		// ����Ă���ꍇ�̓o�b�t�@�[�ɓǂݍ���
//
//		// �t�@�C�������k����Ă��邩�ǂ����ŏ����𕪊�
//		if( FileH->PressDataSize != NONE_PAL )
//		{
//			// ���k����Ă���ꍇ
//
//			// ��������ɓǂݍ���ł��邩�ǂ����ŏ����𕪊�
//			if( DXA->MemoryOpenFlag == TRUE )
//			{
//				if( DXA->MemoryImageReadOnlyFlag )
//				{
//					void *temp ;
//
//					// ���k�f�[�^���������ɓǂݍ���ł���𓀂���
//
//					// ���k�f�[�^�����܂郁�����̈�̊m��
//					temp = DXALLOC( ( size_t )FileH->PressDataSize ) ;
//
//					// ���k�f�[�^�̓ǂݍ���
//					_MEMCPY( temp, (BYTE *)DXA->MemoryImage + DXA->Head.DataStartAddress + FileH->DataAddress, ( size_t )FileH->DataSize ) ;
//					DXA_KeyConv( temp, ( LONGLONG )FileH->DataSize, ( LONGLONG )FileH->DataSize, DXA->Key ) ;
//					
//					// ��
//					DXA_Decode( temp, Buffer ) ;
//					
//					// �������̉��
//					DXFREE( temp ) ;
//				}
//				else
//				{
//					// ��������̈��k�f�[�^���𓀂���
//					DXA_Decode( (BYTE *)DXA->MemoryImage + DXA->Head.DataStartAddress + FileH->DataAddress, Buffer ) ;
//				}
//			}
//			else
//			{
//				void *temp ;
//
//				// ���k�f�[�^���������ɓǂݍ���ł���𓀂���
//
//				// ���k�f�[�^�����܂郁�����̈�̊m��
//				temp = DXALLOC( ( size_t )FileH->PressDataSize ) ;
//
//				// ���k�f�[�^�̓ǂݍ���
//				ReadOnlyFileAccessSeek( DXA->WinFilePointer__, ( LONGLONG )( DXA->Head.DataStartAddress + FileH->DataAddress ), SEEK_SET ) ;
//				DXA_KeyConvFileRead( temp, FileH->PressDataSize, DXA->WinFilePointer__, DXA->Key, ( LONGLONG )FileH->DataSize ) ;
//				
//				// ��
//				DXA_Decode( temp, Buffer ) ;
//				
//				// �������̉��
//				DXFREE( temp ) ;
//			}
//		}
//		else
//		{
//			if( DXA->MemoryOpenFlag == TRUE )
//			{
//				if( DXA->MemoryImageReadOnlyFlag )
//				{
//					// �R�s�[
//					_MEMCPY( Buffer, (BYTE *)DXA->MemoryImage + DXA->Head.DataStartAddress + FileH->DataAddress, ( size_t )FileH->DataSize ) ;
//
//					DXA_KeyConv( Buffer, ( LONGLONG )FileH->DataSize, ( LONGLONG )FileH->DataSize, DXA->Key ) ;
//				}
//				else
//				{
//					// �R�s�[
//					_MEMCPY( Buffer, (BYTE *)DXA->MemoryImage + DXA->Head.DataStartAddress + FileH->DataAddress, ( size_t )FileH->DataSize ) ;
//				}
//			}
//			else
//			{
//				// �t�@�C���|�C���^���ړ�
//				ReadOnlyFileAccessSeek( DXA->WinFilePointer__, ( LONGLONG )( DXA->Head.DataStartAddress + FileH->DataAddress ), SEEK_SET ) ;
//
//				// �ǂݍ���
//				DXA_KeyConvFileRead( Buffer, FileH->DataSize, DXA->WinFilePointer__, DXA->Key, ( LONGLONG )FileH->DataSize ) ;
//			}
//		}
//	}
//	
//	// �I��
//	return 0 ;
//}

// �A�[�J�C�u�t�@�C�����������ɓǂݍ��񂾏ꍇ�̃t�@�C���C���[�W���i�[����Ă���擪�A�h���X���擾����( DXA_OpenArchiveFromFileUseMem �Ⴕ���� DXA_OpenArchiveFromMem �ŊJ�����ꍇ�ɗL�� )
extern void *DXA_GetFileImage( DXARC *DXA )
{
	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	// �������C���[�W����J���Ă��Ȃ�������G���[
	if( DXA->MemoryOpenFlag == FALSE )
	{
		return NULL ;
	}

	// �擪�A�h���X��Ԃ�
	return DXA->MemoryImage ;
}

// �A�[�J�C�u�t�@�C�����̎w��̃t�@�C���̃t�@�C�����̈ʒu�ƃt�@�C���̑傫���𓾂�( -1:�G���[ )
extern int DXA_GetFileInfo( DXARC *DXA, int CharCodeFormat, const char *FilePath, int *Position, int *Size )
{
	BYTE TempBuffer[ 4096 ] ;
	const BYTE *FilePathB ;

	// �����R�[�h�`�����قȂ�ꍇ�͕ϊ�����
	if( CharCodeFormat != DXA->CharCodeFormat )
	{
		ConvString( FilePath, CharCodeFormat, ( char * )TempBuffer, sizeof( TempBuffer ), DXA->CharCodeFormat ) ;
		FilePathB = TempBuffer ;
	}
	else
	{
		FilePathB = ( const BYTE * )FilePath ;
	}

	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	if( DXA->V5Flag )
	{
		DXARC_FILEHEAD_VER5 *FileH ;

		// �w��̃t�@�C���̏��𓾂�
		FileH = DXA_GetFileHeaderV5( DXA, FilePathB ) ;
		if( FileH == NULL )
		{
			return -1 ;
		}

		// �t�@�C���̃f�[�^������ʒu�ƃt�@�C���T�C�Y��ۑ�����
		if( Position != NULL )
		{
			*Position = ( int )( DXA->HeadV5.DataStartAddress + FileH->DataAddress ) ;
		}

		if( Size     != NULL )
		{
			*Size     = ( int )FileH->DataSize ;
		}
	}
	else
	{
		DXARC_FILEHEAD *FileH ;

		// �w��̃t�@�C���̏��𓾂�
		FileH = DXA_GetFileHeader( DXA, FilePathB ) ;
		if( FileH == NULL )
		{
			return -1 ;
		}

		// �t�@�C���̃f�[�^������ʒu�ƃt�@�C���T�C�Y��ۑ�����
		if( Position != NULL )
		{
			*Position = ( int )( DXA->Head.DataStartAddress + FileH->DataAddress ) ;
		}

		if( Size     != NULL )
		{
			*Size     = ( int )( FileH->DataSize ) ;
		}
	}

	// �����I��
	return 0 ;
}



// �A�[�J�C�u�t�@�C�����̃t�@�C�����J��(�t�@�C�������Ƃ͕K�v�Ȃ�)
extern int DXA_STREAM_Initialize( DXARC_STREAM *DXAStream, DXARC *DXA, const BYTE *FilePath, int UseASyncReadFlag )
{
	// �񓯊������I�[�v�����̏ꍇ�͂����ŊJ���I���̂�҂�
	if( DXA->ASyncOpenFlag == TRUE )
	{
		while( DXA_CheckIdle( DXA ) == FALSE )
		{
			Thread_Sleep( 0 ) ;
		}
	}

	// �f�[�^�̃Z�b�g
	DXAStream->Archive          = DXA ;
	DXAStream->EOFFlag          = FALSE ;
	DXAStream->FilePoint        = 0 ;
	DXAStream->DecodeDataBuffer = NULL ;
	DXAStream->DecodeTempBuffer = NULL ;
	DXAStream->UseASyncReadFlag = UseASyncReadFlag ;
	DXAStream->ASyncState       = DXARC_STREAM_ASYNCSTATE_IDLE ;

	// �t�@�C������J���Ă���ꍇ�̓A�[�J�C�u�t�@�C���̃t�@�C���|�C���^���쐬
	if( DXA->MemoryOpenFlag == FALSE )
	{
		DXAStream->WinFilePointer = ReadOnlyFileAccessOpen( DXA->FilePath, FALSE, TRUE, FALSE ) ;
		if( DXAStream->WinFilePointer == 0 )
		{
			return -1 ;
		}
	}

	if( DXA->V5Flag )
	{
		DXARC_FILEHEAD_VER5 *FileH ;

		// �w��̃t�@�C���̏��𓾂�
		FileH = DXA_GetFileHeaderV5( DXA, FilePath ) ;
		if( FileH == NULL )
		{
			if( DXA->MemoryOpenFlag == FALSE )
			{
				ReadOnlyFileAccessClose( DXAStream->WinFilePointer ) ;
				DXAStream->WinFilePointer = 0 ;
			}
			return -1 ;
		}

		// �t�@�C�������Z�b�g
		DXAStream->FileHeadV5 = FileH ;

		// �t�@�C�������k����Ă���ꍇ�͂����œǂݍ���ŉ𓀂��Ă��܂�
		if( DXA->HeadV5.Version >= 0x0002 && FileH->PressDataSize != 0xffffffff )
		{
			// �𓀃f�[�^�����܂郁�����̈�̊m��
			DXAStream->DecodeDataBuffer = DXALLOC( FileH->DataSize ) ;

			// ��������ɓǂݍ��܂�Ă��邩�ǂ����ŏ����𕪊�
			if( DXA->MemoryOpenFlag == TRUE )
			{
				if( DXA->MemoryImageReadOnlyFlag )
				{
					// ���k�f�[�^�����܂郁�����̈�̊m��
					DXAStream->DecodeTempBuffer = DXALLOC( FileH->PressDataSize ) ;

					// ���k�f�[�^�̓ǂݍ���
					_MEMCPY( DXAStream->DecodeTempBuffer, (BYTE *)DXA->MemoryImage + DXA->HeadV5.DataStartAddress + FileH->DataAddress, FileH->PressDataSize ) ;
					if( DXA->HeadV5.Version >= 0x0005 )
					{
						DXA_KeyConv( DXAStream->DecodeTempBuffer, FileH->PressDataSize,                                   FileH->DataSize, DXA->Key ) ;
					}
					else
					{
						DXA_KeyConv( DXAStream->DecodeTempBuffer, FileH->PressDataSize, DXA->HeadV5.DataStartAddress + FileH->DataAddress, DXA->Key ) ;
					}

					// ��
					DXA_Decode( DXAStream->DecodeTempBuffer, DXAStream->DecodeDataBuffer ) ;
				
					// �������̉��
					DXFREE( DXAStream->DecodeTempBuffer ) ;
					DXAStream->DecodeTempBuffer = NULL ;
				}
				else
				{
					// ��
					DXA_Decode( (BYTE *)DXA->MemoryImage + DXA->HeadV5.DataStartAddress + FileH->DataAddress, DXAStream->DecodeDataBuffer ) ;
				}
			}
			else
			{
				// ���k�f�[�^�����܂郁�����̈�̊m��
				DXAStream->DecodeTempBuffer = DXALLOC( FileH->PressDataSize ) ;

				// ���k�f�[�^�̓ǂݍ���
				DXAStream->ASyncReadFileAddress = DXA->HeadV5.DataStartAddress + FileH->DataAddress ;
				ReadOnlyFileAccessSeek( DXAStream->WinFilePointer, ( LONGLONG )DXAStream->ASyncReadFileAddress, SEEK_SET ) ;

				// �񓯊��̏ꍇ�͓ǂݍ��݂ƌ�������ʁX�ɍs��
				if( DXAStream->UseASyncReadFlag == TRUE )
				{
					// �t�@�C������ǂݍ���
					ReadOnlyFileAccessRead( DXAStream->DecodeTempBuffer, FileH->PressDataSize, 1, DXAStream->WinFilePointer ) ;
					DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_PRESSREAD ;
				}
				else
				{
					if( DXA->HeadV5.Version >= 0x0005 )
					{
						DXA_KeyConvFileRead( DXAStream->DecodeTempBuffer, FileH->PressDataSize, DXAStream->WinFilePointer, DXA->Key, FileH->DataSize ) ;
					}
					else
					{
						DXA_KeyConvFileRead( DXAStream->DecodeTempBuffer, FileH->PressDataSize, DXAStream->WinFilePointer, DXA->Key ) ;
					}

					// ��
					DXA_Decode( DXAStream->DecodeTempBuffer, DXAStream->DecodeDataBuffer ) ;
				
					// �������̉��
					DXFREE( DXAStream->DecodeTempBuffer ) ;
					DXAStream->DecodeTempBuffer = NULL ;
				}
			}
		}
	}
	else
	{
		DXARC_FILEHEAD *FileH ;

		// �w��̃t�@�C���̏��𓾂�
		FileH = DXA_GetFileHeader( DXA, FilePath ) ;
		if( FileH == NULL )
		{
			if( DXA->MemoryOpenFlag == FALSE )
			{
				ReadOnlyFileAccessClose( DXAStream->WinFilePointer ) ;
				DXAStream->WinFilePointer = 0 ;
			}
			return -1 ;
		}

		// �t�@�C�������Z�b�g
		DXAStream->FileHead = FileH ;

		// �t�@�C�������k����Ă���ꍇ�͂����œǂݍ���ŉ𓀂��Ă��܂�
		if( FileH->PressDataSize != NONE_PAL )
		{
			// �𓀃f�[�^�����܂郁�����̈�̊m��
			DXAStream->DecodeDataBuffer = DXALLOC( ( size_t )FileH->DataSize ) ;

			// ��������ɓǂݍ��܂�Ă��邩�ǂ����ŏ����𕪊�
			if( DXA->MemoryOpenFlag == TRUE )
			{
				if( DXA->MemoryImageReadOnlyFlag )
				{
					// ���k�f�[�^�����܂郁�����̈�̊m��
					DXAStream->DecodeTempBuffer = DXALLOC( ( size_t )FileH->PressDataSize ) ;

					// ���k�f�[�^�̓ǂݍ���
					_MEMCPY( DXAStream->DecodeTempBuffer, (BYTE *)DXA->MemoryImage + DXA->Head.DataStartAddress + FileH->DataAddress, ( size_t )FileH->DataSize ) ;
					DXA_KeyConv( DXAStream->DecodeTempBuffer, ( LONGLONG )FileH->PressDataSize, ( LONGLONG )FileH->DataSize, DXA->Key ) ;

					// ��
					DXA_Decode( DXAStream->DecodeTempBuffer, DXAStream->DecodeDataBuffer ) ;
				
					// �������̉��
					DXFREE( DXAStream->DecodeTempBuffer ) ;
					DXAStream->DecodeTempBuffer = NULL ;
				}
				else
				{
					// ��
					DXA_Decode( (BYTE *)DXA->MemoryImage + DXA->Head.DataStartAddress + FileH->DataAddress, DXAStream->DecodeDataBuffer ) ;
				}
			}
			else
			{
				// ���k�f�[�^�����܂郁�����̈�̊m��
				DXAStream->DecodeTempBuffer = DXALLOC( ( size_t )FileH->PressDataSize ) ;

				// ���k�f�[�^�̓ǂݍ���
				DXAStream->ASyncReadFileAddress = DXA->Head.DataStartAddress + FileH->DataAddress;
				ReadOnlyFileAccessSeek( DXAStream->WinFilePointer, ( LONGLONG )DXAStream->ASyncReadFileAddress, SEEK_SET ) ;

				// �񓯊��̏ꍇ�͓ǂݍ��݂ƌ�������ʁX�ɍs��
				if( DXAStream->UseASyncReadFlag == TRUE )
				{
					// �t�@�C������ǂݍ���
					ReadOnlyFileAccessRead( DXAStream->DecodeTempBuffer, ( size_t )FileH->PressDataSize, 1, DXAStream->WinFilePointer ) ;
					DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_PRESSREAD ;
				}
				else
				{
					DXA_KeyConvFileRead( DXAStream->DecodeTempBuffer, FileH->PressDataSize, DXAStream->WinFilePointer, DXA->Key, ( LONGLONG )FileH->DataSize ) ;

					// ��
					DXA_Decode( DXAStream->DecodeTempBuffer, DXAStream->DecodeDataBuffer ) ;
				
					// �������̉��
					DXFREE( DXAStream->DecodeTempBuffer ) ;
					DXAStream->DecodeTempBuffer = NULL ;
				}
			}
		}
	}

	// �I��
	return 0 ;
}

// �A�[�J�C�u�t�@�C�����̃t�@�C�������
extern int DXA_STREAM_Terminate( DXARC_STREAM *DXAStream )
{
	// �񓯊��ǂݍ��݂ŏ�Ԃ��܂��ҋ@��Ԃł͂Ȃ�������ҋ@��ԂɂȂ�܂ő҂�
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE )
		{
			Thread_Sleep( 1 ) ;
		}
	}

	// �������̉��
	if( DXAStream->DecodeDataBuffer != NULL )
	{
		DXFREE( DXAStream->DecodeDataBuffer ) ;
		DXAStream->DecodeDataBuffer = NULL ;
	}

	if( DXAStream->DecodeTempBuffer != NULL )
	{
		DXFREE( DXAStream->DecodeTempBuffer ) ;
		DXAStream->DecodeTempBuffer = NULL ;
	}

	// �t�@�C�������
	if( DXAStream->Archive->MemoryOpenFlag == FALSE )
	{
		ReadOnlyFileAccessClose( DXAStream->WinFilePointer ) ;
		DXAStream->WinFilePointer = 0 ;
	}

	// �[��������
	_MEMSET( DXAStream, 0, sizeof( DXARC_STREAM ) ) ;

	// �I��
	return 0 ;
}

// �t�@�C���̓��e��ǂݍ���
extern int DXA_STREAM_Read( DXARC_STREAM *DXAStream, void *Buffer, size_t ReadLength )
{
	size_t ReadSize ;
	ULONGLONG DataSize ;
	ULONGLONG DataStartAddress ;
	ULONGLONG DataAddress ;

	// �񓯊��ǂݍ��݂ŏ�Ԃ��܂��ҋ@��Ԃł͂Ȃ�������ҋ@��ԂɂȂ�܂ő҂�
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE )
		{
			Thread_Sleep( 1 ) ;
		}
	}

	if( DXAStream->Archive->V5Flag )
	{
		DataSize = DXAStream->FileHeadV5->DataSize ;
		DataAddress = DXAStream->FileHeadV5->DataAddress ;
		DataStartAddress = DXAStream->Archive->HeadV5.DataStartAddress ;
	}
	else
	{
		DataSize = DXAStream->FileHead->DataSize ;
		DataAddress = DXAStream->FileHead->DataAddress ;
		DataStartAddress = DXAStream->Archive->Head.DataStartAddress ;
	}

	// EOF �t���O�������Ă�����O��Ԃ�
	if( DXAStream->EOFFlag == TRUE )
	{
		return 0 ;
	}

	// EOF ���o
	if( DataSize == DXAStream->FilePoint )
	{
		DXAStream->EOFFlag = TRUE ;
		return 0 ;
	}

	// �f�[�^��ǂݍ��ޗʂ�ݒ肷��
	ReadSize = ReadLength < DataSize - DXAStream->FilePoint ? ReadLength : ( size_t )( DataSize - DXAStream->FilePoint ) ;

	// �f�[�^�����k����Ă������ǂ����ŏ����𕪊�
	if( DXAStream->DecodeDataBuffer != NULL )
	{
		// �f�[�^���R�s�[����
		_MEMCPY( Buffer, (BYTE *)DXAStream->DecodeDataBuffer + DXAStream->FilePoint, ReadSize ) ;
	}
	else
	{
		// ��������Ƀf�[�^�����邩�ǂ����ŏ����𕪊�
		if( DXAStream->Archive->MemoryOpenFlag == TRUE )
		{
			// ��������ɂ���ꍇ

			// �f�[�^���R�s�[����
			_MEMCPY( Buffer, (BYTE *)DXAStream->Archive->MemoryImage + DataStartAddress + DataAddress + DXAStream->FilePoint, ReadSize ) ;

			if( DXAStream->Archive->MemoryImageReadOnlyFlag )
			{
				if( DXAStream->Archive->V5Flag )
				{
					if( DXAStream->Archive->HeadV5.Version >= 0x0005 )
					{
						DXA_KeyConv( Buffer, ( LONGLONG )ReadSize,                       ( LONGLONG )( DataSize + DXAStream->FilePoint ), DXAStream->Archive->Key ) ;
					}
					else
					{
						DXA_KeyConv( Buffer, ( LONGLONG )ReadSize, ( LONGLONG )( DataStartAddress + DataAddress + DXAStream->FilePoint ), DXAStream->Archive->Key ) ;
					}
				}
				else
				{
					DXA_KeyConv(     Buffer, ( LONGLONG )ReadSize,                       ( LONGLONG )( DataSize + DXAStream->FilePoint ), DXAStream->Archive->Key ) ;
				}
			}
		}
		else
		{
			// �t�@�C������ǂݍ���ł���ꍇ

			// �A�[�J�C�u�t�@�C���|�C���^�ƁA���z�t�@�C���|�C���^����v���Ă��邩���ׂ�
			// ��v���Ă��Ȃ�������A�[�J�C�u�t�@�C���|�C���^���ړ�����
			DXAStream->ASyncReadFileAddress = DataAddress + DataStartAddress + DXAStream->FilePoint ;
			if( ( ULONGLONG )ReadOnlyFileAccessTell( DXAStream->WinFilePointer ) != DXAStream->ASyncReadFileAddress )
			{
				ReadOnlyFileAccessSeek( DXAStream->WinFilePointer, ( LONGLONG )DXAStream->ASyncReadFileAddress, SEEK_SET ) ;
			}

			// �񓯊��ǂݍ��݂̏ꍇ�Ɠ����ǂݍ��݂̏ꍇ�ŏ����𕪊�
			if( DXAStream->UseASyncReadFlag )
			{
				// �t�@�C������ǂݍ���
				ReadOnlyFileAccessRead( Buffer, ReadSize, 1, DXAStream->WinFilePointer ) ;
				DXAStream->ReadBuffer = Buffer;
				DXAStream->ReadSize = ( int )ReadSize;
				DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_READ ;
			}
			else
			{
				// �f�[�^��ǂݍ���
				if( DXAStream->Archive->V5Flag )
				{
					if( DXAStream->Archive->HeadV5.Version >= 0x0005 )
					{
						DXA_KeyConvFileRead( Buffer, ReadSize, DXAStream->WinFilePointer, DXAStream->Archive->Key, ( LONGLONG )( DataSize + DXAStream->FilePoint ) ) ;
					}
					else
					{
						DXA_KeyConvFileRead( Buffer, ReadSize, DXAStream->WinFilePointer, DXAStream->Archive->Key ) ;
					}
				}
				else
				{
					DXA_KeyConvFileRead( Buffer, ReadSize, DXAStream->WinFilePointer, DXAStream->Archive->Key, ( LONGLONG )( DataSize + DXAStream->FilePoint ) ) ;
				}
			}
		}
	}
	
	// EOF �t���O��|��
	DXAStream->EOFFlag = FALSE ;

	// �ǂݍ��񂾕������t�@�C���|�C���^���ړ�����
	DXAStream->FilePoint += ( int )ReadSize ;
	
	// �ǂݍ��񂾗e�ʂ�Ԃ�
	return ( int )ReadSize ;
}
	
// �t�@�C���|�C���^��ύX����
extern	int DXA_STREAM_Seek( DXARC_STREAM *DXAStream, LONGLONG SeekPoint, int SeekMode )
{
	ULONGLONG DataSize ;

	// �񓯊��ǂݍ��݂ŏ�Ԃ��܂��ҋ@��Ԃł͂Ȃ�������ҋ@��ԂɂȂ�܂ő҂�
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE )
		{
			Thread_Sleep( 1 ) ;
		}
	}

	if( DXAStream->Archive->V5Flag )
	{
		DataSize = DXAStream->FileHeadV5->DataSize ;
	}
	else
	{
		DataSize = DXAStream->FileHead->DataSize ;
	}

	// �V�[�N�^�C�v�ɂ���ď����𕪊�
	switch( SeekMode )
	{
	case SEEK_SET : break ;		
	case SEEK_CUR : SeekPoint += ( LONGLONG )( DXAStream->FilePoint ) ; break ;
	case SEEK_END :	SeekPoint  = ( LONGLONG )( DataSize + SeekPoint ) ; break ;
	}
	
	// �␳
	if( SeekPoint > ( LONGLONG )DataSize ) SeekPoint = ( LONGLONG )DataSize ;
	if( SeekPoint < 0 ) SeekPoint = 0 ;
	
	// �Z�b�g
	DXAStream->FilePoint = ( ULONGLONG )SeekPoint ;
	
	// EOF�t���O��|��
	DXAStream->EOFFlag = FALSE ;
	
	// �I��
	return 0 ;
}

// ���݂̃t�@�C���|�C���^�𓾂�
extern	LONGLONG DXA_STREAM_Tell( DXARC_STREAM *DXAStream )
{
	// �񓯊��ǂݍ��݂ŏ�Ԃ��܂��ҋ@��Ԃł͂Ȃ�������ҋ@��ԂɂȂ�܂ő҂�
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE )
		{
			Thread_Sleep( 1 ) ;
		}
	}

	return ( LONGLONG )DXAStream->FilePoint ;
}

// �t�@�C���̏I�[�ɗ��Ă��邩�A�̃t���O�𓾂�
extern	int DXA_STREAM_Eof( DXARC_STREAM *DXAStream )
{
	// �񓯊��ǂݍ��݂ŏ�Ԃ��܂��ҋ@��Ԃł͂Ȃ�������ҋ@��ԂɂȂ�܂ő҂�
	if( DXAStream->UseASyncReadFlag == TRUE && DXAStream->ASyncState != DXARC_STREAM_ASYNCSTATE_IDLE )
	{
		while( DXA_STREAM_IdleCheck( DXAStream ) == FALSE )
		{
			Thread_Sleep( 1 ) ;
		}
	}

	return DXAStream->EOFFlag ? EOF : 0 ;
}

// �ǂݍ��ݏ������������Ă��邩�ǂ����𒲂ׂ�
extern	int	DXA_STREAM_IdleCheck( DXARC_STREAM *DXAStream )
{
	// �񓯊��ǂݍ��݂ł͂Ȃ��ꍇ�͉������� TRUE ��Ԃ�
	if( DXAStream->UseASyncReadFlag == FALSE )
	{
		return TRUE ;
	}

	// ��Ԃɂ���ď����𕪊�
	switch( DXAStream->ASyncState )
	{
	case DXARC_STREAM_ASYNCSTATE_IDLE:			// �ҋ@���
		return TRUE;

	case DXARC_STREAM_ASYNCSTATE_PRESSREAD:		// ���k�f�[�^�ǂݍ��ݑ҂�

		// �ǂݍ��ݏI���҂�
		if( ReadOnlyFileAccessIdleCheck( DXAStream->WinFilePointer ) == FALSE )
		{
			return FALSE;
		}

		// �ǂݍ��ݏI�������܂������O��
		if( DXAStream->Archive->V5Flag )
		{
			if( DXAStream->Archive->HeadV5.Version >= 0x0005 )
			{
				DXA_KeyConv( DXAStream->DecodeTempBuffer, ( LONGLONG )DXAStream->FileHeadV5->PressDataSize, ( LONGLONG )DXAStream->FileHeadV5->DataSize, DXAStream->Archive->Key ) ;
			}
			else
			{
				DXA_KeyConv( DXAStream->DecodeTempBuffer, ( LONGLONG )DXAStream->FileHeadV5->PressDataSize, ( LONGLONG )DXAStream->ASyncReadFileAddress, DXAStream->Archive->Key ) ;
			}
		}
		else
		{
			DXA_KeyConv( DXAStream->DecodeTempBuffer, ( LONGLONG )DXAStream->FileHead->PressDataSize, ( LONGLONG )DXAStream->FileHead->DataSize, DXAStream->Archive->Key ) ;
		}

		// ��
		DXA_Decode( DXAStream->DecodeTempBuffer, DXAStream->DecodeDataBuffer ) ;
	
		// �������̉��
		DXFREE( DXAStream->DecodeTempBuffer ) ;
		DXAStream->DecodeTempBuffer = NULL ;

		// ��Ԃ�ҋ@��Ԃɂ���
		DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_IDLE;
		return TRUE;

	case DXARC_STREAM_ASYNCSTATE_READ:			// �ǂݍ��ݑ҂�

		// �ǂݍ��ݏI���҂�
		if( ReadOnlyFileAccessIdleCheck( DXAStream->WinFilePointer ) == FALSE )
		{
			return FALSE;
		}

		// �ǂݍ��ݏI������献���O��
		if( DXAStream->Archive->V5Flag )
		{
			if( DXAStream->Archive->HeadV5.Version >= 0x0005 )
			{
				DXA_KeyConv( DXAStream->ReadBuffer, ( LONGLONG )DXAStream->ReadSize, ( LONGLONG )( DXAStream->FileHeadV5->DataSize + ( DXAStream->ASyncReadFileAddress - ( DXAStream->FileHeadV5->DataAddress + DXAStream->Archive->HeadV5.DataStartAddress ) ) ), DXAStream->Archive->Key ) ;
			}
			else
			{
				DXA_KeyConv( DXAStream->ReadBuffer, ( LONGLONG )DXAStream->ReadSize, ( LONGLONG )DXAStream->ASyncReadFileAddress, DXAStream->Archive->Key ) ;
			}
		}
		else
		{
			DXA_KeyConv( DXAStream->ReadBuffer, ( LONGLONG )DXAStream->ReadSize, ( LONGLONG )( DXAStream->FileHead->DataSize + ( DXAStream->ASyncReadFileAddress - ( DXAStream->FileHead->DataAddress + DXAStream->Archive->Head.DataStartAddress ) ) ), DXAStream->Archive->Key ) ;
		}

		// ��Ԃ�ҋ@��Ԃɂ���
		DXAStream->ASyncState = DXARC_STREAM_ASYNCSTATE_IDLE;
		return TRUE;
	}

	return TRUE ;
}

// �t�@�C���̃T�C�Y���擾����
extern	LONGLONG DXA_STREAM_Size( DXARC_STREAM *DXAStream )
{
	if( DXAStream->Archive->V5Flag )
	{
		return DXAStream->FileHeadV5->DataSize ;
	}
	else
	{
		return ( LONGLONG )DXAStream->FileHead->DataSize ;
	}
}







// �t���p�X�ł͂Ȃ��p�X��������t���p�X�ɕϊ�����
static int DXA_DIR_ConvertFullPath( const wchar_t *Src, wchar_t *Dest, size_t BufferBytes )
{
	int     i, k ;
	wchar_t iden[ FILEPATH_MAX ] ;
	wchar_t CurrentDir[ FILEPATH_MAX ] ;
	DWORD   CharCode1 ;
	int     CharBytes1 ;
	DWORD   CharCode2 ;
	int     CharBytes2 ;
	DWORD   LastCharCode ;
	int     LastCharBytes ;
	char *  LastCharAddress ;
	int     CharNum ;
	size_t	DestBytes ;

	// �J�����g�f�B���N�g���𓾂�
	_WGETCWD( CurrentDir, sizeof( CurrentDir ) ) ;
	_WCSUPR( CurrentDir ) ;
	if( Src == NULL )
	{
		_WCSCPY_S( Dest, BufferBytes, CurrentDir ) ;
		goto END ;
	}

	i = 0 ;
	k = 0 ;

	DestBytes = 0 ;

	// �P�����ڂƂQ�����ڂ��擾
	CharCode1 = GetCharCode( ( const char * )&( ( BYTE * )Src )[ 0 ], WCHAR_T_CHARCODEFORMAT, &CharBytes1 ) ;
	CharCode2 = 0 ;
	if( CharCode1 != 0 )
	{
		CharCode2 = GetCharCode( ( const char * )&( ( BYTE * )Src )[ CharBytes1 ], WCHAR_T_CHARCODEFORMAT, &CharBytes2 ) ;
	}

	// �ŏ��Ɂw\�x���́w/�x���Q��A���ő����Ă���ꍇ�̓l�b�g���[�N����Ă���Ɣ��f

	if( ( CharCode1 == '\\' && CharCode2 == '\\' ) ||
		( CharCode1 == '/'  && CharCode2 == '/'  ) )
	{
		DestBytes += PutCharCode( '\\', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
		PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;

		i += CharBytes1 + CharBytes2 ;
	}
	else
	// �ŏ����w\�x���́w/�x�̏ꍇ�̓J�����g�h���C�u�̃��[�g�f�B���N�g���܂ŗ�����
	if( CharCode1 == '\\' || CharCode1 == '/' )
	{
		DWORD CurCharCode1 ;
		DWORD CurCharCode2 ;
		int   CurCharBytes1 ;
		int   CurCharBytes2 ;

		CurCharCode1 = GetCharCode( ( char * )&( ( BYTE * )CurrentDir )[ 0             ], WCHAR_T_CHARCODEFORMAT, &CurCharBytes1 ) ;
		CurCharCode2 = GetCharCode( ( char * )&( ( BYTE * )CurrentDir )[ CurCharBytes1 ], WCHAR_T_CHARCODEFORMAT, &CurCharBytes2 ) ;
		DestBytes += PutCharCode( CurCharCode1, WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
		DestBytes += PutCharCode( CurCharCode2, WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
		PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;

		i += CharBytes1 ;
	}
	else
	// �h���C�u����������Ă����炻�̃h���C�u��
	if( CharCode2 == ':' )
	{
		DestBytes += PutCharCode( CHARUP( CharCode1 ), WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
		DestBytes += PutCharCode(         CharCode2  , WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
		PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;

		i += CharBytes1 + CharBytes2 ;

		// : �̌�� \ �}�[�N�͔�΂�
		CharCode1 = GetCharCode( ( const char * )&( ( BYTE * )Src )[ i ], WCHAR_T_CHARCODEFORMAT, &CharBytes1 ) ;
		if( CharCode1 == '\\' )
		{
			i += CharBytes1 ;
		}
	}
	else
	// ����ȊO�̏ꍇ�̓J�����g�f�B���N�g��
	{
		_WCSCPY_S( Dest, BufferBytes, CurrentDir ) ;
		DestBytes += _WCSLEN( Dest ) * sizeof( wchar_t ) ;

		CharNum = GetStringCharNum(  ( const char * )CurrentDir, WCHAR_T_CHARCODEFORMAT ) ;
		if( CharNum != 0 )
		{
			LastCharAddress = ( char * )GetStringCharAddress( ( const char * )CurrentDir, WCHAR_T_CHARCODEFORMAT, CharNum - 1 ) ;
			LastCharCode    = GetCharCode( LastCharAddress, WCHAR_T_CHARCODEFORMAT, &LastCharBytes ) ;
			if( LastCharCode == '\\' || LastCharCode == '/' )
			{
				PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, LastCharAddress, BUFFERBYTES_CANCEL ) ;
				DestBytes -= LastCharBytes ;
			}
		}
	}

	for(;;)
	{
		CharCode1 = GetCharCode( ( const char * )&( ( BYTE * )Src )[ i ], WCHAR_T_CHARCODEFORMAT, &CharBytes1 ) ;
		switch( CharCode1 )
		{
		case '\0' :
			if( k != 0 )
			{
				CharCode2 = GetCharCode( ( const char * )&( ( BYTE * )Dest )[ 0 ], WCHAR_T_CHARCODEFORMAT, &CharBytes2 ) ;
				if( CharCode2 != '\0' )
				{
					DestBytes += PutCharCode( '\\', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
				}
				_WCSCPY_S( ( wchar_t * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes, iden ) ;
				DestBytes = _WCSLEN( Dest ) * sizeof( wchar_t ) ;
				k  = 0 ;
			}
			goto END ;

		case '\\' :
		case '/' :
			// �����񂪖���������X�L�b�v
			if( k == 0 )
			{
				i += CharBytes1 ;
				break ;
			}
			if( _WCSCMP( iden, L"." ) == 0 )
			{
				// �Ȃɂ����Ȃ�
			}
			else
			if( _WCSCMP( iden, L".." ) == 0 )
			{
				// ��󂢃f�B���N�g����
				PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
				CharNum         = GetStringCharNum(  ( const char * )Dest, WCHAR_T_CHARCODEFORMAT ) ;
				LastCharAddress = ( char * )GetStringCharAddress( ( const char * )Dest, WCHAR_T_CHARCODEFORMAT, CharNum - 1 ) ;
				LastCharCode    = GetCharCode( LastCharAddress, WCHAR_T_CHARCODEFORMAT, &LastCharBytes ) ;
				DestBytes -= LastCharBytes ;
				for(;;)
				{
					if( LastCharCode == '\\' || LastCharCode == '/' || LastCharCode == ':' )
					{
						break ;
					}

					PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
					CharNum -- ;
					LastCharAddress = ( char * )GetStringCharAddress( ( const char * )Dest, WCHAR_T_CHARCODEFORMAT, CharNum - 1 ) ;
					LastCharCode    = GetCharCode( LastCharAddress, WCHAR_T_CHARCODEFORMAT, &LastCharBytes ) ;
					DestBytes -= LastCharBytes ;
				}

				if( LastCharCode != ':' )
				{
					PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
				}
				else
				{
					if( BufferBytes - DestBytes > ( size_t )LastCharBytes )
					{
						DestBytes += LastCharBytes ;
					}
				}
			}
			else
			{
				CharCode2 = GetCharCode( ( const char * )&( ( BYTE * )Dest )[ 0 ], WCHAR_T_CHARCODEFORMAT, &CharBytes2 ) ;
				if( CharCode2 != '\0' )
				{
					DestBytes += PutCharCode( '\\', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes ) ;
				}
				_WCSCPY_S( ( wchar_t * )&( ( BYTE * )Dest )[ DestBytes ], BufferBytes - DestBytes, iden ) ;
				DestBytes = _WCSLEN( Dest ) * sizeof( wchar_t ) ;
			}

			k = 0 ;
			i += CharBytes1 ;
			break ;
		
		default :
			k += PutCharCode( CHARUP( CharCode1 ), WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )iden )[ k ], sizeof( iden ) - k ) ;
			     PutCharCode( '\0',                WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )iden )[ k ], sizeof( iden ) - k ) ;

			i += CharBytes1 ;
			break ;
		}
	}
	
END :
	// ����I��
	return 0 ;
}

// �t�@�C�������ꏏ�ɂȂ��Ă���ƕ������Ă���p�X������t�@�C�����ƃf�B���N�g���p�X�𕪊�����
// �t���p�X�ł���K�v�͖����A�t�@�C���������ł��ǂ�
// DirPath �̏I�[�� �� �}�[�N�͕t���Ȃ�
static int DXA_DIR_AnalysisFileNameAndDirPath( DXARC *DXA, const BYTE *Src, BYTE *FileName, size_t FileNameBytes, BYTE *DirPath, size_t DirPathBytes )
{
	int   i ;
	int   Last ;
	int   LastCharBytes ;
	DWORD CharCode ;
	int   CharBytes ;
	
	// �t�@�C�����𔲂��o��
	i    = 0 ;
	Last = -1 ;
	for(;;)
	{
		CharCode = GetCharCode( ( const char * )&Src[ i ], DXA->CharCodeFormat, &CharBytes ) ;
		if( CharCode == '\0' )
		{
			break ;
		}

		if( CharCode == '\\' || CharCode == '/' || CharCode == '\0' || CharCode == ':' )
		{
			Last          = i ;
			LastCharBytes = CharBytes ;
		}

		i += CharBytes ;
	}

	if( FileName != NULL )
	{
		if( Last != -1 )
		{
			if( FileNameBytes > ( size_t )( Last + LastCharBytes ) )
			{
				CL_strncpy_s( DXA->CharCodeFormat, ( char * )FileName, FileNameBytes, ( const char * )&Src[ Last + LastCharBytes ], ( int )( ( FileNameBytes - ( Last + LastCharBytes ) ) / GetCharCodeFormatUnitSize( DXA->CharCodeFormat ) ) ) ;
			}
		}
		else
		{
			CL_strncpy_s( DXA->CharCodeFormat, ( char * )FileName, FileNameBytes, ( const char * )Src, ( int )( FileNameBytes / GetCharCodeFormatUnitSize( DXA->CharCodeFormat ) ) ) ;
		}
	}
	
	// �f�B���N�g���p�X�𔲂��o��
	if( DirPath != NULL )
	{
		if( Last != -1 )
		{
			if( DirPathBytes > ( size_t )Last + 1 )
			{
				_MEMCPY( DirPath, Src, ( size_t )Last ) ;
				PutCharCode( '\0', DXA->CharCodeFormat, ( char * )&DirPath[ Last ], BUFFERBYTES_CANCEL ) ;
			}
		}
		else
		{
			PutCharCode( '\0', DXA->CharCodeFormat, ( char * )&DirPath[ 0 ], DirPathBytes ) ;
		}
	}
	
	// �I��
	return 0 ;
}

// CmpStr �̏����� Src ���K�����邩�ǂ����𒲂ׂ�( 0:�K������  -1:�K�����Ȃ� )
static int DXA_DIR_FileNameCmp( DXARC *DXA, const BYTE *Src, const BYTE *CmpStr )
{
	const BYTE *s, *c ;
	DWORD SrcCharCode ;
	int   SrcCharBytes ;
	DWORD CmpCharCode ;
	int   CmpCharBytes ;

	s = Src ;
	c = CmpStr ;
	for(;;)
	{
		SrcCharCode = GetCharCode( ( const char * )s, DXA->CharCodeFormat, &SrcCharBytes ) ;
		CmpCharCode = GetCharCode( ( const char * )c, DXA->CharCodeFormat, &CmpCharBytes ) ;
		if( SrcCharCode == '\0' || CmpCharCode == '\0' )
		{
			break ;
		}

		switch( CmpCharCode )
		{
		case '?':
			c += CmpCharBytes ;
			s += SrcCharBytes ;
			break;

		case '*':
			while( CmpCharCode == '*' )
			{
				c += CmpCharBytes ;
				CmpCharCode = GetCharCode( ( const char * )c, DXA->CharCodeFormat, &CmpCharBytes ) ;
			}
			if( CmpCharCode == '\0' )
			{
				return 0 ;
			}

			while( SrcCharCode != '\0' && SrcCharCode != CmpCharCode )
			{
				s += SrcCharBytes ;
				SrcCharCode = GetCharCode( ( const char * )s, DXA->CharCodeFormat, &SrcCharBytes ) ;
			}
			if( SrcCharCode == '\0' )
			{
				return -1 ;
			}

			c += CmpCharBytes ;
			s += SrcCharBytes ;
			break;

		default:
			if( CmpCharCode != SrcCharCode )
			{
				return -1 ;
			}

			c += CmpCharBytes ;
			s += SrcCharBytes ;
			break;
		}

		if( ( CmpCharCode == '\0' && SrcCharCode != '\0' ) ||
			( CmpCharCode != '\0' && SrcCharCode == '\0' ) )
		{
			return -1 ;
		}
	}

	return 0 ;
}

// �A�[�J�C�u�t�@�C�����t�H���_�Ɍ����Ăăt�@�C�����J�����̏��𓾂�( -1:�A�[�J�C�u�Ƃ��Ă͑��݂��Ȃ�����  0:���݂��� )
static int DXA_DIR_OpenTest( const wchar_t *FilePath, int *ArchiveIndex, BYTE *ArchiveFilePath, size_t BufferBytes )
{
	int   i ;
	int   len ;
	int   arcindex ;
	wchar_t fullpath[ FILEPATH_MAX ] ;
	wchar_t path[ FILEPATH_MAX ] ;
	wchar_t temp[ FILEPATH_MAX ] ;
	wchar_t dir[ FILEPATH_MAX ] ;
	wchar_t *p ;
	int   BackUseDirectoryPathCharValid ;
	DWORD BackUseDirectoryPathCharCode ;
	int   BackUseDirectoryPathCharBytes ;

	// �t���p�X�𓾂�(���łɑS�Ă̕�����啶���ɂ���)
	DXA_DIR_ConvertFullPath( FilePath, fullpath, sizeof( fullpath ) ) ;

	// �O��Ǝg�p����A�[�J�C�u�̃p�X�������ꍇ�͓����A�[�J�C�u���g�p����
	BackUseDirectoryPathCharValid = FALSE ;
	if( DXARCD.BackUseDirectoryPathLength != 0 && _MEMCMP( fullpath, DXARCD.BackUseDirectory, DXARCD.BackUseDirectoryPathLength ) == 0 )
	{
		BackUseDirectoryPathCharValid = TRUE ;
		BackUseDirectoryPathCharCode  = GetCharCode( ( const char * )&( ( BYTE * )fullpath )[ DXARCD.BackUseDirectoryPathLength ], WCHAR_T_CHARCODEFORMAT, &BackUseDirectoryPathCharBytes ) ;
	}
	if( BackUseDirectoryPathCharValid &&
		( BackUseDirectoryPathCharCode == '\\' || BackUseDirectoryPathCharCode == '/' ) )
	{
		// �O��g�p�����c�w�`�t�@�C�����J��
		arcindex = DXA_DIR_OpenArchive( DXARCD.BackUseDirectory, NULL, -1, FALSE, FALSE, DXARCD.BackUseArchiveIndex ) ;
		if( arcindex == -1 )
		{
			return -1 ;
		}

		// �c�w�`�t�@�C��������ꏊ�ȍ~�̃p�X���쐬����
		p = ( wchar_t * )&( ( BYTE * )fullpath )[ DXARCD.BackUseDirectoryPathLength + BackUseDirectoryPathCharBytes ] ;
	}
	else
	{
		DWORD CharCode1 ;
		int   CharBytes1 ;
		DWORD CharCode2 ;
		int   CharBytes2 ;

		// �O��Ƃ͈Ⴄ�p�X�̏ꍇ�͈ꂩ�璲�ׂ�

		// �f�B���N�g�������ǂ��čs��
		p   = fullpath ;
		len = 0 ;
		for(;;)
		{
			// �l�b�g���[�N����Ă����ꍇ�̐�p����
			if( p - fullpath == 0 )
			{
				// fullpath �̂P�����ڂƂQ�����ڂ��擾
				CharCode1 = GetCharCode( ( const char * )&( ( BYTE * )fullpath )[ 0 ], WCHAR_T_CHARCODEFORMAT, &CharBytes1 ) ;
				CharCode2 = 0 ;
				if( CharCode1 != 0 )
				{
					CharCode2 = GetCharCode( ( const char * )&( ( BYTE * )fullpath )[ CharBytes1 ], WCHAR_T_CHARCODEFORMAT, &CharBytes2 ) ;
				}

				if( CharCode1 == '\\' && CharCode2 == '\\' )
				{
					len += PutCharCode( '\\', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )path )[ len ], sizeof( path ) - len ) ;
					len += PutCharCode( '\\', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )path )[ len ], sizeof( path ) - len ) ;
					       PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )path )[ len ], sizeof( path ) - len ) ;

					p = ( wchar_t * )( ( BYTE * )p + CharBytes1 + CharBytes2 ) ;
				}
			}

			// �f�B���N�g��������
			i = 0 ;
			for(;;)
			{
				CharCode1 = GetCharCode( ( const char * )p, WCHAR_T_CHARCODEFORMAT, &CharBytes1 ) ;
				p = ( wchar_t * )( ( BYTE * )p + CharBytes1 ) ;
				if( CharCode1 == '\0' || CharCode1 == '/' || CharCode1 == '\\' )
				{
					break ;
				}

				len += PutCharCode( CharCode1, WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )path )[ len ], sizeof( path ) - len ) ;
				i   += PutCharCode( CharCode1, WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )dir  )[ i   ], sizeof( dir  ) - i ) ;

			}

			if( CharCode1 == '\0' || i == 0 )
			{
				return -1 ;
			}

			PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )path )[ len ], sizeof( path ) - len ) ;
			PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )dir  )[ i   ], sizeof( dir  ) - i   ) ;

			// �t�H���_����DX�A�[�J�C�u�t�@�C�����ɂ���
			{
				int TempLen ;

				_MEMCPY( temp, path, len ) ;
				TempLen = len ;

				TempLen += PutCharCode( '.', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen ) ;
				if( DXARCD.ArchiveExtensionLength == 0 )
				{
					TempLen += PutCharCode( 'D',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen ) ;
					TempLen += PutCharCode( 'X',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen ) ;
					TempLen += PutCharCode( 'A',  WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen ) ;
					TempLen += PutCharCode( '\0', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen ) ;
				}
				else
				{
					_WCSCPY_S( ( wchar_t * )&( ( BYTE * )temp )[ TempLen ], sizeof( temp ) - TempLen, DXARCD.ArchiveExtension ) ;
				}
			}

			// �c�w�`�t�@�C���Ƃ��ĊJ���Ă݂�
			arcindex = DXA_DIR_OpenArchive( temp ) ;
			if( arcindex != -1 )
			{
				break ;
			}

			// �J���Ȃ������玟�̊K�w��
			len += PutCharCode( '\\', WCHAR_T_CHARCODEFORMAT, ( char * )&( ( BYTE * )path )[ len ], sizeof( path ) - len ) ;
		}

		// �J�����獡��̏���ۑ�����
		_WCSCPY_S( DXARCD.BackUseDirectory, sizeof( DXARCD.BackUseDirectory ), temp ) ;
//		if( DXARCD.ArchiveExtensionLength == 0 )
//		{
//			_MEMCPY( DXARCD.BackUseDirectory, temp, len + ( 1 /* . */ + 3 /* DXA */ + 1 /* '\0' */ ) * sizeof( wchar_t ) ) ;
//		}
//		else
//		{
//			_MEMCPY( DXARCD.BackUseDirectory, temp, len + ( 1 /* . */ + DXARCD.ArchiveExtensionLength + 1 /* '\0' */ ) * sizeof( wchar_t ) ) ;
//		}
		DXARCD.BackUseDirectoryPathLength = len ;
		DXARCD.BackUseArchiveIndex        = arcindex ;
	}

	// �����Z�b�g����
	*ArchiveIndex = arcindex;

	if( ArchiveFilePath )
	{
		int DestCharCodeFormat ;

		DestCharCodeFormat = DXARCD.Archive[ arcindex ]->Archive.CharCodeFormat ;
		ConvString( ( const char * )p, WCHAR_T_CHARCODEFORMAT, ( char * )ArchiveFilePath, BufferBytes, DestCharCodeFormat ) ;
	}

	// �I��
	return 0;
}

// �A�[�J�C�u�t�@�C�����J��
static int DXA_DIR_OpenArchive( const wchar_t *FilePath, void *FileImage, int FileSize, int FileImageCopyFlag, int FileImageReadOnly, int ArchiveIndex, int OnMemory, int ASyncThread )
{
	int					i ;
	int					index ;
	int					newindex ;
	DXARC_DIR_ARCHIVE *	arc ;
	DXARC_DIR_ARCHIVE *	tarc ;
	DXARC				temparc ;

	// �A�[�J�C�u�̎w�肪����ꍇ�͂��̂܂܎g�p����
	if( ArchiveIndex != -1 )
	{
		tarc = DXARCD.Archive[ ArchiveIndex ] ;
		if( tarc != NULL )
		{
			if(	_WCSCMP( FilePath, tarc->Path ) == 0 )
			{
				DXARCD.Archive[ ArchiveIndex ]->UseCounter ++ ;
				return ArchiveIndex ;
			}
		}
	}

	// ���ɊJ����Ă��邩���ׂ�
	newindex = -1 ;
	index    = 0 ;
	for( i = 0 ; i < DXARCD.ArchiveNum ; index ++ )
	{
		arc = DXARCD.Archive[ index ] ;
		if( arc == NULL )
		{
			newindex = index ;
			continue ;
		}
		
		i ++ ;

		if( _WCSCMP( arc->Path, FilePath ) == 0 )
		{
			// ���ɊJ����Ă����ꍇ�͂��̃C���f�b�N�X��Ԃ�
			arc->UseCounter ++ ;
			return index ;
		}
	}
	
	// �Ȃ������ꍇ�͐V�K�Ƀf�[�^��ǉ�����

	// �n���h�������Ɉ�t�̏ꍇ�͎g�p����Ă��Ȃ��A�[�J�C�u�n���h�����������
	if( DXARCD.ArchiveNum == DXA_DIR_MAXARCHIVENUM )
	{
		// ���g�p�̃n���h�������
		DXA_DIR_CloseWaitArchive() ;
		
		// ����ł���t�ł���ꍇ�̓G���[
		if( DXARCD.ArchiveNum == DXA_DIR_MAXARCHIVENUM )
		{
			return -1 ;
		}
	} 
	if( newindex == -1 )
	{
		for( newindex = 0 ; DXARCD.Archive[ newindex ] != NULL ; newindex ++ ){}
	}

	// �A�[�J�C�u�t�@�C�������݂��Ă��邩�m�F���Ă珉��������
	DXA_Initialize( &temparc ) ;
	if( FileImage != NULL )
	{
		// ��������ɓW�J���ꂽ�t�@�C���C���[�W���g�p����ꍇ
		if( DXA_OpenArchiveFromMem( &temparc, FileImage, FileSize, FileImageCopyFlag, FileImageReadOnly, DXARCD.ValidKeyString == TRUE ? DXARCD.KeyString : NULL, FilePath ) < 0 )
			return -1 ;
	}
	else
	if( OnMemory == TRUE )
	{
		// �������ɓǂݍ��ޏꍇ
		if( DXA_OpenArchiveFromFileUseMem( &temparc, FilePath, DXARCD.ValidKeyString == TRUE ? DXARCD.KeyString : NULL, ASyncThread ) < 0 )
			return -1 ;
	}
	else
	{
		// �t�@�C������ǂݍ��ޏꍇ
		if( DXA_OpenArchiveFromFile( &temparc, FilePath, DXARCD.ValidKeyString == TRUE ? DXARCD.KeyString : NULL ) < 0 )
			return -1 ;
	}

	// �V�����A�[�J�C�u�f�[�^�p�̃��������m�ۂ���
	arc = DXARCD.Archive[ newindex ] = ( DXARC_DIR_ARCHIVE * )DXALLOC( sizeof( DXARC_DIR_ARCHIVE ) ) ;
	if( DXARCD.Archive[ newindex ] == NULL )
	{
		DXA_CloseArchive( &temparc ) ;
		DXA_Terminate( &temparc ) ;
		return -1 ;
	}

	// ���Z�b�g
	_MEMCPY( &arc->Archive, &temparc, sizeof( DXARC ) ) ;
	arc->UseCounter = 1 ;
	_WCSCPY_S( arc->Path, sizeof( arc->Path ), FilePath ) ;

	// �g�p���̃A�[�J�C�u�̐��𑝂₷
	DXARCD.ArchiveNum ++ ;

	// �C���f�b�N�X��Ԃ�
	return newindex ;
}

// ���ɊJ����Ă���A�[�J�C�u�̃n���h�����擾����( �߂�l: -1=�������� 0�ȏ�:�n���h�� )
static int DXA_DIR_GetArchive( const wchar_t *FilePath, void *FileImage )
{
	int i, index ;
	DXARC_DIR_ARCHIVE *arc ;

	index = 0 ;
	for( i = 0 ; i < DXARCD.ArchiveNum ; index ++ )
	{
		arc = DXARCD.Archive[index] ;
		if( arc == NULL )
		{
			continue ;
		}

		i ++ ;

		if( FilePath )
		{
			if( _WCSCMP( arc->Path, FilePath ) == 0 )
			{
				return index ;
			}
		}
		else
		{
			if( arc->Archive.MemoryImageCopyFlag )
			{
				if( arc->Archive.MemoryImageOriginal == FileImage )
				{
					return index ;
				}
			}
			else
			{
				if( arc->Archive.MemoryImage == FileImage )
				{
					return index ;
				}
			}
		}
	}

	return -1 ;
}

// �A�[�J�C�u�t�@�C�������
static int DXA_DIR_CloseArchive( int ArchiveHandle )
{
	DXARC_DIR_ARCHIVE *arc ;

	// �g�p����Ă��Ȃ������牽�������I��
	arc = DXARCD.Archive[ArchiveHandle] ;
	if( arc == NULL || arc->UseCounter == 0 )
	{
		return -1 ;
	}

	// �Q�ƃJ�E���^�����炷
	arc->UseCounter -- ;

	// �I��
	return 0 ;
}

// �g�p�����̂�҂��Ă���A�[�J�C�u�t�@�C����S�ĕ���
static void DXA_DIR_CloseWaitArchive( void )
{
	int i, Num, index ;
	DXARC_DIR_ARCHIVE *arc ;
	
	Num = DXARCD.ArchiveNum ;
	for( i = 0, index = 0 ; i < Num ; index ++ )
	{
		if( DXARCD.Archive[index] == NULL )
		{
			continue ;
		}
		i ++ ;

		arc = DXARCD.Archive[index] ;

		// �g���Ă����������Ȃ�
		if( arc->UseCounter > 0 )
		{
			continue ;
		}

		// ��n��
		DXA_CloseArchive( &arc->Archive ) ;
		DXA_Terminate( &arc->Archive ) ;
		DXFREE( arc ) ;
		DXARCD.Archive[index] = NULL ;
		
		// �A�[�J�C�u�̐������炷
		DXARCD.ArchiveNum -- ;
	}
}

// �A�[�J�C�u���f�B���N�g���Ɍ����Ă鏈���̏�����
extern	int DXA_DIR_Initialize( void )
{
	// ���ɏ������ς݂̏ꍇ�͉������Ȃ�
	if( DXARCD.InitializeFlag )
	{
		return -1 ;
	}

	// �N���e�B�J���Z�N�V�����̏�����
	CriticalSection_Initialize( &DXARCD.CriticalSection ) ;

//	_MEMSET( &DXARCD, 0, sizeof( DXARC_DIR ) ) ;
//	DXA_DIR_Terminate() ;

	// �g�p����Ă��Ȃ��A�[�J�C�u�t�@�C�����������
	DXA_DIR_CloseWaitArchive() ;

	// �����������t���O�𗧂Ă�
	DXARCD.InitializeFlag = TRUE ;

	// �I��
	return 0 ;
}


// �A�[�J�C�u���f�B���N�g���Ɍ����Ă鏈���̌�n��
extern int DXA_DIR_Terminate( void )
{
	// ���Ɍ�n���ς݂̏ꍇ�͉������Ȃ�
	if( DXARCD.InitializeFlag == FALSE )
	{
		return -1 ;
	}

	// �g�p����Ă��Ȃ��A�[�J�C�u�t�@�C�����������
	DXA_DIR_CloseWaitArchive() ;

	// �N���e�B�J���Z�N�V�����̌�n��
	CriticalSection_Delete( &DXARCD.CriticalSection ) ;

	// �����������t���O��|��
	DXARCD.InitializeFlag = FALSE ;

	// �I��
	return 0 ;
}

// �A�[�J�C�u�t�@�C���̊g���q��ݒ肷��
extern int DXA_DIR_SetArchiveExtension( const wchar_t *Extension )
{
	int Length ;
	
	Length = ( int )_WCSLEN( Extension ) ;

	if( Length >= 64 || Extension == NULL || Extension[0] == _T( '\0' ) )
	{
		DXARCD.ArchiveExtension[ 0 ]  = 0 ;
		DXARCD.ArchiveExtensionLength = 0 ;
	}
	else
	{
		DXARCD.ArchiveExtensionLength = Length ;
		_WCSCPY_S( DXARCD.ArchiveExtension, sizeof( DXARCD.ArchiveExtension ), Extension ) ;
	}

	// �I��
	return 0 ;
}

// �A�[�J�C�u�t�@�C���ƒʏ�̃t�H���_�̂ǂ�������݂����ꍇ�A�ǂ����D�悳���邩��ݒ肷��( 1:�t�H���_��D�� 0:�c�w�A�[�J�C�u�t�@�C����D��(�f�t�H���g) )
extern int DXA_DIR_SetDXArchivePriority( int Priority )
{
	DXARCD.DXAPriority = Priority ;

	// �I��
	return 0 ;
}

// �A�[�J�C�u�t�@�C���̌��������ݒ肷��
extern int DXA_DIR_SetKeyString( const char *KeyString )
{
	if( KeyString == NULL )
	{
		DXARCD.ValidKeyString = FALSE ;
	}
	else
	{
		DXARCD.ValidKeyString = TRUE ;
		if( _STRLEN( KeyString ) > DXA_KEYSTR_LENGTH )
		{
			_MEMCPY( DXARCD.KeyString, KeyString, DXA_KEYSTR_LENGTH ) ;
			DXARCD.KeyString[ DXA_KEYSTR_LENGTH ] = '\0' ;
		}
		else
		{
			_STRCPY( DXARCD.KeyString, KeyString ) ;
		}
	}

	// �I��
	return 0 ;
}

// �t�@�C�����ۂ��Ɠǂݍ��ފ֐�
extern LONGLONG DXA_DIR_LoadFile( const wchar_t *FilePath, void *Buffer, int BufferSize )
{
	LONGLONG siz ;
	DWORD_PTR handle ;

	handle = DXA_DIR_Open( FilePath ) ;
	if( handle == 0 )
	{
		return false ;
	}

	DXA_DIR_Seek( handle, 0L, SEEK_END ) ;
	siz = DXA_DIR_Tell( handle ) ;
	DXA_DIR_Seek( handle, 0L, SEEK_SET ) ;

	if( siz <= BufferSize )
	{
		DXA_DIR_Read( Buffer, ( size_t )siz, 1, handle ) ;
	}

	DXA_DIR_Close( handle ) ;

	// �I��
	return siz ;
}

// DXA_DIR_Open �̊�{�֐�
extern DWORD_PTR DXA_DIR_Open( const wchar_t *FilePath, int UseCacheFlag, int BlockReadFlag, int UseASyncReadFlag )
{
	int index ;
	DXARC_DIR_FILE *file ;
//	char DXAErrorStr[ FILEPATH_MAX ] ;
	BYTE DxaInFilePath[ FILEPATH_MAX ] ;

	// ����������Ă��Ȃ������珉��������
	if( DXARCD.InitializeFlag == FALSE )
	{
		DXA_DIR_Initialize() ;
	}

	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &DXARCD.CriticalSection ) ;

//	UseCacheFlag  = UseCacheFlag ;
//	BlockReadFlag = BlockReadFlag ;
//	DXAErrorStr[ 0 ] = 0 ;

	// �󂫃f�[�^��T��
	if( DXARCD.FileNum == DXA_DIR_MAXFILENUM )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &DXARCD.CriticalSection ) ;

		DXST_ERRORLOG_ADDUTF16LE( "\x0c\x54\x42\x66\x6b\x30\x8b\x95\x51\x30\x8b\x30\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\x70\x65\x4c\x30\x50\x96\x4c\x75\x92\x30\x85\x8d\x48\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�����ɊJ����t�@�C���̐������E�𒴂��܂���\n" @*/ ) ;
		return 0 ;
	}
	for( index = 0 ; DXARCD.File[index] != NULL ; index ++ ){}

	// �������̊m��
	DXARCD.File[ index ] = (DXARC_DIR_FILE *)DXALLOC( sizeof( DXARC_DIR_FILE ) ) ;
	if( DXARCD.File[ index ] == NULL )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &DXARCD.CriticalSection ) ;

		DXST_ERRORLOG_ADDUTF16LE( "\xd5\x30\xa1\x30\xa4\x30\xeb\x30\x6e\x30\xc5\x60\x31\x58\x92\x30\x3c\x68\x0d\x7d\x59\x30\x8b\x30\xe1\x30\xe2\x30\xea\x30\x6e\x30\xba\x78\xdd\x4f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�t�@�C���̏����i�[���郁�����̊m�ۂɎ��s���܂���\n" @*/ ) ;
		return 0 ;
	}
	file = DXARCD.File[index] ;

	// �A�[�J�C�u�t�@�C���ƃt�H���_�̂ǂ����D�悷�邩�ŏ����𕪊�
	if( DXARCD.DXAPriority == 0 )
	{
		int FileOpen = FALSE ;

		// �A�[�J�C�u��D�悷��ꍇ

		// �A�[�J�C�u�t�@�C�������������ׂ�
		if( DXA_DIR_OpenTest( FilePath, ( int * )&file->UseArchiveIndex, DxaInFilePath, sizeof( DxaInFilePath ) ) == 0 )
		{
			// �f�B���N�g�����Ɠ����̂c�w�`�t�@�C�����J�����炻�̒�����w��̃t�@�C����ǂݍ������Ƃ���
			if( DXA_STREAM_Initialize( &file->DXAStream, &DXARCD.Archive[ file->UseArchiveIndex ]->Archive, DxaInFilePath, UseASyncReadFlag ) < 0 )
			{
//				_STRCPY( DXAErrorStr, DXSTRING( "�c�w�`�t�@�C���̒��Ɏw��̃t�@�C���͌�����܂���ł���\n" ) ) ;
				DXA_DIR_CloseArchive( ( int )file->UseArchiveIndex ) ;
			}
			else
			{
				// �A�[�J�C�u���g�p���Ă���t���O�𗧂Ă�
				file->UseArchiveFlag = 1 ;

				FileOpen = TRUE ;
			}
		}

		if( FileOpen == FALSE )
		{
			// �A�[�J�C�u�t�@�C�������������畁�ʂ̃t�@�C������ǂݍ��ސݒ���s��
			file->UseArchiveFlag = 0 ;

			// ���ʂ̃t�@�C�������������ׂ�
			file->WinFilePointer_ = ReadOnlyFileAccessOpen( FilePath, UseCacheFlag, TRUE, UseASyncReadFlag ) ;
			if( file->WinFilePointer_ == 0 )
			{
				goto ERR ;
			}
			else
			{
				FileOpen = TRUE ;
			}
		}
	}
	else
	{
		// ���ʂ̃t�@�C����D�悷��ꍇ

		// ���ʂ̃t�@�C�������������ׂ�
		if( ( file->WinFilePointer_ = ReadOnlyFileAccessOpen( FilePath, UseCacheFlag, TRUE, UseASyncReadFlag ) ) != 0 )
		{
			// �J�����畁�ʂ̃t�@�C������ǂݍ��ސݒ���s��
			file->UseArchiveFlag = 0 ;
		}
		else
		{
			// �A�[�J�C�u�t�@�C�������������ׂ�
			if( DXA_DIR_OpenTest( FilePath, (int *)&file->UseArchiveIndex, DxaInFilePath, sizeof( DxaInFilePath ) ) == 0 )
			{
				// �f�B���N�g�����Ɠ����̂c�w�`�t�@�C�����J�����炻�̒�����w��̃t�@�C����ǂݍ������Ƃ���
				if( DXA_STREAM_Initialize( &file->DXAStream, &DXARCD.Archive[ file->UseArchiveIndex ]->Archive, DxaInFilePath, UseASyncReadFlag ) < 0 )
				{
//					_STRCPY( DXAErrorStr, DXSTRING( "�c�w�`�t�@�C���̒��Ɏw��̃t�@�C���͌�����܂���ł���\n" ) ) ;
					DXA_DIR_CloseArchive( ( int )file->UseArchiveIndex ) ;
					goto ERR ;
				}
				else
				{
					// �A�[�J�C�u���g�p���Ă���t���O�𗧂Ă�
					file->UseArchiveFlag = 1 ;
				}
			}
			else
			{
				// ����������G���[
				goto ERR;
			}
		}
	}

	// �n���h���̐��𑝂₷
	DXARCD.FileNum ++ ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &DXARCD.CriticalSection ) ;

	// �C���f�b�N�X��Ԃ�
	return index | 0xF0000000 ;

ERR:
	// �������̉��
	if( DXARCD.File[index] != NULL ) DXFREE( DXARCD.File[index] ) ;
	DXARCD.File[index] = NULL ;
	
	// �G���[������o��
//	DXST_ERRORLOGFMT_ADDW(( L"�t�@�C�� %s �̃I�[�v���Ɏ��s���܂���\n", FilePath )) ;
//	if( DXAErrorStr[0] != '\0' ) DXST_ERRORLOGFMT_ADDW(( "�c�w�`�G���[�F%s", DXAErrorStr )) ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &DXARCD.CriticalSection ) ;

	// �G���[�I��
	return 0 ;
}

// �t�@�C�������
extern int DXA_DIR_Close( DWORD_PTR Handle )
{
	// �N���e�B�J���Z�N�V�����̎擾
	CRITICALSECTION_LOCK( &DXARCD.CriticalSection ) ;

	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;

	// �g�p����Ă��Ȃ������牽�������I��
	if( file == NULL )
	{
		// �N���e�B�J���Z�N�V�����̉��
		CriticalSection_Unlock( &DXARCD.CriticalSection ) ;

		return -1 ;
	}
	
	// �A�[�J�C�u���g�p���Ă��邩�ǂ����ŕ���
	if( file->UseArchiveFlag == FALSE )
	{
		// �g�p���Ă��Ȃ��ꍇ�͕W�����o�͂̃t�@�C���|�C���^���������
		ReadOnlyFileAccessClose( file->WinFilePointer_ ) ;
		file->WinFilePointer_ = 0 ;
	}
	else
	{
		// �A�[�J�C�u���g�p���Ă����ꍇ�̓A�[�J�C�u�̎Q�Ɛ������炷
		
		// �A�[�J�C�u�t�@�C���̎Q�Ɛ������炷
		DXA_DIR_CloseArchive( ( int )file->UseArchiveIndex ) ;

		// �A�[�J�C�u�t�@�C���̌�n��
		DXA_STREAM_Terminate( &file->DXAStream ) ;
	}

	// ���������������
	DXFREE( file ) ;
	DXARCD.File[Handle & 0x0FFFFFFF] = NULL ;
	
	// �������炷
	DXARCD.FileNum -- ;

	// �N���e�B�J���Z�N�V�����̉��
	CriticalSection_Unlock( &DXARCD.CriticalSection ) ;

	// �I��
	return 0 ;
}

// �t�@�C���|�C���^�̈ʒu���擾����
extern	LONGLONG DXA_DIR_Tell( DWORD_PTR Handle )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL )
	{
		return -1 ;
	}

	if( file->UseArchiveFlag == 0 )
	{
		return ReadOnlyFileAccessTell( file->WinFilePointer_ ) ;
	}
	else
	{
		return DXA_STREAM_Tell( &file->DXAStream ) ;
	}
}

// �t�@�C���|�C���^�̈ʒu��ύX����
extern int DXA_DIR_Seek( DWORD_PTR Handle, LONGLONG SeekPoint, int SeekType )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL )
	{
		return -1 ;
	}

	if( file->UseArchiveFlag == 0 )
	{
		return ReadOnlyFileAccessSeek( file->WinFilePointer_, SeekPoint, SeekType ) ;
	}
	else
	{
		return DXA_STREAM_Seek( &file->DXAStream, SeekPoint, SeekType ) ;
	}
}

// �t�@�C������f�[�^��ǂݍ���
extern size_t DXA_DIR_Read( void *Buffer, size_t BlockSize, size_t BlockNum, DWORD_PTR Handle )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL )
	{
		return ( size_t )-1 ;
	}

	if( file->UseArchiveFlag == 0 )
	{
		return ReadOnlyFileAccessRead( Buffer, BlockSize, BlockNum, file->WinFilePointer_ ) ;
	}
	else
	{
		return DXA_STREAM_Read( &file->DXAStream, Buffer, BlockSize * BlockNum ) / BlockSize ;
	}
}

// �t�@�C���̏I�[�𒲂ׂ�
extern int DXA_DIR_Eof( DWORD_PTR Handle )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL )
	{
		return -1 ;
	}

	if( file->UseArchiveFlag == 0 )
	{
		return ReadOnlyFileAccessEof( file->WinFilePointer_ ) ;
	}
	else
	{
		return DXA_STREAM_Eof( &file->DXAStream ) ;
	}
}

extern	int DXA_DIR_ChDir( const wchar_t *Path )
{
	_WCHDIR( Path ) ;

	return 0 ;
}

extern	int DXA_DIR_GetDir( wchar_t *Buffer )
{
	_WGETCWD( Buffer, BUFFERBYTES_CANCEL ) ;

	return 0 ;
}

extern	int DXA_DIR_GetDirS( wchar_t *Buffer, size_t BufferBytes )
{
	_WGETCWD( Buffer, BufferBytes ) ;

	return 0 ;
}

extern	int DXA_DIR_IdleCheck( DWORD_PTR Handle )
{
	DXARC_DIR_FILE *file = DXARCD.File[Handle & 0x0FFFFFFF] ;
	if( file == NULL )
	{
		return -1 ;
	}

	if( file->UseArchiveFlag == 0 )
	{
		return ReadOnlyFileAccessIdleCheck( file->WinFilePointer_ ) ;
	}
	else
	{
		return DXA_STREAM_IdleCheck( &file->DXAStream ) ;
	}
}

// �߂�l: -1=�G���[  -1�ȊO=FindHandle
extern DWORD_PTR DXA_DIR_FindFirst( const wchar_t *FilePath, FILEINFOW *Buffer )
{
	DXA_DIR_FINDDATA *find ;
	BYTE nPath[ FILEPATH_MAX ] ;

	// �������̊m��
	find = ( DXA_DIR_FINDDATA * )DXALLOC( sizeof( DXA_DIR_FINDDATA ) ) ;
	if( find == NULL )
	{
		return ( DWORD_PTR )-1 ;
	}
	_MEMSET( find, 0, sizeof( *find ) ) ;

	// �w��̃I�u�W�F�N�g���A�[�J�C�u�t�@�C���������ׂ�
	if( DXA_DIR_OpenTest( FilePath, &find->UseArchiveIndex, nPath, sizeof( nPath ) ) == -1 )
	{
		// �A�[�J�C�u�t�@�C�����ł͂Ȃ������ꍇ�̓t�@�C�����猟������
		find->UseArchiveFlag = 0 ;
		find->FindHandle = ReadOnlyFileAccessFindFirst( FilePath, Buffer ) ;
	}
	else
	{
		// �A�[�J�C�u�t�@�C�����̏ꍇ�̓A�[�J�C�u�t�@�C�������猟������
		find->UseArchiveFlag = 1 ;
		find->FindHandle = DXA_FindFirst( &DXARCD.Archive[ find->UseArchiveIndex ]->Archive, nPath, Buffer ) ;
	}

	// �����n���h�����擾�ł��Ȃ������ꍇ�̓G���[
	if( find->FindHandle == ( DWORD_PTR )-1 )
	{
		// �A�[�J�C�u�t�@�C�����̏ꍇ�̓A�[�J�C�u�t�@�C���̎g�p�J�E���g�����炷
		if( find->UseArchiveFlag != 0 )
		{
			DXA_DIR_CloseArchive( find->UseArchiveIndex ) ;
		}

		DXFREE( find );
		return ( DWORD_PTR )-1 ;
	}

	// �n���h����Ԃ�
	return (DWORD_PTR)find ;
}

// �߂�l: -1=�G���[  0=����
extern int DXA_DIR_FindNext( DWORD_PTR FindHandle, FILEINFOW *Buffer )
{
	DXA_DIR_FINDDATA *find;

	find = (DXA_DIR_FINDDATA *)FindHandle;
	if( find->UseArchiveFlag == 0 )
	{
		return ReadOnlyFileAccessFindNext( find->FindHandle, Buffer ) ;
	}
	else
	{
		return DXA_FindNext( find->FindHandle, Buffer ) ;
	}
}

// �߂�l: -1=�G���[  0=����
extern int DXA_DIR_FindClose( DWORD_PTR FindHandle )
{
	DXA_DIR_FINDDATA *find;

	find = (DXA_DIR_FINDDATA *)FindHandle;
	if( find->UseArchiveFlag == 0 )
	{
		ReadOnlyFileAccessFindClose( find->FindHandle ) ;
	}
	else
	{
		DXA_FindClose( find->FindHandle );
		DXA_DIR_CloseArchive( find->UseArchiveIndex ) ;
	}

	DXFREE( find );

	return 0;
}




// �w��̂c�w�`�t�@�C�����ۂ��ƃ������ɓǂݍ���( �߂�l: -1=�G���[  0=���� )
extern int NS_DXArchivePreLoad( const TCHAR *FilePath , int ASyncThread )
{
	wchar_t fullpath[ FILEPATH_MAX ] ;

	// �t���p�X�𓾂�(���łɑS�Ă̕�����啶���ɂ���)
#ifdef UNICODE
	DXA_DIR_ConvertFullPath( FilePath, fullpath, sizeof( fullpath ) ) ;
#else // UNICODE
	wchar_t FilePathBuffer[ FILEPATH_MAX ] ;

	ConvString( ( const char * )FilePath, _TCHARCODEFORMAT, ( char * )FilePathBuffer, sizeof( FilePathBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
	DXA_DIR_ConvertFullPath( FilePathBuffer, fullpath, sizeof( fullpath ) ) ;
#endif // UNICODE

	return DXA_DIR_OpenArchive( fullpath, NULL, -1, FALSE, FALSE, -1, TRUE, ASyncThread ) == -1 ? -1 : 0 ;
}

// �w��̂c�w�`�t�@�C���̎��O�ǂݍ��݂������������ǂ������擾����( �߂�l�F TRUE=�������� FALSE=�܂� )
extern int NS_DXArchiveCheckIdle( const TCHAR *FilePath )
{
	int handle;
	wchar_t fullpath[FILEPATH_MAX];

	// �t���p�X�𓾂�(���łɑS�Ă̕�����啶���ɂ���)
#ifdef UNICODE
	DXA_DIR_ConvertFullPath( FilePath, fullpath, sizeof( fullpath ) ) ;
#else // UNICODE
	wchar_t FilePathBuffer[ FILEPATH_MAX ] ;

	ConvString( ( const char * )FilePath, _TCHARCODEFORMAT, ( char * )FilePathBuffer, sizeof( FilePathBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
	DXA_DIR_ConvertFullPath( FilePathBuffer, fullpath, sizeof( fullpath ) ) ;
#endif // UNICODE

	// �t�@�C���p�X����n���h�����擾����
	handle = DXA_DIR_GetArchive( fullpath ) ;
	if( handle == -1 )
	{
		return 0 ;
	}

	// �����������������ǂ����𓾂�
	return DXA_CheckIdle( &DXARCD.Archive[handle]->Archive ) ;
}

// �w��̂c�w�`�t�@�C��������������������
extern int NS_DXArchiveRelease( const TCHAR *FilePath )
{
	int handle;
	wchar_t fullpath[ FILEPATH_MAX ] ;

	// �t���p�X�𓾂�(���łɑS�Ă̕�����啶���ɂ���)
#ifdef UNICODE
	DXA_DIR_ConvertFullPath( FilePath, fullpath, sizeof( fullpath ) ) ;
#else // UNICODE
	wchar_t FilePathBuffer[ FILEPATH_MAX ] ;

	ConvString( ( const char * )FilePath, _TCHARCODEFORMAT, ( char * )FilePathBuffer, sizeof( FilePathBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
	DXA_DIR_ConvertFullPath( FilePathBuffer, fullpath, sizeof( fullpath ) ) ;
#endif // UNICODE

	// �t�@�C���p�X����n���h�����擾����
	handle = DXA_DIR_GetArchive( fullpath ) ;
	if( handle == -1 )
	{
		return 0 ;
	}

	// ����
	DXA_DIR_CloseArchive( handle ) ;
	DXA_DIR_CloseWaitArchive() ;

	// �I��
	return 0 ;
}

// �c�w�`�t�@�C���̒��Ɏw��̃t�@�C�������݂��邩�ǂ����𒲂ׂ�ATargetFilePath �͂c�w�`�t�@�C�����J�����g�t�H���_�Ƃ����ꍇ�̃p�X( �߂�l:  -1=�G���[  0:����  1:���� )
extern int NS_DXArchiveCheckFile( const TCHAR *FilePath, const TCHAR *TargetFilePath )
{
	int index, ret ;
	DXARC_DIR_ARCHIVE *Archive ;
	wchar_t fullpath[ FILEPATH_MAX ] ;

	// �t���p�X�𓾂�(���łɑS�Ă̕�����啶���ɂ���)
#ifdef UNICODE
	DXA_DIR_ConvertFullPath( FilePath, fullpath, sizeof( fullpath ) ) ;
#else // UNICODE
	wchar_t FilePathBuffer[ FILEPATH_MAX ] ;

	ConvString( ( const char * )FilePath, _TCHARCODEFORMAT, ( char * )FilePathBuffer, sizeof( FilePathBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
	DXA_DIR_ConvertFullPath( FilePathBuffer, fullpath, sizeof( fullpath ) ) ;
#endif // UNICODE

	// �A�[�J�C�u�t�@�C�������邩�ǂ������ׂ�
	index = DXA_DIR_OpenArchive( fullpath ) ;
	if( index == -1 )
	{
		return -1 ;
	}

	// �A�[�J�C�u�̒��Ɏw��̃t�@�C�������邩�ǂ����𒲂ׂ�
	Archive = DXARCD.Archive[ index ] ;
	ret = DXA_GetFileInfo( &Archive->Archive, _TCHARCODEFORMAT, ( const char * )TargetFilePath, NULL, NULL ) ;

	DXA_DIR_CloseArchive( index ) ;

	// ���ʂ�Ԃ�
	return ret == -1 ? 0 : 1 ;
}

// ��������ɓW�J���ꂽ�c�w�`�t�@�C�����w��̃t�@�C���p�X�ɂ��邱�Ƃɂ���
extern int NS_DXArchiveSetMemImage(		void *ArchiveImage, int ArchiveImageSize, const TCHAR *EmulateFilePath, int ArchiveImageCopyFlag, int ArchiveImageReadOnly )
{
	wchar_t fullpath[ FILEPATH_MAX ] ;

	// �t���p�X�𓾂�(���łɑS�Ă̕�����啶���ɂ���)
#ifdef UNICODE
	DXA_DIR_ConvertFullPath( EmulateFilePath, fullpath, sizeof( fullpath ) ) ;
#else // UNICODE
	wchar_t FilePathBuffer[ FILEPATH_MAX ] ;

	ConvString( ( const char * )EmulateFilePath, _TCHARCODEFORMAT, ( char * )FilePathBuffer, sizeof( FilePathBuffer ), WCHAR_T_CHARCODEFORMAT ) ;
	DXA_DIR_ConvertFullPath( FilePathBuffer, fullpath, sizeof( fullpath ) ) ;
#endif // UNICODE

	return DXA_DIR_OpenArchive( fullpath, ArchiveImage, ArchiveImageSize, ArchiveImageCopyFlag, ArchiveImageReadOnly, -1, FALSE, FALSE ) == -1 ? -1 : 0;
}

// DXArchiveSetMemImage �̐ݒ����������
extern int NS_DXArchiveReleaseMemImage(	void *ArchiveImage )
{
	int handle;

	// �t�@�C���p�X����n���h�����擾����
	handle = DXA_DIR_GetArchive( NULL, ArchiveImage ) ;
	if( handle == -1 )
	{
		return 0 ;
	}

	// ����
	DXA_DIR_CloseArchive( handle ) ;
	DXA_DIR_CloseWaitArchive() ;

	// �I��
	return 0 ;
}

#endif


// �f�[�^���𓀂���( �߂�l:�𓀌�̃f�[�^�T�C�Y )
#define MIN_COMPRESS		(4)						// �Œሳ�k�o�C�g��
#define MAX_SEARCHLISTNUM	(64)					// �ő��v����T���ׂ̃��X�g��H��ő吔
#define MAX_SUBLISTNUM		(65536)					// ���k���ԒZ�k�̂��߂̃T�u���X�g�̍ő吔
#define MAX_COPYSIZE 		(0x1fff + MIN_COMPRESS)	// �Q�ƃA�h���X����R�s�[�o�؂�ő�T�C�Y( ���k�R�[�h���\���ł���R�s�[�T�C�Y�̍ő�l + �Œሳ�k�o�C�g�� )
#define MAX_ADDRESSLISTNUM	(1024 * 1024 * 1)		// �X���C�h�����̍ő�T�C�Y
#define MAX_POSITION		(1 << 24)				// �Q�Ɖ\�ȍő告�΃A�h���X( 16MB )

// ���k���ԒZ�k�p���X�g
typedef struct LZ_LIST
{
	LZ_LIST *next, *prev ;
	DWORD address ;
} LZ_LIST ;

// �f�[�^�����k����( �߂�l:���k��̃f�[�^�T�C�Y )
extern	int	DXA_Encode( void *Src, DWORD SrcSize, void *Dest )
{
	int dstsize ;
	int    bonus,    conbo,    conbosize,    address,    addresssize ;
	int maxbonus, maxconbo, maxconbosize, maxaddress, maxaddresssize ;
	BYTE keycode, *srcp, *destp, *dp, *sp, *sp2, *sp1 ;
	DWORD srcaddress, code ;
	int j ;
	DWORD i, m ;
	DWORD maxlistnum, maxlistnummask, listaddp ;
	DWORD sublistnum, sublistmaxnum ;
	LZ_LIST *listbuf, *listtemp, *list, *newlist ;
	BYTE *listfirsttable, *usesublistflagtable, *sublistbuf ;
	
	// �T�u���X�g�̃T�C�Y�����߂�
	{
			 if( SrcSize < 100 * 1024 )			sublistmaxnum = 1 ;
		else if( SrcSize < 3 * 1024 * 1024 )	sublistmaxnum = MAX_SUBLISTNUM / 3 ;
		else									sublistmaxnum = MAX_SUBLISTNUM ;
	}

	// ���X�g�̃T�C�Y�����߂�
	{
		maxlistnum = MAX_ADDRESSLISTNUM ;
		if( maxlistnum > SrcSize )
		{
			while( ( maxlistnum >> 1 ) > 0x100 && ( maxlistnum >> 1 ) > SrcSize )
				maxlistnum >>= 1 ;
		}
		maxlistnummask = maxlistnum - 1 ;
	}

	// �������̊m��
	usesublistflagtable   = (BYTE *)DXALLOC(
		sizeof( DWORD_PTR )	* 65536 +					// ���C�����X�g�̐擪�I�u�W�F�N�g�p�̈�
		sizeof( LZ_LIST   )	* maxlistnum +				// ���C�����X�g�p�̈�
		sizeof( BYTE      )	* 65536 +					// �T�u���X�g���g�p���Ă��邩�t���O�p�̈�
		sizeof( DWORD_PTR )	* 256 * sublistmaxnum ) ;	// �T�u���X�g�p�̈�
		
	// �A�h���X�̃Z�b�g
	listfirsttable =     usesublistflagtable + sizeof( BYTE      ) * 65536 ;
	sublistbuf     =          listfirsttable + sizeof( DWORD_PTR ) * 65536 ;
	listbuf        = (LZ_LIST *)( sublistbuf + sizeof( DWORD_PTR ) * 256 * sublistmaxnum ) ;
	
	// ������
	_MEMSET( usesublistflagtable, 0, sizeof( BYTE      ) * 65536               ) ;
	_MEMSET(          sublistbuf, 0, sizeof( DWORD_PTR ) * 256 * sublistmaxnum ) ;
	_MEMSET(      listfirsttable, 0, sizeof( DWORD_PTR ) * 65536               ) ;
	list = listbuf ;
	for( i = maxlistnum / 8 ; i ; i --, list += 8 )
	{
		list[0].address =
		list[1].address =
		list[2].address =
		list[3].address =
		list[4].address =
		list[5].address =
		list[6].address =
		list[7].address = 0xffffffff ;
	}

	srcp  = (BYTE *)Src ;
	destp = (BYTE *)Dest ;

	// ���k���f�[�^�̒��ň�ԏo���p�x���Ⴂ�o�C�g�R�[�h����������
	{
		DWORD qnum, table[256], mincode ;

		for( i = 0 ; i < 256 ; i ++ )
			table[i] = 0 ;
		
		sp   = srcp ;
		qnum = SrcSize / 8 ;
		i    = qnum * 8 ;
		for( ; qnum ; qnum --, sp += 8 )
		{
			table[sp[0]] ++ ;
			table[sp[1]] ++ ;
			table[sp[2]] ++ ;
			table[sp[3]] ++ ;
			table[sp[4]] ++ ;
			table[sp[5]] ++ ;
			table[sp[6]] ++ ;
			table[sp[7]] ++ ;
		}
		for( ; i < SrcSize ; i ++, sp ++ )
			table[*sp] ++ ;
			
		keycode = 0 ;
		mincode = table[0] ;
		for( i = 1 ; i < 256 ; i ++ )
		{
			if( mincode < table[i] ) continue ;
			mincode = table[i] ;
			keycode = (BYTE)i ;
		}
	}

	// ���k���̃T�C�Y���Z�b�g
	((DWORD *)destp)[0] = SrcSize ;

	// �L�[�R�[�h���Z�b�g
	destp[8] = keycode ;

	// ���k����
	dp               = destp + 9 ;
	sp               = srcp ;
	srcaddress       = 0 ;
	dstsize          = 0 ;
	listaddp         = 0 ;
	sublistnum       = 0 ;
	while( srcaddress < SrcSize )
	{
		// �c��T�C�Y���Œሳ�k�T�C�Y�ȉ��̏ꍇ�͈��k���������Ȃ�
		if( srcaddress + MIN_COMPRESS >= SrcSize ) goto NOENCODE ;

		// ���X�g���擾
		code = *((WORD *)sp) ;
		list = (LZ_LIST *)( listfirsttable + code * sizeof( DWORD_PTR ) ) ;
		if( usesublistflagtable[code] == 1 )
		{
			list = (LZ_LIST *)( (DWORD_PTR *)list->next + sp[2] ) ;
		}
		else
		{
			if( sublistnum < sublistmaxnum )
			{
				list->next = (LZ_LIST *)( sublistbuf + sizeof( DWORD_PTR ) * 256 * sublistnum ) ;
				list       = (LZ_LIST *)( (DWORD_PTR *)list->next + sp[2] ) ;
			
				usesublistflagtable[code] = 1 ;
				sublistnum ++ ;
			}
		}

		// ��Ԉ�v���̒����R�[�h��T��
		maxconbo   = -1 ;
		maxaddress = -1 ;
		maxbonus   = -1 ;
		for( m = 0, listtemp = list->next ; /*m < MAX_SEARCHLISTNUM &&*/ listtemp != NULL ; listtemp = listtemp->next, m ++ )
		{
			address = ( int )( srcaddress - listtemp->address ) ;
			if( address >= MAX_POSITION )
			{
				if( listtemp->prev ) listtemp->prev->next = listtemp->next ;
				if( listtemp->next ) listtemp->next->prev = listtemp->prev ;
				listtemp->address = 0xffffffff ;
				continue ;
			}
			
			sp2 = &sp[-address] ;
			sp1 = sp ;
			if( srcaddress + MAX_COPYSIZE < SrcSize )
			{
				conbo = MAX_COPYSIZE / 4 ;
				while( conbo && *((DWORD *)sp2) == *((DWORD *)sp1) )
				{
					sp2 += 4 ;
					sp1 += 4 ;
					conbo -- ;
				}
				conbo = MAX_COPYSIZE - ( MAX_COPYSIZE / 4 - conbo ) * 4 ;

				while( conbo && *sp2 == *sp1 )
				{
					sp2 ++ ;
					sp1 ++ ;
					conbo -- ;
				}
				conbo = MAX_COPYSIZE - conbo ;
			}
			else
			{
				for( conbo = 0 ;
						conbo < MAX_COPYSIZE &&
						conbo + srcaddress < SrcSize &&
						sp[conbo - address] == sp[conbo] ;
							conbo ++ ){}
			}

			if( conbo >= 4 )
			{
				conbosize   = ( conbo - MIN_COMPRESS ) < 0x20 ? 0 : 1 ;
				addresssize = address < 0x100 ? 0 : ( address < 0x10000 ? 1 : 2 ) ;
				bonus       = conbo - ( 3 + conbosize + addresssize ) ;

				if( bonus > maxbonus )
				{
					maxconbo       = conbo ;
					maxaddress     = address ;
					maxaddresssize = addresssize ;
					maxconbosize   = conbosize ;
					maxbonus       = bonus ;
				}
			}
		}

		// ���X�g�ɓo�^
		newlist = &listbuf[listaddp] ;
		if( newlist->address != 0xffffffff )
		{
			if( newlist->prev ) newlist->prev->next = newlist->next ;
			if( newlist->next ) newlist->next->prev = newlist->prev ;
			newlist->address = 0xffffffff ;
		}
		newlist->address = srcaddress ;
		newlist->prev    = list ;
		newlist->next    = list->next ;
		if( list->next != NULL ) list->next->prev = newlist ;
		list->next       = newlist ;
		listaddp         = ( listaddp + 1 ) & maxlistnummask ;

		// ��v�R�[�h��������Ȃ�������񈳏k�R�[�h�Ƃ��ďo��
		if( maxconbo == -1 )
		{
NOENCODE:
			// �L�[�R�[�h�������ꍇ�͂Q��A���ŏo�͂���
			if( *sp == keycode )
			{
				if( destp != NULL )
				{
					dp[0]  =
					dp[1]  = keycode ;
					dp += 2 ;
				}
				dstsize += 2 ;
			}
			else
			{
				if( destp != NULL )
				{
					*dp = *sp ;
					dp ++ ;
				}
				dstsize ++ ;
			}
			sp ++ ;
			srcaddress ++ ;
		}
		else
		{
			// ���������ꍇ�͌������ʒu�ƒ������o�͂���
			
			// �L�[�R�[�h�ƌ������ʒu�ƒ������o��
			if( destp != NULL )
			{
				// �L�[�R�[�h�̏o��
				*dp++ = keycode ;

				// �o�͂���A�����͍Œ� MIN_COMPRESS ���邱�Ƃ��O��Ȃ̂� - MIN_COMPRESS �������̂��o�͂���
				maxconbo -= MIN_COMPRESS ;

				// �A�����O�`�S�r�b�g�ƘA�����A���΃A�h���X�̃r�b�g�����o��
				*dp = (BYTE)( ( ( maxconbo & 0x1f ) << 3 ) | ( maxconbosize << 2 ) | maxaddresssize ) ;

				// �L�[�R�[�h�̘A���̓L�[�R�[�h�ƒl�̓������񈳏k�R�[�h��
				// ���f���邽�߁A�L�[�R�[�h�̒l�ȏ�̏ꍇ�͒l���{�P����
				if( *dp >= keycode ) dp[0] += 1 ;
				dp ++ ;

				// �A�����T�`�P�Q�r�b�g���o��
				if( maxconbosize == 1 )
					*dp++ = (BYTE)( ( maxconbo >> 5 ) & 0xff ) ;

				// maxconbo �͂܂��g������ - MIN_COMPRESS ��������߂�
				maxconbo += MIN_COMPRESS ;

				// �o�͂��鑊�΃A�h���X�͂O��( ���݂̃A�h���X�|�P )��}���̂ŁA�|�P�������̂��o�͂���
				maxaddress -- ;

				// ���΃A�h���X���o��
				*dp++ = (BYTE)( maxaddress ) ;
				if( maxaddresssize > 0 )
				{
					*dp++ = (BYTE)( maxaddress >> 8 ) ;
					if( maxaddresssize == 2 )
						*dp++ = (BYTE)( maxaddress >> 16 ) ;
				}
			}
			
			// �o�̓T�C�Y�����Z
			dstsize += 3 + maxaddresssize + maxconbosize ;
			
			// ���X�g�ɏ���ǉ�
			if( srcaddress + maxconbo < SrcSize )
			{
				sp2 = &sp[1] ;
				for( j = 1 ; j < maxconbo && (DWORD_PTR)&sp2[2] - (DWORD_PTR)srcp < SrcSize ; j ++, sp2 ++ )
				{
					code = *((WORD *)sp2) ;
					list = (LZ_LIST *)( listfirsttable + code * sizeof( DWORD_PTR ) ) ;
					if( usesublistflagtable[code] == 1 )
					{
						list = (LZ_LIST *)( (DWORD_PTR *)list->next + sp2[2] ) ;
					}
					else
					{
						if( sublistnum < sublistmaxnum )
						{
							list->next = (LZ_LIST *)( sublistbuf + sizeof( DWORD_PTR ) * 256 * sublistnum ) ;
							list       = (LZ_LIST *)( (DWORD_PTR *)list->next + sp2[2] ) ;
						
							usesublistflagtable[code] = 1 ;
							sublistnum ++ ;
						}
					}

					newlist = &listbuf[listaddp] ;
					if( newlist->address != 0xffffffff )
					{
						if( newlist->prev ) newlist->prev->next = newlist->next ;
						if( newlist->next ) newlist->next->prev = newlist->prev ;
						newlist->address = 0xffffffff ;
					}
					newlist->address = srcaddress + j ;
					newlist->prev = list ;
					newlist->next = list->next ;
					if( list->next != NULL ) list->next->prev = newlist ;
					list->next = newlist ;
					listaddp = ( listaddp + 1 ) & maxlistnummask ;
				}
			}
			
			sp         += maxconbo ;
			srcaddress += maxconbo ;
		}
	}

	// ���k��̃f�[�^�T�C�Y��ۑ�����
	*((DWORD *)&destp[4]) = ( DWORD )( dstsize + 9 ) ;

	// �m�ۂ����������̉��
	DXFREE( usesublistflagtable ) ;

	// �f�[�^�̃T�C�Y��Ԃ�
	return dstsize + 9 ;
}

extern int DXA_Decode( void *Src, void *Dest )
{
	DWORD srcsize, destsize, code, indexsize, keycode, conbo, index = 0 ;
	BYTE *srcp, *destp, *dp, *sp ;

	destp = (BYTE *)Dest ;
	srcp  = (BYTE *)Src ;
	
	// �𓀌�̃f�[�^�T�C�Y�𓾂�
	destsize = *((DWORD *)&srcp[0]) ;

	// ���k�f�[�^�̃T�C�Y�𓾂�
	srcsize = *((DWORD *)&srcp[4]) - 9 ;

	// �L�[�R�[�h
	keycode = srcp[8] ;
	
	// �o�͐悪�Ȃ��ꍇ�̓T�C�Y�����Ԃ�
	if( Dest == NULL )
	{
		return ( int )destsize ;
	}
	
	// �W�J�J�n
	sp  = srcp + 9 ;
	dp  = destp ;
	while( srcsize )
	{
		// �L�[�R�[�h�������ŏ����𕪊�
		if( sp[0] != keycode )
		{
			// �񈳏k�R�[�h�̏ꍇ�͂��̂܂܏o��
			*dp = *sp ;
			dp      ++ ;
			sp      ++ ;
			srcsize -- ;
			continue ;
		}
	
		// �L�[�R�[�h���A�����Ă����ꍇ�̓L�[�R�[�h���̂��o��
		if( sp[1] == keycode )
		{
			*dp = (BYTE)keycode ;
			dp      ++ ;
			sp      += 2 ;
			srcsize -= 2 ;
			
			continue ;
		}

		// ���o�C�g�𓾂�
		code = sp[1] ;

		// �����L�[�R�[�h�����傫�Ȓl�������ꍇ�̓L�[�R�[�h
		// �Ƃ̃o�b�e�B���O�h�~�ׂ̈Ɂ{�P���Ă���̂Ł|�P����
		if( code > keycode ) code -- ;

		sp      += 2 ;
		srcsize -= 2 ;

		// �A�������擾����
		conbo = code >> 3 ;
		if( code & ( 0x1 << 2 ) )
		{
			conbo |= *sp << 5 ;
			sp      ++ ;
			srcsize -- ;
		}
		conbo += MIN_COMPRESS ;	// �ۑ����Ɍ��Z�����ŏ����k�o�C�g���𑫂�

		// �Q�Ƒ��΃A�h���X���擾����
		indexsize = code & 0x3 ;
		switch( indexsize )
		{
		case 0 :
			index = *sp ;
			sp      ++ ;
			srcsize -- ;
			break ;
			
		case 1 :
			index = *((WORD *)sp) ;
			sp      += 2 ;
			srcsize -= 2 ;
			break ;
			
		case 2 :
			index = ( DWORD )( *((WORD *)sp) | ( sp[2] << 16 ) ) ;
			sp      += 3 ;
			srcsize -= 3 ;
			break ;
		}
		index ++ ;		// �ۑ����Ɂ|�P���Ă���̂Ł{�P����

		// �W�J
		if( index < conbo )
		{
			DWORD num ;

			num  = index ;
			while( conbo > num )
			{
				_MEMCPY( dp, dp - num, num ) ;
				dp    += num ;
				conbo -= num ;
				num   += num ;
			}
			if( conbo != 0 )
			{
				_MEMCPY( dp, dp - num, conbo ) ;
				dp += conbo ;
			}
		}
		else
		{
			_MEMCPY( dp, dp - index, conbo ) ;
			dp += conbo ;
		}
	}

	// �𓀌�̃T�C�Y��Ԃ�
	return (int)destsize ;
}

// �o�C�i���f�[�^�𔼊p������ɕϊ�����( �߂�l:�ϊ���̃f�[�^�T�C�Y )
extern DWORD BinToChar128( void *Src, DWORD SrcSize, void *Dest )
{
	unsigned int DestSize ;

	DestSize  = SrcSize + ( SrcSize + 6 ) / 7 + 5 ;

	if( Dest != NULL )
	{
		unsigned int PackNum ;
		unsigned int ModNum ;
		unsigned char *DestP ;
		unsigned char *SrcP ;
		unsigned int i ;

		DestP = ( unsigned char * )Dest ;
		SrcP  = ( unsigned char * )&SrcSize ;

		DestP[ 0 ] = BinToChar128Table[                                 ( SrcP[ 0 ] >> 1 ) ] ;
		DestP[ 1 ] = BinToChar128Table[ ( ( SrcP[ 0 ] & 0x01 ) << 6 ) | ( SrcP[ 1 ] >> 2 ) ] ;
		DestP[ 2 ] = BinToChar128Table[ ( ( SrcP[ 1 ] & 0x03 ) << 5 ) | ( SrcP[ 2 ] >> 3 ) ] ;
		DestP[ 3 ] = BinToChar128Table[ ( ( SrcP[ 2 ] & 0x07 ) << 4 ) | ( SrcP[ 3 ] >> 4 ) ] ;
		DestP[ 4 ] = BinToChar128Table[ ( ( SrcP[ 3 ] & 0x0f ) << 3 )                      ] ;

		DestP += 5 ;

		PackNum = SrcSize / 7 ;
		ModNum  = SrcSize - PackNum * 7 ;
		SrcP  = ( unsigned char * )Src ;
		for( i = 0 ; i < PackNum ; i ++ )
		{
			DestP[ 0 ] = BinToChar128Table[                                 ( SrcP[ 0 ] >> 1 ) ] ;
			DestP[ 1 ] = BinToChar128Table[ ( ( SrcP[ 0 ] & 0x01 ) << 6 ) | ( SrcP[ 1 ] >> 2 ) ] ;
			DestP[ 2 ] = BinToChar128Table[ ( ( SrcP[ 1 ] & 0x03 ) << 5 ) | ( SrcP[ 2 ] >> 3 ) ] ;
			DestP[ 3 ] = BinToChar128Table[ ( ( SrcP[ 2 ] & 0x07 ) << 4 ) | ( SrcP[ 3 ] >> 4 ) ] ;
			DestP[ 4 ] = BinToChar128Table[ ( ( SrcP[ 3 ] & 0x0f ) << 3 ) | ( SrcP[ 4 ] >> 5 ) ] ;
			DestP[ 5 ] = BinToChar128Table[ ( ( SrcP[ 4 ] & 0x1f ) << 2 ) | ( SrcP[ 5 ] >> 6 ) ] ;
			DestP[ 6 ] = BinToChar128Table[ ( ( SrcP[ 5 ] & 0x3f ) << 1 ) | ( SrcP[ 6 ] >> 7 ) ] ;
			DestP[ 7 ] = BinToChar128Table[ ( ( SrcP[ 6 ] & 0x7f )      )                      ] ;

			DestP += 8 ;
			SrcP  += 7 ;
		}

		if( ModNum != 0 )
		{
			DestP[ 0 ] = BinToChar128Table[                                 ( SrcP[ 0 ] >> 1 ) ] ;
			if( ModNum == 1 )
			{
				DestP[ 1 ] = BinToChar128Table[ ( ( SrcP[ 0 ] & 0x01 ) << 6 ) ] ;
			}
			else
			{
				DestP[ 1 ] = BinToChar128Table[ ( ( SrcP[ 0 ] & 0x01 ) << 6 ) | ( SrcP[ 1 ] >> 2 ) ] ;
				if( ModNum == 2 )
				{
					DestP[ 2 ] = BinToChar128Table[ ( ( SrcP[ 1 ] & 0x03 ) << 5 ) ] ;
				}
				else
				{
					DestP[ 2 ] = BinToChar128Table[ ( ( SrcP[ 1 ] & 0x03 ) << 5 ) | ( SrcP[ 2 ] >> 3 ) ] ;
					if( ModNum == 3 )
					{
						DestP[ 3 ] = BinToChar128Table[ ( ( SrcP[ 2 ] & 0x07 ) << 4 ) ] ;
					}
					else
					{
						DestP[ 3 ] = BinToChar128Table[ ( ( SrcP[ 2 ] & 0x07 ) << 4 ) | ( SrcP[ 3 ] >> 4 ) ] ;
						if( ModNum == 4 )
						{
							DestP[ 4 ] = BinToChar128Table[ ( ( SrcP[ 3 ] & 0x0f ) << 3 ) ] ;
						}
						else
						{
							DestP[ 4 ] = BinToChar128Table[ ( ( SrcP[ 3 ] & 0x0f ) << 3 ) | ( SrcP[ 4 ] >> 5 ) ] ;
							if( ModNum == 5 )
							{
								DestP[ 5 ] = BinToChar128Table[ ( ( SrcP[ 4 ] & 0x1f ) << 2 ) ] ;
							}
							else
							{
								DestP[ 5 ] = BinToChar128Table[ ( ( SrcP[ 4 ] & 0x1f ) << 2 ) | ( SrcP[ 5 ] >> 6 ) ] ;
								if( ModNum == 6 )
								{
									DestP[ 6 ] = BinToChar128Table[ ( ( SrcP[ 5 ] & 0x3f ) << 1 ) ] ;
								}
								else
								{
									DestP[ 6 ] = BinToChar128Table[ ( ( SrcP[ 5 ] & 0x3f ) << 1 ) | ( SrcP[ 6 ] >> 7 ) ] ;
									DestP[ 7 ] = BinToChar128Table[ ( ( SrcP[ 6 ] & 0x7f )      )                      ] ;
								}
							}
						}
					}
				}
			}
		}
	}

	return DestSize ;
}

// ���p��������o�C�i���f�[�^�ɕϊ�����( �߂�l:�ϊ���̃f�[�^�T�C�Y )
extern DWORD Char128ToBin( void *Src, void *Dest )
{
	unsigned int DestSize ;
	unsigned char *SrcP ;
	unsigned char *DestP ;

	SrcP    = ( unsigned char * )Src ;
	DestP   = ( unsigned char * )&DestSize ;

	DestP[ 0 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 0 ] ] << 1 ) | ( Char128ToBinTable[ SrcP[ 1 ] ] >> 6 ) ) ;
	DestP[ 1 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 1 ] ] << 2 ) | ( Char128ToBinTable[ SrcP[ 2 ] ] >> 5 ) ) ;
	DestP[ 2 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 2 ] ] << 3 ) | ( Char128ToBinTable[ SrcP[ 3 ] ] >> 4 ) ) ;
	DestP[ 3 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 3 ] ] << 4 ) | ( Char128ToBinTable[ SrcP[ 4 ] ] >> 3 ) ) ;

	SrcP += 5 ;

	if( Dest != NULL )
	{
		unsigned int PackNum ;
		unsigned int ModNum ;
		unsigned int i ;

		PackNum = DestSize / 7 ;
		ModNum  = DestSize - PackNum * 7 ;
		DestP = ( unsigned char * )Dest ;
		for( i = 0 ; i < PackNum ; i ++ )
		{
			DestP[ 0 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 0 ] ] << 1 ) | ( Char128ToBinTable[ SrcP[ 1 ] ] >> 6 ) ) ;
			DestP[ 1 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 1 ] ] << 2 ) | ( Char128ToBinTable[ SrcP[ 2 ] ] >> 5 ) ) ;
			DestP[ 2 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 2 ] ] << 3 ) | ( Char128ToBinTable[ SrcP[ 3 ] ] >> 4 ) ) ;
			DestP[ 3 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 3 ] ] << 4 ) | ( Char128ToBinTable[ SrcP[ 4 ] ] >> 3 ) ) ;
			DestP[ 4 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 4 ] ] << 5 ) | ( Char128ToBinTable[ SrcP[ 5 ] ] >> 2 ) ) ;
			DestP[ 5 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 5 ] ] << 6 ) | ( Char128ToBinTable[ SrcP[ 6 ] ] >> 1 ) ) ;
			DestP[ 6 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 6 ] ] << 7 ) | ( Char128ToBinTable[ SrcP[ 7 ] ]      ) ) ;

			DestP += 7 ;
			SrcP  += 8 ;
		}

		if( ModNum != 0 )
		{
			DestP[ 0 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 0 ] ] << 1 ) | ( Char128ToBinTable[ SrcP[ 1 ] ] >> 6 ) ) ;
			if( ModNum > 1 )
			{
				DestP[ 1 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 1 ] ] << 2 ) | ( Char128ToBinTable[ SrcP[ 2 ] ] >> 5 ) ) ;
				if( ModNum > 2 )
				{
					DestP[ 2 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 2 ] ] << 3 ) | ( Char128ToBinTable[ SrcP[ 3 ] ] >> 4 ) ) ;
					if( ModNum > 3 )
					{
						DestP[ 3 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 3 ] ] << 4 ) | ( Char128ToBinTable[ SrcP[ 4 ] ] >> 3 ) ) ;
						if( ModNum > 4 )
						{
							DestP[ 4 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 4 ] ] << 5 ) | ( Char128ToBinTable[ SrcP[ 5 ] ] >> 2 ) ) ;
							if( ModNum > 5 )
							{
								DestP[ 5 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 5 ] ] << 6 ) | ( Char128ToBinTable[ SrcP[ 6 ] ] >> 1 ) ) ;
								if( ModNum > 6 )
								{
									DestP[ 6 ] = ( unsigned char )( ( Char128ToBinTable[ SrcP[ 6 ] ] << 7 ) | ( Char128ToBinTable[ SrcP[ 7 ] ]      ) ) ;
								}
							}
						}
					}
				}
			}
		}
	}

	return DestSize ;
}

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE
