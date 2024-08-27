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
#include "godItemManager.h"
#include "resultManager.h"
#include "retentionManager.h"
#include "camera.h"
#include "player.h"
#include "player_clone.h"
#include "enemy.h"
#include "sound.h"

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

// TODO
#include "weed.h"
#include "collision.h"

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

	const int HITSTOP_TIME = 0;	// ヒットストップフレーム

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
	m_state		(STATE_NONE),	// 状態
	m_nSave		(-1)			// セーブポイント
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
	m_state		= STATE_START;	// 状態
	m_nSave		= -1;			// セーブポイント

	// リザルトマネージャーの生成
	m_pResult = CResultManager::Create();
	if (m_pResult == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 回り込みカメラの設定
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_AROUND);
	GET_MANAGER->GetCamera()->SetDestAround();

	// シネマスコープ始め
	//CSceneGame::GetCinemaScope()->SetScopeIn();

#if 0

	// TODO：仮置き
	// プレイヤーを通常状態にする
	CPlayer::GetList()->GetList().front()->SetState(CPlayer::EState::STATE_NORMAL);
	CPlayer::GetList()->GetList().front()->SetAlpha(1.0f);

	// 通常状態にする
	m_state = STATE_NORMAL;

#else

	// スタートカメラのリセット処理
	GET_MANAGER->GetCamera()->StartReset();

#endif // 0

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

		// ゴールがない場合抜ける
		if (CGoal::GetGoal() == nullptr) { break; }

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
		break;

	case STATE_START:
		break;

	case STATE_GODITEM:

		/*
			この状態時は自動的に更新が行われる神器獲得演出マネージャーがゲーム画面を操作します。
			状態の復帰もマネージャーがプレイヤーの操作を検知し行うのでこちら側から管理する必要はないです。
		*/
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
//	勾玉獲得処理
//============================================================
void CGameManager::PossessGodItem(const CGodItem::EType typeID)
{
	// タイマーの計測一時停止
	CSceneGame::GetTimerUI()->EnableStop(true);

	// プレイヤーの状態を神器獲得状態にする
	GET_PLAYER->SetEnableGodItem(true);

	// 神器獲得演出マネージャーを生成
	CGodItemManager::Create(typeID);

	// 神器獲得状態にする
	m_state = STATE_GODITEM;
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
	switch (win)
	{
	case CRetentionManager::WIN_SUCCESS: // クリア成功

		GET_RETENTION->SetResult(win, CSceneGame::GetTimerUI()->GetTime(), -1);

		// TODO：タイミング変えたいかも(BGMの再生)
		PLAY_SOUND(CSound::LABEL_BGM_RESULT);
		break;

	case CRetentionManager::WIN_FAIL: // クリア失敗
		GET_RETENTION->SetResult(win, CSceneGame::GetTimerUI()->GetTime(), m_nSave);

		// TODO：タイミング変えたいかも(BGMの再生)
		PLAY_SOUND(CSound::LABEL_BGM_GAMEOVER);
		break;

	default:
		assert(false);
		GET_RETENTION->SetResult(win, CSceneGame::GetTimerUI()->GetTime(), -1);
		break;
	}

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

	// BGMの停止
	STOP_SOUND(CSound::LABEL_BGM_FOREST);
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
