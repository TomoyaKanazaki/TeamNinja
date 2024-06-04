#pragma once
//=========================================
//
//  ���Ƃ����w�b�_�[ (gimmick_fall.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "gimmick_action.h"

//=========================================
//  �N���X��`
//=========================================
class CGimmickFall : public CGimmickAction
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

};
