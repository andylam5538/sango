/*****************************************************************************
 *
 *三校合同コンテスト
 *
 *作る時間 ： 2015.5.1
 *
 *GP11A 248 朱鷹仁 40582
 *
 ******************************************************************************/
//=============================================================================
//
// メイン処理 [main.cpp]
//
//=============================================================================
#include "main.h"
#include "botton.h"
#include "debugproc.h"
#include "gameelement.h"
#include "gameManager.h"
#include "gimmick.h"
#include "input.h"
#include "item.h"
#include "number.h"
#include "playbar.h"
#include "player.h"
#include "timeCounter.h"
#include "timeTrack.h"
#include "wall.h"
#include "slider.h"
#include "effect.h"
#include "mouseLocal.h"
#include "background.h"
#include "fade.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"					// ウインドウのクラス名
#define WINDOW_NAME		"三校合同コンテスト"		// ウインドウのキャプション名

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(HWND hWnd);
void Draw(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;						// Direct3D オブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;				// Deviceオブジェクト(描画に必要)
float				g_fMouseX=0, g_fMouseY=0;			// マウス設定	(鼠标设定)
HDC					g_hdc=NULL,g_mdc=NULL,g_bufdc=NULL;	// 全局设备环境句柄与全局内存DC句柄
GAMEMODE			g_mode = GAMEMODE_TITLE;			// モード 默认启动画面 MODE结构体中设置了启动画面和游戏画面的选择
SCENE				g_scene[SCENE_TYPE_MAX];			// ステージ

int					g_nSceneNum = 0;					// シーンの番号
int					g_nLastSceneNum = 0;				// 今まで一番最後のステージ
#ifdef _DEBUG
int					g_nCountFPS;			// FPSカウンタ
#endif

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND hWnd;
	MSG msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
						CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,	// WS_VISIBLE | WS_POPUP(FULL SCREEM)  WS_OVERLAPPEDWINDOW(WINDOWS)
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,
						SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),
						NULL,
						NULL,
						hInstance,
						NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	// 窗口SIZE设置
	if(FAILED(Init(hInstance, hWnd, true)))
	{
		return -1;
	}

	//フレームカウント初期化
	timeBeginPeriod(1);				// 分解能を設定
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
			{
#ifdef _DEBUG
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
#ifdef _DEBUG
				// FPS表示
				PrintDebugProc("FPS:%d\n", g_nCountFPS);
				// マウスポジション表示
				PrintDebugProc("◆◆◆！！！ウィンドウのサイズを固定しなさい！！！◆◆◆\n");
				PrintDebugProc("◆◆◆ゲームモードに時間軸を右クリックして、時間軸を変換できる(0~5=>5~0)◆◆◆\n");
				PrintDebugProc("MouseX:%f\n", g_fMouseX);
				PrintDebugProc("MouseY:%f\n", g_fMouseY);
				PrintDebugProc("◆F1◆ゲームモード   ◆F2◆壁を編集するモード   ◆F3◆ギミック編集モード   ◆F4◆プレイヤー編集モード\n");
				PrintDebugProc("◆F5◆削除モード     ◆F6◆ステージ切り替える   ◆F7◆時間軸編集モード\n");
#endif
				dwExecLastTime = dwCurrentTime;
				// 更新処理
				Update(hWnd);
				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}
	
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();

	timeEndPeriod(1);				// 分解能を戻す

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//定义一个PAINTSTRUCT结构体来记录一些绘制信息
	//PAINTSTRUCT ps;

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:						// [ESC]キーが押された
			int nID;
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if(nID == IDYES)
				DestroyWindow(hWnd);		// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	case WM_MOUSEMOVE:						//鼠标移动消息
		g_fMouseX = LOWORD(lParam);			//取得鼠标X坐标
		//if(g_fMouseX > SCREEN_WIDTH)		//设置临界坐标
		//	g_fMouseX = SCREEN_WIDTH;
		//else if(g_fMouseX < 0)
		//		g_fMouseX = 0;

		g_fMouseY = HIWORD(lParam);			//取得鼠标Y坐标
		//if(g_fMouseY > SCREEN_HEIGHT)
		//	g_fMouseY = SCREEN_HEIGHT;
		//else if(g_fMouseY < 0)
		//	g_fMouseY = 0;
		break;

	case WM_SIZE:
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory( &d3dpp, sizeof(d3dpp) );						// ワークをゼロクリア
	d3dpp.BackBufferCount			= 1;						// バックバッファの数
	d3dpp.BackBufferWidth			= SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			= SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			= d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	// 映像信号に同期してフリップする
	d3dpp.Windowed					= bWindow;					// ウィンドウモード
	d3dpp.EnableAutoDepthStencil	= TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				// デプスバッファとして16bitを使う

	if(bWindow)
	{// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{// フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
									D3DDEVTYPE_HAL, 
									hWnd, 
									D3DCREATE_HARDWARE_VERTEXPROCESSING, 
									&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
										D3DDEVTYPE_HAL, 
										hWnd, 
										D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
										&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
											D3DDEVTYPE_REF,
											hWnd, 
											D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
											&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダリングステートパラメータの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートパラメータの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージ加算合成処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	// マウスの場所の初期化設定（鼠标初始位置设定）
	g_fMouseX = 300;
	g_fMouseY = 100;

	POINT pt,lt,rb;
	RECT rect;
	//设定光标位置
	pt.x = 300;
	pt.y = 100;
	//ClientToScreen(hWnd,&pt);		// 把坐标从当前窗体转化成全屏幕

	// 把屏幕坐标转化成相对当前窗体的坐标
	ScreenToClient(	hWnd,			// 用户区域用于转换的窗口句柄
					&pt);			// 指向一个含有要转换的用户坐标的结构的指针，如果函数调用成功，新屏幕坐标复制到此结构。
									// 返回值：如果函数调用成功，返回值为非零值，否则为零。
	SetCursorPos(pt.x,pt.y);
	
	ShowCursor(false);				// 隐藏鼠标光标

	//限制鼠标光标移动区域
	GetClientRect(hWnd,&rect);		// 取得窗口内部矩形 GetWindowRect 取得屏幕内部矩形
	//将矩形左上点坐标存入lt中
	lt.x = rect.left;
	lt.y = rect.top;
	//将矩形右下坐标存入rb中
	rb.x = rect.right;
	rb.y = rect.bottom;
	//将lt和rb的窗口坐标转换为屏幕坐标
	ClientToScreen(hWnd,&lt);
	ClientToScreen(hWnd,&rb);
	//以屏幕坐标重新设定矩形区域
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	//限制鼠标光标移动区域
	ClipCursor(&rect);
	
	for(int nCntScene = 0; nCntScene < SCENE_TYPE_MAX; nCntScene++)
	{
		g_scene[nCntScene].nSceneNum = nCntScene;
		g_scene[nCntScene].bIsClear = false;
	}

	// 時間計算の初期化
	InitTimeCounter();

	// デバッグ表示処理の初期化
	InitDebugProc();

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	// の初期化
	InitGameElement();

	// 時間軸の初期化
	InitPlaybar();

	// 壁の初期化
	InitWall();

	// ギミックの初期化
	InitGimmick();

	// プレーヤーの初期化
	InitPlayer();

	// ボタンの初期化
	InitBotton();

	// 時間軌跡の初期化
	InitTimeTrack();

	// ゲームマネジメントの初期化
	InitGameManager();

	// 数字の初期化
	InitNumber();

	// アイテムの初期化
	InitItem();

	// スライドの初期化
	InitSlider();

	//InitBarEffect();

	// マウスの初期化
	InitMouseLocal();

	// 背景の初期化
	InitBackground();

	// フェードの初期化
	InitFade();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	if(g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}
	// デバッグ表示処理の終了処理
	UninitDebugProc();

	// 入力処理の終了処理
	UninitInput();

	// の終了処理
	UninitGameElement();

	// 時間軸の終了処理
	UninitPlaybar();

	// 壁の終了処理
	UninitWall();

	// ギミックの終了処理
	UninitGimmick();

	// プレーヤーの終了処理
	UninitPlayer();

	// ボタンの終了処理
	UninitBotton();

	// 時間軌跡の終了処理
	UninitTimeTrack();

	// ゲームマネジメントの終了処理
	UninitGameManager();

	// 数字の終了処理
	UninitNumber();

	// アイテムの終了処理
	UninitItem();

	// スライドの終了処理
	UninitSlider();

	// マウスの終了処理
	UninitMouseLocal();

	// 背景の終了処理
	UninitBackground();

	// フェードの終了処理
	UninitFade();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(HWND hWnd)
{
	// 入力更新
	UpdateInput();
	// フェードの更新处理
	UpdateFade();
	// 背景の更新处理
	UpdateBackground();
	// ゲームマネジメントの更新处理
	UpdateGameManager();
	// 更新处理
	UpdateGameElement();
	// マウスの更新处理
	UpdateMouseLocal();

	switch(g_mode)
	{
	case GAMEMODE_LOGO:
		//InitLoading();

		//DrawLoading();
		//shadowの初期化
		//InitLogo();


		//UninitLoading();
		break;

	case GAMEMODE_TITLE:

		break;
	case GAMEMODE_STAGE_SELECT:

		break;

	case GAMEMODE_GAME:
	{
		// 時間計算更新处理
		UpdateTimeCounter();

		// 壁更新处理
		UpdateWall();

		// プレーヤー更新处理
		UpdatePlayer();

		// ギミック軸更新处理
		UpdateGimmick();

		// ボタン更新处理
		UpdateBotton();

		// 時間軌跡の更新处理
		UpdateTimeTrack();

		// 時間軸更新处理 此处更新不能早于时间线轨迹的更新
		UpdatePlaybar();

		// 数字の更新处理
		UpdateNumber();

		// アイテムの更新处理
		UpdateItem();

		// スライドの更新处理
		UpdateSlider();

		//UpdateBarEffect();
		break;
	}
	case GAMEMODE_END:

		break;

	default:
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// 背景の描画
		DrawBackground();
		// ゲームマネジメントの描画
		DrawGameManager();

		switch(g_mode)
		{
		case GAMEMODE_LOGO:
			//InitLoading();

			//DrawLoading();
			//shadowの初期化
			//InitLogo();


			//UninitLoading();
			break;

		case GAMEMODE_TITLE:
			break;
		case GAMEMODE_STAGE_SELECT:

			break;

		case GAMEMODE_GAME:
		{
			// 時間軸描画
			DrawPlaybar();

			// 壁描画
			DrawWall();

			// ギミック描画
			DrawGimmick();

			// プレーヤー描画
			DrawPlayer();

			// ボタン描画
			DrawBotton();

			// 時間軌跡の描画
			DrawTimeTrack();

			// 数字の描画
			DrawNumber();

			// アイテムの描画
			DrawItem();

			// スライドの描画
			DrawSlider();


			DrawBarEffect();
		
			break;
		}
		case GAMEMODE_END:

			break;
		default:
			break;
		}

		// 描画
		DrawGameElement();
		// マウスの描画
		DrawMouseLocal();
		// フェードの描画
		DrawFade();


#ifdef _DEBUG
		// デバッグ表示
		DrawDebugProc();
#endif	
		// Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

float *GetMouX(void)
{
	return &g_fMouseX;
}

float *GetMouY(void)
{
	return &g_fMouseY;
}


//-----------------------------------【GamePaint( )函数】--------------------------------------
//	描述：绘制函数，在此函数中进行绘制操作
//--------------------------------------------------------------------------------------------------
void GamePaint( HWND hWnd )
{
	HPEN pen, oldPen;							// PAINTの個数
	int centerX, centerY;
	RECT rect;

	GetClientRect(hWnd, &rect);
	centerX = rect.right / 2;
	centerY = rect.bottom / 2;

	// 座標軸の描画
	pen = CreatePen(PS_SOLID,					// 类型
					3,							// 线宽
					RGB(255, 0, 0));			// 颜色

	oldPen = (HPEN)SelectObject(g_hdc,			// 设备环境句柄
								pen);			// 被选用对象的句柄

	//“光标”移动到对应的（centerX，0）坐标处
	MoveToEx(	g_hdc,							// 窗体句柄
				centerX,						// X轴坐标
				0,								// Y轴坐标
				NULL);							// 是否存放上一个点

	//从（centerX，0）坐标处向（centerX，rect.bottom）绘制线段
	LineTo(g_hdc, centerX, rect.bottom);

	MoveToEx(g_hdc, 0, centerY, NULL);
	LineTo(g_hdc, rect.right, centerY);
	SelectObject(g_hdc, oldPen);
	DeleteObject(pen);							// 成功return true　失敗return false
}

//=============================================================================
// モード設定
//=============================================================================
void SetMode(GAMEMODE mode)
{
	g_mode = mode;

	// の初期化
	UninitGameElement();

	// ボタンの終了処理
	UninitBotton();

	// 時間軌跡の終了処理
	UninitTimeTrack();

	// 数字の終了処理
	UninitNumber();

	// アイテムの終了処理
	UninitItem();

	// スライドの終了処理
	UninitSlider();

	switch(g_mode)
	{
	case GAMEMODE_LOGO:
		//InitLoading();

		//DrawLoading();
		//shadowの初期化
		//InitLogo();
		//UninitLoading();
		break;

	case GAMEMODE_TITLE:
		// の初期化
		InitGameElement();
		break;

	case GAMEMODE_STAGE_SELECT:
		// 時間軸の初期化
		InitPlaybar();

		// 時間計算の初期化
		InitTimeCounter();

		// ボタンの初期化
		InitBotton();

		// 時間軌跡の初期化
		InitTimeTrack();

		// 数字の初期化
		InitNumber();

		// アイテムの初期化
		InitItem();

		// スライドの初期化
		InitSlider();

		// マウスの初期化
		InitMouseLocal();

		// の初期化
		InitGameElement();
		break;

	case GAMEMODE_GAME:
		// 時間計算の初期化
		InitTimeCounter();

		// の初期化
		InitGameElement();

		// ボタンの初期化
		InitBotton();

		// 時間軌跡の初期化
		InitTimeTrack();

		// 数字の初期化
		InitNumber();

		// アイテムの初期化
		InitItem();

		// スライドの初期化
		InitSlider();

		// マウスの初期化
		InitMouseLocal();
		break;

	case GAMEMODE_END:

		break;

	default:
		break;
	}
}

//=============================================================================
// モード取得
//=============================================================================
GAMEMODE *GetMode(void)
{
	return &g_mode;
}
//=============================================================================
// ステージ取得
//=============================================================================
SCENE *GetScene(void)
{
	return g_scene;
}

// スデージの番号をゲット
int *GetSceneNum(void)
{
	return &g_nSceneNum;
}
// 今まで一番最後のステージをゲット
int *GetLastSceneNum(void)
{
	return &g_nLastSceneNum;
}