//-----------------------------------------------------------------------------
// 
// 		�c�w���C�u����		WindowsOS�p���͏��v���O����
// 
//  	Ver 3.16d
// 
//-----------------------------------------------------------------------------

// �c�w���C�u�����쐬���p��`
#define __DX_MAKE

#include "../DxCompileConfig.h"

#ifndef DX_NON_INPUT

// �C���N���[�h----------------------------------------------------------------
#include "DxInputWin.h"
#include "DxWindow.h"
#include "DxWinAPI.h"
#include "DxGuid.h"
#include "../DxLog.h"
#include "../DxInput.h"
#include "../DxSystem.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����`------------------------------------------------------------------

#define DEADZONE							(DWORD)( 0.35 * 65536 )
#define DEADZONE_DIRECTINPUT( ZONE )		(DWORD)( 10000 * (ZONE) / 65536)
#define DEADZONE_WINMM( ZONE )				(DWORD)(0x8000 * (ZONE) / 65536)
#define DEADZONE_XINPUT( ZONE )				(SHORT)( 32767 * (ZONE) / 65536)
#define DEADZONE_XINPUT_TRIGGER( ZONE )		(SHORT)(   255 * (ZONE) / 65536)
#define VALIDRANGE_WINMM( ZONE )			(0x8000 - DEADZONE_WINMM(ZONE))
#define VALIDRANGE_XINPUT( ZONE )			( 32767 - DEADZONE_XINPUT(ZONE))
#define VALIDRANGE_XINPUT_TRIGGER( ZONE )	(   255 - DEADZONE_XINPUT_TRIGGER(ZONE))

#ifndef VK_OEM_PLUS
#define VK_OEM_PLUS			(0xBB)
#endif

#ifndef VK_OEM_COMMA
#define VK_OEM_COMMA		(0xBC)
#endif

#ifndef VK_OEM_MINUS
#define VK_OEM_MINUS		(0xBD)
#endif

#ifndef VK_OEM_PERIOD
#define VK_OEM_PERIOD		(0xBE)
#endif

#ifndef VK_OEM_1
#define VK_OEM_1			(0xBA)
#endif

#ifndef VK_OEM_2
#define VK_OEM_2			(0xBF)
#endif

#ifndef VK_OEM_3
#define VK_OEM_3			(0xC0)
#endif

#ifndef VK_OEM_4
#define VK_OEM_4			(0xDB)
#endif

#ifndef VK_OEM_5
#define VK_OEM_5			(0xDC)
#endif

#ifndef VK_OEM_6
#define VK_OEM_6			(0xDD)
#endif

#ifndef VK_OEM_7
#define VK_OEM_7			(0xDE)
#endif

#ifndef VK_OEM_102
#define VK_OEM_102			(0xE2)
#endif

#ifndef VK_OEM_COPY
#define VK_OEM_COPY			(0xF2)
#endif

#ifndef VK_OEM_AUTO
#define VK_OEM_AUTO			(0xF3)
#endif

#ifndef VK_OEM_ENLW
#define VK_OEM_ENLW			(0xF4)
#endif

#ifndef WHEEL_DELTA
#define WHEEL_DELTA			(120)
#endif

// �^��`----------------------------------------------------------------------

// �萔��` ----------------------------------------------------------------------

//#include "DxInputDef.h"
// �L�[�{�[�h�f�o�C�X�̃f�[�^�t�H�[�}�b�g��` =============

// �f�t�h�c
extern GUID GUIDDIKEYBOARD ;

// �f�o�C�X�I�u�W�F�N�g�f�[�^
extern D_DIOBJECTDATAFORMAT C_ODFDIKEYBOARD[256] ;

// �f�o�C�X�f�[�^�t�H�[�}�b�g
extern D_DIDATAFORMAT C_DFDIKEYBOARD ;


// �W���C�X�e�B�b�N�f�o�C�X�̃f�[�^�t�H�[�}�b�g��` =======

// �f�t�h�c
extern GUID GUIDDIJOYSTICK[8] ;

// �f�o�C�X�I�u�W�F�N�g�f�[�^
extern D_DIOBJECTDATAFORMAT C_ODFDIJOYSTICK[44] ;

// �f�o�C�X�f�[�^�t�H�[�}�b�g
extern D_DIDATAFORMAT C_DFDIJOYSTICK ;
extern D_DIDATAFORMAT C_DFDIJOYSTICK2 ;
extern D_DIDATAFORMAT C_DFDIMOUSE2 ;
extern D_DIDATAFORMAT C_DFDIMOUSE ;

// �f�[�^�錾------------------------------------------------------------------

// �L�[�{�[�h�f�o�C�X�̃f�[�^�t�H�[�}�b�g��` -----------------------

// �f�t�h�c
GUID GUIDDIKEYBOARD =
{
//	Data1		Data2	Data3	Data4
	0x55728220,	0xd33c,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00", 
} ;

// �f�o�C�X�I�u�W�F�N�g�f�[�^
D_DIOBJECTDATAFORMAT C_ODFDIKEYBOARD[256] =
{
//		pguid				dwOfs	dwType 		dwFlags
	{ 	&GUIDDIKEYBOARD, 	0x00,	0x8000000c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x01,	0x8000010c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x02,	0x8000020c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x03,	0x8000030c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x04,	0x8000040c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x05,	0x8000050c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x06,	0x8000060c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x07,	0x8000070c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x08,	0x8000080c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x09,	0x8000090c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x0a,	0x80000a0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x0b,	0x80000b0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x0c,	0x80000c0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x0d,	0x80000d0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x0e,	0x80000e0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x0f,	0x80000f0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0x10,	0x8000100c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x11,	0x8000110c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x12,	0x8000120c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x13,	0x8000130c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x14,	0x8000140c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x15,	0x8000150c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x16,	0x8000160c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x17,	0x8000170c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x18,	0x8000180c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x19,	0x8000190c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x1a,	0x80001a0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x1b,	0x80001b0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x1c,	0x80001c0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x1d,	0x80001d0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x1e,	0x80001e0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x1f,	0x80001f0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0x20,	0x8000200c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x21,	0x8000210c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x22,	0x8000220c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x23,	0x8000230c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x24,	0x8000240c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x25,	0x8000250c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x26,	0x8000260c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x27,	0x8000270c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x28,	0x8000280c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x29,	0x8000290c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x2a,	0x80002a0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x2b,	0x80002b0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x2c,	0x80002c0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x2d,	0x80002d0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x2e,	0x80002e0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x2f,	0x80002f0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0x30,	0x8000300c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x31,	0x8000310c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x32,	0x8000320c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x33,	0x8000330c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x34,	0x8000340c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x35,	0x8000350c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x36,	0x8000360c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x37,	0x8000370c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x38,	0x8000380c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x39,	0x8000390c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x3a,	0x80003a0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x3b,	0x80003b0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x3c,	0x80003c0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x3d,	0x80003d0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x3e,	0x80003e0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x3f,	0x80003f0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0x40,	0x8000400c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x41,	0x8000410c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x42,	0x8000420c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x43,	0x8000430c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x44,	0x8000440c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x45,	0x8000450c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x46,	0x8000460c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x47,	0x8000470c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x48,	0x8000480c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x49,	0x8000490c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x4a,	0x80004a0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x4b,	0x80004b0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x4c,	0x80004c0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x4d,	0x80004d0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x4e,	0x80004e0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x4f,	0x80004f0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0x50,	0x8000500c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x51,	0x8000510c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x52,	0x8000520c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x53,	0x8000530c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x54,	0x8000540c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x55,	0x8000550c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x56,	0x8000560c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x57,	0x8000570c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x58,	0x8000580c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x59,	0x8000590c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x5a,	0x80005a0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x5b,	0x80005b0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x5c,	0x80005c0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x5d,	0x80005d0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x5e,	0x80005e0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x5f,	0x80005f0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0x60,	0x8000600c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x61,	0x8000610c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x62,	0x8000620c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x63,	0x8000630c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x64,	0x8000640c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x65,	0x8000650c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x66,	0x8000660c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x67,	0x8000670c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x68,	0x8000680c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x69,	0x8000690c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x6a,	0x80006a0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x6b,	0x80006b0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x6c,	0x80006c0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x6d,	0x80006d0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x6e,	0x80006e0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x6f,	0x80006f0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0x70,	0x8000700c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x71,	0x8000710c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x72,	0x8000720c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x73,	0x8000730c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x74,	0x8000740c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x75,	0x8000750c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x76,	0x8000760c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x77,	0x8000770c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x78,	0x8000780c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x79,	0x8000790c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x7a,	0x80007a0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x7b,	0x80007b0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x7c,	0x80007c0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x7d,	0x80007d0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x7e,	0x80007e0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x7f,	0x80007f0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0x80,	0x8000800c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x81,	0x8000810c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x82,	0x8000820c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x83,	0x8000830c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x84,	0x8000840c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x85,	0x8000850c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x86,	0x8000860c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x87,	0x8000870c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x88,	0x8000880c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x89,	0x8000890c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x8a,	0x80008a0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x8b,	0x80008b0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x8c,	0x80008c0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x8d,	0x80008d0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x8e,	0x80008e0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x8f,	0x80008f0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0x90,	0x8000900c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x91,	0x8000910c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x92,	0x8000920c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x93,	0x8000930c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x94,	0x8000940c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x95,	0x8000950c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x96,	0x8000960c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x97,	0x8000970c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x98,	0x8000980c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x99,	0x8000990c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x9a,	0x80009a0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x9b,	0x80009b0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x9c,	0x80009c0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x9d,	0x80009d0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x9e,	0x80009e0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0x9f,	0x80009f0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0xa0,	0x8000a00c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xa1,	0x8000a10c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xa2,	0x8000a20c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xa3,	0x8000a30c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xa4,	0x8000a40c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xa5,	0x8000a50c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xa6,	0x8000a60c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xa7,	0x8000a70c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xa8,	0x8000a80c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xa9,	0x8000a90c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xaa,	0x8000aa0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xab,	0x8000ab0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xac,	0x8000ac0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xad,	0x8000ad0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xae,	0x8000ae0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xaf,	0x8000af0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0xb0,	0x8000b00c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xb1,	0x8000b10c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xb2,	0x8000b20c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xb3,	0x8000b30c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xb4,	0x8000b40c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xb5,	0x8000b50c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xb6,	0x8000b60c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xb7,	0x8000b70c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xb8,	0x8000b80c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xb9,	0x8000b90c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xba,	0x8000ba0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xbb,	0x8000bb0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xbc,	0x8000bc0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xbd,	0x8000bd0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xbe,	0x8000be0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xbf,	0x8000bf0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0xc0,	0x8000c00c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xc1,	0x8000c10c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xc2,	0x8000c20c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xc3,	0x8000c30c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xc4,	0x8000c40c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xc5,	0x8000c50c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xc6,	0x8000c60c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xc7,	0x8000c70c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xc8,	0x8000c80c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xc9,	0x8000c90c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xca,	0x8000ca0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xcb,	0x8000cb0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xcc,	0x8000cc0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xcd,	0x8000cd0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xce,	0x8000ce0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xcf,	0x8000cf0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0xd0,	0x8000d00c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xd1,	0x8000d10c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xd2,	0x8000d20c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xd3,	0x8000d30c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xd4,	0x8000d40c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xd5,	0x8000d50c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xd6,	0x8000d60c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xd7,	0x8000d70c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xd8,	0x8000d80c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xd9,	0x8000d90c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xda,	0x8000da0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xdb,	0x8000db0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xdc,	0x8000dc0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xdd,	0x8000dd0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xde,	0x8000de0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xdf,	0x8000df0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0xe0,	0x8000e00c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xe1,	0x8000e10c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xe2,	0x8000e20c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xe3,	0x8000e30c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xe4,	0x8000e40c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xe5,	0x8000e50c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xe6,	0x8000e60c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xe7,	0x8000e70c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xe8,	0x8000e80c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xe9,	0x8000e90c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xea,	0x8000ea0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xeb,	0x8000eb0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xec,	0x8000ec0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xed,	0x8000ed0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xee,	0x8000ee0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xef,	0x8000ef0c,	0x0 },

	{ 	&GUIDDIKEYBOARD, 	0xf0,	0x8000f00c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xf1,	0x8000f10c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xf2,	0x8000f20c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xf3,	0x8000f30c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xf4,	0x8000f40c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xf5,	0x8000f50c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xf6,	0x8000f60c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xf7,	0x8000f70c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xf8,	0x8000f80c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xf9,	0x8000f90c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xfa,	0x8000fa0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xfb,	0x8000fb0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xfc,	0x8000fc0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xfd,	0x8000fd0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xfe,	0x8000fe0c,	0x0 },
	{ 	&GUIDDIKEYBOARD, 	0xff,	0x8000ff0c,	0x0 },
} ;

// �f�o�C�X�f�[�^�t�H�[�}�b�g
D_DIDATAFORMAT C_DFDIKEYBOARD =
{
#ifdef _WIN64
	32, 	// dwSize
	24,		// dwObjSize
#else
	24, 	// dwSize
	16,		// dwObjSize
#endif
	0x02,	// dwFlags
	256,	// dwDataSize
	256,	// dwNumObjs
	C_ODFDIKEYBOARD, // rgodf
} ;


// �W���C�X�e�B�b�N�f�o�C�X�̃f�[�^�t�H�[�}�b�g��` -----------------

