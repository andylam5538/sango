/*****************************************************************************
 *
 * ギミックの表示処理 [gimmick.cpp]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#include "gimmick.h"
#include "input.h"
#include "load.h"
#include "save.h"
#include "debugproc.h"
#include "playbar.h"
#include "timeCounter.h"
#include "wall.h"
#include "player.h"
#include "timeTrack.h"
#include "gameManager.h"
#include "number.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
////////////////////////////
// ギミックのモード
////////////////////////////
// Type001
#define	GIMMICK_01_SIZE_X		(50)									// ギミックの幅
#define	GIMMICK_01_SIZE_Y		(50)									// ギミックの高さ
#define GIMMICK_01_RADIUS		(GIMMICK_01_SIZE_X / 4)					// ギミックのあたり判定半径
#define GIMMICK_01_RADIUSDIS	(50)									// ギミックの動く半径
#define	GIMMICK_01_RUN_TIME		(15.0f)									// ギミックの運動時間
#define GIMMICK_01_TIME_START	(0.0f)									// ギミック始まる時間
#define GIMMICK_01_TIME_END		(GIMMICK_01_RUN_TIME)					// ギミック終わる時間
#define GIMMICK_01_DEGREE_MIN	(180.0f)								// ギミック最小角度
#define GIMMICK_01_DEGREE_MAX	(360.0f)								// ギミック最大角度
// Type002
#define	GIMMICK_02_SIZE_X		(50)									// ギミックの幅
#define	GIMMICK_02_SIZE_Y		(50)									// ギミックの高さ
#define GIMMICK_02_RADIUS		(GIMMICK_02_SIZE_X / 4)					// ギミックのあたり判定半径
#define GIMMICK_02_RADIUSDIS	(50)									// ギミックの動く半径
#define	GIMMICK_02_RUN_TIME		(15.0f)									// ギミックの運動時間
#define GIMMICK_02_TIME_START	(0.0f)									// ギミック始まる時間
#define GIMMICK_02_TIME_END		(GIMMICK_02_RUN_TIME)					// ギミック終わる時間
#define GIMMICK_02_DEGREE_MIN	(180.0f)								// ギミック最小角度
#define GIMMICK_02_DEGREE_MAX	(360.0f)								// ギミック最大角度

// Type003
#define	GIMMICK_03_SIZE_X		(200.0f)								// ギミックの幅
#define	GIMMICK_03_SIZE_Y		(300.0f)								// ギミックの高さ
#define	GIMMICK_03_RUN_TIME		(5.0f)									// ギミックの運動時間
#define GIMMICK_03_TIME_START	(0.0f)									// ギミック始まる時間
#define GIMMICK_03_TIME_END		(GIMMICK_03_TIME_START + GIMMICK_03_RUN_TIME)					// ギミック終わる時間
#define GIMMICK_03_LOOP_TIME	(60.0f * GIMMICK_03_RUN_TIME)			// ループ時間
#define	GIMMICK_03_GOAL_Y		(700.0f)								// ギミックの目標１位置(X座標)

// Type004
#define	GIMMICK_04_SIZE_X		(200.0f)								// ギミックの幅
#define	GIMMICK_04_SIZE_Y		(300.0f)								// ギミックの高さ
#define	GIMMICK_04_RUN_TIME		(5.0f)									// ギミックの運動時間
#define GIMMICK_04_TIME_START	(0.0f)									// ギミック始まる時間
#define GIMMICK_04_TIME_END		(GIMMICK_04_TIME_START + GIMMICK_04_RUN_TIME)					// ギミック終わる時間
#define GIMMICK_04_LOOP_TIME	(60.0f * GIMMICK_04_RUN_TIME)			// ループ時間
#define	GIMMICK_04_GOAL_Y		(200.0f)								// ギミックの目標１位置(X座標)

// Type005
#define	GIMMICK_05_SIZE_X		(200.0f)								// ギミックの幅
#define	GIMMICK_05_SIZE_Y		(100.0f)								// ギミックの高さ
#define	GIMMICK_05_RUN_TIME		(5.0f)									// ギミックの運動時間
#define GIMMICK_05_TIME_START	(0.0f)									// ギミック始まる時間
#define GIMMICK_05_TIME_END		(GIMMICK_05_TIME_START + GIMMICK_05_RUN_TIME)					// ギミック終わる時間
#define GIMMICK_05_LOOP_TIME	(60.0f * GIMMICK_05_RUN_TIME)			// ループ時間
#define	GIMMICK_05_GOAL_X		(SCREEN_WIDTH)								// ギミックの目標１位置(X座標)

// Type006
#define	GIMMICK_06_SIZE_X		(200.0f)								// ギミックの幅
#define	GIMMICK_06_SIZE_Y		(100.0f)								// ギミックの高さ
#define	GIMMICK_06_RUN_TIME		(5.0f)									// ギミックの運動時間
#define GIMMICK_06_TIME_START	(0.0f)									// ギミック始まる時間
#define GIMMICK_06_TIME_END		(GIMMICK_06_TIME_START + GIMMICK_06_RUN_TIME)					// ギミック終わる時間
#define GIMMICK_06_LOOP_TIME	(60.0f * GIMMICK_06_RUN_TIME)			// ループ時間
#define	GIMMICK_06_GOAL_X		(0.0f)									// ギミックの目標１位置(X座標)

#define GIMMICK_MOVEDELAY_TIME	(15)									// 移動の遅延時間

////////////////////////////
// 1列6行图组动画
////////////////////////////
//#define	TEX_PATTERN_DIVIDE_X1		(1)												// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
//#define	TEX_PATTERN_DIVIDE_Y6		(6)												// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)
//#define	NUM_ANIM_PATTERN_X1_Y6		(TEX_PATTERN_DIVIDE_X1 * TEX_PATTERN_DIVIDE_Y6)	// アニメーションのパターン数




////////////////////////////
// 动画播放帧数
////////////////////////////
//#define	TIME_CHANGE_PATTERN_MAGIC		(9)			// アニメーションの切り替わるタイミング

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexGimmick(LPDIRECT3DDEVICE9 pDevice);
void SetColorGimmick(int nIdxGimmick, D3DXCOLOR col);

void SetVertexGimmick(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组polygon地址
						float scaleX,					// テクスチャのX軸のスケール		X轴的比例参数
						float scaleY);					// テクスチャのY軸のスケール		Y轴的比例参数

void SetTextureGimmick(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组地址
						int nPatternAnim,				// 今のテクスチャの番号				记录当前显示帧
						int numAnimPattern, 			// テクスチャ全部の個数				记录当前对应的组成图组的图片数量
						int texPatternDivideX,			// 横のテクスチャの個数				记录当前图组X轴图片数量
						int texPatternDivideY );		// 縦のテクスチャの個数				记录当前图组Y轴图片数量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTextureGimmick[MAX_GIMMICK] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffGimmick = NULL;				// 頂点バッファインターフェースへのポインタ

int						g_nCreateGimmickNum;						// 制作した番号
int						g_nBeInControl;								// 選択したギミック
int						g_nMenuSelect;								// 選択したメニュー
int						g_nMoveDelay_GIM;							// 移動の遅延
bool					g_bIsGimmickCreate;							// 制作かどうか

GIMMICK					g_aGimmick[MAX_GIMMICK];					// 木ワーク
WALL					*g_aWallDataGim = GetWallData();			// 壁のデータゲット
PLAYBAR					*g_aPlaybarGim = GetPlaybar();				// 時間軸のデータゲット
PLAYER					*g_getPlayer = GetPlayer();					// プレーヤーのデータゲット
TIMECOUNTER				*g_timeCountGim;							// 時間計算
TIMETRACK				*g_pTimeTrackGim = GetTimeTrack();
//////////////////////
// 贴图的种类路径
//////////////////////
const char *c_pTextureGimmick[GIMMICK_TYPE_MAX]=		// 不同模式下的背景切换
{
	"data/TEXTURE/gimmicks/gimmick.png",				// ギミック1
	"data/TEXTURE/gimmicks/gimmick.png",				// ギミック2
	"data/TEXTURE/gimmicks/gimmick_def_100_100.png",	// ギミック3
	"data/TEXTURE/gimmicks/gimmick_def_100_100.png",	// ギミック4
	"data/TEXTURE/gimmicks/gimmick_def_100_100.png",	// ギミック5
	"data/TEXTURE/gimmicks/gimmick_def_100_100.png",	// ギミック6
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGimmick(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexGimmick(pDevice);

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < GIMMICK_TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(	pDevice,									// デバイスへのポインタ
									c_pTextureGimmick[nCntTex],					// ファイルの名前
									&g_apD3DTextureGimmick[nCntTex]);			// 読み込むメモリー
	}

	for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
	{
		g_aGimmick[nCntGim].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGimmick[nCntGim].posSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGimmick[nCntGim].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGimmick[nCntGim].speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGimmick[nCntGim].speedSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGimmick[nCntGim].fWidth = 0;
		g_aGimmick[nCntGim].fHeight = 0;
		g_aGimmick[nCntGim].fTimeStart = 0;
		g_aGimmick[nCntGim].fTimeEnd = 0;
		g_aGimmick[nCntGim].fGoalX = -1;
		g_aGimmick[nCntGim].fGoalY = -1;
		g_aGimmick[nCntGim].fDegree = 0;
		g_aGimmick[nCntGim].fDegreeSpeed = 0;
		g_aGimmick[nCntGim].fMaxX = 0;
		g_aGimmick[nCntGim].fMinX = 0;
		g_aGimmick[nCntGim].fMaxY = 0;
		g_aGimmick[nCntGim].fMinY = 0;
		g_aGimmick[nCntGim].fRadius = 0;
		
		g_aGimmick[nCntGim].bUse = false;
		g_aGimmick[nCntGim].bCurrentPri01 = false;
		g_aGimmick[nCntGim].bCurrentPri02 = false;
		g_aGimmick[nCntGim].bCurrentPri03 = false;
		g_aGimmick[nCntGim].bCurrentPri04 = false;
		g_aGimmick[nCntGim].bCurrentPri05 = false;
		g_aGimmick[nCntGim].bCurrentPri05 = false;
		g_aGimmick[nCntGim].bTimeChange = false;
		g_aGimmick[nCntGim].bIsLock = false;

		g_aGimmick[nCntGim].nType = -1;
		g_aGimmick[nCntGim].nLastTrack = 0;
		g_aGimmick[nCntGim].fLoopCount = 0;
		g_aGimmick[nCntGim].nPriorityNum01 = -1;
		g_aGimmick[nCntGim].nPriorityNum02 = -1;
		g_aGimmick[nCntGim].nPriorityNum03 = -1;
		g_aGimmick[nCntGim].nNextPriNum = -1;
	}

	g_bIsGimmickCreate = true;
	g_nCreateGimmickNum = -1;
	g_nBeInControl = -1;
	g_nMenuSelect = -1;
	g_nMoveDelay_GIM = 0;

	g_timeCountGim = GetTimeCounter();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitGimmick(void)
{
	for(int nCntTex = 0; nCntTex < MAX_GIMMICK; nCntTex++)
	{
		if(g_apD3DTextureGimmick[nCntTex] != NULL)
		{// テクスチャの開放
			g_apD3DTextureGimmick[nCntTex]->Release();
			g_apD3DTextureGimmick[nCntTex] = NULL;
		}
	}

	if(g_pD3DVtxBuffGimmick != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffGimmick->Release();
		g_pD3DVtxBuffGimmick = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGimmick(void)
{
	// 每次更新都初始化轨迹线的使用
	for(int nCntTrack = 0; nCntTrack < MAX_TIMETRACK; nCntTrack++)
	{
		if(g_pTimeTrackGim[nCntTrack].bUse)
		{
			g_pTimeTrackGim[nCntTrack].bUse = false;
		}
	}

	// 時間帯の優先順位を入力
	UpDataPriority();

	// 編集モードは更新しない
	if(*GetGameMode() != SELECTMODE_GIMMICKEDIT)
	{
		if(*GetIsStop() == true)
		{
			for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
			{
				if(g_aGimmick[nCntGim].bUse == true)
				{
					// 位置回到原点 时间停止后初始化位置
					g_aGimmick[nCntGim].bCurrentPri01 = false;
					g_aGimmick[nCntGim].bCurrentPri02 = false;
					g_aGimmick[nCntGim].bCurrentPri03 = false;
					g_aGimmick[nCntGim].bCurrentPri04 = false;
					g_aGimmick[nCntGim].bCurrentPri05 = false;
					g_aGimmick[nCntGim].bCurrentPri06 = false;

					int nTemp = 0;

					if(!g_aGimmick[nCntGim].bIsLock)
					{
						nTemp = (int)(g_aPlaybarGim[g_aGimmick[nCntGim].nPriorityNum01].fTimeStart * UPDATA_FRAME);
						//if(nTemp != 0)
						//	nTemp -= 1;
						g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
					}
					else
					{
						g_aGimmick[nCntGim].fLoopCount = 0;
						g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[0];
					}
				}
			}
		}

		if(*GetIsPlay() == true)
		{
			// 确认前后时间点
			// 隣の時間点を確認する
			for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
			{
				if(g_aGimmick[nCntGim].bUse == true)
				{
					// 固定しない場合
					if(!g_aGimmick[nCntGim].bIsLock)
					{
						// 根据实际时间切换播放顺序 PBNum->Playbar Number
						// 実行順番を確認
						int nPBNum = 0;
						if(g_timeCountGim->fCurrentTime >= 0 && g_timeCountGim->fCurrentTime <= 5)
						{
							nPBNum = g_aGimmick[nCntGim].nPriorityNum01;
							// 次の時間軸の時間帯
							g_aGimmick[nCntGim].nNextPriNum = g_aGimmick[nCntGim].nPriorityNum02;
						}
						if(g_timeCountGim->fCurrentTime > 5 && g_timeCountGim->fCurrentTime <= 10)
						{
							nPBNum = g_aGimmick[nCntGim].nPriorityNum02;
							// 次の時間軸の時間帯
							g_aGimmick[nCntGim].nNextPriNum = g_aGimmick[nCntGim].nPriorityNum03;
						}
						if(g_timeCountGim->fCurrentTime > 10 && g_timeCountGim->fCurrentTime <= 15)
						{
							nPBNum = g_aGimmick[nCntGim].nPriorityNum03;
							// 次の時間軸の時間帯
							g_aGimmick[nCntGim].nNextPriNum = -1;
						}

						// 如果该时间条为发生反转
						if(!g_aPlaybarGim[nPBNum].bIsInvert)
						{
							// 根据时间条的优先级别确定起始时间点
							switch ((int)g_aPlaybarGim[nPBNum].fTimeStart)
							{
							case 0:
								if(!g_aGimmick[nCntGim].bCurrentPri01)
								{
									g_aGimmick[nCntGim].fLoopCount = 0;
									g_timeCountGim->fPlaybarTime = 0;
									g_aGimmick[nCntGim].bTimeChange = false;
								}
								g_aGimmick[nCntGim].bCurrentPri01 = true;
								g_aGimmick[nCntGim].bCurrentPri02 = false;
								g_aGimmick[nCntGim].bCurrentPri03 = false;
								g_aGimmick[nCntGim].bCurrentPri04 = false;
								g_aGimmick[nCntGim].bCurrentPri05 = false;
								g_aGimmick[nCntGim].bCurrentPri06 = false;
				
								break;
							case 5:	
								if(!g_aGimmick[nCntGim].bCurrentPri02)
								{
									g_aGimmick[nCntGim].fLoopCount = 0;
									g_timeCountGim->fPlaybarTime = 5;
									g_aGimmick[nCntGim].bTimeChange = false;
								}
								g_aGimmick[nCntGim].bCurrentPri01 = false;
								g_aGimmick[nCntGim].bCurrentPri02 = true;
								g_aGimmick[nCntGim].bCurrentPri03 = false;
								g_aGimmick[nCntGim].bCurrentPri04 = false;
								g_aGimmick[nCntGim].bCurrentPri05 = false;
								g_aGimmick[nCntGim].bCurrentPri06 = false;
								break;
							case 10:
								if(!g_aGimmick[nCntGim].bCurrentPri03)
								{
									g_aGimmick[nCntGim].fLoopCount = 0;
									g_timeCountGim->fPlaybarTime = 10;
									g_aGimmick[nCntGim].bTimeChange = false;
								}
								g_aGimmick[nCntGim].bCurrentPri01 = false;
								g_aGimmick[nCntGim].bCurrentPri02 = false;
								g_aGimmick[nCntGim].bCurrentPri03 = true;
								g_aGimmick[nCntGim].bCurrentPri04 = false;
								g_aGimmick[nCntGim].bCurrentPri05 = false;
								g_aGimmick[nCntGim].bCurrentPri06 = false;
								break;
							}
						}
						else
						{
							// 根据时间条的优先级别确定起始时间点
							switch ((int)g_aPlaybarGim[nPBNum].fTimeStart)
							{
							case 5:// 第一条逆转
								if(!g_aGimmick[nCntGim].bCurrentPri04)
								{
									g_aGimmick[nCntGim].fLoopCount = 0;
									g_timeCountGim->fPlaybarTime = 0;
									g_aGimmick[nCntGim].bTimeChange = false;
								}
								g_aGimmick[nCntGim].bCurrentPri01 = false;
								g_aGimmick[nCntGim].bCurrentPri02 = false;
								g_aGimmick[nCntGim].bCurrentPri03 = false;
								g_aGimmick[nCntGim].bCurrentPri04 = true;
								g_aGimmick[nCntGim].bCurrentPri05 = false;
								g_aGimmick[nCntGim].bCurrentPri06 = false;
				
								break;
							case 10:// 第二条逆转
								if(!g_aGimmick[nCntGim].bCurrentPri05)
								{
									g_aGimmick[nCntGim].fLoopCount = 0;
									g_timeCountGim->fPlaybarTime = 5;
									g_aGimmick[nCntGim].bTimeChange = false;
								}
								g_aGimmick[nCntGim].bCurrentPri01 = false;
								g_aGimmick[nCntGim].bCurrentPri02 = false;
								g_aGimmick[nCntGim].bCurrentPri03 = false;
								g_aGimmick[nCntGim].bCurrentPri04 = false;
								g_aGimmick[nCntGim].bCurrentPri05 = true;
								g_aGimmick[nCntGim].bCurrentPri06 = false;
								break;
							case 15:// 第三条逆转
								if(!g_aGimmick[nCntGim].bCurrentPri06)
								{
									g_aGimmick[nCntGim].fLoopCount = 0;
									g_timeCountGim->fPlaybarTime = 10;
									g_aGimmick[nCntGim].bTimeChange = false;
								}
								g_aGimmick[nCntGim].bCurrentPri01 = false;
								g_aGimmick[nCntGim].bCurrentPri02 = false;
								g_aGimmick[nCntGim].bCurrentPri03 = false;
								g_aGimmick[nCntGim].bCurrentPri04 = false;
								g_aGimmick[nCntGim].bCurrentPri05 = false;
								g_aGimmick[nCntGim].bCurrentPri06 = true;
								break;
							}
						}

						if(!*GetIsPause())
						{
							// 根据再生时间来播放
							// 再生時間順番でギミックを移動する
							if(g_aGimmick[nCntGim].bCurrentPri01 == true)
							{
								// ギミックのポジション再生
								// 当前时间 + 跨越（跳跃）的时间段长度 < 当前场景最长时间 更新机关位置
								if(g_timeCountGim->fPlaybarTime > TIME_0 && g_timeCountGim->fPlaybarTime < TIME_5)
								{
									if(g_aGimmick[nCntGim].nNextPriNum != -1)
									{
										// 次の時間軸の時間帯 時間方向の判断
										float fNextPriStart = g_aPlaybarGim[g_aGimmick[nCntGim].nNextPriNum].fTimeStart;
										float fNextPriEnd = g_aPlaybarGim[g_aGimmick[nCntGim].nNextPriNum].fTimeEnd;
										// 走完自身时间条需要的消耗 加上 该时间条的终点到下一个时间条的起点的消耗
										float fLoopSpeed = (TIME_5 * UPDATA_FRAME + abs(TIME_5 - fNextPriStart) * UPDATA_FRAME) / 300;

										// 正引き
										if(g_timeCountGim->fPlaybarTime <= fNextPriEnd)
										{
											int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_0 * UPDATA_FRAME);
											if(nTemp > fNextPriStart * UPDATA_FRAME)
												nTemp = (int)(fNextPriStart * UPDATA_FRAME);
											g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
											g_aGimmick[nCntGim].fLoopCount += fLoopSpeed;
											// 時間軌跡の表示
											SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
										}
										// 逆引き
										else if(g_timeCountGim->fPlaybarTime >= fNextPriEnd)
										{
											if(g_aGimmick[nCntGim].fLoopCount < TIME_5 * UPDATA_FRAME && !g_aGimmick[nCntGim].bTimeChange)
											{
												int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_0 * UPDATA_FRAME);
												if(nTemp > TIME_5 * UPDATA_FRAME)
													nTemp = (int)(TIME_5 * UPDATA_FRAME);
												g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
												g_aGimmick[nCntGim].fLoopCount += fLoopSpeed;
												// 時間軌跡の表示
												SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
											}
											else
												g_aGimmick[nCntGim].bTimeChange = true;

											if(g_aGimmick[nCntGim].bTimeChange == true)
											{
												int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_0 * UPDATA_FRAME);
												if(nTemp < fNextPriStart * UPDATA_FRAME)
													nTemp = (int)(fNextPriStart * UPDATA_FRAME);
												g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
												g_aGimmick[nCntGim].fLoopCount -= fLoopSpeed;
												// 時間軌跡の表示
												SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_GRAY);
											}
										}
									}
									// 次の時間帯はない
									else
									{
										int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_0 * UPDATA_FRAME);
										if(nTemp > TIME_5 * UPDATA_FRAME)
												nTemp = (int)(TIME_5 * UPDATA_FRAME);
										g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
										g_aGimmick[nCntGim].fLoopCount ++;
										// 時間軌跡の表示
										SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
									}
								}
							}
							if(g_aGimmick[nCntGim].bCurrentPri02 == true)
							{
								if(g_timeCountGim->fPlaybarTime > TIME_5 && g_timeCountGim->fPlaybarTime < TIME_10)
								{
									if(g_aGimmick[nCntGim].nNextPriNum != -1)
									{
										// 次の時間軸の時間帯
										float fNextPriStart = g_aPlaybarGim[g_aGimmick[nCntGim].nNextPriNum].fTimeStart;
										float fNextPriEnd = g_aPlaybarGim[g_aGimmick[nCntGim].nNextPriNum].fTimeEnd;
										// 走完自身时间条需要的消耗 加上 该时间条的终点到下一个时间条的起点的消耗
										float fLoopSpeed = (TIME_5 * UPDATA_FRAME + abs(TIME_10 - fNextPriStart) * UPDATA_FRAME) / 300;

										// 正引き
										if(g_timeCountGim->fPlaybarTime <= fNextPriEnd)
										{
											int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_5 * UPDATA_FRAME);
											if(nTemp > fNextPriStart * UPDATA_FRAME)
												nTemp = (int)(fNextPriStart * UPDATA_FRAME);
											g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
											g_aGimmick[nCntGim].fLoopCount += fLoopSpeed;
											// 時間軌跡の表示
											SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_G);
										}
										// 逆引き
										else if(g_timeCountGim->fPlaybarTime >= fNextPriEnd)
										{
											if(g_aGimmick[nCntGim].fLoopCount < TIME_5 * UPDATA_FRAME && !g_aGimmick[nCntGim].bTimeChange)
											{
												int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_5 * UPDATA_FRAME);
												if(nTemp > TIME_10 * UPDATA_FRAME)
													nTemp = (int)(TIME_10 * UPDATA_FRAME);
												g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
												g_aGimmick[nCntGim].fLoopCount += fLoopSpeed;
												// 時間軌跡の表示
												SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_G);
											}
											else
												g_aGimmick[nCntGim].bTimeChange = true;

											if(g_aGimmick[nCntGim].bTimeChange == true)
											{
												int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_5 * UPDATA_FRAME);
												if(nTemp < fNextPriStart * UPDATA_FRAME)
													nTemp = (int)(fNextPriStart * UPDATA_FRAME);
												g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
												g_aGimmick[nCntGim].fLoopCount -= fLoopSpeed;
												// 時間軌跡の表示
												SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_GRAY);
											}
										}
									}
									else
									{
										int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_5 * UPDATA_FRAME);
										if(nTemp > TIME_10 * UPDATA_FRAME)
												nTemp = (int)(TIME_10 * UPDATA_FRAME);
										g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
										g_aGimmick[nCntGim].fLoopCount ++;
										// 時間軌跡の表示
										SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_G);
									}
								}
							}
							if(g_aGimmick[nCntGim].bCurrentPri03 == true)
							{
								if(g_timeCountGim->fPlaybarTime > TIME_10 && g_timeCountGim->fPlaybarTime < TIME_15)
								{
									if(g_aGimmick[nCntGim].nNextPriNum != -1)
									{
										// 次の時間軸の時間帯
										float fNextPriStart = g_aPlaybarGim[g_aGimmick[nCntGim].nNextPriNum].fTimeStart;
										float fNextPriEnd = g_aPlaybarGim[g_aGimmick[nCntGim].nNextPriNum].fTimeEnd;
										// 走完自身时间条需要的消耗 加上 该时间条的终点到下一个时间条的起点的消耗
										float fLoopSpeed = (TIME_5 * UPDATA_FRAME + abs(TIME_15 - fNextPriStart) * UPDATA_FRAME) / 300;

										// 正引き
										if(g_timeCountGim->fPlaybarTime <= fNextPriEnd)
										{
											int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_10 * UPDATA_FRAME);
											if(nTemp > fNextPriStart * UPDATA_FRAME)
												nTemp = (int)(fNextPriStart * UPDATA_FRAME);
											g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
											g_aGimmick[nCntGim].fLoopCount += fLoopSpeed;
											// 時間軌跡の表示
											SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_B);
										}
										// 逆引き
										else if(g_timeCountGim->fPlaybarTime >= fNextPriEnd)
										{
											if(g_aGimmick[nCntGim].fLoopCount < TIME_5 * UPDATA_FRAME && !g_aGimmick[nCntGim].bTimeChange)
											{
												int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_10 * UPDATA_FRAME);
												if(nTemp >= TIME_15 * UPDATA_FRAME)
													nTemp = (int)(TIME_15 * UPDATA_FRAME);

												g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
												g_aGimmick[nCntGim].fLoopCount += fLoopSpeed;
												// 時間軌跡の表示
												SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_B);
											}
											else
												g_aGimmick[nCntGim].bTimeChange = true;

											if(g_aGimmick[nCntGim].bTimeChange == true)
											{
												int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_10 * UPDATA_FRAME);
												if(nTemp < fNextPriStart * UPDATA_FRAME)
													nTemp = (int)(fNextPriStart * UPDATA_FRAME);
												g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
												g_aGimmick[nCntGim].fLoopCount -= fLoopSpeed;
												// 時間軌跡の表示
												SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_GRAY);
											}
										}
									}
									else
									{
										int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_10 * UPDATA_FRAME);
										if(nTemp > TIME_15 * UPDATA_FRAME)
											nTemp = (int)(TIME_15 * UPDATA_FRAME);
										g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
										g_aGimmick[nCntGim].fLoopCount ++;
										// 時間軌跡の表示
										SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_B);
									}
								}
							}

							if(g_aGimmick[nCntGim].bCurrentPri04 == true)
							{
								// ギミックのポジション再生
								// 当前时间 + 跨越（跳跃）的时间段长度 < 当前场景最长时间 更新机关位置
								// 如果不从0秒开始动就无法退回到最开始的0帧位置
								if(g_timeCountGim->fPlaybarTime >= TIME_0 && g_timeCountGim->fPlaybarTime < TIME_5)
								{
									if(g_aGimmick[nCntGim].nNextPriNum != -1)
									{
										// 次の時間軸の時間帯 時間方向の判断
										float fNextPriStart = g_aPlaybarGim[g_aGimmick[nCntGim].nNextPriNum].fTimeStart;
										float fNextPriEnd = g_aPlaybarGim[g_aGimmick[nCntGim].nNextPriNum].fTimeEnd;
										// 走完自身时间条需要的消耗 加上 该时间条的终点到下一个时间条的起点的消耗
										float fLoopSpeed = (TIME_5 * UPDATA_FRAME + abs(TIME_0 - fNextPriStart) * UPDATA_FRAME) / 300;

										// 正引き
										if(g_timeCountGim->fPlaybarTime >= fNextPriEnd)
										{
											int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_5 * UPDATA_FRAME);
											if(nTemp < fNextPriStart * UPDATA_FRAME)
												nTemp = (int)(fNextPriStart * UPDATA_FRAME);
											g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
											g_aGimmick[nCntGim].fLoopCount -= fLoopSpeed;
											// 時間軌跡の表示
											SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
										}
										// 逆引き 1-2-1
										else if(g_timeCountGim->fPlaybarTime <= fNextPriEnd)
										{
											if(g_aGimmick[nCntGim].fLoopCount > (-TIME_5 * UPDATA_FRAME) && !g_aGimmick[nCntGim].bTimeChange)
											{
												int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_5 * UPDATA_FRAME);
												if(nTemp < TIME_0 * UPDATA_FRAME)
													nTemp = (int)(TIME_0 * UPDATA_FRAME);
												g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
												g_aGimmick[nCntGim].fLoopCount -= fLoopSpeed;
												// 時間軌跡の表示
												SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
											}
											else
											{
												if(!g_aGimmick[nCntGim].bTimeChange)
													g_aGimmick[nCntGim].fLoopCount = TIME_0 * UPDATA_FRAME;
												g_aGimmick[nCntGim].bTimeChange = true;
											}

											if(g_aGimmick[nCntGim].bTimeChange == true)
											{
												int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_0 * UPDATA_FRAME);
												if(nTemp > fNextPriStart * UPDATA_FRAME)
													nTemp = (int)(fNextPriStart * UPDATA_FRAME);
												g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
												g_aGimmick[nCntGim].fLoopCount += fLoopSpeed;
												// 時間軌跡の表示
												SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_GRAY);
											}
										}
									}
									// 次の時間帯はない
									else
									{
										int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_5 * UPDATA_FRAME);
										if(nTemp < TIME_0 * UPDATA_FRAME)
											nTemp = (int)(TIME_0 * UPDATA_FRAME);
										g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
										g_aGimmick[nCntGim].fLoopCount --;
										// 時間軌跡の表示
										SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
									}
								}
							}

							if(g_aGimmick[nCntGim].bCurrentPri05 == true)
							{
								// ギミックのポジション再生
								// 当前时间 + 跨越（跳跃）的时间段长度 < 当前场景最长时间 更新机关位置
								if(g_timeCountGim->fPlaybarTime > TIME_5 && g_timeCountGim->fPlaybarTime < TIME_10)
								{
									if(g_aGimmick[nCntGim].nNextPriNum != -1)
									{
										// 次の時間軸の時間帯 時間方向の判断
										float fNextPriStart = g_aPlaybarGim[g_aGimmick[nCntGim].nNextPriNum].fTimeStart;
										float fNextPriEnd = g_aPlaybarGim[g_aGimmick[nCntGim].nNextPriNum].fTimeEnd;
										// 走完自身时间条需要的消耗 加上 该时间条的终点到下一个时间条的起点的消耗
										float fLoopSpeed = (TIME_5 * UPDATA_FRAME + abs(TIME_5 - fNextPriStart) * UPDATA_FRAME) / 300;

										// 正引き
										if(g_timeCountGim->fPlaybarTime >= fNextPriEnd)
										{
											int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_10 * UPDATA_FRAME);
											if(nTemp < fNextPriStart * UPDATA_FRAME)
												nTemp = (int)(fNextPriStart * UPDATA_FRAME);
											g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
											g_aGimmick[nCntGim].fLoopCount -= fLoopSpeed;
											// 時間軌跡の表示
											SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
										}
										// 逆引き
										else if(g_timeCountGim->fPlaybarTime <= fNextPriEnd)
										{
											if(g_aGimmick[nCntGim].fLoopCount > (-TIME_5 * UPDATA_FRAME) && !g_aGimmick[nCntGim].bTimeChange)
											{
												int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_10 * UPDATA_FRAME);
												if(nTemp < TIME_5 * UPDATA_FRAME)
													nTemp = (int)(TIME_5 * UPDATA_FRAME);
												g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
												g_aGimmick[nCntGim].fLoopCount -= fLoopSpeed;
												// 時間軌跡の表示
												SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
											}
											else
											{
												if(!g_aGimmick[nCntGim].bTimeChange)
													g_aGimmick[nCntGim].fLoopCount = TIME_0 * UPDATA_FRAME;
												g_aGimmick[nCntGim].bTimeChange = true;
											}

											if(g_aGimmick[nCntGim].bTimeChange == true)
											{
												int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_5 * UPDATA_FRAME);
												if(nTemp > fNextPriStart * UPDATA_FRAME)
													nTemp = (int)(fNextPriStart * UPDATA_FRAME);
												g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
												g_aGimmick[nCntGim].fLoopCount += fLoopSpeed;
												// 時間軌跡の表示
												SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_GRAY);
											}
										}
									}
									// 次の時間帯はない
									else
									{
										int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_10 * UPDATA_FRAME);
										if(nTemp < TIME_5 * UPDATA_FRAME)
											nTemp = (int)(TIME_5 * UPDATA_FRAME);
										g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
										g_aGimmick[nCntGim].fLoopCount --;
										// 時間軌跡の表示
										SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
									}
								}
							}

							if(g_aGimmick[nCntGim].bCurrentPri06 == true)
							{
								// ギミックのポジション再生
								// 当前时间 + 跨越（跳跃）的时间段长度 < 当前场景最长时间 更新机关位置
								if(g_timeCountGim->fPlaybarTime > TIME_10 && g_timeCountGim->fPlaybarTime < TIME_15)
								{
									if(g_aGimmick[nCntGim].nNextPriNum != -1)
									{
										// 次の時間軸の時間帯 時間方向の判断
										float fNextPriStart = g_aPlaybarGim[g_aGimmick[nCntGim].nNextPriNum].fTimeStart;
										float fNextPriEnd = g_aPlaybarGim[g_aGimmick[nCntGim].nNextPriNum].fTimeEnd;
										// 走完自身时间条需要的消耗 加上 该时间条的终点到下一个时间条的起点的消耗
										float fLoopSpeed = (TIME_5 * UPDATA_FRAME + abs(TIME_10 - fNextPriStart) * UPDATA_FRAME) / 300;

										// 正引き
										if(g_timeCountGim->fPlaybarTime >= fNextPriEnd)
										{
											int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_15 * UPDATA_FRAME);
											if(nTemp < fNextPriStart * UPDATA_FRAME)
												nTemp = (int)(fNextPriStart * UPDATA_FRAME);
											g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
											g_aGimmick[nCntGim].fLoopCount -= fLoopSpeed;
											// 時間軌跡の表示
											SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
										}
										// 逆引き
										else if(g_timeCountGim->fPlaybarTime <= fNextPriEnd)
										{
											if(g_aGimmick[nCntGim].fLoopCount > (-TIME_5 * UPDATA_FRAME) && !g_aGimmick[nCntGim].bTimeChange)
											{
												int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_15 * UPDATA_FRAME);
												if(nTemp < TIME_10 * UPDATA_FRAME)
													nTemp = (int)(TIME_10 * UPDATA_FRAME);
												g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
												g_aGimmick[nCntGim].fLoopCount -= fLoopSpeed;
												// 時間軌跡の表示
												SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
											}
											else
											{
												if(!g_aGimmick[nCntGim].bTimeChange)
													g_aGimmick[nCntGim].fLoopCount = TIME_0 * UPDATA_FRAME;
												g_aGimmick[nCntGim].bTimeChange = true;
											}

											if(g_aGimmick[nCntGim].bTimeChange == true)
											{
												int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_10 * UPDATA_FRAME);
												if(nTemp > fNextPriStart * UPDATA_FRAME)
													nTemp = (int)(fNextPriStart * UPDATA_FRAME);
												g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
												g_aGimmick[nCntGim].fLoopCount += fLoopSpeed;
												// 時間軌跡の表示
												SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_GRAY);
											}
										}
									}
									// 次の時間帯はない
									else
									{
										int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount + TIME_15 * UPDATA_FRAME);
										if(nTemp < TIME_10 * UPDATA_FRAME)
											nTemp = (int)(TIME_10 * UPDATA_FRAME);
										g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
										g_aGimmick[nCntGim].fLoopCount--;
										// 時間軌跡の表示
										SetTimeTrack(D3DXVECTOR3(g_aGimmick[nCntGim].pos.x, g_aGimmick[nCntGim].pos.y, 0), D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
									}
								}
							}
						}
					}
					// 固定した場合
					else
					{
						if(!*GetIsPause())
						{
							if(g_timeCountGim->fCurrentTime > TIME_0 && g_timeCountGim->fCurrentTime < TIME_15)
							{
								int nTemp = (int)(g_aGimmick[nCntGim].fLoopCount);
								g_aGimmick[nCntGim].pos = g_aGimmick[nCntGim].track[nTemp];
								g_aGimmick[nCntGim].fLoopCount++;
							}
						}
					}
				}
			}
		}
	}
	// ギミックの表示とあたり判定
	// 時間軸３部分
	for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
	{
		if(g_aGimmick[nCntGim].bUse)
		{
			switch(g_aGimmick[nCntGim].nType)
			{
			case GIMMICK_ATTACK_001:
				g_aGimmick[nCntGim].fMaxX = g_aGimmick[nCntGim].pos.x + g_aGimmick[nCntGim].fWidth / 2.0f;
				g_aGimmick[nCntGim].fMinX = g_aGimmick[nCntGim].pos.x - g_aGimmick[nCntGim].fWidth / 2.0f;
				g_aGimmick[nCntGim].fMaxY = g_aGimmick[nCntGim].pos.y + g_aGimmick[nCntGim].fHeight / 2.0f;
				g_aGimmick[nCntGim].fMinY = g_aGimmick[nCntGim].pos.y - g_aGimmick[nCntGim].fHeight / 2.0f;
				break;
			case GIMMICK_ATTACK_002:
				g_aGimmick[nCntGim].fMaxX = g_aGimmick[nCntGim].pos.x + g_aGimmick[nCntGim].fWidth / 2.0f;
				g_aGimmick[nCntGim].fMinX = g_aGimmick[nCntGim].pos.x - g_aGimmick[nCntGim].fWidth / 2.0f;
				g_aGimmick[nCntGim].fMaxY = g_aGimmick[nCntGim].pos.y + g_aGimmick[nCntGim].fHeight / 2.0f;
				g_aGimmick[nCntGim].fMinY = g_aGimmick[nCntGim].pos.y - g_aGimmick[nCntGim].fHeight / 2.0f;
				break;
			case GIMMICK_HELP_003:
				g_aGimmick[nCntGim].fMaxX = g_aGimmick[nCntGim].pos.x + g_aGimmick[nCntGim].fWidth / 2.0f;
				g_aGimmick[nCntGim].fMinX = g_aGimmick[nCntGim].pos.x - g_aGimmick[nCntGim].fWidth / 2.0f;
				g_aGimmick[nCntGim].fMaxY = g_aGimmick[nCntGim].pos.y + g_aGimmick[nCntGim].fHeight / 2.0f;
				g_aGimmick[nCntGim].fMinY = g_aGimmick[nCntGim].pos.y - g_aGimmick[nCntGim].fHeight / 2.0f;
				break;
			case GIMMICK_HELP_004:
				g_aGimmick[nCntGim].fMaxX = g_aGimmick[nCntGim].pos.x + g_aGimmick[nCntGim].fWidth / 2.0f;
				g_aGimmick[nCntGim].fMinX = g_aGimmick[nCntGim].pos.x - g_aGimmick[nCntGim].fWidth / 2.0f;
				g_aGimmick[nCntGim].fMaxY = g_aGimmick[nCntGim].pos.y + g_aGimmick[nCntGim].fHeight / 2.0f;
				g_aGimmick[nCntGim].fMinY = g_aGimmick[nCntGim].pos.y - g_aGimmick[nCntGim].fHeight / 2.0f;
				break;
			case GIMMICK_HELP_005:
				g_aGimmick[nCntGim].fMaxX = g_aGimmick[nCntGim].pos.x + g_aGimmick[nCntGim].fWidth / 2.0f;
				g_aGimmick[nCntGim].fMinX = g_aGimmick[nCntGim].pos.x - g_aGimmick[nCntGim].fWidth / 2.0f;
				g_aGimmick[nCntGim].fMaxY = g_aGimmick[nCntGim].pos.y + g_aGimmick[nCntGim].fHeight / 2.0f;
				g_aGimmick[nCntGim].fMinY = g_aGimmick[nCntGim].pos.y - g_aGimmick[nCntGim].fHeight / 2.0f;
				break;
			case GIMMICK_HELP_006:
				g_aGimmick[nCntGim].fMaxX = g_aGimmick[nCntGim].pos.x + g_aGimmick[nCntGim].fWidth / 2.0f;
				g_aGimmick[nCntGim].fMinX = g_aGimmick[nCntGim].pos.x - g_aGimmick[nCntGim].fWidth / 2.0f;
				g_aGimmick[nCntGim].fMaxY = g_aGimmick[nCntGim].pos.y + g_aGimmick[nCntGim].fHeight / 2.0f;
				g_aGimmick[nCntGim].fMinY = g_aGimmick[nCntGim].pos.y - g_aGimmick[nCntGim].fHeight / 2.0f;
				break;
			}

			// パターンの切り替え
			SetTextureGimmick(nCntGim, 0, 1, 1, 1);
			SetVertexGimmick(nCntGim, 1, 1);

			if(IsGimmickHitPlayer(nCntGim) == true)
				GetPlayer()[0].bIsHitByGmmic = true;
				//*GetIsPlay() = false;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGimmick(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffGimmick, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
	{
		if(g_aGimmick[nCntGim].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureGimmick[g_aGimmick[nCntGim].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntGim * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexGimmick(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_GIMMICK,		// 頂点データ用に確保するバッファサイズ(バイト単位)
											D3DUSAGE_WRITEONLY,									// 頂点バッファの使用法　
											FVF_VERTEX_2D,										// 使用する頂点フォーマット
											D3DPOOL_MANAGED,									// リソースのバッファを保持するメモリクラスを指定
											&g_pD3DVtxBuffGimmick,								// 頂点バッファインターフェースへのポインタ
											NULL)))												// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffGimmick->Lock(0, 0, (void**)&pVtx, 0);


		for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++, pVtx += 4)
		{
		// 頂点座標の設定
			//pVtx[0].vtx = D3DXVECTOR3(g_gimmick[GIMMICK_MAX].pos.x - GIMMICK_SIZE_X / 2, g_gimmick[GIMMICK_MAX].pos.y - GIMMICK_SIZE_Y / 2, 0.0f);
			//pVtx[1].vtx = D3DXVECTOR3(g_gimmick[GIMMICK_MAX].pos.x + GIMMICK_SIZE_X / 2, g_gimmick[GIMMICK_MAX].pos.y - GIMMICK_SIZE_Y / 2, 0.0f);
			//pVtx[2].vtx = D3DXVECTOR3(g_gimmick[GIMMICK_MAX].pos.x - GIMMICK_SIZE_X / 2, g_gimmick[GIMMICK_MAX].pos.y + GIMMICK_SIZE_Y / 2, 0.0f);
			//pVtx[3].vtx = D3DXVECTOR3(g_gimmick[GIMMICK_MAX].pos.x + GIMMICK_SIZE_X / 2, g_gimmick[GIMMICK_MAX].pos.y + GIMMICK_SIZE_Y / 2, 0.0f);

			// rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

			//// テクスチャ座標の設定
			//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			//pVtx[1].tex = D3DXVECTOR2(TEX_PATTERN_SIZE_X, 0.0f);
			//pVtx[2].tex = D3DXVECTOR2(0.0f, TEX_PATTERN_SIZE_Y);
			//pVtx[3].tex = D3DXVECTOR2(TEX_PATTERN_SIZE_X, TEX_PATTERN_SIZE_Y);

			// 頂点データをアンロックする
			g_pD3DVtxBuffGimmick->Unlock();
		}
	}
	return S_OK;
}



//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorGimmick(int nIdxGimmick, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffGimmick->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdxGimmick * 4);

	// 頂点座標の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	g_pD3DVtxBuffGimmick->Unlock();
}

void SetVertexGimmick(	int nIdx,					//记录当前对应的图组polygon地址
						float scaleX,				//X轴的放大参数
						float scaleY)				//自身Y轴的大小参数
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffGimmick->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(g_aGimmick[nIdx].pos.x - g_aGimmick[nIdx].fWidth / 2 * scaleX, g_aGimmick[nIdx].pos.y - g_aGimmick[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_aGimmick[nIdx].pos.x + g_aGimmick[nIdx].fWidth / 2 * scaleX, g_aGimmick[nIdx].pos.y - g_aGimmick[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_aGimmick[nIdx].pos.x - g_aGimmick[nIdx].fWidth / 2 * scaleX, g_aGimmick[nIdx].pos.y + g_aGimmick[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_aGimmick[nIdx].pos.x + g_aGimmick[nIdx].fWidth / 2 * scaleX, g_aGimmick[nIdx].pos.y + g_aGimmick[nIdx].fHeight / 2 * scaleY, 0.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffGimmick->Unlock();
}


void SetTextureGimmick(	int nIdx,						//记录当前对应的图组地址
						int nPatternAnim,				//记录当前显示帧
						int numAnimPattern, 			//记录当前对应的组成图组的图片数量
						int texPatternDivideX,			//记录当前图组X轴图片数量
						int texPatternDivideY )			//记录当前图组Y轴图片数量
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffGimmick->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdx * 4);

		// テクスチャ座標の設定 纹理坐标设定
		fPosXLeft = (float)(nPatternAnim % texPatternDivideX) * 1.0f / texPatternDivideX;
		fPosXRight = (float)(nPatternAnim % texPatternDivideX) * 1.0f / texPatternDivideX + 1.0f / texPatternDivideX;
		fPosYUp = (float)(nPatternAnim / (numAnimPattern / texPatternDivideY)) * 1.0f / texPatternDivideY;
		fPosYDown = (float)(nPatternAnim / (numAnimPattern / texPatternDivideY)) * 1.0f / texPatternDivideY + 1.0f / texPatternDivideY;

		pVtx[0].tex = D3DXVECTOR2(fPosXLeft, fPosYUp);
		pVtx[1].tex = D3DXVECTOR2(fPosXRight, fPosYUp);
		pVtx[2].tex = D3DXVECTOR2(fPosXLeft, fPosYDown);
		pVtx[3].tex = D3DXVECTOR2(fPosXRight, fPosYDown);

		// 頂点データをアンロックする
		g_pD3DVtxBuffGimmick->Unlock();
	}
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetGimmick(D3DXVECTOR3 pos, D3DXCOLOR col, int nType, char aRemarks[256])
{
	for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
	{
		if(!g_aGimmick[nCntGim].bUse)
		{
			g_aGimmick[nCntGim].pos = pos;
			g_aGimmick[nCntGim].nLastTrack = 0;
			g_aGimmick[nCntGim].bUse = true;
			strcpy_s(g_aGimmick[nCntGim].aRemarks, aRemarks);

			switch (nType)
			{
			case GIMMICK_ATTACK_001:
				g_aGimmick[nCntGim].fWidth = GIMMICK_01_SIZE_X;
				g_aGimmick[nCntGim].fHeight = GIMMICK_01_SIZE_Y;
				g_aGimmick[nCntGim].fTimeStart = GIMMICK_01_TIME_START;
				g_aGimmick[nCntGim].fTimeEnd = GIMMICK_01_TIME_END;
				g_aGimmick[nCntGim].fRadius = GIMMICK_01_RADIUS;
				g_aGimmick[nCntGim].fRadiusDis = GIMMICK_01_RADIUSDIS;
				g_aGimmick[nCntGim].fDegree = GIMMICK_01_DEGREE_MIN;
				g_aGimmick[nCntGim].nType = nType;
				g_aGimmick[nCntGim].nMode = MODE_ATTACK;
				break;
				
			case GIMMICK_ATTACK_002:
				g_aGimmick[nCntGim].fWidth = GIMMICK_02_SIZE_X;
				g_aGimmick[nCntGim].fHeight = GIMMICK_02_SIZE_Y;
				g_aGimmick[nCntGim].fTimeStart = GIMMICK_02_TIME_START;
				g_aGimmick[nCntGim].fTimeEnd = GIMMICK_02_TIME_END;
				g_aGimmick[nCntGim].fRadius = GIMMICK_02_RADIUS;
				g_aGimmick[nCntGim].fRadiusDis = GIMMICK_02_RADIUSDIS;
				g_aGimmick[nCntGim].fDegree = GIMMICK_02_DEGREE_MIN;
				g_aGimmick[nCntGim].nType = nType;
				g_aGimmick[nCntGim].nMode = MODE_ATTACK;
				break;

			case GIMMICK_HELP_003:
				g_aGimmick[nCntGim].fWidth = GIMMICK_03_SIZE_X;
				g_aGimmick[nCntGim].fHeight = GIMMICK_03_SIZE_Y;
				g_aGimmick[nCntGim].fTimeStart = GIMMICK_03_TIME_START;
				g_aGimmick[nCntGim].fTimeEnd = GIMMICK_03_TIME_END;
				g_aGimmick[nCntGim].nType = nType;
				g_aGimmick[nCntGim].nMode = MODE_HELP;
				g_aGimmick[nCntGim].fGoalY = GIMMICK_03_GOAL_Y - g_aGimmick[nCntGim].fHeight / 2;
				break;

			case GIMMICK_HELP_004:
				g_aGimmick[nCntGim].fWidth = GIMMICK_04_SIZE_X;
				g_aGimmick[nCntGim].fHeight = GIMMICK_04_SIZE_Y;
				g_aGimmick[nCntGim].fTimeStart = GIMMICK_04_TIME_START;
				g_aGimmick[nCntGim].fTimeEnd = GIMMICK_04_TIME_END;
				g_aGimmick[nCntGim].nType = nType;
				g_aGimmick[nCntGim].nMode = MODE_HELP;
				g_aGimmick[nCntGim].fGoalY = GIMMICK_04_GOAL_Y + g_aGimmick[nCntGim].fHeight / 2;
				break;

			case GIMMICK_HELP_005:
				g_aGimmick[nCntGim].fWidth = GIMMICK_05_SIZE_X;
				g_aGimmick[nCntGim].fHeight = GIMMICK_05_SIZE_Y;
				g_aGimmick[nCntGim].fTimeStart = GIMMICK_05_TIME_START;
				g_aGimmick[nCntGim].fTimeEnd = GIMMICK_05_TIME_END;
				g_aGimmick[nCntGim].nType = nType;
				g_aGimmick[nCntGim].nMode = MODE_HELP;
				g_aGimmick[nCntGim].fGoalX = GIMMICK_05_GOAL_X - g_aGimmick[nCntGim].fWidth / 2;
				break;

			case GIMMICK_HELP_006:
				g_aGimmick[nCntGim].fWidth = GIMMICK_06_SIZE_X;
				g_aGimmick[nCntGim].fHeight = GIMMICK_06_SIZE_Y;
				g_aGimmick[nCntGim].fTimeStart = GIMMICK_06_TIME_START;
				g_aGimmick[nCntGim].fTimeEnd = GIMMICK_06_TIME_END;
				g_aGimmick[nCntGim].nType = nType;
				g_aGimmick[nCntGim].nMode = MODE_HELP;
				g_aGimmick[nCntGim].fGoalX = GIMMICK_06_GOAL_X + g_aGimmick[nCntGim].fWidth / 2;
				break;

			default:
				break;
			}
			//SetVertexGimmick(nCntGim, 1, 1);
			
			// 頂点カラーの設定
			SetColorGimmick(nCntGim, col);
			SetupGimmickTrack(nCntGim);
			*GetCleanupSwitch() = false;
			return nCntGim;
		}
	}
	return -1;
}


// ギミックの制作設定
void GimmickCreateMode(void)
{
	// 左键单击创建物体
	if(IsMouseLeftTriggered())
	{
		g_bIsGimmickCreate = true;
		g_nCreateGimmickNum = -1;
	}

	// 制作したギミックを修正する
	if(g_bIsGimmickCreate == true)
	{
		bool bTemp = false;
		int nTemp = -1;
		if(IsMouseLeftPressed())
		{
			g_nMoveDelay_GIM++;

			PrintDebugProc("*******g_nMoveDelay_GIM = %d*******\n", g_nMoveDelay_GIM);

			for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
			{
				if(g_aGimmick[nCntGim].bUse)
				{
					if(	g_aGimmick[nCntGim].fMaxX > *GetMouX() &&
						g_aGimmick[nCntGim].fMinX < *GetMouX() &&
						g_aGimmick[nCntGim].fMaxY > *GetMouY() &&
						g_aGimmick[nCntGim].fMinY < *GetMouY())
					{
						bTemp = true;
						nTemp = nCntGim;
						g_nBeInControl = nCntGim;
					}
				}
			}
			if(bTemp == true && g_nMoveDelay_GIM > GIMMICK_MOVEDELAY_TIME)
			{
				/***实时更新轨迹必须在gimmick绑定鼠标位置之前实现***/
				// 重新排列需要重置最后一针的时间点，也就是从头开始记录
				//g_aGimmick[nTemp].nLastTrack = 0;
				// 针对新建的机关进行模拟轨迹构建
				SetupGimmickTrack(nTemp);
				// 重排轨迹线
				*GetCleanupSwitch() = false;

				float fX = 0, fY = 0;
				fX = *GetMouX() - (int)*GetMouX() % 5;
				fY = *GetMouY() - (int)*GetMouY() % 5;
				g_aGimmick[nTemp].pos = D3DXVECTOR3(fX, fY, 0.0f);
				g_aGimmick[nTemp].posSave = D3DXVECTOR3(fX, fY, 0.0f);
			}
		}
		else
			g_nMoveDelay_GIM = 0;
	}

	// 右键单击切换物体
	// ギミックの種類を変換する
	if(IsMouseRightTriggered())
	{
		g_bIsGimmickCreate = false;

		// 如果此时物体已经创建并跟随鼠标，那么删除当前物体，切换到下一个。
		if(g_nCreateGimmickNum == -1)
			g_nCreateGimmickNum = SetGimmick(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GIMMICK_ATTACK_001, "GIMMICK_ATTACK_001");

		if(g_nCreateGimmickNum != -1)
		{
			int nType = g_aGimmick[g_nCreateGimmickNum].nType;
			g_aGimmick[g_nCreateGimmickNum].nLastTrack = 0;
			g_aGimmick[g_nCreateGimmickNum].bUse = false;

			nType++;
			if(nType == GIMMICK_TYPE_MAX)
				nType = 0;

			switch (nType)
			{
				case GIMMICK_ATTACK_001:
					g_nCreateGimmickNum = SetGimmick(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GIMMICK_ATTACK_001, "GIMMICK_ATTACK_001");
					break;
				case GIMMICK_ATTACK_002:
					g_nCreateGimmickNum = SetGimmick(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GIMMICK_ATTACK_002, "GIMMICK_ATTACK_002");
					break;
				case GIMMICK_HELP_003:
					g_nCreateGimmickNum = SetGimmick(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GIMMICK_HELP_003, "GIMMICK_HELP_003");
					break;
				case GIMMICK_HELP_004:
					g_nCreateGimmickNum = SetGimmick(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GIMMICK_HELP_004, "GIMMICK_HELP_004");
					break;
				case GIMMICK_HELP_005:
					g_nCreateGimmickNum = SetGimmick(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GIMMICK_HELP_005, "GIMMICK_HELP_005");
					break;
				case GIMMICK_HELP_006:
					g_nCreateGimmickNum = SetGimmick(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GIMMICK_HELP_006, "GIMMICK_HELP_006");
					break;
				default:
					break;
			}
		}
	}

	if(g_bIsGimmickCreate == false)
	{
		/***实时更新轨迹必须在gimmick绑定鼠标位置之前实现***/
		// 针对新建的机关进行模拟轨迹构建
		SetupGimmickTrack(g_nCreateGimmickNum);
		// 重排轨迹线
		*GetCleanupSwitch() = false;

		float fX = 0, fY = 0;
		fX = *GetMouX() - (int)*GetMouX() % 5;
		fY = *GetMouY() - (int)*GetMouY() % 5;
		g_aGimmick[g_nCreateGimmickNum].pos.x = fX;
		g_aGimmick[g_nCreateGimmickNum].pos.y = fY;
	}
	
	// 打印当前墙壁类型名称
	switch(g_aGimmick[g_nCreateGimmickNum].nType)
	{
		case GIMMICK_ATTACK_001:
			PrintDebugProc("【OBJECT NAME】: GIMMICK_ATTACK_001\n");
			break;
		case GIMMICK_ATTACK_002:
			PrintDebugProc("【OBJECT NAME】: GIMMICK_ATTACK_002\n");
			break;
		case GIMMICK_HELP_003:
			PrintDebugProc("【OBJECT NAME】: GIMMICK_HELP_003\n");
			break;
		case GIMMICK_HELP_004:
			PrintDebugProc("【OBJECT NAME】: GIMMICK_HELP_004\n");
			break;
		case GIMMICK_HELP_005:
			PrintDebugProc("【OBJECT NAME】: GIMMICK_HELP_005\n");
			break;
		case GIMMICK_HELP_006:
			PrintDebugProc("【OBJECT NAME】: GIMMICK_HELP_006\n");
			break;
		default:
			break;
	}

	// 根据被最近选中的机关类型，显示该机关的属性值
	if(g_nBeInControl != -1)
		switch(g_aGimmick[g_nBeInControl].nType)
		{
			case GIMMICK_ATTACK_001:
			{
				PrintDebugProc("\n\n");
				PrintDebugProc("**************************************************\n");
				PrintDebugProc("【今選択したギミック種類】: GIMMICK_ATTACK_001\n");
				PrintDebugProc("【メニュー】 【属性】    【数値】\n");
				PrintDebugProc("   【1】   【開始時間】  : %f\n", g_aGimmick[g_nBeInControl].fTimeStart);
				PrintDebugProc("   【2】   【終わる時間】: %f\n", g_aGimmick[g_nBeInControl].fTimeEnd);
				PrintDebugProc("   【3】   【半径】:       %f\n", g_aGimmick[g_nBeInControl].fRadiusDis);
				PrintDebugProc("**************************************************\n");
				PrintDebugProc("\n\n");

				if(GetKeyboardTrigger(DIK_1))	g_nMenuSelect = 1;
				if(GetKeyboardTrigger(DIK_2))	g_nMenuSelect = 2;
				if(GetKeyboardTrigger(DIK_3))	g_nMenuSelect = 3;
			
				switch(g_nMenuSelect)
				{
				case 1:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【1】   【開始時間】  : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fTimeStart -= 0.1f;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fTimeStart += 0.1f;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fTimeStart -= 0.5f;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fTimeStart += 0.5f;
					break;
				}
				case 2:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【2】   【終わる時間】: %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fTimeEnd -= 0.1f;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fTimeEnd += 0.1f;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fTimeEnd -= 0.5f;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fTimeEnd += 0.5f;
					break;
				}
				case 3:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【3】   【半径】:       %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fRadiusDis -= 0.1f;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fRadiusDis += 0.1f;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fRadiusDis -= 0.5f;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fRadiusDis += 0.5f;
					break;
				}

				default:
					break;
				}
				break;
			}
			case GIMMICK_ATTACK_002:
			{
				PrintDebugProc("\n\n");
				PrintDebugProc("**************************************************\n");
				PrintDebugProc("【今選択したギミック種類】: GIMMICK_ATTACK_002\n");
				PrintDebugProc("【メニュー】 【属性】    【数値】\n");
				PrintDebugProc("   【1】   【開始時間】  : %f\n", g_aGimmick[g_nBeInControl].fTimeStart);
				PrintDebugProc("   【2】   【終わる時間】: %f\n", g_aGimmick[g_nBeInControl].fTimeEnd);
				PrintDebugProc("   【3】   【半径】:       %f\n", g_aGimmick[g_nBeInControl].fRadiusDis);
				PrintDebugProc("**************************************************\n");
				PrintDebugProc("\n\n");

				if(GetKeyboardTrigger(DIK_1))	g_nMenuSelect = 1;
				if(GetKeyboardTrigger(DIK_2))	g_nMenuSelect = 2;
				if(GetKeyboardTrigger(DIK_3))	g_nMenuSelect = 3;
			
				switch(g_nMenuSelect)
				{
				case 1:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【1】   【開始時間】  : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fTimeStart -= 0.1f;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fTimeStart += 0.1f;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fTimeStart -= 0.5f;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fTimeStart += 0.5f;
					break;
				}
				case 2:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【2】   【終わる時間】: %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fTimeEnd -= 0.1f;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fTimeEnd += 0.1f;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fTimeEnd -= 0.5f;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fTimeEnd += 0.5f;
					break;
				}
				case 3:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【3】   【半径】:       %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fRadiusDis -= 0.1f;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fRadiusDis += 0.1f;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fRadiusDis -= 0.5f;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fRadiusDis += 0.5f;
					break;
				}

				default:
					break;
				}
				break;
			}
			// 上から下
			case GIMMICK_HELP_003:
			{
				PrintDebugProc("\n\n");
				PrintDebugProc("**************************************************\n");
				PrintDebugProc("【今選択したギミック種類】: GIMMICK_HELP_003\n");
				PrintDebugProc("【メニュー】 【属性】       【数値】\n");
				PrintDebugProc("   【1】   【開始時間】      : %f\n", g_aGimmick[g_nBeInControl].fTimeStart);
				PrintDebugProc("   【2】   【終わる時間】    : %f\n", g_aGimmick[g_nBeInControl].fTimeEnd);
				PrintDebugProc("   【3】   【エンドポイント】: %f\n", g_aGimmick[g_nBeInControl].fGoalY + g_aGimmick[g_nBeInControl].fHeight / 2);
				PrintDebugProc("   【4】   【サイズ_X】      : %f\n", g_aGimmick[g_nBeInControl].fWidth);
				PrintDebugProc("   【5】   【サイズ_Y】      : %f\n", g_aGimmick[g_nBeInControl].fHeight);
				PrintDebugProc("**************************************************\n");
				PrintDebugProc("\n\n");
			
				if(GetKeyboardTrigger(DIK_1))	g_nMenuSelect = 1;
				if(GetKeyboardTrigger(DIK_2))	g_nMenuSelect = 2;
				if(GetKeyboardTrigger(DIK_3))	g_nMenuSelect = 3;
				if(GetKeyboardTrigger(DIK_4))	g_nMenuSelect = 4;
				if(GetKeyboardTrigger(DIK_5))	g_nMenuSelect = 5;

				switch(g_nMenuSelect)
				{
				case 1:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【1】   【開始時間】      : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fTimeStart -= 0.1f;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fTimeStart += 0.1f;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fTimeStart -= 0.5f;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fTimeStart += 0.5f;
					break;
				}
				case 2:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【2】   【終わる時間】    : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fTimeEnd -= 0.1f;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fTimeEnd += 0.1f;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fTimeEnd -= 0.5f;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fTimeEnd += 0.5f;
					break;
				}
				case 3:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【3】   【エンドポイント】: %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fGoalY -= 1;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fGoalY += 1;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fGoalY -= 5;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fGoalY += 5;
					break;
				}
				case 4:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 5】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 10】\n");
					PrintDebugProc("   【4】   【サイズ_X】      : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fWidth -= 5;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fWidth += 5;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fWidth -= 10;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fWidth += 10;
					break;
				}
				case 5:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 5】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 10】\n");
					PrintDebugProc("   【4】   【サイズ_Y】      : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
					{
						g_aGimmick[g_nBeInControl].fHeight -= 5;
						g_aGimmick[g_nBeInControl].fGoalY += 5.0f / 2.0f;
					}
					if(GetKeyboardRepeat(DIK_RIGHT))
					{
						g_aGimmick[g_nBeInControl].fHeight += 5;
						g_aGimmick[g_nBeInControl].fGoalY -= 5.0f / 2.0f;
					}
					if(GetKeyboardRepeat(DIK_DOWN))
					{
						g_aGimmick[g_nBeInControl].fHeight -= 10;
						g_aGimmick[g_nBeInControl].fGoalY += 10.0f / 2.0f;
					}
					if(GetKeyboardRepeat(DIK_UP))
					{
						g_aGimmick[g_nBeInControl].fHeight += 10;
						g_aGimmick[g_nBeInControl].fGoalY -= 10.0f / 2.0f;
					}
					break;
				}
				default:
					break;
				}
				break;
			}
			// 下から上下
			case GIMMICK_HELP_004:
			{
				PrintDebugProc("\n\n");
				PrintDebugProc("**************************************************\n");
				PrintDebugProc("【今選択したギミック種類】: GIMMICK_HELP_004\n");
				PrintDebugProc("【メニュー】 【属性】       【数値】\n");
				PrintDebugProc("   【1】   【開始時間】      : %f\n", g_aGimmick[g_nBeInControl].fTimeStart);
				PrintDebugProc("   【2】   【終わる時間】    : %f\n", g_aGimmick[g_nBeInControl].fTimeEnd);
				PrintDebugProc("   【3】   【エンドポイント】: %f\n", g_aGimmick[g_nBeInControl].fGoalY - g_aGimmick[g_nBeInControl].fHeight / 2);
				PrintDebugProc("   【4】   【サイズ_X】      : %f\n", g_aGimmick[g_nBeInControl].fWidth);
				PrintDebugProc("   【5】   【サイズ_Y】      : %f\n", g_aGimmick[g_nBeInControl].fHeight);
				PrintDebugProc("**************************************************\n");
				PrintDebugProc("\n\n");
			
				if(GetKeyboardTrigger(DIK_1))	g_nMenuSelect = 1;
				if(GetKeyboardTrigger(DIK_2))	g_nMenuSelect = 2;
				if(GetKeyboardTrigger(DIK_3))	g_nMenuSelect = 3;
				if(GetKeyboardTrigger(DIK_4))	g_nMenuSelect = 4;
				if(GetKeyboardTrigger(DIK_5))	g_nMenuSelect = 5;
			
				switch(g_nMenuSelect)
				{
				case 1:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【1】   【開始時間】      : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fTimeStart -= 0.1f;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fTimeStart += 0.1f;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fTimeStart -= 0.5f;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fTimeStart += 0.5f;
					break;
				}
				case 2:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【2】   【終わる時間】    : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fTimeEnd -= 0.1f;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fTimeEnd += 0.1f;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fTimeEnd -= 0.5f;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fTimeEnd += 0.5f;
					break;
				}
				case 3:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【3】   【エンドポイント】: %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fGoalY -= 1;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fGoalY += 1;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fGoalY -= 5;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fGoalY += 5;
					break;
				}
				case 4:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 5】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 10】\n");
					PrintDebugProc("   【4】   【サイズ_X】      : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fWidth -= 5;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fWidth += 5;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fWidth -= 10;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fWidth += 10;
					break;
				}
				case 5:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 5】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 10】\n");
					PrintDebugProc("   【4】   【サイズ_Y】      : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
					{
						g_aGimmick[g_nBeInControl].fHeight -= 5;
						g_aGimmick[g_nBeInControl].fGoalY -= 5.0f / 2.0f;
					}
					if(GetKeyboardRepeat(DIK_RIGHT))
					{
						g_aGimmick[g_nBeInControl].fHeight += 5;
						g_aGimmick[g_nBeInControl].fGoalY += 5.0f / 2.0f;
					}
					if(GetKeyboardRepeat(DIK_DOWN))
					{
						g_aGimmick[g_nBeInControl].fHeight -= 10;
						g_aGimmick[g_nBeInControl].fGoalY -= 10.0f / 2.0f;
					}
					if(GetKeyboardRepeat(DIK_UP))
					{
						g_aGimmick[g_nBeInControl].fHeight += 10;
						g_aGimmick[g_nBeInControl].fGoalY += 10.0f / 2.0f;
					}
					break;
				}
				default:
					break;
				}
				break;
			}
			case GIMMICK_HELP_005:
			{
				PrintDebugProc("\n\n");
				PrintDebugProc("**************************************************\n");
				PrintDebugProc("【今選択したギミック種類】: GIMMICK_HELP_005\n");
				PrintDebugProc("【メニュー】 【属性】       【数値】\n");
				PrintDebugProc("   【1】   【開始時間】      : %f\n", g_aGimmick[g_nBeInControl].fTimeStart);
				PrintDebugProc("   【2】   【終わる時間】    : %f\n", g_aGimmick[g_nBeInControl].fTimeEnd);
				PrintDebugProc("   【3】   【エンドポイント】: %f\n", g_aGimmick[g_nBeInControl].fGoalX - g_aGimmick[g_nBeInControl].fWidth / 2);
				PrintDebugProc("   【4】   【サイズ_X】      : %f\n", g_aGimmick[g_nBeInControl].fWidth);
				PrintDebugProc("   【5】   【サイズ_Y】      : %f\n", g_aGimmick[g_nBeInControl].fHeight);
				PrintDebugProc("**************************************************\n");
				PrintDebugProc("\n\n");
			
				if(GetKeyboardTrigger(DIK_1))	g_nMenuSelect = 1;
				if(GetKeyboardTrigger(DIK_2))	g_nMenuSelect = 2;
				if(GetKeyboardTrigger(DIK_3))	g_nMenuSelect = 3;
				if(GetKeyboardTrigger(DIK_4))	g_nMenuSelect = 4;
				if(GetKeyboardTrigger(DIK_5))	g_nMenuSelect = 5;
			
				switch(g_nMenuSelect)
				{
				case 1:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【1】   【開始時間】      : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fTimeStart -= 0.1f;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fTimeStart += 0.1f;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fTimeStart -= 0.5f;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fTimeStart += 0.5f;
					break;
				}
				case 2:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【2】   【終わる時間】    : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fTimeEnd -= 0.1f;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fTimeEnd += 0.1f;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fTimeEnd -= 0.5f;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fTimeEnd += 0.5f;
					break;
				}
				case 3:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
					PrintDebugProc("   【3】   【エンドポイント】: %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fGoalX -= 1;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fGoalX += 1;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fGoalX -= 5;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fGoalX += 5;
					break;
				}
				case 4:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 5】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 10】\n");
					PrintDebugProc("   【4】   【サイズ_X】      : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
					{
						g_aGimmick[g_nBeInControl].fWidth -= 5;
						g_aGimmick[g_nBeInControl].fGoalX -= 5.0f / 2.0f;
					}
					if(GetKeyboardRepeat(DIK_RIGHT))
					{
						g_aGimmick[g_nBeInControl].fWidth += 5;
						g_aGimmick[g_nBeInControl].fGoalX += 5.0f / 2.0f;
					}
					if(GetKeyboardRepeat(DIK_DOWN))
					{
						g_aGimmick[g_nBeInControl].fWidth -= 10;
						g_aGimmick[g_nBeInControl].fGoalX -= 10.0f / 2.0f;
					}
					if(GetKeyboardRepeat(DIK_UP))
					{
						g_aGimmick[g_nBeInControl].fWidth += 10;
						g_aGimmick[g_nBeInControl].fGoalX += 10.0f / 2.0f;
					}
					break;
				}
				case 5:
				{
					PrintDebugProc("矢印キー【左】、【右】   【+- 5】\n");
					PrintDebugProc("矢印キー【上】、【下】   【+- 10】\n");
					PrintDebugProc("   【4】   【サイズ_Y】      : %f\n\n");
					if(GetKeyboardRepeat(DIK_LEFT))
						g_aGimmick[g_nBeInControl].fHeight -= 5;
					if(GetKeyboardRepeat(DIK_RIGHT))
						g_aGimmick[g_nBeInControl].fHeight += 5;
					if(GetKeyboardRepeat(DIK_DOWN))
						g_aGimmick[g_nBeInControl].fHeight -= 10;
					if(GetKeyboardRepeat(DIK_UP))
						g_aGimmick[g_nBeInControl].fHeight += 10;
					break;
				}
				default:
					break;
				}
				break;
			}
			default:
				break;
		}

	// 修改最近选中的机关属性值
	// 再次生成运动轨迹及位置信息
	if(g_nBeInControl != -1)
	{
		/***实时更新轨迹必须在gimmick绑定鼠标位置之前实现***/
		// 针对新建的机关进行模拟轨迹构建
		SetupGimmickTrack(g_nBeInControl);
		// 重排轨迹线
		*GetCleanupSwitch() = false;
		g_aGimmick[g_nBeInControl].pos = g_aGimmick[g_nBeInControl].posSave;
		if(*GetIsPlay())
			g_nBeInControl = -1;
	}
}

