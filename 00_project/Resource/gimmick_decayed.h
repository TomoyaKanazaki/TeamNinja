#pragma once
//=========================================
//
//  ���������w�b�_�[ (gimmick_decaed.h)
//  Author : Tomoya Kanzaki
//
//=========================================
#include "field.h"

//=========================================
//  �N���X��`
//=========================================
class CGimmickDecaed : public CField
{
public:

	// �����o�֐�
	CGimmickDecaed();
	~CGimmickDecaed() override;

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(const float fDeltaTime) override;
	void Draw(CShader* pShader = nullptr) override;

private:

	// �����o�ϐ�
	bool m_bFall; // �������t���O

};
