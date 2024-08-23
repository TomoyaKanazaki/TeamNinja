#pragma once
//===========================================
//
//  ���G�f�B�^�[(editWeed.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "editorObject.h"
#include "weed.h"

//==========================================
//  �O���錾
//==========================================
class CObjectMeshCylinder;

//===========================================
//  �N���X��`
//===========================================
class CEditWeed : public CEditorObject
{
public:

	// �I�u�W�F�N�g���\����
	struct SInfo
	{
		float fSize;	// �傫��
	};

	// �����o�֐�
	CEditWeed(CEditStage* pEditor);
	~CEditWeed() override;

	HRESULT Init(void) override; // ������
	void Uninit(void) override; // �I��
	void Update(void) override; // �X�V
	HRESULT Save(void) override; // �ۑ�
	bool IsSave(void) override; // �ۑ��󋵎擾
	void SaveInfo(void) override; // ���ۑ�
	void LoadInfo(void) override; // ���Ǎ�
	void DrawDebugControl(void) override; // ����\���`��
	void DrawDebugInfo(void) override; // ���\���`��

private:

	// �����o�֐�
	void UpdateSize(void); // �傫���X�V
	void UpdateTime(void); //�����p�x�X�V
	void UpdateNum(void); // �������X�V
	void Create(void); // ������
	void Release(void); // ���j��
	void DeleteCollision(const bool bRelase); // ���̍폜����

	// �����o�ϐ�
	CObjectMeshCylinder* m_pCylinder; // �I�u�W�F�N�g���
	SInfo m_infoCreate; // �I�u�W�F�N�g�z�u���
	bool m_bSave; // �ۑ���
	int m_nCoolTime; // �����p�x
	int m_nNum; // ������
};
