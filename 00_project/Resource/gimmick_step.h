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
	D3DXVECTOR3 CalcWaitPoint(const int Idx) override;	// �e���g���̑ҋ@�ʒu���Z�o
	D3DXVECTOR3 CalcWaitRotation(const int Idx, const CPlayerClone* pClone) override;	// �e���g���̑ҋ@�������Z�o

private:

	// �����o�֐�
	bool Climb(CPlayer* player); // �o��

	// �����o�ϐ�
	bool m_bClimb; // �o���Ă钆�t���O

};
