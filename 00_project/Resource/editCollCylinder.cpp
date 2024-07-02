#if 1
//============================================================
//
//	�G�f�B�b�g�R���W�����V�����_�[���� [editCollCylinder.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editCollCylinder.h"
#include "editManager.h"
#include "manager.h"
#include "useful.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_UP_RADIUS		(DIK_T)	// ���a�g��L�[
#define NAME_UP_RADIUS		("T")	// ���a�g��\��
#define KEY_DOWN_RADIUS		(DIK_G)	// ���a�k���L�[
#define NAME_DOWN_RADIUS	("G")	// ���a�k���\��
#define KEY_UP_HEIGHT		(DIK_Y)	// �����g��L�[
#define NAME_UP_HEIGHT		("Y")	// �����g��\��
#define KEY_DOWN_HEIGHT		(DIK_H)	// �����k���L�[
#define NAME_DOWN_HEIGHT	("H")	// �����k���\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float INIT_RADIUS = 20.0f;	// �����̔��a
	const float INIT_HEIGHT = 20.0f;	// �����̍���
	const float	INIT_ALPHA = 0.5f;		// �z�u�O�̃��l
	const float SCALING = 1.0f;			// �����蔻��̊g�k��
}

//************************************************************
//	�e�N���X [CEditCollCylinder] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditCollCylinder::CEditCollCylinder(const int nIdx) : CEditorCollShape(nIdx)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pCylinder = nullptr;	// �V�����_�[���
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// �R���W�����V�����_�[�z�u���
	m_bSave = false;	// �ۑ���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditCollCylinder::~CEditCollCylinder()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditCollCylinder::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pCylinder = nullptr;					// �V�����_�[���
	m_infoCreate.fRadius = INIT_RADIUS;		// ���a
	m_infoCreate.fHeight = INIT_HEIGHT;		// ����
	m_bSave = false;						// �ۑ���

	// �e�N���X�̏�����
	if (FAILED(CEditorCollShape::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �R���W�����V�����_�[�̐���
	m_pCylinder = CCollisionCylinder::Create
	( // ����
		VEC3_ZERO,				// �ʒu
		GetVec3OffSet(),		// �I�t�Z�b�g���W
		m_infoCreate.fRadius,	// ���a
		m_infoCreate.fHeight	// ����
	);
	if (m_pCylinder == nullptr)
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
void CEditCollCylinder::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorCollShape::Uninit();

	// �R���W�����V�����_�[�̏I��
	SAFE_UNINIT(m_pCylinder);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditCollCylinder::Update(void)
{
#if _DEBUG

	// �e�N���X�̍X�V
	CEditorCollShape::Update();

	// �傫���̍X�V
	UpdateSize();

	// �ʒu�𔽉f
	m_pCylinder->SetOffset(m_pCylinder->GetPos());

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditCollCylinder::IsSave(void)
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
void CEditCollCylinder::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditCollCylinder::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditCollCylinder::DrawDebugControl(void)
{
#if _DEBUG

	// ����\���̕`��
	CEditorCollShape::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "���a�F[%s/%s+%s]\n", NAME_UP_RADIUS, NAME_DOWN_RADIUS, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "�����F[%s/%s+%s]\n", NAME_UP_HEIGHT, NAME_DOWN_HEIGHT, NAME_TRIGGER);

#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditCollCylinder::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorCollShape::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f�F[���a]\n", m_infoCreate.fRadius);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f�F[����]\n", m_infoCreate.fHeight);

#endif	// _DEBUG
}

//============================================================
//	�ʒu�X�V����
//============================================================
void CEditCollCylinder::UpdateOffset(void)
{
	// �����̍X�V
	CEditorCollShape::UpdateOffset();

#if _DEBUG

	// �����𔽉f
	m_pCylinder->GetTube()->SetVec3Position(GetVec3OffSet());

#endif	// _DEBUG
}

//============================================================
//	�傫���̍X�V����
//============================================================
void CEditCollCylinder::UpdateSize(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// �傫����ύX
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_RADIUS))
		{
			m_infoCreate.fRadius += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_RADIUS))
		{
			m_infoCreate.fRadius -= SCALING;
		}
		if (pKeyboard->IsPress(KEY_UP_HEIGHT))
		{
			m_infoCreate.fHeight += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_HEIGHT))
		{
			m_infoCreate.fHeight -= SCALING;
		}
	}
	else
	{
		if (pKeyboard->IsTrigger(KEY_UP_RADIUS))
		{
			m_infoCreate.fRadius += SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_RADIUS))
		{
			m_infoCreate.fRadius -= SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_UP_HEIGHT))
		{
			m_infoCreate.fHeight += SCALING;
		}
		if (pKeyboard->IsTrigger(KEY_DOWN_HEIGHT))
		{
			m_infoCreate.fHeight -= SCALING;
		}
	}

	// �傫����␳
	useful::LimitMinNum(m_infoCreate.fRadius, SCALING);
	useful::LimitMinNum(m_infoCreate.fHeight, SCALING);

#ifdef _DEBUG

	// �傫���𔽉f
	m_pCylinder->GetTube()->SetRadius(m_infoCreate.fRadius);
	m_pCylinder->GetTube()->SetHeight(m_infoCreate.fHeight);

#endif // _DEBUG
}

//============================================================
//	��������
//============================================================
void CEditCollCylinder::Create(void)
{
#ifdef _DEBUG

	//----------------------------------------------------
	//	�R���W�����V�����_�[�̏���z�u�p�ɕύX
	//----------------------------------------------------
	// �����X�V�E�����`���ON�ɂ���LABEL��ύX����
	m_pCylinder->GetTube()->SetEnableUpdate(true);
	m_pCylinder->GetTube()->SetEnableDraw(true);
	m_pCylinder->GetTube()->SetLabel(CObject::LABEL_DEBUG);

#endif // _DEBUG

	// ���ۑ���ݒ�
	m_bSave = false;

	//----------------------------------------------------
	//	�V�����R���W�����V�����_�[�̐���
	//----------------------------------------------------
	// �R���W�����V�����_�[�̐���
	m_pCylinder = CCollisionCylinder::Create
	( // ����
		VEC3_ZERO,				// �ʒu
		GetVec3OffSet(),		// �I�t�Z�b�g���W
		m_infoCreate.fRadius,	// ���a
		m_infoCreate.fHeight	// ����
	);
	assert(m_pCylinder != nullptr);
}

#endif
