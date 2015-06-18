/*****************************************************************************
 *
 * スライド表示処理 [slider.cpp]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
#include "slider.h"
#include "input.h"
#include "gameManager.h"
#include "playbar.h"
#include "timeCounter.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
////////////////////////////
// 默认位置和尺寸
////////////////////////////
#define	SLIDER_POS_X			(SLIDER_SIZE_X / 2)						// 默认の位置(X座標)
#define	SLIDER_POS_Y			(SCREEN_HEIGHT - SLIDER_SIZE_Y / 2	)	// 默认の位置(Y座標)
#define	SLIDER_SIZE_X			(30)									// 默认の幅
#define	SLIDER_SIZE_Y			(100)									// 默认の高さ

////////////////////////////
// 1列6行图组动画
////////////////////////////
//#define	TEX_PATTERN_DIVIDE_X1		(1)												// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
//#define	TEX_PATTERN_DIVIDE_Y6		(6)												// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)
//#define	NUM_ANIM_PATTERN_X1_Y6		(TEX_PATTERN_DIVIDE_X1 * TEX_PATTERN_DIVIDE_Y6)	// アニメーションのパターン数

////////////////////////////
// 动画播放帧数
////////////////////////////
//#define	TIME_CHANSLIDER_PATTERN_MAGIC		(9)				// アニメーションの切り替わるタイミング

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexSlider(LPDIRECT3DDEVICE9 pDevice);
void SetColorSlider(int nIdxSlider, D3DXCOLOR col);

void SetVertexSlider(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组polygon地址
							float scaleX,				// テクスチャのX軸のスケール		X轴的比例参数
							float scaleY);				// テクスチャのY軸のスケール		Y轴的比例参数

void SetTextureSlider(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组地址
							int nPatternAnim,			// 今のテクスチャの番号				记录当前显示帧
							int numAnimPattern, 		// テクスチャ全部の個数				记录当前对应的组成图组的图片数量
							int texPatternDivideX,		// 横のテクスチャの個数				记录当前图组X轴图片数量
							int texPatternDivideY );	// 縦のテクスチャの個数				记录当前图组Y轴图片数量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTextureSlider[MAX_SLIDER] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffSlider = NULL;				// 頂点バッファインターフェースへのポインタ

SLIDER					g_aSLIDER[MAX_SLIDER];					// 木ワーク

//////////////////////
// 贴图的种类路径
//////////////////////
const char *c_pTextureSlider[SLIDER_TYPE_MAX]=
{
	"data/TEXTURE/playbar/playPoint.png",			// 播放条滑块
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSlider(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexSlider(pDevice);

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < SLIDER_TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
									c_pTextureSlider[nCntTex],				// ファイルの名前
									&g_apD3DTextureSlider[nCntTex]);		// 読み込むメモリー
	}

	for(int nCntSlider = 0; nCntSlider < MAX_SLIDER; nCntSlider++)
	{
		g_aSLIDER[nCntSlider].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSLIDER[nCntSlider].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aSLIDER[nCntSlider].fWidth = 0;
		g_aSLIDER[nCntSlider].fHeight = 0;
		g_aSLIDER[nCntSlider].fSpeed = abs(SLIDER_POS_X - (SCREEN_WIDTH - (SLIDER_SIZE_X / 2))) / (TIME_15 * UPDATA_FRAME);

		g_aSLIDER[nCntSlider].bUse = false;

		g_aSLIDER[nCntSlider].nCounterAnim = 0;
		g_aSLIDER[nCntSlider].nPatternAnim = 0;
		g_aSLIDER[nCntSlider].nType = -1;
	}

	SetSlider(D3DXVECTOR3(SLIDER_POS_X, SLIDER_POS_Y, 0), SLIDER_SIZE_X, SLIDER_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SLIDER_POINT);
	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitSlider(void)
{
	for(int nCntTex = 0; nCntTex < MAX_SLIDER; nCntTex++)
	{
		if(g_apD3DTextureSlider[nCntTex] != NULL)
		{// テクスチャの開放
			g_apD3DTextureSlider[nCntTex]->Release();
			g_apD3DTextureSlider[nCntTex] = NULL;
		}
	}

	if(g_pD3DVtxBuffSlider != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffSlider->Release();
		g_pD3DVtxBuffSlider = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSlider(void)
{
	for(int nCntSlider = 0; nCntSlider < MAX_SLIDER; nCntSlider++)
	{
		if(g_aSLIDER[nCntSlider].bUse == true)
		{
			if(*GetIsPlay() == true && !*GetIsPause())
			{
				if(GetTimeCounter()->fCurrentTime <= (*PlaybarNum()) * TIME_5)
					g_aSLIDER[nCntSlider].pos.x += g_aSLIDER[nCntSlider].fSpeed;
			}
			else if(*GetIsStop() == true)
				g_aSLIDER[nCntSlider].pos.x = SLIDER_POS_X;

			SetVertexSlider(nCntSlider, 1, 1);	
			SetTextureSlider(nCntSlider, 0, 1, 1, 1);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSlider(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffSlider, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntSlider = 0; nCntSlider < MAX_SLIDER; nCntSlider++)
	{
		if(g_aSLIDER[nCntSlider].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureSlider[g_aSLIDER[nCntSlider].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntSlider * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexSlider(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_SLIDER,		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,									// 頂点バッファの使用法　
												FVF_VERTEX_2D,										// 使用する頂点フォーマット
												D3DPOOL_MANAGED,									// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffSlider,							// 頂点バッファインターフェースへのポインタ
												NULL)))												// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffSlider->Lock(0, 0, (void**)&pVtx, 0);


		for(int nCntSlider = 0; nCntSlider < MAX_SLIDER; nCntSlider++, pVtx += 4)
		{
		// 頂点座標の設定
			//pVtx[0].vtx = D3DXVECTOR3(g_slider[SLIDER_MAX].pos.x - SLIDER_SIZE_X / 2, g_slider[SLIDER_MAX].pos.y - SLIDER_SIZE_Y / 2, 0.0f);
			//pVtx[1].vtx = D3DXVECTOR3(g_slider[SLIDER_MAX].pos.x + SLIDER_SIZE_X / 2, g_slider[SLIDER_MAX].pos.y - SLIDER_SIZE_Y / 2, 0.0f);
			//pVtx[2].vtx = D3DXVECTOR3(g_slider[SLIDER_MAX].pos.x - SLIDER_SIZE_X / 2, g_slider[SLIDER_MAX].pos.y + SLIDER_SIZE_Y / 2, 0.0f);
			//pVtx[3].vtx = D3DXVECTOR3(g_slider[SLIDER_MAX].pos.x + SLIDER_SIZE_X / 2, g_slider[SLIDER_MAX].pos.y + SLIDER_SIZE_Y / 2, 0.0f);

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
			g_pD3DVtxBuffSlider->Unlock();
		}
	}

	return S_OK;
}



//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorSlider(int nIdxSlider, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffSlider->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdxSlider * 4);

	// 頂点座標の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	g_pD3DVtxBuffSlider->Unlock();
}

void SetVertexSlider(	int nIdx,					// 今のテクスチャの配列番号			记录当前对应的图组polygon地址
						float scaleX,				// テクスチャのX軸のスケール		X轴的比例参数
						float scaleY)				// テクスチャのY軸のスケール		Y轴的比例参数
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffSlider->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(g_aSLIDER[nIdx].pos.x - g_aSLIDER[nIdx].fWidth / 2 * scaleX, g_aSLIDER[nIdx].pos.y - g_aSLIDER[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_aSLIDER[nIdx].pos.x + g_aSLIDER[nIdx].fWidth / 2 * scaleX, g_aSLIDER[nIdx].pos.y - g_aSLIDER[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_aSLIDER[nIdx].pos.x - g_aSLIDER[nIdx].fWidth / 2 * scaleX, g_aSLIDER[nIdx].pos.y + g_aSLIDER[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_aSLIDER[nIdx].pos.x + g_aSLIDER[nIdx].fWidth / 2 * scaleX, g_aSLIDER[nIdx].pos.y + g_aSLIDER[nIdx].fHeight / 2 * scaleY, 0.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffSlider->Unlock();
}


void SetTextureSlider(	int nIdx,					// 今のテクスチャの配列番号			记录当前对应的图组地址
						int nPatternAnim,			// 今のテクスチャの番号				记录当前显示帧
						int numAnimPattern, 		// テクスチャ全部の個数				记录当前对应的组成图组的图片数量
						int texPatternDivideX,		// 横のテクスチャの個数				记录当前图组X轴图片数量
						int texPatternDivideY )		// 縦のテクスチャの個数				记录当前图组Y轴图片数量
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffSlider->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pD3DVtxBuffSlider->Unlock();
	}
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetSlider(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int type)
{
	for(int nCntSlider = 0; nCntSlider < MAX_SLIDER; nCntSlider++)
	{
		if(!g_aSLIDER[nCntSlider].bUse)
		{
			g_aSLIDER[nCntSlider].pos = pos;
			g_aSLIDER[nCntSlider].fWidth = fWidth;
			g_aSLIDER[nCntSlider].fHeight = fHeight;
			g_aSLIDER[nCntSlider].bUse = true;
			g_aSLIDER[nCntSlider].nType = type;
			g_aSLIDER[nCntSlider].nCounterAnim = 0;
			g_aSLIDER[nCntSlider].nPatternAnim = 0;
			
			SetVertexSlider(nCntSlider, 1, 1);
			
			// 頂点カラーの設定
			SetColorSlider(nCntSlider, col);
			break;
		}
	}
}