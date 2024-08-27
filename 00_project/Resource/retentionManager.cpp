//============================================================
//
//	�f�[�^�ۑ��}�l�[�W���[���� [retentionManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "retentionManager.h"
#include "manager.h"
#include "stage.h"
#include "godItem.h"
#include "transpoint.h"

//************************************************************
//	�e�N���X [CRetentionManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRetentionManager::CRetentionManager()
{
	// �����o�ϐ����N���A
	memset(&m_result, 0, sizeof(m_result));	// ���U���g���
}

//============================================================
//	�f�X�g���N�^
//============================================================
CRetentionManager::~CRetentionManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CRetentionManager::Init(void)
{
	// �����o�ϐ���������
	m_result.win	= WIN_NONE;	// ������
	m_result.fTime = 0.0f;		// �o�߃^�C��
	m_result.nSave = -1;		// �Ō�̃Z�[�u�|�C���g

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CRetentionManager::Uninit(void)
{

}

//============================================================
//	��������
//============================================================
CRetentionManager *CRetentionManager::Create(void)
{
	// �f�[�^�ۑ��}�l�[�W���[�̐���
	CRetentionManager *pRetentionManager = new CRetentionManager;
	if (pRetentionManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �f�[�^�ۑ��}�l�[�W���[�̏�����
		if (FAILED(pRetentionManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �f�[�^�ۑ��}�l�[�W���[�̔j��
			SAFE_DELETE(pRetentionManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pRetentionManager;
	}
}

//============================================================
//	�j������
//============================================================
void CRetentionManager::Release(CRetentionManager *&prRetentionManager)
{
	// �f�[�^�ۑ��}�l�[�W���[�̏I��
	assert(prRetentionManager != nullptr);
	prRetentionManager->Uninit();

	// �������J��
	SAFE_DELETE(prRetentionManager);
}

//============================================================
//	���U���g���̐ݒ菈��
//============================================================
void CRetentionManager::SetResult(const EWin win, const float fTime, const int nSave)
{
	// ���������K�l�ł͂Ȃ��ꍇ������
	if (win <= WIN_NONE || win >= WIN_MAX) { assert(false); return; }

	// �����̃N���A�󋵂�ݒ�
	m_result.win = win;

	// �����̌o�߃^�C����ݒ�
	m_result.fTime = fTime;

	// �����̃Z�[�u�|�C���g��ݒ�
	m_result.nSave = nSave;

	if (win == WIN_SUCCESS)
	{ // �X�e�[�W�N���A�����ꍇ���ʂ̊l���󋵂��X�V

		// �O��܂ł̌��ʂ̊l���󋵂�ǂݍ���
		bool bOldGet[CGodItem::TYPE_MAX] = {};	// �ۑ�����Ă���l����
		CGodItem::LoadPossess(GET_STAGE->GetCurMapSaveGodItemPass().c_str(), &bOldGet[0]);

		// ����̌��ʂ̊l���󋵂�K��
		bool bCurGet[CGodItem::TYPE_MAX] = {};	// ����̊l����
		for (int i = 0; i < CGodItem::TYPE_MAX; i++)
		{
			// ����̊l���󋵂����蓖��
			bCurGet[i] = (CGodItem::IsGet((CGodItem::EType)i) || bOldGet[i]);
		}

		// ���ʂ̊l���󋵂������o��
		CGodItem::SavePossess(GET_STAGE->GetCurMapSaveGodItemPass().c_str(), &bCurGet[0]);

		// �J�ڐ�̉���t���O�e�L�X�g�p�X���쐬
		std::filesystem::path fsPath(GET_STAGE->GetOpenMapDirectory());	// �J�ڐ�}�b�v�p�X
		fsPath.append("open.txt");										// �f�B���N�g���ɉ���t���O�̃x�[�X�l�[���ǉ�

		// ����󋵂̏����o��
		CTransPoint::SaveOpen(fsPath.string().c_str(), true);
	}
}
