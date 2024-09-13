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

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float APPEAR_ADD_ALPHA = 0.04f;		// 出現状態の透明度の加算数
	const float DISAPPEAR_SUB_ALPHA = 0.05f;	// 消滅状態の透明度の減算数
	const int WAIT_COUNT = 120;					// 待機状態のカウント数

	namespace logo
	{
		const D3DXVECTOR3 SIZE[CLogoManager::POLYGON_MAX] =		// サイズ
		{
			D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),		// 背景
			D3DXVECTOR3(520.0f, 520.0f, 0.0f),					// ロゴ
		};
		const D3DXCOLOR COL[CLogoManager::POLYGON_MAX] =		// 色
		{
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),		// 背景
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)		// ロゴ
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
	m_nStateCount(0),			// 状態カウント
	m_bDisappear(false)			// 消滅状況
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
	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{
		// ロゴマネージャーの生成
		m_apLogo[nCnt] = CObject2D::Create(SCREEN_CENT, logo::SIZE[nCnt], VEC3_ZERO, logo::COL[nCnt]);

		if (m_apLogo[nCnt] == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}

		// テクスチャの割り当て
		m_apLogo[nCnt]->BindTexture(logo::TEXTURE[nCnt]);
	}

	// 出現状態にする
	m_state = STATE_APPEAR;

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

	// 自身を消す
	delete this;
}

//============================================================
//	更新処理
//============================================================
void CLogoManager::Update(const float fDeltaTime)
{
	// スキップ処理
	UpdateSkip();

	switch (m_state)
	{
	case CLogoManager::STATE_APPEAR:

		// 出現状態処理
		UpdateAppear();

		break;

	case CLogoManager::STATE_WAIT:

		// 待機状態処理
		UpdateWait();

		break;

	case CLogoManager::STATE_DISAPPEAR:

		// 消滅処理
		UpdateDisappear();

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

//============================================================
// 出現状態処理
//============================================================
void CLogoManager::UpdateAppear(void)
{
	// 透明度を取得
	float fAlpha = m_apLogo[POLYGON_LOGO]->GetAlpha();

	// 透明度を加算する
	fAlpha += APPEAR_ADD_ALPHA;

	if (fAlpha >= 1.0f)
	{ // 透明度が1.0f以上の場合

		// 透明度を補正する
		fAlpha = 1.0f;

		// 待機状態にする
		m_state = STATE_WAIT;
	}

	// 透明度を設定
	m_apLogo[POLYGON_LOGO]->SetAlpha(fAlpha);
}

//============================================================
// 待機状態処理
//============================================================
void CLogoManager::UpdateWait(void)
{
	// 状態カウントを加算する
	m_nStateCount++;

	// 状態カウントが一定数未満の場合、関数を抜ける
	if (m_nStateCount < WAIT_COUNT) { return; }

	// 状態カウントを0にする
	m_nStateCount = 0;

	// 消滅状態にする
	m_state = STATE_DISAPPEAR;
}

//============================================================
// 消滅状態処理
//============================================================
void CLogoManager::UpdateDisappear(void)
{
	// 透明度を0に設定する
	float fAlpha = m_apLogo[POLYGON_LOGO]->GetAlpha();

	if (fAlpha >= 0.0f)
	{ // 透明度が0.0f以上の場合

		// 透明度を減算させる
		fAlpha -= DISAPPEAR_SUB_ALPHA;
		m_apLogo[POLYGON_LOGO]->SetAlpha(fAlpha);

		// 関数を抜ける
		return;
	}

	// 透明度を取得
	fAlpha = m_apLogo[POLYGON_BACK]->GetAlpha();

	// 透明度を減算する
	fAlpha -= DISAPPEAR_SUB_ALPHA;

	if (fAlpha <= 0.0f)
	{ // 透明度が0.0f以下の場合

		// 透明度を補正する
		fAlpha = 0.0f;

		// 消滅状況を true にする
		m_bDisappear = true;
	}

	// 透明度を設定
	m_apLogo[POLYGON_BACK]->SetAlpha(fAlpha);
}

//============================================================
// スキップ操作更新
//============================================================
void CLogoManager::UpdateSkip(void)
{
	CInputKeyboard* pKey = GET_INPUTKEY;	// キーボード情報
	CInputPad* pPad = GET_INPUTPAD;	// パッド情報

	if (pKey->IsTrigger(DIK_SPACE)
		|| pKey->IsTrigger(DIK_RETURN)
		|| pPad->IsTrigger(CInputPad::KEY_A)
		|| pPad->IsTrigger(CInputPad::KEY_B)
		|| pPad->IsTrigger(CInputPad::KEY_X)
		|| pPad->IsTrigger(CInputPad::KEY_Y))
	{
		// 消滅状態にする
		m_state = STATE_DISAPPEAR;
	}
}
