#pragma once
//===========================================
//
//  �A���N���X(plant.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "object3D.h"

//===========================================
//  �N���X��`
//===========================================
class CPlant : public CObject3D
{
public:

	// �����o�֐�
	CPlant();
	~CPlant();

	virtual HRESULT Init(void) override;	// ������
	virtual void Uninit(void) override;		// �I��
	virtual void Update(const float fDeltaTime) override;		// �X�V
	virtual void Draw(CShader* pShader = nullptr) override;		// �`��

private:

};
