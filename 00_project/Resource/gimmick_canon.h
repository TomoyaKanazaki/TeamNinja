#pragma once
//=========================================
//
//  ��C�̃M�~�b�N(gimmick_canon.h)
//  Author : Tomoya Kanazaki
//
//=========================================
#include "gimmick_action.h"

//=========================================
//  �N���X��`
//=========================================
class CGimmickCanon : public CGimmickAction
{
public:

	// �ڕW��Z���W��
	enum ETarget
	{
		TARGET_LEFT = -1, // ����
		TARGET_CENTER, // ����
		TARGET_RIGHT, // �E��
		TARGET_MAX
	};

	// �����o�֐�
	CGimmickCanon();
	~CGimmickCanon() override;

	HRESULT Init(void) override; // ����������
	void Uninit(void) override; // �I������
	void Update(const float fDeltaTime) override; // �X�V����
	void Draw(CShader* pShader = nullptr) override; // �`�揈��
	D3DXVECTOR3 CalcWaitPoint(const int Idx) override; // �e���g���̑ҋ@�ʒu���Z�o
	D3DXVECTOR3 CalcWaitRotation(const int Idx, const D3DXVECTOR3& rPos) override; // �e���g���̑ҋ@�������Z�o

	void SetTarget(const float fTarget) { m_fTarget = fTarget; } // �ڕW�ʒu�̐ݒ�

	// �ÓI�����o�֐�
	static CGimmickCanon* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot, ETarget eTarget);

private:

	// �����o�֐�
	void Shoot(); // ����

	// �����o�ϐ�
	float m_fTarget; // �ڕW�ʒu
	bool m_bShoot; // ���ˍς�
};
