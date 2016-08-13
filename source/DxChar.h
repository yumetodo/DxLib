// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�����R�[�h�֌W�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXCHAR_H__
#define __DXCHAR_H__

// �C���N���[�h ------------------------------------------------------------------
#include "DxCompileConfig.h"
#include "DxLib.h"
#include "DxStatic.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// BufferBytes �L�����Z���p�}�N��
#define BUFFERBYTES_CANCEL					(0x7fffffff)

// �V�t�gJIS�Q�o�C�g��������
#define CHECK_SHIFTJIS_2BYTE( x )			( ( BYTE )( ( ( ( BYTE )(x) ) ^ 0x20) - ( BYTE )0xa1 ) < 0x3c )

// UTF16LE�T���Q�[�g�y�A����( ���g���G���f�B�A�����p )
#define CHECK_CPU_LE_UTF16LE_4BYTE( x )		( ( ( x ) & 0xfc00 ) == 0xd800 )

// UTF16LE�T���Q�[�g�y�A����( �r�b�O�G���f�B�A�����p )
#define CHECK_CPU_BE_UTF16LE_4BYTE( x )		( ( ( ( ( ( ( WORD )( x ) ) >> 8 ) & 0xff ) | ( ( ( WORD )( x ) << 8 ) & 0xff00 ) ) & 0xfc00 ) == 0xd800 )

// UTF16BE�T���Q�[�g�y�A����( ���g���G���f�B�A�����p )
#define CHECK_CPU_LE_UTF16BE_4BYTE( x )		CHECK_CPU_BE_UTF16LE_4BYTE( x )

// UTF16BE�T���Q�[�g�y�A����( �r�b�O�G���f�B�A�����p )
#define CHECK_CPU_BE_UTF16BE_4BYTE( x )		CHECK_CPU_LE_UTF16LE_4BYTE( x )

// wchar_t �T���Q�[�g�y�A����( UTF-32 or UTF-16 �z�� )
#ifdef WCHAR_T_BE
	#define CHECK_WCHAR_T_DOUBLE( x )	( sizeof( wchar_t ) == 2 && ( ( ( WORD )( x ) & 0x00fc ) == 0x00d8 ) )
#else
	#define CHECK_WCHAR_T_DOUBLE( x )	( sizeof( wchar_t ) == 2 && ( ( ( WORD )( x ) & 0xfc00 ) == 0xd800 ) )
#endif

// �\���̒�` --------------------------------------------------------------------

// UTF-16�Ɗe�����R�[�h�̑Ή��\�̏��
struct CHARCODETABLEINFO
{
	WORD				MultiByteToUTF16[ 0x10000 ] ;		// �e�����R�[�h����UTF-16�ɕϊ����邽�߂̃e�[�u��
	WORD				UTF16ToMultiByte[ 0x10000 ] ;		// UTF-16����e�����R�[�h�ɕϊ����邽�߂̃e�[�u��
} ;

// �����R�[�h�����֌W�Ŏg�p������
struct CHARCODESYSTEM
{
	int					InitializeFlag ;					// �������������s�������ǂ���( TRUE:�s����  FALSE:�s���Ă��Ȃ� )

	CHARCODETABLEINFO	CharCodeCP932Info ;					// Shift-JIS�̕����R�[�h���
	CHARCODETABLEINFO	CharCodeCP936Info ;					// GB2312�̕����R�[�h���
	CHARCODETABLEINFO	CharCodeCP949Info ;					// UHC�̕����R�[�h���
	CHARCODETABLEINFO	CharCodeCP950Info ;					// BIG5�̕����R�[�h���
	CHARCODETABLEINFO	CharCodeCP1252Info ;				// ����(���e�������̕����R�[�h)�̕����R�[�h���
	CHARCODETABLEINFO	CharCodeISO_IEC_8859_15Info ;		// ����(���e�������̕����R�[�h)�̕����R�[�h���
} ;

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

extern CHARCODESYSTEM g_CharCodeSystem ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

extern	int				InitCharCode( void ) ;																					// �����R�[�h�֘A�����̏��������s��

