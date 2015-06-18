/*****************************************************************************
 *
 * 時間軸表示処理 [playbar.h]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#ifndef _PLAYBAR_H_
#define _PLAYBAR_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_PLAYBAR			(3)					// 最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3				pos;				// 位置
	D3DXVECTOR3				rot;				// モデルの回転
	
	float					fWidth;				// 幅
	float					fHeight;			// 高さ
	float					fInitialPosX;		// 最初の位置
	float					fTimeStart;			// 始まる時間
	float					fTimeEnd;			// 終わる時間

	bool					bUse;				// 使用しているかどうか
	bool					bIsInvert;			// 時間軸の時間順番交換

	int						nCounterAnim;		// 计数器
	int						nPatternAnim;		// 记录图片帧
	int						nType;				// 类型
	int						nPriority;			// 優先順番
}PLAYBAR;

//////////////////////
// 贴图的种类名称
//////////////////////
enum
{
	PLAYBAR_3_1 = 0,			// タイプ０
	PLAYBAR_3_2,
	PLAYBAR_3_3,
	PLAYBAR_TYPE_MAX
};

// 優先順番
enum
{
	PRIORITY_FIRST = 0,			// タイプ０
	PRIORITY_SECOND,
	PRIORITY_THIRD,
	PRIORITY_FOURTH,
	PRIORITY_FIFTH,
	PRIORITY_SIXTH,
	PRIORITY_SEVENTH,
	PRIORITY_EIGHTH,
	PRIORITY_NINTH,
	PRIORITY_TENTH,
	PRIORITY_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlaybar(void);
void UninitPlaybar(void);
void UpdatePlaybar(void);
void DrawPlaybar(void);
void SetPlaybar(D3DXCOLOR col, int ntype);
void PlaybarPosChange(int nBeControlNum);
PLAYBAR *GetPlaybar(void);
void PlaybarNumCount(void);
int *PlaybarNum(void);
void InitDataPlayerbar(void);

#endif
