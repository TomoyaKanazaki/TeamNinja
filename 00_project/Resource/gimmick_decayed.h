#pragma once
//=========================================
//
//  ���������w�b�_�[ (gimmick_decayed.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "field.h"

//=========================================
//  �N���X��`
//=========================================
class CGimmickDecayed : public CField
{
public:

	// �����o�֐�
	CGimmickDecayed();
	~CGimmickDecayed() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

	bool IsFall() const override { return m_bFall; } // ��������

private:

	// �����o�֐�
	void Hit(CPlayerClone* pClone) override; // �������Ă����ꍇ�̏���
	void Miss(CPlayerClone* pClone) override; // �������Ă��Ȃ��ꍇ�̏���
	void Count(); // ����Ă���L�����N�^�[�����̌v�Z����

	// �����o�ϐ�
	bool m_bFall; // �������t���O

};
