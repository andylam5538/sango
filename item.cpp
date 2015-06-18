/*****************************************************************************
 *
 * アイテム表表示処理 [item.cpp]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 * 
 ******************************************************************************/
#include "item.h"
#include "input.h"
#include "gameManager.h"
#include "gimmick.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
////////////////////////////
// 默认位置和尺寸
////////////////////////////
#define	ITEM_POS_X			(300)									// 默认の位置(X座標)
#define	ITEM_POS_Y			(300)									// 默认の位置(Y座標)
#define	ITEM_SIZE_X			(100)									// 默认の幅
#define	ITEM_SIZE_Y			(100)									// 默认の高さ


////////////////////////////
// 1列6行图组动画
////////////////////////////
#define	TEX_PATTERN_DIVIDE_X1		(1)												// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y6		(6)												// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)
#define	NUM_ANIM_PATTERN_X1_Y6		(TEX_PATTERN_DIVIDE_X1 * TEX_PATTERN_DIVIDE_Y6)	// アニメーションのパターン数

////////////////////////////
// 动画播放帧数
////////////////////////////
//#define	TIME_CHANITEM_PATTERN_MAGIC		(9)				// アニメーションの切り替わるタイミング

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexItem(LPDIRECT3DDEVICE9 pDevice);
void SetColorItem(int nIdxItem, D3DXCOLOR col);

void SetVertexItem(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组polygon地址
					float scaleX,					// テクスチャのX軸のスケール		X轴的比例参数
					float scaleY);					// テクスチャのY軸のスケール		Y轴的比例参数

void SetTextureItem(int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组地址
					int nPatternAnim,				// 今のテクスチャの番号				记录当前显示帧
					int numAnimPattern, 			// テクスチャ全部の個数				记录当前对应的组成图组的图片数量
					int texPatternDivideX,			// 横のテクスチャの個数				记录当前图组X轴图片数量
					int texPatternDivideY );		// 縦のテクスチャの個数				记录当前图组Y轴图片数量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTextureItem[MAX_ITEM] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffItem = NULL;				// 頂点バッファインターフェースへのポインタ

ITEM					g_aItem[MAX_ITEM];						// 木ワーク
GIMMICK					*g_pGimmick_Item = GetGimmickData();	// ギミックのデータゲット
//////////////////////
// 贴图的种类路径
//////////////////////
const char *c_pTextureItem[ITEM_TYPE_MAX]=				// 不同模式下的背景切换
{
	"data/TEXTURE/items/timelock_02_100_100.png",		// 禁止时间逆流标志
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexItem(pDevice);

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < ITEM_TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
								c_pTextureItem[nCntTex],					// ファイルの名前
								&g_apD3DTextureItem[nCntTex]);				// 読み込むメモリー
	}

	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].fWidth = 0;
		g_aItem[nCntItem].fHeight = 0;

		g_aItem[nCntItem].bUse = false;

		g_aItem[nCntItem].nCounterAnim = 0;
		g_aItem[nCntItem].nPatternAnim = 0;
		g_aItem[nCntItem].nType = -1;
		g_aItem[nCntItem].nBelongTo = -1;
	}

	//SetItem(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ITEM_TYPE_01);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitItem(void)
{
	for(int nCntTex = 0; nCntTex < MAX_ITEM; nCntTex++)
	{
		if(g_apD3DTextureItem[nCntTex] != NULL)
		{// テクスチャの開放
			g_apD3DTextureItem[nCntTex]->Release();
			g_apD3DTextureItem[nCntTex] = NULL;
		}
	}

	if(g_pD3DVtxBuffItem != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffItem->Release();
		g_pD3DVtxBuffItem = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItem(void)
{
	//if(!*GetIsPlay())
	//{

	//}

	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if(g_aItem[nCntItem].bUse == true)
		{
			int nTempPos = g_aItem[nCntItem].nBelongTo;
			g_aItem[nCntItem].pos = g_pGimmick_Item[nTempPos].pos;

			SetTextureItem(nCntItem, 0, 1, 1, 1);
			SetVertexItem(nCntItem, 1, 1);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffItem, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if(g_aItem[nCntItem].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureItem[g_aItem[nCntItem].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntItem * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexItem(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_ITEM,		// 頂点データ用に確保するバッファサイズ(バイト単位)
											D3DUSAGE_WRITEONLY,									// 頂点バッファの使用法　
											FVF_VERTEX_2D,										// 使用する頂点フォーマット
											D3DPOOL_MANAGED,									// リソースのバッファを保持するメモリクラスを指定
											&g_pD3DVtxBuffItem,								// 頂点バッファインターフェースへのポインタ
											NULL)))												// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);


		for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pVtx += 4)
		{
		// 頂点座標の設定
			//pVtx[0].vtx = D3DXVECTOR3(g_item[ITEM_MAX].pos.x - ITEM_SIZE_X / 2, g_item[ITEM_MAX].pos.y - ITEM_SIZE_Y / 2, 0.0f);
			//pVtx[1].vtx = D3DXVECTOR3(g_item[ITEM_MAX].pos.x + ITEM_SIZE_X / 2, g_item[ITEM_MAX].pos.y - ITEM_SIZE_Y / 2, 0.0f);
			//pVtx[2].vtx = D3DXVECTOR3(g_item[ITEM_MAX].pos.x - ITEM_SIZE_X / 2, g_item[ITEM_MAX].pos.y + ITEM_SIZE_Y / 2, 0.0f);
			//pVtx[3].vtx = D3DXVECTOR3(g_item[ITEM_MAX].pos.x + ITEM_SIZE_X / 2, g_item[ITEM_MAX].pos.y + ITEM_SIZE_Y / 2, 0.0f);

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
			g_pD3DVtxBuffItem->Unlock();
		}
	}

	return S_OK;
}



//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorItem(int nIdxItem, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdxItem * 4);

	// 頂点座標の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	g_pD3DVtxBuffItem->Unlock();
}

