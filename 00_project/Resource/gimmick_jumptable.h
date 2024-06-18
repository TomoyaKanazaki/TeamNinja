//============================================================
//
// ジャンプ台ヘッダー [gimmick_jumptable.h]
// Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GIMMICK_JUMPTABLE_H_
#define _GIMMICK_JUMPTABLE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "gimmick_action.h"

//************************************************************
//	クラス定義
//************************************************************
// ジャンプ台クラス
class CGimmickJumpTable : public CGimmickAction
{
public:

	// コンストラクタ
	CGimmickJumpTable();

	// デストラクタ
	~CGimmickJumpTable() override;

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画
	D3DXVECTOR3 CalcWaitPoint(const int Idx) const override; // 各分身毎の待機位置を算出

private:

};

#endif	// _GIMMICK_H_
