//============================================================
//
//	�N���A�}�l�[�W���[���� [clearManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "clearManager.h"
#include "clearFailManager.h"
#include "clearSuccessManager.h"

//************************************************************
//	�e�N���X [CClearManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CClearManager::CClearManager()
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CClearManager::~CClearManager()
{

}

//============================================================
//	��������
//============================================================
CClearManager *CClearManager::Create(const CRetentionManager::EWin win)
{
	// �N���A�}�l�[�W���[�̐���
	CClearManager *pClearManager = nullptr;	// �N���A�}�l�[�W���[���
	switch (win)
	{ // ���[�h���Ƃ̏���
	case CRetentionManager::WIN_FAIL:
		pClearManager = new CClearFailManager;
		break;

	case CRetentionManager::WIN_SUCCESS:
		pClearManager = new CClearSuccessManager;
		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (pClearManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �N���A�}�l�[�W���[�̏�����
		if (FAILED(pClearManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���A�}�l�[�W���[�̔j��
			SAFE_DELETE(pClearManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pClearManager;
	}
}

//============================================================
//	�j������
//============================================================
void CClearManager::Release(CClearManager *&prClearManager)
{
	// �N���A�}�l�[�W���[�̏I��
	assert(prClearManager != nullptr);
	prClearManager->Uninit();

	// �������J��
	SAFE_DELETE(prClearManager);
}
