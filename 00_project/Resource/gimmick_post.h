#pragma once
//=========================================
//
//  設置ギミック (gimmick_post.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick_action.h"

//=========================================
//  クラス定義
//=========================================
class CGimmickPost : public CGimmickAction
{
public:

	// メンバ関数
	CGimmickPost();
	~CGimmickPost() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

	bool IsSet() { return m_bPost; } // 設置フラグの取得

private:

	// メンバ変数
	bool m_bPost; // 設置フラグ

};
