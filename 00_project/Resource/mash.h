#pragma once
//==========================================
//
//  �ӂ���(mash.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "actor.h"

//==========================================
//  �N���X��`
//==========================================
class CMash : public CActor
{
public:

	// �����o�֐�
	CMash();
	~CMash();

	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	void SetVec3Position(const D3DXVECTOR3& rPos) override;

private:

	// �����o�֐�
	void Move(const float fDeltaTime); // �ړ�

	// �����o�ϐ�
	D3DXVECTOR3 m_posDefault; // �����ʒu��ۑ�����ϐ�
	D3DXVECTOR3 m_offsetMove; // �ړ���̃I�t�Z�b�g��ۑ�����ϐ�
	D3DXVECTOR3 m_move; // �ړ��ʂ�ۑ�����ϐ�

};
