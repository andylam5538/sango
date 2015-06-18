/*****************************************************************************
 *
 * マウス表示処理 [mouseLocal.h]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#ifndef _MOUSELOCAL_H_
#define _MOUSELOCAL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_MOUSELOCAL			(1)					// 最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3				pos;				// 位置
	
	float					fWidth;				// 幅
	float					fHeight;			// 高さ

	bool					bUse;				// 使用しているかどうか
	bool					bIsActiva;			// 選ばれたかどうか

	int						nCounterAnim;		// 计数器
	int						nPatternAnim;		// 记录图片帧
	int						nType;				// 类型
}MOUSELOCAL;

//////////////////////
// 贴图的种类名称
//////////////////////
enum
{
	MOUSELOCAL_TYPE_01 = 0,						// タイプ０
	MOUSELOCAL_TYPE_MAX
};
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitMouseLocal(void);
void UninitMouseLocal(void);
void UpdateMouseLocal(void);
void DrawMouseLocal(void);
void SetMouseLocal(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int type);
#endif
