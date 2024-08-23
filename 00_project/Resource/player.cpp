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
#include "transpoint.h"
#include "effect3D.h"
#include "actor.h"
#include "coin.h"
#include "godItem.h"
#include "effekseerControl.h"
#include "effekseerManager.h"
#include "gimmick_action.h"
#include "enemyAttack.h"
#include "tension.h"
#include "retentionManager.h"

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
	const float	GRAVITY		= 3600.0f;			// �d��
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
	const float SPAWN_ADD_HEIGHT = 5000.0f;		// �X�|�[����Ԃŏグ�鍂��

	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 15.0f, 0.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), XCOL_CYAN);	// �I�t�Z�b�g���
	const int ORBIT_PART = 15;	// ������

	const float	STEALTH_MOVE	= 300.0f;	// �E�ё��̈ړ���
	const float	NORMAL_MOVE = 480.0f;	// �ʏ�̈ړ���
	const float	DODGE_MOVE = 800.0f;	// ����̈ړ���
	const float	DAMAGE_MOVE = 400.0f;	// �m�b�N�o�b�N�̈ړ���
	const float CLONE_MOVE = NORMAL_MOVE * 1.1f; // ���g�̈ړ���

	const int INIT_CLONE = 5; // �ŏ��Ɏg���镪�g�̐�
	const int HEAL_CHECKPOINT = 3; // �`�F�b�N�|�C���g�̉񕜗�
	const float DISTANCE_CLONE = 50.0f; // ���g�̏o���ʒu�Ƃ̋���
	const float GIMMICK_TIMER = 0.5f; // ���ڃM�~�b�N�𐶐��ł��鎞��
	const float STICK_ERROR = D3DX_PI * 0.875f; // �X�e�B�b�N�̓��͌덷���e�͈�

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
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
	m_pOrbit		(nullptr),		// �O�Ղ̏��
	m_oldPos		(VEC3_ZERO),	// �ߋ��ʒu
	m_move			(VEC3_ZERO),	// �ړ���
	m_destRot		(VEC3_ZERO),	// �ڕW����
	m_state			(STATE_NONE),	// ���
	m_bJump			(false),		// �W�����v��
	m_nCounterState	(0),			// ��ԊǗ��J�E���^�[
	m_nWalkCount	(0),			// ���s���J�E���g
	m_pCheckPoint	(nullptr),		// �Z�[�u�����`�F�b�N�|�C���g
	m_fScalar		(0.0f),			// �ړ���
	m_bClone		(true),			// ���g����\�t���O
	m_bGimmickClone	(false),		// �M�~�b�N�N���[���̐����t���O
	m_fGimmickTimer	(0.0f),			// �M�~�b�N�N���[���̐����^�C�}�[
	m_fTempStick	(0.0f),			// �X�e�B�b�N�̓��͊p��ۑ�����ϐ�
	m_bGetCamera	(false),		// �J�����擾�t���O
	m_fCameraRot	(0.0f),			// �J�����̊p�x
	m_fStickRot		(0.0f),			// �X�e�B�b�N�̊p�x
	m_sFrags		({}),			// �t�B�[���h�t���O
	m_pCurField		(nullptr),		// ���ݏ���Ă�n��
	m_pOldField		(nullptr),		// �O�����Ă��n��
	m_pEffectdata	(nullptr)		// �G�t�F�N�g���
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
	m_pOrbit		= nullptr;		// �O�Ղ̏��
	m_oldPos		= VEC3_ZERO;	// �ߋ��ʒu
	m_move			= VEC3_ZERO;	// �ړ���
	m_destRot		= VEC3_ZERO;	// �ڕW����
	m_state			= STATE_NONE;	// ���
	m_bJump			= true;			// �W�����v��
	m_nCounterState	= 0;			// ��ԊǗ��J�E���^�[
	m_nWalkCount	= 0;			// ���s���J�E���g
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

	// �O�Ղ̐���
	//m_pOrbit = COrbit::Create
	//( // ����
	//	GetParts(MODEL_BODY)->GetPtrMtxWorld(),	// �e�}�g���b�N�X
	//	ORBIT_OFFSET,	// �I�t�Z�b�g���
	//	ORBIT_PART		// ������
	//);
	//if (m_pOrbit == nullptr)
	//{ // ��g�p���̏ꍇ

	//	// ���s��Ԃ�
	//	assert(false);
	//	return E_FAIL;
	//}

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

	// �m�C�̓Q�[�W�̐���
	for (int i = 0; i < INIT_CLONE; ++i)
	{
		CTension::Create();
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayer::Uninit(void)
{
	// �O�Ղ̏I��
	SAFE_UNINIT(m_pOrbit);

	// �G�t�F�N�g�̍폜
	if (m_pEffectdata != nullptr)
	{
		SAFE_DELETE(m_pEffectdata);
		m_pEffectdata = nullptr;
	}

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

		// �������Ȃ���Ԏ��̍X�V
		currentMotion = UpdateNone(fDeltaTime);
		break;

	case STATE_SPAWN:

		// �X�|�[����Ԏ��̍X�V
		currentMotion = UpdateSpawn(fDeltaTime);
		break;

	case STATE_START:

		// �X�^�[�g��Ԏ��̍X�V
		currentMotion = UpdateStart(fDeltaTime);
		break;

	case STATE_NORMAL:

		// �ʏ��Ԃ̍X�V
		currentMotion = UpdateNormal(fDeltaTime);
		break;

	case STATE_GODITEM:

		// �_��l����Ԃ̍X�V
		currentMotion = UpdateGodItem(fDeltaTime);
		break;

	case STATE_DODGE:

		// �����Ԃ̍X�V
		currentMotion = UpdateDodge(fDeltaTime);
		break;

	case STATE_DEATH:

		// ���S��Ԃ̍X�V
		currentMotion = UpdateDeath(fDeltaTime);
		break;

	case STATE_DAMAGE:

		// �_���[�W��Ԃ̍X�V
		currentMotion = UpdateDamage(fDeltaTime);
		break;

	default:
		assert(false);
		break;
	}

	// �G�t�F�N�g�̍폜
	if (m_pEffectdata != nullptr && !m_pEffectdata->GetExist())
	{
		SAFE_DELETE(m_pEffectdata);
		m_pEffectdata = nullptr;
	}

	// �O�Ղ̍X�V
	if (m_pOrbit != nullptr) { m_pOrbit->Update(fDeltaTime); }

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	UpdateMotion(currentMotion, fDeltaTime);

#ifdef _DEBUG

	// ���͏����󂯎��|�C���^
	CInputKeyboard* pKeyboard = GET_INPUTKEY;

	if (pKeyboard->IsTrigger(DIK_RIGHT))
	{
		RecoverCheckPoint();
	}

#endif
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(CShader *pShader)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw(pShader);
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);		// ���g

}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);		// ���g
}

