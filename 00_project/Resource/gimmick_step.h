#pragma once
//=========================================
//
//  踏み台ギミック (gimmick_step.h)
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
	D3DXVECTOR3 CalcWaitPoint(const int Idx) const override; // 各分身毎の待機位置を算出

private:

	// メンバ関数
	void Climb(CPlayer* player, const float fDeltaTime); // 登る

	// メンバ変数
	float m_fSummit; // 登頂位置
	bool m_bSummit; // 登頂フラグ

};
