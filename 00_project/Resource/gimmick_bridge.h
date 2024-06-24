#pragma once
//=========================================
//
//  橋のギミック(gimmick_bridge.h)
//  Author : Tomoya Kanazaki
//
//=========================================
#include "gimmick_action.h"

//=========================================
//  クラス定義
//=========================================
class CGimmickBridge : public CGimmickAction
{
public:

	// コンストラクタ
	CGimmickBridge();

	// デストラクタ
	~CGimmickBridge() override;

	// オーバーライド関数
	HRESULT Init(void) override; // 初期化処理
	void Uninit(void) override; // 終了処理
	void Update(const float fDeltaTime) override; // 更新処理
	void Draw(CShader* pShader = nullptr) override; // 描画処理
	D3DXVECTOR3 CalcWaitPoint(const int Idx) const override; // 各分身毎の待機位置を算出

private:

};
