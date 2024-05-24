//============================================================
//
//	�G�f�B�^�[���� [editor.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editor.h"
#include "manager.h"
#include "editManager.h"
#include "editStage.h"

//************************************************************
//	�e�N���X [CEditor] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditor::CEditor(CEditManager *pEditManager)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pEditManager = pEditManager;	// �G�f�B�b�g�}�l�[�W���[

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditor::~CEditor()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	��������
//============================================================
CEditor *CEditor::Create(CEditManager *pEditManager, EType type)
{
#if _DEBUG

	// �|�C���^��錾
	CEditor *pEditor = nullptr;	// �G�f�B�^�[���

	// �G�f�B�^�[�̐���
	switch (type)
	{ // ��ނ��Ƃ̏���
	case TYPE_STAGE:
		pEditor = new CEditStage(pEditManager);	// �G�f�B�b�g�X�e�[�W
		break;

	case TYPE_COLLISION:

		// TODO�F�G�f�B�b�g�����蔻��ł�����u������
		//pEditor = new CEditCollision(pEditManager);	// �G�f�B�b�g�����蔻��
		pEditor = new CEditStage(pEditManager);	// �G�f�B�b�g�X�e�[�W

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (pEditor == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�f�B�^�[�̏�����
		if (FAILED(pEditor->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�f�B�^�[�̔j��
			SAFE_DELETE(pEditor);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEditor;
	}

#else	// NDEBUG

	// nullptr��Ԃ�
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	�j������
//============================================================
void CEditor::Release(CEditor *&prEditor)
{
#if _DEBUG

	// �G�f�B�^�[�̏I��
	assert(prEditor != nullptr);
	prEditor->Uninit();

	// �������J��
	SAFE_DELETE(prEditor);

#endif	// _DEBUG
}

//============================================================
//	�G�f�B�b�g�}�l�[�W���[�擾����
//============================================================
CEditManager *CEditor::GetPtrEditManager(void) const
{
#if _DEBUG

	// �G�f�B�b�g�}�l�[�W���[��Ԃ�
	return m_pEditManager;

#else	// NDEBUG

	// nullptr��Ԃ�
	return nullptr;

#endif	// _DEBUG
}
