//========================================================================================
//
// �G�t�F�N�V�A�R���g���[���N���X
// Author: �O�� ���V��
//
//========================================================================================
//�C���N���[�h
#include "effekseerControl.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sceneGame.h"
#include "pause.h"
#include <codecvt>
#include <locale>
#include "hitStop.h"
#pragma warning(disable : 4996)
CEffekseer* CEffekseer::pInstance = NULL;
//======================================================
//�R���X�g���N�^
//======================================================
CEffekseer::CEffekseer()
{
}
//======================================================
//�f�X�g���N�^
//======================================================
CEffekseer::~CEffekseer()
{
}
//======================================================
//������
//======================================================
void CEffekseer::Init()
{
	//�G�t�F�N�V�A�n
	m_vEffect.clear();
	// �G�t�F�N�g�̃}�l�[�W���[�̍쐬
	m_EfkManager = ::Effekseer::Manager::Create(8000);

	// �`��f�o�C�X�̍쐬
	auto graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(CManager::GetInstance()->GetRenderer()->GetDevice());

	// Create a renderer of effects
	// �G�t�F�N�g�̃����_���[�̍쐬
	m_efkRenderer = ::EffekseerRendererDX9::Renderer::Create(graphicsDevice, 8000);

	// Sprcify rendering modules
	// �`�惂�W���[���̐ݒ�
	m_EfkManager->SetSpriteRenderer(m_efkRenderer->CreateSpriteRenderer());
	m_EfkManager->SetRibbonRenderer(m_efkRenderer->CreateRibbonRenderer());
	m_EfkManager->SetRingRenderer(m_efkRenderer->CreateRingRenderer());
	m_EfkManager->SetTrackRenderer(m_efkRenderer->CreateTrackRenderer());
	m_EfkManager->SetModelRenderer(m_efkRenderer->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	m_EfkManager->SetTextureLoader(m_efkRenderer->CreateTextureLoader());
	m_EfkManager->SetModelLoader(m_efkRenderer->CreateModelLoader());
	m_EfkManager->SetMaterialLoader(m_efkRenderer->CreateMaterialLoader());
	m_EfkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
	// ���W�n��ݒ肷��B�A�v���P�[�V�����ƈ�v������K�v������B
	m_EfkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

}
//======================================================
//�I��
//======================================================
void CEffekseer::Uninit()
{
	m_EfkManager->StopAllEffects();
	m_vEffect.shrink_to_fit();
	int nSize = (int)m_vEffect.size();
	for (int i = 0; i < nSize; i++)
	{
		if (m_vEffect[i]->m_bAutoDelete && m_vEffect[i] != NULL)
		{
			delete m_vEffect[i];
			m_vEffect[i] = NULL;
		}
	}
	m_vEffect.shrink_to_fit();
	//onLostDevice();
	delete pInstance;
	pInstance = NULL;
}
//======================================================
//�G�t�F�N�g�S�폜
//======================================================
void CEffekseer::AllClear()
{
	m_EfkManager->StopAllEffects();
	m_vEffect.shrink_to_fit();
	int nSize = (int)m_vEffect.size();
	for (int i = 0; i < nSize; i++)
	{
		if (m_vEffect[i]->m_bAutoDelete && m_vEffect[i] != NULL)
		{
			delete m_vEffect[i];
			m_vEffect[i] = NULL;
		
		}
	}
	for (int i = 0; i < nSize; i++)
	{
		if (m_vEffect[i] == NULL)
		{
			m_vEffect[i]->Erase();
		}
	}
}
//======================================================
//�X�V
//======================================================
void CEffekseer::Update()
{
	time++;

	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (GET_MANAGER->GetScene()->GetMode() == CScene::MODE_GAME)
	{ // ���[�h���Q�[���̏ꍇ

		if (CSceneGame::GetPause()->IsPause()
			|| CSceneGame::GetHitStop()->IsStop())
		{ // �|�[�Y���E�q�b�g�X�g�b�v���̏ꍇ
			return;
		}
	}

	m_vEffect.shrink_to_fit();
	for (int i = 0; i < (int)m_vEffect.size(); i++)
	{
		Effekseer::Handle loacalhandle = m_vEffect[i]->handle;

		if (!m_EfkManager->Exists(loacalhandle))
		{
			m_EfkManager->StopEffect(loacalhandle);

			// �폜
			if (!m_vEffect[i]->m_bLoop)
			{
				if (m_vEffect[i]->m_bAutoDelete)
				{
					delete m_vEffect[i];
					m_vEffect[i]->Erase();
				}
			}
			else
			{

				// �V���ɃG�t�F�N�g���Đ��������B���W�̓G�t�F�N�g��\���������ꏊ��ݒ肷��
				// (�ʒu���]�A�g��k�����ݒ肵�Ȃ����K�v������)
				m_vEffect[i]->handle = m_EfkManager->Play(m_vEffect[i]->efcRef, 0.0f, 0.0f, 0.0f);

				m_EfkManager->SetLocation(m_vEffect[i]->handle, m_vEffect[i]->m_pos.X, m_vEffect[i]->m_pos.Y, m_vEffect[i]->m_pos.Z);
				m_EfkManager->SetRotation(m_vEffect[i]->handle, m_vEffect[i]->m_rot.X, m_vEffect[i]->m_rot.Y, m_vEffect[i]->m_rot.Z);
				m_EfkManager->SetScale(m_vEffect[i]->handle, m_vEffect[i]->m_fScale, m_vEffect[i]->m_fScale, m_vEffect[i]->m_fScale);
			}
		}
		else
		{// �Đ���
			m_vEffect[i]->m_pos += m_vEffect[i]->m_move;
			m_EfkManager->SetLocation(m_vEffect[i]->handle, m_vEffect[i]->m_pos.X, m_vEffect[i]->m_pos.Y, m_vEffect[i]->m_pos.Z);
			m_EfkManager->SetRotation(m_vEffect[i]->handle, m_vEffect[i]->m_rot.X, m_vEffect[i]->m_rot.Y, m_vEffect[i]->m_rot.Z);
			m_EfkManager->SetScale(m_vEffect[i]->handle, m_vEffect[i]->m_fScale, m_vEffect[i]->m_fScale, m_vEffect[i]->m_fScale);
		}

	}
	// ���C���[�p�����[�^�̐ݒ�
	Effekseer::Manager::LayerParameter layerParameter;

	if (pCamera != NULL)
	{
		::Effekseer::Matrix44 cameraMatrix;
		cameraMatrix = pCamera->GetCamera(CCamera::EType::TYPE_MAIN).mtxView;
		::Effekseer::Matrix44 invEfkCameraMatrix;
		::Effekseer::Matrix44::Inverse(invEfkCameraMatrix, cameraMatrix);
		layerParameter.ViewerPosition = ::Effekseer::Vector3D(invEfkCameraMatrix.Values[3][0], invEfkCameraMatrix.Values[3][1], invEfkCameraMatrix.Values[3][2]);
	}
	m_EfkManager->SetLayerParameter(0, layerParameter);

	// �}�l�[�W���[�̍X�V
	Effekseer::Manager::UpdateParameter updateParameter;
	m_EfkManager->Update(updateParameter);
}
//======================================================
//�`��
//======================================================
void CEffekseer::Draw()
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	m_efkRenderer->SetTime(time / 60.0f);
	
	// ���Ԃ��X�V����
	if (pCamera != NULL)
	{
		// �G�t�F�N�g�̕`��J�n�������s���B
		m_efkRenderer->BeginRendering();
		// Specify a projection matrix
		// ���e�s���ݒ�
		::Effekseer::Matrix44 projectionMatrix; 
		D3DXMATRIX mtx = pCamera->GetCamera(CCamera::EType::TYPE_MAIN).mtxView;
		//D3DXMatrixInverse(&mtx, NULL, &mtx);

		projectionMatrix = pCamera->GetCamera(CCamera::EType::TYPE_MAIN).mtxProjection;
		m_efkRenderer->SetProjectionMatrix(projectionMatrix);
		
		::Effekseer::Matrix44 cameraMatrix; cameraMatrix = mtx;
		
		// Specify a camera matrix
		// �J�����s���ݒ�
	
		m_efkRenderer->SetCameraMatrix(cameraMatrix);
		// �G�t�F�N�g�̕`����s���B
		Effekseer::Manager::DrawParameter drawParameter;
		drawParameter.ZNear = 0.0f;
		drawParameter.ZFar = 1.0f;
		drawParameter.ViewProjectionMatrix = m_efkRenderer->GetCameraProjectionMatrix();
		m_EfkManager->Draw(drawParameter);

		// �G�t�F�N�g�̕`��I������
		m_efkRenderer->EndRendering();
	}
	
}
//======================================================
//�G�t�F�N�g����
//======================================================
CEffekseer::CEffectData* CEffekseer::Create(std::string path, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, float fScale, bool bLoop, bool bAutoDelete)
{
	CEffectData* pEffect = new CEffectData;
	pEffect->efcRef = Loading(path);
	pEffect->Path = path;
	// �G�t�F�N�g�̍Đ�
	pEffect->handle = m_EfkManager->Play(pEffect->efcRef, pos.x, pos.y, pos.z);
	pEffect->m_pos = pos;
	pEffect->m_rot = rot;
	pEffect->m_move = move;
	pEffect->m_fScale = fScale;
	pEffect->m_bLoop = bLoop;
	pEffect->m_bAutoDelete = bAutoDelete;
	return pEffect;
}
//======================================================
//�ǂݍ���
//======================================================
Effekseer::EffectRef CEffekseer::Loading(std::string path)
{
	// char16_t�ɕϊ�
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	std::u16string string16t = converter.from_bytes(path);

	// �G�t�F�N�g�̓Ǎ�
	auto effect = Effekseer::Effect::Create(m_EfkManager, string16t.c_str());


	//onLostDevice = [effect]() -> void
	//{
	//	// �ǂݍ��񂾃G�t�F�N�g�̃��\�[�X�͑S�Ĕj������B
	//	if (effect != nullptr)
	//	{
	//		effect->UnloadResources();
	//	}
	//};

	//onResetDevice = [effect]() -> void
	//{
	//	// �G�t�F�N�g�̃��\�[�X���ēǂݍ��݂���B
	//	if (effect != nullptr)
	//	{
	//		effect->ReloadResources();
	//	}
	//};

	return effect;
}
//======================================================
//�G�t�F�N�g���R���X�g���N�^
//======================================================
CEffekseer::CEffectData::CEffectData()
{
	handle = NULL;
	GetInstance()->m_vEffect.push_back(this);
}
//======================================================
//�G�t�F�N�g���f�X�g���N�^
//======================================================
CEffekseer::CEffectData::~CEffectData()
{
	if (CEffekseer::GetInstance()->GetManager()->Exists(handle))
	{
		CEffekseer::GetInstance()->GetManager()->StopEffect(handle);
	}
	if (!m_bAutoDelete)
	{
		Erase();
	}
}
//======================================================
//�z�񂩂玩�g���폜
//======================================================	
void CEffekseer::CEffectData::Erase()
{
	GetInstance()->m_vEffect.erase(std::find(GetInstance()->m_vEffect.begin(), GetInstance()->m_vEffect.end(), this));
}
//======================================================
//�G�t�F�N�g�C���X�^���X�̑��݊m�F
//======================================================	
bool CEffekseer::CEffectData::GetExist()
{
	return CEffekseer::GetInstance()->GetManager()->Exists(handle);
}