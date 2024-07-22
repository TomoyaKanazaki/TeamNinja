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
#include "timeUI.h"

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

		const char* TEXTURE		= "data\\TEXTURE\\resultFade000.png";	// �t�F�[�h�e�N�X�`��
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(800.0f, 1770.0f, 0.0f);	// �t�F�[�h�傫��
		const D3DXCOLOR	  COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);	// �t�F�[�h�F
		const D3DXVECTOR3 UP_POS	= D3DXVECTOR3(FADE_CENT, -900.0f, 0.0f);		// �t�F�[�h��ʒu
		const D3DXVECTOR3 CENT_POS	= D3DXVECTOR3(FADE_CENT, SCREEN_CENT.y, 0.0f);	// �t�F�[�h�����ʒu
		const D3DXVECTOR3 DOWN_POS	= D3DXVECTOR3(FADE_CENT, SCREEN_HEIGHT + 900.0f, 0.0f);			// �t�F�[�h���ʒu
		const D3DXVECTOR3 UP_MIDDLE_POS		= D3DXVECTOR3(FADE_CENT, UP_POS.y + 300.0f, 0.0f);		// �t�F�[�h�㒆���ʒu
		const D3DXVECTOR3 DOWN_MIDDLE_POS	= D3DXVECTOR3(FADE_CENT, DOWN_POS.y - 300.0f, 0.0f);	// �t�F�[�h�������ʒu
	}

	namespace title
	{
		const char *FONT	= "data\\FONT\\��S�V�b�N.otf";	// �t�H���g�p�X
		const bool	ITALIC	= false;			// �C�^���b�N
		const float	WAIT_TIME		= 0.5f;		// �^�C�g���ҋ@����
		const float	CHAR_HEIGHT		= 100.0f;	// �����c��
		const float	LINE_HEIGHT		= 100.0f;	// �s�ԏc��
		const float	WAIT_TIME_NOR	= 0.08f;	// �����\���̑ҋ@����

		const D3DXVECTOR3 POS = D3DXVECTOR3(540.0f, 35.0f, 0.0f);	// �e�L�X�g�ʒu
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// ���z�u
		const CText2D::EAlignY	 ALIGN_Y = CText2D::YALIGN_TOP;		// �c�z�u
	}

	namespace stamp
	{
		const float	MOVE_TIME	= 0.3f;	// �ړ�����
		const float	WAIT_TIME	= 0.5f;	// �n���R�ҋ@����
		const char* TEXTURE		= "data\\TEXTURE\\resultStamp000.png";		// �n���R�e�N�X�`��
		const D3DXVECTOR3 POS	= D3DXVECTOR3(1020.0f, 145.0f, 0.0f);		// �n���R�ʒu
		const D3DXVECTOR3 ROT	= D3DXVECTOR3(0.0f, 0.0f, -0.16f);			// �n���R����
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(454.0f, 147.0f, 0.05f);	// �n���R�ڕW�傫��
		const D3DXVECTOR3 INIT_SIZE	= DEST_SIZE * 10.0f;					// �n���R�����傫��
		const D3DXVECTOR3 DIFF_SIZE = stamp::DEST_SIZE - stamp::INIT_SIZE;	// �n���R�����傫��
	}

	namespace time
	{
		const char	  *FONT		= "data\\FONT\\��S�V�b�N.otf";	// �t�H���g�p�X
		const wchar_t *STRING	= L"�C�����s����";	// ������
		const bool	  ITALIC	= false;	// �C�^���b�N
		const float	  MOVE_TIME	= 0.15f;	// �ړ�����
		const float	  WAIT_TIME	= 0.5f;		// �^�C�g���ҋ@����
		const float DEST_HEIGHT = 100.0f;	// �����ڕW�c��
		const float INIT_HEIGHT = DEST_HEIGHT * 10.0f;	// ���������c��
		const float DIFF_HEIGHT = time::DEST_HEIGHT - time::INIT_HEIGHT;	// ���������c��

		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// ���z�u
		const D3DXVECTOR3 POS = D3DXVECTOR3(540.0f, 322.0f, 0.0f);	// �ʒu
	}

	namespace val_time
	{
		const D3DXVECTOR3 POS			= D3DXVECTOR3(770.0f, 405.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 VAL_SIZE		= D3DXVECTOR3(46.8f * 1.4f, 62.4f * 1.4f, 0.0f);		// �����傫��
		const D3DXVECTOR3 PART_SIZE		= D3DXVECTOR3(27.3f * 1.4f, 62.4f * 1.4f, 0.0f);		// ��؂�傫��
		const D3DXVECTOR3 VAL_SPACE		= D3DXVECTOR3(VAL_SIZE.x * 0.85f, 0.0f, 0.0f);	// ������
		const D3DXVECTOR3 PART_SPACE	= D3DXVECTOR3(PART_SIZE.x * 0.85f, 0.0f, 0.0f);	// ��؂��
		const CValue::EType		TYPE	= CValue::TYPE_NORMAL;		// �������
		const CTimeUI::EAlignX	ALIGN_X	= CTimeUI::XALIGN_LEFT;		// ���z�u
		const CTimeUI::EAlignY	ALIGN_Y	= CTimeUI::YALIGN_CENTER;	// �c�z�u
	}

	namespace god
	{
		const char	  *FONT		= "data\\FONT\\��S�V�b�N.otf";	// �t�H���g�p�X
		const wchar_t *STRING	= L"�l�������_��";	// ������
		const bool	  ITALIC	= false;	// �C�^���b�N
		const float	  HEIGHT	= 100.0f;	// �����c��
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// ���z�u
		const D3DXVECTOR3 POS = D3DXVECTOR3(540.0f, 525.0f, 0.0f);	// �ʒu
	}

	namespace icon_god
	{
		const D3DXVECTOR3 POS	= D3DXVECTOR3(865.0f, 620.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(140.0f, 0.0f, 0.0f);		// ��
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
	m_pStamp	(nullptr),		// �n���R���
	m_pTime		(nullptr),		// ���s���ԃ^�C�g�����
	m_pTimeVal	(nullptr),		// ���s���ԏ��
	m_pGodItem	(nullptr),		// �_��^�C�g�����
	m_state		(STATE_NONE),	// ���
	m_fMoveY	(0.0f),			// �c�ړ���
	m_fCurTime	(0.0f)			// ���݂̑ҋ@����
{
	// �����o�ϐ����N���A
	memset(&m_apGodItemIcon[0], 0, sizeof(m_apGodItemIcon));	// �_��A�C�R�����
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
	memset(&m_apGodItemIcon[0], 0, sizeof(m_apGodItemIcon));	// �_��A�C�R�����
	m_pFade		= nullptr;			// �t�F�[�h���
	m_pTitle	= nullptr;			// �^�C�g�����
	m_pStamp	= nullptr;			// �n���R���
	m_pTime		= nullptr;			// ���s���ԃ^�C�g�����
	m_pTimeVal	= nullptr;			// ���s���ԏ��
	m_pGodItem	= nullptr;			// �_��^�C�g�����
	m_state		= STATE_FADE_WAIT;	// ���
	m_fMoveY	= 0.0f;				// �c�ړ���
	m_fCurTime	= 0.0f;				// ���݂̑ҋ@����

	//--------------------------------------------------------
	//	�t�F�[�h�̐��� / �����ݒ�
	//--------------------------------------------------------
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

	// �e�N�X�`��������
	m_pFade->BindTexture(fade::TEXTURE);

	// �D�揇�ʂ�ݒ�
	m_pFade->SetPriority(PRIORITY);

	// ���x����ݒ�
	m_pFade->SetLabel(CObject::LABEL_UI);	// �����j��/�X�V�����郉�x��

	//--------------------------------------------------------
	//	�^�C�g���̐��� / �����ݒ�
	//--------------------------------------------------------
	// �^�C�g���̐���
	m_pTitle = CScrollText2D::Create
	( // ����
		title::FONT,			// �t�H���g�p�X
		title::ITALIC,			// �C�^���b�N
		title::POS,				// ���_�ʒu
		title::WAIT_TIME_NOR,	// �����\���̑ҋ@����
		title::CHAR_HEIGHT,		// �����c��
		title::LINE_HEIGHT,		// �s�ԏc��
		title::ALIGN_X,			// ���z�u
		title::ALIGN_Y			// �c�z�u
	);
	if (m_pTitle == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pTitle->SetPriority(PRIORITY);

	// TODO�F�����Ƀ^�C�g��
	m_pTitle->AddString(L"������");
	m_pTitle->AddString(L"�@�X�e�[�W��");

	//--------------------------------------------------------
	//	�n���R�̐��� / �����ݒ�
	//--------------------------------------------------------
	// �n���R�̐���
	m_pStamp = CObject2D::Create
	( // ����
		stamp::POS,			// �ʒu
		stamp::INIT_SIZE,	// �傫��
		stamp::ROT			// ����
	);
	if (m_pStamp == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`��������
	m_pStamp->BindTexture(stamp::TEXTURE);

	// �D�揇�ʂ�ݒ�
	m_pStamp->SetPriority(PRIORITY);

	// ���x����ݒ�
	m_pStamp->SetLabel(CObject::LABEL_UI);	// �����j��/�X�V�����郉�x��

	// �����`���OFF�ɂ���
	m_pStamp->SetEnableDraw(false);

	//--------------------------------------------------------
	//	���s���ԃ^�C�g���̐��� / �����ݒ�
	//--------------------------------------------------------
	// ���s���ԃ^�C�g���̐���
	m_pTime = CString2D::Create
	( // ����
		time::FONT,			// �t�H���g�p�X
		time::ITALIC,		// �C�^���b�N
		time::STRING,		// �w�蕶����
		time::POS,			// ���_�ʒu
		time::INIT_HEIGHT,	// �����c��
		time::ALIGN_X		// ���z�u
	);
	if (m_pTime == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pTime->SetPriority(PRIORITY);

	// �����`���OFF�ɂ���
	m_pTime->SetEnableDraw(false);

	//--------------------------------------------------------
	//	���s���Ԃ̐��� / �����ݒ�
	//--------------------------------------------------------
	// ���s���Ԃ̐���
	m_pTimeVal = CTimeUI::Create
	( // ����
		0.0f,					// �\������	// TODO�F�o�ߎ��Ԃ�ݒ�
		val_time::POS,			// �ʒu
		val_time::VAL_SIZE,		// �����̑傫��
		val_time::PART_SIZE,	// ��؂�̑傫��
		val_time::VAL_SPACE,	// �����̋�
		val_time::PART_SPACE,	// ��؂�̋�
		val_time::TYPE,			// �������
		val_time::ALIGN_X,		// ���z�u
		val_time::ALIGN_Y		// �c�z�u
	);
	if (m_pTimeVal == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pTimeVal->SetPriority(PRIORITY);

	// �����`���OFF�ɂ���
	m_pTimeVal->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�_��^�C�g���̐��� / �����ݒ�
	//--------------------------------------------------------
	// �_��^�C�g���̐���
	m_pGodItem = CString2D::Create
	( // ����
		god::FONT,		// �t�H���g�p�X
		god::ITALIC,	// �C�^���b�N
		god::STRING,	// �w�蕶����
		god::POS,		// ���_�ʒu
		god::HEIGHT,	// �����c��
		god::ALIGN_X	// ���z�u
	);
	if (m_pGodItem == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pGodItem->SetPriority(PRIORITY);

	// �����`���OFF�ɂ���
	m_pGodItem->SetEnableDraw(false);

	//--------------------------------------------------------
	//	�_��A�C�R���̐��� / �����ݒ�
	//--------------------------------------------------------
	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // �_��̑������J��Ԃ�

		// �A�C�R�������ʒu���v�Z
		D3DXVECTOR3 pos = icon_god::POS + (icon_god::SPACE * (float)i);	// �����ʒu

		// �_��A�C�R���̐���
		m_apGodItemIcon[i] = CObject2D::Create(pos, icon_god::SIZE);
		if (m_apGodItemIcon[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apGodItemIcon[i]->SetPriority(PRIORITY);

		// ���x����ݒ�
		m_apGodItemIcon[i]->SetLabel(CObject::LABEL_UI);	// �����j��/�X�V�����郉�x��

		// �����`���OFF�ɂ���
		m_apGodItemIcon[i]->SetEnableDraw(false);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CResultManager::Uninit(void)
{

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

	case STATE_FADE_WAIT:

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

	case STATE_STAGE_WAIT:

		// �X�e�[�W�\���ҋ@�̍X�V
		UpdateStageWait(fDeltaTime);
		break;

	case STATE_STAGE_TITLE:

		// �X�e�[�W�^�C�g���̍X�V
		UpdateStageTitle(fDeltaTime);
		break;

	case STATE_CLEAR_STAMP:

		// �N���A�n���R�����̍X�V
		UpdateStamp(fDeltaTime);
		break;

	case STATE_TIME_WAIT:

		// ���s���ԕ\���ҋ@�̍X�V
		UpdateTimeWait(fDeltaTime);
		break;

	case STATE_TIME_TITLE:

		// ���s���ԃ^�C�g���\���̍X�V
		UpdateTitleTime(fDeltaTime);
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
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= fade::FADEWAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

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
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= fade::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

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

		// �X�e�[�W�\���ҋ@��Ԃɂ���
		m_state = STATE_STAGE_WAIT;
	}

	m_pFade->SetVec3Position(posFade);	// �t�F�[�h�ʒu�𔽉f
}

//============================================================
//	�X�e�[�W�\���ҋ@�̍X�V����
//============================================================
void CResultManager::UpdateStageWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= title::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �X�e�[�W�^�C�g���̕���������J�n����
		m_pTitle->SetEnableScroll(true);

		// �X�e�[�W�^�C�g����Ԃɂ���
		m_state = STATE_STAGE_TITLE;
	}
}

//============================================================
//	�X�e�[�W�^�C�g���̍X�V����
//============================================================
void CResultManager::UpdateStageTitle(const float fDeltaTime)
{
	// �������肪�I�����Ă��Ȃ��ꍇ������
	if (m_pTitle->IsScroll()) { return; }

	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= stamp::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �n���R�̎����`���ON�ɂ���
		m_pStamp->SetEnableDraw(true);

		// �N���A�n���R������Ԃɂ���
		m_state = STATE_CLEAR_STAMP;
	}
}

//============================================================
//	�N���A�n���R�����̍X�V����
//============================================================
void CResultManager::UpdateStamp(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, stamp::MOVE_TIME);

	// �n���R�̑傫���𔽉f
	m_pStamp->SetVec3Sizing(stamp::INIT_SIZE + (stamp::DIFF_SIZE * fRate));

	if (m_fCurTime >= stamp::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �n���R�̑傫����␳
		m_pStamp->SetVec3Sizing(stamp::DEST_SIZE);

		// ���s���ԕ\���ҋ@��Ԃɂ���
		m_state = STATE_TIME_WAIT;
	}
}

//============================================================
//	���s���ԕ\���ҋ@�̍X�V����
//============================================================
void CResultManager::UpdateTimeWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= time::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// ���s���ԃ^�C�g���̎����`���ON�ɂ���
		m_pTime->SetEnableDraw(true);

		// ���s���ԃ^�C�g���\����Ԃɂ���
		m_state = STATE_TIME_TITLE;
	}
}

//============================================================
//	���s���ԃ^�C�g���\���̍X�V����
//============================================================
void CResultManager::UpdateTitleTime(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, time::MOVE_TIME);

	// ���s���ԃ^�C�g���̑傫���𔽉f
	m_pTime->SetCharHeight(time::INIT_HEIGHT + (time::DIFF_HEIGHT * fRate));

	if (m_fCurTime >= time::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// ���s���ԃ^�C�g���̑傫����␳
		m_pTime->SetCharHeight(time::DEST_HEIGHT);

		// �ҋ@��Ԃɂ���
		m_state = STATE_WAIT;
	}
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
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= fade::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

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
