#pragma once
//==========================================
//
//  砂利道ギミック(field_gravel.h)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field.h"

//==========================================
//  クラス定義
//==========================================
class CGimmickGravel : public CField
{
public:

	// メンバ関数
	CGimmickGravel();
	~CGimmickGravel() override;

	// オーバーライド関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

	void Hit(CPlayerClone* pClone) override;	// 分身に当たっていた時の処理
	void Miss(CPlayerClone* pClone) override;	// 分身に当たっていない時の処理
	void Hit(CPlayer* pPlayer) override; // 当たっていた場合の処理
	void Miss(CPlayer* pPlayer) override; // 当たっていない場合の処理

};
