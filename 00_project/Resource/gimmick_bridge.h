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

	// �R���X�g���N�^
	CGimmickBridge();

	// �f�X�g���N�^
	~CGimmickBridge() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override; // ����������
	void Uninit(void) override; // �I������
	void Update(const float fDeltaTime) override; // �X�V����
	void Draw(CShader* pShader = nullptr) override; // �`�揈��
	D3DXVECTOR3 CalcWaitPoint(const int Idx) const override; // �e���g���̑ҋ@�ʒu���Z�o

private:

};
