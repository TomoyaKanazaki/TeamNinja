#if 0
//============================================================
//
//	�G�f�B�b�g�t�B�[���h�w�b�_�[ [editField.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_FIELD_H_
#define _EDIT_FIELD_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editStage.h"
#include "field.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�t�B�[���h�N���X
class CEditField : public CEditStage
{
public:
	// �R���X�g���N�^
	CEditField();

	// �f�X�g���N�^
	~CEditField() override;

	// �t�B�[���h���\����
	struct SInfo
	{
		CField::EType type;	// ���
		D3DXVECTOR2 size;	// �傫��
		D3DXCOLOR col;		// �F
		POSGRID2 part;		// ������
		POSGRID2 texPart;	// �e�N�X�`��������
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(void) override;		// �X�V
	void SaveInfo(void) override;	// ���ۑ�
	void LoadInfo(void) override;	// ���Ǎ�
	void DrawDebugControl(void) override;	// ����\���`��
	void DrawDebugInfo(void) override;		// ���\���`��
	void Save(FILE *pFile) override;		// �ۑ�

private:
	// �����o�֐�
	void UpdateSizing(void);	// �傫���X�V
	void UpdateTexPart(void);	// �e�N�X�`�������X�V
	void ChangeType(void);		// ��ޕύX
	void CreateField(void);		// �t�B�[���h����
	void ReleaseField(void);	// �t�B�[���h�j��

	void DeleteCollisionField(const bool bRelase);	// �t�B�[���h�̍폜����
	void InitAllColorField(void);					// �t�B�[���h�̐F�S������

	// �����o�ϐ�
	CField *m_pField;	// �t�B�[���h���
	SInfo m_infoCreate;	// �t�B�[���h�z�u���
};

#endif	// _EDIT_FIELD_H_
#endif
