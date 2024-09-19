//============================================================
//
//	データ保存マネージャーヘッダー [retentionManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RETENTION_MANAGER_H_
#define _RETENTION_MANAGER_H_

//************************************************************
//	クラス定義
//************************************************************
// データ保存マネージャークラス
class CRetentionManager
{
public:
	// 勝利列挙
	enum EWin
	{
		WIN_NONE,		// 無し
		WIN_FAIL,		// 勝利失敗
		WIN_SUCCESS,	// 勝利成功
		WIN_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CRetentionManager();

	// デストラクタ
	~CRetentionManager();

	// リザルト情報構造体
	struct SResult
	{
		EWin win;		// 勝利状況
		float fTime;	// 経過タイム
		int nSave;		// 最後のセーブポイント
	};

	// ランキング更新情報構造体
	struct SUpdateRank
	{
		std::string sTransPath;	// 更新した遷移パス
		int nRank;				// 更新した順位
	};

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了

	// 静的メンバ関数
	static CRetentionManager *Create(void);	// 生成
	static void Release(CRetentionManager *&prRetentionManager);	// 破棄

	// メンバ関数
	void SetResult(const EWin win, const float fTime, const int nSave);	// リザルト情報設定
	void InitUpdateRank(void);											// ランキング更新情報初期化
	void SetUpdateRank(const char* pTransPath, const int nRank);		// ランキング更新情報設定
	SUpdateRank GetUpdateRank(void) const	{ return m_updateRank; }	// ランキング更新情報取得
	void SetTransIdx(const int nIdx)		{ m_nTransIdx = nIdx; }		// 遷移ポイントインデックス設定
	int GetTransIdx(void) const				{ return m_nTransIdx; }		// 遷移ポイントインデックス取得
	EWin GetWin(void) const		{ return m_result.win; }		// 勝利状況取得
	void InitWin(void)			{ m_result.win = WIN_NONE; }	// 勝利状況初期化
	float GetTime(void) const	{ return m_result.fTime; }		// 経過タイム取得
	int GetSave(void) const		{ return m_result.nSave; }		// セーブポイント取得
	void InitSave(void)			{ m_result.nSave = NONE_IDX; }	// セーブポイント初期化

	void SetEnableSaveStart(void)	{ m_bSaveStart = (m_result.nSave != NONE_IDX); }	// セーブからの開始フラグ設定
	bool IsSaveStart(void) const	{ return m_bSaveStart; }	// セーブからの開始フラグ取得

private:
	
	// メンバ変数
	SResult m_result;			// リザルト情報
	SUpdateRank m_updateRank;	// ランキング更新情報
	int m_nTransIdx;			// 遷移ポイントインデックス
	bool m_bSaveStart;			// セーブからの開始フラグ

};

#endif	// _RETENTION_MANAGER_H_
