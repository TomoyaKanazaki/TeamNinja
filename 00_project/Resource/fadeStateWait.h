//============================================================
//
//	余韻管理状態ヘッダー [fadeStateWait.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FADE_STATE_WAIT_H_
#define _FADE_STATE_WAIT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "fadeState.h"

//************************************************************
//	クラス定義
//************************************************************
// 余韻管理状態クラス
class CFadeStateWait : public CFadeState
{
public:
	// コンストラクタ
	CFadeStateWait(const float fWaitTime, CFadeState* pNextState);

	// デストラクタ
	~CFadeStateWait() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新

private:
	// メンバ変数
	CFadeState* m_pNextState;	// 遷移先の状態
	float m_fWaitTime;			// 現在の余韻時間
};

#endif	// _FADE_STATE_WAIT_H_
