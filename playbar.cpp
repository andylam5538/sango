/*****************************************************************************
 *
 * 時間軸表示処理 [playbar.cpp]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 * 
 ******************************************************************************/
#include "playbar.h"
#include "input.h"
#include "gameManager.h"
#include "timeTrack.h"
#include "effect.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
////////////////////////////
// 默认位置和尺寸
////////////////////////////
#define	PLAYBAR_POS_X			(0)									// 默认の位置(X座標)
#define	PLAYBAR_POS_Y			(0)									// 默认の位置(Y座標)
#define	PLAYBAR_SIZE_X			(SCREEN_WIDTH)						// 默认の幅
#define	PLAYBAR_SIZE_Y			(SCREEN_HEIGHT)						// 默认の高さ

////////////////////////////
// 播放条尺寸
////////////////////////////
#define	PLAYBAR_LONG_POS_X		(SCREEN_WIDTH / 2)					// 默认の位置(X座標)
#define	PLAYBAR_LONG_POS_Y		(200)								// 默认の位置(Y座標)
#define	PLAYBAR_LONG_SIZE_X		(1100)								// 默认の幅
#define	PLAYBAR_LONG_SIZE_Y		(100)								// 默认の高さ

#define	PLAYBAR_3_SIZE_X		(400)								// 3_1の幅
#define	PLAYBAR_3_SIZE_Y		(100)								// 3_1の高さ

#define	PLAYBAR_3_POS01_X		(PLAYBAR_3_SIZE_X / 2)							// 3_1の位置(X座標)左
#define	PLAYBAR_3_POS02_X		(PLAYBAR_3_SIZE_X / 2 + PLAYBAR_3_SIZE_X)		// 3_1の位置(X座標)真ん中
#define	PLAYBAR_3_POS03_X		(PLAYBAR_3_SIZE_X / 2 + PLAYBAR_3_SIZE_X * 2)	// 3_1の位置(X座標)右

#define	PLAYBAR_3_1_TIME_S		(0)
#define	PLAYBAR_3_1_TIME_E		(5)
#define	PLAYBAR_3_2_TIME_S		(5)
#define	PLAYBAR_3_2_TIME_E		(10)
#define	PLAYBAR_3_3_TIME_S		(10)
#define	PLAYBAR_3_3_TIME_E		(15)

#define	PLAYBAR_3_POS_Y			(800 + PLAYBAR_3_SIZE_Y / 2)		// 3_1の位置(Y座標)

////////////////////////////
// 1列6行图组动画
////////////////////////////
#define	TEX_PATTERN_DIVIDE_X1		(1)												// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y6		(6)												// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)
#define	NUM_ANIM_PATTERN_X1_Y6		(TEX_PATTERN_DIVIDE_X1 * TEX_PATTERN_DIVIDE_Y6)	// アニメーションのパターン数

////////////////////////////
// 动画播放帧数
////////////////////////////
//#define	TIME_CHANPLAYBAR_PATTERN_MAGIC		(9)				// アニメーションの切り替わるタイミング

#define	PLAYBAR_MOVE_SPEED			(15)						// 移動量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlaybar(LPDIRECT3DDEVICE9 pDevice);
void SetColorPlaybar(int nIdxPlaybar, D3DXCOLOR col);

void SetVertexPlaybar(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组polygon地址
						float scaleX,					// テクスチャのX軸のスケール		X轴的比例参数
						float scaleY);					// テクスチャのY軸のスケール		Y轴的比例参数

void SetTexturePlaybar(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组地址
						int nPatternAnim,				// 今のテクスチャの番号				记录当前显示帧
						int numAnimPattern, 			// テクスチャ全部の個数				记录当前对应的组成图组的图片数量
						int texPatternDivideX,			// 横のテクスチャの個数				记录当前图组X轴图片数量
						int texPatternDivideY );		// 縦のテクスチャの個数				记录当前图组Y轴图片数量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTexturePlaybar[MAX_PLAYBAR] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffPlaybar = NULL;					// 頂点バッファインターフェースへのポインタ

