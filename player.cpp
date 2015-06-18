/*****************************************************************************
 *
 * プレーヤー表示処理 [g_aPlayer[0].cpp]
 *
 * 作る時間 ： 2015.5.1
 *
 * GP11A 248 朱鷹仁 40582
 * 
 ******************************************************************************/
#include "player.h"
#include "input.h"
#include "gameManager.h"
#include "wall.h"
#include "gimmick.h"
#include "debugproc.h"
#include "fade.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
////////////////////////////
// 默认位置和尺寸
////////////////////////////
#define	PLAYER_POS_X			(PLAYER_SIZE_X / 2)		// 默认の位置(X座標)
#define	PLAYER_POS_Y			(650)					// 默认の位置(Y座標)
#define	PLAYER_SIZE_X			(100)					// 默认の幅
#define	PLAYER_SIZE_Y			(100)					// 默认の高さ
#define PLAYER_RADIUS			(PLAYER_SIZE_X / 4)		// 默认の半径
#define PLAYER_GRAVITY			(0.9f)					// 重量
#define PLAYER_SPEED_X			(5.0f)
#define PLAYER_SPEED_Y			(8.0f)
////////////////////////////
// 8列1行图组动画
////////////////////////////
#define	TEX_PATTERN_DIVIDE_X9		(9)												// アニメーションパターンのテクスチャ内での分割数(Ｘ方向)
#define	TEX_PATTERN_DIVIDE_Y1		(1)												// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)
#define	TEX_PATTERN_DIVIDE_Y2		(2)												// アニメーションパターンのテクスチャ内での分割数(Ｙ方向)
#define	NUM_ANIM_PATTERN_X9_Y1		(TEX_PATTERN_DIVIDE_X9 * TEX_PATTERN_DIVIDE_Y1)	// アニメーションのパターン数
#define	NUM_ANIM_PATTERN_X9_Y2		(TEX_PATTERN_DIVIDE_X9 * TEX_PATTERN_DIVIDE_Y2)	// アニメーションのパターン数

////////////////////////////
// 动画播放帧数
////////////////////////////
#define	TIME_CHANGE_PLAYER_MOTION	(8)												// アニメーションの切り替わるタイミング

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice);
void SetColorPlayer(int nIdxPlayer, D3DXCOLOR col);

void SetVertexPlayer(	int nIdx,							// 今のテクスチャの配列番号			记录当前对应的图组polygon地址
						float scaleX,						// テクスチャのX軸のスケール		X轴的比例参数
						float scaleY);						// テクスチャのY軸のスケール		Y轴的比例参数

void SetTexturePlayer(	int nIdx,							// 今のテクスチャの配列番号			记录当前对应的图组地址
						int nPatternAnim,					// 今のテクスチャの番号				记录当前显示帧
						int numAnimPattern, 				// テクスチャ全部の個数				记录当前对应的组成图组的图片数量
						int texPatternDivideX,				// 横のテクスチャの個数				记录当前图组X轴图片数量
						int texPatternDivideY );			// 縦のテクスチャの個数				记录当前图组Y轴图片数量

bool IsHitBySlope(D3DXVECTOR3 nextPos, D3DXVECTOR2 *Intersection, bool isFall);
bool IsHitByWall(D3DXVECTOR3 nextPos, bool isFall);
bool IsHitByGimmic(D3DXVECTOR3 nextPos, bool isFall);
bool CheckCanWalk(float speed, bool isGoLeft);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTexturePlayer[MAX_PLAYER] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffPlayer = NULL;				// 頂点バッファインターフェースへのポインタ

