#pragma once
//==========================================
//
//  �������M�~�b�N(gimmick_gravel.h)
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
	void Hit(CPlayerClone* pClone) override;	// ���g�ɓ������Ă������̏���
	void Miss(CPlayerClone* pClone) override;	// ���g�ɓ������Ă��Ȃ����̏���

};
