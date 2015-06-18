/*****************************************************************************
 *
 * 数字の表示処理 [number.h]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 * 
 ******************************************************************************/
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUMBER_MAX						(2)					// 最大数

//*************************************
// 構造体宣言
//*************************************
typedef struct
{
	D3DXVECTOR3			pos;			// 位置
	D3DXVECTOR3			rot;			// 向き
	float				fWidth;			// 幅
	float				fHeight;		// 高さ
	bool				bUse;			// 使用しているかどうか
	int					nType;			// 種類
}NUMBER;

typedef struct
{
	int					nItemLockTime;	// 時間固定
}ITEM_NUMBER;
//*************************************
// NUMBERの種類
//*************************************
enum
{
	NUMBERTYPE_000 = 0,		// タイプ０
	NUMBERTYPE_001,			// タイプ１
	NUMBERTYPE_002,			// タイプ２
	NUMBERTYPE_003,
	NUMBERTYPE_004,
	NUMBERTYPE_005,
	NUMBERTYPE_006,
	NUMBERTYPE_007,
	NUMBERTYPE_008,
	NUMBERTYPE_009,
	NUMBERTYPE_MAX
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitNumber(void);
void UninitNumber(void);
void UpdateNumber(void);
void DrawNumber(void);
void SetColorNumber(int nIdx, D3DCOLOR col_3);
void SetNumber(D3DXVECTOR3 pos, int nType);
ITEM_NUMBER* GetItemNum(void);
void UpdataItemNum(int nNum);
#endif
