/*****************************************************************************
 *
 *三校合同コンテスト
 *
 *作る時間 ： 2015.5.1
 *
 *GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"
#include <stdio.h>

#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#if 1	// [ここを"0"にした場合、"構成プロパティ" -> "リンカ" -> "入力" -> "追加の依存ファイル"に対象ライブラリを設定する]
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#endif

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ビルボード頂点フォーマット( 頂点座標[3D] / 法線 / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_BILLBOARD	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCREEN_WIDTH	(1200)				// ウインドウの幅
#define SCREEN_HEIGHT	(900)				// ウインドウの高さ
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// ウインドウの中心Ｙ座標

#define	NUM_VERTEX		(4)	// 頂点数
#define	NUM_POLYGON		(2)	// ポリゴン数

#define UPDATA_FRAME	(60)

#define TIME_0			(0.0f)
#define TIME_5			(5.0f)
#define TIME_10			(10.0f)
#define TIME_15			(15.0f)
#define TIME_20			(20.0f)
#define TIME_25			(25.0f)
#define TIME_30			(30.0f)

#define SCENE_MODE		(SCENE_01)

#define ALPHA_MAX		(255)
// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;

// 上記３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DXVECTOR3 nor;		// 法線ベクトル
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_3D;

// 上記ビルボード頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_BILLBOARD;

typedef struct
{
	bool	bIsClear;		// 頂点座標
	int		nSceneNum;		// 反射光
} SCENE;

typedef enum
{
	SCENE_01 = 0,
	SCENE_02,
	SCENE_03,
	SCENE_04,
	SCENE_05,
	SCENE_TYPE_MAX
}SCENEMODE;

typedef enum
{
	SELECTMODE_PLAY = 0,
	SELECTMODE_WALLEDIT,
	SELECTMODE_GIMMICKEDIT,
	SELECTMODE_PLAYEREDIT,
	SELECTMODE_DELETE,
	SELECTMODE_SCENECHANGE,
	SELECTMODE_PLAYERBAR_NUM,
	SELECTMODE_ITEMNUMEDIT,
	SELECTMODE_TYPE_MAX
}SELECTMODE;

// モードの種類
typedef enum
{
	GAMEMODE_LOGO = 0,
	GAMEMODE_TITLE,		// タイトル画面
	GAMEMODE_STAGE_SELECT,
	GAMEMODE_GAME,			// ゲーム画面
	GAMEMODE_END,
	GAMEMODE_MAX
} GAMEMODE;

//線分の定義
typedef struct
{
	D3DXVECTOR2 vec2StartPoint;				//足元にある直線の始点（あたり判定用）
	D3DXVECTOR2 vec2Direct;						//方向ベクトル
}SEGMENT;//線分の定義

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
float *GetMouX(void);
float *GetMouY(void);
GAMEMODE *GetMode(void);
void SetMode(GAMEMODE mode);
SCENE *GetScene(void);
int *GetSceneNum(void);
int *GetLastSceneNum(void);
#endif