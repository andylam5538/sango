/*****************************************************************************
 *
 * マウス表示処理 [mouseLocal.cpp]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 * 
 ******************************************************************************/
#include "mouseLocal.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
////////////////////////////
// 默认位置和尺寸
////////////////////////////
#define	MOUSELOCAL_POS_X					(0)									// マウスの位置(X座標)
#define	MOUSELOCAL_POS_Y					(0)									// マウスの位置(Y座標)
#define	MOUSELOCAL_SIZE_X				(50)								// マウスの幅
#define	MOUSELOCAL_SIZE_Y				(50)								// マウスの高さ
////////////////////////////
// 8列1行图组动画
////////////////////////////
#define	TEX_PATTERN_DIVIDE_X8		(8)												// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y1		(1)												// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)
#define	NUM_ANIM_PATTERN_X8_Y1		(TEX_PATTERN_DIVIDE_X8 * TEX_PATTERN_DIVIDE_Y1)	// アニメーションのパターン数

////////////////////////////
// 动画播放帧数
////////////////////////////
#define	TIME_CHANGE_MOUSELOCAL_MOTION	(9)												// アニメーションの切り替わるタイミング

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexMouseLocal(LPDIRECT3DDEVICE9 pDevice);
void SetColorMouseLocal(int nIdxMouseLocal, D3DCOLOR col_3);

void SetVertexMouseLocal(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组polygon地址
						float scaleX,					// テクスチャのX軸のスケール		X轴的比例参数
						float scaleY);					// テクスチャのY軸のスケール		Y轴的比例参数

void SetTextureMouseLocal(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组地址
						int nPatternAnim,				// 今のテクスチャの番号				记录当前显示帧
						int numAnimPattern, 			// テクスチャ全部の個数				记录当前对应的组成图组的图片数量
						int texPatternDivideX,			// 横のテクスチャの個数				记录当前图组X轴图片数量
						int texPatternDivideY );		// 縦のテクスチャの個数				记录当前图组Y轴图片数量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTextureMouseLocal[MAX_MOUSELOCAL] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffMouseLocal = NULL;					// 頂点バッファインターフェースへのポインタ

MOUSELOCAL					g_aMouseLocal[MAX_MOUSELOCAL];			// 木ワーク

