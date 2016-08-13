//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Windows�pOgg�֌W�v���O����
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "../DxCompileConfig.h"

#if !defined( DX_NON_OGGVORBIS ) && !defined( DX_NON_OGGTHEORA )

// �C���N���[�h----------------------------------------------------------------
#include "../DxLib.h"
#include "../DxStatic.h"
#include "../DxUseCLib.h"
#include "../DxLog.h"
#include "DxGraphicsWin.h"
#include "DxUseCLibOggWin.h"
#include "DxUseCLibOggD3D9.h"
#include "DxUseCLibOggD3D11.h"

//namespace DxLib
//{

// �}�N����`------------------------------------------------------------------

// �^��`----------------------------------------------------------------------

// �f�[�^�錾------------------------------------------------------------------


// �֐��v���g�^�C�v�錾 -------------------------------------------------------

static DWORD WINAPI	TheoraDecode_Thread( LPVOID Data ) ;

// �v���O����------------------------------------------------------------------

// ���ˑ��֐�

// �n�[�h�E�G�A�@�\���L���ȏꍇ�� YUV �t�H�[�}�b�g�̈ꎞ�ۑ��p�e�N�X�`�����쐬����
extern	int	TheoraDecode_CreateSurface_PF( DECODE_THEORA *DT )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return TheoraDecode_D3D11_CreateSurface_PF( DT ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return TheoraDecode_D3D9_CreateSurface_PF( DT ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

extern	void	TheoraDecode_ReleaseSurface_PF(	DECODE_THEORA *DT )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		TheoraDecode_D3D11_ReleaseSurface_PF( DT ) ;
		break ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		TheoraDecode_D3D9_ReleaseSurface_PF( DT ) ;
		break ;
#endif // DX_NON_DIRECT3D9

	default :
		break ;
	}
}

// �J�����g�t���[����RGB�摜���쐬����( �߂�l  1:�쐬���ꂽ  0:����Ȃ����� )
extern	int		TheoraDecode_SetupImage_PF( DECODE_THEORA *DT, volatile THEORA_STOCKFRAME *Stock, int ASyncThread )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return TheoraDecode_D3D11_SetupImage_PF( DT, Stock, ASyncThread ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return TheoraDecode_D3D9_SetupImage_PF( DT, Stock, ASyncThread ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �ꎞ�o�b�t�@�� YUV �t�H�[�}�b�g�̃e�N�X�`���𓾂�
extern	const void *TheoraDecode_GetYUVImage_PF( DECODE_THEORA *DT )
{
	switch( GRAWIN.Setting.UseGraphicsAPI )
	{
#ifndef DX_NON_DIRECT3D11
	case GRAPHICS_API_DIRECT3D11_WIN32 :
		return TheoraDecode_D3D11_GetYUVImage_PF( DT ) ;
#endif // DX_NON_DIRECT3D11

#ifndef DX_NON_DIRECT3D9
	case GRAPHICS_API_DIRECT3D9_WIN32 :
		return TheoraDecode_D3D9_GetYUVImage_PF( DT ) ;
#endif // DX_NON_DIRECT3D9

	default :
		return 0 ;
	}
}

// �f�R�[�h�X���b�h
/*
DWORD WINAPI TheoraDecode_Thread( LPVOID Data )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )Data ;

	while( TheoraDecode_Thread_LoopProcess( DT ) != 2 ){}

	// �X���b�h�I��
	ExitThread( 0 ) ;

	// �I��
	return 0 ;
}
*/
void TheoraDecode_Thread( THREAD_INFO *pThreadInfo, void *ASyncLoadThreadData )
{
	DECODE_THEORA *DT = ( DECODE_THEORA * )ASyncLoadThreadData ;

	while( TheoraDecode_Thread_LoopProcess( DT ) != 2 ){}
}

// Ogg Theora �̓ǂݍ��ݏ����̏������s�������̊��ˑ��������s���֐�
extern int TheoraDecode_InitializeStream_PF( DECODE_THEORA *DT )
{
	// �f�R�[�h�����X���b�h�̍쐬
	DT->ThreadStopRequest = 1 ;
	DT->ThreadState = THEORAT_STATE_IDLE ;
	DT->ThreadStandbyTime = NS_GetNowCount() ;
//	DT->DecodeThreadHandle = CreateThread( NULL, 0, TheoraDecode_Thread, DT, 0, ( LPDWORD )&DT->DecodeThreadID ) ;
//	if( DT->DecodeThreadHandle == NULL )
//	{
//		DXST_ERRORLOG_ADDA( "Theora \x83\x80\x81\x5b\x83\x72\x81\x5b\x83\x66\x83\x52\x81\x5b\x83\x68\x97\x70\x83\x58\x83\x8c\x83\x62\x83\x68\x82\xcc\x8d\xec\x90\xac\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n"/*@ "Theora ���[�r�[�f�R�[�h�p�X���b�h�̍쐬�Ɏ��s���܂���\n" @*/ );
//		return -1 ;
//	}
	if( Thread_Create( &DT->DecodeThreadInfo, TheoraDecode_Thread, DT ) == -1 )
	{
		DXST_ERRORLOG_ADDA( "Theora \x83\x80\x81\x5b\x83\x72\x81\x5b\x83\x66\x83\x52\x81\x5b\x83\x68\x97\x70\x83\x58\x83\x8c\x83\x62\x83\x68\x82\xcc\x8d\xec\x90\xac\x82\xc9\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5\x82\xbd\n"/*@ "Theora ���[�r�[�f�R�[�h�p�X���b�h�̍쐬�Ɏ��s���܂���\n" @*/ );
		return -1 ;
	}
//	Thread_SetPriority( &DT->DecodeThreadInfo, DX_THREAD_PRIORITY_NORMAL ) ;
	while( Thread_Resume( &DT->DecodeThreadInfo ) != 1 )
	{
		Thread_Sleep( 0 ) ;
	}

	return 0 ;
}



//}

#endif // #if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )


