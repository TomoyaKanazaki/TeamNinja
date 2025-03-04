#pragma once
//==========================================
//
//  水場ギミック(field_Xbridge.h)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field.h"

//==========================================
//  クラス定義
//==========================================
class CGimmickXBridge : public CField
{
public:

	// メンバ関数
	CGimmickXBridge();
	~CGimmickXBridge() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

	// メンバ関数
	void Hit(CPlayerClone* pClone) override; // 当たっていた場合の処理
	void Miss(CPlayerClone* pClone) override; // 当たっていない場合の処理
	void Hit(CPlayer* pPlayer) override; // 当たっていた場合の処理
	void Miss(CPlayer* pPlayer) override; // 当たっていない場合の処理

};
