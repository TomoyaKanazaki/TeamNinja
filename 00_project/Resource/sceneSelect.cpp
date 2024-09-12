//============================================================
//
//	セレクト画面処理 [sceneSelect.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneSelect.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "stage.h"
#include "selectManager.h"
#include "editManager.h"
#include "retentionManager.h"
#include "godItem.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* INIT_MAPPASS = "data\\MAP\\SELECT00\\map.txt";	// 初期マップパス
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CSelectManager	*CSceneSelect::m_pSelectManager	= nullptr;	// セレクトマネージャー
CEditManager	*CSceneSelect::m_pEditManager	= nullptr;	// エディットマネージャー

//************************************************************
//	子クラス [CSceneSelect] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneSelect::CSceneSelect(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneSelect::~CSceneSelect()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneSelect::Init(void)
{
	// 生成マップパスをステージ選択マップに変更
	GET_STAGE->SetInitMapPass(INIT_MAPPASS);

	// シーンの初期化
	if (FAILED(CScene::Init()))	// 当たり判定/プレイヤー生成
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// セーブポイントの初期化
	GET_RETENTION->InitSave();

	// 神器取得状況の初期化
	CGodItem::InitGet();

	// セレクトマネージャーの生成
	m_pSelectManager = CSelectManager::Create();
	if (m_pSelectManager == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

#if _DEBUG

	// エディットマネージャーの生成
	m_pEditManager = CEditManager::Create();
	if (m_pEditManager == nullptr)
	{ // 生成に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

#endif	// _DEBUG

	// BGMの再生
	PLAY_SOUND(CSound::LABEL_BGM_SELECT);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSceneSelect::Uninit(void)
{
	// セレクトマネージャーの破棄
	SAFE_REF_RELEASE(m_pSelectManager);

#if _DEBUG

	// エディットマネージャーの破棄
	SAFE_REF_RELEASE(m_pEditManager);

#endif	// _DEBUG

	// シーンの終了
	CScene::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSceneSelect::Update(const float fDeltaTime)
{
	// セレクトマネージャーの更新
	assert(m_pSelectManager != nullptr);
	m_pSelectManager->Update(fDeltaTime);

	// シーンの更新
	CScene::Update(fDeltaTime);

#ifdef _DEBUG

	// エディットマネージャーの更新
	assert(m_pEditManager != nullptr);
	m_pEditManager->Update();

#endif	// _DEBUG
}

//============================================================
//	セレクトマネージャー取得処理
//============================================================
CSelectManager *CSceneSelect::GetSelectManager(void)
{
	// インスタンス未使用
	assert(m_pSelectManager != nullptr);

	// セレクトマネージャーのポインタを返す
	return m_pSelectManager;
}

//============================================================
//	エディターマネージャー取得処理
//============================================================
CEditManager *CSceneSelect::GetEditManager(void)
{
	// インスタンス未使用
	assert(m_pEditManager != nullptr);

	// エディターマネージャーのポインタを返す
	return m_pEditManager;
}
