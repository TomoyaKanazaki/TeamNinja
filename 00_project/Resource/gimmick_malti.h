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

private:

};
