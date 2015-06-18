/*****************************************************************************
 *
 * 画面成分表示処理 [gameElement.cpp]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 * 场景中单数且长期出现的物品处理
 ******************************************************************************/
#include "gameElement.h"
#include "input.h"
#include "gameManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
////////////////////////////
// 默认位置和尺寸
////////////////////////////
#define	GE_POS_X			(SCREEN_WIDTH / 2)					// 默认の位置(X座標)
#define	GE_POS_Y			(SCREEN_HEIGHT / 2)					// 默认の位置(Y座標)
#define	GE_SIZE_X			(SCREEN_WIDTH)						// 默认の幅
#define	GE_SIZE_Y			(SCREEN_HEIGHT)						// 默认の高さ

#define	GE_CLOCK_POS_X		(150)								// 默认の位置(X座標)
#define	GE_CLOCK_POS_Y		(100)								// 默认の位置(Y座標)
#define	GE_CLOCK_SIZE_X		(200)								// 默认の幅
#define	GE_CLOCK_SIZE_Y		(100)								// 默认の高さ

#define	GE_STAGEICON_SIZE_X	(150)								// stageiconの幅
#define	GE_STAGEICON_SIZE_Y	(150)								// stageiconの高さ
#define GE_STAGEICON_RADIUS	(GE_STAGEICON_SIZE_X / 4)			// stageiconの半径
////////////////////////////
// 1列6行图组动画
////////////////////////////
#define	TEX_PATTERN_DIVIDE_X1		(1)												// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y6		(6)												// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)
#define	NUM_ANIM_PATTERN_X1_Y6		(TEX_PATTERN_DIVIDE_X1 * TEX_PATTERN_DIVIDE_Y6)	// アニメーションのパターン数

////////////////////////////
// 动画播放帧数
////////////////////////////
#define	TIME_CHANGE_PATTERN_MAGIC		(9)				// アニメーションの切り替わるタイミング

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexGameElement(LPDIRECT3DDEVICE9 pDevice);
void SetColorGameElement(int nIdxGameElement, D3DXCOLOR col);

void SetVertexGameElement(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组polygon地址
							float scaleX,					// テクスチャのX軸のスケール		X轴的比例参数
							float scaleY);					// テクスチャのY軸のスケール		Y轴的比例参数

void SetTextureGameElement(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组地址
							int nPatternAnim,				// 今のテクスチャの番号				记录当前显示帧
							int numAnimPattern, 			// テクスチャ全部の個数				记录当前对应的组成图组的图片数量
							int texPatternDivideX,			// 横のテクスチャの個数				记录当前图组X轴图片数量
							int texPatternDivideY );		// 縦のテクスチャの個数				记录当前图组Y轴图片数量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTextureGameElement[MAX_GAMEELEMENT] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffGameElement = NULL;						// 頂点バッファインターフェースへのポインタ

GAMEELEMENT				g_aGE[MAX_GAMEELEMENT];				// 木ワーク

// 计数器
int						g_nCounterAnimPowerUpP1;

//记录HP图片帧
int						g_nPatternAnimPowerUpP1;

