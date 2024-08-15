//============================================================
//
//	�_��l�����o�}�l�[�W���[���� [godItemManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "godItemManager.h"
#include "retentionManager.h"
#include "manager.h"
#include "camera.h"
#include "fade.h"
#include "loadtext.h"
#include "stage.h"
#include "object2D.h"
#include "scrollString2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// �_��l�����o�}�l�[�W���[�̗D�揇��

	namespace title
	{
		const char *TEXTURE		= "data\\TEXTURE\\get_magatama.png";	// �e�N�X�`���p�X
		const float	MOVE_TIME	= 0.68f;	// �ړ�����
		const float	WAIT_TIME	= 0.15f;	// ���l�ҋ@����
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(632.0f, 184.0f, 0.0f);	// �傫��
		const D3DXCOLOR DEST_COL	= XCOL_WHITE;			// �ڕW�F
		const D3DXCOLOR INIT_COL	= XCOL_AWHITE;			// �����F
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;	// �����F
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(SCREEN_CENT.x, 445.0f, 0.0f);		// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// �����ʒu
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// �����ʒu
	}

	namespace line
	{
		const float	MOVE_TIME	= 0.5f;	// �ړ�����
		const float	WAIT_TIME	= 0.5f;	// �ҋ@����
		const D3DXVECTOR3 POS	= D3DXVECTOR3(SCREEN_CENT.x, 530.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(980.0f, 20.0f, 0.0f);		// �ڕW�傫��
		const D3DXVECTOR3 INIT_SIZE	= D3DXVECTOR3(0.0f, DEST_SIZE.y, 0.0f);	// �����傫��
		const D3DXVECTOR3 DIFF_SIZE	= DEST_SIZE - INIT_SIZE;				// �����傫��
	}

	namespace name
	{
		const char *FONT	= "data\\FONT\\��S�V�b�N.otf";	// �t�H���g�p�X
		const bool	ITALIC	= false;			// �C�^���b�N
		const float	WAIT_TIME		= 0.5f;		// �ҋ@����
		const float	CHAR_HEIGHT		= 70.0f;	// �����c��
		const float	WAIT_TIME_NOR	= 0.012f;	// �����\���̑ҋ@����
		const D3DXVECTOR3 POS = D3DXVECTOR3(SCREEN_CENT.x, 580.0f, 0.0f);	// �ʒu
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_CENTER;		// ���z�u
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CGodItemManager::AFuncUpdateState CGodItemManager::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,						// �������Ȃ��X�V
	&CGodItemManager::UpdateLine,	// �����o���X�V
	&CGodItemManager::UpdateTitle,	// �^�C�g���o���X�V
	&CGodItemManager::UpdateName,	// ���O�o���X�V
	&CGodItemManager::UpdateWait,	// �ҋ@�X�V
	&CGodItemManager::UpdateEnd,	// �I���X�V
};

//************************************************************
//	�e�N���X [CGodItemManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGodItemManager::CGodItemManager() :
	m_pTitle	(nullptr),		// �^�C�g�����
	m_pLine		(nullptr),		// �������
	m_pName		(nullptr),		// ���O���
	m_state		(STATE_NONE),	// ���
	m_fCurTime	(0.0f)			// ���݂̑ҋ@����
{
	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CGodItemManager::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	�f�X�g���N�^
//============================================================
CGodItemManager::~CGodItemManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGodItemManager::Init(void)
{
	// �����o�ϐ���������
	m_state		= STATE_LINE;	// ���	// TODO�F���Ƃŕς��
	m_pTitle	= nullptr;	// �^�C�g�����
	m_pLine		= nullptr;	// �������
	m_pName		= nullptr;	// ���O���
	m_fCurTime	= 0.0f;		// ���݂̑ҋ@����

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
	m_pTitle->SetPriority(PRIORITY);

	// ���x����ݒ�
	m_pTitle->SetLabel(CObject::LABEL_UI);	// �����j��/�X�V�����郉�x��

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
	m_pLine->SetPriority(PRIORITY);

	// ���x����ݒ�
	m_pLine->SetLabel(CObject::LABEL_UI);	// �����j��/�X�V�����郉�x��

	//--------------------------------------------------------
	//	���O�̐��� / �����ݒ�
	//--------------------------------------------------------
	// ���O�̐���
	m_pName = CScrollString2D::Create
	( // ����
		name::FONT,				// �t�H���g�p�X
		name::ITALIC,			// �C�^���b�N
		L"",					// �\��������
		name::POS,				// ���_�ʒu
		name::WAIT_TIME_NOR,	// �����\���̑ҋ@����
		name::CHAR_HEIGHT,		// �����c��
		name::ALIGN_X			// ���z�u
	);
	if (m_pName == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pName->SetPriority(PRIORITY);

	// TODO
#if 0
	// �e�L�X�g������
	loadtext::BindText(m_pName, loadtext::LoadText(GET_STAGE->GetCurMapStagePass().c_str(), 0));
#else
	m_pName->SetString(L"���e�L�X�g�@�����ɏ�i�𕶏͉�");
#endif

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGodItemManager::Uninit(void)
{
	// �^�C�g���̏I��
	SAFE_UNINIT(m_pTitle);

	// �����̏I��
	SAFE_UNINIT(m_pLine);

	// ���O�̏I��
	SAFE_UNINIT(m_pName);
}

//============================================================
//	�X�V����
//============================================================
void CGodItemManager::Update(const float fDeltaTime)
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
CGodItemManager *CGodItemManager::Create(void)
{
	// �_��l�����o�}�l�[�W���[�̐���
	CGodItemManager *pGodItemManager = new CGodItemManager;
	if (pGodItemManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �_��l�����o�}�l�[�W���[�̏�����
		if (FAILED(pGodItemManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �_��l�����o�}�l�[�W���[�̔j��
			SAFE_DELETE(pGodItemManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pGodItemManager;
	}
}

//============================================================
//	�j������
//============================================================
void CGodItemManager::Release(CGodItemManager *&prGodItemManager)
{
	// �_��l�����o�}�l�[�W���[�̏I��
	assert(prGodItemManager != nullptr);
	prGodItemManager->Uninit();

	// �������J��
	SAFE_DELETE(prGodItemManager);
}

//============================================================
//	�����o���̍X�V����
//============================================================
void CGodItemManager::UpdateLine(const float fDeltaTime)
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
void CGodItemManager::UpdateTitle(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, title::MOVE_TIME);

	// ���s���Ԃ̐F�𔽉f
	m_pTitle->SetColor(title::INIT_COL + (title::DIFF_COL * fRate));

	// ���s���Ԃ̈ʒu�𔽉f
	m_pTitle->SetVec3Position(title::INIT_POS + (title::DIFF_POS * fRate));

	if (m_fCurTime >= title::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// ���s���Ԃ̐F��␳
		m_pTitle->SetColor(title::DEST_COL);

		// ���s���Ԃ̈ʒu��␳
		m_pTitle->SetVec3Position(title::DEST_POS);

		// ���O�̕���������J�n����
		m_pName->SetEnableScroll(true);

		// �ҋ@��Ԃɂ���
		m_state = STATE_WAIT;
	}
}

//============================================================
//	���O�o���̍X�V����
//============================================================
void CGodItemManager::UpdateName(const float fDeltaTime)
{
	if (!m_pName->IsScroll())
	{ // �������肪�I�������ꍇ

		// �ҋ@��Ԃɂ���
		m_state = STATE_WAIT;
	}
}

//============================================================
//	�ҋ@�̍X�V����
//============================================================
void CGodItemManager::UpdateWait(const float fDeltaTime)
{

}

//============================================================
//	�I���̍X�V����
//============================================================
void CGodItemManager::UpdateEnd(const float fDeltaTime)
{

}

//============================================================
//	�X�L�b�v����̍X�V����
//============================================================
void CGodItemManager::UpdateSkip(void)
{
	//if (m_state >= STATE_WAIT)	// TODO�F�܂��Ȃ�����WAIT�͍��Ȃ��Ƃ�
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
void CGodItemManager::SkipStaging(void)
{
	// TODO�F���o�X�L�b�v�����낤��
#if 0
	CRetentionManager::EWin win = GET_RETENTION->GetWin();	// �N���A��

	// �N���A�}�l�[�W���[�X�V��Ԃɂ���
	m_state = STATE_CLEAR;

	// �_��l�����o�J������ڕW�ʒu�ɐݒ�
	GET_MANAGER->GetCamera()->SetDestGodItem();

	// �t�F�[�h��ڕW�ʒu�ɐݒ�
	m_pFade->SetVec3Position(fade::CENT_POS);

	// �S������\��������
	m_pTitle->SetEnableDraw(true);

	// �N���A���Ƃ̃n���R�e�N�X�`��������
	m_pStamp->BindTexture(stamp::TEXTURE[win]);

	// �n���R�����o��̌����ڂɂ���
	m_pStamp->SetEnableDraw(true);				// �����`���ON�ɂ���
	m_pStamp->SetVec3Sizing(stamp::DEST_SIZE);	// �ڕW�T�C�Y�ɐݒ�

	if (m_pClear == nullptr)
	{ // �N���A�}�l�[�W���[����������Ă��Ȃ��ꍇ

		// �N���A�}�l�[�W���[�̐���
		m_pClear = CClearManager::Create(win);
		assert(m_pClear != nullptr);
	}

	// �N���A�}�l�[�W���[�̉��o�X�L�b�v
	assert(m_pClear != nullptr);
	m_pClear->SkipStaging();
#endif
}
