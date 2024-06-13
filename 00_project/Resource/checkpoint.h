//==========================================
//
//  �`�F�b�N�|�C���g(checkpoint.h)
//  Author : Tomoya Kanazaki
//  Adder�FRitsuki Obara
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

	HRESULT Init() override; // ������
	void Uninit() override; // �I��
	void Update(const float fDeltaTime) override; // �X�V
	void Draw(CShader* pShader = nullptr) override; // �`��

	int GetSaveTension() const { return m_nSaveTension; }	// �Z�[�u���̎m�C�͎擾

	// �ÓI�����o�֐�
	static CCheckPoint* Create(const D3DXVECTOR3& rPos);	// ��������
	static CListManager<CCheckPoint>* GetList(void);		// ���X�g�擾
	static HRESULT LoadSetup(void);	// �Z�b�g�A�b�v

private:

	// �����o�֐�
	void CollisionPlayer();	// �v���C���[�Ƃ̓����蔻��

	// �����o�ϐ�
	CListManager<CCheckPoint>::AIterator m_iterator;	// �C�e���[�^�[
	bool m_bSave; // �Z�[�u�t���O
	int m_nSaveTension; // �Z�[�u���̎m�C��

	// �ÓI�����o�ϐ�
	static int m_nNumAll;
	static CListManager<CCheckPoint>* m_pList;	// �I�u�W�F�N�g���X�g
};
