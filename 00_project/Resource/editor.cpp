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
//	�}�N����`
//************************************************************
#define NAME_CHANGE_EDITOR	("1")	// �G�f�B�b�g�^�C�v�ύX�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TYPE_NAME[] =	// �G�f�B�b�g�^�C�v��
	{
		"�X�e�[�W",
		"�����蔻��",
	};
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TYPE_NAME) == CEditor::TYPE_MAX, "ERROR : Type Count Mismatch");

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
//	����������
//============================================================
HRESULT CEditor::Init(void)
{
	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEditor::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CEditor::Update(void)
{

}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditor::DrawDebugControl(void)
{
	DebugProc::Print(DebugProc::POINT_RIGHT, "�G�f�B�b�g�^�C�v�ύX�F[%s]\n", NAME_CHANGE_EDITOR);
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditor::DrawDebugInfo(void)
{
	DebugProc::Print(DebugProc::POINT_RIGHT, "%s�F[�G�f�B�b�g�^�C�v]\n", TYPE_NAME[m_pEditManager->GetTypeEditor()]);
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
		pEditor = CEditStage::Create(pEditManager, CEditStage::TYPE_FIELD);	// �G�f�B�b�g�X�e�[�W
		break;

	case TYPE_COLLISION:

		// TODO�F�G�f�B�b�g�����蔻��ł�����u������
		//pEditor = CEditCollision;	// �G�f�B�b�g�����蔻��
		pEditor = CEditStage::Create(pEditManager, CEditStage::TYPE_FIELD);	// �G�f�B�b�g�X�e�[�W

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

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

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