// �f�t�h�c
GUID GUID_X_AXIS	= { 0xa36d02e0, 0xc9f3, 0x11cf, "\xbf\xc7\x44\x45\x53\x54\x00" };
GUID GUID_Y_AXIS	= { 0xa36d02e1,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00" };
GUID GUID_Z_AXIS	= { 0xa36d02e2,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00" };
GUID GUID_RX_AXIS	= { 0xa36d02f4,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00" };
GUID GUID_RY_AXIS	= { 0xa36d02f5,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00" };
GUID GUID_RZ_AXIS	= { 0xa36d02e3,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00" };
GUID GUID_SLIDER	= { 0xa36d02e4,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00" };
GUID GUID_POV		= { 0xa36d02f2,	0xc9f3,	0x11cf,	"\xbf\xc7\x44\x45\x53\x54\x00" };

// �W���C�X�e�B�b�N�P�f�o�C�X�I�u�W�F�N�g�f�[�^
D_DIOBJECTDATAFORMAT C_ODFDIJOYSTICK[44] =
{
//	pguid				dwOfs	dwType		dwFlags
	&GUID_X_AXIS,		0x00,	0x80ffff03,	0x100,
	&GUID_Y_AXIS,		0x04,	0x80ffff03,	0x100,
	&GUID_Z_AXIS,		0x08,	0x80ffff03,	0x100,
	&GUID_RX_AXIS,		0x0c,	0x80ffff03,	0x100,
	&GUID_RY_AXIS,		0x10,	0x80ffff03,	0x100,
	&GUID_RZ_AXIS,		0x14,	0x80ffff03,	0x100,
	&GUID_SLIDER,		0x18,	0x80ffff03,	0x100,
	&GUID_SLIDER,		0x1c,	0x80ffff03,	0x100,
	&GUID_POV,			0x20,	0x80ffff10,	0x000,
	&GUID_POV,			0x24,	0x80ffff10,	0x000,
	&GUID_POV,			0x28,	0x80ffff10,	0x000,
	&GUID_POV,			0x2c,	0x80ffff10,	0x000,
	NULL,				0x30,	0x80ffff0c,	0x000,
	NULL,				0x31,	0x80ffff0c,	0x000,
	NULL,				0x32,	0x80ffff0c,	0x000,
	NULL,				0x33,	0x80ffff0c,	0x000,
	NULL,				0x34,	0x80ffff0c,	0x000,
	NULL,				0x35,	0x80ffff0c,	0x000,
	NULL,				0x36,	0x80ffff0c,	0x000,
	NULL,				0x37,	0x80ffff0c,	0x000,
	NULL,				0x38,	0x80ffff0c,	0x000,
	NULL,				0x39,	0x80ffff0c,	0x000,
	NULL,				0x3a,	0x80ffff0c,	0x000,
	NULL,				0x3b,	0x80ffff0c,	0x000,
	NULL,				0x3c,	0x80ffff0c,	0x000,
	NULL,				0x3d,	0x80ffff0c,	0x000,
	NULL,				0x3e,	0x80ffff0c,	0x000,
	NULL,				0x3f,	0x80ffff0c,	0x000,
	NULL,				0x40,	0x80ffff0c,	0x000,
	NULL,				0x41,	0x80ffff0c,	0x000,
	NULL,				0x42,	0x80ffff0c,	0x000,
	NULL,				0x43,	0x80ffff0c,	0x000,
	NULL,				0x44,	0x80ffff0c,	0x000,
	NULL,				0x45,	0x80ffff0c,	0x000,
	NULL,				0x46,	0x80ffff0c,	0x000,
	NULL,				0x47,	0x80ffff0c,	0x000,
	NULL,				0x48,	0x80ffff0c,	0x000,
	NULL,				0x49,	0x80ffff0c,	0x000,
	NULL,				0x4a,	0x80ffff0c,	0x000,
	NULL,				0x4b,	0x80ffff0c,	0x000,
	NULL,				0x4c,	0x80ffff0c,	0x000,
	NULL,				0x4d,	0x80ffff0c,	0x000,
	NULL,				0x4e,	0x80ffff0c,	0x000,
	NULL,				0x4f,	0x80ffff0c,	0x000,
} ;

// �W���C�X�e�B�b�N�P�f�o�C�X�f�[�^�t�H�[�}�b�g
D_DIDATAFORMAT C_DFDIJOYSTICK =
{
#ifdef _WIN64
	32, 	// dwSize
	24,		// dwObjSize
#else
	24, 	// dwSize
	16,		// dwObjSize
#endif
	0x01,	// dwFlags
	80,		// dwDataSize
	44,		// dwNumObjs
	C_ODFDIJOYSTICK, // rgodf
} ;

// �W���C�X�e�B�b�N�Q�f�o�C�X�I�u�W�F�N�g�f�[�^
D_DIOBJECTDATAFORMAT C_ODFDIJOYSTICK2[164] =
{
//	pguid				dwOfs	dwType		dwFlags
	&GUID_X_AXIS,		0x00,	0x80ffff03,	0x100,
	&GUID_Y_AXIS,		0x04,	0x80ffff03,	0x100,
	&GUID_Z_AXIS,		0x08,	0x80ffff03,	0x100,
	&GUID_RX_AXIS,		0x0c,	0x80ffff03,	0x100,
	&GUID_RY_AXIS,		0x10,	0x80ffff03,	0x100,
	&GUID_RZ_AXIS,		0x14,	0x80ffff03,	0x100,
	&GUID_SLIDER,		0x18,	0x80ffff03,	0x100,
	&GUID_SLIDER,		0x1c,	0x80ffff03,	0x100,
	&GUID_POV,			0x20,	0x80ffff10,	0x000,
	&GUID_POV,			0x24,	0x80ffff10,	0x000,
	&GUID_POV,			0x28,	0x80ffff10,	0x000,
	&GUID_POV,			0x2c,	0x80ffff10,	0x000,
	NULL,				0x30,	0x80ffff0c,	0x000,
	NULL,				0x31,	0x80ffff0c,	0x000,
	NULL,				0x32,	0x80ffff0c,	0x000,
	NULL,				0x33,	0x80ffff0c,	0x000,
	NULL,				0x34,	0x80ffff0c,	0x000,
	NULL,				0x35,	0x80ffff0c,	0x000,
	NULL,				0x36,	0x80ffff0c,	0x000,
	NULL,				0x37,	0x80ffff0c,	0x000,
	NULL,				0x38,	0x80ffff0c,	0x000,
	NULL,				0x39,	0x80ffff0c,	0x000,
	NULL,				0x3a,	0x80ffff0c,	0x000,
	NULL,				0x3b,	0x80ffff0c,	0x000,
	NULL,				0x3c,	0x80ffff0c,	0x000,
	NULL,				0x3d,	0x80ffff0c,	0x000,
	NULL,				0x3e,	0x80ffff0c,	0x000,
	NULL,				0x3f,	0x80ffff0c,	0x000,
	NULL,				0x40,	0x80ffff0c,	0x000,
	NULL,				0x41,	0x80ffff0c,	0x000,
	NULL,				0x42,	0x80ffff0c,	0x000,
	NULL,				0x43,	0x80ffff0c,	0x000,
	NULL,				0x44,	0x80ffff0c,	0x000,
	NULL,				0x45,	0x80ffff0c,	0x000,
	NULL,				0x46,	0x80ffff0c,	0x000,
	NULL,				0x47,	0x80ffff0c,	0x000,
	NULL,				0x48,	0x80ffff0c,	0x000,
	NULL,				0x49,	0x80ffff0c,	0x000,
	NULL,				0x4a,	0x80ffff0c,	0x000,
	NULL,				0x4b,	0x80ffff0c,	0x000,
	NULL,				0x4c,	0x80ffff0c,	0x000,
	NULL,				0x4d,	0x80ffff0c,	0x000,
	NULL,				0x4e,	0x80ffff0c,	0x000,
	NULL,				0x4f,	0x80ffff0c,	0x000,
	NULL,				0x50,	0x80ffff0c,	0x000,
	NULL,				0x51,	0x80ffff0c,	0x000,
	NULL,				0x52,	0x80ffff0c,	0x000,
	NULL,				0x53,	0x80ffff0c,	0x000,
	NULL,				0x54,	0x80ffff0c,	0x000,
	NULL,				0x55,	0x80ffff0c,	0x000,
	NULL,				0x56,	0x80ffff0c,	0x000,
	NULL,				0x57,	0x80ffff0c,	0x000,
	NULL,				0x58,	0x80ffff0c,	0x000,
	NULL,				0x59,	0x80ffff0c,	0x000,
	NULL,				0x5a,	0x80ffff0c,	0x000,
	NULL,				0x5b,	0x80ffff0c,	0x000,
	NULL,				0x5c,	0x80ffff0c,	0x000,
	NULL,				0x5d,	0x80ffff0c,	0x000,
	NULL,				0x5e,	0x80ffff0c,	0x000,
	NULL,				0x5f,	0x80ffff0c,	0x000,
	NULL,				0x60,	0x80ffff0c,	0x000,
	NULL,				0x61,	0x80ffff0c,	0x000,
	NULL,				0x62,	0x80ffff0c,	0x000,
	NULL,				0x63,	0x80ffff0c,	0x000,
	NULL,				0x64,	0x80ffff0c,	0x000,
	NULL,				0x65,	0x80ffff0c,	0x000,
	NULL,				0x66,	0x80ffff0c,	0x000,
	NULL,				0x67,	0x80ffff0c,	0x000,
	NULL,				0x68,	0x80ffff0c,	0x000,
	NULL,				0x69,	0x80ffff0c,	0x000,
	NULL,				0x6a,	0x80ffff0c,	0x000,
	NULL,				0x6b,	0x80ffff0c,	0x000,
	NULL,				0x6c,	0x80ffff0c,	0x000,
	NULL,				0x6d,	0x80ffff0c,	0x000,
	NULL,				0x6e,	0x80ffff0c,	0x000,
	NULL,				0x6f,	0x80ffff0c,	0x000,
	NULL,				0x70,	0x80ffff0c,	0x000,
	NULL,				0x71,	0x80ffff0c,	0x000,
	NULL,				0x72,	0x80ffff0c,	0x000,
	NULL,				0x73,	0x80ffff0c,	0x000,
	NULL,				0x74,	0x80ffff0c,	0x000,
	NULL,				0x75,	0x80ffff0c,	0x000,
	NULL,				0x76,	0x80ffff0c,	0x000,
	NULL,				0x77,	0x80ffff0c,	0x000,
	NULL,				0x78,	0x80ffff0c,	0x000,
	NULL,				0x79,	0x80ffff0c,	0x000,
	NULL,				0x7a,	0x80ffff0c,	0x000,
	NULL,				0x7b,	0x80ffff0c,	0x000,
	NULL,				0x7c,	0x80ffff0c,	0x000,
	NULL,				0x7d,	0x80ffff0c,	0x000,
	NULL,				0x7e,	0x80ffff0c,	0x000,
	NULL,				0x7f,	0x80ffff0c,	0x000,
	NULL,				0x80,	0x80ffff0c,	0x000,
	NULL,				0x81,	0x80ffff0c,	0x000,
	NULL,				0x82,	0x80ffff0c,	0x000,
	NULL,				0x83,	0x80ffff0c,	0x000,
	NULL,				0x84,	0x80ffff0c,	0x000,
	NULL,				0x85,	0x80ffff0c,	0x000,
	NULL,				0x86,	0x80ffff0c,	0x000,
	NULL,				0x87,	0x80ffff0c,	0x000,
	NULL,				0x88,	0x80ffff0c,	0x000,
	NULL,				0x89,	0x80ffff0c,	0x000,
	NULL,				0x8a,	0x80ffff0c,	0x000,
	NULL,				0x8b,	0x80ffff0c,	0x000,
	NULL,				0x8c,	0x80ffff0c,	0x000,
	NULL,				0x8d,	0x80ffff0c,	0x000,
	NULL,				0x8e,	0x80ffff0c,	0x000,
	NULL,				0x8f,	0x80ffff0c,	0x000,
	NULL,				0x90,	0x80ffff0c,	0x000,
	NULL,				0x91,	0x80ffff0c,	0x000,
	NULL,				0x92,	0x80ffff0c,	0x000,
	NULL,				0x93,	0x80ffff0c,	0x000,
	NULL,				0x94,	0x80ffff0c,	0x000,
	NULL,				0x95,	0x80ffff0c,	0x000,
	NULL,				0x96,	0x80ffff0c,	0x000,
	NULL,				0x97,	0x80ffff0c,	0x000,
	NULL,				0x98,	0x80ffff0c,	0x000,
	NULL,				0x99,	0x80ffff0c,	0x000,
	NULL,				0x9a,	0x80ffff0c,	0x000,
	NULL,				0x9b,	0x80ffff0c,	0x000,
	NULL,				0x9c,	0x80ffff0c,	0x000,
	NULL,				0x9d,	0x80ffff0c,	0x000,
	NULL,				0x9e,	0x80ffff0c,	0x000,
	NULL,				0x9f,	0x80ffff0c,	0x000,
	NULL,				0xa0,	0x80ffff0c,	0x000,
	NULL,				0xa1,	0x80ffff0c,	0x000,
	NULL,				0xa2,	0x80ffff0c,	0x000,
	NULL,				0xa3,	0x80ffff0c,	0x000,
	NULL,				0xa4,	0x80ffff0c,	0x000,
	NULL,				0xa5,	0x80ffff0c,	0x000,
	NULL,				0xa6,	0x80ffff0c,	0x000,
	NULL,				0xa7,	0x80ffff0c,	0x000,
	NULL,				0xa8,	0x80ffff0c,	0x000,
	NULL,				0xa9,	0x80ffff0c,	0x000,
	NULL,				0xaa,	0x80ffff0c,	0x000,
	NULL,				0xab,	0x80ffff0c,	0x000,
	NULL,				0xac,	0x80ffff0c,	0x000,
	NULL,				0xad,	0x80ffff0c,	0x000,
	NULL,				0xae,	0x80ffff0c,	0x000,
	NULL,				0xaf,	0x80ffff0c,	0x000,
	&GUID_X_AXIS,		0xb0,	0x80ffff03,	0x200,
	&GUID_Y_AXIS,		0xb4,	0x80ffff03,	0x200,
	&GUID_Z_AXIS,		0xb8,	0x80ffff03,	0x200,
	&GUID_RX_AXIS,		0xbc,	0x80ffff03,	0x200,
	&GUID_RY_AXIS,		0xc0,	0x80ffff03,	0x200,
	&GUID_RZ_AXIS,		0xc4,	0x80ffff03,	0x200,
	&GUID_SLIDER,		0x18,	0x80ffff03,	0x200,
	&GUID_SLIDER,		0x1c,	0x80ffff03,	0x200,
	&GUID_X_AXIS,		0xd0,	0x80ffff03,	0x300,
	&GUID_Y_AXIS,		0xd4,	0x80ffff03,	0x300,
	&GUID_Z_AXIS,		0xd8,	0x80ffff03,	0x300,
	&GUID_RX_AXIS,		0xdc,	0x80ffff03,	0x300,
	&GUID_RY_AXIS,		0xe0,	0x80ffff03,	0x300,
	&GUID_RZ_AXIS,		0xe4,	0x80ffff03,	0x300,
	&GUID_SLIDER,		0x18,	0x80ffff03,	0x300,
	&GUID_SLIDER,		0x1c,	0x80ffff03,	0x300,
	&GUID_X_AXIS,		0xf0,	0x80ffff03,	0x400,
	&GUID_Y_AXIS,		0xf4,	0x80ffff03,	0x400,
	&GUID_Z_AXIS,		0xf8,	0x80ffff03,	0x400,
	&GUID_RX_AXIS,		0xfc,	0x80ffff03,	0x400,
	&GUID_RY_AXIS,		0x00,	0x80ffff03,	0x400,
	&GUID_RZ_AXIS,		0x04,	0x80ffff03,	0x400,
	&GUID_SLIDER,		0x18,	0x80ffff03,	0x400,
	&GUID_SLIDER,		0x1c,	0x80ffff03,	0x400,
} ;

// �W���C�X�e�B�b�N�Q�f�o�C�X�f�[�^�t�H�[�}�b�g
D_DIDATAFORMAT C_DFDIJOYSTICK2 =
{
#ifdef _WIN64
	32, 	// dwSize
	24,		// dwObjSize
#else
	24, 	// dwSize
	16,		// dwObjSize
#endif
	0x01,	// dwFlags
	272,	// dwDataSize
	164,	// dwNumObjs
	C_ODFDIJOYSTICK2, // rgodf
} ;


GUID GUIDDIMOUSE2[3] =
{
	0xa36d02e0, 0xc9f3, 0x11cf, 0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00, 
	0xa36d02e1, 0xc9f3, 0x11cf, 0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00, 
	0xa36d02e2, 0xc9f3, 0x11cf, 0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00, 
} ;

D_DIOBJECTDATAFORMAT C_ODFDIMOUSE2[11] =
{
	&GUIDDIMOUSE2[0],	0x00, 0x00ffff03, 0x000,
	&GUIDDIMOUSE2[1],	0x04, 0x00ffff03, 0x000,
	&GUIDDIMOUSE2[2],	0x08, 0x80ffff03, 0x000,
	NULL,				0x0c, 0x00ffff0c, 0x000,
	NULL,				0x0d, 0x00ffff0c, 0x000,
	NULL,				0x0e, 0x80ffff0c, 0x000,
	NULL, 				0x0f, 0x80ffff0c, 0x000,
	NULL, 				0x10, 0x80ffff0c, 0x000,
	NULL, 				0x11, 0x80ffff0c, 0x000,
	NULL, 				0x12, 0x80ffff0c, 0x000,
	NULL, 				0x13, 0x80ffff0c, 0x000,
} ;

D_DIDATAFORMAT C_DFDIMOUSE2 =
{
#ifdef _WIN64
	32, 	// dwSize
	24,		// dwObjSize
#else
	24, 	// dwSize
	16,		// dwObjSize
#endif
	2,		// dwFlags
	20,		// dwDataSize
	11,		// dwNumObjs
	C_ODFDIMOUSE2,
} ;


GUID GUIDDIMOUSE[3] =
{
	0xa36d02e0, 0xc9f3, 0x11cf, 0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00, 
	0xa36d02e1, 0xc9f3, 0x11cf, 0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00, 
	0xa36d02e2, 0xc9f3, 0x11cf, 0xbf, 0xc7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00, 
} ;

D_DIOBJECTDATAFORMAT C_ODFDIMOUSE[7] =
{
	&GUIDDIMOUSE[0],	0x00, 0x00ffff03, 0x000,
	&GUIDDIMOUSE[1],	0x04, 0x00ffff03, 0x000,
	&GUIDDIMOUSE[2],	0x08, 0x80ffff03, 0x000,
	NULL,				0x0c, 0x00ffff0c, 0x000,
	NULL,				0x0d, 0x00ffff0c, 0x000,
	NULL,				0x0e, 0x80ffff0c, 0x000,
	NULL, 				0x0f, 0x80ffff0c, 0x000,
} ;

D_DIDATAFORMAT C_DFDIMOUSE =
{
#ifdef _WIN64
	32, 	// dwSize
	24,		// dwObjSize
#else
	24, 	// dwSize
	16,		// dwObjSize
#endif
	2,		// dwFlags
	16,		// dwDataSize
	7,		// dwNumObjs
	C_ODFDIMOUSE,
} ;

const static unsigned short __KeyMap[][3 /* 0:Windows���z�L�[�R�[�h  1:DirectInput�L�[�R�[�h  2:DirectInput�g�p���Ɏg�p���邩�ۂ� */ ] =
{
	'A',			D_DIK_A,			0,	// �`�L�[
	'B',			D_DIK_B,			0,	// �a�L�[
	'C',			D_DIK_C,			0,	// �b�L�[
	'D',			D_DIK_D,			0,	// �c�L�[
	'E',			D_DIK_E,			0,	// �d�L�[
	'F',			D_DIK_F,			0,	// �e�L�[
	'G',			D_DIK_G,			0,	// �f�L�[
	'H',			D_DIK_H,			0,	// �g�L�[
	'I',			D_DIK_I,			0,	// �h�L�[
	'J',			D_DIK_J,			0,	// �i�L�[
	'K',			D_DIK_K,			0,	// �j�L�[
	'L',			D_DIK_L,			0,	// �k�L�[
	'M',			D_DIK_M,			0,	// �l�L�[
	'N',			D_DIK_N,			0,	// �m�L�[
	'O',			D_DIK_O,			0,	// �n�L�[
	'P',			D_DIK_P,			0,	// �o�L�[
	'Q',			D_DIK_Q,			0,	// �p�L�[
	'R',			D_DIK_R,			0,	// �q�L�[
	'S',			D_DIK_S,			0,	// �r�L�[
	'T',			D_DIK_T,			0,	// �s�L�[
	'U',			D_DIK_U,			0,	// �t�L�[
	'V',			D_DIK_V,			0,	// �u�L�[
	'W',			D_DIK_W,			0,	// �v�L�[
	'X',			D_DIK_X,			0,	// �w�L�[
	'Y',			D_DIK_Y,			0,	// �x�L�[
	'Z',			D_DIK_Z,			0,	// �y�L�[
	'0',			D_DIK_0,			0,	// �O�L�[
	'1',			D_DIK_1,			0,	// �P�L�[
	'2',			D_DIK_2,			0,	// �Q�L�[
	'3',			D_DIK_3,			0,	// �R�L�[
	'4',			D_DIK_4,			0,	// �S�L�[
	'5',			D_DIK_5,			0,	// �T�L�[
	'6',			D_DIK_6,			0,	// �U�L�[
	'7',			D_DIK_7,			0,	// �V�L�[
	'8',			D_DIK_8,			0,	// �W�L�[
	'9',			D_DIK_9,			0,	// �X�L�[

	VK_LEFT,		D_DIK_LEFT,			1,	// ���L�[
	VK_UP,			D_DIK_UP,			1,	// ��L�[
	VK_RIGHT,		D_DIK_RIGHT,		1,	// �E�L�[
	VK_DOWN,		D_DIK_DOWN,			1,	// ���L�[

	VK_OEM_PLUS,	D_DIK_SEMICOLON,	0,	// �G�L�[
	VK_OEM_1,		D_DIK_COLON,		0,	// �F�L�[
	VK_OEM_4,		D_DIK_LBRACKET,		0,	// �m�L�[
	VK_OEM_6,		D_DIK_RBRACKET,		0,	// �n�L�[
	VK_OEM_3,		D_DIK_AT,			0,	// ���L�[
	VK_OEM_102,		D_DIK_BACKSLASH,	0,	// �_�L�[
	VK_OEM_COMMA,	D_DIK_COMMA,		0,	// �C�L�[
	VK_OEM_7,		D_DIK_PREVTRACK,	0,	// �O���[

	VK_OEM_MINUS,	D_DIK_MINUS,		0,	// �|�L�[
	VK_OEM_5,		D_DIK_YEN,			0,	// ���L�[
	VK_OEM_PERIOD,	D_DIK_PERIOD,		0,	// �D�L�[
	VK_OEM_2,		D_DIK_SLASH,		0,	// �^�L�[

	VK_DELETE,		D_DIK_DELETE,		0,	// �f���[�g�L�[

	VK_ESCAPE,		D_DIK_ESCAPE,		0,	// �G�X�P�[�v�L�[
	VK_SPACE,		D_DIK_SPACE,		0,	// �X�y�[�X�L�[

	VK_BACK, 		D_DIK_BACK,			0,	// �o�b�N�X�y�[�X�L�[
	VK_TAB, 		D_DIK_TAB,			0,	// �^�u�L�[
	VK_RETURN,		D_DIK_RETURN,		0,	// �G���^�[�L�[

	VK_NUMLOCK,		D_DIK_NUMLOCK,		1,	// �e���L�[NumLock�L�[
	VK_NUMPAD0,		D_DIK_NUMPAD0,		1,	// �e���L�[�O
	VK_NUMPAD1,		D_DIK_NUMPAD1,		1,	// �e���L�[�P
	VK_NUMPAD2,		D_DIK_NUMPAD2,		1,	// �e���L�[�Q
	VK_NUMPAD3,		D_DIK_NUMPAD3,		1,	// �e���L�[�R
	VK_NUMPAD4,		D_DIK_NUMPAD4,		1,	// �e���L�[�S
	VK_NUMPAD5,		D_DIK_NUMPAD5,		1,	// �e���L�[�T
	VK_NUMPAD6,		D_DIK_NUMPAD6,		1,	// �e���L�[�U
	VK_NUMPAD7,		D_DIK_NUMPAD7,		1,	// �e���L�[�V
	VK_NUMPAD8,		D_DIK_NUMPAD8,		1,	// �e���L�[�W
	VK_NUMPAD9,		D_DIK_NUMPAD9,		1,	// �e���L�[�X
	VK_MULTIPLY,	D_DIK_MULTIPLY,		1,	// �e���L�[���L�[
	VK_ADD,			D_DIK_ADD,			1,	// �e���L�[�{�L�[
	VK_SUBTRACT,	D_DIK_SUBTRACT,		1,	// �e���L�[�|�L�[
	VK_DECIMAL,		D_DIK_DECIMAL,		1,	// �e���L�[�D�L�[
	VK_DIVIDE,		D_DIK_DIVIDE,		1,	// �e���L�[�^�L�[

	VK_RETURN,		D_DIK_NUMPADENTER,	1,	// �e���L�[�̃G���^�[�L�[
//	VK_NUMPADENTER,	D_DIK_NUMPADENTER,	1,	// �e���L�[�̃G���^�[�L�[

	VK_LSHIFT,		D_DIK_LSHIFT,		1,	// ���V�t�g�L�[
	VK_RSHIFT,		D_DIK_RSHIFT,		1,	// �E�V�t�g�L�[
	VK_LCONTROL,	D_DIK_LCONTROL,		1,	// ���R���g���[���L�[
	VK_RCONTROL,	D_DIK_RCONTROL,		1,	// �E�R���g���[���L�[
	VK_PRIOR,		D_DIK_PGUP,			1,	// �o�������t�o�L�[
	VK_NEXT,		D_DIK_PGDN,			1,	// �o�������c�������L�[
	VK_END,			D_DIK_END,			1,	// �G���h�L�[
	VK_HOME,		D_DIK_HOME,			1,	// �z�[���L�[
	VK_INSERT,		D_DIK_INSERT,		1,	// �C���T�[�g�L�[

	VK_LMENU,		D_DIK_LALT,			1,	// ���`�k�s�L�[
	VK_RMENU,		D_DIK_RALT,			1,	// �E�`�k�s�L�[
	VK_SCROLL,		D_DIK_SCROLL,		1,	// ScrollLock�L�[
	VK_APPS,		D_DIK_APPS,			1,	// �A�v���P�[�V�������j���[�L�[
	VK_CAPITAL,		D_DIK_CAPSLOCK,		1,	// CaspLock�L�[
	VK_SNAPSHOT,	D_DIK_SYSRQ,		1,	// PrintScreen�L�[
	VK_PAUSE,		D_DIK_PAUSE,		1,	// PauseBreak�L�[
	VK_LWIN,		D_DIK_LWIN,			1,	// ���v�����L�[
	VK_RWIN,		D_DIK_RWIN,			1,	// �E�v�����L�[

	VK_OEM_ENLW,	D_DIK_KANJI,		1,	// ���p�^�S�p�L�[
	VK_CONVERT,		D_DIK_CONVERT,		1,	// �ϊ��L�[
	VK_NONCONVERT,	D_DIK_NOCONVERT,	1,	// ���ϊ��L�[
	VK_OEM_COPY,	D_DIK_KANA,			0,	// �J�i�L�[

	VK_F1,			D_DIK_F1,			1,	// �e�P�L�[
	VK_F2,			D_DIK_F2,			1,	// �e�Q�L�[
	VK_F3,			D_DIK_F3,			1,	// �e�R�L�[
	VK_F4,			D_DIK_F4,			1,	// �e�S�L�[
	VK_F5,			D_DIK_F5,			1,	// �e�T�L�[
	VK_F6,			D_DIK_F6,			1,	// �e�U�L�[
	VK_F7,			D_DIK_F7,			1,	// �e�V�L�[
	VK_F8,			D_DIK_F8,			1,	// �e�W�L�[
	VK_F9,			D_DIK_F9,			1,	// �e�X�L�[
	VK_F10,			D_DIK_F10,			1,	// �e�P�O�L�[
	VK_F11,			D_DIK_F11,			1,	// �e�P�P�L�[
	VK_F12,			D_DIK_F12,			1,	// �e�P�Q�L�[

	0xffff,			0xffff,
} ;

#define XINPUT_TO_DIRECTINPUT_BUTTONNUM		(10)
WORD XInputButtonToDirectInputButtonNo[ XINPUT_TO_DIRECTINPUT_BUTTONNUM ] =
{
	D_XINPUT_GAMEPAD_A,
	D_XINPUT_GAMEPAD_B,
	D_XINPUT_GAMEPAD_X,
	D_XINPUT_GAMEPAD_Y,
	D_XINPUT_GAMEPAD_LEFT_SHOULDER,
	D_XINPUT_GAMEPAD_RIGHT_SHOULDER,
	D_XINPUT_GAMEPAD_BACK,
	D_XINPUT_GAMEPAD_START,
	D_XINPUT_GAMEPAD_LEFT_THUMB,
	D_XINPUT_GAMEPAD_RIGHT_THUMB
} ;

// �֐��v���g�^�C�v�錾 -------------------------------------------------------

// �W���C�p�b�h�񋓗p�R�[���o�b�N�֐�
		BOOL FAR PASCAL EnumJoypadProc( const D_DIDEVICEINSTANCEW *pdinst , LPVOID pvRef ) ;

// �v���O����------------------------------------------------------------------

BOOL CALLBACK EffectEnumCallBack( const D_DIEFFECTINFOA * /*Info*/, void * /*Data*/ )
{
	return D_DIENUM_CONTINUE ;
}

// �W���C�p�b�h�񋓗p�R�[���o�b�N�֐�
BOOL FAR PASCAL EnumJoypadProc( const D_DIDEVICEINSTANCEW *pdinst , LPVOID /*pvRef*/ )
{
	HRESULT hr ;
	D_IDirectInputDevice7 *Joystick ;
	D_DIPROPRANGE diprg ;
	D_DIPROPDWORD dipdw ;
	HANDLE Event ;
	INPUTPADDATA *pad ;
	int i ;

	pad = &InputSysData.Pad[ InputSysData.PadNum ] ;

	// �W���C�X�e�B�b�N�f�o�C�X�̍쐬
	pad->PF.XInputDeviceNo = -1 ;
	if( InputSysData.PF.UseDirectInput8Flag == TRUE )
	{
		hr = InputSysData.PF.DirectInputObject->CreateDevice( pdinst->guidInstance, ( D_IDirectInputDevice ** )&Joystick , NULL ) ;
	}
	else
	{
		hr = InputSysData.PF.DirectInputObject->CreateDeviceEx( pdinst->guidInstance, IID_IDIRECTINPUTDEVICE7, ( void ** )&Joystick , NULL ) ;
	}
	if( hr != D_DI_OK )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\x65\x51\x9b\x52\xc5\x88\x6e\x7f\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���͑��u�̎擾�Ɏ��s���܂���\n" @*/ ) ;
		return D_DIENUM_CONTINUE ;
	}

	// ���\��
	{
		D_DIDEVICEINSTANCEW State ;

		_MEMSET( &State, 0, sizeof( State ) ) ;
		State.dwSize = sizeof( State ) ;
		Joystick->GetDeviceInfo( &State ) ;

		// Xbox360 �R���g���[���̏ꍇ�ŁAXInput�Ō��o����Ă�����e��
		if( _WCSSTR( State.tszInstanceName, L"XBOX 360 For Windows" ) != NULL )
		{
			int i ;

			for( i = 0 ; i < InputSysData.PadNum ; i ++ )
			{
				if( InputSysData.Pad[ i ].PF.XInputDeviceNo >= 0 )
					break ;
			}
			if( i != InputSysData.PadNum )
			{
				Joystick->Release() ;
				Joystick = NULL ;
				return D_DIENUM_CONTINUE ;
			}
		}

		DXST_ERRORLOG_ADDUTF16LE( "\x65\x51\x9b\x52\xc5\x88\x6e\x7f\x92\x30\x8b\x89\x64\x30\x51\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���͑��u�������܂���\n" @*/ ) ;

		_WCSCPY_S( pad->PF.InstanceName, sizeof( pad->PF.InstanceName ), State.tszInstanceName );
		_WCSCPY_S( pad->PF.ProductName,  sizeof( pad->PF.ProductName  ), State.tszProductName  );

		DXST_ERRORLOG_TABADD ;
		DXST_ERRORLOGFMT_ADDW(( L"Device Instance Name : %s", State.tszInstanceName )) ;

		DXST_ERRORLOGFMT_ADDW(( L"Device Product Name  : %s", State.tszProductName )) ;
		DXST_ERRORLOG_TABSUB ;
	}
	
	// �W���C�X�p�b�h�̃f�[�^�`����ݒ�
//	hr = Joystick->SetDataFormat( &c_dfDIJoystick ) ;
	hr = Joystick->SetDataFormat( &C_DFDIJOYSTICK ) ;
	if( hr != D_DI_OK )
	{
		Joystick->Release() ;
		DXST_ERRORLOG_ADDUTF16LE( "\xb8\x30\xe7\x30\xa4\x30\xd1\x30\xc3\x30\xc9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x67\x30\x6f\x30\x42\x30\x8a\x30\x7e\x30\x5b\x30\x93\x30\x67\x30\x57\x30\x5f\x30\x01\x30\xc8\x53\x6f\x30\x2d\x8a\x9a\x5b\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�W���C�p�b�h�f�o�C�X�ł͂���܂���ł����A���͐ݒ�Ɏ��s���܂���\n" @*/ ) ;
		return D_DIENUM_CONTINUE ;
	}

	// �W���C�p�b�h�̋������x����ݒ肷��
//	hr = Joystick->SetCooperativeLevel( NS_GetMainWindowHandle(), D_DISCL_BACKGROUND | D_DISCL_NONEXCLUSIVE ) ;
	hr = Joystick->SetCooperativeLevel( NS_GetMainWindowHandle(), D_DISCL_FOREGROUND | D_DISCL_EXCLUSIVE ) ;
	if( hr != D_DI_OK )
	{
//		Joystick->Release() ;
		DXST_ERRORLOG_ADDUTF16LE( "\xb8\x30\xe7\x30\xa4\x30\xd1\x30\xc3\x30\xc9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\x54\x53\xbf\x8a\xec\x30\xd9\x30\xeb\x30\x6e\x30\x2d\x8a\x9a\x5b\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�W���C�p�b�h�f�o�C�X�̋������x���̐ݒ�Ɏ��s���܂���\n" @*/ ) ;
//		return D_DIENUM_CONTINUE ;
	}

	// �w�͈̔͂�ݒ�
	diprg.diph.dwSize		= sizeof( diprg ) ;
	diprg.diph.dwHeaderSize	= sizeof( diprg.diph ) ;
	diprg.diph.dwObj		= D_DIJOFS_X ;
	diprg.diph.dwHow		= D_DIPH_BYOFFSET ;
	diprg.lMin				= -DIRINPUT_MAX ;
	diprg.lMax				= +DIRINPUT_MAX ;
	hr = Joystick->SetProperty( D_DIPROP_RANGE , &diprg.diph ) ;
	if( hr != D_DI_OK )
	{
//		Joystick->Release() ;
		DXST_ERRORLOG_ADDUTF16LE( "\xb8\x30\xe7\x30\xa4\x30\xd1\x30\xc3\x30\xc9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\x38\xff\xf8\x8e\xa2\x95\xc2\x4f\x6e\x30\x2d\x8a\x9a\x5b\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�W���C�p�b�h�f�o�C�X�̂w���֌W�̐ݒ�Ɏ��s���܂���\n" @*/ ) ;
//		return D_DIENUM_CONTINUE ;
	}

	// �x�͈̔͂�ݒ�
	diprg.diph.dwObj		= D_DIJOFS_Y ;
	hr = Joystick->SetProperty( D_DIPROP_RANGE , &diprg.diph ) ;
	if( hr != D_DI_OK )
	{
//		Joystick->Release() ;
		DXST_ERRORLOG_ADDUTF16LE( "\xb8\x30\xe7\x30\xa4\x30\xd1\x30\xc3\x30\xc9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\x39\xff\xf8\x8e\xa2\x95\xc2\x4f\x6e\x30\x2d\x8a\x9a\x5b\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�W���C�p�b�h�f�o�C�X�̂x���֌W�̐ݒ�Ɏ��s���܂���\n" @*/ ) ;
//		return D_DIENUM_CONTINUE ;
	}

	// �y�͈̔͂�ݒ�
	diprg.diph.dwObj		= D_DIJOFS_Z ;
	Joystick->SetProperty( D_DIPROP_RANGE , &diprg.diph ) ;

	// �q�w�q�x�q�y�͈̔͂�ݒ�
	diprg.diph.dwObj		= D_DIJOFS_RX ;
	Joystick->SetProperty( D_DIPROP_RANGE , &diprg.diph ) ;
	diprg.diph.dwObj		= D_DIJOFS_RY ;
	Joystick->SetProperty( D_DIPROP_RANGE , &diprg.diph ) ;
	diprg.diph.dwObj		= D_DIJOFS_RZ ;
	Joystick->SetProperty( D_DIPROP_RANGE , &diprg.diph ) ;

	// �����]�[���̃Z�b�g
	pad->DeadZone = DEADZONE ;

	// �w�̖����]�[����ݒ�
	dipdw.diph.dwSize		= sizeof( dipdw ) ;
	dipdw.diph.dwHeaderSize	= sizeof( dipdw.diph ) ;
	dipdw.diph.dwObj		= D_DIJOFS_X ;
	dipdw.diph.dwHow		= D_DIPH_BYOFFSET ;
	dipdw.dwData			= DEADZONE_DIRECTINPUT( pad->DeadZone ) ;
	hr = Joystick->SetProperty( D_DIPROP_DEADZONE , &dipdw.diph ) ;
	if( hr != D_DI_OK )
	{
//		Joystick->Release() ;
		DXST_ERRORLOG_ADDUTF16LE( "\xb8\x30\xe7\x30\xa4\x30\xd1\x30\xc3\x30\xc9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\x38\xff\xf8\x8e\x6e\x30\x21\x71\xb9\x52\xbe\x30\xfc\x30\xf3\x30\x6e\x30\x2d\x8a\x9a\x5b\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�W���C�p�b�h�f�o�C�X�̂w���̖����]�[���̐ݒ�Ɏ��s���܂���\n" @*/ ) ;
//		return D_DIENUM_CONTINUE ;
	}

	// �x�̖����]�[����ݒ�
	dipdw.diph.dwObj		= D_DIJOFS_Y ;
	hr = Joystick->SetProperty( D_DIPROP_DEADZONE , &dipdw.diph ) ;
	if( hr != D_DI_OK )
	{
//		Joystick->Release() ;
		DXST_ERRORLOG_ADDUTF16LE( "\xb8\x30\xe7\x30\xa4\x30\xd1\x30\xc3\x30\xc9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\x39\xff\xf8\x8e\x6e\x30\x21\x71\xb9\x52\xbe\x30\xfc\x30\xf3\x30\x6e\x30\x2d\x8a\x9a\x5b\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�W���C�p�b�h�f�o�C�X�̂x���̖����]�[���̐ݒ�Ɏ��s���܂���\n" @*/ ) ;
//		return D_DIENUM_CONTINUE ;
	}

	// �y�̖����]�[����ݒ�
	dipdw.diph.dwObj		= D_DIJOFS_Z ;
	Joystick->SetProperty( D_DIPROP_DEADZONE , &dipdw.diph ) ;

	// �q�w�q�x�q�y�̖����]�[����ݒ�
	dipdw.diph.dwObj		= D_DIJOFS_RX ;
	Joystick->SetProperty( D_DIPROP_DEADZONE , &dipdw.diph ) ;
	dipdw.diph.dwObj		= D_DIJOFS_RY ;
	Joystick->SetProperty( D_DIPROP_DEADZONE , &dipdw.diph ) ;
	dipdw.diph.dwObj		= D_DIJOFS_RZ ;
	Joystick->SetProperty( D_DIPROP_DEADZONE , &dipdw.diph ) ;

	// �t�H�[�X�t�F�[�h�o�b�N�I�u�W�F�N�g�̍쐬(PS�p�b�h�z��)
	{
		DWORD dwAxes[2] = { D_DIJOFS_X, D_DIJOFS_Y } ;
		LONG lDirection[2] = { 0, 0 } ;

//		Joystick->EnumEffects( EffectEnumCallBack, NULL, D_DIEFT_ALL ) ;

		// �E�����[�^�[�p(�茳�̃p�b�h�ł͉��̂� GUID_SINE �G�t�F�N�g�͉E���[�^�[�ɂȂ��Ă�)
		{
			D_DIPERIODIC diPeriodic ;	// �^�C�v�ŗL�p�����[�^
			D_DIENVELOPE diEnvelope ;	// �G���x���[�v
			D_DIEFFECT diEffect ;		// �ėp�p�����[�^

			_MEMSET( &diEffect, 0, sizeof( diEffect ) ) ;
			_MEMSET( &diEnvelope, 0, sizeof( diEnvelope ) ) ;
			_MEMSET( &diPeriodic, 0, sizeof( diPeriodic ) ) ;

			// ���Ƀ^�C�v�ŗL�p�����[�^������������B���̗�̂悤�Ȓl��ݒ肷��ƁA1/20 �b�����̑S�͎����I�G�t�F�N�g�𐶐����邱�ƂɂȂ�B

			diPeriodic.dwMagnitude	= D_DI_FFNOMINALMAX ;
			diPeriodic.lOffset		= 0;
			diPeriodic.dwPhase		= 0;
			diPeriodic.dwPeriod		= (DWORD) (1.5 * D_DI_SECONDS);
			// �`�F�[���\�[�̃��[�^�[���n�����悤�Ƃ��āA�Z���ԊP�̂悤�ȑ������o���A����ɂ��ꂪ�~��ł����Ƃ����G�t�F�N�g�𓾂邽�߂ɁA�G���x���[�v�ɔ��b�Ԃ̃A�^�b�N���Ԃ� 1 �b�Ԃ̃t�F�[�h���Ԃ�ݒ肷��B�������āA�Z���Ԃ̈ێ����Ԓl��������B

			diEnvelope.dwSize			= sizeof(D_DIENVELOPE);
			diEnvelope.dwAttackLevel	= 0;
			diEnvelope.dwAttackTime		= (DWORD) (0.5 * D_DI_SECONDS);
			diEnvelope.dwFadeLevel		= 0;
			diEnvelope.dwFadeTime		= (DWORD) (1.0 * D_DI_SECONDS);
			// ���ɁA��{�I�G�t�F�N�g�p�����[�^��ݒ肷��B�����̃p�����[�^�ɂ́A�����ƃf�o�C�X �I�u�W�F�N�g (�{�^���Ǝ�) �̎��ʕ��@�����肷��t���O�A�G�t�F�N�g�̃T���v�������ƃQ�C���A����т����قǏ����������̃f�[�^�ւ̃|�C���^���܂܂��B����ɁA�W���C�X�e�B�b�N�̔��˃{�^���ɃG�t�F�N�g������t���āA�{�^���������΃G�t�F�N�g�������I�ɍĐ�����悤�ɂ���B

			diEffect.dwSize						= sizeof(D_DIEFFECT);
			diEffect.dwFlags					= D_DIEFF_POLAR | D_DIEFF_OBJECTOFFSETS;
			diEffect.dwDuration					= INFINITE ;
 
			diEffect.dwSamplePeriod				= 0;               // �f�t�H���g�l
			diEffect.dwGain						= D_DI_FFNOMINALMAX;         // �X�P�[�����O�Ȃ�
			diEffect.dwTriggerButton			= D_DIEB_NOTRIGGER ; // D_DIJOFS_BUTTON( 0 );
			diEffect.dwTriggerRepeatInterval	= 0;
			diEffect.cAxes						= 2;
			diEffect.rgdwAxes					= dwAxes;
			diEffect.rglDirection				= &lDirection[0];
			diEffect.lpEnvelope					= &diEnvelope;
			diEffect.cbTypeSpecificParams		= sizeof(diPeriodic);
			diEffect.lpvTypeSpecificParams		= &diPeriodic;
			// �Z�b�g�A�b�v�͂����܂ŁB����ƃG�t�F�N�g�𐶐��ł���B

			pad->Effect[ DINPUTPAD_MOTOR_RIGHT ].PF.DIEffect = NULL ;
			if( InputSysData.NoUseVibrationFlag == FALSE )
			{
				Joystick->CreateEffect(
								 GUID_SINE,     // �񋓂���� GUID
								 &diEffect,      // �f�[�^�̏ꏊ
								 &pad->Effect[ DINPUTPAD_MOTOR_RIGHT ].PF.DIEffect,  // �C���^�[�t�F�C�X �|�C���^��u���ꏊ
								 NULL ) ;          // �W�����Ȃ�
			}
		}

		// �������[�^�[�ݒ�(�茳�̃p�b�h�ł͉��̂� GUID_CONSTANTFORCE �G�t�F�N�g�͉E���[�^�[�ɂȂ��Ă�)
		{
			DWORD    rgdwAxes[2] = { D_DIJOFS_X, D_DIJOFS_Y };
			LONG     rglDirection[2] = { 0, 0 };
			D_DICONSTANTFORCE cf = { D_DI_FFNOMINALMAX };

			D_DIEFFECT eff;
			_MEMSET( &eff, 0, sizeof(eff) );
			eff.dwSize                  = sizeof(D_DIEFFECT);
			eff.dwFlags                 = D_DIEFF_CARTESIAN | D_DIEFF_OBJECTOFFSETS;
			eff.dwDuration              = INFINITE;
			eff.dwSamplePeriod          = 0;
			eff.dwGain                  = D_DI_FFNOMINALMAX;
			eff.dwTriggerButton         = D_DIEB_NOTRIGGER;
			eff.dwTriggerRepeatInterval = 0;
			eff.cAxes                   = 2;
			eff.rgdwAxes                = rgdwAxes;
			eff.rglDirection            = rglDirection;
			eff.lpEnvelope              = 0;
			eff.cbTypeSpecificParams    = sizeof(D_DICONSTANTFORCE);
			eff.lpvTypeSpecificParams   = &cf;
			eff.dwStartDelay            = 0;

			pad->Effect[ DINPUTPAD_MOTOR_LEFT ].PF.DIEffect = NULL ;
			if( InputSysData.NoUseVibrationFlag == FALSE )
			{
				hr = Joystick->CreateEffect(
								 GUID_CONSTANTFORCE,	// �񋓂���� GUID
								 &eff,					// �f�[�^�̏ꏊ
								 &pad->Effect[ DINPUTPAD_MOTOR_LEFT ].PF.DIEffect,  // �C���^�[�t�F�C�X �|�C���^��u���ꏊ
								 NULL					// �W�����Ȃ�
				) ;
				if( hr != D_DI_OK )
				{
					DXST_ERRORLOG_ADDUTF16LE(( "\x68\x54\x1f\x67\x84\x76\xa8\x30\xd5\x30\xa7\x30\xaf\x30\xc8\x30\x6e\x30\x5c\x4f\x10\x62\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x02\x30\x0a\x00\x00"/*@ L"�����I�G�t�F�N�g�̍쐬�Ɏ��s���܂����B\n" @*/ )) ;
				}
			}
		}

		// �U���֌W�̐ݒ�̏��������s��
		for( i = 0 ; i < DINPUTPAD_MOTOR_NUM ; i ++ )
		{
			pad->Effect[ i ].PlayFlag  = FALSE ;
			pad->Effect[ i ].Power     = D_DI_FFNOMINALMAX ;
			pad->Effect[ i ].PlayState = FALSE ;
			pad->Effect[ i ].SetPower  = 0 ;
		}
//		pad->EffectNotCommonFlag    = FALSE ;
//		pad->EffectCommon.PlayFlag	= FALSE ;
//		pad->EffectCommon.Power		= D_DI_FFNOMINALMAX ;
//		pad->Effect[ DINPUTPAD_MOTOR_LEFT ].PlayFlag	= FALSE ;
//		pad->Effect[ DINPUTPAD_MOTOR_LEFT ].Power		= D_DI_FFNOMINALMAX ;
//		pad->Effect[ DINPUTPAD_MOTOR_RIGHT ].PlayFlag	= FALSE ;
//		pad->Effect[ DINPUTPAD_MOTOR_RIGHT ].Power		= D_DI_FFNOMINALMAX ;
//		pad->EffectPlayStateLeft	= FALSE ;
//		pad->EffectPlayStateRight	= FALSE ;
	}

	// �ω��ʒm�p�̃C�x���g���쐬�E�A�^�b�`����
	Event = CreateEvent( NULL, TRUE, FALSE, NULL ) ;
	hr = Joystick->SetEventNotification( Event ) ;
	if( hr != D_DI_POLLEDDEVICE && hr != D_DI_OK )
	{
		DXST_ERRORLOG_ADDUTF16LE( "\xb8\x30\xe7\x30\xa4\x30\xd1\x30\xc3\x30\xc9\x30\xa4\x30\xd9\x30\xf3\x30\xc8\x30\x6e\x30\xbb\x30\xc3\x30\xc8\x30\xa2\x30\xc3\x30\xd7\x30\x4c\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�W���C�p�b�h�C�x���g�̃Z�b�g�A�b�v�����s���܂���\n" @*/ ) ;
		return D_DIENUM_CONTINUE ;
	}

	// �f�o�C�X���擾����
	Joystick->Acquire() ;

	// �C�x���g�n���h����ۑ�
	pad->PF.Event = Event ;

	// �W���C�p�b�h�f�o�C�X�̃A�h���X��ۑ�
	pad->PF.Device = Joystick ;

	// �X�e�[�^�X��������
	_MEMSET( &pad->State, 0, sizeof( D_DIJOYSTATE ) ) ;

	// �W���C�p�b�h�̐��𑝂₷
	InputSysData.PadNum ++ ;

	DXST_ERRORLOG_ADDUTF16LE( "\xb8\x30\xe7\x30\xa4\x30\xd1\x30\xc3\x30\xc9\x30\x6e\x30\xfd\x8f\xa0\x52\x6f\x30\x63\x6b\x38\x5e\x6b\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�W���C�p�b�h�̒ǉ��͐���ɏI�����܂���\n" @*/ ) ;

	// �I��
	return ( InputSysData.PadNum != MAX_JOYPAD_NUM ) ? D_DIENUM_CONTINUE : D_DIENUM_STOP ;
}


// ���̓V�X�e�������������鏈���̊��ˑ�����
extern int InitializeInputSystem_PF_Timing0( void )
{
	HRESULT hr ;
	int i, j;
	int KeyToJoypadInput[ MAX_JOYPAD_NUM ][ 32 ][ 4 ] ;
	static int NowInitialize = FALSE ;

	// ����������
	if( InputSysData.PF.DirectInputObject != NULL )
	{
		return 0 ;
	}

	// ���ɏ������������J�n����Ă���ꍇ�����������ɏI��
	if( NowInitialize )
	{
		return 0 ;
	}

	// ���������t���O�𗧂Ă�
	NowInitialize = TRUE ;

	// �[��������
	int	InitializeFlag									= InputSysData.InitializeFlag ;
	int NoUseXInputFlag									= InputSysData.PF.NoUseXInputFlag ;
	int NoUseDirectInputFlag							= InputSysData.PF.NoUseDirectInputFlag ;
	int UseDirectInputFlag								= InputSysData.PF.UseDirectInputFlag ;
	int KeyboardNotUseDirectInputFlag					= InputSysData.PF.KeyboardNotUseDirectInputFlag ;
	int KeyExclusiveCooperativeLevelFlag				= InputSysData.PF.KeyExclusiveCooperativeLevelFlag ;
	int KeyToJoypadInputInitializeFlag					= InputSysData.KeyToJoypadInputInitializeFlag ;
	int NoUseVibrationFlag								= InputSysData.NoUseVibrationFlag ;
	_MEMCPY( KeyToJoypadInput, InputSysData.KeyToJoypadInput, sizeof( InputSysData.KeyToJoypadInput ) ) ;
	_MEMSET( &InputSysData, 0, sizeof( InputSysData ) ) ; 
	InputSysData.PF.NoUseXInputFlag						= NoUseXInputFlag ;
	InputSysData.PF.NoUseDirectInputFlag				= NoUseDirectInputFlag ;
	InputSysData.PF.UseDirectInputFlag					= UseDirectInputFlag ;
	InputSysData.PF.KeyboardNotUseDirectInputFlag		= KeyboardNotUseDirectInputFlag ;
	InputSysData.PF.KeyExclusiveCooperativeLevelFlag	= KeyExclusiveCooperativeLevelFlag ;
	InputSysData.KeyToJoypadInputInitializeFlag			= KeyToJoypadInputInitializeFlag ;
	InputSysData.NoUseVibrationFlag						= NoUseVibrationFlag ;
	InputSysData.InitializeFlag							= InitializeFlag ;
	_MEMCPY( InputSysData.KeyToJoypadInput, KeyToJoypadInput, sizeof( InputSysData.KeyToJoypadInput ) ) ;

	// �����I�� DirectInput8 ���g�p����悤�ɂ���
	InputSysData.PF.UseDirectInput8Flag = TRUE ;

	// �c�����������h�������� ���g�p���邩�ǂ����ŏ����𕪊�
START:
	if( InputSysData.PF.NoUseDirectInputFlag == FALSE )
	{
		int i ;
		const wchar_t *XInputDllFileName[] = 
		{
			L"xinput1_4.dll",
			L"xinput1_3.dll",
			L"xinput9_1_0.dll",
			NULL
		} ;

		// DirectInput ���g�p����ꍇ

		DXST_ERRORLOG_ADDUTF16LE( "\x44\x00\x69\x00\x72\x00\x65\x00\x63\x00\x74\x00\x49\x00\x6e\x00\x70\x00\x75\x00\x74\x00\xa2\x95\xc2\x4f\x1d\x52\x1f\x67\x16\x53\xe6\x51\x06\x74\x0a\x00\x00"/*@ L"DirectInput�֌W����������\n" @*/ ) ;
		DXST_ERRORLOG_TABADD ;

		// �w�h�����������g�p���Ȃ��t���O�������Ă��Ȃ�������w�h���������̂c�k�k��ǂݍ���
		if( InputSysData.PF.NoUseXInputFlag == FALSE )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\x58\x00\x49\x00\x6e\x00\x70\x00\x75\x00\x74\x00\x20\x00\x44\x00\x4c\x00\x4c\x00\x20\x00\x6e\x30\xad\x8a\x7f\x30\xbc\x8f\x7f\x30\x2d\x4e\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"XInput DLL �̓ǂݍ��ݒ�... " @*/ ) ;
			for( i = 0 ; XInputDllFileName[ i ] != NULL ; i ++ )
			{
				InputSysData.PF.XInputDLL = LoadLibraryW( XInputDllFileName[ i ] ) ;
				if( InputSysData.PF.XInputDLL != NULL )
					break ;
			}

			if( InputSysData.PF.XInputDLL == NULL )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x31\x59\x57\x65\x01\x30\x58\x00\x49\x00\x6e\x00\x70\x00\x75\x00\x74\x00\x6f\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x5b\x30\x93\x30\x0a\x00\x00"/*@ L"���s�AXInput�͎g�p���܂���\n" @*/ ) ;
			}
			else
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;

				InputSysData.PF.XInputGetStateFunc = ( DWORD ( WINAPI * )( DWORD, D_XINPUT_STATE*     ) )GetProcAddress( InputSysData.PF.XInputDLL, "XInputGetState" ) ;
				InputSysData.PF.XInputSetStateFunc = ( DWORD ( WINAPI * )( DWORD, D_XINPUT_VIBRATION* ) )GetProcAddress( InputSysData.PF.XInputDLL, "XInputSetState" ) ;
			}
		}

		// �c�����������h���������I�u�W�F�N�g���쐬����
		DXST_ERRORLOG_ADDUTF16LE( "\x44\x00\x69\x00\x72\x00\x65\x00\x63\x00\x74\x00\x49\x00\x6e\x00\x70\x00\x75\x00\x74\x00\x37\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x2d\x4e\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"DirectInput7 �̎擾��... " @*/ ) ;

		if( InputSysData.PF.UseDirectInput8Flag == FALSE )
		{
			hr = WinAPIData.Win32Func.CoCreateInstanceFunc( CLSID_DIRECTINPUT , NULL, CLSCTX_INPROC_SERVER, IID_IDIRECTINPUT7, ( LPVOID * )&InputSysData.PF.DirectInputObject ) ;
			if( !FAILED( hr ) )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;
				DXST_ERRORLOG_TABADD ;
				DXST_ERRORLOG_ADDUTF16LE( "\x15\x5f\x4d\x30\x9a\x7d\x4d\x30\x1d\x52\x1f\x67\x16\x53\xe6\x51\x06\x74\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"������������������... " @*/ ) ;
				hr = InputSysData.PF.DirectInputObject->Initialize( WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ), 0x700 ) ;
				if( FAILED( hr ) ) 
				{
					DXST_ERRORLOG_ADDUTF16LE( "\x1d\x52\x1f\x67\x16\x53\x6b\x30\x31\x59\x57\x65\x02\x30\x00"/*@ L"�������Ɏ��s�B" @*/ ) ;
					InputSysData.PF.DirectInputObject->Release() ;
					InputSysData.PF.DirectInputObject = NULL ;
					goto ER1;
				}
				DXST_ERRORLOG_ADDUTF16LE( "\x1d\x52\x1f\x67\x16\x53\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����������\n" @*/ ) ;
				DXST_ERRORLOG_TABSUB ;
			}
		}

		if( InputSysData.PF.DirectInputObject == NULL )
		{
			if( InputSysData.PF.UseDirectInput8Flag == FALSE )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x02\x30\x0a\x00\x00"/*@ L"�擾�Ɏ��s�B\n" @*/ ) ;
				DXST_ERRORLOGFMT_ADDUTF16LE(( "\xa8\x30\xe9\x30\xfc\x30\xb3\x30\xfc\x30\xc9\x30\x20\x00\x30\x00\x78\x00\x25\x00\x78\x00\x0a\x00\x00"/*@ L"�G���[�R�[�h 0x%x\n" @*/, hr ));
			}
ER1:
			DXST_ERRORLOG_ADDUTF16LE( "\x44\x00\x69\x00\x72\x00\x65\x00\x63\x00\x74\x00\x49\x00\x6e\x00\x70\x00\x75\x00\x74\x00\x38\x00\x20\x00\x6e\x30\xd6\x53\x97\x5f\x92\x30\x66\x8a\x7f\x30\x7e\x30\x59\x30\x2e\x00\x2e\x00\x2e\x00\x00"/*@ L"DirectInput8 �̎擾�����݂܂�..." @*/ ) ;
			hr = WinAPIData.Win32Func.CoCreateInstanceFunc( CLSID_DIRECTINPUT8 , NULL, CLSCTX_INPROC_SERVER, IID_IDIRECTINPUT8, ( LPVOID * )&InputSysData.PF.DirectInputObject ) ;
			if( !FAILED( hr ) )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����\n" @*/ ) ;
				DXST_ERRORLOG_TABADD ;
				DXST_ERRORLOG_ADDUTF16LE( "\x15\x5f\x4d\x30\x9a\x7d\x4d\x30\x1d\x52\x1f\x67\x16\x53\xe6\x51\x06\x74\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"������������������... " @*/ ) ;
				hr = InputSysData.PF.DirectInputObject->Initialize( WinAPIData.Win32Func.GetModuleHandleWFunc( NULL ) , 0x800 ) ;
				if( FAILED( hr ) ) 
				{
					DXST_ERRORLOG_ADDUTF16LE( "\x1d\x52\x1f\x67\x16\x53\x6b\x30\x31\x59\x57\x65\x02\x30\x00"/*@ L"�������Ɏ��s�B" @*/ ) ;
					InputSysData.PF.DirectInputObject->Release() ;
					InputSysData.PF.DirectInputObject = NULL ;
					goto ER2;
				}
				DXST_ERRORLOG_ADDUTF16LE( "\x1d\x52\x1f\x67\x16\x53\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����������\n" @*/ ) ;
				InputSysData.PF.UseDirectInput8Flag = TRUE;
				DXST_ERRORLOG_TABSUB ;
			}
			else
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x02\x30\x00"/*@ L"�擾�Ɏ��s�B" @*/ ) ;
ER2:
				DXST_ERRORLOG_ADDUTF16LE( "\x44\x00\x69\x00\x72\x00\x65\x00\x63\x00\x74\x00\x49\x00\x6e\x00\x70\x00\x75\x00\x74\x00\x20\x00\x92\x30\x7f\x4f\x28\x75\x57\x30\x7e\x30\x5b\x30\x93\x30\x02\x30\x0a\x00\x00"/*@ L"DirectInput ���g�p���܂���B\n" @*/ ) ;
				DXST_ERRORLOG_TABSUB ;

				if( InputSysData.PF.DirectInputObject ) InputSysData.PF.DirectInputObject->Release() ;
				InputSysData.PF.DirectInputObject = NULL;
				InputSysData.PF.NoUseDirectInputFlag = TRUE;
//				return -1 ;
				goto START;
			}
		}

		// ���C���E�C���h�E���A�N�e�B�u�ɂ���
		// BringWindowToTop( NS_GetMainWindowHandle() ) ;

		// �A�N�e�B�u�ɂȂ�܂ő҂�
		NS_ProcessMessage() ;

		// �W���C�p�b�h�f�o�C�X������������
		SetupJoypad() ;

		// �}�E�X�f�o�C�X������������
		InputSysData.PF.MouseDeviceObject = NULL ;
		{
			// �}�E�X�f�o�C�X���쐬����
			DXST_ERRORLOG_ADDUTF16LE( "\xde\x30\xa6\x30\xb9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\x1d\x52\x1f\x67\x16\x53\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�}�E�X�f�o�C�X�̏�����... " @*/ ) ; 
			if( InputSysData.PF.UseDirectInput8Flag == TRUE )
			{
				hr = InputSysData.PF.DirectInputObject->CreateDevice( GUID_SYSMOUSE, ( D_IDirectInputDevice ** )&InputSysData.PF.MouseDeviceObject , NULL ) ;
			}
			else
			{
				hr = InputSysData.PF.DirectInputObject->CreateDeviceEx( GUID_SYSMOUSE, IID_IDIRECTINPUTDEVICE7, ( void ** )&InputSysData.PF.MouseDeviceObject , NULL ) ;
			}
			if( hr != D_DI_OK )
			{
				InputSysData.PF.MouseDeviceObject = NULL ;
				goto MOUSEDEVICEINITEND ;
//				InputSysData.PF.DirectInputObject->Release() ;
//				InputSysData.PF.DirectInputObject = NULL ;

//				NowInitialize = FALSE ;
//				return DxLib_Error( L"�}�E�X�f�o�C�X�̎擾�Ɏ��s���܂���" ) ;
			}

			// �}�E�X�f�o�C�X�̃f�[�^�`����ݒ肷��
	//		hr = InputSysData.PF.MouseDeviceObject->SetDataFormat( &c_dfDIMouse2 ) ;
			hr = InputSysData.PF.MouseDeviceObject->SetDataFormat( &C_DFDIMOUSE2 ) ;
			if( hr != D_DI_OK )
			{
				InputSysData.PF.MouseDeviceObject->Release() ;
				InputSysData.PF.MouseDeviceObject = NULL ;
				goto MOUSEDEVICEINITEND ;

//				InputSysData.PF.DirectInputObject->Release() ;
//				InputSysData.PF.DirectInputObject = NULL ;

//				NowInitialize = FALSE ;
//				return DxLib_Error( L"�}�E�X�f�o�C�X�̃f�[�^�`���̐ݒ�Ɏ��s���܂���" ) ;
			}

			// �}�E�X�̋������x����ݒ肷��
	//		hr = InputSysData.PF.MouseDeviceObject->SetCooperativeLevel( NS_GetMainWindowHandle(), D_DISCL_EXCLUSIVE | D_DISCL_FOREGROUND ) ;
			hr = InputSysData.PF.MouseDeviceObject->SetCooperativeLevel( NS_GetMainWindowHandle(), D_DISCL_NONEXCLUSIVE | D_DISCL_BACKGROUND ) ;
	//		hr = InputSysData.PF.MouseDeviceObject->SetCooperativeLevel( NS_GetMainWindowHandle(), D_DISCL_NONEXCLUSIVE | D_DISCL_FOREGROUND ) ;
 			if( hr != D_DI_OK ) 
			{
				InputSysData.PF.MouseDeviceObject->Release() ;
				InputSysData.PF.MouseDeviceObject = NULL ;
				goto MOUSEDEVICEINITEND ;

//				InputSysData.PF.DirectInputObject->Release() ;
//				InputSysData.PF.DirectInputObject = NULL ;

//				NowInitialize = FALSE ;
//				return DxLib_Error( L"�}�E�X�f�o�C�X�̋������x���̐ݒ�Ɏ��s���܂���" ) ;
			}

			DXST_ERRORLOG_ADDUTF16LE( "\x1d\x52\x1f\x67\x16\x53\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����������\n" @*/ ) ;

MOUSEDEVICEINITEND:
			if( InputSysData.PF.MouseDeviceObject == NULL )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xde\x30\xa6\x30\xb9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�}�E�X�f�o�C�X�̎擾�Ɏ��s���܂���\n" @*/ ) ; 
			}

			InputSysData.KeyInputGetTime = ( int )WinAPIData.Win32Func.timeGetTimeFunc() ;
		}

		// �L�[�{�[�h�f�o�C�X������������
		InputSysData.PF.KeyboardDeviceObject = NULL ;
		if( InputSysData.PF.KeyboardNotUseDirectInputFlag == FALSE )
		{
			HANDLE Event ;

			// �L�[�{�[�h�f�o�C�X���쐬����
			DXST_ERRORLOG_ADDUTF16LE( "\xad\x30\xfc\x30\xdc\x30\xfc\x30\xc9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\x1d\x52\x1f\x67\x16\x53\x2e\x00\x2e\x00\x2e\x00\x20\x00\x00"/*@ L"�L�[�{�[�h�f�o�C�X�̏�����... " @*/ ) ; 
			if( InputSysData.PF.UseDirectInput8Flag == TRUE )
			{
				hr = InputSysData.PF.DirectInputObject->CreateDevice( GUID_SYSKEYBOARD, ( D_IDirectInputDevice ** )&InputSysData.PF.KeyboardDeviceObject , NULL ) ;
			}
			else
			{
				hr = InputSysData.PF.DirectInputObject->CreateDeviceEx( GUID_SYSKEYBOARD, IID_IDIRECTINPUTDEVICE7, ( void ** )&InputSysData.PF.KeyboardDeviceObject , NULL ) ;
			}
			if( hr != D_DI_OK )
			{
				if( InputSysData.PF.MouseDeviceObject )
				{
					InputSysData.PF.MouseDeviceObject->Release() ;
					InputSysData.PF.MouseDeviceObject = NULL ;
				}

				InputSysData.PF.DirectInputObject->Release() ;
				InputSysData.PF.DirectInputObject = NULL ;

				NowInitialize = FALSE ;
				return DxLib_ErrorUTF16LE( "\xad\x30\xfc\x30\xdc\x30\xfc\x30\xc9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�L�[�{�[�h�f�o�C�X�̎擾�Ɏ��s���܂���" @*/ ) ;
			}

			// �L�[�{�[�h�f�o�C�X�̃f�[�^�`����ݒ肷��
	//		hr = InputSysData.PF.KeyboardDeviceObject->SetDataFormat( &c_dfDIKeyboard ) ;
			hr = InputSysData.PF.KeyboardDeviceObject->SetDataFormat( &C_DFDIKEYBOARD ) ;
			if( hr != D_DI_OK )
			{
				if( InputSysData.PF.MouseDeviceObject )
				{
					InputSysData.PF.MouseDeviceObject->Release() ;
					InputSysData.PF.MouseDeviceObject = NULL ;
				}

				InputSysData.PF.KeyboardDeviceObject->Release() ;
				InputSysData.PF.KeyboardDeviceObject = NULL ;

				InputSysData.PF.DirectInputObject->Release() ;
				InputSysData.PF.DirectInputObject = NULL ;

				NowInitialize = FALSE ;
				return DxLib_ErrorUTF16LE( "\xad\x30\xfc\x30\xdc\x30\xfc\x30\xc9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\xc7\x30\xfc\x30\xbf\x30\x62\x5f\x0f\x5f\x6e\x30\x2d\x8a\x9a\x5b\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�L�[�{�[�h�f�o�C�X�̃f�[�^�`���̐ݒ�Ɏ��s���܂���" @*/ ) ;
			}

			// �L�[�{�[�h�̋������x����ݒ肷��
			if( InputSysData.PF.KeyExclusiveCooperativeLevelFlag )
			{
				hr = InputSysData.PF.KeyboardDeviceObject->SetCooperativeLevel( NS_GetMainWindowHandle(), D_DISCL_EXCLUSIVE | D_DISCL_FOREGROUND ) ;
			}
			else
			{
				hr = InputSysData.PF.KeyboardDeviceObject->SetCooperativeLevel( NS_GetMainWindowHandle(), D_DISCL_NONEXCLUSIVE | D_DISCL_BACKGROUND ) ;
			}
	//		hr = InputSysData.PF.KeyboardDeviceObject->SetCooperativeLevel( NS_GetMainWindowHandle(), D_DISCL_EXCLUSIVE | D_DISCL_FOREGROUND ) ;
	//		hr = InputSysData.PF.KeyboardDeviceObject->SetCooperativeLevel( NS_GetMainWindowHandle(), D_DISCL_NONEXCLUSIVE | D_DISCL_FOREGROUND ) ;
 			if( hr != D_DI_OK ) 
			{
				if( InputSysData.PF.MouseDeviceObject )
				{
					InputSysData.PF.MouseDeviceObject->Release() ;
					InputSysData.PF.MouseDeviceObject = NULL ;
				}

				InputSysData.PF.KeyboardDeviceObject->Release() ;
				InputSysData.PF.KeyboardDeviceObject = NULL ;

				InputSysData.PF.DirectInputObject->Release() ;
				InputSysData.PF.DirectInputObject = NULL ;

				NowInitialize = FALSE ;
				return DxLib_ErrorUTF16LE( "\xad\x30\xfc\x30\xdc\x30\xfc\x30\xc9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\x54\x53\xbf\x8a\xec\x30\xd9\x30\xeb\x30\x6e\x30\x2d\x8a\x9a\x5b\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�L�[�{�[�h�f�o�C�X�̋������x���̐ݒ�Ɏ��s���܂���" @*/ ) ;
			}

			// �o�b�t�@��ݒ肵�Ă݂�
			{
				D_DIPROPDWORD DIProp ;

				DIProp.diph.dwSize = sizeof( DIProp ) ;
				DIProp.diph.dwHeaderSize = sizeof( DIProp.diph ) ;
				DIProp.diph.dwObj = 0 ;
				DIProp.diph.dwHow = D_DIPH_DEVICE ;
				DIProp.dwData = 100 ;
				if( InputSysData.PF.KeyboardDeviceObject->SetProperty( D_DIPROP_BUFFERSIZE, &DIProp.diph ) != D_DI_OK )
				{
					DXST_ERRORLOG_ADDUTF16LE( "\xad\x30\xfc\x30\xdc\x30\xfc\x30\xc9\x30\x6e\x30\xd7\x30\xed\x30\xd1\x30\xc6\x30\xa3\x30\x2d\x8a\x9a\x5b\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�L�[�{�[�h�̃v���p�e�B�ݒ�Ɏ��s���܂���\n" @*/ ) ;
				}
			}

			// �ω��ʒm�p�̃C�x���g���쐬�E�A�^�b�`����
			Event = CreateEvent( NULL, TRUE, FALSE, NULL ) ;
			hr = InputSysData.PF.KeyboardDeviceObject->SetEventNotification( Event ) ;
			if( hr != D_DI_POLLEDDEVICE && hr != D_DI_OK )
			{
				DXST_ERRORLOG_ADDUTF16LE( "\xad\x30\xfc\x30\xdc\x30\xfc\x30\xc9\x30\xa4\x30\xd9\x30\xf3\x30\xc8\x30\x6e\x30\xbb\x30\xc3\x30\xc8\x30\xa2\x30\xc3\x30\xd7\x30\x4c\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"�L�[�{�[�h�C�x���g�̃Z�b�g�A�b�v�����s���܂���\n" @*/ ) ;
				NowInitialize = FALSE ;
				return -1 ;
			}
			InputSysData.PF.KeyEvent = Event ;

			// �L�[�{�[�h�̃f�o�C�X���擾����
			hr = InputSysData.PF.KeyboardDeviceObject->Acquire() ;
			if( hr != D_DI_OK )
			{
				hr = InputSysData.PF.KeyboardDeviceObject->Acquire() ;
				if( hr != D_DI_OK )
				{
					if( InputSysData.PF.MouseDeviceObject )
					{
						InputSysData.PF.MouseDeviceObject->Release() ;
						InputSysData.PF.MouseDeviceObject = NULL ;
					}

					InputSysData.PF.KeyboardDeviceObject->Release() ;
					InputSysData.PF.KeyboardDeviceObject = NULL ;

					InputSysData.PF.DirectInputObject->Release() ;
					InputSysData.PF.DirectInputObject = NULL ;

					NowInitialize = FALSE ;
					return DxLib_ErrorUTF16LE( "\xad\x30\xfc\x30\xdc\x30\xfc\x30\xc9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�L�[�{�[�h�f�o�C�X�̎擾�Ɏ��s���܂���" @*/ ) ;
				}
			}

			DXST_ERRORLOG_ADDUTF16LE( "\x1d\x52\x1f\x67\x16\x53\x10\x62\x9f\x52\x0a\x00\x00"/*@ L"����������\n" @*/ ) ;

			InputSysData.KeyInputGetTime = ( int )WinAPIData.Win32Func.timeGetTimeFunc() ;
		}
		

		DXST_ERRORLOG_TABSUB ;
		DXST_ERRORLOG_ADDUTF16LE( "\x44\x00\x69\x00\x72\x00\x65\x00\x63\x00\x74\x00\x49\x00\x6e\x00\x70\x00\x75\x00\x74\x00\x20\x00\xa2\x95\x23\x90\x6e\x30\x1d\x52\x1f\x67\x16\x53\x6f\x30\x63\x6b\x38\x5e\x6b\x30\x42\x7d\x86\x4e\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"DirectInput �֘A�̏������͐���ɏI�����܂���\n" @*/ ) ;
	}
	else
	{
		// DirectInput ���g�p���Ȃ��ꍇ
		int i ;
		JOYCAPSW joycaps ;
		DXST_ERRORLOG_TABADD ;

		// �p�b�h�̐��𒲂ׂ�
		for( i = 0 ; i < MAX_JOYPAD_NUM ; i ++ )
		{
//			_MEMSET( &joyex, 0, sizeof( joyex ) ) ;
//			joyex.dwSize  = sizeof( joyex ) ;
//			joyex.dwFlags = JOY_RETURNALL ;
//			if( WinAPIData.Win32Func.joyGetPosExFunc( i, &joyex ) != JOYERR_NOERROR ) break ;
			_MEMSET( &joycaps, 0, sizeof( joycaps ) ) ;
			hr = ( HRESULT )WinAPIData.Win32Func.joyGetDevCapsFunc( ( UINT )i, &joycaps, sizeof( joycaps ) ) ;
			if( hr != JOYERR_NOERROR ) break ;
			InputSysData.Pad[ i ].PF.RightStickFlag = ( joycaps.wCaps & JOYCAPS_HASZ ) && ( joycaps.wCaps & JOYCAPS_HASR ) ;
			InputSysData.Pad[ i ].DeadZone = DEADZONE ;
		}
		InputSysData.PadNum = i ;

		DXST_ERRORLOGFMT_ADDUTF16LE(( "\xd1\x30\xc3\x30\xc9\x30\x6e\x30\x70\x65\x6f\x30\x20\x00\x25\x00\x64\x00\x20\x00\x0b\x50\x67\x30\x59\x30\x0a\x00\x00"/*@ L"�p�b�h�̐��� %d �ł�\n" @*/, i ));

		DXST_ERRORLOG_TABSUB ;
		InputSysData.PF.DirectInputObject = (D_IDirectInput7 *)(DWORD_PTR)0xffffffff ;
		DXST_ERRORLOG_ADDUTF16LE( "\x65\x51\x9b\x52\xa2\x95\x23\x90\x6e\x30\x1d\x52\x1f\x67\x16\x53\x92\x30\x57\x30\x7e\x30\x57\x30\x5f\x30\x0a\x00\x00"/*@ L"���͊֘A�̏����������܂���\n" @*/ ) ;
	}

	// �L�[�{�[�h�ƃW���C�p�b�h�̓��͂̃f�t�H���g�̑Ή��\��ݒ肷��
	if( InputSysData.KeyToJoypadInputInitializeFlag == FALSE )
	{
		InputSysData.KeyToJoypadInputInitializeFlag = TRUE ;

		for( i = 0 ; i < MAX_JOYPAD_NUM ; i ++ )
		{
			for ( j = 0 ; j < 32 ; j ++ )
			{
				InputSysData.KeyToJoypadInput[ i ][ j ][ 0 ] = -1 ;
				InputSysData.KeyToJoypadInput[ i ][ j ][ 1 ] = -1 ;
			}
		}
		InputSysData.KeyToJoypadInput[ 0 ][  0 ][ 0 ] = D_DIK_NUMPAD2;
		InputSysData.KeyToJoypadInput[ 0 ][  0 ][ 1 ] = D_DIK_DOWN;
		InputSysData.KeyToJoypadInput[ 0 ][  1 ][ 0 ] = D_DIK_NUMPAD4;
		InputSysData.KeyToJoypadInput[ 0 ][  1 ][ 1 ] = D_DIK_LEFT;
		InputSysData.KeyToJoypadInput[ 0 ][  2 ][ 0 ] = D_DIK_NUMPAD6;
		InputSysData.KeyToJoypadInput[ 0 ][  2 ][ 1 ] = D_DIK_RIGHT;
		InputSysData.KeyToJoypadInput[ 0 ][  3 ][ 0 ] = D_DIK_NUMPAD8;
		InputSysData.KeyToJoypadInput[ 0 ][  3 ][ 1 ] = D_DIK_UP;
		InputSysData.KeyToJoypadInput[ 0 ][  4 ][ 0 ] = D_DIK_Z;
		InputSysData.KeyToJoypadInput[ 0 ][  5 ][ 0 ] = D_DIK_X;
		InputSysData.KeyToJoypadInput[ 0 ][  6 ][ 0 ] = D_DIK_C;
		InputSysData.KeyToJoypadInput[ 0 ][  7 ][ 0 ] = D_DIK_A;
		InputSysData.KeyToJoypadInput[ 0 ][  8 ][ 0 ] = D_DIK_S;
		InputSysData.KeyToJoypadInput[ 0 ][  9 ][ 0 ] = D_DIK_D;
		InputSysData.KeyToJoypadInput[ 0 ][ 10 ][ 0 ] = D_DIK_Q;
		InputSysData.KeyToJoypadInput[ 0 ][ 11 ][ 0 ] = D_DIK_W;
		InputSysData.KeyToJoypadInput[ 0 ][ 12 ][ 0 ] = D_DIK_ESCAPE;
		InputSysData.KeyToJoypadInput[ 0 ][ 13 ][ 0 ] = D_DIK_SPACE;
	}

	// ���������t���O��|��
	NowInitialize = FALSE ;

	// ����I��
	return 0 ;
}

// ���̓V�X�e���̌�n�������鏈���̊��ˑ�����
extern int TerminateInputSystem_PF_Timing0( void )
{
	// ����������
	if( InputSysData.PF.DirectInputObject == NULL )
	{
		return 0 ;
	}

	// �c�����������h�������� ���g�p���Ă������ǂ����ŏ����𕪊�
	if( InputSysData.PF.DirectInputObject != (D_IDirectInput7 *)(DWORD_PTR)0xffffffff )
	{
		// DirectInput ���g�p�����ꍇ
		DXST_ERRORLOG_ADDUTF16LE( "\x44\x00\x69\x00\x72\x00\x65\x00\x63\x00\x74\x00\x49\x00\x6e\x00\x70\x00\x75\x00\x74\x00\x20\x00\xa2\x95\x23\x90\x6e\x30\x42\x7d\x86\x4e\xe6\x51\x06\x74\x2e\x00\x2e\x00\x2e\x00\x20\x00\x8c\x5b\x86\x4e\x0a\x00\x00"/*@ L"DirectInput �֘A�̏I������... ����\n" @*/ ) ;

		// �W���C�p�b�h�f�o�C�X�̉��
		TerminateJoypad() ;

		// �L�[�{�[�h�f�o�C�X�̉��
		if( InputSysData.PF.KeyboardDeviceObject != NULL )
		{
			InputSysData.PF.KeyboardDeviceObject->SetEventNotification( NULL ) ;
			InputSysData.PF.KeyboardDeviceObject->Unacquire() ;
			InputSysData.PF.KeyboardDeviceObject->Release() ;
			InputSysData.PF.KeyboardDeviceObject = NULL ;
			CloseHandle( InputSysData.PF.KeyEvent ) ;
		}

		// �}�E�X�f�o�C�X�̉��
		if( InputSysData.PF.MouseDeviceObject != NULL )
		{
			InputSysData.PF.MouseDeviceObject->Unacquire() ;
			InputSysData.PF.MouseDeviceObject->Release() ;
			InputSysData.PF.MouseDeviceObject = NULL ;
		}

		// �c�����������h���������I�u�W�F�N�g�̉��
		InputSysData.PF.DirectInputObject->Release() ;
		InputSysData.PF.DirectInputObject = NULL ;

		// �w�h���������̌�n������
		if( InputSysData.PF.XInputDLL )
		{
			FreeLibrary( InputSysData.PF.XInputDLL ) ;
			InputSysData.PF.XInputDLL = NULL ;
			InputSysData.PF.XInputGetStateFunc = NULL ;
			InputSysData.PF.XInputSetStateFunc = NULL ;
		}
	}
	else
	{
		// DirectInput ���g�p���Ȃ������ꍇ
		DXST_ERRORLOG_ADDUTF16LE( "\x65\x51\x9b\x52\xa2\x95\x23\x90\x6e\x30\x42\x7d\x86\x4e\xe6\x51\x06\x74\x2e\x00\x2e\x00\x2e\x00\x20\x00\x8c\x5b\x86\x4e\x0a\x00\x00"/*@ L"���͊֘A�̏I������... ����\n" @*/ ) ;

		InputSysData.PF.DirectInputObject = NULL ;
	}

	// ����I��
	return 0 ;
}

// �������������s�����ˑ�����
extern int AutoInitialize_PF( void )
{
	// �E�C���h�E���N���[�Y�����O�Ȃ珉�������s��
	if( NS_GetWindowCloseFlag() == FALSE )
	{
		return InitializeInputSystem() ;
	}

	// ����ȊO�̏ꍇ�͒ʏ�I��
	return 0 ;
}

// �W���C�p�b�h�̃Z�b�g�A�b�v�̊��ˑ�����
extern int SetupJoypad_PF( void )
{
	// ����������
	if( InputSysData.PF.DirectInputObject == NULL || InputSysData.PF.DirectInputObject == (D_IDirectInput7 *)(DWORD_PTR)0xffffffff )
	{
		return -1 ;
	}

	// ���ɏ���������Ă����ꍇ���l���Č�n�����ŏ��ɍs��
	TerminateJoypad() ;

	// �W���C�p�b�h�f�o�C�X������������
	DXST_ERRORLOG_ADDUTF16LE( "\xb8\x30\xe7\x30\xa4\x30\xd1\x30\xc3\x30\xc9\x30\x6e\x30\x1d\x52\x1f\x67\x16\x53\x2e\x00\x2e\x00\x2e\x00\x20\x00\x0a\x00\x00"/*@ L"�W���C�p�b�h�̏�����... \n" @*/ ) ;
	InputSysData.PadNum = 0 ;

	// XInput �Ŏ擾�ł���p�b�h���Ɍ��o
	if( InputSysData.PF.XInputDLL != NULL )
	{
		DWORD i ;
		DWORD j ;
		INPUTPADDATA *pad ;
		D_XINPUT_STATE state ;

		pad = &InputSysData.Pad[ InputSysData.PadNum ] ;
		for( i = 0 ; i < 4 ; i ++ )
		{
			if( InputSysData.PF.XInputGetStateFunc( i, &state ) != ERROR_SUCCESS )
				continue ;

			DXST_ERRORLOGFMT_ADDUTF16LE(( "\xb8\x30\xe7\x30\xa4\x30\xd1\x30\xc3\x30\xc9\x30\xca\x30\xf3\x30\xd0\x30\xfc\x30\x20\x00\x25\x00\x64\x00\x20\x00\x6f\x30\x20\x00\x58\x00\x62\x00\x6f\x00\x78\x00\x33\x00\x36\x00\x30\x00\x20\x00\x43\x00\x6f\x00\x6e\x00\x74\x00\x72\x00\x6f\x00\x6c\x00\x6c\x00\x65\x00\x72\x00\x20\x00\x67\x30\x59\x30\x00"/*@ L"�W���C�p�b�h�i���o�[ %d �� Xbox360 Controller �ł�" @*/, InputSysData.PadNum )) ;

			// XInput �ł̃f�o�C�X�ԍ���ۑ�
			pad->PF.XInputDeviceNo = ( int )i ;

			// �����]�[���̃Z�b�g
			pad->DeadZone = DEADZONE ;

			// �U���֌W�̐ݒ�̏��������s��
			for( j = 0 ; j < DINPUTPAD_MOTOR_NUM ; j ++ )
			{
				pad->Effect[ j ].PF.DIEffect   = NULL ;
				pad->Effect[ j ].PlayFlag	= FALSE ;
				pad->Effect[ j ].Power		= 0 ;
				pad->Effect[ j ].PlayState  = FALSE ;
				pad->Effect[ j ].SetPower   = 0 ;
			}
//			pad->EffectNotCommonFlag    = FALSE ;
//			pad->EffectCommon.PlayFlag	= FALSE ;
//			pad->EffectCommon.Power		= D_DI_FFNOMINALMAX ;
//			pad->Effect[ DINPUTPAD_MOTOR_LEFT ].PlayFlag	= FALSE ;
//			pad->Effect[ DINPUTPAD_MOTOR_LEFT ].Power		= D_DI_FFNOMINALMAX ;
//			pad->Effect[ DINPUTPAD_MOTOR_RIGHT ].PlayFlag	= FALSE ;
//			pad->Effect[ DINPUTPAD_MOTOR_RIGHT ].Power		= D_DI_FFNOMINALMAX ;
//			pad->EffectPlayStateLeft	= FALSE ;
//			pad->EffectPlayStateRight	= FALSE ;

			// �X�e�[�^�X��������
			_MEMSET( &pad->State, 0, sizeof( D_DIJOYSTATE ) ) ;
			_MEMSET( &pad->XInputState, 0, sizeof( pad->XInputState ) ) ;

			// ���O��ݒ�
			_WCSCPY_S( pad->PF.InstanceName, sizeof( pad->PF.InstanceName ), L"Controller (XBOX 360 For Windows)" );
			_WCSCPY_S( pad->PF.ProductName,  sizeof( pad->PF.ProductName  ), L"Controller (XBOX 360 For Windows)" );

			pad ++ ;
			InputSysData.PadNum ++ ;
		}
	}

	InputSysData.PF.DirectInputObject->EnumDevices( D_DIDEVTYPE_JOYSTICK, EnumJoypadProc, NULL, D_DIEDFL_ATTACHEDONLY ) ;

	// ����I��
	return 0 ;
}

// �W���C�p�b�h�̌�n�����s�������̊��ˑ�����
extern int TerminateJoypad_PF( void )
{
	int i ;
	int j ;
	INPUTPADDATA *pad ;

	// ����������
	if( InputSysData.PF.DirectInputObject == NULL || InputSysData.PF.DirectInputObject == (D_IDirectInput7 *)(DWORD_PTR)0xffffffff )
	{
		return -1 ;
	}

	// �W���C�p�b�h�f�o�C�X�̉��
	pad = InputSysData.Pad ;
	for( i = 0 ; i < InputSysData.PadNum ; i ++ , pad ++ )
	{
		if( pad->PF.XInputDeviceNo < 0 )
		{
			pad->PF.Device->Unacquire() ;
			pad->PF.Device->SetEventNotification( NULL ) ;

			for( j = 0 ; j < DINPUTPAD_MOTOR_NUM ; j ++ )
			{
				if( pad->Effect[ j ].PF.DIEffect != NULL )
				{
					pad->Effect[ j ].PF.DIEffect->Stop() ;
					pad->Effect[ j ].PF.DIEffect->Unload() ;
					pad->Effect[ j ].PF.DIEffect = NULL ;
				}
			}

			pad->PF.Device->Release() ;
			pad->PF.Device = NULL ;

			CloseHandle( pad->PF.Event ) ;
		}
		
		_MEMSET( pad, 0, sizeof( *pad ) ) ;
	}

	InputSysData.PadNum = 0 ;

	// ����I��
	return 0 ;
}

// ���͏�Ԃ̍X�V�̊��ˑ�����
extern int UpdateKeyboardInputState_PF( int UseProcessMessage )
{
	unsigned char KeyBuffer[256] ;
	int Time, hr, i ;
	int Result = 0 ;

	// �L�[�{�[�h���͏����� DirectInput ���g�p���邩�ǂ����ŏ����𕪊�
	if( InputSysData.PF.NoUseDirectInputFlag          == TRUE ||
		InputSysData.PF.KeyboardNotUseDirectInputFlag == TRUE ||
		InputSysData.PF.KeyboardDeviceObject          == NULL ||
		( InputSysData.PF.UseDirectInputFlag == FALSE && WinData.WindowsVersion >= DX_WINDOWSVERSION_VISTA ) )
	{
		// ���C���X���b�h�ȊO����Ă΂ꂽ�ꍇ�͉��������ɏI��
		if( GetCurrentThreadId() != WinData.MainThreadID )
		{
			goto ENDFUNCTION ;
		}

		// �܂��O��̎擾���� 1msec ���o�߂��Ă��Ȃ��ꍇ�͈ȑO�̏�Ԃ�Ԃ�
		Time = ( int )WinAPIData.Win32Func.timeGetTimeFunc() ;
		if( InputSysData.KeyInputGetTime == Time ) goto ENDFUNCTION ;
		InputSysData.KeyInputGetTime = Time ;

		// �L�[���͂̃��b�Z�[�W�����Ă���\��������̂ŁA���b�Z�[�W�������s��
		if( UseProcessMessage != FALSE && NS_ProcessMessage() != 0 )
		{
			goto ENDFUNCTION ;
		}

		// �W���`�o�h�œ��͏�Ԃ��擾����
		GetKeyboardState( KeyBuffer ) ;
		_MEMSET( &InputSysData.KeyInputBuf, 0, sizeof( InputSysData.KeyInputBuf ) ) ;
		for( i = 0 ; __KeyMap[i][0] != 0xffff ; i ++ )
			InputSysData.KeyInputBuf[__KeyMap[i][1]] = KeyBuffer[__KeyMap[i][0]] ;

		InputSysData.MouseInputBuf[ 0 ] = ( BYTE )( ( KeyBuffer[ VK_LBUTTON ] & 0x80 ) ? 1 : 0 ) ;
		InputSysData.MouseInputBuf[ 1 ] = ( BYTE )( ( KeyBuffer[ VK_RBUTTON ] & 0x80 ) ? 1 : 0 ) ;
		InputSysData.MouseInputBuf[ 2 ] = ( BYTE )( ( KeyBuffer[ VK_MBUTTON ] & 0x80 ) ? 1 : 0 ) ;
		InputSysData.MouseInputBufValidFlag = TRUE ;
	}
	else
	{
		// �܂��O��̎擾���� 1msec ���o�߂��Ă��Ȃ��ꍇ�͈ȑO�̏�Ԃ�Ԃ�
		Time = ( int )WinAPIData.Win32Func.timeGetTimeFunc() ;
		if( InputSysData.KeyInputGetTime == Time && WaitForSingleObject( InputSysData.PF.KeyEvent, 0 ) != WAIT_OBJECT_0 )
		{
			goto ENDFUNCTION ;
		}

		// �L�[���͂̃��b�Z�[�W�����Ă���\��������̂ŁA���b�Z�[�W�������s��
		if( UseProcessMessage != FALSE && NS_ProcessMessage() != 0 )
		{
			goto ENDFUNCTION ;
		}

		// ����������
		if( InputSysData.PF.DirectInputObject == NULL )
		{
			goto ENDFUNCTION ;
		}

		// �X�e�[�^�X�擾�A���X�g���Ă�����Ď擾
		hr = InputSysData.PF.KeyboardDeviceObject->GetDeviceState( sizeof( InputSysData.KeyInputBuf ) , ( LPVOID )InputSysData.KeyInputBuf ) ;
//		if( hr == DIERR_INPUTLOST )
		if( hr != D_DI_OK )
		{
			if( InputSysData.PF.KeyboardDeviceObject->Acquire() == D_DI_OK )
			{
				hr = InputSysData.PF.KeyboardDeviceObject->GetDeviceState( sizeof( InputSysData.KeyInputBuf ) , ( LPVOID )InputSysData.KeyInputBuf ) ;
				if( hr != D_DI_OK ) goto ENDFUNCTION ;
			}
			else
			{
				goto ENDFUNCTION ;
			}
		}

		InputSysData.KeyInputGetTime = Time ;
		
		// �C�x���g�����Z�b�g
		if( WaitForSingleObject( InputSysData.PF.KeyEvent, 0 ) == WAIT_OBJECT_0 ) ResetEvent( InputSysData.PF.KeyEvent ) ;

		// �E�V�t�g�L�[��W���`�o�h������
		GetKeyboardState( KeyBuffer ) ;
		InputSysData.KeyInputBuf[ KEY_INPUT_RSHIFT ] = KeyBuffer[ VK_RSHIFT ] ;

		// ******* �c�����������h�����������M�p�ł��Ȃ��̂ŁA�ꕔ�̃L�[�������W���`�o�h�ŃL�[��Ԃ��擾 ******* //
/*		if( InputSysData.PF.KeyExclusiveCooperativeLevelFlag == FALSE )
		{
			//���C���X���b�h�ȊO�������炱�̏������s��Ȃ�
			if( WinData.MainThreadID == GetCurrentThreadId() )
			{
				// �W���`�o�h�œ��͏�Ԃ��擾����
				GetKeyboardState( KeyBuffer ) ;
				for( i = 0 ; __KeyMap[i][0] != 0xffff ; i ++ )
					if( __KeyMap[i][2] != 1 )
						InputSysData.KeyInputBuf[__KeyMap[i][1]] = KeyBuffer[__KeyMap[i][0]] ;
			}
		}
*/	}

	Result = 1 ;

ENDFUNCTION:

	return Result ;
}

// �p�b�h�̓��͏�Ԃ̍X�V�̊��ˑ�����
extern int UpdateJoypadInputState_PF( int PadNo )
{
	INPUTPADDATA *pad = &InputSysData.Pad[ PadNo ] ;
	D_DIJOYSTATE InputState ;
	JOYINFOEX joyex ;
	HRESULT hr ;
	int Time, i ;

	// DirectInput ���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( InputSysData.PF.NoUseDirectInputFlag == TRUE )
	{
		// DirectInput ���g�p���Ȃ��ꍇ

		// �O��̎擾���� 1msec ���o�߂��Ă��Ȃ��ꍇ�͈ȑO�̏�Ԃ�Ԃ�
		Time = ( int )WinAPIData.Win32Func.timeGetTimeFunc() ;
		if( pad->GetTime == Time ) return 0 ;
		pad->GetTime = Time ;

		// ���͏�Ԃ��擾����
		_MEMSET( &joyex, 0, sizeof( joyex ) ) ;
		joyex.dwSize  = sizeof( JOYINFOEX ) ;
		joyex.dwFlags = JOY_RETURNALL ;
		if( WinAPIData.Win32Func.joyGetPosExFunc( ( UINT )PadNo, &joyex ) != JOYERR_NOERROR )
		{
			// �擾�ł��Ȃ������ꍇ�͐ڑ�����Ă��Ȃ��Ƃ������ƂȂ̂ŏ����[���Ŗ��߂Ă���
			_MEMSET( &pad->State, 0, sizeof( pad->State ) ) ;
		}
		else
		{
			// �}���`���f�B�A�`�o�h�p�̓��͒l�� DirectInput �p�ɕϊ�����

			switch( joyex.dwXpos )
			{
			case 0x7fff :	pad->State.X =             0 ;	break ;
			case 0x0000 :	pad->State.X = -DIRINPUT_MAX ;	break ;
			case 0xffff :	pad->State.X =  DIRINPUT_MAX ;	break ;
			default :
				     if( joyex.dwXpos < 0x8000 - DEADZONE_WINMM( pad->DeadZone ) ) pad->State.X = ( LONG )( -( LONG )( ( ( 0x8000 - DEADZONE_WINMM( pad->DeadZone ) ) - (LONG)joyex.dwXpos ) * DIRINPUT_MAX / VALIDRANGE_WINMM( pad->DeadZone ) ) ) ;
				else if( joyex.dwXpos > 0x8000 + DEADZONE_WINMM( pad->DeadZone ) ) pad->State.X = ( LONG )(          ( ( (LONG)joyex.dwXpos - ( 0x8000 + DEADZONE_WINMM( pad->DeadZone ) ) ) * DIRINPUT_MAX / VALIDRANGE_WINMM( pad->DeadZone ) ) ) ;
				else                                                               pad->State.X = 0 ;
				break ;
			}

			switch( joyex.dwYpos )
			{
			case 0x7fff :	pad->State.Y =             0 ;	break ;
			case 0x0000 :	pad->State.Y = -DIRINPUT_MAX ;	break ;
			case 0xffff :	pad->State.Y =  DIRINPUT_MAX ;	break ;
			default :
				     if( joyex.dwYpos < 0x8000 - DEADZONE_WINMM( pad->DeadZone ) ) pad->State.Y = ( LONG )( -( LONG )( ( ( 0x8000 - DEADZONE_WINMM( pad->DeadZone ) ) - (LONG)joyex.dwYpos ) * DIRINPUT_MAX / VALIDRANGE_WINMM( pad->DeadZone ) ) ) ;
				else if( joyex.dwYpos > 0x8000 + DEADZONE_WINMM( pad->DeadZone ) ) pad->State.Y = ( LONG )(          ( ( (LONG)joyex.dwYpos - ( 0x8000 + DEADZONE_WINMM( pad->DeadZone ) ) ) * DIRINPUT_MAX / VALIDRANGE_WINMM( pad->DeadZone ) ) ) ;
				else                                                               pad->State.Y = 0 ;
				break ;
			}

			if( pad->PF.RightStickFlag )
			{
				switch( joyex.dwZpos )
				{
				case 0x7fff :	pad->State.Z =             0 ;	break ;
				case 0x0000 :	pad->State.Z = -DIRINPUT_MAX ;	break ;
				case 0xffff :	pad->State.Z =  DIRINPUT_MAX ;	break ;
				default :
						 if( joyex.dwZpos < 0x8000 - DEADZONE_WINMM( pad->DeadZone ) ) pad->State.Z = ( LONG )( -( LONG )( ( ( 0x8000 - DEADZONE_WINMM( pad->DeadZone ) ) - (LONG)joyex.dwZpos ) * DIRINPUT_MAX / VALIDRANGE_WINMM( pad->DeadZone ) ) ) ;
					else if( joyex.dwZpos > 0x8000 + DEADZONE_WINMM( pad->DeadZone ) ) pad->State.Z = ( LONG )(          ( ( (LONG)joyex.dwZpos - ( 0x8000 + DEADZONE_WINMM( pad->DeadZone ) ) ) * DIRINPUT_MAX / VALIDRANGE_WINMM( pad->DeadZone ) ) ) ;
					else                                                               pad->State.Z = 0 ;
					break ;
				}

				switch( joyex.dwRpos )
				{
				case 0x7fff :	pad->State.Rz =             0 ;	break ;
				case 0x0000 :	pad->State.Rz = -DIRINPUT_MAX ;	break ;
				case 0xffff :	pad->State.Rz =  DIRINPUT_MAX ;	break ;
				default :
						 if( joyex.dwRpos < 0x8000 - DEADZONE_WINMM( pad->DeadZone ) ) pad->State.Rz = ( LONG )( -( LONG )( ( ( 0x8000 - DEADZONE_WINMM( pad->DeadZone ) ) - (LONG)joyex.dwRpos ) * DIRINPUT_MAX / VALIDRANGE_WINMM( pad->DeadZone ) ) ) ;
					else if( joyex.dwRpos > 0x8000 + DEADZONE_WINMM( pad->DeadZone ) ) pad->State.Rz = ( LONG )(          ( ( (LONG)joyex.dwRpos - ( 0x8000 + DEADZONE_WINMM( pad->DeadZone ) ) ) * DIRINPUT_MAX / VALIDRANGE_WINMM( pad->DeadZone ) ) ) ;
					else                                                               pad->State.Rz = 0 ;
					break ;
				}

				switch( joyex.dwUpos )
				{
				case 0x7fff :	pad->State.Rx =             0 ;	break ;
				case 0x0000 :	pad->State.Rx = -DIRINPUT_MAX ;	break ;
				case 0xffff :	pad->State.Rx =  DIRINPUT_MAX ;	break ;
				default :
						 if( joyex.dwUpos < 0x8000 - DEADZONE_WINMM( pad->DeadZone ) ) pad->State.Rx = ( LONG )( -( LONG )( ( ( 0x8000 - DEADZONE_WINMM( pad->DeadZone ) ) - (LONG)joyex.dwUpos ) * DIRINPUT_MAX / VALIDRANGE_WINMM( pad->DeadZone ) ) ) ;
					else if( joyex.dwUpos > 0x8000 + DEADZONE_WINMM( pad->DeadZone ) ) pad->State.Rx = ( LONG )(          ( ( (LONG)joyex.dwUpos - ( 0x8000 + DEADZONE_WINMM( pad->DeadZone ) ) ) * DIRINPUT_MAX / VALIDRANGE_WINMM( pad->DeadZone ) ) ) ;
					else                                                               pad->State.Rx = 0 ;
					break ;
				}
			}
			else
			{
				pad->State.Z = 0 ;
				pad->State.Rx = 0 ;
				pad->State.Rz = 0 ;
			}

			for( i = 0 ; i < 24 ; i ++ )
			{
				if( joyex.dwButtons & ( 1 << i ) ) pad->State.Buttons[ i ] = 0x80 ;
				else                               pad->State.Buttons[ i ] = 0x00 ;
			}
		}
	}
	else
	if( pad->PF.XInputDeviceNo < 0 )
	{
		// DirectInput ���g�p����ꍇ

		// �O��̎擾���� 1msec ���o�߂��Ă��Ȃ��ꍇ�͈ȑO�̏�Ԃ�Ԃ�
		Time = ( int )WinAPIData.Win32Func.timeGetTimeFunc() ;
		if( pad->GetTime == Time && WaitForSingleObject( pad->PF.Event, 0 ) != WAIT_OBJECT_0 )
		{
			return 0 ;
		}
		pad->GetTime = Time ;

		// ���̓f�[�^�̃|�[�����O
		pad->PF.Device->Poll() ;

		hr = pad->PF.Device->GetDeviceState( sizeof( D_DIJOYSTATE ) , &InputState ) ;
		if( hr != D_DI_OK )
		{
			if( NS_ProcessMessage() != 0 )
			{
				return -1 ;
			}
			if( InputSysData.PF.DirectInputObject == NULL )
			{
				return -1 ;
			}

			if( pad->PF.Device->Acquire() == D_DI_OK )
			{
				if( NS_ProcessMessage() )
				{
					return -1 ;
				}
				if( InputSysData.PF.DirectInputObject == NULL )
				{
					return -1 ;
				}

				hr = pad->PF.Device->GetDeviceState( sizeof( D_DIJOYSTATE ) , &InputState ) ;
			}
		}

		if( hr == D_DI_OK )
		{
			pad->State.X = InputState.lX ;
			pad->State.Y = InputState.lY ;
			pad->State.Z = InputState.lZ ;
			pad->State.Rx = InputState.lRx ;
			pad->State.Ry = InputState.lRy ;
			pad->State.Rz = InputState.lRz ;
			pad->State.Slider[ 0 ] = InputState.rglSlider[ 0 ] ;
			pad->State.Slider[ 1 ] = InputState.rglSlider[ 1 ] ;
			pad->State.POV[ 0 ] = InputState.rgdwPOV[ 0 ] ;
			pad->State.POV[ 1 ] = InputState.rgdwPOV[ 1 ] ;
			pad->State.POV[ 2 ] = InputState.rgdwPOV[ 2 ] ;
			pad->State.POV[ 3 ] = InputState.rgdwPOV[ 3 ] ;
			pad->State.Buttons[ 0 ] = InputState.rgbButtons[ 0 ] ;
			pad->State.Buttons[ 1 ] = InputState.rgbButtons[ 1 ] ;
			pad->State.Buttons[ 2 ] = InputState.rgbButtons[ 2 ] ;
			pad->State.Buttons[ 3 ] = InputState.rgbButtons[ 3 ] ;
			pad->State.Buttons[ 4 ] = InputState.rgbButtons[ 4 ] ;
			pad->State.Buttons[ 5 ] = InputState.rgbButtons[ 5 ] ;
			pad->State.Buttons[ 6 ] = InputState.rgbButtons[ 6 ] ;
			pad->State.Buttons[ 7 ] = InputState.rgbButtons[ 7 ] ;
			pad->State.Buttons[ 8 ] = InputState.rgbButtons[ 8 ] ;
			pad->State.Buttons[ 9 ] = InputState.rgbButtons[ 9 ] ;
			pad->State.Buttons[ 10 ] = InputState.rgbButtons[ 10 ] ;
			pad->State.Buttons[ 11 ] = InputState.rgbButtons[ 11 ] ;
			pad->State.Buttons[ 12 ] = InputState.rgbButtons[ 12 ] ;
			pad->State.Buttons[ 13 ] = InputState.rgbButtons[ 13 ] ;
			pad->State.Buttons[ 14 ] = InputState.rgbButtons[ 14 ] ;
			pad->State.Buttons[ 15 ] = InputState.rgbButtons[ 15 ] ;
			pad->State.Buttons[ 16 ] = InputState.rgbButtons[ 16 ] ;
			pad->State.Buttons[ 17 ] = InputState.rgbButtons[ 17 ] ;
			pad->State.Buttons[ 18 ] = InputState.rgbButtons[ 18 ] ;
			pad->State.Buttons[ 19 ] = InputState.rgbButtons[ 19 ] ;
			pad->State.Buttons[ 20 ] = InputState.rgbButtons[ 20 ] ;
			pad->State.Buttons[ 21 ] = InputState.rgbButtons[ 21 ] ;
			pad->State.Buttons[ 22 ] = InputState.rgbButtons[ 22 ] ;
			pad->State.Buttons[ 23 ] = InputState.rgbButtons[ 23 ] ;
			pad->State.Buttons[ 24 ] = InputState.rgbButtons[ 24 ] ;
			pad->State.Buttons[ 25 ] = InputState.rgbButtons[ 25 ] ;
			pad->State.Buttons[ 26 ] = InputState.rgbButtons[ 26 ] ;
			pad->State.Buttons[ 27 ] = InputState.rgbButtons[ 27 ] ;
			pad->State.Buttons[ 28 ] = InputState.rgbButtons[ 28 ] ;
			pad->State.Buttons[ 29 ] = InputState.rgbButtons[ 29 ] ;
			pad->State.Buttons[ 30 ] = InputState.rgbButtons[ 30 ] ;
			pad->State.Buttons[ 31 ] = InputState.rgbButtons[ 31 ] ;
		}

		// �C�x���g�̃��Z�b�g
		if( WaitForSingleObject( pad->PF.Event, 0 ) == WAIT_OBJECT_0 ) ResetEvent( pad->PF.Event ) ;
	}
	else
	{
		D_XINPUT_STATE XInputState ;

		// XInput ���g�p����ꍇ

		// �O��̎擾���� 1msec ���o�߂��Ă��Ȃ��ꍇ�͈ȑO�̏�Ԃ�Ԃ�
		Time = ( int )WinAPIData.Win32Func.timeGetTimeFunc() ;
		if( pad->GetTime == Time ) return 0 ;
		pad->GetTime = Time ;

		if( InputSysData.PF.XInputGetStateFunc == NULL ||
			InputSysData.PF.XInputGetStateFunc( ( DWORD )pad->PF.XInputDeviceNo, &XInputState ) != ERROR_SUCCESS )
		{
			// �擾�ł��Ȃ������ꍇ�͐ڑ�����Ă��Ȃ��Ƃ������ƂȂ̂ŏ����[���Ŗ��߂Ă���
			_MEMSET( &pad->XInputState, 0, sizeof( pad->XInputState ) ) ;
		}
		else
		{
			WORD Buttons ;

			// XInput�p�̓��͒l��ۑ�
			pad->XInputState.Buttons[ 0  ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 0 ) & 1 ) ;
			pad->XInputState.Buttons[ 1  ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 1 ) & 1 ) ;
			pad->XInputState.Buttons[ 2  ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 2 ) & 1 ) ;
			pad->XInputState.Buttons[ 3  ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 3 ) & 1 ) ;
			pad->XInputState.Buttons[ 4  ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 4 ) & 1 ) ;
			pad->XInputState.Buttons[ 5  ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 5 ) & 1 ) ;
			pad->XInputState.Buttons[ 6  ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 6 ) & 1 ) ;
			pad->XInputState.Buttons[ 7  ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 7 ) & 1 ) ;
			pad->XInputState.Buttons[ 8  ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 8 ) & 1 ) ;
			pad->XInputState.Buttons[ 9  ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 9 ) & 1 ) ;
			pad->XInputState.Buttons[ 10 ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 10 ) & 1 ) ;
			pad->XInputState.Buttons[ 11 ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 11 ) & 1 ) ;
			pad->XInputState.Buttons[ 12 ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 12 ) & 1 ) ;
			pad->XInputState.Buttons[ 13 ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 13 ) & 1 ) ;
			pad->XInputState.Buttons[ 14 ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 14 ) & 1 ) ;
			pad->XInputState.Buttons[ 15 ] = ( unsigned char )( ( XInputState.Gamepad.wButtons >> 15 ) & 1 ) ;
			pad->XInputState.LeftTrigger   = XInputState.Gamepad.bLeftTrigger ;
			pad->XInputState.RightTrigger  = XInputState.Gamepad.bRightTrigger ;
			pad->XInputState.ThumbLX       = XInputState.Gamepad.sThumbLX ;
			pad->XInputState.ThumbLY       = XInputState.Gamepad.sThumbLY ;
			pad->XInputState.ThumbRX       = XInputState.Gamepad.sThumbRX ;
			pad->XInputState.ThumbRY       = XInputState.Gamepad.sThumbRY ;

			// XInput�p�̓��͒l�� DirectInput �p�ɕϊ�����

			switch( XInputState.Gamepad.sThumbLX )
			{
			case  32767 : pad->State.X =  DIRINPUT_MAX ; break ;
			case -32768 : pad->State.X = -DIRINPUT_MAX ; break ;
			default :
				     if( XInputState.Gamepad.sThumbLX < -DEADZONE_XINPUT( pad->DeadZone ) ) pad->State.X = -( ( -XInputState.Gamepad.sThumbLX - DEADZONE_XINPUT( pad->DeadZone ) ) * DIRINPUT_MAX / VALIDRANGE_XINPUT( pad->DeadZone ) ) ;
				else if( XInputState.Gamepad.sThumbLX >  DEADZONE_XINPUT( pad->DeadZone ) ) pad->State.X =  ( (  XInputState.Gamepad.sThumbLX - DEADZONE_XINPUT( pad->DeadZone ) ) * DIRINPUT_MAX / VALIDRANGE_XINPUT( pad->DeadZone ) ) ;
				else                                                                        pad->State.X = 0 ;
				break ;
			}

			switch( XInputState.Gamepad.sThumbLY )
			{
			case  32767 : pad->State.Y = -DIRINPUT_MAX ; break ;
			case -32768 : pad->State.Y =  DIRINPUT_MAX ; break ;
			default :
				     if( XInputState.Gamepad.sThumbLY < -DEADZONE_XINPUT( pad->DeadZone ) ) pad->State.Y =  ( ( -XInputState.Gamepad.sThumbLY - DEADZONE_XINPUT( pad->DeadZone ) ) * DIRINPUT_MAX / VALIDRANGE_XINPUT( pad->DeadZone ) ) ;
				else if( XInputState.Gamepad.sThumbLY >  DEADZONE_XINPUT( pad->DeadZone ) ) pad->State.Y = -( (  XInputState.Gamepad.sThumbLY - DEADZONE_XINPUT( pad->DeadZone ) ) * DIRINPUT_MAX / VALIDRANGE_XINPUT( pad->DeadZone ) ) ;
				else                                                                        pad->State.Y = 0 ;
				break ;
			}

			switch( XInputState.Gamepad.sThumbRX )
			{
			case  32767 : pad->State.Rx =  DIRINPUT_MAX ; break ;
			case -32768 : pad->State.Rx = -DIRINPUT_MAX ; break ;
			default :
				     if( XInputState.Gamepad.sThumbRX < -DEADZONE_XINPUT( pad->DeadZone ) ) pad->State.Rx = -( ( -XInputState.Gamepad.sThumbRX - DEADZONE_XINPUT( pad->DeadZone ) ) * DIRINPUT_MAX / VALIDRANGE_XINPUT( pad->DeadZone ) ) ;
				else if( XInputState.Gamepad.sThumbRX >  DEADZONE_XINPUT( pad->DeadZone ) ) pad->State.Rx =  ( (  XInputState.Gamepad.sThumbRX - DEADZONE_XINPUT( pad->DeadZone ) ) * DIRINPUT_MAX / VALIDRANGE_XINPUT( pad->DeadZone ) ) ;
				else                                                                        pad->State.Rx = 0 ;
				break ;
			}

			switch( XInputState.Gamepad.sThumbRY )
			{
			case  32767 : pad->State.Ry = -DIRINPUT_MAX ; break ;
			case -32768 : pad->State.Ry =  DIRINPUT_MAX ; break ;
			default :
				     if( XInputState.Gamepad.sThumbRY < -DEADZONE_XINPUT( pad->DeadZone ) ) pad->State.Ry =  ( ( -XInputState.Gamepad.sThumbRY - DEADZONE_XINPUT( pad->DeadZone ) ) * DIRINPUT_MAX / VALIDRANGE_XINPUT( pad->DeadZone ) ) ;
				else if( XInputState.Gamepad.sThumbRY >  DEADZONE_XINPUT( pad->DeadZone ) ) pad->State.Ry = -( (  XInputState.Gamepad.sThumbRY - DEADZONE_XINPUT( pad->DeadZone ) ) * DIRINPUT_MAX / VALIDRANGE_XINPUT( pad->DeadZone ) ) ;
				else                                                                        pad->State.Ry = 0 ;
				break ;
			}

			Buttons = XInputState.Gamepad.wButtons ;
			for( i = 0 ; i < XINPUT_TO_DIRECTINPUT_BUTTONNUM ; i ++ )
			{
				pad->State.Buttons[ i ] = ( BYTE )( ( Buttons & XInputButtonToDirectInputButtonNo[ i ] ) ? 0x80 : 0x00 ) ;
			}

			     if( ( Buttons & D_XINPUT_GAMEPAD_DPAD_LEFT  ) == 0 && ( Buttons & D_XINPUT_GAMEPAD_DPAD_UP    ) != 0 && ( Buttons & D_XINPUT_GAMEPAD_DPAD_RIGHT ) == 0 ) pad->State.POV[ 0 ] =     0 ;
			else if(                                                   ( Buttons & D_XINPUT_GAMEPAD_DPAD_UP    ) != 0 && ( Buttons & D_XINPUT_GAMEPAD_DPAD_RIGHT ) != 0 ) pad->State.POV[ 0 ] =  4500 ;
			else if( ( Buttons & D_XINPUT_GAMEPAD_DPAD_UP    ) == 0 && ( Buttons & D_XINPUT_GAMEPAD_DPAD_RIGHT ) != 0 && ( Buttons & D_XINPUT_GAMEPAD_DPAD_DOWN  ) == 0 ) pad->State.POV[ 0 ] =  9000 ;
			else if(                                                   ( Buttons & D_XINPUT_GAMEPAD_DPAD_RIGHT ) != 0 && ( Buttons & D_XINPUT_GAMEPAD_DPAD_DOWN  ) != 0 ) pad->State.POV[ 0 ] = 13500 ;
			else if( ( Buttons & D_XINPUT_GAMEPAD_DPAD_RIGHT ) == 0 && ( Buttons & D_XINPUT_GAMEPAD_DPAD_DOWN  ) != 0 && ( Buttons & D_XINPUT_GAMEPAD_DPAD_LEFT  ) == 0 ) pad->State.POV[ 0 ] = 18000 ;
			else if(                                                   ( Buttons & D_XINPUT_GAMEPAD_DPAD_DOWN  ) != 0 && ( Buttons & D_XINPUT_GAMEPAD_DPAD_LEFT  ) != 0 ) pad->State.POV[ 0 ] = 22500 ;
			else if( ( Buttons & D_XINPUT_GAMEPAD_DPAD_DOWN  ) == 0 && ( Buttons & D_XINPUT_GAMEPAD_DPAD_LEFT  ) != 0 && ( Buttons & D_XINPUT_GAMEPAD_DPAD_UP    ) == 0 ) pad->State.POV[ 0 ] = 27000 ;
			else if(                                                   ( Buttons & D_XINPUT_GAMEPAD_DPAD_LEFT  ) != 0 && ( Buttons & D_XINPUT_GAMEPAD_DPAD_UP    ) != 0 ) pad->State.POV[ 0 ] = 31500 ;
			else                                                                                                                                                          pad->State.POV[ 0 ] = 0xffffffff ;
			pad->State.POV[ 1 ] = 0xffffffff ;
			pad->State.POV[ 2 ] = 0xffffffff ;
			pad->State.POV[ 3 ] = 0xffffffff ;

			pad->State.Z = 0 ;
			if( XInputState.Gamepad.bLeftTrigger  > DEADZONE_XINPUT_TRIGGER( pad->DeadZone ) )
			{
				pad->State.Z += ( XInputState.Gamepad.bLeftTrigger  - DEADZONE_XINPUT_TRIGGER( pad->DeadZone ) ) * DIRINPUT_MAX / VALIDRANGE_XINPUT_TRIGGER( pad->DeadZone ) ;
			}
			if( XInputState.Gamepad.bRightTrigger > DEADZONE_XINPUT_TRIGGER( pad->DeadZone ) )
			{
				pad->State.Z -= ( XInputState.Gamepad.bRightTrigger - DEADZONE_XINPUT_TRIGGER( pad->DeadZone ) ) * DIRINPUT_MAX / VALIDRANGE_XINPUT_TRIGGER( pad->DeadZone ) ;
			}
		}
	}

	// �I��
	return 0 ;
}

