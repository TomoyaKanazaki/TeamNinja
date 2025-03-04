//============================================================
//
//	レンダラー処理 [renderer.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "renderState.h"
#include "camera.h"
#include "texture.h"
#include "object2D.h"
#include "debug.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DFORMAT	FORMAT_DEPTH_STENCIL = D3DFMT_D24S8;		// 深度ステンシルのフォーマット (深度バッファ：24bit, ステンシルバッファ：8bit使用)
	const D3DCOLOR	COL_CLEAR = D3DCOLOR_RGBA(0, 0, 0, 255);	// 画面クリア時の色

	const DWORD FLAG_CLEAR = (D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER);	// クリアするバッファーのビットフラグ
}

//************************************************************
//	親クラス [CRenderer] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CRenderer::CRenderer() :
	m_pD3D				(nullptr),	// Direct3Dオブジェクト
	m_pD3DDevice		(nullptr),	// Direct3Dデバイス
	m_pDrawScreen		(nullptr),	// 画面描画用の2Dポリゴン
	m_nRenderTextureID	(0),		// レンダーテクスチャのインデックス
	m_pRenderTextureSurface		(nullptr),	// 描画サーフェイスへのポインタ
	m_pDepthStencilSurface		(nullptr),	// Zバッファ・ステンシルバッファのサーフェイスへのポインタ
	m_pDefRenderTextureSurface	(nullptr),	// 元の描画サーフェイス保存用
	m_pDefDepthStencilSurface	(nullptr)	// 元のZバッファ・ステンシルバッファのサーフェイス保存用
{

}

//============================================================
//	デストラクタ
//============================================================
CRenderer::~CRenderer()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	// 変数を宣言
	D3DDISPLAYMODE			d3ddm;	// ディスプレイモード
	D3DPRESENT_PARAMETERS	d3dpp;	// プレゼンテーションパラメータ

	// メンバ変数を初期化
	m_pD3D				= nullptr;	// Direct3Dオブジェクト
	m_pD3DDevice		= nullptr;	// Direct3Dデバイス
	m_pDrawScreen		= nullptr;	// 画面描画用の2Dポリゴン
	m_nRenderTextureID	= NONE_IDX;	// レンダーテクスチャのインデックス

	m_pRenderTextureSurface		= nullptr;	// 描画サーフェイスへのポインタ
	m_pDepthStencilSurface		= nullptr;	// Zバッファ・ステンシルバッファのサーフェイスへのポインタ
	m_pDefRenderTextureSurface	= nullptr;	// 元の描画サーフェイス保存用
	m_pDefDepthStencilSurface	= nullptr;	// 元のZバッファ・ステンシルバッファのサーフェイス保存用

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{ // オブジェクトの生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{ // ディスプレイモードの取得に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// パラメータのゼロクリア
	d3dpp.BackBufferWidth	= SCREEN_WIDTH;		// ゲーム画面サイズ (幅)
	d3dpp.BackBufferHeight	= SCREEN_HEIGHT;	// ゲーム画面サイズ (高さ)
	d3dpp.BackBufferFormat	= d3ddm.Format;		// バックバッファの形式
	d3dpp.BackBufferCount	= 1;				// バックバッファの数

	d3dpp.SwapEffect		= D3DSWAPEFFECT_DISCARD;		// ダブルバッファの切り替え (映像信号に同期)
	d3dpp.EnableAutoDepthStencil	= TRUE;					// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	= FORMAT_DEPTH_STENCIL;	// 深度バッファ：24bit, ステンシルバッファ：8bitを使用
	d3dpp.Windowed					= bWindow;				// ウインドウモード

	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成
	if (FAILED(CreateDevice(hWnd, d3dpp)))
	{ // デバイスの生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// レンダーステート情報の初期化
	CRenderState::BindDevice(&m_pD3DDevice);	// デバイスを割当
	CRenderState::InitRenderState();			// 情報の初期化

	// サンプラーステートの設定 (テクスチャの拡縮補間の設定)
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定 (テクスチャのアルファブレンドの設定)
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,	D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2,	D3DTA_CURRENT);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,	D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2,	D3DTA_CURRENT);

	m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP,	D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2,	D3DTA_CURRENT);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2,	D3DTA_CURRENT);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CRenderer::Uninit(void)
{
	// 画面描画用の2Dポリゴンの終了
	SAFE_UNINIT(m_pDrawScreen);

	// 描画サーフェイスの破棄
	SAFE_RELEASE(m_pRenderTextureSurface);

	// Zバッファ・ステンシルバッファのサーフェイスの破棄
	SAFE_RELEASE(m_pDepthStencilSurface);

	// 元の描画サーフェイスの破棄
	SAFE_RELEASE(m_pDefRenderTextureSurface);

	// 元のZバッファ・ステンシルバッファのサーフェイス破棄
	SAFE_RELEASE(m_pDefDepthStencilSurface);

	// Direct3Dデバイスの破棄
	SAFE_RELEASE(m_pD3DDevice);

	// Direct3Dオブジェクトの破棄
	SAFE_RELEASE(m_pD3D);
}

