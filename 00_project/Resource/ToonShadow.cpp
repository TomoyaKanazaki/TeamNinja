//============================================================
//
//	�g�D�[���V�F�[�_�[+�V���h�E�}�b�s���O [ToonShadow.cpp]
//	Author�F�O�엳�V��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "ToonShadow.h"
#include "ZTexture.h"
#include "manager.h"
#include "renderer.h"
#include "light.h"
#include "texture.h"
#include "camera.h"
//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* TEXTURE_FILE[] =	// �e�N�X�`���萔
	{
		"data\\TEXTURE\\SHADER\\toon004.png",	// �g�D�[���}�b�v�e�N�X�`��
	};

	const char* EFFECT_FX = "data\\SHADER\\Toonshadow.fx";	// �g�D�[���V�F�[�_�[�̃G�t�F�N�g�t�@�C��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CToonShadow::TEXTURE_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CToonShadow* CToonShadow::m_pShader = nullptr;	// �V�F�[�_�[���

//************************************************************
//	�e�N���X [CToonShadow] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CToonShadow::CToonShadow() :
	m_hTextureToon(nullptr),	// �g�D�[���}�b�v�e�N�X�`��
	m_hDirectLight(nullptr),	// ���s�����̕����x�N�g��
	m_hDiffuse(nullptr),	// �g�U��
	m_hAmbient(nullptr),	// ����
	m_hEmissive(nullptr),	// ���ˌ�
	m_hLightProjMat(nullptr),
	m_hLightViewMat(nullptr),
	m_hShadowMapTex(nullptr)

