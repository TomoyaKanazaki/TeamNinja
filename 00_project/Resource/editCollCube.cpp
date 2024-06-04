#if 1
//============================================================
//
//	�G�f�B�b�g�R���W�����L���[�u���� [editCollCube.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editCollCube.h"
#include "editManager.h"
#include "manager.h"
#include "useful.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_UP_WIDTH		(DIK_T)	// ���g��L�[
#define NAME_UP_WIDTH		("T")	// ���g��\��
#define KEY_DOWN_WIDTH		(DIK_G)	// ���k���L�[
#define NAME_DOWN_WIDTH		("G")	// ���k���\��
#define KEY_UP_HEIGHT		(DIK_Y)	// �����g��L�[
#define NAME_UP_HEIGHT		("Y")	// �����g��\��
#define KEY_DOWN_HEIGHT		(DIK_H)	// �����k���L�[
#define NAME_DOWN_HEIGHT	("H")	// �����k���\��
#define KEY_UP_DEPTH		(DIK_U)	// ���s�g��L�[
#define NAME_UP_DEPTH		("U")	// ���s�g��\��
#define KEY_DOWN_DEPTH		(DIK_J)	// ���s�k���L�[
#define NAME_DOWN_DEPTH		("J")	// ���s�k���\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float INIT_WIDTH = 20.0f;		// �����̕�
	const float INIT_HEIGHT = 20.0f;	// �����̍���
	const float INIT_DEPTH = 20.0f;		// �����̉��s
	const float	INIT_ALPHA = 0.5f;		// �z�u�O�̃��l
	const float SCALING = 1.0f;			// �����蔻��̊g�k��
}

//************************************************************
//	�e�N���X [CEditCollCube] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditCollCube::CEditCollCube(const int nIdx) : CEditorCollShape(nIdx)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pCube = nullptr;	// �L���[�u���
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// �R���W�����L���[�u�z�u���
	m_bSave = false;	// �ۑ���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditCollCube::~CEditCollCube()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditCollCube::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pCube = nullptr;						// �L���[�u���
	m_infoCreate.fWidth = INIT_WIDTH;		// ��
	m_infoCreate.fHeight = INIT_HEIGHT;		// ����
	m_infoCreate.fDepth = INIT_DEPTH;		// ���s
	m_bSave = false;						// �ۑ���

	// �e�N���X�̏�����
	if (FAILED(CEditorCollShape::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �R���W�����L���[�u�̐���
	m_pCube = CCollisionCube::Create
	( // ����
		VEC3_ZERO,				// �ʒu
		GetVec3OffSet(),		// �I�t�Z�b�g���W
		m_infoCreate.fWidth,	// ��
		m_infoCreate.fHeight,	// ����
		m_infoCreate.fDepth		// ���s
	);
	if (m_pCube == nullptr)
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
void CEditCollCube::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorCollShape::Uninit();

	// �R���W�����L���[�u�̏I��
	SAFE_UNINIT(m_pCube);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditCollCube::Update(void)
{
#if _DEBUG

	// �e�N���X�̍X�V
	CEditorCollShape::Update();

	// �傫���̍X�V
	UpdateSizing();

	// �ʒu�𔽉f
	m_pCube->SetOffset(m_pCube->GetPos());

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditCollCube::IsSave(void)
{
#if _DEBUG

	// �ۑ��󋵂�Ԃ�
	return m_bSave;

#else	// NDEBUG

	// false��Ԃ�
	return false;

#endif	// _DEBUG
}

//============================================================
//	���ۑ�����
//============================================================
void CEditCollCube::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditCollCube::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditCollCube::DrawDebugControl(void)
{
#if _DEBUG

	// ����\���̕`��
	CEditorCollShape::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "���F[%s/%s+%s]\n", NAME_UP_WIDTH, NAME_DOWN_WIDTH, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�����F[%s/%s+%s]\n", NAME_UP_HEIGHT, NAME_DOWN_HEIGHT, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "���s�F[%s/%s+%s]\n", NAME_UP_DEPTH, NAME_DOWN_DEPTH, NAME_TRIGGER);

#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditCollCube::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorCollShape::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f�F[��]\n", m_infoCreate.fWidth);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f�F[����]\n", m_infoCreate.fHeight);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f�F[���s]\n", m_infoCreate.fDepth);

#endif	// _DEBUG
}

//============================================================
// ��������
//============================================================
void CEditCollCube::Create(void)
{
#ifdef _DEBUG

	//----------------------------------------------------
	//	�R���W�����L���[�u�̏���z�u�p�ɕύX
	//----------------------------------------------------
	// �����X�V�E�����`���ON�ɂ���LABEL��ύX����
	m_pCube->GetCube()->SetEnableUpdate(true);
	m_pCube->GetCube()->SetEnableDraw(true);
	m_pCube->GetCube()->SetLabel(CObject::LABEL_DEBUG);

#endif // _DEBUG

	// ���ۑ���ݒ�
	m_bSave = false;

	//----------------------------------------------------
	//	�V�����R���W�����L���[�u�̐���
	//----------------------------------------------------
	// �R���W�����L���[�u�̐���
	m_pCube = CCollisionCube::Create
	( // ����
		VEC3_ZERO,				// �ʒu
		GetVec3OffSet(),		// �I�t�Z�b�g���W
		m_infoCreate.fWidth,	// ���a
		m_infoCreate.fHeight,	// ����
		m_infoCreate.fDepth		// ���s
	);
	assert(m_pCube != nullptr);
}

//============================================================
//	�ʒu�X�V����
//============================================================
void CEditCollCube::UpdateOffset(void)
{
	// �����̍X�V
	CEditorCollShape::UpdateOffset();

#if _DEBUG

	// �����𔽉f
	m_pCube->GetCube()->SetVec3Position(GetVec3OffSet());

#endif	// _DEBUG
}

//============================================================
//	�傫���̍X�V����
//============================================================
void CEditCollCube::UpdateSizing(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// �傫����ύX
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_WIDTH))
		{
			m_infoCreate.fWidth += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_WIDTH))
		{
			m_infoCreate.fWidth -= SCALING;
		}
		if (pKeyboard->IsPress(KEY_UP_HEIGHT))
		{
			m_infoCreate.fHeight += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_HEIGHT))
		{
			m_infoCreate.fHeight -= SCALING;
		}
		if (pKeyboard->IsPress(KEY_UP_DEPTH))
		{
			m_infoCreate.fDepth += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_DEPTH))
		{
			m_infoCreate.fDepth -= SCALING;
		}
	}
	else
	{
		if (pKeyboard->IsTrigger(KEY_UP_WIDTH))
		{
			m_infoCreate.fWidth += SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_WIDTH))
		{
			m_infoCreate.fWidth -= SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_UP_HEIGHT))
		{
			m_infoCreate.fHeight += SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_HEIGHT))
		{
			m_infoCreate.fHeight -= SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_UP_DEPTH))
		{
			m_infoCreate.fDepth += SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_DEPTH))
		{
			m_infoCreate.fDepth -= SCALING;
		}
	}

	// �傫����␳
	useful::LimitMinNum(m_infoCreate.fWidth, SCALING);
	useful::LimitMinNum(m_infoCreate.fHeight, SCALING);
	useful::LimitMinNum(m_infoCreate.fDepth, SCALING);

#ifdef _DEBUG

	// �傫���𔽉f
	m_pCube->GetCube()->SetVec3Sizing(D3DXVECTOR3
	(
		m_infoCreate.fWidth,
		m_infoCreate.fHeight,
		m_infoCreate.fDepth
	));

#endif // _DEBUG
}

#endif
