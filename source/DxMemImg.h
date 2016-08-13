// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		�������C���[�W����p�w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

// ���d�C���N���[�h�h�~�p��`
#ifndef __DX_MEMIMG_H__
#define __DX_MEMIMG_H__

// �C���N���[�h----------------------------------------------------------------
#include "DxCompileConfig.h"
#include "DxLib.h"


#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����`------------------------------------------------------------------

#define MEMIMG_VALID(MIMG)			( (MIMG).Image != NULL )
#define MEMIMG_INITIALIZECODE		(0x12345678)

#define MEMIMG_XRGB8_A				(0x00000000)
#define MEMIMG_XRGB8_R				(0x00ff0000)
#define MEMIMG_XRGB8_G				(0x0000ff00)
#define MEMIMG_XRGB8_B				(0x000000ff)

#define MEMIMG_XRGB8_LR				(16)
#define MEMIMG_XRGB8_LG				(8)
#define MEMIMG_XRGB8_LB				(0)

#define MEMIMG_ARGB8_A				(0xff000000)
#define MEMIMG_ARGB8_R				(0x00ff0000)
#define MEMIMG_ARGB8_G				(0x0000ff00)
#define MEMIMG_ARGB8_B				(0x000000ff)

#define MEMIMG_X8A8R5G6B5_A			(0x00ff0000)
#define MEMIMG_X8A8R5G6B5_R			(0x0000f800)
#define MEMIMG_X8A8R5G6B5_G			(0x000007e0)
#define MEMIMG_X8A8R5G6B5_B			(0x0000001f)

#define MEMIMG_R5G6B5_A				(0x0000)
#define MEMIMG_R5G6B5_R				(0xf800)
#define MEMIMG_R5G6B5_G				(0x07e0)
#define MEMIMG_R5G6B5_B				(0x001f)

#define MEMIMG_R5G6B5_LR			(11)
#define MEMIMG_R5G6B5_LG			(5)
#define MEMIMG_R5G6B5_LB			(0)

#define MEMIMG_R5G6B5_ALPHAMASK1	(0x7bef7bef)
#define MEMIMG_R5G6B5_ADDMASK1		(0xf7def7df)
#define MEMIMG_R5G6B5_ADDMASK2		(0x08210820)
#define MEMIMG_R5G6B5_ADDMASK3		(0x08210820)
#define MEMIMG_R5G6B5_ADDSHFT		(5)

#define MEMIMG_R5G6B5_SUBMASK1		MEMIMG_R5G6B5_ADDMASK1
#define MEMIMG_R5G6B5_SUBMASK2		MEMIMG_R5G6B5_ADDMASK2
#define MEMIMG_R5G6B5_SUBMASK3		MEMIMG_R5G6B5_ADDMASK3
#define MEMIMG_R5G6B5_SUBSHFT		MEMIMG_R5G6B5_ADDSHFT

#define MEMIMG_RGB5_R				(0x7c00)
#define MEMIMG_RGB5_G				(0x3e0)
#define MEMIMG_RGB5_B				(0x1f)

#define MEMIMG_RGB5_LR				(10)
#define MEMIMG_RGB5_LG				(5)
#define MEMIMG_RGB5_LB				(0)

#define MEMIMG_XRGB8_ALPHAMASK1		(0x007f7f7f)
#define MEMIMG_XRGB8_ADDMASK1		(0x00fefeff)
#define MEMIMG_XRGB8_ADDMASK2		(0x01010100)
#define MEMIMG_XRGB8_ADDMASK3		(0x00010100)
#define MEMIMG_XRGB8_ADDSHFT		(8)

#define MEMIMG_XRGB8_SUBMASK1		MEMIMG_XRGB8_ADDMASK1
#define MEMIMG_XRGB8_SUBMASK2		MEMIMG_XRGB8_ADDMASK2
#define MEMIMG_XRGB8_SUBMASK3		MEMIMG_XRGB8_ADDMASK3
#define MEMIMG_XRGB8_SUBSHFT		MEMIMG_XRGB8_ADDSHFT

// �Œ菬���_�Ɏg�p����鏬���_�r�b�g��
#define QUALI	(1 << 18)

// �y�C���g�p�o�b�t�@�̃T�C�Y
#define PAINTBUFFERSIZE		(8000)

// �A���t�@�p�}�X�N�}�N��
#if 1
#define MK565_5 	(0X00000000)         //ShiftMasking  PixelFormat565
#define MK565_4 	(0X08610861)
#define MK565_3 	(0X18E318E3)
#define MK565_2 	(0X39E739E7)
#define MK565_1 	(0X7BEF7BEF)
#define MK565_0 	(0XFFFFFFFF)
#else
#define MK565_5 	(0X00000000)         //ShiftMasking  PixelFormat565
#define MK565_4 	(0X08410841)
#define MK565_3 	(0X18c318c3)
#define MK565_2 	(0X39c739c7)
#define MK565_1 	(0X7BcF7BcF)
#define MK565_0 	(0XFFFFFFFF)
#endif

#define MK565_R 	(0xf800)
#define MK565_G 	(0x7e0)
#define MK565_B 	(0x1f)

#define MK565_LR 	(11)
#define MK565_LG 	(5)
#define MK565_LB 	(0)

#define MK565_WR 	(5)
#define MK565_WG 	(6)
#define MK565_WB 	(5)

#define MK565_MR 	(0x20)
#define MK565_MG 	(0x40)
#define MK565_MB 	(0x20)
									    //ADD Mask      PixelFormat565
#define MK565_ADD4 	(0x7BEF7BEF)    //���オ�蕔�����O�ɂ���
#define MK565_ADD5 	(0x84108410)    //���オ�茟�o�}�X�N�iADD�Ŏg�p�j
#define MK565_ADD6 	(0x08210821)	//�ŉ��ʃr�b�g�}�X�N

#define MK565_ADD1 	(0xF7DF)		// ���オ�蕔�����O�ɂ���
#define MK565_ADD2 	(0x10820)		// ���オ��r�b�g���o�}�X�N
#define MK565_ADD3 	(0x0820)		// �ŉ��ʃr�b�g�}�X�N

#define MK565_SUB1 	(0xF7DF)		// ���オ�蕔�����O�ɂ���
#define MK565_SUB2 	(0x10820)		// ���オ��r�b�g���o�}�X�N
#define MK565_SUB3 	(0x0820)		// �ŉ��ʃr�b�g�}�X�N


//#define MK565_ADD1 	(0x7BEE7BEE)
//#define MK565_ADD2 	(0x84108410)
//#define MK565_ADD3 	(0x08610861)	//�ŉ��ʃr�b�g�}�X�N

#define MK565_R5 	(0x00000000)
#define MK565_R4 	(0x08000800)
#define MK565_R3 	(0x18001800)
#define MK565_R2 	(0x38003800)
#define MK565_R1 	(0x78007800)
#define MK565_R0 	(0xf800f800)

#define MK565_G5 	(0x00000000)
#define MK565_G4 	(0x00600060)
#define MK565_G3 	(0x00e000e0)
#define MK565_G2 	(0x01e001e0)
#define MK565_G1 	(0x03e003e0)
#define MK565_G0 	(0x07e007e0)

#define MK565_B5 	(0x00000000)
#define MK565_B4 	(0x00010001)
#define MK565_B3 	(0x00030003)
#define MK565_B2 	(0x00070007)
#define MK565_B1 	(0x000f000f)
#define MK565_B0 	(0x001f001f)

#define MK565_TN 	(64)


#define MK555_5 	(0X00000000)         //ShiftMasking  PixelFormat555
#define MK555_4 	(0X04210421)
#define MK555_3 	(0X0C630C63)
#define MK555_2 	(0X1CE71CE7)
#define MK555_1 	(0X3DEF3DEF)
#define MK555_0 	(0XFFFFFFFF)

#define MK555_R5 	(0x00000000)
#define MK555_R4 	(0x04000400)
#define MK555_R3 	(0x0c000c00)
#define MK555_R2 	(0x1c001c00)
#define MK555_R1 	(0x3c003c00)
#define MK555_R0 	(0x7c007c00)

#define MK555_G5 	(0x00000000)
#define MK555_G4 	(0x00200020)
#define MK555_G3 	(0x00600060)
#define MK555_G2 	(0x00e000e0)
#define MK555_G1 	(0x01e001e0)
#define MK555_G0 	(0x03e003e0)

#define MK555_B5 	(0x00000000)
#define MK555_B4 	(0x00010001)
#define MK555_B3 	(0x00030003)
#define MK555_B2 	(0x00070007)
#define MK555_B1 	(0x000f000f)
#define MK555_B0 	(0x001f001f)

#define MK555_R 	(0x7c00)
#define MK555_G 	(0x3e0)
#define MK555_B 	(0x1f)

#define MK555_LR 	(10)
#define MK555_LG 	(5)
#define MK555_LB 	(0)

#define MK555_WR 	(5)
#define MK555_WG 	(5)
#define MK555_WB 	(5)

#define MK555_MR 	(0x20)
#define MK555_MG 	(0x20)
#define MK555_MB 	(0x20)

#define MK555_TN 	(64)

/*									    //ADD Mask      PixelFormat555
#define MK555_ADD1 	(0x3DEF3DEF)      //���オ�蕔�����O�ɂ���
//#define MK555_ADD2 	(0x42104210)      //���オ�茟�o�}�X�N�iADD�Ŏg�p�j

//#define MK555_ADD1 	(0x3DEE3DEE)
#define MK555_ADD2 	(0x42104210)
#define MK555_ADD3 	(0x04210421)	//�ŉ��ʃr�b�g�}�X�N
*/

#define MK555_ADD4 	(0x3DEF3DEF)    //���オ�蕔�����O�ɂ���
#define MK555_ADD5 	(0x42104210)    //���オ�茟�o�}�X�N�iADD�Ŏg�p�j
#define MK555_ADD6 	(0x04210421)	//�ŉ��ʃr�b�g�}�X�N

#define MK555_ADD1 	(0x7BDF)		// ���オ�蕔�����O�ɂ���
#define MK555_ADD2 	(0x8420)		// ���オ��r�b�g���o�}�X�N
#define MK555_ADD3 	(0x0420)		// �ŉ��ʃr�b�g�}�X�N

#define MK555_SUB1 	(0x7BDF)		// ���オ�蕔�����O�ɂ���
#define MK555_SUB2 	(0x8420)		// ���オ��r�b�g���o�}�X�N
#define MK555_SUB3 	(0x0420)		// �ŉ��ʃr�b�g�}�X�N



#define MK888_5 	(0X00070707)         //ShiftMasking  PixelFormat888
#define MK888_4 	(0X000F0F0F)
#define MK888_3 	(0X001F1F1F)
#define MK888_2 	(0X003F3F3F)
#define MK888_1 	(0X007F7F7F)
#define MK888_0 	(0X00FFFFFF)

#define MK888_R5 	(0x00070000)
#define MK888_R4 	(0x000f0000)
#define MK888_R3 	(0x001f0000)
#define MK888_R2 	(0x003f0000)
#define MK888_R1 	(0x007f0000)
#define MK888_R0 	(0x00ff0000)

#define MK888_G5 	(0x00000700)
#define MK888_G4 	(0x00000f00)
#define MK888_G3 	(0x00001f00)
#define MK888_G2 	(0x00003f00)
#define MK888_G1 	(0x00007f00)
#define MK888_G0 	(0x0000ff00)

#define MK888_B5 	(0x00000007)
#define MK888_B4 	(0x0000000f)
#define MK888_B3 	(0x0000001f)
#define MK888_B2 	(0x0000003f)
#define MK888_B1 	(0x0000007f)
#define MK888_B0 	(0x000000ff)


#define MK888_R 	(0xFF0000)
#define MK888_G 	(0x00FF00)
#define MK888_B 	(0x0000FF)

#define MK888_LR 	(16)
#define MK888_LG 	(8)
#define MK888_LB 	(0)

#define MK888_WR 	(8)
#define MK888_WG 	(8)
#define MK888_WB 	(8)

#define MK888_MR 	(0x100)
#define MK888_MG 	(0x100)
#define MK888_MB 	(0x100)

#define MK888_TN 	(256)

/*									    //ADD Mask      PixelFormat565
#define MK888_ADD1 	(0x7F7F7F)      //���オ�蕔�����O�ɂ���
//#define MK888_ADD2 	(0x808080)      //���オ�茟�o�}�X�N�iADD�Ŏg�p�j

//#define MK888_ADD1 	(0x7F7F7E)
#define MK888_ADD2 	(0x808080)
#define MK888_ADD3 	(0x10101)	//�ŉ��ʃr�b�g�}�X�N
*/

#define MK888_ADD4 	(0x7F7F7F)		//���オ�蕔�����O�ɂ���
#define MK888_ADD5 	(0x808080)		//���オ�茟�o�}�X�N�iADD�Ŏg�p�j
#define MK888_ADD6 	(0x010101)		//�ŉ��ʃr�b�g�}�X�N

#define MK888_ADD1 	(0x0FEFEFF)		// ���オ�蕔�����O�ɂ���
#define MK888_ADD2 	(0x1010100)		// ���オ��r�b�g���o�}�X�N
#define MK888_ADD3 	(0x0010100)		// �ŉ��ʃr�b�g�}�X�N

#define MK888_SUB1 	(0x0FEFEFF)		// ���オ�蕔�����O�ɂ���
#define MK888_SUB2 	(0x1010100)		// ���オ��r�b�g���o�}�X�N
#define MK888_SUB3	(0x0010100)		// �ŉ��ʃr�b�g�}�X�N







