//============================================================
//
//	Z�e�N�X�`������ [ZTexture.cpp]
//	Author�F�O�엳�V��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "ZTexture.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "texture.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CZTexture* CZTexture::m_pShader = nullptr;	// �V�F�[�_�[���

// �R���X�g���N�^
CZTexture::CZTexture()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	m_hWorldMat = NULL;
	m_hViewMat = NULL;
	m_hProjMat = NULL;
	m_hTechnique = NULL;
}

// �f�X�g���N�^
CZTexture::~CZTexture()
{
	// COM�|�C���^�𖾎��I�ɏ���
	m_cpDepthBuff = NULL;	// �[�x�o�b�t�@
	m_cpZTexSurf = NULL;	// Z�l�e�N�X�`���T�[�t�F�C�X
	m_cpZTex = NULL;		// Z�l�e�N�X�`��
	m_cpDevBuffer = NULL;	// �f�o�C�X�o�b�N�o�b�t�@
	m_cpDevDepth = NULL;	// �f�o�C�X�[�x�o�b�t�@
	m_cpEffect = NULL;		// Z�l�v���b�g�G�t�F�N�g
}


// ���������\�b�h
bool CZTexture::Init()
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���
	if (pDevice == NULL) return false;
	if constexpr (ZTEX_WIDTH == 0 || ZTEX_HEIGHT == 0) return false;

	HRESULT hr;
	ID3DXBuffer* pError = NULL;
	// Z�l�v���b�g�V�F�[�_�v���O������ǂݍ���
	if (FAILED(D3DXCreateEffectFromFile(
		pDevice,
		("data\\SHADER\\ZTexCreator.fx"),
		NULL,
		NULL,
		0,
		NULL,
		&m_cpEffect,
		&pError)))
		if (pError)
		{
			OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
			//�f�o�b�O�R���\�[���ɕ\������
			MessageBoxA(NULL, (LPCSTR)pError->GetBufferPointer(), "Shader Error", MB_OK);
			return false;
		}
	

	// �G�t�F�N�g���̊e��p�����[�^�n���h�����擾
	m_hWorldMat = m_cpEffect->GetParameterByName(NULL, "matWorld");
	m_hViewMat = m_cpEffect->GetParameterByName(NULL, "matView");
	m_hProjMat = m_cpEffect->GetParameterByName(NULL, "matProj");
	m_hTechnique = m_cpEffect->GetTechniqueByName("ZValuePlotTec");
	if (!m_hWorldMat || !m_hViewMat || !m_hProjMat || !m_hTechnique)
		return false;


	// �w���Z�l�e�N�X�`���𐶐�
	hr = D3DXCreateTexture(
		pDevice,
		ZTEX_WIDTH, ZTEX_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A32B32G32R32F,
		D3DPOOL_DEFAULT,
		&m_cpZTex);
	if (FAILED(hr))
		return false;

	m_cpZTex->GetSurfaceLevel(0, &m_cpZTexSurf);	// �T�[�t�F�C�X�擾

	// �`��f�o�C�X�ɒ�`����Ă���o�b�t�@�̔\�͂��擾
	IDirect3DSurface9 *pSurf;
	pDevice->GetDepthStencilSurface(&pSurf);
	D3DSURFACE_DESC Desc;
	pSurf->GetDesc(&Desc);
	pSurf->Release();

	// �Ǝ��[�x�o�b�t�@���쐬
	hr = pDevice->CreateDepthStencilSurface(
		ZTEX_WIDTH, ZTEX_HEIGHT,
		Desc.Format,
		Desc.MultiSampleType,
		Desc.MultiSampleQuality,
		FALSE,
		&m_cpDepthBuff,
		NULL
	);
	if (FAILED(hr))
		return false;

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(VIEWING_ANGLE), (ZTEX_WIDTH / ZTEX_HEIGHT), NEAR_CLIP, FAR_CLIP);
	D3DXMatrixLookAtLH(&m_matView, &VIEW_POINT, &FOCUS_POINT, &LOOK_UP);

	D3DXCreateSprite(pDevice, &m_pSprite);// �X�v���C�g�쐬

	m_bBegin = false;
	return true;
}


// �`��ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s���ݒ�
void CZTexture::SetWorldMatrix(D3DXMATRIX *pMat)
{
	m_matWorld = *pMat;
}


// �r���[�s���ݒ�
void CZTexture::SetViewMatrix(D3DXMATRIX *pMat)
{
	m_matView = *pMat;
}


