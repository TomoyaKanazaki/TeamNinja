#pragma once
//=========================================
//
//  �����Ǘ��M�~�b�N (gimmick_malti.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick.h"

//=========================================
//  �N���X��`
//=========================================
class CGimmickMalti : public CGimmick
{
public:

	// �����o�֐�
	CGimmickMalti();
	~CGimmickMalti() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

	bool IsActive() { return m_bActive; }

private:

	// �����o�֐�
	HRESULT Create(); // �ݒu�M�~�b�N�̐���

	// �����o�ϐ�
	bool m_bActive; // �A�N�e�B�u�t���O
	CListManager<CGimmick>::AIterator m_iterator; // �C�e���[�^�[
	CListManager<CGimmick>* m_pGimmick; // �I�u�W�F�N�g���X�g

};
