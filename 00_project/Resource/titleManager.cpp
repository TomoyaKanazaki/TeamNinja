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
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 5;	// �^�C�g���̗D�揇��

	namespace icon_bg
	{
		const float	MOVE_TIME	= 0.68f;	// �ړ�����
		const float	PLUS_TIME	= 0.1f;		// �o�߂̉�������
		const float	WAIT_TIME	= 0.15f;	// �A�C�R���w�i�ҋ@����
		const float	DEST_ALPHA	= 1.0f;		// �ڕW�����x
		const float	INIT_ALPHA	= 0.0f;		// ���������x
		const float	DIFF_ALPHA	= DEST_ALPHA - INIT_ALPHA;	// ���������x
		const D3DXCOLOR DEST_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, DEST_ALPHA);		// �ڕW�F
		const D3DXCOLOR INIT_COL	= D3DXCOLOR(1.0f, 1.0f, 1.0f, INIT_ALPHA);		// �����F
		const D3DXVECTOR3 SPACE		= D3DXVECTOR3(140.0f, 0.0f, 0.0f);				// ��
		const D3DXVECTOR3 DEST_POS	= D3DXVECTOR3(865.0f, 585.0f, 0.0f);			// �ڕW�ʒu
		const D3DXVECTOR3 INIT_POS	= DEST_POS + D3DXVECTOR3(0.0f, 40.0f, 0.0f);	// �����ʒu
		const D3DXVECTOR3 DIFF_POS	= DEST_POS - INIT_POS;							// �����ʒu
	}
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

#if 0
	//--------------------------------------------------------
	//	�^�C�g���̐����E�ݒ�
	//--------------------------------------------------------
	for (int i = 0; i < NUM_LOGO; i++)
	{ // ���S�̕��������J��Ԃ�

		// �^�C�g���̐���
		m_apLogo[i] = CObject2D::Create
		( // ����
			logo::POS,			// �ʒu
			logo::INIT_SIZE		// �傫��
		);
		if (m_apLogo[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`��������
		m_apLogo[i]->BindTexture("");

		// �D�揇�ʂ�ݒ�
		m_apLogo[i]->SetPriority(PRIORITY);

		// ���x����ݒ�
		m_apLogo[i]->SetLabel(CObject::LABEL_UI);

		// �`���OFF�ɂ���
		m_apLogo[i]->SetEnableDraw(false);
	}
#endif

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
