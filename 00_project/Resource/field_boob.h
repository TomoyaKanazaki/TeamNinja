#pragma once
//==========================================
//
//  �D�M�~�b�N(field_boob.h)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field.h"

//==========================================
//  �N���X��`
//==========================================
class CGimmickBoob : public CField
{
public:

	// �����o�֐�
	CGimmickBoob();
	~CGimmickBoob() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

	// �����o�֐�
	void Hit(CPlayerClone* pClone) override; // �������Ă����ꍇ�̏���
	void Miss(CPlayerClone* pClone) override; // �������Ă��Ȃ��ꍇ�̏���
	void Hit(CPlayer* pPlayer) override; // �������Ă����ꍇ�̏���
	void Miss(CPlayer* pPlayer) override; // �������Ă��Ȃ��ꍇ�̏���

	// �����o�ϐ�
	bool m_bLiquid;

};