//////////////////////
// 贴图的种类路径
//////////////////////
const char *c_pTextureMouseLocal[MOUSELOCAL_TYPE_MAX]=	// 不同模式下的背景切换
{
	"data/TEXTURE/mouse/mouse.png",						// プレー
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitMouseLocal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexMouseLocal(pDevice);

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < MOUSELOCAL_TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,								// デバイスへのポインタ
									c_pTextureMouseLocal[nCntTex],			// ファイルの名前
									&g_apD3DTextureMouseLocal[nCntTex]);	// 読み込むメモリー
	}

	for(int nCntBot = 0; nCntBot < MAX_MOUSELOCAL; nCntBot++)
	{
		g_aMouseLocal[nCntBot].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aMouseLocal[nCntBot].fWidth = 0;
		g_aMouseLocal[nCntBot].fHeight = 0;

		g_aMouseLocal[nCntBot].bUse = false;
		g_aMouseLocal[nCntBot].bIsActiva = false;

		g_aMouseLocal[nCntBot].nCounterAnim = 0;
		g_aMouseLocal[nCntBot].nPatternAnim = 0;
		g_aMouseLocal[nCntBot].nType = -1;
	}

	SetMouseLocal(D3DXVECTOR3(MOUSELOCAL_POS_X, MOUSELOCAL_POS_Y, 0), MOUSELOCAL_SIZE_X, MOUSELOCAL_SIZE_Y, D3DCOLOR_RGBA(255, 255, 255, 255), MOUSELOCAL_TYPE_01);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitMouseLocal(void)
{
	for(int nCntTex = 0; nCntTex < MAX_MOUSELOCAL; nCntTex++)
	{
		if(g_apD3DTextureMouseLocal[nCntTex] != NULL)
		{// テクスチャの開放
			g_apD3DTextureMouseLocal[nCntTex]->Release();
			g_apD3DTextureMouseLocal[nCntTex] = NULL;
		}
	}

	if(g_pD3DVtxBuffMouseLocal != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffMouseLocal->Release();
		g_pD3DVtxBuffMouseLocal = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMouseLocal(void)
{
	g_aMouseLocal[0].pos.x = *GetMouX();
	g_aMouseLocal[0].pos.y = *GetMouY();

	SetVertexMouseLocal(0, 1, 1);
	SetTextureMouseLocal(0, 0, 1, 1, 1);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMouseLocal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffMouseLocal, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntBot = 0; nCntBot < MAX_MOUSELOCAL; nCntBot++)
	{
		if(g_aMouseLocal[nCntBot].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureMouseLocal[g_aMouseLocal[nCntBot].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntBot * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexMouseLocal(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_MOUSELOCAL,		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,									// 頂点バッファの使用法　
												FVF_VERTEX_2D,										// 使用する頂点フォーマット
												D3DPOOL_MANAGED,									// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffMouseLocal,							// 頂点バッファインターフェースへのポインタ
												NULL)))												// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffMouseLocal->Lock(0, 0, (void**)&pVtx, 0);


		for(int nCntBot = 0; nCntBot < MAX_MOUSELOCAL; nCntBot++, pVtx += 4)
		{
		// 頂点座標の設定
			//pVtx[0].vtx = D3DXVECTOR3(g_mouseLocal[MouseLocal_MAX].pos.x - MouseLocal_SIZE_X / 2, g_mouseLocal[MouseLocal_MAX].pos.y - MouseLocal_SIZE_Y / 2, 0.0f);
			//pVtx[1].vtx = D3DXVECTOR3(g_mouseLocal[MouseLocal_MAX].pos.x + MouseLocal_SIZE_X / 2, g_mouseLocal[MouseLocal_MAX].pos.y - MouseLocal_SIZE_Y / 2, 0.0f);
			//pVtx[2].vtx = D3DXVECTOR3(g_mouseLocal[MouseLocal_MAX].pos.x - MouseLocal_SIZE_X / 2, g_mouseLocal[MouseLocal_MAX].pos.y + MouseLocal_SIZE_Y / 2, 0.0f);
			//pVtx[3].vtx = D3DXVECTOR3(g_mouseLocal[MouseLocal_MAX].pos.x + MouseLocal_SIZE_X / 2, g_mouseLocal[MouseLocal_MAX].pos.y + MouseLocal_SIZE_Y / 2, 0.0f);

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
			g_pD3DVtxBuffMouseLocal->Unlock();
		}
	}

	return S_OK;
}



//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorMouseLocal(int nIdxMouseLocal, D3DCOLOR col_3)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffMouseLocal->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdxMouseLocal * 4);

	// 頂点座標の設定
	pVtx[0].diffuse =
	pVtx[1].diffuse =
	pVtx[2].diffuse =
	pVtx[3].diffuse = col_3;

	// 頂点データをアンロックする
	g_pD3DVtxBuffMouseLocal->Unlock();
}

void SetVertexMouseLocal(	int nIdx,					//记录当前对应的图组polygon地址
						float scaleX,				//X轴的放大参数
						float scaleY)				//自身Y轴的大小参数
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffMouseLocal->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(g_aMouseLocal[nIdx].pos.x, g_aMouseLocal[nIdx].pos.y, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_aMouseLocal[nIdx].pos.x + g_aMouseLocal[nIdx].fWidth * scaleX, g_aMouseLocal[nIdx].pos.y, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_aMouseLocal[nIdx].pos.x, g_aMouseLocal[nIdx].pos.y + g_aMouseLocal[nIdx].fHeight * scaleY, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_aMouseLocal[nIdx].pos.x + g_aMouseLocal[nIdx].fWidth * scaleX, g_aMouseLocal[nIdx].pos.y + g_aMouseLocal[nIdx].fHeight * scaleY, 0.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffMouseLocal->Unlock();
}


void SetTextureMouseLocal(	int nIdx,						//记录当前对应的图组地址
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
		g_pD3DVtxBuffMouseLocal->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pD3DVtxBuffMouseLocal->Unlock();
	}
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetMouseLocal(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int type)
{
	for(int nCntBot = 0; nCntBot < MAX_MOUSELOCAL; nCntBot++)
	{
		if(!g_aMouseLocal[nCntBot].bUse)
		{
			g_aMouseLocal[nCntBot].pos = pos;
			g_aMouseLocal[nCntBot].fWidth = fWidth;
			g_aMouseLocal[nCntBot].fHeight = fHeight;
			g_aMouseLocal[nCntBot].bUse = true;
			g_aMouseLocal[nCntBot].nType = type;
			g_aMouseLocal[nCntBot].nCounterAnim = 0;
			g_aMouseLocal[nCntBot].nPatternAnim = 0;

			SetVertexMouseLocal(nCntBot, 1, 1);
			
			// 頂点カラーの設定
			SetColorMouseLocal(nCntBot, col);
			break;
		}
	}
}