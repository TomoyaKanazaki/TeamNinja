#if 1
//============================================================
//
//	�G�f�B�b�g�R���W�����|���S������ [editCollPolygon.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "editCollPolygon.h"
#include "editManager.h"
#include "manager.h"
#include "useful.h"

//************************************************************
//	�}�N����`
//************************************************************
#define KEY_UP_WIDTH		(DIK_T)	// X���g��L�[
#define NAME_UP_WIDTH		("T")	// X���g��\��
#define KEY_DOWN_WIDTH		(DIK_G)	// X���k���L�[
#define NAME_DOWN_WIDTH		("G")	// X���k���\��
#define KEY_UP_DEPTH		(DIK_Y)	// Y���g��L�[
#define NAME_UP_DEPTH		("Y")	// Y���g��\��
#define KEY_DOWN_DEPTH		(DIK_H)	// Y���k���L�[
#define NAME_DOWN_DEPTH		("H")	// Y���k���\��

#define KEY_UP_ROT_X		(DIK_U)	// X���g��L�[
#define NAME_UP_ROT_X		("U")	// X���g��\��
#define KEY_DOWN_ROT_X		(DIK_J)	// X���k���L�[
#define NAME_DOWN_ROT_X		("J")	// X���k���\��
#define KEY_UP_ROT_Z		(DIK_I)	// Z���g��L�[
#define NAME_UP_ROT_Z		("I")	// Z���g��\��
#define KEY_DOWN_ROT_Z		(DIK_K)	// Z���k���L�[
#define NAME_DOWN_ROT_Z		("K")	// Z���k���\��

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float INIT_RADIUS = 20.0f;	// �����̔��a
	const float	INIT_ALPHA = 0.5f;		// �z�u�O�̃��l
	const float SCALING = 1.0f;			// �����蔻��̊g�k��
	const float ROT = 0.01f;			// �����̊g�k��
}

//************************************************************
//	�e�N���X [CEditCollPolygon] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEditCollPolygon::CEditCollPolygon(const int nIdx) : CEditorCollShape(nIdx)
{
#if _DEBUG

	// �����o�ϐ����N���A
	m_pPolygon = nullptr;	// �|���S�����
	memset(&m_infoCreate, 0, sizeof(m_infoCreate));	// �R���W�����|���S���z�u���
	m_bSave = false;	// �ۑ���

#endif	// _DEBUG
}

//============================================================
//	�f�X�g���N�^
//============================================================
CEditCollPolygon::~CEditCollPolygon()
{
#if _DEBUG
#endif	// _DEBUG
}

//============================================================
//	����������
//============================================================
HRESULT CEditCollPolygon::Init(void)
{
#if _DEBUG

	// �����o�ϐ���������
	m_pPolygon = nullptr;				// �|���S�����
	m_infoCreate.rot = VEC3_ZERO;		// ����
	m_infoCreate.size = VEC3_ZERO;		// �T�C�Y
	m_bSave = false;					// �ۑ���

	// �e�N���X�̏�����
	if (FAILED(CEditorCollShape::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �R���W�����|���S���̐���
	m_pPolygon = CCollisionPolygon::Create
	( // ����
		VEC3_ZERO,			// �ʒu
		GetVec3OffSet(),	// �I�t�Z�b�g���W
		m_infoCreate.rot,	// ����
		m_infoCreate.size	// �T�C�Y
	);
	if (m_pPolygon == nullptr)
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
void CEditCollPolygon::Uninit(void)
{
#if _DEBUG

	// �e�N���X�̏I��
	CEditorCollShape::Uninit();

	// �R���W�����|���S���̏I��
	SAFE_UNINIT(m_pPolygon);

#endif	// _DEBUG
}

//============================================================
//	�X�V����
//============================================================
void CEditCollPolygon::Update(void)
{
#if _DEBUG

	// �e�N���X�̍X�V
	CEditorCollShape::Update();

	// �����̍X�V
	UpdateRot();

	// �傫���̍X�V
	UpdateSize();

	// �ʒu�𔽉f
	m_pPolygon->SetOffset(m_pPolygon->GetPos());

#endif	// _DEBUG
}

//============================================================
//	�ۑ��󋵎擾����
//============================================================
bool CEditCollPolygon::IsSave(void)
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
void CEditCollPolygon::SaveInfo(void)
{
#if _DEBUG

	// ���݂̏���ۑ�
	//m_save = m_infoCreate;

#endif	// _DEBUG
}

//============================================================
//	���Ǎ�����
//============================================================
void CEditCollPolygon::LoadInfo(void)
{
#if _DEBUG

	// �ۑ�����ݒ�
	//m_infoCreate = m_save;

#endif	// _DEBUG
}

//============================================================
//	����\���̕`�揈��
//============================================================
void CEditCollPolygon::DrawDebugControl(void)
{
#if _DEBUG

	// ����\���̕`��
	CEditorCollShape::DrawDebugControl();

	DebugProc::Print(DebugProc::POINT_RIGHT, "X����]�F[%s/%s+%s]\n", NAME_UP_ROT_X, NAME_DOWN_ROT_X, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "Z����]�F[%s/%s+%s]\n", NAME_UP_ROT_Z, NAME_DOWN_ROT_Z, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "���F[%s/%s+%s]\n", NAME_UP_WIDTH, NAME_DOWN_WIDTH, NAME_TRIGGER);
	DebugProc::Print(DebugProc::POINT_RIGHT, "���s�F[%s/%s+%s]\n", NAME_UP_DEPTH, NAME_DOWN_DEPTH, NAME_TRIGGER);

#endif	// _DEBUG
}

//============================================================
//	���\���̕`�揈��
//============================================================
void CEditCollPolygon::DrawDebugInfo(void)
{
#if _DEBUG

	// ���\���̕`��
	CEditorCollShape::DrawDebugInfo();

	DebugProc::Print(DebugProc::POINT_RIGHT, "%f�F[X��]\n", m_infoCreate.rot.x);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f�F[Y��]\n", m_infoCreate.rot.y);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f�F[Z��]\n", m_infoCreate.rot.z);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f�F[��]\n", m_infoCreate.size.x);
	DebugProc::Print(DebugProc::POINT_RIGHT, "%f�F[���s]\n", m_infoCreate.size.z);

#endif	// _DEBUG
}

//============================================================
//	�ʒu�X�V����
//============================================================
void CEditCollPolygon::UpdateOffset(void)
{
	// �����̍X�V
	CEditorCollShape::UpdateOffset();

#if _DEBUG

	// �I�t�Z�b�g�𔽉f
	m_pPolygon->GetPolygon()->SetVec3Position(GetVec3OffSet());

#endif	// _DEBUG
}

//============================================================
// �����X�V
//============================================================
void CEditCollPolygon::UpdateRot(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// ������ύX
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_ROT_X))
		{
			m_infoCreate.rot.x += ROT;
		}
		if (pKeyboard->IsPress(KEY_DOWN_ROT_X))
		{
			m_infoCreate.rot.x -= ROT;
		}

		if (pKeyboard->IsPress(KEY_UP_ROT_Z))
		{
			m_infoCreate.rot.z += ROT;
		}
		if (pKeyboard->IsPress(KEY_DOWN_ROT_Z))
		{
			m_infoCreate.rot.z -= ROT;
		}
	}
	else
	{
		if (pKeyboard->IsPress(KEY_UP_ROT_X))
		{
			m_infoCreate.rot.x += ROT;
		}
		if (pKeyboard->IsPress(KEY_DOWN_ROT_X))
		{
			m_infoCreate.rot.x -= ROT;
		}

		if (pKeyboard->IsPress(KEY_UP_ROT_Z))
		{
			m_infoCreate.rot.z += ROT;
		}
		if (pKeyboard->IsPress(KEY_DOWN_ROT_Z))
		{
			m_infoCreate.rot.z -= ROT;
		}
	}

