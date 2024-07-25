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
class CClearManager;	// クリアマネージャークラス
class CObject2D;		// オブジェクト2Dクラス
class CScrollText2D;	// 文字送りテキスト2Dクラス

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
		STATE_FADE_WAIT,		// フェード待機状態
		STATE_FADEIN,			// フェードイン状態
		STATE_FADEIN_WAIT,		// フェードイン待機状態
		STATE_FADEIN_ACCEL,		// フェードイン加速状態
		STATE_STAGE_WAIT,		// ステージ表示待機状態
		STATE_STAGE_TITLE,		// ステージタイトル状態
		STATE_STAMP,			// ハンコ押し状態
		STATE_CLEAR,			// クリアマネージャー更新状態
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
	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CResultManager::*AFuncUpdateState)(const float fDeltaTime);

	// 静的メンバ変数
	static AFuncUpdateState m_aFuncUpdateState[];	// 状態更新関数

	// メンバ関数
	void UpdateFadeWait(const float fDeltaTime);		// フェード待機更新
	void UpdateFadeIn(const float fDeltaTime);			// フェードイン更新
	void UpdateFadeInWait(const float fDeltaTime);		// フェードイン待機更新
	void UpdateFadeInAccel(const float fDeltaTime);		// フェードイン加速更新
	void UpdateStageWait(const float fDeltaTime);		// ステージ表示待機更新
	void UpdateStageTitle(const float fDeltaTime);		// ステージタイトル更新
	void UpdateStamp(const float fDeltaTime);			// ハンコ押し更新
	void UpdateClear(const float fDeltaTime);			// クリアマネージャー更新
	void UpdateFadeOut(const float fDeltaTime);			// フェードアウト更新
	void UpdateFadeOutWait(const float fDeltaTime);		// フェードアウト待機更新
	void UpdateFadeOutAccel(const float fDeltaTime);	// フェードアウト加速更新
	void UpdateEnd(const float fDeltaTime);				// 終了更新
	void SetAllMove(const D3DXVECTOR3& rMove);	// 全UIオブジェクト移動
	void SkipStaging(void);	// 演出スキップ

	// メンバ変数
	CClearManager* m_pClear;	// クリアマネージャー情報
	CObject2D* m_pFade;			// フェード情報
	CScrollText2D* m_pTitle;	// タイトル情報
	CObject2D* m_pStamp;		// ハンコ情報
	EState m_state;				// 状態
	float m_fMoveY;				// 縦移動量
	float m_fCurTime;			// 現在の待機時間
};

#endif	// _RESULTMANAGER_H_
