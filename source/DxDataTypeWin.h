// -------------------------------------------------------------------------------
// 
// 		�c�w���C�u����		Windows�p�f�[�^�^�C�v��`�w�b�_�t�@�C��
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXDATATYPEWIN_H__
#define __DXDATATYPEWIN_H__

// �C���N���[�h ------------------------------------------------------------------
#include "DxCompileConfig.h"
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

// ���C�u���������N��`--------------------------------------------------------

#ifndef __DX_MAKE
	#ifndef DX_LIB_NOT_DEFAULTPATH
		#ifndef DX_GCC_COMPILE
			#ifndef DX_SRC_COMPILE
				#ifdef _MSC_VER
					#if _MSC_VER <  1700
						#ifdef _WIN64
							#ifdef _DEBUG
								#pragma comment( lib, "DxDrawFunc_x64_d.lib"		)		//  �`�敔���̔����o��
								#ifdef UNICODE
									#pragma comment( lib, "DxLibW_x64_d.lib"		)		//  �c�w���C�u�����g�p�w��
									#pragma comment( lib, "DxUseCLibW_x64_d.lib"	)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
								#else
									#pragma comment( lib, "DxLib_x64_d.lib"			)		//  �c�w���C�u�����g�p�w��
									#pragma comment( lib, "DxUseCLib_x64_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
								#endif
							#else // _DEBUG
								#pragma comment( lib, "DxDrawFunc_x64.lib"			)		//  �`�敔���̔����o��
								#ifdef UNICODE
									#pragma comment( lib, "DxLibW_x64.lib"			)		//  �c�w���C�u�����g�p�w��
									#pragma comment( lib, "DxUseCLibW_x64.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
								#else
									#pragma comment( lib, "DxLib_x64.lib"			)		//  �c�w���C�u�����g�p�w��
									#pragma comment( lib, "DxUseCLib_x64.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
								#endif
							#endif // _DEBUG
						#else // _WIN64
							#ifdef _DEBUG
								#pragma comment( lib, "DxDrawFunc_d.lib"		)		//  �`�敔���̔����o��
								#ifdef UNICODE
									#pragma comment( lib, "DxLibW_d.lib"		)		//  �c�w���C�u�����g�p�w��
									#pragma comment( lib, "DxUseCLibW_d.lib"	)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
								#else
									#pragma comment( lib, "DxLib_d.lib"			)		//  �c�w���C�u�����g�p�w��
									#pragma comment( lib, "DxUseCLib_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
								#endif
							#else // _DEBUG
								#pragma comment( lib, "DxDrawFunc.lib"			)		//  �`�敔���̔����o��
								#ifdef UNICODE
									#pragma comment( lib, "DxLibW.lib"			)		//  �c�w���C�u�����g�p�w��
									#pragma comment( lib, "DxUseCLibW.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
								#else
									#pragma comment( lib, "DxLib.lib"			)		//  �c�w���C�u�����g�p�w��
									#pragma comment( lib, "DxUseCLib.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
								#endif
							#endif // _DEBUG
						#endif // _WIN64
					#else	// _MSC_VER <  1700
						#if _MSC_VER >= 1900
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "DxDrawFunc_vs2015_x64_d.lib"			)		//  �`�敔���̔����o��
									#ifdef UNICODE
										#pragma comment( lib, "DxLibW_vs2015_x64_d.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLibW_vs2015_x64_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#else
										#pragma comment( lib, "DxLib_vs2015_x64_d.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLib_vs2015_x64_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#endif
								#else // _DEBUG
									#pragma comment( lib, "DxDrawFunc_vs2015_x64.lib"			)		//  �`�敔���̔����o��
									#ifdef UNICODE
										#pragma comment( lib, "DxLibW_vs2015_x64.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLibW_vs2015_x64.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#else
										#pragma comment( lib, "DxLib_vs2015_x64.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLib_vs2015_x64.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#endif
								#endif // _DEBUG
							#else // _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "DxDrawFunc_vs2015_x86_d.lib"			)		//  �`�敔���̔����o��
									#ifdef UNICODE
										#pragma comment( lib, "DxLibW_vs2015_x86_d.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLibW_vs2015_x86_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#else
										#pragma comment( lib, "DxLib_vs2015_x86_d.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLib_vs2015_x86_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#endif
								#else // _DEBUG
									#pragma comment( lib, "DxDrawFunc_vs2015_x86.lib"			)		//  �`�敔���̔����o��
									#ifdef UNICODE
										#pragma comment( lib, "DxLibW_vs2015_x86.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLibW_vs2015_x86.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#else
										#pragma comment( lib, "DxLib_vs2015_x86.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLib_vs2015_x86.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#endif
								#endif // _DEBUG
							#endif // _WIN64
						#elif _MSC_VER >= 1800
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "DxDrawFunc_vs2012_x64_d.lib"			)		//  �`�敔���̔����o��
									#ifdef UNICODE
										#pragma comment( lib, "DxLibW_vs2012_x64_d.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLibW_vs2013_x64_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#else
										#pragma comment( lib, "DxLib_vs2012_x64_d.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLib_vs2013_x64_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#endif
								#else // _DEBUG
									#pragma comment( lib, "DxDrawFunc_vs2012_x64.lib"			)		//  �`�敔���̔����o��
									#ifdef UNICODE
										#pragma comment( lib, "DxLibW_vs2012_x64.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLibW_vs2013_x64.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#else
										#pragma comment( lib, "DxLib_vs2012_x64.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLib_vs2013_x64.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#endif
								#endif // _DEBUG
							#else // _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "DxDrawFunc_vs2012_x86_d.lib"			)		//  �`�敔���̔����o��
									#ifdef UNICODE
										#pragma comment( lib, "DxLibW_vs2012_x86_d.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLibW_vs2013_x86_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#else
										#pragma comment( lib, "DxLib_vs2012_x86_d.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLib_vs2013_x86_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#endif
								#else // _DEBUG
									#pragma comment( lib, "DxDrawFunc_vs2012_x86.lib"			)		//  �`�敔���̔����o��
									#ifdef UNICODE
										#pragma comment( lib, "DxLibW_vs2012_x86.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLibW_vs2013_x86.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#else
										#pragma comment( lib, "DxLib_vs2012_x86.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLib_vs2013_x86.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#endif
								#endif // _DEBUG
							#endif // _WIN64
						#elif _MSC_VER >= 1700
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "DxDrawFunc_vs2012_x64_d.lib"			)		//  �`�敔���̔����o��
									#ifdef UNICODE
										#pragma comment( lib, "DxLibW_vs2012_x64_d.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLibW_vs2012_x64_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#else
										#pragma comment( lib, "DxLib_vs2012_x64_d.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLib_vs2012_x64_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#endif
								#else // _DEBUG
									#pragma comment( lib, "DxDrawFunc_vs2012_x64.lib"			)		//  �`�敔���̔����o��
									#ifdef UNICODE
										#pragma comment( lib, "DxLibW_vs2012_x64.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLibW_vs2012_x64.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#else
										#pragma comment( lib, "DxLib_vs2012_x64.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLib_vs2012_x64.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#endif
								#endif // _DEBUG
							#else // _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "DxDrawFunc_vs2012_x86_d.lib"			)		//  �`�敔���̔����o��
									#ifdef UNICODE
										#pragma comment( lib, "DxLibW_vs2012_x86_d.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLibW_vs2012_x86_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#else
										#pragma comment( lib, "DxLib_vs2012_x86_d.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLib_vs2012_x86_d.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#endif
								#else // _DEBUG
									#pragma comment( lib, "DxDrawFunc_vs2012_x86.lib"			)		//  �`�敔���̔����o��
									#ifdef UNICODE
										#pragma comment( lib, "DxLibW_vs2012_x86.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLibW_vs2012_x86.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#else
										#pragma comment( lib, "DxLib_vs2012_x86.lib"			)		//  �c�w���C�u�����g�p�w��
										#pragma comment( lib, "DxUseCLib_vs2012_x86.lib"		)		//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
									#endif
								#endif // _DEBUG
							#endif // _WIN64
						#endif // // _MSC_VER >= 1700
					#endif // // _MSC_VER <  1700
				#else // _MSC_VER
					#pragma comment( lib, "DxDrawFunc.lib"		)			//  �`�敔���̔����o��
					#ifdef UNICODE
						#pragma comment( lib, "DxLibW.lib"		)			//  �c�w���C�u�����g�p�w��
						#pragma comment( lib, "DxUseCLibW.lib"	)			//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
					#else
						#pragma comment( lib, "DxLib.lib"		)			//  �c�w���C�u�����g�p�w��
						#pragma comment( lib, "DxUseCLib.lib"	)			//  �W���b���C�u�������g�p���镔���� lib �t�@�C���̎g�p�w��
					#endif
				#endif // _MSC_VER

				#ifdef _DEBUG
					#pragma comment( linker, "/NODEFAULTLIB:libcmt.lib" )
					#pragma comment( linker, "/NODEFAULTLIB:libc.lib" )
					#pragma comment( linker, "/NODEFAULTLIB:libcd.lib" )
		//			#pragma comment( linker, "/NODEFAULTLIB:msvcrt.lib" )
		//			#pragma comment( linker, "/NODEFAULTLIB:msvcrtd.lib" )
				#else
					#pragma comment( linker, "/NODEFAULTLIB:libcmtd.lib" )
					#pragma comment( linker, "/NODEFAULTLIB:libc.lib" )
					#pragma comment( linker, "/NODEFAULTLIB:libcd.lib" )
		//			#pragma comment( linker, "/NODEFAULTLIB:msvcrt.lib" )
		//			#pragma comment( linker, "/NODEFAULTLIB:msvcrtd.lib" )
				#endif
			#endif
			//#pragma comment( lib, "libcmt.lib"		)				//  C�W���}���`�X���b�h�Ή����C�u����
			#pragma comment( lib, "kernel32.lib"		)			//  Win32�J�[�l�����C�u����
			//#pragma comment( lib, "comctl32.lib"		)			//�@Win32API���C�u����
			#pragma comment( lib, "user32.lib"		)				//  Win32API���C�u����
			#pragma comment( lib, "gdi32.lib"		)				//  Win32API���C�u����
			#pragma comment( lib, "advapi32.lib"		)			//  Win32API���C�u����
			//#pragma comment( lib, "ole32.lib"		)				//  Win32API���C�u����
			#pragma comment( lib, "shell32.lib"		)				//  �}���`���f�B�A���C�u����
			//#pragma comment( lib, "winmm.lib"		)				//  �}���`���f�B�A���C�u����
			#ifndef DX_NON_MOVIE
				//#pragma comment( lib, "Strmiids.lib" )			//�@DirectShow���C�u����
			#endif
			#ifndef DX_NON_NETWORK
				//#pragma comment( lib, "wsock32.lib" )				//  WinSockets���C�u����
			#endif
			#ifndef DX_NON_KEYEX
				//#pragma comment( lib, "imm32.lib" )					// �h�l�d����p���C�u����
			#endif
			#ifndef DX_NON_ACM
				//#pragma comment( lib, "msacm32.lib" )				// �`�b�l����p���C�u���� 
			#endif
			#ifndef DX_NON_BULLET_PHYSICS
				#ifdef _MSC_VER
					#if _MSC_VER <  1700
						#ifdef DX_USE_VC8_BULLET_PHYSICS_LIB
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libbulletcollision_vc8_x64_d.lib" )	// Visual C++ 8.0 �ŃR���p�C������ Bullet Physics ���C�u���� 
									#pragma comment( lib, "libbulletdynamics_vc8_x64_d.lib" )
									#pragma comment( lib, "libbulletmath_vc8_x64_d.lib" )
								#else
									#pragma comment( lib, "libbulletcollision_vc8_x64.lib" )	// Visual C++ 8.0 �ŃR���p�C������ Bullet Physics ���C�u���� 
									#pragma comment( lib, "libbulletdynamics_vc8_x64.lib" )
									#pragma comment( lib, "libbulletmath_vc8_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libbulletcollision_vc8_d.lib" )	// Visual C++ 8.0 �ŃR���p�C������ Bullet Physics ���C�u���� 
									#pragma comment( lib, "libbulletdynamics_vc8_d.lib" )
									#pragma comment( lib, "libbulletmath_vc8_d.lib" )
								#else
									#pragma comment( lib, "libbulletcollision_vc8.lib" )	// Visual C++ 8.0 �ŃR���p�C������ Bullet Physics ���C�u���� 
									#pragma comment( lib, "libbulletdynamics_vc8.lib" )
									#pragma comment( lib, "libbulletmath_vc8.lib" )
								#endif
							#endif
						#else // DX_USE_VC8_BULLET_PHYSICS_LIB
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libbulletcollision_vc8_x64_d.lib" )	// Visual C++ 8.0 �ŃR���p�C������ Bullet Physics ���C�u���� 
									#pragma comment( lib, "libbulletdynamics_vc8_x64_d.lib" )
									#pragma comment( lib, "libbulletmath_vc8_x64_d.lib" )
								#else
									#pragma comment( lib, "libbulletcollision_vc8_x64.lib" )	// Visual C++ 8.0 �ŃR���p�C������ Bullet Physics ���C�u���� 
									#pragma comment( lib, "libbulletdynamics_vc8_x64.lib" )
									#pragma comment( lib, "libbulletmath_vc8_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libbulletcollision_vc6_d.lib" )	// Visual C++ 6.0 �ŃR���p�C������ Bullet Physics ���C�u���� 
									#pragma comment( lib, "libbulletdynamics_vc6_d.lib" )
									#pragma comment( lib, "libbulletmath_vc6_d.lib" )
								#else
									#pragma comment( lib, "libbulletcollision_vc6.lib" )	// Visual C++ 6.0 �ŃR���p�C������ Bullet Physics ���C�u���� 
									#pragma comment( lib, "libbulletdynamics_vc6.lib" )
									#pragma comment( lib, "libbulletmath_vc6.lib" )
								#endif
							#endif
						#endif // DX_USE_VC8_BULLET_PHYSICS_LIB
					#else // _MSC_VER <  1700
						#if _MSC_VER >= 1900
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libbulletcollision_vs2015_x64_d.lib" )
									#pragma comment( lib, "libbulletdynamics_vs2015_x64_d.lib" )
									#pragma comment( lib, "libbulletmath_vs2015_x64_d.lib" )
								#else
									#pragma comment( lib, "libbulletcollision_vs2015_x64.lib" )
									#pragma comment( lib, "libbulletdynamics_vs2015_x64.lib" )
									#pragma comment( lib, "libbulletmath_vs2015_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libbulletcollision_vs2015_x86_d.lib" )
									#pragma comment( lib, "libbulletdynamics_vs2015_x86_d.lib" )
									#pragma comment( lib, "libbulletmath_vs2015_x86_d.lib" )
								#else
									#pragma comment( lib, "libbulletcollision_vs2015_x86.lib" )
									#pragma comment( lib, "libbulletdynamics_vs2015_x86.lib" )
									#pragma comment( lib, "libbulletmath_vs2015_x86.lib" )
								#endif
							#endif
						#elif _MSC_VER >= 1800
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libbulletcollision_vs2013_x64_d.lib" )
									#pragma comment( lib, "libbulletdynamics_vs2013_x64_d.lib" )
									#pragma comment( lib, "libbulletmath_vs2013_x64_d.lib" )
								#else
									#pragma comment( lib, "libbulletcollision_vs2013_x64.lib" )
									#pragma comment( lib, "libbulletdynamics_vs2013_x64.lib" )
									#pragma comment( lib, "libbulletmath_vs2013_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libbulletcollision_vs2013_x86_d.lib" )
									#pragma comment( lib, "libbulletdynamics_vs2013_x86_d.lib" )
									#pragma comment( lib, "libbulletmath_vs2013_x86_d.lib" )
								#else
									#pragma comment( lib, "libbulletcollision_vs2013_x86.lib" )
									#pragma comment( lib, "libbulletdynamics_vs2013_x86.lib" )
									#pragma comment( lib, "libbulletmath_vs2013_x86.lib" )
								#endif
							#endif
						#elif _MSC_VER >= 1700
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libbulletcollision_vs2012_x64_d.lib" )
									#pragma comment( lib, "libbulletdynamics_vs2012_x64_d.lib" )
									#pragma comment( lib, "libbulletmath_vs2012_x64_d.lib" )
								#else
									#pragma comment( lib, "libbulletcollision_vs2012_x64.lib" )
									#pragma comment( lib, "libbulletdynamics_vs2012_x64.lib" )
									#pragma comment( lib, "libbulletmath_vs2012_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libbulletcollision_vs2012_x86_d.lib" )
									#pragma comment( lib, "libbulletdynamics_vs2012_x86_d.lib" )
									#pragma comment( lib, "libbulletmath_vs2012_x86_d.lib" )
								#else
									#pragma comment( lib, "libbulletcollision_vs2012_x86.lib" )
									#pragma comment( lib, "libbulletdynamics_vs2012_x86.lib" )
									#pragma comment( lib, "libbulletmath_vs2012_x86.lib" )
								#endif
							#endif
						#endif // _MSC_VER >= 1700
					#endif // _MSC_VER < 1700
				#else // _MSC_VER
					#pragma comment( lib, "libbulletcollision.lib" )	// Bullet Physics ���C�u���� 
					#pragma comment( lib, "libbulletdynamics.lib" )
					#pragma comment( lib, "libbulletmath.lib" )
				#endif // _MSC_VER
			#endif
			#ifndef DX_NON_TIFFREAD
				#ifdef _MSC_VER
					#if _MSC_VER <  1700
						#ifdef _WIN64
							#ifdef _DEBUG
								#pragma comment( lib, "libtiff_x64_d.lib" )		// �s�h�e�e���C�u����
							#else
								#pragma comment( lib, "libtiff_x64.lib" )		// �s�h�e�e���C�u����
							#endif
						#else
							#ifdef _DEBUG
								#pragma comment( lib, "libtiff_d.lib" )			// �s�h�e�e���C�u����
							#else
								#pragma comment( lib, "libtiff.lib" )			// �s�h�e�e���C�u����
							#endif
						#endif
					#else // _MSC_VER <  1700
						#if _MSC_VER >= 1900
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libtiff_vs2015_x64_d.lib" )	// �s�h�e�e���C�u����
								#else
									#pragma comment( lib, "libtiff_vs2015_x64.lib" )	// �s�h�e�e���C�u����
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libtiff_vs2015_x86_d.lib" )		// �s�h�e�e���C�u����
								#else
									#pragma comment( lib, "libtiff_vs2015_x86.lib" )		// �s�h�e�e���C�u����
								#endif
							#endif
						#elif _MSC_VER >= 1800
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libtiff_vs2013_x64_d.lib" )		// �s�h�e�e���C�u����
								#else
									#pragma comment( lib, "libtiff_vs2013_x64.lib" )		// �s�h�e�e���C�u����
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libtiff_vs2013_x86_d.lib" )		// �s�h�e�e���C�u����
								#else
									#pragma comment( lib, "libtiff_vs2013_x86.lib" )		// �s�h�e�e���C�u����
								#endif
							#endif
						#elif _MSC_VER >= 1700
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libtiff_vs2012_x64_d.lib" )	// �s�h�e�e���C�u����
								#else
									#pragma comment( lib, "libtiff_vs2012_x64.lib" )	// �s�h�e�e���C�u����
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libtiff_vs2012_x86_d.lib" )	// �s�h�e�e���C�u����
								#else
									#pragma comment( lib, "libtiff_vs2012_x86.lib" )	// �s�h�e�e���C�u����
								#endif
							#endif
						#endif // _MSC_VER >= 1700
					#endif // _MSC_VER <  1700
				#else // _MSC_VER
					#pragma comment( lib, "libtiff.lib" )			// �s�h�e�e���C�u����
				#endif // _MSC_VER
			#endif
			#ifndef DX_NON_PNGREAD
				#ifdef _MSC_VER
					#if _MSC_VER <  1700
						#ifdef _WIN64
							#ifdef _DEBUG
								#pragma comment( lib, "libpng_x64_d.lib" )		// �o�m�f���C�u����
								#pragma comment( lib, "zlib_x64_d.lib" )
							#else
								#pragma comment( lib, "libpng_x64.lib" )		// �o�m�f���C�u����
								#pragma comment( lib, "zlib_x64.lib" )
							#endif
						#else
							#ifdef _DEBUG
								#pragma comment( lib, "libpng_d.lib" )			// �o�m�f���C�u����
								#pragma comment( lib, "zlib_d.lib" )
							#else
								#pragma comment( lib, "libpng.lib" )			// �o�m�f���C�u����
								#pragma comment( lib, "zlib.lib" )
							#endif
						#endif
					#else // MSC_VER <  1700
						#if _MSC_VER >= 1900
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libpng_vs2015_x64_d.lib" )	// �o�m�f���C�u����
									#pragma comment( lib, "zlib_vs2015_x64_d.lib" )
								#else
									#pragma comment( lib, "libpng_vs2015_x64.lib" )		// �o�m�f���C�u����
									#pragma comment( lib, "zlib_vs2015_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libpng_vs2015_x86_d.lib" )	// �o�m�f���C�u����
									#pragma comment( lib, "zlib_vs2015_x86_d.lib" )
								#else
									#pragma comment( lib, "libpng_vs2015_x86.lib" )		// �o�m�f���C�u����
									#pragma comment( lib, "zlib_vs2015_x86.lib" )
								#endif
							#endif
						#elif _MSC_VER >= 1800
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libpng_vs2013_x64_d.lib" )	// �o�m�f���C�u����
									#pragma comment( lib, "zlib_vs2013_x64_d.lib" )
								#else
									#pragma comment( lib, "libpng_vs2013_x64.lib" )		// �o�m�f���C�u����
									#pragma comment( lib, "zlib_vs2013_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libpng_vs2013_x86_d.lib" )	// �o�m�f���C�u����
									#pragma comment( lib, "zlib_vs2013_x86_d.lib" )
								#else
									#pragma comment( lib, "libpng_vs2013_x86.lib" )		// �o�m�f���C�u����
									#pragma comment( lib, "zlib_vs2013_x86.lib" )
								#endif
							#endif
						#elif _MSC_VER >= 1700
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libpng_vs2012_x64_d.lib" )	// �o�m�f���C�u����
									#pragma comment( lib, "zlib_vs2012_x64_d.lib" )
								#else
									#pragma comment( lib, "libpng_vs2012_x64.lib" )		// �o�m�f���C�u����
									#pragma comment( lib, "zlib_vs2012_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libpng_vs2012_x86_d.lib" )	// �o�m�f���C�u����
									#pragma comment( lib, "zlib_vs2012_x86_d.lib" )
								#else
									#pragma comment( lib, "libpng_vs2012_x86.lib" )		// �o�m�f���C�u����
									#pragma comment( lib, "zlib_vs2012_x86.lib" )
								#endif
							#endif
						#endif // _MSC_VER >= 1700
					#endif // _MSC_VER < 1700
				#else // _MSC_VER
					#pragma comment( lib, "libpng.lib" )			// �o�m�f���C�u����
					#pragma comment( lib, "zlib.lib" )
				#endif // _MSC_VER
			#endif
			#ifndef DX_NON_JPEGREAD
				#ifdef _MSC_VER
					#if _MSC_VER <  1700
						#ifdef _WIN64
							#ifdef _DEBUG
								#pragma comment( lib, "libjpeg_x64_d.lib" )		// �i�o�d�f���C�u����
							#else
								#pragma comment( lib, "libjpeg_x64.lib" )		// �i�o�d�f���C�u����
							#endif
						#else
							#ifdef _DEBUG
								#pragma comment( lib, "libjpeg_d.lib" )			// �i�o�d�f���C�u����
							#else
								#pragma comment( lib, "libjpeg.lib" )			// �i�o�d�f���C�u����
							#endif
						#endif
					#else // _MSC_VER <  1700
						#if _MSC_VER >= 1900
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libjpeg_vs2015_x64_d.lib" )		// �i�o�d�f���C�u����
								#else
									#pragma comment( lib, "libjpeg_vs2015_x64.lib" )		// �i�o�d�f���C�u����
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libjpeg_vs2015_x86_d.lib" )		// �i�o�d�f���C�u����
								#else
									#pragma comment( lib, "libjpeg_vs2015_x86.lib" )		// �i�o�d�f���C�u����
								#endif
							#endif
						#elif _MSC_VER >= 1800
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libjpeg_vs2013_x64_d.lib" )		// �i�o�d�f���C�u����
								#else
									#pragma comment( lib, "libjpeg_vs2013_x64.lib" )		// �i�o�d�f���C�u����
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libjpeg_vs2013_x86_d.lib" )		// �i�o�d�f���C�u����
								#else
									#pragma comment( lib, "libjpeg_vs2013_x86.lib" )		// �i�o�d�f���C�u����
								#endif
							#endif
						#elif _MSC_VER >= 1700
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "libjpeg_vs2012_x64_d.lib" )		// �i�o�d�f���C�u����
								#else
									#pragma comment( lib, "libjpeg_vs2012_x64.lib" )		// �i�o�d�f���C�u����
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "libjpeg_vs2012_x86_d.lib" )		// �i�o�d�f���C�u����
								#else
									#pragma comment( lib, "libjpeg_vs2012_x86.lib" )		// �i�o�d�f���C�u����
								#endif
							#endif
						#endif // _MSC_VER >= 1700
					#endif // _MSC_VER <  1700
				#else // _MSC_VER
					#pragma comment( lib, "libjpeg.lib" )			// �i�o�d�f���C�u����
				#endif // _MSC_VER
			#endif
			#ifndef DX_NON_OGGVORBIS								// �n�����u�������������C�u����
				#ifdef _MSC_VER
					#if _MSC_VER <  1700
						#ifdef _WIN64
							#ifdef _DEBUG
								#pragma comment( lib, "ogg_static_x64_d.lib" )
								#pragma comment( lib, "vorbis_static_x64_d.lib" )
								#pragma comment( lib, "vorbisfile_static_x64_d.lib" )
							#else
								#pragma comment( lib, "ogg_static_x64.lib" )
								#pragma comment( lib, "vorbis_static_x64.lib" )
								#pragma comment( lib, "vorbisfile_static_x64.lib" )
							#endif
						#else
							#ifdef _DEBUG
								#pragma comment( lib, "ogg_static_d.lib" )
								#pragma comment( lib, "vorbis_static_d.lib" )
								#pragma comment( lib, "vorbisfile_static_d.lib" )
							#else
								#pragma comment( lib, "ogg_static.lib" )
								#pragma comment( lib, "vorbis_static.lib" )
								#pragma comment( lib, "vorbisfile_static.lib" )
							#endif
						#endif
					#else // _MSC_VER <  1700
						#if _MSC_VER >= 1900
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "ogg_static_vs2015_x64_d.lib" )
									#pragma comment( lib, "vorbis_static_vs2015_x64_d.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2015_x64_d.lib" )
								#else
									#pragma comment( lib, "ogg_static_vs2015_x64.lib" )
									#pragma comment( lib, "vorbis_static_vs2015_x64.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2015_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "ogg_static_vs2015_x86_d.lib" )
									#pragma comment( lib, "vorbis_static_vs2015_x86_d.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2015_x86_d.lib" )
								#else
									#pragma comment( lib, "ogg_static_vs2015_x86.lib" )
									#pragma comment( lib, "vorbis_static_vs2015_x86.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2015_x86.lib" )
								#endif
							#endif
						#elif _MSC_VER >= 1800
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "ogg_static_vs2013_x64_d.lib" )
									#pragma comment( lib, "vorbis_static_vs2013_x64_d.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2013_x64_d.lib" )
								#else
									#pragma comment( lib, "ogg_static_vs2013_x64.lib" )
									#pragma comment( lib, "vorbis_static_vs2013_x64.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2013_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "ogg_static_vs2013_x86_d.lib" )
									#pragma comment( lib, "vorbis_static_vs2013_x86_d.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2013_x86_d.lib" )
								#else
									#pragma comment( lib, "ogg_static_vs2013_x86.lib" )
									#pragma comment( lib, "vorbis_static_vs2013_x86.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2013_x86.lib" )
								#endif
							#endif
						#elif _MSC_VER >= 1700
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "ogg_static_vs2012_x64_d.lib" )
									#pragma comment( lib, "vorbis_static_vs2012_x64_d.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2012_x64_d.lib" )
								#else
									#pragma comment( lib, "ogg_static_vs2012_x64.lib" )
									#pragma comment( lib, "vorbis_static_vs2012_x64.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2012_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "ogg_static_vs2012_x86_d.lib" )
									#pragma comment( lib, "vorbis_static_vs2012_x86_d.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2012_x86_d.lib" )
								#else
									#pragma comment( lib, "ogg_static_vs2012_x86.lib" )
									#pragma comment( lib, "vorbis_static_vs2012_x86.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2012_x86.lib" )
								#endif
							#endif
						#endif // _MSC_VER >= 1700
					#endif // _MSC_VER < 1700
				#else // _MSC_VER
					#pragma comment( lib, "ogg_static.lib" )
					#pragma comment( lib, "vorbis_static.lib" )
					#pragma comment( lib, "vorbisfile_static.lib" )
				#endif // _MSC_VER
			#endif
			#ifndef DX_NON_OGGTHEORA								// �n�����s�������������C�u����
				#ifdef _MSC_VER
					#if _MSC_VER <  1700
						#ifdef _WIN64
							#ifdef _DEBUG
								#pragma comment( lib, "ogg_static_x64_d.lib" )
								#pragma comment( lib, "vorbis_static_x64_d.lib" )
								#pragma comment( lib, "vorbisfile_static_x64_d.lib" )

								#pragma comment( lib, "libtheora_static_x64_d.lib" )
							#else
								#pragma comment( lib, "ogg_static_x64.lib" )
								#pragma comment( lib, "vorbis_static_x64.lib" )
								#pragma comment( lib, "vorbisfile_static_x64.lib" )

								#pragma comment( lib, "libtheora_static_x64.lib" )
							#endif
						#else
							#ifdef _DEBUG
								#pragma comment( lib, "ogg_static_d.lib" )
								#pragma comment( lib, "vorbis_static_d.lib" )
								#pragma comment( lib, "vorbisfile_static_d.lib" )

								#pragma comment( lib, "libtheora_static_d.lib" )
							#else
								#pragma comment( lib, "ogg_static.lib" )
								#pragma comment( lib, "vorbis_static.lib" )
								#pragma comment( lib, "vorbisfile_static.lib" )

								#pragma comment( lib, "libtheora_static.lib" )
							#endif
						#endif
					#else // _MSC_VER <  1700
						#if _MSC_VER >= 1900
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "ogg_static_vs2015_x64_d.lib" )
									#pragma comment( lib, "vorbis_static_vs2015_x64_d.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2015_x64_d.lib" )

									#pragma comment( lib, "libtheora_static_vs2015_x64_d.lib" )
								#else
									#pragma comment( lib, "ogg_static_vs2015_x64.lib" )
									#pragma comment( lib, "vorbis_static_vs2015_x64.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2015_x64.lib" )

									#pragma comment( lib, "libtheora_static_vs2015_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "ogg_static_vs2015_x86_d.lib" )
									#pragma comment( lib, "vorbis_static_vs2015_x86_d.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2015_x86_d.lib" )

									#pragma comment( lib, "libtheora_static_vs2015_x86_d.lib" )
								#else
									#pragma comment( lib, "ogg_static_vs2015_x86.lib" )
									#pragma comment( lib, "vorbis_static_vs2015_x86.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2015_x86.lib" )

									#pragma comment( lib, "libtheora_static_vs2015_x86.lib" )
								#endif
							#endif
						#elif _MSC_VER >= 1800
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "ogg_static_vs2013_x64_d.lib" )
									#pragma comment( lib, "vorbis_static_vs2013_x64_d.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2013_x64_d.lib" )

									#pragma comment( lib, "libtheora_static_vs2013_x64_d.lib" )
								#else
									#pragma comment( lib, "ogg_static_vs2013_x64.lib" )
									#pragma comment( lib, "vorbis_static_vs2013_x64.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2013_x64.lib" )

									#pragma comment( lib, "libtheora_static_vs2013_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "ogg_static_vs2013_x86_d.lib" )
									#pragma comment( lib, "vorbis_static_vs2013_x86_d.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2013_x86_d.lib" )

									#pragma comment( lib, "libtheora_static_vs2013_x86_d.lib" )
								#else
									#pragma comment( lib, "ogg_static_vs2013_x86.lib" )
									#pragma comment( lib, "vorbis_static_vs2013_x86.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2013_x86.lib" )

									#pragma comment( lib, "libtheora_static_vs2013_x86.lib" )
								#endif
							#endif
						#elif _MSC_VER >= 1700
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "ogg_static_vs2012_x64_d.lib" )
									#pragma comment( lib, "vorbis_static_vs2012_x64_d.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2012_x64_d.lib" )

									#pragma comment( lib, "libtheora_static_vs2012_x64_d.lib" )
								#else
									#pragma comment( lib, "ogg_static_vs2012_x64.lib" )
									#pragma comment( lib, "vorbis_static_vs2012_x64.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2012_x64.lib" )

									#pragma comment( lib, "libtheora_static_vs2012_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "ogg_static_vs2012_x86_d.lib" )
									#pragma comment( lib, "vorbis_static_vs2012_x86_d.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2012_x86_d.lib" )

									#pragma comment( lib, "libtheora_static_vs2012_x86_d.lib" )
								#else
									#pragma comment( lib, "ogg_static_vs2012_x86.lib" )
									#pragma comment( lib, "vorbis_static_vs2012_x86.lib" )
									#pragma comment( lib, "vorbisfile_static_vs2012_x86.lib" )

									#pragma comment( lib, "libtheora_static_vs2012_x86.lib" )
								#endif
							#endif
						#endif // _MSC_VER >= 1700
					#endif // _MSC_VER < 1700
				#else // _MSC_VER
					#pragma comment( lib, "ogg_static.lib" )
					#pragma comment( lib, "vorbis_static.lib" )
					#pragma comment( lib, "vorbisfile_static.lib" )

					#pragma comment( lib, "libtheora_static.lib" )
				#endif // _MSC_VER
			#endif
			#ifndef DX_NON_OPUS								// Opus���C�u����
				#ifdef _MSC_VER
					#if _MSC_VER <  1700
						#ifdef _WIN64
							#ifdef _DEBUG
								#pragma comment( lib, "opus_x64_d.lib" )
								#pragma comment( lib, "opusfile_x64_d.lib" )
								#pragma comment( lib, "silk_common_x64_d.lib" )
								#pragma comment( lib, "celt_x64_d.lib" )
							#else
								#pragma comment( lib, "opus_x64.lib" )
								#pragma comment( lib, "opusfile_x64.lib" )
								#pragma comment( lib, "silk_common_x64.lib" )
								#pragma comment( lib, "celt_x64.lib" )
							#endif
						#else
							#ifdef _DEBUG
								#pragma comment( lib, "opus_d.lib" )
								#pragma comment( lib, "opusfile_d.lib" )
								#pragma comment( lib, "silk_common_d.lib" )
								#pragma comment( lib, "celt_d.lib" )
							#else
								#pragma comment( lib, "opus.lib" )
								#pragma comment( lib, "opusfile.lib" )
								#pragma comment( lib, "silk_common.lib" )
								#pragma comment( lib, "celt.lib" )
							#endif
						#endif
					#else // _MSC_VER <  1700
						#if _MSC_VER >= 1900
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "opus_vs2015_x64_d.lib" )
									#pragma comment( lib, "opusfile_vs2015_x64_d.lib" )
									#pragma comment( lib, "silk_common_vs2015_x64_d.lib" )
									#pragma comment( lib, "celt_vs2015_x64_d.lib" )
								#else
									#pragma comment( lib, "opus_vs2015_x64.lib" )
									#pragma comment( lib, "opusfile_vs2015_x64.lib" )
									#pragma comment( lib, "silk_common_vs2015_x64.lib" )
									#pragma comment( lib, "celt_vs2015_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "opus_vs2015_x86_d.lib" )
									#pragma comment( lib, "opusfile_vs2015_x86_d.lib" )
									#pragma comment( lib, "silk_common_vs2015_x86_d.lib" )
									#pragma comment( lib, "celt_vs2015_x86_d.lib" )
								#else
									#pragma comment( lib, "opus_vs2015_x86.lib" )
									#pragma comment( lib, "opusfile_vs2015_x86.lib" )
									#pragma comment( lib, "silk_common_vs2015_x86.lib" )
									#pragma comment( lib, "celt_vs2015_x86.lib" )
								#endif
							#endif
						#elif _MSC_VER >= 1800
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "opus_vs2013_x64_d.lib" )
									#pragma comment( lib, "opusfile_vs2013_x64_d.lib" )
									#pragma comment( lib, "silk_common_vs2013_x64_d.lib" )
									#pragma comment( lib, "celt_vs2013_x64_d.lib" )
								#else
									#pragma comment( lib, "opus_vs2013_x64.lib" )
									#pragma comment( lib, "opusfile_vs2013_x64.lib" )
									#pragma comment( lib, "silk_common_vs2013_x64.lib" )
									#pragma comment( lib, "celt_vs2013_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "opus_vs2013_x86_d.lib" )
									#pragma comment( lib, "opusfile_vs2013_x86_d.lib" )
									#pragma comment( lib, "silk_common_vs2013_x86_d.lib" )
									#pragma comment( lib, "celt_vs2013_x86_d.lib" )
								#else
									#pragma comment( lib, "opus_vs2013_x86.lib" )
									#pragma comment( lib, "opusfile_vs2013_x86.lib" )
									#pragma comment( lib, "silk_common_vs2013_x86.lib" )
									#pragma comment( lib, "celt_vs2013_x86.lib" )
								#endif
							#endif
						#elif _MSC_VER >= 1700
							#ifdef _WIN64
								#ifdef _DEBUG
									#pragma comment( lib, "opus_vs2012_x64_d.lib" )
									#pragma comment( lib, "opusfile_vs2012_x64_d.lib" )
									#pragma comment( lib, "silk_common_vs2012_x64_d.lib" )
									#pragma comment( lib, "celt_vs2012_x64_d.lib" )
								#else
									#pragma comment( lib, "opus_vs2012_x64.lib" )
									#pragma comment( lib, "opusfile_vs2012_x64.lib" )
									#pragma comment( lib, "silk_common_vs2012_x64.lib" )
									#pragma comment( lib, "celt_vs2012_x64.lib" )
								#endif
							#else
								#ifdef _DEBUG
									#pragma comment( lib, "opus_vs2012_x86_d.lib" )
									#pragma comment( lib, "opusfile_vs2012_x86_d.lib" )
									#pragma comment( lib, "silk_common_vs2012_x86_d.lib" )
									#pragma comment( lib, "celt_vs2012_x86_d.lib" )
								#else
									#pragma comment( lib, "opus_vs2012_x86.lib" )
									#pragma comment( lib, "opusfile_vs2012_x86.lib" )
									#pragma comment( lib, "silk_common_vs2012_x86.lib" )
									#pragma comment( lib, "celt_vs2012_x86.lib" )
								#endif
							#endif
						#endif // _MSC_VER >= 1700
					#endif // _MSC_VER < 1700
				#else // _MSC_VER
					#pragma comment( lib, "opus.lib" )
					#pragma comment( lib, "opusfile.lib" )
					#pragma comment( lib, "silk_common.lib" )
					#pragma comment( lib, "celt.lib" )
				#endif // _MSC_VER
			#endif
		#endif  // DX_GCC_COMPILE
	#endif	// DX_LIB_NOT_DEFAULTPATH
