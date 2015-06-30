/*****************************************************************************
 *
 * ギミックの表示処理 [gimmick.h]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#ifndef _GIMMICK_H_
#define _GIMMICK_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_GIMMICK					(10)		// 最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3		pos;				// 位置
	D3DXVECTOR3		posSave;			// 位置を保存する
	D3DXVECTOR3		rot;				// モデルの回転
	D3DXVECTOR3		speed;				// すばやさ
	D3DXVECTOR3		speedSave;			// すばやさを保存する
	D3DXVECTOR3		track[1200];		// 足跡

	float			fWidth;				// 幅
	float			fHeight;			// 高さ
	float			fTimeStart;			// 始まる時間 用于初始化路径和行动路线
	float			fTimeEnd;			// 終わる時間 用于初始化路径和行动路线
	float			fLoopCount;			// ループの回数計算
	float			fGoalX;				// 目的地X
	float			fGoalY;				// 目的地X
	float			fDegree;			// 角度
	float			fDegreeSpeed;		// 角度変換速度
	float			fMaxX;				// X軸最大幅
	float			fMinX;				// X軸最小幅
	float			fMaxY;				// Y軸最大幅
	float			fMinY;				// Y軸最小幅
	float			fRadius;			// あたり判定の半径
	float			fRadiusDis;			// 動く半径

	bool			bCurrentPri01;		// 時間帯スイッチ
	bool			bCurrentPri02;		// 時間帯スイッチ
	bool			bCurrentPri03;		// 時間帯スイッチ
	bool			bCurrentPri04;		// 時間帯スイッチ(時間帯逆転)
	bool			bCurrentPri05;		// 時間帯スイッチ(時間帯逆転)
	bool			bCurrentPri06;		// 時間帯スイッチ(時間帯逆転)

	bool			bUse;				// 使用しているかどうか
	bool			bTimeChange;		// 時間帯交換
	bool			bIsLock;			// 時間帯固定

	char			aRemarks[256];		// コメント

	int				nType;				// 種類
	int				nMode;				// ギミックの種類
	int				nLastTrack;			// 最後の動作
	int				nPriorityNum01;		// 時間軸順番1
	int				nPriorityNum02;		// 時間軸順番2
	int				nPriorityNum03;		// 時間軸順番3
	int				nNextPriNum;		// 次の時間軸の時間帯
}GIMMICK;

//////////////////////
// テクスチャの種類
//////////////////////
enum
{
	GIMMICK_ATTACK_001 = 0,
	GIMMICK_ATTACK_002,
	GIMMICK_HELP_003,
	GIMMICK_HELP_004,
	GIMMICK_HELP_005,
	GIMMICK_HELP_006,
	GIMMICK_TYPE_MAX
};
// ギミックの種類
enum
{
	MODE_ATTACK = 0,
	MODE_HELP,
	MODE_TYPE_MAX
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitGimmick(void);
void UninitGimmick(void);
void UpdateGimmick(void);
void DrawGimmick(void);
int SetGimmick(D3DXVECTOR3 pos, D3DXCOLOR col, int type, char aRemarks[256]);
void GimmickCreateMode(void);
void GimmickMode(int nCntRoopTime, int nCntGim, int nType);
void TrackData(int nCntGim);
void UpDataPriority(void);
bool IsGimmickHitWall(int nCntGim, float fMoveX, float fMoveY);
GIMMICK *GetGimmickData(void);
bool IsGimmickHitPlayer(int nCntGim);
void SetupGimmickTrack(int nCntGim);
void InitDataGimmick(void);
void DeleteGimmick(void);
bool IsGimmickLock(int* pCntGim, bool* pIsLock, int* pItemNum);
#endif