// �ˉe�ϊ��s���ݒ�
void CZTexture::SetProjMatrix(D3DXMATRIX *pMat)
{
	m_matProj = *pMat;
}


// �`��̊J�n��錾����
HRESULT CZTexture::Begin()
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���
	// ���������������s���Ă��邩�`�F�b�N
	if (m_cpDepthBuff == NULL || m_cpZTex == NULL)
		return E_FAIL;

	// �f�o�C�X�������Ă���o�b�t�@���ꎞ�ۑ�
	pDevice->GetRenderTarget(0, &m_cpDevBuffer);
	pDevice->GetDepthStencilSurface(&m_cpDevDepth);

	// �f�o�C�X��Z�l�e�N�X�`���T�[�t�F�C�X�Ɛ[�x�o�b�t�@��ݒ�
	pDevice->SetRenderTarget(0, m_cpZTexSurf);
	pDevice->SetDepthStencilSurface(m_cpDepthBuff);

	// �e�T�[�t�F�C�X��������
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);

	// �v���O���}�u���V�F�[�_�̃e�N�j�b�N��ݒ�
	m_cpEffect->SetTechnique(m_hTechnique);

	// �V�F�[�_�̊J�n��錾
	UINT Tmp;
	m_cpEffect->Begin(&Tmp, 0);
	m_bBegin = true;
	return S_OK;
}


// �p�X�̊J�n��錾����
HRESULT CZTexture::BeginPass()
{
	m_cpEffect->BeginPass(0);	// Z�l�v�Z��1�p�X
	return S_OK;
}


// �o�^����Ă���p�����[�^�����G�t�F�N�g�ɃZ�b�g
bool CZTexture::SetParamToEffect()
{
	if (m_cpEffect == NULL) return false;

	m_cpEffect->SetMatrix(m_hWorldMat, &m_matWorld);
	m_cpEffect->SetMatrix(m_hViewMat, &m_matView);
	m_cpEffect->SetMatrix(m_hProjMat, &m_matProj);

	return true;
}


// �p�X�̏I����錾����
HRESULT CZTexture::EndPass()
{
	m_cpEffect->EndPass();
	return S_OK;
}


// �`��̏I����錾����
HRESULT CZTexture::End()
{
	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���
	m_cpEffect->End();

	// �f�o�C�X�Ɍ��̃T�[�t�F�C�X��߂�
	pDevice->SetRenderTarget(0, m_cpDevBuffer);
	pDevice->SetDepthStencilSurface(m_cpDevDepth);

	m_cpDevBuffer = NULL;
	m_cpDevDepth = NULL;

	// �Œ�@�\�ɖ߂�
	pDevice->SetVertexShader(NULL);
	pDevice->SetPixelShader(NULL);
	m_bBegin = false;
	return S_OK;
}


// Z�l�e�N�X�`�����擾����
bool CZTexture::GetZTex(IDirect3DTexture9 **cpTex)
{
	*cpTex = m_cpZTex;
	return true;
}

//============================================================
//	��������
//============================================================
CZTexture* CZTexture::Create(void)
{
	// �C���X�^���X�g�p��
	assert(m_pShader == nullptr);

	// �V�F�[�_�[�̐���
	m_pShader = new CZTexture;
	if (m_pShader == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ������
		if (!m_pShader->Init())
		{ // �������Ɏ��s�����ꍇ

			// �j��
			SAFE_DELETE(m_pShader);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return m_pShader;
	}
}

//============================================================
//	�擾����
//============================================================
CZTexture* CZTexture::GetInstance(void)
{
	if (m_pShader == nullptr)
	{
		m_pShader = CZTexture::Create();
	}

	// �|�C���^��Ԃ�
	return m_pShader;
}

//============================================================
//	�j������
//============================================================
void CZTexture::Release(void)
{
	// �I��
	assert(m_pShader != nullptr);
	// �������J��
	SAFE_DELETE(m_pShader);
}
//============================================================
//	�X�v���C�g�`�揈��
//============================================================
void CZTexture::DrawSprite()
{
	//�X�P�[�����O
	D3DXMATRIX SpriteScaleMat;
	D3DXMatrixScaling(&SpriteScaleMat, 0.25f, 0.25f, 1.0f);
	m_pSprite->SetTransform(&SpriteScaleMat);
	//�`��
	m_pSprite->Begin(0);
	m_pSprite->Draw(m_cpZTex, NULL, NULL, NULL, 0xffffffff);
	m_pSprite->End();
}