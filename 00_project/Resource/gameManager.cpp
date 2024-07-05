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
#include "rankingManager.h"
#include "retentionManager.h"
#include "camera.h"
#include "player.h"
#include "multiModel.h"

#include "enemy.h"
#include "checkpoint.h"
#include "popupUI.h"
#include "goal.h"

#include "gimmick.h"
#include "gimmick_malti.h"
#include "actor.h"
#include "MapModel.h"
#include "camera_change.h"


//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 POS_NAME	 = D3DXVECTOR3(0.0f, 60.0f, 400.0f);	// ���O�̕\���ʒu
	const D3DXVECTOR3 POS_SKIP	 = D3DXVECTOR3(1092.0f, 673.0f, 0.0f);	// �X�L�b�v����̕\���ʒu
	const D3DXVECTOR3 SIZE_SKIP	 = D3DXVECTOR3(381.0f, 77.0f, 0.0f);	// �X�L�b�v����̕\���傫��
	const int CHANGE_UI_PRIORITY = 5;		// �V�l�}�X�R�[�v�I������UI�D�揇��
	const float GAMEEND_WAITTIME = 2.0f;	// ���U���g��ʂւ̑J�ڗ]�C�t���[��

	const char* MAP_TXT = "data\\TXT\\map.txt"; // �}�b�v���̃p�X
	const char* START_TEXTURE = "data\\TEXTURE\\start.png";		// �J�n�̃e�N�X�`��

#ifdef _DEBUG
	bool bCamera = false;
#endif
}

//************************************************************
//	�e�N���X [CGameManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGameManager::CGameManager() :
	m_state	(STATE_NONE)	// ���
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

	// �X�^�[�gUI�𐶐�
	CPopUpUI::Create(START_TEXTURE);

