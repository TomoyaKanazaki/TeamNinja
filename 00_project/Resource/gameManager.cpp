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
#include "rankingManager.h"
#include "retentionManager.h"
#include "camera.h"
#include "player.h"
#include "multiModel.h"

#include "enemy.h"
#include "checkpoint.h"
#include "popupUI.h"
#include "goal.h"

#include "gimmick.h"
#include "gimmick_malti.h"
#include "actor.h"
#include "MapModel.h"


//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 POS_NAME	 = D3DXVECTOR3(0.0f, 60.0f, 400.0f);	// 名前の表示位置
	const D3DXVECTOR3 POS_SKIP	 = D3DXVECTOR3(1092.0f, 673.0f, 0.0f);	// スキップ操作の表示位置
	const D3DXVECTOR3 SIZE_SKIP	 = D3DXVECTOR3(381.0f, 77.0f, 0.0f);	// スキップ操作の表示大きさ
	const int CHANGE_UI_PRIORITY = 5;		// シネマスコープ終了時のUI優先順位
	const float GAMEEND_WAITTIME = 2.0f;	// リザルト画面への遷移余韻フレーム

	const char* MAP_TXT = "data\\TXT\\map.txt"; // マップ情報のパス
	const char* START_TEXTURE = "data\\TEXTURE\\start.png";		// 開始のテクスチャ
}

//************************************************************
//	親クラス [CGameManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGameManager::CGameManager() :
	m_state	(STATE_NONE)	// 状態
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
	m_state = STATE_NORMAL;	// 状態

	// スタートUIを生成
	CPopUpUI::Create(START_TEXTURE);