PLAYER					g_aPlayer[MAX_PLAYER];					// 木ワーク
WALL						*g_pWallDataPlayer = GetWallData();		// 壁のデータゲット
GIMMICK					*g_pGimPlayer = GetGimmickData();
//////////////////////
// 贴图的种类路径
//////////////////////
const char *c_pTexturePlayer[PLAYER_TYPE_MAX]=
{
	"data/TEXTURE/player/player.png",							// プレーヤー
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexPlayer(pDevice);

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < PLAYER_TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,									// デバイスへのポインタ
									c_pTexturePlayer[nCntTex],					// ファイルの名前
									&g_apD3DTexturePlayer[nCntTex]);	// 読み込むメモリー
	}

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_aPlayer[nCntPlayer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer[nCntPlayer].posSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer[nCntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer[nCntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer[nCntPlayer].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_aPlayer[nCntPlayer].fWidth = 0;
		g_aPlayer[nCntPlayer].fHeight = 0;
		g_aPlayer[nCntPlayer].fSpeedX = 0;
		g_aPlayer[nCntPlayer].fSpeedY = 0;
		g_aPlayer[nCntPlayer].fRadius = 0;
		g_aPlayer[nCntPlayer].fMaxX = 0;
		g_aPlayer[nCntPlayer].fMinX = 0;
		g_aPlayer[nCntPlayer].fMaxY = 0;
		g_aPlayer[nCntPlayer].fMinY = 0;

		g_aPlayer[nCntPlayer].bUse = false;
		g_aPlayer[nCntPlayer].bIsHit = false;
		g_aPlayer[nCntPlayer].bIsChangeDir = false;
		g_aPlayer[nCntPlayer].bIsGoLeft = false;	
		g_aPlayer[nCntPlayer].bIsOnfloor= false;	
		g_aPlayer[nCntPlayer].bIsOnSlope= false;
		g_aPlayer[nCntPlayer].bIsHitByGmmic = false;

		g_aPlayer[nCntPlayer].nState = 0;
		g_aPlayer[nCntPlayer].nCounterAnim = 0;
		g_aPlayer[nCntPlayer].nPatternAnim = 0;
		g_aPlayer[nCntPlayer].nType = -1;
	}

	//SetPlayer(D3DXVECTOR3(PLAYER_POS_X, PLAYER_POS_Y, 0), PLAYER_SIZE_X, PLAYER_SIZE_Y, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_001);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for(int nCntTex = 0; nCntTex < MAX_PLAYER; nCntTex++)
	{
		if(g_apD3DTexturePlayer[nCntTex] != NULL)
		{// テクスチャの開放
			g_apD3DTexturePlayer[nCntTex]->Release();
			g_apD3DTexturePlayer[nCntTex] = NULL;
		}
	}

	if(g_pD3DVtxBuffPlayer != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffPlayer->Release();
		g_pD3DVtxBuffPlayer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	D3DXVECTOR3 nextPos;		//次に移動する位置
	D3DXVECTOR2 Intersection = D3DXVECTOR2(0.0f, 0.0f);	//坂道との交点

	// プレーヤー最大範囲と最小範囲の更新
	g_aPlayer[0].fMaxX = g_aPlayer[0].pos.x + g_aPlayer[0].fWidth / 4;
	g_aPlayer[0].fMinX = g_aPlayer[0].pos.x - g_aPlayer[0].fWidth / 4;
	g_aPlayer[0].fMaxY = g_aPlayer[0].pos.y + g_aPlayer[0].fHeight / 2;
	g_aPlayer[0].fMinY = g_aPlayer[0].pos.y - g_aPlayer[0].fHeight / 2;

	if(!*GetIsStop() && !*GetIsPause())
	{
		//移動する処理
		{
			//水平移動に関する
			g_aPlayer[0].bIsHit			= false;
			nextPos = D3DXVECTOR3(g_aPlayer[0].pos.x + g_aPlayer[0].move.x, g_aPlayer[0].pos.y, 0.0f);

			//坂との当たった場合
			if(IsHitBySlope(nextPos, &Intersection,  false))
			{
				g_aPlayer[0].bIsHit = true;
			}else
			{//坂と当たっていない、かつ坂の上にある場合
				if(g_aPlayer[0].bIsOnSlope)
				{
					//位置補正
					g_aPlayer[0].pos.y = Intersection.y - g_aPlayer[0].size.y / 2;
				}
			}

			//壁に対する当たり判定
			if(IsHitByWall(nextPos,false))
			{
				g_aPlayer[0].bIsHit = true;
				//方向逆転
				g_aPlayer[0].move.x *= -1.0f;
				g_aPlayer[0].bIsChangeDir = g_aPlayer[0].bIsChangeDir ? false : true;
				g_aPlayer[0].bIsGoLeft = !g_aPlayer[0].bIsGoLeft;
			}
			//移動可能な道があるか
			if(g_aPlayer[0].bIsOnfloor)
			{
				//歩く道がなければ
				if(!CheckCanWalk(g_aPlayer[0].move.x, g_aPlayer[0].bIsGoLeft))
				{
					g_aPlayer[0].bIsHit = true;
				}
			}
			if(IsHitByGimmic(nextPos, false))
			{
				g_aPlayer[0].bIsHit = true;
				//g_aPlayer[0].bIsHitByGmmic = true;
			}
			if(!g_aPlayer[0].bIsHit)
			{
				//水平移動の移動量を加算
				g_aPlayer[0].pos.x = nextPos.x;
			}

			//垂直移動処理
			nextPos = D3DXVECTOR3(g_aPlayer[0].pos.x, g_aPlayer[0].pos.y + g_aPlayer[0].move.y, 0.0f);
			//坂の上でない場合
			if(g_aPlayer[0].bIsOnSlope == false)
			{
				if(IsHitBySlope(nextPos, &Intersection,true))
				{
					g_aPlayer[0].bIsHit = true;
				}else
				{
					if(g_aPlayer[0].bIsOnSlope)
					{
						g_aPlayer[0].pos.y = Intersection.y - g_aPlayer[0].size.y / 2;
					}
				}
			}
			if(IsHitByWall(nextPos,true))
			{
				g_aPlayer[0].bIsHit = true;
			}
			if(IsHitByGimmic(nextPos,true))
			{
				g_aPlayer[0].bIsHit = true;
			}
			//坂道の上でも、オブジェクトでも当たっていない
			if(g_aPlayer[0].bIsOnSlope == false && g_aPlayer[0].bIsHit == false)
			{
				g_aPlayer[0].pos.y = nextPos.y;
			}
		}

		if(g_aPlayer[0].bIsHitByGmmic == false)
		{
			//アニメション処理
			g_aPlayer[0].nCounterAnim++;
			// 移動方向
			if(g_aPlayer[0].bIsChangeDir == false)
			{
				if( (g_aPlayer[0].nCounterAnim % TIME_CHANGE_PLAYER_MOTION) == 0 )
					g_aPlayer[0].nPatternAnim = (( g_aPlayer[0].nPatternAnim + 1 ) % (NUM_ANIM_PATTERN_X9_Y1 - 1));
				SetVertexPlayer(0, 1, 1);
				SetTexturePlayer(0, g_aPlayer[0].nPatternAnim, NUM_ANIM_PATTERN_X9_Y2, TEX_PATTERN_DIVIDE_X9, TEX_PATTERN_DIVIDE_Y2);
			}
			else
			{
				if( (g_aPlayer[0].nCounterAnim % TIME_CHANGE_PLAYER_MOTION) == 0 )
					g_aPlayer[0].nPatternAnim = (( g_aPlayer[0].nPatternAnim + 1 ) % (NUM_ANIM_PATTERN_X9_Y1 - 1));
				SetVertexPlayer(0, 1, 1);
				SetTexturePlayer(0, g_aPlayer[0].nPatternAnim + NUM_ANIM_PATTERN_X9_Y1, NUM_ANIM_PATTERN_X9_Y2, TEX_PATTERN_DIVIDE_X9, TEX_PATTERN_DIVIDE_Y2);
			}
		}
		else{
				if(g_aPlayer[0].bIsChangeDir == false)
				{
					SetVertexPlayer(0, 1, 1);
					SetTexturePlayer(0, NUM_ANIM_PATTERN_X9_Y1 - 1, NUM_ANIM_PATTERN_X9_Y2, TEX_PATTERN_DIVIDE_X9, TEX_PATTERN_DIVIDE_Y2);
				}
				else
				{
					SetVertexPlayer(0, 1, 1);
					SetTexturePlayer(0, NUM_ANIM_PATTERN_X9_Y2 - 1, NUM_ANIM_PATTERN_X9_Y2, TEX_PATTERN_DIVIDE_X9, TEX_PATTERN_DIVIDE_Y2);
				}
			}
		}
	else if(*GetIsStop() == true)
	{
		if(*GetGameMode() != SELECTMODE_PLAYEREDIT)
		{
			g_aPlayer[0].pos = g_aPlayer[0].posSave;
			g_aPlayer[0].fSpeedX = abs(g_aPlayer[0].fSpeedX);
			g_aPlayer[0].bIsChangeDir = false;
		}
		g_aPlayer[0].bIsGoLeft = false;
		g_aPlayer[0].bIsHit = false;
		g_aPlayer[0].bIsOnfloor = false;
		g_aPlayer[0].bIsOnSlope = false;
		g_aPlayer[0].bIsHitByGmmic = false;
		//g_aPlayer[0].move = D3DXVECTOR3(PLAYER_SPEED_X, PLAYER_SPEED_Y, 0.0f);
		g_aPlayer[0].move.x = abs(g_aPlayer[0].move.x);
		SetVertexPlayer(0, 1, 1);
		SetTexturePlayer(0, 0, NUM_ANIM_PATTERN_X9_Y2, TEX_PATTERN_DIVIDE_X9, TEX_PATTERN_DIVIDE_Y2);
	}

	if(IsPlayerReachGoal(0, 0, 0) == true)
	{
		// 获得当前关卡的号码
		int* nCurrentSceneNum = GetSceneNum();
		int* nLastSceneNum = GetLastSceneNum();

		if(*nCurrentSceneNum > *nLastSceneNum)
			*nLastSceneNum = *nCurrentSceneNum;
		//
		// 获得当前关卡记录 包含关卡数量及是否被攻略
		SCENE* tempScene = GetScene();

		if(!(tempScene[*nCurrentSceneNum].bIsClear))
		{
			tempScene[*nCurrentSceneNum].bIsClear = true;
			(*nLastSceneNum)++;
		}
		SetFade(FADE_OUT, FADE_IN, GAMEMODE_STAGE_SELECT);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pD3DVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(g_aPlayer[nCntPlayer].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTexturePlayer[g_aPlayer[nCntPlayer].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntPlayer * 4), NUM_POLYGON);
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if( FAILED( pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX * MAX_PLAYER,		// 頂点データ用に確保するバッファサイズ(バイト単位)
											D3DUSAGE_WRITEONLY,									// 頂点バッファの使用法　
											FVF_VERTEX_2D,										// 使用する頂点フォーマット
											D3DPOOL_MANAGED,									// リソースのバッファを保持するメモリクラスを指定
											&g_pD3DVtxBuffPlayer,								// 頂点バッファインターフェースへのポインタ
											NULL)))												// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);


		for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++, pVtx += 4)
		{
		// 頂点座標の設定
			//pVtx[0].vtx = D3DXVECTOR3(g_g_aPlayer[0][Player_MAX].pos.x - Player_SIZE_X / 2, g_g_aPlayer[0][Player_MAX].pos.y - Player_SIZE_Y / 2, 0.0f);
			//pVtx[1].vtx = D3DXVECTOR3(g_g_aPlayer[0][Player_MAX].pos.x + Player_SIZE_X / 2, g_g_aPlayer[0][Player_MAX].pos.y - Player_SIZE_Y / 2, 0.0f);
			//pVtx[2].vtx = D3DXVECTOR3(g_g_aPlayer[0][Player_MAX].pos.x - Player_SIZE_X / 2, g_g_aPlayer[0][Player_MAX].pos.y + Player_SIZE_Y / 2, 0.0f);
			//pVtx[3].vtx = D3DXVECTOR3(g_g_aPlayer[0][Player_MAX].pos.x + Player_SIZE_X / 2, g_g_aPlayer[0][Player_MAX].pos.y + Player_SIZE_Y / 2, 0.0f);

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
			g_pD3DVtxBuffPlayer->Unlock();
		}
	}

	return S_OK;
}



