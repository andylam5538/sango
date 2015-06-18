/*****************************************************************************
 *
 * ボタン表示処理 [botton.cpp]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 * 
 ******************************************************************************/
#include "botton.h"
#include "input.h"
#include "gameManager.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
////////////////////////////
// 默认位置和尺寸
////////////////////////////
#define	BOTTON_POS_X			(1000)									// 默认の位置(X座標)
#define	BOTTON_POS_Y			(100)									// 默认の位置(Y座標)
#define	BOTTON_SIZE_X			(100)									// 默认の幅
#define	BOTTON_SIZE_Y			(100)									// 默认の高さ
#define BOTTON_SWITCH_SPEED		(20)									// ポタン交換すばやさ
////////////////////////////
// 8列1行图组动画
////////////////////////////
#define	TEX_PATTERN_DIVIDE_X8		(8)												// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y1		(1)												// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)
#define	NUM_ANIM_PATTERN_X8_Y1		(TEX_PATTERN_DIVIDE_X8 * TEX_PATTERN_DIVIDE_Y1)	// アニメーションのパターン数

////////////////////////////
// 动画播放帧数
////////////////////////////
#define	TIME_CHANGE_BOTTON_MOTION	(9)												// アニメーションの切り替わるタイミング

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexBotton(LPDIRECT3DDEVICE9 pDevice);
void SetColorBotton(int nIdxBotton, D3DCOLOR col_3);

void SetVertexBotton(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组polygon地址
						float scaleX,					// テクスチャのX軸のスケール		X轴的比例参数
						float scaleY);					// テクスチャのY軸のスケール		Y轴的比例参数

void SetTextureBotton(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组地址
						int nPatternAnim,				// 今のテクスチャの番号				记录当前显示帧
						int numAnimPattern, 			// テクスチャ全部の個数				记录当前对应的组成图组的图片数量
						int texPatternDivideX,			// 横のテクスチャの個数				记录当前图组X轴图片数量
						int texPatternDivideY );		// 縦のテクスチャの個数				记录当前图组Y轴图片数量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTextureBotton[MAX_BOTTON] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffBotton = NULL;						// 頂点バッファインターフェースへのポインタ

BOTTON					g_aBotton[MAX_BOTTON];			// 木ワーク

// 计数器
int						g_nCounterAnimBotton;
//记录HP图片帧
int						g_nPatternAnimBotton;

float					*g_fBotGetMouX = GetMouX();
float					*g_fBotGetMouY = GetMouY();

