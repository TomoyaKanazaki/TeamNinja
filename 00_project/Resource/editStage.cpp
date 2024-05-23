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
#define NAME_CHANGE_STAGE	("2")	// �G�f�B�b�g�X�e�[�W�^�C�v�ύX�\��

#define KEY_FAR		(DIK_W)	// ���ړ��L�[
#define NAME_FAR	("W")	// ���ړ��\��
#define KEY_NEAR	(DIK_S)	// ��O�ړ��L�[
#define NAME_NEAR	("S")	// ��O�ړ��\��
#define KEY_RIGHT	(DIK_D)	// �E�ړ��L�[
#define NAME_RIGHT	("D")	// �E�ړ��\��
#define KEY_LEFT	(DIK_A)	// ���ړ��L�[
#define NAME_LEFT	("A")	// ���ړ��\��
#define KEY_UP		(DIK_E)	// ��ړ��L�[
#define NAME_UP		("E")	// ��ړ��\��
#define KEY_DOWN	(DIK_Q)	// ���ړ��L�[
#define NAME_DOWN	("Q")	// ���ړ��\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TYPE_NAME[] =	// �G�f�B�b�g�X�e�[�W�^�C�v��
	{
		"�n��",
		"��",
	};
}

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
	m_pos = VEC3_ZERO;	// �ʒu

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
	m_pos = VEC3_ZERO;	// �ʒu

	// �e�N���X�̏�����
	if (FAILED(CEditor::Init()))
	{ // �������Ɏ��s�����ꍇ

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
}

//============================================================
//	�X�V����
//============================================================
void CEditStage::Update(void)
{
	// �e�N���X�̍X�V
	CEditor::Update();

	// �ʒu�̍X�V
	UpdatePosition();
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditStage::DrawDebugControl(void)
{
	// ����\���̕`��
	CEditor::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "�G�f�B�b�g�X�e�[�W�^�C�v�ύX�F[%s]\n", NAME_CHANGE_STAGE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�ړ��F[%s/%s/%s/%s/%s/%s+%s]\n", NAME_FAR, NAME_LEFT, NAME_NEAR, NAME_RIGHT, NAME_UP, NAME_DOWN, NAME_TRIGGER);
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditStage::DrawDebugInfo(void)
{
	// ���\���̕`��
	CEditor::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%s�F[�G�f�B�b�g�X�e�[�W�^�C�v]\n", TYPE_NAME[GetPtrEditManager()->GetTypeEditor()]);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f�F[�ʒu]\n", m_pos.x, m_pos.y, m_pos.z);
}

//============================================================
//	��������
//============================================================
CEditStage *CEditStage::Create(CEditManager *pEditManager, EType type)
{
#if _DEBUG

	// �|�C���^��錾
	CEditStage *pEditStage = nullptr;	// �G�f�B�b�g�X�e�[�W���

	// �G�f�B�b�g�X�e�[�W�̐���
	switch (type)
	{ // ��ނ��Ƃ̏���
	case TYPE_FIELD:
		pEditStage = new CEditField(pEditManager);	// �G�f�B�b�g�t�B�[���h
		break;

	case TYPE_WALL:

		// TODO�F�G�f�B�b�g�t�B�[���h�ł�����u������
		pEditStage = new CEditField(pEditManager);	// �G�f�B�b�g�t�B�[���h
		//pEditStage = new CEditWall(pEditManager);	// �G�f�B�b�g�E�H�[��

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (pEditStage == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�f�B�b�g�X�e�[�W�̏�����
		if (FAILED(pEditStage->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�f�B�b�g�X�e�[�W�̔j��
			SAFE_DELETE(pEditStage);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEditStage;
	}

#else	// NDEBUG

	// nullptr��Ԃ�
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	�j������
//============================================================
void CEditStage::Release(CEditStage *&prEditStage)
{
#if _DEBUG

	// �G�f�B�b�g�X�e�[�W�̏I��
	assert(prEditStage != nullptr);
	prEditStage->Uninit();

	// �������J��
	SAFE_DELETE(prEditStage);

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	�ʒu�̍X�V����
//============================================================
void CEditStage::UpdatePosition(void)
{
	// �ʒu��ύX
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_FAR))
		{
			m_pos.z += editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_NEAR))
		{
			m_pos.z -= editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_RIGHT))
		{
			m_pos.x += editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_LEFT))
		{
			m_pos.x -= editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_UP))
		{
			m_pos.y += editstage::SIZE;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN))
		{
			m_pos.y -= editstage::SIZE;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_FAR))
		{
			m_pos.z += editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_NEAR))
		{
			m_pos.z -= editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_RIGHT))
		{
			m_pos.x += editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_LEFT))
		{
			m_pos.x -= editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP))
		{
			m_pos.y += editstage::SIZE;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN))
		{
			m_pos.y -= editstage::SIZE;
		}
	}
}
