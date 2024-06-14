#pragma once
//=========================================
//
//  ���Ƃ����w�b�_�[ (gimmick_fall.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "field.h"

//=========================================
//  �N���X��`
//=========================================
class CGimmickFall : public CField
{
public:

	// �����o�֐�
	CGimmickFall();
	~CGimmickFall() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

	// �����o�֐�
	void Hit(CPlayerClone* pClone) override; // �������Ă����ꍇ�̏���
	void Miss(CPlayerClone* pClone) override; // �������Ă��Ȃ��ꍇ�̏���

	// �����o�ϐ�
	bool m_bFall; // �����t���O

};
