/*****************************************************************************
 *
 * 時間軌跡の表示処理 [timeTrack.h]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#ifndef _TIMETRACK_H_
#define _TIMETRACK_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TIMETRACK		(5000)				// 最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3				pos;				// 位置
	D3DXVECTOR3				rot;				// モデルの回転
	
	float					fWidth;				// 幅
	float					fHeight;			// 高さ
	float					fSpeed;				// すばやさ
	float					fScl;				// 大きさ

	bool					bUse;				// 使用しているかどうか

	int						nCounterAnim;		// 计数器
	int						nPatternAnim;		// 记录图片帧
	int						nType;				// 类型
	int						nAlpha;				// 透明度

}TIMETRACK;

//////////////////////
// 贴图的种类名称
//////////////////////
enum
{
	TIMETRACK_R = 0,						// タイプ０
	TIMETRACK_G,
	TIMETRACK_B,
	TIMETRACK_GRAY,
	TIMETRACK_TYPE_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTimeTrack(void);
void UninitTimeTrack(void);
void UpdateTimeTrack(void);
void DrawTimeTrack(void);
void SetTimeTrack(D3DXVECTOR3 pos, D3DXCOLOR col, int type);
void SetupTimeTrack(void);
bool *GetCleanupSwitch(void);
TIMETRACK *GetTimeTrack(void);
#endif
