//============================================================
//
//	�G�f�B�^�[�I�u�W�F�N�g���� [editorObject.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editorObject.h"
#include "manager.h"
#include "editManager.h"

#include "editField.h"
#include "editActor.h"
#include "editGimmick.h"

//************************************************************
//	�}�N����`
//************************************************************
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

}

//************************************************************
//	�e�N���X [CEditorObject] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditorObject::CEditorObject()
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pos = VEC3_ZERO;			// �ʒu
	m_rot = VEC3_ZERO;			// ����
	m_angle = CEditor::ANGLE_0;	// �p�x

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditorObject::~CEditorObject()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditorObject::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pos = VEC3_ZERO;			// �ʒu
	m_rot = VEC3_ZERO;			// ����
	m_angle = CEditor::ANGLE_0;	// �p�x

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
void CEditorObject::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditorObject::Update(void)
{
#if _DEBUG

	// �ʒu�̍X�V
	UpdatePosition();

	// �����̍X�V
	UpdateRotation();

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditorObject::DrawDebugControl(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "�ړ��F[%s/%s/%s/%s/%s/%s+%s]\n", NAME_FAR, NAME_LEFT, NAME_NEAR, NAME_RIGHT, NAME_UP, NAME_DOWN, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "��]�F[%s/%s]\n", NAME_ROTA_RIGHT, NAME_ROTA_LEFT);

#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditorObject::DrawDebugInfo(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f�F[�ʒu]\n", m_pos.x, m_pos.y, m_pos.z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f�F[����]\n", m_rot.x, m_rot.y, m_rot.z);

#endif	// _DEBUG
}

//============================================================
//	��������
//============================================================
CEditorObject *CEditorObject::Create(CEditStage::EType type)
{
#if _DEBUG

	// �|�C���^��錾
	CEditorObject *pEditorObject = nullptr;	// �G�f�B�^�[�I�u�W�F�N�g���

	// �G�f�B�^�[�I�u�W�F�N�g�̐���
	switch (type)
	{ // ��ނ��Ƃ̏���
	case CEditStage::TYPE_FIELD:
		pEditorObject = new CEditField;		// �G�f�B�b�g�t�B�[���h
		break;

	case CEditStage::TYPE_WALL:

		// TODO�F�G�f�B�b�g�E�H�[���ł�����u������
		pEditorObject = new CEditField;	// �G�f�B�b�g�t�B�[���h
		//pEditorObject = new CEditWall;	// �G�f�B�b�g�E�H�[��

		break;

	case CEditStage::TYPE_ACTOR:
		pEditorObject = new CEditActor;		// �G�f�B�b�g�A�N�^�[
		break;

	case CEditStage::TYPE_GIMMICK:
		pEditorObject = new CEditGimmick;	// �G�f�B�b�g�M�~�b�N
		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (pEditorObject == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�f�B�^�[�I�u�W�F�N�g�̏�����
		if (FAILED(pEditorObject->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�f�B�^�[�I�u�W�F�N�g�̔j��
			SAFE_DELETE(pEditorObject);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEditorObject;
	}

#else	// NDEBUG

	// nullptr��Ԃ�
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	�j������
//============================================================
void CEditorObject::Release(CEditorObject *&prEditorObject)
{
#if _DEBUG

	// �G�f�B�^�[�I�u�W�F�N�g�̏I��
	assert(prEditorObject != nullptr);
	prEditorObject->Uninit();

	// �������J��
	SAFE_DELETE(prEditorObject);

#endif	// _DEBUG
}

//============================================================
//	�ʒu�̍X�V����
//============================================================
void CEditorObject::UpdatePosition(void)
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
void CEditorObject::UpdateRotation(void)
{
	// ������ύX
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���
	if (m_pKeyboard->IsTrigger(KEY_ROTA_RIGHT))
	{
		// �p�x������]
		m_angle = (CEditor::EAngle)((m_angle + (CEditor::ANGLE_MAX - 1)) % CEditor::ANGLE_MAX);

		// ���݂̊p�x����������v�Z
		int nTemp = ((m_angle - 1) + (CEditor::ANGLE_MAX - 1)) % CEditor::ANGLE_MAX;
		m_rot.y = ((float)nTemp * HALF_PI) - D3DX_PI;
	}
	if (m_pKeyboard->IsTrigger(KEY_ROTA_LEFT))
	{
		// �p�x���E��]
		m_angle = (CEditor::EAngle)((m_angle + 1) % CEditor::ANGLE_MAX);

		// ���݂̊p�x����������v�Z
		int nTemp = ((m_angle - 1) + (CEditor::ANGLE_MAX - 1)) % CEditor::ANGLE_MAX;
		m_rot.y = ((float)nTemp * HALF_PI) - D3DX_PI;
	}

	// �����𐳋K��
	useful::NormalizeRot(m_rot);
}
