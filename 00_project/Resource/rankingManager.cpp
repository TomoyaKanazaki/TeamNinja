//============================================================
//
//	�����L���O�}�l�[�W���[���� [rankingManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "rankingManager.h"
#include "retentionManager.h"
#include "manager.h"
#include "camera.h"
#include "fade.h"
#include "loadtext.h"
#include "stage.h"
#include "object2D.h"
#include "player.h"
#include "string2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIO_BG = 5;	// �w�i�̗D�揇��
	const int PRIO_UI = 6;	// UI�̗D�揇��

	namespace fade
	{
		const float	MOVE_INTIME	 = 0.2f;	// �C���ϓ�����
		const float	MOVE_OUTTIME = 0.5f;	// �A�E�g�ϓ�����
		const float DEST_ALPHA	 = 0.5f;	// �ڕW�����x
		const float INIT_ALPHA	 = 0.0f;	// ���������x
		const float DIFF_ALPHA	 = DEST_ALPHA - INIT_ALPHA;	// ���������x
		const D3DXCOLOR INIT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, INIT_ALPHA);		// �����F
		const D3DXVECTOR3 SIZE	 = D3DXVECTOR3(SCREEN_SIZE.x, 280.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 POS	 = D3DXVECTOR3(SCREEN_CENT.x, 548.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 ROT	 = VEC3_ZERO;	// ����
	}

	namespace title
	{
		const char *TEXTURE		= "data\\TEXTURE\\get_magatama.png";	// �e�N�X�`���p�X
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(632.0f, 184.0f, 0.0f);	// �傫��
		const float	MOVE_TIME	= 0.68f;	// �ړ�����
		const D3DXCOLOR DEST_COL	= XCOL_WHITE;			// �ڕW�F
		const D3DXCOLOR INIT_COL	= XCOL_AWHITE;			// �����F
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;	// �����F
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(SCREEN_CENT.x, 505.0f, 0.0f);		// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// �����ʒu
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// �����ʒu
	}

	namespace line
	{
		const float	MOVE_TIME	= 0.5f;	// �ړ�����
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, 590.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(980.0f, 10.0f, 0.0f);		// �ڕW�傫��
		const D3DXVECTOR3 INIT_SIZE	= D3DXVECTOR3(0.0f, DEST_SIZE.y, 0.0f);	// �����傫��
		const D3DXVECTOR3 DIFF_SIZE	= DEST_SIZE - INIT_SIZE;				// �����傫��
	}

	namespace select
	{
		const wchar_t *STRING[] = { L"�͂�", L"������" };	// ������
		const char *FONT = "data\\FONT\\�ʂ˂�������������v7��.ttf";	// �t�H���g�p�X

		const bool	ITALIC		= false;	// �C�^���b�N
		const float	MOVE_TIME	= 0.68f;	// �ړ�����
		const float	PLUS_TIME	= 0.25f;	// �o�߂̉�������
		const float	WAIT_TIME	= 0.15f;	// �ҋ@����
		const float	CHAR_HEIGHT	= 80.0f;	// �����c��
		const float	DEST_ALPHA	= 1.0f;		// �ڕW�����x
		const float	INIT_ALPHA	= 0.0f;		// ���������x
		const float	DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// ���������x

		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);		// ��I�����̐F
		const D3DXCOLOR CHOICE_COL	= XCOL_YELLOW;								// �I�����̐F
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, DEST_ALPHA);	// �ڕW�F
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(0.6f, 0.6f, 0.6f, INIT_ALPHA);	// �����F
		const D3DXVECTOR3 SPACE		= D3DXVECTOR3(360.0f, 0.0f, 0.0f);			// ��
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_CENTER;			// ���z�u

		const D3DXVECTOR3 DEST_POS = D3DXVECTOR3(SCREEN_CENT.x - SPACE.x * 0.5f, 640.0f, 0.0f);	// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS = DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);					// �����ʒu
		const D3DXVECTOR3 DIFF_POS = DEST_POS - INIT_POS;										// �����ʒu
	}

	namespace fall
	{
		const float	MOVE_TIME	= 0.5f;	// �ړ�����
		const float DEST_ALPHA	= 0.0f;	// �ڕW�����x
		const float INIT_ALPHA	= 1.0f;	// ���������x
		const float DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// ���������x
		const D3DXCOLOR DEST_COL	 = D3DXCOLOR(1.0f, 1.0f, 1.0f, DEST_ALPHA);	// �ڕW�F
		const D3DXCOLOR INIT_COL	 = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// �����F
		const D3DXCOLOR DIFF_COL	 = DEST_COL - INIT_COL;				// �����F
		const D3DXVECTOR3 OFFSET_POS = D3DXVECTOR3(0.0f, 30.0f, 0.0f);	// �����ʒu�I�t�Z�b�g
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CRankingManager::AFuncUpdateState CRankingManager::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,							// �������Ȃ��X�V
	&CRankingManager::UpdateFadeOut,	// �t�F�[�h�A�E�g�X�V
	&CRankingManager::UpdateLine,		// �����o���X�V
	&CRankingManager::UpdateTitle,		// �^�C�g���o���X�V
	&CRankingManager::UpdateSelect,		// �I�����\���X�V
	&CRankingManager::UpdateWait,		// �ҋ@�X�V
	&CRankingManager::UpdateFadeIn,		// �t�F�[�h�C���X�V
	&CRankingManager::UpdateFall,		// UI�����X�V
	&CRankingManager::UpdateEnd,		// �I���X�V
};

