/*****************************************************************************
 *
 * 壁の表示処理 [wall.h]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_WALL			(1000)				// 最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3				pos;				// 位置
	D3DXVECTOR3				rot;				// モデルの回転
	D3DXVECTOR2				vec2StartPoint;				//足元にある直線の始点（あたり判定用）
	D3DXVECTOR2				vec2Direct;						//方向ベクトル

	float					fWidth;				// 幅
	float					fHeight;			// 高さ
	float					fMaxX;				// X軸最大幅
	float					fMinX;				// X軸最小幅
	float					fMaxY;				// Y軸最大幅
	float					fMinY;				// Y軸最小幅
	float					fSteep;			//傾き
	
	bool					bUse;				// 使用しているかどうか
	char					aRemarks[256];		// コメント

	int						nType;				// 種類

} WALL;

//////////////////////
// 贴图的种类名称
//////////////////////
enum
{
	WALL_TYPE_001 = 0,
	WALL_TYPE_002,
	WALL_TYPE_SLOPE_UP_45,
	WALL_TYPE_SLOPE_DOWN_45,
	WALL_TYPE_GOAL,
	WALL_TYPE_DOOR_R,
	WALL_TYPE_DOOR_G,
	WALL_TYPE_DOOR_B,
	WALL_TYPE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
int SetWall(D3DXVECTOR3 pos, D3DXCOLOR col, int type, char aRemarks[256]);
void WallCreateMode(void);
WALL *GetWallData(void);
void InitDataWall(void);
void DeleteWall(void);
#endif
