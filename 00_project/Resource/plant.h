#pragma once
//===========================================
//
//  �A���N���X(plant.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "objectBillboard.h"

//===========================================
//  �N���X��`
//===========================================
class CPlant : public CObjectBillboard
{
public:

	// �����o�֐�
	CPlant();
	~CPlant();

	virtual HRESULT Init(void) override; // ����������
	virtual void Uninit(void) override; // �I������
	virtual void Update(const float fDeltaTime) override; // �X�V����
	virtual void Draw(CShader* pShader = nullptr) override; // �`�揈��

	// �ÓI�����o�֐�
	static CPlant* Create(const D3DXVECTOR3& rPos, const char* sPass);

private:

};