// �p�b�h�G�t�F�N�g�̍Đ���Ԃ��X�V����֐��̊��ˑ�����
extern int RefreshEffectPlayState_PF( void )
{
	int                 i ;
	int                 j ;
	int                 num ;
	int                 time ;
	int                 PlayState ;
	int                 SetPower ;
	int                 Flag ;
	INPUTPADDATA        *Pad ;
	D_DICONSTANTFORCE   cf ;
	D_DIEFFECT          eff;
	D_XINPUT_VIBRATION  VibState ;

	// ����������
	if( InputSysData.PF.DirectInputObject == NULL /* && NS_GetWindowCloseFlag() == FALSE */ )
		return 0 ;

	// �c�����������h�����������g�p���Ă��Ȃ��ꍇ�̓G�t�F�N�g�����͏o���Ȃ��̂ŏI��
	if( InputSysData.PF.DirectInputObject == (D_IDirectInput7 *)(DWORD_PTR)0xffffffff ) return 0 ;

	// �p�b�h�f�o�C�X�̍Ď擾
	num  = InputSysData.PadNum ;
	Pad  = InputSysData.Pad ;
	time = NS_GetNowCount() ;

	// �G�t�F�N�g�\���̂̏�����
	_MEMSET( &eff, 0, sizeof( eff ) ) ;
	eff.dwSize                = sizeof( D_DIEFFECT ) ;
	eff.dwFlags               = D_DIEFF_CARTESIAN | D_DIEFF_OBJECTOFFSETS ;
	eff.lpEnvelope            = 0 ;
	eff.cbTypeSpecificParams  = sizeof( D_DICONSTANTFORCE ) ;
	eff.lpvTypeSpecificParams = &cf ;
	eff.dwStartDelay          = 0 ;

	// �\�t�g�̃A�N�e�B�u��Ԃɂ���ď����𕪊�
	if( NS_GetActiveFlag() == TRUE )
	{
		// �\�t�g���A�N�e�B�u�ȏꍇ
		for( i = 0 ; i < num ; i ++, Pad ++ )
		{
			// XInput ���ǂ����ŏ����𕪊�
			if( Pad->PF.XInputDeviceNo >= 0 )
			{
				// XInput �̏ꍇ
				VibState.wLeftMotorSpeed  = 0 ;
				VibState.wRightMotorSpeed = 0 ;
				Flag = 0 ;

				for( j = 0 ; j < DINPUTPAD_MOTOR_NUM ; j ++ )
				{
					if( Pad->Effect[ j ].PlayFlag )
					{
						SetPower = Pad->Effect[ j ].Power ;
					}
					else
					{
						SetPower = 0 ;
					}

					switch( j )
					{
					case DINPUTPAD_MOTOR_LEFT :
						VibState.wLeftMotorSpeed  = ( WORD )( SetPower * 65535 / D_DI_FFNOMINALMAX ) ;
						break ;

					case DINPUTPAD_MOTOR_RIGHT :
						VibState.wRightMotorSpeed = ( WORD )( SetPower * 65535 / D_DI_FFNOMINALMAX ) ;
						break ;
					}

					if( Pad->Effect[ j ].SetPower != SetPower )
					{
						Flag = 1 ;
						Pad->Effect[ j ].SetPower = SetPower ;
					}

					Pad->Effect[ j ].PlayState = Pad->Effect[ j ].PlayFlag ;
				}

				if( InputSysData.PF.XInputSetStateFunc != NULL && Flag == 1 )
				{
					InputSysData.PF.XInputSetStateFunc( ( DWORD )Pad->PF.XInputDeviceNo, &VibState ) ;
				}
			}
			else
			{
				if( Pad->PF.Device == NULL ) continue ;

				for( j = 0 ; j < DINPUTPAD_MOTOR_NUM ; j ++ )
				{
					if( Pad->Effect[ j ].PF.DIEffect == NULL )
					{
						continue ;
					}

					if( Pad->Effect[ j ].PlayFlag )
					{
						SetPower = Pad->Effect[ j ].Power ;
					}
					else
					{
						SetPower = 0 ;
					}

					PlayState = SetPower > 0 ? TRUE : FALSE ;

					if( Pad->Effect[ j ].SetPower  != SetPower ||
						Pad->Effect[ j ].PlayState != PlayState )
					{
						cf.lMagnitude = SetPower ;

						if( PlayState == TRUE )
						{
							if( Pad->Effect[ j ].PlayState == TRUE )
							{
								if( Pad->Effect[ j ].SetPower != SetPower )
								{
									Pad->Effect[ j ].PF.DIEffect->SetParameters( &eff, D_DIEP_TYPESPECIFICPARAMS | D_DIEP_START ) ;
								}
							}
							else
							if( Pad->Effect[ j ].PlayState == FALSE )
							{
								if( Pad->Effect[ j ].SetPower != SetPower )
								{
									Pad->Effect[ j ].PF.DIEffect->SetParameters( &eff, D_DIEP_TYPESPECIFICPARAMS ) ;
								}
								Pad->Effect[ j ].PF.DIEffect->Start( INFINITE, 0 ) ;
							}
						}
						else
						{
							if( Pad->Effect[ j ].PlayState )
							{
								Pad->Effect[ j ].PF.DIEffect->Stop() ;
							}

							if( Pad->Effect[ j ].SetPower != SetPower )
							{
								Pad->Effect[ j ].PF.DIEffect->SetParameters( &eff, D_DIEP_TYPESPECIFICPARAMS ) ;
							}
						}

						Pad->Effect[ j ].SetPower  = SetPower ;
						Pad->Effect[ j ].PlayState = Pad->Effect[ j ].PlayFlag ;
					}
				}
			}
		}
	}
	else
	{
		// �\�t�g����A�N�e�B�u�ȏꍇ
		for( i = 0 ; i < num ; i ++, Pad ++ )
		{
			if( Pad->PF.XInputDeviceNo >= 0 )
			{
				VibState.wLeftMotorSpeed  = 0 ;
				VibState.wRightMotorSpeed = 0 ;
				Flag = 0 ;

				for( j = 0 ; j < DINPUTPAD_MOTOR_NUM ; j ++ )
				{
					if( Pad->Effect[ j ].SetPower != 0 )
					{
						Flag = 1 ;
						Pad->Effect[ j ].SetPower = 0 ;
					}

					Pad->Effect[ j ].PlayState = FALSE ;
				}

				if( InputSysData.PF.XInputSetStateFunc != NULL && Flag == 1 )
				{
					InputSysData.PF.XInputSetStateFunc( ( DWORD )Pad->PF.XInputDeviceNo, &VibState ) ;
				}
			}
			else
			{
				if( Pad->PF.Device == NULL ) continue ;

				for( j = 0 ; j < DINPUTPAD_MOTOR_NUM ; j ++ )
				{
					if( Pad->Effect[ j ].PF.DIEffect == NULL )
					{
						continue ;
					}

					if( Pad->Effect[ j ].SetPower  != 0 ||
						Pad->Effect[ j ].PlayState != FALSE )
					{
						cf.lMagnitude = 0 ;

						if( Pad->Effect[ j ].PlayState )
						{
							Pad->Effect[ j ].PF.DIEffect->Stop() ;
						}

						if( Pad->Effect[ j ].SetPower != 0 )
						{
							Pad->Effect[ j ].PF.DIEffect->SetParameters( &eff, D_DIEP_TYPESPECIFICPARAMS ) ;
						}

						Pad->Effect[ j ].SetPower  = 0 ;
						Pad->Effect[ j ].PlayState = FALSE ;
					}
				}
			}
		}
	}

	// �I��
	return 0 ;
}