//************************************************************
//	�e�N���X [CRankingManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRankingManager::CRankingManager() :
	m_pFade		 (nullptr),		// �t�F�[�h���
	m_pTitle	 (nullptr),		// �^�C�g�����
	m_pLine		 (nullptr),		// �������
	m_state		 (STATE_NONE),	// ���
	m_fCurTime	 (0.0f),		// ���݂̑ҋ@����
	m_nCurSelect (0),			// ���݂̑I����
	m_nOldSelect (0)			// �O��̑I����
{
	// �����o�ϐ����N���A
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// �I�������

	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CRankingManager::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	�f�X�g���N�^
//============================================================
CRankingManager::~CRankingManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CRankingManager::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// �I�������
	m_state		 = STATE_FADEOUT;	// ���
	m_pFade		 = nullptr;	// �t�F�[�h���
	m_pTitle	 = nullptr;	// �^�C�g�����
	m_pLine		 = nullptr;	// �������
	m_fCurTime	 = 0.0f;	// ���݂̑ҋ@����
	m_nCurSelect = 0;		// ���݂̑I����
	m_nOldSelect = 0;		// �O��̑I����

	//--------------------------------------------------------
	//	�t�F�[�h�̐��� / �����ݒ�
	//--------------------------------------------------------
	// �t�F�[�h�̐���
	m_pFade = CObject2D::Create
	( // ����
		fade::POS,		// �ʒu
		fade::SIZE,		// �傫��
		fade::ROT,		// ����
		fade::INIT_COL	// �F
	);
	if (m_pFade == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pFade->SetPriority(PRIO_BG);

	// ���x����ݒ�
	m_pFade->SetLabel(CObject::LABEL_UI);

	//--------------------------------------------------------
	//	�^�C�g���̐��� / �����ݒ�
	//--------------------------------------------------------
	// �^�C�g���̐���
	m_pTitle = CObject2D::Create
	( // ����
		title::INIT_POS,	// �ʒu
		title::SIZE,		// �傫��
		VEC3_ZERO,			// ����
		title::INIT_COL		// �F
	);
	if (m_pTitle == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`��������
	m_pTitle->BindTexture(title::TEXTURE);

	// �D�揇�ʂ�ݒ�
	m_pTitle->SetPriority(PRIO_UI);

	// ���x����ݒ�
	m_pTitle->SetLabel(CObject::LABEL_UI);

	//--------------------------------------------------------
	//	�����̐��� / �����ݒ�
	//--------------------------------------------------------
	// �����̐���
	m_pLine = CObject2D::Create
	( // ����
		line::POS,		// �ʒu
		line::INIT_SIZE	// �傫��
	);
	if (m_pLine == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pLine->SetPriority(PRIO_UI);

	// ���x����ݒ�
	m_pLine->SetLabel(CObject::LABEL_UI);

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
		m_apSelect[i]->SetPriority(PRIO_UI);

		// ���x����ݒ�
		m_apSelect[i]->SetLabel(CObject::LABEL_UI);

		// �����`���OFF�ɂ���
		m_apSelect[i]->SetEnableDraw(false);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CRankingManager::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CRankingManager::Update(const float fDeltaTime)
{
	assert(m_state > NONE_IDX && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ

		// �e��Ԃ��Ƃ̍X�V
		(this->*(m_aFuncUpdateState[m_state]))(fDeltaTime);
	}

	// �X�L�b�v����̍X�V
	UpdateSkip();
}

//============================================================
//	��������
//============================================================
CRankingManager *CRankingManager::Create(void)
{
	// �����L���O�}�l�[�W���[�̐���
	CRankingManager *pRankingManager = new CRankingManager;
	if (pRankingManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �����L���O�}�l�[�W���[�̏�����
		if (FAILED(pRankingManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �����L���O�}�l�[�W���[�̔j��
			SAFE_DELETE(pRankingManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pRankingManager;
	}
}

//============================================================
//	�j������
//============================================================
void CRankingManager::Release(CRankingManager *&prRankingManager)
{
	// �����L���O�}�l�[�W���[�̏I��
	assert(prRankingManager != nullptr);
	prRankingManager->Uninit();

	// �������J��
	SAFE_DELETE(prRankingManager);
}

//============================================================
//	�t�F�[�h�A�E�g�̍X�V����
//============================================================
void CRankingManager::UpdateFadeOut(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, fade::MOVE_OUTTIME);

	// �t�F�[�h�̓����x�𔽉f
	m_pFade->SetAlpha(fade::INIT_ALPHA + (fade::DIFF_ALPHA * fRate));

	if (m_fCurTime >= fade::MOVE_OUTTIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �t�F�[�h�̓����x��␳
		m_pFade->SetAlpha(fade::DEST_ALPHA);

		// �����o����Ԃɂ���
		m_state = STATE_LINE;
	}
}

//============================================================
//	�����o���̍X�V����
//============================================================
void CRankingManager::UpdateLine(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, line::MOVE_TIME);

	// �����̑傫���𔽉f
	m_pLine->SetVec3Sizing(line::INIT_SIZE + (line::DIFF_SIZE * fRate));

	if (m_fCurTime >= line::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �����̑傫����␳
		m_pLine->SetVec3Sizing(line::DEST_SIZE);

		// �^�C�g���o����Ԃɂ���
		m_state = STATE_TITLE;
	}
}

//============================================================
//	�^�C�g���o���̍X�V����
//============================================================
void CRankingManager::UpdateTitle(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, title::MOVE_TIME);

	// �^�C�g���̐F�𔽉f
	m_pTitle->SetColor(title::INIT_COL + (title::DIFF_COL * fRate));

	// �^�C�g���̈ʒu�𔽉f
	m_pTitle->SetVec3Position(title::INIT_POS + (title::DIFF_POS * fRate));

	if (m_fCurTime >= title::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �^�C�g���̐F��␳
		m_pTitle->SetColor(title::DEST_COL);

		// �^�C�g���̈ʒu��␳
		m_pTitle->SetVec3Position(title::DEST_POS);

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
void CRankingManager::UpdateSelect(const float fDeltaTime)
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
void CRankingManager::UpdateWait(const float fDeltaTime)
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
	||  pPad->IsTrigger(CInputPad::KEY_A))
	{
		// �t�F�[�h�C����Ԃɂ���
		m_state = STATE_FADEIN;
	}
}

//============================================================
//	�t�F�[�h�C���̍X�V����
//============================================================
void CRankingManager::UpdateFadeIn(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::OutQuad(m_fCurTime, fade::MOVE_INTIME, 0.0f);

	// �t�F�[�h�̓����x�𔽉f
	m_pFade->SetAlpha(fade::INIT_ALPHA + (fade::DIFF_ALPHA * fRate));

	if (m_fCurTime >= fade::MOVE_INTIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �t�F�[�h�̓����x��␳
		m_pFade->SetAlpha(fade::INIT_ALPHA);

		// UI������Ԃɂ���
		m_state = STATE_FALL;
	}
}

//============================================================
//	UI�����̍X�V����
//============================================================
void CRankingManager::UpdateFall(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, fall::MOVE_TIME);

	// �F�𔽉f
	m_pTitle->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// �^�C�g�����
	m_pLine->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// �������

	// �ʒu�𔽉f
	m_pTitle->SetVec3Position(title::DEST_POS + (((title::DEST_POS + fall::OFFSET_POS) - title::DEST_POS) * fRate));	// �^�C�g�����
	m_pLine->SetVec3Position(line::POS + (((line::POS + fall::OFFSET_POS) - line::POS) * fRate));	// �������

	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		D3DXVECTOR3 posSelect = select::DEST_POS + (((select::DEST_POS + fall::OFFSET_POS) - select::DEST_POS) * fRate);	// �ʒu
		D3DXVECTOR3 offsetSelect = (select::SPACE * (float)i);	// �I�t�Z�b�g

		// �ʒu�𔽉f
		m_apSelect[i]->SetVec3Position(posSelect + offsetSelect);

		// �F�𔽉f
		m_apSelect[i]->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));
	}

	if (m_fCurTime >= fall::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �F��␳
		m_pTitle->SetColor(fall::DEST_COL);	// �^�C�g�����
		m_pLine->SetColor(fall::DEST_COL);	// �������

		switch (m_nCurSelect)
		{ // �I�������Ƃ̏���
		case SELECT_YES:
		{
			// ����̐ݒ�
			GET_PLAYER->SetEnter();
			break;
		}
		case SELECT_NO:
		{
			// �ʏ��Ԃɂ���
			GET_PLAYER->SetState(CPlayer::STATE_SELECT_NORMAL);
			break;
		}
		default:
			assert(false);
			break;
		};

		// �I����Ԃɂ���
		m_state = STATE_END;
	}
}

//============================================================
//	�I���̍X�V����
//============================================================
void CRankingManager::UpdateEnd(const float fDeltaTime)
{

}

//============================================================
//	�X�L�b�v����̍X�V����
//============================================================
void CRankingManager::UpdateSkip(void)
{
	if (m_state < STATE_WAIT)
	{ // ���o�X�L�b�v���\�ȏꍇ

		CInputKeyboard*	pKey = GET_INPUTKEY;	// �L�[�{�[�h���
		CInputPad*		pPad = GET_INPUTPAD;	// �p�b�h���
		if (pKey->IsTrigger(DIK_SPACE)
		||  pKey->IsTrigger(DIK_RETURN)
		||  pPad->IsTrigger(CInputPad::KEY_A)
		||  pPad->IsTrigger(CInputPad::KEY_B)
		||  pPad->IsTrigger(CInputPad::KEY_X)
		||  pPad->IsTrigger(CInputPad::KEY_Y))
		{
			// ���o�̃X�L�b�v
			SkipStaging();
		}
	}
}

//============================================================
//	���o�̃X�L�b�v����
//============================================================
void CRankingManager::SkipStaging(void)
{
	// �ҋ@��Ԃɂ���
	m_state = STATE_WAIT;

	// �t�F�[�h�̓����x��␳
	m_pFade->SetAlpha(fade::DEST_ALPHA);

	// �����̑傫����␳
	m_pLine->SetVec3Sizing(line::DEST_SIZE);

	// �^�C�g���̐F��␳
	m_pTitle->SetColor(title::DEST_COL);

	// �^�C�g���̈ʒu��␳
	m_pTitle->SetVec3Position(title::DEST_POS);
}
