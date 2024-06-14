#pragma once
//=========================================
//
//  朽ちた床ヘッダー (gimmick_decaed.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "field.h"

//=========================================
//  クラス定義
//=========================================
class CGimmickDecaed : public CField
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

	// メンバ変数
	bool m_bFall; // 床抜けフラグ

};
