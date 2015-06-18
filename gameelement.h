/*****************************************************************************
 *
 * 画面成分表示処理 [gameElement.h]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#ifndef _GAMEELEMENT_H_
#define _GAMEELEMENT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_GAMEELEMENT								(50)		// 最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3				pos;				// 位置
	D3DXVECTOR3				rot;				// モデルの回転
	
	float					fWidth;				// 幅
	float					fHeight;			// 高さ
	float					fRadius;			// 半径

	bool					bUse;				// 使用しているかどうか

	int						nCounterAnim;		// 计数器
	int						nPatternAnim;		// 记录图片帧
	int						nType;				// 类型
	int						nViewMode;			// 
} GAMEELEMENT;

//////////////////////
// 贴图的种类名称
//////////////////////
enum
{
	// 順番に並んでいる
	GE_STAGE_SHELL = 0,
	GE_STAGE_STARFISH,
	GE_STAGE_OCTOPUS,
	GE_STAGE_ANCHOR,
	GE_STAGE_SHIPWRECKS,
	GE_STAGE_VALUABLES,
	GE_STAGE_CASTLE,
	GE_STAGE_SOLDIER,
	GE_STAGE_POTATOES,
	GE_STAGE_CASKS,
	GE_STAGE_NEWSHIP,
	GE_STAGE_CACTUS,
	GE_STAGE_CAMEL,
	GE_STAGE_SCORPION,
	GE_STAGE_SPHINX,
	GE_STAGE_MAP,

	GE_PLAYERBAR_CLOCK,
	GE_TYPE_MAX
};

enum
{
	VIEW_STAGE_SELECT = 0,
	VIEW_OTHER,
	VIEW_TYPE_MAX
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGameElement(void);
void UninitGameElement(void);
void UpdateGameElement(void);
void DrawGameElement(void);
void SetGameElement(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int nType, int nGameMode);
bool IsMouseHitIcon(int* pIconNum);
GAMEELEMENT *GetGameElement(void);
#endif