PLAYBAR					g_aPlaybar[MAX_PLAYBAR];			// 木ワーク

bool					g_bIsPlaybarMove;					// 时间条是否移动
int						g_nBeControlNum;
int						g_nPlaybarNum;						// 時間軸全部の個数
float					g_fDistPbar2MousX, g_fDistPbar2MousY;

//////////////////////
// 贴图的种类路径
//////////////////////
const char *c_pTexturePlaybar[PLAYBAR_TYPE_MAX]=
{
	"data/TEXTURE/playbar/400_200_R.png",						// 进度条1
	"data/TEXTURE/playbar/400_200_G.png",						// 进度条2
	"data/TEXTURE/playbar/400_200_B.png",						// 进度条3
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlaybar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexPlaybar(pDevice);

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < PLAYBAR_TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
									c_pTexturePlaybar[nCntTex],					// ファイルの名前
									&g_apD3DTexturePlaybar[nCntTex]);	// 読み込むメモリー
	}

	for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++)
	{
		g_aPlaybar[nCntPlayBar].fWidth = 0;
		g_aPlaybar[nCntPlayBar].fHeight = 0;
		g_aPlaybar[nCntPlayBar].fInitialPosX = 0;
		g_aPlaybar[nCntPlayBar].fTimeStart = -1;
		g_aPlaybar[nCntPlayBar].fTimeEnd = -1;

		g_aPlaybar[nCntPlayBar].bUse = false;
		g_aPlaybar[nCntPlayBar].bIsInvert = false;

		g_aPlaybar[nCntPlayBar].nCounterAnim = 0;
		g_aPlaybar[nCntPlayBar].nPatternAnim = 0;
		g_aPlaybar[nCntPlayBar].nType = -1;
		g_aPlaybar[nCntPlayBar].nPriority = -1;
	}

	g_bIsPlaybarMove = false;
	g_nBeControlNum = -1;						// 当前控制的播放条
	g_nPlaybarNum = 0;
	g_fDistPbar2MousX = 0;
	g_fDistPbar2MousY = 0;

	//SetPlaybar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYBAR_3_1);
	
	PlaybarNumCount();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitPlaybar(void)
{
	for(int nCntTex = 0; nCntTex < MAX_PLAYBAR; nCntTex++)
	{
		if(g_apD3DTexturePlaybar[nCntTex] != NULL)
		{// テクスチャの開放
			g_apD3DTexturePlaybar[nCntTex]->Release();
			g_apD3DTexturePlaybar[nCntTex] = NULL;
		}
	}

	if(g_pD3DVtxBuffPlaybar != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffPlaybar->Release();
		g_pD3DVtxBuffPlaybar = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlaybar(void)
{
	if(!*GetIsPlay())
	{
		// 如果鼠标左键单击的时候，当前位置在时间条的范围内，那么该条跟随鼠标移动
		// シークバーがマウスの位置にくっついている
		if(IsMouseLeftPressed())
		{
			if(!g_bIsPlaybarMove)
				for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++)
				{
					if(g_aPlaybar[nCntPlayBar].bUse == true)
					{
						float fX = *GetMouX(), fY = *GetMouY();
						if(	fX >= g_aPlaybar[nCntPlayBar].pos.x - g_aPlaybar[nCntPlayBar].fWidth / 2 && 
							fX <= g_aPlaybar[nCntPlayBar].pos.x + g_aPlaybar[nCntPlayBar].fWidth / 2 &&
							fY >= g_aPlaybar[nCntPlayBar].pos.y - g_aPlaybar[nCntPlayBar].fHeight / 2 &&
							fY <= g_aPlaybar[nCntPlayBar].pos.y + g_aPlaybar[nCntPlayBar].fHeight / 2)
						{
							{
								// X軸
								g_fDistPbar2MousX = abs(g_aPlaybar[nCntPlayBar].pos.x - fX);
								// Y軸
								//g_fDistPbar2MousY = abs(g_aPlaybar[nCntPlayBar].pos.y - fY);
							}
							g_bIsPlaybarMove = true;
							g_nBeControlNum = nCntPlayBar;
						}
						//SetBarEffect(&g_aPlaybar[nCntPlayBar]);
					}
				}

			if(g_bIsPlaybarMove == true)
			{
				float fX = *GetMouX(), fY = *GetMouY();
				// X軸
				if(fX < g_aPlaybar[g_nBeControlNum].pos.x)
					g_aPlaybar[g_nBeControlNum].pos.x = fX + g_fDistPbar2MousX;
				else
					g_aPlaybar[g_nBeControlNum].pos.x = fX - g_fDistPbar2MousX;
				// Y軸
				//if(nY < g_aPlaybar[g_nBeControlNum].pos.y)
				//	g_aPlaybar[g_nBeControlNum].pos.y = fY + g_fDistPbar2MousY;
				//else
				//	g_aPlaybar[g_nBeControlNum].pos.y = fY - g_fDistPbar2MousY;
			}

			// 重排轨迹线
			*GetCleanupSwitch() = false;
		}
		else
		{
			g_bIsPlaybarMove = false;
		}

		if(IsMouseRightTriggered())
		{
			for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++)
			{
				if(g_aPlaybar[nCntPlayBar].bUse == true)
				{
					float fX = *GetMouX(), fY = *GetMouY();
					if(	fX >= g_aPlaybar[nCntPlayBar].pos.x - g_aPlaybar[nCntPlayBar].fWidth / 2 && 
						fX <= g_aPlaybar[nCntPlayBar].pos.x + g_aPlaybar[nCntPlayBar].fWidth / 2 &&
						fY >= g_aPlaybar[nCntPlayBar].pos.y - g_aPlaybar[nCntPlayBar].fHeight / 2 &&
						fY <= g_aPlaybar[nCntPlayBar].pos.y + g_aPlaybar[nCntPlayBar].fHeight / 2)
					{
						g_aPlaybar[nCntPlayBar].bIsInvert = g_aPlaybar[nCntPlayBar].bIsInvert ? false : true;
						float fTempChange = 0;
						fTempChange = g_aPlaybar[nCntPlayBar].fTimeEnd;
						g_aPlaybar[nCntPlayBar].fTimeEnd = g_aPlaybar[nCntPlayBar].fTimeStart;
						g_aPlaybar[nCntPlayBar].fTimeStart = fTempChange;
						// 重排轨迹线
						*GetCleanupSwitch() = false;
					}
				}
			}
		}

		for(int nCntPlayBar = 0; nCntPlayBar < g_nPlaybarNum; nCntPlayBar++)
		{
			if(g_aPlaybar[nCntPlayBar].bUse)
			{
				// 
				if(	g_aPlaybar[nCntPlayBar].nType == PLAYBAR_3_1 ||
					g_aPlaybar[nCntPlayBar].nType == PLAYBAR_3_2 ||
					g_aPlaybar[nCntPlayBar].nType == PLAYBAR_3_3)
				{

					if(!g_aPlaybar[nCntPlayBar].bIsInvert)
						SetTexturePlaybar(nCntPlayBar, 0, 2, 1, 2);
					else
						SetTexturePlaybar(nCntPlayBar, 1, 2, 1, 2);

					SetVertexPlaybar(nCntPlayBar, 1, 1);
				}
			}
		}
	}

	//g_aPlaybar[0].pos.x = *GetMouX();
	//g_aPlaybar[0].pos.y = *GetMouY();
	if(g_nBeControlNum != -1)
		PlaybarPosChange(g_nBeControlNum);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlaybar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffPlaybar, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++)
	{
		if(g_aPlaybar[nCntPlayBar].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTexturePlaybar[g_aPlaybar[nCntPlayBar].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlayBar * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexPlaybar(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_PLAYBAR,		// 頂点データ用に確保するバッファサイズ(バイト単位)
											D3DUSAGE_WRITEONLY,									// 頂点バッファの使用法　
											FVF_VERTEX_2D,										// 使用する頂点フォーマット
											D3DPOOL_MANAGED,									// リソースのバッファを保持するメモリクラスを指定
											&g_pD3DVtxBuffPlaybar,								// 頂点バッファインターフェースへのポインタ
											NULL)))												// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffPlaybar->Lock(0, 0, (void**)&pVtx, 0);


		for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++, pVtx += 4)
		{
		// 頂点座標の設定
			//pVtx[0].vtx = D3DXVECTOR3(g_playbar[PLAYBAR_MAX].pos.x - PLAYBAR_SIZE_X / 2, g_playbar[PLAYBAR_MAX].pos.y - PLAYBAR_SIZE_Y / 2, 0.0f);
			//pVtx[1].vtx = D3DXVECTOR3(g_playbar[PLAYBAR_MAX].pos.x + PLAYBAR_SIZE_X / 2, g_playbar[PLAYBAR_MAX].pos.y - PLAYBAR_SIZE_Y / 2, 0.0f);
			//pVtx[2].vtx = D3DXVECTOR3(g_playbar[PLAYBAR_MAX].pos.x - PLAYBAR_SIZE_X / 2, g_playbar[PLAYBAR_MAX].pos.y + PLAYBAR_SIZE_Y / 2, 0.0f);
			//pVtx[3].vtx = D3DXVECTOR3(g_playbar[PLAYBAR_MAX].pos.x + PLAYBAR_SIZE_X / 2, g_playbar[PLAYBAR_MAX].pos.y + PLAYBAR_SIZE_Y / 2, 0.0f);

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
			g_pD3DVtxBuffPlaybar->Unlock();
		}
	}

	return S_OK;
}



