#pragma once
//=========================================
//
//  ���̃M�~�b�N(gimmick_bridge.h)
//  Author : Tomoya Kanazaki
//
//=========================================
#include "gimmick_action.h"

//=========================================
//  �N���X��`
//=========================================
class CGimmickBridge : public CGimmickAction
{
public:

	// �����o�֐�
	CGimmickBridge();
	~CGimmickBridge() override;

	HRESULT Init(void) override; // ����������
	void Uninit(void) override; // �I������
	void Update(const float fDeltaTime) override; // �X�V����
	void Draw(CShader* pShader = nullptr) override; // �`�揈��
	D3DXVECTOR3 CalcWaitPoint(const int Idx) const override; // �e���g���̑ҋ@�ʒu���Z�o

private:

	// �����o�֐�
	void CalcConectPoint(); // ���̒[�̌v�Z����

	// �����o�ϐ�
	bool m_bSet; // �ݒ�ς݃t���O
	D3DXVECTOR3 m_ConectPoint[2]; // ���̒[

};
