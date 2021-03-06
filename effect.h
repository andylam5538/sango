//=============================================================================
//
// effect.h
// エフェクト関連各種処理
// GP12A331 伊澤佐千生
//
// 6/4現在は拡縮のエフェクトのみなので、アップデート以外特になし
// 影付ければbetter	
//
//=============================================================================
#ifndef _EFFECT__
#define _EFFECT__

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
//#include "playbar.h"


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	int CunterEffect;					// 拡縮のカウンター　徐々に拡縮するために使用
	int CounterSpeed;					// 拡縮の速さ
	int CountRevers;					// 拡縮の+-反転用
	float fWidth;						// 現在の幅
	float fHeight;						// 現在の高さ
	float fBeforeWidth;					// 元の幅   保存用ワーク
	float fBeforeHeight;				// 元の高さ 保存用ワーク
	bool bUse;							// エフェクトが使用中か
}EFFECT;

void InitBarEffect(void);
void UninitBarEffect(void);
//void UpdateBarEffect(PLAYBAR *playbar);
void DrawBarEffect(void);
//void SetBarEffect(PLAYBAR *pPlaybar);
void SetBarEffect(int nPlaybarNum);

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#endif