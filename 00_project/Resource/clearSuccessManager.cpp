//============================================================
//
//	�N���A�����}�l�[�W���[���� [clearSuccessManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "clearSuccessManager.h"
#include "retentionManager.h"
#include "manager.h"
#include "sceneGame.h"
#include "anim2D.h"
#include "string2D.h"
#include "scrollText2D.h"
#include "timeUI.h"
#include "blink2D.h"
#include "godItem.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// �N���A�����}�l�[�W���[�̗D�揇��

	namespace time
	{
		const char		*FONT	= "data\\FONT\\��S�V�b�N.otf";	// �t�H���g�p�X
		const wchar_t	*STRING	= L"�C�����s����";	// ������
		const bool	ITALIC		= false;	// �C�^���b�N
		const float	MOVE_TIME	= 0.2f;		// �ړ�����
		const float	WAIT_TIME	= 0.5f;		// �^�C�g���ҋ@����
		const float DEST_HEIGHT	= 100.0f;	// �����ڕW�c��
		const float INIT_HEIGHT	= DEST_HEIGHT * 14.0f;	// ���������c��
		const float DIFF_HEIGHT	= DEST_HEIGHT - INIT_HEIGHT;	// ���������c��
		const D3DXVECTOR3 POS	= D3DXVECTOR3(540.0f, 292.0f, 0.0f);	// �ʒu
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;		// ���z�u
	}

	namespace val_time
	{
		const float	MOVE_TIME = 0.68f;	// �ړ�����
		const float	WAIT_TIME = 0.15f;	// ���l�ҋ@����
		const D3DXCOLOR DEST_COL		= XCOL_WHITE;			// �ڕW�F
		const D3DXCOLOR INIT_COL		= XCOL_AWHITE;			// �����F
		const D3DXCOLOR DIFF_COL		= DEST_COL - INIT_COL;	// �����F
		const D3DXVECTOR3 DEST_POS		= D3DXVECTOR3(770.0f, 375.0f, 0.0f);			// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS		= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// �����ʒu
		const D3DXVECTOR3 DIFF_POS		= DEST_POS - INIT_POS;							// �����ʒu
		const D3DXVECTOR3 VAL_SIZE		= D3DXVECTOR3(65.5f, 87.0f, 0.0f);				// �����傫��
		const D3DXVECTOR3 PART_SIZE		= D3DXVECTOR3(38.0f, 87.0f, 0.0f);				// ��؂�傫��
		const D3DXVECTOR3 VAL_SPACE		= D3DXVECTOR3(VAL_SIZE.x * 0.85f, 0.0f, 0.0f);	// ������
		const D3DXVECTOR3 PART_SPACE	= D3DXVECTOR3(PART_SIZE.x * 0.85f, 0.0f, 0.0f);	// ��؂��
		const CValue::EType		TYPE	= CValue::TYPE_NORMAL;		// �������
		const CTimeUI::EAlignX	ALIGN_X	= CTimeUI::XALIGN_LEFT;		// ���z�u
		const CTimeUI::EAlignY	ALIGN_Y	= CTimeUI::YALIGN_CENTER;	// �c�z�u
	}

	namespace item
	{
		const char		*FONT	= "data\\FONT\\��S�V�b�N.otf";	// �t�H���g�p�X
		const wchar_t	*STRING	= L"�l����������";		// ������
		const bool		ITALIC	= false;				// �C�^���b�N
		const float	MOVE_TIME	= time::MOVE_TIME;		// �ړ�����
		const float	WAIT_TIME	= time::WAIT_TIME;		// �^�C�g���ҋ@����
		const float	DEST_HEIGHT	= time::DEST_HEIGHT;	// �����ڕW�c��
		const float	INIT_HEIGHT	= time::INIT_HEIGHT;	// ���������c��
		const float	DIFF_HEIGHT	= time::DIFF_HEIGHT;	// ���������c��
		const D3DXVECTOR3 POS	= D3DXVECTOR3(540.0f, 490.0f, 0.0f);	// �ʒu
		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;		// ���z�u
	}

	namespace icon_item
	{
		const char* TEXTURE = "data\\TEXTURE\\itemGod000.png";				// �_��e�N�X�`��
		const POSGRID2 TEX_PART	= POSGRID2(3, 1);							// �e�N�X�`������
		const D3DXVECTOR3 POS	= D3DXVECTOR3(865.0f, 585.0f, 0.0f);		// �ʒu
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(140.0f, 0.0f, 0.0f);			// ��
		const D3DXVECTOR3 DEST_SIZE	= D3DXVECTOR3(140.0f, 140.0f, 0.0f);	// �ڕW�傫��
		const D3DXVECTOR3 INIT_SIZE	= DEST_SIZE * 10.0f;					// �����傫��
		const D3DXVECTOR3 DIFF_SIZE	= DEST_SIZE - INIT_SIZE;				// �����傫��

		const float	MOVE_TIME	= 0.4f;		// �ړ�����
		const float	PLUS_TIME	= 0.45f;	// �o�߂̉�������
		const float	WAIT_TIME	= 0.5f;		// �A�C�R���ҋ@����
		const float	DEST_ALPHA	= 1.0f;		// �ڕW�����x
		const float	INIT_ALPHA	= 0.0f;		// ���������x
		const float	DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// ���������x
		const D3DXCOLOR DEST_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, DEST_ALPHA);	// �ڕW�F
		const D3DXCOLOR INIT_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);	// �����F
	}

	namespace icon_bg
	{
		const float	MOVE_TIME	= 0.68f;	// �ړ�����
		const float	PLUS_TIME	= 0.1f;		// �o�߂̉�������
		const float	WAIT_TIME	= 0.15f;	// �A�C�R���w�i�ҋ@����
		const float	DEST_ALPHA	= 1.0f;		// �ڕW�����x
		const float	INIT_ALPHA	= 0.0f;		// ���������x
		const float	DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// ���������x
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(0.35f, 0.35f, 0.35f, DEST_ALPHA);	// �ڕW�F
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(0.35f, 0.35f, 0.35f, INIT_ALPHA);	// �����F
		const D3DXVECTOR3 DEST_POS	= icon_item::POS;								// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// �����ʒu
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// �����ʒu
	}

	namespace control
	{
		const char* TEXTURE		= "data\\TEXTURE\\resultControl000.png";	// ����e�N�X�`��
		const D3DXVECTOR3 POS	= D3DXVECTOR3(620.0f, 640.0f, 0.0f);		// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(284.0f, 136.0f, 0.0f);		// �傫��
		const float MIN_ALPHA	= 0.4f;	// �Œᓧ���x
		const float MAX_ALPHA	= 1.0f;	// �ő哧���x
		const float LEVEL_ALPHA	= 3.4f;	// �����x�̉�����
		const float SUB_ALPHA	= 1.2f;	// �C���̃��l������
		const float ADD_ALPHA	= 1.2f;	// �A�E�g�̃��l������
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CClearSuccessManager::AFuncUpdateState CClearSuccessManager::m_aFuncUpdateState[] =	// ��ԍX�V�֐�
{
	nullptr,										// �������Ȃ��X�V
	&CClearSuccessManager::UpdateTimeTitleWait,		// ���s���ԃ^�C�g���ҋ@�X�V
	&CClearSuccessManager::UpdateTimeTitle,			// ���s���ԃ^�C�g���\���X�V
	&CClearSuccessManager::UpdateTimeValueWait,		// ���s���ԑҋ@�X�V
	&CClearSuccessManager::UpdateTimeValue,			// ���s���ԕ\���X�V
	&CClearSuccessManager::UpdateItemTitleWait,		// �_��^�C�g���ҋ@�X�V
	&CClearSuccessManager::UpdateItemTitle,			// �_��^�C�g���\���X�V
	&CClearSuccessManager::UpdateItemIconBgWait,	// �_��A�C�R���w�i�ҋ@�X�V
	&CClearSuccessManager::UpdateItemIconBg,		// �_��A�C�R���w�i�\���X�V
	&CClearSuccessManager::UpdateItemIconWait,		// �_��A�C�R���ҋ@�X�V
	&CClearSuccessManager::UpdateItemIcon,			// �_��A�C�R���\���X�V
	&CClearSuccessManager::UpdateWait,				// �ҋ@�X�V
	nullptr,										// �I���X�V
};

//************************************************************
//	�e�N���X [CClearSuccessManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CClearSuccessManager::CClearSuccessManager() :
	m_pTime		(nullptr),		// ���s���ԃ^�C�g�����
	m_pTimeVal	(nullptr),		// ���s���ԏ��
	m_pGodItem	(nullptr),		// �_��^�C�g�����
	m_pControl	(nullptr),		// ������
	m_state		(STATE_NONE),	// ���
	m_fCurTime	(0.0f)			// ���݂̑ҋ@����
{
	// �����o�ϐ����N���A
	memset(&m_apGodItemBG[0],   0, sizeof(m_apGodItemBG));		// �_��A�C�R���w�i���
	memset(&m_apGodItemIcon[0], 0, sizeof(m_apGodItemIcon));	// �_��A�C�R�����

	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_aFuncUpdateState) == CClearSuccessManager::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	�f�X�g���N�^
//============================================================
CClearSuccessManager::~CClearSuccessManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CClearSuccessManager::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apGodItemBG[0], 0, sizeof(m_apGodItemBG));		// �_��A�C�R���w�i���
	memset(&m_apGodItemIcon[0], 0, sizeof(m_apGodItemIcon));	// �_��A�C�R�����
	m_state = STATE_TIME_TITLE_WAIT;	// ���
	m_pTime		= nullptr;	// ���s���ԃ^�C�g�����
	m_pTimeVal	= nullptr;	// ���s���ԏ��
	m_pGodItem	= nullptr;	// �_��^�C�g�����
	m_pControl	= nullptr;	// ������
	m_fCurTime	= 0.0f;		// ���݂̑ҋ@����

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
		CSceneGame::TIME_LIMIT - GET_RETENTION->GetTime(),	// �\������
		val_time::INIT_POS,		// �ʒu
		val_time::VAL_SIZE,		// �����̑傫��
		val_time::PART_SIZE,	// ��؂�̑傫��
		val_time::VAL_SPACE,	// �����̋�
		val_time::PART_SPACE,	// ��؂�̋�
		val_time::TYPE,			// �������
		val_time::ALIGN_X,		// ���z�u
		val_time::ALIGN_Y,		// �c�z�u
		VEC3_ZERO,				// ����
		val_time::INIT_COL		// �F
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
		item::FONT,			// �t�H���g�p�X
		item::ITALIC,		// �C�^���b�N
		item::STRING,		// �w�蕶����
		item::POS,			// ���_�ʒu
		item::INIT_HEIGHT,	// �����c��
		item::ALIGN_X		// ���z�u
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
	//	�_��A�C�R���w�i�̐��� / �����ݒ�
	//--------------------------------------------------------
	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // �_��̑������J��Ԃ�

		// �A�C�R���w�i�����ʒu���v�Z
		D3DXVECTOR3 posBG = icon_bg::INIT_POS + (icon_item::SPACE * (float)i);	// �A�C�R���w�i�����ʒu

		// �_��A�C�R���w�i�̐���
		m_apGodItemBG[i] = CAnim2D::Create(icon_item::TEX_PART.x, icon_item::TEX_PART.y, posBG, icon_item::DEST_SIZE, VEC3_ZERO, icon_bg::INIT_COL);
		if (m_apGodItemBG[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`��������
		m_apGodItemBG[i]->BindTexture(icon_item::TEXTURE);

		// �e�N�X�`���p�^�[����ݒ�
		m_apGodItemBG[i]->SetPattern(i);

		// �D�揇�ʂ�ݒ�
		m_apGodItemBG[i]->SetPriority(PRIORITY);

		// ���x����ݒ�
		m_apGodItemBG[i]->SetLabel(CObject::LABEL_UI);	// �����j��/�X�V�����郉�x��

		// �����`���OFF�ɂ���
		m_apGodItemBG[i]->SetEnableDraw(false);
	}

	//--------------------------------------------------------
	//	�_��A�C�R���̐��� / �����ݒ�
	//--------------------------------------------------------
	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // �_��̑������J��Ԃ�

		// �A�C�R�������ʒu���v�Z
		D3DXVECTOR3 posIcon = icon_item::POS + (icon_item::SPACE * (float)i);	// �A�C�R�������ʒu

		// �_��A�C�R���̐���
		m_apGodItemIcon[i] = CAnim2D::Create(icon_item::TEX_PART.x, icon_item::TEX_PART.y, posIcon, icon_item::INIT_SIZE, VEC3_ZERO, icon_item::INIT_COL);
		if (m_apGodItemIcon[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`��������
		m_apGodItemIcon[i]->BindTexture(icon_item::TEXTURE);

		// �e�N�X�`���p�^�[����ݒ�
		m_apGodItemIcon[i]->SetPattern(i);

		// �D�揇�ʂ�ݒ�
		m_apGodItemIcon[i]->SetPriority(PRIORITY);

		// ���x����ݒ�
		m_apGodItemIcon[i]->SetLabel(CObject::LABEL_UI);	// �����j��/�X�V�����郉�x��

		// �����`���OFF�ɂ���
		m_apGodItemIcon[i]->SetEnableDraw(false);
	}

	//--------------------------------------------------------
	//	����̐��� / �����ݒ�
	//--------------------------------------------------------
	// ����̐���
	m_pControl = CBlink2D::Create
	( // ����
		control::POS,			// �ʒu
		control::SIZE,			// �傫��
		control::MIN_ALPHA,		// �Œᓧ���x
		control::MAX_ALPHA,		// �ő哧���x
		control::LEVEL_ALPHA,	// �����x�̉�����
		control::SUB_ALPHA,		// �C���̃��l������
		control::ADD_ALPHA		// �A�E�g�̃��l������
	);
	if (m_pControl == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`��������
	m_pControl->BindTexture(control::TEXTURE);

	// �D�揇�ʂ�ݒ�
	m_pControl->SetPriority(PRIORITY);

	// �����`���OFF�ɂ���
	m_pControl->SetEnableDraw(false);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CClearSuccessManager::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CClearSuccessManager::Update(const float fDeltaTime)
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
void CClearSuccessManager::SkipStaging(void)
{
	// �ҋ@��Ԃɂ���
	m_state = STATE_WAIT;

	// ���s���ԃ^�C�g�������o��̌����ڂɂ���
	m_pTime->SetEnableDraw(true);				// �����`���ON�ɂ���
	m_pTime->SetCharHeight(time::DEST_HEIGHT);	// �ڕW�T�C�Y�ɐݒ�

	// ���s���Ԃ����o��̌����ڂɂ���
	m_pTimeVal->SetEnableDraw(true);					// �����`���ON�ɂ���
	m_pTimeVal->SetVec3Position(val_time::DEST_POS);	// �ڕW�ʒu�ɐݒ�
	m_pTimeVal->SetColor(val_time::DEST_COL);			// �ڕW�F�ɐݒ�

	// �_��^�C�g�������o��̌����ڂɂ���
	m_pGodItem->SetEnableDraw(true);				// �����`���ON�ɂ���
	m_pGodItem->SetCharHeight(item::DEST_HEIGHT);	// �ڕW�T�C�Y�ɐݒ�

	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // �_��̑������J��Ԃ�

		// �_��A�C�R���w�i�̖ڕW�����ʒu���v�Z
		D3DXVECTOR3 posDest = icon_bg::DEST_POS + (icon_item::SPACE * (float)i);

		// �_��A�C�R���w�i�����o��̌����ڂɂ���
		m_apGodItemBG[i]->SetEnableDraw(true);			// �����`���ON�ɂ���
		m_apGodItemBG[i]->SetVec3Position(posDest);		// �ڕW�ʒu�ɐݒ�
		m_apGodItemBG[i]->SetColor(icon_bg::DEST_COL);	// �ڕW�F�ɐݒ�

		// �_��A�C�R�������o��̌����ڂɂ���
		m_apGodItemIcon[i]->SetEnableDraw(CGodItem::IsGet((CGodItem::EType)i));	// �����`����l�����Ă���ꍇON�ɂ���
		m_apGodItemIcon[i]->SetVec3Sizing(icon_item::DEST_SIZE);	// �ڕW�T�C�Y�ɐݒ�
		m_apGodItemIcon[i]->SetColor(icon_item::DEST_COL);			// �ڕW�F�ɐݒ�
	}

	// ��������o��̌����ڂɂ���
	m_pControl->SetEnableDraw(true);	// �����`���ON�ɂ���
	m_pControl->SetBlink(true);			// �_�ł��J�n����
}

//============================================================
//	�SUI�I�u�W�F�N�g�̈ړ�����
//============================================================
void CClearSuccessManager::SetAllMove(const D3DXVECTOR3& rMove)
{
	// ���s���ԃ^�C�g���̈ʒu���ړ�
	m_pTime->SetVec3Position(m_pTime->GetVec3Position() + rMove);

	// ���s���Ԃ̈ʒu���ړ�
	m_pTimeVal->SetVec3Position(m_pTimeVal->GetVec3Position() + rMove);

	// �_��^�C�g���̈ʒu���ړ�
	m_pGodItem->SetVec3Position(m_pGodItem->GetVec3Position() + rMove);

	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // �_��̑������J��Ԃ�

		// �_��A�C�R���w�i�̈ʒu���ړ�
		m_apGodItemBG[i]->SetVec3Position(m_apGodItemBG[i]->GetVec3Position() + rMove);

		// �_��A�C�R���̈ʒu���ړ�
		m_apGodItemIcon[i]->SetVec3Position(m_apGodItemIcon[i]->GetVec3Position() + rMove);
	}

	// ����̈ʒu���ړ�
	m_pControl->SetVec3Position(m_pControl->GetVec3Position() + rMove);
}

//============================================================
//	���s���ԃ^�C�g���ҋ@�̍X�V����
//============================================================
void CClearSuccessManager::UpdateTimeTitleWait(const float fDeltaTime)
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
void CClearSuccessManager::UpdateTimeTitle(const float fDeltaTime)
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

		// ���s���ԑҋ@��Ԃɂ���
		m_state = STATE_TIME_VALUE_WAIT;
	}
}

//============================================================
//	���s���ԃ^�C�g���ҋ@�̍X�V����
//============================================================
void CClearSuccessManager::UpdateTimeValueWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= val_time::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// ���s���Ԃ̎����`���ON�ɂ���
		m_pTimeVal->SetEnableDraw(true);

		// ���s���ԕ\����Ԃɂ���
		m_state = STATE_TIME_VALUE;
	}
}

