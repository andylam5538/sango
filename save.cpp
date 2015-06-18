/*****************************************************************************
 *
 *保存処理
 *
 *作る時間 ： 2015.4.28
 *
 *GP12A 331 朱鷹仁 40582
 *
 ******************************************************************************/
#include "save.h"
#include "wall.h"
#include "gimmick.h"
#include "player.h"
#include "number.h"
#include "playbar.h"
//*****************************************************************************
// グローバル変数
//*****************************************************************************
WALL		*g_pWall_Save = GetWallData();
GIMMICK		*g_pGimmick_Save = GetGimmickData();
PLAYER		*g_pPlayer_Save = GetPlayer();
ITEM_NUMBER	*g_pNumItem_Save = GetItemNum();
// セーブ処理
bool SaveData(int nMode)
{
	FILE *pFile;

	// SAVEMODE_WALL:
	switch(nMode)
	{
	case SCENE_01:
		if((pFile = fopen("data/DATABASE/SCENE_01/wallData_SCENE_01.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_02:
		if((pFile = fopen("data/DATABASE/SCENE_02/wallData_SCENE_02.csv", "w")) == NULL)
			return false;
		break;	
	case SCENE_03:
		if((pFile = fopen("data/DATABASE/SCENE_03/wallData_SCENE_03.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_04:
		if((pFile = fopen("data/DATABASE/SCENE_04/wallData_SCENE_04.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_05:
		if((pFile = fopen("data/DATABASE/SCENE_05/wallData_SCENE_05.csv", "w")) == NULL)
			return false;
		break;
	default:
		break;
	}
	fprintf(pFile,"pos.x,pos.y,pos.z,bUse,nType,aRemarks\n");
	for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if(g_pWall_Save[nCntWall].bUse == true)
		{
			fprintf(pFile,"%f,%f,%f,%d,%d,%s\n",
					g_pWall_Save[nCntWall].pos.x,
					g_pWall_Save[nCntWall].pos.y,
					g_pWall_Save[nCntWall].pos.z,
					g_pWall_Save[nCntWall].bUse,
					g_pWall_Save[nCntWall].nType,
					g_pWall_Save[nCntWall].aRemarks);
		}
	}
	fclose(pFile);

	// SAVEMODE_GIMMICK:
	switch(nMode)
	{
	case SCENE_01:
		if((pFile = fopen("data/DATABASE/SCENE_01/gimmickData_SCENE_01.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_02:
		if((pFile = fopen("data/DATABASE/SCENE_02/gimmickData_SCENE_02.csv", "w")) == NULL)
			return false;
		break;	
	case SCENE_03:
		if((pFile = fopen("data/DATABASE/SCENE_03/gimmickData_SCENE_03.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_04:
		if((pFile = fopen("data/DATABASE/SCENE_04/gimmickData_SCENE_04.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_05:
		if((pFile = fopen("data/DATABASE/SCENE_05/gimmickData_SCENE_05.csv", "w")) == NULL)
			return false;
		break;
	default:
		break;
	}
	fprintf(pFile,"pos.x,pos.y,pos.z,fTimeStart,fTimeEnd,fRadiusDis,fWidth,fHeight,fGoalX,fGoalY,bUse,nType,aRemarks\n");
	for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
	{
		if(g_pGimmick_Save[nCntGim].bUse == true)
		{
			fprintf(pFile,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%d,%s\n",
					g_pGimmick_Save[nCntGim].pos.x,
					g_pGimmick_Save[nCntGim].pos.y,
					g_pGimmick_Save[nCntGim].pos.z,
					g_pGimmick_Save[nCntGim].fTimeStart,
					g_pGimmick_Save[nCntGim].fTimeEnd,
					g_pGimmick_Save[nCntGim].fRadiusDis,			// 半径距离
					g_pGimmick_Save[nCntGim].fWidth,
					g_pGimmick_Save[nCntGim].fHeight,
					g_pGimmick_Save[nCntGim].fGoalX,
					g_pGimmick_Save[nCntGim].fGoalY,
					g_pGimmick_Save[nCntGim].bUse,
					g_pGimmick_Save[nCntGim].nType,
					g_pGimmick_Save[nCntGim].aRemarks);
		}
	}
	fclose(pFile);

	// SAVEMODE_PLAYER:
	switch(nMode)
	{
	case SCENE_01:
		if((pFile = fopen("data/DATABASE/SCENE_01/playerData_SCENE_01.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_02:
		if((pFile = fopen("data/DATABASE/SCENE_02/playerData_SCENE_02.csv", "w")) == NULL)
			return false;
		break;	
	case SCENE_03:
		if((pFile = fopen("data/DATABASE/SCENE_03/playerData_SCENE_03.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_04:
		if((pFile = fopen("data/DATABASE/SCENE_04/playerData_SCENE_04.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_05:
		if((pFile = fopen("data/DATABASE/SCENE_05/playerData_SCENE_05.csv", "w")) == NULL)
			return false;
		break;
	default:
		break;
	}
	fprintf(pFile,"pos.x,pos.y,pos.z,move.x,bUse,nType\n");
	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(g_pPlayer_Save[nCntPlayer].bUse)
		{
			fprintf(pFile,"%f,%f,%f,%f,%d,%d\n",
					g_pPlayer_Save[nCntPlayer].pos.x,
					g_pPlayer_Save[nCntPlayer].pos.y,
					g_pPlayer_Save[nCntPlayer].pos.z,
					abs(g_pPlayer_Save[nCntPlayer].move.x),
					g_pPlayer_Save[nCntPlayer].bUse,
					g_pPlayer_Save[nCntPlayer].nType);
		}
	}
	fclose(pFile);


	// SAVEMODE_ITEMNUM:
	switch(nMode)
	{
	case SCENE_01:
		if((pFile = fopen("data/DATABASE/SCENE_01/itemData_SCENE_01.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_02:
		if((pFile = fopen("data/DATABASE/SCENE_02/itemData_SCENE_02.csv", "w")) == NULL)
			return false;
		break;	
	case SCENE_03:
		if((pFile = fopen("data/DATABASE/SCENE_03/itemData_SCENE_03.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_04:
		if((pFile = fopen("data/DATABASE/SCENE_04/itemData_SCENE_04.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_05:
		if((pFile = fopen("data/DATABASE/SCENE_05/itemData_SCENE_05.csv", "w")) == NULL)
			return false;
		break;
	default:
		break;
	}
	fprintf(pFile,"itemNumber\n");
	fprintf(pFile,"%d\n", g_pNumItem_Save->nItemLockTime);

	fclose(pFile);

	// SAVEMODE_PLAYBARNUM:
	switch(nMode)
	{
	case SCENE_01:
		if((pFile = fopen("data/DATABASE/SCENE_01/playbar_SCENE_01.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_02:
		if((pFile = fopen("data/DATABASE/SCENE_02/playbar_SCENE_02.csv", "w")) == NULL)
			return false;
		break;	
	case SCENE_03:
		if((pFile = fopen("data/DATABASE/SCENE_03/playbar_SCENE_03.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_04:
		if((pFile = fopen("data/DATABASE/SCENE_04/playbar_SCENE_04.csv", "w")) == NULL)
			return false;
		break;
	case SCENE_05:
		if((pFile = fopen("data/DATABASE/SCENE_05/playbar_SCENE_05.csv", "w")) == NULL)
			return false;
		break;
	default:
		break;
	}
	fprintf(pFile,"playbarNumber\n");
	fprintf(pFile,"%d\n", *PlaybarNum());

	fclose(pFile);

	return true;
}