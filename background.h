/*****************************************************************************
 *
 * 背景表示処理 [background.h]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BACKGROUND								(5)		// 最大数
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

	bool					bUse;				// 使用しているかどうか

	int						nCounterAnim;		// 计数器
	int						nPatternAnim;		// 记录图片帧
	int						nType;				// 类型

} BACKGROUND;

//////////////////////
// 贴图的种类名称
//////////////////////
enum
{
	BACKGROUND_TITLE = 0,						// タイプ０
	BACKGROUND_SCENE_SELECT,
	BACKGROUND_SCENE_01,
	BACKGROUND_EDIT_NET,
	BACKGROUND_TYPE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBackground(void);
void UninitBackground(void);
void UpdateBackground(void);
void DrawBackground(void);
void SetBackground(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int type);
BACKGROUND *GetBackG(void);
#endif
