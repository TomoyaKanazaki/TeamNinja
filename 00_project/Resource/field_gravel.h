#pragma once
//==========================================
//
//  �������M�~�b�N(field_gravel.h)
//  Author : Tomoya Kanazaki
// 
//==========================================
#include "field.h"

//==========================================
//  �N���X��`
//==========================================
class CGimmickGravel : public CField
{
public:

	// �����o�֐�
	CGimmickGravel();
	~CGimmickGravel() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

	void Hit(CPlayerClone* pClone) override;	// ���g�ɓ������Ă������̏���
	void Miss(CPlayerClone* pClone) override;	// ���g�ɓ������Ă��Ȃ����̏���
	void Hit(CPlayer* pPlayer) override; // �������Ă����ꍇ�̏���
	void Miss(CPlayer* pPlayer) override; // �������Ă��Ȃ��ꍇ�̏���

};