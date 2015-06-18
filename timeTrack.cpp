/*****************************************************************************
 *
 * 時間軌跡の表示処理 [timeTrack.cpp]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 * 
 ******************************************************************************/
#include "timeTrack.h"
#include "input.h"
#include "gimmick.h"
#include "playbar.h"
#include "gameManager.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
////////////////////////////
// 默认位置和尺寸
////////////////////////////
#define	TIMETRACK_POS_X				(500)									// 默认の位置(X座標)
#define	TIMETRACK_POS_Y				(500)									// 默认の位置(Y座標)
#define	TIMETRACK_SIZE_X			(35)									// 默认の幅
#define	TIMETRACK_SIZE_Y			(35)									// 默认の高さ
#define TIMETRACK_SWITCH_SPEED		(10)									// 交換のすばやさ
#define TIMETRACK_SCL_SPEED			(0.95f)									// スケールのすばやさ
#define TIMETRACK_SPACE				(3)										// 時間軌跡始の間隔
#define TIMETRACK_SIZE				(0.5f)									// 時間軌跡始の大きさ
#define TIMETRACK_BEGINSIZE			(0.1f)									// 時間軌跡始めるサイズ（ゲーム停止）
#define TIMETRACK_ALPHA				(100)									// 時間軌跡の透明度
////////////////////////////
// 8列1行图组动画
////////////////////////////
#define	TEX_PATTERN_DIVIDE_X8		(8)												// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y1		(1)												// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)
#define	NUM_ANIM_PATTERN_X8_Y1		(TEX_PATTERN_DIVIDE_X8 * TEX_PATTERN_DIVIDE_Y1)	// アニメーションのパターン数

////////////////////////////
// 动画播放帧数
////////////////////////////
#define	TIME_CHANGE_TIMETRACK_MOTION	(9)											// アニメーションの切り替わるタイミング

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTimeTrack(LPDIRECT3DDEVICE9 pDevice);
void SetColorTimeTrack(int nIdxTimeTrack, D3DCOLOR col_3);

void SetVertexTimeTrack(int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组polygon地址
						float scaleX,					// テクスチャのX軸のスケール		X轴的比例参数
						float scaleY);					// テクスチャのY軸のスケール		Y轴的比例参数

void SetTextureTimeTrack(int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组地址
						int nPatternAnim,				// 今のテクスチャの番号				记录当前显示帧
						int numAnimPattern, 			// テクスチャ全部の個数				记录当前对应的组成图组的图片数量
						int texPatternDivideX,			// 横のテクスチャの個数				记录当前图组X轴图片数量
						int texPatternDivideY );		// 縦のテクスチャの個数				记录当前图组Y轴图片数量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTextureTimeTrack[MAX_TIMETRACK] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTimeTrack = NULL;						// 頂点バッファインターフェースへのポインタ

TIMETRACK				g_aTimeTrack[MAX_TIMETRACK];		// 木ワーク

