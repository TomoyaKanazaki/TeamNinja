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

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float APPEAR_ADD_ALPHA = 0.04f;		// �o����Ԃ̓����x�̉��Z��
	const float DISAPPEAR_SUB_ALPHA = 0.05f;	// ���ŏ�Ԃ̓����x�̌��Z��
	const int WAIT_COUNT = 120;					// �ҋ@��Ԃ̃J�E���g��

	namespace logo
	{
		const D3DXVECTOR3 SIZE[CLogoManager::POLYGON_MAX] =		// �T�C�Y
		{
			D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),		// �w�i
			D3DXVECTOR3(520.0f, 520.0f, 0.0f),					// ���S
		};
		const D3DXCOLOR COL[CLogoManager::POLYGON_MAX] =		// �F
		{
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),		// �w�i
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)		// ���S
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
	m_nStateCount(0),			// ��ԃJ�E���g
	m_bDisappear(false)			// ���ŏ�
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
	for (int nCnt = 0; nCnt < POLYGON_MAX; nCnt++)
	{
		// ���S�}�l�[�W���[�̐���
		m_apLogo[nCnt] = CObject2D::Create(SCREEN_CENT, logo::SIZE[nCnt], VEC3_ZERO, logo::COL[nCnt]);

		if (m_apLogo[nCnt] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`���̊��蓖��
		m_apLogo[nCnt]->BindTexture(logo::TEXTURE[nCnt]);
	}

	// �o����Ԃɂ���
	m_state = STATE_APPEAR;

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

	// ���g������
	delete this;
}

//============================================================
//	�X�V����
//============================================================
void CLogoManager::Update(const float fDeltaTime)
{
	// �X�L�b�v����
	UpdateSkip();

	switch (m_state)
	{
	case CLogoManager::STATE_APPEAR:

		// �o����ԏ���
		UpdateAppear();

		break;

	case CLogoManager::STATE_WAIT:

		// �ҋ@��ԏ���
		UpdateWait();

		break;

	case CLogoManager::STATE_DISAPPEAR:

		// ���ŏ���
		UpdateDisappear();

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

//============================================================
// �o����ԏ���
//============================================================
void CLogoManager::UpdateAppear(void)
{
	// �����x���擾
	float fAlpha = m_apLogo[POLYGON_LOGO]->GetAlpha();

	// �����x�����Z����
	fAlpha += APPEAR_ADD_ALPHA;

	if (fAlpha >= 1.0f)
	{ // �����x��1.0f�ȏ�̏ꍇ

		// �����x��␳����
		fAlpha = 1.0f;

		// �ҋ@��Ԃɂ���
		m_state = STATE_WAIT;
	}

	// �����x��ݒ�
	m_apLogo[POLYGON_LOGO]->SetAlpha(fAlpha);
}

//============================================================
// �ҋ@��ԏ���
//============================================================
void CLogoManager::UpdateWait(void)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	// ��ԃJ�E���g����萔�����̏ꍇ�A�֐��𔲂���
	if (m_nStateCount < WAIT_COUNT) { return; }

	// ��ԃJ�E���g��0�ɂ���
	m_nStateCount = 0;

	// ���ŏ�Ԃɂ���
	m_state = STATE_DISAPPEAR;
}

//============================================================
// ���ŏ�ԏ���
//============================================================
void CLogoManager::UpdateDisappear(void)
{
	// �����x��0�ɐݒ肷��
	float fAlpha = m_apLogo[POLYGON_LOGO]->GetAlpha();

	if (fAlpha >= 0.0f)
	{ // �����x��0.0f�ȏ�̏ꍇ

		// �����x�����Z������
		fAlpha -= DISAPPEAR_SUB_ALPHA;
		m_apLogo[POLYGON_LOGO]->SetAlpha(fAlpha);

		// �֐��𔲂���
		return;
	}

	// �����x���擾
	fAlpha = m_apLogo[POLYGON_BACK]->GetAlpha();

	// �����x�����Z����
	fAlpha -= DISAPPEAR_SUB_ALPHA;

	if (fAlpha <= 0.0f)
	{ // �����x��0.0f�ȉ��̏ꍇ

		// �����x��␳����
		fAlpha = 0.0f;

		// ���ŏ󋵂� true �ɂ���
		m_bDisappear = true;
	}

	// �����x��ݒ�
	m_apLogo[POLYGON_BACK]->SetAlpha(fAlpha);
}

//============================================================
// �X�L�b�v����X�V
//============================================================
void CLogoManager::UpdateSkip(void)
{
	CInputKeyboard* pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputPad* pPad = GET_INPUTPAD;	// �p�b�h���

	if (pKey->IsTrigger(DIK_SPACE)
		|| pKey->IsTrigger(DIK_RETURN)
		|| pPad->IsTrigger(CInputPad::KEY_A)
		|| pPad->IsTrigger(CInputPad::KEY_B)
		|| pPad->IsTrigger(CInputPad::KEY_X)
		|| pPad->IsTrigger(CInputPad::KEY_Y))
	{
		// ���ŏ�Ԃɂ���
		m_state = STATE_DISAPPEAR;
	}
}
