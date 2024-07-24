//============================================================
//
//	クリア成功マネージャーヘッダー [clearSuccessManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _CLEAR_SUCCESS_MANAGER_H_
#define _CLEAR_SUCCESS_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "clearManager.h"
#include "stage.h"

//************************************************************
//	前方宣言
//************************************************************
class CAnim2D;			// アニメーション2Dクラス
class CString2D;		// 文字列2Dクラス
class CScrollText2D;	// 文字送りテキスト2Dクラス
class CTimeUI;			// タイムUIクラス
class CBlink2D;			// 点滅オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// クリア成功マネージャークラス
class CClearSuccessManager : public CClearManager
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,			// 何もしない状態
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
		STATE_END,				// 終了状態
		STATE_MAX				// この列挙型の総数
	};

	// コンストラクタ
	CClearSuccessManager();

	// デストラクタ
	~CClearSuccessManager() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void SetAllMove(const D3DXVECTOR3& rMove) override;	// 全UIオブジェクト移動
	void SkipStaging(void) override;					// 演出スキップ
	bool IsEndState(void) const override { return (m_state == STATE_END); }	// 終了状況取得

private:
	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CClearSuccessManager::*AFuncUpdateState)(const float fDeltaTime);

	// 静的メンバ変数
	static AFuncUpdateState m_aFuncUpdateState[];	// 状態更新関数

	// メンバ関数
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

	// メンバ変数
	CAnim2D* m_apGodItemBG[CStage::GOD_MAX];	// 神器アイコン背景情報
	CAnim2D* m_apGodItemIcon[CStage::GOD_MAX];	// 神器アイコン情報
	CString2D* m_pTime;		// 遂行時間タイトル情報
	CTimeUI* m_pTimeVal;	// 遂行時間情報
	CString2D* m_pGodItem;	// 神器タイトル情報
	CBlink2D* m_pControl;	// 操作情報
	EState m_state;			// 状態
	float m_fCurTime;		// 現在の待機時間
};

#endif	// _CLEAR_SUCCESS_MANAGER_H_
