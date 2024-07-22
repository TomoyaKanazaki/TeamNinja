//============================================================
//
//	���U���g�}�l�[�W���[���� [resultManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "resultManager.h"
#include "retentionManager.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"
#include "string2D.h"
#include "scrollText2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// ���U���g�}�l�[�W���[�̗D�揇��

	namespace fade
	{
		const float FADE_CENT		= 880.0f;	// �t�F�[�h���SX���W
		const float	FADEWAIT_TIME	= 1.2f;		// �t�F�[�h�J�n�ҋ@����
		const float	WAIT_TIME		= 0.15f;	// �t�F�[�h�ҋ@����
		const float	ADD_MOVE		= 0.65f;	// �t�F�[�h�ړ���
		const float	ADD_ACCEL_MOVE	= 2.1f;		// �t�F�[�h�����ړ���

		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(800.0f, 1770.0f, 0.0f);	// �t�F�[�h�傫��
		const D3DXCOLOR	  COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);	// �t�F�[�h�F
		const D3DXVECTOR3 UP_POS	= D3DXVECTOR3(FADE_CENT, -900.0f, 0.0f);		// �t�F�[�h��ʒu
		const D3DXVECTOR3 CENT_POS	= D3DXVECTOR3(FADE_CENT, SCREEN_CENT.y, 0.0f);	// �t�F�[�h�����ʒu
		const D3DXVECTOR3 DOWN_POS	= D3DXVECTOR3(FADE_CENT, SCREEN_HEIGHT + 900.0f, 0.0f);			// �t�F�[�h���ʒu
		const D3DXVECTOR3 UP_MIDDLE_POS		= D3DXVECTOR3(FADE_CENT, UP_POS.y + 300.0f, 0.0f);		// �t�F�[�h�㒆���ʒu
		const D3DXVECTOR3 DOWN_MIDDLE_POS	= D3DXVECTOR3(FADE_CENT, DOWN_POS.y - 300.0f, 0.0f);	// �t�F�[�h�������ʒu
	}

	namespace text
	{
		const char *FONT = "data\\FONT\\��S�V�b�N.otf";	// �t�H���g�p�X
		//const char *PASS = "data\\TEXT\\intro.txt";	// �e�L�X�g�p�X
		const bool	ITALIC			= false;	// �C�^���b�N
		const float	CHAR_HEIGHT		= 45.0f;	// �����c��
		const float	LINE_HEIGHT		= 62.0f;	// �s�ԏc��
		const float	WAIT_TIME_NOR	= 0.105f;	// �����\���̑ҋ@����

		const D3DXVECTOR3 POS = D3DXVECTOR3(fade::FADE_CENT, 460.0f, 0.0f);	// �e�L�X�g�ʒu
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// ���z�u
		const CText2D::EAlignY	 ALIGN_Y = CText2D::YALIGN_TOP;		// �c�z�u
	}
}

