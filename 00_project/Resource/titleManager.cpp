//============================================================
//
//	タイトルマネージャー処理 [titleManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "titleManager.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"
#include "titleLogo2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 5;	// タイトルの優先順位
	const D3DXVECTOR3 ABS_INIT_OFFSET	= D3DXVECTOR3(80.0f, 0.0f, 0.0f);	// 生成位置オフセットの絶対値
	const D3DXVECTOR3 DEST_OFFSET		= D3DXVECTOR3(0.0f, 170.0f, 0.0f);	// 生成位置オフセットの絶対値
	const D3DXVECTOR3 POS_POLY	= D3DXVECTOR3(640.0f, 95.0f, 0.0f);			// 位置
	const D3DXVECTOR3 SIZE_POLY	= D3DXVECTOR3(320.0f, 320.0f, 0.0f);		// 大きさ
	const POSGRID2 TEX_PART = POSGRID2(1, 4);	// テクスチャ分割数
	const float MOVE_TIME = 1.8f;	// 生成移動時間
	const float WAIT_TIME = 0.092f;	// 生成待機時間

	const D3DXCOLOR COL_BLUR[] =	// ブラー色
	{
		D3DCOLOR_RGBA(110, 228, 234, 255),	// 水色
		D3DCOLOR_RGBA(255, 142, 204, 255),	// 紫色
	};
}

//************************************************************
//	親クラス [CTitleManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTitleManager::CTitleManager()
{
	// メンバ変数をクリア
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));	// タイトル情報
}

//============================================================
//	デストラクタ
//============================================================
CTitleManager::~CTitleManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTitleManager::Init(void)
{
	// メンバ変数を初期化
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));	// タイトル情報

	//--------------------------------------------------------
	//	タイトルの生成・設定
	//--------------------------------------------------------
	for (int i = 0; i < NUM_LOGO; i++)
	{ // ロゴの文字数分繰り返す

		int nFlag = (i % 2);					// 数値フラグ
		float fSide = (1.0f - (nFlag * 2.0f));	// 生成方向係数
		float fWaitTime = WAIT_TIME * (float)i;	// 待機時間
		D3DXCOLOR colBlur = COL_BLUR[nFlag];	// 生成位置オフセット
		D3DXVECTOR3 destOffset = DEST_OFFSET * (float)i;	// 目標位置
		D3DXVECTOR3 initOffset = ABS_INIT_OFFSET * fSide;	// 生成位置オフセット

		// タイトルの生成
		m_apLogo[i] = CTitleLogo2D::Create
		( // 引数
			"data\\TEXTURE\\title_blur000.png",	// ブラーテクスチャパス
			POS_POLY + destOffset,	// 位置
			initOffset,	// オフセット
			SIZE_POLY,	// 大きさ
			MOVE_TIME,	// 移動時間
			fWaitTime	// 待機時間
		);
		if (m_apLogo[i] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャを割当
		m_apLogo[i]->BindTexture("data\\TEXTURE\\title_logo000.png");

		// オーラテクスチャを割当
		m_apLogo[i]->BindAuraTexture("data\\TEXTURE\\title_aura000.png");

		// ブラー色を設定
		m_apLogo[i]->SetBlurColor(colBlur);

		// パターンを設定
		m_apLogo[i]->SetPattern(i);

		// テクスチャ横分割数を設定
		m_apLogo[i]->SetWidthPattern(TEX_PART.x);

		// テクスチャ縦分割数を設定
		m_apLogo[i]->SetHeightPattern(TEX_PART.y);

		// 優先順位を設定
		m_apLogo[i]->SetPriority(PRIORITY);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTitleManager::Uninit(void)
{
	for (int i = 0; i < NUM_LOGO; i++)
	{ // ロゴの文字数分繰り返す

		// タイトル情報の終了
		SAFE_UNINIT(m_apLogo[i]);
	}
}

//============================================================
//	更新処理
//============================================================
void CTitleManager::Update(const float fDeltaTime)
{
	if (GET_INPUTKEY->IsTrigger(DIK_0))
	{
		for (int i = 0; i < NUM_LOGO; i++)
		{ // ロゴの文字数分繰り返す
			m_apLogo[i]->SetStag();
		}
	}
}

//============================================================
//	生成処理
//============================================================
CTitleManager *CTitleManager::Create(void)
{
	// タイトルマネージャーの生成
	CTitleManager *pTitleManager = new CTitleManager;
	if (pTitleManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// タイトルマネージャーの初期化
		if (FAILED(pTitleManager->Init()))
		{ // 初期化に失敗した場合

			// タイトルマネージャーの破棄
			SAFE_DELETE(pTitleManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pTitleManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CTitleManager::Release(CTitleManager *&prTitleManager)
{
	// タイトルマネージャーの終了
	assert(prTitleManager != nullptr);
	prTitleManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prTitleManager);
}
