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
#include "checkpoint.h"
#include "popupUI.h"
#include "goal.h"

#include "gimmick.h"
#include "gimmick_malti.h"
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

	const CCamera::SSwing CLEAR_SWING = CCamera::SSwing(18.0f, 2.2f, 0.35f);	// リザルト遷移時のカメラ揺れ
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

// 森00マップ用ギミック置き場
#if 0

	

	// ジャンプ台
	//CGimmick::Create(D3DXVECTOR3(8050.0f, 0.0f, 400.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 800.0f), CGimmick::TYPE_JUMPTABLE, 3);



	// 橋
	//CGimmick::Create(D3DXVECTOR3(14400.0f, 500.0f, -250.0f), EAngle::ANGLE_90, D3DXVECTOR3(650.0f, 0.0f, 1100.0f), CGimmick::TYPE_BRIDGE, 5);


	//=================================================
	// [ボタンのみ]
	//=================================================
	// ボタン
	//{ // 複数ボタン：３つ
	//	std::vector<CGimmickMalti::SButton> vec;
	//	vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(17900.0f, 1200.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
	//	vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(17275.0f, 1200.0f, 500.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
	//	vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(18475.0f, 1200.0f, 500.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
	//  vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(17275.0f, 1200.0f, -500.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
	//	vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(18475.0f, 1200.0f, -500.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
	// 
	//	CGimmickMalti::Create(vec);
	//}
#endif

