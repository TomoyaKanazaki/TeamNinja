#pragma once
//===========================================
//
//  �`���[�g���A���N���X(tutorial.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "objectBillboard.h"

//===========================================
//  �N���X��`
//===========================================
class CTutorial : public CObjectBillboard
{
public:

	enum EType
	{
		TYPE_MOVE = 0, // ���s
		TYPE_CLONE, // ���g
		TYPE_GIMMICK, // �M�~�b�N
		TYPE_MAX, // �ő吔
	};

	// �����o�֐�
	CTutorial();
	~CTutorial();

	virtual HRESULT Init(void) override; // ����������
	virtual void Uninit(void) override; // �I������
	virtual void Update(const float fDeltaTime) override; // �X�V����
	virtual void Draw(CShader* pShader = nullptr) override; // �`�揈��
	void BindTexture(const char* pTexturePass) override;	// �e�N�X�`������ (�p�X)

	// �ÓI�����o�֐�
	static CTutorial* Create(const D3DXVECTOR3& rPos, const EType type);
	static CListManager<CTutorial>* GetList(void); // ���X�g�擾

private:

	// �ÓI�����o�ϐ�
	static CListManager<CTutorial>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CTutorial>::AIterator m_iterator; // �C�e���[�^�[
	float m_fGapRate; // �h��͈̔�

};
