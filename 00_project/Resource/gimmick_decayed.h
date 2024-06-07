#pragma once
//=========================================
//
//  朽ちた床ヘッダー (gimmick_decaed.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick_action.h"

//=========================================
//  クラス定義
//=========================================
class CGimmickDecaed : public CGimmickAction
{
public:

	// メンバ関数
	CGimmickDecaed();
	~CGimmickDecaed() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

};
