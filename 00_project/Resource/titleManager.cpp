//============================================================
//
//	�^�C�g���}�l�[�W���[���� [titleManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "titleManager.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "fade.h"
#include "texture.h"
#include "model.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEX_LOGO_FILE[] =	// ���S�e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\npn_title.png",	// �^�C�g���e�N�X�`��
	};

	const char *TEX_START_FILE[] =	// �X�^�[�g�e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\title003.png",	// START�e�N�X�`��
	};

	const int PRIORITY = 5;	// �^�C�g���̗D�揇��

	namespace fade
	{
		const D3DXCOLOR COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);	// �^�C�g���t�F�[�h�̐F

		const float SUB_ALPHA = 0.008f;	// ���l�̌��Z��
	}

	namespace logo
	{
		const D3DXVECTOR3 POS = D3DXVECTOR3(1100.0f,  290.0f, 0.0f);	// �^�C�g�����S�̈ʒu
		const D3DXVECTOR3 STOP_POS = D3DXVECTOR3(130.0f, 290.0f, 0.0f);	// �^�C�g�����S�̈ʒu

		const D3DXVECTOR3 SIZE = D3DXVECTOR3(450.0f, 600.0f, 0.0f);	// �^�C�g�����S�̑傫��
		const D3DXVECTOR3 INIT_SIZE = SIZE * 0.0f;	// �^�C�g�����S�̏����̑傫��

		const D3DXCOLOR COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �^�C�g�����S�̐F

		const float		  MOVE  = 10.0f;	// �^�C�g�����S�̈ړ���
		const float	INIT_SCALE	= 0.0f;		// �^�C�g�����S�̏����g�嗦
		const float	SUB_SCALE	= 0.1f;		// �^�C�g�����S�g�嗦�̉��Z��
		const int WAIT_TIME		= 40;		// �^�C�g�����S�ړ��܂ł̑ҋ@����
	}

	namespace select
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(350.0f, 560.0f, 0.0f);	// �I���̈ʒu
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(560.0f, 0.0f, 0.0f);		// �I���̋��
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(420.0f, 140.0f, 0.0f);	// �I���̑傫��

		const D3DXCOLOR CHOICE_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �I�𒆃J���[
		const D3DXCOLOR DEFAULT_COL	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	// ��I�𒆃J���[
	}

	namespace start
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(640.0f, 600.0f, 0.0f);	// �X�^�[�g�̈ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(500.0f, 120.0f, 0.0f);	// �X�^�[�g�̑傫��

		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// �X�^�[�g�̏����F
		const D3DXCOLOR COL      = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �X�^�[�g�̐F

		const float INIT_ALPHA = 0.0f;	// �����x�̏����l
		const float SUB_ALPHA = 0.016f;	// �����x�̉��Z��

		const float	SUB_SCALE = 0.2f;	// �X�^�[�g�̊g�嗦�̉��Z��
	}
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
//static_assert(NUM_ARRAY(TEX_LOGO_FILE)   == CTitleManager::LOGO_MAX,   "ERROR : LogoTexture Count Mismatch");
//static_assert(NUM_ARRAY(TEX_START_FILE)	 == CTitleManager::SELECT_MAX, "ERROR : SelectTexture Count Mismatch");

