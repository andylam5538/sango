/*****************************************************************************
 *
 * 壁の表示処理 [wall.cpp]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#include "wall.h"
#include "input.h"
#include "load.h"
#include "save.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
////////////////////////////
// 壁のサイズ
////////////////////////////
#define	WALL_01_POS_X		(WALL_01_SIZE_X / 2)					// 壁の位置(X座標)
#define	WALL_01_POS_Y		(700.0f)								// 壁の位置(Y座標)
#define	WALL_01_SIZE_X		(100.0f)								// 壁の幅
#define	WALL_01_SIZE_Y		(100.0f)								// 壁の高さ

#define	WALL_02_SIZE_X		(50.0f)									// 壁の幅
#define	WALL_02_SIZE_Y		(50.0f)									// 壁の高さ

#define	WALL_SLOPE_UP_45_SIZE_X		(200.0f)						//坂道ブロックの幅
#define	WALL_SLOPE_UP_45_SIZE_Y		(200.0f)						//坂道ブロックの高さ
#define	WALL_SLOPE_DOWN_45_SIZE_X	(200.0f)						//坂道ブロックの幅
#define	WALL_SLOPE_DOWN_45_SIZE_Y	(200.0f)						//坂道ブロックの高さ

#define	WALL_GOAL_SIZE_X			(50.0f)							// 壁の幅
#define	WALL_GOAL_SIZE_Y			(200.0f)						// 壁の高さ

#define	WALL_DOOR_SIZE_X			(20.0f)							// ドアの幅
#define	WALL_DOOR_SIZE_Y			(100.0f)						// ドアの高さ
////////////////////////////
// 1列6行图组动画
////////////////////////////
#define	TEX_PATTERN_DIVIDE_X1		(1)												// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y6		(6)												// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)
#define	NUM_ANIM_PATTERN_X1_Y6		(TEX_PATTERN_DIVIDE_X1 * TEX_PATTERN_DIVIDE_Y6)	// アニメーションのパターン数

#define WALL_MOVEDELAY_TIME			(15)							// 移動の遅延時間


////////////////////////////
// 动画播放帧数
////////////////////////////
#define	TIME_CHANGE_PATTERN_MAGIC		(9)			// アニメーションの切り替わるタイミング

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexWall(LPDIRECT3DDEVICE9 pDevice);
void SetColorWall(int nIdxWall, D3DXCOLOR col);
void SetVertexWall(	int nIdx,						//记录当前对应的图组polygon地址
					float scaleX,					//X轴的放大参数
					float scaleY);					//自身Y轴的大小参数

void SetTextureWall(int nIdx,						//记录当前对应的图组地址
					int nPatternAnim,				//记录当前显示帧
					int numAnimPattern, 			//记录当前对应的组成图组的图片数量
					int texPatternDivideX,			//记录当前图组X轴图片数量
					int texPatternDivideY );		//记录当前图组Y轴图片数量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTextureWall[MAX_WALL] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffWall = NULL;				// 頂点バッファインターフェースへのポインタ

WALL					g_aWall[MAX_WALL];						// 木ワーク
bool					g_bIsWallCreating;						// 制作かどうか
bool					g_ModeSwitch;							// 制作モード交換
int						g_nCreatingNum;							// 制作した番号
int						g_nMoveDelay_WALL;						// 移動の遅延
//////////////////////
// 贴图的种类路径
//////////////////////
const char *c_pTextureWall[WALL_TYPE_MAX]=
{
	"data/TEXTURE/background/wall_100_100.png",				// 壁
	"data/TEXTURE/background/wall_100_100.png",				// 壁
	"data/TEXTURE/background/slope_up_45_320_320.png",		// 坂道45度
	"data/TEXTURE/background/slope_down_45_320_320.png",	// 坂道45度
	"data/TEXTURE/background/goal_1_4.png",					// GOAL
	"data/TEXTURE/background/doorWithLock_20_100_R.png",	// 赤い門
	"data/TEXTURE/background/doorWithLock_20_100_G.png",	// 緑の門
	"data/TEXTURE/background/doorWithLock_20_100_B.png",	// 青い門
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexWall(pDevice);

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < WALL_TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
									c_pTextureWall[nCntTex],					// ファイルの名前
									&g_apD3DTextureWall[nCntTex]);	// 読み込むメモリー
	}

	for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].vec2Direct	  = D3DXVECTOR2(0.0f, 0.0f);
		g_aWall[nCntWall].vec2StartPoint = D3DXVECTOR2(0.0f, 0.0f);

		g_aWall[nCntWall].fWidth = 0;
		g_aWall[nCntWall].fHeight = 0;
		g_aWall[nCntWall].fMaxX = 0;
		g_aWall[nCntWall].fMinX = 0;
		g_aWall[nCntWall].fMaxY = 0;
		g_aWall[nCntWall].fMinY = 0;

		g_aWall[nCntWall].bUse = false;

		g_aWall[nCntWall].nType = -1;
	}

	g_bIsWallCreating = true;
	g_ModeSwitch = false;
	g_nCreatingNum = -1;
	g_nMoveDelay_WALL = 0;

	//LoadData(g_aWall);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitWall(void)
{
	for(int nCntTex = 0; nCntTex < MAX_WALL; nCntTex++)
	{
		if(g_apD3DTextureWall[nCntTex] != NULL)
		{// テクスチャの開放
			g_apD3DTextureWall[nCntTex]->Release();
			g_apD3DTextureWall[nCntTex] = NULL;
		}
	}

	if(g_pD3DVtxBuffWall != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffWall->Release();
		g_pD3DVtxBuffWall = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWall(void)
{
	for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if(g_aWall[nCntWall].bUse)
		{
			if(	g_aWall[nCntWall].nType == WALL_TYPE_001)
			{
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + WALL_01_SIZE_X / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - WALL_01_SIZE_X / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + WALL_01_SIZE_Y / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - WALL_01_SIZE_Y / 2;
				// パターンの切り替え
				SetTextureWall(nCntWall, 0, 1, 1, 1);
				SetVertexWall(nCntWall, 1, 1);
			}
			if(	g_aWall[nCntWall].nType == WALL_TYPE_002)
			{
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + WALL_02_SIZE_X / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - WALL_02_SIZE_X / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + WALL_02_SIZE_Y / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - WALL_02_SIZE_Y / 2;
				// パターンの切り替え
				SetTextureWall(nCntWall, 0, 1, 1, 1);
				SetVertexWall(nCntWall, 1, 1);
			}
			if(g_aWall[nCntWall].nType == WALL_TYPE_SLOPE_UP_45)
			{
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + WALL_SLOPE_UP_45_SIZE_X / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - WALL_SLOPE_UP_45_SIZE_X / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + WALL_SLOPE_UP_45_SIZE_Y / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - WALL_SLOPE_UP_45_SIZE_Y / 2;

				D3DXVECTOR2 startPoint = D3DXVECTOR2(g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2, g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2);
				D3DXVECTOR2 endPoint = D3DXVECTOR2(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2);

				g_aWall[nCntWall].vec2StartPoint = startPoint;
				g_aWall[nCntWall].vec2Direct = endPoint - startPoint;
				g_aWall[nCntWall].fSteep = (endPoint.y - startPoint.y) / (endPoint.x - startPoint.x);//傾きの設定
				// パターンの切り替え
				SetTextureWall(nCntWall, 0, 1, 1, 1);
				SetVertexWall(nCntWall, 1, 1);
			}
			if(g_aWall[nCntWall].nType == WALL_TYPE_SLOPE_DOWN_45)
			{
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + WALL_SLOPE_DOWN_45_SIZE_X / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - WALL_SLOPE_DOWN_45_SIZE_X / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + WALL_SLOPE_DOWN_45_SIZE_Y / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - WALL_SLOPE_DOWN_45_SIZE_Y / 2;

				D3DXVECTOR2 startPoint = D3DXVECTOR2(g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2, g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2);
				D3DXVECTOR2 endPoint = D3DXVECTOR2(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2, g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2);

				g_aWall[nCntWall].vec2StartPoint = startPoint;
				g_aWall[nCntWall].vec2Direct = endPoint - startPoint;
				g_aWall[nCntWall].fSteep = (endPoint.y - startPoint.y) / (endPoint.x - startPoint.x);//傾きの設定
				// パターンの切り替え
				SetTextureWall(nCntWall, 0, 1, 1, 1);
				SetVertexWall(nCntWall, 1, 1);
			}
			if(	g_aWall[nCntWall].nType == WALL_TYPE_GOAL)
			{
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + WALL_GOAL_SIZE_X / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - WALL_GOAL_SIZE_X / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + WALL_GOAL_SIZE_Y / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - WALL_GOAL_SIZE_Y / 2;
				// パターンの切り替え
				SetTextureWall(nCntWall, 0, 1, 1, 1);
				SetVertexWall(nCntWall, 1, 1);
			}
			if(	g_aWall[nCntWall].nType == WALL_TYPE_DOOR_R)
			{
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + WALL_DOOR_SIZE_X / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - WALL_DOOR_SIZE_X / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + WALL_DOOR_SIZE_Y / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - WALL_DOOR_SIZE_Y / 2;
				// パターンの切り替え
				SetTextureWall(nCntWall, 0, 1, 1, 1);
				SetVertexWall(nCntWall, 1, 1);
			}
			if(	g_aWall[nCntWall].nType == WALL_TYPE_DOOR_G)
			{
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + WALL_DOOR_SIZE_X / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - WALL_DOOR_SIZE_X / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + WALL_DOOR_SIZE_Y / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - WALL_DOOR_SIZE_Y / 2;
				// パターンの切り替え
				SetTextureWall(nCntWall, 0, 1, 1, 1);
				SetVertexWall(nCntWall, 1, 1);
			}
			if(	g_aWall[nCntWall].nType == WALL_TYPE_DOOR_B)
			{
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + WALL_DOOR_SIZE_X / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - WALL_DOOR_SIZE_X / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + WALL_DOOR_SIZE_Y / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - WALL_DOOR_SIZE_Y / 2;
				// パターンの切り替え
				SetTextureWall(nCntWall, 0, 1, 1, 1);
				SetVertexWall(nCntWall, 1, 1);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffWall, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if(g_aWall[nCntWall].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureWall[g_aWall[nCntWall].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntWall * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexWall(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_WALL,		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,				// 頂点バッファの使用法　
												FVF_VERTEX_2D,					// 使用する頂点フォーマット
												D3DPOOL_MANAGED,				// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffWall,				// 頂点バッファインターフェースへのポインタ
												NULL)))							// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);


		for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pVtx += 4)
		{
		// 頂点座標の設定
			//pVtx[0].vtx = D3DXVECTOR3(g_wall[WALL_MAX].pos.x - WALL_01_SIZE_X / 2, g_wall[WALL_MAX].pos.y - WALL_01_SIZE_Y / 2, 0.0f);
			//pVtx[1].vtx = D3DXVECTOR3(g_wall[WALL_MAX].pos.x + WALL_01_SIZE_X / 2, g_wall[WALL_MAX].pos.y - WALL_01_SIZE_Y / 2, 0.0f);
			//pVtx[2].vtx = D3DXVECTOR3(g_wall[WALL_MAX].pos.x - WALL_01_SIZE_X / 2, g_wall[WALL_MAX].pos.y + WALL_01_SIZE_Y / 2, 0.0f);
			//pVtx[3].vtx = D3DXVECTOR3(g_wall[WALL_MAX].pos.x + WALL_01_SIZE_X / 2, g_wall[WALL_MAX].pos.y + WALL_01_SIZE_Y / 2, 0.0f);

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
			g_pD3DVtxBuffWall->Unlock();
		}
	}
	return S_OK;
}



//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorWall(int nIdxWall, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdxWall * 4);

	// 頂点座標の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	g_pD3DVtxBuffWall->Unlock();
}

void SetVertexWall(	int nIdx,					//记录当前对应的图组polygon地址
					float scaleX,				//X轴的放大参数
					float scaleY)				//自身Y轴的大小参数
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(g_aWall[nIdx].pos.x - g_aWall[nIdx].fWidth / 2 * scaleX, g_aWall[nIdx].pos.y - g_aWall[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_aWall[nIdx].pos.x + g_aWall[nIdx].fWidth / 2 * scaleX, g_aWall[nIdx].pos.y - g_aWall[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_aWall[nIdx].pos.x - g_aWall[nIdx].fWidth / 2 * scaleX, g_aWall[nIdx].pos.y + g_aWall[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_aWall[nIdx].pos.x + g_aWall[nIdx].fWidth / 2 * scaleX, g_aWall[nIdx].pos.y + g_aWall[nIdx].fHeight / 2 * scaleY, 0.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffWall->Unlock();
}


void SetTextureWall(	int nIdx,						//记录当前对应的图组地址
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
		g_pD3DVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pD3DVtxBuffWall->Unlock();
	}
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetWall(D3DXVECTOR3 pos, D3DXCOLOR col, int type, char aRemarks[256])
{
	D3DXVECTOR2 startPoint;
	D3DXVECTOR2 endPoint;
	for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if(!g_aWall[nCntWall].bUse)
		{
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].bUse = true;
			
			switch (type)
			{
			case WALL_TYPE_001:
				g_aWall[nCntWall].fWidth = WALL_01_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_01_SIZE_Y;	
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].nType = type;
				strcpy(g_aWall[nCntWall].aRemarks, aRemarks);
				break;

			case WALL_TYPE_002:
				g_aWall[nCntWall].fWidth = WALL_02_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_02_SIZE_Y;	
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].nType = type;
				strcpy(g_aWall[nCntWall].aRemarks, aRemarks);
				break;
			
			case WALL_TYPE_SLOPE_UP_45:
				g_aWall[nCntWall].fWidth = WALL_SLOPE_UP_45_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_SLOPE_UP_45_SIZE_Y;	
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;

				//坂道線分の設定
				startPoint = D3DXVECTOR2(g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2, g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2);
				endPoint = D3DXVECTOR2(g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2);
				g_aWall[nCntWall].vec2StartPoint = startPoint;
				g_aWall[nCntWall].vec2Direct = endPoint - startPoint;
				g_aWall[nCntWall].fSteep = (endPoint.y - startPoint.y) / (endPoint.x - startPoint.x);//傾きの設定
				g_aWall[nCntWall].nType = type;
				strcpy(g_aWall[nCntWall].aRemarks, aRemarks);
				break;

			case WALL_TYPE_SLOPE_DOWN_45:
				g_aWall[nCntWall].fWidth = WALL_SLOPE_DOWN_45_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_SLOPE_DOWN_45_SIZE_Y;	
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;

				//坂道線分の設定
				startPoint = D3DXVECTOR2(g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2, g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2);
				endPoint = D3DXVECTOR2(g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2, g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2);
				g_aWall[nCntWall].vec2StartPoint = startPoint;
				g_aWall[nCntWall].vec2Direct = endPoint - startPoint;
				g_aWall[nCntWall].fSteep = (endPoint.y - startPoint.y) / (endPoint.x - startPoint.x);//傾きの設定
				g_aWall[nCntWall].nType = type;
				strcpy(g_aWall[nCntWall].aRemarks, aRemarks);
				break;
			
			case WALL_TYPE_GOAL:
				g_aWall[nCntWall].fWidth = WALL_GOAL_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_GOAL_SIZE_Y;	
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].nType = type;
				strcpy(g_aWall[nCntWall].aRemarks, "WALL_TYPE_GOAL");
				break;

			case WALL_TYPE_DOOR_R:
				g_aWall[nCntWall].fWidth = WALL_GOAL_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_GOAL_SIZE_Y;	
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].nType = type;
				strcpy(g_aWall[nCntWall].aRemarks, "WALL_TYPE_DOOR_R");
				break;

			case WALL_TYPE_DOOR_G:
				g_aWall[nCntWall].fWidth = WALL_GOAL_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_GOAL_SIZE_Y;	
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].nType = type;
				strcpy(g_aWall[nCntWall].aRemarks, "WALL_TYPE_DOOR_G");
				break;

			case WALL_TYPE_DOOR_B:
				g_aWall[nCntWall].fWidth = WALL_GOAL_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_GOAL_SIZE_Y;	
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].nType = type;
				strcpy(g_aWall[nCntWall].aRemarks, "WALL_TYPE_DOOR_B");
				break;

			default:
				break;
			}
			//SetVertexWall(nCntWall, 1, 1);
			
			// 頂点カラーの設定
			SetColorWall(nCntWall, col);

			return nCntWall;
		}
	}
	return -1;
}


// 読み込むデータを初期化
// 根据读取的预存的类型进行初始化设置。
void InitDataWall(void)
{
	for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if(g_aWall[nCntWall].bUse == true)
		{
			switch (g_aWall[nCntWall].nType)
			{
			case WALL_TYPE_001:
				g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aWall[nCntWall].fWidth = WALL_01_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_01_SIZE_Y;
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				break;

			case WALL_TYPE_002:
				g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aWall[nCntWall].fWidth = WALL_02_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_02_SIZE_Y;
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				break;

			case WALL_TYPE_SLOPE_UP_45:
				g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aWall[nCntWall].fWidth = WALL_SLOPE_UP_45_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_SLOPE_UP_45_SIZE_X;
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				break;

			case WALL_TYPE_SLOPE_DOWN_45:
				g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aWall[nCntWall].fWidth = WALL_SLOPE_DOWN_45_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_SLOPE_DOWN_45_SIZE_X;
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				break;

			case WALL_TYPE_GOAL:
				g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aWall[nCntWall].fWidth = WALL_GOAL_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_GOAL_SIZE_Y;
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				strcpy(g_aWall[nCntWall].aRemarks, "WALL_TYPE_GOAL");
				break;

			case WALL_TYPE_DOOR_R:
				g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aWall[nCntWall].fWidth = WALL_DOOR_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_GOAL_SIZE_Y;
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				strcpy(g_aWall[nCntWall].aRemarks, "WALL_TYPE_DOOR_R");
				break;

			case WALL_TYPE_DOOR_G:
				g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aWall[nCntWall].fWidth = WALL_GOAL_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_GOAL_SIZE_Y;
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				strcpy(g_aWall[nCntWall].aRemarks, "WALL_TYPE_DOOR_G");
				break;
			
			case WALL_TYPE_DOOR_B:
				g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aWall[nCntWall].fWidth = WALL_GOAL_SIZE_X;
				g_aWall[nCntWall].fHeight = WALL_GOAL_SIZE_Y;
				g_aWall[nCntWall].fMaxX = g_aWall[nCntWall].pos.x + g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMinX = g_aWall[nCntWall].pos.x - g_aWall[nCntWall].fWidth / 2;
				g_aWall[nCntWall].fMaxY = g_aWall[nCntWall].pos.y + g_aWall[nCntWall].fHeight / 2;
				g_aWall[nCntWall].fMinY = g_aWall[nCntWall].pos.y - g_aWall[nCntWall].fHeight / 2;
				strcpy(g_aWall[nCntWall].aRemarks, "WALL_TYPE_DOOR_B");
				break;

			default:
				break;
			}
		}
	}
}


// 壁を作る
void WallCreateMode(void)
{
	// 左键单击创建物体
	// 壁を制作する
	if(IsMouseLeftTriggered())
	{
		g_bIsWallCreating = true;
		g_nCreatingNum = -1;
	}

	// 制作した壁のポジションを修正する
	if(g_bIsWallCreating == true)
	{
		bool bTemp = false;
		int nTemp = -1;
		if(IsMouseLeftPressed())
		{
			g_nMoveDelay_WALL++;
			//PrintDebugProc("*******g_nMoveDelay_WALL = %d*******\n", g_nMoveDelay_WALL);

			for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
			{
				if(g_aWall[nCntWall].bUse)
				{
					if(	g_aWall[nCntWall].fMaxX > *GetMouX() &&
						g_aWall[nCntWall].fMinX < *GetMouX() &&
						g_aWall[nCntWall].fMaxY > *GetMouY() &&
						g_aWall[nCntWall].fMinY < *GetMouY())
					{
						bTemp = true;
						nTemp = nCntWall;
					}
				}
			}

			if(bTemp == true && g_nMoveDelay_WALL > WALL_MOVEDELAY_TIME)
			{
				float fX = 0, fY = 0;
				if(g_aWall[nTemp].nType == WALL_TYPE_001)
				{
					fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[nTemp].fWidth / 4);
					fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[nTemp].fWidth / 4);
				}
				else if(g_aWall[nTemp].nType == WALL_TYPE_002)
				{
					fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[nTemp].fWidth / 10);
					fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[nTemp].fWidth / 10);
				}
				else if(g_aWall[nTemp].nType == WALL_TYPE_SLOPE_UP_45)
				{
					fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[nTemp].fWidth / 10);
					fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[nTemp].fWidth / 10);
				}
				else if(g_aWall[nTemp].nType == WALL_TYPE_SLOPE_DOWN_45)
				{
					fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[nTemp].fWidth / 10);
					fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[nTemp].fWidth / 10);
				}
				else if(g_aWall[nTemp].nType == WALL_TYPE_GOAL)
				{
					fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[nTemp].fWidth / 10);
					fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[nTemp].fHeight / 10);
				}
				else if(g_aWall[nTemp].nType == WALL_TYPE_DOOR_R)
				{
					fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[nTemp].fWidth / 10);
					fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[nTemp].fHeight / 10);
				}
				else if(g_aWall[nTemp].nType == WALL_TYPE_DOOR_G)
				{
					fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[nTemp].fWidth / 10);
					fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[nTemp].fHeight / 10);
				}
				else if(g_aWall[nTemp].nType == WALL_TYPE_DOOR_B)
				{
					fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[nTemp].fWidth / 10);
					fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[nTemp].fHeight / 10);
				}

				g_aWall[nTemp].pos.x = fX;
				g_aWall[nTemp].pos.y = fY;
			}
		}
		else
			g_nMoveDelay_WALL = 0;
	}

	// 右键单击切换物体
	// 壁の種類を変換する
	if(IsMouseRightTriggered())
	{
		g_bIsWallCreating = false;

		// 制作した壁無い場合、制作する
		if(g_nCreatingNum == -1)
			g_nCreatingNum = SetWall(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_TYPE_001, "WALL");

		// 如果此时物体已经创建并跟随鼠标，那么删除当前物体，切换到下一个。
		if(g_nCreatingNum != -1)
		{
			int nType = g_aWall[g_nCreatingNum].nType;
			g_aWall[g_nCreatingNum].bUse = false;

			nType++;
			if(nType == WALL_TYPE_MAX)
				nType = 0;

			switch (nType)
			{
				case WALL_TYPE_001:
					g_nCreatingNum = SetWall(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_TYPE_001, "WALL_TYPE_001");
					break;
				case WALL_TYPE_002:
					g_nCreatingNum = SetWall(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_TYPE_002, "WALL_TYPE_002");
					break;
				case WALL_TYPE_SLOPE_UP_45:
					g_nCreatingNum = SetWall(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_TYPE_SLOPE_UP_45, "WALL_TYPE_SLOPE_UP_45");
					break;
				case WALL_TYPE_SLOPE_DOWN_45:
					g_nCreatingNum = SetWall(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_TYPE_SLOPE_DOWN_45, "WALL_TYPE_SLOPE_DOWN_45");
					break;
				case WALL_TYPE_GOAL:
					g_nCreatingNum = SetWall(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_TYPE_GOAL, "WALL_TYPE_GOAL");
					break;
				case WALL_TYPE_DOOR_R:
					g_nCreatingNum = SetWall(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_TYPE_DOOR_R, "WALL_TYPE_DOOR_R");
					break;
				case WALL_TYPE_DOOR_G:
					g_nCreatingNum = SetWall(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_TYPE_DOOR_G, "WALL_TYPE_DOOR_G");
					break;
				case WALL_TYPE_DOOR_B:
					g_nCreatingNum = SetWall(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_TYPE_DOOR_B, "WALL_TYPE_DOOR_B");
					break;
				default:
					break;
			}
		}
	}

	// 键盘1,2快速切换墙砖
	// 壁の種類を変換する
	if(GetKeyboardTrigger(DIK_1))
	{
		g_bIsWallCreating = false;

		// 制作した壁無い場合、制作する
		if(g_nCreatingNum == -1)
			g_nCreatingNum = SetWall(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_TYPE_001, "WALL");

		// 如果此时物体已经创建并跟随鼠标，那么删除当前物体，创建墙1。
		if(g_nCreatingNum != -1)
		{
			g_aWall[g_nCreatingNum].bUse = false;
			g_nCreatingNum = SetWall(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_TYPE_001, "WALL_TYPE_001");
		}
	}
	if(GetKeyboardTrigger(DIK_2))
	{
		g_bIsWallCreating = false;

		// 制作した壁無い場合、制作する
		if(g_nCreatingNum == -1)
			g_nCreatingNum = SetWall(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_TYPE_002, "WALL_TYPE_002");

		// 如果此时物体已经创建并跟随鼠标，那么删除当前物体，创建墙1。
		if(g_nCreatingNum != -1)
		{
			g_aWall[g_nCreatingNum].bUse = false;
			g_nCreatingNum = SetWall(D3DXVECTOR3(*GetMouX(), *GetMouY(), 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WALL_TYPE_002, "WALL_TYPE_002");
		}
	}

	if(g_bIsWallCreating == false)
	{
		float fX = 0, fY = 0;
		if(g_aWall[g_nCreatingNum].nType == WALL_TYPE_001)
		{
			fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[g_nCreatingNum].fWidth / 4);
			fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[g_nCreatingNum].fWidth / 4);
		}
		else if(g_aWall[g_nCreatingNum].nType == WALL_TYPE_002)
		{
			fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[g_nCreatingNum].fWidth / 10);
			fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[g_nCreatingNum].fWidth / 10);
		}
		else if(g_aWall[g_nCreatingNum].nType == WALL_TYPE_SLOPE_UP_45)
		{
			fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[g_nCreatingNum].fWidth / 4);
			fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[g_nCreatingNum].fWidth / 4);
		}
		else if(g_aWall[g_nCreatingNum].nType == WALL_TYPE_SLOPE_DOWN_45)
		{
			fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[g_nCreatingNum].fWidth / 4);
			fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[g_nCreatingNum].fWidth / 4);
		}
		else if(g_aWall[g_nCreatingNum].nType == WALL_TYPE_GOAL)
		{
			fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[g_nCreatingNum].fWidth / 10);
			fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[g_nCreatingNum].fHeight / 10);
		}
		else if(g_aWall[g_nCreatingNum].nType == WALL_TYPE_DOOR_R)
		{
			fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[g_nCreatingNum].fWidth / 10);
			fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[g_nCreatingNum].fHeight / 10);
		}
		else if(g_aWall[g_nCreatingNum].nType == WALL_TYPE_DOOR_G)
		{
			fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[g_nCreatingNum].fWidth / 10);
			fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[g_nCreatingNum].fHeight / 10);
		}
		else if(g_aWall[g_nCreatingNum].nType == WALL_TYPE_DOOR_B)
		{
			fX = *GetMouX() - (int)*GetMouX() % ((int)g_aWall[g_nCreatingNum].fWidth / 10);
			fY = *GetMouY() - (int)*GetMouY() % ((int)g_aWall[g_nCreatingNum].fHeight / 10);
		}
		g_aWall[g_nCreatingNum].pos.x = fX;
		g_aWall[g_nCreatingNum].pos.y = fY;
	}

	//打印当前墙壁类型名称
	// 壁の種類を表示
	switch(g_aWall[g_nCreatingNum].nType)
	{
		case WALL_TYPE_001:
			PrintDebugProc("【OBJECT NAME】: WALL_TYPE_001\n");
			break;
		case WALL_TYPE_002:
			PrintDebugProc("【OBJECT NAME】: WALL_TYPE_002\n");
			break;
		case WALL_TYPE_SLOPE_UP_45:
			PrintDebugProc("【OBJECT NAME】: WALL_TYPE_SLOPE_UP_45\n");
			break;
		case WALL_TYPE_SLOPE_DOWN_45:
			PrintDebugProc("【OBJECT NAME】: WALL_TYPE_SLOPE_DOWN_45\n");
			break;
		case WALL_TYPE_GOAL:
			PrintDebugProc("【OBJECT NAME】: WALL_TYPE_GOAL\n");
			break;
		case WALL_TYPE_DOOR_R:
			PrintDebugProc("【OBJECT NAME】: WALL_TYPE_DOOR_R\n");
			break;
		case WALL_TYPE_DOOR_G:
			PrintDebugProc("【OBJECT NAME】: WALL_TYPE_DOOR_G\n");
			break;
		case WALL_TYPE_DOOR_B:
			PrintDebugProc("【OBJECT NAME】: WALL_TYPE_DOOR_B\n");
			break;
		default:
			break;
	}
}
// 壁のデータゲット
WALL *GetWallData(void)
{
	return g_aWall;
}

void DeleteWall(void)
{
	if(IsMouseLeftTriggered())
	{
		for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
		{
			if(g_aWall[nCntWall].bUse)
			{
				if(	g_aWall[nCntWall].fMaxX > *GetMouX() &&
					g_aWall[nCntWall].fMinX < *GetMouX() &&
					g_aWall[nCntWall].fMaxY > *GetMouY() &&
					g_aWall[nCntWall].fMinY < *GetMouY())
				{
					g_aWall[nCntWall].bUse = false;
				}
			}
		}
	}
}