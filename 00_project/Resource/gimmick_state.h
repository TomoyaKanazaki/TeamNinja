//============================================================
//
// 状態ギミックヘッダー [gimmick_state.h]
// Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GIMMICK_STATE_H_
#define _GIMMICK_STATE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "gimmick.h"

//************************************************************
//	クラス定義
//************************************************************
// 状態ギミッククラス
class CGimmickState : public CGimmick
{
public:

	// コンストラクタ
	CGimmickState();

	// デストラクタ
	virtual ~CGimmickState() override;

	// オーバーライド関数
	virtual HRESULT Init(void) override;	// 初期化
	virtual void Uninit(void) override;		// 終了
	virtual void Update(const float fDeltaTime) override;	// 更新
	virtual void Draw(CShader* pShader = nullptr) override;	// 描画

private:

};

#endif	// _GIMMICK_ACTION_H_