//============================================================
//	更新処理
//============================================================
void CRenderer::Update(void)
{
	// オブジェクトの全更新
	CObject::UpdateAll();
}

//============================================================
//	描画処理
//============================================================
void CRenderer::Draw(void)
{
	// 変数を宣言
	HRESULT			hr;				// 異常終了の確認用
	D3DVIEWPORT9	viewportDef;	// カメラのビューポート保存用

	//--------------------------------------------------------
	//	テクスチャ作成用の描画
	//--------------------------------------------------------
	// 塗りつぶしモードの設定
	GET_MANAGER->GetDebug()->SetFillMode();

	// 描画サーフェイスを作成したものに変更
	hr = m_pD3DDevice->SetRenderTarget(0, m_pRenderTextureSurface);
	assert(SUCCEEDED(hr));

	// Zバッファ・ステンシルバッファのサーフェイスを作成したものに変更
	hr = m_pD3DDevice->SetDepthStencilSurface(m_pDepthStencilSurface);
	assert(SUCCEEDED(hr));

	// バックバッファとZバッファのクリア
	hr = m_pD3DDevice->Clear(0, nullptr, FLAG_CLEAR, COL_CLEAR, 1.0f, 0);
	assert(SUCCEEDED(hr));

	// テクスチャ作成用の描画
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{ // 描画開始が成功した場合

		// 現在のビューポートを取得
		m_pD3DDevice->GetViewport(&viewportDef);

		// カメラの設定
		GET_MANAGER->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

		// オブジェクトの全描画
		CObject::DrawAll();

		// ビューポートを元に戻す
		m_pD3DDevice->SetViewport(&viewportDef);

		// 描画終了
		hr = m_pD3DDevice->EndScene();
		assert(SUCCEEDED(hr));
	}

	//--------------------------------------------------------
	//	画面用の描画
	//--------------------------------------------------------
	// 塗りつぶしモードを設定
	GET_DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// ポリゴンを塗りつぶす

	// 描画サーフェイスを元に戻す
	hr = m_pD3DDevice->SetRenderTarget(0, m_pDefRenderTextureSurface);
	assert(SUCCEEDED(hr));

	// Zバッファ・ステンシルバッファのサーフェイスを元に戻す
	hr = m_pD3DDevice->SetDepthStencilSurface(m_pDefDepthStencilSurface);
	assert(SUCCEEDED(hr));

	// バックバッファとZバッファのクリア
	hr = m_pD3DDevice->Clear(0, nullptr, FLAG_CLEAR, COL_CLEAR, 1.0f, 0);
	assert(SUCCEEDED(hr));

	// 画面の描画
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{ // 描画開始が成功した場合

		// 現在のビューポートを取得
		m_pD3DDevice->GetViewport(&viewportDef);

		// カメラの設定
		GET_MANAGER->GetCamera()->SetCamera(CCamera::TYPE_MAIN);

		// サンプラーステートを設定
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP); // U方向のラッピングを無効化
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP); // V方向のラッピングを無効化
		
		// 画面描画用の2Dポリゴンの描画
		m_pDrawScreen->Draw();

		// デバッグ表示の描画
		GET_MANAGER->GetDebugProc()->Draw();

		// サンプラーステートを設定
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// U方向のラッピングを有効化
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// V方向のラッピングを有効化

		// ビューポートを元に戻す
		m_pD3DDevice->SetViewport(&viewportDef);

		// 描画終了
		hr = m_pD3DDevice->EndScene();
		assert(SUCCEEDED(hr));
	}

	// バックバッファとフロントバッファの入れ替え
	hr = m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
	assert(SUCCEEDED(hr));
}