// �w��̃p�b�h���U���ɑΉ����Ă��邩�ǂ������擾����( TRUE:�Ή����Ă���  FALSE:�Ή����Ă��Ȃ� )
extern int CheckJoypadVibrationEnable_PF( INPUTPADDATA *pad, int EffectIndex )
{
	return ( pad->PF.XInputDeviceNo >= 0 || ( pad->PF.Device != NULL && pad->Effect[ EffectIndex ].PF.DIEffect != NULL ) ) ? TRUE : FALSE ;
}

// �w��̓��̓f�o�C�X�� XInput �ɑΉ����Ă��邩�ǂ������擾���鏈���̊��ˑ�����( �߂�l  TRUE:XInput�Ή��̓��̓f�o�C�X  FALSE:XInput��Ή��̓��̓f�o�C�X   -1:�G���[ )( DX_INPUT_KEY �� DX_INPUT_KEY_PAD1 �ȂǁA�L�[�{�[�h�����ރ^�C�v�� InputType �ɓn���ƃG���[�ƂȂ� -1 ��Ԃ� )�̊��ˑ�����
extern int CheckJoypadXInput_PF( int InputType )
{
	INPUTPADDATA *pad ;
	int JoypadNum = ( InputType & ~DX_INPUT_KEY ) - 1 ;

	pad = &InputSysData.Pad[ JoypadNum ] ;

	return pad->PF.XInputDeviceNo < 0 ? FALSE : TRUE ;
}