// ギミックの移動設定
void GimmickMode(int nCntRoopTime, int nCntGim, int nType)
{
	switch (nType)
	{
	case GIMMICK_ATTACK_001:
		TrackData(nCntGim);
		// 当前循环节点超过机关开始时间，并且当前时间不超过整个时间轴最大时间
		if(nCntRoopTime >= g_aGimmick[nCntGim].fTimeStart * UPDATA_FRAME && nCntRoopTime < g_aGimmick[nCntGim].fTimeEnd * UPDATA_FRAME)
		{
			float fTempPosX = g_aGimmick[nCntGim].posSave.x + g_aGimmick[nCntGim].fRadiusDis;
			float fTempPosY = g_aGimmick[nCntGim].posSave.y;
			g_aGimmick[nCntGim].pos.x = fTempPosX + cos(D3DX_PI / 180 * g_aGimmick[nCntGim].fDegree) * g_aGimmick[nCntGim].fRadiusDis;
			g_aGimmick[nCntGim].pos.y = fTempPosY + sin(D3DX_PI / 180 * g_aGimmick[nCntGim].fDegree) * g_aGimmick[nCntGim].fRadiusDis;
			g_aGimmick[nCntGim].fDegreeSpeed = GIMMICK_01_DEGREE_MIN / (float)((g_aGimmick[nCntGim].fTimeEnd - g_aGimmick[nCntGim].fTimeStart) * UPDATA_FRAME);
			g_aGimmick[nCntGim].fDegree += g_aGimmick[nCntGim].fDegreeSpeed;

			if(g_aGimmick[nCntGim].fDegree >= GIMMICK_01_DEGREE_MAX)
			{
				g_aGimmick[nCntGim].fDegree = GIMMICK_01_DEGREE_MIN;
			}
		}
		// 毎回移動した　位置を保存する
		break;

	case GIMMICK_ATTACK_002:
		TrackData(nCntGim);
		// 当前循环节点超过机关开始时间，并且当前时间不超过整个时间轴最大时间
		if(nCntRoopTime >= g_aGimmick[nCntGim].fTimeStart * UPDATA_FRAME && nCntRoopTime < g_aGimmick[nCntGim].fTimeEnd * UPDATA_FRAME)
		{
			float fTempPosX = g_aGimmick[nCntGim].posSave.x + g_aGimmick[nCntGim].fRadiusDis;
			float fTempPosY = g_aGimmick[nCntGim].posSave.y;
			g_aGimmick[nCntGim].pos.x = fTempPosX + cos(D3DX_PI / 180 * g_aGimmick[nCntGim].fDegree) * g_aGimmick[nCntGim].fRadiusDis;
			g_aGimmick[nCntGim].pos.y = fTempPosY + sin(D3DX_PI / 180 * g_aGimmick[nCntGim].fDegree) * g_aGimmick[nCntGim].fRadiusDis;
			g_aGimmick[nCntGim].fDegreeSpeed = GIMMICK_02_DEGREE_MIN / (float)((g_aGimmick[nCntGim].fTimeEnd - g_aGimmick[nCntGim].fTimeStart) * UPDATA_FRAME);
			g_aGimmick[nCntGim].fDegree -= g_aGimmick[nCntGim].fDegreeSpeed;

			if(g_aGimmick[nCntGim].fDegree <= GIMMICK_02_DEGREE_MIN)
			{
				g_aGimmick[nCntGim].fDegree = GIMMICK_02_DEGREE_MAX;
			}
		}
		//TrackData(nCntGim);
		break;

	case GIMMICK_HELP_003:
		TrackData(nCntGim);
		// 在规定的起始和结束时间内运动
		if(nCntRoopTime >= g_aGimmick[nCntGim].fTimeStart * UPDATA_FRAME && nCntRoopTime < g_aGimmick[nCntGim].fTimeEnd * UPDATA_FRAME)
		{
			g_aGimmick[nCntGim].pos.y += g_aGimmick[nCntGim].speed.y;
		}
		if(nCntRoopTime >= g_aGimmick[nCntGim].fTimeEnd * UPDATA_FRAME)
			g_aGimmick[nCntGim].pos.y = g_aGimmick[nCntGim].fGoalY;			// float型の誤差補正

		//TrackData(nCntGim);
		break;

	case GIMMICK_HELP_004:
		TrackData(nCntGim);
		// 在规定的起始和结束时间内运动
		if(nCntRoopTime >= g_aGimmick[nCntGim].fTimeStart * UPDATA_FRAME && nCntRoopTime < g_aGimmick[nCntGim].fTimeEnd * UPDATA_FRAME)
		{
			g_aGimmick[nCntGim].pos.y -= g_aGimmick[nCntGim].speed.y;
		}
		if(nCntRoopTime >= g_aGimmick[nCntGim].fTimeEnd * UPDATA_FRAME)
			g_aGimmick[nCntGim].pos.y = g_aGimmick[nCntGim].fGoalY;			// float型の誤差補正
		//TrackData(nCntGim);
		break;

	case GIMMICK_HELP_005:
		TrackData(nCntGim);
		// 在规定的起始和结束时间内运动
		if(nCntRoopTime >= g_aGimmick[nCntGim].fTimeStart * UPDATA_FRAME && nCntRoopTime < g_aGimmick[nCntGim].fTimeEnd * UPDATA_FRAME)
		{
			g_aGimmick[nCntGim].pos.x += g_aGimmick[nCntGim].speed.x;
		}
		//TrackData(nCntGim);
		break;

	case GIMMICK_HELP_006:
		TrackData(nCntGim);
		// 在规定的起始和结束时间内运动
		if(nCntRoopTime >= g_aGimmick[nCntGim].fTimeStart * UPDATA_FRAME && nCntRoopTime < g_aGimmick[nCntGim].fTimeEnd * UPDATA_FRAME)
		{
			g_aGimmick[nCntGim].pos.x -= g_aGimmick[nCntGim].speed.x;
		}
		//TrackData(nCntGim);
		break;
	default:
		break;
	}
}

