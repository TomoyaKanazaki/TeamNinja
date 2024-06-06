#pragma once
//=========================================
//
//  複数管理ギミック (gimmick_malti.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick.h"

//=========================================
//  クラス定義
//=========================================
class CGimmickMalti : public CGimmick
{
public:

	// メンバ関数
	CGimmickMalti();
	~CGimmickMalti() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

};
