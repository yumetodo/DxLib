// ============================================================================
//
//		�t�@�C���֌W�̃��C�u����
//
//		Creator			: �R�c�@�I
//		Creation Data	: 09/17/2004
//
// ============================================================================

#ifndef __FILELIB
#define __FILELIB

// include --------------------------------------
#include <stdio.h>
#include "DataType.h"

// define ---------------------------------------

// data type ------------------------------------

// �t�@�C���̓��t�f�[�^
typedef struct tagFILE_DATE
{
	u64 Create ;			// �쐬����
	u64 LastAccess ;		// �ŏI�A�N�Z�X����
	u64 LastWrite ;			// �ŏI�X�V����
} FILE_DATE ;

// �t�@�C�����
typedef struct tagFILE_INFO
{
	char *FileName ;			// �t�@�C����
	char *RelDirectoryPath ;	// �񋓏������̃J�����g�f�B���N�g������̑��΃f�B���N�g���p�X( ����� '\' �������Ă��� )
	char *AbsDirectoryPath ;	// ��΃f�B���N�g���p�X( ����� '\' �������Ă��� )
	u32 Size ;					// �t�@�C���̃T�C�Y(�f�B���N�g���̏ꍇ�͉��������Ă��Ȃ�)
	u32 IsDirectory ;			// �f�B���N�g�����ۂ�
	FILE_DATE Date ;			// ���t�f�[�^
	u32 Attributes ;			// �t�@�C������
} FILE_INFO ;

// �t�@�C����񃊃X�g
typedef struct tagFILE_INFOLIST
{
	int Num ;					// �f�B���N�g�����̃t�@�C���̐�
	FILE_INFO  *List ;			// �f�B���N�g�����̃t�@�C���̏����i�[�����z��ւ̃|�C���^
} FILE_INFOLIST ;

// data -----------------------------------------

// function proto type --------------------------

extern void EasyEncode( void *Data , unsigned int Size ) ;					// �f�[�^���ȈՈÍ�������֐�
extern void EasyEncodeFileWrite( void *Data, int Size, FILE *fp ) ;			// �f�[�^���ȈՈÍ������ăt�@�C���ɏ����o���֐�
extern void EasyEncodeFileRead( void *Data, int Size, FILE *fp ) ;			// �f�[�^���ȈՈÍ������ăt�@�C������ǂݍ��ފ֐�

extern int LoadFileMem( const char *Path, void **DataBuf, int *Size ) ;		// �t�@�C���̓��e���������ɓǂݍ���( 0:����  -1:���s )
extern int LoadFileMem( const char *Path, void *DataBuf, int *Size ) ;		// �t�@�C���̓��e���������ɓǂݍ���( 0:����  -1:���s )
extern int SaveFileMem( const char *Path, void *Data,   int Size ) ;		// �������̓��e���t�@�C���ɏ����o�� 

// �w��̃f�B���N�g�����쐬����A���Ԃ̃f�B���N�g�������݂��Ȃ��ꍇ�͍쐬����
// �Ō���� '\' �������Ă���������
// �h���C�u���̌�� '\' ���Ȃ��ꍇ�͐���ɓ��삵�Ȃ�
extern int __CreateDirectory( const char *Path ) ;

// �w��̃p�X�������Ă�����̂��f�B���N�g�����ǂ����𓾂�( 1:�f�B���N�g��  0:�t�@�C�� )
extern int IsDirectory( const char *Path ) ;

extern int CreateFileInfo( const char *Path, FILE_INFO *FileInfoBuffer ) ;	// �w��̃p�X�̏��𓾂�(-1:�G���[���� 0:����I��)
extern int ReleaseFileInfo( FILE_INFO *FileInfo ) ;							// �p�X���̌�n�����s��
extern int SetFileTimeStamp( const char *Path, FILE_INFO *FileInfo ) ;		// �w��̃p�X�̃t�@�C���̃^�C���X�^���v�� FileInfo �Ɋi�[����Ă���^�C���X�^���v�ɂ���
extern int CmpFileTimeStamp( FILE_INFO *FileInfo1, FILE_INFO *FileInfo2, bool Create = true, bool LastAccess = true, bool LastWrite = true );	// ��̃t�@�C���̃^�C���X�^���v����v���Ă��邩�ǂ����𓾂�(0:��v���Ă��� -1:��v���Ă��Ȃ�)

