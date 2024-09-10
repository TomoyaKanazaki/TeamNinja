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
#include "player.h"
#include "titleLogo2D.h"
#include "blink2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 5;	// �^�C�g���̗D�揇��
	const D3DXVECTOR3 ABS_INIT_OFFSET	= D3DXVECTOR3(80.0f, 0.0f, 0.0f);	// �����ʒu�I�t�Z�b�g�̐�Βl
	const D3DXVECTOR3 DEST_OFFSET		= D3DXVECTOR3(0.0f, 170.0f, 0.0f);	// �����ʒu�I�t�Z�b�g�̐�Βl
	const D3DXVECTOR3 POS_ONE	= D3DXVECTOR3(640.0f, 95.0f, 0.0f);			// ���ڕW�ʒu
	const D3DXVECTOR3 POS_TWO	= D3DXVECTOR3(260.0f, 95.0f, 0.0f);			// ���ڕW�ʒu
	const D3DXVECTOR3 SIZE_POLY	= D3DXVECTOR3(320.0f, 320.0f, 0.0f);		// �傫��
	const POSGRID2 TEX_PART		= POSGRID2(1, 4);	// �e�N�X�`��������
	const float MOVE_TIME_ONE	= 1.8f;		// �����ړ�����
	const float MOVE_TIME_TWO	= 3.0f;		// �����ړ�����
	const float WAIT_TIME		= 0.092f;	// �����ҋ@����
	const float TRANS_TIME		= 3.0f;		// �J�ڑҋ@����

	const D3DXCOLOR COL_BLUR[] =	// �u���[�F
	{
		D3DCOLOR_RGBA(110, 228, 234, 255),	// ���F
		D3DCOLOR_RGBA(255, 142, 204, 255),	// ���F
	};

	const char* TEXTURE_BLUR = "data\\TEXTURE\\title_blur000.png";	// �u���[�e�N�X�`��
	const char* TEXTURE_LOGO = "data\\TEXTURE\\title_logo000.png";	// ���S�e�N�X�`��
	const char* TEXTURE_AURA = "data\\TEXTURE\\title_aura000.png";	// �I�[���e�N�X�`��

	namespace control
	{
		const char* TEXTURE		= "data\\TEXTURE\\title_start000.png";	// ����e�N�X�`��
		const D3DXVECTOR3 POS	= D3DXVECTOR3(845.0f, 595.0f, 0.0f);	// �ʒu
		const D3DXVECTOR3 SIZE	= D3DXVECTOR3(848.0f, 180.0f, 0.0f);	// �傫��
		const float MIN_ALPHA	= 0.4f;	// �Œᓧ���x
		const float MAX_ALPHA	= 1.0f;	// �ő哧���x
		const float LEVEL_ALPHA	= 3.4f;	// �����x�̉�����
		const float SUB_ALPHA	= 0.5f;	// �C���̃��l������
		const float ADD_ALPHA	= 1.0f;	// �A�E�g�̃��l������
	}
}

//************************************************************
//	�e�N���X [CTitleManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================