//============================================================
//	��������
//============================================================
CPlayer *CPlayer::Create
(
	const EType type,			// ���
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot		// ����
)
{
	// �v���C���[�̐���
	CPlayer *pPlayer = nullptr;	// �v���C���[���
	switch (type)
	{ // ��ނ��Ƃ̏���
	case TYPE_SELECT:
	case TYPE_GAME:
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

		// �Z�[�u�����擾
		const int nSave = GET_RETENTION->GetSave();

		// �ʒu��ݒ�
		if (nSave == -1 || CCheckPoint::GetList() == nullptr)
		{
			pPlayer->SetVec3Position(rPos);
		}
		else
		{
			// �`�F�b�N�|�C���g�̃��X�g���擾
			CCheckPoint* point = *CCheckPoint::GetList()->GetIndex(nSave);

			// �`�F�b�N�|�C���g�̍��W��ݒ肷��
			pPlayer->SetVec3Position(point->GetVec3Position());
		}

		if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
		{ // �Q�[�����[�h�̏ꍇ

#if 1
			// �ʒu��ݒ肷��
			D3DXVECTOR3 pos = pPlayer->GetVec3Position();
			pos.y += SPAWN_ADD_HEIGHT;
			pPlayer->SetVec3Position(pos);

#endif // 0
		}

		pPlayer->m_oldPos = rPos;	// �ߋ��ʒu������̈ʒu�ɂ���

		// ������ݒ�
		pPlayer->SetVec3Rotation(rRot);
		pPlayer->m_destRot = rRot;	// �ڕW����������̌����ɂ���

		// �J�n�G�t�F�N�g�𐶐�
		GET_EFFECT->Create
		( // ����
			"data\\EFFEKSEER\\gamestart.efkefc",	// �G�t�F�N�g�p�X
			pPlayer->GetVec3Position(),				// �ʒu
			VEC3_ZERO,	// ����
			VEC3_ZERO,	// �ړ���
			60.0f		// �g�嗦
		);

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
bool CPlayer::HitKnockBack(const int nDamage, const D3DXVECTOR3& rVecKnock)
{
	if (IsDeath()) { return false; }	// ���S�ς�
	if (m_state != STATE_NORMAL) { return false; }	// �ʏ��ԈȊO

	// �q�b�g�G�t�F�N�g���o��
	GET_EFFECT->Create("data\\EFFEKSEER\\hit.efkefc", GetVec3Position() + OFFSET_JUMP, GetVec3Rotation(), VEC3_ZERO, 250.0f);

	// �_���[�W��ԂɕύX
	m_state = STATE_DAMAGE;

	// �m�b�N�o�b�N����������
	D3DXVECTOR3 rot = GetVec3Rotation();
	rot.y = atan2f(rVecKnock.x, rVecKnock.z);
	SetVec3Rotation(rot);

	// ���񂶂Ⴄ��
	if (CTension::GetList() == nullptr || CTension::GetUseNum() == 0)
	{
		m_state = STATE_DEATH;
	}

	// �m�C�͂���������
	CTension::Vanish();

	return true;
}

//============================================================
//	�q�b�g����
//============================================================
bool CPlayer::Hit(const int nDamage)
{
	if (IsDeath())				 { return false; }	// ���S�ς�
	if (m_state != STATE_NORMAL) { return false; }	// �ʏ��ԈȊO

	// �q�b�g�G�t�F�N�g���o��
	GET_EFFECT->Create("data\\EFFEKSEER\\hit.efkefc", GetVec3Position() + OFFSET_JUMP, GetVec3Rotation(), VEC3_ZERO, 250.0f);

	// �_���[�W��ԂɕύX
	m_state = STATE_DAMAGE;

	// ���񂶂Ⴄ��
	if (CTension::GetList() == nullptr || CTension::GetUseNum() == 0)
	{
		m_state = STATE_DEATH;
	}

	// �m�C�͂���������
	CTension::Vanish();

	return true;
}

//============================================================
//	�^�C���A�b�v����
//============================================================
void CPlayer::TimeUp(void)
{
	// ��ΎE��
	m_state = STATE_DEATH;
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
//	�_��l���̐ݒ菈��
//============================================================
void CPlayer::SetEnableGodItem(const bool bGet)
{
	if (bGet)
	{ // �擾��Ԃ̐ݒ�

		// �v���C���[������ݒ�
		D3DXVECTOR3 rotDest = VEC3_ZERO;	// �ڕW����
		rotDest.y = GET_MANAGER->GetCamera()->GetDestRotation().y;	// ���[�̓J����������

		// �������J�����ڐ���
		SetDestRotation(rotDest);

		// �ړ��ʂ�������
		SetMove(VEC3_ZERO);

		// �_��l�����[�V�����ɂ���
		SetMotion(MOTION_GET);

		// �_��l����Ԃɂ���
		m_state = STATE_GODITEM;
	}
	else
	{ // �擾��Ԃ̉���

		// �ҋ@���[�V�����ɂ���
		SetMotion(MOTION_IDOL);

		// �ʏ��Ԃɂ���
		m_state = STATE_NORMAL;	// TODO�F������񂿂��ߋ���Ԃ̕���������
	}
}

//============================================================
//	���U���g�̐ݒ菈��
//============================================================
void CPlayer::SetResult()
{
	// �v���C���[������ݒ�
	D3DXVECTOR3 rotDest = VEC3_ZERO;	// �ڕW����
	rotDest.y = GET_MANAGER->GetCamera()->GetDestRotation().y;	// ���[�̓J����������

	// ������~������
	SetState(CPlayer::STATE_NONE);

	// �������J�����ڐ���
	SetDestRotation(rotDest);

	// �ړ��ʂ�������
	SetMove(VEC3_ZERO);

	// �ҋ@���[�V������ݒ�
	switch (GET_RETENTION->GetWin())
	{
	case CRetentionManager::WIN_FAIL:
		SetMotion(MOTION_DEATH);
		break;
	case CRetentionManager::WIN_SUCCESS:
		SetMotion(MOTION_IDOL);
		break;
	}
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
//  �`�F�b�N�|�C���g�ł̉񕜏���
//==========================================
void CPlayer::RecoverCheckPoint()
{
	// �m�C�͂��񕜂���
	for (int i = 0; i < HEAL_CHECKPOINT; ++i)
	{
		CTension::Create();
	}
}

//===========================================
//  ������(�t���O)�̒ǉ�
//===========================================
void CPlayer::AddFrags(const char cFrag)
{
	// ��������������ɓ������������݂�����֐��𔲂���
	if (m_sFrags.find(cFrag) != std::string::npos) { return; }

	// ������Ɏ󂯎�����t���O��ǉ�����
	m_sFrags += cFrag;
}

//=========================================
//  ������(�t���O)�̍폜
//===========================================
void CPlayer::SabFrags(const char cFrag)
{
	// ����������������ԍ����擾����
	size_t nIdx = m_sFrags.find(cFrag);

	// ��������Ƀt���O�����݂��Ȃ������ꍇ�֐��𔲂���
	if (nIdx == std::string::npos) { return; }

	// �����񂩂�t���O���폜����
	m_sFrags.erase(nIdx);
}

//============================================================
//	�������Ȃ���Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateNone(const float fDeltaTime)
{
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �d�͂̍X�V
	UpdateGravity(fDeltaTime);

	// �ʒu�X�V
	UpdatePosition(posPlayer, fDeltaTime);

	// ���n����
	UpdateLanding(posPlayer, fDeltaTime);

	// �����X�V
	UpdateRotation(rotPlayer, fDeltaTime);

	// �ǂ̓����蔻��
	GET_STAGE->CollisionWall(posPlayer, m_oldPos, RADIUS, HEIGHT, m_move, &m_bJump);

	// ��l�̕ǂ̔���
	GET_STAGE->LimitPosition(posPlayer, RADIUS);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// ���݂̃��[�V������Ԃ�
	return MOTION_IDOL;
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
		SetState(STATE_START);
	}

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
// �X�^�[�g��Ԏ��̍X�V
//============================================================
CPlayer::EMotion CPlayer::UpdateStart(const float fDeltaTime)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu���擾

	// �d�͂̍X�V
	UpdateGravity(fDeltaTime);

	// �ʒu�X�V
	UpdatePosition(pos, fDeltaTime);

	// ���n����
	UpdateLanding(pos, fDeltaTime);

	// �ǂ̓����蔻��
	GET_STAGE->CollisionWall(pos, m_oldPos, RADIUS, HEIGHT, m_move, &m_bJump);

	// ��l�̕ǂ̔���
	GET_STAGE->LimitPosition(pos, RADIUS);

	// �ʒu�𔽉f
	SetVec3Position(pos);

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
	UpdateGravity(fDeltaTime);

	// �ʒu�X�V
	UpdatePosition(posPlayer, fDeltaTime);

	// ���n����
	UpdateLanding(posPlayer, fDeltaTime);

	// �����X�V
	UpdateRotation(rotPlayer, fDeltaTime);

	// �ǂ̓����蔻��
	GET_STAGE->CollisionWall(posPlayer, m_oldPos, RADIUS, HEIGHT, m_move, &m_bJump);

	// �R�C���Ƃ̓����蔻�菈��
	CollisionCoin(posPlayer);

	// �_��Ƃ̓����蔻�菈��
	CollisionGodItem(posPlayer);

	// ��l�̕ǂ̔���
	GET_STAGE->LimitPosition(posPlayer, RADIUS);

	// �X�e�[�W�J�ڂ̍X�V
	UpdateTrans(posPlayer);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// ���g�̏���
	if(ControlClone(posPlayer, rotPlayer, fDeltaTime))
	{ currentMotion = MOTION_DODGE; }

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//===========================================
//	�_��l����Ԏ��̍X�V����
//===========================================
CPlayer::EMotion CPlayer::UpdateGodItem(const float fDeltaTime)
{
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu���擾

	// �������f
	SetVec3Rotation(m_destRot);

	// �ǂ̓����蔻��
	GET_STAGE->CollisionWall(pos, m_oldPos, RADIUS, HEIGHT, m_move, &m_bJump);

	// ��l�̕ǂ̔���
	GET_STAGE->LimitPosition(pos, RADIUS);

	// �ʒu�𔽉f
	SetVec3Position(pos);

	// �_��l�����[�V������Ԃ�
	return MOTION_GET;
}

//===========================================
//  �����Ԏ��̍X�V����
//===========================================
CPlayer::EMotion CPlayer::UpdateDodge(const float fDeltaTime)
{
	// �����̎擾
	float rot = GetVec3Rotation().y;

	// �ړ������̎Z�o
	m_move.x = sinf(rot) * DODGE_MOVE;
	m_move.z = cosf(rot) * DODGE_MOVE;

	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �G�t�F�N�g�̈ʒu��ݒ肷��
	if (m_pEffectdata != nullptr) { m_pEffectdata->m_pos = GetCenterPos(); }

	// �d�͂̍X�V
	UpdateGravity(fDeltaTime);

	// �ʒu�X�V
	UpdatePosition(pos, fDeltaTime);

	// ���n����
	UpdateLanding(pos, fDeltaTime);

	// �ǂ̓����蔻��
	GET_STAGE->CollisionWall(pos, m_oldPos, RADIUS, HEIGHT, m_move, &m_bJump);

	// ��l�̕ǂ̔���
	GET_STAGE->LimitPosition(pos, RADIUS);

	// �ʒu�𔽉f
	SetVec3Position(pos);

	return MOTION_DODGE;
}

//===========================================
//  ���S��Ԃ̍X�V����
//===========================================
CPlayer::EMotion CPlayer::UpdateDeath(const float fDeltaTime)
{
	// ���U���g���Ăяo��
	GET_GAMEMANAGER->TransitionResult(CRetentionManager::WIN_FAIL);

	DebugProc::Print(DebugProc::POINT_CENTER, "���S���\n");

	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �d�͂̍X�V
	UpdateGravity(fDeltaTime);

	// �ʒu�X�V
	UpdatePosition(pos, fDeltaTime);

	// ���n����
	UpdateLanding(pos, fDeltaTime);

	// �ǂ̓����蔻��
	GET_STAGE->CollisionWall(pos, m_oldPos, RADIUS, HEIGHT, m_move, &m_bJump);

	// ��l�̕ǂ̔���
	GET_STAGE->LimitPosition(pos, RADIUS);

	// �ʒu�𔽉f
	SetVec3Position(pos);

	// ���S���[�V����
	return MOTION_DEATH;
}

//===========================================
//  �_���[�W��Ԃ̍X�V����
//===========================================
CPlayer::EMotion CPlayer::UpdateDamage(const float fDeltaTime)
{
	// �����̎擾
	float rot = GetVec3Rotation().y;

	// �ړ������̎Z�o
	m_move.x = sinf(rot) * DAMAGE_MOVE;
	m_move.z = cosf(rot) * DAMAGE_MOVE;

	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �d�͂̍X�V
	UpdateGravity(fDeltaTime);

	// �ʒu�X�V
	UpdatePosition(pos, fDeltaTime);

	// ���n����
	UpdateLanding(pos, fDeltaTime);

	// �ǂ̓����蔻��
	GET_STAGE->CollisionWall(pos, m_oldPos, RADIUS, HEIGHT, m_move, &m_bJump);

	// ��l�̕ǂ̔���
	GET_STAGE->LimitPosition(pos, RADIUS);

	// �ʒu�𔽉f
	SetVec3Position(pos);

	// �_���[�W���[�V����
	return MOTION_DAMAGE;
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
		m_move.x = -sinf(fMoveRot) * NORMAL_MOVE * (fSpeed / SHRT_MAX);
		m_move.z = -cosf(fMoveRot) * NORMAL_MOVE * (fSpeed / SHRT_MAX);

		// ���ɏ���Ă���ꍇ�ړ��ʂ�����
		if (m_sFrags.find(CField::GetFlag(CField::TYPE_XBRIDGE)) != std::string::npos)
		{
			m_move.z = 0.0f;
		}
		if (m_sFrags.find(CField::GetFlag(CField::TYPE_ZBRIDGE)) != std::string::npos)
		{
			m_move.x = 0.0f;
		}

#ifdef _DEBUG
		if (pPad->IsPress(CInputPad::KEY_Y))
		{
			m_move.x *= 5.0f;
			m_move.z *= 5.0f;
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
void CPlayer::UpdateGravity(const float fDeltaTime)
{
	// �d�͂����Z
	m_move.y -= GRAVITY * fDeltaTime;
}

//============================================================
//	���n�󋵂̍X�V����
//============================================================
bool CPlayer::UpdateLanding(D3DXVECTOR3& rPos, const float fDeltaTime)
{
	bool bLand = false;	// ���n�t���O
	CStage *pStage = GET_STAGE;	// �X�e�[�W���
	D3DXVECTOR3 move = m_move * fDeltaTime;		//���݂̈ړ����x���ꎞ�ۑ�

	// �W�����v��Ԃ�ۑ�
	bool bJumpTemp = m_bJump;

	// �A�N�^�[�Ƃ̓����蔻��
	CollisionActor(rPos, bLand);

	// �O��̒��n�n�ʂ�ۑ�
	m_pOldField = m_pCurField;

	// �n�ʁE�����ʒu�E�A�N�^�[�̒��n����
	if (pStage->LandFieldPosition(rPos, m_oldPos, m_move, &m_pCurField)
	|| pStage->LandLimitPosition(rPos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

		// ���n���Ă����Ԃɂ���
		bLand = true;

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}

	if (m_pCurField != nullptr)
	{ // ���ݒn�ʂɒ��n���Ă���ꍇ

		// �������Ă����Ԃɂ���
		m_pCurField->Hit(this);
	}

	if (m_pCurField != m_pOldField)
	{ // �O��ƈႤ�n�ʂ̏ꍇ

		if (m_pOldField != nullptr)
		{ // �O��n�ʂɒ��n���Ă���ꍇ

			// �������Ă��Ȃ���Ԃɂ���
			m_pOldField->Miss(this);
		}
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

	// ���ۂ̃W�����v���Ăяo��
	if (!bLand && !bJumpTemp)
	{
		FloorEdgeJump();
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

	case MOTION_DASH:	// ���s���[�V����

		// �u�����h���̏ꍇ������
		if (GetMotionBlendFrame() != 0) { break; }

		if (GetMotionKey() % 2 == 0 && GetMotionKeyCounter() == 0)
		{ // ���������^�C�~���O�̏ꍇ

			// ���s����炷
			PLAY_SOUND(CSound::LABEL_SE_PLAYERWALK_000);

			// TODO�F���s�G�t�F�N�g
#if 0
			// �G�t�F�N�g���o��
			GET_EFFECT->Create("data\\EFFEKSEER\\walk.efkefc", GetVec3Position(), VEC3_ZERO, VEC3_ZERO, 250.0f);
#endif
		}

		break;

	case MOTION_STEALTHWALK:	// �E�ё����[�V����

		break;

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

	case MOTION_DODGE:	// ������[�V����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);

			// ��Ԃ̍X�V
			m_state = STATE_NORMAL;
		}

		break;

	case MOTION_DEATH:	// ���S���[�V����

		break;

	case MOTION_DAMAGE:	// �_���[�W���[�V����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);

			// ��Ԃ̍X�V
			m_state = STATE_NORMAL;
		}

		break;

	case MOTION_SAVE:	// �`�F�b�N�|�C���g���[�V����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);

			// ��Ԃ̍X�V
			m_state = STATE_NORMAL;
		}

		break;

	case MOTION_GET:	// �_��擾���[�V����
		break;

	case MOTION_START:	// �X�^�[�g���[�V����
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
//	�X�e�[�W�J�ڂ̍X�V����
//==========================================
void CPlayer::UpdateTrans(D3DXVECTOR3& rPos)
{
	CInputKeyboard*	pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputPad*		pPad = GET_INPUTPAD;	// �p�b�h���

	// �G��Ă���J�ڃ|�C���g���擾
	CTransPoint *pHitTrans = CTransPoint::Collision(rPos, RADIUS);

	// �J�ڃ|�C���g�ɐG��Ă��Ȃ��ꍇ������
	if (pHitTrans == nullptr) { return; }

	if (pKey->IsTrigger(DIK_SPACE)
	||  pKey->IsTrigger(DIK_RETURN)
	||  pPad->IsTrigger(CInputPad::KEY_A)
	||  pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X)
	||  pPad->IsTrigger(CInputPad::KEY_Y))
	{
		// �J�ڃ|�C���g�̃}�b�v�p�X�ɑJ��
		GET_STAGE->SetInitMapPass(pHitTrans->GetTransMapPass().c_str());
		GET_MANAGER->SetLoadScene(CScene::MODE_GAME);
	}
}

//==========================================
//  ���g�̏���
//==========================================
bool CPlayer::ControlClone(D3DXVECTOR3& rPos, D3DXVECTOR3& rRot, const float fDeltaTime)
{
	// ����\�t���O���m�F
	if (!m_bClone) { return false; }

	// ���͏��̎󂯎��
	CInputPad* pPad = GET_INPUTPAD;

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

	// ���g���폜
	DelelteClone();

	// �E�X�e�B�b�N�̓��͂��Ȃ��ꍇ�֐��𔲂���
	if (!pPad->GetTriggerRStick()) { return false; }

	// ����������Ăяo��
	if (Dodge(rPos, pPad))
	{
		// �ړ������̎Z�o
		m_move.x = sinf(rRot.y) * DODGE_MOVE;
		m_move.z = cosf(rRot.y) * DODGE_MOVE;

		// TOOD �G�t�F�N�g���o��
		if (m_pEffectdata == nullptr)
		{
			m_pEffectdata = GET_EFFECT->Create("data\\EFFEKSEER\\dodge.efkefc", GetCenterPos(), rRot, VEC3_ZERO, 25.0f, false, false);
		}

		// �������炷
		PLAY_SOUND(CSound::LABEL_SE_PLAYERSTEP_000);

		// �m�C�͂𑝂₷
		CTension::Create();

		// �����ԂɕύX
		m_state = STATE_DODGE;
		return true;
	}

	// �g�p�\�Ȏm�C�͂��Ȃ������ꍇ�֐��𔲂���
	if (CTension::GetUseNum() <= 0) { return false; }

	// �M�~�b�N�̒��ڐ������ł���ꍇ�֐��𔲂���
	if (CreateGimmick(fDeltaTime)) { return false; }

	// �X�e�B�b�N���͂̕������擾����
	float fRotStick = pPad->GetPressRStickRot();

	// �J�����̌������擾
	float fCameraRot = GET_MANAGER->GetCamera()->GetRotation().y;

	// �X�e�B�b�N������3D��ԂɑΉ�����
	float fTemp = -(fCameraRot + fRotStick);
	useful::NormalizeRot(fTemp);

	// ���g�̈ʒu���Z�o
	D3DXVECTOR3 pos = rPos + D3DXVECTOR3
	(
		DISTANCE_CLONE * cosf(fTemp),
		0.0f,
		DISTANCE_CLONE * sinf(fTemp)
	);

	// ���g�̈ړ��ʂ��Z�o����
	D3DXVECTOR3 move = D3DXVECTOR3
	(
		CLONE_MOVE * cosf(fTemp),
		0.0f,
		CLONE_MOVE * sinf(fTemp)
	);

	// �������g���o��
	if(CPlayerClone::Create(pos, move) == nullptr)
	{
		m_bGimmickClone = true;
		m_fGimmickTimer = 0.0f;
		m_fTempStick = fRotStick;
	}

	// ���g���ݏo������炷
	PLAY_SOUND(CSound::LABEL_SE_CLONEPOP_000);

	return false;
}

//==========================================
//  ���g���Ăі߂�
//==========================================
void CPlayer::DelelteClone()
{
	// �p�b�h�̓��͏����擾����
	CInputPad* pPad = GET_INPUTPAD;

	// �E�X�e�B�b�N�̉������݂��Ȃ������ꍇ�֐��𔲂���
	if (!pPad->IsTrigger(CInputPad::KEY_RSTICKPUSH)) { return; }

	if (CPlayerClone::GetList() != nullptr)
	{ // ���X�g�� NULL ����Ȃ��ꍇ

		// ���g��������炷
		PLAY_SOUND(CSound::LABEL_SE_CLONELOST_000);
	}

	// ���g���폜����
	CPlayerClone::Delete();
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

	// �X�e�B�b�N���͂��Ȃ��ꍇ�֐��𔲂���
	if (!pPad->GetTriggerRStick()) { return false; }

	// �O����͂Ƃ̌덷�����e�͈͊O�̏ꍇ�֐��𔲂���
	if (m_fTempStick - pPad->GetPressRStickRot() > STICK_ERROR)
	{
		m_fGimmickTimer = 0.0f;
		m_bGimmickClone = false;
		return false;
	}

	// �M�~�b�N�̃��X�g���擾
	if (CGimmickAction::GetList() == nullptr) { return false; }
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
		float fDistance = vecToGimmick.x * vecToGimmick.x + vecToGimmick.y * vecToGimmick.y + vecToGimmick.z * vecToGimmick.z;
		if (fTempDistance < fDistance) { continue; }

		// �ΏۃM�~�b�N��ۑ�����
		pGimmick = gimmick;
		fTempDistance = fDistance;
	}

	// �������߂��Ďg�p�\�Ȏm�C�͂�����Ă��Ȃ��ꍇ�֐��𔲂���
	int nNumNeed = pGimmick->GetNumActive() - pGimmick->GetNumClone();
	if (!pGimmick->CollisionPlayer() || CTension::GetUseNum() < nNumNeed) { return false; }

	// ���ڃM�~�b�N�ɂȂ镪�g��K�v������
	for (int i = 0; i < nNumNeed; ++i)
	{
		CPlayerClone::Create(pGimmick);
	}

	// �t���O�����Z�b�g���֐��𔲂���
	m_fGimmickTimer = 0.0f;
	m_bGimmickClone = false;

	return true;
}

//===========================================
//  �������
//===========================================
bool CPlayer::Dodge(D3DXVECTOR3& rPos, CInputPad* pPad)
{
	// ���X�g��null�̏ꍇfalse��Ԃ�
	if (CEnemyAttack::GetList() == nullptr) { return false; }

	// �U������G�̃��X�g���擾
	std::list<CEnemyAttack*> list = CEnemyAttack::GetList()->GetList();

	// �U���͈͂��擾
	D3DXVECTOR3 coliisionUp = CEnemyAttack::GetAttackUp();
	D3DXVECTOR3 coliisionDown = CEnemyAttack::GetAttackDown();

	// �S�Ă̓G���m�F����
	for (CEnemyAttack* enemy : list)
	{
		// ����\��ԂłȂ��ꍇ���ɐi��
		if (!enemy->IsDodge()) { continue; }

		// �{�b�N�X�̓����蔻��
		if (!collision::Box3D
		(
			rPos,						// ����ʒu
			enemy->GetVec3Position(),	// ����ڕW�ʒu
			GetVec3Sizing(),			// ����T�C�Y(�E�E��E��)
			GetVec3Sizing(),			// ����T�C�Y(���E���E�O)
			coliisionUp,				// ����ڕW�T�C�Y(�E�E��E��)
			coliisionDown				// ����ڕW�T�C�Y(���E���E�O)
		))
		{
			// �������Ă��Ȃ��ꍇ�͎��ɐi��
			continue;
		}

		// �X�e�B�b�N���͂̕������擾����
		float fRotStick = pPad->GetPressRStickRot() + D3DX_PI * 0.5f;

		// �J�����̌������擾
		float fCameraRot = GET_MANAGER->GetCamera()->GetRotation().y;

		// �X�e�B�b�N������3D��ԂɑΉ�����
		float fTemp = fRotStick - fCameraRot;
		useful::NormalizeRot(fTemp);

		// �X�e�B�b�N����������
		D3DXVECTOR3 rot = GetVec3Rotation();
		rot.y = fTemp;
		SetVec3Rotation(rot);

		// ����ɐ�����true��Ԃ�
		return true;
	}

	return false;
}

//===========================================
//  ���ۂ̃W�����v����
//===========================================
void CPlayer::FloorEdgeJump()
{
	// ��ړ��ʂ�^����
	m_move.y = JUMP_MOVE;

	// �W�����v���ɂ���
	m_bJump = true;

	// ���[�V�����̐ݒ�
	SetMotion(MOTION_JUMP_MINI, BLEND_FRAME_OTHER);

	// ���W�����v����炷
	PLAY_SOUND(CSound::LABEL_SE_PLAYERJUMP_S);
}

//==========================================
// �A�N�^�[�̓����蔻��
//==========================================
void CPlayer::CollisionActor(D3DXVECTOR3& pos, bool& rLand)
{
	bool bJump = true;

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
			bJump		// �W�����v��
		);
	}

	// �ʒu��K�p
	SetVec3Position(pos);

	if (!rLand &&
		bJump == false)
	{ // ���n�󋵂� false ���A�W�����v�󋵂� false �̏ꍇ

		// ���n����� true �ɂ���
		rLand = true;

		// �W�����v�󋵂� false �ɂ���
		m_bJump = false;
	}
}

//==========================================
// �R�C���Ƃ̓����蔻��
//==========================================
void CPlayer::CollisionCoin(const D3DXVECTOR3& pos)
{
	// �R�C���̃��X�g�\����������Δ�����
	if (CCoin::GetList() == nullptr) { return; }

	// ���X�g���擾
	std::list<CCoin*> list = CCoin::GetList()->GetList();

	for (auto coin : list)
	{
		// �����蔻�菈��
		coin->Collision
		(
			pos,		// �ʒu
			RADIUS		// ���a
		);
	}
}

//==========================================
// �_��Ƃ̓����蔻��
//==========================================
void CPlayer::CollisionGodItem(const D3DXVECTOR3& pos)
{
	// �_��̃��X�g�\����������Δ�����
	if (CGodItem::GetList() == nullptr) { return; }

	// ���X�g���擾
	std::list<CGodItem*> list = CGodItem::GetList()->GetList();

	for (auto godItem : list)
	{
		// �����蔻�菈��
		godItem->Collision
		(
			pos,		// �ʒu
			RADIUS		// ���a
		);
	}
}

//============================================================
//	�Z�b�g�A�b�v����
//============================================================
HRESULT CPlayer::LoadSetup(const char* pPass)
{
	EType type = TYPE_SELECT;		// ��ނ̑���p
	D3DXVECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	D3DXVECTOR3 rot = VEC3_ZERO;	// �����̑���p

	// �t�@�C�����J��
	std::ifstream file(pPass);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�v���C���[�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �t�@�C����Ǎ�
	std::string str;	// �Ǎ�������
	while (file >> str)
	{ // �t�@�C���̏I�[�ł͂Ȃ��ꍇ���[�v

		if (str.front() == '#')
		{ // �R�����g�A�E�g����Ă���ꍇ

			// ��s�S�ēǂݍ���
			std::getline(file, str);
		}
		else if (str == "STAGE_PLAYERSET")
		{
			do
			{ // END_STAGE_PLAYERSET��ǂݍ��ނ܂Ń��[�v

				// �������ǂݍ���
				file >> str;

				if (str == "TYPE")
				{
					file >> str;	// ����Ǎ�
					file >> str;	// ��ނ�Ǎ�

					// �������񋓂ɕϊ�
					if		(str == "SELECT")	{ type = TYPE_SELECT; }
					else if	(str == "GAME")		{ type = TYPE_GAME; }
				}
				else if (str == "POS")
				{
					file >> str;	// ����Ǎ�

					// �ʒu��Ǎ�
					file >> pos.x;
					file >> pos.y;
					file >> pos.z;
				}
				else if (str == "ROT")
				{
					file >> str;	// ����Ǎ�

					// ������Ǎ�
					file >> rot.x;
					file >> rot.y;
					file >> rot.z;
				}
			} while (str != "END_STAGE_PLAYERSET");	// END_STAGE_CHECKSET��ǂݍ��ނ܂Ń��[�v

			// �v���C���[�̐���
			if (CPlayer::Create(type, pos, D3DXToRadian(rot)) == nullptr)
			{ // �m�ۂɎ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}
		}
	}

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
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
	D3DXVECTOR3 pos = GetVec3Position();

	if (pKey->IsTrigger(DIK_I))
	{ // �O�֌W�ړ�
		if (pKey->IsTrigger(DIK_J))
		{
			fStickRot = (D3DX_PI * 0.75f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// �������g���o��
			CPlayerClone::Create(pos, move);
		}
		else if (pKey->IsTrigger(DIK_L))
		{
			fStickRot = (D3DX_PI * -0.75f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// �������g���o��
			CPlayerClone::Create(pos, move);
		}
		else
		{
			fStickRot = D3DX_PI;

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// �������g���o��
			CPlayerClone::Create(pos, move);
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
			CPlayerClone::Create(pos, move);
		}
		else if (pKey->IsTrigger(DIK_L))
		{
			fStickRot = (D3DX_PI * -0.25f);

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// �������g���o��
			CPlayerClone::Create(pos, move);
		}
		else
		{
			fStickRot = 0.0f;

			move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
			move.y = 0.0f;
			move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

			// �������g���o��
			CPlayerClone::Create(pos, move);
		}
	}
	else if (pKey->IsTrigger(DIK_J))
	{ // ���֌W�ړ�
		fStickRot = (D3DX_PI * 0.5f);

		move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
		move.y = 0.0f;
		move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

		// �������g���o��
		CPlayerClone::Create(pos, move);
	}
	else if (pKey->IsTrigger(DIK_L))
	{ // �E�֌W�ړ�
		fStickRot = (D3DX_PI * -0.5f);

		move.x = sinf(fStickRot - D3DX_PI) * 7.0f;
		move.y = 0.0f;
		move.z = cosf(fStickRot - D3DX_PI) * 7.0f;

		// �������g���o��
		CPlayerClone::Create(pos, move);
	}
}

#endif // _DEBUG