//************************************************************
//	�e�N���X [CResultManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CResultManager::CResultManager() :
	m_pFade		(nullptr),		// �t�F�[�h���
	m_pTitle	(nullptr),		// �^�C�g�����
	m_pGodItem	(nullptr),		// �_��^�C�g�����
	m_pTime		(nullptr),		// �^�C���^�C�g�����
	m_state		(STATE_NONE),	// ���
	m_fMoveY	(0.0f),			// �c�ړ���
	m_nCurTime	(0.0f)			// ���݂̑ҋ@����
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CResultManager::~CResultManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CResultManager::Init(void)
{
	// �����o�ϐ���������
	m_pFade		= nullptr;			// �t�F�[�h���
	m_pTitle	= nullptr;			// �^�C�g�����
	m_pGodItem	= nullptr;			// �_��^�C�g�����
	m_pTime		= nullptr;			// �^�C���^�C�g�����
	m_state		= STATE_FADEWAIT;	// ���
	m_fMoveY	= 0.0f;				// �c�ړ���
	m_nCurTime	= 0.0f;				// ���݂̑ҋ@����

	// �t�F�[�h�̐���
	m_pFade = CObject2D::Create
	( // ����
		fade::UP_POS,	// �ʒu
		fade::SIZE,		// �傫��
		VEC3_ZERO,		// ����
		fade::COL		// �F
	);
	if (m_pFade == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pFade->BindTexture("data\\TEXTURE\\resultFade000.png");	// TODO

	// �D�揇�ʂ�ݒ�
	m_pFade->SetPriority(PRIORITY);

	// �^�C�g���̐���
	m_pTitle = CScrollText2D::Create
	( // ����
		text::FONT,				// �t�H���g�p�X
		text::ITALIC,			// �C�^���b�N
		text::POS,				// ���_�ʒu
		text::WAIT_TIME_NOR,	// �����\���̑ҋ@����
		text::CHAR_HEIGHT,		// �����c��
		text::LINE_HEIGHT,		// �s�ԏc��
		text::ALIGN_X,			// ���z�u
		text::ALIGN_Y			// �c�z�u
	);
	if (m_pTitle == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ��t�F�[�h����ɂ���
	m_pTitle->SetPriority(PRIORITY);

	// TODO�F�����Ƀ^�C�g��
	m_pTitle->AddString(L"������");
	m_pTitle->AddString(L"�@�X�e�[�W��");

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CResultManager::Uninit(void)
{
	// �t�F�[�h�̏I��
	SAFE_UNINIT(m_pFade);
}

//============================================================
//	�X�V����
//============================================================
void CResultManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:
		break;

	case STATE_FADEWAIT:

		// �t�F�[�h�ҋ@�̍X�V
		UpdateFadeWait(fDeltaTime);
		break;

	case STATE_FADEIN:

		// �t�F�[�h�C���̍X�V
		UpdateFadeIn(fDeltaTime);
		break;

	case STATE_FADEIN_WAIT:

		// �t�F�[�h�C���ҋ@�̍X�V
		UpdateFadeInWait(fDeltaTime);
		break;

	case STATE_FADEIN_ACCEL:

		// �t�F�[�h�C�������̍X�V
		UpdateFadeInAccel(fDeltaTime);
		break;

	case STATE_STAGE_TITLE:

		// �X�e�[�W�^�C�g���̍X�V
		UpdateStageTitle(fDeltaTime);
		break;

	case STATE_WAIT:

		// �ҋ@�̍X�V
		UpdateWait(fDeltaTime);

		// TODO
		m_state = STATE_FADEOUT;
		break;

	case STATE_FADEOUT:

		// �t�F�[�h�A�E�g�̍X�V
		UpdateFadeOut(fDeltaTime);
		break;

	case STATE_FADEOUT_WAIT:

		// �t�F�[�h�A�E�g�ҋ@�̍X�V
		UpdateFadeOutWait(fDeltaTime);
		break;

	case STATE_FADEOUT_ACCEL:

		// �t�F�[�h�A�E�g�����̍X�V
		UpdateFadeOutAccel(fDeltaTime);
		break;

	case STATE_END:

		// �t�F�[�h���̏ꍇ������
		if (GET_MANAGER->GetFade()->IsFade()) { break; }

		// �I����ʂɑJ�ڂ���
		GET_MANAGER->SetLoadScene(CScene::MODE_SELECT);
		return;

	default:	// ��O����
		assert(false);
		break;
	}

	// �t�F�[�h�̍X�V
	m_pFade->Update(fDeltaTime);
}

//============================================================
//	��������
//============================================================
CResultManager *CResultManager::Create(void)
{
	// ���U���g�}�l�[�W���[�̐���
	CResultManager *pResultManager = new CResultManager;
	if (pResultManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���U���g�}�l�[�W���[�̏�����
		if (FAILED(pResultManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���U���g�}�l�[�W���[�̔j��
			SAFE_DELETE(pResultManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pResultManager;
	}
}

//============================================================
//	�j������
//============================================================
void CResultManager::Release(CResultManager *&prResultManager)
{
	// ���U���g�}�l�[�W���[�̏I��
	assert(prResultManager != nullptr);
	prResultManager->Uninit();

	// �������J��
	SAFE_DELETE(prResultManager);
}

//============================================================
//	�t�F�[�h�ҋ@�̍X�V����
//============================================================
void CResultManager::UpdateFadeWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_nCurTime += fDeltaTime;
	if (m_nCurTime >= fade::FADEWAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_nCurTime = 0;

		// �t�F�[�h�C����Ԃɂ���
		m_state = STATE_FADEIN;
	}
}

//============================================================
//	�t�F�[�h�C���̍X�V����
//============================================================
void CResultManager::UpdateFadeIn(const float fDeltaTime)
{
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// �t�F�[�h�ʒu

	// �ړ��ʂ����Z
	m_fMoveY += fade::ADD_MOVE;

	// �t�F�[�h�̈ʒu�����Z
	posFade.y += m_fMoveY;

	if (posFade.y > fade::UP_MIDDLE_POS.y)
	{ // ���݈ʒu����~�ʒu�𒴂����ꍇ

		// �t�F�[�h���~�ʒu�ɕ␳
		posFade.y = fade::UP_MIDDLE_POS.y;

		// �ړ��ʂ�������
		m_fMoveY = 0.0f;

		// �t�F�[�h�C���ҋ@��Ԃɂ���
		m_state = STATE_FADEIN_WAIT;
	}

	m_pFade->SetVec3Position(posFade);	// �t�F�[�h�ʒu�𔽉f
}

//============================================================
//	�t�F�[�h�C���ҋ@�̍X�V����
//============================================================
void CResultManager::UpdateFadeInWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_nCurTime += fDeltaTime;
	if (m_nCurTime >= fade::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_nCurTime = 0;

		// �t�F�[�h�C��������Ԃɂ���
		m_state = STATE_FADEIN_ACCEL;
	}
}

//============================================================
//	�t�F�[�h�C�������̍X�V����
//============================================================
void CResultManager::UpdateFadeInAccel(const float fDeltaTime)
{
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// �t�F�[�h�ʒu

	// �ړ��ʂ����Z
	m_fMoveY += fade::ADD_ACCEL_MOVE;

	// �t�F�[�h�̈ʒu�����Z
	posFade.y += m_fMoveY;

	if (posFade.y > fade::CENT_POS.y)
	{ // ���݈ʒu����~�ʒu�𒴂����ꍇ

		// �t�F�[�h���~�ʒu�ɕ␳
		posFade.y = fade::CENT_POS.y;

		// �ړ��ʂ�������
		m_fMoveY = 0.0f;

		// �X�e�[�W�^�C�g���̕���������J�n����
		m_pTitle->SetEnableScroll(true);

		// �X�e�[�W�^�C�g����Ԃɂ���
		m_state = STATE_STAGE_TITLE;
	}

	m_pFade->SetVec3Position(posFade);	// �t�F�[�h�ʒu�𔽉f
}

//============================================================
//	�X�e�[�W�^�C�g���̍X�V����
//============================================================
void CResultManager::UpdateStageTitle(const float fDeltaTime)
{
	// �������肪�I�����Ă��Ȃ��ꍇ������
	if (m_pTitle->IsScroll()) { return; }

	// �ҋ@��Ԃɂ���
	m_state = STATE_WAIT;
}

//============================================================
//	�ҋ@�̍X�V����
//============================================================
void CResultManager::UpdateWait(const float fDeltaTime)
{
#if 0
	D3DXCOLOR colControl = m_pControl->GetColor();	// ����\���F

	if (colControl.a < control::MIN_COL.a)
	{ // �����x���Œ�����Ⴂ�ꍇ

		// �����x�����Z
		colControl.a += control::ADD_ALPHA;

		if (colControl.a > control::MIN_COL.a)
		{ // �����x�����߂����ꍇ

			// �����x��␳
			colControl.a = control::MIN_COL.a;
		}

		// ����\���F��ݒ�
		m_pControl->SetColor(colControl);
	}
	else
	{ // �����x���Œ���ȏ�̏ꍇ

		// �ϐ���錾
		float fAddAlpha = 0.0f;	// �����x�̉��Z��

		// �����x���グ��
		m_fSinAlpha += control::ADD_SINROT;
		useful::NormalizeRot(m_fSinAlpha);	// �������K��

		// �����x���Z�ʂ����߂�
		fAddAlpha = (control::MAX_ADD_ALPHA / 2.0f) * (sinf(m_fSinAlpha) - 1.0f);

		// ����\���F��ݒ�
		m_pControl->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, control::BASIC_ALPHA + fAddAlpha));
	}
#endif
}

//============================================================
//	�t�F�[�h�A�E�g�̍X�V����
//============================================================
void CResultManager::UpdateFadeOut(const float fDeltaTime)
{
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// �t�F�[�h�ʒu

	// �ړ��ʂ����Z
	m_fMoveY += fade::ADD_MOVE;

	// �t�F�[�h�̈ʒu�����Z
	posFade.y += m_fMoveY;

	if (posFade.y > fade::DOWN_MIDDLE_POS.y)
	{ // ���݈ʒu����~�ʒu�𒴂����ꍇ

		// �t�F�[�h���~�ʒu�ɕ␳
		posFade.y = fade::DOWN_MIDDLE_POS.y;

		// �ړ��ʂ�������
		m_fMoveY = 0.0f;

		// �t�F�[�h�A�E�g�ҋ@��Ԃɂ���
		m_state = STATE_FADEOUT_WAIT;
	}

	m_pFade->SetVec3Position(posFade);	// �t�F�[�h�ʒu�𔽉f
}

//============================================================
//	�t�F�[�h�A�E�g�ҋ@�̍X�V����
//============================================================
void CResultManager::UpdateFadeOutWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_nCurTime += fDeltaTime;
	if (m_nCurTime >= fade::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_nCurTime = 0;

		// �t�F�[�h�A�E�g������Ԃɂ���
		m_state = STATE_FADEOUT_ACCEL;
	}
}

//============================================================
//	�t�F�[�h�A�E�g�����̍X�V����
//============================================================
void CResultManager::UpdateFadeOutAccel(const float fDeltaTime)
{
	D3DXVECTOR3 posFade = m_pFade->GetVec3Position();	// �t�F�[�h�ʒu

	// �ړ��ʂ����Z
	m_fMoveY += fade::ADD_ACCEL_MOVE;

	// �t�F�[�h�̈ʒu�����Z
	posFade.y += m_fMoveY;

	if (posFade.y > fade::DOWN_POS.y)
	{ // ���݈ʒu����~�ʒu�𒴂����ꍇ

		// �t�F�[�h���~�ʒu�ɕ␳
		posFade.y = fade::DOWN_POS.y;

		// �ړ��ʂ�������
		m_fMoveY = 0.0f;

		// �I����Ԃɂ���
		m_state = STATE_END;
	}

	m_pFade->SetVec3Position(posFade);	// �t�F�[�h�ʒu�𔽉f
}