#ifdef _DEBUG

	// �����𔽉f
	m_pPolygon->GetPolygon()->SetVec3Rotation(m_infoCreate.rot);

#endif // _DEBUG
}

//============================================================
//	�傫���̍X�V����
//============================================================
void CEditCollPolygon::UpdateSize(void)
{
	CInputKeyboard* pKeyboard = GET_INPUTKEY;	// �L�[�{�[�h���

	// �傫����ύX
	if (!pKeyboard->IsPress(KEY_TRIGGER))
	{
		if (pKeyboard->IsPress(KEY_UP_WIDTH))
		{
			m_infoCreate.size.x += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_WIDTH))
		{
			m_infoCreate.size.x -= SCALING;
		}

		if (pKeyboard->IsPress(KEY_UP_DEPTH))
		{
			m_infoCreate.size.z += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_DEPTH))
		{
			m_infoCreate.size.z -= SCALING;
		}
	}
	else
	{
		if (pKeyboard->IsPress(KEY_UP_WIDTH))
		{
			m_infoCreate.size.x += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_WIDTH))
		{
			m_infoCreate.size.x -= SCALING;
		}

		if (pKeyboard->IsPress(KEY_UP_DEPTH))
		{
			m_infoCreate.size.z += SCALING;
		}
		if (pKeyboard->IsPress(KEY_DOWN_DEPTH))
		{
			m_infoCreate.size.z -= SCALING;
		}
	}

	// �傫����␳
	useful::LimitMinNum(m_infoCreate.size.x, SCALING);
	useful::LimitMinNum(m_infoCreate.size.z, SCALING);

#ifdef _DEBUG

	// �傫���𔽉f
	m_pPolygon->GetPolygon()->SetVec3Sizing(m_infoCreate.size);

#endif // _DEBUG
}

//============================================================
//	��������
//============================================================
void CEditCollPolygon::Create(void)
{
#ifdef _DEBUG

	//----------------------------------------------------
	//	�R���W�����|���S���̏���z�u�p�ɕύX
	//----------------------------------------------------
	// �����X�V�E�����`���ON�ɂ���LABEL��ύX����
	m_pPolygon->GetPolygon()->SetEnableUpdate(true);
	m_pPolygon->GetPolygon()->SetEnableDraw(true);
	m_pPolygon->GetPolygon()->SetLabel(CObject::LABEL_DEBUG);

#endif // _DEBUG

	// ���ۑ���ݒ�
	m_bSave = false;

	//----------------------------------------------------
	//	�V�����R���W�����|���S���̐���
	//----------------------------------------------------
	// �R���W�����|���S���̐���
	m_pPolygon = CCollisionPolygon::Create
	( // ����
		VEC3_ZERO,				// �ʒu
		GetVec3OffSet(),		// �I�t�Z�b�g���W
		m_infoCreate.rot,		// ����
		m_infoCreate.size		// �T�C�Y
	);
	assert(m_pPolygon != nullptr);
}

#endif
