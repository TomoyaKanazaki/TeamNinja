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
#include "scrollText2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
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

	namespace name
	{
		const char *FONT = "data\\FONT\\��S�V�b�N.otf";	// �t�H���g�p�X
		const bool	ITALIC			= true;		// �C�^���b�N
		const float	CHAR_HEIGHT		= 140.0f;	// �����c��
		const float	LINE_HEIGHT		= 120.0f;	// �s�ԏc��
		const float	WAIT_TIME_NOR	= 0.017f;	// �����\���̑ҋ@����

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
CBalloonManager::CBalloonManager(CTransPoint* pParent) :
	m_pParent	(pParent)	// �J�ڃ|�C���g���
{

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
	//--------------------------------------------------------
	//	�����ς݂̃e�N�X�`���쐬�p�I�u�W�F�N�g�̔j��
	//--------------------------------------------------------
	// �r���{�[�h�V�[�����̃I�u�W�F�N�g��S�j��
	CObject::ReleaseAll(CObject::SCENE_BILLBOARD);

	//--------------------------------------------------------
	//	�X�e�[�W��ʃ|���S���̐��� / �ݒ�
	//--------------------------------------------------------
	// �X�e�[�W��ʃ|���S���̐���
	CObject2D* pStage = CObject2D::Create(stage::POS, stage::SIZE);
	if (pStage == nullptr) { assert(false); return E_FAIL; }	// ���s�����ꍇ������

	// ���̐ݒ�
	pStage->BindTexture("data\\TEXTURE\\stage000.png");	// �X�e�[�W��ʂ̃e�N�X�`������	// TODO�F�Œ�ɂȂ��Ă��
	pStage->SetScene(CObject::SCENE_BILLBOARD);			// �I�u�W�F�N�g�V�[�����r���{�[�h��
	pStage->SetLabel(CObject::LABEL_UI);				// �����X�V/�����j������悤��

	//--------------------------------------------------------
	//	�t���[���|���S���̐��� / �ݒ�
	//--------------------------------------------------------
	// �t���[���|���S���̐���
	CObject2D* pFrame = CObject2D::Create(frame::POS, frame::SIZE);
	if (pFrame == nullptr) { assert(false); return E_FAIL; }	// ���s�����ꍇ������

	// ���̐ݒ�
	pFrame->BindTexture(frame::TEXTURE);		// �t���[���̃e�N�X�`������
	pFrame->SetScene(CObject::SCENE_BILLBOARD);	// �I�u�W�F�N�g�V�[�����r���{�[�h��
	pFrame->SetLabel(CObject::LABEL_UI);		// �����X�V/�����j������悤��

	//--------------------------------------------------------
	//	�X�e�[�W���̉e�̐��� / �ݒ�
	//--------------------------------------------------------
	// �X�e�[�W���̉e�̐���
	CScrollText2D* pShadow = CScrollText2D::Create
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
	if (pShadow == nullptr) { assert(false); return E_FAIL; }	// ���s�����ꍇ������

	// ���̐ݒ�
	pShadow->SetScene(CObject::SCENE_BILLBOARD);	// �I�u�W�F�N�g�V�[�����r���{�[�h��
	pShadow->SetEnableScroll(true);					// ����������J�n

	// �e�L�X�g������
	loadtext::BindText(pShadow, loadtext::LoadText(GET_STAGE->Regist(m_pParent->GetTransMapPass().c_str()).sStage.c_str(), 0));

	//--------------------------------------------------------
	//	�X�e�[�W���̐��� / �ݒ�
	//--------------------------------------------------------
	// �X�e�[�W���̐���
	CScrollText2D* pName = CScrollText2D::Create
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
	if (pName == nullptr) { assert(false); return E_FAIL; }	// ���s�����ꍇ������

	// ���̐ݒ�
	pName->SetScene(CObject::SCENE_BILLBOARD);	// �I�u�W�F�N�g�V�[�����r���{�[�h��
	pName->SetEnableScroll(true);				// ����������J�n

	// �e�L�X�g������
	loadtext::BindText(pName, loadtext::LoadText(GET_STAGE->Regist(m_pParent->GetTransMapPass().c_str()).sStage.c_str(), 0));

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

}

//============================================================
//	�`�揈��
//============================================================
void CBalloonManager::Draw(CShader *pShader)
{

}

//============================================================
//	��������
//============================================================
CBalloonManager *CBalloonManager::Create(CTransPoint* pParent)
{
	// �����o���}�l�[�W���[�̐���
	CBalloonManager *pBalloonManager = new CBalloonManager(pParent);
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
