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
//	インクルードファイル
//************************************************************
#include "stage.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;		// オブジェクト2Dクラス
class CAnim2D;			// アニメーション2Dクラス
class CString2D;		// 文字列2Dクラス
class CScrollText2D;	// 文字送りテキスト2Dクラス
class CTimeUI;			// タイムUIクラス

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
		STATE_CLEAR_STAMP,		// クリアハンコ押し状態
		STATE_TIME_TITLE_WAIT,	// 遂行時間タイトル待機状態
		STATE_TIME_TITLE,		// 遂行時間タイトル表示状態
		STATE_TIME_VALUE_WAIT,	// 遂行時間待機状態
		STATE_TIME_VALUE,		// 遂行時間表示状態
		STATE_ITEM_TITLE_WAIT,	// 神器タイトル待機状態
		STATE_ITEM_TITLE,		// 神器タイトル表示状態
		STATE_ITEM_BG_WAIT,		// 神器アイコン背景待機状態
		STATE_ITEM_BG,			// 神器アイコン背景表示状態
		STATE_ITEM_ICON_WAIT,	// 神器アイコン待機状態
		STATE_ITEM_ICON,		// 神器アイコン表示状態
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
	void UpdateStamp(const float fDeltaTime);			// クリアハンコ押し更新
	void UpdateTimeTitleWait(const float fDeltaTime);	// 遂行時間タイトル待機更新
	void UpdateTimeTitle(const float fDeltaTime);		// 遂行時間タイトル表示更新
	void UpdateTimeValueWait(const float fDeltaTime);	// 遂行時間待機更新
	void UpdateTimeValue(const float fDeltaTime);		// 遂行時間表示更新
	void UpdateItemTitleWait(const float fDeltaTime);	// 神器タイトル待機更新
	void UpdateItemTitle(const float fDeltaTime);		// 神器タイトル表示更新
	void UpdateItemIconBgWait(const float fDeltaTime);	// 神器アイコン背景待機更新
	void UpdateItemIconBg(const float fDeltaTime);		// 神器アイコン背景表示更新
	void UpdateItemIconWait(const float fDeltaTime);	// 神器アイコン待機更新
	void UpdateItemIcon(const float fDeltaTime);		// 神器アイコン表示更新
	void UpdateWait(const float fDeltaTime);			// 待機更新
	void UpdateFadeOut(const float fDeltaTime);			// フェードアウト更新
	void UpdateFadeOutWait(const float fDeltaTime);		// フェードアウト待機更新
	void UpdateFadeOutAccel(const float fDeltaTime);	// フェードアウト加速更新
	void UpdateEnd(const float fDeltaTime);				// 終了更新

	// メンバ変数
	CAnim2D* m_apGodItemBG[CStage::GOD_MAX];	// 神器アイコン背景情報
	CAnim2D* m_apGodItemIcon[CStage::GOD_MAX];	// 神器アイコン情報
	CObject2D* m_pFade;			// フェード情報
	CScrollText2D* m_pTitle;	// タイトル情報
	CObject2D* m_pStamp;		// ハンコ情報
	CString2D* m_pTime;			// 遂行時間タイトル情報
	CTimeUI* m_pTimeVal;		// 遂行時間情報
	CString2D* m_pGodItem;		// 神器タイトル情報
	EState m_state;				// 状態
	float m_fMoveY;				// 縦移動量
	float m_fCurTime;			// 現在の待機時間
};

#endif	// _RESULTMANAGER_H_
