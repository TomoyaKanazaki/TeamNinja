//============================================================
//
//	タイトルマネージャーヘッダー [titleManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TITLEMANAGER_H_
#define _TITLEMANAGER_H_

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// タイトルマネージャークラス
class CTitleManager
{
public:
	// ロゴ列挙
	//enum ELogo
	//{
	//	LOGO_NEVER = 0,	// NEVERロゴ
	//	LOGO_GIVE,		// GIVEロゴ
	//	LOGO_UP,		// UP!ロゴ
	//	LOGO_MAX		// この列挙型の総数
	//};

	// スタート状態列挙
	enum EStartState
	{
		STARTSTATE_NONE = 0,	// 何もしない状態
		STARTSTATE_APPEAR,		// 出現状態
		STARTSTATE_DISAPPEAR,	// 消滅状態
		STARTSTATE_SCALE,		// 拡大状態
		STARTSTATE_MAX			// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_FADEOUT,	// フェードアウト状態
		STATE_SCALE,	// タイトル縮小状態
		STATE_MOVE,		// タイトル移動状態
		STATE_WAIT,		// 遷移待機状態
		STATE_TRANS,	// 遷移状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CTitleManager();

	// デストラクタ
	~CTitleManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(const float fDeltaTime);	// 更新

	// 静的メンバ関数
	static CTitleManager *Create(void);	// 生成
	static void Release(CTitleManager *&prTitleManager);	// 破棄

private:
	// メンバ関数
	void UpdateFade(void);	// フェードアウト
	void UpdateScale(void);	// タイトル拡大
	void UpdateMove(void);	// タイトル移動
	void UpdateStart(void);	// 遷移決定
	void UpdateBlink(void);	// スタート点滅
	void UpdateTrans(void);	// 遷移
	void SkipStaging(void);	// 演出スキップ

	// メンバ変数
	CObject2D *m_pLogo;		// タイトル表示の情報
	CObject2D *m_pFade;		// フェードの情報
	CObject2D* m_pStart;	// スタートの情報
	EState m_state;		// 状態
	EStartState m_startState;	// スタートの状態
	float m_fScale;		// タイトル拡大率
	float m_fMove;		// タイトルロゴの移動量
	int m_nSelect;		// 現在の選択
	int m_nOldSelect;	// 前回の選択
	int m_nWaitCounter;	// タイトル移動までの待機時間
};

#endif	// _TITLEMANAGER_H_
