// -------------------------------------------------------------------------------
// 
// 		ＤＸライブラリ		モデルデータ制御プログラム( Direct3D11 )ヘッダファイル
// 
// 				Ver 3.16d
// 
// -------------------------------------------------------------------------------

#ifndef __DXMODELD3D11_H__
#define __DXMODELD3D11_H__

#include "../DxCompileConfig.h"

#ifndef DX_NON_GRAPHICS

#ifndef DX_NON_DIRECT3D11

// インクルード ------------------------------------------------------------------

#ifndef DX_NON_MODEL

#include "../DxLib.h"
#include "../DxModel.h"

#ifndef DX_NON_NAMESPACE

namespace DxLib
{

#endif // DX_NON_NAMESPACE

// マクロ定義 --------------------------------------------------------------------

// 構造体定義 --------------------------------------------------------------------

// 剛体メッシュタイプ頂点構造体
struct MV1_VERTEX_SIMPLE_D3D11
{
	VECTOR					Position ;							// 座標
	VECTOR					Normal ;							// 法線
	COLOR_U8				DiffuseColor ;						// ディフューズ色
	COLOR_U8				SpecularColor ;						// スペキュラー色
	FLOAT4					UVs[ 2 ] ;							// テクスチャ座標
} ;

// 剛体メッシュタイプバンプマップ用頂点構造体
struct MV1_VERTEX_SIMPLE_BUMP_D3D11
{
	VECTOR					Position ;							// 座標
	VECTOR					Tangent ;							// 頂点の接線
	VECTOR					Binormal ;							// 頂点の従法線
	VECTOR					Normal ;							// 法線
	COLOR_U8				DiffuseColor ;						// ディフューズ色
	COLOR_U8				SpecularColor ;						// スペキュラー色
	FLOAT4					UVs[ 2 ] ;							// テクスチャ座標
} ;

// スキンメッシュタイプ頂点構造体(関連ボーン４つ以内)
struct MV1_VERTEX_SKIN_B4_D3D11
{
	VECTOR					Position ;							// 座標
	unsigned char			MatrixIndex[ 4 ] ;					// 行列インデックス( 行列番号×４ )
	float					MatrixWeight[ 4 ] ;					// 行列ウエイト
	VECTOR					Normal ;							// 法線
	COLOR_U8				DiffuseColor ;						// ディフューズ色
	COLOR_U8				SpecularColor ;						// スペキュラー色
	FLOAT4					UVs[ 2 ] ;							// テクスチャ座標
} ;

// スキンメッシュタイプバンプマップ用頂点構造体(関連ボーン４つ以内)
struct MV1_VERTEX_SKIN_B4_BUMP_D3D11
{
	VECTOR					Position ;							// 座標
	unsigned char			MatrixIndex[ 4 ] ;					// 行列インデックス( 行列番号×４ )
	float					MatrixWeight[ 4 ] ;					// 行列ウエイト
	VECTOR					Tangent ;							// 頂点の接線
	VECTOR					Binormal ;							// 頂点の従法線
	VECTOR					Normal ;							// 法線
	COLOR_U8				DiffuseColor ;						// ディフューズ色
	COLOR_U8				SpecularColor ;						// スペキュラー色
	FLOAT4					UVs[ 2 ] ;							// テクスチャ座標
} ;

// スキンメッシュタイプ頂点構造体(関連ボーン８つ以内)
struct MV1_VERTEX_SKIN_B8_D3D11
{
	VECTOR					Position ;							// 座標
	unsigned char			MatrixIndex[ 8 ] ;					// 行列インデックス( 行列番号×４ )
	float					MatrixWeight[ 8 ] ;					// 行列ウエイト
	VECTOR					Normal ;							// 法線
	COLOR_U8				DiffuseColor ;						// ディフューズ色
	COLOR_U8				SpecularColor ;						// スペキュラー色
	FLOAT4					UVs[ 2 ] ;							// テクスチャ座標
} ;

// スキンメッシュタイプバンプマップ用頂点構造体(関連ボーン８つ以内)
struct MV1_VERTEX_SKIN_B8_BUMP_D3D11
{
	VECTOR					Position ;							// 座標
	unsigned char			MatrixIndex[ 8 ] ;					// 行列インデックス( 行列番号×８ )
	float					MatrixWeight[ 8 ] ;					// 行列ウエイト
	VECTOR					Tangent ;							// 頂点の接線
	VECTOR					Binormal ;							// 頂点の従法線
	VECTOR					Normal ;							// 法線
	COLOR_U8				DiffuseColor ;						// ディフューズ色
	COLOR_U8				SpecularColor ;						// スペキュラー色
	FLOAT4					UVs[ 2 ] ;							// テクスチャ座標
} ;

// Direct3D11用 モデルデータ管理用構造体
struct MV1_MODEL_MANAGE_DIRECT3D11
{
	void *					CommonBuffer ;						// 主にシェイプ座標の更新で使用する汎用バッファ
	int						CommonBufferSize ;					// 主にシェイプ座標の更新で使用する汎用バッファのサイズ
} ;

// Direct3D11用頂点バッファ情報
struct MV1_VERTEXBUFFER_DIRECT3D11
{
	D_ID3D11Buffer *		VertexBuffer ;						// 頂点バッファ
	D_ID3D11Buffer *		IndexBuffer ;						// インデックスバッファ
} ;

// Direct3D11用トライアングルリスト基データ情報
struct MV1_TRIANGLE_LIST_BASE_DIRECT3D11
{
	BYTE					SkinFreeBoneVertexBufferUpdate ;	// ９ボーン以上のスキニングメッシュ処理用頂点データを更新したかどうか( TRUE:更新済み  FALSE:未更新 )
	void *					SkinFreeBoneVertexBuffer ;			// ９ボーン以上のスキニングメッシュ処理用頂点データ
} ;

// Direct3D11用トライアングルリスト情報
struct MV1_TRIANGLE_LIST_DIRECT3D11
{
	void *					SkinFreeBoneVertexPositionBuffer ;	// ９ボーン以上のスキニングメッシュ処理用頂点座標データ
	D_ID3D11Buffer *		VertexBuffer ;						// 頂点バッファ
} ;

// 内部大域変数宣言 --------------------------------------------------------------

extern MV1_MODEL_MANAGE_DIRECT3D11 MV1Man_D3D11 ;

// 関数プロトタイプ宣言-----------------------------------------------------------

// 汎用バッファ関数
extern	int				MV1_D3D11_CommonBuffer_Setup( int Size ) ;						// 指定サイズの汎用バッファの準備を行う
extern	int				MV1_D3D11_CommonBuffer_Terminate( void ) ;						// 汎用バッファの後始末を行う

// 環境依存関数
extern	int				MV1_D3D11_Terminate_PF( void ) ;																// モデル機能の後始末
extern	int				MV1_D3D11_TerminateModelBaseHandle_PF( MV1_MODEL_BASE *ModelBase ) ;							// モデルデータハンドルの後始末
extern	int				MV1_D3D11_TerminateTriangleListBaseTempBuffer_PF( MV1_TRIANGLE_LIST_BASE *MBTList ) ;				// トライアングルリストの一時処理用のバッファを開放する
extern	void			MV1_D3D11_SetupPackDrawInfo_PF( MV1_MODEL_BASE *ModelBase ) ;									// 同時複数描画関係の情報をセットアップする
extern	int				MV1_D3D11_SetupVertexBufferBase_PF( int MV1ModelBaseHandle, int DuplicateNum = 1, int ASyncThread = FALSE ) ;	// モデル基データの頂点バッファのセットアップをする( -1:エラー )
extern	int				MV1_D3D11_SetupVertexBuffer_PF( int MHandle, int ASyncThread = FALSE ) ;						// モデルデータの頂点バッファのセットアップをする( -1:エラー )
extern	int				MV1_D3D11_TerminateVertexBufferBase_PF( int MV1ModelBaseHandle ) ;								// 頂点バッファの後始末をする( -1:エラー )
extern	int				MV1_D3D11_TerminateVertexBuffer_PF( int MV1ModelHandle ) ;										// 頂点バッファの後始末をする( -1:エラー )
extern	int				MV1_D3D11_SetupShapeVertex_PF( int MHandle ) ;													// シェイプデータのセットアップをする
extern	int				MV1_D3D11_BeginRender_PF( MV1_MODEL *Model ) ;													// ３Ｄモデルのレンダリングの準備を行う
extern	int				MV1_D3D11_EndRender_PF( void ) ;																// ３Ｄモデルのレンダリングの後始末を行う
extern	void			MV1_D3D11_DrawMesh_PF( MV1_MESH *Mesh, int TriangleListIndex = -1 ) ;							// メッシュ描画部分を抜き出したもの

#ifndef DX_NON_NAMESPACE

}

#endif // DX_NON_NAMESPACE

#endif // DX_NON_MODEL

#endif // DX_NON_DIRECT3D11

#endif // DX_NON_GRAPHICS

#endif // __DXMODELD3D11_H__
