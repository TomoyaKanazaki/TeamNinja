#pragma once
//==========================================
//
//  掃除したての床ギミック(gimmick_cleen.h)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_state.h"

//==========================================
//  クラス定義
//==========================================
class CGimmickCleen : public CGimmickState
{
public:

	// メンバ関数
	CGimmickCleen();
	~CGimmickCleen() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

	// メンバ関数
	void HitClone(CPlayerClone* pClone) override; // 分身に当たっていた時の処理
	void MissClone(CPlayerClone* pClone) override; // 分身に当たっていない場合の処理

};
