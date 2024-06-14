#pragma once
//=========================================
//
//  落とし穴ヘッダー (gimmick_fall.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "field.h"

//=========================================
//  クラス定義
//=========================================
class CGimmickFall : public CField
{
public:

	// メンバ関数
	CGimmickFall();
	~CGimmickFall() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

	// メンバ変数
	bool m_bFall; // 落下フラグ

};