//============================================================
//	���s���ԕ\���̍X�V����
//============================================================
void CClearSuccessManager::UpdateTimeValue(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InOutQuad(m_fCurTime, 0.0f, val_time::MOVE_TIME);

	// ���s���Ԃ̐F�𔽉f
	m_pTimeVal->SetColor(val_time::INIT_COL + (val_time::DIFF_COL * fRate));

	// ���s���Ԃ̈ʒu�𔽉f
	m_pTimeVal->SetVec3Position(val_time::INIT_POS + (val_time::DIFF_POS * fRate));

	if (m_fCurTime >= val_time::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// ���s���Ԃ̐F��␳
		m_pTimeVal->SetColor(val_time::DEST_COL);

		// ���s���Ԃ̈ʒu��␳
		m_pTimeVal->SetVec3Position(val_time::DEST_POS);

		// �_��^�C�g���ҋ@��Ԃɂ���
		m_state = STATE_ITEM_TITLE_WAIT;
	}
}

//============================================================
//	�_��^�C�g���ҋ@�̍X�V����
//============================================================
void CClearSuccessManager::UpdateItemTitleWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= item::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �_��^�C�g���̎����`���ON�ɂ���
		m_pGodItem->SetEnableDraw(true);

		// �_��^�C�g���\����Ԃɂ���
		m_state = STATE_ITEM_TITLE;
	}
}

//============================================================
//	�_��^�C�g���\���̍X�V����
//============================================================
void CClearSuccessManager::UpdateItemTitle(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InQuad(m_fCurTime, 0.0f, item::MOVE_TIME);

	// �_��^�C�g���̑傫���𔽉f
	m_pGodItem->SetCharHeight(item::INIT_HEIGHT + (item::DIFF_HEIGHT * fRate));

	if (m_fCurTime >= item::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �_��^�C�g���̑傫����␳
		m_pGodItem->SetCharHeight(item::DEST_HEIGHT);

		// �_��A�C�R���w�i�ҋ@��Ԃɂ���
		m_state = STATE_ITEM_BG_WAIT;
	}
}

//============================================================
//	�_��A�C�R���w�i�ҋ@�̍X�V����
//============================================================
void CClearSuccessManager::UpdateItemIconBgWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= icon_bg::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		for (int i = 0; i < CStage::GOD_MAX; i++)
		{ // �_��̑������J��Ԃ�

			// �_��A�C�R���w�i�̎����`���ON�ɂ���
			m_apGodItemBG[i]->SetEnableDraw(true);
		}

		// �_��A�C�R���w�i�\����Ԃɂ���
		m_state = STATE_ITEM_BG;
	}
}

