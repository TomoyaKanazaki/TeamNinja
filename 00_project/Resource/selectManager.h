//============================================================
//
//	セレクトマネージャーヘッダー [selectManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SELECTMANAGER_H_
#define _SELECTMANAGER_H_

//************************************************************
//	前方宣言
//************************************************************
class CRankingManager;	// ランキングマネージャークラス
class CTransPoint;		// 遷移ポイントクラス

//************************************************************
//	クラス定義
//************************************************************
// セレクトマネージャークラス
class CSelectManager
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_NORMAL,	// 通常状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CSelectManager();

	// デストラクタ
	~CSelectManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(const float fDeltaTime);		// 更新
	void SetState(const EState state);			// 状態設定
	EState GetState(void) const;				// 状態取得
	HRESULT SetDispRanking(CTransPoint* pHit);	// ランキング表示設定

	// 静的メンバ関数
	static CSelectManager *Create(void);	// 生成
	static void Release(CSelectManager *&prSelectManager);	// 破棄

private:
	// メンバ変数
	CRankingManager* m_pRanking;	// ランキングマネージャー
	EState m_state;	// 状態
};

#endif	// _SELECTMANAGER_H_
