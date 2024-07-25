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
#include "camera.h"
#include "fade.h"
#include "clearManager.h"
#include "object2D.h"
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
		const float	LINE_HEIGHT		= 80.0f;	// �s�ԏc��
		const float	WAIT_TIME_NOR	= 0.08f;	// �����\���̑ҋ@����
		const D3DXVECTOR3 POS = D3DXVECTOR3(540.0f, 40.0f, 0.0f);	// �e�L�X�g�ʒu
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// ���z�u
		const CText2D::EAlignY	 ALIGN_Y = CText2D::YALIGN_TOP;		// �c�z�u
	}

	namespace stamp
	{
		const char* TEXTURE[] =	// �n���R�e�N�X�`��
		{
			nullptr,								// �N���A���薳��
			"data\\TEXTURE\\resultStamp001.png",	// �N���A���s�e�N�X�`��
			"data\\TEXTURE\\resultStamp000.png",	// �N���A�����e�N�X�`��
		};

		const float	MOVE_TIME	= 0.3f;	// �ړ�����
		const float	WAIT_TIME	= 0.5f;	// �n���R�ҋ@����
		const D3DXVECTOR3 POS	= D3DXVECTOR3(1020.0f, 140.0f, 0.0f);		// �n���R�ʒu
		const D3DXVECTOR3 ROT	= D3DXVECTOR3(0.0f, 0.0f, -0.16f);			// �n���R����
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(454.0f, 147.0f, 0.0f);	// �n���R�ڕW�傫��
		const D3DXVECTOR3 INIT_SIZE	= DEST_SIZE * 10.0f;					// �n���R�����傫��
		const D3DXVECTOR3 DIFF_SIZE = DEST_SIZE - INIT_SIZE;				// �n���R�����傫��
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CResultManager::AFuncUpdateState CResultManager::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,								// �������Ȃ��X�V
	&CResultManager::UpdateFadeWait,		// �t�F�[�h�ҋ@�X�V
	&CResultManager::UpdateFadeIn,			// �t�F�[�h�C���X�V
	&CResultManager::UpdateFadeInWait,		// �t�F�[�h�C���ҋ@�X�V
	&CResultManager::UpdateFadeInAccel,		// �t�F�[�h�C�������X�V
	&CResultManager::UpdateStageWait,		// �X�e�[�W�\���ҋ@�X�V
	&CResultManager::UpdateStageTitle,		// �X�e�[�W�^�C�g���X�V
	&CResultManager::UpdateStamp,			// �n���R�����X�V
	&CResultManager::UpdateClear,			// �N���A�X�V
	&CResultManager::UpdateFadeOut,			// �t�F�[�h�A�E�g�X�V
	&CResultManager::UpdateFadeOutWait,		// �t�F�[�h�A�E�g�ҋ@�X�V
	&CResultManager::UpdateFadeOutAccel,	// �t�F�[�h�A�E�g�����X�V
	&CResultManager::UpdateEnd,				// �I���X�V
};

//************************************************************
//	�e�N���X [CResultManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CResultManager::CResultManager() :
	m_pClear	(nullptr),		// �N���A�}�l�[�W���[���
	m_pFade		(nullptr),		// �t�F�[�h���
	m_pTitle	(nullptr),		// �^�C�g�����
	m_pStamp	(nullptr),		// �n���R���
	m_state		(STATE_NONE),	// ���
	m_fMoveY	(0.0f),			// �c�ړ���
	m_fCurTime	(0.0f)			// ���݂̑ҋ@����
{
	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CResultManager::STATE_MAX, "ERROR : State Count Mismatch");
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
	m_state		= STATE_FADE_WAIT;	// ���
	m_pClear	= nullptr;	// �N���A�}�l�[�W���[���
	m_pFade		= nullptr;	// �t�F�[�h���
	m_pTitle	= nullptr;	// �^�C�g�����
	m_pStamp	= nullptr;	// �n���R���
	m_fMoveY	= 0.0f;		// �c�ړ���
	m_fCurTime	= 0.0f;		// ���݂̑ҋ@����

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

	// �D�揇�ʂ�ݒ�
	m_pStamp->SetPriority(PRIORITY);

	// ���x����ݒ�
	m_pStamp->SetLabel(CObject::LABEL_UI);	// �����j��/�X�V�����郉�x��

	// �����`���OFF�ɂ���
	m_pStamp->SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CResultManager::Uninit(void)
{
	// �N���A�}�l�[�W���[�̔j��
	SAFE_REF_RELEASE(m_pClear);
}

//============================================================
//	�X�V����
//============================================================
void CResultManager::Update(const float fDeltaTime)
{
	assert(m_state > NONE_IDX && m_state < STATE_MAX);
	if (m_aFuncUpdateState[m_state] != nullptr)
	{ // �X�V�֐����w�肳��Ă���ꍇ

		// �e��Ԃ��Ƃ̍X�V
		(this->*(m_aFuncUpdateState[m_state]))(fDeltaTime);
	}

	// TODO
	if (GET_INPUTKEY->IsTrigger(DIK_0))
	{
		// ���o�̃X�L�b�v
		SkipStaging();
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

		// �N���A���Ƃ̃n���R�e�N�X�`��������
		m_pStamp->BindTexture(stamp::TEXTURE[GET_RETENTION->GetWin()]);

		// �n���R������Ԃɂ���
		m_state = STATE_STAMP;
	}
}