CTitleManager::CTitleManager() :
	m_pStart	(nullptr),		// �J�n������
	m_state		(STATE_NONE),	// ���
	m_fCurTime	(0.0f)			// ���݂̌o�ߎ���
{
	// �����o�ϐ����N���A
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));	// �^�C�g�����
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
	// �����o�ϐ���������
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));	// �^�C�g�����
	m_pStart	= nullptr;		// �J�n������
	m_state		= STATE_NONE;	// ���
	m_fCurTime	= 0.0f;			// ���݂̌o�ߎ���

	//--------------------------------------------------------
	//	�^�C�g���̐����E�ݒ�
	//--------------------------------------------------------
	for (int i = 0; i < NUM_LOGO; i++)
	{ // ���S�̕��������J��Ԃ�

		int nFlag = (i % 2);					// ���l�t���O
		float fSide = (1.0f - (nFlag * 2.0f));	// ���������W��
		float fWaitTime = WAIT_TIME * (float)i;	// �ҋ@����
		D3DXCOLOR colBlur = COL_BLUR[nFlag];	// �����ʒu�I�t�Z�b�g
		D3DXVECTOR3 destOffset = DEST_OFFSET * (float)i;	// �ڕW�ʒu
		D3DXVECTOR3 initOffset = ABS_INIT_OFFSET * fSide;	// �����ʒu�I�t�Z�b�g

		// �^�C�g���̐���
		m_apLogo[i] = CTitleLogo2D::Create
		( // ����
			TEXTURE_BLUR,			// �u���[�e�N�X�`���p�X
			POS_ONE + destOffset,	// ���ڕW�ʒu
			POS_TWO + destOffset,	// ���ڕW�ʒu
			initOffset,		// �I�t�Z�b�g
			SIZE_POLY,		// �傫��
			MOVE_TIME_ONE,	// ���ړ�����
			MOVE_TIME_TWO,	// ���ړ�����
			fWaitTime,		// ���ҋ@����
			fWaitTime		// ���ҋ@����
		);
		if (m_apLogo[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`��������
		m_apLogo[i]->BindTexture(TEXTURE_LOGO);

		// �I�[���e�N�X�`��������
		m_apLogo[i]->BindAuraTexture(TEXTURE_AURA);

		// �u���[�F��ݒ�
		m_apLogo[i]->SetBlurColor(colBlur);

		// �p�^�[����ݒ�
		m_apLogo[i]->SetPattern(i);

		// �e�N�X�`������������ݒ�
		m_apLogo[i]->SetWidthPattern(TEX_PART.x);

		// �e�N�X�`���c��������ݒ�
		m_apLogo[i]->SetHeightPattern(TEX_PART.y);

		// �D�揇�ʂ�ݒ�
		m_apLogo[i]->SetPriority(PRIORITY);
	}

	// �J�n����̐���
	m_pStart = CBlink2D::Create
	( // ����
		control::POS,			// �ʒu
		control::SIZE,			// �傫��
		control::MIN_ALPHA,		// �Œᓧ���x
		control::MAX_ALPHA,		// �ő哧���x
		control::LEVEL_ALPHA,	// �����x�̉�����
		control::SUB_ALPHA,		// �C���̃��l������
		control::ADD_ALPHA		// �A�E�g�̃��l������
	);
	if (m_pStart == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`��������
	m_pStart->BindTexture(control::TEXTURE);

	// �D�揇�ʂ�ݒ�
	m_pStart->SetPriority(PRIORITY);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTitleManager::Uninit(void)
{
	for (int i = 0; i < NUM_LOGO; i++)
	{ // ���S�̕��������J��Ԃ�

		// �^�C�g�����̏I��
		SAFE_UNINIT(m_apLogo[i]);
	}
}

//============================================================
//	�X�V����
//============================================================
void CTitleManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:	// �������Ȃ�
	{
		// �t�F�[�h���Ȃ甲����
		if (GET_MANAGER->GetFade()->IsFade()) { break; }

		for (int i = 0; i < NUM_LOGO; i++)
		{ // ���S�̕��������J��Ԃ�

			// ���o���J�n����
			m_apLogo[i]->SetStag();
		}

		// ���S�\����Ԃɂ���
		m_state = STATE_LOGO;
		break;
	}
	case STATE_LOGO:	// ���S�\��
	{
		int nCurIdx = NUM_LOGO - 1;	// �Ō���C���f�b�N�X
		if (!m_apLogo[nCurIdx]->IsStag())
		{ // �Ō���̉��o���I�������ꍇ

			// �J�n����̓_�����J�n����
			m_pStart->SetBlink(true);

			// ����ҋ@��Ԃɂ���
			m_state = STATE_WAIT;
		}
		break;
	}
	case STATE_WAIT:	// ����ҋ@
	{
		if (GET_INPUTKEY->IsAnyTrigger()
		||  GET_INPUTPAD->IsAnyTrigger())
		{
			// �J�����̑����Ԑݒ�
			GET_CAMERA->SetRotateRun();

			// ���o�J�n�̐ݒ�
			GET_PLAYER->SetStart();

			// �J�n����̓_�����I������
			m_pStart->SetBlink(false);

			// �E�Ҏ�����Ԃɂ���
			m_state = STATE_RUN;
		}
		break;
	}
	case STATE_RUN:		// �E�Ҏ���
	{
		// �o�ߎ��Ԃ����Z
		m_fCurTime += fDeltaTime;
		if (m_fCurTime >= TRANS_TIME)
		{ // �v���C���[���������ނ�𒴂����ꍇ

			// �Z���N�g��ʂɑJ��
			GET_MANAGER->SetLoadScene(CScene::MODE_SELECT);
		}
		break;
	}
	case STATE_END:		// �I��
	{
		break;
	}
	default:	// ��O����
		assert(false);
		break;
	}

	// TODO�F�f�o�b�O
#ifdef _DEBUG
	if (GET_INPUTKEY->IsTrigger(DIK_F5))
	{
		GET_MANAGER->SetLoadScene(CScene::MODE_SELECT);
	}
#endif
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