//************************************************************
//	�e�N���X [CTitleManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTitleManager::CTitleManager() :
	m_pLogo			(nullptr),		// �^�C�g���\���̏��
	m_pFade			(nullptr),		// �t�F�[�h�̏��
	m_pStart		(nullptr),		// �X�^�[�g�̏��
	m_state			(STATE_NONE),	// ���
	m_startState	(STARTSTATE_NONE),	// �X�^�[�g���
	m_fScale		(0.0f),			// �^�C�g���g�嗦
	m_nSelect		(0),			// ���݂̑I��
	m_nOldSelect	(0),			// �O��̑I��
	m_nWaitCounter	(0)				// �^�C�g���ړ��܂ł̑ҋ@����
{
	// �����o�ϐ����N���A
	//memset(&m_apLogo[0],	0, sizeof(m_apLogo));	// �^�C�g���\���̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTitleManager::~CTitleManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTitleManager::Init(void)
{
	// �|�C���^��錾
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`���ւ̃|�C���^

	// �����o�ϐ���������
	m_pLogo			= nullptr;			// �^�C�g���\���̏��
	m_pFade			= nullptr;			// �t�F�[�h�̏��
	m_pStart		= nullptr;			// �X�^�[�g�̏��
	m_state			= STATE_FADEOUT;	// ���
	m_startState    = STARTSTATE_APPEAR;// �X�^�[�g���
	m_fScale		= logo::INIT_SCALE;	// �^�C�g���g�嗦
	m_fMove			= logo::MOVE;		// �^�C�g�����S�̈ړ���
	m_nSelect		= 0;				// ���݂̑I��
	m_nOldSelect	= 0;				// �O��̑I��
	m_nWaitCounter  = 0;				// �^�C�g���ړ��܂ł̑ҋ@����

	//--------------------------------------------------------
	//	�^�C�g�����S�̐����E�ݒ�
	//--------------------------------------------------------
	// �^�C�g�����S�̐���
	m_pLogo = CObject2D::Create
	( // ����
		logo::POS,			// �ʒu
		logo::INIT_SIZE		// �����̑傫��
	);
	if (m_pLogo == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pLogo->BindTexture(pTexture->Regist(TEX_LOGO_FILE[0]));

	// �D�揇�ʂ�ݒ�
	m_pLogo->SetPriority(PRIORITY);

	// �`������Ȃ��ݒ�ɂ���
	m_pLogo->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�X�^�[�g�̐����E�ݒ�
	//--------------------------------------------------------
	// �X�^�[�g�̐���
	m_pStart = CObject2D::Create
	( // ����
		start::POS,		// �ʒu
		start::SIZE,	// �傫��
		VEC3_ZERO,		// ����
		start::INIT_COL	// �F
	);
	if (m_pStart == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`����o�^�E����
	m_pStart->BindTexture(pTexture->Regist(TEX_START_FILE[0]));

	// �D�揇�ʂ�ݒ�
	m_pStart->SetPriority(PRIORITY);

	// �`������Ȃ��ݒ�ɂ���
	m_pStart->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�t�F�[�h�̐����E�ݒ�
	//--------------------------------------------------------
	// �t�F�[�h�̐���
	m_pFade = CObject2D::Create
	( // ����
		SCREEN_CENT,	// �ʒu
		SCREEN_SIZE,	// �傫��
		VEC3_ZERO,		// ����
		fade::COL		// �F
	);
	if (m_pFade == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pFade->SetPriority(PRIORITY);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTitleManager::Uninit(void)
{
	// �^�C�g�����S�̏I��
	SAFE_UNINIT(m_pLogo);

	// �t�F�[�h�̏I��
	SAFE_UNINIT(m_pFade);

	// �X�^�[�g�̏I��
	SAFE_UNINIT(m_pStart);
}

//============================================================
//	�X�V����
//============================================================
void CTitleManager::Update(const float fDeltaTime)
{
	// �J�ڌ���̍X�V
	UpdateStart();

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ����

		// ����

		break;

	case STATE_FADEOUT:	// �t�F�[�h�A�E�g���

		// �t�F�[�h�A�E�g�̍X�V
		UpdateFade();

		break;

	case STATE_SCALE:	// �^�C�g���k�����

		// �^�C�g���ړ��̍X�V
		UpdateScale();

		break;

	case STATE_MOVE:	// �^�C�g���ړ����

		// �^�C�g���ړ��̍X�V
		UpdateMove();

		break;

	case STATE_WAIT:	// �J�ڑҋ@���

		// �X�^�[�g�_��
		UpdateBlink();

		break;

	case STATE_TRANS:	// �J�ڏ��

		// �J��
		UpdateTrans();

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �^�C�g�����S�̍X�V
	m_pLogo->Update(fDeltaTime);

	// �t�F�[�h�̍X�V
	m_pFade->Update(fDeltaTime);

	// �X�^�[�g�̍X�V
	m_pStart->Update(fDeltaTime);
}

//============================================================
//	��������
//============================================================
CTitleManager *CTitleManager::Create(void)
{
	// �^�C�g���}�l�[�W���[�̐���
	CTitleManager *pTitleManager = new CTitleManager;
	if (pTitleManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �^�C�g���}�l�[�W���[�̏�����
		if (FAILED(pTitleManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �^�C�g���}�l�[�W���[�̔j��
			SAFE_DELETE(pTitleManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pTitleManager;
	}
}

//============================================================
//	�j������
//============================================================
void CTitleManager::Release(CTitleManager *&prTitleManager)
{
	// �^�C�g���}�l�[�W���[�̏I��
	assert(prTitleManager != nullptr);
	prTitleManager->Uninit();

	// �������J��
	SAFE_DELETE(prTitleManager);
}

//============================================================
//	�t�F�[�h�A�E�g�̍X�V����
//============================================================
void CTitleManager::UpdateFade(void)
{
	// �ϐ���錾
	D3DXCOLOR colFade = m_pFade->GetColor();	// �t�F�[�h�̐F

	if (colFade.a > 0.0f)
	{ // �����ɂȂ��Ă��Ȃ��ꍇ

		// �����x�����Z
		colFade.a -= fade::SUB_ALPHA;
	}
	else
	{ // �����ɂȂ����ꍇ

		// �����x��␳
		colFade.a = 0.0f;

		// �`�������ݒ�ɂ���
		m_pLogo->SetEnableDraw(true);

		// �X�^�[�g��`�悷��ݒ�ɂ���
		m_pStart->SetEnableDraw(true);

		// ��Ԃ�ύX
		m_state = STATE_SCALE;	// �^�C�g���k�����
	}

	// �����x�𔽉f
	m_pFade->SetColor(colFade);
}

//============================================================
// �^�C�g���g��
//============================================================
void CTitleManager::UpdateScale(void)
{
	if (m_fScale < 1.0f)
	{ // �g�嗦���ő�l��菬�����ꍇ

		// �g�嗦�����Z
		m_fScale += logo::SUB_SCALE;

		// �^�C�g�����S�̑傫����ݒ�
		m_pLogo->SetVec3Sizing(logo::SIZE * m_fScale);
	}
	else
	{ // �g�嗦���ő�l�ȏ�̏ꍇ

		if (m_nWaitCounter >= logo::WAIT_TIME)
		{ // ��莞�Ԍo�����ꍇ

			// �^�C�g���ړ��̑ҋ@���Ԃ�������
			m_nWaitCounter = 0;

			// �g�嗦��␳
			m_fScale = 1.0f;

			// �^�C�g�����S�̑傫����ݒ�
			m_pLogo->SetVec3Sizing(logo::SIZE);

			// ��Ԃ�ύX
			m_state = STATE_MOVE;	// �^�C�g���ړ����

			// �J�����̍X�V���ĊJ
			GET_MANAGER->GetCamera()->SetEnableUpdate(true);

			// �T�E���h�̍Đ�
			GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
		}
		else
		{ // ��莞�Ԍo���ĂȂ��ꍇ

			// �^�C�g���ړ��̑ҋ@���ԉ��Z
			m_nWaitCounter++;
		}
	}
}

//============================================================
//	�^�C�g���ړ��̍X�V����
//============================================================
void CTitleManager::UpdateMove(void)
{
	D3DXVECTOR3 pos;		// �ʒu

	// �ʒu�擾
	pos = m_pLogo->GetVec3Position();

	if (pos.x >= 130.0f)
	{ // ����̈ʒu�ɓ��B����܂�

		// �ʒu�X�V
		pos.x -= m_fMove;

		// �ʒu��ݒ�
		m_pLogo->SetVec3Position(pos);
	}
	else
	{
		// ��Ԃ�ύX
		m_state = STATE_WAIT;	// �J�ڑҋ@���

		// �J�����̍X�V���ĊJ
		GET_MANAGER->GetCamera()->SetEnableUpdate(true);

		// �T�E���h�̍Đ�
		GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
	}
}

//============================================================
//	�J�ڌ���̍X�V����
//============================================================
void CTitleManager::UpdateStart(void)
{
	// �|�C���^��錾
	CInputKeyboard	*pKeyboard	= GET_INPUTKEY;	// �L�[�{�[�h
	CInputPad		*pPad		= GET_INPUTPAD;	// �p�b�h

	if (pKeyboard->IsTrigger(DIK_RETURN)
	||  pKeyboard->IsTrigger(DIK_SPACE)
	||  pPad->IsTrigger(CInputPad::KEY_A)
	||  pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X)
	||  pPad->IsTrigger(CInputPad::KEY_Y)
	||  pPad->IsTrigger(CInputPad::KEY_START))
	{
		if (m_state != STATE_WAIT && m_state != STATE_TRANS)
		{ // �J�ڑҋ@ && �J�ڏ�Ԃł͂Ȃ��ꍇ

			// ���o�X�L�b�v
			SkipStaging();

			// �T�E���h�̍Đ�
			GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_001);	// ���艹01
		}
		else if(m_state != STATE_TRANS)
		{ // �J�ڑҋ@��Ԃ̏ꍇ

			// �J�ڏ�Ԃɂ���
			m_state = STATE_TRANS;
		}
	}
}

//============================================================
// �X�^�[�g�_��
//============================================================
void CTitleManager::UpdateBlink(void)
{
	D3DXCOLOR col;		// �F

	if (GET_MANAGER->GetFade()->GetState() == CFade::FADE_NONE)
	{ // �t�F�[�h���ł͂Ȃ��ꍇ

		// �F�擾
		col = m_pStart->GetColor();

		if (m_startState == STARTSTATE_APPEAR)
		{ // �o����Ԃ̏ꍇ

			// �����x���Z
			col.a += start::SUB_ALPHA;

			if (col.a >= 1.0f)
			{ // ���S�ɕs�����̏ꍇ

				// �����x�␳
				col.a = 1.0f;

				// ���ŏ�Ԃɂ���
				m_startState = STARTSTATE_DISAPPEAR;
			}
		}
		else if (m_startState == STARTSTATE_DISAPPEAR)
		{ // ���ŏ�Ԃ̏ꍇ

			// �����x���Z
			col.a -= start::SUB_ALPHA;

			if (col.a <= 0.0f)
			{ // ���S�ɓ����̏ꍇ

				// �����x�␳
				col.a = 0.0f;

				// ���ŏ�Ԃɂ���
				m_startState = STARTSTATE_APPEAR;
			}
		}
		
		// �F�ݒ�
		m_pStart->SetColor(col);
	}
}

//============================================================
// �J��
//============================================================
void CTitleManager::UpdateTrans(void)
{
	if (GET_MANAGER->GetFade()->GetState() == CFade::FADE_NONE)
	{ // �t�F�[�h���ł͂Ȃ��ꍇ

		D3DXCOLOR col;	// �F

		// �X�^�[�g�̐F���擾
		col = m_pStart->GetColor();

		if (col.a >= 0.0f)
		{ // ��������Ȃ��ꍇ

			// �g�嗦�����Z
			m_fScale += start::SUB_SCALE;
			col.a -= start::SUB_ALPHA;

			// �X�^�[�g�̑傫����ݒ�
			m_pStart->SetVec3Sizing(start::SIZE * m_fScale);

			// �X�^�[�g�̐F��ݒ�
			m_pStart->SetColor(col);
		}
		else
		{ // �����̏ꍇ

			// ��Ԃ�ύX
			m_state = STATE_NONE;	// �������Ȃ����

			// �V�[���̐ݒ�
			GET_MANAGER->SetScene(CScene::MODE_GAME);	// �Q�[�����

			// �T�E���h�̍Đ�
			GET_MANAGER->GetSound()->Play(CSound::LABEL_SE_DECISION_000);	// ���艹00
		}
	}
}

//============================================================
//	���o�X�L�b�v����
//============================================================
void CTitleManager::SkipStaging(void)
{
	// �^�C�g�����S�̑傫����ݒ�
	m_pLogo->SetVec3Sizing(logo::SIZE);

	// �^�C�g�����S�̈ʒu��ݒ�
	m_pLogo->SetVec3Position(logo::STOP_POS);

	// �`�������ݒ�ɂ���
	m_pLogo->SetEnableDraw(true);

	// �t�F�[�h�𓧖��ɂ���
	m_pFade->SetColor(XCOL_ABLACK);

	// �X�^�[�g��`�悷��ݒ�ɂ���
	m_pStart->SetEnableDraw(true);

	// �J�����̍X�V���ĊJ
	GET_MANAGER->GetCamera()->SetEnableUpdate(true);

	// ��Ԃ�ύX
	m_state = STATE_WAIT;	// �J�ڑҋ@���
}