// �}�E�X�̃{�^���̏�Ԃ𓾂鏈���̊��ˑ�����
extern int GetMouseInput_PF( void )
{
	D_DIMOUSESTATE2 input ;
	HRESULT hr ;
	int res ;

	// DirectInput ���g�p���Ă��邩�ǂ����ŏ����𕪊�
	if( CheckUseDirectInputMouse() == FALSE )
	{
		// DirectInput ���g�p���Ȃ��ꍇ
		UpdateKeyboardInputState() ;
		if( InputSysData.MouseInputBufValidFlag == TRUE )
		{
			res = 0 ;
			if( InputSysData.MouseInputBuf[ 0 ] ) res |= MOUSE_INPUT_LEFT ;
			if( InputSysData.MouseInputBuf[ 1 ] ) res |= MOUSE_INPUT_RIGHT ;
			if( InputSysData.MouseInputBuf[ 2 ] ) res |= MOUSE_INPUT_MIDDLE ;
		}
		else
		{
			unsigned char KeyBuffer[256] ;

			GetKeyboardState( KeyBuffer ) ;
			res = 0 ;
			if( KeyBuffer[ VK_LBUTTON ] & 0x80 ) res |= MOUSE_INPUT_LEFT ;
			if( KeyBuffer[ VK_RBUTTON ] & 0x80 ) res |= MOUSE_INPUT_RIGHT ;
			if( KeyBuffer[ VK_MBUTTON ] & 0x80 ) res |= MOUSE_INPUT_MIDDLE ;
		}
	}
	else
	{
		// DirectInput ���g�p����ꍇ

		hr = InputSysData.PF.MouseDeviceObject->GetDeviceState( sizeof( D_DIMOUSESTATE2 ), (LPVOID)&input ) ;
		if( hr != D_DI_OK )
		{
			if( InputSysData.PF.MouseDeviceObject->Acquire() == D_DI_OK )
			{
				hr = InputSysData.PF.MouseDeviceObject->GetDeviceState( sizeof( D_DIMOUSESTATE2 ) , (LPVOID)&input ) ;
				if( hr != D_DI_OK ) return 0 ;
			}
			else return 0 ;
		}

		res = 0 ;
		if( input.rgbButtons[0] & 0x80 ) res |= MOUSE_INPUT_1 ;
		if( input.rgbButtons[1] & 0x80 ) res |= MOUSE_INPUT_2 ;
		if( input.rgbButtons[2] & 0x80 ) res |= MOUSE_INPUT_3 ;
		if( input.rgbButtons[3] & 0x80 ) res |= MOUSE_INPUT_4 ;
		if( input.rgbButtons[4] & 0x80 ) res |= MOUSE_INPUT_5 ;
		if( input.rgbButtons[5] & 0x80 ) res |= MOUSE_INPUT_6 ;
		if( input.rgbButtons[6] & 0x80 ) res |= MOUSE_INPUT_7 ;
		if( input.rgbButtons[7] & 0x80 ) res |= MOUSE_INPUT_8 ;

		InputSysData.MouseMoveZ += ( int )input.lZ ;
	}

	return res ;
}

