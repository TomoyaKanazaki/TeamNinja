#pragma once
//=========================================
//
//  �W�����v��w�b�_�[ (gimmick_step.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick_action.h"

//===========================================
//  �O���錾
//===========================================
class CPlayer;

//=========================================
//  �N���X��`
//=========================================
class CGimmickStep : public CGimmickAction
{
public:

	// �����o�֐�
	CGimmickStep();
	~CGimmickStep() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

	// �����o�֐�
	void Climb(CPlayer* player); // �o��

};
