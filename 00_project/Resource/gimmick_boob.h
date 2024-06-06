#pragma once
//==========================================
//
//  �h�u�M�~�b�N(gimmick_boob.h)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_state.h"

//==========================================
//  �N���X��`
//==========================================
class CGimmickBoob : public CGimmickState
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
	void HitClone(CPlayerClone* pClone) override; // ���g�ɓ������Ă������̏���
	void MissClone(CPlayerClone* pClone) override; // ���g�ɓ������Ă��Ȃ��ꍇ�̏���

};
