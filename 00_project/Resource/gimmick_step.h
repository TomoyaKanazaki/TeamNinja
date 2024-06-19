#pragma once
//=========================================
//
//  ���ݑ�M�~�b�N (gimmick_step.h)
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
	D3DXVECTOR3 CalcWaitPoint(const int Idx) const override; // �e���g���̑ҋ@�ʒu���Z�o

private:

	// �����o�֐�
	void Climb(CPlayer* player, const float fDeltaTime); // �o��

	// �����o�ϐ�
	float m_fSummit; // �o���ʒu
	bool m_bSummit; // �o���t���O

};