{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CToonShadow::~CToonShadow()
{

}

//============================================================
//	����������
//============================================================
HRESULT CToonShadow::Init(void)
{
	// �ϐ���錾
	D3DCAPS9 caps;	// �n�[�h�E�F�A�@�\
	HRESULT hr;		// �ُ�I���̊m�F�p

	// �|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���
	LPD3DXBUFFER pError = nullptr;	// �R���p�C���G���[���
	LPD3DXEFFECT pEffect = nullptr;	// �G�t�F�N�g�ݒ�p

	// �����o�ϐ���������
	m_hTextureToon = nullptr;	// �g�D�[���}�b�v�e�N�X�`��
	m_hDirectLight = nullptr;	// ���s�����̕����x�N�g��
	m_hDiffuse = nullptr;	// �g�U��
	m_hAmbient = nullptr;	// ����
	m_hEmissive = nullptr;	// ���ˌ�
	
	// �V�F�[�_�[�̏�����
	if (FAILED(CShader::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �n�[�h�E�F�A�@�\���擾
	pDevice->GetDeviceCaps(&caps);

	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1)
		&& caps.PixelShaderVersion >= D3DPS_VERSION(2, 0))
	{ // ���_�E�s�N�Z���V�F�[�_�̃o�[�W�������g�p�\�ȏꍇ

		// �G�t�F�N�g�t�@�C���̓Ǎ�
		hr = D3DXCreateEffectFromFile
		( // ����
			pDevice,	// �f�o�C�X�ւ̃|�C���^
			EFFECT_FX,	// �G�t�F�N�g�t�@�C��
			nullptr,	// �v���v���Z�b�T��`
			nullptr,	// �C���N���[�h����
			0,			// �Ǎ��I�v�V�����t���O
			nullptr,	// �n���h���C���^�[�t�F�[�X
			&pEffect,	// �G�t�F�N�g�C���^�[�t�F�[�X
			&pError		// �R���p�C���G���[���
		);
		if (SUCCEEDED(hr))
		{ // �Ǎ��ɐ��������ꍇ

			// �G�t�F�N�g����ݒ�
			SetEffect(pEffect);

			// �e�N�j�b�N�֐����擾�E�ݒ�
			SetTechnique(pEffect->GetTechniqueByName("TShader"));

			// �e�}�g���b�N�X�̃n���h�����擾�E�ݒ�
			SetMatrixWorld(pEffect->GetParameterByName(nullptr, "s_mtxWorld"));		// ���[���h�}�g���b�N�X
			SetMatrixView(pEffect->GetParameterByName(nullptr, "s_mtxView"));		// �r���[�}�g���b�N�X
			SetMatrixProjection(pEffect->GetParameterByName(nullptr, "s_mtxProj"));	// �v���W�F�N�V�����}�g���b�N�X

			// �n���h�����擾
			m_hTextureToon = pEffect->GetParameterByName(nullptr, "s_textureToon");	// �g�D�[���}�b�v�e�N�X�`��
			m_hDirectLight = pEffect->GetParameterByName(nullptr, "s_dirLight");		// ���s�����̕����x�N�g��
			m_hDiffuse = pEffect->GetParameterByName(nullptr, "s_diffuse");		// �g�U��
			m_hAmbient = pEffect->GetParameterByName(nullptr, "s_ambient");		// ����
			m_hEmissive = pEffect->GetParameterByName(nullptr, "s_emissive");		// ���ˌ�
			
			m_hLightProjMat = pEffect->GetParameterByName(nullptr, "s_mtxLightProj");		// �ˉe�ϊ��s��
			m_hLightViewMat = pEffect->GetParameterByName(nullptr, "s_mtxLightView");		// ���C�g�r���[�ϊ��s��
			m_hShadowMapTex = pEffect->GetParameterByName(nullptr, "s_texShadowMap");		// �V���h�E�}�b�v�e�N�X�`��
			m_hEyePos = pEffect->GetParameterByName(nullptr, "s_EyePos");					// ���_

			m_hFogColor = pEffect->GetParameterByName(nullptr, "s_FogColor");				// �t�H�O�F
			m_hFogNear = pEffect->GetParameterByName(nullptr, "s_FogNear");					// �t�H�O�J�n����
			m_hFogFar = pEffect->GetParameterByName(nullptr, "s_FogFar");					// �t�H�O�I������


			// �g�D�[���}�b�v�e�N�X�`����ݒ�
			SetToonMapTexture(TEXTURE_TOON);
		}
		else
		{ // �Ǎ��Ɏ��s�����ꍇ
			if (pError)
			{
				OutputDebugStringA((LPCSTR)pError->GetBufferPointer());
				//�f�o�b�O�R���\�[���ɕ\������
				MessageBoxA(NULL, (LPCSTR)pError->GetBufferPointer(), "Shader Error", MB_OK);
			}
			// �ǂݍ��ݎ��s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}
	else
	{ // �o�[�W�������g�p�s�ȏꍇ

		// �Â��o�[�W�����ɂ�鎸�s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CToonShadow::Uninit(void)
{
	// �V�F�[�_�[�̏I��
	CShader::Uninit();
}

//============================================================
//	�e�N�X�`���̐ݒ菈�� (�|�C���^)
//============================================================
void CToonShadow::SetTexture(const CTexture::STexture& rTexture)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �G�t�F�N�g�Ƀe�N�X�`���̎g�p�󋵂�ݒ�
	if (rTexture.pTexture == nullptr)
	{ // �e�N�X�`�����g�p����Ă��Ȃ��ꍇ

		
	}
	else
	{ // �e�N�X�`�����g�p����Ă���ꍇ


	}
}

//============================================================
//	�e�N�X�`���̐ݒ菈�� (�C���f�b�N�X)
//============================================================
void CToonShadow::SetTexture(const int nTextureID)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �G�t�F�N�g�Ƀe�N�X�`���̎g�p�󋵂�ݒ�
	if (nTextureID == NONE_IDX)
	{ // �e�N�X�`�����g�p����Ă��Ȃ��ꍇ

		
	}
	else if (nTextureID > NONE_IDX)
	{ // �e�N�X�`�����g�p����Ă���ꍇ

	
	}
	else { assert(false); }	// �g�p�s�C���f�b�N�X
}

//============================================================
//	�}�e���A���̐ݒ菈��
//============================================================
void CToonShadow::SetMaterial(const D3DMATERIAL9& rMaterial)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �|�C���^��錾
	LPD3DXEFFECT pEffect = GetEffect();	// �G�t�F�N�g���

	// �ϐ���錾
	D3DXVECTOR4 diffuse = D3DXVECTOR4(rMaterial.Diffuse.r, rMaterial.Diffuse.g, rMaterial.Diffuse.b, rMaterial.Diffuse.a);	// �g�U��
	D3DXVECTOR4 ambient = D3DXVECTOR4(rMaterial.Ambient.r, rMaterial.Ambient.g, rMaterial.Ambient.b, rMaterial.Ambient.a);	// ����
	D3DXVECTOR4 emissive = D3DXVECTOR4(rMaterial.Emissive.r, rMaterial.Emissive.g, rMaterial.Emissive.b, rMaterial.Emissive.a);	// ���ˌ�

	// �G�t�F�N�g�Ƀ}�e���A���̊g�U���E�����E���ˌ���ݒ�
	pEffect->SetVector(m_hDiffuse, &diffuse);
	pEffect->SetVector(m_hAmbient, &ambient);
	pEffect->SetVector(m_hEmissive, &emissive);
}

//============================================================
//	�g�U���̐ݒ菈��
//============================================================
void CToonShadow::SetDiffuse(const D3DXCOLOR& rDiffuse)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �ϐ���錾
	D3DXVECTOR4 diffuse = D3DXVECTOR4(rDiffuse.r, rDiffuse.g, rDiffuse.b, rDiffuse.a);	// �g�U��

	// �G�t�F�N�g�Ɋg�U����ݒ�
	GetEffect()->SetVector(m_hDiffuse, &diffuse);
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CToonShadow::SetAmbient(const D3DXCOLOR& rAmbient)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �ϐ���錾
	D3DXVECTOR4 ambient = D3DXVECTOR4(rAmbient.r, rAmbient.g, rAmbient.b, rAmbient.a);	// ����

	// �G�t�F�N�g�Ɋ�����ݒ�
	GetEffect()->SetVector(m_hAmbient, &ambient);
}