//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorPlayer(int nIdxPlayer, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdxPlayer * 4);

	// 頂点座標の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	g_pD3DVtxBuffPlayer->Unlock();
}

void SetVertexPlayer(	int nIdx,					//记录当前对应的图组polygon地址
						float scaleX,				//X轴的放大参数
						float scaleY)				//自身Y轴的大小参数
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(g_aPlayer[nIdx].pos.x - g_aPlayer[nIdx].fWidth / 2 * scaleX, g_aPlayer[nIdx].pos.y - g_aPlayer[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_aPlayer[nIdx].pos.x + g_aPlayer[nIdx].fWidth / 2 * scaleX, g_aPlayer[nIdx].pos.y - g_aPlayer[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_aPlayer[nIdx].pos.x - g_aPlayer[nIdx].fWidth / 2 * scaleX, g_aPlayer[nIdx].pos.y + g_aPlayer[nIdx].fHeight / 2 * scaleY, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_aPlayer[nIdx].pos.x + g_aPlayer[nIdx].fWidth / 2 * scaleX, g_aPlayer[nIdx].pos.y + g_aPlayer[nIdx].fHeight / 2 * scaleY, 0.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffPlayer->Unlock();
}


void SetTexturePlayer(	int nIdx,						// 今のテクスチャの配列番号			记录当前对应的图组地址
						int nPatternAnim,				// 今のテクスチャの番号				记录当前显示帧
						int numAnimPattern, 			// テクスチャ全部の個数				记录当前对应的组成图组的图片数量
						int texPatternDivideX,			// 横のテクスチャの個数				记录当前图组X轴图片数量
						int texPatternDivideY )			// 縦のテクスチャの個数				记录当前图组Y轴图片数量
{
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pD3DVtxBuffPlayer->Unlock();
	}
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetPlayer(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col, int type)
{
	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(!g_aPlayer[nCntPlayer].bUse)
		{
			g_aPlayer[nCntPlayer].pos = pos;
			g_aPlayer[nCntPlayer].posSave = g_aPlayer[nCntPlayer].pos;
			g_aPlayer[nCntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_aPlayer[nCntPlayer].fWidth = fWidth;
			g_aPlayer[nCntPlayer].fHeight = fHeight;
			g_aPlayer[nCntPlayer].size = D3DXVECTOR3(fWidth / 2, fHeight, 0.0f);

			g_aPlayer[nCntPlayer].fSpeedX = PLAYER_SPEED_X;
			g_aPlayer[nCntPlayer].fSpeedY = PLAYER_SPEED_Y;
			g_aPlayer[nCntPlayer].move = D3DXVECTOR3(g_aPlayer[nCntPlayer].fSpeedX, g_aPlayer[nCntPlayer].fSpeedY, 0.0f);

			g_aPlayer[nCntPlayer].fRadius = PLAYER_RADIUS;

			g_aPlayer[nCntPlayer].bIsHit = false;
			g_aPlayer[nCntPlayer].bIsChangeDir = false;
			g_aPlayer[nCntPlayer].bIsGoLeft = false;	
			g_aPlayer[nCntPlayer].bIsOnfloor= false;	
			g_aPlayer[nCntPlayer].bIsOnSlope= false;
			g_aPlayer[nCntPlayer].bIsHitByGmmic = false;
			g_aPlayer[nCntPlayer].bUse = true;
		
			g_aPlayer[nCntPlayer].nType = type;
			g_aPlayer[nCntPlayer].nCounterAnim = 0;
			g_aPlayer[nCntPlayer].nPatternAnim = 0;
			g_aPlayer[nCntPlayer].nState = 0;
			
			SetVertexPlayer(nCntPlayer, 1, 1);
			
			// 頂点カラーの設定
			SetColorPlayer(nCntPlayer, col);
			break;
		}
	}
}
// プレーヤーのデータゲット
PLAYER *GetPlayer(void)
{
	return g_aPlayer;
}
// プレーヤーと壁のあたり判定
bool IsPlayerReachGoal(int nCntPlayer, float fMoveX, float fMoveY)
{
	for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if(g_pWallDataPlayer[nCntWall].bUse && g_pWallDataPlayer[nCntWall].nType == WALL_TYPE_GOAL)
		{
			if(	g_aPlayer[nCntPlayer].fMaxX - fMoveX > g_pWallDataPlayer[nCntWall].fMinX &&
				g_aPlayer[nCntPlayer].fMinX - fMoveX < g_pWallDataPlayer[nCntWall].fMaxX &&
				g_aPlayer[nCntPlayer].fMaxY - fMoveY > g_pWallDataPlayer[nCntWall].fMinY &&
				g_aPlayer[nCntPlayer].fMinY - fMoveY < g_pWallDataPlayer[nCntWall].fMaxY)
			{
				// 纵向移动时才开启判断
				// 横移動した時
				//if(fMoveY != 0)
					//g_aPlayer[nCntPlayer].pos.y = g_pWallDataPlayer[nCntWall].pos.y - g_pWallDataPlayer[nCntWall].fHeight / 2 - g_aPlayer[nCntPlayer].fHeight / 2;
				g_pWallDataPlayer[nCntWall].bUse = false;
				return true;
			}
		}
	}
	return false;
}

bool IsMouseHitPlayer(void)
{
	if(	(g_aPlayer[0].pos.x - *GetMouX()) * (g_aPlayer[0].pos.x - *GetMouX()) +
		(g_aPlayer[0].pos.y - *GetMouY()) * (g_aPlayer[0].pos.y - *GetMouY()) <=
		(g_aPlayer[0].fRadius * 2 + 1) * (g_aPlayer[0].fRadius * 2 + 1))
	{
		float fX = 0, fY = 0;
		fX = *GetMouX() - (int)*GetMouX() % 5;
		fY = *GetMouY() - (int)*GetMouY() % 5;
		g_aPlayer[0].pos.x = fX;
		g_aPlayer[0].pos.y = fY;
		g_aPlayer[0].posSave = g_aPlayer[0].pos;
		return true;
	}
	return false;
}


// 読み込むデータを初期化
// 根据读取的预存的类型进行初始化设置。
void InitDataPlayer(void)
{
	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if(g_aPlayer[nCntPlayer].bUse)
		{
			switch (g_aPlayer[nCntPlayer].nType)
			{
			case PLAYER_001:
				g_aPlayer[nCntPlayer].posSave = g_aPlayer[nCntPlayer].pos;
				g_aPlayer[nCntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aPlayer[nCntPlayer].fRadius = PLAYER_RADIUS;

				g_aPlayer[nCntPlayer].fWidth = PLAYER_SIZE_X;
				g_aPlayer[nCntPlayer].fHeight = PLAYER_SIZE_Y;
				g_aPlayer[nCntPlayer].size = D3DXVECTOR3(PLAYER_SIZE_X / 2, PLAYER_SIZE_Y, 0.0f);

				g_aPlayer[nCntPlayer].fSpeedX = PLAYER_SPEED_X;
				g_aPlayer[nCntPlayer].fSpeedY = PLAYER_SPEED_Y;
				g_aPlayer[nCntPlayer].move.y = g_aPlayer[nCntPlayer].fSpeedY;

				g_aPlayer[nCntPlayer].bIsHit = false;
				g_aPlayer[nCntPlayer].bIsChangeDir = false;
				g_aPlayer[nCntPlayer].bIsGoLeft = false;	
				g_aPlayer[nCntPlayer].bIsOnfloor= false;	
				g_aPlayer[nCntPlayer].bIsOnSlope= false;
				g_aPlayer[nCntPlayer].bIsHitByGmmic = false;
				g_aPlayer[nCntPlayer].bUse = true;
		
				g_aPlayer[nCntPlayer].nState = 0;

				g_aPlayer[nCntPlayer].nCounterAnim = 0;
				g_aPlayer[nCntPlayer].nPatternAnim = 0;
			break;

			default:
				break;
			}
		}
	}
}

// ギミックとプレーヤーのあたり判定
bool IsPlayerHitGimmick(int nCntPlayer, float fMoveX, float fMoveY)
{
	for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
	{
		if(g_pGimPlayer[nCntGim].bUse == true)
		{
			if(g_pGimPlayer[nCntGim].nMode != MODE_ATTACK)
			{
				if(	g_aPlayer[nCntPlayer].fMaxX - fMoveX > g_pGimPlayer[nCntGim].fMinX &&
					g_aPlayer[nCntPlayer].fMinX - fMoveX < g_pGimPlayer[nCntGim].fMaxX &&
					g_aPlayer[nCntPlayer].fMaxY - fMoveY > g_pGimPlayer[nCntGim].fMinY &&
					g_aPlayer[nCntPlayer].fMinY - fMoveY < g_pGimPlayer[nCntGim].fMaxY)
				{
					// 纵向移动时才开启判断
					// 横移動した時
					if(fMoveY != 0)
						g_aPlayer[nCntPlayer].pos.y = g_pGimPlayer[nCntGim].pos.y - g_pGimPlayer[nCntGim].fHeight / 2 - g_aPlayer[nCntPlayer].fHeight / 2;
					return true;
				}
			}
		}
	}
	return false;
}

//外積を求める
float D3DXVec2Cross(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2)
{
	float cross = vec1.x * vec2.y - vec1.y * vec2.x;
	return cross;
}

//直線交差判定
//線分１、線分２、交点
bool IsHitByLine(SEGMENT segment1, SEGMENT segment2, D3DXVECTOR2* point)
{
	bool isHit = false;
	D3DXVECTOR2 vec = segment2.vec2StartPoint - segment1.vec2StartPoint;
	float CrossV1_V2 = D3DXVec2Cross(segment1.vec2Direct, segment2.vec2Direct);
	
	//平行
	if(CrossV1_V2 == 0.0f)
	{
		return false;
	}

	float CrossV_V1 = D3DXVec2Cross(vec, segment1.vec2Direct);
	float CrossV_V2 = D3DXVec2Cross(vec, segment2.vec2Direct);

	float t1 = CrossV_V2 / CrossV1_V2;
	float t2 = CrossV_V1 / CrossV1_V2;

	const float eps = 0.00001f;
	if((t1+ eps < 0.0f || t1- eps >1.0f) || (t2 + eps < 0.0f || t2 - eps>1.0f))
	{
		return false;
	}
	isHit = true;

	*point = segment1.vec2StartPoint + segment1.vec2Direct * t1;

	return isHit;
}

//四角当たり判定
bool IsHitByBox(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 sizeA, D3DXVECTOR3 sizeB)
{
	float leftA =		posA.x	-	sizeA.x / 2;
	float rightA =	posA.x	+	sizeA.x / 2;
	float topA =		posA.y	-	sizeA.y / 2;
	float bottomA =	posA.y	+	sizeA.y / 2;

	float leftB =		posB.x	-	sizeB.x / 2;
	float rightB =	posB.x	+	sizeB.x / 2;
	float topB =		posB.y	-	sizeB.y / 2;
	float bottomB =posB.y	+	sizeB.y / 2;

	if(leftA < rightB 
		&&rightA > leftB
		&&topA < bottomB
		&&bottomA > topB)
	{
		return true;
	}
	return false;
}

//坂の当たり判定
bool IsHitBySlope(D3DXVECTOR3 nextPos, D3DXVECTOR2 *Intersection, bool isFall)
{
	g_aPlayer[0].bIsHit = false;
	D3DXVECTOR3 tmpSize;
	for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if(g_pWallDataPlayer[nCntWall].bUse)
		{
			if(g_pWallDataPlayer[nCntWall].nType == WALL_TYPE_SLOPE_UP_45 || g_pWallDataPlayer[nCntWall].nType == WALL_TYPE_SLOPE_DOWN_45)
			{
				tmpSize = D3DXVECTOR3(g_pWallDataPlayer[nCntWall].fWidth, g_pWallDataPlayer[nCntWall].fHeight, 0.0f);
				if(IsHitByBox(nextPos, g_pWallDataPlayer[nCntWall].pos, g_aPlayer[0].size, tmpSize))
				{
					//坂のオブジェクトと当たった
					g_aPlayer[0].bIsHit = true;

					//移動後の線分を計算
					SEGMENT tmpSegmentPlayer;
					SEGMENT tmpSegmentSlope;
					D3DXVECTOR2 startPoint;
					D3DXVECTOR2 endPoint;

					//坂の方向を確認
					//傾きが０より大きい場合、－＿
					//傾きが０より小さい場合、＿－
					if(g_pWallDataPlayer[nCntWall].fSteep > 0)
					{
						startPoint	= D3DXVECTOR2(nextPos.x - g_aPlayer[0].fWidth /2, nextPos.y + g_aPlayer[0].fHeight / 2);
						endPoint		= D3DXVECTOR2(nextPos.x - g_aPlayer[0].fWidth /2, nextPos.y - g_aPlayer[0].fHeight / 2);
					}else if(g_pWallDataPlayer[nCntWall].fSteep < 0)
					{
						//当たり計算の辺を設定
						startPoint	= D3DXVECTOR2(nextPos.x + g_aPlayer[0].fWidth /2, nextPos.y + g_aPlayer[0].fHeight / 2);
						endPoint		= D3DXVECTOR2(nextPos.x + g_aPlayer[0].fWidth /2, nextPos.y - g_aPlayer[0].fHeight / 2);
					}

					tmpSegmentPlayer.vec2StartPoint	= startPoint;
					tmpSegmentPlayer.vec2Direct		= endPoint - startPoint;
					tmpSegmentSlope.vec2StartPoint	= g_pWallDataPlayer[nCntWall].vec2StartPoint;
					tmpSegmentSlope.vec2Direct		= g_pWallDataPlayer[nCntWall].vec2Direct;

					//水平移動量を加算
					if(IsHitByLine(tmpSegmentPlayer, tmpSegmentSlope, Intersection))
					{
						//坂の上にある
						g_aPlayer[0].bIsOnSlope	= true;
						g_aPlayer[0].bIsOnfloor	= false;
						g_aPlayer[0].bIsHit			= false;
						return g_aPlayer[0].bIsHit;
						//break;
					}
					//水平移動量を追加しても坂道に当たらなかった場合
					//垂直へ検索する
					else
					{
						//水平移動の場合にだけ行う
						//坂道についていないけど、降りれば付けるかをチェック
						if(!isFall)
						{
							D3DXVECTOR3 nextPosYPlus = nextPos;
							//検索上限、坂道の底辺
							float LimitMaxY = g_pWallDataPlayer[nCntWall].pos.y + g_pWallDataPlayer[nCntWall].fHeight / 2;
							while(nextPos.y< LimitMaxY)
							{
								if(g_pWallDataPlayer[nCntWall].fSteep > 0)
								{
									startPoint	= D3DXVECTOR2(nextPos.x - g_aPlayer[0].fWidth /2, nextPos.y + g_aPlayer[0].fHeight / 2);
									endPoint		= D3DXVECTOR2(nextPos.x - g_aPlayer[0].fWidth /2, nextPos.y - g_aPlayer[0].fHeight / 2);
								}else if(g_pWallDataPlayer[nCntWall].fSteep < 0)
								{
									//当たり計算の辺を設定
									startPoint	= D3DXVECTOR2(nextPos.x + g_aPlayer[0].fWidth /2, nextPos.y + g_aPlayer[0].fHeight / 2);
									endPoint		= D3DXVECTOR2(nextPos.x + g_aPlayer[0].fWidth /2, nextPos.y - g_aPlayer[0].fHeight / 2);
								}

								tmpSegmentPlayer.vec2StartPoint	= startPoint;
								tmpSegmentPlayer.vec2Direct		= endPoint - startPoint;
								tmpSegmentSlope.vec2StartPoint	= g_pWallDataPlayer[nCntWall].vec2StartPoint;
								tmpSegmentSlope.vec2Direct		= g_pWallDataPlayer[nCntWall].vec2Direct;
								
								//垂直移動量を加算
								if(IsHitByLine(tmpSegmentPlayer, tmpSegmentSlope, Intersection))
								{
									//坂の上にある
									g_aPlayer[0].bIsOnSlope = false;
									g_aPlayer[0].bIsOnfloor = false;
									g_aPlayer[0].bIsHit	= false;
									return g_aPlayer[0].bIsHit;
									//break;
								}
								nextPos.y += g_aPlayer[0].fSpeedY;
							}
								//坂道の上にある状態である場合
								//プレイヤーの上辺が坂道ブロックの上辺より小さいとき
							if( (g_aPlayer[0].bIsOnSlope == true) && (g_aPlayer[0].pos.y - g_aPlayer[0].size.y /2) < (g_pWallDataPlayer[nCntWall].pos.y - g_pWallDataPlayer[nCntWall].fHeight / 2))
							{
									g_aPlayer[0].pos.y		= g_pWallDataPlayer[nCntWall].pos.y - g_pWallDataPlayer[nCntWall].fHeight / 2 - g_aPlayer[0].size.y / 2;
									//下向きの延長線は坂道に当たっていない
									g_aPlayer[0].bIsOnfloor = true;
									g_aPlayer[0].bIsOnSlope = false;
									g_aPlayer[0].bIsHit	= false;
									return g_aPlayer[0].bIsHit;
							}else
							{
								g_aPlayer[0].bIsHit	= false;
								g_aPlayer[0].bIsOnSlope = false;
								return g_aPlayer[0].bIsHit;
							}
						}
						//下方向移動
						//ブロックに当たったが、坂道に当たっていない場合は、ブロックの上にある
						else
						{
							{
								D3DXVECTOR3 nextPosYPlus = nextPos;
								//検索上限、坂道の底辺
								float LimitMaxY = g_pWallDataPlayer[nCntWall].pos.y + g_pWallDataPlayer[nCntWall].fHeight / 2;
								while(nextPos.y< LimitMaxY)
								{
									if(g_pWallDataPlayer[nCntWall].fSteep > 0)
									{
										startPoint	= D3DXVECTOR2(nextPos.x - g_aPlayer[0].fWidth /2, nextPos.y + g_aPlayer[0].fHeight / 2);
										endPoint		= D3DXVECTOR2(nextPos.x - g_aPlayer[0].fWidth /2, nextPos.y - g_aPlayer[0].fHeight / 2);
									}else if(g_pWallDataPlayer[nCntWall].fSteep < 0)
									{
										//当たり計算の辺を設定
										startPoint	= D3DXVECTOR2(nextPos.x + g_aPlayer[0].fWidth /2, nextPos.y + g_aPlayer[0].fHeight / 2);
										endPoint		= D3DXVECTOR2(nextPos.x + g_aPlayer[0].fWidth /2, nextPos.y - g_aPlayer[0].fHeight / 2);
									}

									tmpSegmentPlayer.vec2StartPoint	= startPoint;
									tmpSegmentPlayer.vec2Direct		= endPoint - startPoint;
									tmpSegmentSlope.vec2StartPoint	= g_pWallDataPlayer[nCntWall].vec2StartPoint;
									tmpSegmentSlope.vec2Direct		= g_pWallDataPlayer[nCntWall].vec2Direct;
									
									//垂直移動量を加算
									if(IsHitByLine(tmpSegmentPlayer, tmpSegmentSlope, Intersection))
									{
										//坂の上にある
										g_aPlayer[0].bIsOnSlope = false;
										g_aPlayer[0].bIsOnfloor = false;
										g_aPlayer[0].bIsHit	= false;
										return g_aPlayer[0].bIsHit;
										//break;
									}
									nextPos.y += g_aPlayer[0].fSpeedY;
								}
							}
							g_aPlayer[0].bIsOnfloor = true;
							g_aPlayer[0].bIsHit = true;
							return g_aPlayer[0].bIsHit;
						}
					}
				}
			}
		}
	}
	g_aPlayer[0].bIsOnSlope = false;
	g_aPlayer[0].bIsHit = false;
	return g_aPlayer[0].bIsHit;
}

