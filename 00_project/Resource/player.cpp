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
#include "rankingManager.h"
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

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\CHARACTER\\player.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

	const int	PRIORITY	= 3;			// �v���C���[�̗D�揇��
	const int	BLEND_FRAME	= 5;			// ���[�V�����̃u�����h�t���[��
	const float	JUMP		= 1260.0f;		// �W�����v�㏸��
	const float	GRAVITY		= 60.0f;		// �d��
	const float	RADIUS		= 20.0f;		// ���a
	const float	REV_ROTA	= 0.15f;		// �����ύX�̕␳�W��
	const float	ADD_MOVE	= 0.08f;		// ��A�N�V�������̑��x���Z��
	const float	JUMP_REV	= 0.16f;		// �ʏ��Ԏ��̋󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV	= 0.16f;		// �ʏ��Ԏ��̒n��̈ړ��ʂ̌����W��
	const float	SPAWN_ADD_ALPHA	= 0.03f;	// �X�|�[����Ԏ��̓����x�̉��Z��

	const D3DXVECTOR3 DMG_ADDROT	= D3DXVECTOR3(0.04f, 0.0f, -0.02f);	// �_���[�W��Ԏ��̃v���C���[��]��
	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// �e�̑傫��

	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 15.0f, 0.0f), D3DXVECTOR3(0.0f, -15.0f, 0.0f), XCOL_CYAN);	// �I�t�Z�b�g���
	const int ORBIT_PART = 15;	// ������

	const float STEALTH_BORDER	= 16000.0f;	// �E�ё��ɂȂ��̃X�s�[�h
	const float	STEALTH_MOVE	= 100.0f;	// �E�ё��̈ړ���
	const float	NORMAL_MOVE		= 600.0f;	// �ʏ�̈ړ���

	const char* PARAM_FILE = "data\\TXT\\PlayerParameter.txt";

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
	m_pShadow			(nullptr),		// �e�̏��
	m_pOrbit			(nullptr),		// �O�Ղ̏��
	m_oldPos			(VEC3_ZERO),	// �ߋ��ʒu
	m_move				(VEC3_ZERO),	// �ړ���
	m_destRot			(VEC3_ZERO),	// �ڕW����
	m_state				(STATE_NONE),	// ���
	m_bJump				(false),		// �W�����v��
	m_nCounterState		(0),			// ��ԊǗ��J�E���^�[
	m_pTensionGauge		(nullptr),		// �m�C�̓Q�[�W�̃|�C���^
	m_nMaxTension		(0),			// �ő�m�C��
	m_nInitTension		(0),			// �����m�C��
	m_nSpeedTension		(0),			// �m�C�̓Q�[�W�̑������x
	m_nMaxClone			(0),			// ��x�ɕ��g�ł�����
	m_nRecover			(0),			// �W���X�g�A�N�V�����ł̉񕜗�
	m_pCheckPoint		(nullptr),		// �Z�[�u�����`�F�b�N�|�C���g
	m_fHeght			(0.0f),			// ����
	m_fInertial			(0.0f),			// ������
	m_fScalar			(0.0f),			// �ړ���
	m_fChargeTime		(0.0f)			// ���ߎ���
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
	m_pShadow			= nullptr;		// �e�̏��
	m_pOrbit			= nullptr;		// �O�Ղ̏��
	m_oldPos			= VEC3_ZERO;	// �ߋ��ʒu
	m_move				= VEC3_ZERO;	// �ړ���
	m_destRot			= VEC3_ZERO;	// �ڕW����
	m_state				= STATE_NONE;	// ���
	m_bJump				= true;			// �W�����v��
	m_nCounterState		= 0;			// ��ԊǗ��J�E���^�[
	m_pTensionGauge		= nullptr;		// �m�C�̓Q�[�W�̃|�C���^
	m_pCheckPoint		= nullptr;		// �Z�[�u�����`�F�b�N�|�C���g
	m_fScalar			= 0.0f;			// �ړ���

	// �萔�p�����[�^�̓ǂݍ���
	LoadParameter();

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
		m_nMaxTension, m_nSpeedTension, D3DXVECTOR3(300.0f, 30.0f, 0.0f),
		D3DXVECTOR3(300.0f, 30.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.56f, 0.87f, 1.0f),
		D3DXCOLOR(0.31f, 0.89f, 0.97f, 1.0f)
	);
	m_pTensionGauge->SetNum(m_nInitTension);
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
	// �ϐ���錾
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

	// �A�N�^�[�̓����蔻��
	CollisionActor();

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
	case CScene::MODE_RESULT:
	case CScene::MODE_RANKING:
		break;

	case CScene::MODE_TUTORIAL:
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
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 500.0f, 0.0f);	// �ʒu
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

	// TPS�J�����̖ڕW�ʒu�̐ݒ�
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_TPS);
	GET_MANAGER->GetCamera()->SetDestTps();
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
	return m_fHeght;
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
	float fDiff = (float)(m_nMaxTension - nTension);

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
	m_pTensionGauge->AddNum(m_nRecover);
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
	CStage *pStage = CScene::GetStage();		// �X�e�[�W���

	// �ړ�����
	currentMotion = UpdateMove();

	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(posPlayer, fDeltaTime);

	// ���n����
	UpdateLanding(posPlayer);

	// �����X�V
	UpdateRotation(rotPlayer);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posPlayer, RADIUS);

	// ���g�̏���
	ControlClone(posPlayer, rotPlayer);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// �ۑ��ʒu�̍X�V
	UpdateSaveTeleport();

	// TODO�F�G�t�F�N�g
	CEffect3D::Create(posPlayer, 10.0f);

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
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
	
	// KANAZAKI�F�E�ё��ƃ_�b�V���̔��菑��������

	// �X�e�B�b�N�̌X������ړ��ʂ�ݒ�
	float fSpeed = pPad->GetPressLStickTilt();	// �X�e�B�b�N�̌X����
	if (pad::DEAD_ZONE < fSpeed)
	{ // �f�b�h�]�[���ȏ�̏ꍇ

		if (fSpeed >= STEALTH_BORDER)
		{ // �ʏ푬�x�̏ꍇ

			// ���x��ʏ�ɂ���
			fSpeed = NORMAL_MOVE;

			// ���s���[�V�����ɂ���
			currentMotion = MOTION_DASH;
		}
		else
		{ // �E�ё��̏ꍇ

			// ���x��E�ё��ɂ���
			fSpeed = STEALTH_MOVE;

			// �E�ё����[�V�����ɂ���
			currentMotion = MOTION_STEALTHWALK;
		}

		// �X�e�B�b�N�������擾
		float fStickRot = pPad->GetPressLStickRot() - (D3DX_PI * 0.5f);

		// �ڕW������ݒ�
		m_destRot.y = fStickRot;
		useful::NormalizeRot(m_destRot.y);	// �����̐��K��

		// �ړ��ʂ�ݒ肷��
		m_move.x = sinf(fStickRot + D3DX_PI) * fSpeed;
		m_move.z = cosf(fStickRot + D3DX_PI) * fSpeed;
	}

