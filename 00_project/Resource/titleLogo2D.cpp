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
#include "manager.h"
#include "texture.h"
#include "camera.h"

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
		const float MOVE_TIME	 = 1.0f;	// �ړ�����
		const float	DEST_ALPHA	 = 1.0f;	// �ڕW�����x
		const float	INIT_ALPHA	 = 0.0f;	// ���������x
		const D3DXCOLOR DEST_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, DEST_ALPHA);	// �ڕW�F
		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// �����F
		const D3DXCOLOR DIFF_COL = DEST_COL - INIT_COL;						// �����F
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CTitleLogo2D> *CTitleLogo2D::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�e�N���X [CTitleLogo2D] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTitleLogo2D::CTitleLogo2D(const char* pBlurTexPath) : CAnim2D(CObject::LABEL_UI, CObject::SCENE_MAIN, CObject::DIM_2D, PRIORITY),
	m_pBlurTexPath	(pBlurTexPath),	// �u���[�e�N�X�`���p�X
	m_pAura			(nullptr),		// �I�[�����
	m_pBlur			(nullptr),		// �u���[���
	m_state			(STATE_NONE),	// ���
	m_fMoveTimeOne	(0.0f),			// ���ړ�����
	m_fMoveTimeTwo	(0.0f),			// ���ړ�����
	m_fWaitTimeOne	(0.0f),			// ���ҋ@����
	m_fWaitTimeTwo	(0.0f),			// ���ҋ@����
	m_fCurTime		(0.0f),			// ���݂̑ҋ@����
	m_fSinAlpha		(0.0f),			// ��������
	m_offset		(VEC3_ZERO),	// �����ʒu�I�t�Z�b�g
	m_initPos		(VEC3_ZERO),	// �����ʒu
	m_destPosOne	(VEC3_ZERO),	// ���ڕW�ʒu
	m_destPosTwo	(VEC3_ZERO)		// ���ڕW�ʒu
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
	m_pAura			= nullptr;		// �I�[�����
	m_pBlur			= nullptr;		// �u���[���
	m_state			= STATE_NONE;	// ���
	m_fMoveTimeOne	= 0.0f;			// ���ړ�����
	m_fMoveTimeTwo	= 0.0f;			// ���ړ�����
	m_fWaitTimeOne	= 0.0f;			// ���ҋ@����
	m_fWaitTimeTwo	= 0.0f;			// ���ҋ@����
	m_fCurTime		= 0.0f;			// ���݂̑ҋ@����
	m_fSinAlpha		= -HALF_PI;		// ��������
	m_offset		= VEC3_ZERO;	// �����ʒu�I�t�Z�b�g
	m_initPos		= VEC3_ZERO;	// �����ʒu
	m_destPosOne	= VEC3_ZERO;	// ���ڕW�ʒu
	m_destPosTwo	= VEC3_ZERO;	// ���ڕW�ʒu

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

	// ���Z�����ɂ���
	m_pAura->GetRenderState()->SetAlphaBlend(CRenderState::BLEND_ADD);

	// �u���[�̐���
	m_pBlur = CBlur2D::Create(this, XCOL_WHITE, 0.025f, 40, GET_MANAGER->GetTexture()->Regist(m_pBlurTexPath));
	if (m_pBlur == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CTitleLogo2D>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

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

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

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
	case STATE_ONE_MOVE_WAIT:

		// ���ړ��ҋ@�̍X�V
		UpdateMoveOneWait(fDeltaTime);
		break;

	case STATE_ONE_MOVE:

		// ���ړ��̍X�V
		UpdateMoveOne(fDeltaTime);
		break;

	case STATE_TWO_MOVE_WAIT:

		// ���ړ��ҋ@�̍X�V
		UpdateMoveTwoWait(fDeltaTime);
		break;

	case STATE_TWO_MOVE:

		// ���ړ��̍X�V
		UpdateMoveTwo(fDeltaTime);
		break;

	case STATE_AURA_WAIT:

		// �I�[���ҋ@�̍X�V
		UpdateAuraWait(fDeltaTime);
		break;

	case STATE_AURA:

		// �I�[���̍X�V
		UpdateAura(fDeltaTime);
		break;

	case STATE_END:

		// �I�[���_�ł̍X�V
		UpdateBlinkAura(fDeltaTime);
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
	const char* pBlurTexPath,		// �u���[�e�N�X�`���p�X
	const D3DXVECTOR3& rDestPosOne,	// ���ڕW�ʒu
	const D3DXVECTOR3& rDestPosTwo,	// ���ڕW�ʒu
	const D3DXVECTOR3& rOffset,		// �I�t�Z�b�g
	const D3DXVECTOR3& rSize,		// �傫��
	const float fMoveTimeOne,		// ���ړ�����
	const float fMoveTimeTwo,		// ���ړ�����
	const float fWaitTimeOne,		// ���ҋ@����
	const float fWaitTimeTwo		// ���ҋ@����
)
{
	// �^�C�g�����S2D�̐���
	CTitleLogo2D *pTitleLogo2D = new CTitleLogo2D(pBlurTexPath);
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
		pTitleLogo2D->m_initPos = rDestPosOne + pTitleLogo2D->m_offset;

		// ���ڕW�ʒu��ݒ�
		pTitleLogo2D->m_destPosOne = rDestPosOne;

		// ���ڕW�ʒu��ݒ�
		pTitleLogo2D->m_destPosTwo = rDestPosTwo;

		// �J�n�ʒu��ݒ�
		pTitleLogo2D->SetVec3Position(pTitleLogo2D->m_initPos);

		// �傫����ݒ�
		pTitleLogo2D->SetVec3Sizing(rSize);

		// ���ړ����Ԃ�ݒ�
		pTitleLogo2D->m_fMoveTimeOne = fMoveTimeOne;

		// ���ړ����Ԃ�ݒ�
		pTitleLogo2D->m_fMoveTimeTwo = fMoveTimeTwo;

		// ���ҋ@���Ԃ�ݒ�
		pTitleLogo2D->m_fWaitTimeOne = fWaitTimeOne;

		// ���ҋ@���Ԃ�ݒ�
		pTitleLogo2D->m_fWaitTimeTwo = fWaitTimeTwo;

		// �m�ۂ����A�h���X��Ԃ�
		return pTitleLogo2D;
	}
}