extern	int				GetCharCodeFormatUnitSize(	int CharCodeFormat ) ;														// �w��̕����R�[�h�`���̏��ŏ��T�C�Y���擾����( �߂�l�F�o�C�g�� )
extern	int				GetCharBytes(			const char *CharCode, int CharCodeFormat ) ;									// �P�����̃o�C�g�����擾����( �߂�l�F�P�����̃o�C�g�� )
extern	DWORD			GetCharCode(			const char *CharCode, int CharCodeFormat, int *CharBytes ) ;					// �P�����̕����R�[�h�ƕ����̃o�C�g�����擾����
extern	int				PutCharCode(			DWORD CharCode, int CharCodeFormat, char *Dest, size_t BufferBytes ) ;			// �����R�[�h��ʏ�̕�����ɕϊ�����A�I�[�Ƀk�������͏������܂Ȃ�( �߂�l�F�������񂾃o�C�g�� )
extern	DWORD			ConvCharCode(			DWORD SrcCharCode, int SrcCharCodeFormat, int DestCharCodeFormat ) ;			// �����R�[�h���w��̕����R�[�h�`���̕����ɕϊ�����
extern	int				ConvCharCodeString(		const DWORD *Src, int SrcCharCodeFormat, DWORD *Dest, size_t BufferBytes, int DestCharCodeFormat ) ;	// �P�����S�o�C�g�̔z����A�ʕ����R�[�h�`���̂P�����S�o�C�g�̔z��ɕϊ�����( �߂�l�F�ϊ���̃T�C�Y�A�k�������܂�( �P�ʁF�o�C�g ) )
extern	int				StringToCharCodeString( const char  *Src, int SrcCharCodeFormat, DWORD *Dest, size_t BufferBytes  ) ;	// ��������P�����S�o�C�g�̔z��ɕϊ�����( �߂�l�F�ϊ���̃T�C�Y�A�k�������܂�( �P�ʁF�o�C�g ) )
extern	int				CharCodeStringToString( const DWORD *Src, char *Dest, size_t BufferBytes, int DestCharCodeFormat ) ;	// �P�����S�o�C�g�̔z��𕶎���ɕϊ�����( �߂�l�F�ϊ���̃T�C�Y�A�k�������܂�( �P�ʁF�o�C�g ) )
extern	int				ConvString(				const char *Src, int SrcCharCodeFormat, char *Dest, size_t BufferBytes, int DestCharCodeFormat ) ;		// ��������w��̕����R�[�h�`���̕�����ɕϊ�����( �߂�l�F�ϊ���̃T�C�Y�A�k�������܂�( �P�ʁF�o�C�g ) )
extern	int				GetStringCharNum(		const char *String, int CharCodeFormat ) ;										// ������Ɋ܂܂�镶�������擾����
extern	const char *	GetStringCharAddress(	const char *String, int CharCodeFormat, int Index ) ;							// �w��ԍ��̕����̃A�h���X���擾����
extern	DWORD			GetStringCharCode(		const char *String, int CharCodeFormat, int Index ) ;							// �w��ԍ��̕����̃R�[�h���擾����

