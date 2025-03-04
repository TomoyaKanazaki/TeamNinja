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
#include "rankingManager.h"
#include "sceneGame.h"

//************************************************************
//	親クラス [CRetentionManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CRetentionManager::CRetentionManager() :
	m_nTransIdx		(NONE_IDX),	// 遷移ポイントインデックス
	m_bSaveStart	(false)		// セーブからの開始フラグ
{

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
	m_result.fTime	= 0.0f;		// 経過タイム
	m_result.nSave	= -1;		// 最後のセーブポイント
	m_nTransIdx		= NONE_IDX;	// 遷移ポイントインデックス
	m_bSaveStart	= false;	// セーブからの開始フラグ
	m_updateRank.sTransPath	= "";		// 更新した遷移パス
	m_updateRank.nRank		= NONE_IDX;	// 更新した順位

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

		if (!GET_RETENTION->IsSaveStart())
		{ // セーブからの開始ではない場合ランキングを更新

			// ランキングの設定
			const int nUpdateRank = CRankingManager::SetRank(CSceneGame::TIME_LIMIT - fTime);	// 更新された順位を保存

			// ランキングの更新情報を保存
			SetUpdateRank(GET_STAGE->GetCurMapPass().c_str(), nUpdateRank);
		}

		if (!GET_STAGE->GetOpenMapDirectory().empty())
		{ // パスが指定されている場合

			// 遷移先の解放フラグテキストパスを作成
			std::filesystem::path fsPath(GET_STAGE->GetOpenMapDirectory());	// 遷移先マップパス
			fsPath.append("open.txt");										// ディレクトリに解放フラグのベースネーム追加

			// 解放状況の書き出し
			CTransPoint::SaveOpen(fsPath.string().c_str(), true);
		}
	}
}

//============================================================
//	ランキング更新情報の初期化処理
//============================================================
void CRetentionManager::InitUpdateRank(void)
{
	// 更新した遷移パスを初期化
	m_updateRank.sTransPath = "";

	// 更新した順位を初期化
	m_updateRank.nRank = NONE_IDX;
}

//============================================================
//	ランキング更新情報の設定処理
//============================================================
void CRetentionManager::SetUpdateRank(const char* pTransPath, const int nRank)
{
	// 更新した遷移パスを保存
	m_updateRank.sTransPath = pTransPath;

	// 更新した順位を保存
	m_updateRank.nRank = nRank;
}
