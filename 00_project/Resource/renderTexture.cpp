//============================================================
//
//	レンダーテクスチャ処理 [renderTexture.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "renderTexture.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	子クラス [CRenderTexture] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CRenderTexture::CRenderTexture(const CObject::EScene scene) :
	m_pSurTexture	(nullptr),	// テクスチャサーフェイスへのポインタ
	m_nTextureID	(0),		// レンダーテクスチャインデックス
	m_scene			(scene)		// 描画シーン
{

}

//============================================================
//	デストラクタ
//============================================================
CRenderTexture::~CRenderTexture()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CRenderTexture::Init(void)
{
	CTexture *pTexture = GET_MANAGER->GetTexture();	// テクスチャへのポインタ
	const D3DFORMAT format = (m_scene == CObject::SCENE_MAIN) ? D3DFMT_X8R8G8B8 : D3DFMT_A8R8G8B8;	// ピクセルフォーマット

	// メンバ変数を初期化
	m_pSurTexture	= nullptr;	// テクスチャサーフェイスへのポインタ
	m_nTextureID	= 0;		// レンダーテクスチャインデックス

	// 空のスクリーンテクスチャを生成
	m_nTextureID = pTexture->Regist(CTexture::SInfo
	( // 引数
		SCREEN_WIDTH,			// テクスチャ横幅
		SCREEN_HEIGHT,			// テクスチャ縦幅
		0,						// ミップマップレベル
		D3DUSAGE_RENDERTARGET,	// 性質・確保オプション
		format,					// ピクセルフォーマット
		D3DPOOL_DEFAULT			// 格納メモリ
	));

	// スクリーン描画サーフェイスの取得
	HRESULT hr = pTexture->GetPtr(m_nTextureID)->GetSurfaceLevel
	( // 引数
		0,				// ミップマップレベル
		&m_pSurTexture	// テクスチャサーフェイスへのポインタ
	);
	if (FAILED(hr))
	{ // サーフェイス取得に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CRenderTexture::Uninit(void)
{
	// テクスチャサーフェイスの破棄
	SAFE_RELEASE(m_pSurTexture);
}

//============================================================
//	描画処理
//============================================================
void CRenderTexture::Draw(void)
{
	CRenderer *pRenderer = GET_RENDERER;	// レンダラーへのポインタ

	// レンダーテクスチャへの書き込み
	pRenderer->DrawRenderTexture(m_scene, &m_pSurTexture);
}

//============================================================
//	生成処理
//============================================================
CRenderTexture *CRenderTexture::Create(const CObject::EScene scene)
{
	// レンダーテクスチャの生成
	CRenderTexture *pRenderTexture = new CRenderTexture(scene);
	if (pRenderTexture == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// レンダーテクスチャの初期化
		if (FAILED(pRenderTexture->Init()))
		{ // 初期化に失敗した場合

			// レンダーテクスチャの破棄
			SAFE_DELETE(pRenderTexture);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pRenderTexture;
	}
}

//============================================================
//	破棄処理
//============================================================
void CRenderTexture::Release(CRenderTexture *&prRenderTexture)
{
	// レンダーテクスチャの終了
	assert(prRenderTexture != nullptr);
	prRenderTexture->Uninit();

	// メモリ開放
	SAFE_DELETE(prRenderTexture);
}
