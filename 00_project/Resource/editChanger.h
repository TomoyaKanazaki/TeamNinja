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
	// �R���X�g���N�^
	CEditChanger(CEditStage* pEditor);

	// �f�X�g���N�^
	~CEditChanger() override;

	// �t�B�[���h���\����
	struct SInfo
	{
		CField::EType type;	// ���
		D3DXVECTOR2 size;	// �傫��
		POSGRID2 part;		// ������
		POSGRID2 texPart;	// �e�N�X�`��������
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	HRESULT Save(void) override;	// �ۑ�
	bool IsSave(void) override;		// �ۑ��󋵎擾
	void SaveInfo(void) override;	// ���ۑ�
	void LoadInfo(void) override;	// ���Ǎ�
	void DrawDebugControl(void) override;	// ����\���`��
	void DrawDebugInfo(void) override;		// ���\���`��

private:
	// �I�[�o�[���C�h�֐�
	void UpdateRotation(void) override;	// �����X�V

	// �����o�֐�
	void UpdateSizing(void);	// �傫���X�V
	void UpdateTexPart(void);	// �e�N�X�`�������X�V
	void ChangeType(void);		// ��ޕύX
	void CreateField(void);		// �t�B�[���h����
	void ReleaseField(void);	// �t�B�[���h�j��
	void DeleteCollisionField(const bool bRelase);	// �t�B�[���h�̍폜����
	void InitAllColorField(void);					// �t�B�[���h�̐F�S������

	// �����o�ϐ�
	CEditChanger* m_pChanger; // �I�u�W�F�N�g���
	SInfo m_infoCreate; // �I�u�W�F�N�g�z�u���
	bool m_bSave; // �ۑ���
};
