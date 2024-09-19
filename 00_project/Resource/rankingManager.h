//============================================================
//
//	ランキングマネージャーヘッダー [rankingManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RANKING_MANAGER_H_
#define _RANKING_MANAGER_H_

//************************************************************
//	前方宣言
//************************************************************
class CTransPoint;	// 遷移ポイントクラス
class CObject2D;	// オブジェクト2Dクラス
class CString2D;	// 文字列2Dクラス
class CTimeUI;		// タイムUIクラス
class CAnim2D;		// アニメーション2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// ランキングマネージャークラス
class CRankingManager
{
public:
	// 定数
	static constexpr int MAX_RANK = 3;	// ランキング上位表示数

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_FADEOUT,	// フェードアウト状態
		STATE_SPAWN,	// UI出現状態
		STATE_WAIT,		// 待機状態
		STATE_FADEIN,	// フェードイン状態
		STATE_FALL,		// UI消失状態
		STATE_END,		// 終了状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CRankingManager(CTransPoint* pParent);

	// デストラクタ
	~CRankingManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(const float fDeltaTime);	// 更新
	void SetState(const EState state)		{ m_state = state; }	// 状態設定
	EState GetState(void) const				{ return m_state; }		// 状態取得
	bool IsEnd(void) const	{ return (m_state == STATE_END); }		// 演出終了フラグ取得

	// 静的メンバ関数
	static CRankingManager *Create(CTransPoint* pParent);		// 生成
	static void Release(CRankingManager *&prRankingManager);	// 破棄
	static int SetRank(const float fNewTime);	// ランキング設定

private:
	// 選択列挙
	enum ESelect
	{
		SELECT_YES = 0,	// はい
		SELECT_NO,		// いいえ
		SELECT_MAX		// この列挙型の総数
	};

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CRankingManager::*AFuncUpdateState)(const float fDeltaTime);

	// 静的メンバ関数
	static int SortRank(const float fNewTime, float* pRankArray);	// ランキングソート
	static HRESULT LoadRank(const char* pPath, float* pRankArray);	// ランキング読込
	static HRESULT SaveRank(const char* pPath, float* pRankArray);	// ランキング保存

	// メンバ関数
	void UpdateFadeOut(const float fDeltaTime);	// フェードアウト更新
	void UpdateSpawn(const float fDeltaTime);	// UI出現更新
	void UpdateWait(const float fDeltaTime);	// 待機更新
	void UpdateFadeIn(const float fDeltaTime);	// フェードイン更新
	void UpdateFall(const float fDeltaTime);	// UI消失更新
	void UpdateEnd(const float fDeltaTime);		// 終了更新
	void UpdateSkip(void);	// スキップ操作更新
	void SkipStaging(void);	// 演出スキップ

	// 静的メンバ変数
	static AFuncUpdateState m_aFuncUpdateState[];	// 状態更新関数

	// メンバ変数
	CAnim2D* m_apRankValue[MAX_RANK];	// ランキング順位情報
	CTimeUI* m_apRankTime[MAX_RANK];	// ランキング時間情報
	CString2D* m_apSelect[SELECT_MAX];	// 選択肢情報
	CObject2D* m_pFade;		// フェード情報
	CObject2D* m_pTitle;	// タイトル情報
	CObject2D* m_pBalloon;	// 吹き出し情報
	CString2D* m_pShadow;	// ランキングタイトルの影情報
	CString2D* m_pName;		// ランキングタイトル情報
	CTransPoint* m_pParent;	// 遷移ポイント情報
	EState m_state;			// 状態
	float m_fCurTime;		// 現在の待機時間
	int m_nCurSelect;		// 現在の選択肢
	int m_nOldSelect;		// 前回の選択肢
};

#endif	// _RANKING_MANAGER_H_
