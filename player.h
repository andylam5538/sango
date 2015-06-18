/*****************************************************************************
 *
 * プレーヤー表示処理 [player.h]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_PLAYER	(2)		// 最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3				pos;				// 位置
	D3DXVECTOR3				posSave;			// 位置の保存
	D3DXVECTOR3				rot;				// モデルの回転
	D3DXVECTOR3				move;				// 移動量
	D3DXVECTOR3				size;				// サイズ
	
	float					fWidth;				// 幅
	float					fHeight;			// 高さ
	float					fSpeedX;			// すばやさX
	float					fSpeedY;			// すばやさX
	float					fRadius;			// 半径
	float					fMaxX;				// X軸最大幅
	float					fMinX;				// X軸最小幅
	float					fMaxY;				// Y軸最大幅
	float					fMinY;				// Y軸最小幅

	bool					bUse;				// 使用しているかどうか
	bool					bIsHit;				// 当たるかどうか
	bool					bIsChangeDir;		// 向きを変える
	bool					bIsGoLeft;				//左に向く
	bool					bIsOnfloor;				//フロアに立っているか
	bool					bIsOnSlope;				//坂の上にあるか
	bool					bIsHitByGmmic;			//ギミックに当たったか

	int						nState;				//現在の状態
	int						nCounterAnim;		// 计数器
	int						nPatternAnim;		// 记录图片帧
	int						nType;				// 类型

}PLAYER;

//////////////////////
// 贴图的种类名称
//////////////////////
enum
{
	PLAYER_001 = 0,						// タイプ０
	PLAYER_TYPE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void SetPlayer(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int type);
PLAYER *GetPlayer(void);
bool IsPlayerReachGoal(int nCntPlayer, float fMoveX, float fMoveY);
bool IsPlayerHitGimmick(int nCntPlayer, float fMoveX, float fMoveY);
bool IsMouseHitPlayer(void);
void InitDataPlayer(void);
void PlayerEdit(void);
#endif
