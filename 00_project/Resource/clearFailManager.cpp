//============================================================
//
//	�N���A���s�}�l�[�W���[���� [clearFailManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "clearFailManager.h"
#include "manager.h"
#include "string2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int	PRIORITY	= 6;		// �N���A���s�}�l�[�W���[�̗D�揇��
	const float	FADE_CENT	= 880.0f;	// �t�F�[�h���SX���W

	namespace cont
	{
		const char		*FONT	= "data\\FONT\\��S�V�b�N.otf";	// �t�H���g�p�X
		const wchar_t	*STRING	= L"�C������蒼��";			// ������

		const bool	ITALIC		= false;	// �C�^���b�N
		const float	MOVE_TIME	= 0.2f;		// �ړ�����
		const float	WAIT_TIME	= 0.5f;		// �^�C�g���ҋ@����
		const float	DEST_HEIGHT	= 130.0f;	// �����ڕW�c��
		const float	INIT_HEIGHT	= DEST_HEIGHT * 14.0f;			// ���������c��
		const float	DIFF_HEIGHT	= DEST_HEIGHT - INIT_HEIGHT;	// ���������c��

		const D3DXVECTOR3 POS = D3DXVECTOR3(FADE_CENT, 360.0f, 0.0f);	// �ʒu
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_CENTER;	// ���z�u
	}

	namespace select
	{
		const wchar_t *STRING[] = { L"�͂�", L"������" };	// ������
		const char *FONT = "data\\FONT\\��S�V�b�N.otf";	// �t�H���g�p�X

		const bool	ITALIC		= false;	// �C�^���b�N
		const float	MOVE_TIME	= 0.68f;	// �ړ�����
		const float	PLUS_TIME	= 0.25f;	// �o�߂̉�������
		const float	WAIT_TIME	= 0.15f;	// �ҋ@����
		const float	CHAR_HEIGHT	= 100.0f;	// �����c��
		const float	DEST_ALPHA	= 1.0f;		// �ڕW�����x
		const float	INIT_ALPHA	= 0.0f;		// ���������x
		const float	DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// ���������x

		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);		// ��I�����̐F
		const D3DXCOLOR CHOICE_COL	= XCOL_YELLOW;								// �I�����̐F
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, DEST_ALPHA);	// �ڕW�F
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, INIT_ALPHA);	// �����F
		const D3DXVECTOR3 SPACE		= D3DXVECTOR3(360.0f, 0.0f, 0.0f);			// ��
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_CENTER;			// ���z�u

		const D3DXVECTOR3 DEST_POS = D3DXVECTOR3(FADE_CENT - SPACE.x * 0.5f, 540.0f, 0.0f);	// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS = DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);				// �����ʒu
		const D3DXVECTOR3 DIFF_POS = DEST_POS - INIT_POS;									// �����ʒu
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CClearFailManager::AFuncUpdateState CClearFailManager::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,										// �������Ȃ��X�V
	&CClearFailManager::UpdateContinueTitleWait,	// �R���e�j���[�^�C�g���ҋ@�X�V
	&CClearFailManager::UpdateContinueTitle,		// �R���e�j���[�^�C�g���\���X�V
	&CClearFailManager::UpdateSelectWait,			// �I�����ҋ@�X�V
	&CClearFailManager::UpdateSelect,				// �I�����\���X�V
	&CClearFailManager::UpdateWait,					// �ҋ@�X�V
	nullptr,										// �I���X�V
};

