//============================================================
//
//	アイリスイン状態ヘッダー [fadeStateIrisIn.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FADE_STATE_IRISIN_H_
#define _FADE_STATE_IRISIN_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "fadeState.h"

//************************************************************
//	クラス定義
//************************************************************
// アイリスイン状態クラス
class CFadeStateIrisIn : public CFadeState
{
public:
	// コンストラクタ
	CFadeStateIrisIn(std::function<D3DXVECTOR3(void)> pFuncPos);

	// デストラクタ
	~CFadeStateIrisIn() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新

private:
	// メンバ変数
	std::function<D3DXVECTOR3(void)> m_pFuncPos;	// 切り抜き型位置関数ポインタ
	float m_fInitRad;	// 初期半径
	float m_fCurTime;	// 現在の経過時間
};

#endif	// _FADE_STATE_IRISIN_H_
