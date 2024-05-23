//============================================================
//
//	�G�f�B�b�g�}�l�[�W���[�w�b�_�[ [editManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EDIT_MANAGER_H_
#define _EDIT_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editor.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_DOUBLE		(DIK_LCONTROL)	// ��d���L�[
#define NAME_DOUBLE		("LCTRL")		// ��d���\��
#define KEY_TRIGGER		(DIK_LSHIFT)	// �g���K�[�L�[
#define NAME_TRIGGER	("LSHIFT")		// �g���K�[�\��
#define KEY_REVERSE		(DIK_LALT)		// ����t�]�L�[
#define NAME_REVERSE	("LALT")		// ����t�]�\��

//************************************************************
//	�N���X��`
//************************************************************
// �G�f�B�b�g�}�l�[�W���[�N���X
class CEditManager
{
public:
	// �R���X�g���N�^
	CEditManager();

	// �f�X�g���N�^
	~CEditManager();

	// �����o�֐�
	HRESULT Init(void);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	void SwitchEnableEdit(void);				// �G�f�B�b�g�󋵐ؑ�
	void SetEnableEdit(const bool bEdit);		// �G�f�B�b�g�󋵐ݒ�
	bool IsEdit(void) const { return m_bEdit; }	// �G�f�B�b�g�󋵎擾

	// �ÓI�����o�֐�
	static CEditManager *Create(void);	// ����
	static void Release(CEditManager *&prEditManager);	// �j��

private:
	// �����o�֐�
	void ChangeEditorType(void);	// �G�f�B�^�[�^�C�v�ύX
	void DrawDebugControl(void);	// ����\���`��
	void DrawDebugInfo(void);		// ���\���`��
	void Save(void);				// �ۑ�

	// �����o�ϐ�
	CEditor *m_pEditor;		// �G�f�B�^�[���
	bool m_bEdit;			// �G�f�B�b�g��
	CEditor::EType m_type;	// �G�f�B�^�[�^�C�v
};

#endif	// _EDIT_MANAGER_H_