//************************************************************
//	�e�N���X [CClearFailManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CClearFailManager::CClearFailManager() :
	m_pContinue	 (nullptr),		// �R���e�j���[���
	m_state		 (STATE_NONE),	// ���
	m_fCurTime	 (0.0f),		// ���݂̑ҋ@����
	m_nCurSelect (0),			// ���݂̑I����
	m_nOldSelect (0)			// �O��̑I����
{
	// �����o�ϐ����N���A
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// �I�������

	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CClearFailManager::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	�f�X�g���N�^
//============================================================
CClearFailManager::~CClearFailManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CClearFailManager::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// �I�������
	m_state		 = STATE_CONTINUE_TITLE_WAIT;		// ���
	m_pContinue	 = nullptr;	// �R���e�j���[���
	m_fCurTime	 = 0.0f;	// ���݂̑ҋ@����
	m_nCurSelect = 0;		// ���݂̑I����
	m_nOldSelect = 0;		// �O��̑I����

	//--------------------------------------------------------
	//	�R���e�j���[�̐��� / �����ݒ�
	//--------------------------------------------------------
	// �R���e�j���[�̐���
	m_pContinue = CString2D::Create
	( // ����
		cont::FONT,			// �t�H���g�p�X
		cont::ITALIC,		// �C�^���b�N
		cont::STRING,		// �w�蕶����
		cont::POS,			// ���_�ʒu
		cont::INIT_HEIGHT,	// �����c��
		cont::ALIGN_X		// ���z�u
	);
	if (m_pContinue == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pContinue->SetPriority(PRIORITY);

	// �����`���OFF�ɂ���
	m_pContinue->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�I�����̐��� / �����ݒ�
	//--------------------------------------------------------
	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		// �����ʒu���v�Z
		D3DXVECTOR3 posIcon = select::INIT_POS + (select::SPACE * (float)i);	// �����ʒu

		// �I�����̐���
		m_apSelect[i] = CString2D::Create
		( // ����
			select::FONT,			// �t�H���g�p�X
			select::ITALIC,			// �C�^���b�N
			select::STRING[i],		// �w�蕶����
			posIcon,				// ���_�ʒu
			select::CHAR_HEIGHT,	// �����c��
			select::ALIGN_X,		// ���z�u
			VEC3_ZERO,				// ���_����
			select::INIT_COL		// �F
		);
		if (m_apSelect[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apSelect[i]->SetPriority(PRIORITY);

		// �����`���OFF�ɂ���
		m_apSelect[i]->SetEnableDraw(false);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CClearFailManager::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CClearFailManager::Update(const float fDeltaTime)
{
	assert(m_state > NONE_IDX && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ

		// �e��Ԃ��Ƃ̍X�V
		(this->*(m_aFuncUpdateState[m_state]))(fDeltaTime);
	}
}

//============================================================
//	���o�̃X�L�b�v����
//============================================================
void CClearFailManager::SkipStaging(void)
{
	// �ҋ@��Ԃɂ���
	m_state = STATE_WAIT;

	// �R���e�j���[�����o��̌����ڂɂ���
	m_pContinue->SetEnableDraw(true);				// �����`���ON�ɂ���
	m_pContinue->SetCharHeight(cont::DEST_HEIGHT);	// �ڕW�T�C�Y�ɐݒ�

	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		// �I�����̖ڕW�����ʒu���v�Z
		D3DXVECTOR3 posDest = select::DEST_POS + (select::SPACE * (float)i);

		// �I���������o��̌����ڂɂ���
		m_apSelect[i]->SetEnableDraw(true);			// �����`���ON�ɂ���
		m_apSelect[i]->SetVec3Position(posDest);	// �ڕW�ʒu�ɐݒ�
		m_apSelect[i]->SetColor(select::DEST_COL);	// �ڕW�F�ɐݒ�
	}
}

//============================================================
//	�SUI�I�u�W�F�N�g�̈ړ�����
//============================================================
void CClearFailManager::SetAllMove(const D3DXVECTOR3& rMove)
{
	// �R���e�j���[�̈ʒu���ړ�
	m_pContinue->SetVec3Position(m_pContinue->GetVec3Position() + rMove);

	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		// �I�����̈ʒu���ړ�
		m_apSelect[i]->SetVec3Position(m_apSelect[i]->GetVec3Position() + rMove);
	}
}

//============================================================
//	�R���e�j���[�^�C�g���ҋ@�̍X�V����
//============================================================
void CClearFailManager::UpdateContinueTitleWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= cont::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �R���e�j���[�^�C�g���̎����`���ON�ɂ���
		m_pContinue->SetEnableDraw(true);

		// �R���e�j���[�^�C�g���\����Ԃɂ���
		m_state = STATE_CONTINUE_TITLE;
	}
}

//============================================================
//	�R���e�j���[�^�C�g���\���̍X�V����
//============================================================
void CClearFailManager::UpdateContinueTitle(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, cont::MOVE_TIME);

	// �R���e�j���[�^�C�g���̑傫���𔽉f
	m_pContinue->SetCharHeight(cont::INIT_HEIGHT + (cont::DIFF_HEIGHT * fRate));

	if (m_fCurTime >= cont::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �R���e�j���[�^�C�g���̑傫����␳
		m_pContinue->SetCharHeight(cont::DEST_HEIGHT);

		// �I�����ҋ@��Ԃɂ���
		m_state = STATE_SELECT_WAIT;
	}
}

//============================================================
//	�I�����ҋ@�̍X�V����
//============================================================
void CClearFailManager::UpdateSelectWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= select::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		for (int i = 0; i < SELECT_MAX; i++)
		{ // �I�����̑������J��Ԃ�

			// �I�����̎����`���ON�ɂ���
			m_apSelect[i]->SetEnableDraw(true);
		}

		// �I�����\����Ԃɂ���
		m_state = STATE_SELECT;
	}
}

