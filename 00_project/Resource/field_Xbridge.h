#pragma once
//==========================================
//
//  ����M�~�b�N(field_Xbridge.h)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field.h"

//==========================================
//  �N���X��`
//==========================================
class CGimmickXBridge : public CField
{
public:

	// �����o�֐�
	CGimmickXBridge();
	~CGimmickXBridge() override;

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

};