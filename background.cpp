/*****************************************************************************
 *
 * 背景表示処理 [background.cpp]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 * 
 ******************************************************************************/
#include "background.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
////////////////////////////
// 默认位置和尺寸
////////////////////////////
#define	BACKGROUND_POS_X			(SCREEN_WIDTH / 2)					// 默认の位置(X座標)
#define	BACKGROUND_POS_Y			(SCREEN_HEIGHT / 2)					// 默认の位置(Y座標)
#define	BACKGROUND_SIZE_X			(SCREEN_WIDTH)						// 默认の幅
#define	BACKGROUND_SIZE_Y			(SCREEN_HEIGHT)						// 默认の高さ
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBackground(LPDIRECT3DDEVICE9 pDevice);
void SetColorBackground(int nIdxBackground, D3DXCOLOR col);

void SetVertexBackground(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组polygon地址
							float scaleX,					// テクスチャのX軸のスケール		X轴的比例参数
							float scaleY);					// テクスチャのY軸のスケール		Y轴的比例参数

void SetTextureBackground(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组地址
							int nPatternAnim,				// 今のテクスチャの番号				记录当前显示帧
							int numAnimPattern, 			// テクスチャ全部の個数				记录当前对应的组成图组的图片数量
							int texPatternDivideX,			// 横のテクスチャの個数				记录当前图组X轴图片数量
							int texPatternDivideY );		// 縦のテクスチャの個数				记录当前图组Y轴图片数量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTextureBackground[MAX_BACKGROUND] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBackground = NULL;						// 頂点バッファインターフェースへのポインタ

BACKGROUND				g_aBackground[MAX_BACKGROUND];						// 木ワーク

//////////////////////
// 贴图的种类路径
//////////////////////
const char *c_pTextureBackground[BACKGROUND_TYPE_MAX]=
{
	"data/TEXTURE/background/title.png",					// title
	"data/TEXTURE/background/scene_select.png",				// title
	"data/TEXTURE/background/1200_900_BG.png",				// 背景
	"data/TEXTURE/background/net.png",				// edit background
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexBackground(pDevice);

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < BACKGROUND_TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
									c_pTextureBackground[nCntTex],					// ファイルの名前
									&g_apD3DTextureBackground[nCntTex]);	// 読み込むメモリー
	}

	for(int nCntNum = 0; nCntNum < MAX_BACKGROUND; nCntNum++)
	{
		g_aBackground[nCntNum].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBackground[nCntNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aBackground[nCntNum].fWidth = 0;
		g_aBackground[nCntNum].fHeight = 0;
		g_aBackground[nCntNum].fInitialPosX = 0;

		g_aBackground[nCntNum].bUse = false;

		g_aBackground[nCntNum].nCounterAnim = 0;
		g_aBackground[nCntNum].nPatternAnim = 0;
		g_aBackground[nCntNum].nType = -1;
	}

	SetBackground(D3DXVECTOR3(BACKGROUND_POS_X, BACKGROUND_POS_Y, 0), BACKGROUND_SIZE_X, BACKGROUND_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BACKGROUND_TITLE);
	SetBackground(D3DXVECTOR3(BACKGROUND_POS_X, BACKGROUND_POS_Y, 0), BACKGROUND_SIZE_X, BACKGROUND_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BACKGROUND_SCENE_SELECT);
	SetBackground(D3DXVECTOR3(BACKGROUND_POS_X, BACKGROUND_POS_Y, 0), BACKGROUND_SIZE_X, BACKGROUND_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BACKGROUND_SCENE_01);
	SetBackground(D3DXVECTOR3(BACKGROUND_POS_X, BACKGROUND_POS_Y, 0), BACKGROUND_SIZE_X, BACKGROUND_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BACKGROUND_EDIT_NET);

	return S_OK;
}
//=============================================================================s
// 終了処理
//=============================================================================
void UninitBackground(void)
{
	for(int nCntTex = 0; nCntTex < MAX_BACKGROUND; nCntTex++)
	{
		if(g_apD3DTextureBackground[nCntTex] != NULL)
		{// テクスチャの開放
			g_apD3DTextureBackground[nCntTex]->Release();
			g_apD3DTextureBackground[nCntTex] = NULL;
		}
	}

	if(g_pD3DVtxBuffBackground != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffBackground->Release();
		g_pD3DVtxBuffBackground = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBackground(void)
{
	for(int nCntNum = 0; nCntNum < MAX_BACKGROUND; nCntNum++)
	{
		if(g_aBackground[nCntNum].bUse == true)
		{
			SetVertexBackground(nCntNum, 1, 1);	
			SetTextureBackground(nCntNum, 0, 1, 1, 1);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffBackground, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	switch(*GetMode())
	{
	case GAMEMODE_LOGO:

		break;

	case GAMEMODE_TITLE:
	{
		if(g_aBackground[BACKGROUND_TITLE].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureBackground[g_aBackground[BACKGROUND_TITLE].nType]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (BACKGROUND_TITLE * 4), NUM_POLYGON);
		}
		break;
	}
	case GAMEMODE_STAGE_SELECT:
	{
		if(g_aBackground[BACKGROUND_SCENE_SELECT].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureBackground[g_aBackground[BACKGROUND_SCENE_SELECT].nType]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (BACKGROUND_SCENE_SELECT * 4), NUM_POLYGON);
		}
		break;
	}
	case GAMEMODE_GAME:
	{
		if(g_aBackground[BACKGROUND_SCENE_01].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureBackground[g_aBackground[BACKGROUND_SCENE_01].nType]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (BACKGROUND_SCENE_01 * 4), NUM_POLYGON);
		}
		if(g_aBackground[BACKGROUND_EDIT_NET].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureBackground[g_aBackground[BACKGROUND_EDIT_NET].nType]);
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (BACKGROUND_SCENE_01 * 4), NUM_POLYGON);
		}
		break;
	}
	case GAMEMODE_END:

		break;

	default:
		break;
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBackground(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BACKGROUND,		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,									// 頂点バッファの使用法　
												FVF_VERTEX_2D,										// 使用する頂点フォーマット
												D3DPOOL_MANAGED,									// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffBackground,							// 頂点バッファインターフェースへのポインタ
												NULL)))												// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBackground->Lock(0, 0, (void**)&pVtx, 0);


		for(int nCntNum = 0; nCntNum < MAX_BACKGROUND; nCntNum++, pVtx += 4)
		{
		// 頂点座標の設定
			//pVtx[0].vtx = D3DXVECTOR3(g_background[nCntNum].pos.x - GE_SIZE_X / 2, g_background[nCntNum].pos.y - GE_SIZE_Y / 2, 0.0f);
			//pVtx[1].vtx = D3DXVECTOR3(g_background[nCntNum].pos.x + GE_SIZE_X / 2, g_background[nCntNum].pos.y - GE_SIZE_Y / 2, 0.0f);
			//pVtx[2].vtx = D3DXVECTOR3(g_background[nCntNum].pos.x - GE_SIZE_X / 2, g_background[nCntNum].pos.y + GE_SIZE_Y / 2, 0.0f);
			//pVtx[3].vtx = D3DXVECTOR3(g_background[nCntNum].pos.x + GE_SIZE_X / 2, g_background[nCntNum].pos.y + GE_SIZE_Y / 2, 0.0f);

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
			g_pD3DVtxBuffBackground->Unlock();
		}
	}

	return S_OK;
}



