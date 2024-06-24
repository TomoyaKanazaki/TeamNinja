//============================================================
//
// 飛び降りヘッダー [gimmick_jumpoff.h]
// Author：佐藤根詩音
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GIMMICK_JUMPOFF_H_
#define _GIMMICK_JUMPOFF_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "gimmick_action.h"

//************************************************************
//	クラス定義
//************************************************************
class CGimmickJumpOff : public CGimmickAction
{
public:
	CGimmickJumpOff();
	~CGimmickJumpOff();

	HRESULT Init(void);	// 初期化
	void Uninit(void);		// 終了
	void Update(const float fDeltaTime);	// 更新
	void Draw(CShader* pShader = nullptr);	// 描画
	D3DXVECTOR3 CalcWaitPoint(const int Idx) override; // 各分身毎の待機位置を算出

private:

};

#endif
