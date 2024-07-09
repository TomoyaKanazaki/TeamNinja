//============================================================
//
//	シーン処理 [scene.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "camera.h"
#include "effekseerManager.h"
#include "sceneTitle.h"
#include "sceneTutorial.h"
#include "sceneGame.h"
#include "sceneResult.h"
#include "sceneRanking.h"
#include "collManager.h"
#include "player.h"
#include "stage.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char *SETUP_STAGE = "data\\TXT\\MAP\\FOREST00\\stage.txt";		// セットアップテキスト相対パス
	const char *SETUP_ACTOR = "data\\TXT\\MAP\\FOREST00\\actor.txt";	// セットアップテキスト相対パス
	const char *SETUP_POINT = "data\\TXT\\MAP\\FOREST00\\point.txt";	// セットアップテキスト相対パス
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CCollManager* CScene::m_pCollManager = nullptr;	// 当たり判定マネージャー

//************************************************************
//	親クラス [CScene] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScene::CScene(const EMode mode) : m_mode(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CScene::~CScene()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CScene::Init(void)
{
	// 当たり判定の生成
	m_pCollManager = CCollManager::Create();
	if (m_pCollManager == nullptr)
	{ // 生成に生成した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// プレイヤーの生成
	CPlayer::Create(m_mode);

	// ステージの割当
	GET_STAGE->BindStage("data\\TXT\\MAP\\FOREST00\\map.txt");	// TODO：今だけ確定で初期マップ読込

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CScene::Uninit(void)
{
	// 当たり判定の終了
	SAFE_UNINIT(m_pCollManager);
}

//============================================================
//	更新処理
//============================================================
void CScene::Update(const float fDeltaTime)
{
	// ポインタを宣言
	CManager			*pManager	= GET_MANAGER;				// マネージャー
	CLight				*pLight		= pManager->GetLight();		// ライト
	CCamera				*pCamera	= pManager->GetCamera();	// カメラ
	CRenderer			*pRenderer	= pManager->GetRenderer();	// レンダラー
	CEffekseerManager	*pEffekseer	= pManager->GetEffekseer();	// エフェクシア

	// ライトの更新
	assert(pLight != nullptr);
	pLight->Update(fDeltaTime);

	// カメラの更新
	assert(pCamera != nullptr);
	pCamera->Update(fDeltaTime);

	// レンダラーの更新
	assert(pRenderer != nullptr);
	pRenderer->Update(fDeltaTime);

	// エフェクシアの更新
	assert(pEffekseer != nullptr);
	pEffekseer->Update(fDeltaTime);
}

//============================================================
//	生成処理
//============================================================
CScene *CScene::Create(EMode mode)
{
	// シーンの生成
	CScene *pScene = nullptr;	// シーン情報
	switch (mode)
	{ // モードごとの処理
	case MODE_TITLE:
		pScene = new CSceneTitle(mode);
		break;

	case MODE_TUTORIAL:
		pScene = new CSceneTutorial(mode);
		break;

	case MODE_GAME:
		pScene = new CSceneGame(mode);
		break;

	case MODE_RESULT:
		pScene = new CSceneResult(mode);
		break;

	case MODE_RANKING:
		pScene = new CSceneRanking(mode);
		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 確保したアドレスを返す
	assert(pScene != nullptr);
	return pScene;
}

//============================================================
//	破棄処理
//============================================================
void CScene::Release(CScene *&prScene)
{
	// シーンの終了
	assert(prScene != nullptr);
	prScene->Uninit();

	// メモリ開放
	SAFE_DELETE(prScene);
}

//============================================================
//	当たり判定マネージャー取得
//============================================================
CCollManager *CScene::GetCollManager(void)
{
	// インスタンス未使用
	assert(m_pCollManager != nullptr);

	// 当たり判定マネージャーのポインタを返す
	return m_pCollManager;
}

//============================================================
//	プレイヤー取得処理
//============================================================
CPlayer *CScene::GetPlayer(void)
{
	CListManager<CPlayer> *pListManager = CPlayer::GetList();				// プレイヤーリストマネージャー
	if (pListManager == nullptr)		{ return nullptr; }					// リスト未使用の場合抜ける
	if (pListManager->GetNumAll() != 1)	{ assert(false); return nullptr; }	// プレイヤーが1人ではない場合抜ける
	CPlayer *pPlayer = pListManager->GetList().front();						// プレイヤーの情報

	// プレイヤーのポインタを返す
	return pPlayer;
}

//============================================================
//	モードの設定処理
//============================================================
void CScene::SetMode(const EMode mode)
{
	// 引数のモードを設定
	m_mode = mode;
}

//============================================================
//	モード取得処理
//============================================================
CScene::EMode CScene::GetMode(void) const
{
	// 現在のモードを返す
	return m_mode;
}
