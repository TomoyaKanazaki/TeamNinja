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
	m_state	(STATE_NONE),	// 状態
	m_pGoal	(nullptr)		// ゴールのポインタ
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
	m_pGoal = nullptr;		// ゴールのポインタ

	// スタートUIを生成
	CPopUpUI::Create(START_TEXTURE);

	CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, 400.0f), VEC3_ZERO, CEnemy::TYPE_STALK);
	CEnemy::Create(D3DXVECTOR3(-600.0f, 0.0f, -500.0f), VEC3_ZERO, CEnemy::TYPE_CAVEAT);

	CGimmick::Create(D3DXVECTOR3(400.0f, 0.0f, -500.0f), D3DXVECTOR3(200.0f, 0.0f, 50.0f), CGimmick::TYPE_DECAED, 2);
	CGimmick::Create(D3DXVECTOR3(400.0f, 0.0f, -300.0f), D3DXVECTOR3(200.0f, 0.0f, 50.0f), CGimmick::TYPE_FALL, 2);
	CGimmick::Create(D3DXVECTOR3(-400.0f, 0.0f, -500.0f), D3DXVECTOR3(200.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 2);

	CGimmick::Create(D3DXVECTOR3(-400.0f, 0.0f, -700.0f), D3DXVECTOR3(200.0f, 0.0f, 50.0f), CGimmick::TYPE_BOOB, 2);
	CGimmick::Create(D3DXVECTOR3(-600.0f, 0.0f, -700.0f), D3DXVECTOR3(200.0f, 0.0f, 50.0f), CGimmick::TYPE_GRAVEL, 2);
	CGimmick::Create(D3DXVECTOR3(-800.0f, 0.0f, -700.0f), D3DXVECTOR3(200.0f, 0.0f, 50.0f), CGimmick::TYPE_WATER, 2);

	CGimmick::Create(D3DXVECTOR3(-1000.0f, 0.0f, -500.0f), D3DXVECTOR3(100.0f, 0.0f, 50.0f), CGimmick::TYPE_JUMPOFF, 2);
	CGimmick::Create(D3DXVECTOR3(-1000.0f, 0.0f, -200.0f), D3DXVECTOR3(200.0f, 0.0f, 100.0f), CGimmick::TYPE_HEAVYDOOR, 6);

	// アクターを生成
	CActor::Create(CActor::TYPE_ROCK_S, VEC3_ZERO);

	CMapModel::Create(D3DXVECTOR3(-800.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE1);

	CMapModel::Create(D3DXVECTOR3(-400.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE2);

	CMapModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE3);

	CMapModel::Create(D3DXVECTOR3(400.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE4);

	CMapModel::Create(D3DXVECTOR3(800.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE5);

	CMapModel::Create(D3DXVECTOR3(100.0f, 0.0f, -100.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_LANTERN1);

	// マップを生成
	if (FAILED(MapLoad())) { return E_FAIL; }

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGameManager::Uninit(void)
{
	SAFE_UNINIT(m_pGoal);
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
		if (m_pGoal != nullptr)
		{
			if (m_pGoal->GetClear())
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

		// UIを表示
		CPopUpUI::Create();
	}
	else
	{
		// UIを表示
		CPopUpUI::Create();
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

//==========================================
//  マップの生成
//==========================================
HRESULT CGameManager::MapLoad()
{
	//ローカル変数宣言
	FILE* pFile; // ファイルポインタ

	//ファイルを読み取り専用で開く
	pFile = fopen(MAP_TXT, "r");

	// ファイルが開けなかった場合
	if (pFile == NULL) { assert(false); return E_FAIL; }

	// 情報の読み込み
	while (1)
	{
		// 文字列の記録用
		char aStr[256];

		// 文字列読み込み
		fscanf(pFile, "%s", &aStr[0]);

		// 条件分岐
		if (strcmp(&aStr[0], "CHECKPOINT") == 0) // チェックポイントの生成
		{
			// データの取得用変数
			D3DXVECTOR3 pos, rot;

			// 文字列読み込み (POS)
			fscanf(pFile, "%s", &aStr[0]);

			// データ取得
			fscanf(pFile, "%f", &pos.x);
			fscanf(pFile, "%f", &pos.y);
			fscanf(pFile, "%f", &pos.z);

			// 文字列読み込み (ROT)
			fscanf(pFile, "%s", &aStr[0]);

			// データ取得
			fscanf(pFile, "%f", &rot.x);
			fscanf(pFile, "%f", &rot.y);
			fscanf(pFile, "%f", &rot.z);

			// チェックポイントを生成
			CCheckPoint::Create(pos);
		}
		if (strcmp(&aStr[0], "GOAL") == 0) // チェックポイントの生成
		{
			// データの取得用変数
			D3DXVECTOR3 pos, rot;

			// 文字列読み込み (POS)
			fscanf(pFile, "%s", &aStr[0]);

			// データ取得
			fscanf(pFile, "%f", &pos.x);
			fscanf(pFile, "%f", &pos.y);
			fscanf(pFile, "%f", &pos.z);

			// 文字列読み込み (ROT)
			fscanf(pFile, "%s", &aStr[0]);

			// データ取得
			fscanf(pFile, "%f", &rot.x);
			fscanf(pFile, "%f", &rot.y);
			fscanf(pFile, "%f", &rot.z);

			// チェックポイントを生成
			m_pGoal = CGoal::Create(pos, rot);
		}
		if (strcmp(&aStr[0], "END_OF_FILE") == 0) // 読み込み終了
		{
			break;
		}
	}

	return S_OK;
}
