//============================================================
//
//	�Z���N�g�}�l�[�W���[���� [selectManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "selectManager.h"
#include "manager.h"
#include "camera.h"
#include "fade.h"
#include "scene.h"
#include "sceneSelect.h"
#include "player.h"

//************************************************************
//	�e�N���X [CSelectManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSelectManager::CSelectManager() :
	m_state	(STATE_NONE)	// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSelectManager::~CSelectManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSelectManager::Init(void)
{
	// �����o�ϐ���������
	m_state = STATE_NORMAL;	// ���

	// ��荞�݃J�����̐ݒ�
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_AROUND);
	GET_MANAGER->GetCamera()->SetDestAround();

	// TODO�F���u��
	// �v���C���[��ʏ��Ԃɂ���
	CPlayer::GetList()->GetList().front()->SetState(CPlayer::EState::STATE_NORMAL);
	CPlayer::GetList()->GetList().front()->SetAlpha(1.0f);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSelectManager::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CSelectManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:
	case STATE_NORMAL:
		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	��Ԑݒ菈��
//============================================================
void CSelectManager::SetState(const EState state)
{
	// ��Ԃ�ݒ�
	m_state = state;
}

//============================================================
//	��Ԏ擾����
//============================================================
CSelectManager::EState CSelectManager::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	��������
//============================================================
CSelectManager *CSelectManager::Create(void)
{
	// �Z���N�g�}�l�[�W���[�̐���
	CSelectManager *pSelectManager = new CSelectManager;
	if (pSelectManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �Z���N�g�}�l�[�W���[�̏�����
		if (FAILED(pSelectManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �Z���N�g�}�l�[�W���[�̔j��
			SAFE_DELETE(pSelectManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pSelectManager;
	}
}

//============================================================
//	�j������
//============================================================
void CSelectManager::Release(CSelectManager *&prSelectManager)
{
	// �Z���N�g�}�l�[�W���[�̏I��
	assert(prSelectManager != nullptr);
	prSelectManager->Uninit();

	// �������J��
	SAFE_DELETE(prSelectManager);
}