//============================================================
//	�_��A�C�R���w�i�\���̍X�V����
//============================================================
void CClearSuccessManager::UpdateItemIconBg(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �A�C�R���w�i�̈ړ�
	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // �_��̑������J��Ԃ�

		// �A�C�R���w�i���ꂼ��̌o�ߎ��Ԃ��v�Z
		float fRateTime = m_fCurTime - (icon_bg::PLUS_TIME * (float)i);
		useful::LimitNum(fRateTime, 0.0f, icon_bg::MOVE_TIME);	// �o�ߎ��Ԃ�␳

		// ���ꂼ��̌o�ߎ������犄�����v�Z
		float fRate = easeing::InOutQuad(fRateTime, 0.0f, icon_bg::MOVE_TIME);

		// �A�C�R���w�i�̈ʒu���v�Z
		D3DXVECTOR3 posInit = icon_bg::INIT_POS + (icon_item::SPACE * (float)i);

		// �A�C�R���w�i�̐F���v�Z
		D3DXCOLOR colCur = icon_bg::INIT_COL;
		colCur.a = icon_bg::INIT_ALPHA + (icon_bg::DIFF_ALPHA * fRate);	// ���݂̓����x��ݒ�

		// �_��A�C�R���w�i�̈ʒu�𔽉f
		m_apGodItemBG[i]->SetVec3Position(posInit + (icon_bg::DIFF_POS * fRate));

		// �_��A�C�R���w�i�̐F�𔽉f
		m_apGodItemBG[i]->SetColor(colCur);
	}

	// �A�C�R���w�i�̈ړ��␳
	if (m_fCurTime >= icon_bg::MOVE_TIME + icon_bg::PLUS_TIME * (CStage::GOD_MAX - 1))
	{ // �S�A�C�R���w�i�̑ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		for (int i = 0; i < CStage::GOD_MAX; i++)
		{ // �_��̑������J��Ԃ�

			// �A�C�R���w�i�̖ڕW�����ʒu���v�Z
			D3DXVECTOR3 posDest = icon_bg::DEST_POS + (icon_item::SPACE * (float)i);

			// �_��A�C�R���w�i�̈ʒu��␳
			m_apGodItemBG[i]->SetVec3Position(posDest);

			// �_��A�C�R���w�i�̐F��␳
			m_apGodItemBG[i]->SetColor(icon_bg::DEST_COL);
		}

		// �_��A�C�R���ҋ@��Ԃɂ���
		m_state = STATE_ITEM_ICON_WAIT;
	}
}