//============================================================
//	���o�X�L�b�v����
//============================================================
void CTitleLogo2D::EndStag(void)
{
	// �^�C�}�[��������
	m_fCurTime = 0.0f;

	// �J�����̍X�V���I���ɂ���
	GET_CAMERA->SetEnableUpdate(true);

	// �u���[�̏I��
	m_pBlur->SetState(CBlur2D::STATE_NONE);

	// �ʒu��␳
	SetVec3Position(m_destPosTwo);

	// �F��␳
	SetColor(logo::DEST_COL);

	// �I�[���F��␳
	m_pAura->SetColor(aura::DEST_COL);

	// �I����Ԃɂ���
	m_state = STATE_END;
}

//============================================================
//	�ړ��ҋ@�̍X�V����
//============================================================
void CTitleLogo2D::UpdateMoveOneWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= m_fWaitTimeOne)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �u���[�̊J�n
		m_pBlur->SetState(CBlur2D::STATE_NORMAL);

		// �ړ���Ԃɂ���
		m_state = STATE_ONE_MOVE;
	}
}

//============================================================
//	�ړ��̍X�V����
//============================================================
void CTitleLogo2D::UpdateMoveOne(const float fDeltaTime)
{
	// �����ʒu���v�Z
	const D3DXVECTOR3 DIFF_POS = m_destPosOne - m_initPos;

	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, m_fMoveTimeOne);

	// �F�𔽉f
	SetColor(logo::INIT_COL + (logo::DIFF_COL * fRate));

	// �ʒu�𔽉f
	SetVec3Position(m_initPos + (DIFF_POS * fRate));

	if (m_fCurTime >= m_fMoveTimeOne)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �F��␳
		SetColor(logo::DEST_COL);

		// �ʒu��␳
		SetVec3Position(m_destPosOne);

		// �ړ��ҋ@��Ԃɂ���
		m_state = STATE_TWO_MOVE_WAIT;
	}
}

//============================================================
//	���ړ��ҋ@�̍X�V����
//============================================================
void CTitleLogo2D::UpdateMoveTwoWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= m_fWaitTimeTwo)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �J�����̍X�V���I���ɂ���
		GET_CAMERA->SetEnableUpdate(true);

		// �ړ���Ԃɂ���
		m_state = STATE_TWO_MOVE;
	}
}

//============================================================
//	���ړ��̍X�V����
//============================================================
void CTitleLogo2D::UpdateMoveTwo(const float fDeltaTime)
{
	// �����ʒu���v�Z
	const D3DXVECTOR3 DIFF_POS = m_destPosTwo - m_destPosOne;

	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InOutQuart(m_fCurTime, 0.0f, m_fMoveTimeTwo);

	// �ʒu�𔽉f
	SetVec3Position(m_destPosOne + (DIFF_POS * fRate));

	if (m_fCurTime >= m_fMoveTimeTwo)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �ʒu��␳
		SetVec3Position(m_destPosTwo);

		// �u���[�̏I��
		m_pBlur->SetState(CBlur2D::STATE_VANISH);

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

		// �I����Ԃɂ���
		m_state = STATE_END;
	}
}

//============================================================
//	�S�I�����菈��
//============================================================
bool CTitleLogo2D::IsAllEnd(void)
{
	// ���X�g���Ȃ��ꍇ������
	if (m_pList == nullptr) { return false; }	// �n�܂��Ă���Ȃ�

	std::list< CTitleLogo2D*> list = m_pList->GetList();	// �������X�g
	for (const auto& rList : list)
	{ // ���X�g�v�f�����J��Ԃ�

		// �I����ԈȊO�̏ꍇ������
		if (rList->m_state != STATE_END) { return false; }	// �I�����Ă��Ȃ�
	}

	// �����܂ŗ�����S���I�����Ă���
	return true;
}

//============================================================
//	�I�[���_�ł̍X�V����
//============================================================
void CTitleLogo2D::UpdateBlinkAura(const float fDeltaTime)
{
	const float m_fAddSinRot	= 3.0f;		// ���������̉��Z��
	const float m_fMinAlpha		= 0.25f;	// �Œᓧ���x
	const float m_fMaxAlpha		= 1.0f;		// �ő哧���x

	if (CTitleLogo2D::IsAllEnd())
	{ // �S�^�C�g�����S�����o�I�����Ă���ꍇ

		// �T�C���J�[�u��������]
		m_fSinAlpha += m_fAddSinRot * fDeltaTime;
		useful::NormalizeRot(m_fSinAlpha);	// �������K��

		// �����x���Z�ʂ��v�Z
		float fAddAlpha = ((m_fMaxAlpha - m_fMinAlpha) * 0.5f) * (sinf(m_fSinAlpha) - 1.0f) * -1.0f;

		// �����x��ݒ�
		m_pAura->SetAlpha(m_fMinAlpha + fAddAlpha);
	}
}
