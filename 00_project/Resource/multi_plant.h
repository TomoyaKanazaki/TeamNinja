#pragma once
//===========================================
//
//  �A���Ǘ��N���X(multi_plant.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "object3D.h"

//===========================================
//  �N���X��`
//===========================================
class CMultiPlant : public CObject3D
{
public:

	// �Ԃ̎��
	enum EType
	{
		TYPE_TEST = 0, // TODO ��ނƂ����O�Ƃ����������ɂ���
		TYPE_MAX
	};

	// �����o�֐�
	CMultiPlant();
	~CMultiPlant();

	virtual HRESULT Init(void) override; // ������
	virtual void Uninit(void) override; // �I��
	virtual void Update(const float fDeltaTime) override; // �X�V
	virtual void Draw(CShader* pShader = nullptr) override; // �`��

	// �ÓI�����o�֐�
	static CMultiPlant* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const EType type, int nNum); // ����

private:

	// �����o�֐�
	void Grow(); // �Ԃ̐���

	// �����o�ϐ�
	EType m_Type; // ��������A���̎��
	D3DXVECTOR3 m_pos; // ���S���W
	D3DXVECTOR3 m_size; // �����͈�
	int m_nNum; // ������
	bool m_bGrow; // �A�������t���O

};
