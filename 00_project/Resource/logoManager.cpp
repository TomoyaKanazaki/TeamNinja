//============================================================
//
//	���S�}�l�[�W���[���� [logoManager.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "logoManager.h"
#include "manager.h"
#include "fade.h"
#include "scene.h"
#include "sceneLogo.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	namespace logo
	{
		const D3DXVECTOR3 SIZE[CLogoManager::POLYGON_MAX] =		// �T�C�Y
		{
			D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),		// �w�i
			D3DXVECTOR3(200.0f, 200.0f, 0.0f),					// ���S
		};
		const char* TEXTURE[CLogoManager::POLYGON_MAX] =
		{
			nullptr,							// ���S
			"data\\TEXTURE\\alcohol.png",		// ���S
		};
	}
}

//************************************************************
//	�e�N���X [CLogoManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLogoManager::CLogoManager() :
	m_state(STATE_APPEAR),		// ���
	m_nStateCount(0)			// ��ԃJ�E���g
{
	memset(&m_apLogo[0], 0, sizeof(m_apLogo));
}

//============================================================
//	�f�X�g���N�^
//============================================================
CLogoManager::~CLogoManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CLogoManager::Init(void)
{
	// �ҋ@��Ԃɂ���
	m_state = STATE_WAIT;

	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{
		// ���S�}�l�[�W���[�̐���
		m_apLogo[nCnt] = CObject2D::Create(SCREEN_CENT, logo::SIZE[nCnt]);

		if (m_apLogo[nCnt] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`���̊��蓖��
		m_apLogo[nCnt]->BindTexture(logo::TEXTURE[nCnt]);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLogoManager::Uninit(void)
{
	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{
		// ���S�}�l�[�W���[�̔j��
		SAFE_UNINIT(m_apLogo[nCnt]);
	}
}

//============================================================
//	�X�V����
//============================================================
void CLogoManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{
	case CLogoManager::STATE_APPEAR:



		break;

	case CLogoManager::STATE_WAIT:

		m_nStateCount++;

		if (m_nStateCount >= 60)
		{
			m_state = STATE_DISAPPEAR;
		}

		break;

	case CLogoManager::STATE_DISAPPEAR:

		// �V�[���̐ݒ�
		GET_MANAGER->SetLoadScene(CScene::MODE_TITLE);	// �Z���N�g���

		break;

	default:
		assert(false);
		break;
	}
}

//============================================================
//	��������
//============================================================
CLogoManager* CLogoManager::Create(void)
{
	// �Q�[���}�l�[�W���[�̐���
	CLogoManager* pLogoManager = new CLogoManager;
	if (pLogoManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �Q�[���}�l�[�W���[�̏�����
		if (FAILED(pLogoManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �Q�[���}�l�[�W���[�̔j��
			SAFE_DELETE(pLogoManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pLogoManager;
	}
}

//============================================================
//	�j������
//============================================================
void CLogoManager::Release(CLogoManager*& prLogoManager)
{
	// �Q�[���}�l�[�W���[�̏I��
	assert(prLogoManager != nullptr);
	prLogoManager->Uninit();

	// �������J��
	SAFE_DELETE(prLogoManager);
}