// 森02マップ用ギミック置き場
#if 0

	//=================================================
	// [左側のやつ]
	//=================================================
	// 重ドア：中央
	CGimmick::Create(D3DXVECTOR3(600.0f, 1.0f, 100.0f), EAngle::ANGLE_0, D3DXVECTOR3(400.0f, 0.0f, 100.0f), CGimmick::TYPE_HEAVYDOOR, 4);

	// ジャンプ台：奥
	CGimmick::Create(D3DXVECTOR3(-575.0f, 150.0f, 425.0f), EAngle::ANGLE_0, D3DXVECTOR3(650.0f, 0.0f, 750.0f), CGimmick::TYPE_JUMPTABLE, 4);
	CGimmick::Create(D3DXVECTOR3(4574.0f, 300.0f, 450.0f), EAngle::ANGLE_0, D3DXVECTOR3(750.0f, 0.0f, 50.0f), CGimmick::TYPE_JUMPTABLE, 3);


	// ジャンプ台：手前
	CGimmick::Create(D3DXVECTOR3(4950.0f, 700.0f, 75.0f), EAngle::ANGLE_0, D3DXVECTOR3(50.0f, 0.0f, 750.0f), CGimmick::TYPE_JUMPTABLE, 4);


	// 踏み台(梯子)：奥
	CGimmick::Create(D3DXVECTOR3(-1350.0f, 0.0f, 225.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 1150.0f), CGimmick::TYPE_STEP, 1);
	CGimmick::Create(D3DXVECTOR3(3000.0f, 500.0f, 450.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 700.0f), CGimmick::TYPE_STEP, 1);
	CGimmick::Create(D3DXVECTOR3(3800.0f, 300.0f, 450.0f), EAngle::ANGLE_90, D3DXVECTOR3(800.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 3);
	CGimmick::Create(D3DXVECTOR3(6400.0f, 0.0f, 0.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 1600.0f), CGimmick::TYPE_STEP, 4);


	// 踏み台(梯子)：手前
	CGimmick::Create(D3DXVECTOR3(1750.0f, 0.0f, -350.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 900.0f), CGimmick::TYPE_STEP, 4);
	CGimmick::Create(D3DXVECTOR3(2550.0f, 0.0f, -350.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 900.0f), CGimmick::TYPE_STEP, 4);
	CGimmick::Create(D3DXVECTOR3(2150.0f, 300.0f, 100.0f), EAngle::ANGLE_90, D3DXVECTOR3(800.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 4);
	CGimmick::Create(D3DXVECTOR3(3000.0f, 0.0f, -350.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 900.0f), CGimmick::TYPE_STEP, 4);
	CGimmick::Create(D3DXVECTOR3(3175.0f, 300.0f, 100.0f), EAngle::ANGLE_90, D3DXVECTOR3(350.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 3);
	CGimmick::Create(D3DXVECTOR3(3400.0f, 300.0f, -350.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 900.0f), CGimmick::TYPE_STEP, 1);
	CGimmick::Create(D3DXVECTOR3(3775.0f, 400.0f, 100.0f), EAngle::ANGLE_90, D3DXVECTOR3(750.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 1);
	CGimmick::Create(D3DXVECTOR3(4200.0f, 500.0f, 275.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 300.0f), CGimmick::TYPE_STEP, 3);
	CGimmick::Create(D3DXVECTOR3(4550.0f, 400.0f, -125.0f), EAngle::ANGLE_90, D3DXVECTOR3(800.0f, 0.0f, 400.0f), CGimmick::TYPE_STEP, 4);
	CGimmick::Create(D3DXVECTOR3(5300.0f, 1400.0f, 75.0f), EAngle::ANGLE_90, D3DXVECTOR3(200.0f, 0.0f, 200.0f), CGimmick::TYPE_STEP, 6);

	// 橋：奥
	CGimmick::Create(D3DXVECTOR3(600.0f, 600.0f, 450.0f), EAngle::ANGLE_90, D3DXVECTOR3(850.0f, 0.0f, 700.0f), CGimmick::TYPE_BRIDGE, 5);


	// 橋：手前
	CGimmick::Create(D3DXVECTOR3(2775.0f, 300.0f, -350.0f), EAngle::ANGLE_90, D3DXVECTOR3(500.0f, 0.0f, 900.0f), CGimmick::TYPE_BRIDGE, 3);

	//=================================================
	// [水辺]
	//=================================================
	// ジャンプ台：手前
	CGimmick::Create(D3DXVECTOR3(8225.0f, 100.0f, -575.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 450.0f), CGimmick::TYPE_JUMPTABLE, 3);
	CGimmick::Create(D3DXVECTOR3(9100.0f, 350.0f, -550.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 400.0f), CGimmick::TYPE_JUMPTABLE, 3);
	CGimmick::Create(D3DXVECTOR3(8850.0f, 350.0f, -375.0f), EAngle::ANGLE_90, D3DXVECTOR3(350.0f, 0.0f, 50.0f), CGimmick::TYPE_JUMPTABLE, 3);

	// ジャンプ台：奥
	CGimmick::Create(D3DXVECTOR3(7200.0f, 0.0f, 475.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 650.0f), CGimmick::TYPE_JUMPTABLE, 4);


	// 踏み台(梯子)：手前
	CGimmick::Create(D3DXVECTOR3(7200.0f, 0.0f, -575.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 450.0f), CGimmick::TYPE_STEP, 1);

	// 踏み台(梯子)：奥
	CGimmick::Create(D3DXVECTOR3(9350.0f, 500.0f, 625.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 350.0f), CGimmick::TYPE_STEP, 2);
	CGimmick::Create(D3DXVECTOR3(9675.0f, 650.0f, 100.0f), EAngle::ANGLE_90, D3DXVECTOR3(300.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 1);
	CGimmick::Create(D3DXVECTOR3(10750.0f, 125.0f, 350.0f), EAngle::ANGLE_90, D3DXVECTOR3(400.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 4);


	// 橋：手前
	CGimmick::Create(D3DXVECTOR3(9875.0f, 125.0f, 50.0f), EAngle::ANGLE_90, D3DXVECTOR3(400.0f, 0.0f, 450.0f), CGimmick::TYPE_BRIDGE, 3);

	// 橋：奥
	CGimmick::Create(D3DXVECTOR3(8700.0f, 500.0f, 625.0f), EAngle::ANGLE_90, D3DXVECTOR3(550.0f, 0.0f, 350.0f), CGimmick::TYPE_BRIDGE, 4);
	CGimmick::Create(D3DXVECTOR3(10550.0f, 725.0f, -550.0f), EAngle::ANGLE_90, D3DXVECTOR3(300.0f, 0.0f, 400.0f), CGimmick::TYPE_BRIDGE, 3);
	CGimmick::Create(D3DXVECTOR3(12075.0f, 550.0f, -200.0f), EAngle::ANGLE_0, D3DXVECTOR3(400.0f, 0.0f, 350.0f), CGimmick::TYPE_BRIDGE, 3);
#endif

// わんわんおー
#if 0
	CEnemyWolf::Create(D3DXVECTOR3(0.0f, 2000.0f, 300.0f), VEC3_ZERO, CEnemyAttack::TYPE_WOLF, 400.0f, 400.0f, 600.0, 500.0f);
	CEnemyWolf::Create(D3DXVECTOR3(0.0f, 2000.0f, -300.0f), VEC3_ZERO, CEnemyAttack::TYPE_WOLF, 400.0f, 400.0f, 600.0, 500.0f);
#endif

// さむらい
#if 1
	//CEnemyStalk::Create(D3DXVECTOR3(300.0f, 0.0f, 400.0f), VEC3_ZERO, CEnemyAttack::TYPE_STALK, 400.0f, 400.0f, 600.0, 500.0f);
	CEnemyStalk::Create(D3DXVECTOR3(700.0f, 0.0f, -60.0f), VEC3_ZERO, CEnemyAttack::TYPE_STALK, 400.0f, 400.0f, 600.0, 500.0f);

	// 勝手に追加してごめぇんね(チュートリアルマップの敵)
	CEnemyStalk::Create(D3DXVECTOR3(12950.0f, 650.0f, 100.0f), VEC3_ZERO, CEnemyAttack::TYPE_STALK, 400.0f, 400.0f, 600.0, 500.0f);

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
			{
				TransitionResult(CRetentionManager::EWin::WIN_CLEAR);
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

	// カメラ揺れの設定
	GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, CLEAR_SWING);

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
