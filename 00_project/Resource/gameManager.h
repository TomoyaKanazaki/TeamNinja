//============================================================
//
//	ゲームマネージャーヘッダー [gameManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "retentionManager.h"
#include "godItem.h"

//************************************************************
//	前方宣言
//************************************************************
class CResultManager;	// リザルトマネージャー

//************************************************************
//	クラス定義
//************************************************************
// ゲームマネージャークラス
class CGameManager
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_START,	// スタート状態
		STATE_NORMAL,	// 通常状態
		STATE_GODITEM,	// 神器獲得状態
		STATE_RESULT,	// リザルト状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CGameManager();

	// デストラクタ
	~CGameManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(const float fDeltaTime);	// 更新
	void SetState(const EState state)	{ m_state = state; }	// 状態設定
	EState GetState(void) const			{ return m_state; }		// 状態取得
	void SetSave(const int nSave)		{ m_nSave = nSave; }	// セーブ設定
	int GetSave(void) const				{ return m_nSave; }		// セーブ取得
	void PossessGodItem(const CGodItem::EType typeID);			// 勾玉獲得
	void TransitionResult(const CRetentionManager::EWin win);	// リザルト画面遷移

	// 静的メンバ関数
	static CGameManager *Create(void);	// 生成
	static void Release(CGameManager *&prGameManager);	// 破棄

private:
	// メンバ変数
	CResultManager *m_pResult;	// リザルトマネージャー
	EState m_state;	// 状態
	int m_nSave;	// 最後のセーブポイント
};

#endif	// _GAMEMANAGER_H_
