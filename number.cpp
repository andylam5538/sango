/*****************************************************************************
 *
 * 数字の表示処理 [number.cpp]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 * 
 ******************************************************************************/
#include "number.h"
#include "input.h"
#include "gameManager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
////////////////////////////
// 默认位置和尺寸
////////////////////////////
#define	NUMBER_POS_X		(270)					// 默认の位置(X座標)
#define	NUMBER_POS_Y		(100)					// 默认の位置(Y座標)
#define	NUMBER_SIZE_X		(80)					// 默认の幅
#define	NUMBER_SIZE_Y		(80)					// 默认の高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexNumber(void);
void SetVertexNumber(	int nIdx,					// 今のテクスチャの配列番号			记录当前对应的图组polygon地址
						float scaleX,				// テクスチャのX軸のスケール		X轴的比例参数
						float scaleY);				// テクスチャのY軸のスケール		Y轴的比例参数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureNumber[NUMBERTYPE_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffNumber = NULL;						// 頂点バッファインターフェースへのポインタ

NUMBER					g_aNumber[NUMBER_MAX];
ITEM_NUMBER				g_nNumItem;										// アイテムの個数
const char *c_pTextureNumber[NUMBERTYPE_MAX]=
{
	"data/TEXTURE/numbers/0_L.png",
	"data/TEXTURE/numbers/1_L.png",
	"data/TEXTURE/numbers/2_L.png",
	"data/TEXTURE/numbers/3_L.png",
	"data/TEXTURE/numbers/4_L.png",
	"data/TEXTURE/numbers/5_L.png",
	"data/TEXTURE/numbers/6_L.png",
	"data/TEXTURE/numbers/7_L.png",
	"data/TEXTURE/numbers/8_L.png",
	"data/TEXTURE/numbers/9_L.png",
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitNumber(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntNumber = 0; nCntNumber < NUMBER_MAX; nCntNumber++)
	{
		g_aNumber[nCntNumber].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aNumber[nCntNumber].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aNumber[nCntNumber].fHeight = 0;
		g_aNumber[nCntNumber].fWidth = 0;
		g_aNumber[nCntNumber].bUse = false;
		g_aNumber[nCntNumber].nType = -1;
	}
	// 頂点情報の作成
	MakeVertexNumber();

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < NUMBERTYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
									c_pTextureNumber[nCntTex],			// ファイルの名前
									&g_pD3DTextureNumber[nCntTex]);		// 読み込むメモリー
	}
	//nNumItem.nItemLockTime = 2;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitNumber(void)
{
	for(int nCntNumber = 0; nCntNumber < NUMBERTYPE_MAX; nCntNumber++)
	{
		if(g_pD3DTextureNumber[nCntNumber] != NULL)
		{// テクスチャの開放
			g_pD3DTextureNumber[nCntNumber]->Release();
			g_pD3DTextureNumber[nCntNumber] = NULL;
		}
	}

	if(g_pD3DVtxBuffNumber != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffNumber->Release();
		g_pD3DVtxBuffNumber = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateNumber(void)
{
	// 每次更新时都初始化数字
	for(int nCntNumber = 0; nCntNumber < NUMBER_MAX; nCntNumber++)
	{
		if(g_aNumber[nCntNumber].bUse == true)
			g_aNumber[nCntNumber].bUse = false;
	}

	// 比分的十位
	SetNumber(D3DXVECTOR3(NUMBER_POS_X, NUMBER_POS_Y, 0.0f), g_nNumItem.nItemLockTime);

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawNumber(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffNumber, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for(int nCntNumber = 0; nCntNumber < NUMBER_MAX; nCntNumber++)
	{
		if(g_aNumber[nCntNumber].bUse)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pD3DTextureNumber[g_aNumber[nCntNumber].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntNumber * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点の作成
//=============================================================================
HRESULT MakeVertexNumber(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// オブジェクトの頂点バッファを生成
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * NUMBER_MAX,		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,								// 頂点バッファの使用法　
												FVF_VERTEX_2D,									// 使用する頂点フォーマット
												D3DPOOL_MANAGED,								// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffNumber,						// 頂点バッファインターフェースへのポインタ
												NULL)))											// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffNumber->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		for(int nCntNumber = 0; nCntNumber < NUMBER_MAX; nCntNumber++, pVtx += 4)
		{
			/*pVtx[0].vtx = D3DXVECTOR3(g_number[nCntNumber].pos.x - NUMBER_SIZE_X / 2, g_number[nCntNumber].pos.y - NUMBER_SIZE_Y / 2, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(g_number[nCntNumber].pos.x + NUMBER_SIZE_X / 2, g_number[nCntNumber].pos.y - NUMBER_SIZE_Y / 2, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(g_number[nCntNumber].pos.x - NUMBER_SIZE_X / 2, g_number[nCntNumber].pos.y + NUMBER_SIZE_Y / 2, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(g_number[nCntNumber].pos.x + NUMBER_SIZE_X / 2, g_number[nCntNumber].pos.y + NUMBER_SIZE_Y / 2, 0.0f);*/

			// rhwの設定
			pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

			// 反射光の設定
			pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		}
		// 頂点データをアンロックする
		g_pD3DVtxBuffNumber->Unlock();
	}

	return S_OK;
}

void SetVertexNumber(	int nIdx,					//记录当前对应的图组polygon地址
						float scaleX,				//X轴的放大参数
						float scaleY)				//自身Y轴的大小参数
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffNumber->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(g_aNumber[nIdx].pos.x - g_aNumber[nIdx].fWidth / 2 * scaleX, g_aNumber[nIdx].pos.y - g_aNumber[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_aNumber[nIdx].pos.x + g_aNumber[nIdx].fWidth / 2 * scaleX, g_aNumber[nIdx].pos.y - g_aNumber[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_aNumber[nIdx].pos.x - g_aNumber[nIdx].fWidth / 2 * scaleX, g_aNumber[nIdx].pos.y + g_aNumber[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_aNumber[nIdx].pos.x + g_aNumber[nIdx].fWidth / 2 * scaleX, g_aNumber[nIdx].pos.y + g_aNumber[nIdx].fHeight / 2 * scaleY, 0.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffNumber->Unlock();
}


void SetNumber(D3DXVECTOR3 pos, int nType)
{
	for(int nCntNumber = 0; nCntNumber < NUMBER_MAX; nCntNumber++)
	{
		if(!g_aNumber[nCntNumber].bUse)
		{
			g_aNumber[nCntNumber].pos = pos;
			g_aNumber[nCntNumber].nType = nType;
			g_aNumber[nCntNumber].bUse = true;
			g_aNumber[nCntNumber].fHeight = NUMBER_SIZE_X;
			g_aNumber[nCntNumber].fWidth = NUMBER_SIZE_Y;

			SetVertexNumber(nCntNumber, 1, 1);
			////数字跳动动画
			//if(g_bIsScaleAnimPlay == true)
			//{
			//	switch(g_nScaleAnimCounter % 10)
			//	{
			//		case 2:
			//			SetVertexNumber(nCntNumber, 80);
			//			break;

			//		case 4:
			//			SetVertexNumber(nCntNumber, 60);
			//			break;

			//		case 6:
			//			SetVertexNumber(nCntNumber, 40);
			//			break;

			//		case 8:
			//			SetVertexNumber(nCntNumber, 20);
			//			g_bIsScaleAnimPlay = false;
			//			break;

			//		default:
			//			break;
			//	}
			//}
			//else
			//{
			//	SetVertexNumber(nCntNumber, 0);
			//	//SetColorNumber(nCntNumber, D3DCOLOR_RGBA(1, 1, 1, 1));
			//	g_nScaleAnimCounter = 0;
			//}
			//g_number[nCntNumber].bUse = true;
			//break;
		}
	}
}
void SetColorNumber(int nIdx, D3DCOLOR col_3)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffNumber->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].diffuse =
	pVtx[1].diffuse =
	pVtx[2].diffuse =
	pVtx[3].diffuse = col_3;

	// 頂点データをアンロックする
	g_pD3DVtxBuffNumber->Unlock();
}
// アイテム個数のデータゲット
ITEM_NUMBER* GetItemNum(void)
{
	return &g_nNumItem;
}
// アイテム個数を更新する
void UpdataItemNum(int nNum)
{
	g_nNumItem.nItemLockTime += nNum;
}