//============================================================
//	レンダーテクスチャー生成処理
//============================================================
HRESULT CRenderer::CreateRenderTexture(void)
{
	// 変数を宣言
	HRESULT hr;	// 異常終了の確認用

	// ポインタを宣言
	CTexture *pTexture = GET_MANAGER->GetTexture();	// テクスチャへのポインタ
	assert(pTexture != nullptr);

	// 空のテクスチャを生成
	m_nRenderTextureID = pTexture->Regist(CTexture::SInfo
	( // 引数
		SCREEN_WIDTH,			// テクスチャ横幅
		SCREEN_HEIGHT,			// テクスチャ縦幅
		0,						// ミップマップレベル
		D3DUSAGE_RENDERTARGET,	// 性質・確保オプション
		D3DFMT_X8R8G8B8,		// ピクセルフォーマット
		D3DPOOL_DEFAULT			// 格納メモリ
	));

	// 画面描画用の2Dポリゴンの生成
	if (m_pDrawScreen == nullptr)
	{ // 画面描画用の2Dポリゴンが非使用中の場合

		// 画面描画用の2Dポリゴンの生成
		m_pDrawScreen = CObject2D::Create(SCREEN_CENT, SCREEN_SIZE);
		if (m_pDrawScreen == nullptr)
		{ // 画面描画用の2Dポリゴンが非使用中の場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを割当
		m_pDrawScreen->BindTexture(m_nRenderTextureID);

		// ラベルをスクリーンに設定
		m_pDrawScreen->SetLabel(CObject::LABEL_SCREEN);

		// 自動描画をOFFにする
		m_pDrawScreen->SetEnableDraw(false);
	}
	else { assert(false); return E_FAIL; }

	// テクスチャ用サーフェイスの生成
	hr = m_pD3DDevice->CreateDepthStencilSurface
	( // 引数
		SCREEN_WIDTH,				// 深度ステンシルのサーフェス横幅
		SCREEN_HEIGHT,				// 深度ステンシルのサーフェス縦幅
		FORMAT_DEPTH_STENCIL,		// 深度ステンシルのサーフェス形式
		D3DMULTISAMPLE_NONE,		// マルチサンプリングのバッファー型
		0,							// 品質レベル
		FALSE,						// Zバッファ破棄の有効/無効
		&m_pDepthStencilSurface,	// Zバッファ・ステンシルバッファのサーフェイスへのポインタ
		nullptr						// nullptr
	);
	if (FAILED(hr))
	{ // 描画先の生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 描画サーフェイスの取得
	hr = pTexture->GetTexture(m_nRenderTextureID)->GetSurfaceLevel
	( // 引数
		0,							// ミップマップレベル
		&m_pRenderTextureSurface	// 描画サーフェイスへのポインタ
	);
	if (FAILED(hr))
	{ // サーフェイスの取得に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 元の描画サーフェイスを保存
	hr = m_pD3DDevice->GetRenderTarget(0, &m_pDefRenderTextureSurface);
	if (FAILED(hr))
	{ // 描画サーフェイスの取得に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 元のZバッファ・ステンシルバッファのサーフェイスを保存
	hr = m_pD3DDevice->GetDepthStencilSurface(&m_pDefDepthStencilSurface);
	if (FAILED(hr))
	{ // Zバッファ・ステンシルバッファの取得に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	デバイス取得処理
//============================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void) const
{
	// デバイスのポインタを返す
	return m_pD3DDevice;
}

//============================================================
//	生成処理
//============================================================
CRenderer *CRenderer::Create(HWND hWnd, BOOL bWindow)
{
	// レンダラーの生成
	CRenderer *pRenderer = new CRenderer;
	if (pRenderer == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// レンダラーの初期化
		if (FAILED(pRenderer->Init(hWnd, bWindow)))
		{ // 初期化に失敗した場合

			// レンダラーの破棄
			SAFE_DELETE(pRenderer);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pRenderer;
	}
}

//============================================================
//	破棄処理
//============================================================
void CRenderer::Release(CRenderer *&prRenderer)
{
	// レンダラーの終了
	assert(prRenderer != nullptr);
	prRenderer->Uninit();

	// メモリ開放
	SAFE_DELETE(prRenderer);
}

//============================================================
//	デバイス生成処理
//============================================================
HRESULT CRenderer::CreateDevice(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp)
{
	// 変数を宣言
	HRESULT hr;	// 異常終了の確認用

	// 描画処理と頂点処理をハードウェアが行う設定でデバイスを生成
	hr = m_pD3D->CreateDevice
	( // 引数
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice
	);
	if (SUCCEEDED(hr))
	{ // 生成に成功した場合

		return S_OK;
	}

	// CPUが性能不足でデバイス生成に失敗した場合
	// 描画処理はハードウェア、頂点処理はCPUが行う設定でデバイスを生成
	hr = m_pD3D->CreateDevice
	( // 引数
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice
	);
	if (SUCCEEDED(hr))
	{ // 生成に成功した場合

		return S_OK;
	}

	// ハードウェアが性能不足でデバイス生成に失敗した場合
	// 描画処理と頂点処理をCPUが行う設定でデバイスを生成
	hr = m_pD3D->CreateDevice
	( // 引数
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_REF,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice
	);
	if (SUCCEEDED(hr))
	{ // 生成に成功した場合

		return S_OK;
	}

	// CPUとハードウェアの性能が使い物にならなかった場合
	// デバイス生成の失敗を返す
	return E_FAIL;
}
