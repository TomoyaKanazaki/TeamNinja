#pragma once
//==========================================
//
//  �������M�~�b�N(gimmick_gravel.h)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_state.h"

//==========================================
//  �N���X��`
//==========================================
class CGimmickGravel : public CGimmickState
{
public:

	// �����o�֐�
	CGimmickGravel();
	~CGimmickGravel() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

};
