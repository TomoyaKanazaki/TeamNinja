#pragma once
//===========================================
//
//  �A���Ǘ��N���X(multi_plant.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "object3D.h"
#include "gimmick.h"

//===========================================
//  �N���X��`
//===========================================
class CMultiPlant : public CObject3D
{
public:

	// �����o�֐�
	CMultiPlant();
	~CMultiPlant();

	virtual HRESULT Init(void) override; // ������
	virtual void Uninit(void) override; // �I��
	virtual void Update(const float fDeltaTime) override; // �X�V
	virtual void Draw(CShader* pShader = nullptr) override; // �`��

	void Grow(); // �Ԃ̐���

	// �ÓI�����o�֐�
	static CMultiPlant* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const CGimmick::EType type, int nNum); // ����

private:

	// �����o�ϐ�
	CGimmick::EType m_Type; // ��������A���̎��
	int m_nNum; // ������
	bool m_bGrow; // �A�������t���O

};
