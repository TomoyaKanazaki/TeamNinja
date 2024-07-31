//============================================================
//
//	�����_�[�e�N�X�`������ [renderTexture.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "renderTexture.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�q�N���X [CRenderTexture] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRenderTexture::CRenderTexture(const CObject::EScene scene) :
	m_pSurTexture	(nullptr),	// �e�N�X�`���T�[�t�F�C�X�ւ̃|�C���^
	m_nTextureID	(0),		// �����_�[�e�N�X�`���C���f�b�N�X
	m_scene			(scene)		// �`��V�[��
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CRenderTexture::~CRenderTexture()
{

}

//============================================================
//	����������
//============================================================
HRESULT CRenderTexture::Init(void)
{
	CTexture *pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	const D3DFORMAT format = (m_scene == CObject::SCENE_MAIN) ? D3DFMT_X8R8G8B8 : D3DFMT_A8R8G8B8;	// �s�N�Z���t�H�[�}�b�g

	// �����o�ϐ���������
	m_pSurTexture	= nullptr;	// �e�N�X�`���T�[�t�F�C�X�ւ̃|�C���^
	m_nTextureID	= 0;		// �����_�[�e�N�X�`���C���f�b�N�X

	// ��̃X�N���[���e�N�X�`���𐶐�
	m_nTextureID = pTexture->Regist(CTexture::SInfo
	( // ����
		SCREEN_WIDTH,			// �e�N�X�`������
		SCREEN_HEIGHT,			// �e�N�X�`���c��
		0,						// �~�b�v�}�b�v���x��
		D3DUSAGE_RENDERTARGET,	// �����E�m�ۃI�v�V����
		format,					// �s�N�Z���t�H�[�}�b�g
		D3DPOOL_DEFAULT			// �i�[������
	));

	// �X�N���[���`��T�[�t�F�C�X�̎擾
	HRESULT hr = pTexture->GetPtr(m_nTextureID)->GetSurfaceLevel
	( // ����
		0,				// �~�b�v�}�b�v���x��
		&m_pSurTexture	// �e�N�X�`���T�[�t�F�C�X�ւ̃|�C���^
	);
	if (FAILED(hr))
	{ // �T�[�t�F�C�X�擾�Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CRenderTexture::Uninit(void)
{
	// �e�N�X�`���T�[�t�F�C�X�̔j��
	SAFE_RELEASE(m_pSurTexture);
}

//============================================================
//	�`�揈��
//============================================================
void CRenderTexture::Draw(void)
{
	CRenderer *pRenderer = GET_RENDERER;	// �����_���[�ւ̃|�C���^

	// �����_�[�e�N�X�`���ւ̏�������
	pRenderer->DrawRenderTexture(m_scene, &m_pSurTexture);
}

//============================================================
//	��������
//============================================================
CRenderTexture *CRenderTexture::Create(const CObject::EScene scene)
{
	// �����_�[�e�N�X�`���̐���
	CRenderTexture *pRenderTexture = new CRenderTexture(scene);
	if (pRenderTexture == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �����_�[�e�N�X�`���̏�����
		if (FAILED(pRenderTexture->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �����_�[�e�N�X�`���̔j��
			SAFE_DELETE(pRenderTexture);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pRenderTexture;
	}
}

//============================================================
//	�j������
//============================================================
void CRenderTexture::Release(CRenderTexture *&prRenderTexture)
{
	// �����_�[�e�N�X�`���̏I��
	assert(prRenderTexture != nullptr);
	prRenderTexture->Uninit();

	// �������J��
	SAFE_DELETE(prRenderTexture);
}
