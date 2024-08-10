//============================================================
//
//	クリア失敗マネージャーヘッダー [clearFailManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _CLEAR_FAIL_MANAGER_H_
#define _CLEAR_FAIL_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "clearManager.h"

//************************************************************
//	前方宣言
//************************************************************
class CString2D;	// 文字列2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// クリア失敗マネージャークラス
class CClearFailManager : public CClearManager
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,				// 何もしない状態
		STATE_CONTINUE_TITLE_WAIT,	// コンテニュータイトル待機状態
		STATE_CONTINUE_TITLE,		// コンテニュータイトル表示状態
		STATE_SELECT_WAIT,			// 選択肢待機状態
		STATE_SELECT,				// 選択肢表示状態
		STATE_WAIT,					// 待機状態
		STATE_END,					// 終了状態
		STATE_MAX					// この列挙型の総数
	};

	// コンストラクタ
	CClearFailManager();

	// デストラクタ
	~CClearFailManager() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void SetAllMove(const D3DXVECTOR3& rMove) override;	// 全UIオブジェクト移動
	void SkipStaging(void) override;					// 演出スキップ
	bool IsSkipOK(void) const override	 { return (m_state < STATE_WAIT); }	// スキップ可能状況取得
	bool IsEndState(void) const override { return (m_state == STATE_END); }	// 終了状況取得

private:
	// 選択列挙
	enum ESelect
	{
		SELECT_YES = 0,	// はい
		SELECT_NO,		// いいえ
		SELECT_MAX		// この列挙型の総数
	};

	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CClearFailManager::*AFuncUpdateState)(const float fDeltaTime);

	// 静的メンバ変数
	static AFuncUpdateState m_aFuncUpdateState[];	// 状態更新関数

	// メンバ関数
	void UpdateContinueTitleWait(const float fDeltaTime);	// コンテニュータイトル待機更新
	void UpdateContinueTitle(const float fDeltaTime);		// コンテニュータイトル表示更新
	void UpdateSelectWait(const float fDeltaTime);			// 選択肢待機更新
	void UpdateSelect(const float fDeltaTime);				// 選択肢表示更新
	void UpdateWait(const float fDeltaTime);				// 待機更新

	// メンバ変数
	CString2D* m_apSelect[SELECT_MAX];	// 選択肢情報
	CString2D* m_pContinue;				// コンテニュー情報
	EState m_state;		// 状態
	float m_fCurTime;	// 現在の待機時間
	int m_nCurSelect;	// 現在の選択肢
	int m_nOldSelect;	// 前回の選択肢
};

#endif	// _CLEAR_FAIL_MANAGER_H_
