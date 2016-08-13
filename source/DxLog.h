// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		���O�v���O�����w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXLOG_H__
#define __DXLOG_H__

// �C���N���[�h ------------------------------------------------------------------

#include "DxCompileConfig.h"
#include "DxFile.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

//�c�w���C�u�������̃��e������������ɗ͌��炷���ǂ����̒�`�𔽉f����ׂ̃}�N��
#if !defined( DX_NON_LITERAL_STRING ) && !defined( DX_NON_LOG )
	#define DXSTRING( x )					x
	#define DXST_ERRORLOG_ADD(x)			NS_ErrorLogAdd(x)
	#define DXST_ERRORLOG_ADDA(x)			ErrorLogAddA(x)
	#define DXST_ERRORLOG_ADDW(x)			ErrorLogAddW(x)
	#define DXST_ERRORLOG_ADDUTF16LE(x)		ErrorLogAddUTF16LE(x)
	#define DXST_ERRORLOGFMT_ADD(x)			NS_ErrorLogFmtAdd x
	#define DXST_ERRORLOGFMT_ADDA(x)		ErrorLogFmtAddA x
	#define DXST_ERRORLOGFMT_ADDW(x)		ErrorLogFmtAddW x
	#define DXST_ERRORLOGFMT_ADDUTF16LE(x)	ErrorLogFmtAddUTF16LE x
	#define DXST_ERRORLOG_TABADD			NS_ErrorLogTabAdd()
	#define DXST_ERRORLOG_TABSUB			NS_ErrorLogTabSub()
	#define DXERRORNETLOG_ADD(x)			ErrorNetLogAdd(x)
	#define DXERRORNETWORK(x)				ErrorNetWork x
#else
	#define DXSTRING( x )					" "
	#define DXST_ERRORLOG_ADD(x)			(-1)
	#define DXST_ERRORLOG_ADDA(x)			(-1)
	#define DXST_ERRORLOG_ADDW(x)			(-1)
	#define DXST_ERRORLOG_ADDUTF16LE(x)		(-1)
	#define DXST_ERRORLOGFMT_ADD(x)			(-1)
	#define DXST_ERRORLOGFMT_ADDA(x)		(-1)
	#define DXST_ERRORLOGFMT_ADDW(x)		(-1)
	#define DXST_ERRORLOGFMT_ADDUTF16LE(x)	(-1)
	#define DXST_ERRORLOG_TABADD			(0)
	#define DXST_ERRORLOG_TABSUB			(0)
	#define DXERRORNETLOG_ADD(x)			(-1)
	#define DXERRORNETWORK(x)				(-1)
#endif


#ifndef DX_NON_LOG

#define LOG_MAXHEIGHT				(80)				// �P��ʂɓ��郍�O�̍ő�s
#define LOG_MAXLENGTH				(255)				// �P�s�̃��O�Ɏg���镶����̒���

// ���O�����p��`
#define LOG_FONTSIZE				(16)				// ���O�Ɏg���t�H���g�̃T�C�Y
#define LOG_FONTTICK				(4)					// ���O�\���Ɏg���t�H���g�̑���
#define LOG_TABWIDTH				(4)					// �^�u����̕�

// �\���̒�` --------------------------------------------------------------------

// ���O�����f�[�^�\����
struct LOGDATA
{
	int						InitializeFlag ;					// �����������t���O

	int						NonUseTimeStampFlag ;				// �^�C���X�^���v���o�͂��Ȃ����ǂ����̃t���O
	int						NotLogOutFlag ;						// ���O�o�͂��s�����t���O(TRUE�ōs��Ȃ�)
	int						LogStartTime ;						// �N�����̃^�C���X�^���v
	wchar_t					LogOutDirectory[ FILEPATH_MAX ] ;	// ���O�o�͂��s���t�@�C����ۑ�����f�B���N�g��
	int						UseLogDateName ;					// ���O�o�͂��s���t�@�C���ɓ��t�����邩�ǂ����̃t���O
	wchar_t					LogFileName[256] ;					// ���O�o�͂��s���t�@�C���̖��O
	int						ErTabNum ;							// �G���[���O�r�o���̃^�u�̐�
	int						ErTabStop ;							// �^�u�r�o�X�g�b�v�t���O

#ifndef DX_NON_PRINTF_DX
	int						LogInitializeFlag ;					// ���O�f�[�^������������Ă��邩�t���O
	int						LogUserFontHandle ;					// ���[�U�[�w��̃��O�`��p�t�H���g�n���h��
	int						LogFontHandle ;						// ���O�`��p�t�H���g�n���h��
	int						LogFontHandleLostFlag ;				// �t�H���g�n���h�����폜���ꂽ�ۂɗ��t���O
	int						LogX ,LogY ;						// ���݂̃��O�̃J�[�\���ʒu
	wchar_t					LogString[LOG_MAXHEIGHT][LOG_MAXLENGTH] ; // ���O������
	int						LogDrawFlag ;						// ���O����ʏ�ɕ\�����邩�A�t���O
	int						LogFontSize ;						// ���O�t�H���g�̃T�C�Y
	int						LogTabWidth ;						// ���O�̃^�u�̕�
	int						LogDrawWidth ;						// ���݃J�[�\��������s�̕`�敝
	int						LogFileOutFlag ;					// ���O���t�@�C���o�͂��邩�A�t���O
	RECT					LogDrawArea ;						// ���O��`�悷��̈�
#endif // DX_NON_PRINTF_DX

} ;

