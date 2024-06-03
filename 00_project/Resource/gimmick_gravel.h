#pragma once
//==========================================
//
//  砂利道ギミック(gimmick_gravel.h)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_state.h"

//==========================================
//  クラス定義
//==========================================
class CGimmickGravel : public CGimmickState
{
public:

	// メンバ関数
	CGimmickGravel();
	~CGimmickGravel() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

	// メンバ関数
	void SetChar(CPlayerClone* pClone) override; // 文字列(フラグ)を追加
	void DeleteChar(CPlayerClone* pClone) override; // 文字列(フラグ)を削除

};
