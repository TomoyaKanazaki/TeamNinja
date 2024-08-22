//============================================================
//
//	セレクトマネージャー処理 [selectManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "selectManager.h"
#include "manager.h"
#include "camera.h"
#include "fade.h"
#include "scene.h"
#include "sceneSelect.h"
#include "player.h"

//************************************************************
//	親クラス [CSelectManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSelectManager::CSelectManager() :
	m_state	(STATE_NONE)	// 状態
{

}

//============================================================
//	デストラクタ
//============================================================
CSelectManager::~CSelectManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSelectManager::Init(void)
{
	// メンバ変数を初期化
	m_state = STATE_NORMAL;	// 状態

	// 回り込みカメラの設定
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_AROUND);
	GET_MANAGER->GetCamera()->SetDestAround();

	// TODO：仮置き
	// プレイヤーを通常状態にする
	CPlayer::GetList()->GetList().front()->SetState(CPlayer::EState::STATE_NORMAL);
	CPlayer::GetList()->GetList().front()->SetAlpha(1.0f);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSelectManager::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CSelectManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:
	case STATE_NORMAL:
		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	状態設定処理
//============================================================
void CSelectManager::SetState(const EState state)
{
	// 状態を設定
	m_state = state;
}

//============================================================
//	状態取得処理
//============================================================
CSelectManager::EState CSelectManager::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	生成処理
//============================================================
CSelectManager *CSelectManager::Create(void)
{
	// セレクトマネージャーの生成
	CSelectManager *pSelectManager = new CSelectManager;
	if (pSelectManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// セレクトマネージャーの初期化
		if (FAILED(pSelectManager->Init()))
		{ // 初期化に失敗した場合

			// セレクトマネージャーの破棄
			SAFE_DELETE(pSelectManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pSelectManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CSelectManager::Release(CSelectManager *&prSelectManager)
{
	// セレクトマネージャーの終了
	assert(prSelectManager != nullptr);
	prSelectManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prSelectManager);
}