// �W���C�p�b�h�̖����]�[���̐ݒ���s���֐��̊��ˑ�����
extern int SetJoypadDeadZone_PF( INPUTPADDATA *pad )
{
	D_DIPROPDWORD dipdw ;

	// �G���[�`�F�b�N
	if( pad->PF.Device == NULL  )
	{
		return 0 ;
	}

	// �w�̖����]�[����ݒ�
	dipdw.diph.dwSize		= sizeof( dipdw ) ;
	dipdw.diph.dwHeaderSize	= sizeof( dipdw.diph ) ;
	dipdw.diph.dwObj		= D_DIJOFS_X ;
	dipdw.diph.dwHow		= D_DIPH_BYOFFSET ;
	dipdw.dwData			= 10000 * pad->DeadZone / 65536 ;
	pad->PF.Device->SetProperty( D_DIPROP_DEADZONE , &dipdw.diph ) ;

	// �x�̖����]�[����ݒ�
	dipdw.diph.dwObj		= D_DIJOFS_Y ;
	pad->PF.Device->SetProperty( D_DIPROP_DEADZONE , &dipdw.diph ) ;

	// �y�̖����]�[����ݒ�
	dipdw.diph.dwObj		= D_DIJOFS_Z ;
	pad->PF.Device->SetProperty( D_DIPROP_DEADZONE , &dipdw.diph ) ;

	// �y��]�̖����]�[����ݒ�
	dipdw.diph.dwObj		= D_DIJOFS_RZ ;
	pad->PF.Device->SetProperty( D_DIPROP_DEADZONE , &dipdw.diph ) ;

	// ����I��
	return 0 ;
}