#endif // __DX_MAKE

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// �}�N����` --------------------------------------------------------------------

// ���ˑ��L�[���[�h�Ȃ�
#ifndef FASTCALL
#define FASTCALL	__fastcall
#endif

// �c�����������h���������̃o�[�W�����ݒ�
#define DIRECTINPUT_VERSION 0x700

#ifndef DWORD_PTR
#ifdef _WIN64
#define DWORD_PTR	ULONGLONG
#else
#define DWORD_PTR	DWORD
#endif
#endif

#ifndef LONG_PTR
#ifdef _WIN64
#define LONG_PTR	__int64
#else
#define LONG_PTR	int
#endif
#endif

#define DX_READSOUNDFUNCTION_ACM					(1 << ( DX_READSOUNDFUNCTION_DEFAULT_NUM + 0 ))		// ACM ���g�p�����ǂݍ��ݏ���
#define DX_READSOUNDFUNCTION_MP3					(1 << ( DX_READSOUNDFUNCTION_DEFAULT_NUM + 1 ))		// ACM ���g�p���� MP3 �̓ǂݍ��ݏ���
#define DX_READSOUNDFUNCTION_DSMP3					(1 << ( DX_READSOUNDFUNCTION_DEFAULT_NUM + 2 ))		// DirectShow ���g�p���� MP3 �̓ǂݍ��ݏ���

