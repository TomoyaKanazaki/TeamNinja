//============================================================
//
//	�����蔻��G�f�B�^�[���� [editorCollShape.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "editorCollShape.h"
#include "editManager.h"

#include "editCollCube.h"
#include "editCollCylinder.h"
#include "editCollSphere.h"
#include "actor.h"

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
#define KEY_CREATE	(DIK_0)	// �����L�[
#define NAME_CREATE	("0")	// �����\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float MOVE = 1.0f;		// �ړ���
}

//************************************************************
//	�e�N���X [CEditorCollShape] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditorCollShape::CEditorCollShape(const int nIdx)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_offset = VEC3_ZERO;		// �I�t�Z�b�g���W

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditorCollShape::~CEditorCollShape()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditorCollShape::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_offset = VEC3_ZERO;		// �I�t�Z�b�g���W

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
void CEditorCollShape::Uninit(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditorCollShape::Update(void)
{
#if _DEBUG

	// �I�t�Z�b�g���W�̍X�V
	UpdateOffset();

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditorCollShape::DrawDebugControl(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "�ړ��F[%s/%s/%s/%s/%s/%s+%s]\n", NAME_FAR, NAME_LEFT, NAME_NEAR, NAME_RIGHT, NAME_UP, NAME_DOWN, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�ݒu�F[%s]\n", NAME_CREATE);

#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditorCollShape::DrawDebugInfo(void)
{
#if _DEBUG

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f %f %f�F[�I�t�Z�b�g]\n", m_offset.x, m_offset.y, m_offset.z);

#endif	// _DEBUG
}

//============================================================
//	��������
//============================================================
CEditorCollShape* CEditorCollShape::Create(CEditCollision::EType type, const int nIdx)
{
#if _DEBUG

	// �|�C���^��錾
	CEditorCollShape* pEditorObject = nullptr;	// �G�f�B�^�[�I�u�W�F�N�g���

	// �G�f�B�^�[�I�u�W�F�N�g�̐���
	switch (type)
	{
	case CEditCollision::TYPE_CUBE:
		pEditorObject = new CEditCollCube(nIdx);		// �G�f�B�b�g�R���W�����L���[�u
		break;
	case CEditCollision::TYPE_CYLINDER:
		pEditorObject = new CEditCollCylinder(nIdx);	// �G�f�B�b�g�R���W�����V�����_�[
		break;
	case CEditCollision::TYPE_SPHERE:
		pEditorObject = new CEditCollSphere(nIdx);		// �G�f�B�b�g�R���W�����X�t�B�A
		break;
	default:		// ��O����
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
void CEditorCollShape::Release(CEditorCollShape*& prEditorObject)
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
//	�I�t�Z�b�g���W�̍X�V����
//============================================================
void CEditorCollShape::UpdateOffset(void)
{
	// �ʒu��ύX
	CInputKeyboard* m_pKeyboard = CManager::GetInstance()->GetKeyboard();	// �L�[�{�[�h���
	if (!m_pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (m_pKeyboard->IsPress(KEY_FAR))
		{
			m_offset.z += MOVE;
		}
		if (m_pKeyboard->IsPress(KEY_NEAR))
		{
			m_offset.z -= MOVE;
		}
		if (m_pKeyboard->IsPress(KEY_RIGHT))
		{
			m_offset.x += MOVE;
		}
		if (m_pKeyboard->IsPress(KEY_LEFT))
		{
			m_offset.x -= MOVE;
		}
		if (m_pKeyboard->IsPress(KEY_UP))
		{
			m_offset.y += MOVE;
		}
		if (m_pKeyboard->IsPress(KEY_DOWN))
		{
			m_offset.y -= MOVE;
		}
	}
	else
	{
		if (m_pKeyboard->IsTrigger(KEY_FAR))
		{
			m_offset.z += MOVE;
		}
		if (m_pKeyboard->IsTrigger(KEY_NEAR))
		{
			m_offset.z -= MOVE;
		}
		if (m_pKeyboard->IsTrigger(KEY_RIGHT))
		{
			m_offset.x += MOVE;
		}
		if (m_pKeyboard->IsTrigger(KEY_LEFT))
		{
			m_offset.x -= MOVE;
		}
		if (m_pKeyboard->IsTrigger(KEY_UP))
		{
			m_offset.y += MOVE;
		}
		if (m_pKeyboard->IsTrigger(KEY_DOWN))
		{
			m_offset.y -= MOVE;
		}
	}
}
