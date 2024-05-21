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
#define KEY_MOVE_UP		(DIK_UP)	// �ړ��ʏ㏸�L�[
#define NAME_MOVE_UP	("��")		// �ړ��ʏ㏸�\��
#define KEY_MOVE_DOWN	(DIK_DOWN)	// �ړ��ʉ��~�L�[
#define NAME_MOVE_DOWN	("��")		// �ړ��ʉ��~�\��

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
	const float INIT_MOVE	= 10.0f;	// �z�u���̏����ړ���
	const float CHANGE_MOVE	= 2.0f;		// �z�u���̈ړ��ʂ̕ϓ���
	const float MIN_MOVE	= 10.0f;	// �z�u���̍ŏ��ړ���
	const float MAX_MOVE	= 100.0f;	// �z�u���̍ő�ړ���
}

//************************************************************
//	�e�N���X [CEditStage] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditStage::CEditStage(CEditManager *pEditManager)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pEditManager = pEditManager;	// �G�f�B�b�g�}�l�[�W���[
	m_fMove	= 0.0f;			// �ʒu�ړ���
	m_pos	= VEC3_ZERO;	// �ʒu

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
	m_fMove	= INIT_MOVE;	// �ʒu�ړ���
	m_pos	= VEC3_ZERO;	// �ʒu

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEditStage::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CEditStage::Update(void)
{
	// �ړ��ʂ̍X�V
	UpdateChangeMove();

	// �ʒu�̍X�V
	UpdatePosition();
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditStage::DrawDebugControl(void)
{
	DebugProc::Print(DebugProc::POINT_RIGHT, "�ړ��F[%s/%s/%s/%s/%s/%s+%s]\n", NAME_FAR, NAME_LEFT, NAME_NEAR, NAME_RIGHT, NAME_UP, NAME_DOWN, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�ړ��ʕύX�F[%s/%s+%s]\n", NAME_MOVE_UP, NAME_MOVE_DOWN, NAME_DOUBLE);
	DebugProc::Print(DebugProc::POINT_RIGHT, "--------------------------------------\n");
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditStage::DrawDebugInfo(void)
{
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f�F[�ړ���]\n", m_fMove);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f�F[�ʒu]\n", m_pos.x, m_pos.y, m_pos.z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "--------------------------------------\n");
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
//	�G�f�B�b�g�}�l�[�W���[�擾����
//============================================================
CEditManager *CEditStage::GetPtrEditManager(void) const
{
#if _DEBUG

	// �G�f�B�b�g�}�l�[�W���[��Ԃ�
	return m_pEditManager;

#else	// NDEBUG

	// nullptr��Ԃ�
	return nullptr;

#endif	// _DEBUG
}

//============================================================
//	�ړ��ʂ̍X�V����
//============================================================
void CEditStage::UpdateChangeMove(void)
{
	// �ړ��ʂ�ύX
	CInputKeyboard *m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���
	if (m_pKeyboard->IsPress(KEY_DOUBLE))
	{
		if (!m_pKeyboard->IsPress(KEY_TRIGGER))
		{
			if (m_pKeyboard->IsPress(KEY_MOVE_UP))
			{
				m_fMove += CHANGE_MOVE;
			}
			if (m_pKeyboard->IsPress(KEY_MOVE_DOWN))
			{
				m_fMove -= CHANGE_MOVE;
			}
		}
		else
		{
			if (m_pKeyboard->IsTrigger(KEY_MOVE_UP))
			{
				m_fMove += CHANGE_MOVE;
			}
			if (m_pKeyboard->IsTrigger(KEY_MOVE_DOWN))
			{
				m_fMove -= CHANGE_MOVE;
			}
		}
	}

	// �ړ��ʂ�␳
	useful::LimitNum(m_fMove, MIN_MOVE, MAX_MOVE);
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
			m_pos.z += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_NEAR))
		{
			m_pos.z -= m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_RIGHT))
		{
			m_pos.x += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_LEFT))
		{
			m_pos.x -= m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_UP))
		{
			m_pos.y += m_fMove;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN))
		{
			m_pos.y -= m_fMove;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_FAR))
		{
			m_pos.z += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_NEAR))
		{
			m_pos.z -= m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_RIGHT))
		{
			m_pos.x += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_LEFT))
		{
			m_pos.x -= m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP))
		{
			m_pos.y += m_fMove;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN))
		{
			m_pos.y -= m_fMove;
		}
	}
}