void TrackData(int nCntGim)
{
	// 从开始运动保存到最后的时间点
	// 保存每帧的位置信息 从当前点最后一个节点往后保存
	g_aGimmick[nCntGim].track[g_aGimmick[nCntGim].nLastTrack] = g_aGimmick[nCntGim].pos;
	g_aGimmick[nCntGim].nLastTrack++;
}
// ギミックと壁のあたり判定
bool IsGimmickHitWall(int nCntGim, float fMoveX, float fMoveY)
{
	for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if(g_aWallDataGim[nCntWall].bUse)
		{
			if(	g_aGimmick[nCntGim].fMaxX - fMoveX > g_aWallDataGim[nCntWall].fMinX &&
				g_aGimmick[nCntGim].fMinX - fMoveX < g_aWallDataGim[nCntWall].fMaxX &&
				g_aGimmick[nCntGim].fMaxY - fMoveY > g_aWallDataGim[nCntWall].fMinY &&
				g_aGimmick[nCntGim].fMinY - fMoveY < g_aWallDataGim[nCntWall].fMaxY)
				return true;
		}
	}
	return false;
}
// ギミックとプレーヤーのあたり判定
bool IsGimmickHitPlayer(int nCntGim)
{
	if(g_aGimmick[nCntGim].nMode != MODE_HELP)
	{
		for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{
			if(g_getPlayer[nCntPlayer].bUse)
			{
				if(	(g_aGimmick[nCntGim].pos.x - g_getPlayer[nCntPlayer].pos.x) * (g_aGimmick[nCntGim].pos.x - g_getPlayer[nCntPlayer].pos.x) +
					(g_aGimmick[nCntGim].pos.y - g_getPlayer[nCntPlayer].pos.y) * (g_aGimmick[nCntGim].pos.y - g_getPlayer[nCntPlayer].pos.y) <=
					(g_aGimmick[nCntGim].fRadius + g_getPlayer[nCntPlayer].fRadius) * (g_aGimmick[nCntGim].fRadius + g_getPlayer[nCntPlayer].fRadius))
					return true;
			}
		}
	}
	return false;
}
// 時間帯の優先順位を入力
void UpDataPriority(void)
{
	// 保存最新时间轴排列后的时间先后分布
	for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
	{
		if(g_aGimmick[nCntGim].bUse == true)
		{
			for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++)
			{
				if(g_aPlaybarGim[nCntPlayBar].bUse == true)
				{
					if(g_aPlaybarGim[nCntPlayBar].nPriority == PRIORITY_FIRST)
						g_aGimmick[nCntGim].nPriorityNum01 = nCntPlayBar;
					if(g_aPlaybarGim[nCntPlayBar].nPriority == PRIORITY_SECOND)
						g_aGimmick[nCntGim].nPriorityNum02 = nCntPlayBar;
					if(g_aPlaybarGim[nCntPlayBar].nPriority == PRIORITY_THIRD)
						g_aGimmick[nCntGim].nPriorityNum03 = nCntPlayBar;
				}
			}
		}
	}
}


