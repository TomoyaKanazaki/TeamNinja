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
#include "godItemManager.h"
#include "resultManager.h"
#include "retentionManager.h"
#include "camera.h"
#include "player.h"
#include "player_clone.h"
#include "enemy.h"
#include "sound.h"

#include "enemyStalk.h"
#include "enemyWolf.h"
#include "enemyAmbush.h"
#include "checkpoint.h"
#include "popupUI.h"
#include "goal.h"

#include "gimmick.h"
#include "gimmick_multi.h"
#include "actor.h"
#include "MapModel.h"
#include "camera_change.h"

// TODO
#include "weed.h"
#include "collision.h"

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

	const int HITSTOP_TIME = 0;	// �q�b�g�X�g�b�v�t���[��

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
	m_state		(STATE_NONE),	// ���
	m_nSave		(-1)			// �Z�[�u�|�C���g
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
	m_state		= STATE_START;	// ���
	m_nSave		= -1;			// �Z�[�u�|�C���g

	// ���U���g�}�l�[�W���[�̐���
	m_pResult = CResultManager::Create();
	if (m_pResult == nullptr)
	{ // �����Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ��荞�݃J�����̐ݒ�
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_AROUND);
	GET_MANAGER->GetCamera()->SetDestAround();

	// �V�l�}�X�R�[�v�n��
	//CSceneGame::GetCinemaScope()->SetScopeIn();

#if 0

	// TODO�F���u��
	// �v���C���[��ʏ��Ԃɂ���
	CPlayer::GetList()->GetList().front()->SetState(CPlayer::EState::STATE_NORMAL);
	CPlayer::GetList()->GetList().front()->SetAlpha(1.0f);

	// �ʏ��Ԃɂ���
	m_state = STATE_NORMAL;

#else

	// �X�^�[�g�J�����̃��Z�b�g����
	GET_MANAGER->GetCamera()->StartReset();

#endif // 0

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

		// �S�[�����Ȃ��ꍇ������
		if (CGoal::GetGoal() == nullptr) { break; }

		if (CGoal::GetGoal()->GetClear())
		{ // �N���A�����ꍇ

			// �N���A�����Ń��U���g�J��
			TransitionResult(CRetentionManager::EWin::WIN_SUCCESS);
		}
		else if (CSceneGame::GetTimerUI()->GetState() == CTimer::STATE_END)
		{ // ���Ԑ؂�ɂȂ����ꍇ

			// �v���C���[�̃^�C���A�b�v����
			GET_PLAYER->TimeUp();

			// �N���A���s�Ń��U���g�J��
			TransitionResult(CRetentionManager::EWin::WIN_FAIL);
		}
		break;

	case STATE_START:
		break;

	case STATE_GODITEM:

		/*
			���̏�Ԏ��͎����I�ɍX�V���s����_��l�����o�}�l�[�W���[���Q�[����ʂ𑀍삵�܂��B
			��Ԃ̕��A���}�l�[�W���[���v���C���[�̑�������m���s���̂ł����瑤����Ǘ�����K�v�͂Ȃ��ł��B
		*/
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
//	���ʊl������
//============================================================
void CGameManager::PossessGodItem(const CGodItem::EType typeID)
{
	// �^�C�}�[�̌v���ꎞ��~
	CSceneGame::GetTimerUI()->EnableStop(true);

	// �v���C���[�̏�Ԃ�_��l����Ԃɂ���
	GET_PLAYER->SetEnableGodItem(true);

	// �_��l�����o�}�l�[�W���[�𐶐�
	CGodItemManager::Create(typeID);

	// �_��l����Ԃɂ���
	m_state = STATE_GODITEM;
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

	// ���U���g���̕ۑ�
	switch (win)
	{
	case CRetentionManager::WIN_SUCCESS: // �N���A����

		GET_RETENTION->SetResult(win, CSceneGame::GetTimerUI()->GetTime(), -1);

		// TODO�F�^�C�~���O�ς���������(BGM�̍Đ�)
		PLAY_SOUND(CSound::LABEL_BGM_RESULT);
		break;

	case CRetentionManager::WIN_FAIL: // �N���A���s
		GET_RETENTION->SetResult(win, CSceneGame::GetTimerUI()->GetTime(), m_nSave);

		// TODO�F�^�C�~���O�ς���������(BGM�̍Đ�)
		PLAY_SOUND(CSound::LABEL_BGM_GAMEOVER);
		break;

	default:
		assert(false);
		GET_RETENTION->SetResult(win, CSceneGame::GetTimerUI()->GetTime(), -1);
		break;
	}

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

	// BGM�̒�~
	STOP_SOUND(CSound::LABEL_BGM_FOREST);
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
