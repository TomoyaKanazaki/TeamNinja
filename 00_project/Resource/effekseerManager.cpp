//============================================================
//
//	�G�t�F�N�V�A�}�l�[�W���[���� [effekseerManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "effekseerManager.h"
#include "effekseerControl.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const CObject::ELabel	LABEL		= CObject::LABEL_NONE;	// �G�t�F�N�V�A�}�l�[�W���[�̃��x��
	const CObject::EDim		DIM			= CObject::DIM_3D;		// �G�t�F�N�V�A�}�l�[�W���[�̎���
	const int				PRIORITY	= 7;					// �G�t�F�N�V�A�}�l�[�W���[�̗D�揇��
}

//************************************************************
//	�q�N���X [CEffekseerManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEffekseerManager::CEffekseerManager() : CObject(LABEL, DIM, PRIORITY)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEffekseerManager::~CEffekseerManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEffekseerManager::Init(void)
{
	// �G�t�F�N�V�A�̏�����
	GET_EFFECT->Init();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEffekseerManager::Uninit(void)
{
	// �G�t�F�N�V�A�̏I��
	GET_EFFECT->Uninit();

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CEffekseerManager::Update(const float fDeltaTime)
{
	// �G�t�F�N�V�A�̍X�V
	GET_EFFECT->Update();
}

//============================================================
//	�`�揈��
//============================================================
void CEffekseerManager::Draw(CShader * /*pShader*/)
{
	// �G�t�F�N�V�A�̕`��
	GET_EFFECT->Draw();
}

//============================================================
//	��������
//============================================================
CEffekseerManager *CEffekseerManager::Create(void)
{
	// �G�t�F�N�V�A�}�l�[�W���[�̐���
	CEffekseerManager *pEffekseerManager = new CEffekseerManager;
	if (pEffekseerManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�t�F�N�V�A�}�l�[�W���[�̏�����
		if (FAILED(pEffekseerManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�t�F�N�V�A�}�l�[�W���[�̔j��
			SAFE_DELETE(pEffekseerManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pEffekseerManager;
	}
}
