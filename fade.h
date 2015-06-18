//=============================================================================
//
// フェード処理 [fade.h]
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

//*************************************
// フェードの状態
//*************************************
typedef enum
{
	FADE_NONE = 0,		// 何もない状態
	FADE_IN,			// フェードイン処理
	FADE_OUT,			// フェードアウト処理
	BLACK_IN,
	BLACK_OUT,
	FADE_MAX
} FADE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitFade(void);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

void SetFade(FADE out, FADE in, GAMEMODE next);
FADE GetFade(void);

#endif
