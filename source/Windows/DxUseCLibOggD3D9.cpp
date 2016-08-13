//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Ogg�֌W�v���O����( Direct3D9 )
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_DIRECT3D9

#if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

// �C���N���[�h----------------------------------------------------------------
#include "../DxLib.h"
#include "../DxStatic.h"
#include "../DxUseCLib.h"
#include "../DxGraphics.h"
#include "../DxLog.h"
#include "DxGraphicsAPIWin.h"
#include "DxUseCLibOggD3D9.h"
#include "DxUseCLibOggWin.h"

//namespace DxLib
//{

// �}�N����`------------------------------------------------------------------

// �^��`----------------------------------------------------------------------

// �f�[�^�錾------------------------------------------------------------------


// �֐��v���g�^�C�v�錾 -------------------------------------------------------

// �v���O����------------------------------------------------------------------

// ���ˑ��֐�

#ifndef DX_NON_OGGTHEORA

// �n�[�h�E�G�A�@�\���L���ȏꍇ�� YUV �t�H�[�}�b�g�̈ꎞ�ۑ��p�e�N�X�`�����쐬����
extern	int	TheoraDecode_D3D9_CreateSurface_PF( DECODE_THEORA *DT )
{
	ogg_uint32_t w, h ;

	// YUV�t�H�[�}�b�g�̑Ή��𖳌���
	DT->YUVImageFourCC = 0 ;
	DT->ValidYUVSurface = FALSE ;

	return 0 ;

	// �n�[�h�E�G�A�@�\���L���ȏꍇ�� YUV �t�H�[�}�b�g�̈ꎞ�ۑ��p�e�N�X�`�����쐬����
	if( DT->NotUseYUVFormatSurface || GSYS.Setting.ValidHardware == FALSE || Graphics_Hardware_CheckValid_PF() == 0 )
	{
		return 0 ;
	}

	DT->PF->D3D9.YUVSurface = NULL ;

	w = DT->TheoraInfo.width ;
	h = DT->TheoraInfo.height ;

	// �ŏ��� YV12 �`��������
	DT->YUVImageFourCC = MAKEFOURCC( 'Y', 'V', '1', '2' ) ;
	Direct3DDevice9_CreateOffscreenPlainSurface( w, h, ( D_D3DFORMAT )DT->YUVImageFourCC, D_D3DPOOL_DEFAULT, ( D_IDirect3DSurface9 ** )&DT->PF->D3D9.YUVSurface, NULL ) ;

	// �쐬�Ɏ��s������ YUY2 �t�H�[�}�b�g�������Ă݂�
	if( DT->PF->D3D9.YUVSurface == NULL )
	{
		DT->YUVImageFourCC = MAKEFOURCC( 'Y', 'U', 'Y', '2' ) ;
		Direct3DDevice9_CreateOffscreenPlainSurface( w, h, ( D_D3DFORMAT )DT->YUVImageFourCC, D_D3DPOOL_DEFAULT, ( D_IDirect3DSurface9 ** )&DT->PF->D3D9.YUVSurface, NULL ) ;
	}

	// ����ł��ʖڂȂ� UYVY �t�H�[�}�b�g������
	if( DT->PF->D3D9.YUVSurface == NULL )
	{
		DT->YUVImageFourCC = MAKEFOURCC( 'U', 'Y', 'V', 'Y' ) ;
		Direct3DDevice9_CreateOffscreenPlainSurface( w, h, ( D_D3DFORMAT )DT->YUVImageFourCC, D_D3DPOOL_DEFAULT, ( D_IDirect3DSurface9 ** )&DT->PF->D3D9.YUVSurface, NULL ) ;
	}

	// ����ł��ʖڂȂ� YUV �t�H�[�}�b�g����߂�
	if( DT->PF->D3D9.YUVSurface == NULL )
	{
		DT->YUVImageFourCC = 0 ;
		DT->ValidYUVSurface = FALSE ;
	}
	else
	{
		DT->ValidYUVSurface = TRUE ;
	}

	// ����I��
	return 0 ;
}

extern	void	TheoraDecode_D3D9_ReleaseSurface_PF( DECODE_THEORA *DT )
{
	DT->YUVSurfaceReleaseRequest = FALSE ;

	if( DT->PF->D3D9.YUVSurface != NULL )
	{
		DT->PF->D3D9.YUVSurface->Release() ;
		DT->PF->D3D9.YUVSurface = NULL ;
	}
}

