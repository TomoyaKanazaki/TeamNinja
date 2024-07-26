//============================================================
//
//	ゲームマネージャー処理 [gameManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "gameManager.h"
#include "manager.h"
#include "fade.h"
#include "scene.h"
#include "sceneGame.h"
#include "cinemaScope.h"
#include "timerUI.h"
#include "hitstop.h"
#include "resultManager.h"
#include "retentionManager.h"
#include "camera.h"
#include "player.h"
#include "player_clone.h"
#include "enemy.h"

#include "enemyStalk.h"
#include "enemyWolf.h"
#include "enemyAmbush.h"
#include "checkpoint.h"
#include "popupUI.h"
#include "goal.h"

#include "gimmick.h"
#include "gimmick_multi.h"
#include "actor.h"
#include "MapModel.h"
#include "camera_change.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* MAP_TXT			= "data\\TXT\\map.txt";			// マップ情報のパス
	const char* START_TEXTURE	= "data\\TEXTURE\\start.png";	// 開始のテクスチャ
	const char* END_TEXTURE[] =	// 終了のテクスチャ
	{
		nullptr,					// テクスチャ無し
		"data\\TEXTURE\\end.png",	// 敗北のテクスチャ
		"data\\TEXTURE\\end.png",	// 勝利のテクスチャ
	};

	const int HITSTOP_TIME = 75;	// ヒットストップフレーム

#ifdef _DEBUG
	bool bCamera = false;
#endif
}

//************************************************************
//	親クラス [CGameManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGameManager::CGameManager() :
	m_pResult	(nullptr),		// リザルトマネージャー
	m_state		(STATE_NONE)	// 状態
{

}

//============================================================
//	デストラクタ
//============================================================
CGameManager::~CGameManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGameManager::Init(void)
{
	// メンバ変数を初期化
	m_pResult	= nullptr;		// リザルトマネージャー
	m_state		= STATE_NORMAL;	// 状態

	// リザルトマネージャーの生成
	m_pResult = CResultManager::Create();
	if (m_pResult == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// スタートUIを生成
	CPopUpUI::Create(START_TEXTURE);

// わんわんおー
#if 0
	CEnemyWolf::Create(D3DXVECTOR3(0.0f, 2000.0f, 300.0f), VEC3_ZERO, 400.0f, 400.0f, 600.0, 500.0f);
	CEnemyWolf::Create(D3DXVECTOR3(0.0f, 2000.0f, -300.0f), VEC3_ZERO, 400.0f, 400.0f, 600.0, 500.0f);
#endif

// さむらい
#if 0
	//CEnemyStalk::Create(D3DXVECTOR3(300.0f, 0.0f, 400.0f), VEC3_ZERO, 400.0f, 400.0f, 600.0, 500.0f);
	CEnemyStalk::Create(D3DXVECTOR3(700.0f, 0.0f, -60.0f), VEC3_ZERO, 400.0f, 400.0f, 600.0, 500.0f);

#endif

	// 回り込みカメラの設定
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_AROUND);
	GET_MANAGER->GetCamera()->SetDestAround();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGameManager::Uninit(void)
{
	// リザルトマネージャーの破棄
	SAFE_REF_RELEASE(m_pResult);
}

//============================================================
//	更新処理
//============================================================
void CGameManager::Update(const float fDeltaTime)
{
#ifdef _DEBUG // カメラ切り替え
	if (GET_INPUTKEY->IsTrigger(DIK_RSHIFT))
	{
		// 状態を切り替え
		bCamera = !bCamera;

		if (bCamera)
		{
			GET_MANAGER->GetCamera()->SetState(CCamera::STATE_AROUND);
			GET_MANAGER->GetCamera()->SetDestAround();
		}
		else
		{
			GET_MANAGER->GetCamera()->SetState(CCamera::STATE_TELEPHOTO);
			GET_MANAGER->GetCamera()->SetDestTelephoto();
		}
	}
#endif

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:
	case STATE_NORMAL:

		// ゴールしていた場合リザルト
		if (CGoal::GetGoal() != nullptr)
		{
			if (CGoal::GetGoal()->GetClear())
			{ // クリアした場合

				// クリア成功でリザルト遷移
				TransitionResult(CRetentionManager::EWin::WIN_SUCCESS);
			}
			else if (CSceneGame::GetTimerUI()->GetState() == CTimer::STATE_END)
			{ // 時間切れになった場合

				// プレイヤーのタイムアップ処理
				GET_PLAYER->TimeUp();

				// クリア失敗でリザルト遷移
				TransitionResult(CRetentionManager::EWin::WIN_FAIL);
			}
		}
		break;

	case STATE_RESULT:

		if (!CSceneGame::GetHitStop()->IsStop())
		{ // ヒットストップが終わった場合

			// リザルトマネージャーの更新
			m_pResult->Update(fDeltaTime);
		}
		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	リザルト画面遷移処理
//============================================================
void CGameManager::TransitionResult(const CRetentionManager::EWin win)
{
	// フェード中の場合抜ける
	if (GET_MANAGER->GetFade()->IsFade()) { return; }

	// タイマーの計測終了
	CSceneGame::GetTimerUI()->End();

	// リザルト情報の保存
	GET_RETENTION->SetResult(win, CSceneGame::GetTimerUI()->GetTime());

	// キャラクターたちを全て消滅させる
	CPlayerClone::VanishAll();	// 分身
	CEnemy::VanishAll();		// 敵

	// ヒットストップ終了時に呼ばれる関数を作成
	auto funcEndHitStop = []
	{
		// プレイヤーをリザルト状態にする
		GET_PLAYER->SetResult();

		// カメラをリザルト状態にする
		GET_MANAGER->GetCamera()->SetState(CCamera::STATE_RESULT);
	};

	// ヒットストップの設定
	CSceneGame::GetHitStop()->SetStop(HITSTOP_TIME, funcEndHitStop);

	// リザルト状態にする
	m_state = STATE_RESULT;
}

//============================================================
//	生成処理
//============================================================
CGameManager *CGameManager::Create(void)
{
	// ゲームマネージャーの生成
	CGameManager *pGameManager = new CGameManager;
	if (pGameManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ゲームマネージャーの初期化
		if (FAILED(pGameManager->Init()))
		{ // 初期化に失敗した場合

			// ゲームマネージャーの破棄
			SAFE_DELETE(pGameManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pGameManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CGameManager::Release(CGameManager *&prGameManager)
{
	// ゲームマネージャーの終了
	assert(prGameManager != nullptr);
	prGameManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prGameManager);
}
