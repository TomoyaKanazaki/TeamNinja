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
#include "editorObject.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_CHANGE_OBJECT	(DIK_2)	// �I�u�W�F�N�g�^�C�v�ύX�L�[
#define NAME_CHANGE_OBJECT	("2")	// �I�u�W�F�N�g�^�C�v�ύX�\��

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

#define KEY_ROTA_RIGHT	(DIK_Z)	// �E��]�L�[
#define NAME_ROTA_RIGHT	("Z")	// �E��]�\��
#define KEY_ROTA_LEFT	(DIK_C)	// ����]�L�[
#define NAME_ROTA_LEFT	("C")	// ����]�\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TYPE_NAME[] =	// �I�u�W�F�N�g�^�C�v��
	{
		"�n��",
		"��",
		"�A�N�^�[",
		"�`�F�b�N�|�C���g",
		"�S�[���|�C���g",
		"�M�~�b�N",
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
	m_pEditor	= nullptr;		// �G�f�B�^�[���
	m_type		= TYPE_FIELD;	// �I�u�W�F�N�g�^�C�v
	m_pos		= VEC3_ZERO;	// �ʒu
	m_rot		= VEC3_ZERO;	// ����
	m_angle		= ANGLE_0;		// �p�x

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
#if _DEBUG

	// �����o�ϐ���������
	m_pEditor	= nullptr;		// �G�f�B�^�[���
	m_type		= TYPE_FIELD;	// �I�u�W�F�N�g�^�C�v
	m_pos		= VEC3_ZERO;	// �ʒu
	m_rot		= VEC3_ZERO;	// ����
	m_angle		= ANGLE_0;		// �p�x

	// �G�f�B�^�[���̐���
	m_pEditor = CEditorObject::Create(this, m_type);
	if (m_pEditor == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

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
void CEditStage::Uninit(void)
{
#if _DEBUG

	// �G�f�B�^�[���̔j��
	SAFE_REF_RELEASE(m_pEditor);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditStage::Update(void)
{
#if _DEBUG

	// �I�u�W�F�N�g�^�C�v�̕ύX
	ChangeObjectType();

	// �G�f�B�^�[���̍X�V
	assert(m_pEditor != nullptr);
	m_pEditor->Update();

#endif	// _DEBUG
}

//============================================================
//	�ۑ�����
//============================================================
HRESULT CEditStage::Save(void)
{
#if _DEBUG

	// �G�f�B�^�[���̕ۑ�
	assert(m_pEditor != nullptr);
	if (FAILED(m_pEditor->Save()))
	{ // �Z�[�u�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;

#else	// NDEBUG

	// ������Ԃ�
	return S_OK;

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditStage::IsSave(void)
{
#if _DEBUG

	// �G�f�B�^�[���̕ۑ�
	assert(m_pEditor != nullptr);
	return m_pEditor->IsSave();

#else	// NDEBUG

	// false��Ԃ�
	return false;

#endif	// _DEBUG
}

//============================================================
//	���ۑ�����
//============================================================
void CEditStage::SaveInfo(void)
{
#if _DEBUG

	// �G�f�B�^�[���̏��ۑ�
	assert(m_pEditor != nullptr);
	m_pEditor->SaveInfo();

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditStage::LoadInfo(void)
{
#if _DEBUG

	// �G�f�B�^�[���̏��Ǎ�
	assert(m_pEditor != nullptr);
	m_pEditor->LoadInfo();

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditStage::DrawDebugControl(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "�G�f�B�b�g�X�e�[�W�^�C�v�ύX�F[%s]\n", NAME_CHANGE_OBJECT);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�ړ��F[%s/%s/%s/%s/%s/%s+%s]\n", NAME_FAR, NAME_LEFT, NAME_NEAR, NAME_RIGHT, NAME_UP, NAME_DOWN, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "��]�F[%s/%s]\n", NAME_ROTA_RIGHT, NAME_ROTA_LEFT);

	// �G�f�B�^�[���̑���\��
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugControl();

#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditStage::DrawDebugInfo(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "%s�F[�G�f�B�b�g�X�e�[�W�^�C�v]\n", TYPE_NAME[m_type]);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f�F[�ʒu]\n", m_pos.x, m_pos.y, m_pos.z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f�F[����]\n", m_rot.x, m_rot.y, m_rot.z);

	// �G�f�B�^�[���̏��\��
	assert(m_pEditor != nullptr);
	m_pEditor->DrawDebugInfo();

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

//============================================================
//	�����X�V����
//============================================================
void CEditStage::UpdateRotation(void)
{
	// ������ύX
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���
	if (m_pKeyboard->IsTrigger(KEY_ROTA_RIGHT))
	{
		// �p�x������]
		m_angle = (EAngle)((m_angle + (ANGLE_MAX - 1)) % ANGLE_MAX);

		// ���݂̊p�x����������v�Z
		int nTemp = ((m_angle - 1) + (ANGLE_MAX - 1)) % ANGLE_MAX;
		m_rot.y = ((float)nTemp * HALF_PI) - D3DX_PI;
	}
	if (m_pKeyboard->IsTrigger(KEY_ROTA_LEFT))
	{
		// �p�x���E��]
		m_angle = (EAngle)((m_angle + 1) % ANGLE_MAX);

		// ���݂̊p�x����������v�Z
		int nTemp = ((m_angle - 1) + (ANGLE_MAX - 1)) % ANGLE_MAX;
		m_rot.y = ((float)nTemp * HALF_PI) - D3DX_PI;
	}

	// �����𐳋K��
	useful::NormalizeRot(m_rot);
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

		// �I�u�W�F�N�g�^�C�v�̕ύX
		m_type = (CEditStage::EType)((m_type + 1) % CEditStage::TYPE_MAX);

		if (m_pEditor == nullptr)
		{
			// �G�f�B�^�[���̐���
			m_pEditor = CEditorObject::Create(this, m_type);
			assert(m_pEditor != nullptr);	// �������s
		}
	}
}