void SetupGimmickTrack(int nCntGim)
{
	// ギミックの移動トラック 初期化
	//for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
	{
		if(g_aGimmick[nCntGim].bUse == true)
		{
			// 重新排列需要重置最后一针的时间点，也就是从头开始记录
			g_aGimmick[nCntGim].nLastTrack = 0;
			if(	g_aGimmick[nCntGim].nType == GIMMICK_ATTACK_001)
			{
				g_aGimmick[nCntGim].fDegree = GIMMICK_01_DEGREE_MIN;
				for(int nCntRoopTime = 0; nCntRoopTime <= TIME_15 * UPDATA_FRAME; nCntRoopTime++)
				{
					GimmickMode(nCntRoopTime, nCntGim, GIMMICK_ATTACK_001);
				}
			}

			if(	g_aGimmick[nCntGim].nType == GIMMICK_ATTACK_002)
			{
				g_aGimmick[nCntGim].fDegree = GIMMICK_01_DEGREE_MAX;
				for(int nCntRoopTime = 0; nCntRoopTime <= TIME_15 * UPDATA_FRAME; nCntRoopTime++)
				{
					GimmickMode(nCntRoopTime, nCntGim, GIMMICK_ATTACK_002);
				}
			}
			
			if(	g_aGimmick[nCntGim].nType == GIMMICK_HELP_003)
			{
				g_aGimmick[nCntGim].speed.y = abs(g_aGimmick[nCntGim].pos.y - g_aGimmick[nCntGim].fGoalY) / (float)((g_aGimmick[nCntGim].fTimeEnd - g_aGimmick[nCntGim].fTimeStart) * UPDATA_FRAME);
				for(int nCntRoopTime = 0; nCntRoopTime <= TIME_15 * UPDATA_FRAME; nCntRoopTime++)
				{
					GimmickMode(nCntRoopTime, nCntGim, GIMMICK_HELP_003);
				}
				//// float型の誤差補正
				//g_aGimmick[nCntGim].track[(int)(TIME_15 * UPDATA_FRAME)].y = g_aGimmick[nCntGim].fGoalY;
			}

			if(	g_aGimmick[nCntGim].nType == GIMMICK_HELP_004)
			{
				g_aGimmick[nCntGim].speed.y = abs(g_aGimmick[nCntGim].pos.y - g_aGimmick[nCntGim].fGoalY) / (float)((g_aGimmick[nCntGim].fTimeEnd - g_aGimmick[nCntGim].fTimeStart) * UPDATA_FRAME);
				for(int nCntRoopTime = 0; nCntRoopTime <= TIME_15 * UPDATA_FRAME; nCntRoopTime++)
				{
					GimmickMode(nCntRoopTime, nCntGim, GIMMICK_HELP_004);
				}
			}

			if(	g_aGimmick[nCntGim].nType == GIMMICK_HELP_005)
			{
				g_aGimmick[nCntGim].speed.x = abs(g_aGimmick[nCntGim].pos.x - g_aGimmick[nCntGim].fGoalX) / (float)((g_aGimmick[nCntGim].fTimeEnd - g_aGimmick[nCntGim].fTimeStart) * UPDATA_FRAME);
				for(int nCntRoopTime = 0; nCntRoopTime <= TIME_15 * UPDATA_FRAME; nCntRoopTime++)
				{
					GimmickMode(nCntRoopTime, nCntGim, GIMMICK_HELP_005);
				}
			}

			if(	g_aGimmick[nCntGim].nType == GIMMICK_HELP_006)
			{
				g_aGimmick[nCntGim].speed.x = abs(g_aGimmick[nCntGim].pos.x - g_aGimmick[nCntGim].fGoalX) / (float)((g_aGimmick[nCntGim].fTimeEnd - g_aGimmick[nCntGim].fTimeStart) * UPDATA_FRAME);
				for(int nCntRoopTime = 0; nCntRoopTime <= TIME_15 * UPDATA_FRAME; nCntRoopTime++)
				{
					GimmickMode(nCntRoopTime, nCntGim, GIMMICK_HELP_006);
				}
			}
		}
	}
}
// ギミックのデータゲット
GIMMICK *GetGimmickData(void)
{
	return g_aGimmick;
}

