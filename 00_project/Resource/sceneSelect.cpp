//============================================================
//
//	�Z���N�g��ʏ��� [sceneSelect.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneSelect.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "stage.h"
#include "selectManager.h"
#include "editManager.h"
#include "retentionManager.h"
#include "godItem.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* INIT_MAPPASS = "data\\MAP\\SELECT00\\map.txt";	// �����}�b�v�p�X
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CSelectManager	*CSceneSelect::m_pSelectManager	= nullptr;	// �Z���N�g�}�l�[�W���[
CEditManager	*CSceneSelect::m_pEditManager	= nullptr;	// �G�f�B�b�g�}�l�[�W���[

//************************************************************
//	�q�N���X [CSceneSelect] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneSelect::CSceneSelect(const EMode mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneSelect::~CSceneSelect()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneSelect::Init(void)
{
	// �����}�b�v�p�X���X�e�[�W�I���}�b�v�ɕύX
	GET_STAGE->SetInitMapPass(INIT_MAPPASS);

	// �V�[���̏�����
	if (FAILED(CScene::Init()))	// �����蔻��/�v���C���[����
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �Z�[�u�|�C���g�̏�����
	GET_RETENTION->InitSave();

	// �_��擾�󋵂̏�����
	CGodItem::InitGet();

	// �Z���N�g�}�l�[�W���[�̐���
	m_pSelectManager = CSelectManager::Create();
	if (m_pSelectManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

#if _DEBUG

	// �G�f�B�b�g�}�l�[�W���[�̐���
	m_pEditManager = CEditManager::Create();
	if (m_pEditManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		return E_FAIL;
	}

#endif	// _DEBUG

	// BGM�̍Đ�
	PLAY_SOUND(CSound::LABEL_BGM_SELECT);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSceneSelect::Uninit(void)
{
	// �Z���N�g�}�l�[�W���[�̔j��
	SAFE_REF_RELEASE(m_pSelectManager);

#if _DEBUG

	// �G�f�B�b�g�}�l�[�W���[�̔j��
	SAFE_REF_RELEASE(m_pEditManager);

#endif	// _DEBUG

	// �V�[���̏I��
	CScene::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSceneSelect::Update(const float fDeltaTime)
{
	// �Z���N�g�}�l�[�W���[�̍X�V
	assert(m_pSelectManager != nullptr);
	m_pSelectManager->Update(fDeltaTime);

	// �V�[���̍X�V
	CScene::Update(fDeltaTime);

#ifdef _DEBUG

	// �G�f�B�b�g�}�l�[�W���[�̍X�V
	assert(m_pEditManager != nullptr);
	m_pEditManager->Update();

#endif	// _DEBUG
}

//============================================================
//	�Z���N�g�}�l�[�W���[�擾����
//============================================================
CSelectManager *CSceneSelect::GetSelectManager(void)
{
	// �C���X�^���X���g�p
	assert(m_pSelectManager != nullptr);

	// �Z���N�g�}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pSelectManager;
}

//============================================================
//	�G�f�B�^�[�}�l�[�W���[�擾����
//============================================================
CEditManager *CSceneSelect::GetEditManager(void)
{
	// �C���X�^���X���g�p
	assert(m_pEditManager != nullptr);

	// �G�f�B�^�[�}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pEditManager;
}
