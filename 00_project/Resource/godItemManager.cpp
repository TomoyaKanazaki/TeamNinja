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

#include "sceneGame.h"
#include "gameManager.h"
#include "timerUI.h"
#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIO_BG = 5;	// �w�i�̗D�揇��
	const int PRIO_UI = 6;	// UI�̗D�揇��

	namespace fade
	{
		const float	MOVE_TIME	 = 0.5f;	// �ϓ�����
		const float DEST_ALPHA	 = 0.5f;	// �ڕW�����x
		const float INIT_ALPHA	 = 0.0f;	// ���������x
		const float DIFF_ALPHA	 = DEST_ALPHA - INIT_ALPHA;	// ���������x
		const D3DXCOLOR INIT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, INIT_ALPHA);		// �����F
		const D3DXVECTOR3 SIZE	 = D3DXVECTOR3(SCREEN_SIZE.x, 260.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 POS	 = D3DXVECTOR3(SCREEN_CENT.x, 555.0f, 0.0f);	// �ʒu
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
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(980.0f, 20.0f, 0.0f);		// �ڕW�傫��
		const D3DXVECTOR3 INIT_SIZE	= D3DXVECTOR3(0.0f, DEST_SIZE.y, 0.0f);	// �����傫��
		const D3DXVECTOR3 DIFF_SIZE	= DEST_SIZE - INIT_SIZE;				// �����傫��
	}

	namespace name
	{
		const char *FONT	= "data\\FONT\\��S�V�b�N.otf";	// �t�H���g�p�X
		const bool	ITALIC	= false;			// �C�^���b�N
		const float	CHAR_HEIGHT		= 70.0f;	// �����c��
		const float	WAIT_TIME_NOR	= 0.012f;	// �����\���̑ҋ@����
		const D3DXVECTOR3 POS = D3DXVECTOR3(SCREEN_CENT.x, 640.0f, 0.0f);	// �ʒu
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_CENTER;		// ���z�u
	}

	namespace fall
	{
		const float	MOVE_TIME = 1.5f;	// �ړ�����
		const D3DXCOLOR DEST_COL	 = XCOL_AWHITE;			// �ڕW�F
		const D3DXCOLOR INIT_COL	 = XCOL_WHITE;			// �����F
		const D3DXCOLOR DIFF_COL	 = DEST_COL - INIT_COL;	// �����F
		const D3DXVECTOR3 OFFSET_POS = D3DXVECTOR3(0.0f, 30.0f, 0.0f);	// �����ʒu�I�t�Z�b�g
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CGodItemManager::AFuncUpdateState CGodItemManager::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,							// �������Ȃ��X�V
	&CGodItemManager::UpdateFadeOut,	// �t�F�[�h�A�E�g�X�V
	&CGodItemManager::UpdateLine,		// �����o���X�V
	&CGodItemManager::UpdateTitle,		// �^�C�g���o���X�V
	&CGodItemManager::UpdateName,		// ���O�o���X�V
	&CGodItemManager::UpdateWait,		// �ҋ@�X�V
	&CGodItemManager::UpdateFadeIn,		// �t�F�[�h�C���X�V
	&CGodItemManager::UpdateFall,		// UI�����X�V
	&CGodItemManager::UpdateEnd,		// �I���X�V
};
CGodItemManager* CGodItemManager::m_pThisClass = nullptr;	// ���g�̃C���X�^���X

//************************************************************
//	�e�N���X [CGodItemManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGodItemManager::CGodItemManager() : CObject(CObject::LABEL_MANAGER, CObject::SCENE_MAIN, CObject::DIM_2D, PRIO_UI),
	m_pFade		(nullptr),		// �t�F�[�h���
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
	m_state		= STATE_FADEOUT;	// ���
	m_pFade		= nullptr;			// �t�F�[�h���
	m_pTitle	= nullptr;			// �^�C�g�����
	m_pLine		= nullptr;			// �������
	m_pName		= nullptr;			// ���O���
	m_fCurTime	= 0.0f;				// ���݂̑ҋ@����

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

	//--------------------------------------------------------
	//	���O�̐��� / �����ݒ�
	//--------------------------------------------------------
	// ���O�̐���
	m_pName = CScrollString2D::Create
	( // ����
		name::FONT,				// �t�H���g�p�X
		name::ITALIC,			// �C�^���b�N
		L"�e�L�X�g�Ǎ��G���[",	// �\��������
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
	m_pName->SetPriority(PRIO_UI);

	// ���x����ݒ�
	m_pName->SetLabel(CObject::LABEL_NONE);	// �����j��/�X�V�����Ȃ����x��

	// �J������_��l����Ԃɂ���
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_GODITEM);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGodItemManager::Uninit(void)
{
	// �t�F�[�h�̏I��
	SAFE_UNINIT(m_pFade);

	// �^�C�g���̏I��
	SAFE_UNINIT(m_pTitle);

	// �����̏I��
	SAFE_UNINIT(m_pLine);

	// ���O�̏I��
	SAFE_UNINIT(m_pName);

	// �I�u�W�F�N�g��j��
	Release();

	// ���g�̕ۑ����Ă����C���X�^���X��������
	m_pThisClass = nullptr;
}

