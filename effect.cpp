//=============================================================================
//
// effect.cpp
// エフェクト関連各種処理
// GP12A331 伊澤佐千生
//
// 6/4現在は拡縮のエフェクトのみなので、アップデート以外特になし
// 影付ければbetter	
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "effect.h"
#include "playbar.h"
#include "input.h"
#include "gameManager.h"
#include "timeTrack.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFFECT_UPPER_MAX	(120)
#define EFFECT_LOWWER_MAX	(80)
#define COUNT_SPEED			(2)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
EFFECT		g_BarEffect;
PLAYBAR*	g_pPlaybar_Eff = GetPlaybar();
//=============================================================================
//
// void InitBarEffect(void)
// Playbarのエフェクトの処理初期化
// 
//
//=============================================================================
void InitBarEffect(void)
{
	g_BarEffect.CunterEffect = 0;				// 拡縮のカウンター　徐々に拡縮するために使用
	g_BarEffect.CounterSpeed = COUNT_SPEED;		// 拡縮の速さ
	g_BarEffect.CountRevers = 1;				// 1か-1が入る　拡縮の+-に使用
	g_BarEffect.fWidth = 0;						// 現在の幅
	g_BarEffect.fHeight = 0;					// 現在の高さ
	g_BarEffect.fBeforeWidth = 0;				// 元の幅   保存用ワーク
	g_BarEffect.fBeforeHeight = 0;				// 元の高さ 保存用ワーク
	g_BarEffect.bUse = false;					// 使用中か
}

//=============================================================================
//
// void UninitBarEffect()
// Playbarのエフェクトの終了処理
// 特になし
//
//=============================================================================
void UninitBarEffect(void)
{
	
}

//=============================================================================
//
// void UpdateBarEffect(void)
// Playbarのエフェクトのアップデート処理
// 引数PlayBarのポインタ
// SetBarEffect()からの
//
//=============================================================================
void UpdateBarEffect(PLAYBAR *playbar)
{
	// エフェクト中であれば
	if(g_BarEffect.bUse)
	{
		// 左クリックしているとき
		if(IsMouseLeftPressed())
		{	
			// カウンター上限
			if(g_BarEffect.CunterEffect >= EFFECT_UPPER_MAX)
			{
				// 特になし
				g_BarEffect.CunterEffect = EFFECT_UPPER_MAX;
			}
			// カウンター下限
			else if(g_BarEffect.CunterEffect <= EFFECT_LOWWER_MAX)
			{
				g_BarEffect.CountRevers *= -1;					// 下限まで行ったので、カウント反転
				g_BarEffect.CunterEffect = EFFECT_LOWWER_MAX;	// 一応補正
			}
			// カウンター範囲内なら
			else
			{
				g_BarEffect.CunterEffect += g_BarEffect.CounterSpeed * g_BarEffect.CountRevers;	// カウンター加算
			}
		}
		// 左クリックされていない時
		else
		{
			playbar->fHeight = g_BarEffect.fBeforeHeight;	// 拡縮処理前のwidthを保存
			playbar->fWidth = g_BarEffect.fBeforeWidth;		// 拡縮処理前のheightを保存 
			InitBarEffect();								// 左クリックを離されたので、初期化
		}

		playbar->fHeight = g_BarEffect.fHeight;				// 計算後縦幅代入
		playbar->fWidth = g_BarEffect.fWidth;				// 計算後横幅代入
	}
}

//=============================================================================
//
// void DrawBarEffect()
// Playbarのエフェクトの描画処理
// 拡縮のみなので特になし
//
//=============================================================================
void DrawBarEffect(void)
{
	
}

//=============================================================================
//
// void SetBarEffect()
// エフェクトセット
// BeforeWidthとBeforeHeightを保存・bUseをtrue
//
//=============================================================================
//void SetBarEffect(PLAYBAR *pPlaybar)
//{
//	// 拡縮前の情報を格納
//	g_BarEffect.fBeforeHeight = pPlaybar->fHeight;
//	g_BarEffect.fBeforeWidth = pPlaybar->fWidth;
//	
//	// 拡縮用変数
//	g_BarEffect.fHeight = pPlaybar->fHeight;
//	g_BarEffect.fWidth = pPlaybar->fWidth;
//	g_BarEffect.bUse = true;
//}

void SetBarEffect(int nPlaybarNum)
{
	// 拡縮前の情報を格納
	g_BarEffect.fBeforeHeight = g_pPlaybar_Eff[nPlaybarNum].fHeight;
	g_BarEffect.fBeforeWidth = g_pPlaybar_Eff[nPlaybarNum].fWidth;
	
	// 拡縮用変数
	g_BarEffect.fHeight = g_pPlaybar_Eff[nPlaybarNum].fHeight;
	g_BarEffect.fWidth = g_pPlaybar_Eff[nPlaybarNum].fWidth;
	g_BarEffect.bUse = true;
}

void MousePressedEffect(int nPlaybarNum)
{

}