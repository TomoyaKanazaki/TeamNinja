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
#include "titleLogo2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 5;	// �^�C�g���̗D�揇��
	const D3DXVECTOR3 ABS_INIT_OFFSET	= D3DXVECTOR3(80.0f, 0.0f, 0.0f);	// �����ʒu�I�t�Z�b�g�̐�Βl
	const D3DXVECTOR3 DEST_OFFSET		= D3DXVECTOR3(0.0f, 170.0f, 0.0f);	// �����ʒu�I�t�Z�b�g�̐�Βl
	const D3DXVECTOR3 POS_POLY	= D3DXVECTOR3(640.0f, 95.0f, 0.0f);			// �ʒu
	const D3DXVECTOR3 SIZE_POLY	= D3DXVECTOR3(320.0f, 320.0f, 0.0f);		// �傫��
	const POSGRID2 TEX_PART = POSGRID2(1, 4);	// �e�N�X�`��������
	const float MOVE_TIME = 1.8f;	// �����ړ�����
	const float WAIT_TIME = 0.092f;	// �����ҋ@����

	const D3DXCOLOR COL_BLUR[] =	// �u���[�F
	{
		D3DCOLOR_RGBA(110, 228, 234, 255),	// ���F
		D3DCOLOR_RGBA(255, 142, 204, 255),	// ���F
	};
}

//************************************************************
//	�e�N���X [CTitleManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTitleManager::CTitleManager()
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
			"data\\TEXTURE\\title_blur000.png",	// �u���[�e�N�X�`���p�X
			POS_POLY + destOffset,	// �ʒu
			initOffset,	// �I�t�Z�b�g
			SIZE_POLY,	// �傫��
			MOVE_TIME,	// �ړ�����
			fWaitTime	// �ҋ@����
		);
		if (m_apLogo[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`��������
		m_apLogo[i]->BindTexture("data\\TEXTURE\\title_logo000.png");

		// �I�[���e�N�X�`��������
		m_apLogo[i]->BindAuraTexture("data\\TEXTURE\\title_aura000.png");

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
	if (GET_INPUTKEY->IsTrigger(DIK_0))
	{
		for (int i = 0; i < NUM_LOGO; i++)
		{ // ���S�̕��������J��Ԃ�
			m_apLogo[i]->SetStag();
		}
	}
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
