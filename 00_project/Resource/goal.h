//==========================================
//
//  �S�[��(goal.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#pragma once
#include "objectModel.h"

//==========================================
//  �N���X��`
//==========================================
class CGoal : public CObjectModel
{
public:

	// �����o�֐�
	CGoal();
	~CGoal();

	HRESULT Init() override; // ������
	void Uninit() override; // �I��
	void Update(const float fDeltaTime) override; // �X�V
	void Draw(CShader* pShader = nullptr) override; // �`��

	bool GetClear() const { return m_bClear; }// �N���A��Ԃ̎擾

	// �ÓI�����o�֐�
	static CGoal* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot); // ��������

private:

	// �����o�֐�
	void CollisionPlayer();	// �v���C���[�Ƃ̓����蔻��
	void Load(); // �O�����̓ǂݍ���

	// �����o�ϐ�
	bool m_bClear; // �N���A�t���O
	float m_fRadius; // �����蔻�蔼�a
	float m_fRotSpeed; // ��]���x

};