void SetVertexItem(	int nIdx,							//记录当前对应的图组polygon地址
							float scaleX,				//X轴的放大参数
							float scaleY)				//自身Y轴的大小参数
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(g_aItem[nIdx].pos.x - g_aItem[nIdx].fWidth / 2 * scaleX, g_aItem[nIdx].pos.y - g_aItem[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_aItem[nIdx].pos.x + g_aItem[nIdx].fWidth / 2 * scaleX, g_aItem[nIdx].pos.y - g_aItem[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_aItem[nIdx].pos.x - g_aItem[nIdx].fWidth / 2 * scaleX, g_aItem[nIdx].pos.y + g_aItem[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_aItem[nIdx].pos.x + g_aItem[nIdx].fWidth / 2 * scaleX, g_aItem[nIdx].pos.y + g_aItem[nIdx].fHeight / 2 * scaleY, 0.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffItem->Unlock();
}

void SetTextureItem(int nIdx,						//记录当前对应的图组地址
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
		g_pD3DVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pD3DVtxBuffItem->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetItem(D3DXCOLOR col, int ntype, int nBelongTo)
{
	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if(!g_aItem[nCntItem].bUse)
		{
			g_aItem[nCntItem].bUse = true;
			g_aItem[nCntItem].nCounterAnim = 0;
			g_aItem[nCntItem].nPatternAnim = 0;
			g_aItem[nCntItem].nBelongTo = nBelongTo;
			
			switch (ntype)
			{
			case ITEM_TYPE_01:
				g_aItem[nCntItem].pos = D3DXVECTOR3(ITEM_POS_X, ITEM_POS_Y, 0.0f);
				g_aItem[nCntItem].nType = ITEM_TYPE_01;
				g_aItem[nCntItem].fHeight = ITEM_SIZE_X;
				g_aItem[nCntItem].fWidth = ITEM_SIZE_Y;
				break;
			}

			SetVertexItem(nCntItem, 1, 1);
			
			// 頂点カラーの設定
			SetColorItem(nCntItem, col);

			break;
		}
	}
}

// 時間軸のデータゲット
ITEM *GetItem(void)
{
	return g_aItem;
}

void DeleteItem(int nDeleteNum)
{
	for(int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if(g_aItem[nCntItem].bUse == true && g_aItem[nCntItem].nBelongTo == nDeleteNum)
			g_aItem[nCntItem].bUse = false;
	}
}