//壁との当たり判定
bool IsHitByWall(D3DXVECTOR3 nextPos, bool isFall)
{
	D3DXVECTOR3 tmpSize;
	for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if(g_pWallDataPlayer[nCntWall].bUse)
		{
			if(g_pWallDataPlayer[nCntWall].nType == WALL_TYPE_001|| g_pWallDataPlayer[nCntWall].nType == WALL_TYPE_002)
			{
				tmpSize = D3DXVECTOR3(g_pWallDataPlayer[nCntWall].fWidth, g_pWallDataPlayer[nCntWall].fHeight, 0.0f);
				if(IsHitByBox(nextPos, g_pWallDataPlayer[nCntWall].pos, g_aPlayer[0].size, tmpSize))
				{
					//もし縦移動でなければい、Y軸位置補正
					if(isFall)
					{	//プレイヤーの位置を計算　pos.y = 壁の位置 - 壁の高さ　/　２　- プレイヤーの高さ / 2
						g_aPlayer[0].pos.y = g_pWallDataPlayer[nCntWall].pos.y - g_pWallDataPlayer[nCntWall].fHeight / 2 - g_aPlayer[0].size.y / 2;
						g_aPlayer[0].bIsOnfloor	= true;
					}
					return true;
				}
			}
		}
	}
	return false;
}