#ifdef _DEBUG

	// �L�[�{�[�h�̈ړ�����
	DebugMoveControl();

	// �W�����v����
	DebugJumpControl();

#endif

	// �ړ��ʂ��X�J���[�l�ɕϊ�����
	m_fScalar = sqrtf(m_move.x * m_move.x + m_move.z * m_move.z);

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
	CInputKeyboard* pKey = GET_INPUTKEY;
	if (pKey->IsTrigger(DIK_Q))
	{
		// �`�F�b�N�|�C���g�ɋA��
		SaveReset();
	}
}

//============================================================
//	���n�󋵂̍X�V����
//============================================================
bool CPlayer::UpdateLanding(D3DXVECTOR3& rPos)
{
	bool bLand = false;	// ���n��
	CStage *pStage = CScene::GetStage();	// �X�e�[�W���

	// �W�����v���Ă����Ԃɂ���
	m_bJump = true;

	// TODO�F�Ȃ񂩕ρI

	// �n�ʁE�����ʒu�̒��n����
	if (pStage->LandFieldPosition(rPos, m_move)
	||  pStage->LandLimitPosition(rPos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

		// ���n���Ă����Ԃɂ���
		bLand = true;

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
	}

	// ���n�󋵂�Ԃ�
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
	m_posCenter.y += m_fHeght * 0.5f;
}

//============================================================
//	�����̍X�V����
//============================================================
void CPlayer::UpdateRotation(D3DXVECTOR3& rRot)
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
	rRot.y += fDiffRot * REV_ROTA;

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

	// �ϐ���錾
	int nAnimMotion = GetMotionType();	// ���ݍĐ����̃��[�V����

	if (nMotion != NONE_IDX)
	{ // ���[�V�������ݒ肳��Ă���ꍇ

		if (IsMotionLoop())
		{ // ���[�v���郂�[�V�����������ꍇ

			if (nAnimMotion != nMotion)
			{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(nMotion, BLEND_FRAME);
			}
		}
	}

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update(fDeltaTime);
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
void CPlayer::ControlClone(D3DXVECTOR3& rPos, D3DXVECTOR3& rRot)
{
	// TODO�F�������m�F�I

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
	if (CPlayerClone::GetList() != nullptr && CPlayerClone::GetList()->GetNumAll() >= m_nMaxClone) { return; }

	// �E�X�e�B�b�N�̓��͂��Ȃ��ꍇ�֐��𔲂���
	if (!pPad->GetTriggerRStick()) { return; }

#ifndef _DEBUG

	// �m�C�͂���������
	m_pTensionGauge->AddNum(-500);

#endif

	// �v���C���[�̕������擾
	float fRotPlayer = GetVec3Rotation().y;

	// �X�e�B�b�N���͂̕������擾����
	float fRotStick = pPad->GetPressRStickRot();

	// �X�e�B�b�N������3D��ԂɑΉ�����
	float fTemp = fRotStick - (D3DX_PI * 0.5f);
	useful::NormalizeRot(fTemp);

	// �v���C���[��������X�e�B�b�N�̕��������Z
	float fRot = fRotPlayer - fTemp;
	useful::NormalizeRot(fRot);

	// ���߂��l�ƃ΂̌덷���������ꍇ���Ă��镪�g���o���Ċ֐��𔲂���
	if (fabsf(fRot) >= D3DX_PI * 0.875f)
	{
		CPlayerClone::Create();
		GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_zitu_2.efkefc", rPos, rRot, VEC3_ZERO, 15.0f);
		return;
	}

	// ���g�̈ړ��ʂ��Z�o����
	D3DXVECTOR3 move = D3DXVECTOR3
	(
		m_fScalar * cosf(-fRotStick),
		0.0f,
		m_fScalar * sinf(-fRotStick)
	);

	// �������g���o��
	CPlayerClone::Create(move);
	GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_zitu_2.efkefc", rPos, rRot, VEC3_ZERO, 15.0f);
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
	if (pPad->IsTrigger(CInputPad::KEY_RSTICKPUSH)) { return; }

	// ���g��Ǐ]����
	CPlayerClone::CallBack();
}

//==========================================
// �A�N�^�[�̓����蔻��
//==========================================
void CPlayer::CollisionActor()
{
	// �A�N�^�[�̃��X�g�\����������Δ�����
	if (CActor::GetList() == nullptr) { return; }

	std::list<CActor*> list = CActor::GetList()->GetList();	// ���X�g���擾
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu

	for (auto actor : list)
	{
		// �����蔻�菈��
		actor->Collision
		(
			pos,		// �ʒu
			m_oldPos,	// �O��̈ʒu
			RADIUS,		// ���a
			RADIUS,		// ����
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
		m_move.y = JUMP;

		// �W�����v���ɂ����
		m_bJump = true;
	}
}

//==========================================
// �L�[�{�[�h�̈ړ����쏈��
//==========================================
void CPlayer::DebugMoveControl(void)
{
	CInputKeyboard* pKey = GET_INPUTKEY;
	float fMoveRot = 0.0f;	// �ړ�����

	if (pKey->IsPress(DIK_W))
	{
		if (pKey->IsPress(DIK_A))
		{
			// �ړ�������ݒ�
			fMoveRot = (D3DX_PI * 0.75f);

			// �ʒu��ݒ�
			m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
			m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;
		}
		else if (pKey->IsPress(DIK_D))
		{
			// �ړ�������ݒ�
			fMoveRot = (D3DX_PI * -0.75f);

			// �ʒu��ݒ�
			m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
			m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;
		}
		else
		{
			// �ړ�������ݒ�
			fMoveRot = D3DX_PI;

			// �ʒu��ݒ�
			m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
			m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;
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
			m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
			m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;
		}
		else if (pKey->IsPress(DIK_D))
		{
			// �ړ�������ݒ�
			fMoveRot = (D3DX_PI * -0.25f);

			// �ʒu��ݒ�
			m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
			m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;
		}
		else
		{
			// �ړ�������ݒ�
			fMoveRot = 0.0f;

			// �ʒu��ݒ�
			m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
			m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;
		}

		// ������ݒ�
		m_destRot.y = fMoveRot;
	}
	else if (pKey->IsPress(DIK_A))
	{
		// �ړ�������ݒ�
		fMoveRot = (D3DX_PI * 0.5f);

		// �ʒu��ݒ�
		m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
		m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;

		// ������ݒ�
		m_destRot.y = fMoveRot;
	}
	else if (pKey->IsPress(DIK_D))
	{
		// �ړ�������ݒ�
		fMoveRot = (D3DX_PI * -0.5f);

		// �ʒu��ݒ�
		m_move.x += sinf(fMoveRot - D3DX_PI) * 7.0f;
		m_move.z += cosf(fMoveRot - D3DX_PI) * 7.0f;

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

//==========================================
//  �萔�ǂݍ���
//==========================================
void CPlayer::LoadParameter()
{
	//���[�J���ϐ��錾
	FILE* pFile; // �t�@�C���|�C���^

	//�t�@�C����ǂݎ���p�ŊJ��
	pFile = fopen(PARAM_FILE, "r");

	// �t�@�C�����J���Ȃ������ꍇ
	if (pFile == NULL) { assert(false); return; }

	// ���̓ǂݍ���
	while (1)
	{
		// ������̋L�^�p
		char aStr[256];

		// ������ǂݍ���
		fscanf(pFile, "%s", &aStr[0]);

		// �������򏈗�
		if (strcmp(&aStr[0], "TENSION_MAX") == 0) // �m�C�Q�[�W�̍ő�l
		{
			// �f�[�^���i�[
			fscanf(pFile, "%d", &m_nMaxTension);
		}
		if (strcmp(&aStr[0], "TENSION_INIT") == 0) // �m�C�Q�[�W�̏����l
		{
			// �f�[�^���i�[
			fscanf(pFile, "%d", &m_nInitTension);
		}
		if (strcmp(&aStr[0], "GAUGE_SPEED") == 0) // �m�C�Q�[�W�̑������x
		{
			// �f�[�^���i�[
			fscanf(pFile, "%d", &m_nSpeedTension);
		}
		if (strcmp(&aStr[0], "MAX_CLONE") == 0) // ��x�ɕ��g�ł�����
		{
			// �f�[�^���i�[
			fscanf(pFile, "%d", &m_nMaxClone);
		}
		if (strcmp(&aStr[0], "JUST_RECOVER") == 0) // �W���X�g�A�N�V�����ł̉񕜗�
		{
			// �f�[�^���i�[
			fscanf(pFile, "%d", &m_nRecover);
		}
		if (strcmp(&aStr[0], "HEIGHT") == 0) // �����̎擾
		{
			// �f�[�^���i�[
			fscanf(pFile, "%f", &m_fHeght);
		}
		if (strcmp(&aStr[0], "INERTIAL") == 0) // �����̎擾
		{
			// �f�[�^���i�[
			fscanf(pFile, "%f", &m_fInertial);
		}
		if (strcmp(&aStr[0], "CHARGE_TIME") == 0) // ���ߎ��Ԃ̎擾
		{
			// �f�[�^���i�[
			fscanf(pFile, "%f", &m_fChargeTime);
		}
		if (strcmp(&aStr[0], "END_OF_FILE") == 0) // �ǂݍ��ݏI��
		{
			break;
		}
	}
}

#endif // _DEBUG