extern	void			CL_strcpy(            int CharCodeFormat, char *Dest,                     const char *Src ) ;
extern	void			CL_strcpy_s(          int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src ) ;
extern	void			CL_strpcpy(           int CharCodeFormat, char *Dest,                     const char *Src, int Pos ) ;
extern	void			CL_strpcpy_s(         int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src, int Pos ) ;
extern	void			CL_strpcpy2(          int CharCodeFormat, char *Dest,                     const char *Src, int Pos ) ;
extern	void			CL_strpcpy2_s(        int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src, int Pos ) ;
extern	void			CL_strncpy(           int CharCodeFormat, char *Dest,                     const char *Src, int Num ) ;
extern	void			CL_strncpy_s(         int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src, int Num ) ;
extern	void			CL_strncpy2(          int CharCodeFormat, char *Dest,                     const char *Src, int Num ) ;
extern	void			CL_strncpy2_s(        int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src, int Num ) ;
extern	void			CL_strrncpy(          int CharCodeFormat, char *Dest,                     const char *Src, int Num ) ;
extern	void			CL_strrncpy_s(        int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src, int Num ) ;
extern	void			CL_strrncpy2(         int CharCodeFormat, char *Dest,                     const char *Src, int Num ) ;
extern	void			CL_strrncpy2_s(       int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src, int Num ) ;
extern	void			CL_strpncpy(          int CharCodeFormat, char *Dest,                     const char *Src, int Pos, int Num ) ;
extern	void			CL_strpncpy_s(        int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src, int Pos, int Num ) ;
extern	void			CL_strpncpy2(         int CharCodeFormat, char *Dest,                     const char *Src, int Pos, int Num ) ;
extern	void			CL_strpncpy2_s(       int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src, int Pos, int Num ) ;
extern	void			CL_strcat(            int CharCodeFormat, char *Dest,                     const char *Src ) ;
extern	void			CL_strcat_s(          int CharCodeFormat, char *Dest, size_t BufferBytes, const char *Src ) ;
extern	size_t			CL_strlen(            int CharCodeFormat, const char *Str ) ;
extern	size_t			CL_strlen2(           int CharCodeFormat, const char *Str ) ;
extern	int				CL_strcmp(            int CharCodeFormat, const char *Str1, const char *Str2 ) ;
extern	int				CL_stricmp(           int CharCodeFormat, const char *Str1, const char *Str2 ) ;
extern	int				CL_strcmp_str2_ascii( int CharCodeFormat, const char *Str1, const char *Str2 ) ;
extern	int				CL_strncmp(           int CharCodeFormat, const char *Str1, const char *Str2, int Num ) ;
extern	int				CL_strncmp2(          int CharCodeFormat, const char *Str1, const char *Str2, int Num ) ;
extern	int				CL_strpncmp(          int CharCodeFormat, const char *Str1, const char *Str2, int Pos, int Num ) ;
extern	int				CL_strpncmp2(         int CharCodeFormat, const char *Str1, const char *Str2, int Pos, int Num ) ;
extern	DWORD			CL_strgetchr(         int CharCodeFormat, const char *Str, int Pos, int *CharNums ) ;
extern	DWORD			CL_strgetchr2(        int CharCodeFormat, const char *Str, int Pos, int *CharNums ) ;
extern	int				CL_strputchr(         int CharCodeFormat, char *Str, int Pos, DWORD CharCode ) ;
extern	int				CL_strputchr2(        int CharCodeFormat, char *Str, int Pos, DWORD CharCode ) ;
extern	const char *	CL_strpos(            int CharCodeFormat, const char *Str, int Pos ) ;
extern	const char *	CL_strpos2(           int CharCodeFormat, const char *Str, int Pos ) ;
extern	const char *	CL_strstr(            int CharCodeFormat, const char *Str1, const char *Str2 ) ;
extern	int				CL_strstr2(           int CharCodeFormat, const char *Str1, const char *Str2 ) ;
extern	const char *	CL_strrstr(           int CharCodeFormat, const char *Str1, const char *Str2 ) ;
extern	int				CL_strrstr2(          int CharCodeFormat, const char *Str1, const char *Str2 ) ;
extern	const char *	CL_strchr(            int CharCodeFormat, const char *Str, DWORD CharCode ) ;
extern	int				CL_strchr2(           int CharCodeFormat, const char *Str, DWORD CharCode ) ;
extern	const char *	CL_strrchr(           int CharCodeFormat, const char *Str, DWORD CharCode ) ;
extern	int				CL_strrchr2(          int CharCodeFormat, const char *Str, DWORD CharCode ) ;
extern	char *			CL_strupr(            int CharCodeFormat, char *Str ) ;
extern	int				CL_vsprintf(          int CharCodeFormat, int IsWChar, int CharCharCodeFormat, int WCharCharCodeFormat, char *Buffer,                    const char *FormatString, va_list Arg ) ;
extern	int				CL_vsnprintf(         int CharCodeFormat, int IsWChar, int CharCharCodeFormat, int WCharCharCodeFormat, char *Buffer, size_t BufferSize, const char *FormatString, va_list Arg ) ;
extern	int				CL_sprintf(           int CharCodeFormat, int IsWChar, int CharCharCodeFormat, int WCharCharCodeFormat, char *Buffer,                    const char *FormatString, ... ) ;
extern	int				CL_snprintf(          int CharCodeFormat, int IsWChar, int CharCharCodeFormat, int WCharCharCodeFormat, char *Buffer, size_t BufferSize, const char *FormatString, ... ) ;
extern	char *			CL_itoa(              int CharCodeFormat, int Value, char *Buffer,                     int Radix ) ;
extern	char *			CL_itoa_s(            int CharCodeFormat, int Value, char *Buffer, size_t BufferBytes, int Radix ) ;
extern	int				CL_atoi(              int CharCodeFormat, const char *Str ) ;
extern	double			CL_atof(              int CharCodeFormat, const char *Str ) ;
extern	int				CL_vsscanf(           int CharCodeFormat, int IsWChar, int CharCharCodeFormat, int WCharCharCodeFormat, const char *String, const char *FormatString, va_list Arg ) ;
extern	int				CL_sscanf(            int CharCodeFormat, int IsWChar, int CharCharCodeFormat, int WCharCharCodeFormat, const char *String, const char *FormatString, ... ) ;

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXCHAR_H__
