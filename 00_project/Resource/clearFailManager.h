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
class CAnim2D;		// アニメーション2Dクラス
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
		STATE_NONE = 0,	// 何もしない状態
		STATE_WAIT,		// 待機状態
		STATE_END,		// 終了状態
		STATE_MAX		// この列挙型の総数
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
	bool IsEndState(void) const override { return (m_state == STATE_END); }	// 終了状況取得

private:
	// 状態更新の関数ポインタ型エイリアス定義
	typedef void (CClearFailManager::*AFuncUpdateState)(const float fDeltaTime);

	// 静的メンバ変数
	static AFuncUpdateState m_aFuncUpdateState[];	// 状態更新関数

	// メンバ関数
	void UpdateWait(const float fDeltaTime);	// 待機更新

	// メンバ変数
	EState m_state;		// 状態
	float m_fCurTime;	// 現在の待機時間
};

#endif	// _CLEAR_FAIL_MANAGER_H_
