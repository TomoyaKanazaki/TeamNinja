//============================================================
//
//	ゲーム画面処理 [sceneGame.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneGame.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"

#include "gameManager.h"
#include "pause.h"
#include "edit.h"

#include "stage.h"
#include "player.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{

}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CGameManager	*CSceneGame::m_pGameManager	= nullptr;	// ゲームマネージャー
CPause			*CSceneGame::m_pPause		= nullptr;	// ポーズ
CEdit			*CSceneGame::m_pEdit		= nullptr;	// エディット

//************************************************************
//	子クラス [CSceneGame] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneGame::CSceneGame(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneGame::~CSceneGame()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneGame::Init(void)
{
	//--------------------------------------------------------
	//	初期生成
	//--------------------------------------------------------
	// シーンの初期化
	CScene::Init();	// ステージ・プレイヤーの生成

#if _DEBUG

	// エディットの生成
	m_pEdit = CEdit::Create(CEdit::MODE_PLAY);
	if (m_pEdit == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

#endif	// _DEBUG

	// ポーズの生成
	m_pPause = CPause::Create();
	if (m_pPause == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ゲームマネージャーの生成
	m_pGameManager = CGameManager::Create();
	if (m_pGameManager == nullptr)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 操作カメラを設定
	CCamera *pCamera = GET_MANAGER->GetCamera();	// カメラ情報
	pCamera->SetState(CCamera::STATE_CONTROL);		// 操作カメラ設定

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSceneGame::Uninit(void)
{
	// エディットの破棄
	SAFE_REF_RELEASE(m_pEdit);

	// ポーズの破棄
	SAFE_REF_RELEASE(m_pPause);

	// ゲームマネージャーの破棄
	SAFE_REF_RELEASE(m_pGameManager);

	// シーンの終了
	CScene::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSceneGame::Update(void)
{
	if (m_pEdit != nullptr)
	{ // 使用中の場合

		// エディットの更新
		m_pEdit->DrawEditControl();
		m_pEdit->DrawEditData();
		m_pEdit->Update();
	}

	// ポーズの更新
	assert(m_pPause != nullptr);
	m_pPause->Update();

	// ゲームマネージャーの更新
	assert(m_pGameManager != nullptr);
	m_pGameManager->Update();

	if (!m_pPause->IsPause() && m_pEdit->GetMode() == CEdit::MODE_PLAY)
	{ // ポーズ中ではない且つ、プレイ状態の場合

		// シーンの更新
		CScene::Update();
	}

#ifdef _DEBUG

	else
	{ // ポーズ中の場合

		if (GET_MANAGER->GetCamera()->GetState() == CCamera::STATE_CONTROL)
		{ // カメラが操作状態の場合

			// カメラの更新
			GET_MANAGER->GetCamera()->Update();
		}
	}

#endif	// _DEBUG

}

//============================================================
//	描画処理
//============================================================
void CSceneGame::Draw(void)
{

}

//============================================================
//	ゲームマネージャー取得処理
//============================================================
CGameManager *CSceneGame::GetGameManager(void)
{
	// インスタンス未使用
	assert(m_pGameManager != nullptr);

	// ゲームマネージャーのポインタを返す
	return m_pGameManager;
}

//============================================================
//	ポーズ取得処理
//============================================================
CPause *CSceneGame::GetPause(void)
{
	// インスタンス未使用
	assert(m_pPause != nullptr);

	// ポーズのポインタを返す
	return m_pPause;
}

//============================================================
//	エディット設定処理
//============================================================
void CSceneGame::SetEdit(CEdit *pEdit)
{
	// インスタンス未使用
	assert(pEdit != nullptr);

	// エディットのポインタを設定
	m_pEdit = pEdit;
}

//============================================================
//	エディット取得処理
//============================================================
CEdit *CSceneGame::GetEdit(void)
{
#if _DEBUG

	// インスタンス未使用
	assert(m_pEdit != nullptr);

	// エディットのポインタを返す
	return m_pEdit;

#else	// NDEBUG

	return nullptr;

#endif	// _DEBUG
}
