//============================================================
//
//	�����o���}�l�[�W���[���� [balloonManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "balloonManager.h"
#include "manager.h"
#include "stage.h"
#include "loadtext.h"
#include "transpoint.h"
#include "object2D.h"
#include "roll2D.h"
#include "scale2D.h"
#include "scrollText2D.h"
#include "anim2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXCOLOR COL_OPEN	= XCOL_WHITE;							// ����\�F
	const D3DXCOLOR COL_UNOPEN	= D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f);	// ����s�\�F

	namespace stage
	{
		const D3DXVECTOR3 POS	= SCREEN_CENT;			// �ʒu
		const D3DXVECTOR3 SIZE	= SCREEN_SIZE * 0.66f;	// �傫��
	}

	namespace frame
	{
		const char* TEXTURE		= "data\\TEXTURE\\stageFrame000.png";	// �t���[���e�N�X�`���p�X
		const D3DXVECTOR3 POS	= SCREEN_CENT;							// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(1227.0f, 720.0f, 0.0f);	// �傫��
	}

	namespace star
	{
		const char*	TEXTURE	= "data\\TEXTURE\\star000.png";	// �藠���e�N�X�`���p�X
		const D3DXVECTOR3 POS[CBalloonManager::NUM_STAR]	= { D3DXVECTOR3(140.0f, 230.0f, 0.0f),	D3DXVECTOR3(240.0f, 140.0f, 0.0f) };	// �ʒu
		const D3DXVECTOR3 ROT[CBalloonManager::NUM_STAR]	= { D3DXVECTOR3(0.0f, 0.0f, 0.8f),		D3DXVECTOR3(0.0f, 0.0f, 0.2f) };		// ����
		const D3DXVECTOR3 SIZE[CBalloonManager::NUM_STAR]	= { D3DXVECTOR3(250.0f, 250.0f, 0.0f),	D3DXVECTOR3(320.0f, 320.0f, 0.0f) };	// �傫��
		const D3DXCOLOR COL[CBalloonManager::NUM_STAR]		= { XCOL_WHITE, D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f) };	// �F
		const float	ADD_ROT[CBalloonManager::NUM_STAR]		= { 0.2f, 0.3f };	// ��]��
	}

	namespace cont
	{
		const char* TEXTURE		= "data\\TEXTURE\\button_a.png";		// ������@�e�N�X�`���p�X
		const D3DXVECTOR3 POS	= D3DXVECTOR3(975.0f, 500.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(160.0f, 160.0f, 0.0f);	// �傫��
		const float MIN_SCALE	= 0.95f;	// �Œ�g�嗦
		const float MAX_SCALE	= 1.45f;	// �ő�g�嗦
		const float INIT_SCALE	= 0.95f;	// �����g�嗦
		const float CALC_SCALE	= 7.0f;		// �g�嗦�̉�����
	}

	namespace item
	{
		const D3DXCOLOR COL[] =
		{
			D3DXCOLOR(0.35f, 0.35f, 0.35f, 1.0f),	// ���l���F
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// �l���ςݐF
		};

		const char* TEXTURE		= "data\\TEXTURE\\itemGod000.png";		// �_��e�N�X�`��
		const POSGRID2 TEX_PART	= POSGRID2(3, 1);						// �e�N�X�`������
		const D3DXVECTOR3 POS	= D3DXVECTOR3(180.0f, 570.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SPACE	= D3DXVECTOR3(165.0f, 0.0f, 0.0f);		// ��
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(190.0f, 190.0f, 0.0f);	// �傫��
	}

	namespace name
	{
		const char *FONT = "data\\FONT\\�ʂ˂�������������v7��.ttf";	// �t�H���g�p�X
		const bool	ITALIC			= true;		// �C�^���b�N
		const float	CHAR_HEIGHT		= 120.0f;	// �����c��
		const float	LINE_HEIGHT		= 120.0f;	// �s�ԏc��
		const float	WAIT_TIME_NOR	= 0.017f;	// �����\���̑ҋ@����
		const float	WAIT_TIME_NOR_FIRST	= 0.17f;	// �����\���̑ҋ@����

		const D3DXVECTOR3 POS	 = D3DXVECTOR3(90.0f, 40.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 OFFSET = D3DXVECTOR3(8.0f, 8.0f, 0.0f);	// �e�I�t�Z�b�g
		const D3DXVECTOR3 ROT	 = VEC3_ZERO;						// ����

		const CString2D::EAlignX ALIGN_X = CString2D::XALIGN_LEFT;	// ���z�u
		const CText2D::EAlignY	 ALIGN_Y = CText2D::YALIGN_TOP;		// �c�z�u
		const D3DXCOLOR COL_SHADOW	= XCOL_BLUE;	// �e�̐F
		const D3DXCOLOR COL_NAME	= XCOL_WHITE;	// ���O�̐F
	}
}

//************************************************************
//	�q�N���X [CBalloonManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBalloonManager::CBalloonManager(CTransPoint* pParent, const bool bOpen) :
	m_pParent	(pParent),		// �J�ڃ|�C���g
	m_pStage	(nullptr),		// �X�e�[�W���
	m_pFrame	(nullptr),		// �t���[��
	m_pCont		(nullptr),		// ������@
	m_pShadow	(nullptr),		// �X�e�[�W���̉e
	m_pName		(nullptr),		// �X�e�[�W��
	m_state		(STATE_NONE),	// ���
	m_bOpen		(bOpen)			// �X�e�[�W����t���O
{
	// �����o�ϐ����N���A
	memset(&m_apGodItem[0], 0, sizeof(m_apGodItem));	// �_��A�C�R��
	memset(&m_apStar[0], 0, sizeof(m_apStar));			// �藠��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CBalloonManager::~CBalloonManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBalloonManager::Init(void)
{
	// �����o�ϐ���������
	memset(&m_apGodItem[0], 0, sizeof(m_apGodItem));	// �_��A�C�R��
	memset(&m_apStar[0], 0, sizeof(m_apStar));			// �藠��
	m_pStage	= nullptr;		// �X�e�[�W���
	m_pFrame	= nullptr;		// �t���[��
	m_pCont		= nullptr;		// ������@
	m_pShadow	= nullptr;		// �X�e�[�W���̉e
	m_pName		= nullptr;		// �X�e�[�W��
	m_state		= STATE_NONE;	// ���

	// �r���{�[�h�V�[�����̃I�u�W�F�N�g��S�j��
	CObject::ReleaseAll(CObject::SCENE_BILLBOARD);

	// �|���S���F��ݒ�
	D3DXCOLOR colPoly = (m_bOpen) ? COL_OPEN : COL_UNOPEN;

	//--------------------------------------------------------
	//	�X�e�[�W��ʃ|���S���̐��� / �ݒ�
	//--------------------------------------------------------
	// �X�e�[�W��ʃ|���S���̐���
	m_pStage = CObject2D::Create(stage::POS, stage::SIZE, VEC3_ZERO, colPoly);
	if (m_pStage == nullptr) { assert(false); return E_FAIL; }	// ���s�����ꍇ������

	// ���̐ݒ�
	m_pStage->BindTexture("data\\TEXTURE\\stage000.png");	// �X�e�[�W��ʂ̃e�N�X�`������	// TODO�F�Œ�ɂȂ��Ă��
	m_pStage->SetScene(CObject::SCENE_BILLBOARD);			// �I�u�W�F�N�g�V�[�����r���{�[�h��
	m_pStage->SetLabel(CObject::LABEL_UI);					// �����X�V/�����j������悤��

	//--------------------------------------------------------
	//	�t���[���|���S���̐��� / �ݒ�
	//--------------------------------------------------------
	// �t���[���|���S���̐���
	m_pFrame = CObject2D::Create(frame::POS, frame::SIZE, VEC3_ZERO, colPoly);
	if (m_pFrame == nullptr) { assert(false); return E_FAIL; }	// ���s�����ꍇ������

	// ���̐ݒ�
	m_pFrame->BindTexture(frame::TEXTURE);			// �t���[���̃e�N�X�`������
	m_pFrame->SetScene(CObject::SCENE_BILLBOARD);	// �I�u�W�F�N�g�V�[�����r���{�[�h��
	m_pFrame->SetLabel(CObject::LABEL_UI);			// �����X�V/�����j������悤��

	//--------------------------------------------------------
	//	�藠���̐��� / �ݒ�
	//--------------------------------------------------------
	for (int i = 0; i < CBalloonManager::NUM_STAR; i++)
	{ // �藠���̑������J��Ԃ�

		// �藠���̐F��ݒ�
		D3DXCOLOR colStar = (m_bOpen) ? star::COL[i] : COL_UNOPEN;

		// �藠���̐���
		m_apStar[i] = CRoll2D::Create(star::POS[i], star::SIZE[i], star::ADD_ROT[i], star::ROT[i], colStar);
		if (m_apStar[i] == nullptr) { assert(false); return E_FAIL; }	// ���s�����ꍇ������

		// ���̐ݒ�
		m_apStar[i]->BindTexture(star::TEXTURE);			// �t���[���̃e�N�X�`������
		m_apStar[i]->SetScene(CObject::SCENE_BILLBOARD);	// �I�u�W�F�N�g�V�[�����r���{�[�h��
	}

	//--------------------------------------------------------
	//	������@�̐��� / �ݒ�
	//--------------------------------------------------------
	// ������@�̐���
	m_pCont = CScale2D::Create(cont::POS, cont::SIZE, cont::MIN_SCALE, cont::MAX_SCALE, cont::INIT_SCALE, cont::CALC_SCALE, 1.0f, 1.0f, VEC3_ZERO, colPoly);
	if (m_pCont == nullptr) { assert(false); return E_FAIL; }	// ���s�����ꍇ������

	// ���̐ݒ�
	m_pCont->BindTexture(cont::TEXTURE);			// ������@�̃e�N�X�`������
	m_pCont->SetScene(CObject::SCENE_BILLBOARD);	// �I�u�W�F�N�g�V�[�����r���{�[�h��
	m_pCont->SetLabel(CObject::LABEL_UI);			// �����X�V/�����j������悤��

	//--------------------------------------------------------
	//	�_��A�C�R���̐��� / �����ݒ�
	//--------------------------------------------------------
	// �_��l���p�X���쐬
	std::filesystem::path path = m_pParent->GetTransMapPass();	// �}�b�v�f�B���N�g��
	path = path.parent_path();	// �}�b�v�f�B���N�g���ɂ���
	path.append("goditem.bin");	// �_��l���p�X��ǉ�

	// �O��܂ł̌��ʂ̊l���󋵂�ǂݍ���
	bool bOldGet[CGodItem::TYPE_MAX] = {};	// �ۑ�����Ă���l����
	CGodItem::LoadPossess(path.string().c_str(), &bOldGet[0]);

	for (int i = 0; i < CGodItem::TYPE_MAX; i++)
	{ // �_��̑������J��Ԃ�

		// �A�C�R�������ʒu���v�Z
		D3DXVECTOR3 posIcon = item::POS + (item::SPACE * (float)i);

		// �藠���̐F��ݒ�
		D3DXCOLOR colIcon = (m_bOpen) ? item::COL[(int)bOldGet[i]] : item::COL[(int)false];

		// �_��A�C�R���̐���
		m_apGodItem[i] = CAnim2D::Create(item::TEX_PART.x, item::TEX_PART.y, posIcon, item::SIZE, VEC3_ZERO, colIcon);
		if (m_apGodItem[i] == nullptr) { assert(false); return E_FAIL; }	// ���s�����ꍇ������

		// ���̐ݒ�
		m_apGodItem[i]->BindTexture(item::TEXTURE);			// ���ʂ̃e�N�X�`������
		m_apGodItem[i]->SetScene(CObject::SCENE_BILLBOARD);	// �I�u�W�F�N�g�V�[�����r���{�[�h��
		m_apGodItem[i]->SetLabel(CObject::LABEL_UI);		// �����X�V/�����j������悤��
		m_apGodItem[i]->SetPattern(i);						// �e�N�X�`���p�^�[����ݒ�
	}

	if (m_bOpen)
	{ // �������Ă���ꍇ

		//----------------------------------------------------
		//	�X�e�[�W���̉e�̐��� / �ݒ�
		//----------------------------------------------------
		// �X�e�[�W���̉e�̐���
		m_pShadow = CScrollText2D::Create
		( // ����
			name::FONT,					// �t�H���g�p�X
			name::ITALIC,				// �C�^���b�N
			name::POS + name::OFFSET,	// ���_�ʒu
			name::WAIT_TIME_NOR,		// �����\���̑ҋ@����
			name::CHAR_HEIGHT,			// �����c��
			name::LINE_HEIGHT,			// �s�ԏc��
			name::ALIGN_X,				// ���z�u
			name::ALIGN_Y,				// �c�z�u
			name::ROT,					// ���_����
			name::COL_SHADOW			// �F
		);
		if (m_pShadow == nullptr) { assert(false); return E_FAIL; }	// ���s�����ꍇ������

		// ���̐ݒ�
		m_pShadow->SetScene(CObject::SCENE_BILLBOARD);	// �I�u�W�F�N�g�V�[�����r���{�[�h��

		// �e�L�X�g������
		loadtext::BindText(m_pShadow, loadtext::LoadText(GET_STAGE->Regist(m_pParent->GetTransMapPass().c_str()).sStage.c_str(), 0));

		//----------------------------------------------------
		//	�X�e�[�W���̐��� / �ݒ�
		//----------------------------------------------------
		// �X�e�[�W���̐���
		m_pName = CScrollText2D::Create
		( // ����
			name::FONT,				// �t�H���g�p�X
			name::ITALIC,			// �C�^���b�N
			name::POS,				// ���_�ʒu
			name::WAIT_TIME_NOR,	// �����\���̑ҋ@����
			name::CHAR_HEIGHT,		// �����c��
			name::LINE_HEIGHT,		// �s�ԏc��
			name::ALIGN_X,			// ���z�u
			name::ALIGN_Y,			// �c�z�u
			name::ROT,				// ���_����
			name::COL_NAME			// �F
		);
		if (m_pName == nullptr) { assert(false); return E_FAIL; }	// ���s�����ꍇ������

		// ���̐ݒ�
		m_pName->SetScene(CObject::SCENE_BILLBOARD);	// �I�u�W�F�N�g�V�[�����r���{�[�h��

		// �e�L�X�g������
		loadtext::BindText(m_pName, loadtext::LoadText(GET_STAGE->Regist(m_pParent->GetTransMapPass().c_str()).sStage.c_str(), 0));
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CBalloonManager::Uninit(void)
{
	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CBalloonManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:
		break;

	case STATE_STAG:

		if (!m_pName->IsScroll())
		{ // �������肪�I�������ꍇ

			for (int i = 0; i < CBalloonManager::NUM_STAR; i++)
			{ // �藠���̑������J��Ԃ�

				// ������]��ON�ɂ���
				m_apStar[i]->SetRoll(true);
			}

			// �����g�k��ON�ɂ���
			m_pCont->SetScale(true);

			// �I����Ԃɂ���
			m_state = STATE_END;
		}
		break;

	case STATE_END:
		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CBalloonManager::Draw(CShader * /*pShader*/)
{

}

//============================================================
//	��������
//============================================================
CBalloonManager *CBalloonManager::Create(CTransPoint* pParent, const bool bOpen)
{
	// �����o���}�l�[�W���[�̐���
	CBalloonManager *pBalloonManager = new CBalloonManager(pParent, bOpen);
	if (pBalloonManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �����o���}�l�[�W���[�̏�����
		if (FAILED(pBalloonManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �����o���}�l�[�W���[�̔j��
			SAFE_DELETE(pBalloonManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pBalloonManager;
	}
}

//============================================================
//	���񉉏o�J�n�̐ݒ菈��
//============================================================
void CBalloonManager::SetFirstStag(void)
{
	// �����\���̑ҋ@���Ԑݒ�
	m_pShadow->SetNextTime(name::WAIT_TIME_NOR_FIRST);	// �X�e�[�W���̉e
	m_pName->SetNextTime(name::WAIT_TIME_NOR_FIRST);	// �X�e�[�W��

	// ����������J�n
	m_pShadow->SetEnableDraw(true);
	m_pName->SetEnableDraw(true);

	for (int i = 0; i < CBalloonManager::NUM_STAR; i++)
	{ // �藠���̑������J��Ԃ�

		// ������]��ON�ɂ���
		m_apStar[i]->SetRoll(true);
	}

	// �I����Ԃɂ���
	m_state = STATE_END;
}

//============================================================
//	���o�J�n�̐ݒ菈��
//============================================================
void CBalloonManager::SetStag(void)
{
	if (m_bOpen)
	{ // �������Ă���ꍇ

		// �����\���̑ҋ@���Ԑݒ�
		m_pShadow->SetNextTime(name::WAIT_TIME_NOR);	// �X�e�[�W���̉e
		m_pName->SetNextTime(name::WAIT_TIME_NOR);		// �X�e�[�W��

		// ����������J�n
		m_pShadow->SetEnableScroll(true);
		m_pName->SetEnableScroll(true);

		// ���o��Ԃɂ���
		m_state = STATE_STAG;
	}
	else
	{ // �������Ă��Ȃ��ꍇ

		// �I����Ԃɂ���
		m_state = STATE_END;
	}
}
