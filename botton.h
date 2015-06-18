/*****************************************************************************
 *
 * ボタン表示処理 [botton.h]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#ifndef _BOTTON_H_
#define _BOTTON_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BOTTON	(3)		// 最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3				pos;				// 位置
	D3DXVECTOR3				rot;				// モデルの回転
	
	float					fWidth;				// 幅
	float					fHeight;			// 高さ
	float					fSpeed;				// すばやさ
	float					fMaxX;				// X軸最大幅
	float					fMinX;				// X軸最小幅
	float					fMaxY;				// Y軸最大幅
	float					fMinY;				// Y軸最小幅

	bool					bUse;				// 使用しているかどうか
	bool					bIsActiva;			// 選ばれたかどうか

	int						nCounterAnim;		// 计数器
	int						nPatternAnim;		// 记录图片帧
	int						nType;				// 类型
	int						nAlpha;				// 透明度

}BOTTON;

//////////////////////
// 贴图的种类名称
//////////////////////
enum
{
	BOTTON_PLAY = 0,						// タイプ０
	BOTTON_PAUSE,
	BOTTON_STOP,
	BOTTON_TYPE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBotton(void);
void UninitBotton(void);
void UpdateBotton(void);
void DrawBotton(void);
void SetBotton(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int type);
#endif
