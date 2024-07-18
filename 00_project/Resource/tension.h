#pragma once
//===========================================
//
//  �m�C�̓Q�[�W(tension.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "object2D.h"

//===========================================
//  �N���X��`
//===========================================
class CTension : CObject2D
{
public:

	// �����o�֐�
	CTension();
	~CTension();

	HRESULT Init() override; // ����������
	void Uninit() override; // �I������
	void Update(const float fDeltaTime) override; // �X�V����
	void Draw(CShader* pShader = nullptr) override; // �`�揈��

	bool IsUse() { return m_bUse; } // �g�p�\�t���O�̎擾

	// �ÓI�����o�֐�
	static CTension* Create(); // ��������
	static CListManager<CTension>* GetList(); // ���X�g�擾
	static int GetUseNum(); // �g�p�\�Ȏm�C�͂̎擾

private:

	// �����o�֐�
	void CalcPosition(); // ���W�̌v�Z����
	void ChangeColor(); //�F�̕ύX
	void SwitchUse(); // �g�p�\��Ԃ̕ύX

	// �����o�ϐ�
	CListManager<CTension>::AIterator m_iterator;	// �C�e���[�^�[
	bool m_bUse; // �g�p�\�t���O

	// �ÓI�����o�ϐ�
	static CListManager<CTension>* m_pList;	// �I�u�W�F�N�g���X�g
	static int m_nUseNum; // �g�p�\�Ȏm�C�͂̐�

};