// 计数器
int						g_nCounterAnimTimeTrack;
//记录HP图片帧
int						g_nPatternAnimTimeTrack;
int						g_nAlpha;							// 透明度
float					g_fTimeTrackNum;					// 時間軌跡の個数
float					g_fScl;								// 大きさ
bool					g_bCleanupSwitch;					// 是否清理屏幕
PLAYBAR					*g_aPlaybarTT;						// 時間軸のデータゲット
GIMMICK					*g_aGimmickTT;						// ギミックのデータゲット
//////////////////////
// 贴图的种类路径
//////////////////////
const char *c_pTextureTimeTrack[TIMETRACK_TYPE_MAX]=		// 不同模式下的背景切换
{
	"data/TEXTURE/tracks/track_R.png",						// Red
	"data/TEXTURE/tracks/track_G.png",						// Green
	"data/TEXTURE/tracks/track_B.png",						// Blue
	"data/TEXTURE/tracks/track_Gray.png",					// Gray
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTimeTrack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexTimeTrack(pDevice);

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < TIMETRACK_TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,								// デバイスへのポインタ
									c_pTextureTimeTrack[nCntTex],			// ファイルの名前
									&g_apD3DTextureTimeTrack[nCntTex]);	// 読み込むメモリー
	}

	for(int nCntTrack = 0; nCntTrack < MAX_TIMETRACK; nCntTrack++)
	{
		g_aTimeTrack[nCntTrack].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTimeTrack[nCntTrack].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aTimeTrack[nCntTrack].fWidth = 0;
		g_aTimeTrack[nCntTrack].fHeight = 0;
		g_aTimeTrack[nCntTrack].fSpeed = 0;
		g_aTimeTrack[nCntTrack].fScl = 1.0f;

		g_aTimeTrack[nCntTrack].bUse = false;

		g_aTimeTrack[nCntTrack].nCounterAnim = 0;
		g_aTimeTrack[nCntTrack].nPatternAnim = 0;
		g_aTimeTrack[nCntTrack].nType = -1;
		g_aTimeTrack[nCntTrack].nAlpha = 255;
	}

	g_nCounterAnimTimeTrack = 0;
	g_nPatternAnimTimeTrack = 0;
	g_fTimeTrackNum = 0;
	g_nAlpha = 255;
	g_fScl = TIMETRACK_BEGINSIZE;
	g_bCleanupSwitch = false;
	g_aPlaybarTT = GetPlaybar();	
	g_aGimmickTT = GetGimmickData();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitTimeTrack(void)
{
	for(int nCntTex = 0; nCntTex < MAX_TIMETRACK; nCntTex++)
	{
		if(g_apD3DTextureTimeTrack[nCntTex] != NULL)
		{// テクスチャの開放
			g_apD3DTextureTimeTrack[nCntTex]->Release();
			g_apD3DTextureTimeTrack[nCntTex] = NULL;
		}
	}

	if(g_pD3DVtxBuffTimeTrack != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffTimeTrack->Release();
		g_pD3DVtxBuffTimeTrack = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimeTrack(void)
{
	//// 每次更新都初始化轨迹线的使用
	//for(int nCntTrack = 0; nCntTrack < MAX_TIMETRACK; nCntTrack++)
	//{
	//	if(g_aTimeTrack[nCntTrack].bUse)
	//	{
	//		g_aTimeTrack[nCntTrack].bUse = false;
	//	}
	//}

	// 每一次游戏停止都只运行一次
	if(!*GetIsPlay())
	{
		// 清空界面中的轨迹痕迹
		if(!g_bCleanupSwitch)
		{
			for(int nCntTrack = 0; nCntTrack < MAX_TIMETRACK; nCntTrack++)
			{
				if(g_aTimeTrack[nCntTrack].bUse)
					g_aTimeTrack[nCntTrack].bUse = false;
			}
			g_bCleanupSwitch = true;

			// 数值初始化
			g_nAlpha = TIMETRACK_ALPHA;				// 透明数值
			//g_fScl = TIMETRACK_BEGINSIZE;			// 放大系数
			//g_fTimeTrackNum = 0;					// 目前使用的轨迹数
		}
	}
	// 每一次游戏开始都只运行一次
	else if(!*GetIsStop())
	{
		// 清空界面中的轨迹痕迹
		if(g_bCleanupSwitch == true)
		{
			for(int nCntTrack = 0; nCntTrack < MAX_TIMETRACK; nCntTrack++)
			{
				if(g_aTimeTrack[nCntTrack].bUse)
				{
					g_aTimeTrack[nCntTrack].bUse = false;
				}
			}
			g_bCleanupSwitch = false;
		}
	}

	if(!*GetIsStop())
	{
		for(int nCntTrack = 0; nCntTrack < MAX_TIMETRACK; nCntTrack++)
		{
			if(g_aTimeTrack[nCntTrack].bUse == true)
			{
				if(g_aTimeTrack[nCntTrack].nAlpha > 0)
					g_aTimeTrack[nCntTrack].nAlpha -= TIMETRACK_SWITCH_SPEED;
				g_aTimeTrack[nCntTrack].fScl *= TIMETRACK_SCL_SPEED;

				if(g_aTimeTrack[nCntTrack].nAlpha <= 0)
					g_aTimeTrack[nCntTrack].bUse = false;

				SetVertexTimeTrack(nCntTrack, g_aTimeTrack[nCntTrack].fScl, g_aTimeTrack[nCntTrack].fScl);
				SetTextureTimeTrack(nCntTrack, 0, 1, 1, 1);
				SetColorTimeTrack(nCntTrack, D3DCOLOR_RGBA(255, 255, 255, g_aTimeTrack[nCntTrack].nAlpha));
			}
		}
	}
	else
	{
		SetupTimeTrack();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTimeTrack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffTimeTrack, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntTrack = 0; nCntTrack < MAX_TIMETRACK; nCntTrack++)
	{
		if(g_aTimeTrack[nCntTrack].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureTimeTrack[g_aTimeTrack[nCntTrack].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntTrack * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexTimeTrack(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_TIMETRACK,		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,									// 頂点バッファの使用法　
												FVF_VERTEX_2D,										// 使用する頂点フォーマット
												D3DPOOL_MANAGED,									// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffTimeTrack,							// 頂点バッファインターフェースへのポインタ
												NULL)))												// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTimeTrack->Lock(0, 0, (void**)&pVtx, 0);


		for(int nCntTrack = 0; nCntTrack < MAX_TIMETRACK; nCntTrack++, pVtx += 4)
		{
		// 頂点座標の設定
			//pVtx[0].vtx = D3DXVECTOR3(g_timeTrack[TimeTrack_MAX].pos.x - TimeTrack_SIZE_X / 2, g_timeTrack[TimeTrack_MAX].pos.y - TimeTrack_SIZE_Y / 2, 0.0f);
			//pVtx[1].vtx = D3DXVECTOR3(g_timeTrack[TimeTrack_MAX].pos.x + TimeTrack_SIZE_X / 2, g_timeTrack[TimeTrack_MAX].pos.y - TimeTrack_SIZE_Y / 2, 0.0f);
			//pVtx[2].vtx = D3DXVECTOR3(g_timeTrack[TimeTrack_MAX].pos.x - TimeTrack_SIZE_X / 2, g_timeTrack[TimeTrack_MAX].pos.y + TimeTrack_SIZE_Y / 2, 0.0f);
			//pVtx[3].vtx = D3DXVECTOR3(g_timeTrack[TimeTrack_MAX].pos.x + TimeTrack_SIZE_X / 2, g_timeTrack[TimeTrack_MAX].pos.y + TimeTrack_SIZE_Y / 2, 0.0f);

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
			g_pD3DVtxBuffTimeTrack->Unlock();
		}
	}

	return S_OK;
}



//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorTimeTrack(int nIdxTimeTrack, D3DCOLOR col_3)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffTimeTrack->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdxTimeTrack * 4);

	// 頂点座標の設定
	pVtx[0].diffuse =
	pVtx[1].diffuse =
	pVtx[2].diffuse =
	pVtx[3].diffuse = col_3;

	// 頂点データをアンロックする
	g_pD3DVtxBuffTimeTrack->Unlock();
}

void SetVertexTimeTrack(int nIdx,					//记录当前对应的图组polygon地址
						float scaleX,				//X轴的放大参数
						float scaleY)				//自身Y轴的大小参数
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffTimeTrack->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(g_aTimeTrack[nIdx].pos.x - g_aTimeTrack[nIdx].fWidth / 2 * scaleX, g_aTimeTrack[nIdx].pos.y - g_aTimeTrack[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_aTimeTrack[nIdx].pos.x + g_aTimeTrack[nIdx].fWidth / 2 * scaleX, g_aTimeTrack[nIdx].pos.y - g_aTimeTrack[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_aTimeTrack[nIdx].pos.x - g_aTimeTrack[nIdx].fWidth / 2 * scaleX, g_aTimeTrack[nIdx].pos.y + g_aTimeTrack[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_aTimeTrack[nIdx].pos.x + g_aTimeTrack[nIdx].fWidth / 2 * scaleX, g_aTimeTrack[nIdx].pos.y + g_aTimeTrack[nIdx].fHeight / 2 * scaleY, 0.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffTimeTrack->Unlock();
}


void SetTextureTimeTrack(	int nIdx,						//记录当前对应的图组地址
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
		g_pD3DVtxBuffTimeTrack->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pD3DVtxBuffTimeTrack->Unlock();
	}
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetTimeTrack(D3DXVECTOR3 pos, D3DXCOLOR col, int type)
{
	for(int nCntTrack = 0; nCntTrack < MAX_TIMETRACK; nCntTrack++)
	{
		if(!g_aTimeTrack[nCntTrack].bUse)
		{
			g_aTimeTrack[nCntTrack].pos = pos;
			g_aTimeTrack[nCntTrack].fWidth = TIMETRACK_SIZE_X;
			g_aTimeTrack[nCntTrack].fHeight = TIMETRACK_SIZE_Y;
			g_aTimeTrack[nCntTrack].fSpeed = (float)SCREEN_WIDTH / (float)(TIME_15 * 60);
			g_aTimeTrack[nCntTrack].bUse = true;
			g_aTimeTrack[nCntTrack].nType = type;
			g_aTimeTrack[nCntTrack].nCounterAnim = 0;
			g_aTimeTrack[nCntTrack].nPatternAnim = 0;
			g_aTimeTrack[nCntTrack].nAlpha = 255;
			g_aTimeTrack[nCntTrack].fScl = 1;

			//SetVertexTimeTrack(nCntTrack, 1, 1);
			
			// 頂点カラーの設定
			SetColorTimeTrack(nCntTrack, col);
			//g_fTimeTrackNum ++;
			break;
		}
	}
}

// 軌跡の表示
void SetupTimeTrack(void)
{
	int nCntTraShow = 0;
	int nLoopTimes = 0;

	// 不运动的时候显示轨迹
	for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
	{
		//g_fScl = TIMETRACK_BEGINSIZE;		// 放大系数
		//nLoopTimes = 0;						// 根据轨迹点数量控制循环次数 每一个机关的开始初始一次

		if(g_aGimmickTT[nCntGim].bUse == true)
		{
			g_fScl = TIMETRACK_BEGINSIZE;		// 放大系数
			nLoopTimes = 0;						// 根据轨迹点数量控制循环次数 每一个机关的开始初始一次
			// 当前时间轴第1节时间
			int nStart_01 = (int)(g_aPlaybarTT[g_aGimmickTT[nCntGim].nPriorityNum01].fTimeStart * UPDATA_FRAME);
			int nEnd_01 = (int)(g_aPlaybarTT[g_aGimmickTT[nCntGim].nPriorityNum01].fTimeEnd * UPDATA_FRAME);

			// 当前时间轴第2节时间
			int nStart_02 = (int)(g_aPlaybarTT[g_aGimmickTT[nCntGim].nPriorityNum02].fTimeStart * UPDATA_FRAME);
			int nEnd_02 = (int)(g_aPlaybarTT[g_aGimmickTT[nCntGim].nPriorityNum02].fTimeEnd * UPDATA_FRAME);

			// 当前时间轴第3节时间
			int nStart_03 = (int)(g_aPlaybarTT[g_aGimmickTT[nCntGim].nPriorityNum03].fTimeStart * UPDATA_FRAME);
			int nEnd_03 = (int)(g_aPlaybarTT[g_aGimmickTT[nCntGim].nPriorityNum03].fTimeEnd * UPDATA_FRAME);

			for(int nCntTrack = nStart_01; nCntTrack < nEnd_01; nCntTrack += TIMETRACK_SPACE)
			{
				D3DXVECTOR3 fPos = g_aGimmickTT[nCntGim].track[nCntTrack];
				if(nStart_01 == TIME_0 * UPDATA_FRAME)
					SetTimeTrack(fPos, D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
				if(nStart_01 == TIME_5 * UPDATA_FRAME)
					SetTimeTrack(fPos, D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_G);
				if(nStart_01 == TIME_10 * UPDATA_FRAME)
					SetTimeTrack(fPos, D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_B);
				// 累计每个机关创建的轨迹点
				nLoopTimes++;
			}
			for(int nCntTrack = nStart_02; nCntTrack < nEnd_02; nCntTrack += TIMETRACK_SPACE)
			{
				D3DXVECTOR3 fPos = g_aGimmickTT[nCntGim].track[nCntTrack];
				if(nStart_02 == TIME_0 * UPDATA_FRAME)
					SetTimeTrack(fPos, D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
				if(nStart_02 == TIME_5 * UPDATA_FRAME)
					SetTimeTrack(fPos, D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_G);
				if(nStart_02 == TIME_10 * UPDATA_FRAME)
					SetTimeTrack(fPos, D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_B);
				// 累计每个机关创建的轨迹点
				nLoopTimes++;
			}
			for(int nCntTrack = nStart_03; nCntTrack < nEnd_03; nCntTrack += TIMETRACK_SPACE)
			{
				D3DXVECTOR3 fPos = g_aGimmickTT[nCntGim].track[nCntTrack];
				if(nStart_03 == TIME_0 * UPDATA_FRAME)
					SetTimeTrack(fPos, D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_R);
				if(nStart_03 == TIME_5 * UPDATA_FRAME)
					SetTimeTrack(fPos, D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_G);
				if(nStart_03 == TIME_10 * UPDATA_FRAME)
					SetTimeTrack(fPos, D3DCOLOR_RGBA(255, 255, 255, 255), TIMETRACK_B);
				// 累计每个机关创建的轨迹点
				nLoopTimes++;
			}

			// 统计需要循环的次数，每一个机关总得创建了多少个轨迹点就循环多少次

			// 显示轨迹 nLoopTimes根据轨迹点数量控制循环次数
			for(int nTrack = 0; nTrack < nLoopTimes; nTrack++)
			{
				if(g_aTimeTrack[nCntTraShow].bUse)
				{
					//if(g_nAlpha > 0)
					//	g_nAlpha --;
					//else
						g_nAlpha = TIMETRACK_ALPHA;

					if(g_fScl < TIMETRACK_SIZE)
						g_fScl += (TIMETRACK_SIZE - TIMETRACK_BEGINSIZE) / nLoopTimes;

					g_aTimeTrack[nCntTraShow].fScl = g_fScl;
					g_aTimeTrack[nCntTraShow].nAlpha = g_nAlpha;

					SetVertexTimeTrack(nCntTraShow, g_aTimeTrack[nCntTraShow].fScl, g_aTimeTrack[nCntTraShow].fScl);
					SetTextureTimeTrack(nCntTraShow, 0, 1, 1, 1);
					SetColorTimeTrack(nCntTraShow, D3DCOLOR_RGBA(255, 255, 255, g_nAlpha));
				}
				// 累积已被使用的轨迹点编号
				nCntTraShow++;
			}
		}
	}
}

bool *GetCleanupSwitch(void)
{
	return &g_bCleanupSwitch;
}
// 時間軌跡のデータゲット
TIMETRACK *GetTimeTrack(void)
{
	return g_aTimeTrack;
}