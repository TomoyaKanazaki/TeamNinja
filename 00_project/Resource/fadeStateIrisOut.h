//============================================================
//
//	アイリスアウト状態ヘッダー [fadeStateIrisOut.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FADE_STATE_IRISOUT_H_
#define _FADE_STATE_IRISOUT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "fadeState.h"

//************************************************************
//	クラス定義
//************************************************************
// アイリスアウト状態クラス
class CFadeStateIrisOut : public CFadeState
{
public:
	// コンストラクタ
	CFadeStateIrisOut(std::function<D3DXVECTOR3(void)> pFuncPos);

	// デストラクタ
	~CFadeStateIrisOut() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void SetContext(CFade* pContext) override;		// コンテキスト設定

private:
	// メンバ変数
	std::function<D3DXVECTOR3(void)> m_pFuncPos;	// 切り抜き型位置関数ポインタ
};

#endif	// _FADE_STATE_IRISOUT_H_