// Direct3D9 �p�e�N�X�`���t�H�[�}�b�g
#define DX_TEXTUREFORMAT_DIRECT3D9_R8G8B8				(1)
#define DX_TEXTUREFORMAT_DIRECT3D9_A8R8G8B8				(2)
#define DX_TEXTUREFORMAT_DIRECT3D9_X8R8G8B8				(3)
#define DX_TEXTUREFORMAT_DIRECT3D9_R5G6B5				(4)
#define DX_TEXTUREFORMAT_DIRECT3D9_X1R5G5B5				(5)
#define DX_TEXTUREFORMAT_DIRECT3D9_A1R5G5B5				(6)
#define DX_TEXTUREFORMAT_DIRECT3D9_A4R4G4B4				(7)
#define DX_TEXTUREFORMAT_DIRECT3D9_R3G3B2				(8)
#define DX_TEXTUREFORMAT_DIRECT3D9_A8R3G3B2				(9)
#define DX_TEXTUREFORMAT_DIRECT3D9_X4R4G4B4				(10)
#define DX_TEXTUREFORMAT_DIRECT3D9_A2B10G10R10			(11)
#define DX_TEXTUREFORMAT_DIRECT3D9_G16R16				(12)
#define DX_TEXTUREFORMAT_DIRECT3D9_A8B8G8R8				(13)
#define DX_TEXTUREFORMAT_DIRECT3D9_X8B8G8R8				(14)
#define DX_TEXTUREFORMAT_DIRECT3D9_A2R10G10B10			(15)
#define DX_TEXTUREFORMAT_DIRECT3D9_A16B16G16R16			(16)
#define DX_TEXTUREFORMAT_DIRECT3D9_R16F					(17)
#define DX_TEXTUREFORMAT_DIRECT3D9_G16R16F				(18)
#define DX_TEXTUREFORMAT_DIRECT3D9_A16B16G16R16F		(19)
#define DX_TEXTUREFORMAT_DIRECT3D9_R32F					(20)
#define DX_TEXTUREFORMAT_DIRECT3D9_G32R32F				(21)
#define DX_TEXTUREFORMAT_DIRECT3D9_A32B32G32R32F		(22)

// �\���̒�` --------------------------------------------------------------------

// �e�[�u��-----------------------------------------------------------------------

// �������ϐ��錾 --------------------------------------------------------------

// �֐��v���g�^�C�v�錾-----------------------------------------------------------

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // __DXDATATYPEWIN_H__
