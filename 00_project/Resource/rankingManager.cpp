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
#include "renderer.h"
#include "loadtext.h"
#include "stage.h"
#include "object2D.h"
#include "player.h"
#include "string2D.h"
#include "timeUI.h"
#include "anim2D.h"
#include "timer.h"
#include "transpoint.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIO_BG = 5;	// �w�i�̗D�揇��
	const int PRIO_UI = 6;	// UI�̗D�揇��
	const float RANK_TIME[CRankingManager::MAX_RANK] = { 150.0f, 180.0f, 225.0f };	// ���������L���O�^�C��

	namespace fade
	{
		const float	MOVE_INTIME	 = 0.15f;	// �C���ϓ�����
		const float	MOVE_OUTTIME = 0.15f;	// �A�E�g�ϓ�����
		const float DEST_ALPHA	 = 0.75f;	// �ڕW�����x
		const float INIT_ALPHA	 = 0.0f;	// ���������x
		const float DIFF_ALPHA	 = DEST_ALPHA - INIT_ALPHA;	// ���������x
		const D3DXCOLOR INIT_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, INIT_ALPHA);		// �����F
		const D3DXVECTOR3 SIZE	 = D3DXVECTOR3(SCREEN_SIZE.x, 520.0f, 0.0f);	// �傫��
		const D3DXVECTOR3 POS	 = SCREEN_CENT;	// �ʒu
		const D3DXVECTOR3 ROT	 = VEC3_ZERO;	// ����
	}

	namespace title
	{
		const char *TEXTURE		= "data\\TEXTURE\\get_magatama.png";	// �e�N�X�`���p�X
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(632.0f, 184.0f, 0.0f) * 0.8f;	// �傫��
		const float	MOVE_TIME	= 0.68f;	// �ړ�����
		const D3DXCOLOR DEST_COL	= XCOL_WHITE;			// �ڕW�F
		const D3DXCOLOR INIT_COL	= XCOL_AWHITE;			// �����F
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;	// �����F
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(SCREEN_CENT.x, 180.0f, 0.0f);		// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// �����ʒu
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// �����ʒu
	}

	namespace balloon
	{
		const char *TEXTURE		= "data\\TEXTURE\\get_magatama.png";	// �e�N�X�`���p�X
		const D3DXVECTOR3 SIZE	= SCREEN_SIZE * 0.42f;		// �傫��
		const float	MOVE_TIME	= 0.68f;					// �ړ�����
		const D3DXCOLOR DEST_COL	= XCOL_WHITE;			// �ڕW�F
		const D3DXCOLOR INIT_COL	= XCOL_AWHITE;			// �����F
		const D3DXCOLOR DIFF_COL	= DEST_COL - INIT_COL;	// �����F
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(345.0f, 385.0f, 0.0f);			// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// �����ʒu
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// �����ʒu
	}

	namespace name
	{
		const char		*FONT	= "data\\FONT\\�ʂ˂�������������v7��.ttf";	// �t�H���g�p�X
		const wchar_t	*STRING = L"���s����";	// �\��������
		const bool		ITALIC	= true;			// �C�^���b�N

		const float	CHAR_HEIGHT	 = 120.0f * 0.42f;	// �����c��
		const D3DXVECTOR3 POS	 = D3DXVECTOR3(635.0f, 275.0f, 0.0f);		// �ʒu
		const D3DXVECTOR3 OFFSET = D3DXVECTOR3(8.0f, 8.0f, 0.0f) * 0.42f;	// �e�I�t�Z�b�g
		const D3DXVECTOR3 ROT	 = VEC3_ZERO;								// ����

		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// ���z�u
		const D3DXCOLOR COL_SHADOW	= XCOL_ABLUE;	// �e�̐F
		const D3DXCOLOR COL_NAME	= XCOL_AWHITE;	// ���O�̐F
	}

	namespace rank
	{
		const char *TEXTURE		= "data\\TEXTURE\\ranking000.png";		// �t�H���g�p�X
		const POSGRID2 PART		= POSGRID2(1, 3);						// �e�N�X�`��������
		const D3DXVECTOR3 POS	= D3DXVECTOR3(725.0f, 338.5f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(0.0f, 66.5f, 0.0f);		// ��
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(125.4f, 74.25f, 0.0f);	// �����傫��
	}

	namespace time
	{
		const D3DXVECTOR3 POS			= D3DXVECTOR3(979.0f, rank::POS.y, 0.0f);		// �ʒu
		const D3DXVECTOR3 SPACE			= rank::SPACE;									// ��
		const D3DXVECTOR3 VAL_SIZE		= D3DXVECTOR3(62.82f, 74.25f, 0.0f) * 0.87f;	// �����傫��
		const D3DXVECTOR3 PART_SIZE		= D3DXVECTOR3(27.85f, 63.65f, 0.0f) * 0.87f;	// ��؂�傫��
		const D3DXVECTOR3 VAL_SPACE		= D3DXVECTOR3(VAL_SIZE.x * 0.85f, 0.0f, 0.0f);	// ������
		const D3DXVECTOR3 PART_SPACE	= D3DXVECTOR3(PART_SIZE.x * 0.85f, 0.0f, 0.0f);	// ��؂��
		const CValue::EType TYPE		= CValue::TYPE_NORMAL;		// �������
		const CTimeUI::EAlignX ALIGN_X	= CTimeUI::XALIGN_CENTER;	// ���z�u
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

		const D3DXVECTOR3 DEST_POS = D3DXVECTOR3(SCREEN_CENT.x - SPACE.x * 0.5f, 555.0f, 0.0f);	// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS = DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);					// �����ʒu
		const D3DXVECTOR3 DIFF_POS = DEST_POS - INIT_POS;										// �����ʒu
	}

	namespace fall
	{
		const float	MOVE_TIME	= 0.25f;	// �ړ�����
		const float DEST_ALPHA	= 0.0f;		// �ڕW�����x
		const float INIT_ALPHA	= 1.0f;		// ���������x
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
	&CRankingManager::UpdateSpawn,		// UI�o���X�V
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
CRankingManager::CRankingManager(CTransPoint* pParent) :
	m_pFade		 (nullptr),		// �t�F�[�h���
	m_pTitle	 (nullptr),		// �^�C�g�����
	m_pBalloon	 (nullptr),		// �����o�����
	m_pShadow	 (nullptr),		// �����L���O�^�C�g���̉e���
	m_pName		 (nullptr),		// �����L���O�^�C�g�����
	m_pParent	 (pParent),		// �J�ڃ|�C���g���
	m_state		 (STATE_NONE),	// ���
	m_fCurTime	 (0.0f),		// ���݂̑ҋ@����
	m_nCurSelect (0),			// ���݂̑I����
	m_nOldSelect (0)			// �O��̑I����
{
	// �����o�ϐ����N���A
	memset(&m_apRankValue[0], 0, sizeof(m_apRankValue));	// �����L���O���ʏ��
	memset(&m_apRankTime[0], 0, sizeof(m_apRankTime));		// �����L���O���ԏ��
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));			// �I�������

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
	memset(&m_apRankValue[0], 0, sizeof(m_apRankValue));	// �����L���O���ʏ��
	memset(&m_apRankTime[0], 0, sizeof(m_apRankTime));		// �����L���O���ԏ��
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));			// �I�������
	m_state		 = STATE_FADEOUT;	// ���
	m_pFade		 = nullptr;	// �t�F�[�h���
	m_pTitle	 = nullptr;	// �^�C�g�����
	m_pBalloon	 = nullptr;	// �����o�����
	m_pShadow	 = nullptr;	// �����L���O�^�C�g���̉e���
	m_pName		 = nullptr;	// �����L���O�^�C�g�����
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
	//	�����o���̐��� / �����ݒ�
	//--------------------------------------------------------
	// �����o���̐���
	m_pBalloon = CObject2D::Create
	( // ����
		balloon::INIT_POS,	// �ʒu
		balloon::SIZE,		// �傫��
		VEC3_ZERO,			// ����
		balloon::INIT_COL	// �F
	);
	if (m_pBalloon == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �r���{�[�h�V�[���̃����_�[�e�N�X�`��������
	m_pBalloon->BindTexture(GET_RENDERER->GetRenderTextureIndex(CObject::SCENE_BILLBOARD));

	// �D�揇�ʂ�ݒ�
	m_pBalloon->SetPriority(PRIO_UI);

	// ���x����ݒ�
	m_pBalloon->SetLabel(CObject::LABEL_UI);

	//--------------------------------------------------------
	//	�����L���O�^�C�g���e�̐��� / �����ݒ�
	//--------------------------------------------------------
	// �����L���O�^�C�g���e�̐���
	m_pShadow = CString2D::Create
	( // ����
		name::FONT,					// �t�H���g�p�X
		name::ITALIC,				// �C�^���b�N
		name::STRING,				// �\��������
		name::POS + name::OFFSET,	// ���_�ʒu
		name::CHAR_HEIGHT,			// �����c��
		name::ALIGN_X,				// ���z�u
		name::ROT,					// ���_����
		name::COL_SHADOW			// �F
	);
	if (m_pShadow == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pShadow->SetPriority(PRIO_UI);

	//--------------------------------------------------------
	//	�����L���O�^�C�g���̐��� / �����ݒ�
	//--------------------------------------------------------
	// �����L���O�^�C�g���̐���
	m_pName = CString2D::Create
	( // ����
		name::FONT,			// �t�H���g�p�X
		name::ITALIC,		// �C�^���b�N
		name::STRING,		// �\��������
		name::POS,			// ���_�ʒu
		name::CHAR_HEIGHT,	// �����c��
		name::ALIGN_X,		// ���z�u
		name::ROT,			// ���_����
		name::COL_NAME		// �F
	);
	if (m_pName == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �D�揇�ʂ�ݒ�
	m_pName->SetPriority(PRIO_UI);

	//--------------------------------------------------------
	//	�����L���O�̐��� / �����ݒ�
	//--------------------------------------------------------
	float aRank[MAX_RANK] = {};	// �����L���O�z��

	// �����L���O�p�X���쐬
	std::filesystem::path path = m_pParent->GetTransMapPass();	// �}�b�v�f�B���N�g��
	path = path.parent_path();	// �}�b�v�f�B���N�g���ɂ���
	path.append("ranking.bin");	// �����L���O�p�X��ǉ�

	// �����L���O�Ǎ�
	if (FAILED(LoadRank(path.string().c_str(), &aRank[0])))
	{ // �Ǎ��Ɏ��s�����ꍇ

		// �����L���O�̍ēǍ�
		if (FAILED(LoadRank(path.string().c_str(), &aRank[0])))
		{ // �ēǍ��Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	for (int i = 0; i < MAX_RANK; i++)
	{ // �����L���O�\�������J��Ԃ�

		//----------------------------------------------------
		//	�����L���O���ʂ̐��� / �����ݒ�
		//----------------------------------------------------
		// �����ʒu���v�Z
		D3DXVECTOR3 posRank = rank::POS + (rank::SPACE * (float)i);	// �����ʒu

		// �����L���O���Ԃ̐���
		m_apRankValue[i] = CAnim2D::Create
		( // ����
			rank::PART.x,	// �e�N�X�`����������
			rank::PART.y,	// �e�N�X�`���c������
			posRank,		// �ʒu
			rank::SIZE,		// �傫��
			VEC3_ZERO,		// ����
			XCOL_AWHITE		// �F
		);
		if (m_apRankValue[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`��������
		m_apRankValue[i]->BindTexture(rank::TEXTURE);

		// �p�^�[����ݒ�
		m_apRankValue[i]->SetPattern(i);

		// �D�揇�ʂ�ݒ�
		m_apRankValue[i]->SetPriority(PRIO_UI);

		// ���x����ݒ�
		m_apRankValue[i]->SetLabel(CObject::LABEL_UI);

		//----------------------------------------------------
		//	�����L���O���Ԃ̐��� / �����ݒ�
		//----------------------------------------------------
		// �����ʒu���v�Z
		D3DXVECTOR3 posTime = time::POS + (time::SPACE * (float)i);	// �����ʒu

		// �����L���O���Ԃ̐���
		m_apRankTime[i] = CTimeUI::Create
		( // ����
			aRank[i],				// �\������
			posTime,				// �ʒu
			time::VAL_SIZE,			// �����̑傫��
			time::PART_SIZE,		// ��؂�̑傫��
			time::VAL_SPACE,		// �����̋�
			time::PART_SPACE,		// ��؂�̋�
			time::TYPE,				// �������
			time::ALIGN_X,			// ���z�u
			CTimeUI::YALIGN_CENTER,	// �c�z�u
			VEC3_ZERO,				// ����
			XCOL_AWHITE				// �F
		);
		if (m_apRankTime[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �D�揇�ʂ�ݒ�
		m_apRankTime[i]->SetPriority(PRIO_UI);
	}

	//--------------------------------------------------------
	//	�I�����̐��� / �����ݒ�
	//--------------------------------------------------------
	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		// �����ʒu���v�Z
		D3DXVECTOR3 posSelect = select::INIT_POS + (select::SPACE * (float)i);	// �����ʒu

		// �I�����̐���
		m_apSelect[i] = CString2D::Create
		( // ����
			select::FONT,			// �t�H���g�p�X
			select::ITALIC,			// �C�^���b�N
			select::STRING[i],		// �w�蕶����
			posSelect,				// ���_�ʒu
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
CRankingManager *CRankingManager::Create(CTransPoint* pParent)
{
	// �����L���O�}�l�[�W���[�̐���
	CRankingManager *pRankingManager = new CRankingManager(pParent);
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
//	�����L���O�̐ݒ菈��
//============================================================
int CRankingManager::SetRank(const float fNewTime)
{
	int nLowIdx = MAX_RANK - 1;	// �ŉ��ʃC���f�b�N�X
	int nUpdateIdx = NONE_IDX;	// �X�V���ʃC���f�b�N�X
	float aRank[MAX_RANK];		// �����L���O����

	// �����L���O�̓Ǎ�
	if (FAILED(LoadRank(GET_STAGE->GetCurMapSaveRankPass().c_str(), &aRank[0])))
	{ // �Ǎ��Ɏ��s�����ꍇ

		// �����L���O�̍ēǍ�
		if (FAILED(LoadRank(GET_STAGE->GetCurMapSaveRankPass().c_str(), &aRank[0])))
		{ // �ēǍ��Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return NONE_IDX;
		}
	}

	if (fNewTime < aRank[nLowIdx])
	{ // �ŉ��ʂ̎��Ԃ�葬���ꍇ

		// �����L���O�̃\�[�g
		nUpdateIdx = SortRank(fNewTime, &aRank[0]);

		// �����L���O�̕ۑ�
		SaveRank(GET_STAGE->GetCurMapSaveRankPass().c_str(), &aRank[0]);
	}

	// ���ʂ��X�V���ꂽ�C���f�b�N�X��Ԃ�
	return nUpdateIdx;
}

//============================================================
//	�����L���O�̃\�[�g����
//============================================================
int CRankingManager::SortRank(const float fNewTime, float* pRankArray)
{
	int nLowIdx = MAX_RANK - 1;	// �ŉ��ʃC���f�b�N�X
	int nUpdateIdx = NONE_IDX;	// �X�V���ʃC���f�b�N�X
	int	nCurMinIdx;				// �ŏ��l�̃C���f�b�N�X

	// ���݂̍ŉ��ʂ̏��Ə�������
	pRankArray[nLowIdx] = fNewTime;

	for (int nCntKeep = 0; nCntKeep < nLowIdx; nCntKeep++)
	{ // ����ւ���l�̑��� -1�񕪌J��Ԃ�

		// ���݂̌J��Ԃ������� (�v�f1�Ƃ���)
		nCurMinIdx = nCntKeep;

		for (int nCntSort = nCntKeep + 1; nCntSort < MAX_RANK; nCntSort++)
		{ // ����ւ���l�̑��� -nCntKeep���J��Ԃ�

			if (pRankArray[nCurMinIdx] > pRankArray[nCntSort])
			{ // �ŏ��l�ɐݒ肳��Ă���l���A���݂̒l�̂ق����������ꍇ

				// ���݂̗v�f�ԍ����ŏ��l�ɐݒ�
				nCurMinIdx = nCntSort;
			}
		}

		if (nCntKeep != nCurMinIdx)
		{ // �ŏ��l�̗v�f�ԍ��ɕϓ����������ꍇ

			// �v�f�̓���ւ�
			float fKeepNum			= pRankArray[nCntKeep];
			pRankArray[nCntKeep]	= pRankArray[nCurMinIdx];
			pRankArray[nCurMinIdx]	= fKeepNum;

			// �X�V���ꂽ�C���f�b�N�X��ۑ�
			nUpdateIdx = nCntKeep;
		}
	}

	// ���ʂ��X�V���ꂽ�C���f�b�N�X��Ԃ�
	return nUpdateIdx;
}

//============================================================
//	�����L���O�̓Ǎ�����
//============================================================
HRESULT CRankingManager::LoadRank(const char* pPath, float* pRankArray)
{
	// �t�@�C�����J��
	std::ifstream file(pPath, std::ios_base::binary);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		float aRank[MAX_RANK];	// �����L���O�����p

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�����L���O�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// �l�����ׂčő�^�C���ɂ���
		for (int i = 0; i < MAX_RANK; i++)
		{ aRank[i] = RANK_TIME[i]; }

		// �N���A�^�C���̕ۑ�
		SaveRank(pPath, &aRank[0]);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����̊l���󋵂�ǂݍ���
	file.read((char*)pRankArray, sizeof(float) * MAX_RANK);

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�����L���O�̕ۑ�����
//============================================================
HRESULT CRankingManager::SaveRank(const char* pPath, float* pRankArray)
{
	// �t�@�C�����J��
	std::ofstream file(pPath, std::ios_base::binary);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�����L���O�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����̊l���󋵂������o��
	file.write((char*)pRankArray, sizeof(float) * MAX_RANK);

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
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

		// UI�o����Ԃɂ���
		m_state = STATE_SPAWN;
	}
}

//============================================================
//	UI�o���̍X�V����
//============================================================
void CRankingManager::UpdateSpawn(const float fDeltaTime)
{
	// �^�C�}�[�����Z
	m_fCurTime += fDeltaTime;

	// �o�ߎ����̊������v�Z
	float fRate = easeing::InOutQuad(m_fCurTime, fall::MOVE_TIME, 0.0f);

	// �^�C�g�����̔��f
	m_pTitle->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));		// �F�𔽉f
	m_pTitle->SetVec3Position(title::DEST_POS + (((title::DEST_POS + fall::OFFSET_POS) - title::DEST_POS) * fRate));	// �ʒu�𔽉f

	// �����o�����̔��f
	m_pBalloon->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// �F�𔽉f
	m_pBalloon->SetVec3Position(balloon::DEST_POS + (((balloon::DEST_POS + fall::OFFSET_POS) - balloon::DEST_POS) * fRate));	// �ʒu�𔽉f

	// �����L���O�^�C�g���e���̔��f
	const D3DXVECTOR3 posShadow = name::POS + name::OFFSET;				// �e�����ʒu
	m_pShadow->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));	// �����x�𔽉f
	m_pShadow->SetVec3Position(posShadow + (((posShadow + fall::OFFSET_POS) - posShadow) * fRate));	// �ʒu�𔽉f

	// �����L���O�^�C�g�����̔��f
	m_pName->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));	// �����x�𔽉f
	m_pName->SetVec3Position(name::POS + (((name::POS + fall::OFFSET_POS) - name::POS) * fRate));	// �ʒu�𔽉f

	for (int i = 0; i < MAX_RANK; i++)
	{ // �����L���O�\�������J��Ԃ�

		// �����L���O���ԏ��̔��f
		const D3DXVECTOR3 posRank = rank::POS + (rank::SPACE * (float)i);		// �����ʒu
		m_apRankValue[i]->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// �F�𔽉f
		m_apRankValue[i]->SetVec3Position(posRank + (((posRank + fall::OFFSET_POS) - posRank) * fRate));	// �ʒu�𔽉f

		// �����L���O���ԏ��̔��f
		const D3DXVECTOR3 posTime = time::POS + (time::SPACE * (float)i);		// �����ʒu
		m_apRankTime[i]->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// �F�𔽉f
		m_apRankTime[i]->SetVec3Position(posTime + (((posTime + fall::OFFSET_POS) - posTime) * fRate));		// �ʒu�𔽉f
	}

	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		D3DXVECTOR3 posSelect = select::DEST_POS + (((select::DEST_POS + fall::OFFSET_POS) - select::DEST_POS) * fRate);	// �ʒu
		D3DXVECTOR3 offsetSelect = (select::SPACE * (float)i);	// �I�t�Z�b�g

		// �I�����̔��f
		m_apSelect[i]->SetVec3Position(posSelect + offsetSelect);	// �ʒu�𔽉f
		m_apSelect[i]->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));	// �����x�𔽉f
	}

	if (m_fCurTime >= fall::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �^�C�g�����̐F��␳
		m_pTitle->SetColor(fall::INIT_COL);

		// �����o�����̐F��␳
		m_pBalloon->SetColor(fall::INIT_COL);

		// �����L���O�^�C�g���e���̓����x��␳
		m_pShadow->SetAlpha(fall::INIT_ALPHA);

		// �����L���O�^�C�g�����̓����x��␳
		m_pName->SetAlpha(fall::INIT_ALPHA);

		for (int i = 0; i < MAX_RANK; i++)
		{ // �����L���O�\�������J��Ԃ�

			// �����L���O���ԏ��̐F��␳
			m_apRankValue[i]->SetColor(fall::INIT_COL);

			// �����L���O���ԏ��̐F��␳
			m_apRankTime[i]->SetColor(fall::INIT_COL);
		}

		for (int i = 0; i < SELECT_MAX; i++)
		{ // �I�����̑������J��Ԃ�

			// �I�����̓����x��␳
			m_apSelect[i]->SetAlpha(fall::INIT_ALPHA);
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

	// �^�C�g�����̔��f
	m_pTitle->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));		// �F�𔽉f
	m_pTitle->SetVec3Position(title::DEST_POS + (((title::DEST_POS + fall::OFFSET_POS) - title::DEST_POS) * fRate));	// �ʒu�𔽉f

	// �����o�����̔��f
	m_pBalloon->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// �F�𔽉f
	m_pBalloon->SetVec3Position(balloon::DEST_POS + (((balloon::DEST_POS + fall::OFFSET_POS) - balloon::DEST_POS) * fRate));	// �ʒu�𔽉f

	// �����L���O�^�C�g���e���̔��f
	const D3DXVECTOR3 posShadow = name::POS + name::OFFSET;				// �e�����ʒu
	m_pShadow->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));	// �����x�𔽉f
	m_pShadow->SetVec3Position(posShadow + (((posShadow + fall::OFFSET_POS) - posShadow) * fRate));	// �ʒu�𔽉f

	// �����L���O�^�C�g�����̔��f
	m_pName->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));	// �����x�𔽉f
	m_pName->SetVec3Position(name::POS + (((name::POS + fall::OFFSET_POS) - name::POS) * fRate));	// �ʒu�𔽉f

	for (int i = 0; i < MAX_RANK; i++)
	{ // �����L���O�\�������J��Ԃ�

		// �����L���O���ԏ��̔��f
		const D3DXVECTOR3 posRank = rank::POS + (rank::SPACE * (float)i);		// �����ʒu
		m_apRankValue[i]->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// �F�𔽉f
		m_apRankValue[i]->SetVec3Position(posRank + (((posRank + fall::OFFSET_POS) - posRank) * fRate));	// �ʒu�𔽉f

		// �����L���O���ԏ��̔��f
		const D3DXVECTOR3 posTime = time::POS + (time::SPACE * (float)i);		// �����ʒu
		m_apRankTime[i]->SetColor(fall::INIT_COL + (fall::DIFF_COL * fRate));	// �F�𔽉f
		m_apRankTime[i]->SetVec3Position(posTime + (((posTime + fall::OFFSET_POS) - posTime) * fRate));		// �ʒu�𔽉f
	}

	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		D3DXVECTOR3 posSelect = select::DEST_POS + (((select::DEST_POS + fall::OFFSET_POS) - select::DEST_POS) * fRate);	// �ʒu
		D3DXVECTOR3 offsetSelect = (select::SPACE * (float)i);	// �I�t�Z�b�g

		// �I�����̔��f
		m_apSelect[i]->SetVec3Position(posSelect + offsetSelect);	// �ʒu�𔽉f
		m_apSelect[i]->SetAlpha(fall::INIT_ALPHA + (fall::DIFF_ALPHA * fRate));	// �����x�𔽉f
	}

	if (m_fCurTime >= fall::MOVE_TIME)
	{ // �ҋ@���I�������ꍇ

		// �^�C�}�[��������
		m_fCurTime = 0.0f;

		// �^�C�g�����̐F��␳
		m_pTitle->SetColor(fall::DEST_COL);

		// �����o�����̐F��␳
		m_pBalloon->SetColor(fall::DEST_COL);

		// �����L���O�^�C�g���e���̓����x��␳
		m_pShadow->SetAlpha(fall::DEST_ALPHA);

		// �����L���O�^�C�g�����̓����x��␳
		m_pName->SetAlpha(fall::DEST_ALPHA);

		for (int i = 0; i < MAX_RANK; i++)
		{ // �����L���O�\�������J��Ԃ�

			// �����L���O���ԏ��̐F��␳
			m_apRankValue[i]->SetColor(fall::DEST_COL);

			// �����L���O���ԏ��̐F��␳
			m_apRankTime[i]->SetColor(fall::DEST_COL);
		}

		for (int i = 0; i < SELECT_MAX; i++)
		{ // �I�����̑������J��Ԃ�

			// �I�����̓����x��␳
			m_apSelect[i]->SetAlpha(fall::DEST_ALPHA);
		}

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

	// �^�C�g���̐F��␳
	m_pTitle->SetColor(title::DEST_COL);

	// �^�C�g���̈ʒu��␳
	m_pTitle->SetVec3Position(title::DEST_POS);

	// �����o�����̐F��␳
	m_pBalloon->SetColor(balloon::DEST_COL);

	// �����L���O�^�C�g���e���̓����x��␳
	m_pShadow->SetAlpha(1.0f);

	// �����L���O�^�C�g�����̓����x��␳
	m_pName->SetAlpha(1.0f);

	for (int i = 0; i < MAX_RANK; i++)
	{ // �����L���O�\�������J��Ԃ�

		// �����L���O���ԏ��̐F��␳
		m_apRankValue[i]->SetColor(XCOL_WHITE);

		// �����L���O���ԏ��̐F��␳
		m_apRankTime[i]->SetColor(XCOL_WHITE);
	}

	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		// �I�����̐F��␳
		m_apSelect[i]->SetColor(select::DEST_COL);

		// �I�����̈ʒu��␳
		m_apSelect[i]->SetVec3Position(select::DEST_POS + (select::SPACE * (float)i));
	}
}
