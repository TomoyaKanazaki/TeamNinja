//============================================================
//
//	�^�C�g�����S2D���� [titleLogo2D.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "titleLogo2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// �^�C�g�����S2D�\���̗D�揇��

	namespace logo
	{
		const float	DEST_ALPHA	 = 1.0f;	// �ڕW�����x
		const float	INIT_ALPHA	 = 0.0f;	// ���������x
		const D3DXCOLOR DEST_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, DEST_ALPHA);	// �ڕW�F
		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// �����F
		const D3DXCOLOR DIFF_COL = DEST_COL - INIT_COL;						// �����F
	}

	namespace aura
	{
		const float WAIT_TIME	 = 0.25f;	// �ҋ@����
		const float MOVE_TIME	 = 1.25f;	// �ړ�����
		const float	DEST_ALPHA	 = 1.0f;	// �ڕW�����x
		const float	INIT_ALPHA	 = 0.0f;	// ���������x
		const D3DXCOLOR DEST_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, DEST_ALPHA);	// �ڕW�F
		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// �����F
		const D3DXCOLOR DIFF_COL = DEST_COL - INIT_COL;						// �����F
	}
}

//************************************************************
//	�e�N���X [CTitleLogo2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTitleLogo2D::CTitleLogo2D() : CAnim2D(CObject::LABEL_UI, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY),
	m_pAura		(nullptr),		// �I�[�����
	m_state		(STATE_NONE),	// ���
	m_fMoveTime	(0.0f),			// �ړ�����
	m_fWaitTime	(0.0f),			// �ҋ@����
	m_fCurTime	(0.0f),			// ���݂̑ҋ@����
	m_offset	(VEC3_ZERO),	// �����ʒu�I�t�Z�b�g
	m_initPos	(VEC3_ZERO),	// �����ʒu
	m_destPos	(VEC3_ZERO)		// �ڕW�ʒu
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTitleLogo2D::~CTitleLogo2D()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTitleLogo2D::Init(void)
{
	// �����o�ϐ���������
	m_pAura		= nullptr;		// �I�[�����
	m_state		= STATE_NONE;	// ���
	m_fMoveTime	= 0.0f;			// �ړ�����
	m_fWaitTime	= 0.0f;			// �ҋ@����
	m_fCurTime	= 0.0f;			// ���݂̑ҋ@����
	m_offset	= VEC3_ZERO;	// �����ʒu�I�t�Z�b�g
	m_initPos	= VEC3_ZERO;	// �����ʒu
	m_destPos	= VEC3_ZERO;	// �ڕW�ʒu

	// �A�j���[�V����2D�̏�����
	if (FAILED(CAnim2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������ݒ�
	SetVec3Rotation(VEC3_ZERO);

	// �F��ݒ�
	SetColor(XCOL_AWHITE);

	// �I�[���̐���
	m_pAura = CAnim2D::Create(1, 1, VEC3_ZERO);
	if (m_pAura == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������ݒ�
	m_pAura->SetVec3Rotation(VEC3_ZERO);

	// �F��ݒ�
	m_pAura->SetColor(XCOL_AWHITE);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTitleLogo2D::Uninit(void)
{
	// �I�[���̏I��
	SAFE_UNINIT(m_pAura);

	// �A�j���[�V����2D�̏I��
	CAnim2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CTitleLogo2D::Update(const float fDeltaTime)
{
	// �������Ȃ���Ԃ̏ꍇ������
	if (m_state == STATE_NONE) { return; }

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_MOVE_WAIT:

		// �ړ��ҋ@�̍X�V
		UpdateMoveWait(fDeltaTime);
		break;

	case STATE_MOVE:

		// �ړ��̍X�V
		UpdateMove(fDeltaTime);
		break;

	case STATE_AURA_WAIT:

		// �I�[���ҋ@�̍X�V
		UpdateAuraWait(fDeltaTime);
		break;

	case STATE_AURA:

		// �I�[���̍X�V
		UpdateAura(fDeltaTime);
		break;

	default:
		assert(false);
		break;
	}

	// �I�[���̍X�V
	m_pAura->Update(fDeltaTime);

	// �A�j���[�V����2D�̍X�V
	CAnim2D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CTitleLogo2D::Draw(CShader *pShader)
{
	// �A�j���[�V����2D�̕`��
	CAnim2D::Draw(pShader);
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CTitleLogo2D::SetVec3Position(const D3DXVECTOR3& rPos)
{
	// �e�̈ʒu��ݒ�
	CAnim2D::SetVec3Position(rPos);

	// �I�[���̈ʒu��ݒ�
	m_pAura->SetVec3Position(rPos);
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CTitleLogo2D::SetVec3Sizing(const D3DXVECTOR3& rSize)
{
	// �e�̑傫����ݒ�
	CAnim2D::SetVec3Sizing(rSize);

	// �I�[���̑傫����ݒ�
	m_pAura->SetVec3Sizing(rSize);
}

//============================================================
//	�p�^�[���̐ݒ菈��
//============================================================
void CTitleLogo2D::SetPattern(const int nPattern)
{
	// �e�̃p�^�[����ݒ�
	CAnim2D::SetPattern(nPattern);

	// �I�[���̃p�^�[����ݒ�
	m_pAura->SetPattern(nPattern);
}

//============================================================
//	�e�N�X�`���̉��������̐ݒ菈��
//============================================================
void CTitleLogo2D::SetWidthPattern(const int nWidthPtrn)
{
	// �e�̃e�N�X�`������������ݒ�
	CAnim2D::SetWidthPattern(nWidthPtrn);

	// �I�[���̃e�N�X�`������������ݒ�
	m_pAura->SetWidthPattern(nWidthPtrn);
}

//============================================================
//	�e�N�X�`���̏c�������̐ݒ菈��
//============================================================
void CTitleLogo2D::SetHeightPattern(const int nHeightPtrn)
{
	// �e�̃e�N�X�`���c��������ݒ�
	CAnim2D::SetHeightPattern(nHeightPtrn);

	// �I�[���̃e�N�X�`���c��������ݒ�
	m_pAura->SetHeightPattern(nHeightPtrn);
}

//============================================================
//	��������
//============================================================
CTitleLogo2D *CTitleLogo2D::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rOffset,	// �I�t�Z�b�g
	const D3DXVECTOR3& rSize,	// �傫��
	const float fMoveTime,		// �ړ�����
	const float fWaitTime		// �ҋ@����
)
{
	// �^�C�g�����S2D�̐���
	CTitleLogo2D *pTitleLogo2D = new CTitleLogo2D;
	if (pTitleLogo2D == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �^�C�g�����S2D�̏�����
		if (FAILED(pTitleLogo2D->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �^�C�g�����S2D�̔j��
			SAFE_DELETE(pTitleLogo2D);
			return nullptr;
		}

		// �����ʒu�I�t�Z�b�g��ݒ�
		pTitleLogo2D->m_offset = rOffset;

		// �����ʒu��ݒ�
		pTitleLogo2D->m_initPos = rPos + pTitleLogo2D->m_offset;

		// �ڕW�ʒu��ݒ�
		pTitleLogo2D->m_destPos = rPos;

		// �J�n�ʒu��ݒ�
		pTitleLogo2D->SetVec3Position(pTitleLogo2D->m_initPos);

		// �傫����ݒ�
		pTitleLogo2D->SetVec3Sizing(rSize);

		// �ړ����Ԃ�ݒ�
		pTitleLogo2D->m_fMoveTime = fMoveTime;

		// �ҋ@���Ԃ�ݒ�
		pTitleLogo2D->m_fWaitTime = fWaitTime;

		// �m�ۂ����A�h���X��Ԃ�
		return pTitleLogo2D;
	}
}

//============================================================
//	�ړ��ҋ@�̍X�V����
//============================================================
void CTitleLogo2D::UpdateMoveWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= m_fWaitTime)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �ړ���Ԃɂ���
		m_state = STATE_MOVE;
	}
}

//============================================================
//	�ړ��̍X�V����
//============================================================
void CTitleLogo2D::UpdateMove(const float fDeltaTime)
{
	// �����ʒu���v�Z
	const D3DXVECTOR3 DIFF_POS = m_destPos - m_initPos;

	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, m_fMoveTime);

	// �F�𔽉f
	SetColor(logo::INIT_COL + (logo::DIFF_COL * fRate));

	// �ʒu�𔽉f
	SetVec3Position(m_initPos + (DIFF_POS * fRate));

	if (m_fCurTime >= m_fMoveTime)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �F��␳
		SetColor(logo::DEST_COL);

		// �ʒu��␳
		SetVec3Position(m_destPos);

		// �I�[���ҋ@��Ԃɂ���
		m_state = STATE_AURA_WAIT;
	}
}

//============================================================
//	�I�[���ҋ@�̍X�V����
//============================================================
void CTitleLogo2D::UpdateAuraWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= aura::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �I�[����Ԃɂ���
		m_state = STATE_AURA;
	}
}

//============================================================
//	�I�[���̍X�V����
//============================================================
void CTitleLogo2D::UpdateAura(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, aura::MOVE_TIME);

	// �F�𔽉f
	m_pAura->SetColor(aura::INIT_COL + (aura::DIFF_COL * fRate));

	if (m_fCurTime >= aura::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �F��␳
		m_pAura->SetColor(aura::DEST_COL);

		// �������Ȃ���Ԃɂ���
		m_state = STATE_NONE;
	}
}
