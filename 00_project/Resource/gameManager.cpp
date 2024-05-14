//============================================================
//
//	�Q�[���}�l�[�W���[���� [gameManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gameManager.h"
#include "manager.h"
#include "fade.h"
#include "scene.h"
#include "sceneGame.h"
#include "cinemaScope.h"
#include "timerUI.h"
#include "retentionManager.h"
#include "camera.h"
#include "player.h"
#include "multiModel.h"

#include "enemy.h"
#include "checkpoint.h"
#include "goal.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 POS_NAME	 = D3DXVECTOR3(0.0f, 60.0f, 400.0f);	// ���O�̕\���ʒu
	const D3DXVECTOR3 POS_SKIP	 = D3DXVECTOR3(1092.0f, 673.0f, 0.0f);	// �X�L�b�v����̕\���ʒu
	const D3DXVECTOR3 SIZE_SKIP	 = D3DXVECTOR3(381.0f, 77.0f, 0.0f);	// �X�L�b�v����̕\���傫��
	const int CHANGE_UI_PRIORITY = 5;	// �V�l�}�X�R�[�v�I������UI�D�揇��
	const int GAMEEND_WAIT_FRAME = 0;	// ���U���g��ʂւ̑J�ڗ]�C�t���[��

	const char* MAP_TXT = "data\\TXT\\map.txt"; // �}�b�v���̃p�X
}

//************************************************************
//	�e�N���X [CGameManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGameManager::CGameManager() :
	m_state	(STATE_NONE),	// ���
	m_pGoal	(nullptr)		// �S�[���̃|�C���^
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGameManager::~CGameManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGameManager::Init(void)
{
	// �����o�ϐ���������
	m_state = STATE_NORMAL;	// ���
	m_pGoal = nullptr;		// �S�[���̃|�C���^

	CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, 400.0f), VEC3_ZERO, CEnemy::TYPE_CHASE);

	// �}�b�v�𐶐�
	if (FAILED(MapLoad())) { return E_FAIL; }

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGameManager::Uninit(void)
{
	SAFE_UNINIT(m_pGoal);
}

//============================================================
//	�X�V����
//============================================================
void CGameManager::Update(const float fDeltaTime)
{
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:
	case STATE_NORMAL:
		
		// �m�C�͂�0�̏ꍇ���U���g��
		if (GET_PLAYER->GetTension() == 0) 
		{
			TransitionResult(CRetentionManager::EWin::WIN_FAILED);
		}

		// �S�[�����Ă����ꍇ���U���g
		if (m_pGoal != nullptr)
		{
			if (m_pGoal->GetClear())
			{
				TransitionResult(CRetentionManager::EWin::WIN_CLEAR);
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
void CGameManager::SetState(const EState state)
{
	// ��Ԃ�ݒ�
	m_state = state;
}

//============================================================
//	��Ԏ擾����
//============================================================
CGameManager::EState CGameManager::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	���U���g��ʑJ�ڏ���
//============================================================
void CGameManager::TransitionResult(const CRetentionManager::EWin win)
{
	// �^�C�}�[�̌v���I��
	CSceneGame::GetTimerUI()->End();

	// ���U���g����ۑ�
	// TODO�FGetTime���C��
#if 0
	GET_RETENTION->SetResult(win, CSceneGame::GetTimerUI()->GetTime());
#else
	GET_RETENTION->SetResult(win, 0);
#endif

	// ���U���g��ʂɑJ��
	GET_MANAGER->SetScene(CScene::MODE_RESULT, GAMEEND_WAIT_FRAME);
}

//============================================================
//	��������
//============================================================
CGameManager *CGameManager::Create(void)
{
	// �Q�[���}�l�[�W���[�̐���
	CGameManager *pGameManager = new CGameManager;
	if (pGameManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �Q�[���}�l�[�W���[�̏�����
		if (FAILED(pGameManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �Q�[���}�l�[�W���[�̔j��
			SAFE_DELETE(pGameManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pGameManager;
	}
}

//============================================================
//	�j������
//============================================================
void CGameManager::Release(CGameManager *&prGameManager)
{
	// �Q�[���}�l�[�W���[�̏I��
	assert(prGameManager != nullptr);
	prGameManager->Uninit();

	// �������J��
	SAFE_DELETE(prGameManager);
}

//==========================================
//  �}�b�v�̐���
//==========================================
HRESULT CGameManager::MapLoad()
{
	//���[�J���ϐ��錾
	FILE* pFile; // �t�@�C���|�C���^

	//�t�@�C����ǂݎ���p�ŊJ��
	pFile = fopen(MAP_TXT, "r");

	// �t�@�C�����J���Ȃ������ꍇ
	if (pFile == NULL) { assert(false); return E_FAIL; }

	// ���̓ǂݍ���
	while (1)
	{
		// ������̋L�^�p
		char aStr[256];

		// ������ǂݍ���
		fscanf(pFile, "%s", &aStr[0]);

		// ��������
		if (strcmp(&aStr[0], "CHECKPOINT") == 0) // �`�F�b�N�|�C���g�̐���
		{
			// �f�[�^�̎擾�p�ϐ�
			D3DXVECTOR3 pos, rot;

			// ������ǂݍ��� (POS)
			fscanf(pFile, "%s", &aStr[0]);

			// �f�[�^�擾
			fscanf(pFile, "%f", &pos.x);
			fscanf(pFile, "%f", &pos.y);
			fscanf(pFile, "%f", &pos.z);

			// ������ǂݍ��� (ROT)
			fscanf(pFile, "%s", &aStr[0]);

			// �f�[�^�擾
			fscanf(pFile, "%f", &rot.x);
			fscanf(pFile, "%f", &rot.y);
			fscanf(pFile, "%f", &rot.z);

			// �`�F�b�N�|�C���g�𐶐�
			CCheckPoint::Create(pos, rot);
		}
		if (strcmp(&aStr[0], "GOAL") == 0) // �`�F�b�N�|�C���g�̐���
		{
			// �f�[�^�̎擾�p�ϐ�
			D3DXVECTOR3 pos, rot;

			// ������ǂݍ��� (POS)
			fscanf(pFile, "%s", &aStr[0]);

			// �f�[�^�擾
			fscanf(pFile, "%f", &pos.x);
			fscanf(pFile, "%f", &pos.y);
			fscanf(pFile, "%f", &pos.z);

			// ������ǂݍ��� (ROT)
			fscanf(pFile, "%s", &aStr[0]);

			// �f�[�^�擾
			fscanf(pFile, "%f", &rot.x);
			fscanf(pFile, "%f", &rot.y);
			fscanf(pFile, "%f", &rot.z);

			// �`�F�b�N�|�C���g�𐶐�
			m_pGoal = CGoal::Create(pos, rot);
		}
		if (strcmp(&aStr[0], "END_OF_FILE") == 0) // �ǂݍ��ݏI��
		{
			break;
		}
	}

	return S_OK;
}
