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
		TYPE_GUARD, // �K�[�h
		TYPE_DOGDE, // ���
		TYPE_MAX, // �ő吔
	};

	// �����o�֐�
	CTutorial();
	~CTutorial();

	HRESULT Init(void) override; // ����������
	void Uninit(void) override; // �I������
	void Update(const float fDeltaTime) override; // �X�V����
	void Draw(CShader* pShader = nullptr) override; // �`�揈��

	// �ÓI�����o�֐�
	static CTutorial* Create(const D3DXVECTOR3& rPos, const EType type);
	static CListManager<CTutorial>* GetList(); // ���X�g�擾

private:

	// �ÓI�����o�ϐ�
	static CListManager<CTutorial>* m_pList; // �I�u�W�F�N�g���X�g

	// �����o�֐�
	void BindTexture(const char* pTexturePass) override; // �e�N�X�`������ (�p�X)
	void Scaling(const float fDeltaTime); // �X�P�[�����O�ύX����

	// �����o�ϐ�
	CListManager<CTutorial>::AIterator m_iterator; // �C�e���[�^�[
	D3DXVECTOR3 m_sizeDefault; // �����T�C�Y
	bool m_bIn; // ��������
	float m_fTime; // �ړ�����

};