//////////////////////
// 贴图的种类路径
//////////////////////
const char *c_pTextureGameElement[GE_TYPE_MAX]=		// 不同模式下的背景切换
{
	"data/TEXTURE/stages/shell_200_400.png",		// 贝壳
	"data/TEXTURE/stages/starfish_200_400.png",		// 海星
	"data/TEXTURE/stages/octopus_200_400.png",		// 章鱼
	"data/TEXTURE/stages/anchor_200_400.png",		// 船锚
	"data/TEXTURE/stages/shipwrecks_200_400.png",	// 沉船
	"data/TEXTURE/stages/valuables_200_400.png",	// 财宝
	"data/TEXTURE/stages/castle_200_400.png",		// 城堡
	"data/TEXTURE/stages/soldier_200_400.png",		// 士兵
	"data/TEXTURE/stages/potatoes_200_400.png",		// 马铃薯
	"data/TEXTURE/stages/casks_200_400.png",		// 酒桶
	"data/TEXTURE/stages/newship_200_400.png",		// 新船
	"data/TEXTURE/stages/cactus_200_400.png",		// 仙人掌
	"data/TEXTURE/stages/camel_200_400.png",		// 骆驼
	"data/TEXTURE/stages/scorpion_200_400.png",		// 蝎子
	"data/TEXTURE/stages/sphinx_200_400.png",		// 狮身人面像
	"data/TEXTURE/stages/map_200_400.png",			// 藏宝图
	"data/TEXTURE/items/timelock_02_200_100.png",	// 禁止时间逆流标志	
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGameElement(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexGameElement(pDevice);

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < GE_TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
									c_pTextureGameElement[nCntTex],					// ファイルの名前
									&g_apD3DTextureGameElement[nCntTex]);	// 読み込むメモリー
	}

	for(int nCntGE = 0; nCntGE < MAX_GAMEELEMENT; nCntGE++)
	{
		g_aGE[nCntGE].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aGE[nCntGE].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aGE[nCntGE].fWidth = 0;
		g_aGE[nCntGE].fHeight = 0;
		g_aGE[nCntGE].fRadius = 0;

		g_aGE[nCntGE].bUse = false;

		g_aGE[nCntGE].nCounterAnim = 0;
		g_aGE[nCntGE].nPatternAnim = 0;
		g_aGE[nCntGE].nType = -1;
		g_aGE[nCntGE].nViewMode = -1;
	}

	g_nCounterAnimPowerUpP1 = 0;
	g_nPatternAnimPowerUpP1 = 0;

	SetGameElement(D3DXVECTOR3(50, 800, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_SHELL, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(200, 700, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_STARFISH, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(100, 600, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_OCTOPUS, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(300, 450, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_ANCHOR, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(150, 300, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_SHIPWRECKS, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(400, 100, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_VALUABLES, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(650, 150, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_CASTLE, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(500, 250, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_SOLDIER, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(700, 450, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_POTATOES, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(400, 650, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_CASKS, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(850, 800, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_NEWSHIP, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(1050, 750, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_CACTUS, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(950, 600, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_CAMEL, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(1100, 450, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_SCORPION, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(850, 250, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_SPHINX, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(1100, 50, 0), GE_STAGEICON_SIZE_X, GE_STAGEICON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_STAGE_MAP, VIEW_STAGE_SELECT);
	SetGameElement(D3DXVECTOR3(GE_CLOCK_POS_X, GE_CLOCK_POS_Y, 0), GE_CLOCK_SIZE_X, GE_CLOCK_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), GE_PLAYERBAR_CLOCK, VIEW_OTHER);
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitGameElement(void)
{
	for(int nCntTex = 0; nCntTex < MAX_GAMEELEMENT; nCntTex++)
	{
		if(g_apD3DTextureGameElement[nCntTex] != NULL)
		{// テクスチャの開放
			g_apD3DTextureGameElement[nCntTex]->Release();
			g_apD3DTextureGameElement[nCntTex] = NULL;
		}
	}

	if(g_pD3DVtxBuffGameElement != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffGameElement->Release();
		g_pD3DVtxBuffGameElement = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGameElement(void)
{
	switch(*GetMode())
	{
	case GAMEMODE_LOGO:

		break;

	case GAMEMODE_TITLE:

		break;

	case GAMEMODE_STAGE_SELECT:
		{
			for(int nCntGE = 0; nCntGE < MAX_GAMEELEMENT; nCntGE++)
			{
				if(g_aGE[nCntGE].bUse == true)
				{
					if(g_aGE[nCntGE].nViewMode == VIEW_STAGE_SELECT)
					{
						SetVertexGameElement(nCntGE, 1, 1);	
						SetTextureGameElement(nCntGE, g_aGE[nCntGE].nPatternAnim, 2, 1, 2);
					}
				}
			}

			//int* nCurrentSceneNum = GetSceneNum();
			int* nLastSceneNum = GetLastSceneNum();
			for(int nCntScene = 0; nCntScene <= *nLastSceneNum; nCntScene++)
			{
				for(int nCntGE = 0; nCntGE < MAX_GAMEELEMENT; nCntGE++)
				{
					if(g_aGE[nCntGE].bUse == true && g_aGE[nCntGE].nType <= *nLastSceneNum)
					{
						g_aGE[nCntGE].nPatternAnim = 1;
						SetVertexGameElement(nCntGE, 1, 1);	
						SetTextureGameElement(nCntGE, g_aGE[nCntGE].nPatternAnim, 2, 1, 2);
					}
				}
			}
		}
		break;

	case GAMEMODE_GAME:
		{
			for(int nCntGE = 0; nCntGE < MAX_GAMEELEMENT; nCntGE++)
			{
				if(g_aGE[nCntGE].bUse == true)
				{
					if(g_aGE[nCntGE].nViewMode != VIEW_STAGE_SELECT)
					{
						SetVertexGameElement(nCntGE, 1, 1);	
						SetTextureGameElement(nCntGE, g_aGE[nCntGE].nPatternAnim, 1, 1, 1);
					}
				}
			}
		}
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
void DrawGameElement(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffGameElement, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntGE = 0; nCntGE < MAX_GAMEELEMENT; nCntGE++)
	{
		if(g_aGE[nCntGE].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureGameElement[g_aGE[nCntGE].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntGE * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexGameElement(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_GAMEELEMENT,		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,									// 頂点バッファの使用法　
												FVF_VERTEX_2D,										// 使用する頂点フォーマット
												D3DPOOL_MANAGED,									// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffGameElement,							// 頂点バッファインターフェースへのポインタ
												NULL)))												// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffGameElement->Lock(0, 0, (void**)&pVtx, 0);


		for(int nCntGE = 0; nCntGE < MAX_GAMEELEMENT; nCntGE++, pVtx += 4)
		{
		// 頂点座標の設定
			//pVtx[0].vtx = D3DXVECTOR3(g_gameElement[GE_MAX].pos.x - GE_SIZE_X / 2, g_gameElement[GE_MAX].pos.y - GE_SIZE_Y / 2, 0.0f);
			//pVtx[1].vtx = D3DXVECTOR3(g_gameElement[GE_MAX].pos.x + GE_SIZE_X / 2, g_gameElement[GE_MAX].pos.y - GE_SIZE_Y / 2, 0.0f);
			//pVtx[2].vtx = D3DXVECTOR3(g_gameElement[GE_MAX].pos.x - GE_SIZE_X / 2, g_gameElement[GE_MAX].pos.y + GE_SIZE_Y / 2, 0.0f);
			//pVtx[3].vtx = D3DXVECTOR3(g_gameElement[GE_MAX].pos.x + GE_SIZE_X / 2, g_gameElement[GE_MAX].pos.y + GE_SIZE_Y / 2, 0.0f);

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
			g_pD3DVtxBuffGameElement->Unlock();
		}
	}

	return S_OK;
}



//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorGameElement(int nIdxGameElement, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffGameElement->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdxGameElement * 4);

	// 頂点座標の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	g_pD3DVtxBuffGameElement->Unlock();
}

void SetVertexGameElement(	int nIdx,					//记录当前对应的图组polygon地址
							float scaleX,				//X轴的放大参数
							float scaleY)				//自身Y轴的大小参数
{
	VERTEX_2D *pVtx;
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffGameElement->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(g_aGE[nIdx].pos.x - g_aGE[nIdx].fWidth / 2 * scaleX, g_aGE[nIdx].pos.y - g_aGE[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_aGE[nIdx].pos.x + g_aGE[nIdx].fWidth / 2 * scaleX, g_aGE[nIdx].pos.y - g_aGE[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_aGE[nIdx].pos.x - g_aGE[nIdx].fWidth / 2 * scaleX, g_aGE[nIdx].pos.y + g_aGE[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_aGE[nIdx].pos.x + g_aGE[nIdx].fWidth / 2 * scaleX, g_aGE[nIdx].pos.y + g_aGE[nIdx].fHeight / 2 * scaleY, 0.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffGameElement->Unlock();
}

void SetTextureGameElement(	int nIdx,						//记录当前对应的图组地址
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
		g_pD3DVtxBuffGameElement->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pD3DVtxBuffGameElement->Unlock();
	}
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetGameElement(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int nType, int nViewMode)
{
	for(int nCntGE = 0; nCntGE < MAX_GAMEELEMENT; nCntGE++)
	{
		if(!g_aGE[nCntGE].bUse)
		{
			g_aGE[nCntGE].pos = pos;
			g_aGE[nCntGE].fWidth = fWidth;
			g_aGE[nCntGE].fHeight = fHeight;
			g_aGE[nCntGE].fRadius = GE_STAGEICON_RADIUS;
			g_aGE[nCntGE].bUse = true;
			g_aGE[nCntGE].nType = nType;
			g_aGE[nCntGE].nCounterAnim = 0;
			g_aGE[nCntGE].nPatternAnim = 0;
			g_aGE[nCntGE].nViewMode = nViewMode;
			
			SetVertexGameElement(nCntGE, 1, 1);
			
			// 頂点カラーの設定
			SetColorGameElement(nCntGE, col);
			break;
		}
	}
}


// マウスとシーンのアイコンのあたり判定
bool IsMouseHitIcon(int* pIconNum)
{
	for(int nCntGE = 0; nCntGE < MAX_GAMEELEMENT; nCntGE++)
	{
		if(g_aGE[nCntGE].bUse == true)
		{
			if(g_aGE[nCntGE].nViewMode == VIEW_STAGE_SELECT && g_aGE[nCntGE].nPatternAnim == 1)
			{
				if(	(g_aGE[nCntGE].pos.x - *GetMouX()) * (g_aGE[nCntGE].pos.x - *GetMouX()) +
					(g_aGE[nCntGE].pos.y - *GetMouY()) * (g_aGE[nCntGE].pos.y - *GetMouY()) <=
					g_aGE[nCntGE].fRadius * g_aGE[nCntGE].fRadius)
				{
					*pIconNum = nCntGE;
					return true;
				}
			}
		}
	}
	return false;
}

GAMEELEMENT *GetGameElement(void)
{
	return g_aGE;
}