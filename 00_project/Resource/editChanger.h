#pragma once
//===========================================
//
//  �J�������ύX�n�_�G�f�B�^�[(editChanger.h)
//  Author : Tomoya Kanazaki
//
//===========================================
#include "editorObject.h"
#include "camera_change.h"

//===========================================
//  �N���X��`
//===========================================
class CEditChanger : public CEditorObject
{
public:

	// �I�u�W�F�N�g���\����
	struct SInfo
	{
		D3DXVECTOR3 size;	// �傫��
		CCameraChanger::EDirection dir;	// ���
		CCameraChanger::ERotation rot;	// ���
	};

	// �����o�֐�
	CEditChanger(CEditStage* pEditor);
	~CEditChanger() override;

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
	void UpdateRotation(void) override;	// �����X�V

	void UpdateSize(void); // �傫���X�V
	void ChangeDirection(void); // �����ύX
	void ChangeRotation(void); // �p�x�ύX
	void CreateField(void); // �t�B�[���h����
	void ReleaseField(void); // �t�B�[���h�j��
	void DeleteCollisionField(const bool bRelase); // �t�B�[���h�̍폜����

	// �����o�ϐ�
	CCameraChanger* m_pChanger; // �I�u�W�F�N�g���
	SInfo m_infoCreate; // �I�u�W�F�N�g�z�u���
	bool m_bSave; // �ۑ���
};