// �L�[�{�[�h�̋������x����r�����x���ɂ��邩�ǂ����̃t���O���Z�b�g����
extern int NS_SetKeyExclusiveCooperativeLevelFlag( int Flag )
{
	int hr ;

	// ����������Ă��Ȃ�������t���O�����ۑ����ďI��
	if( InputSysData.PF.KeyboardDeviceObject == NULL )
	{
		InputSysData.PF.KeyExclusiveCooperativeLevelFlag = Flag ;
		return 0 ;
	}

	// ��Ԃ������������牽�������I��
	if( InputSysData.PF.KeyExclusiveCooperativeLevelFlag == Flag )
		return 0 ;

	// ��x�A�N�Z�X�����������
	InputSysData.PF.KeyboardDeviceObject->Unacquire() ;

	// �������x�����Z�b�g
	InputSysData.PF.KeyboardDeviceObject->SetCooperativeLevel( NS_GetMainWindowHandle(),
			( DWORD )( Flag == TRUE ? ( D_DISCL_EXCLUSIVE | D_DISCL_FOREGROUND ) : ( D_DISCL_NONEXCLUSIVE | D_DISCL_BACKGROUND ) ) ) ;

	// �A�N�Z�X�����擾����
	hr = InputSysData.PF.KeyboardDeviceObject->Acquire() ;
	if( hr != D_DI_OK )
	{
		hr = InputSysData.PF.KeyboardDeviceObject->Acquire() ;
		if( hr != D_DI_OK )
		{
			DXST_ERRORLOG_ADDUTF16LE( "\xad\x30\xfc\x30\xdc\x30\xfc\x30\xc9\x30\xc7\x30\xd0\x30\xa4\x30\xb9\x30\x6e\x30\xd6\x53\x97\x5f\x6b\x30\x31\x59\x57\x65\x57\x30\x7e\x30\x57\x30\x5f\x30\x00"/*@ L"�L�[�{�[�h�f�o�C�X�̎擾�Ɏ��s���܂���" @*/ ) ;
			return -1 ;
		}
	}

	// �t���O��ۑ�����
	InputSysData.PF.KeyExclusiveCooperativeLevelFlag = Flag ;

	// �I��
	return 0 ;
}

