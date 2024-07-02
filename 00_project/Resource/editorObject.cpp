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
#include "editWall.h"
#include "editActor.h"
#include "editCheckPoint.h"
#include "editGoalPoint.h"
#include "editChanger.h"

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
CEditorObject::CEditorObject(CEditStage* pEditor)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pEditor = pEditor;	// �G�f�B�^�[���

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
#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditorObject::DrawDebugInfo(void)
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	��������
//============================================================
CEditorObject *CEditorObject::Create(CEditStage* pEditor, CEditStage::EType type)
{
#if _DEBUG

	// �|�C���^��錾
	CEditorObject *pEditorObject = nullptr;	// �G�f�B�^�[�I�u�W�F�N�g���

	// �G�f�B�^�[�I�u�W�F�N�g�̐���
	switch (type)
	{ // ��ނ��Ƃ̏���
	case CEditStage::TYPE_FIELD:
		pEditorObject = new CEditField(pEditor);		// �G�f�B�b�g�t�B�[���h
		break;

	case CEditStage::TYPE_WALL:
		pEditorObject = new CEditWall(pEditor);			// �G�f�B�b�g�E�H�[��
		break;

	case CEditStage::TYPE_ACTOR:
		pEditorObject = new CEditActor(pEditor);		// �G�f�B�b�g�A�N�^�[
		break;

	case CEditStage::TYPE_CHECKPOINT:
		pEditorObject = new CEditCheckPoint(pEditor);	// �G�f�B�b�g�`�F�b�N�|�C���g
		break;

	case CEditStage::TYPE_GOALPOINT:
		pEditorObject = new CEditGoalPoint(pEditor);	// �G�f�B�b�g�S�[���|�C���g
		break;

	case CEditStage::TYPE_GIMMICK:
		pEditorObject = new CEditActor(pEditor);		// �G�f�B�b�g�M�~�b�N TODO�F���Ƃł��
		break;

	case CEditStage::TYPE_CHANGER:
		pEditorObject = new CEditChanger(pEditor);		// �G�f�B�b�g�J�����ύX�n�_
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
	// �ʒu�̍X�V
	m_pEditor->UpdatePosition();
}

//============================================================
//	�����X�V����
//============================================================
void CEditorObject::UpdateRotation(void)
{
	// �����̍X�V
	m_pEditor->UpdateRotation();
}
