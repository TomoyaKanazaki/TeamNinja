//============================================================
//
//	ロゴマネージャー処理 [logoManager.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "logoManager.h"
#include "manager.h"
#include "fade.h"
#include "scene.h"
#include "sceneLogo.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	namespace logo
	{
		const D3DXVECTOR3 SIZE[CLogoManager::POLYGON_MAX] =		// サイズ
		{
			D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),		// 背景
			D3DXVECTOR3(200.0f, 200.0f, 0.0f),					// ロゴ
		};
		const char* TEXTURE[CLogoManager::POLYGON_MAX] =
		{
			nullptr,							// ロゴ
			"data\\TEXTURE\\alcohol.png",		// ロゴ
		};
	}
}

//************************************************************
//	親クラス [CLogoManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CLogoManager::CLogoManager() :
	m_state(STATE_APPEAR),		// 状態
	m_nStateCount(0)			// 状態カウント
{
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));
}

//============================================================
//	デストラクタ
//============================================================
CLogoManager::~CLogoManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CLogoManager::Init(void)
{
	// 待機状態にする
	m_state = STATE_WAIT;

	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{
		// ロゴマネージャーの生成
		m_apLogo[nCnt] = CObject2D::Create(SCREEN_CENT, logo::SIZE[nCnt]);

		if (m_apLogo[nCnt] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャの割り当て
		m_apLogo[nCnt]->BindTexture(logo::TEXTURE[nCnt]);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CLogoManager::Uninit(void)
{
	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{
		// ロゴマネージャーの破棄
		SAFE_UNINIT(m_apLogo[nCnt]);
	}
}

//============================================================
//	更新処理
//============================================================
void CLogoManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{
	case CLogoManager::STATE_APPEAR:



		break;

	case CLogoManager::STATE_WAIT:

		m_nStateCount++;

		if (m_nStateCount >= 60)
		{
			m_state = STATE_DISAPPEAR;
		}

		break;

	case CLogoManager::STATE_DISAPPEAR:

		// シーンの設定
		GET_MANAGER->SetLoadScene(CScene::MODE_TITLE);	// セレクト画面

		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
//	生成処理
//============================================================
CLogoManager* CLogoManager::Create(void)
{
	// ゲームマネージャーの生成
	CLogoManager* pLogoManager = new CLogoManager;
	if (pLogoManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ゲームマネージャーの初期化
		if (FAILED(pLogoManager->Init()))
		{ // 初期化に失敗した場合

			// ゲームマネージャーの破棄
			SAFE_DELETE(pLogoManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pLogoManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CLogoManager::Release(CLogoManager*& prLogoManager)
{
	// ゲームマネージャーの終了
	assert(prLogoManager != nullptr);
	prLogoManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prLogoManager);
}
