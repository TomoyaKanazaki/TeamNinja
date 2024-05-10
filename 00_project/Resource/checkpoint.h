//==========================================
//
//  �`�F�b�N�|�C���g(checkpoint.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#pragma once
#include "objectModel.h"

//==========================================
//  �N���X��`
//==========================================
class CCheckPoint : public CObjectModel
{
public:

	// �����o�֐�
	CCheckPoint();
	~CCheckPoint();

	HRESULT Init(void) override; // ������
	void Uninit(void) override; // �I��
	void Update(const float fDeltaTime) override; // �X�V
	void Draw(CShader* pShader = nullptr) override; // �`��

	// �ÓI�����o�֐�
	static CCheckPoint* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rRot); // ��������

private:

	// �����o�֐�
	void CollisionPlayer(void);	// �v���C���[�Ƃ̓����蔻��

	// �����o�ϐ�
	bool m_bSave; // �Z�[�u�t���O
	float m_fRadius; // �����蔻�蔼�a
	int m_nSaveTension; // �Z�[�u���̎m�C��

	// �ÓI�����o�ϐ�
	static int m_nNumAll;

};