// 読み込むデータを初期化
// 根据读取的预存的类型进行初始化设置。
void InitDataGimmick(void)
{
	for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
	{
		if(g_aGimmick[nCntGim].bUse == true)
		{
			switch (g_aGimmick[nCntGim].nType)
			{
			case GIMMICK_ATTACK_001:
				g_aGimmick[nCntGim].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aGimmick[nCntGim].posSave = g_aGimmick[nCntGim].pos;
				g_aGimmick[nCntGim].fRadius = GIMMICK_01_RADIUS;
				g_aGimmick[nCntGim].fDegree = GIMMICK_01_DEGREE_MIN;
				g_aGimmick[nCntGim].nMode = MODE_ATTACK;

				break;

			case GIMMICK_ATTACK_002:
				g_aGimmick[nCntGim].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aGimmick[nCntGim].posSave = g_aGimmick[nCntGim].pos;
				g_aGimmick[nCntGim].fRadius = GIMMICK_02_RADIUS;
				g_aGimmick[nCntGim].fDegree = GIMMICK_02_DEGREE_MIN;
				g_aGimmick[nCntGim].nMode = MODE_ATTACK;
				break;

			case GIMMICK_HELP_003:
				g_aGimmick[nCntGim].posSave = g_aGimmick[nCntGim].pos;
				g_aGimmick[nCntGim].nMode = MODE_HELP;
				break;

			case GIMMICK_HELP_004:
				g_aGimmick[nCntGim].posSave = g_aGimmick[nCntGim].pos;
				g_aGimmick[nCntGim].nMode = MODE_HELP;
				break;

			case GIMMICK_HELP_005:
				g_aGimmick[nCntGim].posSave = g_aGimmick[nCntGim].pos;
				g_aGimmick[nCntGim].nMode = MODE_HELP;
				break;

			case GIMMICK_HELP_006:
				g_aGimmick[nCntGim].posSave = g_aGimmick[nCntGim].pos;
				g_aGimmick[nCntGim].nMode = MODE_HELP;
				break;
			default:
				break;
			}
		}
	}
}
// ギミックの削除
void DeleteGimmick(void)
{
	if(IsMouseLeftTriggered())
	{
		for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
		{
			if(g_aGimmick[nCntGim].bUse == true)
			{
				float fSizeX = 0, fSizeY = 0;
				fSizeX = g_aGimmick[nCntGim].fWidth;
				fSizeY = g_aGimmick[nCntGim].fHeight;
				// 这里不能用fMaxX或者fMinX来做界限判断，此时的两个值已经被修改为最后一针位置的范围了
				if(	g_aGimmick[nCntGim].pos.x + fSizeX / 2 > *GetMouX() &&
					g_aGimmick[nCntGim].pos.x - fSizeX / 2 < *GetMouX() &&
					g_aGimmick[nCntGim].pos.y + fSizeY / 2 > *GetMouY() &&
					g_aGimmick[nCntGim].pos.y - fSizeY / 2 < *GetMouY())
				{
					g_aGimmick[nCntGim].bUse = false;
				}
			}
		}
	}
}
// ギミックの時間順番判断
bool IsGimmickLock(int* pCntGim, bool* pIsLock, int* pItemNum)
{
	if(IsMouseRightTriggered())
	{
		for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
		{
			if(g_aGimmick[nCntGim].bUse == true)
			{
				if(	g_aGimmick[nCntGim].fMaxX > *GetMouX() &&
					g_aGimmick[nCntGim].fMinX < *GetMouX() &&
					g_aGimmick[nCntGim].fMaxY > *GetMouY() &&
					g_aGimmick[nCntGim].fMinY < *GetMouY())
				{
					// 固定しない + アイテムを使える場合、固定する
					if(!g_aGimmick[nCntGim].bIsLock && *pItemNum > 0)
					{
						g_aGimmick[nCntGim].bIsLock = true;
						g_aGimmick[nCntGim].fLoopCount = 0;
						*pCntGim = nCntGim;
						*pIsLock = true;
						return true;
					}

					// 固定した場合、外す
					if(g_aGimmick[nCntGim].bIsLock == true)
					{
						g_aGimmick[nCntGim].bIsLock = false;
						g_aGimmick[nCntGim].fLoopCount = 0;
						*pCntGim = nCntGim;
						*pIsLock = false;
						return true;
					}
				}
			}
		}
	}
	return false;
}