//============================================================
//
//	クリアマネージャー処理 [clearManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "clearManager.h"
#include "clearFailManager.h"
#include "clearSuccessManager.h"

//************************************************************
//	親クラス [CClearManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CClearManager::CClearManager()
{

}

//============================================================
//	デストラクタ
//============================================================
CClearManager::~CClearManager()
{

}

//============================================================
//	生成処理
//============================================================
CClearManager *CClearManager::Create(const CRetentionManager::EWin win)
{
	// クリアマネージャーの生成
	CClearManager *pClearManager = nullptr;	// クリアマネージャー情報
	switch (win)
	{ // モードごとの処理
	case CRetentionManager::WIN_FAIL:
		pClearManager = new CClearFailManager;
		break;

	case CRetentionManager::WIN_SUCCESS:
		pClearManager = new CClearSuccessManager;
		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	if (pClearManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// クリアマネージャーの初期化
		if (FAILED(pClearManager->Init()))
		{ // 初期化に失敗した場合

			// クリアマネージャーの破棄
			SAFE_DELETE(pClearManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pClearManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CClearManager::Release(CClearManager *&prClearManager)
{
	// クリアマネージャーの終了
	assert(prClearManager != nullptr);
	prClearManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prClearManager);
}
