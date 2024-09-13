//============================================================
//
//	Zテクスチャ生成 [ZTexture.cpp]
//	Author：丹野竜之介
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "ZTexture.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "texture.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CZTexture* CZTexture::m_pShader = nullptr;	// シェーダー情報

// コンストラクタ
CZTexture::CZTexture()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	m_hWorldMat = NULL;
	m_hViewMat = NULL;
	m_hProjMat = NULL;
	m_hTechnique = NULL;
}

// デストラクタ
CZTexture::~CZTexture()
{
	// COMポインタを明示的に消去
	m_cpDepthBuff = NULL;	// 深度バッファ
	m_cpZTexSurf = NULL;	// Z値テクスチャサーフェイス
	m_cpZTex = NULL;		// Z値テクスチャ
	m_cpDevBuffer = NULL;	// デバイスバックバッファ
	m_cpDevDepth = NULL;	// デバイス深度バッファ
	m_cpEffect = NULL;		// Z値プロットエフェクト
}


// 初期化メソッド
bool CZTexture::Init()
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報
	if (pDevice == NULL) return false;
	if constexpr (ZTEX_WIDTH == 0 || ZTEX_HEIGHT == 0) return false;

	HRESULT hr;
	ID3DXBuffer* pError = NULL;
	// Z値プロットシェーダプログラムを読み込む
	if (FAILED(D3DXCreateEffectFromFile(
		pDevice,
		("data\\SHADER\\ZTexCreator.fx"),
		NULL,
		NULL,
		0,
		NULL,
		&m_cpEffect,
		&pError)))
		if (pError)
		{
			OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
			//デバッグコンソールに表示する
			MessageBoxA(NULL, (LPCSTR)pError->GetBufferPointer(), "Shader Error", MB_OK);
			return false;
		}
	

	// エフェクト内の各種パラメータハンドルを取得
	m_hWorldMat = m_cpEffect->GetParameterByName(NULL, "matWorld");
	m_hViewMat = m_cpEffect->GetParameterByName(NULL, "matView");
	m_hProjMat = m_cpEffect->GetParameterByName(NULL, "matProj");
	m_hTechnique = m_cpEffect->GetTechniqueByName("ZValuePlotTec");
	if (!m_hWorldMat || !m_hViewMat || !m_hProjMat || !m_hTechnique)
		return false;


	// 指定のZ値テクスチャを生成
	hr = D3DXCreateTexture(
		pDevice,
		ZTEX_WIDTH, ZTEX_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A32B32G32R32F,
		D3DPOOL_DEFAULT,
		&m_cpZTex);
	if (FAILED(hr))
		return false;

	m_cpZTex->GetSurfaceLevel(0, &m_cpZTexSurf);	// サーフェイス取得

	// 描画デバイスに定義されているバッファの能力を取得
	IDirect3DSurface9 *pSurf;
	pDevice->GetDepthStencilSurface(&pSurf);
	D3DSURFACE_DESC Desc;
	pSurf->GetDesc(&Desc);
	pSurf->Release();

	// 独自深度バッファを作成
	hr = pDevice->CreateDepthStencilSurface(
		ZTEX_WIDTH, ZTEX_HEIGHT,
		Desc.Format,
		Desc.MultiSampleType,
		Desc.MultiSampleQuality,
		FALSE,
		&m_cpDepthBuff,
		NULL
	);
	if (FAILED(hr))
		return false;

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(VIEWING_ANGLE), (ZTEX_WIDTH / ZTEX_HEIGHT), NEAR_CLIP, FAR_CLIP);
	D3DXMatrixLookAtLH(&m_matView, &VIEW_POINT, &FOCUS_POINT, &LOOK_UP);

	D3DXCreateSprite(pDevice, &m_pSprite);// スプライト作成

	m_bBegin = false;
	return true;
}


// 描画対象オブジェクトのワールド変換行列を設定
void CZTexture::SetWorldMatrix(D3DXMATRIX *pMat)
{
	m_matWorld = *pMat;
}


// ビュー行列を設定
void CZTexture::SetViewMatrix(D3DXMATRIX *pMat)
{
	m_matView = *pMat;
}