//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorPlaybar(int nIdxPlaybar, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffPlaybar->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdxPlaybar * 4);

	// 頂点座標の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	g_pD3DVtxBuffPlaybar->Unlock();
}

void SetVertexPlaybar(	int nIdx,					//记录当前对应的图组polygon地址
						float scaleX,				//X轴的放大参数
						float scaleY)				//自身Y轴的大小参数
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffPlaybar->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(g_aPlaybar[nIdx].pos.x - g_aPlaybar[nIdx].fWidth / 2 * scaleX, g_aPlaybar[nIdx].pos.y - g_aPlaybar[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_aPlaybar[nIdx].pos.x + g_aPlaybar[nIdx].fWidth / 2 * scaleX, g_aPlaybar[nIdx].pos.y - g_aPlaybar[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_aPlaybar[nIdx].pos.x - g_aPlaybar[nIdx].fWidth / 2 * scaleX, g_aPlaybar[nIdx].pos.y + g_aPlaybar[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_aPlaybar[nIdx].pos.x + g_aPlaybar[nIdx].fWidth / 2 * scaleX, g_aPlaybar[nIdx].pos.y + g_aPlaybar[nIdx].fHeight / 2 * scaleY, 0.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffPlaybar->Unlock();
}


void SetTexturePlaybar(	int nIdx,					//记录当前对应的图组地址
						int nPatternAnim,			//记录当前显示帧
						int numAnimPattern, 		//记录当前对应的组成图组的图片数量
						int texPatternDivideX,		//记录当前图组X轴图片数量
						int texPatternDivideY )		//记录当前图组Y轴图片数量
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffPlaybar->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pD3DVtxBuffPlaybar->Unlock();
	}
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetPlaybar(D3DXCOLOR col, int ntype)
{
	for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++)
	{
		if(!g_aPlaybar[nCntPlayBar].bUse)
		{
			g_aPlaybar[nCntPlayBar].bUse = true;
			g_aPlaybar[nCntPlayBar].bIsInvert = false;
			g_aPlaybar[nCntPlayBar].nCounterAnim = 0;
			g_aPlaybar[nCntPlayBar].nPatternAnim = 0;
			
			switch (ntype)
			{
			case PLAYBAR_3_1:
				g_aPlaybar[nCntPlayBar].pos = D3DXVECTOR3(PLAYBAR_3_POS01_X, PLAYBAR_3_POS_Y, 0.0f);
				g_aPlaybar[nCntPlayBar].fInitialPosX = g_aPlaybar[nCntPlayBar].pos.x;
				g_aPlaybar[nCntPlayBar].fWidth = PLAYBAR_3_SIZE_X;
				g_aPlaybar[nCntPlayBar].fHeight = PLAYBAR_3_SIZE_Y;
				g_aPlaybar[nCntPlayBar].nType = PLAYBAR_3_1;
				g_aPlaybar[nCntPlayBar].nPriority = PRIORITY_FIRST;
				g_aPlaybar[nCntPlayBar].fTimeStart = PLAYBAR_3_1_TIME_S;
				g_aPlaybar[nCntPlayBar].fTimeEnd = PLAYBAR_3_1_TIME_E;
				break;

			case PLAYBAR_3_2:
				g_aPlaybar[nCntPlayBar].pos = D3DXVECTOR3(PLAYBAR_3_POS02_X, PLAYBAR_3_POS_Y, 0.0f);
				g_aPlaybar[nCntPlayBar].fInitialPosX = g_aPlaybar[nCntPlayBar].pos.x;
				g_aPlaybar[nCntPlayBar].fWidth = PLAYBAR_3_SIZE_X;
				g_aPlaybar[nCntPlayBar].fHeight = PLAYBAR_3_SIZE_Y;
				g_aPlaybar[nCntPlayBar].nType = PLAYBAR_3_2;
				g_aPlaybar[nCntPlayBar].nPriority = PRIORITY_SECOND;
				g_aPlaybar[nCntPlayBar].fTimeStart = PLAYBAR_3_2_TIME_S;
				g_aPlaybar[nCntPlayBar].fTimeEnd = PLAYBAR_3_2_TIME_E;
				break;

			case PLAYBAR_3_3:
				g_aPlaybar[nCntPlayBar].pos = D3DXVECTOR3(PLAYBAR_3_POS03_X, PLAYBAR_3_POS_Y, 0.0f);
				g_aPlaybar[nCntPlayBar].fInitialPosX = g_aPlaybar[nCntPlayBar].pos.x;
				g_aPlaybar[nCntPlayBar].fWidth = PLAYBAR_3_SIZE_X;
				g_aPlaybar[nCntPlayBar].fHeight = PLAYBAR_3_SIZE_Y;
				g_aPlaybar[nCntPlayBar].nType = PLAYBAR_3_3;
				g_aPlaybar[nCntPlayBar].nPriority = PRIORITY_THIRD;
				g_aPlaybar[nCntPlayBar].fTimeStart = PLAYBAR_3_3_TIME_S;
				g_aPlaybar[nCntPlayBar].fTimeEnd = PLAYBAR_3_3_TIME_E;
				break;
			}

			SetVertexPlaybar(nCntPlayBar, 1, 1);
			
			// 頂点カラーの設定
			SetColorPlaybar(nCntPlayBar, col);

			break;
		}
	}
}

void PlaybarPosChange(int nBeControlNum)
{
	// 中心点交換
	for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++)
	{
		if(g_aPlaybar[nCntPlayBar].bUse == true && nBeControlNum != nCntPlayBar)
		{
			// 先ず、当たり判定
			// 时间轴首先进行碰撞判定
			if(	g_aPlaybar[nBeControlNum].pos.x + PLAYBAR_3_SIZE_X / 2 > g_aPlaybar[nCntPlayBar].pos.x - PLAYBAR_3_SIZE_X / 2 &&
				g_aPlaybar[nBeControlNum].pos.x - PLAYBAR_3_SIZE_X / 2 < g_aPlaybar[nCntPlayBar].pos.x + PLAYBAR_3_SIZE_X / 2)
			{
				// 次、シークバーはどこから
				// 接着判断碰撞时是从哪个方向移动过来的 从左到右
				if(g_aPlaybar[nBeControlNum].pos.x + PLAYBAR_3_SIZE_X / 2 > g_aPlaybar[nCntPlayBar].fInitialPosX &&
					g_aPlaybar[nBeControlNum].fInitialPosX < g_aPlaybar[nCntPlayBar].fInitialPosX)
				{
					float fTemp = 0;
					int nTemp = 0;
					fTemp = g_aPlaybar[nCntPlayBar].fInitialPosX;
					g_aPlaybar[nCntPlayBar].fInitialPosX = g_aPlaybar[nBeControlNum].fInitialPosX;
					g_aPlaybar[nBeControlNum].fInitialPosX = fTemp;

					nTemp = g_aPlaybar[nCntPlayBar].nPriority;
					g_aPlaybar[nCntPlayBar].nPriority = g_aPlaybar[nBeControlNum].nPriority;
					g_aPlaybar[nBeControlNum].nPriority = nTemp;
				}
				// 从右到左
				else if(g_aPlaybar[nBeControlNum].pos.x - PLAYBAR_3_SIZE_X / 2 < g_aPlaybar[nCntPlayBar].fInitialPosX &&
						g_aPlaybar[nBeControlNum].fInitialPosX > g_aPlaybar[nCntPlayBar].fInitialPosX)
				{
					float fTemp = 0;
					int nTemp = 0;
					fTemp = g_aPlaybar[nCntPlayBar].fInitialPosX;
					g_aPlaybar[nCntPlayBar].fInitialPosX = g_aPlaybar[nBeControlNum].fInitialPosX;
					g_aPlaybar[nBeControlNum].fInitialPosX = fTemp;

					nTemp = g_aPlaybar[nCntPlayBar].nPriority;
					g_aPlaybar[nCntPlayBar].nPriority = g_aPlaybar[nBeControlNum].nPriority;
					g_aPlaybar[nBeControlNum].nPriority = nTemp;
				}
			}
		}
	}
	// 時間軸移動
	for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++)
	{
		if(g_aPlaybar[nCntPlayBar].bUse == true)
		{
			if(g_aPlaybar[nCntPlayBar].pos.x < g_aPlaybar[nCntPlayBar].fInitialPosX)
			{
				g_aPlaybar[nCntPlayBar].pos.x += PLAYBAR_MOVE_SPEED;
				if(g_aPlaybar[nCntPlayBar].pos.x > g_aPlaybar[nCntPlayBar].fInitialPosX)
				{
					g_aPlaybar[nCntPlayBar].pos.x = g_aPlaybar[nCntPlayBar].fInitialPosX;
				}
			}
			else if(g_aPlaybar[nCntPlayBar].pos.x > g_aPlaybar[nCntPlayBar].fInitialPosX)
			{
				g_aPlaybar[nCntPlayBar].pos.x -= PLAYBAR_MOVE_SPEED;
				if(g_aPlaybar[nCntPlayBar].pos.x < g_aPlaybar[nCntPlayBar].fInitialPosX)
				{
					g_aPlaybar[nCntPlayBar].pos.x = g_aPlaybar[nCntPlayBar].fInitialPosX;
				}
			}
		}
	}
}

// 時間軸のデータゲット
PLAYBAR *GetPlaybar(void)
{
	return g_aPlaybar;
}
// 時間軸全部の個数 計算
void PlaybarNumCount(void)
{
	g_nPlaybarNum = 0;
	for(int nCntPlayBar = 0; nCntPlayBar < MAX_PLAYBAR; nCntPlayBar++)
	{
		if(g_aPlaybar[nCntPlayBar].bUse == true)
			g_nPlaybarNum++;
	}
}
// 時間軸全部の個数 ゲット
int *PlaybarNum(void)
{
	return &g_nPlaybarNum;
}
// 読み込むデータを初期化
void InitDataPlayerbar(void)
{
	switch(g_nPlaybarNum)
	{
	case 1:
	{
		SetPlaybar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYBAR_3_1);
		break;
	}
	case 2:
	{
		SetPlaybar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYBAR_3_1);
		SetPlaybar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYBAR_3_2);
		break;
	}
	case 3:
	{
		SetPlaybar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYBAR_3_1);
		SetPlaybar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYBAR_3_2);
		SetPlaybar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYBAR_3_3);
		break;
	}
	default:
		break;
	}
}