//============================================================
//	�_��A�C�R���ҋ@�̍X�V����
//============================================================
void CClearSuccessManager::UpdateItemIconWait(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= icon_item::WAIT_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		for (int i = 0; i < CStage::GOD_MAX; i++)
		{ // �_��̑������J��Ԃ�

			// �l�������_��A�C�R���̎����`���ON�ɂ���
			m_apGodItemIcon[i]->SetEnableDraw(CGodItem::IsGet((CGodItem::EType)i));
		}

		// �_��A�C�R���\����Ԃɂ���
		m_state = STATE_ITEM_ICON;
	}
}

//============================================================
//	�_��A�C�R���\���̍X�V����
//============================================================
void CClearSuccessManager::UpdateItemIcon(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �A�C�R���̃T�C�Y�ύX
	for (int i = 0; i < CStage::GOD_MAX; i++)
	{ // �_��̑������J��Ԃ�

		// �A�C�R���w�i���ꂼ��̌o�ߎ��Ԃ��v�Z
		float fRateTime = m_fCurTime - (icon_item::PLUS_TIME * (float)i);
		useful::LimitNum(fRateTime, 0.0f, icon_item::MOVE_TIME);	// �o�ߎ��Ԃ�␳

		// ���ꂼ��̌o�ߎ������犄�����v�Z
		float fRate = easeing::InQuad(fRateTime, 0.0f, icon_item::MOVE_TIME);

		// �A�C�R���w�i�̐F���v�Z
		D3DXCOLOR colCur = icon_item::INIT_COL;
		colCur.a = icon_item::INIT_ALPHA + (icon_item::DIFF_ALPHA * fRate);	// ���݂̓����x��ݒ�

		// �_��A�C�R���̑傫���𔽉f
		m_apGodItemIcon[i]->SetVec3Sizing(icon_item::INIT_SIZE + (icon_item::DIFF_SIZE * fRate));

		// �_��A�C�R���̐F�𔽉f
		m_apGodItemIcon[i]->SetColor(colCur);
	}

	// �A�C�R���̃T�C�Y�ύX�␳
	if (m_fCurTime >= icon_item::MOVE_TIME + icon_item::PLUS_TIME * (CStage::GOD_MAX - 1))
	{ // �S�A�C�R���̑ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		for (int i = 0; i < CStage::GOD_MAX; i++)
		{ // �_��̑������J��Ԃ�

			// �_��A�C�R���̑傫����␳
			m_apGodItemIcon[i]->SetVec3Sizing(icon_item::DEST_SIZE);

			// �_��A�C�R���̐F��␳
			m_apGodItemIcon[i]->SetColor(icon_item::DEST_COL);
		}

		// ����̎����`���ON�ɂ���
		m_pControl->SetEnableDraw(true);

		// ����̓_�ł��J�n����
		m_pControl->SetBlink(true);

		// �ҋ@��Ԃɂ���
		m_state = STATE_WAIT;
	}
}

//============================================================
//	�ҋ@�̍X�V����
//============================================================
void CClearSuccessManager::UpdateWait(const float fDeltaTime)
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
		// �I����Ԃɂ���
		m_state = STATE_END;
	}
}