// �������ϐ��錾 --------------------------------------------------------------

extern LOGDATA LogData ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// ���O�o�͋@�\�֐�
extern	int			ErrorLogInitialize(		void ) ;										// �G���[�����o���t�@�C��������������
extern	int			ErrorLogTerminate(		void ) ;										// �G���[�����o���t�@�C���̌�n��
extern	int			ErrorLogAddA(			const char    *ErrorStr ) ;						// �G���[�����������o��( char�� )
extern	int			ErrorLogAddW(			const wchar_t *ErrorStr ) ;						// �G���[�����������o��( wchar_t�� )
extern	int			ErrorLogAddUTF16LE(		const char    *ErrorStr ) ;						// �G���[�����������o��( UTF16LE�� )
extern 	int			ErrorLogFmtAddA(		const char	  *FormatString , ... ) ;			// �����t���G���[�����������o��( char�� )
extern 	int			ErrorLogFmtAddW(		const wchar_t *FormatString , ... ) ;			// �����t���G���[�����������o��( wchar_t�� )
extern 	int			ErrorLogFmtAddUTF16LE(	const char    *FormatString , ... ) ;			// �����t���G���[�����������o��( UTF16LE�� )

// wchar_t�Ŋ֐�
extern	int			ErrorLogAdd_WCHAR_T(	const wchar_t *ErrorStr ) ;						// ���O�t�@�C��( Log.txt ) �ɕ�������o�͂���
extern 	int			ErrorLogFmtAdd_WCHAR_T(	const wchar_t *FormatString , ... ) ;			// �����t���� ���O�t�@�C��( Log.txt ) �ɕ�������o�͂���( ������ printf �Ɠ��� )
extern 	int			AppLogAdd_WCHAR_T(		const wchar_t *String , ... ) ;					// ErrorLogFmtAdd �Ɠ����@�\�̊֐�

extern	int			SetApplicationLogSaveDirectory_WCHAR_T(	const wchar_t *DirectoryPath );	// ���O�t�@�C��( Log.txt ) ��ۑ�����f�B���N�g���p�X��ݒ肷��


// ���ˑ��֐�
extern	int			ErrorLogInitialize_PF( const wchar_t *LogFilePath ) ;					// �G���[�����o���t�@�C�������������鏈���̊��ˑ�����
extern	int			ErrorLogAdd_WCHAR_T_PF( const wchar_t *LogFilePath, const wchar_t *ErrorStr ) ;	// �G���[�����������o�������̊��ˑ�����



#ifndef DX_NON_PRINTF_DX

extern	int			IsInitializeLog( void ) ;												// ���O�@�\�̏��������s�����ǂ������擾����
extern	int			InitializeLog( void ) ;													// ���O�@�\�̏�����
extern	int			TerminateLog( void ) ;													// ���O�����̌�n�����s��
extern	int			DrawLog( void ) ;														// ���O��`�悷��
extern 	int			printfDxBase( const TCHAR *String ) ;									// printfDx �̏���������ł͂Ȃ�������������Ƃ���o�[�W����

// wchar_t�Ŋ֐�
extern 	int			printfDx_WCHAR_T(		 const wchar_t *FormatString , ... ) ;				// printf �Ɠ��������ŉ�ʂɕ������\�����邽�߂̊֐�
extern 	int			printfDxBase_WCHAR_T(	 const wchar_t *String ) ;							// printfDx_WCHAR_T �̏���������ł͂Ȃ�������������Ƃ���o�[�W����
extern 	int			putsDx_WCHAR_T(			 const wchar_t *String , int NewLine ) ;			// puts �Ɠ��������ŉ�ʂɕ������\�����邽�߂̊֐�

#endif // DX_NON_PRINTF_DX

#endif // DX_NON_LOG

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXLOG_H__

