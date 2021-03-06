﻿/*****************************************************************************
 *
 * ゲームマネジメント処理 [gameManager.h]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//////////////////////
// 贴图的种类名称
//////////////////////


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGameManager(void);
void UninitGameManager(void);
void UpdateGameManager(void);
void DrawGameManager(void);
bool *GetIsPlay(void);
bool *GetIsStop(void);
bool *GetIsPause(void);
int *GetGameMode(void);
#endif
