#if 1
//============================================================
//
//	�G�f�B�b�g�R���W�����X�t�B�A���� [editCollSphere.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editCollSphere.h"
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

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float INIT_RADIUS = 20.0f;	// �����̔��a
	const float	INIT_ALPHA = 0.5f;		// �z�u�O�̃��l
	const float SCALING = 1.0f;			// �����蔻��̊g�k��
}

//************************************************************
//	�e�N���X [CEditCollSphere] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditCollSphere::CEditCollSphere(const int nIdx) : CEditorCollShape(nIdx)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pSphere = nullptr;	// �X�t�B�A���
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// �R���W�����X�t�B�A�z�u���
	m_bSave = false;	// �ۑ���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditCollSphere::~CEditCollSphere()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditCollSphere::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pSphere = nullptr;					// �X�t�B�A���
	m_infoCreate.fRadius = INIT_RADIUS;		// ���a
	m_bSave = false;						// �ۑ���

	// �e�N���X�̏�����
	if (FAILED(CEditorCollShape::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �R���W�����X�t�B�A�̐���
	m_pSphere = CCollisionSphere::Create
	( // ����
		VEC3_ZERO,				// �ʒu
		GetVec3OffSet(),		// �I�t�Z�b�g���W
		m_infoCreate.fRadius	// ���a
	);
	if (m_pSphere == nullptr)
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
void CEditCollSphere::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorCollShape::Uninit();

	// �R���W�����X�t�B�A�̏I��
	SAFE_UNINIT(m_pSphere);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditCollSphere::Update(void)
{
#if _DEBUG

	// �e�N���X�̍X�V
	CEditorCollShape::Update();

	// �傫���̍X�V
	UpdateSize();

	// �ʒu�𔽉f
	m_pSphere->SetOffset(m_pSphere->GetPos());

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditCollSphere::IsSave(void)
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
void CEditCollSphere::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditCollSphere::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditCollSphere::DrawDebugControl(void)
{
#if _DEBUG

	// ����\���̕`��
	CEditorCollShape::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "���a�F[%s/%s+%s]\n", NAME_UP_RADIUS, NAME_DOWN_RADIUS, NAME_TRIGGER);

#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditCollSphere::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorCollShape::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f�F[���a]\n", m_infoCreate.fRadius);

#endif	// _DEBUG
}

//============================================================
//	�ʒu�X�V����
//============================================================
void CEditCollSphere::UpdateOffset(void)
{
	// �����̍X�V
	CEditorCollShape::UpdateOffset();

#if _DEBUG

	// �����𔽉f
	m_pSphere->GetSphere()->SetVec3Position(GetVec3OffSet());

#endif	// _DEBUG
}

//============================================================
//	�傫���̍X�V����
//============================================================
void CEditCollSphere::UpdateSize(void)
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
	}

	// �傫����␳
	useful::LimitMinNum(m_infoCreate.fRadius, SCALING);

#ifdef _DEBUG

	// �傫���𔽉f
	m_pSphere->GetSphere()->SetRadius(m_infoCreate.fRadius);

#endif // _DEBUG
}

//============================================================
//	��������
//============================================================
void CEditCollSphere::Create(void)
{
#ifdef _DEBUG

	//----------------------------------------------------
	//	�R���W�����X�t�B�A�̏���z�u�p�ɕύX
	//----------------------------------------------------
	// �����X�V�E�����`���ON�ɂ���LABEL��ύX����
	m_pSphere->GetSphere()->SetEnableUpdate(true);
	m_pSphere->GetSphere()->SetEnableDraw(true);
	m_pSphere->GetSphere()->SetLabel(CObject::LABEL_DEBUG);

#endif // _DEBUG

	// ���ۑ���ݒ�
	m_bSave = false;

	//----------------------------------------------------
	//	�V�����R���W�����X�t�B�A�̐���
	//----------------------------------------------------
	// �R���W�����X�t�B�A�̐���
	m_pSphere = CCollisionSphere::Create
	( // ����
		VEC3_ZERO,				// �ʒu
		GetVec3OffSet(),		// �I�t�Z�b�g���W
		m_infoCreate.fRadius	// ���a
	);
	assert(m_pSphere != nullptr);
}

#endif
