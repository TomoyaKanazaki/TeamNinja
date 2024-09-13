#pragma once
//==========================================
//
//  �M��UI�N���X(drown.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "object2D.h"

//==========================================
//  �O���錾
//==========================================
class CObject2D;

//==========================================
//  �N���X��`
//==========================================
class CDrown : public CObject2D
{
public:

	// �����o�֐�
	CDrown();
	~CDrown();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override; // ������
	void Uninit(void) override; // �I��
	void Update(const float fDeltaTime) override; // �X�V
	void Draw(CShader *pShader = nullptr) override; // �`��
	void Move(const float fDeltaTime); // �ړ�

	void SetMove(bool bFrag) { m_bMove = bFrag; } // �t���O��ݒ�
	bool IsBrind() { m_bBrind; } // �ډB���t���O�̎擾

	// �ÓI�����o�֐�
	static CDrown* Create(void);

private:

	// �����o�ϐ�
	bool m_bMove; // �ړ��t���O
	bool m_bBrind; // �ډB�������t���O
	float m_fMove; // �ړ���

};