//////////////////////
// 贴图的种类路径
//////////////////////
const char *c_pTextureBotton[BOTTON_TYPE_MAX]=		// 不同模式下的背景切换
{
	"data/TEXTURE/bottons/play.png",				// プレー
	"data/TEXTURE/bottons/pause.png",				// ポーズ
	"data/TEXTURE/bottons/stop.png",				// ストップ
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBotton(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexBotton(pDevice);

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < BOTTON_TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,								// デバイスへのポインタ
									c_pTextureBotton[nCntTex],			// ファイルの名前
									&g_apD3DTextureBotton[nCntTex]);	// 読み込むメモリー
	}

	for(int nCntBot = 0; nCntBot < MAX_BOTTON; nCntBot++)
	{
		g_aBotton[nCntBot].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBotton[nCntBot].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aBotton[nCntBot].fWidth = 0;
		g_aBotton[nCntBot].fHeight = 0;
		g_aBotton[nCntBot].fSpeed = 0;
		g_aBotton[nCntBot].fMaxX = 0;
		g_aBotton[nCntBot].fMinX = 0;
		g_aBotton[nCntBot].fMaxY = 0;
		g_aBotton[nCntBot].fMinY = 0;

		g_aBotton[nCntBot].bUse = false;
		g_aBotton[nCntBot].bIsActiva = false;

		g_aBotton[nCntBot].nCounterAnim = 0;
		g_aBotton[nCntBot].nPatternAnim = 0;
		g_aBotton[nCntBot].nType = -1;
		g_aBotton[nCntBot].nAlpha = 255;
	}

	g_nCounterAnimBotton = 0;
	g_nPatternAnimBotton = 0;

	SetBotton(D3DXVECTOR3(BOTTON_POS_X, BOTTON_POS_Y, 0), BOTTON_SIZE_X, BOTTON_SIZE_Y, D3DCOLOR_RGBA(255, 255, 255, 255), BOTTON_PLAY);
	SetBotton(D3DXVECTOR3(BOTTON_POS_X, BOTTON_POS_Y, 0), BOTTON_SIZE_X, BOTTON_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BOTTON_PAUSE);
	SetBotton(D3DXVECTOR3(BOTTON_POS_X + BOTTON_SIZE_X + 10, BOTTON_POS_Y, 0), BOTTON_SIZE_X, BOTTON_SIZE_Y, D3DCOLOR_RGBA(255, 255, 255, 255), BOTTON_STOP);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitBotton(void)
{
	for(int nCntTex = 0; nCntTex < MAX_BOTTON; nCntTex++)
	{
		if(g_apD3DTextureBotton[nCntTex] != NULL)
		{// テクスチャの開放
			g_apD3DTextureBotton[nCntTex]->Release();
			g_apD3DTextureBotton[nCntTex] = NULL;
		}
	}

	if(g_pD3DVtxBuffBotton != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffBotton->Release();
		g_pD3DVtxBuffBotton = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBotton(void)
{
	if(*GetIsStop() == true)
	{
		g_aBotton[BOTTON_PLAY].nAlpha = 255;
		g_aBotton[BOTTON_PAUSE].nAlpha = 0;
	}

	if(*GetGameMode() == SELECTMODE_PLAY)
	{
		if(IsMouseLeftTriggered())
		{
			for(int nCntBot = 0; nCntBot < MAX_BOTTON; nCntBot++)
			{
				if(g_aBotton[nCntBot].bUse)
				{
					if(g_aBotton[nCntBot].nAlpha > 0)
					{
						// 按键碰撞判定
						if(	*g_fBotGetMouX < g_aBotton[nCntBot].fMaxX && *g_fBotGetMouX > g_aBotton[nCntBot].fMinX &&
							*g_fBotGetMouY < g_aBotton[nCntBot].fMaxY && *g_fBotGetMouX > g_aBotton[nCntBot].fMinY)
						{	// 如果按键类型为播放
							if(g_aBotton[nCntBot].nType == BOTTON_PLAY)
							{
								// 被激活代表透明度开始减少
								g_aBotton[BOTTON_PLAY].bIsActiva = true;
								g_aBotton[BOTTON_PAUSE].bIsActiva = false;
								*GetIsPlay() = true;
								*GetIsStop() = false;
								*GetIsPause() = false;
							}
							// 如果按键类型为暂停
							else if(g_aBotton[nCntBot].nType == BOTTON_PAUSE)
							{
								g_aBotton[BOTTON_PLAY].bIsActiva = false;
								g_aBotton[BOTTON_PAUSE].bIsActiva = true;
								*GetIsPause() = true;
							}
							else if(g_aBotton[nCntBot].nType == BOTTON_STOP)
							{
								*GetIsStop() = true;
								*GetIsPlay() = false;
								*GetIsPause() = false;
								GetPlayer()[0].bIsHit = false;
							}
						}
					}
				}
			}
		}
	}

	if(!*GetIsStop())
	{
		if(g_aBotton[BOTTON_PLAY].bIsActiva == true && !g_aBotton[BOTTON_PAUSE].bIsActiva)
		{
			g_aBotton[BOTTON_PLAY].nAlpha -= BOTTON_SWITCH_SPEED;
			if(g_aBotton[BOTTON_PLAY].nAlpha < 0)
				g_aBotton[BOTTON_PLAY].nAlpha = 0;

			g_aBotton[BOTTON_PAUSE].nAlpha += BOTTON_SWITCH_SPEED;
			if(g_aBotton[BOTTON_PAUSE].nAlpha > 255)
				g_aBotton[BOTTON_PAUSE].nAlpha = 255;
		}
		if(g_aBotton[BOTTON_PAUSE].bIsActiva == true && !g_aBotton[BOTTON_PLAY].bIsActiva)
		{
			g_aBotton[BOTTON_PAUSE].nAlpha -= BOTTON_SWITCH_SPEED;
			if(g_aBotton[BOTTON_PAUSE].nAlpha < 0)
				g_aBotton[BOTTON_PAUSE].nAlpha = 0;

			g_aBotton[BOTTON_PLAY].nAlpha += BOTTON_SWITCH_SPEED;
			if(g_aBotton[BOTTON_PLAY].nAlpha > 255)
				g_aBotton[BOTTON_PLAY].nAlpha = 255;
		}
	}

	for(int nCntBot = 0; nCntBot < MAX_BOTTON; nCntBot++)
	{
		if(g_aBotton[nCntBot].bUse)
		{
			SetVertexBotton(nCntBot, 1, 1);
			SetTextureBotton(nCntBot, 0, 1, 1, 1);
			SetColorBotton(nCntBot, D3DCOLOR_RGBA(255, 255, 255, g_aBotton[nCntBot].nAlpha));
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBotton(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffBotton, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntBot = 0; nCntBot < MAX_BOTTON; nCntBot++)
	{
		if(g_aBotton[nCntBot].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureBotton[g_aBotton[nCntBot].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntBot * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexBotton(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_BOTTON,		// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,									// 頂点バッファの使用法　
												FVF_VERTEX_2D,										// 使用する頂点フォーマット
												D3DPOOL_MANAGED,									// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffBotton,							// 頂点バッファインターフェースへのポインタ
												NULL)))												// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffBotton->Lock(0, 0, (void**)&pVtx, 0);


		for(int nCntBot = 0; nCntBot < MAX_BOTTON; nCntBot++, pVtx += 4)
		{
		// 頂点座標の設定
			//pVtx[0].vtx = D3DXVECTOR3(g_botton[Botton_MAX].pos.x - Botton_SIZE_X / 2, g_botton[Botton_MAX].pos.y - Botton_SIZE_Y / 2, 0.0f);
			//pVtx[1].vtx = D3DXVECTOR3(g_botton[Botton_MAX].pos.x + Botton_SIZE_X / 2, g_botton[Botton_MAX].pos.y - Botton_SIZE_Y / 2, 0.0f);
			//pVtx[2].vtx = D3DXVECTOR3(g_botton[Botton_MAX].pos.x - Botton_SIZE_X / 2, g_botton[Botton_MAX].pos.y + Botton_SIZE_Y / 2, 0.0f);
			//pVtx[3].vtx = D3DXVECTOR3(g_botton[Botton_MAX].pos.x + Botton_SIZE_X / 2, g_botton[Botton_MAX].pos.y + Botton_SIZE_Y / 2, 0.0f);

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
			g_pD3DVtxBuffBotton->Unlock();
		}
	}

	return S_OK;
}



//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorBotton(int nIdxBotton, D3DCOLOR col_3)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffBotton->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdxBotton * 4);

	// 頂点座標の設定
	pVtx[0].diffuse =
	pVtx[1].diffuse =
	pVtx[2].diffuse =
	pVtx[3].diffuse = col_3;

	// 頂点データをアンロックする
	g_pD3DVtxBuffBotton->Unlock();
}

void SetVertexBotton(	int nIdx,					//记录当前对应的图组polygon地址
						float scaleX,				//X轴的放大参数
						float scaleY)				//自身Y轴的大小参数
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffBotton->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(g_aBotton[nIdx].pos.x - g_aBotton[nIdx].fWidth / 2 * scaleX, g_aBotton[nIdx].pos.y - g_aBotton[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_aBotton[nIdx].pos.x + g_aBotton[nIdx].fWidth / 2 * scaleX, g_aBotton[nIdx].pos.y - g_aBotton[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_aBotton[nIdx].pos.x - g_aBotton[nIdx].fWidth / 2 * scaleX, g_aBotton[nIdx].pos.y + g_aBotton[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_aBotton[nIdx].pos.x + g_aBotton[nIdx].fWidth / 2 * scaleX, g_aBotton[nIdx].pos.y + g_aBotton[nIdx].fHeight / 2 * scaleY, 0.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffBotton->Unlock();
}


void SetTextureBotton(	int nIdx,						//记录当前对应的图组地址
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
		g_pD3DVtxBuffBotton->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pD3DVtxBuffBotton->Unlock();
	}
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetBotton(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int type)
{
	for(int nCntBot = 0; nCntBot < MAX_BOTTON; nCntBot++)
	{
		if(!g_aBotton[nCntBot].bUse)
		{
			g_aBotton[nCntBot].pos = pos;
			g_aBotton[nCntBot].fWidth = fWidth;
			g_aBotton[nCntBot].fHeight = fHeight;
			g_aBotton[nCntBot].fSpeed = (float)SCREEN_WIDTH / (float)(TIME_15 * 60);
			g_aBotton[nCntBot].bUse = true;
			g_aBotton[nCntBot].nType = type;
			g_aBotton[nCntBot].nCounterAnim = 0;
			g_aBotton[nCntBot].nPatternAnim = 0;
			g_aBotton[nCntBot].nAlpha = 255;

			g_aBotton[nCntBot].fMaxX = pos.x + fWidth / 2;
			g_aBotton[nCntBot].fMinX = pos.x - fWidth / 2;
			g_aBotton[nCntBot].fMaxY = pos.y + fHeight / 2;
			g_aBotton[nCntBot].fMinY = pos.y - fHeight / 2;

			switch(type)
			{
			case BOTTON_PLAY:
				g_aBotton[nCntBot].nAlpha = 255;
				break;
			case BOTTON_PAUSE:
				g_aBotton[nCntBot].nAlpha = 0;
				break;
			default:
				g_aBotton[nCntBot].nAlpha = 255;
				break;
			}

			SetVertexBotton(nCntBot, 1, 1);
			
			// 頂点カラーの設定
			SetColorBotton(nCntBot, col);
			break;
		}
	}
}