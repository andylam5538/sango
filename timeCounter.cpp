/*****************************************************************************
 *
 * 時間計算の表示処理 [timeCounter.cpp]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#include "timeCounter.h"
#include "debugproc.h"
#include "input.h"
#include "gameManager.h"
#include "playbar.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
TIMECOUNTER	g_nTimeCounter;					// 時間計算
int		g_nFrameCounter;					// フレーム
bool	bIsTimeReCount;						// 時間回す
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTimeCounter(void)
{
	g_nFrameCounter = 0;
	g_nTimeCounter.fCurrentTime = 0;
	g_nTimeCounter.fPlaybarTime = 0;
	g_nTimeCounter.nTime_0 = 0;
	g_nTimeCounter.nTime_5 = 5;
	g_nTimeCounter.nTime_10 = 10;
	g_nTimeCounter.nTime_15 = 15;
	bIsTimeReCount = false;
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitTimeCounter(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimeCounter(void)
{
	if(*GetIsPlay() == true && !*GetIsPause())
	{
		g_nFrameCounter++;
		if(g_nFrameCounter % 6 == 0)
		{
			g_nTimeCounter.fCurrentTime += 0.1f;
			g_nTimeCounter.fPlaybarTime += 0.1f;
		}
	}
	else if(*GetIsStop() == true)
	{
		g_nTimeCounter.fCurrentTime = 0;
		g_nTimeCounter.fPlaybarTime = 0;
	}
	
	// 添加时间轴判断进行时间复位
	if(g_nTimeCounter.fCurrentTime >= *PlaybarNum() * TIME_5)
	{
		g_nTimeCounter.fCurrentTime = 0;
		//g_nTimeCounter.fPlaybarTime = 0;
		bIsTimeReCount = true;
	}

	// ゲームモードだけ表示する
	if(*GetGameMode() == SELECTMODE_PLAY)
	{
		PrintDebugProc("SystemTime : %f\n", g_nTimeCounter.fCurrentTime);
		PrintDebugProc("PlaybarTime : %f\n", g_nTimeCounter.fPlaybarTime);
	}
}

//=============================================================================
// 描画処理
//=============================================================================

// 時間数のデータゲット
TIMECOUNTER *GetTimeCounter(void)
{
	return &g_nTimeCounter;
}

bool *GetIsTimeReCount(void)
{
	return &bIsTimeReCount;
}