// 射影変換行列を設定
void CZTexture::SetProjMatrix(D3DXMATRIX *pMat)
{
	m_matProj = *pMat;
}


// 描画の開始を宣言する
HRESULT CZTexture::Begin()
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報
	// 初期化が正しく行われているかチェック
	if (m_cpDepthBuff == NULL || m_cpZTex == NULL)
		return E_FAIL;

	// デバイスが持っているバッファを一時保存
	pDevice->GetRenderTarget(0, &m_cpDevBuffer);
	pDevice->GetDepthStencilSurface(&m_cpDevDepth);

	// デバイスにZ値テクスチャサーフェイスと深度バッファを設定
	pDevice->SetRenderTarget(0, m_cpZTexSurf);
	pDevice->SetDepthStencilSurface(m_cpDepthBuff);

	// 各サーフェイスを初期化
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);

	// プログラマブルシェーダのテクニックを設定
	m_cpEffect->SetTechnique(m_hTechnique);

	// シェーダの開始を宣言
	UINT Tmp;
	m_cpEffect->Begin(&Tmp, 0);
	m_bBegin = true;
	return S_OK;
}


// パスの開始を宣言する
HRESULT CZTexture::BeginPass()
{
	m_cpEffect->BeginPass(0);	// Z値計算は1パス
	return S_OK;
}


// 登録されているパラメータ情報をエフェクトにセット
bool CZTexture::SetParamToEffect()
{
	if (m_cpEffect == NULL) return false;

	m_cpEffect->SetMatrix(m_hWorldMat, &m_matWorld);
	m_cpEffect->SetMatrix(m_hViewMat, &m_matView);
	m_cpEffect->SetMatrix(m_hProjMat, &m_matProj);

	return true;
}


// パスの終了を宣言する
HRESULT CZTexture::EndPass()
{
	m_cpEffect->EndPass();
	return S_OK;
}


// 描画の終了を宣言する
HRESULT CZTexture::End()
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイス情報
	m_cpEffect->End();

	// デバイスに元のサーフェイスを戻す
	pDevice->SetRenderTarget(0, m_cpDevBuffer);
	pDevice->SetDepthStencilSurface(m_cpDevDepth);

	m_cpDevBuffer = NULL;
	m_cpDevDepth = NULL;

	// 固定機能に戻す
	pDevice->SetVertexShader(NULL);
	pDevice->SetPixelShader(NULL);
	m_bBegin = false;
	return S_OK;
}


// Z値テクスチャを取得する
bool CZTexture::GetZTex(IDirect3DTexture9 **cpTex)
{
	*cpTex = m_cpZTex;
	return true;
}

//============================================================
//	生成処理
//============================================================
CZTexture* CZTexture::Create(void)
{
	// インスタンス使用中
	assert(m_pShader == nullptr);

	// シェーダーの生成
	m_pShader = new CZTexture;
	if (m_pShader == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 初期化
		if (!m_pShader->Init())
		{ // 初期化に失敗した場合

			// 破棄
			SAFE_DELETE(m_pShader);
			return nullptr;
		}

		// 確保したアドレスを返す
		return m_pShader;
	}
}

//============================================================
//	取得処理
//============================================================
CZTexture* CZTexture::GetInstance(void)
{
	if (m_pShader == nullptr)
	{
		m_pShader = CZTexture::Create();
	}

	// ポインタを返す
	return m_pShader;
}

//============================================================
//	破棄処理
//============================================================
void CZTexture::Release(void)
{
	// 終了
	assert(m_pShader != nullptr);
	// メモリ開放
	SAFE_DELETE(m_pShader);
}
//============================================================
//	スプライト描画処理
//============================================================
void CZTexture::DrawSprite()
{
	//スケーリング
	D3DXMATRIX SpriteScaleMat;
	D3DXMatrixScaling(&SpriteScaleMat, 0.25f, 0.25f, 1.0f);
	m_pSprite->SetTransform(&SpriteScaleMat);
	//描画
	m_pSprite->Begin(0);
	m_pSprite->Draw(m_cpZTex, NULL, NULL, NULL, 0xffffffff);
	m_pSprite->End();
}