//============================================================
//	�X�V����
//============================================================
void CGodItemManager::Update(const float fDeltaTime)
{
	// �t�F�[�h�̍X�V
	m_pFade->Update(fDeltaTime);

	// �^�C�g���̍X�V
	m_pTitle->Update(fDeltaTime);

	// �����̍X�V
	m_pLine->Update(fDeltaTime);

	// ���O�̍X�V
	m_pName->Update(fDeltaTime);

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
//	�`�揈��
//============================================================
void CGodItemManager::Draw(CShader * /*pShader*/)
{

}

//============================================================
//	��������
//============================================================
CGodItemManager *CGodItemManager::Create(const CGodItem::EType typeID)
{
	if (m_pThisClass != nullptr)
	{ // ���N���X�̑��C���X�^���X������ꍇ

		// �C���X�^���X��j��
		SAFE_UNINIT(m_pThisClass);
	}

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

		// ���O�̕������ݒ�
		pGodItemManager->SetName(typeID);

		// �C���X�^���X��ۑ�
		assert(m_pThisClass == nullptr);
		m_pThisClass = pGodItemManager;

		// �m�ۂ����A�h���X��Ԃ�
		return pGodItemManager;
	}
}

//============================================================
//	�t�F�[�h�A�E�g�̍X�V����
//============================================================
void CGodItemManager::UpdateFadeOut(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, fade::MOVE_TIME);

	// �t�F�[�h�̓����x�𔽉f
	m_pFade->SetAlpha(fade::INIT_ALPHA + (fade::DIFF_ALPHA * fRate));

	if (m_fCurTime >= fade::MOVE_TIME)
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
	CInputKeyboard*	pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputPad*		pPad = GET_INPUTPAD;	// �p�b�h���
	if (pKey->IsTrigger(DIK_SPACE)
	||  pKey->IsTrigger(DIK_RETURN)
	||  pPad->IsTrigger(CInputPad::KEY_A)
	||  pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X)
	||  pPad->IsTrigger(CInputPad::KEY_Y))
	{
		// �^�C�}�[�̌v���ĊJ
		CSceneGame::GetTimerUI()->EnableStop(false);

		// �v���C���[�̏�Ԃ����ɖ߂�
		GET_PLAYER->SetEnableGodItem(false);

		// �Q�[���}�l�[�W���[��ʏ��Ԃɖ߂�
		CSceneGame::GetGameManager()->SetState(CGameManager::STATE_NORMAL);

		// �J��������荞�ݏ�Ԃɂ���
		GET_MANAGER->GetCamera()->SetState(CCamera::STATE_AROUND);
		GET_MANAGER->GetCamera()->SetDestAround();

		// �t�F�[�h�C����Ԃɂ���
		m_state = STATE_FADEIN;
	}
}

//============================================================
//	�t�F�[�h�C���̍X�V����
//============================================================
void CGodItemManager::UpdateFadeIn(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::OutQuad(m_fCurTime, fade::MOVE_TIME, 0.0f);

	// �t�F�[�h�̓����x�𔽉f
	m_pFade->SetAlpha(fade::INIT_ALPHA + (fade::DIFF_ALPHA * fRate));

	if (m_fCurTime >= fade::MOVE_TIME)
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
void CGodItemManager::UpdateFall(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, fall::MOVE_TIME);

	// �F�𔽉f
	m_pTitle->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// �^�C�g�����
	m_pLine->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// �������
	m_pName->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// ���O���

	// �ʒu�𔽉f
	m_pTitle->SetVec3Position(title::DEST_POS + (((title::DEST_POS + fall::OFFSET_POS) - title::DEST_POS) * fRate));	// �^�C�g�����
	m_pLine->SetVec3Position(line::POS + (((line::POS + fall::OFFSET_POS) - line::POS) * fRate));	// �������
	m_pName->SetVec3Position(name::POS + (((name::POS + fall::OFFSET_POS) - name::POS) * fRate));	// ���O���

	if (m_fCurTime >= fall::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �F��␳
		m_pTitle->SetColor(fall::DEST_COL);	// �^�C�g�����
		m_pLine->SetColor(fall::DEST_COL);	// �������
		m_pName->SetColor(fall::DEST_COL);	// ���O���

		// �I����Ԃɂ���
		m_state = STATE_END;
	}
}

//============================================================
//	�I���̍X�V����
//============================================================
void CGodItemManager::UpdateEnd(const float fDeltaTime)
{
	// ���g�̏I��
	Uninit();
}

//============================================================
//	���O�̕�����ݒ菈��
//============================================================
void CGodItemManager::SetName(const CGodItem::EType typeID)
{
	// �e�L�X�g������
	loadtext::BindString(m_pName, loadtext::LoadText(GET_STAGE->GetCurMapGodItemPass().c_str(), (int)typeID));
}

//============================================================
//	�X�L�b�v����̍X�V����
//============================================================
void CGodItemManager::UpdateSkip(void)
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
void CGodItemManager::SkipStaging(void)
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

	// ���O�̕��������ׂĕ\��
	m_pName->SetEnableDraw(true);

	// �_��l���J�����̖ڕW�ʒu�ɂ���
	GET_MANAGER->GetCamera()->SetDestGodItem();
}