// �L�[�{�[�h�̓��͏����� DirectInput ���g��Ȃ����A�t���O���Z�b�g����
extern int NS_SetKeyboardNotDirectInputFlag( int Flag )
{
	InputSysData.PF.KeyboardNotUseDirectInputFlag = Flag ? TRUE : FALSE ;

	// �I��
	return 0 ;
}

// ���͏����� DirectInput ���g�p���邩�ǂ����̃t���O���Z�b�g����
extern int NS_SetUseDirectInputFlag( int Flag )
{
	InputSysData.PF.NoUseDirectInputFlag = !Flag ;
	InputSysData.PF.UseDirectInputFlag = Flag ;

	// �I��
	return 0 ;
}

// Xbox360�R���g���[���̓��͏����� XInput ���g�p���邩�ǂ�����ݒ肷��( TRUE:XInput ���g�p����( �f�t�H���g )�@�@FALSE:XInput ���g�p���Ȃ� )
extern int NS_SetUseXInputFlag(	int Flag )
{
	InputSysData.PF.NoUseXInputFlag = !Flag ;

	// �I��
	return 0 ;
}

// �W���C�p�b�h�̂f�tI�c�𓾂�
extern int NS_GetJoypadGUID( int PadIndex, GUID *GuidInstanceBuffer, GUID *GuidProductBuffer )
{
	D_DIDEVICEINSTANCEW inst ;

	if( InputSysData.PadNum <= PadIndex )
		return -1 ;

	if( InputSysData.Pad[ PadIndex ].PF.Device == NULL ) return -1 ;

	_MEMSET( &inst, 0, sizeof( inst ) ) ;
	inst.dwSize = sizeof( inst ) ;
	InputSysData.Pad[ PadIndex ].PF.Device->GetDeviceInfo( &inst ) ;
	if( GuidInstanceBuffer != NULL ) *GuidInstanceBuffer = inst.guidInstance ;
	if( GuidProductBuffer  != NULL ) *GuidProductBuffer  = inst.guidProduct ;

	return 0 ;
}

// �W���C�p�b�h�̃f�o�C�X�o�^���Ɛ��i�o�^�����擾����
extern int NS_GetJoypadName( int InputType, TCHAR *InstanceNameBuffer, TCHAR *ProductNameBuffer )
{
#ifdef UNICODE
	return GetJoypadName_WCHAR_T( InputType, InstanceNameBuffer, ProductNameBuffer ) ;
#else
	int Result ;
	wchar_t TempInstanceNameBuffer[ 256 ] ;
	wchar_t TempProductNameBuffer[ 256 ] ;

	Result = GetJoypadName_WCHAR_T( InputType, TempInstanceNameBuffer, TempProductNameBuffer ) ;

	if( InstanceNameBuffer )
	{
		ConvString( ( const char * )TempInstanceNameBuffer, WCHAR_T_CHARCODEFORMAT, InstanceNameBuffer, BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;
	}

	if( ProductNameBuffer )
	{
		ConvString( ( const char * )TempProductNameBuffer, WCHAR_T_CHARCODEFORMAT, ProductNameBuffer, BUFFERBYTES_CANCEL, _TCHARCODEFORMAT ) ;
	}

	return Result ;
#endif
}

// �W���C�p�b�h�̃f�o�C�X�o�^���Ɛ��i�o�^�����擾����
extern int GetJoypadName_WCHAR_T( int InputType, wchar_t *InstanceNameBuffer, wchar_t *ProductNameBuffer )
{
	int JoypadNum = ( InputType & ~DX_INPUT_KEY ) - 1 ;
	INPUTPADDATA *pad = &InputSysData.Pad[ JoypadNum ] ;

	// �\�t�g����A�N�e�B�u�̏ꍇ�̓A�N�e�B�u�ɂȂ�܂ő҂�
	CheckActiveState() ;
	
	// ����������Ă��Ȃ�����������t���ŏ��������s��
	if( InputSysData.InitializeFlag == FALSE )
	{
		return AutoInitialize_PF() ;
	}

	// �G���[�`�F�b�N
	if( JoypadNum < 0 || JoypadNum >= InputSysData.PadNum || ( pad->PF.Device == NULL && pad->PF.XInputDeviceNo < 0 ) )
		return -1 ;

	// ���O���R�s�[
	if( InstanceNameBuffer != NULL ) _WCSCPY( InstanceNameBuffer, pad->PF.InstanceName ) ;
	if( ProductNameBuffer  != NULL ) _WCSCPY( ProductNameBuffer,  pad->PF.ProductName  ) ;

	// �I��
	return 0;
}

// �c�w���C�u�����̃L�[�R�[�h���� Windows �̉��z�L�[�R�[�h���擾����
extern int NS_ConvertKeyCodeToVirtualKey( int KeyCode )
{
	int i ;

	for( i = 0 ; __KeyMap[i][0] != 0xffff ; i ++ )
	{
		if( KeyCode == __KeyMap[i][1] ) break ;
	}

	return __KeyMap[i][0] ;
}

//  Windows �̉��z�L�[�R�[�h( VK_LEFT �Ȃ� ) �ɑΉ�����c�w���C�u�����̃L�[�R�[�h( KEY_INPUT_A �Ȃ� )���擾����( VirtualKey:�ϊ�������Windows�̉��z�L�[�R�[�h�@�߂�l�F�c�w���C�u�����̃L�[�R�[�h )
extern int NS_ConvertVirtualKeyToKeyCode( int VirtualKey )
{
	int i ;

	for( i = 0 ; __KeyMap[i][0] != 0xffff ; i ++ )
	{
		if( VirtualKey == __KeyMap[i][0] ) break ;
	}

	return __KeyMap[i][1] ;
}

// �L�[�{�[�h�̃o�b�t�@����f�[�^���擾���鏈��(�o�b�t�@����ɂ��邾�������ǁE�E�E)
extern int KeyboradBufferProcess( void )
{
	D_DIDEVICEOBJECTDATA_DX7 data7[5] ;
	D_DIDEVICEOBJECTDATA_DX8 data8[5] ;
	DWORD num ;

	if( InputSysData.PF.KeyboardDeviceObject == NULL )
		return 0 ;

	num = 5 ;
	while( num == 5 )
	{
		if( InputSysData.PF.UseDirectInput8Flag == FALSE )
		{
			if( InputSysData.PF.KeyboardDeviceObject->GetDeviceData( sizeof( D_DIDEVICEOBJECTDATA_DX7 ), data7, &num, 0 ) != D_DI_OK ) return 0 ;
		}
		else
		{
			if( InputSysData.PF.KeyboardDeviceObject->GetDeviceData( sizeof( D_DIDEVICEOBJECTDATA_DX8 ), data8, &num, 0 ) != D_DI_OK ) return 0 ;
		}
//		if( num != 0 ) DXST_ERRORLOGFMT_ADDW(( L"�����ꂽ�������ꂽ�������L�[:0x%x  0x%x \n", data[0].dwOfs, data[0].dwData & 0xff )) ;
	}
	
	return 0 ;
}

// �c�����������h���������f�o�C�X�̎擾��Ԃ��X�V����
extern int RefreshInputDeviceAcquireState( void )
{
	int i, num ;
	INPUTPADDATA *Pad ;

	// ����������
	if( InputSysData.PF.DirectInputObject == NULL && NS_GetWindowCloseFlag() == FALSE )
		return 0 ;

	// DirectInput ���g�p���Ă��Ȃ��ꍇ�͉��������I��
	if( InputSysData.PF.DirectInputObject == (D_IDirectInput7 *)(DWORD_PTR)0xffffffff ) return 0 ;

	// �p�b�h�f�o�C�X�̍Ď擾
	num = InputSysData.PadNum ;
	Pad = InputSysData.Pad ;
	for( i = 0 ; i < num ; i ++, Pad ++ )
	{
		if( Pad->PF.Device != NULL )
		{
			Pad->PF.Device->Unacquire() ;

			// �\�t�g�̃A�N�e�B�u��Ԃɂ���ċ������x����ύX
			if( NS_GetActiveFlag() == TRUE )
			{
				Pad->PF.Device->SetCooperativeLevel( NS_GetMainWindowHandle(), D_DISCL_FOREGROUND | D_DISCL_EXCLUSIVE ) ;
			}
			else
			{
				Pad->PF.Device->SetCooperativeLevel( NS_GetMainWindowHandle(), D_DISCL_BACKGROUND | D_DISCL_NONEXCLUSIVE ) ;
			}

			Pad->PF.Device->Acquire() ;
		}
	}

	// �L�[�{�[�h�f�o�C�X�̍Ď擾
	if( InputSysData.PF.KeyboardDeviceObject != NULL )
	{
		InputSysData.PF.KeyboardDeviceObject->Unacquire() ;
		InputSysData.PF.KeyboardDeviceObject->Acquire() ;
	}

	// �I��
	return 0 ;
}

// �}�E�X�̏��擾�ɂc�����������h�����������g�p���Ă��邩�ǂ������擾����( �߂�l  TRUE:DirectInput���g�p���Ă���  FALSE:DirectInput���g�p���Ă��Ȃ� )
extern	int CheckUseDirectInputMouse( void )
{
	if( InputSysData.PF.NoUseDirectInputFlag == TRUE || InputSysData.PF.MouseDeviceObject == NULL || ( InputSysData.PF.UseDirectInputFlag == FALSE && WinData.WindowsVersion >= DX_WINDOWSVERSION_VISTA ) )
	{
		return FALSE ;
	}

	return TRUE ;
}

// �c�����������h�����������g�p�����}�E�X�z�C�[���̈ړ��l���擾����
extern	int GetDirectInputMouseMoveZ( int CounterReset )
{
	int Result ;

	NS_GetMouseInput() ;

	Result = InputSysData.MouseMoveZ / WHEEL_DELTA ;
	if( CounterReset )
	{
		InputSysData.MouseMoveZ -= Result * WHEEL_DELTA ;
	}

	return Result ;
}

// �c�����������h�����������g�p�����}�E�X�z�C�[���̈ړ��l���擾����( float�� )
extern	float GetDirectInputMouseMoveZF( int CounterReset )
{
	float Result ;

	NS_GetMouseInput() ;

	Result = ( float )InputSysData.MouseMoveZ / WHEEL_DELTA ;
	if( CounterReset )
	{
		InputSysData.MouseMoveZ = 0 ;
	}

	return Result ;
}




#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_INPUT


