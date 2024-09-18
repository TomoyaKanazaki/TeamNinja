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
#include "transpoint.h"
#include "rankingManager.h"

//************************************************************
//	�e�N���X [CSelectManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSelectManager::CSelectManager() :
	m_pRanking	(nullptr),		// �����L���O�}�l�[�W���[
	m_state		(STATE_NONE)	// ���
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
	m_pRanking	= nullptr;		// �����L���O�}�l�[�W���[
	m_state		= STATE_NORMAL;	// ���

	int nTransIdx = GET_RETENTION->GetTransIdx();	// �J�ڃ|�C���g�C���f�b�N�X
	if (nTransIdx != NONE_IDX)
	{ // �O��̑J�ڃ|�C���g���ݒ肳��Ă���ꍇ

		CTransPoint* pTransPoint = *CTransPoint::GetList()->GetIndex(nTransIdx);	// �J�ڃ|�C���g���
		CPlayer* pPlayer = GET_PLAYER;	// �v���C���[���
		CCamera* pCamera = GET_CAMERA;	// �J�������

		// �v���C���[�̈ʒu��J�ڃ|�C���g�ɕ␳
		pPlayer->SetVec3Position(pTransPoint->GetVec3Position());

		// �J�����ڕW�ʒu�̕␳
		pCamera->SetDestSelect();

		// �v���C���[�̌������J���������ɐݒ�
		D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, pCamera->GetDestRotation().y, 0.0f);	// �J��������
		pPlayer->SetVec3Rotation(rotCamera);
		pPlayer->SetDestRotation(rotCamera);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSelectManager::Uninit(void)
{
	// �����L���O�}�l�[�W���[�̔j��
	SAFE_REF_RELEASE(m_pRanking);
}

//============================================================
//	�X�V����
//============================================================
void CSelectManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:
		break;

	case STATE_NORMAL:

		if (m_pRanking != nullptr)
		{
			// �����L���O�}�l�[�W���[�̍X�V
			m_pRanking->Update(fDeltaTime);
			if (m_pRanking->IsEnd())
			{ // �����L���O�\�����I�������ꍇ

				// �����L���O�}�l�[�W���[�̔j��
				SAFE_REF_RELEASE(m_pRanking);
			}
		}
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
//	�����L���O�\���̐ݒ菈��
//============================================================
HRESULT CSelectManager::SetDispRanking(void)
{
	// �����L���O�������ς݂̏ꍇ������
	if (m_pRanking != nullptr) { assert(false); return E_FAIL; }

	// �����L���O�}�l�[�W���[�̐���
	m_pRanking = CRankingManager::Create();
	if (m_pRanking == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
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