//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorBackground(int nIdxBackground, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffBackground->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdxBackground * 4);

	// 頂点座標の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	g_pD3DVtxBuffBackground->Unlock();
}

void SetVertexBackground(	int nIdx,					//记录当前对应的图组polygon地址
							float scaleX,				//X轴的放大参数
							float scaleY)				//自身Y轴的大小参数
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffBackground->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(g_aBackground[nIdx].pos.x - g_aBackground[nIdx].fWidth / 2 * scaleX, g_aBackground[nIdx].pos.y - g_aBackground[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_aBackground[nIdx].pos.x + g_aBackground[nIdx].fWidth / 2 * scaleX, g_aBackground[nIdx].pos.y - g_aBackground[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_aBackground[nIdx].pos.x - g_aBackground[nIdx].fWidth / 2 * scaleX, g_aBackground[nIdx].pos.y + g_aBackground[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_aBackground[nIdx].pos.x + g_aBackground[nIdx].fWidth / 2 * scaleX, g_aBackground[nIdx].pos.y + g_aBackground[nIdx].fHeight / 2 * scaleY, 0.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffBackground->Unlock();
}


void SetTextureBackground(	int nIdx,						//记录当前对应的图组地址
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
		g_pD3DVtxBuffBackground->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pD3DVtxBuffBackground->Unlock();
	}
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetBackground(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int type)
{
	for(int nCntNum = 0; nCntNum < MAX_BACKGROUND; nCntNum++)
	{
		if(!g_aBackground[nCntNum].bUse)
		{
			g_aBackground[nCntNum].pos = pos;
			g_aBackground[nCntNum].fWidth = fWidth;
			g_aBackground[nCntNum].fHeight = fHeight;
			g_aBackground[nCntNum].bUse = true;
			g_aBackground[nCntNum].nType = type;
			g_aBackground[nCntNum].nCounterAnim = 0;
			g_aBackground[nCntNum].nPatternAnim = 0;
			
			SetVertexBackground(nCntNum, 1, 1);
			
			// 頂点カラーの設定
			SetColorBackground(nCntNum, col);
			break;
		}
	}
}

//=============================================================================
// 背景情報のゲード
//=============================================================================
BACKGROUND *GetBackG(void)
{
	return g_aBackground;
}