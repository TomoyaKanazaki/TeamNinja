#pragma once
//===========================================
//
//  �t�H�O�ύX�N���X(fog_change.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "objectMeshCube.h"

//===========================================
//  �N���X��`
//===========================================
class CFogChanger : public CObjectMeshCube
{
public:

	// �����o�֐�
	CFogChanger();
	~CFogChanger();

	HRESULT Init() override; // ����������
	void Uninit() override; // �I������
	void Update(const float fDeltaTime) override; // �X�V����
	void Draw(CShader* pShader = nullptr) override; // �`�揈��

	// �ÓI�����o�֐�
	static CFogChanger* Create(); // ��������
	static CListManager<CFogChanger>* GetList(void);	// ���X�g�擾

private:
	
	// �����o�֐�
	bool CollChange(); // �q�b�g����̎擾
	void Move(); // �ړ�

	// �����o�ϐ�
	CListManager<CFogChanger>::AIterator m_iterator; // �C�e���[�^�[

	// �ÓI�����o�ϐ�
	static CListManager<CFogChanger>* m_pList; // �I�u�W�F�N�g���X�g
};
