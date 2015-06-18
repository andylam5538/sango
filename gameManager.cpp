/*****************************************************************************
 *
 * ゲームマネジメント処理 [gameManager.h]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#include "gameManager.h"
#include "input.h"
#include "load.h"
#include "save.h"
#include "wall.h"
#include "gimmick.h"
#include "debugproc.h"
#include "timeCounter.h"
#include "player.h"
#include "timeTrack.h"
#include "playbar.h"
#include "gimmick.h"
#include "item.h"
#include "number.h"
#include "fade.h"
#include "gameelement.h"
#include "background.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int		g_nGameMode;
int		g_nMenuSelect_GM;	// 選択したメニュー
bool	g_bIsPlay;			// プレー
bool	g_bIsStop;			// 停止
bool	g_bIsPause;			// ポーズ
bool	g_bIsSceneInit;		// シーンの初期化

PLAYBAR*		g_pPlaybar_GM = GetPlaybar();		// 時間軸のデータゲット
GIMMICK*		g_pGimmick_GM = GetGimmickData();	// ギミックのデータゲット
ITEM_NUMBER*	g_pItemNum_GM = GetItemNum();		// アイテム個数のデータゲット
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGameManager(void)
{
	switch (SCENE_MODE)
	{
	case SCENE_01:
		// 从文件中读取
		LoadData(SCENE_01);
		InitDataWall();
		InitDataGimmick();
		InitDataPlayer();
		InitDataPlayerbar();

		for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
		{
			// 根据补完的数据对机关场景进行位置和运动预处理
			SetupGimmickTrack(nCntGim);
		}
		
		break;
	case SCENE_02:
		// 从文件中读取
		LoadData(SCENE_02);
		InitDataWall();
		InitDataGimmick();
		InitDataPlayer();
		InitDataPlayerbar();

		for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
		{
			// 根据补完的数据对机关场景进行位置和运动预处理
			SetupGimmickTrack(nCntGim);
		}
		break;

	default:
		break;
	}

	g_nGameMode = 0;
	g_nMenuSelect_GM = 0;
	g_bIsPlay = false;
	g_bIsStop = true;
	g_bIsPause = false;
	g_bIsSceneInit = false;
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitGameManager(void)
{
	
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGameManager(void)
{
#ifdef _DEBUG
	if(GetKeyboardTrigger(DIK_F1))
	{
		g_nGameMode = SELECTMODE_PLAY;
	}
	if(GetKeyboardTrigger(DIK_F2))
	{
		g_nGameMode = SELECTMODE_WALLEDIT;
	}
	if(GetKeyboardTrigger(DIK_F3))
	{
		g_nGameMode = SELECTMODE_GIMMICKEDIT;
	}
	if(GetKeyboardTrigger(DIK_F4))
	{
		g_nGameMode = SELECTMODE_PLAYEREDIT;
	}
	if(GetKeyboardTrigger(DIK_F5))
	{
		g_nGameMode = SELECTMODE_DELETE;
	}
	if(GetKeyboardTrigger(DIK_F6))
	{
		g_nGameMode = SELECTMODE_SCENECHANGE;
	}
	if(GetKeyboardTrigger(DIK_F7))
	{
		g_nGameMode = SELECTMODE_PLAYERBAR_NUM;
	}
	if(GetKeyboardTrigger(DIK_F8))
	{
		g_nGameMode = SELECTMODE_ITEMNUMEDIT;
	}
	if(g_nGameMode != SELECTMODE_PLAY)
		GetBackG()[BACKGROUND_EDIT_NET].bUse = true;
	else
		GetBackG()[BACKGROUND_EDIT_NET].bUse = false;

#endif

	switch (g_nGameMode)
	{
	case SELECTMODE_PLAY:
	{
		PrintDebugProc("【ゲームモード】\n");
		if(GetTimeCounter()->fCurrentTime > TIME_15)
			g_bIsPlay = false;

		if(!g_bIsPlay)
		{
			if(IsMouseRightTriggered())
			{
				int nGimNum = -1;
				int nItemNum = GetItemNum()->nItemLockTime;
				bool bIsLock = false;

				if(IsGimmickLock(&nGimNum, &bIsLock, &nItemNum) == true)
				{
					if(bIsLock == true)
					{
						SetItem(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ITEM_TYPE_01, nGimNum);
						UpdataItemNum(-1);
					}
					else
					{
						DeleteItem(nGimNum);
						UpdataItemNum(1);
					}
				}
			}
		}
		break;
	}
	case SELECTMODE_WALLEDIT:
	{
		PrintDebugProc("【壁を編集するモード】\n");
		WallCreateMode();
		break;
	}
	case SELECTMODE_GIMMICKEDIT:
	{
		PrintDebugProc("【ギミック編集モード】\n");
		GimmickCreateMode();
		break;
	}
	case SELECTMODE_PLAYEREDIT:
	{
		PrintDebugProc("【プレイヤー編集モード】\n");
		if(IsMouseLeftPressed())
			IsMouseHitPlayer();
		PlayerEdit();
		break;
	}
	case SELECTMODE_DELETE:
	{
		PrintDebugProc("【削除モード】\n");
		DeleteGimmick();
		DeleteWall();
		break;
	}
	case SELECTMODE_SCENECHANGE:
	{
		PrintDebugProc("【ステージ切り替えるモード】\n");
		if(IsMouseRightTriggered())
		{
			if(*GetSceneNum() < SCENE_TYPE_MAX)
				(*GetSceneNum())++;
			if(*GetSceneNum() == SCENE_TYPE_MAX)
				*GetSceneNum() = 0;
			g_bIsSceneInit = true;
		}

		if(g_bIsSceneInit == true)
		{
			g_bIsSceneInit = false;
			switch (*GetSceneNum())
			{
			case SCENE_01:
				// 从文件中读取
				LoadData(SCENE_01);
				break;
			case SCENE_02:
				// 从文件中读取
				LoadData(SCENE_02);
				break;
			case SCENE_03:
				// 从文件中读取
				LoadData(SCENE_03);
				break;
			case SCENE_04:
				// 从文件中读取
				LoadData(SCENE_04);
				break;
			case SCENE_05:
				// 从文件中读取
				LoadData(SCENE_05);
				break;
			default:
				break;
			}
			// 読み込むデータを初期化
			InitDataPlayer();
			InitDataWall();
			InitDataGimmick();
			InitDataPlayerbar();

			for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
			{
				// 根据补完的数据对机关场景进行位置和运动预处理
				SetupGimmickTrack(nCntGim);
				*GetCleanupSwitch() = false;
			}
		}
		break;
	}
	case SELECTMODE_PLAYERBAR_NUM:
	{
		PrintDebugProc("【時間軸編集モード】\n");
		PrintDebugProc("数字キー「１」　時間軸数：１\n");
		PrintDebugProc("数字キー「２」　時間軸数：２\n");
		PrintDebugProc("数字キー「３」　時間軸数：３\n");
		if(GetKeyboardTrigger(DIK_1))
		{
			for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++)
			{
				//if(g_pPlaybar_GM[nCntPlayBar].bUse == true)
					g_pPlaybar_GM[nCntPlayBar].bUse = false;
			}
			for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
			{
				if(g_pGimmick_GM[nCntGim].bUse)
				{
					g_pGimmick_GM[nCntGim].nPriorityNum01 = -1;
					g_pGimmick_GM[nCntGim].nPriorityNum02 = -1;
					g_pGimmick_GM[nCntGim].nPriorityNum03 = -1;
				}
			}
			SetPlaybar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYBAR_3_1);
			PlaybarNumCount();
		}
		if(GetKeyboardTrigger(DIK_2))
		{
			for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++)
			{
				//if(g_pPlaybar_GM[nCntPlayBar].bUse == true)
					g_pPlaybar_GM[nCntPlayBar].bUse = false;
			}
			for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
			{
				if(g_pGimmick_GM[nCntGim].bUse)
				{
					g_pGimmick_GM[nCntGim].nPriorityNum01 = -1;
					g_pGimmick_GM[nCntGim].nPriorityNum02 = -1;
					g_pGimmick_GM[nCntGim].nPriorityNum03 = -1;
				}
			}
			SetPlaybar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYBAR_3_1);
			SetPlaybar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYBAR_3_2);
			PlaybarNumCount();
		}
		if(GetKeyboardTrigger(DIK_3))
		{
			for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++)
			{
				//if(g_pPlaybar_GM[nCntPlayBar].bUse == true)
					g_pPlaybar_GM[nCntPlayBar].bUse = false;
			}
			for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
			{
				if(g_pGimmick_GM[nCntGim].bUse)
				{
					g_pGimmick_GM[nCntGim].nPriorityNum01 = -1;
					g_pGimmick_GM[nCntGim].nPriorityNum02 = -1;
					g_pGimmick_GM[nCntGim].nPriorityNum03 = -1;
				}
			}
			SetPlaybar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYBAR_3_1);
			SetPlaybar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYBAR_3_2);
			SetPlaybar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYBAR_3_3);
			PlaybarNumCount();
		}
		break;
	}
	case SELECTMODE_ITEMNUMEDIT:
	{
		PrintDebugProc("【アイテム個数編集モード】\n");
		PrintDebugProc("【メニュー】    【アイテム】    【個数】\n");
		PrintDebugProc("数字キー「１」 時間固定アイテム：%d\n\n", g_pItemNum_GM->nItemLockTime);

		if(GetKeyboardTrigger(DIK_1))
		{
			g_nMenuSelect_GM = 1;
		}

		switch (g_nMenuSelect_GM)
		{

			case 1:
				PrintDebugProc("矢印キー【左】、【右】   【プラス/マイナス 1】\n");
				PrintDebugProc("矢印キー【上】、【下】   【プラス/マイナス 5】\n");

				if(GetKeyboardRepeat(DIK_LEFT))
				{
					g_pItemNum_GM->nItemLockTime -= 1;
					if(g_pItemNum_GM->nItemLockTime < 0)
						g_pItemNum_GM->nItemLockTime = 0;
				}
				if(GetKeyboardRepeat(DIK_RIGHT))
				{
					g_pItemNum_GM->nItemLockTime += 1;
					if(g_pItemNum_GM->nItemLockTime > 9)
						g_pItemNum_GM->nItemLockTime = 9;
				}
				if(GetKeyboardRepeat(DIK_DOWN))
				{
					g_pItemNum_GM->nItemLockTime -= 5;
					if(g_pItemNum_GM->nItemLockTime < 0)
						g_pItemNum_GM->nItemLockTime = 0;
				}
				if(GetKeyboardRepeat(DIK_UP))
				{
					g_pItemNum_GM->nItemLockTime += 5;
					if(g_pItemNum_GM->nItemLockTime > 9)
						g_pItemNum_GM->nItemLockTime = 9;
				}
				break;
			default:
				break;
		}

	}
	default:
		break;
	}

	if(!g_bIsPlay)
	{
		switch (*GetSceneNum())
		{
		case SCENE_01:
			PrintDebugProc("【SCENE_01】\n");
			// 今のシーンをセーフする
			if(IsMouseCenterTriggered())
				SaveData(SCENE_01);
			break;

		case SCENE_02:
			PrintDebugProc("【SCENE_02】\n");
			if(IsMouseCenterTriggered())
				SaveData(SCENE_02);
			break;

		case SCENE_03:
			PrintDebugProc("【SCENE_03】\n");
			if(IsMouseCenterTriggered())
				SaveData(SCENE_03);
			break;

		case SCENE_04:
			PrintDebugProc("【SCENE_04】\n");
			if(IsMouseCenterTriggered())
				SaveData(SCENE_04);
			break;

		case SCENE_05:
			PrintDebugProc("【SCENE_05】\n");
			if(IsMouseCenterTriggered())
				SaveData(SCENE_05);
			break;

		default:
			break;
		}
	}

	switch(*GetMode())
	{
	case GAMEMODE_LOGO:
		break;

	case GAMEMODE_TITLE:
	{
		if(GetKeyboardTrigger(DIK_RETURN) || IsMouseLeftTriggered())
			SetFade(FADE_OUT, FADE_IN, GAMEMODE_STAGE_SELECT);
		break;
	}
	case GAMEMODE_STAGE_SELECT:
	{
		g_bIsStop = true;
		g_bIsPlay = false;

		if(GetKeyboardTrigger(DIK_RETURN))
			SetFade(FADE_OUT, FADE_IN, GAMEMODE_GAME);

		if(IsMouseLeftTriggered())
		{
			int pIconNum = 0;
			if(IsMouseHitIcon(&pIconNum) == true)
			{
				*GetSceneNum() = GetGameElement()[pIconNum].nType;
				g_bIsSceneInit = true;
			}
		}

		if(g_bIsSceneInit == true)
		{
			g_bIsSceneInit = false;
			switch (*GetSceneNum())
			{
			case SCENE_01:
				// 从文件中读取
				LoadData(SCENE_01);
				break;
			case SCENE_02:
				// 从文件中读取
				LoadData(SCENE_02);
				break;
			case SCENE_03:
				// 从文件中读取
				LoadData(SCENE_03);
				break;
			case SCENE_04:
				// 从文件中读取
				LoadData(SCENE_04);
				break;
			case SCENE_05:
				// 从文件中读取
				LoadData(SCENE_05);
				break;
			default:
				break;
			}
			// 読み込むデータを初期化
			InitDataPlayer();
			InitDataWall();
			InitDataGimmick();
			InitDataPlayerbar();

			for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
			{
				// 根据补完的数据对机关场景进行位置和运动预处理
				SetupGimmickTrack(nCntGim);
				*GetCleanupSwitch() = false;
			}
			//*GetMode() = GAMEMODE_GAME;

			SetFade(FADE_OUT, FADE_IN, GAMEMODE_GAME);
		}
		
		break;
	}
	case GAMEMODE_GAME:
		break;

	case GAMEMODE_END:
		break;

	default:
		break;
	}



}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGameManager(void)
{
	
}

// ゲームを始めるかどうか
bool *GetIsPlay(void)
{
	return &g_bIsPlay;
}
// ゲームを止まるかどうか
bool *GetIsStop(void)
{
	return &g_bIsStop;
}
// ポーズかどうか
bool *GetIsPause(void)
{
	return &g_bIsPause;
}
// ゲームモードをゲット
int *GetGameMode(void)
{
	return &g_nGameMode;
}