//============================================================
//	�n���R�����̍X�V����
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

		// �N���A�}�l�[�W���[�̐���
		m_pClear = CClearManager::Create(GET_RETENTION->GetWin());
		assert(m_pClear != nullptr);

		// �N���A�}�l�[�W���[�X�V��Ԃɂ���
		m_state = STATE_CLEAR;
	}
}

//============================================================
//	�N���A�}�l�[�W���[�̍X�V����
//============================================================
void CResultManager::UpdateClear(const float fDeltaTime)
{
	// �N���A�}�l�[�W���[�̍X�V
	assert(m_pClear != nullptr);
	m_pClear->Update(fDeltaTime);

	if (m_pClear->IsEndState())
	{ // �N���A���Ƃ̏�ԍX�V���I�������ꍇ

		// �t�F�[�h�A�E�g��Ԃɂ���
		m_state = STATE_FADEOUT;
	}
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

		// �SUI�I�u�W�F�N�g�̈ړ�
		SetAllMove(D3DXVECTOR3(0.0f, m_fMoveY - (posFade.y - fade::DOWN_MIDDLE_POS.y), 0.0f));

		// �t�F�[�h���~�ʒu�ɕ␳
		posFade.y = fade::DOWN_MIDDLE_POS.y;

		// �ړ��ʂ�������
		m_fMoveY = 0.0f;

		// �t�F�[�h�A�E�g�ҋ@��Ԃɂ���
		m_state = STATE_FADEOUT_WAIT;
	}

	// �SUI�I�u�W�F�N�g�̈ړ�
	SetAllMove(D3DXVECTOR3(0.0f, m_fMoveY, 0.0f));

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

		// �SUI�I�u�W�F�N�g�̈ړ�
		SetAllMove(D3DXVECTOR3(0.0f, m_fMoveY - (posFade.y - fade::DOWN_POS.y), 0.0f));

		// �t�F�[�h���~�ʒu�ɕ␳
		posFade.y = fade::DOWN_POS.y;

		// �ړ��ʂ�������
		m_fMoveY = 0.0f;

		// �I����Ԃɂ���
		m_state = STATE_END;
	}

	// �SUI�I�u�W�F�N�g�̈ړ�
	SetAllMove(D3DXVECTOR3(0.0f, m_fMoveY, 0.0f));

	m_pFade->SetVec3Position(posFade);	// �t�F�[�h�ʒu�𔽉f
}

//============================================================
//	�I���̍X�V����
//============================================================
void CResultManager::UpdateEnd(const float fDeltaTime)
{
	// �t�F�[�h���̏ꍇ������
	if (GET_MANAGER->GetFade()->IsFade()) { return; }

	// �N���A�}�l�[�W���[�őI�����ꂽ�V�[���ɑJ�ڂ���
	GET_MANAGER->SetLoadScene(m_pClear->GetNext());
}

//============================================================
//	�SUI�I�u�W�F�N�g�̈ړ�����
//============================================================
void CResultManager::SetAllMove(const D3DXVECTOR3& rMove)
{
	// �^�C�g���̈ʒu���ړ�
	m_pTitle->SetVec3Position(m_pTitle->GetVec3Position() + rMove);

	// �n���R�̈ʒu���ړ�
	m_pStamp->SetVec3Position(m_pStamp->GetVec3Position() + rMove);

	// �N���A�}�l�[�W���[�̑SUI�I�u�W�F�N�g�ړ�
	assert(m_pClear != nullptr);
	m_pClear->SetAllMove(rMove);
}

//============================================================
//	���o�̃X�L�b�v����
//============================================================
void CResultManager::SkipStaging(void)
{
	CRetentionManager::EWin win = GET_RETENTION->GetWin();	// �N���A��

	// �N���A�}�l�[�W���[�X�V��Ԃɂ���
	m_state = STATE_CLEAR;

	// ���U���g�J������ڕW�ʒu�ɐݒ�
	GET_MANAGER->GetCamera()->SetDestResult();

	// �t�F�[�h��ڕW�ʒu�ɐݒ�
	m_pFade->SetVec3Position(fade::CENT_POS);

	// �S������\��������
	m_pTitle->SetEnableDraw(true);

	// �N���A���Ƃ̃n���R�e�N�X�`��������
	m_pStamp->BindTexture(stamp::TEXTURE[win]);

	// �n���R�����o��̌����ڂɂ���
	m_pStamp->SetEnableDraw(true);				// �����`���ON�ɂ���
	m_pStamp->SetVec3Sizing(stamp::DEST_SIZE);	// �ڕW�T�C�Y�ɐݒ�

	// �N���A�}�l�[�W���[�̐���
	m_pClear = CClearManager::Create(win);
	assert(m_pClear != nullptr);

	// �N���A�}�l�[�W���[�̉��o�X�L�b�v
	assert(m_pClear != nullptr);
	m_pClear->SkipStaging();
}