// 16bit

  #define RMSK16(DATA)   ((DWORD)(DATA) & MEMIMG_R5G6B5_R)
  #define GMSK16(DATA)   ((DWORD)(DATA) & MEMIMG_R5G6B5_G)
  #define BMSK16(DATA)   ((DWORD)(DATA) & MEMIMG_R5G6B5_B)

  #define RBOT16(DATA)	 (  (DATA)                    >> MEMIMG_R5G6B5_LR )
  #define GBOT16(DATA)	 ( ((DATA) & MEMIMG_R5G6B5_G) >> MEMIMG_R5G6B5_LG )
  #define BBOT16(DATA)	 (  (DATA) & MEMIMG_R5G6B5_B                      )

  #define RUP16(DATA)	 ( (DATA) << MEMIMG_R5G6B5_LR )
  #define GUP16(DATA)	 ( (DATA) << MEMIMG_R5G6B5_LG )
  #define BUP16(DATA)	 ( (DATA)                     )

  #define BIMG(DATA)	  MemImgManage.BlendGraphTable[(DATA)]

  #define RM16			MEMIMG_R5G6B5_R
  #define GM16			MEMIMG_R5G6B5_G
  #define BM16			MEMIMG_R5G6B5_B

  #define RL16			MEMIMG_R5G6B5_LR
  #define GL16			MEMIMG_R5G6B5_LG
  #define BL16			MEMIMG_R5G6B5_LB

  #define RTABLE(DATA)	RedTable[(DATA)]
  #define GTABLE(DATA)	GreenTable[(DATA)]
  #define BTABLE(DATA)	BlueTable[(DATA)]

  #define MT1(DATA)		RateTable1[(DATA)]
  #define MT2(DATA)		RateTable2[(DATA)]

  #define BPARAM		MemImgManage.BlendParam

  // BI:�u�����h�C���[�W   BR:�P�x  AC:���`�����l�� 
  // BNO:�u�����h����  BAL:���u�����h  BAD:���Z�u�����h  BSB:���Z�u�����h  BML:��Z�u�����h  BMA:��Z�{���u�����h  BIS:�`�挳�̐F�𔽓]�{���u�����h
  // ND:�W���`��  FD:�����`��
  // TCK:���ߐF�`�F�b�N  ACK:���`�F�b�N
  // NTBL:�u�����h�摜�ȊO�̏�b�e�[�u�����g�p���Ȃ�

  // �u�����h����

	// �u�����h�摜����

	  // �P�x����

		// ���`�����l������
		#define CODE_UBI_C16_NBR_UAC_BNO( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
			Src2 = ( DWORD )( (DEST) << 16 ) ;\
			*(DESTP) = (WORD)( RMSK16( ( ( RMSK16(SRC) - RMSK16(DEST) ) * BIMG(BLEND) * SRCA + ( Src2 & ( RM16 << 16 ) ) ) >> 16 ) |\
							   GMSK16( ( ( GMSK16(SRC) - GMSK16(DEST) ) * BIMG(BLEND) * SRCA + ( Src2 & ( GM16 << 16 ) ) ) >> 16 ) |\
							         ( ( ( BMSK16(SRC) - BMSK16(DEST) ) * BIMG(BLEND) * SRCA + ( Src2 & ( BM16 << 16 ) ) ) >> 16 ) ) ;\
		}
		
		// ���`�����l������(�`�F�b�N�R�[�h�t��)
		#define CODE_UBI_C16_NBR_UAC_BNO_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_NBR_NAC_BNO( SRC, DEST, DESTP, BLEND )\
				else				CODE_UBI_C16_NBR_UAC_BNO( SRC, SRCA, DEST, DESTP, BLEND )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_NBR_NAC_BNO( SRC, DEST, DESTP, BLEND )	\
		{\
			Src2 = ( DWORD )( (DEST) << 8 ) ;\
			*(DESTP) = (WORD)( RMSK16( ( ( RMSK16(SRC) - RMSK16(DEST) ) * BIMG(BLEND) + ( Src2 & ( RM16 << 8 ) ) ) >> 8 ) |\
							   GMSK16( ( ( GMSK16(SRC) - GMSK16(DEST) ) * BIMG(BLEND) + ( Src2 & ( GM16 << 8 ) ) ) >> 8 ) |\
							         ( ( ( BMSK16(SRC) - BMSK16(DEST) ) * BIMG(BLEND) + ( Src2 & ( BM16 << 8 ) ) ) >> 8 ) ) ;\
		}
		
		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_NBR_NAC_BNO_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_NBR_NAC_BNO( SRC, DEST, DESTP, BLEND )
		

	  // �P�x����

		// ���`�����l������
		#define CODE_UBI_C16_UBR_UAC_BNO( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( RTABLE( RBOT16(SRC) ) - RBOT16(DEST) ) * BIMG(BLEND) * SRCA + (RMSK16(DEST) << ( 16 - MEMIMG_R5G6B5_LR ) ) ) >> ( 16 - MEMIMG_R5G6B5_LR ) ) |\
							   GMSK16( ( ( GTABLE( GBOT16(SRC) ) - GBOT16(DEST) ) * BIMG(BLEND) * SRCA + (GMSK16(DEST) << ( 16 - MEMIMG_R5G6B5_LG ) ) ) >> ( 16 - MEMIMG_R5G6B5_LG ) ) |\
							         ( ( ( BTABLE( BBOT16(SRC) ) - BBOT16(DEST) ) * BIMG(BLEND) * SRCA + (BMSK16(DEST) << ( 16 - MEMIMG_R5G6B5_LB ) ) ) >> ( 16 - MEMIMG_R5G6B5_LB ) ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BNO_NTBL( SRC, SRCA, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RBOT16(SRC) * RBRIGHT ) >> 8 ) - RBOT16(DEST) ) * BIMG(BLEND) * SRCA + (RMSK16(DEST) << ( 16 - MEMIMG_R5G6B5_LR ) ) ) >> ( 16 - MEMIMG_R5G6B5_LR ) ) |\
							   GMSK16( ( ( ( ( GBOT16(SRC) * GBRIGHT ) >> 8 ) - GBOT16(DEST) ) * BIMG(BLEND) * SRCA + (GMSK16(DEST) << ( 16 - MEMIMG_R5G6B5_LG ) ) ) >> ( 16 - MEMIMG_R5G6B5_LG ) ) |\
							         ( ( ( ( ( BBOT16(SRC) * BBRIGHT ) >> 8 ) - BBOT16(DEST) ) * BIMG(BLEND) * SRCA + (BMSK16(DEST) << ( 16 - MEMIMG_R5G6B5_LB ) ) ) >> ( 16 - MEMIMG_R5G6B5_LB ) ) ) ;\
		}
		
		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_UAC_BNO_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BNO( SRC, DEST, DESTP, BLEND )\
				else				CODE_UBI_C16_UBR_UAC_BNO( SRC, SRCA, DEST, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BNO_ACK_NTBL( SRC, SRCA, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BNO_NTBL( SRC, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )\
				else				CODE_UBI_C16_UBR_UAC_BNO_NTBL( SRC, SRCA, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_UBR_NAC_BNO( SRC, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( RTABLE( RBOT16(SRC) ) - RBOT16(DEST) ) * BIMG(BLEND) + (RMSK16(DEST) >> 3) ) << 3 ) |\
							   GMSK16( ( ( GTABLE( GBOT16(SRC) ) - GBOT16(DEST) ) * BIMG(BLEND) + (GMSK16(DEST) << 3) ) >> 3 ) |\
							         ( ( ( BTABLE( BBOT16(SRC) ) - BBOT16(DEST) ) * BIMG(BLEND) + (BMSK16(DEST) << 8) ) >> 8 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BNO_NTBL( SRC, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RBOT16(SRC) * RBRIGHT ) >> 8 ) - RBOT16(DEST) ) * BIMG(BLEND) + (RMSK16(DEST) >> 3) ) << 3 ) |\
							   GMSK16( ( ( ( ( GBOT16(SRC) * GBRIGHT ) >> 8 ) - GBOT16(DEST) ) * BIMG(BLEND) + (GMSK16(DEST) << 3) ) >> 3 ) |\
							         ( ( ( ( ( BBOT16(SRC) * BBRIGHT ) >> 8 ) - BBOT16(DEST) ) * BIMG(BLEND) + (BMSK16(DEST) << 8) ) >> 8 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_NAC_BNO_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BNO( SRC, DEST, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BNO_TCK_NTBL( SRC, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BNO_NTBL( SRC, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )

	// �u�����h�摜����

	  // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_NBR_UAC_BNO( SRC, SRCA, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( RMSK16(SRC) - RMSK16(DEST) ) * SRCA + ( RMSK16(DEST) << 8 ) ) >> 8 ) |\
							   GMSK16( ( ( GMSK16(SRC) - GMSK16(DEST) ) * SRCA + ( GMSK16(DEST) << 8 ) ) >> 8 ) |\
							         ( ( ( BMSK16(SRC) - BMSK16(DEST) ) * SRCA + ( BMSK16(DEST) << 8 ) ) >> 8 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_UAC_BNO_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_NBR_NAC_BNO( SRC, DEST, DESTP )\
				else				CODE_NBI_C16_NBR_UAC_BNO( SRC, SRCA, DEST, DESTP )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_NBR_NAC_BNO( SRC, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)(SRC) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_NAC_BNO_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_NBR_NAC_BNO( SRC, DEST, DESTP )

	
	  // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_UBR_UAC_BNO( SRC, SRCA, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( RTABLE( RBOT16(SRC) ) - RBOT16(DEST) ) * SRCA + ( RMSK16(DEST) >> 3 ) ) << 3 ) |\
							   GMSK16( ( ( GTABLE( GBOT16(SRC) ) - GBOT16(DEST) ) * SRCA + ( GMSK16(DEST) << 3 ) ) >> 3 ) |\
							         ( ( ( BTABLE( BBOT16(SRC) ) - BBOT16(DEST) ) * SRCA + ( BMSK16(DEST) << 8 ) ) >> 8 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BNO_NTBL( SRC, SRCA, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RBOT16(SRC) * RBRIGHT ) >> 8 ) - RBOT16(DEST) ) * SRCA + ( RMSK16(DEST) >> 3 ) ) << 3 ) |\
							   GMSK16( ( ( ( ( GBOT16(SRC) * GBRIGHT ) >> 8 ) - GBOT16(DEST) ) * SRCA + ( GMSK16(DEST) << 3 ) ) >> 3 ) |\
							         ( ( ( ( ( BBOT16(SRC) * BBRIGHT ) >> 8 ) - BBOT16(DEST) ) * SRCA + ( BMSK16(DEST) << 8 ) ) >> 8 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_UAC_BNO_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BNO( SRC, DEST, DESTP )\
				else				CODE_NBI_C16_UBR_UAC_BNO( SRC, SRCA, DEST, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BNO_ACK_NTBL( SRC, SRCA, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BNO_NTBL( SRC, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )\
				else				CODE_NBI_C16_UBR_UAC_BNO_NTBL( SRC, SRCA, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_UBR_NAC_BNO( SRC, DEST, DESTP )	\
		{\
		    *(DESTP) = (WORD)( RUP16( RTABLE( RBOT16(SRC) ) ) |\
							   GUP16( GTABLE( GBOT16(SRC) ) ) |\
							   BUP16( BTABLE( BBOT16(SRC) ) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BNO_NTBL( SRC, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
		    *(DESTP) = (WORD)( RMSK16( ( RMSK16(SRC) * RBRIGHT ) >> 8 ) |\
							   GMSK16( ( GMSK16(SRC) * GBRIGHT ) >> 8 ) |\
							   BMSK16( ( BMSK16(SRC) * BBRIGHT ) >> 8 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_NAC_BNO_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BNO( SRC, DEST, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BNO_TCK_NTBL( SRC, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BNO_NTBL( SRC, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )

  // ���u�����h

    // �u�����h�摜����
 
      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C16_NBR_UAC_BAL( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( RBOT16(SRC) - RBOT16(DEST) ) * BIMG(BLEND) * SRCA * BPARAM + ( RMSK16(DEST) << 13 ) ) >> 13 ) ) |\
							   GMSK16( ( ( ( GBOT16(SRC) - GBOT16(DEST) ) * BIMG(BLEND) * SRCA * BPARAM + ( GMSK16(DEST) << 19 ) ) >> 19 ) ) |\
							         ( ( ( ( BBOT16(SRC) - BBOT16(DEST) ) * BIMG(BLEND) * SRCA * BPARAM + ( BMSK16(DEST) << 24 ) ) >> 24 ) ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_UAC_BAL_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( RBOT16(SRC) - RBOT16(DEST) ) * BIMG(BLEND) * SRCA * ABRIGHT + ( RMSK16(DEST) << 13 ) ) >> 13 ) ) |\
							   GMSK16( ( ( ( GBOT16(SRC) - GBOT16(DEST) ) * BIMG(BLEND) * SRCA * ABRIGHT + ( GMSK16(DEST) << 19 ) ) >> 19 ) ) |\
							         ( ( ( ( BBOT16(SRC) - BBOT16(DEST) ) * BIMG(BLEND) * SRCA * ABRIGHT + ( BMSK16(DEST) << 24 ) ) >> 24 ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C16_NBR_UAC_BAL_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_NBR_NAC_BAL( SRC, DEST, DESTP, BLEND )\
				else				CODE_UBI_C16_NBR_UAC_BAL( SRC, SRCA, DEST, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_UAC_BAL_ACK_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_NBR_NAC_BAL_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )\
				else				CODE_UBI_C16_NBR_UAC_BAL_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_NBR_NAC_BAL( SRC, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( RMSK16(SRC) - RMSK16(DEST) ) * BIMG(BLEND) * BPARAM + ( RMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							   GMSK16( ( ( ( GMSK16(SRC) - GMSK16(DEST) ) * BIMG(BLEND) * BPARAM + ( GMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							         ( ( ( ( BMSK16(SRC) - BMSK16(DEST) ) * BIMG(BLEND) * BPARAM + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_NAC_BAL_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( RMSK16(SRC) - RMSK16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( RMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							   GMSK16( ( ( ( GMSK16(SRC) - GMSK16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( GMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							         ( ( ( ( BMSK16(SRC) - BMSK16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_NBR_NAC_BAL_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_NBR_NAC_BAL( SRC, DEST, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_NAC_BAL_TCK_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )	\
			if( SRC != TransColor ) CODE_UBI_C16_NBR_NAC_BAL_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )


      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C16_UBR_UAC_BAL( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( RTABLE( RBOT16(SRC) ) - RBOT16(DEST) ) * BIMG(BLEND) * BPARAM * SRCA + ( RMSK16(DEST) << 13 ) ) >> 13 ) |\
							   GMSK16( ( ( GTABLE( GBOT16(SRC) ) - GBOT16(DEST) ) * BIMG(BLEND) * BPARAM * SRCA + ( GMSK16(DEST) << 19 ) ) >> 19 ) |\
							   BMSK16( ( ( BTABLE( BBOT16(SRC) ) - BBOT16(DEST) ) * BIMG(BLEND) * BPARAM * SRCA + ( BMSK16(DEST) << 24 ) ) >> 24 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BAL_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RBOT16(SRC) * RBRIGHT ) >> 8 ) - RBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT * SRCA + ( RMSK16(DEST) << 13 ) ) >> 13 ) |\
							   GMSK16( ( ( ( ( GBOT16(SRC) * GBRIGHT ) >> 8 ) - GBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT * SRCA + ( GMSK16(DEST) << 19 ) ) >> 19 ) |\
							   BMSK16( ( ( ( ( BBOT16(SRC) * BBRIGHT ) >> 8 ) - BBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT * SRCA + ( BMSK16(DEST) << 24 ) ) >> 24 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_UAC_BAL_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BAL( SRC, DEST, DESTP, BLEND )\
				else				CODE_UBI_C16_UBR_UAC_BAL( SRC, SRCA, DEST, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BAL_ACK_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BAL_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else				CODE_UBI_C16_UBR_UAC_BAL_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_UBR_NAC_BAL( SRC, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( RTABLE( RBOT16(SRC) ) - RBOT16(DEST) ) * BIMG(BLEND) * BPARAM + ( RMSK16(DEST) << 5 ) ) >> 5 ) |\
							   GMSK16( ( ( GTABLE( GBOT16(SRC) ) - GBOT16(DEST) ) * BIMG(BLEND) * BPARAM + ( GMSK16(DEST) << 11 ) ) >> 11 ) |\
							   BMSK16( ( ( BTABLE( BBOT16(SRC) ) - BBOT16(DEST) ) * BIMG(BLEND) * BPARAM + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BAL_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RBOT16(SRC) * RBRIGHT ) >> 8 ) - RBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( RMSK16(DEST) << 5 ) ) >> 5 ) |\
							   GMSK16( ( ( ( ( GBOT16(SRC) * GBRIGHT ) >> 8 ) - GBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( GMSK16(DEST) << 11 ) ) >> 11 ) |\
							   BMSK16( ( ( ( ( BBOT16(SRC) * BBRIGHT ) >> 8 ) - BBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_NAC_BAL_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BAL( SRC, DEST, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BAL_TCK_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BAL_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

    // �u�����h�摜����

      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_NBR_UAC_BAL( SRC, SRCA, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( RMSK16(SRC) - RMSK16(DEST) ) * SRCA * BPARAM + ( RMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							   GMSK16( ( ( ( GMSK16(SRC) - GMSK16(DEST) ) * SRCA * BPARAM + ( GMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							         ( ( ( ( BMSK16(SRC) - BMSK16(DEST) ) * SRCA * BPARAM + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_UAC_BAL_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( RMSK16(SRC) - RMSK16(DEST) ) * SRCA * ABRIGHT + ( RMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							   GMSK16( ( ( ( GMSK16(SRC) - GMSK16(DEST) ) * SRCA * ABRIGHT + ( GMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							         ( ( ( ( BMSK16(SRC) - BMSK16(DEST) ) * SRCA * ABRIGHT + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_UAC_BAL_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_NBR_NAC_BAL( SRC, DEST, DESTP )\
				else				CODE_NBI_C16_NBR_UAC_BAL( SRC, SRCA, DEST, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_UAC_BAL_ACK_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_NBR_NAC_BAL_NTBL( SRC, DEST, DESTP, ABRIGHT )\
				else				CODE_NBI_C16_NBR_UAC_BAL_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_NBR_NAC_BAL( SRC, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RUP16( MT2( (int)RBOT16(SRC) - (int)RBOT16(DEST) ) + RBOT16(DEST) ) |\
							   GUP16( MT2( (int)GBOT16(SRC) - (int)GBOT16(DEST) ) + GBOT16(DEST) ) |\
							   BUP16( MT2( (int)BBOT16(SRC) - (int)BBOT16(DEST) ) + BBOT16(DEST) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_NAC_BAL_NTBL( SRC, DEST, DESTP, ABRIGHT )	\
		{\
			*(DESTP) = (WORD)( RUP16( ( ( ( (int)RBOT16(SRC) - (int)RBOT16(DEST) ) * ABRIGHT ) >> 8 ) + RBOT16(DEST) ) |\
							   GUP16( ( ( ( (int)GBOT16(SRC) - (int)GBOT16(DEST) ) * ABRIGHT ) >> 8 ) + GBOT16(DEST) ) |\
							   BUP16( ( ( ( (int)BBOT16(SRC) - (int)BBOT16(DEST) ) * ABRIGHT ) >> 8 ) + BBOT16(DEST) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_NAC_BAL_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_NBR_NAC_BAL( SRC, DEST, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_NAC_BAL_TCK_NTBL( SRC, DEST, DESTP, ABRIGHT )	\
			if( SRC != TransColor ) CODE_NBI_C16_NBR_NAC_BAL_NTBL( SRC, DEST, DESTP, ABRIGHT )


      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_UBR_UAC_BAL( SRC, SRCA, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( RTABLE( RBOT16(SRC) ) - RBOT16(DEST) ) * BPARAM * SRCA + ( RMSK16(DEST) << 5  ) ) >> 5  ) |\
							   GMSK16( ( ( GTABLE( GBOT16(SRC) ) - GBOT16(DEST) ) * BPARAM * SRCA + ( GMSK16(DEST) << 11 ) ) >> 11 ) |\
							   BMSK16( ( ( BTABLE( BBOT16(SRC) ) - BBOT16(DEST) ) * BPARAM * SRCA + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BAL_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RBOT16(SRC) * RBRIGHT ) >> 8 ) - RBOT16(DEST) ) * ABRIGHT * SRCA + ( RMSK16(DEST) << 5  ) ) >> 5  ) |\
							   GMSK16( ( ( ( ( GBOT16(SRC) * GBRIGHT ) >> 8 ) - GBOT16(DEST) ) * ABRIGHT * SRCA + ( GMSK16(DEST) << 11 ) ) >> 11 ) |\
							   BMSK16( ( ( ( ( BBOT16(SRC) * BBRIGHT ) >> 8 ) - BBOT16(DEST) ) * ABRIGHT * SRCA + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_UAC_BAL_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BAL( SRC, DEST, DESTP )\
				else				CODE_NBI_C16_UBR_UAC_BAL( SRC, SRCA, DEST, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BAL_ACK_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BAL_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else				CODE_NBI_C16_UBR_UAC_BAL_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_UBR_NAC_BAL( SRC, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RUP16( MT2( (int)RTABLE( RBOT16(SRC) ) - (int)RBOT16(DEST) ) + RBOT16(DEST) ) |\
							   GUP16( MT2( (int)GTABLE( GBOT16(SRC) ) - (int)GBOT16(DEST) ) + GBOT16(DEST) ) |\
							   BUP16( MT2( (int)BTABLE( BBOT16(SRC) ) - (int)BBOT16(DEST) ) + BBOT16(DEST) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BAL_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RUP16( ( ( ( (int)( ( RBOT16(SRC) * RBRIGHT ) >> 8 ) * ABRIGHT ) >> 8 ) - (int)RBOT16(DEST) ) + RBOT16(DEST) ) |\
							   GUP16( ( ( ( (int)( ( GBOT16(SRC) * GBRIGHT ) >> 8 ) * ABRIGHT ) >> 8 ) - (int)GBOT16(DEST) ) + GBOT16(DEST) ) |\
							   BUP16( ( ( ( (int)( ( BBOT16(SRC) * BBRIGHT ) >> 8 ) * ABRIGHT ) >> 8 ) - (int)BBOT16(DEST) ) + BBOT16(DEST) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_NAC_BAL_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BAL( SRC, DEST, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BAL_TCK_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BAL_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

  // ���Z�u�����h

	// �u�����h�摜����

      // �P�x�Ȃ�

		// ���`�����l������
		#define CODE_UBI_C16_NBR_UAC_BAD( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
			Src1 = (WORD)( RUP16( MT1( (DWORD)( RMSK16(SRC) * SRCA * BIMG(BLEND) ) >> 27 ) ) +\
						   GUP16( MT1( (DWORD)( GMSK16(SRC) * SRCA * BIMG(BLEND) ) >> 21 ) ) +\
						   BUP16( MT1( (DWORD)( BMSK16(SRC) * SRCA * BIMG(BLEND) ) >> 16 ) ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_UAC_BAD_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT )	\
		{\
			Src1 = (WORD)( RUP16( (DWORD)( RBOT16(SRC) * SRCA * BIMG(BLEND) * ABRIGHT ) >> 24 ) +\
						   GUP16( (DWORD)( GBOT16(SRC) * SRCA * BIMG(BLEND) * ABRIGHT ) >> 24 ) +\
						   BUP16( (DWORD)( BBOT16(SRC) * SRCA * BIMG(BLEND) * ABRIGHT ) >> 24 ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C16_NBR_UAC_BAD_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_NBR_NAC_BAD( SRC, DEST, DESTP, BLEND )\
				else				CODE_UBI_C16_NBR_UAC_BAD( SRC, SRCA, DEST, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_UAC_BAD_ACK_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_NBR_NAC_BAD_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )\
				else				CODE_UBI_C16_NBR_UAC_BAD_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_NBR_NAC_BAD( SRC, DEST, DESTP, BLEND )	\
		{\
			Src1 = (WORD)( RUP16( MT1( ( RMSK16(SRC) * BIMG(BLEND) ) >> 19 ) ) +\
						   GUP16( MT1( ( GMSK16(SRC) * BIMG(BLEND) ) >> 13 ) ) +\
						   BUP16( MT1( ( BMSK16(SRC) * BIMG(BLEND) ) >> 8  ) ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_NAC_BAD_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )	\
		{\
			Src1 = (WORD)( RUP16( ( RMSK16(SRC) * BIMG(BLEND) * ABRIGHT ) >> 27 ) +\
						   GUP16( ( GMSK16(SRC) * BIMG(BLEND) * ABRIGHT ) >> 21 ) +\
						   BUP16( ( BMSK16(SRC) * BIMG(BLEND) * ABRIGHT ) >> 16 ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_NBR_NAC_BAD_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_NBR_NAC_BAD( SRC, DEST, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_NAC_BAD_TCK_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )	\
			if( SRC != TransColor ) CODE_UBI_C16_NBR_NAC_BAD_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )

	  // �P�x����

		// ���`�����l������
		#define CODE_UBI_C16_UBR_UAC_BAD( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
			Src1 = (WORD)( RUP16( MT1( ( RTABLE( RBOT16(SRC) ) * BIMG(BLEND) * SRCA ) >> 16 ) ) +\
						   GUP16( MT1( ( GTABLE( GBOT16(SRC) ) * BIMG(BLEND) * SRCA ) >> 16 ) ) +\
						   BUP16( MT1( ( BTABLE( BBOT16(SRC) ) * BIMG(BLEND) * SRCA ) >> 16 ) ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BAD_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			Src1 = (WORD)( RUP16( ( ( ( RBOT16(SRC) * RBRIGHT ) >> 8 ) * BIMG(BLEND) * SRCA * ABRIGHT ) >> 24 ) +\
						   GUP16( ( ( ( GBOT16(SRC) * GBRIGHT ) >> 8 ) * BIMG(BLEND) * SRCA * ABRIGHT ) >> 24 ) +\
						   BUP16( ( ( ( BBOT16(SRC) * BBRIGHT ) >> 8 ) * BIMG(BLEND) * SRCA * ABRIGHT ) >> 24 ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_UAC_BAD_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BAD( SRC, DEST, DESTP, BLEND )\
				else				CODE_UBI_C16_UBR_UAC_BAD( SRC, SRCA, DEST, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BAD_ACK_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BAD_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else				CODE_UBI_C16_UBR_UAC_BAD_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_UBR_NAC_BAD( SRC, DEST, DESTP, BLEND )	\
		{\
			Src1 = (WORD)( RUP16( MT1( ( BIMG(BLEND) * RTABLE( RBOT16(SRC) ) ) >> 8 ) ) +\
						   GUP16( MT1( ( BIMG(BLEND) * GTABLE( GBOT16(SRC) ) ) >> 8 ) ) +\
						   BUP16( MT1( ( BIMG(BLEND) * BTABLE( BBOT16(SRC) ) ) >> 8 ) ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BAD_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			Src1 = (WORD)( RUP16( ( RBOT16(SRC) * RBRIGHT * ABRIGHT * BIMG(BLEND) ) >> 24 ) +\
						   GUP16( ( GBOT16(SRC) * GBRIGHT * ABRIGHT * BIMG(BLEND) ) >> 24 ) +\
						   BUP16( ( BBOT16(SRC) * BBRIGHT * ABRIGHT * BIMG(BLEND) ) >> 24 ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_NAC_BAD_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BAD( SRC, DEST, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BAD_TCK_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BAD_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

	// �u�����h�摜����
	
	  // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_NBR_UAC_BAD( SRC, SRCA, DEST, DESTP )	\
		{\
			Src1 = (WORD)(  RUP16( MT1( ( RMSK16(SRC) * SRCA ) >> 19 ) ) +\
							GUP16( MT1( ( GMSK16(SRC) * SRCA ) >> 13 ) ) +\
							BUP16( MT1( ( BMSK16(SRC) * SRCA ) >> 8  ) ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_UAC_BAD_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT )	\
		{\
			Src1 = (WORD)(  RUP16( ( RMSK16(SRC) * SRCA * ABRIGHT ) >> 27 ) +\
							GUP16( ( GMSK16(SRC) * SRCA * ABRIGHT ) >> 21 ) +\
							BUP16( ( BMSK16(SRC) * SRCA * ABRIGHT ) >> 16 ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_UAC_BAD_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_NBR_NAC_BAD( SRC, DEST, DESTP )\
				else				CODE_NBI_C16_NBR_UAC_BAD( SRC, SRCA, DEST, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_UAC_BAD_ACK_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_NBR_NAC_BAD_NTBL( SRC, DEST, DESTP, ABRIGHT )\
				else				CODE_NBI_C16_NBR_UAC_BAD_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_NBR_NAC_BAD( SRC, DEST, DESTP )	\
		{\
			Src1 = (WORD)(  RUP16( MT1( RBOT16(SRC) ) ) +\
							GUP16( MT1( GBOT16(SRC) ) ) +\
							BUP16( MT1( BBOT16(SRC) ) ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_NAC_BAD_NTBL( SRC, DEST, DESTP, ABRIGHT )	\
		{\
			Src1 = (WORD)(  RUP16( ( RMSK16(SRC) * ABRIGHT ) >> ( 8 + MEMIMG_R5G6B5_LR ) ) +\
							GUP16( ( GMSK16(SRC) * ABRIGHT ) >> ( 8 + MEMIMG_R5G6B5_LG ) ) +\
							BUP16( ( BMSK16(SRC) * ABRIGHT ) >> ( 8 + MEMIMG_R5G6B5_LB ) ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_NAC_BAD_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_NBR_NAC_BAD( SRC, DEST, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_NAC_BAD_TCK_NTBL( SRC, DEST, DESTP, ABRIGHT )	\
			if( SRC != TransColor ) CODE_NBI_C16_NBR_NAC_BAD_NTBL( SRC, DEST, DESTP, ABRIGHT )

	  // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_UBR_UAC_BAD( SRC, SRCA, DEST, DESTP )	\
		{\
			Src1 = (WORD)( RUP16( MT1( RTABLE( ( RMSK16(SRC) * SRCA ) >> 19 ) ) ) +\
						   GUP16( MT1( GTABLE( ( GMSK16(SRC) * SRCA ) >> 13 ) ) ) +\
						   BUP16( MT1( BTABLE( ( BMSK16(SRC) * SRCA ) >> 8  ) ) ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BAD_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			Src1 = (WORD)( RUP16( ( RBOT16(SRC) * SRCA * ABRIGHT * RBRIGHT ) >> 24 ) +\
						   GUP16( ( GBOT16(SRC) * SRCA * ABRIGHT * GBRIGHT ) >> 24 ) +\
						   BUP16( ( BBOT16(SRC) * SRCA * ABRIGHT * BBRIGHT ) >> 24 ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_UAC_BAD_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BAD( SRC, DEST, DESTP )\
				else				CODE_NBI_C16_UBR_UAC_BAD( SRC, SRCA, DEST, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BAD_ACK_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BAD_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else				CODE_NBI_C16_UBR_UAC_BAD_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_UBR_NAC_BAD( SRC, DEST, DESTP )	\
		{\
			Src1 = (WORD)( RUP16( MT1( RTABLE( RBOT16(SRC) ) ) ) +\
						   GUP16( MT1( GTABLE( GBOT16(SRC) ) ) ) +\
						   BUP16( MT1( BTABLE( BBOT16(SRC) ) ) ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BAD_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			Src1 = (WORD)( RUP16( ( RMSK16(SRC) * ABRIGHT * RBRIGHT ) >> ( 16 + MEMIMG_R5G6B5_LR ) ) +\
						   GUP16( ( GMSK16(SRC) * ABRIGHT * GBRIGHT ) >> ( 16 + MEMIMG_R5G6B5_LG ) ) +\
						   BUP16( ( BMSK16(SRC) * ABRIGHT * BBRIGHT ) >> ( 16 + MEMIMG_R5G6B5_LB ) ) ) ;\
			Data = ( Src1 & MEMIMG_R5G6B5_ADDMASK1 ) + ( (DEST) & MEMIMG_R5G6B5_ADDMASK1 ) ;\
			DataD = Data & MEMIMG_R5G6B5_ADDMASK2 ;\
			*(DESTP) = (WORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_R5G6B5_ADDSHFT ) ) ) ) | ( ( Src1 | (DEST) ) & MEMIMG_R5G6B5_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_NAC_BAD_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BAD( SRC, DEST, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BAD_TCK_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT  )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BAD_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

  // ���Z�u�����h

    // �u�����h�摜����
 
      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C16_NBR_UAC_BSB( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
			Src1 = (WORD)( RUP16( MT1( (DWORD)( RMSK16(SRC) * BIMG(BLEND) * SRCA ) >> 27 ) ) +\
						   GUP16( MT1( (DWORD)( GMSK16(SRC) * BIMG(BLEND) * SRCA ) >> 21 ) ) +\
						   BUP16( MT1( (DWORD)( BMSK16(SRC) * BIMG(BLEND) * SRCA ) >> 16 ) ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_UAC_BSB_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT )	\
		{\
			Src1 = (WORD)( RUP16( (DWORD)( RBOT16(SRC) * BIMG(BLEND) * SRCA * ABRIGHT ) >> 24 ) +\
						   GUP16( (DWORD)( GBOT16(SRC) * BIMG(BLEND) * SRCA * ABRIGHT ) >> 24 ) +\
						   BUP16( (DWORD)( BBOT16(SRC) * BIMG(BLEND) * SRCA * ABRIGHT ) >> 24 ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C16_NBR_UAC_BSB_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_NBR_NAC_BSB( SRC, DEST, DESTP, BLEND )\
				else				CODE_UBI_C16_NBR_UAC_BSB( SRC, SRCA, DEST, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_UAC_BSB_ACK_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_NBR_NAC_BSB_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )\
				else				CODE_UBI_C16_NBR_UAC_BSB_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_NBR_NAC_BSB( SRC, DEST, DESTP, BLEND )	\
		{\
			Src1 = (WORD)( RUP16( MT1( ( RMSK16(SRC) * BIMG(BLEND) ) >> ( 8 + MEMIMG_R5G6B5_LR ) ) ) +\
						   GUP16( MT1( ( GMSK16(SRC) * BIMG(BLEND) ) >> ( 8 + MEMIMG_R5G6B5_LG ) ) ) +\
						   BUP16( MT1( ( BMSK16(SRC) * BIMG(BLEND) ) >> ( 8 + MEMIMG_R5G6B5_LB ) ) ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_NAC_BSB_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )	\
		{\
			Src1 = (WORD)( RUP16( ( RMSK16(SRC) * BIMG(BLEND) * ABRIGHT ) >> ( 16 + MEMIMG_R5G6B5_LR ) ) +\
						   GUP16( ( GMSK16(SRC) * BIMG(BLEND) * ABRIGHT ) >> ( 16 + MEMIMG_R5G6B5_LG ) ) +\
						   BUP16( ( BMSK16(SRC) * BIMG(BLEND) * ABRIGHT ) >> ( 16 + MEMIMG_R5G6B5_LB ) ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_NBR_NAC_BSB_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_NBR_NAC_BSB( SRC, DEST, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_NAC_BSB_TCK_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )	\
			if( SRC != TransColor ) CODE_UBI_C16_NBR_NAC_BSB_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )

      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C16_UBR_UAC_BSB( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
			Src1 = (WORD)( RUP16( MT1( ( RTABLE( RBOT16(SRC) ) * BIMG(BLEND) * SRCA ) >> 16 ) ) +\
						   GUP16( MT1( ( GTABLE( GBOT16(SRC) ) * BIMG(BLEND) * SRCA ) >> 16 ) ) +\
						   BUP16( MT1( ( BTABLE( BBOT16(SRC) ) * BIMG(BLEND) * SRCA ) >> 16 ) ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BSB_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			Src1 = (WORD)( RUP16( ( ( ( RBOT16(SRC) * RBRIGHT ) >> 8 ) * BIMG(BLEND) * SRCA * ABRIGHT ) >> 24 ) +\
						   GUP16( ( ( ( GBOT16(SRC) * GBRIGHT ) >> 8 ) * BIMG(BLEND) * SRCA * ABRIGHT ) >> 24 ) +\
						   BUP16( ( ( ( BBOT16(SRC) * BBRIGHT ) >> 8 ) * BIMG(BLEND) * SRCA * ABRIGHT ) >> 24 ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_UAC_BSB_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BSB( SRC, DEST, DESTP, BLEND )\
				else				CODE_UBI_C16_UBR_UAC_BSB( SRC, SRCA, DEST, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BSB_ACK_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BSB_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else				CODE_UBI_C16_UBR_UAC_BSB_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_UBR_NAC_BSB( SRC, DEST, DESTP, BLEND )	\
		{\
			Src1 = (WORD)( RUP16( MT1( ( RTABLE( RBOT16(SRC) ) * BIMG(BLEND) ) >> 8 ) ) +\
						   GUP16( MT1( ( GTABLE( GBOT16(SRC) ) * BIMG(BLEND) ) >> 8 ) ) +\
						   BUP16( MT1( ( BTABLE( BBOT16(SRC) ) * BIMG(BLEND) ) >> 8 ) ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BSB_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			Src1 = (WORD)( RUP16( ( RBOT16(SRC) * BIMG(BLEND) * ABRIGHT * RBRIGHT ) >> 24 ) +\
						   GUP16( ( GBOT16(SRC) * BIMG(BLEND) * ABRIGHT * GBRIGHT ) >> 24 ) +\
						   BUP16( ( BBOT16(SRC) * BIMG(BLEND) * ABRIGHT * BBRIGHT ) >> 24 ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_NAC_BSB_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BSB( SRC, DEST, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BSB_TCK_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BSB_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

    // �u�����h�摜����

	  // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_NBR_UAC_BSB( SRC, SRCA, DEST, DESTP )	\
		{\
		    Src1 = (WORD)(  RUP16( MT1( ( RMSK16(SRC) * SRCA ) >> ( 8 + MEMIMG_R5G6B5_LR ) ) ) +\
							GUP16( MT1( ( GMSK16(SRC) * SRCA ) >> ( 8 + MEMIMG_R5G6B5_LG ) ) ) +\
							BUP16( MT1( ( BMSK16(SRC) * SRCA ) >> ( 8 + MEMIMG_R5G6B5_LB ) ) ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_UAC_BSB_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT )	\
		{\
		    Src1 = (WORD)(  RUP16( ( RMSK16(SRC) * SRCA * ABRIGHT ) >> ( 16 + MEMIMG_R5G6B5_LR ) ) +\
							GUP16( ( GMSK16(SRC) * SRCA * ABRIGHT ) >> ( 16 + MEMIMG_R5G6B5_LG ) ) +\
							BUP16( ( BMSK16(SRC) * SRCA * ABRIGHT ) >> ( 16 + MEMIMG_R5G6B5_LB ) ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_UAC_BSB_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_NBR_NAC_BSB( SRC, DEST, DESTP )\
				else				CODE_NBI_C16_NBR_UAC_BSB( SRC, SRCA, DEST, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_UAC_BSB_ACK_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_NBR_NAC_BSB_NTBL( SRC, DEST, DESTP, ABRIGHT )\
				else				CODE_NBI_C16_NBR_UAC_BSB_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_NBR_NAC_BSB( SRC, DEST, DESTP )	\
		{\
		    Src1 = (WORD)(  RUP16( MT1( RBOT16(SRC) ) ) +\
							GUP16( MT1( GBOT16(SRC) ) ) +\
							BUP16( MT1( BBOT16(SRC) ) ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_NAC_BSB_NTBL( SRC, DEST, DESTP, ABRIGHT )	\
		{\
		    Src1 = (WORD)(  RUP16( ( RMSK16(SRC) * ABRIGHT ) >> ( 8 + MEMIMG_R5G6B5_LR ) ) +\
							GUP16( ( GMSK16(SRC) * ABRIGHT ) >> ( 8 + MEMIMG_R5G6B5_LG ) ) +\
							BUP16( ( BMSK16(SRC) * ABRIGHT ) >> ( 8 + MEMIMG_R5G6B5_LB ) ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_NAC_BSB_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_NBR_NAC_BSB( SRC, DEST, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_NAC_BSB_TCK_NTBL( SRC, DEST, DESTP, ABRIGHT )	\
			if( SRC != TransColor ) CODE_NBI_C16_NBR_NAC_BSB_NTBL( SRC, DEST, DESTP, ABRIGHT )

	  // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_UBR_UAC_BSB( SRC, SRCA, DEST, DESTP )	\
		{\
			Src1 = (WORD)( RUP16( MT1( RTABLE( ( RMSK16(SRC) * SRCA ) >> 19 ) ) ) +\
						   GUP16( MT1( GTABLE( ( GMSK16(SRC) * SRCA ) >> 13 ) ) ) +\
						   BUP16( MT1( BTABLE( ( BMSK16(SRC) * SRCA ) >> 8  ) ) ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BSB_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			Src1 = (WORD)( RUP16( ( RBOT16(SRC) * SRCA * ABRIGHT * RBRIGHT ) >> 24 ) +\
						   GUP16( ( GBOT16(SRC) * SRCA * ABRIGHT * GBRIGHT ) >> 24 ) +\
						   BUP16( ( BBOT16(SRC) * SRCA * ABRIGHT * BBRIGHT ) >> 24 ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_UAC_BSB_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BSB( SRC, DEST, DESTP )\
				else				CODE_NBI_C16_UBR_UAC_BSB( SRC, SRCA, DEST, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BSB_ACK_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BSB_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else				CODE_NBI_C16_UBR_UAC_BSB_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_UBR_NAC_BSB( SRC, DEST, DESTP )	\
		{\
			Src1 = (WORD)( RUP16( MT1( RTABLE( RBOT16(SRC) ) ) ) +\
						   GUP16( MT1( GTABLE( GBOT16(SRC) ) ) ) +\
						   BUP16( MT1( BTABLE( BBOT16(SRC) ) ) ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BSB_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			Src1 = (WORD)( RUP16( ( RMSK16(SRC) * ABRIGHT * RBRIGHT ) >> ( 16 + MEMIMG_R5G6B5_LR ) ) +\
						   GUP16( ( GMSK16(SRC) * ABRIGHT * GBRIGHT ) >> ( 16 + MEMIMG_R5G6B5_LG ) ) +\
						   BUP16( ( BMSK16(SRC) * ABRIGHT * BBRIGHT ) >> ( 16 + MEMIMG_R5G6B5_LB ) ) ) ;\
			Data = ( (DEST) | MEMIMG_R5G6B5_SUBMASK2 ) - ( Src1 & ~MEMIMG_R5G6B5_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_R5G6B5_SUBMASK2 ;\
			*(DESTP) = (WORD)( Data & ( DataD - ( DataD >> MEMIMG_R5G6B5_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_NAC_BSB_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BSB( SRC, DEST, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BSB_TCK_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BSB_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )


  // ��Z�u�����h

    // �u�����h�摜����
 
      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C16_NBR_UAC_BML( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
		    *(DESTP) = (WORD)( RMSK16( ( RBOT16(SRC) * BIMG(BLEND) * SRCA * RBOT16(DEST) ) >> 10  ) |\
							   GMSK16( ( GBOT16(SRC) * BIMG(BLEND) * SRCA * GBOT16(DEST) ) >> 17  ) |\
							         ( ( BBOT16(SRC) * BIMG(BLEND) * SRCA * BBOT16(DEST) ) >> 21 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C16_NBR_UAC_BML_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA == 255 ) 	CODE_UBI_C16_NBR_NAC_BML( SRC, DEST, DESTP, BLEND )\
			else				CODE_UBI_C16_NBR_UAC_BML( SRC, SRCA, DEST, DESTP, BLEND )

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_NBR_NAC_BML( SRC, DEST, DESTP, BLEND )	\
		{\
		    *(DESTP) = (WORD)( RMSK16( ( RBOT16(SRC) * BIMG(BLEND) * RBOT16(DEST) ) >> 2  ) |\
							   GMSK16( ( GBOT16(SRC) * BIMG(BLEND) * GBOT16(DEST) ) >> 9  ) |\
							         ( ( BBOT16(SRC) * BIMG(BLEND) * BBOT16(DEST) ) >> 13 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_NBR_NAC_BML_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_NBR_NAC_BML( SRC, DEST, DESTP, BLEND )

      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C16_UBR_UAC_BML( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
		    *(DESTP) = (WORD)( RMSK16( ( RTABLE( RBOT16(SRC) ) * SRCA * BIMG(BLEND) * RBOT16(DEST) ) >> 10  ) |\
							   GMSK16( ( GTABLE( GBOT16(SRC) ) * SRCA * BIMG(BLEND) * GBOT16(DEST) ) >> 17  ) |\
							         ( ( BTABLE( BBOT16(SRC) ) * SRCA * BIMG(BLEND) * BBOT16(DEST) ) >> 21 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BML_NTBL( SRC, SRCA, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
		    *(DESTP) = (WORD)( RMSK16( ( ( ( RBOT16(SRC) * RBRIGHT ) >> 8 ) * SRCA * BIMG(BLEND) * RBOT16(DEST) ) >> 10  ) |\
							   GMSK16( ( ( ( GBOT16(SRC) * GBRIGHT ) >> 8 ) * SRCA * BIMG(BLEND) * GBOT16(DEST) ) >> 17  ) |\
							         ( ( ( ( BBOT16(SRC) * BBRIGHT ) >> 8 ) * SRCA * BIMG(BLEND) * BBOT16(DEST) ) >> 21 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_UAC_BML_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BML( SRC, DEST, DESTP, BLEND )\
			else				CODE_UBI_C16_UBR_UAC_BML( SRC, SRCA, DEST, DESTP, BLEND )

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BML_ACK_NTBL( SRC, SRCA, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BML_NTBL( SRC, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )\
			else				CODE_UBI_C16_UBR_UAC_BML_NTBL( SRC, SRCA, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_UBR_NAC_BML( SRC, DEST, DESTP, BLEND )	\
		{\
		    *(DESTP) = (WORD)( RMSK16( ( RTABLE( RBOT16(SRC) ) * BIMG(BLEND) * RBOT16(DEST) ) >> 2  ) |\
							   GMSK16( ( GTABLE( GBOT16(SRC) ) * BIMG(BLEND) * GBOT16(DEST) ) >> 9  ) |\
							         ( ( BTABLE( BBOT16(SRC) ) * BIMG(BLEND) * BBOT16(DEST) ) >> 13 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BML_NTBL( SRC, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
		    *(DESTP) = (WORD)( RMSK16( ( RBOT16(SRC) * RBRIGHT * BIMG(BLEND) * RBOT16(DEST) ) >> 10  ) |\
							   GMSK16( ( GBOT16(SRC) * GBRIGHT * BIMG(BLEND) * GBOT16(DEST) ) >> 17  ) |\
							         ( ( BBOT16(SRC) * BBRIGHT * BIMG(BLEND) * BBOT16(DEST) ) >> 21 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_NAC_BML_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BML( SRC, DEST, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BML_TCK_NTBL( SRC, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BML_NTBL( SRC, DEST, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )

    // �u�����h�摜����

      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_NBR_UAC_BML( SRC, SRCA, DEST, DESTP )	\
		{\
		    *(DESTP) = (WORD)( RMSK16( ( RBOT16(SRC) * SRCA * RBOT16(DEST) ) >> 2  ) |\
							   GMSK16( ( GBOT16(SRC) * SRCA * GBOT16(DEST) ) >> 9  ) |\
							         ( ( BBOT16(SRC) * SRCA * BBOT16(DEST) ) >> 13 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_UAC_BML_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA == 255 ) 	CODE_NBI_C16_NBR_NAC_BML( SRC, DEST, DESTP )\
			else				CODE_NBI_C16_NBR_UAC_BML( SRC, SRCA, DEST, DESTP )

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_NBR_NAC_BML( SRC, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( RMSK16(SRC) * RMSK16(DEST) ) >> 16 ) |\
							   GMSK16( ( GMSK16(SRC) * GMSK16(DEST) ) >> 11 ) |\
							         ( ( BMSK16(SRC) * BMSK16(DEST) ) >> 5  ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_NAC_BML_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_NBR_NAC_BML( SRC, DEST, DESTP )

      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_UBR_UAC_BML( SRC, SRCA, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( RTABLE( RBOT16(SRC) ) * SRCA * RBOT16(DEST) ) >> 2 ) |\
							   GMSK16( ( GTABLE( GBOT16(SRC) ) * SRCA * GBOT16(DEST) ) >> 9 ) |\
							         ( ( BTABLE( BBOT16(SRC) ) * SRCA * BBOT16(DEST) ) >> 13 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BML_NTBL( SRC, SRCA, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( RBOT16(SRC) * RBRIGHT * SRCA * RBOT16(DEST) ) >> 10 ) |\
							   GMSK16( ( GBOT16(SRC) * GBRIGHT * SRCA * GBOT16(DEST) ) >> 17 ) |\
							         ( ( BBOT16(SRC) * BBRIGHT * SRCA * BBOT16(DEST) ) >> 21 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_UAC_BML_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BML( SRC, DEST, DESTP )\
			else				CODE_NBI_C16_UBR_UAC_BML( SRC, SRCA, DEST, DESTP )

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BML_ACK_NTBL( SRC, SRCA, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BML_NTBL( SRC, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )\
			else				CODE_NBI_C16_UBR_UAC_BML_NTBL( SRC, SRCA, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_UBR_NAC_BML( SRC, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( RTABLE( RBOT16(SRC) ) * RBOT16(DEST) ) << 6 ) |\
							   GMSK16( ( GTABLE( GBOT16(SRC) ) * GBOT16(DEST) ) >> 1 ) |\
							         ( ( BTABLE( BBOT16(SRC) ) * BBOT16(DEST) ) >> 5 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BML_NTBL( SRC, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( RBOT16(SRC) * RBRIGHT * RBOT16(DEST) ) >> 2 ) |\
							   GMSK16( ( GBOT16(SRC) * GBRIGHT * GBOT16(DEST) ) >> 9 ) |\
							         ( ( BBOT16(SRC) * BBRIGHT * BBOT16(DEST) ) >> 13 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_NAC_BML_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BML( SRC, DEST, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BML_TCK_NTBL( SRC, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BML_NTBL( SRC, DEST, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )

  // ��Z�{���u�����h

    // �u�����h�摜����
 
      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C16_NBR_UAC_BMA( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( ( RBOT16(SRC) * RBOT16(DEST) ) >> 5 ) - RBOT16(DEST) ) * BIMG(BLEND) * SRCA * BPARAM + ( RBOT16(DEST) << 24 ) ) >> 13 ) ) |\
							   GMSK16( ( ( ( ( ( GBOT16(SRC) * GBOT16(DEST) ) >> 6 ) - GBOT16(DEST) ) * BIMG(BLEND) * SRCA * BPARAM + ( GBOT16(DEST) << 24 ) ) >> 19 ) ) |\
							         ( ( ( ( ( ( BBOT16(SRC) * BBOT16(DEST) ) >> 5 ) - BBOT16(DEST) ) * BIMG(BLEND) * SRCA * BPARAM + ( BBOT16(DEST) << 24 ) ) >> 24 ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C16_NBR_UAC_BMA_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA == 255 ) 	CODE_UBI_C16_NBR_NAC_BMA( SRC, DEST, DESTP, BLEND )\
			else				CODE_UBI_C16_NBR_UAC_BMA( SRC, SRCA, DEST, DESTP, BLEND )

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_NBR_NAC_BMA( SRC, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( ( RBOT16(SRC) * RBOT16(DEST) ) >> 5 ) - RBOT16(DEST) ) * BIMG(BLEND) * BPARAM + ( RBOT16(DEST) << 16 ) ) >> 5 ) ) |\
							   GMSK16( ( ( ( ( ( GBOT16(SRC) * GBOT16(DEST) ) >> 6 ) - GBOT16(DEST) ) * BIMG(BLEND) * BPARAM + ( GBOT16(DEST) << 16 ) ) >> 11 ) ) |\
							         ( ( ( ( ( ( BBOT16(SRC) * BBOT16(DEST) ) >> 5 ) - BBOT16(DEST) ) * BIMG(BLEND) * BPARAM + ( BBOT16(DEST) << 16 ) ) >> 16 ) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_NBR_NAC_BMA_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_NBR_NAC_BMA( SRC, DEST, DESTP, BLEND )

      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C16_UBR_UAC_BMA( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RTABLE( RBOT16(SRC) ) * RBOT16(DEST) ) >> 5 ) - RBOT16(DEST) ) * BIMG(BLEND) * BPARAM * SRCA + ( RMSK16(DEST) << 13 ) ) >> 13 ) |\
							   GMSK16( ( ( ( ( GTABLE( GBOT16(SRC) ) * GBOT16(DEST) ) >> 6 ) - GBOT16(DEST) ) * BIMG(BLEND) * BPARAM * SRCA + ( GMSK16(DEST) << 19 ) ) >> 19 ) |\
							   BMSK16( ( ( ( ( BTABLE( BBOT16(SRC) ) * BBOT16(DEST) ) >> 5 ) - BBOT16(DEST) ) * BIMG(BLEND) * BPARAM * SRCA + ( BMSK16(DEST) << 24 ) ) >> 24 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BMA_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RBOT16(SRC) * RBRIGHT * RBOT16(DEST) ) >> 13 ) - RBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT * SRCA + ( RMSK16(DEST) << 13 ) ) >> 13 ) |\
							   GMSK16( ( ( ( ( GBOT16(SRC) * GBRIGHT * GBOT16(DEST) ) >> 14 ) - GBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT * SRCA + ( GMSK16(DEST) << 19 ) ) >> 19 ) |\
							   BMSK16( ( ( ( ( BBOT16(SRC) * BBRIGHT * BBOT16(DEST) ) >> 13 ) - BBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT * SRCA + ( BMSK16(DEST) << 24 ) ) >> 24 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_UAC_BMA_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BMA( SRC, DEST, DESTP, BLEND )\
			else				CODE_UBI_C16_UBR_UAC_BMA( SRC, SRCA, DEST, DESTP, BLEND )

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BMA_ACK_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BMA_NTBL( SRC,       DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			else				CODE_UBI_C16_UBR_UAC_BMA_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_UBR_NAC_BMA( SRC, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RTABLE( RBOT16(SRC) ) * RBOT16(DEST) ) >> 5 ) - RBOT16(DEST) ) * BIMG(BLEND) * BPARAM + ( RMSK16(DEST) << 5  ) ) >> 5  ) |\
							   GMSK16( ( ( ( ( GTABLE( GBOT16(SRC) ) * GBOT16(DEST) ) >> 6 ) - GBOT16(DEST) ) * BIMG(BLEND) * BPARAM + ( GMSK16(DEST) << 11 ) ) >> 11 ) |\
							   BMSK16( ( ( ( ( BTABLE( BBOT16(SRC) ) * BBOT16(DEST) ) >> 5 ) - BBOT16(DEST) ) * BIMG(BLEND) * BPARAM + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BMA_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RBOT16(SRC) * RBRIGHT * RBOT16(DEST) ) >> 13 ) - RBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( RMSK16(DEST) << 5 ) ) >> 5 ) |\
							   GMSK16( ( ( ( ( GBOT16(SRC) * GBRIGHT * GBOT16(DEST) ) >> 14 ) - GBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( GMSK16(DEST) << 11 ) ) >> 11 ) |\
							   BMSK16( ( ( ( ( BBOT16(SRC) * BBRIGHT * BBOT16(DEST) ) >> 13 ) - BBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_NAC_BMA_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BMA( SRC, DEST, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BMA_TCK_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BMA_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

    // �u�����h�摜����

      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_NBR_UAC_BMA( SRC, SRCA, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( ( RMSK16(SRC) * RMSK16(DEST) ) >> 27 ) - RBOT16(DEST) ) * SRCA * BPARAM + ( RBOT16(DEST) << 16 ) ) >> 5  ) ) |\
							   GMSK16( ( ( ( ( ( GMSK16(SRC) * GMSK16(DEST) ) >> 11 ) - GMSK16(DEST) ) * SRCA * BPARAM + ( GMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							         ( ( ( ( ( ( BMSK16(SRC) * BMSK16(DEST) ) >> 5  ) - BMSK16(DEST) ) * SRCA * BPARAM + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_UAC_BMA_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA == 255 ) 	CODE_NBI_C16_NBR_NAC_BMA( SRC, DEST, DESTP )\
			else				CODE_NBI_C16_NBR_UAC_BMA( SRC, SRCA, DEST, DESTP )

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_NBR_NAC_BMA( SRC, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RUP16( MT2( (int)( ( RBOT16(SRC) * RBOT16(DEST) ) >> 5 ) - (int)RBOT16(DEST) ) + RBOT16(DEST) ) |\
							   GUP16( MT2( (int)( ( GBOT16(SRC) * GBOT16(DEST) ) >> 6 ) - (int)GBOT16(DEST) ) + GBOT16(DEST) ) |\
							   BUP16( MT2( (int)( ( BBOT16(SRC) * BBOT16(DEST) ) >> 5 ) - (int)BBOT16(DEST) ) + BBOT16(DEST) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_NAC_BMA_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_NBR_NAC_BMA( SRC, DEST, DESTP )

      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_UBR_UAC_BMA( SRC, SRCA, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RTABLE( RBOT16(SRC) ) * RBOT16(DEST) ) >> 5 ) - RBOT16(DEST) ) * BPARAM * SRCA + ( RMSK16(DEST) << 5  ) ) >> 5  ) |\
							   GMSK16( ( ( ( ( GTABLE( GBOT16(SRC) ) * GBOT16(DEST) ) >> 6 ) - GBOT16(DEST) ) * BPARAM * SRCA + ( GMSK16(DEST) << 11 ) ) >> 11 ) |\
							   BMSK16( ( ( ( ( BTABLE( BBOT16(SRC) ) * BBOT16(DEST) ) >> 5 ) - BBOT16(DEST) ) * BPARAM * SRCA + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BMA_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RBOT16(SRC) * RBRIGHT * RBOT16(DEST) ) >> 13 ) - RBOT16(DEST) ) * ABRIGHT * SRCA + ( RMSK16(DEST) << 5  ) ) >> 5  ) |\
							   GMSK16( ( ( ( ( GBOT16(SRC) * GBRIGHT * GBOT16(DEST) ) >> 14 ) - GBOT16(DEST) ) * ABRIGHT * SRCA + ( GMSK16(DEST) << 11 ) ) >> 11 ) |\
							   BMSK16( ( ( ( ( BBOT16(SRC) * BBRIGHT * BBOT16(DEST) ) >> 13 ) - BBOT16(DEST) ) * ABRIGHT * SRCA + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_UAC_BMA_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BMA( SRC, DEST, DESTP )\
			else				CODE_NBI_C16_UBR_UAC_BMA( SRC, SRCA, DEST, DESTP )

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BMA_ACK_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BMA_NTBL( SRC,       DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			else				CODE_NBI_C16_UBR_UAC_BMA_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_UBR_NAC_BMA( SRC, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RUP16( MT2( (int)( ( RTABLE( RBOT16(SRC) ) * RBOT16(DEST) ) >> 5 ) - (int)RBOT16(DEST) ) + RBOT16(DEST) ) |\
							   GUP16( MT2( (int)( ( GTABLE( GBOT16(SRC) ) * GBOT16(DEST) ) >> 6 ) - (int)GBOT16(DEST) ) + GBOT16(DEST) ) |\
							   BUP16( MT2( (int)( ( BTABLE( BBOT16(SRC) ) * BBOT16(DEST) ) >> 5 ) - (int)BBOT16(DEST) ) + BBOT16(DEST) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BMA_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RUP16( ( ( ( (int)( ( RBOT16(SRC) * RBRIGHT * RBOT16(DEST) ) >> 13 ) * ABRIGHT ) >> 8 ) - (int)RBOT16(DEST) ) + RBOT16(DEST) ) |\
							   GUP16( ( ( ( (int)( ( GBOT16(SRC) * GBRIGHT * GBOT16(DEST) ) >> 14 ) * ABRIGHT ) >> 8 ) - (int)GBOT16(DEST) ) + GBOT16(DEST) ) |\
							   BUP16( ( ( ( (int)( ( BBOT16(SRC) * BBRIGHT * BBOT16(DEST) ) >> 13 ) * ABRIGHT ) >> 8 ) - (int)BBOT16(DEST) ) + BBOT16(DEST) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_NAC_BMA_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BMA( SRC, DEST, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BMA_TCK_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BMA_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )


  // �`�挳�̐F�𔽓]�{���u�����h

    // �u�����h�摜����
 
      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C16_NBR_UAC_BIS( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( RBOT16((WORD)~(SRC)) - RBOT16(DEST) ) * BIMG(BLEND) * SRCA * BPARAM + ( RMSK16(DEST) << 13 ) ) >> 13 ) ) |\
							   GMSK16( ( ( ( GBOT16((WORD)~(SRC)) - GBOT16(DEST) ) * BIMG(BLEND) * SRCA * BPARAM + ( GMSK16(DEST) << 19 ) ) >> 19 ) ) |\
							         ( ( ( ( BBOT16((WORD)~(SRC)) - BBOT16(DEST) ) * BIMG(BLEND) * SRCA * BPARAM + ( BMSK16(DEST) << 24 ) ) >> 24 ) ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_UAC_BIS_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( RBOT16((WORD)~(SRC)) - RBOT16(DEST) ) * BIMG(BLEND) * SRCA * ABRIGHT + ( RMSK16(DEST) << 13 ) ) >> 13 ) ) |\
							   GMSK16( ( ( ( GBOT16((WORD)~(SRC)) - GBOT16(DEST) ) * BIMG(BLEND) * SRCA * ABRIGHT + ( GMSK16(DEST) << 19 ) ) >> 19 ) ) |\
							         ( ( ( ( BBOT16((WORD)~(SRC)) - BBOT16(DEST) ) * BIMG(BLEND) * SRCA * ABRIGHT + ( BMSK16(DEST) << 24 ) ) >> 24 ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C16_NBR_UAC_BIS_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_NBR_NAC_BIS( SRC, DEST, DESTP, BLEND )\
				else				CODE_UBI_C16_NBR_UAC_BIS( SRC, SRCA, DEST, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_UAC_BIS_ACK_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_NBR_NAC_BIS_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )\
				else				CODE_UBI_C16_NBR_UAC_BIS_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_NBR_NAC_BIS( SRC, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( RMSK16((WORD)~(SRC)) - RMSK16(DEST) ) * BIMG(BLEND) * BPARAM + ( RMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							   GMSK16( ( ( ( GMSK16((WORD)~(SRC)) - GMSK16(DEST) ) * BIMG(BLEND) * BPARAM + ( GMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							         ( ( ( ( BMSK16((WORD)~(SRC)) - BMSK16(DEST) ) * BIMG(BLEND) * BPARAM + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_NAC_BIS_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( RMSK16((WORD)~(SRC)) - RMSK16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( RMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							   GMSK16( ( ( ( GMSK16((WORD)~(SRC)) - GMSK16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( GMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							         ( ( ( ( BMSK16((WORD)~(SRC)) - BMSK16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_NBR_NAC_BIS_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_NBR_NAC_BIS( SRC, DEST, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_NBR_NAC_BIS_TCK_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )	\
			if( SRC != TransColor ) CODE_UBI_C16_NBR_NAC_BIS_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT )


      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C16_UBR_UAC_BIS( SRC, SRCA, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( RTABLE( RBOT16((WORD)~(SRC)) ) - RBOT16(DEST) ) * BIMG(BLEND) * BPARAM * SRCA + ( RMSK16(DEST) << 13 ) ) >> 13 ) |\
							   GMSK16( ( ( GTABLE( GBOT16((WORD)~(SRC)) ) - GBOT16(DEST) ) * BIMG(BLEND) * BPARAM * SRCA + ( GMSK16(DEST) << 19 ) ) >> 19 ) |\
							   BMSK16( ( ( BTABLE( BBOT16((WORD)~(SRC)) ) - BBOT16(DEST) ) * BIMG(BLEND) * BPARAM * SRCA + ( BMSK16(DEST) << 24 ) ) >> 24 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BIS_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RBOT16((WORD)~(SRC)) * RBRIGHT ) >> 8 ) - RBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT * SRCA + ( RMSK16(DEST) << 13 ) ) >> 13 ) |\
							   GMSK16( ( ( ( ( GBOT16((WORD)~(SRC)) * GBRIGHT ) >> 8 ) - GBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT * SRCA + ( GMSK16(DEST) << 19 ) ) >> 19 ) |\
							   BMSK16( ( ( ( ( BBOT16((WORD)~(SRC)) * BBRIGHT ) >> 8 ) - BBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT * SRCA + ( BMSK16(DEST) << 24 ) ) >> 24 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_UAC_BIS_ACK( SRC, SRCA, DEST, DESTP, BLEND )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BIS( SRC, DEST, DESTP, BLEND )\
				else				CODE_UBI_C16_UBR_UAC_BIS( SRC, SRCA, DEST, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_UAC_BIS_ACK_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_UBI_C16_UBR_NAC_BIS_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else				CODE_UBI_C16_UBR_UAC_BIS_NTBL( SRC, SRCA, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C16_UBR_NAC_BIS( SRC, DEST, DESTP, BLEND )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( RTABLE( RBOT16((WORD)~(SRC)) ) - RBOT16(DEST) ) * BIMG(BLEND) * BPARAM + ( RMSK16(DEST) << 5 ) ) >> 5 ) |\
							   GMSK16( ( ( GTABLE( GBOT16((WORD)~(SRC)) ) - GBOT16(DEST) ) * BIMG(BLEND) * BPARAM + ( GMSK16(DEST) << 11 ) ) >> 11 ) |\
							   BMSK16( ( ( BTABLE( BBOT16((WORD)~(SRC)) ) - BBOT16(DEST) ) * BIMG(BLEND) * BPARAM + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BIS_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RBOT16((WORD)~(SRC)) * RBRIGHT ) >> 8 ) - RBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( RMSK16(DEST) << 5 ) ) >> 5 ) |\
							   GMSK16( ( ( ( ( GBOT16((WORD)~(SRC)) * GBRIGHT ) >> 8 ) - GBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( GMSK16(DEST) << 11 ) ) >> 11 ) |\
							   BMSK16( ( ( ( ( BBOT16((WORD)~(SRC)) * BBRIGHT ) >> 8 ) - BBOT16(DEST) ) * BIMG(BLEND) * ABRIGHT + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C16_UBR_NAC_BIS_TCK( SRC, DEST, DESTP, BLEND )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BIS( SRC, DEST, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C16_UBR_NAC_BIS_TCK_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRC != TransColor ) CODE_UBI_C16_UBR_NAC_BIS_NTBL( SRC, DEST, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

    // �u�����h�摜����

      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_NBR_UAC_BIS( SRC, SRCA, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( RMSK16((WORD)~(SRC)) - RMSK16(DEST) ) * SRCA * BPARAM + ( RMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							   GMSK16( ( ( ( GMSK16((WORD)~(SRC)) - GMSK16(DEST) ) * SRCA * BPARAM + ( GMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							         ( ( ( ( BMSK16((WORD)~(SRC)) - BMSK16(DEST) ) * SRCA * BPARAM + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_UAC_BIS_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( RMSK16((WORD)~(SRC)) - RMSK16(DEST) ) * SRCA * ABRIGHT + ( RMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							   GMSK16( ( ( ( GMSK16((WORD)~(SRC)) - GMSK16(DEST) ) * SRCA * ABRIGHT + ( GMSK16(DEST) << 16 ) ) >> 16 ) ) |\
							         ( ( ( ( BMSK16((WORD)~(SRC)) - BMSK16(DEST) ) * SRCA * ABRIGHT + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_UAC_BIS_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_NBR_NAC_BIS( SRC, DEST, DESTP )\
				else				CODE_NBI_C16_NBR_UAC_BIS( SRC, SRCA, DEST, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_UAC_BIS_ACK_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_NBR_NAC_BIS_NTBL( SRC, DEST, DESTP, ABRIGHT )\
				else				CODE_NBI_C16_NBR_UAC_BIS_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_NBR_NAC_BIS( SRC, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RUP16( MT2( (int)RBOT16((WORD)~(SRC)) - (int)RBOT16(DEST) ) + RBOT16(DEST) ) |\
							   GUP16( MT2( (int)GBOT16((WORD)~(SRC)) - (int)GBOT16(DEST) ) + GBOT16(DEST) ) |\
							   BUP16( MT2( (int)BBOT16((WORD)~(SRC)) - (int)BBOT16(DEST) ) + BBOT16(DEST) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_NAC_BIS_NTBL( SRC, DEST, DESTP, ABRIGHT )	\
		{\
			*(DESTP) = (WORD)( RUP16( ( ( ( (int)RBOT16((WORD)~(SRC)) - (int)RBOT16(DEST) ) * ABRIGHT ) >> 8 ) + RBOT16(DEST) ) |\
							   GUP16( ( ( ( (int)GBOT16((WORD)~(SRC)) - (int)GBOT16(DEST) ) * ABRIGHT ) >> 8 ) + GBOT16(DEST) ) |\
							   BUP16( ( ( ( (int)BBOT16((WORD)~(SRC)) - (int)BBOT16(DEST) ) * ABRIGHT ) >> 8 ) + BBOT16(DEST) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_NBR_NAC_BIS_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_NBR_NAC_BIS( SRC, DEST, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_NBR_NAC_BIS_TCK_NTBL( SRC, DEST, DESTP, ABRIGHT )	\
			if( SRC != TransColor ) CODE_NBI_C16_NBR_NAC_BIS_NTBL( SRC, DEST, DESTP, ABRIGHT )


      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C16_UBR_UAC_BIS( SRC, SRCA, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( RTABLE( RBOT16((WORD)~(SRC)) ) - RBOT16(DEST) ) * BPARAM * SRCA + ( RMSK16(DEST) << 5  ) ) >> 5  ) |\
							   GMSK16( ( ( GTABLE( GBOT16((WORD)~(SRC)) ) - GBOT16(DEST) ) * BPARAM * SRCA + ( GMSK16(DEST) << 11 ) ) >> 11 ) |\
							   BMSK16( ( ( BTABLE( BBOT16((WORD)~(SRC)) ) - BBOT16(DEST) ) * BPARAM * SRCA + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BIS_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RMSK16( ( ( ( ( RBOT16((WORD)~(SRC)) * RBRIGHT ) >> 8 ) - RBOT16(DEST) ) * ABRIGHT * SRCA + ( RMSK16(DEST) << 5  ) ) >> 5  ) |\
							   GMSK16( ( ( ( ( GBOT16((WORD)~(SRC)) * GBRIGHT ) >> 8 ) - GBOT16(DEST) ) * ABRIGHT * SRCA + ( GMSK16(DEST) << 11 ) ) >> 11 ) |\
							   BMSK16( ( ( ( ( BBOT16((WORD)~(SRC)) * BBRIGHT ) >> 8 ) - BBOT16(DEST) ) * ABRIGHT * SRCA + ( BMSK16(DEST) << 16 ) ) >> 16 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_UAC_BIS_ACK( SRC, SRCA, DEST, DESTP )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BIS( SRC, DEST, DESTP )\
				else				CODE_NBI_C16_UBR_UAC_BIS( SRC, SRCA, DEST, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_UAC_BIS_ACK_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRCA != 0 )\
			{\
				if( SRCA == 255 ) 	CODE_NBI_C16_UBR_NAC_BIS_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else				CODE_NBI_C16_UBR_UAC_BIS_NTBL( SRC, SRCA, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C16_UBR_NAC_BIS( SRC, DEST, DESTP )	\
		{\
			*(DESTP) = (WORD)( RUP16( MT2( (int)RTABLE( RBOT16((WORD)~(SRC)) ) - (int)RBOT16(DEST) ) + RBOT16(DEST) ) |\
							   GUP16( MT2( (int)GTABLE( GBOT16((WORD)~(SRC)) ) - (int)GBOT16(DEST) ) + GBOT16(DEST) ) |\
							   BUP16( MT2( (int)BTABLE( BBOT16((WORD)~(SRC)) ) - (int)BBOT16(DEST) ) + BBOT16(DEST) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BIS_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			*(DESTP) = (WORD)( RUP16( ( ( ( (int)( ( RBOT16((WORD)~(SRC)) * RBRIGHT ) >> 8 ) * ABRIGHT ) >> 8 ) - (int)RBOT16(DEST) ) + RBOT16(DEST) ) |\
							   GUP16( ( ( ( (int)( ( GBOT16((WORD)~(SRC)) * GBRIGHT ) >> 8 ) * ABRIGHT ) >> 8 ) - (int)GBOT16(DEST) ) + GBOT16(DEST) ) |\
							   BUP16( ( ( ( (int)( ( BBOT16((WORD)~(SRC)) * BBRIGHT ) >> 8 ) * ABRIGHT ) >> 8 ) - (int)BBOT16(DEST) ) + BBOT16(DEST) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C16_UBR_NAC_BIS_TCK( SRC, DEST, DESTP )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BIS( SRC, DEST, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C16_UBR_NAC_BIS_TCK_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( SRC != TransColor ) CODE_NBI_C16_UBR_NAC_BIS_NTBL( SRC, DEST, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )


// 32bit

  // �u�����h����

	// �u�����h�摜����

	  // �P�x����

		// ���`�����l������
		#define CODE_UBI_C32_NBR_UAC_BNO( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (SRCP)[0] - (DESTP)[0] ) * BIMG(BLEND) * SRCP[3] + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (SRCP)[1] - (DESTP)[1] ) * BIMG(BLEND) * SRCP[3] + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (SRCP)[2] - (DESTP)[2] ) * BIMG(BLEND) * SRCP[3] + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}
		
		// ���`�����l������(�`�F�b�N�R�[�h�t��)
		#define CODE_UBI_C32_NBR_UAC_BNO_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_NBR_NAC_BNO(  SRCP, DESTP, BLEND )\
				else					CODE_UBI_C32_NBR_UAC_BNO( SRCP, DESTP, BLEND )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_NBR_NAC_BNO( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (SRCP)[0] - (DESTP)[0] ) * BIMG(BLEND) + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (SRCP)[1] - (DESTP)[1] ) * BIMG(BLEND) + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (SRCP)[2] - (DESTP)[2] ) * BIMG(BLEND) + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}
		
		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_NBR_NAC_BNO_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_NBR_NAC_BNO( SRCP, DESTP, BLEND )
		

	  // �P�x����

		// ���`�����l������
		#define CODE_UBI_C32_UBR_UAC_BNO( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( BTABLE( (SRCP)[0] ) - (DESTP)[0] ) * BIMG(BLEND) * SRCP[3] + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( GTABLE( (SRCP)[1] ) - (DESTP)[1] ) * BIMG(BLEND) * SRCP[3] + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( RTABLE( (SRCP)[2] ) - (DESTP)[2] ) * BIMG(BLEND) * SRCP[3] + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BNO_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( ( ( (SRCP)[0] * BBRIGHT ) >> 8 ) - (DESTP)[0] ) * BIMG(BLEND) * SRCP[3] + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( ( ( (SRCP)[1] * GBRIGHT ) >> 8 ) - (DESTP)[1] ) * BIMG(BLEND) * SRCP[3] + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( ( ( (SRCP)[2] * RBRIGHT ) >> 8 ) - (DESTP)[2] ) * BIMG(BLEND) * SRCP[3] + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}
		
		// ���`�����l������(�`�F�b�N�R�[�h�t��)
		#define CODE_UBI_C32_UBR_UAC_BNO_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BNO( SRCP, DESTP, BLEND )\
				else					CODE_UBI_C32_UBR_UAC_BNO( SRCP, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�R�[�h�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BNO_ACK_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BNO_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )\
				else					CODE_UBI_C32_UBR_UAC_BNO_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_UBR_NAC_BNO( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( BTABLE( (SRCP)[0] ) - (DESTP)[0] ) * BIMG(BLEND) + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( GTABLE( (SRCP)[1] ) - (DESTP)[1] ) * BIMG(BLEND) + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( RTABLE( (SRCP)[2] ) - (DESTP)[2] ) * BIMG(BLEND) + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BNO_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( ( ( (SRCP)[0] * BBRIGHT ) >> 8 ) - (DESTP)[0] ) * BIMG(BLEND) + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( ( ( (SRCP)[1] * GBRIGHT ) >> 8 ) - (DESTP)[1] ) * BIMG(BLEND) + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( ( ( (SRCP)[2] * RBRIGHT ) >> 8 ) - (DESTP)[2] ) * BIMG(BLEND) + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_UBR_NAC_BNO_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BNO( SRCP, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BNO_TCK_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BNO_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )

	// �u�����h�摜����

	  // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_NBR_UAC_BNO( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (SRCP)[0] - (DESTP)[0] ) * SRCP[3] + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (SRCP)[1] - (DESTP)[1] ) * SRCP[3] + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (SRCP)[2] - (DESTP)[2] ) * SRCP[3] + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_UAC_BNO_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_NBR_NAC_BNO( SRCP, DESTP )\
				else					CODE_NBI_C32_NBR_UAC_BNO( SRCP, DESTP )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_NBR_NAC_BNO( SRCP, DESTP )	\
		{\
			*((DWORD *)(DESTP)) = *((DWORD *)(SRCP)) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_NAC_BNO_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_NBR_NAC_BNO( SRCP, DESTP )

	
	  // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_UBR_UAC_BNO( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( BTABLE( (SRCP)[0] ) - (DESTP)[0] ) * SRCP[3] + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( GTABLE( (SRCP)[1] ) - (DESTP)[1] ) * SRCP[3] + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( RTABLE( (SRCP)[2] ) - (DESTP)[2] ) * SRCP[3] + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BNO_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( ( ( (SRCP)[0] * BBRIGHT ) >> 8 ) - (DESTP)[0] ) * SRCP[3] + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( ( ( (SRCP)[1] * GBRIGHT ) >> 8 ) - (DESTP)[1] ) * SRCP[3] + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( ( ( (SRCP)[2] * RBRIGHT ) >> 8 ) - (DESTP)[2] ) * SRCP[3] + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_UAC_BNO_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BNO( SRCP, DESTP )\
				else					CODE_NBI_C32_UBR_UAC_BNO( SRCP, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BNO_ACK_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BNO_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )\
				else					CODE_NBI_C32_UBR_UAC_BNO_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_UBR_NAC_BNO( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( BTABLE( (SRCP)[0] ) ) ;\
			(DESTP)[1] = (BYTE)( GTABLE( (SRCP)[1] ) ) ;\
			(DESTP)[2] = (BYTE)( RTABLE( (SRCP)[2] ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BNO_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( (SRCP)[0] * BBRIGHT ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( (SRCP)[1] * GBRIGHT ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( (SRCP)[2] * RBRIGHT ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_NAC_BNO_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BNO( SRCP, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BNO_TCK_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BNO_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )

  // ���u�����h

    // �u�����h�摜����
 
      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C32_NBR_UAC_BAL( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (SRCP)[0] - (DESTP)[0] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[0] << 24 ) ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (SRCP)[1] - (DESTP)[1] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[1] << 24 ) ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (SRCP)[2] - (DESTP)[2] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[2] << 24 ) ) >> 24 ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C32_NBR_UAC_BAL_NTBL( SRCP, DESTP, BLEND, ABRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (SRCP)[0] - (DESTP)[0] ) * ABRIGHT * BIMG(BLEND) * SRCP[3] + ( (DESTP)[0] << 24 ) ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (SRCP)[1] - (DESTP)[1] ) * ABRIGHT * BIMG(BLEND) * SRCP[3] + ( (DESTP)[1] << 24 ) ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (SRCP)[2] - (DESTP)[2] ) * ABRIGHT * BIMG(BLEND) * SRCP[3] + ( (DESTP)[2] << 24 ) ) >> 24 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C32_NBR_UAC_BAL_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_NBR_NAC_BAL( SRCP, DESTP, BLEND )\
				else					CODE_UBI_C32_NBR_UAC_BAL( SRCP, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_NBR_UAC_BAL_ACK_NTBL( SRCP, DESTP, BLEND, ABRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_NBR_NAC_BAL_NTBL( SRCP, DESTP, BLEND, ABRIGHT )\
				else					CODE_UBI_C32_NBR_UAC_BAL_NTBL( SRCP, DESTP, BLEND, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_NBR_NAC_BAL( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (SRCP)[0] - (DESTP)[0] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (SRCP)[1] - (DESTP)[1] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (SRCP)[2] - (DESTP)[2] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C32_NBR_NAC_BAL_NTBL( SRCP, DESTP, BLEND, ABRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (SRCP)[0] - (DESTP)[0] ) * ABRIGHT * BIMG(BLEND) + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (SRCP)[1] - (DESTP)[1] ) * ABRIGHT * BIMG(BLEND) + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (SRCP)[2] - (DESTP)[2] ) * ABRIGHT * BIMG(BLEND) + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_NBR_NAC_BAL_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_NBR_NAC_BAL( SRCP, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_NBR_NAC_BAL_TCK_NTBL( SRCP, DESTP, BLEND, ABRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_NBR_NAC_BAL_NTBL( SRCP, DESTP, BLEND, ABRIGHT )


      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C32_UBR_UAC_BAL( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( BTABLE( (SRCP)[0] ) - (DESTP)[0] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[0] << 24 ) ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( ( GTABLE( (SRCP)[1] ) - (DESTP)[1] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[1] << 24 ) ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( ( RTABLE( (SRCP)[2] ) - (DESTP)[2] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[2] << 24 ) ) >> 24 ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BAL_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( ( ( (SRCP)[0] * BBRIGHT ) >> 8 ) - (DESTP)[0] ) * ABRIGHT * BIMG(BLEND) * SRCP[3] + ( (DESTP)[0] << 24 ) ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( ( ( ( (SRCP)[1] * GBRIGHT ) >> 8 ) - (DESTP)[1] ) * ABRIGHT * BIMG(BLEND) * SRCP[3] + ( (DESTP)[1] << 24 ) ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( ( ( ( (SRCP)[2] * RBRIGHT ) >> 8 ) - (DESTP)[2] ) * ABRIGHT * BIMG(BLEND) * SRCP[3] + ( (DESTP)[2] << 24 ) ) >> 24 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C32_UBR_UAC_BAL_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BAL( SRCP, DESTP, BLEND )\
				else					CODE_UBI_C32_UBR_UAC_BAL( SRCP, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BAL_ACK_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BAL_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else					CODE_UBI_C32_UBR_UAC_BAL_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_UBR_NAC_BAL( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( BTABLE( (SRCP)[0] ) - (DESTP)[0] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( GTABLE( (SRCP)[1] ) - (DESTP)[1] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( RTABLE( (SRCP)[2] ) - (DESTP)[2] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BAL_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( ( ( (SRCP)[0] * BBRIGHT ) >> 8 ) - (DESTP)[0] ) * ABRIGHT * BIMG(BLEND) + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( ( ( (SRCP)[1] * GBRIGHT ) >> 8 ) - (DESTP)[1] ) * ABRIGHT * BIMG(BLEND) + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( ( ( (SRCP)[2] * RBRIGHT ) >> 8 ) - (DESTP)[2] ) * ABRIGHT * BIMG(BLEND) + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_UBR_NAC_BAL_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BAL( SRCP, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BAL_TCK_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BAL_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

    // �u�����h�摜����

      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_NBR_UAC_BAL( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (SRCP)[0] - (DESTP)[0] ) * BPARAM * SRCP[3] + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (SRCP)[1] - (DESTP)[1] ) * BPARAM * SRCP[3] + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (SRCP)[2] - (DESTP)[2] ) * BPARAM * SRCP[3] + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C32_NBR_UAC_BAL_NTBL( SRCP, DESTP, ABRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (SRCP)[0] - (DESTP)[0] ) * ABRIGHT * SRCP[3] + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (SRCP)[1] - (DESTP)[1] ) * ABRIGHT * SRCP[3] + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (SRCP)[2] - (DESTP)[2] ) * ABRIGHT * SRCP[3] + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_UAC_BAL_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_NBR_NAC_BAL( SRCP, DESTP )\
				else					CODE_NBI_C32_NBR_UAC_BAL( SRCP, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_NBR_UAC_BAL_ACK_NTBL( SRCP, DESTP, ABRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_NBR_NAC_BAL_NTBL( SRCP, DESTP, ABRIGHT )\
				else					CODE_NBI_C32_NBR_UAC_BAL_NTBL( SRCP, DESTP, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_NBR_NAC_BAL( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (SRCP)[0] - (DESTP)[0] ) * BPARAM + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (SRCP)[1] - (DESTP)[1] ) * BPARAM + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (SRCP)[2] - (DESTP)[2] ) * BPARAM + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C32_NBR_NAC_BAL_NTBL( SRCP, DESTP, ABRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (SRCP)[0] - (DESTP)[0] ) * ABRIGHT + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (SRCP)[1] - (DESTP)[1] ) * ABRIGHT + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (SRCP)[2] - (DESTP)[2] ) * ABRIGHT + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_NAC_BAL_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_NBR_NAC_BAL( SRCP, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_NBR_NAC_BAL_TCK_NTBL( SRCP, DESTP, ABRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_NBR_NAC_BAL_NTBL( SRCP, DESTP, ABRIGHT )


      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_UBR_UAC_BAL( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( BTABLE( (SRCP)[0] ) - (DESTP)[0] ) * BPARAM * SRCP[3] + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( GTABLE( (SRCP)[1] ) - (DESTP)[1] ) * BPARAM * SRCP[3] + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( RTABLE( (SRCP)[2] ) - (DESTP)[2] ) * BPARAM * SRCP[3] + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BAL_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( ( ( (SRCP)[0] * BBRIGHT ) >> 8 ) - (DESTP)[0] ) * ABRIGHT * SRCP[3] + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( ( ( (SRCP)[1] * GBRIGHT ) >> 8 ) - (DESTP)[1] ) * ABRIGHT * SRCP[3] + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( ( ( (SRCP)[2] * RBRIGHT ) >> 8 ) - (DESTP)[2] ) * ABRIGHT * SRCP[3] + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_UAC_BAL_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BAL( SRCP, DESTP )\
				else					CODE_NBI_C32_UBR_UAC_BAL( SRCP, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BAL_ACK_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BAL_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else					CODE_NBI_C32_UBR_UAC_BAL_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_UBR_NAC_BAL( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( BTABLE( (SRCP)[0] ) - (DESTP)[0] ) * BPARAM + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( GTABLE( (SRCP)[1] ) - (DESTP)[1] ) * BPARAM + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( RTABLE( (SRCP)[2] ) - (DESTP)[2] ) * BPARAM + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BAL_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( ( ( (SRCP)[0] * BBRIGHT ) >> 8 ) - (DESTP)[0] ) * ABRIGHT + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( ( ( (SRCP)[1] * GBRIGHT ) >> 8 ) - (DESTP)[1] ) * ABRIGHT + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( ( ( (SRCP)[2] * RBRIGHT ) >> 8 ) - (DESTP)[2] ) * ABRIGHT + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_NAC_BAL_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BAL( SRCP, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BAL_TCK_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BAL_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

  // ���Z�u�����h

	// �u�����h�摜����

      // �P�x�Ȃ�

		// ���`�����l������
		#define CODE_UBI_C32_NBR_UAC_BAD( SRCP, DESTP, BLEND )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * (SRCP)[3] * BIMG(BLEND) * BPARAM ) >> 24 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * (SRCP)[3] * BIMG(BLEND) * BPARAM ) >> 24 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * (SRCP)[3] * BIMG(BLEND) * BPARAM ) >> 24 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C32_NBR_UAC_BAD_NTBL( SRCP, DESTP, BLEND, ABRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * (SRCP)[3] * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * (SRCP)[3] * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * (SRCP)[3] * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C32_NBR_UAC_BAD_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_NBR_NAC_BAD( SRCP, DESTP, BLEND )\
				else					CODE_UBI_C32_NBR_UAC_BAD( SRCP, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_NBR_UAC_BAD_ACK_NTBL( SRCP, DESTP, BLEND, ABRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_NBR_NAC_BAD_NTBL( SRCP, DESTP, BLEND, ABRIGHT )\
				else					CODE_UBI_C32_NBR_UAC_BAD_NTBL( SRCP, DESTP, BLEND, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_NBR_NAC_BAD( SRCP, DESTP, BLEND )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * BIMG(BLEND) * BPARAM ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * BIMG(BLEND) * BPARAM ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * BIMG(BLEND) * BPARAM ) >> 16 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C32_NBR_NAC_BAD_NTBL( SRCP, DESTP, BLEND, ABRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * BIMG(BLEND) * ABRIGHT ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * BIMG(BLEND) * ABRIGHT ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * BIMG(BLEND) * ABRIGHT ) >> 16 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_NBR_NAC_BAD_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_NBR_NAC_BAD( SRCP, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_NBR_NAC_BAD_TCK_NTBL( SRCP, DESTP, BLEND, ABRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_NBR_NAC_BAD_NTBL( SRCP, DESTP, BLEND, ABRIGHT )

	  // �P�x����

		// ���`�����l������
		#define CODE_UBI_C32_UBR_UAC_BAD( SRCP, DESTP, BLEND )	\
		{\
			SrcB[0] = (BYTE)( ( BTABLE( (SRCP)[0] ) * (SRCP)[3] * BIMG(BLEND) * BPARAM ) >> 24 ) ;\
			SrcB[1] = (BYTE)( ( GTABLE( (SRCP)[1] ) * (SRCP)[3] * BIMG(BLEND) * BPARAM ) >> 24 ) ;\
			SrcB[2] = (BYTE)( ( RTABLE( (SRCP)[2] ) * (SRCP)[3] * BIMG(BLEND) * BPARAM ) >> 24 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BAD_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( ( ( (SRCP)[0] * BBRIGHT ) >> 8 ) * (SRCP)[3] * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			SrcB[1] = (BYTE)( ( ( ( (SRCP)[1] * GBRIGHT ) >> 8 ) * (SRCP)[3] * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			SrcB[2] = (BYTE)( ( ( ( (SRCP)[2] * RBRIGHT ) >> 8 ) * (SRCP)[3] * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C32_UBR_UAC_BAD_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BAD( SRCP, DESTP, BLEND )\
				else					CODE_UBI_C32_UBR_UAC_BAD( SRCP, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BAD_ACK_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BAD_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else					CODE_UBI_C32_UBR_UAC_BAD_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_UBR_NAC_BAD( SRCP, DESTP, BLEND )	\
		{\
			SrcB[0] = (BYTE)( ( BTABLE( (SRCP)[0] ) * BIMG(BLEND) * BPARAM ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( GTABLE( (SRCP)[1] ) * BIMG(BLEND) * BPARAM ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( RTABLE( (SRCP)[2] ) * BIMG(BLEND) * BPARAM ) >> 16 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BAD_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * BBRIGHT * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * GBRIGHT * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * RBRIGHT * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_UBR_NAC_BAD_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BAD( SRCP, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BAD_TCK_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BAD_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

	// �u�����h�摜����
	
	  // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_NBR_UAC_BAD( SRCP, DESTP )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * (SRCP)[3] * BPARAM ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * (SRCP)[3] * BPARAM ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * (SRCP)[3] * BPARAM ) >> 16 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C32_NBR_UAC_BAD_NTBL( SRCP, DESTP, ABRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * (SRCP)[3] * ABRIGHT ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * (SRCP)[3] * ABRIGHT ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * (SRCP)[3] * ABRIGHT ) >> 16 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_UAC_BAD_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_NBR_NAC_BAD( SRCP, DESTP )\
				else					CODE_NBI_C32_NBR_UAC_BAD( SRCP, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_NBR_UAC_BAD_ACK_NTBL( SRCP, DESTP, ABRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_NBR_NAC_BAD_NTBL( SRCP, DESTP, ABRIGHT )\
				else					CODE_NBI_C32_NBR_UAC_BAD_NTBL( SRCP, DESTP, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_NBR_NAC_BAD( SRCP, DESTP )	\
		{\
			SrcB[0] = (BYTE)( MT1( (SRCP)[0] ) ) ;\
			SrcB[1] = (BYTE)( MT1( (SRCP)[1] ) ) ;\
			SrcB[2] = (BYTE)( MT1( (SRCP)[2] ) ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C32_NBR_NAC_BAD_NTBL( SRCP, DESTP, ABRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * ABRIGHT ) >> 8 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * ABRIGHT ) >> 8 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * ABRIGHT ) >> 8 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_NAC_BAD_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_NBR_NAC_BAD( SRCP, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_NBR_NAC_BAD_TCK_NTBL( SRCP, DESTP, ABRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_NBR_NAC_BAD_NTBL( SRCP, DESTP, ABRIGHT )

	  // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_UBR_UAC_BAD( SRCP, DESTP )	\
		{\
			SrcB[0] = (BYTE)( ( BTABLE( (SRCP)[0] ) * (SRCP)[3] * BPARAM ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( GTABLE( (SRCP)[1] ) * (SRCP)[3] * BPARAM ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( RTABLE( (SRCP)[2] ) * (SRCP)[3] * BPARAM ) >> 16 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BAD_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * BBRIGHT * (SRCP)[3] * ABRIGHT ) >> 24 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * GBRIGHT * (SRCP)[3] * ABRIGHT ) >> 24 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * RBRIGHT * (SRCP)[3] * ABRIGHT ) >> 24 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_UAC_BAD_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BAD( SRCP, DESTP )\
				else					CODE_NBI_C32_UBR_UAC_BAD( SRCP, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BAD_ACK_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BAD_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else					CODE_NBI_C32_UBR_UAC_BAD_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_UBR_NAC_BAD( SRCP, DESTP )	\
		{\
			SrcB[0] = (BYTE)( MT1( BTABLE( (SRCP)[0] ) ) ) ;\
			SrcB[1] = (BYTE)( MT1( GTABLE( (SRCP)[1] ) ) ) ;\
			SrcB[2] = (BYTE)( MT1( RTABLE( (SRCP)[2] ) ) ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BAD_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * ABRIGHT * BBRIGHT ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * ABRIGHT * GBRIGHT ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * ABRIGHT * RBRIGHT ) >> 16 ) ;\
			Data  = ( SrcD & MEMIMG_XRGB8_ADDMASK1 ) + ( *((DWORD *)(DESTP)) & MEMIMG_XRGB8_ADDMASK1 ) ;\
			DataD =   Data & MEMIMG_XRGB8_ADDMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( ( ( Data | ( DataD - ( DataD >> MEMIMG_XRGB8_ADDSHFT ) ) ) ) | ( ( SrcD | *((DWORD *)(DESTP)) ) & MEMIMG_XRGB8_ADDMASK3 ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_NAC_BAD_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BAD( SRCP, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BAD_TCK_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BAD_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

  // ���Z�u�����h

    // �u�����h�摜����
 
      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C32_NBR_UAC_BSB( SRCP, DESTP, BLEND )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * (SRCP)[3] * BIMG(BLEND) * BPARAM ) >> 24 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * (SRCP)[3] * BIMG(BLEND) * BPARAM ) >> 24 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * (SRCP)[3] * BIMG(BLEND) * BPARAM ) >> 24 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C32_NBR_UAC_BSB_NTBL( SRCP, DESTP, BLEND, ABRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * (SRCP)[3] * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * (SRCP)[3] * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * (SRCP)[3] * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C32_NBR_UAC_BSB_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_NBR_NAC_BSB( SRCP, DESTP, BLEND )\
				else					CODE_UBI_C32_NBR_UAC_BSB( SRCP, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_NBR_UAC_BSB_ACK_NTBL( SRCP, DESTP, BLEND, ABRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_NBR_NAC_BSB_NTBL( SRCP, DESTP, BLEND, ABRIGHT )\
				else					CODE_UBI_C32_NBR_UAC_BSB_NTBL( SRCP, DESTP, BLEND, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_NBR_NAC_BSB( SRCP, DESTP, BLEND )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * BIMG(BLEND) * BPARAM ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * BIMG(BLEND) * BPARAM ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * BIMG(BLEND) * BPARAM ) >> 16 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C32_NBR_NAC_BSB_NTBL( SRCP, DESTP, BLEND, ABRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * BIMG(BLEND) * ABRIGHT ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * BIMG(BLEND) * ABRIGHT ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * BIMG(BLEND) * ABRIGHT ) >> 16 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_NBR_NAC_BSB_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_NBR_NAC_BSB( SRCP, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_NBR_NAC_BSB_TCK_NTBL( SRCP, DESTP, BLEND, ABRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_NBR_NAC_BSB_NTBL( SRCP, DESTP, BLEND, ABRIGHT )

      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C32_UBR_UAC_BSB( SRCP, DESTP, BLEND )	\
		{\
			SrcB[0] = (BYTE)( ( BTABLE( (SRCP)[0] ) * (SRCP)[3] * BIMG(BLEND) * BPARAM ) >> 24 ) ;\
			SrcB[1] = (BYTE)( ( GTABLE( (SRCP)[1] ) * (SRCP)[3] * BIMG(BLEND) * BPARAM ) >> 24 ) ;\
			SrcB[2] = (BYTE)( ( RTABLE( (SRCP)[2] ) * (SRCP)[3] * BIMG(BLEND) * BPARAM ) >> 24 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BSB_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( ( ( (SRCP)[0] * BBRIGHT ) >> 8 ) * (SRCP)[3] * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			SrcB[1] = (BYTE)( ( ( ( (SRCP)[1] * GBRIGHT ) >> 8 ) * (SRCP)[3] * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			SrcB[2] = (BYTE)( ( ( ( (SRCP)[2] * RBRIGHT ) >> 8 ) * (SRCP)[3] * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C32_UBR_UAC_BSB_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BSB( SRCP, DESTP, BLEND )\
				else					CODE_UBI_C32_UBR_UAC_BSB( SRCP, DESTP, BLEND )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BSB_ACK_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BSB_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else					CODE_UBI_C32_UBR_UAC_BSB_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_UBR_NAC_BSB( SRCP, DESTP, BLEND )	\
		{\
			SrcB[0] = (BYTE)( ( BTABLE( (SRCP)[0] ) * BIMG(BLEND) * BPARAM ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( GTABLE( (SRCP)[1] ) * BIMG(BLEND) * BPARAM ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( RTABLE( (SRCP)[2] ) * BIMG(BLEND) * BPARAM ) >> 16 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BSB_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * BBRIGHT * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * GBRIGHT * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * RBRIGHT * BIMG(BLEND) * ABRIGHT ) >> 24 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_UBR_NAC_BSB_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BSB( SRCP, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BSB_TCK_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BSB_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

    // �u�����h�摜����

	  // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_NBR_UAC_BSB( SRCP, DESTP )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * (SRCP)[3] * BPARAM ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * (SRCP)[3] * BPARAM ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * (SRCP)[3] * BPARAM ) >> 16 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		#define CODE_NBI_C32_NBR_UAC_BSB_NTBL( SRCP, DESTP, ABRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * (SRCP)[3] * ABRIGHT ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * (SRCP)[3] * ABRIGHT ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * (SRCP)[3] * ABRIGHT ) >> 16 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_UAC_BSB_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_NBR_NAC_BSB( SRCP, DESTP )\
				else					CODE_NBI_C32_NBR_UAC_BSB( SRCP, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_UAC_BSB_ACK_NTBL( SRCP, DESTP, ABRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_NBR_NAC_BSB_NTBL( SRCP, DESTP, ABRIGHT )\
				else					CODE_NBI_C32_NBR_UAC_BSB_NTBL( SRCP, DESTP, ABRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_NBR_NAC_BSB( SRCP, DESTP )	\
		{\
			SrcB[0] = (BYTE)( MT1( (SRCP)[0] ) ) ;\
			SrcB[1] = (BYTE)( MT1( (SRCP)[1] ) ) ;\
			SrcB[2] = (BYTE)( MT1( (SRCP)[2] ) ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C32_NBR_NAC_BSB_NTBL( SRCP, DESTP, ABRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * ABRIGHT ) >> 8 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * ABRIGHT ) >> 8 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * ABRIGHT ) >> 8 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_NAC_BSB_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_NBR_NAC_BSB( SRCP, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_NBR_NAC_BSB_TCK_NTBL( SRCP, DESTP, ABRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_NBR_NAC_BSB_NTBL( SRCP, DESTP, ABRIGHT )

	  // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_UBR_UAC_BSB( SRCP, DESTP )	\
		{\
			SrcB[0] = (BYTE)( ( BTABLE( (SRCP)[0] ) * (SRCP)[3] * BPARAM ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( GTABLE( (SRCP)[1] ) * (SRCP)[3] * BPARAM ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( RTABLE( (SRCP)[2] ) * (SRCP)[3] * BPARAM ) >> 16 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BSB_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * BBRIGHT * (SRCP)[3] * ABRIGHT ) >> 24 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * GBRIGHT * (SRCP)[3] * ABRIGHT ) >> 24 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * RBRIGHT * (SRCP)[3] * ABRIGHT ) >> 24 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_UAC_BSB_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BSB( SRCP, DESTP )\
				else					CODE_NBI_C32_UBR_UAC_BSB( SRCP, DESTP )\
			}

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BSB_ACK_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] != 0 )\
			{\
				if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BSB_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
				else					CODE_NBI_C32_UBR_UAC_BSB_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			}

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_UBR_NAC_BSB( SRCP, DESTP )	\
		{\
			SrcB[0] = (BYTE)( MT1( BTABLE( (SRCP)[0] ) ) ) ;\
			SrcB[1] = (BYTE)( MT1( GTABLE( (SRCP)[1] ) ) ) ;\
			SrcB[2] = (BYTE)( MT1( RTABLE( (SRCP)[2] ) ) ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BSB_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			SrcB[0] = (BYTE)( ( (SRCP)[0] * ABRIGHT * BBRIGHT ) >> 16 ) ;\
			SrcB[1] = (BYTE)( ( (SRCP)[1] * ABRIGHT * GBRIGHT ) >> 16 ) ;\
			SrcB[2] = (BYTE)( ( (SRCP)[2] * ABRIGHT * RBRIGHT ) >> 16 ) ;\
			Data = ( *((DWORD *)(DESTP)) | MEMIMG_XRGB8_SUBMASK2 ) - ( SrcD & ~MEMIMG_XRGB8_SUBMASK2 ) ;\
			DataD = Data & MEMIMG_XRGB8_SUBMASK2 ;\
			*((DWORD *)(DESTP)) = (DWORD)( Data & ( DataD - ( DataD >> MEMIMG_XRGB8_SUBSHFT ) ) ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_NAC_BSB_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BSB( SRCP, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BSB_TCK_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BSB_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )


  // ��Z�u�����h

    // �u�����h�摜����
 
      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C32_NBR_UAC_BML( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( (SRCP)[0] * BIMG(BLEND) * (SRCP)[3] * (DESTP)[0] ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( (SRCP)[1] * BIMG(BLEND) * (SRCP)[3] * (DESTP)[1] ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( (SRCP)[2] * BIMG(BLEND) * (SRCP)[3] * (DESTP)[2] ) >> 24 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C32_NBR_UAC_BML_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_NBR_NAC_BML( SRCP, DESTP, BLEND )\
			else					CODE_UBI_C32_NBR_UAC_BML( SRCP, DESTP, BLEND )

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_NBR_NAC_BML( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( (SRCP)[0] * BIMG(BLEND) * (DESTP)[0] ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( (SRCP)[1] * BIMG(BLEND) * (DESTP)[1] ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( (SRCP)[2] * BIMG(BLEND) * (DESTP)[2] ) >> 16 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_NBR_NAC_BML_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_NBR_NAC_BML( SRCP, DESTP, BLEND )

      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C32_UBR_UAC_BML( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( BTABLE( (SRCP)[0] ) * BIMG(BLEND) * (SRCP)[3] * (DESTP)[0] ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( GTABLE( (SRCP)[1] ) * BIMG(BLEND) * (SRCP)[3] * (DESTP)[1] ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( RTABLE( (SRCP)[2] ) * BIMG(BLEND) * (SRCP)[3] * (DESTP)[2] ) >> 24 ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BML_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( ( (SRCP)[0] * BBRIGHT ) >> 8 ) * BIMG(BLEND) * (SRCP)[3] * (DESTP)[0] ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( ( ( (SRCP)[1] * GBRIGHT ) >> 8 ) * BIMG(BLEND) * (SRCP)[3] * (DESTP)[1] ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( ( ( (SRCP)[2] * RBRIGHT ) >> 8 ) * BIMG(BLEND) * (SRCP)[3] * (DESTP)[2] ) >> 24 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C32_UBR_UAC_BML_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BML( SRCP, DESTP, BLEND )\
			else					CODE_UBI_C32_UBR_UAC_BML( SRCP, DESTP, BLEND )

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BML_ACK_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BML_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )\
			else					CODE_UBI_C32_UBR_UAC_BML_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_UBR_NAC_BML( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( BTABLE( (SRCP)[0] ) * BIMG(BLEND) * (DESTP)[0] ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( GTABLE( (SRCP)[1] ) * BIMG(BLEND) * (DESTP)[1] ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( RTABLE( (SRCP)[2] ) * BIMG(BLEND) * (DESTP)[2] ) >> 16 ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BML_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( (SRCP)[0] * BBRIGHT * BIMG(BLEND) * (DESTP)[0] ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( (SRCP)[1] * GBRIGHT * BIMG(BLEND) * (DESTP)[1] ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( (SRCP)[2] * RBRIGHT * BIMG(BLEND) * (DESTP)[2] ) >> 24 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_UBR_NAC_BML_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BML( SRCP, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BML_TCK_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BML_NTBL( SRCP, DESTP, BLEND, RBRIGHT, GBRIGHT, BBRIGHT )

    // �u�����h�摜����

      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_NBR_UAC_BML( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( (SRCP)[0] * (SRCP)[3] * (DESTP)[0] ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( (SRCP)[1] * (SRCP)[3] * (DESTP)[1] ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( (SRCP)[2] * (SRCP)[3] * (DESTP)[2] ) >> 16 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_UAC_BML_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_NBR_NAC_BML( SRCP, DESTP )\
			else					CODE_NBI_C32_NBR_UAC_BML( SRCP, DESTP )

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_NBR_NAC_BML( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( (SRCP)[0] * (DESTP)[0] ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( (SRCP)[1] * (DESTP)[1] ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( (SRCP)[2] * (DESTP)[2] ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_NAC_BML_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_NBR_NAC_BML( SRCP, DESTP )

      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_UBR_UAC_BML( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( BTABLE( (SRCP)[0] ) * (SRCP)[3] * (DESTP)[0] ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( GTABLE( (SRCP)[1] ) * (SRCP)[3] * (DESTP)[1] ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( RTABLE( (SRCP)[2] ) * (SRCP)[3] * (DESTP)[2] ) >> 16 ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BML_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( (SRCP)[0] * BBRIGHT * (SRCP)[3] * (DESTP)[0] ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( (SRCP)[1] * GBRIGHT * (SRCP)[3] * (DESTP)[1] ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( (SRCP)[2] * RBRIGHT * (SRCP)[3] * (DESTP)[2] ) >> 24 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_UAC_BML_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BML( SRCP, DESTP )\
			else					CODE_NBI_C32_UBR_UAC_BML( SRCP, DESTP )

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BML_ACK_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BML_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )\
			else					CODE_NBI_C32_UBR_UAC_BML_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_UBR_NAC_BML( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( BTABLE( (SRCP)[0] ) * (DESTP)[0] ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( GTABLE( (SRCP)[1] ) * (DESTP)[1] ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( RTABLE( (SRCP)[2] ) * (DESTP)[2] ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BML_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( (SRCP)[0] * BBRIGHT * (DESTP)[0] ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( (SRCP)[1] * GBRIGHT * (DESTP)[1] ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( (SRCP)[2] * RBRIGHT * (DESTP)[2] ) >> 16 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_NAC_BML_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BML( SRCP, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BML_TCK_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BML_NTBL( SRCP, DESTP, RBRIGHT, GBRIGHT, BBRIGHT )


  // ��Z�{���u�����h

    // �u�����h�摜����
 
      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C32_NBR_UAC_BMA( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( (SRCP)[0] * (DESTP)[0] ) >> 8 ) - (DESTP)[0] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[0] << 24 ) ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( (SRCP)[1] * (DESTP)[1] ) >> 8 ) - (DESTP)[1] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[1] << 24 ) ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( (SRCP)[2] * (DESTP)[2] ) >> 8 ) - (DESTP)[2] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[2] << 24 ) ) >> 24 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C32_NBR_UAC_BMA_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_NBR_NAC_BMA( SRCP, DESTP, BLEND )\
			else					CODE_UBI_C32_NBR_UAC_BMA( SRCP, DESTP, BLEND )

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_NBR_NAC_BMA( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( (SRCP)[0] * (DESTP)[0] ) >> 8 ) - (DESTP)[0] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( (SRCP)[1] * (DESTP)[1] ) >> 8 ) - (DESTP)[1] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( (SRCP)[2] * (DESTP)[2] ) >> 8 ) - (DESTP)[2] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_NBR_NAC_BMA_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_NBR_NAC_BMA( SRCP, DESTP, BLEND )

      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C32_UBR_UAC_BMA( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( BTABLE( (SRCP)[0] ) * (DESTP)[0] ) >> 8 ) - (DESTP)[0] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[0] << 24 ) ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( GTABLE( (SRCP)[1] ) * (DESTP)[1] ) >> 8 ) - (DESTP)[1] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[1] << 24 ) ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( RTABLE( (SRCP)[2] ) * (DESTP)[2] ) >> 8 ) - (DESTP)[2] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[2] << 24 ) ) >> 24 ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BMA_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( (SRCP)[0] * BBRIGHT * (DESTP)[0] ) >> 16 ) - (DESTP)[0] ) * ABRIGHT * BIMG(BLEND) * SRCP[3] + ( (DESTP)[0] << 24 ) ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( (SRCP)[1] * GBRIGHT * (DESTP)[1] ) >> 16 ) - (DESTP)[1] ) * ABRIGHT * BIMG(BLEND) * SRCP[3] + ( (DESTP)[1] << 24 ) ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( (SRCP)[2] * RBRIGHT * (DESTP)[2] ) >> 16 ) - (DESTP)[2] ) * ABRIGHT * BIMG(BLEND) * SRCP[3] + ( (DESTP)[2] << 24 ) ) >> 24 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C32_UBR_UAC_BMA_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BMA( SRCP, DESTP, BLEND )\
			else					CODE_UBI_C32_UBR_UAC_BMA( SRCP, DESTP, BLEND )

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BMA_ACK_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BMA_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			else					CODE_UBI_C32_UBR_UAC_BMA_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_UBR_NAC_BMA( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( BTABLE( (SRCP)[0] ) * (DESTP)[0] ) >> 8 ) - (DESTP)[0] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( GTABLE( (SRCP)[1] ) * (DESTP)[1] ) >> 8 ) - (DESTP)[1] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( RTABLE( (SRCP)[2] ) * (DESTP)[2] ) >> 8 ) - (DESTP)[2] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BMA_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( (SRCP)[0] * BBRIGHT * (DESTP)[0] ) >> 16 ) - (DESTP)[0] ) * ABRIGHT * BIMG(BLEND) + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( (SRCP)[1] * GBRIGHT * (DESTP)[1] ) >> 16 ) - (DESTP)[1] ) * ABRIGHT * BIMG(BLEND) + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( (SRCP)[2] * RBRIGHT * (DESTP)[2] ) >> 16 ) - (DESTP)[2] ) * ABRIGHT * BIMG(BLEND) + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_UBR_NAC_BMA_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BMA( SRCP, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BMA_TCK_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BMA_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

    // �u�����h�摜����

      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_NBR_UAC_BMA( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( (SRCP)[0] * (DESTP)[0] ) >> 8 ) - (DESTP)[0] ) * BPARAM * SRCP[3] + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( (SRCP)[1] * (DESTP)[1] ) >> 8 ) - (DESTP)[1] ) * BPARAM * SRCP[3] + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( (SRCP)[2] * (DESTP)[2] ) >> 8 ) - (DESTP)[2] ) * BPARAM * SRCP[3] + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_UAC_BMA_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_NBR_NAC_BMA( SRCP, DESTP )\
			else					CODE_NBI_C32_NBR_UAC_BMA( SRCP, DESTP )

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_NBR_NAC_BMA( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( (SRCP)[0] * (DESTP)[0] ) >> 8 ) - (DESTP)[0] ) * BPARAM + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( (SRCP)[1] * (DESTP)[1] ) >> 8 ) - (DESTP)[1] ) * BPARAM + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( (SRCP)[2] * (DESTP)[2] ) >> 8 ) - (DESTP)[2] ) * BPARAM + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_NAC_BMA_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_NBR_NAC_BMA( SRCP, DESTP )

      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_UBR_UAC_BMA( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( BTABLE( (SRCP)[0] ) * (DESTP)[0] ) >> 8 ) - (DESTP)[0] ) * BPARAM * SRCP[3] + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( GTABLE( (SRCP)[1] ) * (DESTP)[1] ) >> 8 ) - (DESTP)[1] ) * BPARAM * SRCP[3] + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( RTABLE( (SRCP)[2] ) * (DESTP)[2] ) >> 8 ) - (DESTP)[2] ) * BPARAM * SRCP[3] + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BMA_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( (SRCP)[0] * BBRIGHT * (DESTP)[0] ) >> 16 ) - (DESTP)[0] ) * ABRIGHT * SRCP[3] + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( (SRCP)[1] * GBRIGHT * (DESTP)[1] ) >> 16 ) - (DESTP)[1] ) * ABRIGHT * SRCP[3] + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( (SRCP)[2] * RBRIGHT * (DESTP)[2] ) >> 16 ) - (DESTP)[2] ) * ABRIGHT * SRCP[3] + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_UAC_BMA_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BMA( SRCP, DESTP )\
			else					CODE_NBI_C32_UBR_UAC_BMA( SRCP, DESTP )

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BMA_ACK_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BMA_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			else					CODE_NBI_C32_UBR_UAC_BMA_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_UBR_NAC_BMA( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( BTABLE( (SRCP)[0] ) * (DESTP)[0] ) >> 8 ) - (DESTP)[0] ) * BPARAM + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( GTABLE( (SRCP)[1] ) * (DESTP)[1] ) >> 8 ) - (DESTP)[1] ) * BPARAM + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( RTABLE( (SRCP)[2] ) * (DESTP)[2] ) >> 8 ) - (DESTP)[2] ) * BPARAM + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BMA_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( (SRCP)[0] * BBRIGHT * (DESTP)[0] ) >> 16 ) - (DESTP)[0] ) * ABRIGHT + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( (SRCP)[1] * GBRIGHT * (DESTP)[1] ) >> 16 ) - (DESTP)[1] ) * ABRIGHT + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( (SRCP)[2] * RBRIGHT * (DESTP)[2] ) >> 16 ) - (DESTP)[2] ) * ABRIGHT + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_NAC_BMA_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BMA( SRCP, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BMA_TCK_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BMA_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )


  // �`�挳�̐F�𔽓]�{���u�����h

    // �u�����h�摜����
 
      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C32_NBR_UAC_BIS( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ~(SRCP)[0] ) - (DESTP)[0] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[0] << 24 ) ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ~(SRCP)[1] ) - (DESTP)[1] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[1] << 24 ) ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ~(SRCP)[2] ) - (DESTP)[2] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[2] << 24 ) ) >> 24 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C32_NBR_UAC_BIS_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_NBR_NAC_BIS( SRCP, DESTP, BLEND )\
			else					CODE_UBI_C32_NBR_UAC_BIS( SRCP, DESTP, BLEND )

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_NBR_NAC_BIS( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ~(SRCP)[0] ) - (DESTP)[0] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ~(SRCP)[1] ) - (DESTP)[1] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ~(SRCP)[2] ) - (DESTP)[2] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_NBR_NAC_BIS_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_NBR_NAC_BIS( SRCP, DESTP, BLEND )

      // �P�x����

		// ���`�����l������
		#define CODE_UBI_C32_UBR_UAC_BIS( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( BTABLE( ( BYTE )~(SRCP)[0] ) ) - (DESTP)[0] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[0] << 24 ) ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( GTABLE( ( BYTE )~(SRCP)[1] ) ) - (DESTP)[1] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[1] << 24 ) ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( RTABLE( ( BYTE )~(SRCP)[2] ) ) - (DESTP)[2] ) * BPARAM * BIMG(BLEND) * SRCP[3] + ( (DESTP)[2] << 24 ) ) >> 24 ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BIS_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( ( BYTE )~(SRCP)[0] * BBRIGHT ) >> 8 ) - (DESTP)[0] ) * ABRIGHT * BIMG(BLEND) * SRCP[3] + ( (DESTP)[0] << 24 ) ) >> 24 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( ( BYTE )~(SRCP)[1] * GBRIGHT ) >> 8 ) - (DESTP)[1] ) * ABRIGHT * BIMG(BLEND) * SRCP[3] + ( (DESTP)[1] << 24 ) ) >> 24 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( ( BYTE )~(SRCP)[2] * RBRIGHT ) >> 8 ) - (DESTP)[2] ) * ABRIGHT * BIMG(BLEND) * SRCP[3] + ( (DESTP)[2] << 24 ) ) >> 24 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_UBI_C32_UBR_UAC_BIS_ACK( SRCP, DESTP, BLEND )	\
			if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BIS( SRCP, DESTP, BLEND )\
			else					CODE_UBI_C32_UBR_UAC_BIS( SRCP, DESTP, BLEND )

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_UAC_BIS_ACK_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] == 255 ) 	CODE_UBI_C32_UBR_NAC_BIS_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			else					CODE_UBI_C32_UBR_UAC_BIS_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

		// ���`�����l���Ȃ�
		#define CODE_UBI_C32_UBR_NAC_BIS( SRCP, DESTP, BLEND )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( BTABLE( (BYTE)~(SRCP)[0] ) ) - (DESTP)[0] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( GTABLE( (BYTE)~(SRCP)[1] ) ) - (DESTP)[1] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( RTABLE( (BYTE)~(SRCP)[2] ) ) - (DESTP)[2] ) * BPARAM * BIMG(BLEND) + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BIS_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( (BYTE)~(SRCP)[0] * BBRIGHT ) >> 8 ) - (DESTP)[0] ) * ABRIGHT * BIMG(BLEND) + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( (BYTE)~(SRCP)[1] * GBRIGHT ) >> 8 ) - (DESTP)[1] ) * ABRIGHT * BIMG(BLEND) + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( (BYTE)~(SRCP)[2] * RBRIGHT ) >> 8 ) - (DESTP)[2] ) * ABRIGHT * BIMG(BLEND) + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_UBI_C32_UBR_NAC_BIS_TCK( SRCP, DESTP, BLEND )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BIS( SRCP, DESTP, BLEND )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_UBI_C32_UBR_NAC_BIS_TCK_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_UBI_C32_UBR_NAC_BIS_NTBL( SRCP, DESTP, BLEND, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

    // �u�����h�摜����

      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_NBR_UAC_BIS( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)~(SRCP)[0] - (DESTP)[0] ) * BPARAM * SRCP[3] + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)~(SRCP)[1] - (DESTP)[1] ) * BPARAM * SRCP[3] + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)~(SRCP)[2] - (DESTP)[2] ) * BPARAM * SRCP[3] + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_UAC_BIS_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_NBR_NAC_BIS( SRCP, DESTP )\
			else					CODE_NBI_C32_NBR_UAC_BIS( SRCP, DESTP )

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_NBR_NAC_BIS( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)~(SRCP)[0] - (DESTP)[0] ) * BPARAM + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)~(SRCP)[1] - (DESTP)[1] ) * BPARAM + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)~(SRCP)[2] - (DESTP)[2] ) * BPARAM + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_NBR_NAC_BIS_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_NBR_NAC_BIS( SRCP, DESTP )

      // �P�x����

		// ���`�����l������
		#define CODE_NBI_C32_UBR_UAC_BIS( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( BTABLE( (BYTE)~(SRCP)[0] ) ) - (DESTP)[0] ) * BPARAM * SRCP[3] + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( GTABLE( (BYTE)~(SRCP)[1] ) ) - (DESTP)[1] ) * BPARAM * SRCP[3] + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( RTABLE( (BYTE)~(SRCP)[2] ) ) - (DESTP)[2] ) * BPARAM * SRCP[3] + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l������(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BIS_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( (BYTE)~(SRCP)[0] * BBRIGHT ) >> 8 ) - (DESTP)[0] ) * ABRIGHT * SRCP[3] + ( (DESTP)[0] << 16 ) ) >> 16 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( (BYTE)~(SRCP)[1] * GBRIGHT ) >> 8 ) - (DESTP)[1] ) * ABRIGHT * SRCP[3] + ( (DESTP)[1] << 16 ) ) >> 16 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( (BYTE)~(SRCP)[2] * RBRIGHT ) >> 8 ) - (DESTP)[2] ) * ABRIGHT * SRCP[3] + ( (DESTP)[2] << 16 ) ) >> 16 ) ;\
		}

		// ���`�����l������(�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_UAC_BIS_ACK( SRCP, DESTP )	\
			if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BIS( SRCP, DESTP )\
			else					CODE_NBI_C32_UBR_UAC_BIS( SRCP, DESTP )

		// ���`�����l������(�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_UAC_BIS_ACK_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( (SRCP)[3] == 255 ) 	CODE_NBI_C32_UBR_NAC_BIS_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )\
			else					CODE_NBI_C32_UBR_UAC_BIS_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )

		// ���`�����l���Ȃ�
		#define CODE_NBI_C32_UBR_NAC_BIS( SRCP, DESTP )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( BTABLE( (BYTE)~(SRCP)[0] ) ) - (DESTP)[0] ) * BPARAM + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( GTABLE( (BYTE)~(SRCP)[1] ) ) - (DESTP)[1] ) * BPARAM + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( RTABLE( (BYTE)~(SRCP)[2] ) ) - (DESTP)[2] ) * BPARAM + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BIS_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
		{\
			(DESTP)[0] = (BYTE)( ( ( (BYTE)( ( (BYTE)~(SRCP)[0] * BBRIGHT ) >> 8 ) - (DESTP)[0] ) * ABRIGHT + ( (DESTP)[0] << 8 ) ) >> 8 ) ;\
			(DESTP)[1] = (BYTE)( ( ( (BYTE)( ( (BYTE)~(SRCP)[1] * GBRIGHT ) >> 8 ) - (DESTP)[1] ) * ABRIGHT + ( (DESTP)[1] << 8 ) ) >> 8 ) ;\
			(DESTP)[2] = (BYTE)( ( ( (BYTE)( ( (BYTE)~(SRCP)[2] * RBRIGHT ) >> 8 ) - (DESTP)[2] ) * ABRIGHT + ( (DESTP)[2] << 8 ) ) >> 8 ) ;\
		}

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)
		#define CODE_NBI_C32_UBR_NAC_BIS_TCK( SRCP, DESTP )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BIS( SRCP, DESTP )

		// ���`�����l���Ȃ�(���ߐF�`�F�b�N�t��)(�e�[�u����g�p)
		#define CODE_NBI_C32_UBR_NAC_BIS_TCK_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )	\
			if( *((DWORD *)(SRCP)) != TransColor ) CODE_NBI_C32_UBR_NAC_BIS_NTBL( SRCP, DESTP, ABRIGHT, RBRIGHT, GBRIGHT, BBRIGHT )











// �\���̒�` --------------------------------------------------------------------

// ���R�ό`�`��⏕�p�|���S���`��֐�
struct DX_POINTDATA
{
	int						x , y ;
	int						u , v ;
} ;

/*
��̓f�[�^�t�H�[�}�b�g

  ��͏��͈�P�o�C�g
	
    2bit(BitMask 11000000):�f�[�^�̎��
      0(00B):�`�悷��K�v������
      1(01B):���l���ő�Ȃ̂Ń��l��K������K�v������
      2(10B):���l��K������K�v������
      3(11B):�s�̏I�[

    6bit(BitMask 00111111):�f�[�^�̎�ނɊ�Â��f�[�^�̒���(�ő�64(0��1�Ɛ�����))
*/

// MEMIMG �� Derivation ���Ă����ʂȕ����̃f�[�^
struct MEMIMGBASE
{
	int						RefCount ;							// �Q�ƃJ�E���g
	int						BaseWidth ;							// ��
	int						BaseHeight ;						// ����
	unsigned int			Pitch ;								// �s�b�`
	int						PitchPow2n ;						// �s�b�`���Q�̂��悾�������̍��V�t�g��( -1 �̏ꍇ�͂Q�̂���ł͂Ȃ� )
	COLORDATA				*ColorDataP ;						// �J���[�t�H�[�}�b�g�ւ̃|�C���^
	unsigned int			TransColor ;						// ���ߐF(�J���[�^�C�v 0,1 �̎��̂ݗL��)
//	unsigned int			TransPalette ;						// ���߃p���b�g�ԍ�(�p���b�g�����鎞�̂ݗL��)
	int						ColorType ;							// �J���[�^�C�v( 0:16bit(RG6B5 or X8A8R5G6B5)  1:32bit(XRGB8 ���� ARGB8)  2:8bit(�u�����h�摜)  3:16bit(�y�o�b�t�@) )
	int						UsePalette ;						// �p���b�g�g�p�̗L��( 1:�L��  0:���� ) �J���[�^�C�v 0,1 �̎��̂ݗL��
	int						ColorNum ;							// �p���b�g�Ŏg�p���Ă���F�̐�
	int						UseAlpha ;							// ���`�����l���̗L��( 1:�L��  0:���� ) �J���[�^�C�v 0,1 �̎��̂ݗL��
	int						AlphaImageValid ;					// AlphaImage ���L�����ǂ���( 1:�L��  0:���� )(�J���[�^�C�v 0,1 �̎��̂ݗL��)
	int						AnalysisDataValid ;					// AnalysisData ���L�����ǂ���( 1:�L��  0:���� )(�J���[�^�C�v 0,1 �̎��̂ݗL��)
	int						UserImageFlag ;						// ���[�U�[�� Image ��p�ӂ������A�t���O
	unsigned int			*Palette ;							// �p���b�g�ւ̃|�C���^�A�h�����ȊO�͓������o�z��� PaletteBase ���g�p����
	unsigned int			*OriginalPalette ;					// �I���W�i���p���b�g�ւ̃|�C���^�A�h�����ȊO�͓������o�z��� OriginalPaletteBase ���g�p����
	unsigned char			*Image ;							// �C���[�W�f�[�^�ւ̃A�h���X
	unsigned char			*AlphaImage ;						// �\�߃��`�����l���̌��ʂ��C���[�W�ɓK�������C���[�W�AUseAlpha �� AlphaImageValid ���P�̎��̂ݗL��(�J���[�^�C�v 0,1 �̎��̂ݗL��)
	unsigned char			*AnalysisData ;						// ���ߐF�̘A���񐔂⃿�`�����l�����ő�l���ŏ��l���A���ꂪ���h�b�g�A�����Ă��邩���̉�͏��AAnalysisDataValid ���P�̎��̂ݗL��(�J���[�^�C�v 0,1 �̎��̂ݗL��)
} ;

// �V�X�e����������̉摜�f�[�^
struct MEMIMG
{
	int						InitializeCheck ;					// �������`�F�b�N�f�[�^
	unsigned int			Width, Height ;						// �T�C�Y
	unsigned char			*UseImage ;							// �C���[�W�f�[�^�̎g�p���Ă���̈�ւ̃A�h���X
	unsigned char			*UseAlphaImage ;					// �\�߃��`�����l���̌��ʂ��C���[�W�ɓK�������C���[�W�̎g�p���Ă���̈�ւ̃A�h���X�AUseAlpha �� AlphaImageValid ���P�̎��̂ݗL��(�J���[�^�C�v 0,1 �̎��̂ݗL��)
	MEMIMGBASE				*Base ;								// ���ʏ��ւ̃|�C���^
} ;

// MEMIMG �Ǘ��\����
struct MEMIMGMANAGE
{
	BYTE					RateTable16[64][64][64] ;			// �����e�[�u�� ������ [�f�l1�̊����l(0=0% 63=100%][�f�l1][�f�l2]
	DWORD					RateTable[256][256] ;				// �����e�[�u�� ������ [�����l(0=0% 255=100%)][�f�l] 
	int						RateTable2[256][512] ;				// �����e�[�u�� ������ [�����l(0=0% 255=100%)][�f�l+255] 
	BYTE					BlendGraphTable[256] ;				// �u�����h�C���[�W�K�����p�e�[�u��
	int						DefaultColorType ;					// �f�t�H���g�̃J���[�^�C�v
	RECT					DrawArea ;							// �`��\�͈�

	int						BlendMode ;							// �u�����h���[�h
	int						BlendParam ;						// �u�����h�p�����[�^
	union
	{
		RGBCOLOR			DrawBright ;							// �`��P�x
		DWORD				bDrawBright ;
	} ;
} ;

// �������ϐ��錾 --------------------------------------------------------------

extern MEMIMGMANAGE MemImgManage ;

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

// �������摜�f�[�^����������֐�( DxMemImg.cpp )
extern	int		InitializeMemImgManage( void ) ;																													// MEMIMG �Ǘ��f�[�^�̏�����

extern	int		SetMemImgDefaultColorType( int ColorType ) ;																										// �f�t�H���g�̃J���[�^�C�v���Z�b�g����
extern	int		SetMemImgDrawArea( const RECT *DrawArea ) ;																											// MEMIMG�p�̕`��͈͂�ݒ肷��
extern	int		SetMemImgDrawBlendMode( int BlendMode, int BlendParam ) ;																							// MEMIMG�p�̃u�����h���[�h�ƃu�����h�p�����[�^�̐ݒ���s��
extern	int		SetMemImgDrawBright( DWORD Bright ) ;																												// MEMIMG�p�̕`��P�x

extern	int		LoadImageToMemImg( const wchar_t *FilePath, MEMIMG *MemImg, DWORD TransColor, int ColorType = -1/*-1=��ʂɍ������t�H�[�}�b�g*/, int UsePaletteFormat = 0 ) ;			// (�⏕�֐�)MEMIMG �\���̂ɉ摜��ǂݍ���( ������Ă񂾏ꍇ InitializeMemImg �͕K�v�Ȃ��A����MemImg�͂O�N���A���Ă����K�v�͖��� )( �߂�l: -1=���s  0=���� )
extern	int		MakeMemImgScreen( MEMIMG *Img, int Width, int Height, int ColorType = -1/*-1=��ʂɍ������t�H�[�}�b�g*/  ) ;										// (�⏕�֐�)�`��Ώۗp�̃������摜���쐬����( InitializeMemImg ���ȗ������������̂��́AMemImg�͂O�N���A���Ă����K�v�͖��� )
extern	int		MakeMemImgZBuffer( MEMIMG *Img, int Width, int Height ) ;																							// (�⏕�֐�)�y�o�b�t�@�̃������C���[�W���쐬����( InitializeMemImg ���ȗ������������̂��́AMemImg�͂O�N���A���Ă����K�v�͖��� )
extern	void	BltMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, const RECT *SrcRect/* NULL:�S�� */, int DestX, int DestY ) ;										// (�⏕�֐�)MEMIMG �ԂŃf�[�^�̓]�����s��
extern	void	DerivationMemImg( MEMIMG *DestImg, MEMIMG *SrcImg, int SrcX, int SrcY, int Width, int Height ) ;													// ���� MEMIMG �̈ꕔ���g�p���� MEMIMG �̏�����������(�h�����������ɂȂ�����h�� MEMIMG ���g�p�s�ɂȂ�A�Ƃ������A��u�������̉摜���Ԏ؂肵�����Ƃ��ʂɂ����g���Ȃ�)

extern	int		InitializeMemImg( MEMIMG *Img, int Width, int Height, int Pitch/*�K��l:-1*/, DWORD TransColor, int ColorType, int UsePalette, int UseAlpha, int AnalysisFlag = TRUE, const void *UserImage = NULL ) ;		// �������摜������������A�\���̂̓[������������Ă���K�v������( �߂�l: -1=���s  0=���� )
extern	int		CheckValidMemImg( const MEMIMG *Img ) ;																												// MEMIMG ���L�����ǂ������擾����
extern	void	TerminateMemImg( MEMIMG *Img ) ;																													// �������摜�̌�n��������( ���O�ŏ����������ꍇ�͎��s����K�v�Ȃ�( �C���[�W�̈�̉�������Ȃ̂� ) )
extern	void	BltBaseImageToMemImg( const BASEIMAGE *RgbBaseImage, const BASEIMAGE *AlphaBaseImage, MEMIMG *MemImg, int SrcX, int SrcY, int Width, int Height, int DestX, int DestY, int UseTransColorConvAlpha = TRUE ) ;	// BASEIMAGE �C���[�W���� MEMIMG �C���[�W�ɉ摜�f�[�^��]������( �p���b�g������ɓ]������� )
extern	void	BltMemImgToBaseImage( BASEIMAGE *BaseImage, const MEMIMG *MemImg, int SrcX, int SrcY, int Width, int Height, int DestX, int DestY, DWORD TransColor = 0, int TransFlag = FALSE ) ;	// MEMIMG �C���[�W���� BASEIMAGE �C���[�W�ɉ摜�f�[�^��]������
extern	void	ClearMemImg(            MEMIMG *MemImg, const RECT *FillArea = NULL, unsigned int Color = 0 ) ;																		// �C���[�W������������
extern	void	DrawMemImg(             MEMIMG *DestImg, const MEMIMG *SrcImg, int DestX, int DestY,                                              int TransFlag = TRUE, const MEMIMG *BlendImg = NULL ) ;							// �C���[�W��`�悷��
extern	void	DrawTurnMemImg(         MEMIMG *DestImg, const MEMIMG *SrcImg, int XTurnFlag, int YTurnFlag, int DestX, int DestY,                int TransFlag = TRUE, const MEMIMG *BlendImg = NULL ) ;
extern	void	DrawEnlargeMemImg(      MEMIMG *DestImg, const MEMIMG *SrcImg, const RECT *DestRect,                                              int TransFlag = TRUE, const MEMIMG *BlendImg = NULL ) ;							// �C���[�W���g��`�悷��
extern	void	DrawEnlargeMemImg(      MEMIMG *DestImg, const MEMIMG *SrcImg, int x1, int y1, int x2, int y2,                                    int TransFlag = TRUE, const MEMIMG *BlendImg = NULL ) ;							// �C���[�W���g��`�悷��
extern	void 	DrawRotationMemImg(     MEMIMG *DestImg, const MEMIMG *SrcImg, int x, int y, float Angle, float EnlageRate,                       int TransFlag = TRUE, int TurnFlag = FALSE, const MEMIMG *BlendImg = NULL ) ;		// �C���[�W����]�`�悷��
extern	void	DrawBasicPolygonMemImg( MEMIMG *DestImg, const MEMIMG *SrcImg, const DX_POINTDATA *pos3,                                          int TransFlag = TRUE, const MEMIMG *BlendImg = NULL, unsigned int Color = 0xffffffff ) ;	// �C���[�W���g�p���ă|���S����`�悷��( �Q�c��Ԃɉ摜��ό`���ĕ`�悷��̂��ړI )
extern	void	DrawTransformMemImg(    MEMIMG *DestImg, const MEMIMG *SrcImg, const POINT *pos4,                                                 int TransFlag = TRUE, const MEMIMG *BlendImg = NULL ) ;							// �C���[�W��ό`�`�悷��
#ifndef DX_NON_GRAPHICS
extern	void	DrawPolygonMemImg(      MEMIMG *DestImg, MEMIMG *ZImg, const MEMIMG *SrcImg, const struct tagVERTEX_2D *Vertex3,                  int TransFlag = TRUE, const MEMIMG *BlendImg = NULL, int ZWriteFlag = TRUE, int GouraudShadeMode = TRUE, int PerspectiveEnable = TRUE, int ScissorTest = TRUE ) ;	// �C���[�W���g�p���ă|���S����`�悷��( �R�c��ԂɂR�c�|���S����`�悷��̂��ړI )
#endif // DX_NON_GRAPHICS

extern	DWORD	GetPixelColorMemImg( const MEMIMG *MemImg, int x, int y ) ;																							// �C���[�W���̎w����W�̐F���擾
extern	void	PaintMemImg(        MEMIMG *DestImg, int x, int y,                   unsigned int FillColor, ULONGLONG BoundaryColor ) ;								// �w��_���狫�E�F������Ƃ���܂œh��Ԃ�
extern	void	DrawPixelMemImg(    MEMIMG *DestImg, int x, int y,                   unsigned int Color ) ;																// �C���[�W�ɓ_��`�悷��
extern	void	DrawFillBoxMemImg(  MEMIMG *DestImg, const RECT *FillRect,           unsigned int Color ) ;																// �C���[�W�ɒ��g�̂���{�b�N�X��`�悷��
extern	void	DrawFillBoxMemImg(  MEMIMG *DestImg, int x1, int y1, int x2, int y2, unsigned int Color ) ;																// �C���[�W�ɒ��g�̂���{�b�N�X��`�悷��
extern	void	DrawLineMemImg(     MEMIMG *DestImg, int x1, int y1, int x2, int y2, unsigned int Color ) ;																// �C���[�W�ɐ���`�悷��
extern	void	DrawLineBoxMemImg(  MEMIMG *DestImg, int x1, int y1, int x2, int y2, unsigned int Color ) ;																// �C���[�W�ɘg�����̃{�b�N�X��`�悷��
extern 	void	DrawPixelSetMemImg( MEMIMG *DestImg, const POINTDATA *PointData, int Num ) ;																		// �C���[�W�ɓ_�̏W����`�悷��
extern	void	DrawLineSetMemImg(  MEMIMG *DestImg, const LINEDATA *LineData,   int Num ) ;																		// �C���[�W�ɐ��̏W����`�悷��
extern	void	DrawCircleMemImg(   MEMIMG *DestImg, int x, int y, int r,            unsigned int Color, int FillFlag ) ;													// �C���[�W�ɉ~��`�悷��
extern	void	DrawOvalMemImg(     MEMIMG *DestImg, int x, int y, int rx, int ry,   unsigned int Color, int FillFlag ) ;													// �C���[�W�ɑȉ~��`�悷��

extern	void	SetBlendGraphParamMemImg( int BorderParam, int BorderRange ) ;																						// �u�����h�O���t�B�b�N�����ɕK�v�ȃe�[�u���̃Z�b�g�A�b�v���s��
extern	COLORDATA *GetMemImgColorData( int ColorType, int UseAlpha, int UsePalette ) ;																				// �w��̃t�H�[�}�b�g�̃J���[�f�[�^�𓾂�
//#ifdef __WINDOWS__
//extern	int		CreatePixelFormat(	D_DDPIXELFORMAT *PixelFormatBuf, int ColorBitDepth, DWORD RedMask, DWORD GreenMask, DWORD BlueMask, DWORD AlphaMask ) ;			// DDPIXELFORMAT�f�[�^���쐬����
//extern	D_DDPIXELFORMAT *GetMemImgPixelFormat( int ColorType, int UseAlpha, int UsePalette ) ;																		// �w��̃t�H�[�}�b�g�̃J���[�f�[�^�𓾂�
//#endif // __WINDOWS__

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif
