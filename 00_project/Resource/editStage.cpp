//============================================================
//
//	�G�f�B�b�g�X�e�[�W���� [editStage.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editStage.h"
#include "manager.h"
#include "editManager.h"
#include "editField.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_CHANGE_OBJECT	(DIK_2)	// �I�u�W�F�N�g�^�C�v�ύX�L�[
#define NAME_CHANGE_OBJECT	("2")	// �I�u�W�F�N�g�^�C�v�ύX�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TYPE_NAME[] =	// �I�u�W�F�N�g�^�C�v��
	{
		"�n��",
		"��",
	};
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TYPE_NAME) == CEditStage::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�e�N���X [CEditStage] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditStage::CEditStage(CEditManager *pEditManager) : CEditor(pEditManager)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pEditor = nullptr;		// �G�f�B�^�[���
	m_typeObject = TYPE_FIELD;	// �I�u�W�F�N�g�^�C�v

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditStage::~CEditStage()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditStage::Init(void)
{
	// �����o�ϐ���������
	m_pEditor = nullptr;		// �G�f�B�^�[���
	m_typeObject = TYPE_FIELD;	// �I�u�W�F�N�g�^�C�v

	// �e�N���X�̏�����
	if (FAILED(CEditor::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �G�f�B�^�[���̐���
	m_pEditor = CEditorObject::Create(GetPtrEditManager(), m_typeObject);
	if (m_pEditor == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEditStage::Uninit(void)
{
	// �e�N���X�̏I��
	CEditor::Uninit();

	// �G�f�B�^�[���̔j��
	SAFE_REF_RELEASE(m_pEditor);
}

//============================================================
//	�X�V����
//============================================================
void CEditStage::Update(void)
{
	// �e�N���X�̍X�V
	CEditor::Update();

	// �I�u�W�F�N�g�^�C�v�̕ύX
	ChangeObjectType();

	// �G�f�B�^�[���̍X�V
	assert(m_pEditor != nullptr);
	m_pEditor->Update();
}

//============================================================
//	���ۑ�����
//============================================================
void CEditStage::SaveInfo(void)
{
	// �G�f�B�^�[���̏��ۑ�
	assert(m_pEditor != nullptr);
	m_pEditor->SaveInfo();
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditStage::LoadInfo(void)
{
	// �G�f�B�^�[���̏��Ǎ�
	assert(m_pEditor != nullptr);
	m_pEditor->LoadInfo();
}

//============================================================
//	�ۑ�����
//============================================================
void CEditStage::Save(FILE *pFile)
{
#if _DEBUG

	// �G�f�B�^�[���̕ۑ�
	assert(m_pEditor != nullptr);
	m_pEditor->Save(pFile);

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditStage::DrawDebugControl(void)
{
	// ����\���̕`��
	CEditor::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "�G�f�B�b�g�X�e�[�W�^�C�v�ύX�F[%s]\n", NAME_CHANGE_OBJECT);

	// �G�f�B�^�[���̑���\��
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugControl();
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditStage::DrawDebugInfo(void)
{
	// ���\���̕`��
	CEditor::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%s�F[�G�f�B�b�g�X�e�[�W�^�C�v]\n", TYPE_NAME[m_typeObject]);

	// �G�f�B�^�[���̏��\��
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugInfo();
}

//============================================================
//	�I�u�W�F�N�g�^�C�v�̕ύX����
//============================================================
void CEditStage::ChangeObjectType(void)
{
	// �I�u�W�F�N�g�^�C�v�̕ύX
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	if (pKeyboard->IsTrigger(KEY_CHANGE_OBJECT))
	{
		// �G�f�B�^�[���̔j��
		SAFE_REF_RELEASE(m_pEditor);

		// �G�f�B�b�g�^�C�v�̕ύX
		m_typeObject = (CEditStage::EType)((m_typeObject + 1) % CEditStage::TYPE_MAX);

		if (m_pEditor == nullptr)
		{
			// �G�f�B�^�[���̐���
			m_pEditor = CEditorObject::Create(GetPtrEditManager(), m_typeObject);
			assert(m_pEditor != nullptr);	// �������s
		}
	}
}