// �J�����g�t���[����RGB�摜���쐬����( �߂�l  1:�쐬���ꂽ  0:����Ȃ����� )
extern	int		TheoraDecode_D3D9_SetupImage_PF( DECODE_THEORA *DT, volatile THEORA_STOCKFRAME *Stock, int ASyncThread )
{
	D_D3DLOCKED_RECT LockRect = { 0 } ;
	int              Ok ;
	unsigned char   *d ;
	unsigned char   *ys ;
	unsigned char   *uvs ;
	int              i ;
	int              j ;
	int              ysadd ;
	int              dadd ;
	int              uvadd ;
	int              w ;
	int              h ;
	int              dpitch ;

	// ���b�N�Ɏ��s������T�[�t�F�X�̍쐬���Ȃ���
	Ok = 0 ;
	for( i = 0 ; i < 3 ; i ++ )
	{
		if( DT->ValidYUVSurface == FALSE || DT->YUVSurfaceReleaseRequest || Direct3DSurface9_LockRect_ASync( DT->PF->D3D9.YUVSurface, &LockRect, NULL, 0, ASyncThread ) != D_DD_OK )
		{
			TheoraDecode_CreateSurface( DT, ASyncThread ) ;
		}
		else
		{
			Ok = 1 ;
			break ;
		}
	}

	// �쐬���Ȃ����Ă��ʖڂ����������
	if( Ok == 1 )
	{
		// �t�H�[�}�b�g�ɂ���ď����𕪊�
		d   = ( unsigned char * )LockRect.pBits ;
		ys  = ( unsigned char * )Stock->YBuffer ;
		uvs = ( unsigned char * )Stock->UVBuffer ;
		dpitch = LockRect.Pitch ;

		// YV12 �̏ꍇ
		if( DT->YUVImageFourCC == MAKEFOURCC( 'Y', 'V', '1', '2' ) )
		{
			if( Stock->YWidth  == Stock->UVWidth  * 2 &&
				Stock->YHeight == Stock->UVHeight * 2 )
			{
				unsigned char *us ;
				unsigned char *vs ;

				us = uvs ;
				vs = uvs + 1 ;

				ysadd  = Stock->YStride  * 2 - Stock->YWidth ;
				uvadd  = Stock->UVStride     - Stock->UVWidth * 2 ;
				dadd   = dpitch          * 2 - Stock->YWidth ;
				w = Stock->YWidth  / 2 ;
				h = Stock->YHeight / 2 ;

				for( i = 0 ; i < Stock->YHeight ; i ++, d += dpitch, ys += Stock->YStride )
				{
					_MEMCPY( d, ys, ( size_t )Stock->YWidth ) ; 
				}
				for( i = 0 ; i < Stock->UVHeight ; i ++, d += dpitch / 2, vs += Stock->UVStride )
				{
					for( j = 0 ; j < Stock->UVWidth ; j ++ )
					{
						d[ j ] = vs[ j << 1 ] ;
					}
				}
				for( i = 0 ; i < Stock->UVHeight ; i ++, d += dpitch / 2, us += Stock->UVStride )
				{
					for( j = 0 ; j < Stock->UVWidth ; j ++ )
					{
						d[ j ] = us[ j << 1 ] ;
					}
				}
			}
		}
		else
		// YUY2 �̏ꍇ
		if( DT->YUVImageFourCC == MAKEFOURCC( 'Y', 'U', 'Y', '2' ) )
		{
			if( Stock->YWidth  == Stock->UVWidth  * 2 &&
				Stock->YHeight == Stock->UVHeight * 2 )
			{
				ysadd  = Stock->YStride  * 2 - Stock->YWidth ;
				uvadd  = Stock->UVStride     - Stock->UVWidth * 2 ;
				dadd   = dpitch          * 2 - Stock->YWidth  * 2 ;
				w = Stock->YWidth  / 2 ;
				h = Stock->YHeight / 2 ;
				for( i = 0; i < h; i++, d += dadd, ys += ysadd, uvs += uvadd )
				{
					for( j = 0; j < w; j ++, d += 4, ys += 2, uvs += 2 )
					{
						d[          0 ] = ys[                  0 ] ;
						d[          2 ] = ys[                  1 ] ;
						d[ dpitch     ] = ys[ Stock->YStride     ] ;
						d[ dpitch + 2 ] = ys[ Stock->YStride + 1 ] ;

						d[          1 ] = uvs[ 0 ] ;
						d[ dpitch + 1 ] = uvs[ 0 ] ;

						d[          3 ] = uvs[ 1 ] ;
						d[ dpitch + 3 ] = uvs[ 1 ] ;
					}
				}
			}
		}
		else
		// UYVY �̏ꍇ
		if( DT->YUVImageFourCC == MAKEFOURCC( 'U', 'Y', 'V', 'Y' ) )
		{
			if( Stock->YWidth  == Stock->UVWidth  * 2 &&
				Stock->YHeight == Stock->UVHeight * 2 )
			{
				ysadd  = Stock->YStride  * 2 - Stock->YWidth ;
				uvadd  = Stock->UVStride     - Stock->UVWidth * 2 ;
				dadd   = dpitch          * 2 - Stock->YWidth  * 2 ;
				w = Stock->YWidth  / 2 ;
				h = Stock->YHeight / 2 ;
				for( i = 0; i < h; i++, d += dadd, ys += ysadd, uvs += uvadd )
				{
					for( j = 0; j < w; j ++, d += 4, ys += 2, uvs += 2 )
					{
						d[          1 ] = ys[                  0 ] ;
						d[          3 ] = ys[                  1 ] ;
						d[ dpitch + 1 ] = ys[ Stock->YStride     ] ;
						d[ dpitch + 3 ] = ys[ Stock->YStride + 1 ] ;

						d[          0 ] = uvs[ 0 ] ;
						d[ dpitch + 0 ] = uvs[ 0 ] ;

						d[          2 ] = uvs[ 1 ] ;
						d[ dpitch + 2 ] = uvs[ 1 ] ;
					}
				}
			}
		}

		// ���b�N������
		Direct3DSurface9_UnlockRect_ASync( DT->PF->D3D9.YUVSurface, ASyncThread ) ;

		// �Z�b�g�A�b�v�t���O�𗧂Ă�
		DT->YUVImageSetup = 1 ;
	}

	// ����I��
	return 0 ;
}

// �ꎞ�o�b�t�@�� YUV �t�H�[�}�b�g�̃e�N�X�`���𓾂�
extern	const void *TheoraDecode_D3D9_GetYUVImage_PF( DECODE_THEORA *DT )
{
	// �A�h���X��Ԃ�
	return DT->PF->D3D9.YUVSurface ;
}

#endif // DX_NON_OGGTHEORA


//}

#endif // #if !defined( DX_NON_OGGVORBIS ) || !defined( DX_NON_OGGTHEORA )

#endif // DX_NON_DIRECT3D9

