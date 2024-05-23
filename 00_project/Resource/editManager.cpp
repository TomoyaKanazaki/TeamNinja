//============================================================
//
//	�G�f�B�b�g�}�l�[�W���[���� [editManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editManager.h"
#include "manager.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_SAVE	(DIK_F9)	// �ۑ��L�[
#define NAME_SAVE	("F9")		// �ۑ��\��
#define KEY_CHANGE_EDITOR	(DIK_1)	// �G�f�B�b�g�^�C�v�ύX�L�[
#define NAME_CHANGE_EDITOR	("1")	// �G�f�B�b�g�^�C�v�ύX�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SAVE_TXT = "data\\TXT\\save_stage.txt";	// �X�e�[�W�Z�[�u�e�L�X�g

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
//	�e�N���X [CEditManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditManager::CEditManager()
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pEditor	= nullptr;	// �G�f�B�^�[���
	m_bSave		= false;	// �ۑ���
	m_bEdit		= false;	// �G�f�B�b�g��
	m_type		= CEditor::TYPE_STAGE;	// �G�f�B�^�[�^�C�v


#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditManager::~CEditManager()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditManager::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pEditor	= nullptr;	// �G�f�B�^�[���
	m_bSave		= false;	// �ۑ���
	m_bEdit		= false;	// �G�f�B�b�g��
	m_type		= CEditor::TYPE_STAGE;	// �G�f�B�^�[�^�C�v

	// ������Ԃ�
	return S_OK;

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	�I������
//============================================================
void CEditManager::Uninit(void)
{
#if _DEBUG

	// �G�f�B�^�[���̔j��
	SAFE_REF_RELEASE(m_pEditor);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditManager::Update(void)
{
#if _DEBUG

	// �G�f�B�b�g���[�h����Ȃ��ꍇ������
	if (!m_bEdit) { return; }

	// �G�f�B�^�[�^�C�v�ύX
	ChangeEditorType();

	// �G�f�B�^�[���̍X�V
	assert(m_pEditor != nullptr);
	m_pEditor->Update();

	// �ۑ�����
	Save();

	// ����\���̕`��
	DrawDebugControl();

	// ���\���̕`��
	DrawDebugInfo();

#endif	// _DEBUG
}

//============================================================
//	���ۑ��̐ݒ菈��
//============================================================
void CEditManager::UnSave(void)
{
	// �ۑ����Ă��Ȃ��󋵂ɂ���
	m_bSave = false;
}

//============================================================
//	�G�f�B�b�g�󋵂̐ؑ֏���
//============================================================
void CEditManager::SwitchEnableEdit(void)
{
	// �G�f�B�b�g�󋵂𔽓]
	SetEnableEdit(!m_bEdit);
}

//============================================================
//	�G�f�B�b�g�󋵂̐ݒ菈��
//============================================================
void CEditManager::SetEnableEdit(const bool bEdit)
{
	// �����̃G�f�B�b�g�󋵂ɂ���
	m_bEdit = bEdit;

	if (m_bEdit)
	{ // �G�f�B�b�gON�̏ꍇ

		// �G�f�B�b�g�X�e�[�W�̐���
		if (m_pEditor == nullptr)
		{ // �G�f�B�b�g�X�e�[�W���g�p����Ă��Ȃ��ꍇ

			m_pEditor = CEditor::Create(this, m_type);
			assert(m_pEditor != nullptr);	// �������s
		}

		// ���Ǎ�
		m_pEditor->LoadInfo();
	}
	else
	{ // �G�f�B�b�gOFF�̏ꍇ

		// ���ۑ�
		m_pEditor->SaveInfo();

		// �G�f�B�b�g�X�e�[�W�̔j��
		SAFE_REF_RELEASE(m_pEditor);
	}
}

//============================================================
//	��������
//============================================================
CEditManager *CEditManager::Create(void)
{
#if _DEBUG

	// �G�f�B�^�[�}�l�[�W���[�̐���
	CEditManager *pEditManager = new CEditManager;
	if (pEditManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�f�B�^�[�}�l�[�W���[�̏�����
		if (FAILED(pEditManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�f�B�^�[�}�l�[�W���[�̔j��
			SAFE_DELETE(pEditManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEditManager;
	}

#else	// NDEBUG

	// nullptr��Ԃ�
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	�j������
//============================================================
void CEditManager::Release(CEditManager *&prEditManager)
{
#if _DEBUG

	// �G�f�B�b�g�}�l�[�W���[�̏I��
	assert(prEditManager != nullptr);
	prEditManager->Uninit();

	// �������J��
	SAFE_DELETE(prEditManager);

#endif	// _DEBUG
}

//============================================================
//	�G�f�B�^�[�^�C�v�̕ύX����
//============================================================
void CEditManager::ChangeEditorType(void)
{
	// �G�f�B�^�[�^�C�v�̕ύX
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	if (pKeyboard->IsTrigger(KEY_CHANGE_EDITOR))
	{
		// �G�f�B�^�[���̔j��
		SAFE_REF_RELEASE(m_pEditor);

		// �G�f�B�^�[�^�C�v�̕ύX
		m_type = (CEditor::EType)((m_type + 1) % CEditor::TYPE_MAX);

		if (m_pEditor == nullptr)
		{
			// �G�f�B�^�[���̐���
			m_pEditor = CEditor::Create(this, m_type);
			assert(m_pEditor != nullptr);	// �������s
		}
	}
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditManager::DrawDebugControl(void)
{
	DebugProc::Print(DebugProc::POINT_RIGHT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "[�G�f�B�b�g����]�@\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "�X�e�[�W�ۑ��F[%s+%s]\n", NAME_DOUBLE, NAME_SAVE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�G�f�B�b�g�^�C�v�ύX�F[%s]\n", NAME_CHANGE_EDITOR);

	// �G�f�B�^�[���̑���\��
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugControl();
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditManager::DrawDebugInfo(void)
{
	DebugProc::Print(DebugProc::POINT_RIGHT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "[�G�f�B�b�g���]�@\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "======================================\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, (m_bSave) ? "�ۑ��ρF[�ۑ���]\n" : "���ۑ��F[�ۑ���]\n");
	DebugProc::Print(DebugProc::POINT_RIGHT, "%s�F[�G�f�B�^�[�^�C�v]\n", TYPE_NAME[m_type]);

	// �G�f�B�^�[���̏��\��
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugInfo();
}

//============================================================
//	�ۑ�����
//============================================================
void CEditManager::Save(void)
{
	// �X�e�[�W��ۑ�
	CInputKeyboard *pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���
	if (pKeyboard->IsPress(KEY_DOUBLE))
	{
		if (pKeyboard->IsTrigger(KEY_SAVE))
		{
			// ���݂̃G�f�B�^�[�^�C�v����ۑ�
			assert(m_pEditor != nullptr);
			m_pEditor->Save();

			// �ۑ�������Ԃɂ���
			m_bSave = true;
		}
	}
}
