/*****************************************************************************
 *
 *読み込み処理
 *
 *作る時間 ： 2015.4.28
 *
 *GP12A 331 朱鷹仁 40582
 *
 ******************************************************************************/
#include "load.h"
#include "wall.h"
#include "gimmick.h"
#include "player.h"
#include "number.h"
#include "playbar.h"
//*****************************************************************************
// グローバル変数
//*****************************************************************************
WALL*			g_pWall_Load = GetWallData();
GIMMICK*		g_pGimmick_Load = GetGimmickData();
PLAYER*			g_pPlayer_Load = GetPlayer();
ITEM_NUMBER*	g_pNumItem_Load = GetItemNum();
PLAYBAR*		g_pPlaybar_Load = GetPlaybar();
// 読み込み処理
bool LoadData(int nMode)
{
	FILE *pFile;
	char aName[256];
	int nCntWall = 0;
	int nCntGim = 0;
	int nCntPlayer = 0;

	// 初期化
	for(int nCntGimInit = 0; nCntGimInit < MAX_GIMMICK; nCntGimInit++)
	{
		if(g_pGimmick_Load[nCntGimInit].bUse == true)
			g_pGimmick_Load[nCntGimInit].bUse = false;
	}
	for(int nCntWallInit = 0; nCntWallInit < MAX_WALL; nCntWallInit++)
	{
		if(g_pWall_Load[nCntWallInit].bUse == true)
			g_pWall_Load[nCntWallInit].bUse = false;
	}
	for(int nCntPlayerInit = 0; nCntPlayerInit < MAX_PLAYER; nCntPlayerInit++)
	{
		if(g_pPlayer_Load[nCntPlayerInit].bUse == true)
			g_pPlayer_Load[nCntPlayerInit].bUse = false;
	}
	for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++)
	{
		if(g_pPlaybar_Load[nCntPlayBar].bUse == true)
			g_pPlaybar_Load[nCntPlayBar].bUse = false;
	}


	// LOADMODE_WALL:
	switch(nMode)
	{
	case SCENE_01:
		if((pFile = fopen("data/DATABASE/SCENE_01/wallData_SCENE_01.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_02:
		if((pFile = fopen("data/DATABASE/SCENE_02/wallData_SCENE_02.csv", "r")) == NULL)
			return false;
		break;	
	case SCENE_03:
		if((pFile = fopen("data/DATABASE/SCENE_03/wallData_SCENE_03.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_04:
		if((pFile = fopen("data/DATABASE/SCENE_04/wallData_SCENE_04.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_05:
		if((pFile = fopen("data/DATABASE/SCENE_05/wallData_SCENE_05.csv", "r")) == NULL)
			return false;
		break;
	default:
		break;
	}
	// 行最初のコメントを削除
	fscanf(pFile, "%s", aName);
	while(!feof(pFile))
	{
		fscanf(	pFile,"%f,%f,%f,%d,%d,%s",
				&g_pWall_Load[nCntWall].pos.x,
				&g_pWall_Load[nCntWall].pos.y,
				&g_pWall_Load[nCntWall].pos.z,
				&g_pWall_Load[nCntWall].bUse,
				&g_pWall_Load[nCntWall].nType,
				&g_pWall_Load[nCntWall].aRemarks);
		nCntWall++;
	}
	fclose(pFile);

	// LOADMODE_GIMMICK:
	switch(nMode)
	{
	case SCENE_01:
		if((pFile = fopen("data/DATABASE/SCENE_01/gimmickData_SCENE_01.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_02:
		if((pFile = fopen("data/DATABASE/SCENE_02/gimmickData_SCENE_02.csv", "r")) == NULL)
			return false;
		break;	
	case SCENE_03:
		if((pFile = fopen("data/DATABASE/SCENE_03/gimmickData_SCENE_03.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_04:
		if((pFile = fopen("data/DATABASE/SCENE_04/gimmickData_SCENE_04.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_05:
		if((pFile = fopen("data/DATABASE/SCENE_05/gimmickData_SCENE_05.csv", "r")) == NULL)
			return false;
		break;
	default:
		break;
	}
	// 行最初のコメントを削除
	fscanf(pFile, "%s", aName);
	while(!feof(pFile))
	{
		fscanf(	pFile,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%s",
				&g_pGimmick_Load[nCntGim].pos.x,
				&g_pGimmick_Load[nCntGim].pos.y,
				&g_pGimmick_Load[nCntGim].pos.z,
				&g_pGimmick_Load[nCntGim].fTimeStart,
				&g_pGimmick_Load[nCntGim].fTimeEnd,
				&g_pGimmick_Load[nCntGim].fRadiusDis,			// 半径距离
				&g_pGimmick_Load[nCntGim].fWidth,
				&g_pGimmick_Load[nCntGim].fHeight,
				&g_pGimmick_Load[nCntGim].fGoalX,
				&g_pGimmick_Load[nCntGim].fGoalY,
				&g_pGimmick_Load[nCntGim].bUse,
				&g_pGimmick_Load[nCntGim].nType,
				&g_pGimmick_Load[nCntGim].aRemarks);
		nCntGim++;
	}
	fclose(pFile);

	// LOADMODE_PLAYER:
	switch(nMode)
	{
	case SCENE_01:
		if((pFile = fopen("data/DATABASE/SCENE_01/playerData_SCENE_01.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_02:
		if((pFile = fopen("data/DATABASE/SCENE_02/playerData_SCENE_02.csv", "r")) == NULL)
			return false;
		break;	
	case SCENE_03:
		if((pFile = fopen("data/DATABASE/SCENE_03/playerData_SCENE_03.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_04:
		if((pFile = fopen("data/DATABASE/SCENE_04/playerData_SCENE_04.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_05:
		if((pFile = fopen("data/DATABASE/SCENE_05/playerData_SCENE_05.csv", "r")) == NULL)
			return false;
		break;
	default:
		break;
	}
	// 行最初のコメントを削除
	fscanf(pFile, "%s", aName);
	while(!feof(pFile))
	{
		fscanf(	pFile,"%f,%f,%f,%f,%d,%d\n",
				&g_pPlayer_Load[nCntPlayer].pos.x,
				&g_pPlayer_Load[nCntPlayer].pos.y,
				&g_pPlayer_Load[nCntPlayer].pos.z,
				&g_pPlayer_Load[nCntPlayer].move.x,
				&g_pPlayer_Load[nCntPlayer].bUse,
				&g_pPlayer_Load[nCntPlayer].nType);
		nCntPlayer++;
	}
	fclose(pFile);

	// LOADMODE_ITEM:
	switch(nMode)
	{
	case SCENE_01:
		if((pFile = fopen("data/DATABASE/SCENE_01/itemData_SCENE_01.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_02:
		if((pFile = fopen("data/DATABASE/SCENE_02/itemData_SCENE_02.csv", "r")) == NULL)
			return false;
		break;	
	case SCENE_03:
		if((pFile = fopen("data/DATABASE/SCENE_03/itemData_SCENE_03.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_04:
		if((pFile = fopen("data/DATABASE/SCENE_04/itemData_SCENE_04.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_05:
		if((pFile = fopen("data/DATABASE/SCENE_05/itemData_SCENE_05.csv", "r")) == NULL)
			return false;
		break;
	default:
		break;
	}
	// 行最初のコメントを削除
	fscanf(pFile, "%s", aName);
	while(!feof(pFile))
	{
		fscanf(	pFile,"%d\n",&g_pNumItem_Load->nItemLockTime);
		nCntPlayer++;
	}
	fclose(pFile);

	// LOADMODE_PLAYBARNUM:
	switch(nMode)
	{
	case SCENE_01:
		if((pFile = fopen("data/DATABASE/SCENE_01/playbar_SCENE_01.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_02:
		if((pFile = fopen("data/DATABASE/SCENE_02/playbar_SCENE_02.csv", "r")) == NULL)
			return false;
		break;	
	case SCENE_03:
		if((pFile = fopen("data/DATABASE/SCENE_03/playbar_SCENE_03.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_04:
		if((pFile = fopen("data/DATABASE/SCENE_04/playbar_SCENE_04.csv", "r")) == NULL)
			return false;
		break;
	case SCENE_05:
		if((pFile = fopen("data/DATABASE/SCENE_05/playbar_SCENE_05.csv", "r")) == NULL)
			return false;
		break;
	default:
		break;
	}
	// 行最初のコメントを削除
	fscanf(pFile, "%s", aName);
	while(!feof(pFile))
	{
		fscanf(	pFile,"%d\n",PlaybarNum());
		nCntPlayer++;
	}
	fclose(pFile);

	return true;
}