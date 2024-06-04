#pragma once
//==========================================
//
//  ����M�~�b�N(gimmick_water.h)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "gimmick_state.h"

//==========================================
//  �N���X��`
//==========================================
class CGimmickWater : public CGimmickState
{
public:

	// �����o�֐�
	CGimmickWater();
	~CGimmickWater() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

	// �����o�֐�
	void HitClone(CPlayerClone* pClone) override; // ���g�ɓ������Ă������̏���
	void MissClone(CPlayerClone* pClone) override; // ���g�ɓ������Ă��Ȃ��ꍇ�̏���

};
