#pragma once
//=========================================
//
//  �f�o�b�O��p�I�u�W�F�N�g(��Ώ���) [debug_object.h]
//  Author�FTomoya Kanazaki
//
//=========================================
#include "object3D.h"
#include "listManager.h"

//=========================================
//  �N���X��`
//=========================================
class CDebugObject : public CObject3D
{
public:

	// �R���X�g���N�^
	CDebugObject();

	// �f�X�g���N�^
	virtual ~CDebugObject() override;

	// �I�[�o�[���C�h�֐�
	virtual HRESULT Init() override; // ������
	virtual void Uninit() override; // �I��
	virtual void Update(const float fDeltaTime) override; // �X�V
	virtual void Draw(CShader* pShader = nullptr) override; // �`��

	bool Hit(const D3DXVECTOR3& rPos);

	// �ÓI�����o�֐�
	static CDebugObject* Create(); // ��������
	static CListManager<CDebugObject>* GetList(); // ���X�g�擾

private:

	// �����o�ϐ�
	CListManager<CDebugObject>::AIterator m_iterator; // �C�e���[�^�[

	// �ÓI�����o�ϐ�
	static CListManager<CDebugObject>* m_pList; // �I�u�W�F�N�g���X�g
};
