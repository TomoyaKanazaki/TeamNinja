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
#include "hitstop.h"
#include "resultManager.h"
#include "retentionManager.h"
#include "camera.h"
#include "player.h"
#include "player_clone.h"
#include "enemy.h"

#include "enemyStalk.h"
#include "enemyWolf.h"
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
	const char* MAP_TXT			= "data\\TXT\\map.txt";			// �}�b�v���̃p�X
	const char* START_TEXTURE	= "data\\TEXTURE\\start.png";	// �J�n�̃e�N�X�`��
	const char* END_TEXTURE[] =	// �I���̃e�N�X�`��
	{
		nullptr,					// �e�N�X�`������
		"data\\TEXTURE\\end.png",	// �s�k�̃e�N�X�`��
		"data\\TEXTURE\\end.png",	// �����̃e�N�X�`��
	};

	const CCamera::SSwing CLEAR_SWING = CCamera::SSwing(18.0f, 2.2f, 0.35f);	// ���U���g�J�ڎ��̃J�����h��
	const int HITSTOP_TIME = 75;	// �q�b�g�X�g�b�v�t���[��

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
	m_pResult	(nullptr),		// ���U���g�}�l�[�W���[
	m_state		(STATE_NONE)	// ���
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
	m_pResult	= nullptr;		// ���U���g�}�l�[�W���[
	m_state		= STATE_NORMAL;	// ���

	// ���U���g�}�l�[�W���[�̐���
	m_pResult = CResultManager::Create();
	if (m_pResult == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�^�[�gUI�𐶐�
	CPopUpUI::Create(START_TEXTURE);

// �X00�}�b�v�p�M�~�b�N�u����
#if 0

	

	// �W�����v��
	//CGimmick::Create(D3DXVECTOR3(8050.0f, 0.0f, 400.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 800.0f), CGimmick::TYPE_JUMPTABLE, 3);



	// ��
	//CGimmick::Create(D3DXVECTOR3(14400.0f, 500.0f, -250.0f), EAngle::ANGLE_90, D3DXVECTOR3(650.0f, 0.0f, 1100.0f), CGimmick::TYPE_BRIDGE, 5);


	//=================================================
	// [�{�^���̂�]
	//=================================================
	// �{�^��
	//{ // �����{�^���F�R��
	//	std::vector<CGimmickMalti::SButton> vec;
	//	vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(17900.0f, 1200.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
	//	vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(17275.0f, 1200.0f, 500.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
	//	vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(18475.0f, 1200.0f, 500.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
	//  vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(17275.0f, 1200.0f, -500.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
	//	vec.push_back(CGimmickMalti::SButton(D3DXVECTOR3(18475.0f, 1200.0f, -500.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f)));
	// 
	//	CGimmickMalti::Create(vec);
	//}
#endif

// �X02�}�b�v�p�M�~�b�N�u����
#if 0

	//=================================================
	// [�����̂��]
	//=================================================
	// �d�h�A�F����
	CGimmick::Create(D3DXVECTOR3(600.0f, 1.0f, 100.0f), EAngle::ANGLE_0, D3DXVECTOR3(400.0f, 0.0f, 100.0f), CGimmick::TYPE_HEAVYDOOR, 4);

	// �W�����v��F��
	CGimmick::Create(D3DXVECTOR3(-575.0f, 150.0f, 425.0f), EAngle::ANGLE_0, D3DXVECTOR3(650.0f, 0.0f, 750.0f), CGimmick::TYPE_JUMPTABLE, 4);
	CGimmick::Create(D3DXVECTOR3(4574.0f, 300.0f, 450.0f), EAngle::ANGLE_0, D3DXVECTOR3(750.0f, 0.0f, 50.0f), CGimmick::TYPE_JUMPTABLE, 3);


	// �W�����v��F��O
	CGimmick::Create(D3DXVECTOR3(4950.0f, 700.0f, 75.0f), EAngle::ANGLE_0, D3DXVECTOR3(50.0f, 0.0f, 750.0f), CGimmick::TYPE_JUMPTABLE, 4);


	// ���ݑ�(��q)�F��
	CGimmick::Create(D3DXVECTOR3(-1350.0f, 0.0f, 225.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 1150.0f), CGimmick::TYPE_STEP, 1);
	CGimmick::Create(D3DXVECTOR3(3000.0f, 500.0f, 450.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 700.0f), CGimmick::TYPE_STEP, 1);
	CGimmick::Create(D3DXVECTOR3(3800.0f, 300.0f, 450.0f), EAngle::ANGLE_90, D3DXVECTOR3(800.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 3);
	CGimmick::Create(D3DXVECTOR3(6400.0f, 0.0f, 0.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 1600.0f), CGimmick::TYPE_STEP, 4);


	// ���ݑ�(��q)�F��O
	CGimmick::Create(D3DXVECTOR3(1750.0f, 0.0f, -350.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 900.0f), CGimmick::TYPE_STEP, 4);
	CGimmick::Create(D3DXVECTOR3(2550.0f, 0.0f, -350.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 900.0f), CGimmick::TYPE_STEP, 4);
	CGimmick::Create(D3DXVECTOR3(2150.0f, 300.0f, 100.0f), EAngle::ANGLE_90, D3DXVECTOR3(800.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 4);
	CGimmick::Create(D3DXVECTOR3(3000.0f, 0.0f, -350.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 900.0f), CGimmick::TYPE_STEP, 4);
	CGimmick::Create(D3DXVECTOR3(3175.0f, 300.0f, 100.0f), EAngle::ANGLE_90, D3DXVECTOR3(350.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 3);
	CGimmick::Create(D3DXVECTOR3(3400.0f, 300.0f, -350.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 900.0f), CGimmick::TYPE_STEP, 1);
	CGimmick::Create(D3DXVECTOR3(3775.0f, 400.0f, 100.0f), EAngle::ANGLE_90, D3DXVECTOR3(750.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 1);
	CGimmick::Create(D3DXVECTOR3(4200.0f, 500.0f, 275.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 300.0f), CGimmick::TYPE_STEP, 3);
	CGimmick::Create(D3DXVECTOR3(4550.0f, 400.0f, -125.0f), EAngle::ANGLE_90, D3DXVECTOR3(800.0f, 0.0f, 400.0f), CGimmick::TYPE_STEP, 4);
	CGimmick::Create(D3DXVECTOR3(5300.0f, 1400.0f, 75.0f), EAngle::ANGLE_90, D3DXVECTOR3(200.0f, 0.0f, 200.0f), CGimmick::TYPE_STEP, 6);

	// ���F��
	CGimmick::Create(D3DXVECTOR3(600.0f, 600.0f, 450.0f), EAngle::ANGLE_90, D3DXVECTOR3(850.0f, 0.0f, 700.0f), CGimmick::TYPE_BRIDGE, 5);


	// ���F��O
	CGimmick::Create(D3DXVECTOR3(2775.0f, 300.0f, -350.0f), EAngle::ANGLE_90, D3DXVECTOR3(500.0f, 0.0f, 900.0f), CGimmick::TYPE_BRIDGE, 3);

	//=================================================
	// [����]
	//=================================================
	// �W�����v��F��O
	CGimmick::Create(D3DXVECTOR3(8225.0f, 100.0f, -575.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 450.0f), CGimmick::TYPE_JUMPTABLE, 3);
	CGimmick::Create(D3DXVECTOR3(9100.0f, 350.0f, -550.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 400.0f), CGimmick::TYPE_JUMPTABLE, 3);
	CGimmick::Create(D3DXVECTOR3(8850.0f, 350.0f, -375.0f), EAngle::ANGLE_90, D3DXVECTOR3(350.0f, 0.0f, 50.0f), CGimmick::TYPE_JUMPTABLE, 3);

	// �W�����v��F��
	CGimmick::Create(D3DXVECTOR3(7200.0f, 0.0f, 475.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 650.0f), CGimmick::TYPE_JUMPTABLE, 4);


	// ���ݑ�(��q)�F��O
	CGimmick::Create(D3DXVECTOR3(7200.0f, 0.0f, -575.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 450.0f), CGimmick::TYPE_STEP, 1);

	// ���ݑ�(��q)�F��
	CGimmick::Create(D3DXVECTOR3(9350.0f, 500.0f, 625.0f), EAngle::ANGLE_90, D3DXVECTOR3(50.0f, 0.0f, 350.0f), CGimmick::TYPE_STEP, 2);
	CGimmick::Create(D3DXVECTOR3(9675.0f, 650.0f, 100.0f), EAngle::ANGLE_90, D3DXVECTOR3(300.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 1);
	CGimmick::Create(D3DXVECTOR3(10750.0f, 125.0f, 350.0f), EAngle::ANGLE_90, D3DXVECTOR3(400.0f, 0.0f, 50.0f), CGimmick::TYPE_STEP, 4);


	// ���F��O
	CGimmick::Create(D3DXVECTOR3(9875.0f, 125.0f, 50.0f), EAngle::ANGLE_90, D3DXVECTOR3(400.0f, 0.0f, 450.0f), CGimmick::TYPE_BRIDGE, 3);

	// ���F��
	CGimmick::Create(D3DXVECTOR3(8700.0f, 500.0f, 625.0f), EAngle::ANGLE_90, D3DXVECTOR3(550.0f, 0.0f, 350.0f), CGimmick::TYPE_BRIDGE, 4);
	CGimmick::Create(D3DXVECTOR3(10550.0f, 725.0f, -550.0f), EAngle::ANGLE_90, D3DXVECTOR3(300.0f, 0.0f, 400.0f), CGimmick::TYPE_BRIDGE, 3);
	CGimmick::Create(D3DXVECTOR3(12075.0f, 550.0f, -200.0f), EAngle::ANGLE_0, D3DXVECTOR3(400.0f, 0.0f, 350.0f), CGimmick::TYPE_BRIDGE, 3);
#endif

// ����񂨁[
#if 0
	CEnemyWolf::Create(D3DXVECTOR3(0.0f, 2000.0f, 300.0f), VEC3_ZERO, CEnemyAttack::TYPE_WOLF, 400.0f, 400.0f, 600.0, 500.0f);
	CEnemyWolf::Create(D3DXVECTOR3(0.0f, 2000.0f, -300.0f), VEC3_ZERO, CEnemyAttack::TYPE_WOLF, 400.0f, 400.0f, 600.0, 500.0f);
#endif

// ���ނ炢
#if 1
	//CEnemyStalk::Create(D3DXVECTOR3(300.0f, 0.0f, 400.0f), VEC3_ZERO, CEnemyAttack::TYPE_STALK, 400.0f, 400.0f, 600.0, 500.0f);
	CEnemyStalk::Create(D3DXVECTOR3(700.0f, 0.0f, -60.0f), VEC3_ZERO, CEnemyAttack::TYPE_STALK, 400.0f, 400.0f, 600.0, 500.0f);

	// ����ɒǉ����Ă��߂����(�`���[�g���A���}�b�v�̓G)
	CEnemyStalk::Create(D3DXVECTOR3(12950.0f, 650.0f, 100.0f), VEC3_ZERO, CEnemyAttack::TYPE_STALK, 400.0f, 400.0f, 600.0, 500.0f);

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
	// ���U���g�}�l�[�W���[�̔j��
	SAFE_REF_RELEASE(m_pResult);
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
			GET_MANAGER->GetCamera()->SetState(CCamera::STATE_AROUND);
			GET_MANAGER->GetCamera()->SetDestAround();
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

		// �S�[�����Ă����ꍇ���U���g
		if (CGoal::GetGoal() != nullptr)
		{
			if (CGoal::GetGoal()->GetClear())
			{
				TransitionResult(CRetentionManager::EWin::WIN_CLEAR);
			}
		}
		break;

	case STATE_RESULT:

		if (!CSceneGame::GetHitStop()->IsStop())
		{ // �q�b�g�X�g�b�v���I������ꍇ

			// ���U���g�}�l�[�W���[�̍X�V
			m_pResult->Update(fDeltaTime);
		}
		break;

	default:	// ��O����
		assert(false);
		break;
	}
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

	// �J�����h��̐ݒ�
	GET_MANAGER->GetCamera()->SetSwing(CCamera::TYPE_MAIN, CLEAR_SWING);

	// ���U���g���̕ۑ�
	GET_RETENTION->SetResult(win, CSceneGame::GetTimerUI()->GetTime());

	// �L�����N�^�[������S�ď��ł�����
	CPlayerClone::VanishAll();	// ���g
	CEnemy::VanishAll();		// �G

	// �q�b�g�X�g�b�v�I�����ɌĂ΂��֐����쐬
	auto funcEndHitStop = []
	{
		// �v���C���[�����U���g��Ԃɂ���
		GET_PLAYER->SetResult();

		// �J���������U���g��Ԃɂ���
		GET_MANAGER->GetCamera()->SetState(CCamera::STATE_RESULT);
	};

	// �q�b�g�X�g�b�v�̐ݒ�
	CSceneGame::GetHitStop()->SetStop(HITSTOP_TIME, funcEndHitStop);

	// ���U���g��Ԃɂ���
	m_state = STATE_RESULT;
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
