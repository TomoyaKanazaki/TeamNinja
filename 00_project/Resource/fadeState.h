//============================================================
//
//	フェード状態ヘッダー [fadeState.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FADE_STATE_H_
#define _FADE_STATE_H_

//************************************************************
//	前方宣言
//************************************************************
class CFade;	// フェードクラス

//************************************************************
//	クラス定義
//************************************************************
// フェード状態クラス
class CFadeState
{
public:
	// コンストラクタ
	CFadeState();

	// デストラクタ
	virtual ~CFadeState();

	// 純粋仮想関数
	virtual HRESULT Init(void)	= 0;	// 初期化
	virtual void Uninit(void)	= 0;	// 終了
	virtual void Update(const float fDeltaTime) = 0;	// 更新

	// メンバ関数
	void SetContext(CFade* pContext) { m_pContext = pContext; }	// コンテキスト設定

protected:
	// メンバ変数
	CFade *m_pContext;	// コンテキスト
};

//************************************************************
//	インクルードファイル
//************************************************************
#include "fadeStateNone.h"
#include "fadeStateWait.h"
#include "fadeStateOut.h"
#include "fadeStateIn.h"
#include "fadeStateIrisOut.h"
#include "fadeStateIrisIn.h"

#endif	// _FADE_STATE_H_