#if 0
	CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, 400.0f), VEC3_ZERO, CEnemy::TYPE_STALK);
	CEnemy::Create(D3DXVECTOR3(-600.0f, 0.0f, -500.0f), VEC3_ZERO, CEnemy::TYPE_CAVEAT);
	CEnemy::Create(D3DXVECTOR3(600.0f, 0.0f, 400.0f), VEC3_ZERO, CEnemy::TYPE_WOLF);

	CGimmick::Create(D3DXVECTOR3(400.0f, 0.0f, -1000.0f), D3DXVECTOR3(300.0f, 0.0f, 100.0f), CGimmick::TYPE_JUMPTABLE, 2);
	CGimmick::Create(D3DXVECTOR3(800.0f, 0.0f, -1300.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f), CGimmick::TYPE_JUMPTABLE, 2);
	CGimmick::Create(D3DXVECTOR3(-400.0f, 0.0f, -500.0f), D3DXVECTOR3(200.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 2);

	CGimmick::Create(D3DXVECTOR3(-1000.0f, 0.0f, -500.0f), D3DXVECTOR3(100.0f, 0.0f, 50.0f), CGimmick::TYPE_JUMPOFF, 2);
	CGimmick::Create(D3DXVECTOR3(-1400.0f, 0.0f, -300.0f), D3DXVECTOR3(400.0f, 0.0f, 100.0f), CGimmick::TYPE_HEAVYDOOR, 6);

	CMapModel::Create(D3DXVECTOR3(-800.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE1);

	CMapModel::Create(D3DXVECTOR3(-400.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE2);

	CMapModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE3);

	CMapModel::Create(D3DXVECTOR3(400.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE4);

	CMapModel::Create(D3DXVECTOR3(1200.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE5);

	CMapModel::Create(D3DXVECTOR3(1500.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE6);

	CMapModel::Create(D3DXVECTOR3(1800.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE7);

	CMapModel::Create(D3DXVECTOR3(2100.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE8);

	CMapModel::Create(D3DXVECTOR3(2400.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE9);

	CMapModel::Create(D3DXVECTOR3(2700.0f, 0.0f, 0.0f), VEC3_ZERO, CMapModel::MODEL_TYPE_HOUSE10);
#endif

// �X�}�b�v�p���M�~�b�N�u����
#if 1

	// �W�����v��
	CGimmick::Create(D3DXVECTOR3(3375.0f, 300.0f, -150.0f), D3DXVECTOR3(50.0f, 0.0f, 1300.0f), CGimmick::TYPE_JUMPTABLE, 2);
	CGimmick::Create(D3DXVECTOR3(3800.0f, 300.0f, 475.0f), D3DXVECTOR3(800.0f, 0.0f, 50.0f), CGimmick::TYPE_JUMPTABLE, 2);
	CGimmick::Create(D3DXVECTOR3(4925.0f, 700.0f, 75.0f), D3DXVECTOR3(50.0f, 0.0f, 750.0f), CGimmick::TYPE_JUMPTABLE, 4);
	
	// ��O
	/*CGimmick::Create(D3DXVECTOR3(8325.0f, 100.0f, -600.0f), D3DXVECTOR3(50.0f, 0.0f, 250.0f), CGimmick::TYPE_JUMPTABLE, 5);
	CGimmick::Create(D3DXVECTOR3(9625.0f, 500.0f, -600.0f), D3DXVECTOR3(50.0f, 0.0f, 250.0f), CGimmick::TYPE_JUMPTABLE, 2);
	CGimmick::Create(D3DXVECTOR3(10175.0f, 300.0f, -600.0f), D3DXVECTOR3(50.0f, 0.0f, 250.0f), CGimmick::TYPE_JUMPTABLE, 2);*/


	//CGimmick::Create(D3DXVECTOR3(9175.0f, 250.0f, 125.0f), D3DXVECTOR3(200.0f, 0.0f, 50.0f), CGimmick::TYPE_JUMPTABLE, 3);
	/*CGimmick::Create(D3DXVECTOR3(9175.0f, 100.0f, -150.0f), D3DXVECTOR3(200.0f, 0.0f, 50.0f), CGimmick::TYPE_JUMPTABLE, 3);
	CGimmick::Create(D3DXVECTOR3(9175.0f, 100.0f, -450.0f), D3DXVECTOR3(200.0f, 0.0f, 50.0f), CGimmick::TYPE_JUMPTABLE, 5);*/
	//CGimmick::Create(D3DXVECTOR3(10000.0f, 100.0f, 525.0f), D3DXVECTOR3(50.0f, 0.0f, 550.0f), CGimmick::TYPE_JUMPTABLE, 4);


	// ��э~��
	/*CGimmick::Create(D3DXVECTOR3(5850.0f, 300.0f, 75.0f), D3DXVECTOR3(700.0f, 0.0f, 750.0f), CGimmick::TYPE_JUMPOFF, 3);
	CGimmick::Create(D3DXVECTOR3(5225.0f, 300.0f, -550.0f), D3DXVECTOR3(550.0f, 0.0f, 500.0f), CGimmick::TYPE_JUMPOFF, 3);
	CGimmick::Create(D3DXVECTOR3(6575.0f, 0.0f, 0.0f), D3DXVECTOR3(350.0f, 0.0f, 1600.0f), CGimmick::TYPE_JUMPOFF, 2);
	
	CGimmick::Create(D3DXVECTOR3(9800.0f, 100.0f, 725.0f), D3DXVECTOR3(250.0f, 0.0f, 150.0f), CGimmick::TYPE_JUMPOFF, 3);
	CGimmick::Create(D3DXVECTOR3(9975.0f, 300.0f, -600.0f), D3DXVECTOR3(150.0f, 0.0f, 250.0f), CGimmick::TYPE_JUMPOFF, 3);
	CGimmick::Create(D3DXVECTOR3(10525.0f, 0.0f, -600.0f), D3DXVECTOR3(250.0f, 0.0f, 250.0f), CGimmick::TYPE_JUMPOFF, 3);*/


	// ���ݑ�
	CGimmick::Create(D3DXVECTOR3(1725.0f, 0.0f, -350.0f), D3DXVECTOR3(50.0f, 0.0f, 900.0f), CGimmick::TYPE_STEP, 4);
	CGimmick::Create(D3DXVECTOR3(2575, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 0.0f, 1600.0f), CGimmick::TYPE_STEP, 4);
	CGimmick::Create(D3DXVECTOR3(4575.0f, 300.0f, 450.0f), D3DXVECTOR3(750.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 4);
	
	CGimmick::Create(D3DXVECTOR3(5225.0f, 1400.0f, 75.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f), CGimmick::TYPE_STEP, 6);
	CGimmick::Create(D3DXVECTOR3(4575.0f, 400.0f, -325.0f), D3DXVECTOR3(750.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 4);
	CGimmick::Create(D3DXVECTOR3(4175.0f, 400.0f, 75.0f), D3DXVECTOR3(50.0f, 0.0f, 750.0f), CGimmick::TYPE_STEP, 4);

	// ��O��
	//CGimmick::Create(D3DXVECTOR3(7175.0f, 0.0f, -575.0f), D3DXVECTOR3(50.0f, 0.0f, 450.0f), CGimmick::TYPE_STEP, 1);


	CGimmick::Create(D3DXVECTOR3(7175.0f, 0.0f, 475.0f), D3DXVECTOR3(50.0f, 0.0f, 650.0f), CGimmick::TYPE_STEP, 4);
	//CGimmick::Create(D3DXVECTOR3(9175.0f, 250.0f, 425.0f), D3DXVECTOR3(200.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 4);
	//CGimmick::Create(D3DXVECTOR3(9350.0f, 500.0f, 725.0f), D3DXVECTOR3(50.0f, 0.0f, 150.0f), CGimmick::TYPE_STEP, 2);


	// ��
	/*CGimmick::Create(D3DXVECTOR3(7650.0f, 100.0f, -575.0f), D3DXVECTOR3(500.0f, 0.0f, 450.0f), CGimmick::TYPE_BRIDGE, 4);*/
	CGimmick::Create(D3DXVECTOR3(8675.0f, 500.0f, 625.0f), D3DXVECTOR3(550.0f, 0.0f, 350.0f), CGimmick::TYPE_BRIDGE, 4);
	/*CGimmick::Create(D3DXVECTOR3(8700.0f, 100.0f, -325.0f), D3DXVECTOR3(800.0f, 0.0f, 250.0f), CGimmick::TYPE_BRIDGE, 6);
	CGimmick::Create(D3DXVECTOR3(8975.0f, 500.0f, -600.0f), D3DXVECTOR3(300.0f, 0.0f, 250.0f), CGimmick::TYPE_BRIDGE, 2);*/

	

#endif

// �X�}�b�v�p���M�~�b�N�u����A
#if 0

	// �W�����v��
	CGimmick::Create(D3DXVECTOR3(3375.0f, 300.0f, -150.0f), D3DXVECTOR3(50.0f, 0.0f, 1300.0f), CGimmick::TYPE_JUMPTABLE, 2);

	// ���ݑ�
	CGimmick::Create(D3DXVECTOR3(1725.0f, 0.0f, -350.0f), D3DXVECTOR3(50.0f, 0.0f, 900.0f), CGimmick::TYPE_STEP, 4);

	// ��
	CGimmick::Create(D3DXVECTOR3(7650.0f, 100.0f, -575.0f), D3DXVECTOR3(500.0f, 0.0f, 450.0f), CGimmick::TYPE_BRIDGE, 4);

#endif

// ����p�M�~�b�N�u����
#if 1
	// ��
	CGimmick::Create(D3DXVECTOR3(2775.0f, 300.0f, -350.0f), D3DXVECTOR3(650.0f, 0.0f, 300.0f), CGimmick::TYPE_BRIDGE, 4);
#endif

// �M�~�b�N�u����
#if 1
	// ��q�F��O
	CGimmick::Create(D3DXVECTOR3(-1450.0f, 1.0f, 450.0f), D3DXVECTOR3(200.0f, 0.0f, 700.0f), CGimmick::TYPE_STEP, 2);
	CGimmick::Create(D3DXVECTOR3(-950.0f, 151.0f, 450.0f), D3DXVECTOR3(200.0f, 0.0f, 700.0f), CGimmick::TYPE_STEP, 6);

	// �W�����v��F��O
	CGimmick::Create(D3DXVECTOR3(-100.0f, 1.0f, -50.0f), D3DXVECTOR3(300.0f, 0.0f, 300.0f), CGimmick::TYPE_JUMPTABLE, 4);

	// ���F����
	CGimmick::Create(D3DXVECTOR3(600.0f, 600.0f, 450.0f), D3DXVECTOR3(1000.0f, 0.0f, 300.0f), CGimmick::TYPE_BRIDGE, 6);

	// �d�h�A�F����
	CGimmick::Create(D3DXVECTOR3(600.0f, 1.0f, 100.0f), D3DXVECTOR3(400.0f, 0.0f, 100.0f), CGimmick::TYPE_HEAVYDOOR, 4);

	// ���ݑ�F�ŉ�
	CGimmick::Create(D3DXVECTOR3(2990.0f, 1.0f, 0.0f), D3DXVECTOR3(10.0f, 0.0f, 1600.0f), CGimmick::TYPE_STEP, 4);

	{ // �����{�^���F�ŉ�
		std::vector<CGimmickMalti::SButton> vec;
		vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(9900.0f, 0.0f, -2250.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
		vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(10550.0f, 0.0f, -3200.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
		vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(10750.0f, 0.0f, -2350.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
		CGimmickMalti::Create(vec);
	}
#endif

	// ����񂨁[
#if 0
	CEnemy::Create(D3DXVECTOR3(0.0f, 2000.0f, 300.0f), VEC3_ZERO, CEnemy::TYPE_WOLF);
	CEnemy::Create(D3DXVECTOR3(0.0f, 2000.0f, -300.0f), VEC3_ZERO, CEnemy::TYPE_WOLF);
#endif

	// ���ނ炢
#if 1
	CEnemy::Create(D3DXVECTOR3(300.0f, 0.0f, 400.0f), VEC3_ZERO, CEnemy::TYPE_STALK);
	CEnemy::Create(D3DXVECTOR3(700.0f, 0.0f, -60.0f), VEC3_ZERO, CEnemy::TYPE_STALK);
#endif

	// ��荞�݃J�����̐ݒ�
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_AROUND);
	GET_MANAGER->GetCamera()->SetDestAround();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGameManager::Uninit(void)
{

}

//============================================================
//	�X�V����
//============================================================
void CGameManager::Update(const float fDeltaTime)
{
#ifdef _DEBUG // �J�����؂�ւ�
	if (GET_INPUTKEY->IsTrigger(DIK_RSHIFT))
	{
		// ��Ԃ�؂�ւ�
		bCamera = !bCamera;

		if (bCamera)
		{
			GET_MANAGER->GetCamera()->SetState(CCamera::STATE_TPS);
			GET_MANAGER->GetCamera()->SetDestTps();
		}
		else
		{
			GET_MANAGER->GetCamera()->SetState(CCamera::STATE_TELEPHOTO);
			GET_MANAGER->GetCamera()->SetDestTelephoto();
		}
	}
#endif

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
		if (CGoal::GetGoal() != nullptr)
		{
			if (CGoal::GetGoal()->GetClear())
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
	// �t�F�[�h���̏ꍇ������
	if (GET_MANAGER->GetFade()->IsFade()) { return; }

	// �^�C�}�[�̌v���I��
	CSceneGame::GetTimerUI()->End();

	// ���U���g����ۑ�
	GET_RETENTION->SetResult(win, CSceneGame::GetTimerUI()->GetTime());

	// ���U���g��ʂɑJ��
	GET_MANAGER->SetLoadScene(CScene::MODE_RESULT, GAMEEND_WAITTIME);

	if (win == CRetentionManager::WIN_CLEAR)
	{ // �������Ă����ꍇ

		// �����L���O�ɐݒ�
		CRankingManager::Set(CSceneGame::GetTimerUI()->GetTime());
	}
	else
	{

	}
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
