//============================================================
//
//	リザルトマネージャーヘッダー [resultManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// リザルトマネージャークラス
class CResultManager
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,			// 何もしない状態
		STATE_FADEWAIT,			// フェード待機状態
		STATE_FADEIN,			// フェードイン状態
		STATE_FADEIN_WAIT,		// フェードイン待機状態
		STATE_FADEIN_ACCEL,		// フェードイン加速状態
		STATE_WAIT,				// 待機状態
		STATE_FADEOUT,			// フェードアウト状態
		STATE_FADEOUT_WAIT,		// フェードアウト待機状態
		STATE_FADEOUT_ACCEL,	// フェードアウト加速状態
		STATE_END,				// 終了状態
		STATE_MAX				// この列挙型の総数
	};

	// コンストラクタ
	CResultManager();

	// デストラクタ
	~CResultManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(const float fDeltaTime);	// 更新
	void SetState(const EState state)	{ m_state = state; }	// 状態設定
	EState GetState(void) const			{ return m_state; }		// 状態取得

	// 静的メンバ関数
	static CResultManager *Create(void);	// 生成
	static void Release(CResultManager *&prResultManager);	// 破棄

private:
	// メンバ関数
	void UpdateFadeWait(const float fDeltaTime);		// フェード待機更新
	void UpdateFadeIn(const float fDeltaTime);			// フェードイン更新
	void UpdateFadeInWait(const float fDeltaTime);		// フェードイン待機更新
	void UpdateFadeInAccel(const float fDeltaTime);		// フェードイン加速更新
	void UpdateWait(const float fDeltaTime);			// 待機更新
	void UpdateFadeOut(const float fDeltaTime);			// フェードアウト更新
	void UpdateFadeOutWait(const float fDeltaTime);		// フェードアウト待機更新
	void UpdateFadeOutAccel(const float fDeltaTime);	// フェードアウト加速更新

	// メンバ変数
	CObject2D *m_pFade;	// フェード情報
	EState m_state;		// 状態
	float m_fMoveY;		// 縦移動量
	float m_nCurTime;	// 現在の待機時間
};

#endif	// _RESULTMANAGER_H_