//ギミックとの当たり判定
bool IsHitByGimmic(D3DXVECTOR3 nextPos, bool isFall)
{
	D3DXVECTOR3 tmpSize;
	for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
	{
		if(g_pGimPlayer[nCntGim].bUse)
		{
			if(g_pGimPlayer[nCntGim].nMode != MODE_ATTACK)
			{
				tmpSize = D3DXVECTOR3(g_pGimPlayer[nCntGim].fWidth, g_pGimPlayer[nCntGim].fHeight, 0.0f);
				if(IsHitByBox(nextPos, g_pGimPlayer[nCntGim].pos, g_aPlayer[0].size, tmpSize))
				{
					//もし縦移動でなければい、Y軸位置補正
					if(isFall)
					{	//プレイヤーの位置を計算　pos.y = 壁の位置 - 壁の高さ　/　２　- プレイヤーの高さ / 2
						g_aPlayer[0].pos.y = g_pGimPlayer[nCntGim].pos.y - g_pGimPlayer[nCntGim].fHeight / 2 - g_aPlayer[0].fHeight / 2;
						g_aPlayer[0].bIsOnfloor	= true;
					}
					return true;
				}
			}
		}
	}
	return false;
}
bool CheckCanWalk(float speed, bool isGoLeft)
{
	bool canWalk = false;
	//当たり判定のボックスを作成
	float nextPosX;
	if(isGoLeft)
	{
		nextPosX = g_aPlayer[0].pos.x - g_aPlayer[0].size.x / 2 - speed;
	}else
	{
		nextPosX = g_aPlayer[0].pos.x + g_aPlayer[0].size.x / 2 + speed;
	}
	//プレイヤの足を踏んでいるボックス
	//	               人
	//	(判定用) ■   ■(判定用) 
	float nextPosTopY			= g_aPlayer[0].pos.y + g_aPlayer[0].size.y /2;
	float nextPosBottomY	= g_aPlayer[0].pos.y + (g_aPlayer[0].size.y /2 + g_aPlayer[0].size.y) ;		//プレイヤと同じ高さのボックスを作成

	//壁に対して
	for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if(g_pWallDataPlayer[nCntWall].bUse)
		{	
			if(g_pWallDataPlayer[nCntWall].nType == WALL_TYPE_001|| g_pWallDataPlayer[nCntWall].nType == WALL_TYPE_002)
			{
				if(nextPosX >=	g_pWallDataPlayer[nCntWall].fMinX &&
					nextPosX <=	g_pWallDataPlayer[nCntWall].fMaxX &&
					nextPosTopY == g_pWallDataPlayer[nCntWall].fMinY)				//ボックスの頂点と壁の上平面が同じ場合
					//nextPosBottomY <=	g_pWallDataPlayer[nCntWall].fMaxY)			//ボックスの底点と壁のした平面が同じ場合
				{
					canWalk = true;
					return canWalk;
				}
			}
		}
	}

		for(int nCntGim = 0; nCntGim < MAX_GIMMICK; nCntGim++)
		{
			if(g_pGimPlayer[nCntGim].bUse)
			{
				if(g_pGimPlayer[nCntGim].nMode != MODE_ATTACK)
				{
					if(nextPosX >=	g_pGimPlayer[nCntGim].fMinX &&
					nextPosX <=	g_pGimPlayer[nCntGim].fMaxX &&
					nextPosTopY == g_pGimPlayer[nCntGim].fMinY)				//ボックスの頂点と壁の上平面が同じ場合
					//nextPosBottomY <=	g_pWallDataPlayer[nCntWall].fMaxY)			//ボックスの底点と壁のした平面が同じ場合
					{
						canWalk = true;
						return canWalk;
					}
				}
			}
		}

	//坂の上にない場合、道があるはずのため、必ず歩ける
	if(g_aPlayer[0].bIsOnSlope != true)
	{
		for(int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
		{
			if(g_pWallDataPlayer[nCntWall].bUse)
			{
				//坂道に対する当たり判定
				if(g_pWallDataPlayer[nCntWall].nType == WALL_TYPE_SLOPE_UP_45 || g_pWallDataPlayer[nCntWall].nType == WALL_TYPE_SLOPE_DOWN_45)
				{
					if(nextPosX >=	g_pWallDataPlayer[nCntWall].fMinX &&
						nextPosX <=	g_pWallDataPlayer[nCntWall].fMaxX &&
						nextPosTopY == g_pWallDataPlayer[nCntWall].fMinY &&				//ボックスの頂点と壁の上平面が同じ場合
						nextPosBottomY <=	g_pWallDataPlayer[nCntWall].fMaxY)			//ボックスの底点と壁のした平面が同じ場合
					{
						canWalk = true;
						return canWalk;
					}
				}
			}
		}
	}
	return canWalk;
}
// プレーヤー編集
void PlayerEdit(void)
{
	PrintDebugProc("\n\n");
	PrintDebugProc("**************************************************\n");
	PrintDebugProc("【メニュー】 【属性】    【数値】\n");
	PrintDebugProc("   【1】   【すばやさX】  : %f\n", g_aPlayer[0].move.x + 5);
	PrintDebugProc("**************************************************\n");
	PrintDebugProc("\n\n");

	PrintDebugProc("矢印キー【左】、【右】   【+- 0.1】\n");
	PrintDebugProc("矢印キー【上】、【下】   【+- 0.5】\n");
	PrintDebugProc("   【1】   【開始時間】  : %f\n\n");
	if(GetKeyboardRepeat(DIK_LEFT))
		g_aPlayer[0].move.x -= 0.1f;
	if(GetKeyboardRepeat(DIK_RIGHT))
		g_aPlayer[0].move.x += 0.1f;
	if(GetKeyboardRepeat(DIK_DOWN))
		g_aPlayer[0].move.x -= 0.5f;
	if(GetKeyboardRepeat(DIK_UP))
		g_aPlayer[0].move.x += 0.5f;
}