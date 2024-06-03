#pragma once
//=========================================
//
//  ジャンプ台ヘッダー (gimmick_step.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick_action.h"

//===========================================
//  前方宣言
//===========================================
class CPlayer;

//=========================================
//  クラス定義
//=========================================
class CGimmickStep : public CGimmickAction
{
public:

	// メンバ関数
	CGimmickStep();
	~CGimmickStep() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

	// メンバ関数
	void Climb(CPlayer* player); // 登る

};
