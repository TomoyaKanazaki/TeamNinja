//============================================================
//
//	�v���C���[���� [player.cpp]
//	Author�F���c�E��
//  Adder : �������
//  Adder : ��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player.h"
#include "manager.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "collision.h"
#include "fade.h"
#include "multiModel.h"
#include "orbit.h"
#include "shadow.h"
#include "orbit.h"
#include "object2D.h"
#include "stage.h"
#include "field.h"
#include "cloneAngleUI.h"
#include "input.h"
#include "player_clone.h"
#include "checkpoint.h"
#include "gauge2D.h"
#include "effect3D.h"
#include "actor.h"
#include "effekseerControl.h"
#include "effekseerManager.h"
#include "gimmick_action.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\CHARACTER\\player.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

	const int	PRIORITY	= 3;				// �v���C���[�̗D�揇��
	const float	JUMP_MOVE	= 615.0f;			// ��W�����v�㏸��
	const float	STEP_MOVE	= JUMP_MOVE * 2.0f;	// ��W�����v�㏸��
	const float	CANON_GRAVITY= 500.0f;			// �d��
	const float CANON_MOVE	= CANON_GRAVITY * 30.0f;	// ������΂��㏸��
	const float REBOUND		= 500.0f;			// �W�����v�̒��˕Ԃ�
	const float	GRAVITY		= 60.0f;			// �d��
	const float	RADIUS		= 20.0f;			// ���a
	const float HEIGHT		= 80.0f;			// �g��
	const float	REV_ROTA	= 9.0f;				// �����ύX�̕␳�W��
	const float LANDING_SPEED_M = -20.0f;		// ���n�G�t�F�N�gM�̐؂�ւ����C��
	const float LANDING_SPEED_L = -35.0f;		// ���n�G�t�F�N�gL�̐؂�ւ����C��
	const float	ADD_MOVE	= 0.08f;			// ��A�N�V�������̑��x���Z��
	const float	JUMP_REV	= 0.16f;			// �ʏ��Ԏ��̋󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV	= 0.16f;			// �ʏ��Ԏ��̒n��̈ړ��ʂ̌����W��
	const float	SPAWN_ADD_ALPHA	= 0.03f;		// �X�|�[����Ԏ��̓����x�̉��Z��
	const int	BLEND_FRAME_OTHER	= 5;		// ���[�V�����̊�{�I�ȃu�����h�t���[��
	const int	BLEND_FRAME_LAND	= 15;		// ���[�V�������n�̃u�����h�t���[��
	const int	CAUTIOUS_TRANS_LOOP	= 7;		// �x�����[�V�����ɑJ�ڂ���ҋ@���[�v��
	const D3DXVECTOR3 DMG_ADDROT	= D3DXVECTOR3(0.04f, 0.0f, -0.02f);	// �_���[�W��Ԏ��̃v���C���[��]��
	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// �e�̑傫��
	const D3DXVECTOR3 OFFSET_JUMP	= D3DXVECTOR3(0.0f, 80.0f, 0.0f);	// ��W�����v�G�t�F�N�g�̔����ʒu�I�t�Z�b�g

	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 15.0f, 0.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), XCOL_CYAN);	// �I�t�Z�b�g���
	const int ORBIT_PART = 15;	// ������

	const float	STEALTH_MOVE	= 300.0f;	// �E�ё��̈ړ���
	const float	NORMAL_MOVE		= 600.0f;	// �ʏ�̈ړ���
	const float CLONE_MOVE		= NORMAL_MOVE * 1.1f; // ���g�̈ړ���

	const int MAX_TENSION = 10000; // �m�C�͂̍ő�l
	const int INIT_TENSION = 5000; // �m�C�͂̏����l
	const int SPEED_TENSION = 30; // �m�C�̓Q�[�W�̑������x
	const int MAX_CLONE = 20; // ���g�̍ő吔
	const float DISTANCE_CLONE = 50.0f; // ���g�̏o���ʒu�Ƃ̋���
	const int JUST_RECOVER = 500; // �W���X�g�A�N�V�����ł̉񕜗�
	const float GIMMICK_TIMER = 0.5f; // ���ڃM�~�b�N�𐶐��ł��鎞��
	const float STICK_ERROR = D3DX_PI * 0.875f; // �X�e�B�b�N�̓��͌덷���e�͈�
	const float GIMMICK_SET_DISTANCE = 10000.0f; // ���ڃM�~�b�N���g�̐����\�͈�

	// �u���[�̏��
	namespace blurInfo
	{
		const float	START_ALPHA = 0.4f;	// �u���[�J�n�����x
		const int	MAX_LENGTH = 15;	// �ێ��I�u�W�F�N�g�ő吔
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CPlayer> *CPlayer::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CPlayer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER, CObject::DIM_3D, PRIORITY),
	m_pShadow		(nullptr),		// �e�̏��
	m_pOrbit		(nullptr),		// �O�Ղ̏��
	m_oldPos		(VEC3_ZERO),	// �ߋ��ʒu
	m_move			(VEC3_ZERO),	// �ړ���
	m_destRot		(VEC3_ZERO),	// �ڕW����
	m_state			(STATE_NONE),	// ���
	m_bJump			(false),		// �W�����v��
	m_nCounterState	(0),			// ��ԊǗ��J�E���^�[
	m_pTensionGauge	(nullptr),		// �m�C�̓Q�[�W�̃|�C���^
	m_pCheckPoint	(nullptr),		// �Z�[�u�����`�F�b�N�|�C���g
	m_fScalar		(0.0f),			// �ړ���
	m_bClone		(true),			// ���g����\�t���O
	m_bGimmickClone	(false),		// �M�~�b�N�N���[���̐����t���O
	m_fGimmickTimer	(0.0f),			// �M�~�b�N�N���[���̐����^�C�}�[
	m_fTempStick	(0.0f),			// �X�e�B�b�N�̓��͊p��ۑ�����ϐ�
	m_bGetCamera	(false),		// �J�����擾�t���O
	m_fCameraRot	(0.0f),			// �J�����̊p�x
	m_fStickRot		(0.0f),			// �X�e�B�b�N�̊p�x
	m_fShootTarget	(0.0f),			// ������ԖڕW
	m_fShootStart	(0.0f),			// ������ъJ�n�n�_
	m_nCanonTime	(0)				// ������ю���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayer::Init(void)
{
	// �����o�ϐ���������
	m_pShadow		= nullptr;		// �e�̏��
	m_pOrbit		= nullptr;		// �O�Ղ̏��
	m_oldPos		= VEC3_ZERO;	// �ߋ��ʒu
	m_move			= VEC3_ZERO;	// �ړ���
	m_destRot		= VEC3_ZERO;	// �ڕW����
	m_state			= STATE_NONE;	// ���
	m_bJump			= true;			// �W�����v��
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_pTensionGauge	= nullptr;		// �m�C�̓Q�[�W�̃|�C���^
	m_pCheckPoint	= nullptr;		// �Z�[�u�����`�F�b�N�|�C���g
	m_fScalar		= 0.0f;			// �ړ���
	m_bClone		= true;			// ���g����\�t���O
	m_bGimmickClone	= false;		// �M�~�b�N�N���[���̐����t���O
	m_fGimmickTimer	= 0.0f;			// �M�~�b�N�N���[���̐����^�C�}�[
	m_fTempStick	= 0.0f;			// �X�e�B�b�N�̓��͊p��ۑ�����ϐ�

	// �I�u�W�F�N�g�L�����N�^�[�̏�����
	if (FAILED(CObjectChara::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �L�����N�^�[���̊���
	BindCharaData(SETUP_TXT);

	// �e�̐���
	m_pShadow = CShadow::Create(CShadow::TEXTURE_NORMAL, SHADOW_SIZE, this);
	if (m_pShadow == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �O�Ղ̐���
	m_pOrbit = COrbit::Create
	( // ����
		GetParts(MODEL_BODY)->GetPtrMtxWorld(),	// �e�}�g���b�N�X
		ORBIT_OFFSET,	// �I�t�Z�b�g���
		ORBIT_PART		// ������
	);
	if (m_pOrbit == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �m�C�̓Q�[�W�𐶐�
	m_pTensionGauge = CGauge2D::Create
	(
		MAX_TENSION, SPEED_TENSION, D3DXVECTOR3(300.0f, 30.0f, 0.0f),
		D3DXVECTOR3(300.0f, 30.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.56f, 0.87f, 1.0f),
		D3DXCOLOR(0.31f, 0.89f, 0.97f, 1.0f)
	);
	m_pTensionGauge->SetNum(INIT_TENSION);
	m_pTensionGauge->SetLabel(LABEL_UI);

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CPlayer>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// �v���C���[���o��������
	SetSpawn();

	// �J�n�G�t�F�N�g���o��
	GET_EFFECT->Create("data\\EFFEKSEER\\gamestart.efkefc", GetVec3Position(), GetVec3Rotation(), VEC3_ZERO, 60.0f);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayer::Uninit(void)
{
	// �m�C�̓Q�[�W�̏I��
	SAFE_UNINIT(m_pTensionGauge);
	
	// �e�̏I��
	m_pShadow->DeleteObjectParent();	// �e�I�u�W�F�N�g���폜
	SAFE_UNINIT(m_pShadow);

	// �O�Ղ̏I��
	SAFE_UNINIT(m_pOrbit);

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	CObjectChara::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayer::Update(const float fDeltaTime)
{
	DebugProc::Print(DebugProc::POINT_CENTER, "pos : (%f, %f, %f)\n", m_posCenter.x, m_posCenter.y, m_posCenter.z);
	DebugProc::Print(DebugProc::POINT_CENTER, "move : (%f, %f, %f)\n", m_move.x * fDeltaTime, m_move.y * fDeltaTime, m_move.z * fDeltaTime);

	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_NONE:
		break;

	case STATE_SPAWN:

		// �X�|�[����Ԏ��̍X�V
		currentMotion = UpdateSpawn(fDeltaTime);
		break;

	case STATE_NORMAL:

		// �ʏ��Ԃ̍X�V
		currentMotion = UpdateNormal(fDeltaTime);
		break;

	default:
		assert(false);
		break;
	}

	// �e�̍X�V
	m_pShadow->Update(fDeltaTime);

	// �O�Ղ̍X�V
	m_pOrbit->Update(fDeltaTime);

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	UpdateMotion(currentMotion, fDeltaTime);

	// �f�o�b�O�\��
	DebugProc::Print(DebugProc::POINT_LEFT, "�m�C�� : %d\n", m_pTensionGauge->GetNum());

#ifdef _DEBUG

	// ���͏����󂯎��|�C���^
	CInputKeyboard* pKeyboard = GET_INPUTKEY;

	// �m�C�͂̕ύX
	if (pKeyboard->IsTrigger(DIK_UP))
	{
		m_pTensionGauge->AddNum(100);
	}
	if (pKeyboard->IsTrigger(DIK_DOWN))
	{
		m_pTensionGauge->AddNum(-100);
	}
	if (pKeyboard->IsTrigger(DIK_RIGHT))
	{
		RecoverCheckPoint();
	}
	if (pKeyboard->IsTrigger(DIK_LEFT))
	{
		RecoverJust();
	}

#endif
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(CShader *pShader)
{
	CToonShader	*pToonShader = CToonShader::GetInstance();	// �g�D�[���V�F�[�_�[���
	if (pToonShader->IsEffectOK())
	{ // �G�t�F�N�g���g�p�\�ȏꍇ

		// �I�u�W�F�N�g�L�����N�^�[�̕`��
		CObjectChara::Draw(pToonShader);
	}
	else
	{ // �G�t�F�N�g���g�p�s�\�ȏꍇ

		// �G�t�F�N�g�G���[
		assert(false);

		// �I�u�W�F�N�g�L�����N�^�[�̕`��
		CObjectChara::Draw(pShader);
	}
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);		// ���g
	m_pShadow->SetEnableUpdate(bUpdate);	// �e
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);		// ���g
	m_pShadow->SetEnableDraw(bDraw);	// �e
}

//============================================================
//	��������
//============================================================
CPlayer *CPlayer::Create(CScene::EMode mode)
{
	// �|�C���^��錾
	CPlayer *pPlayer = nullptr;	// �v���C���[���

	// �v���C���[�̐���
	switch (mode)
	{ // ���[�h���Ƃ̏���
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_SELECT:
	case CScene::MODE_GAME:
		pPlayer = new CPlayer;
		break;

	default:	// ��O����
		assert(false);
		break;
	}

	if (pPlayer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �v���C���[�̏�����
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �v���C���[�̔j��
			SAFE_DELETE(pPlayer);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pPlayer;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CPlayer> *CPlayer::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	�m�b�N�o�b�N�q�b�g����
//============================================================
bool CPlayer::HitKnockBack(const int nDamage, const D3DXVECTOR3& /*rVecKnock*/)
{
	if (IsDeath())				 { return false; }	// ���S�ς�
	if (m_state != STATE_NORMAL) { return false; }	// �ʏ��ԈȊO

	// �m�C�͂�����
	m_pTensionGauge->AddNum(-nDamage);

	return true;
}

//============================================================
//	�q�b�g����
//============================================================
bool CPlayer::Hit(const int nDamage)
{
	if (IsDeath())				 { return false; }	// ���S�ς�
	if (m_state != STATE_NORMAL) { return false; }	// �ʏ��ԈȊO

	// �W�����v�G�t�F�N�g���o��
	GET_EFFECT->Create("data\\EFFEKSEER\\hit.efkefc", GetVec3Position() + OFFSET_JUMP, GetVec3Rotation(), VEC3_ZERO, 250.0f);

	// �m�C�͂�����
	m_pTensionGauge->AddNum(-nDamage);

	return true;
}

//============================================================
//	�o���̐ݒ菈��
//============================================================
void CPlayer::SetSpawn(void)
{
	// ����������
	SetState(STATE_SPAWN);	// �X�|�[����Ԃ̐ݒ�
	SetMotion(MOTION_IDOL);	// �ҋ@���[�V������ݒ�

	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �ʒu��ݒ�
	D3DXVECTOR3 pos = D3DXVECTOR3(-1600.0f, 0.0f, 0.0f);	// �ʒu
	SetVec3Position(pos);

	// ������ݒ�
	D3DXVECTOR3 rot = VEC3_ZERO;	// ����
	SetVec3Rotation(rot);
	m_destRot = rot;

	// �ړ��ʂ�������
	m_move = VEC3_ZERO;

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �����x�𓧖��ɍĐݒ�
	SetAlpha(0.0f);

	// �`����ĊJ
	SetEnableDraw(true);		
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CPlayer::SetState(const EState state)
{
	if (state > NONE_IDX && state < STATE_MAX)
	{ // �͈͓��̏ꍇ

		// �����̏�Ԃ�ݒ�
		m_state = state;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	��Ԏ擾����
//============================================================
CPlayer::EState CPlayer::GetState(void) const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	���a�擾����
//============================================================
float CPlayer::GetRadius(void) const
{
	// ���a��Ԃ�
	return RADIUS;
}

//============================================================
//	�c���擾����
//============================================================
float CPlayer::GetHeight(void) const
{
	// �c����Ԃ�
	return HEIGHT;
}

//============================================================
//	�M�~�b�N�̃n�C�W�����v����
//============================================================
bool CPlayer::GimmickHighJump(const int nNumClone)
{
	// �W�����v���͔�΂Ȃ�
	if (m_bJump) { return false; }

	// ��ړ��ʂ�^����
	m_move.y = JUMP_MOVE * nNumClone;

	// �W�����v���ɂ���
	m_bJump = true;

	// ���[�V�����̐ݒ�
	SetMotion(MOTION_JUMP_HIGH, BLEND_FRAME_OTHER);

	// �W�����v�G�t�F�N�g���o��
	GET_EFFECT->Create("data\\EFFEKSEER\\Highjump.efkefc", GetVec3Position() + OFFSET_JUMP, GetVec3Rotation(), VEC3_ZERO, 25.0f);

	// �Ǐ]���Ă��镪�g������
	CPlayerClone::Delete();

	return true;
}

//============================================================
//	�M�~�b�N�̃n�C�W�����v����
//============================================================
void CPlayer::GimmickLowJump(void)
{
	// ��ړ��ʂ�^����
	m_move.y = STEP_MOVE;

	// �W�����v���ɂ���
	m_bJump = true;

	// ���[�V�����̐ݒ�
	SetMotion(MOTION_JUMP_HIGH, BLEND_FRAME_OTHER);
}

//============================================================
// �M�~�b�N�̔�э~�蒅�n
//============================================================
bool CPlayer::GimmickLand(void)
{
	// �㏸�ʂ�^����
	m_move.y = REBOUND;

	// �W�����v���[�V������ݒ�
	SetMotion(MOTION_JUMP_MINI);

	// ���n���̍Đ�
	PLAY_SOUND(CSound::LABEL_SE_LAND_S);

	return true;
}

//==========================================
//  �m�C�͂̒l���擾
//==========================================
int CPlayer::GetTension() const
{
	// �m�C�̓Q�[�W�����݂��Ȃ��ꍇ
	if (m_pTensionGauge == nullptr) { return -1; }

	// �m�C�͂̒l��Ԃ�
	return m_pTensionGauge->GetNum();
}

//==========================================
//  �`�F�b�N�|�C���g�ł̉񕜏���
//==========================================
void CPlayer::RecoverCheckPoint()
{
	// ���݂̎m�C�͂��擾����
	unsigned int nTension = GetTension();

	// �m�C�̓Q�[�W�����݂��Ȃ������ꍇ�֐��𔲂���
	if (nTension == -1) { return; }

	// �ő�l�ƌ��ݒl�̍������߂�
	float fDiff = (float)(MAX_TENSION - nTension);

	// �����̔����̒l�Ŏm�C�͂��񕜂���
	m_pTensionGauge->AddNum((int)(fDiff *= 0.5f));
}

//==========================================
//  �W���X�g�A�N�V�����ł̉񕜏���
//==========================================
void CPlayer::RecoverJust()
{
	// �m�C�̓Q�[�W�����݂��Ȃ��ꍇ
	if (m_pTensionGauge == nullptr) { return; }

	// �Œ�l�Ŏm�C�͂��񕜂���
	m_pTensionGauge->AddNum(JUST_RECOVER);

	// �񕜃G�t�F�N�g���o��
	GET_EFFECT->Create("data\\EFFEKSEER\\concentration.efkefc", GetVec3Position(), GetVec3Rotation(), VEC3_ZERO, 50.0f);
}

//============================================================
//	�X�|�[����Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateSpawn(const float fDeltaTime)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// �t�F�[�h�A�E�g��Ԏ��̍X�V
	if (UpdateFadeOut(SPAWN_ADD_ALPHA))
	{ // �s�����ɂȂ�؂����ꍇ

		// ��Ԃ�ݒ�
		SetState(STATE_NORMAL);
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateNormal(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;		// ���݂̃��[�V����
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �ړ�����
	currentMotion = UpdateMove();

	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(posPlayer, fDeltaTime);

	// �A�N�^�[�̓����蔻��
	CollisionActor(posPlayer);

	// ���n����
	UpdateLanding(posPlayer, fDeltaTime);

	// �����X�V
	UpdateRotation(rotPlayer, fDeltaTime);

	// �ǂ̓����蔻��
	GET_STAGE->CollisionWall(posPlayer, m_oldPos, RADIUS, HEIGHT, m_move, &m_bJump);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// ���g�̏���
	ControlClone(posPlayer, rotPlayer, fDeltaTime);

	// �ۑ��ʒu�̍X�V
	UpdateSaveTeleport();

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//===========================================
//  ���ˏ�Ԏ��̍X�V����
//===========================================
CPlayer::EMotion CPlayer::UpdateShoot(const float fDeltaTime)
{
	// ������ю��Ԃ̍X�V
	m_nCanonTime++;

	// ���g�̏����擾
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// y���W�̌v�Z
	float fTemp = 0.0f;
	useful::Parabola
	(
		CANON_MOVE,
		-CANON_GRAVITY,
		m_nCanonTime,
		&m_move.y,
		&fTemp
	);

	// �ʒu�X�V
	UpdatePosition(posPlayer, fDeltaTime);

	// �A�N�^�[�̓����蔻��
	CollisionActor(posPlayer);

	// ���n����
	UpdateLanding(posPlayer, fDeltaTime);

	// ���n������ʏ��ԂɑJ�ڂ���
	if (!m_bJump)
	{
		m_state = STATE_NORMAL;
		m_move = VEC3_ZERO;
	}

	// �����X�V
	UpdateRotation(rotPlayer, fDeltaTime);

	// �ǂ̓����蔻��
	GET_STAGE->CollisionWall(posPlayer, m_oldPos, RADIUS, HEIGHT, m_move, &m_bJump);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// TODO ���˂���Ă鎞�̃��[�V������K�p���Ȃ���
	return MOTION_IDOL;
}

//============================================================
//	�ߋ��ʒu�̍X�V����
//============================================================
void CPlayer::UpdateOldPosition(void)
{
	// �ߋ��ʒu���X�V
	m_oldPos = GetVec3Position();
}

//============================================================
//	�ړ��ʁE�ڕW�����̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateMove(void)
{
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// ���͏��̎擾
	CInputPad* pPad = GET_INPUTPAD;
	D3DXVECTOR3 CameraRot = GET_MANAGER->GetCamera()->GetRotation();
	
	// �X�e�B�b�N�̌X������ړ��ʂ�ݒ�
	float fSpeed = pPad->GetPressLStickTilt();	// �X�e�B�b�N�̌X����
	if (pad::DEAD_ZONE < fSpeed)
	{
		// �X�e�B�b�N�������擾
		float fStickRot = pPad->GetPressLStickRot() - (D3DX_PI * 0.5f);

		// �X�e�B�b�N�̊p�x���r
		if(fabsf(m_fStickRot - fStickRot) > 0.1f)
		{
			m_bGetCamera = false;
		}

		// �X�e�B�b�N�̊p�x��ۑ�
		m_fStickRot = fStickRot;

		// �J�����̌������擾
		if (!m_bGetCamera)
		{
			m_fCameraRot = GET_MANAGER->GetCamera()->GetRotation().y;
			m_bGetCamera = true;
		}

		// �ړ��̌������Z�o
		float fMoveRot = fStickRot + m_fCameraRot;

		// �ڕW������ݒ�
		m_destRot.y = fMoveRot;

		// �ړ��ʂ�ݒ肷��
		D3DXVECTOR3 fRate = pPad->GetStickRateL(pad::DEAD_RATE);
		m_move.x = -sinf(fMoveRot) * NORMAL_MOVE;
		m_move.z = -cosf(fMoveRot) * NORMAL_MOVE;

#ifdef _DEBUG
		if (pPad->IsPress(CInputPad::KEY_Y))
		{
			m_move.x *= 3.0f;
			m_move.z *= 3.0f;
		}
#endif
		// ���s���[�V�����ɂ���
		currentMotion = MOTION_DASH;

		// �ړ��ʂ��X�J���[�l�ɕϊ�����
		m_fScalar = sqrtf(m_move.x * m_move.x + m_move.z * m_move.z);

		// �ړ��ʂ���薢���̏ꍇ�E�ё����[�V�����ɂȂ�
		if (m_fScalar <= STEALTH_MOVE)
		{
			// �E�ё����[�V�����ɂ���
			currentMotion = MOTION_STEALTHWALK;
		}
	}
	else
	{
		// �t���O��܂�
		m_bGetCamera = false;
	}

#ifdef _DEBUG

	// �L�[�{�[�h�̈ړ�����
	DebugMoveControl();

	// �W�����v����
	DebugJumpControl();

#endif

	// ���[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	�d�͂̍X�V����
//============================================================
void CPlayer::UpdateGravity(void)
{
	// �d�͂����Z
	m_move.y -= GRAVITY;
}

//============================================================
//	�ۑ��ʒu�̍X�V����
//============================================================
void CPlayer::UpdateSaveTeleport(void)
{
	CInputPad* pPad = GET_INPUTPAD;
	if (pPad->IsTrigger(CInputPad::KEY_BACK))
	{
		// �`�F�b�N�|�C���g�ɋA��
		SaveReset();
	}
}

//============================================================
//	���n�󋵂̍X�V����
//============================================================
bool CPlayer::UpdateLanding(D3DXVECTOR3& rPos, const float fDeltaTime)
{
	bool bLand = false;	// ���n�t���O
	CStage *pStage = GET_STAGE;	// �X�e�[�W���
	D3DXVECTOR3 move = m_move * fDeltaTime;		//���݂̈ړ����x���ꎞ�ۑ�

	// �n�ʁE�����ʒu�̒��n����
	if (pStage->LandFieldPosition(rPos, m_oldPos, m_move)
	||  pStage->LandLimitPosition(rPos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

		// ���n���Ă����Ԃɂ���
		bLand = true;

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}

	// ���݂̃��[�V������ނ��擾
	int nCurMotion = GetMotionType();

	// �W�����v���[�V�����̃t���O��ݒ�
	bool bTypeJump = nCurMotion == MOTION_JUMP_HIGH
				  || nCurMotion == MOTION_JUMP_MINI;

	// �������[�V�����̃t���O��ݒ�
	bool bTypeFall = nCurMotion == MOTION_FALL;

	if (!m_bJump)
	{ // �󒆂ɂ��Ȃ��ꍇ

		if (bTypeJump || bTypeFall)
		{ // ���[�V�������W�����v���A�܂��͗������̏ꍇ

			// ���n���[�V�������w��
			SetMotion(MOTION_LANDING);

			if (move.y < LANDING_SPEED_L)
			{
				// ���n(��)�G�t�F�N�g���o��
				GET_EFFECT->Create("data\\EFFEKSEER\\landing_big.efkefc", GetVec3Position() + move, GetVec3Rotation(), VEC3_ZERO, 90.0f);
				// ���n��(��)�̍Đ�
				PLAY_SOUND(CSound::LABEL_SE_LAND_B);
			}
			else if (move.y < LANDING_SPEED_M)
			{
				// ���n(��)�G�t�F�N�g���o��
				GET_EFFECT->Create("data\\EFFEKSEER\\landing_mid.efkefc", GetVec3Position() + move, GetVec3Rotation(), VEC3_ZERO, 60.0f);
				// ���n��(��)�̍Đ�
				PLAY_SOUND(CSound::LABEL_SE_LAND_B);
			}
			else
			{
				// ���n(��)�G�t�F�N�g���o��
				GET_EFFECT->Create("data\\EFFEKSEER\\landing_small.efkefc", GetVec3Position() + move, GetVec3Rotation(), VEC3_ZERO, 90.0f);
				// ���n��(��)�̍Đ�
				PLAY_SOUND(CSound::LABEL_SE_LAND_S);
			}

			
		}
	}
	else
	{ // �󒆂ɂ���ꍇ

		if (!bTypeJump)
		{ // ���[�V�������W�����v���ł͂Ȃ��ꍇ

			// �������[�V�������w��
			SetMotion(MOTION_FALL);
		}
	}

	// ���n�t���O��Ԃ�
	return bLand;
}

//============================================================
//	�ʒu�̍X�V����
//============================================================
void CPlayer::UpdatePosition(D3DXVECTOR3& rPos, const float fDeltaTime)
{
	// �ړ��ʂ����Z
	rPos += m_move * fDeltaTime;

	// �ړ��ʂ�����
	if (m_bJump)
	{ // �󒆂̏ꍇ

		m_move.x += (0.0f - m_move.x) * JUMP_REV;
		m_move.z += (0.0f - m_move.z) * JUMP_REV;
	}
	else
	{ // �n��̏ꍇ

		m_move.x += (0.0f - m_move.x) * LAND_REV;
		m_move.z += (0.0f - m_move.z) * LAND_REV;
	}

	// ���S���W�̍X�V
	m_posCenter = rPos;
	m_posCenter.y += HEIGHT * 0.5f;
}

//============================================================
//	�����̍X�V����
//============================================================
void CPlayer::UpdateRotation(D3DXVECTOR3& rRot, const float fDeltaTime)
{
	// �ϐ���錾
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����̐��K��
	useful::NormalizeRot(m_destRot.y);

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = m_destRot.y - rRot.y;

	// ���������̐��K��
	useful::NormalizeRot(fDiffRot);

	// �����̍X�V
	rRot.y += fDiffRot * fDeltaTime * REV_ROTA;

	// �����̐��K��
	useful::NormalizeRot(rRot.y);
}

//============================================================
//	���[�V�����E�L�����N�^�[�̍X�V����
//============================================================
void CPlayer::UpdateMotion(int nMotion, const float fDeltaTime)
{
	// ����ł��甲����
	if (IsDeath()) { return; }

	int nAnimMotion = GetMotionType();	// ���ݍĐ����̃��[�V����
	if (nMotion != NONE_IDX)
	{ // ���[�V�������ݒ肳��Ă���ꍇ

		if (IsMotionLoop())
		{ // ���[�v���郂�[�V�������̏ꍇ

			if (nAnimMotion != nMotion)
			{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(nMotion, BLEND_FRAME_OTHER);
			}
		}
		else
		{ // ���[�v���Ȃ����[�V�������̏ꍇ

			switch (GetMotionType())
			{ // ���[�V�������Ƃ̏���
			case MOTION_CAUTIOUS:	// �x�����[�V����
			case MOTION_LANDING:	// ���n���[�V����

				if (nMotion != MOTION_IDOL)
				{ // �ҋ@���[�V�����ȊO�̏ꍇ

					// ���݂̃��[�V�����̐ݒ�
					SetMotion(nMotion, BLEND_FRAME_OTHER);
				}

				break;
			}
		}
	}

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update(fDeltaTime);

	switch (GetMotionType())
	{ // ���[�V�����̎�ނ��Ƃ̏���
	case MOTION_IDOL:	// �ҋ@���[�V����

		if (GetMotionNumLoop() >= CAUTIOUS_TRANS_LOOP)
		{ // �ҋ@���[�V�����ł��΂炭�����ꍇ

			// �x�����[�V�����̐ݒ�
			SetMotion(MOTION_CAUTIOUS, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_CAUTIOUS:	// �x�����[�V����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_LAND);
		}

		break;

#if 0
	case MOTION_DASH:	// ���s���[�V����

		if (GetMotionPose() % 4 == 0 && GetMotionCounter() == 0)
		{ // ���������^�C�~���O�̏ꍇ

			switch (m_land)
			{ // ���n�����Ƃ̏���
			case LAND_OBSTACLE:

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_WALK_OBS);	// ���s���i��Q���j

				break;

			default:

				// �T�E���h�̍Đ�
				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_WALK_BUILD);	// ���s���i�r���j

				break;
			}
		}

		break;

	case MOTION_STEALTHWALK:	// �E�ё����[�V����

		break;
#endif

	case MOTION_JUMP_MINI:	// ���W�����v���[�V����

		if (!m_bJump)
		{ // �W�����v���ł͂Ȃ��ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_JUMP_HIGH:	// ��W�����v���[�V����

		if (!m_bJump)
		{ // �W�����v���ł͂Ȃ��ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_FALL:	// �������[�V����

		if (!m_bJump)
		{ // �W�����v���ł͂Ȃ��ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_LANDING:	// ���n���[�V����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_LAND);
		}

		break;
	}
}

//============================================================
//	�t�F�[�h�A�E�g��Ԏ��̍X�V����
//============================================================
bool CPlayer::UpdateFadeOut(const float fAdd)
{
	// �ϐ���錾
	bool bAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �v���C���[���g�̕`����ĊJ
	CObject::SetEnableDraw(true);

	// �����x���グ��
	fAlpha += fAdd;

	if (fAlpha >= GetMaxAlpha())
	{ // �����x���オ��؂����ꍇ

		// �����x��␳
		fAlpha = GetMaxAlpha();

		// �s�����ɂȂ�؂�����Ԃɂ���
		bAlpha = true;
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bAlpha;
}

//============================================================
//	�t�F�[�h�C����Ԏ��̍X�V����
//============================================================
bool CPlayer::UpdateFadeIn(const float fSub)
{
	// �ϐ���錾
	bool bAlpha = false;		// ������
	float fAlpha = GetAlpha();	// �����x

	// �����x��������
	fAlpha -= fSub;

	if (fAlpha <= 0.0f)
	{ // �����x��������؂����ꍇ

		// �����x��␳
		fAlpha = 0.0f;

		// �����ɂȂ�؂�����Ԃɂ���
		bAlpha = true;

		// �v���C���[���g�̕`����~
		CObject::SetEnableDraw(false);
	}

	// �����x��ݒ�
	SetAlpha(fAlpha);

	// �����󋵂�Ԃ�
	return bAlpha;
}

//==========================================
//  ���g�̏���
//==========================================
void CPlayer::ControlClone(D3DXVECTOR3& rPos, D3DXVECTOR3& rRot, const float fDeltaTime)
{
	// ����\�t���O���m�F
	if (!m_bClone) { return; }

	// ���͏��̎󂯎��
	CInputPad* pPad = GET_INPUTPAD;

	// �Ǐ]���g�̍폜
	if (pPad->IsTrigger(CInputPad::KEY_RB))
	{
		// ���X�g�����݂��Ȃ��ꍇ�ɍ폜���Ȃ�
		if (CPlayerClone::GetList() != nullptr)
		{
			CPlayerClone::Delete();
		}
	}

	// ���g���Ăі߂�
	CallClone();

	// �M�~�b�N�̒��ڐ������ł���ꍇ�֐��𔲂���
	if (CreateGimmick(fDeltaTime)) { return; }

#ifdef _DEBUG

	// �ړ����g�̍폜
	if (pPad->IsTrigger(CInputPad::KEY_LB))
	{
		// ���X�g�����݂��Ȃ��ꍇ�ɍ폜���Ȃ�
		if (CPlayerClone::GetList() != nullptr)
		{
			CPlayerClone::Delete(CPlayerClone::ACTION_MOVE);
		}
	}

#endif

	// ���g�̐�������������ꍇ�֐��𔲂���
	if (CPlayerClone::GetList() != nullptr && CPlayerClone::GetList()->GetNumAll() >= MAX_CLONE) { return; }

	// �m�C�͂�0�Ȃ�֐��𔲂���
	if (m_pTensionGauge->GetNum() <= 0) { return; }

	// �E�X�e�B�b�N�̓��͂��Ȃ��ꍇ�֐��𔲂���
	if (!pPad->GetTriggerRStick()) { return; }

	// TODO�F�m�C�͂̌���ʍl���āI
#if 0
#ifndef _DEBUG
	// �m�C�͂���������
	m_pTensionGauge->AddNum(-500);
#endif
#endif

	// �v���C���[�̕������擾
	float fRotPlayer = GetVec3Rotation().y;

	// �X�e�B�b�N���͂̕������擾����
	float fRotStick = pPad->GetPressRStickRot();

	// �J�����̌������擾
	float fCameraRot = GET_MANAGER->GetCamera()->GetRotation().y;

	// �X�e�B�b�N������3D��ԂɑΉ�����
	float fTemp = fRotStick + fCameraRot;
	useful::NormalizeRot(fTemp);

	// �v���C���[��������X�e�B�b�N�̕��������Z
	float fRot = fRotPlayer - (fTemp - D3DX_PI * 0.5f);
	useful::NormalizeRot(fRot);

	// ���g�̈ʒu���Z�o
	D3DXVECTOR3 pos = rPos + D3DXVECTOR3
	(
		DISTANCE_CLONE * cosf(-fTemp),
		0.0f,
		DISTANCE_CLONE * sinf(-fTemp)
	);

	// ���߂��l�ƃ΂̌덷���������ꍇ���Ă��镪�g���o���Ċ֐��𔲂���
	if (fabsf(fRot) >= STICK_ERROR)
	{
		// �����Ɏ��s���Ă����ꍇ�M�~�b�N�����ҋ@�ɂȂ�
		if (CPlayerClone::Create() == nullptr)
		{
			m_bGimmickClone = true;
			m_fGimmickTimer = 0.0f;
			m_fTempStick = fTemp;
		}
		return;
	}

	// ���g�̈ړ��ʂ��Z�o����
	D3DXVECTOR3 move = D3DXVECTOR3
	(
		CLONE_MOVE * cosf(-fTemp),
		0.0f,
		CLONE_MOVE * sinf(-fTemp)
	);

	// �������g���o��
	if(CPlayerClone::Create(pos, move) == nullptr)
	{
		m_bGimmickClone = true;
		m_fGimmickTimer = 0.0f;
		m_fTempStick = fTemp;
	}
}

//==========================================
//  ���O�̃`�F�b�N�|�C���g�ɋA��
//==========================================
void CPlayer::SaveReset()
{
	// �Z�[�u����Ă��Ȃ��ꍇ�֐��𔲂���
	if (m_pCheckPoint == nullptr) { return; }

	// �`�F�b�N�|�C���g�̍��W�ɔ��
	SetVec3Position(m_pCheckPoint->GetVec3Position());

	// �Z�[�u�������_�ł̎m�C�͂ɂ���
	m_pTensionGauge->SetNum(m_pCheckPoint->GetSaveTension());
}

//==========================================
//  ���g���Ăі߂�
//==========================================
void CPlayer::CallClone()
{
	// �p�b�h�̓��͏����擾����
	CInputPad* pPad = GET_INPUTPAD;

	// �E�X�e�B�b�N�̉������݂��Ȃ������ꍇ�֐��𔲂���
	if (!pPad->IsTrigger(CInputPad::KEY_RSTICKPUSH)) { return; }

	// ���g��Ǐ]����
	CPlayerClone::CallBack();
}

//===========================================
//  ���ڃM�~�b�N�𐶐����鏈��
//===========================================
bool CPlayer::CreateGimmick(const float fDeltaTime)
{
	// �����t���O��off�̏ꍇ�֐��𔲂���
	if (!m_bGimmickClone) { return false; }

	// �^�C�}�[������𒴂��Ă����ꍇ�֐��𔲂���
	if (m_fGimmickTimer >= GIMMICK_TIMER)
	{
		m_fGimmickTimer = 0.0f;
		m_bGimmickClone = false;
		return false;
	}

	// �^�C�}�[�����Z����
	m_fGimmickTimer += fDeltaTime;

	// ���͏��̎擾
	CInputPad* pPad = GET_INPUTPAD;

	// �X�e�B�b�N���͂��������ꍇ
	if (pPad->GetTriggerRStick())
	{
		// �O����͂Ƃ̌덷�����e�͈͊O�̏ꍇ�֐��𔲂���
		if (m_fTempStick - pPad->GetPressRStickRot() > STICK_ERROR)
		{
			m_fGimmickTimer = 0.0f;
			m_bGimmickClone = false;
			return false;
		}

		// �M�~�b�N�̃��X�g���擾
		std::list<CGimmickAction*> list = CGimmickAction::GetList()->GetList();

		// �v���C���[����ł��߂��M�~�b�N���擾����ϐ�
		CGimmickAction* pGimmick = nullptr;
		float fTempDistance = 0.0f; // �ł��߂��M�~�b�N�܂ł̋�����2��

		// �ł��߂��M�~�b�N�𑖍�����
		for (auto gimmick : list)
		{
			// �ۑ����Ă�M�~�b�N��null�̏ꍇ�ۑ����Ď��ɐi��
			if (pGimmick == nullptr)
			{
				pGimmick = gimmick;
				D3DXVECTOR3 vecToGimmick = GetVec3Position() - pGimmick->GetVec3Position();
				fTempDistance = vecToGimmick.x * vecToGimmick.x + vecToGimmick.y * vecToGimmick.y + vecToGimmick.z * vecToGimmick.z;
				continue;
			}

			// �v���C���[�ƑΏۃM�~�b�N�����ԃx�N�g���̎Z�o
			D3DXVECTOR3 vecToGimmick = GetVec3Position() - gimmick->GetVec3Position();

			// ������2�悪�ۑ����ꂽ���l�����傫���ꍇ���ɐi��
			float fDistance = vecToGimmick.x* vecToGimmick.x + vecToGimmick.y * vecToGimmick.y + vecToGimmick.z * vecToGimmick.z;
			if (fTempDistance < fDistance)
			{ continue; }

			// �ΏۃM�~�b�N��ۑ�����
			pGimmick = gimmick;
			fTempDistance = fDistance;
		}

		// �������߂��ꍇ
		if (pGimmick->CollisionPlayer())
		{
			// ���ڃM�~�b�N�ɂȂ镪�g��K�v������
			for (int i = 0; i < pGimmick->GetNumActive(); ++i)
			{
				CPlayerClone::Create(pGimmick);
			}
		}

		// �t���O�����Z�b�g���֐��𔲂���
		m_fGimmickTimer = 0.0f;
		m_bGimmickClone = false;
		return true;
	}

	return true;
}

//==========================================
// �A�N�^�[�̓����蔻��
//==========================================
void CPlayer::CollisionActor(D3DXVECTOR3& pos)
{
	// �A�N�^�[�̃��X�g�\����������Δ�����
	if (CActor::GetList() == nullptr) { return; }

	std::list<CActor*> list = CActor::GetList()->GetList();	// ���X�g���擾

	for (auto actor : list)
	{
		// �����蔻�菈��
		actor->Collision
		(
			pos,		// �ʒu
			m_oldPos,	// �O��̈ʒu
			RADIUS,		// ���a
			HEIGHT,		// ����
			m_move,		// �ړ���
			m_bJump		// �W�����v��
		);
	}

	// �ʒu��K�p
	SetVec3Position(pos);
}

#ifdef _DEBUG

//==========================================
// �W�����v���쏈��
//==========================================
void CPlayer::DebugJumpControl(void)
{
	if (GET_INPUTPAD->IsTrigger(CInputPad::KEY_X)
	||  GET_INPUTKEY->IsTrigger(DIK_SPACE))
	{
		// �㏸�ʂ�^�����
		m_move.y = JUMP_MOVE;

		// �W�����v���ɂ����
		m_bJump = true;

		// �W�����v���[�V������ݒ�
		SetMotion(MOTION_JUMP_MINI);
	}
}

//==========================================
// �L�[�{�[�h�̈ړ����쏈��
//==========================================
void CPlayer::DebugMoveControl(void)
{
	CInputKeyboard* pKey = GET_INPUTKEY;
	float fMoveRot = 0.0f;	// �ړ�����
	const float MOVE = 60.0f;

	if (pKey->IsPress(DIK_W))
	{
		if (pKey->IsPress(DIK_A))
		{
			// �ړ�������ݒ�
			fMoveRot = (D3DX_PI * 0.75f);

			// �ʒu��ݒ�
			m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
			m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;
		}
		else if (pKey->IsPress(DIK_D))
		{
			// �ړ�������ݒ�
			fMoveRot = (D3DX_PI * -0.75f);

			// �ʒu��ݒ�
			m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
			m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;
		}
		else
		{
			// �ړ�������ݒ�
			fMoveRot = D3DX_PI;

			// �ʒu��ݒ�
			m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
			m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;
		}

		// ������ݒ�
		m_destRot.y = fMoveRot;
	}
	else if (pKey->IsPress(DIK_S))
	{
		if (pKey->IsPress(DIK_A))
		{
			// �ړ�������ݒ�
			fMoveRot = (D3DX_PI * 0.25f);

			// �ʒu��ݒ�
			m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
			m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;
		}
		else if (pKey->IsPress(DIK_D))
		{
			// �ړ�������ݒ�
			fMoveRot = (D3DX_PI * -0.25f);

			// �ʒu��ݒ�
			m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
			m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;
		}
		else
		{
			// �ړ�������ݒ�
			fMoveRot = 0.0f;

			// �ʒu��ݒ�
			m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
			m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;
		}

		// ������ݒ�
		m_destRot.y = fMoveRot;
	}
	else if (pKey->IsPress(DIK_A))
	{
		// �ړ�������ݒ�
		fMoveRot = (D3DX_PI * 0.5f);

		// �ʒu��ݒ�
		m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
		m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;

		// ������ݒ�
		m_destRot.y = fMoveRot;
	}
	else if (pKey->IsPress(DIK_D))
	{
		// �ړ�������ݒ�
		fMoveRot = (D3DX_PI * -0.5f);

		// �ʒu��ݒ�
		m_move.x += sinf(fMoveRot - D3DX_PI) * MOVE;
		m_move.z += cosf(fMoveRot - D3DX_PI) * MOVE;

		// ������ݒ�
		m_destRot.y = fMoveRot;
	}

	// �����̐��K��
	useful::NormalizeRot(m_destRot.y);
}

//==========================================
// �L�[�{�[�h�̕��g���쏈��
//==========================================
void CPlayer::DebugCloneControl(void)
{
	CInputKeyboard* pKey = GET_INPUTKEY;
	float fStickRot = 0.0f;
	D3DXVECTOR3 move = VEC3_ZERO;

	if (pKey->IsTrigger(DIK_I))
	{ // �O�֌W�ړ�
		if (pKey->IsTrigger(DIK_J))
		{
			fStickRot = (D3DX_PI * 0.75f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// �������g���o��
			CPlayerClone::Create(move);
		}
		else if (pKey->IsTrigger(DIK_L))
		{
			fStickRot = (D3DX_PI * -0.75f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// �������g���o��
			CPlayerClone::Create(move);
		}
		else
		{
			fStickRot = D3DX_PI;

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// �������g���o��
			CPlayerClone::Create(move);
		}
	}
	else if (pKey->IsTrigger(DIK_K))
	{ // ���֌W�ړ�
		if (pKey->IsTrigger(DIK_J))
		{
			fStickRot = (D3DX_PI * 0.25f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// �������g���o��
			CPlayerClone::Create(move);
		}
		else if (pKey->IsTrigger(DIK_L))
		{
			fStickRot = (D3DX_PI * -0.25f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// �������g���o��
			CPlayerClone::Create(move);
		}
		else
		{
			fStickRot = 0.0f;

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// �������g���o��
			CPlayerClone::Create(move);
		}
	}
	else if (pKey->IsTrigger(DIK_J))
	{ // ���֌W�ړ�
		fStickRot = (D3DX_PI * 0.5f);

		move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
		move.y = 0.0f;
		move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

		// �������g���o��
		CPlayerClone::Create(move);
	}
	else if (pKey->IsTrigger(DIK_L))
	{ // �E�֌W�ړ�
		fStickRot = (D3DX_PI * -0.5f);

		move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
		move.y = 0.0f;
		move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

		// �������g���o��
		CPlayerClone::Create(move);
	}
}

#endif // _DEBUG