//============================================================
//	���ˌ��̐ݒ菈��
//============================================================
void CToonShadow::SetEmissive(const D3DXCOLOR& rEmissive)
{
	if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p

	// �ϐ���錾
	D3DXVECTOR4 emissive = D3DXVECTOR4(rEmissive.r, rEmissive.g, rEmissive.b, rEmissive.a);	// ���ˌ�

	// �G�t�F�N�g�ɕ��ˌ���ݒ�
	GetEffect()->SetVector(m_hEmissive, &emissive);
}

//============================================================
//	�g�U���̂ݐݒ菈��
//============================================================
void CToonShadow::SetOnlyDiffuse(const D3DXCOLOR& rDiffuse)
{
	// �g�U���̐ݒ�
	SetDiffuse(rDiffuse);

	// �����̐ݒ�
	SetAmbient(XCOL_ABLACK);

	// ���ˌ��̐ݒ�
	SetEmissive(XCOL_ABLACK);
}

//============================================================
//	�t�H�O�ݒ菈��
//============================================================
void CToonShadow::SetFog(const D3DXCOLOR& rColor, const float& rNear, const float& rFar)
{
	// �t�H�O�F�̐ݒ�
	D3DXVECTOR4 Color = D3DXVECTOR4(rColor.r, rColor.g, rColor.b, rColor.a);
	GetEffect()->SetVector(m_hFogColor, &Color);
	// �J�n����
	GetEffect()->SetFloat(m_hFogNear, rNear);
	//�I������
	GetEffect()->SetFloat(m_hFogFar, rFar);
}
//============================================================
//	�t�H�O�F�ݒ菈��
//============================================================
void CToonShadow::SetFogColor(const D3DXCOLOR& rColor)
{
	// �t�H�O�F�̐ݒ�
	D3DXVECTOR4 Color = D3DXVECTOR4(rColor.r, rColor.g, rColor.b, rColor.a);
	GetEffect()->SetVector(m_hFogColor, &Color);
}
//============================================================
//	�t�H�O�J�n�����ݒ菈��
//============================================================
void CToonShadow::SetFogNear(const float& rNear)
{
	// �J�n����
	GetEffect()->SetFloat(m_hFogNear, rNear);
}
//============================================================
//	�t�H�O�I�������ݒ菈��
//============================================================
void CToonShadow::SetFogFar(const float& rFar)
{
	// �J�n����
	GetEffect()->SetFloat(m_hFogFar, rFar);
}
//============================================================
//	�V���h�E�}�b�s���O�ݒ菈��
//============================================================
void CToonShadow::SetShadowMap()
{
	CZTexture* pZtex = CZTexture::GetInstance();
	D3DXVECTOR3 vec = FOCUS_POINT - VIEW_POINT;
	D3DXVec3Normalize(&vec, &vec);
	vec = useful::VectorToAngles(vec);
	D3DXVECTOR4 vec4 = {};
	vec4.x = vec.x;
	vec4.y = vec.y;
	vec4.z = vec.z;
	D3DXMATRIX mtxView, mtxProj;
	mtxView = pZtex->GetViewMtx();
	mtxProj = pZtex->GetProjMtx();
	GetEffect()->SetMatrix(m_hLightViewMat,&mtxView);
	GetEffect()->SetMatrix(m_hLightProjMat, &mtxProj);
	GetEffect()->SetVector(m_hDirectLight, &vec4);
	IDirect3DTexture9* pTexture;
	pZtex->GetZTex(&pTexture);
	GetEffect()->SetTexture(m_hShadowMapTex, pTexture);
	D3DXVECTOR3 camera = GET_MANAGER->GetCamera()->GetPositionV();
	D3DXVECTOR4 camera_vec4 = {};
	camera_vec4.x = camera.x;
	camera_vec4.y = camera.y;
	camera_vec4.z = camera.z;
	GetEffect()->SetVector(m_hEyePos, &camera_vec4);
}
//============================================================
//	��������
//============================================================
CToonShadow* CToonShadow::Create(void)
{
	// �C���X�^���X�g�p��
	assert(m_pShader == nullptr);

	// �g�D�[���V�F�[�_�[�̐���
	m_pShader = new CToonShadow;
	if (m_pShader == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �g�D�[���V�F�[�_�[�̏�����
		if (FAILED(m_pShader->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �g�D�[���V�F�[�_�[�̔j��
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
CToonShadow* CToonShadow::GetInstance(void)
{
	if (m_pShader == nullptr)
	{
		m_pShader = CToonShadow::Create();
	}

	// �|�C���^��Ԃ�
	return m_pShader;
}

//============================================================
//	�j������
//============================================================
void CToonShadow::Release(void)
{
	// �g�D�[���V�F�[�_�[�̏I��
	assert(m_pShader != nullptr);
	m_pShader->Uninit();

	// �������J��
	SAFE_DELETE(m_pShader);
}

//============================================================
//	�g�D�[���}�b�v�e�N�X�`���̐ݒ菈��
//============================================================
void CToonShadow::SetToonMapTexture(const ETexture texture)
{
	if (texture > NONE_IDX && texture < TEXTURE_MAX)
	{ // �C���f�b�N�X���͈͓��̏ꍇ

		if (!IsEffectOK()) { assert(false); return; }	// �G�t�F�N�g���g�p
		CTexture* pTexture = GET_MANAGER->GetTexture();	// �e�N�X�`�����

		// �G�t�F�N�g�Ƀg�D�[���}�b�v�p�e�N�X�`����ݒ�
		GetEffect()->SetTexture(m_hTextureToon, pTexture->GetPtr(pTexture->Regist(TEXTURE_FILE[texture])));
	}
	else { assert(false); }	// �͈͊O
}
