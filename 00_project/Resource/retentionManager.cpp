//============================================================
//
//	データ保存マネージャー処理 [retentionManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "retentionManager.h"
#include "manager.h"
#include "stage.h"
#include "godItem.h"
#include "transpoint.h"

//************************************************************
//	親クラス [CRetentionManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CRetentionManager::CRetentionManager()
{
	// メンバ変数をクリア
	memset(&m_result, 0, sizeof(m_result));	// リザルト情報
}

//============================================================
//	デストラクタ
//============================================================
CRetentionManager::~CRetentionManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CRetentionManager::Init(void)
{
	// メンバ変数を初期化
	m_result.win	= WIN_NONE;	// 勝利状況
	m_result.fTime = 0.0f;		// 経過タイム
	m_result.nSave = -1;		// 最後のセーブポイント

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CRetentionManager::Uninit(void)
{

}

//============================================================
//	生成処理
//============================================================
CRetentionManager *CRetentionManager::Create(void)
{
	// データ保存マネージャーの生成
	CRetentionManager *pRetentionManager = new CRetentionManager;
	if (pRetentionManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// データ保存マネージャーの初期化
		if (FAILED(pRetentionManager->Init()))
		{ // 初期化に失敗した場合

			// データ保存マネージャーの破棄
			SAFE_DELETE(pRetentionManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pRetentionManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CRetentionManager::Release(CRetentionManager *&prRetentionManager)
{
	// データ保存マネージャーの終了
	assert(prRetentionManager != nullptr);
	prRetentionManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prRetentionManager);
}

//============================================================
//	リザルト情報の設定処理
//============================================================
void CRetentionManager::SetResult(const EWin win, const float fTime, const int nSave)
{
	// 勝利が正規値ではない場合抜ける
	if (win <= WIN_NONE || win >= WIN_MAX) { assert(false); return; }

	// 引数のクリア状況を設定
	m_result.win = win;

	// 引数の経過タイムを設定
	m_result.fTime = fTime;

	// 引数のセーブポイントを設定
	m_result.nSave = nSave;

	if (win == WIN_SUCCESS)
	{ // ステージクリアした場合勾玉の獲得状況を更新

		// 前回までの勾玉の獲得状況を読み込み
		bool bOldGet[CGodItem::TYPE_MAX] = {};	// 保存されている獲得状況
		CGodItem::LoadPossess(GET_STAGE->GetCurMapSaveGodItemPass().c_str(), &bOldGet[0]);

		// 今回の勾玉の獲得状況を適応
		bool bCurGet[CGodItem::TYPE_MAX] = {};	// 今回の獲得状況
		for (int i = 0; i < CGodItem::TYPE_MAX; i++)
		{
			// 今回の獲得状況を割り当て
			bCurGet[i] = (CGodItem::IsGet((CGodItem::EType)i) || bOldGet[i]);
		}

		// 勾玉の獲得状況を書き出し
		CGodItem::SavePossess(GET_STAGE->GetCurMapSaveGodItemPass().c_str(), &bCurGet[0]);

		// 遷移先の解放フラグテキストパスを作成
		std::filesystem::path fsPath(GET_STAGE->GetOpenMapDirectory());	// 遷移先マップパス
		fsPath.append("open.txt");										// ディレクトリに解放フラグのベースネーム追加

		// 解放状況の書き出し
		CTransPoint::SaveOpen(fsPath.string().c_str(), true);
	}
}