#if 0
	CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, 400.0f), VEC3_ZERO, CEnemy::TYPE_STALK);
	CEnemy::Create(D3DXVECTOR3(-600.0f, 0.0f, -500.0f), VEC3_ZERO, CEnemy::TYPE_CAVEAT);
	CEnemy::Create(D3DXVECTOR3(600.0f, 0.0f, 400.0f), VEC3_ZERO, CEnemy::TYPE_WOLF);

	CGimmick::Create(D3DXVECTOR3(400.0f, 0.0f, -1000.0f), D3DXVECTOR3(300.0f, 0.0f, 100.0f), CGimmick::TYPE_JUMPTABLE, 2);
	CGimmick::Create(D3DXVECTOR3(800.0f, 0.0f, -1300.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f), CGimmick::TYPE_JUMPTABLE, 2);
	CGimmick::Create(D3DXVECTOR3(-400.0f, 0.0f, -500.0f), D3DXVECTOR3(200.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 2);

	CGimmick::Create(D3DXVECTOR3(-1000.0f, 0.0f, -500.0f), D3DXVECTOR3(100.0f, 0.0f, 50.0f), CGimmick::TYPE_JUMPOFF, 2);
	CGimmick::Create(D3DXVECTOR3(-1400.0f, 0.0f, -300.0f), D3DXVECTOR3(400.0f, 0.0f, 100.0f), CGimmick::TYPE_HEAVYDOOR, 6);

	CMapModel::Create(D3DXVECTOR3(-800.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE1);

	CMapModel::Create(D3DXVECTOR3(-400.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE2);

	CMapModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE3);

	CMapModel::Create(D3DXVECTOR3(400.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE4);

	CMapModel::Create(D3DXVECTOR3(1200.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE5);

	CMapModel::Create(D3DXVECTOR3(1500.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE6);

	CMapModel::Create(D3DXVECTOR3(1800.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE7);

	CMapModel::Create(D3DXVECTOR3(2100.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE8);

	CMapModel::Create(D3DXVECTOR3(2400.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE9);

	CMapModel::Create(D3DXVECTOR3(2700.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE10);

#endif

// 小原用ギミック置き場
#if 1
	// 踏み台
	CGimmick::Create(D3DXVECTOR3(2990.0f, 1.0f, 0.0f), D3DXVECTOR3(10.0f, 0.0f, 1600.0f), CGimmick::TYPE_STEP, 4);

	// 重い扉
	CGimmick::Create(D3DXVECTOR3(4500.0f, 301.0f, -300.0f), D3DXVECTOR3(400.0f, 0.0f, 100.0f), CGimmick::TYPE_HEAVYDOOR, 4);

	// ジャンプ台
	CGimmick::Create(D3DXVECTOR3(7150.0f, 301.0f, -300.0f), D3DXVECTOR3(100.0f, 0.0f, 300.0f), CGimmick::TYPE_JUMPTABLE, 3);

	{ // 複数ボタン
		std::vector<CGimmickMalti::SButton> vec;
		vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(6650.0f, 301.0f, 600.0f), D3DXVECTOR3(200.0f, 0.0f, 200.0f)));
		vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(7650.0f, 301.0f, 400.0f), D3DXVECTOR3(200.0f, 0.0f, 200.0f)));
		vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(7400.0f, 701.0f, -300.0f), D3DXVECTOR3(200.0f, 0.0f, 200.0f)));
		CGimmickMalti::Create(vec);
	}

#endif

// 藤田用ギミック置き場
#if 1
	// 梯子：手前
	CGimmick::Create(D3DXVECTOR3(-1450.0f, 1.0f, 450.0f), D3DXVECTOR3(200.0f, 0.0f, 700.0f), CGimmick::TYPE_STEP, 2);
	CGimmick::Create(D3DXVECTOR3(-950.0f, 151.0f, 450.0f), D3DXVECTOR3(200.0f, 0.0f, 700.0f), CGimmick::TYPE_STEP, 3);

	// ジャンプ台：手前
	CGimmick::Create(D3DXVECTOR3(-100.0f, 1.0f, -50.0f), D3DXVECTOR3(300.0f, 0.0f, 300.0f), CGimmick::TYPE_JUMPTABLE, 4);

	// ジャンプ台：奥
	CGimmick::Create(D3DXVECTOR3(1600.0f, 1.0f, 0.0f), D3DXVECTOR3(300.0f, 0.0f, 1600.0f), CGimmick::TYPE_JUMPTABLE, 6);
#endif

	// 複数ボタン
#if 0
	std::vector<CGimmickMalti::SButton> vec;
	vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(-200.0f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
	vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(   0.0f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
	vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3( 200.0f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
	CGimmickMalti::Create(vec);
#endif

	// 激重ドア
#if 0
	CGimmick::Create(D3DXVECTOR3(0.0f, 0.0f, 300.0f), D3DXVECTOR3(400.0f, 0.0f, 100.0f), CGimmick::TYPE_HEAVYDOOR, 2);
#endif

	// わんわんおー
#if 0
	CEnemy::Create(D3DXVECTOR3(0.0f, 2000.0f, 300.0f), VEC3_ZERO, CEnemy::TYPE_WOLF);
#endif

	// さむらい
#if 0
	CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, 400.0f), VEC3_ZERO, CEnemy::TYPE_STALK);
#endif

	// TPSカメラの目標位置の設定
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_TPS);
	GET_MANAGER->GetCamera()->SetDestTps();

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGameManager::Uninit(void)
{

}

//============================================================
//	更新処理
//============================================================
void CGameManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:
	case STATE_NORMAL:
		
		// 士気力が0の場合リザルトに
		if (GET_PLAYER->GetTension() == 0) 
		{
			TransitionResult(CRetentionManager::EWin::WIN_FAILED);
		}

		// ゴールしていた場合リザルト
		if (CGoal::GetGoal() != nullptr)
		{
			if (CGoal::GetGoal()->GetClear())
			{
				TransitionResult(CRetentionManager::EWin::WIN_CLEAR);
			}
		}

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	状態設定処理
//============================================================
void CGameManager::SetState(const EState state)
{
	// 状態を設定
	m_state = state;
}

//============================================================
//	状態取得処理
//============================================================
CGameManager::EState CGameManager::GetState(void) const
{
	// 状態を返す
	return m_state;
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

	// リザルト情報を保存
	GET_RETENTION->SetResult(win, CSceneGame::GetTimerUI()->GetTime());

	// リザルト画面に遷移
	GET_MANAGER->SetLoadScene(CScene::MODE_RESULT, GAMEEND_WAITTIME);

	if (win == CRetentionManager::WIN_CLEAR)
	{ // 勝利していた場合

		// ランキングに設定
		CRankingManager::Set(CSceneGame::GetTimerUI()->GetTime());
	}
	else
	{

	}
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
