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
#include "deltaTime.h"

#include "multiModel.h"
#include "orbit.h"
#include "shadow.h"
#include "orbit.h"
#include "object2D.h"
#include "rankingManager.h"
#include "stage.h"
#include "field.h"
#include "cloneAngleUI.h"

#include "effect3D.h"
#include "particle3D.h"

#include "input.h"
#include "player_clone.h"
#include "checkpoint.h"

#include "gauge2D.h"
#include "blur.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\CHARACTER\\player.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

	const int	PRIORITY	= 3;			// �v���C���[�̗D�揇��
	const float	JUMP		= 21.0f;		// �W�����v�㏸��
	const float	GRAVITY		= 1.0f;			// �d��
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

	const float STEALTH_BORDER	= 15000.0f;	// �E�ё��ɂȂ��̃X�s�[�h
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
	m_bCreateClone		(false),		// ���g�������[�h�t���O
	m_nNumClone			(0),			// �������镪�g�̐�
	m_nMaxClone			(0),			// ��x�ɕ��g�ł�����
	m_nRecover			(0),			// �W���X�g�A�N�V�����ł̉񕜗�
	m_pCheckPoint		(nullptr),		// �Z�[�u�����`�F�b�N�|�C���g
	m_fHeght			(0.0f),			// ����
	m_fInertial			(0.0f),			// ������
	m_pCloneAngleUI		(nullptr),		// ���g�o��������UI
	m_fMove				(0.0f)			// �ړ���
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
	m_bCreateClone		= false;		// ���g�������[�h�t���O
	m_nNumClone			= 0;			// �������镪�g�̐�
	m_pCheckPoint		= nullptr;		// �Z�[�u�����`�F�b�N�|�C���g
	m_pCloneAngleUI		= nullptr;		// ���g�o��������UI
	m_fMove				= 0.0f;			// �ړ���

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

	// ���g�o��������UI�̐���
	m_pCloneAngleUI = CCloneAngleUI::Create(GetVec3Position());
	if (m_pCloneAngleUI == nullptr)
	{ // ��g�p���̏ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

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

	// �u���[�̏��
	D3DXMATERIAL mat = material::GlowCyan();	// �u���[�}�e���A��
	//CBlur::Create
	( // ����
		this,	// �e�I�u�W�F�N�g
		mat,	// �u���[�}�e���A��
		blurInfo::START_ALPHA,	// �u���[�J�n�����x
		blurInfo::MAX_LENGTH	// �ێ��I�u�W�F�N�g�ő吔
	);

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

	// ���g�o��������UI�̏I��
	SAFE_UNINIT(m_pCloneAngleUI);

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
		currentMotion = UpdateSpawn();

		break;

	case STATE_NORMAL:

		// �ʏ��Ԃ̍X�V
		currentMotion = UpdateNormal();

		break;

	default:
		assert(false);
		break;
	}

	// �e�̍X�V
	m_pShadow->Update(fDeltaTime);

	// �O�Ղ̍X�V
	m_pOrbit->Update(fDeltaTime);

	// ���g�o��������UI�̃Z�b�g�A�b�v����
	CloneAngleUISetUp();

	// ���g�o��������UI�̍X�V
	m_pCloneAngleUI->Update(fDeltaTime);

	// ����
	Move();

	// ���g�̏���
	ControlClone();

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	UpdateMotion(currentMotion, fDeltaTime);

	// �f�o�b�O�\��
	DebugProc::Print(DebugProc::POINT_RIGHT, "�m�C�� : %d\n", m_pTensionGauge->GetNum());
	DebugProc::Print(DebugProc::POINT_RIGHT, "�������镪�g : %d", m_nNumClone);

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
bool CPlayer::HitKnockBack(const int /*nDamage*/, const D3DXVECTOR3& /*rVecKnock*/)
{
	if (IsDeath())				 { return false; }	// ���S�ς�
	if (m_state != STATE_NORMAL) { return false; }	// �ʏ��ԈȊO

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
	// �ϐ���錾
	D3DXVECTOR3 set = VEC3_ZERO;	// �����ݒ�p

	// ����������
	SetState(STATE_SPAWN);	// �X�|�[����Ԃ̐ݒ�
	SetMotion(MOTION_IDOL);	// �ҋ@���[�V������ݒ�

	// �J�E���^�[��������
	m_nCounterState = 0;	// ��ԊǗ��J�E���^�[

	// �ʒu��ݒ�
	SetVec3Position(set);

	// ������ݒ�
	SetVec3Rotation(set);
	m_destRot = set;

	// �ړ��ʂ�������
	m_move = VEC3_ZERO;

	// �}�e���A�����Đݒ�
	ResetMaterial();

	// �����x�𓧖��ɍĐݒ�
	SetAlpha(0.0f);

	// �`����ĊJ
	SetEnableDraw(true);

	// �Ǐ]�J�����̖ڕW�ʒu�̐ݒ�
	GET_MANAGER->GetCamera()->SetState(CCamera::STATE_TPS);
	GET_MANAGER->GetCamera()->SetDestFollow();
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

//==========================================
//  �J�[�\���ʒu�̎擾
//==========================================
D3DXVECTOR3 CPlayer::GetTargetPos() const
{
	return m_pCloneAngleUI->GetVec3Position();
}

//============================================================
//	�X�|�[����Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateSpawn(void)
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
CPlayer::EMotion CPlayer::UpdateNormal(void)
{
	// �ϐ���錾
	EMotion currentMotion = MOTION_IDOL;		// ���݂̃��[�V����
	D3DXVECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �|�C���^��錾
	CStage *pStage = CScene::GetStage();	// �X�e�[�W���
	if (pStage == nullptr)
	{ // �X�e�[�W���g�p����Ă��Ȃ��ꍇ

		// �����𔲂���
		assert(false);
		return MOTION_IDOL;
	}

	// �ړ�����
	currentMotion = UpdateMove();

	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(posPlayer);

	// ���n����
	UpdateLanding(posPlayer);

	// �����X�V
	UpdateRotation(rotPlayer);

	// �X�e�[�W�͈͊O�̕␳
	pStage->LimitPosition(posPlayer, RADIUS);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// �`�F�b�N�|�C���g�ɋA��
	CInputKeyboard* pKey = GET_INPUTKEY;
	if (pKey->IsTrigger(DIK_Q))
	{
		SaveReset();
	}

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
	// �ҋ@���[�V������Ԃ�
	return MOTION_IDOL;
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
//	���n�󋵂̍X�V����
//============================================================
bool CPlayer::UpdateLanding(D3DXVECTOR3& rPos)
{
	// �ϐ���錾
	bool bLand = false;	// ���n��

	// �W�����v���Ă����Ԃɂ���
	m_bJump = true;

	// �n�ʁE�����ʒu�̒��n����
	if (CScene::GetStage()->LandFieldPosition(rPos, m_move)
	||  CScene::GetStage()->LandLimitPosition(rPos, m_move, 0.0f))
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
void CPlayer::UpdatePosition(D3DXVECTOR3& rPos)
{
	// �ړ��ʂ����Z
	rPos += m_move * GET_MANAGER->GetDeltaTime()->GetTime();

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
//	���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V����
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
				SetMotion(nMotion);
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
// ���g�o��������UI�̃Z�b�g�A�b�v����
//==========================================
void CPlayer::CloneAngleUISetUp(void)
{
	CInputPad* pPad = GET_INPUTPAD;			// ���͏����擾
	float fSpeed = pPad->GetPressRStickTilt();						// �X�e�B�b�N�̌X��
	float fStickRot = pPad->GetPressRStickRot() + GET_MANAGER->GetCamera()->GetRotation().y + (D3DX_PI * 0.5f);	// �X�e�B�b�N�̌���
	D3DXVECTOR3 pos = GetVec3Position();	// �v���C���[�̈ʒu

	// ���͂��Ă��Ȃ��ƕ\��������
	if (fSpeed == 0.0f) { m_pCloneAngleUI->SetEnableDraw(false); return; }

	// ���g�o�������UI��\������
	m_pCloneAngleUI->SetEnableDraw(true);

	// �����̐��K��
	useful::NormalizeRot(fStickRot);

	// �ʒu��ݒ�
	m_pCloneAngleUI->SetVec3Position
	(
		D3DXVECTOR3
		(
			pos.x + sinf(fStickRot) * 70.0f,
			pos.y + 10.0f,
			pos.z + cosf(fStickRot) * 70.0f
		)
	);

	// ������ݒ�
	m_pCloneAngleUI->SetVec3Rotation
	(
		D3DXVECTOR3
		(
			0.0f,
			fStickRot,
			0.0f
		)
	);
}

//==========================================
//  ���쏈��
//==========================================
void CPlayer::Move()
{
	// �^���̑��@��
	Inertial();

	// ���͏��̎擾
	CInputPad* pPad = GET_INPUTPAD;
	D3DXVECTOR3 CameraRot = GET_MANAGER->GetCamera()->GetRotation();
	
	float fSpeed = pPad->GetPressLStickTilt();		// �X�e�B�b�N�̌X��
	float fStickRot = pPad->GetPressLStickRot() - (D3DX_PI * 0.5f);		// �X�e�B�b�N�̌���

	// ���͂��Ă��Ȃ��Ɣ�����
	if (fSpeed == 0.0f) { m_move = VEC3_ZERO; return; }

	if (fSpeed >= STEALTH_BORDER)
	{ // �ʏ푬�x�̏ꍇ

		// ���x��ʏ�ɂ���
		fSpeed = NORMAL_MOVE;
	}
	else
	{ // �E�ё��̏ꍇ

		// ���x��E�ё��ɂ���
		fSpeed = STEALTH_MOVE;
	}

	// �����ɃJ�����̌��������Z����
	fStickRot += CameraRot.y;

	// �����̐��K��
	useful::NormalizeRot(fStickRot);

	// ������ݒ�
	m_destRot.y = fStickRot;

	// �����̐��K��
	useful::NormalizeRot(m_destRot.y);

	// �ړ��ʂ�ݒ肷��
	m_move.x = sinf(fStickRot + D3DX_PI) * fSpeed;
	m_move.z = cosf(fStickRot + D3DX_PI) * fSpeed;

	// �ړ��ʂ��X�J���[�l�ɕϊ�����
	m_fMove = sqrtf(m_move.x * m_move.x + m_move.z * m_move.z);
}

//==========================================
//  �����̖@��
//==========================================
void CPlayer::Inertial()
{
	// x�������̊���
	m_move.x += (0.0f - m_move.x) * m_fInertial;

	// z�������̊���
	m_move.z += (0.0f - m_move.z) * m_fInertial;
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

//==========================================
//  ���g�̏���
//==========================================
void CPlayer::ControlClone()
{
	// ���͏��̎󂯎��
	CInputPad* pPad = GET_INPUTPAD;

	// �E�X�e�B�b�N�̓���
	if (pPad->GetTriggerRStick())
	{
		// ���g�����݂��Ă��Ȃ��ꍇ���������߂ĕ��g���o��
		if (CPlayerClone::GetList() == nullptr)
		{
			CPlayerClone::Create(m_fChargeTime);
		}
		else // ���݂��Ă����ꍇ�͒Ǐ]���镪�g
		{
			CPlayerClone::Create();
		}
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