//============================================================
//	�I�����\���̍X�V����
//============================================================
void CClearFailManager::UpdateSelect(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �I�����̈ړ�
	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		// �A�C�R���w�i���ꂼ��̌o�ߎ��Ԃ��v�Z
		float fRateTime = m_fCurTime - (select::PLUS_TIME * (float)i);
		useful::LimitNum(fRateTime, 0.0f, select::MOVE_TIME);	// �o�ߎ��Ԃ�␳

		// ���ꂼ��̌o�ߎ������犄�����v�Z
		float fRate = easeing::InOutQuad(fRateTime, 0.0f, select::MOVE_TIME);

		// �I�����̈ʒu���v�Z
		D3DXVECTOR3 posInit = select::INIT_POS + (select::SPACE * (float)i);

		// �I�����̐F���v�Z
		D3DXCOLOR colCur = select::INIT_COL;
		colCur.a = select::INIT_ALPHA + (select::DIFF_ALPHA * fRate);	// ���݂̓����x��ݒ�

		// �I�����̈ʒu�𔽉f
		m_apSelect[i]->SetVec3Position(posInit + (select::DIFF_POS * fRate));

		// �I�����̐F�𔽉f
		m_apSelect[i]->SetColor(colCur);
	}

	// �I�����̈ړ��␳
	if (m_fCurTime >= select::MOVE_TIME + select::PLUS_TIME * (SELECT_MAX - 1))
	{ // �S�I�����̑ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		for (int i = 0; i < SELECT_MAX; i++)
		{ // �I�����̑������J��Ԃ�

			// �I�����̖ڕW�����ʒu���v�Z
			D3DXVECTOR3 posDest = select::DEST_POS + (select::SPACE * (float)i);

			// �I�����̈ʒu��␳
			m_apSelect[i]->SetVec3Position(posDest);

			// �I�����̐F��␳
			m_apSelect[i]->SetColor(select::DEST_COL);
		}

		// �ҋ@��Ԃɂ���
		m_state = STATE_WAIT;
	}
}

//============================================================
//	�ҋ@�̍X�V����
//============================================================
void CClearFailManager::UpdateWait(const float fDeltaTime)
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputPad*		pPad = GET_INPUTPAD;	// �p�b�h���

	//--------------------------------------------------------
	//	�I��������
	//--------------------------------------------------------
	// �O��̑I������ۑ�
	m_nOldSelect = m_nCurSelect;

	// �I��������
	if (pKey->IsTrigger(DIK_LEFT)
	||  pPad->IsTrigger(CInputPad::KEY_LEFT)
	||  pPad->GetTriggerLStick(CInputPad::STICK_LEFT)
	||  pPad->GetTriggerRStick(CInputPad::STICK_LEFT))
	{
		// ���ɑI�������炷
		m_nCurSelect = (m_nCurSelect + 1) % SELECT_MAX;
	}
	if (pKey->IsTrigger(DIK_RIGHT)
	||  pPad->IsTrigger(CInputPad::KEY_RIGHT)
	||  pPad->GetTriggerLStick(CInputPad::STICK_RIGHT)
	||  pPad->GetTriggerRStick(CInputPad::STICK_RIGHT))
	{
		// �E�ɑI�������炷
		m_nCurSelect = (m_nCurSelect + (SELECT_MAX - 1)) % SELECT_MAX;
	}

	// �O��̑I��v�f�̐F�𔒐F�ɐݒ�
	m_apSelect[m_nOldSelect]->SetColor(select::DEFAULT_COL);

	// ���݂̑I��v�f�̐F�����F�ɐݒ�
	m_apSelect[m_nCurSelect]->SetColor(select::CHOICE_COL);

	//--------------------------------------------------------
	//	�I��������
	//--------------------------------------------------------
	if (pKey->IsTrigger(DIK_RETURN)
	|| pPad->IsTrigger(CInputPad::KEY_A)
	|| pPad->IsTrigger(CInputPad::KEY_B))
	{
		switch (m_nCurSelect)
		{ // �I�������Ƃ̏���
		case SELECT_YES:

			// �����}�b�v�����̃}�b�v�Ɏw��
			SetNext(CScene::MODE_GAME);
			break;

		case SELECT_NO:

			// �Z���N�g�}�b�v�����̃}�b�v�Ɏw��
			SetNext(CScene::MODE_SELECT);
			break;

		default:
			assert(false);
			break;
		}

		// �I����Ԃɂ���
		m_state = STATE_END;
	}
}