// �w��̃f�B���N�g���̃t�@�C�����X�g���擾����
// �p�X������̍Ō�Ɂw\�x�������Ă���ɍŌ�̕�������f�B���N�g���Ɣ��f����
// �f�B���N�g���ł͂Ȃ������ꍇ���������Ԃ�
// �t���p�X�ł͂Ȃ��ꍇ�͌��݂̃J�����g�f�B���N�g������̑��΃p�X�ƂȂ�
// FileListInfo �� 0 �ł͂Ȃ��ꍇ�g�p�� ReleaseFileObjectList ���ĂԕK�v������
// SubDirectory �� 1 �ɂ���ƃT�u�f�B���N�g�����̃t�@�C�����S�ė񋓂���
// NULL �A�������͕���������Ȃ��������n�����ꍇ�J�����g�f�B���N�g���̃t�@�C�����񋓂����
// �߂�l : -1 = �G���[  0�ȏ� = �񋓂����t�@�C���̐�
// OmitDirectory �� 1 �ɂ���ƃf�B���N�g���͗񋓂��Ȃ�
// OmitName �ɏ��O�������t�@�C������n���Ƃ��̕���������O���Ă����( ';'�ŕ����̕������A���\ )
// OmitExName �ɏ��O�������g���q��n���Ƃ��̊g���q�����t�@�C�������O���Ă����( ';'�ŕ����̕������A���\ )
// ValidExName �ɗL���ɂ������g���q��n���Ƃ��̊g���q�����t�@�C���̂ݗ񋓂��Ă����( Omit�n�����D��x������, ';'�ŕ����̕������A���\ )
extern int CreateFileList( const char *DirectoryPath, FILE_INFOLIST *FileListInfo = 0,
							int OmitDirectory = 0, int SubDirectory = 0,
							const char *OmitName = 0, const char *OmitExName = 0,
							const char *ValidExName = 0 ) ;
extern int ReleaseFileList( FILE_INFOLIST *DirectoryInfo ) ;

// ����̃p�X�����񂩂�F�X�ȏ����擾����
// ( CurrentDir �̓t���p�X�ł���K�v������(����Ɂw\�x�������Ă������Ă��ǂ�) )
// ( CurrentDir �� 0 �̏ꍇ�͎��ۂ̃J�����g�f�B���N�g�����g�p���� )
extern int AnalyseFilePath(
	const char *Src, // DirPath �̏I�[�ɂ� \ �͕t���Ȃ�
	char *FullPath, char *DirPath, char *FileName, char *Name, char *ExeName, const char *CurrentDir = 0 );

// �t���p�X�ł͂Ȃ��p�X��������t���p�X�ɕϊ�����
// ( CurrentDir �̓t���p�X�ł���K�v������(����Ɂw\�x�������Ă������Ă��ǂ�) )
// ( CurrentDir �� 0 �̏ꍇ�͎��ۂ̃J�����g�f�B���N�g�����g�p���� )
extern int ConvertFullPath__( const char *Src, char *Dest, const char *CurrentDir = 0 ) ;

// �t�@�C�������ꏏ�ɂȂ��Ă���ƕ������Ă���p�X������t�@�C�����ƃf�B���N�g���p�X�𕪊�����
// �t���p�X�ł���K�v�͖����A�t�@�C���������ł��ǂ�
// DirPath �̏I�[�� �� �}�[�N�͕t���Ȃ�
extern int AnalysisFileNameAndDirPath( const char *Src, char *FileName = 0, char *DirPath = 0 ) ;

// �t�@�C���p�X����t�@�C�����Ɗg���q���擾����
extern int AnalysisFileNameAndExeName( const char *Src, char *Name = 0, char *ExeName = 0 ) ;

// �t�@�C���p�X�̊g���q��ς���������𓾂�
extern int GetChangeExeNamePath( const char *Src, char *Dest, const char *ExeName ) ;

extern void SetEnMark( char *PathBuf ) ;			// ����Ɂw\�x�����Ă��Ȃ��ꍇ�͕t����
extern void SetChr( char *PathBuf, char chr ) ;		// ����Ɏw��̕������Ȃ��ꍇ�͂���
extern void DelChr( char *PathBuf, char chr ) ;		// ����Ɏw��̕���������ꍇ�͍폜����

extern int GetExName( const char *Path, char *ExNameBuf ) ;				// �g���q�𓾂�
extern int SetExName( const char *Path, char *ExName, char *DestBuf ) ;	// �g���q��ύX����

extern int CheckTextData( void *buffer, int size ) ;		// �e�L�X�g�f�[�^���ǂ����𔻒肷��( 1:�e�L�X�g�f�[�^  0:�o�C�i���f�[�^ )
extern int CheckTextFile( const char *Path ) ;				// �e�L�X�g�t�@�C�����ǂ����𔻒肷��( 1:�e�L�X�g�f�[�^  0:�o�C�i���f�[�^ )

// �Q�o�C�g���������ׂ�( TRUE:�Q�o�C�g���� FALSE:�P�o�C�g���� ) 
extern int CheckMultiByteChar( char *Buf ) ;

#endif