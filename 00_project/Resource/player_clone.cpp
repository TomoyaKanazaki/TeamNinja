//============================================================
//
//	�v���C���[�̕��g���� [player_clone.cpp]
//	Author�F��������
//	Adder : ���c�E��
//	Adder : �������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player_clone.h"
#include "shadow.h"
#include "input.h"
#include "sound.h"
#include "manager.h"
#include "useful.h"
#include "player.h"
#include "orbit.h"
#include "multiModel.h"
#include "stage.h"
#include "field.h"
#include "actor.h"
#include "wall.h"
#include "gauge2D.h"

#include "collision.h"
#include "gimmick_action.h"
#include "effekseerControl.h"
#include "effekseerManager.h"

#include "camera.h"
#include "tension.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\CHARACTER\\player_clone.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

	const int	PRIORITY	= 3;		// �v���C���[�̗D�揇��
	const int	BLEND_FRAME	= 5;		// ���[�V�����̃u�����h�t���[��
	const float	GRAVITY		= 60.0f;	// �d��
	const float	RADIUS		= 20.0f;	// ���a
	const float HEIGHT		= 80.0f;	// �g��
	const float	REV_ROTA	= 0.15f;	// �����ύX�̕␳�W��
	const float	ADD_MOVE	= 0.08f;	// ��A�N�V�������̑��x���Z��
	const float	JUMP_REV	= 0.16f;	// �ʏ��Ԏ��̋󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV	= 0.16f;	// �ʏ��Ԏ��̒n��̈ړ��ʂ̌����W��
	const int	BLEND_FRAME_OTHER	= 5;		// ���[�V�����̊�{�I�ȃu�����h�t���[��
	const int	BLEND_FRAME_LAND	= 15;		// ���[�V�������n�̃u�����h�t���[��
	const float	SPAWN_ADD_ALPHA		= 0.03f;	// �X�|�[����Ԏ��̓����x�̉��Z��

	const D3DXVECTOR3 DMG_ADDROT	= D3DXVECTOR3(0.04f, 0.0f, -0.02f);	// �_���[�W��Ԏ��̃v���C���[��]��
	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// �e�̑傫��

	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), XCOL_GREEN);	// �I�t�Z�b�g���
	const int ORBIT_PART = 10;	// ������

	const float DISTANCE = 45.0f; // �v���C���[�Ƃ̋���
	const float WALK_TIMER = 10.0f; // �������Ń^�C�}�[
	const float GIMMICK_TIMER = 20.0f; // �������Ń^�C�}�[
	const float DELETE_DISTNCE = 1000.0f; // �M�~�b�N�ɂȂ��Ă��������܂ł̋���

	const float DASH_SPEED = 30.0f; // �_�b�V�����[�V�����ɂȂ鑬�x
	const float STEALTH_SPEED = 1.0f; // �E�ё����[�V�����ɂȂ鑬�x
	const float FALL_SPEED = 0.2f; // ���Ƃ����ҋ@���̈ړ����x�{��
	const float FALL_RETURN_SPEED = 5.0f; // ���Ƃ���������Ƃɖ߂�ړ����x�{��
	const float FALL = 100.0f; // ���Ƃ����ɂ�闎��
	const float FALL_DELETE = 500.0f; // ���Ƃ����ɗ����ď�����܂ł̋���
	const float GIMMICK_HEIGHT = 30.0f; // �M�~�b�N�ɔ������鍂��
	const D3DXVECTOR3 BRIDGE_SCALE = D3DXVECTOR3(1.5f, 1.5f, 1.25f); // ����Ԃł̑傫���̔{��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CPlayerClone>* CPlayerClone::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CPlayerClone] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayerClone::CPlayerClone() : CObjectChara(CObject::LABEL_CLONE, CObject::SCENE_MAIN, CObject::DIM_3D, PRIORITY),
	
	m_pOrbit		(nullptr),			// �O�Ղ̏��
	m_move			(VEC3_ZERO),		// �ړ���
	m_Action		(ACTION_MOVE),		// ���ݍs��
	m_OldAction		(ACTION_MOVE),		// �ߋ��s��
	m_fDeleteTimer	(0.0f),				// �������Ń^�C�}�[
	m_fGimmickTimer	(0.0f),				// �M�~�b�N��t���ԃ^�C�}�[
	m_pGimmick		(nullptr),			// �M�~�b�N�̃|�C���^
	m_sFrags		({}),				// �M�~�b�N�t���O�̕�����
	m_nIdxGimmick	(-1),				// �M�~�b�N���̊Ǘ��ԍ�
	m_pCurField		(nullptr),			// ���݂̒n��
	m_pOldField		(nullptr),			// �ߋ��̒n��
	m_oldPos		(VEC3_ZERO),		// �ߋ��ʒu
	m_destRot		(VEC3_ZERO),		// �ڕW����
	m_bJump			(false),			// �W�����v��
	m_fFallStart	(0.0f),				// ���Ƃ����̗�����O�̍���
	m_size			(VEC3_ZERO),		// �T�C�Y
	m_pField		(nullptr)			// �t�B�[���h�M�~�b�N
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayerClone::~CPlayerClone()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayerClone::Init(void)
{
	// �����o�ϐ���������

	m_pOrbit		= nullptr;			// �O�Ղ̏��
	m_move			= VEC3_ZERO;		// �ړ���
	m_Action		= ACTION_MOVE;		// ���ݍs��
	m_OldAction		= ACTION_MOVE;		// �ߋ��s��
	m_fDeleteTimer	= 0.0f;				// �������Ń^�C�}�[
	m_fGimmickTimer = 0.0f;				// �M�~�b�N��t���ԃ^�C�}�[
	m_pGimmick		= nullptr;			// �M�~�b�N�̃|�C���^
	m_sFrags		= {};				// �M�~�b�N�t���O�̕�����
	m_nIdxGimmick	= -1;				// �M�~�b�N���̊Ǘ��ԍ�
	m_pCurField		= nullptr;			// ���݂̒n��
	m_pOldField		= nullptr;			// �ߋ��̒n��
	m_oldPos		= VEC3_ZERO;		// �ߋ��ʒu
	m_destRot		= VEC3_ZERO;		// �ڕW����
	m_bJump			= true;				// �W�����v��
	m_size			= D3DXVECTOR3(RADIUS, HEIGHT, RADIUS);
	m_pField		= nullptr;			// �t�B�[���h�t���O

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

	// �v���C���[�ʒu�ɐݒ�
	SetVec3Position(GET_PLAYER->GetVec3Position());

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CPlayerClone>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayerClone::Uninit(void)
{
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
void CPlayerClone::Update(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����

	if (m_OldAction == ACTION_BRIDGE
	&&  m_Action != ACTION_BRIDGE)
	{
		// ���̐e���f����������
		GetParts(MODEL_WAIST)->SetParentModel(nullptr);

		// �X�P�[�������Ƃɖ߂�
		SetVec3Scaling(VEC3_ONE);

		// �Q���ׂ��Ă�������C��
		D3DXVECTOR3 rotClone = GetVec3Rotation();	// ���g�������擾
		rotClone.x = rotClone.z = 0.0f;	// Y�����ȊO�͏�����
		SetVec3Rotation(rotClone);		// �����𔽉f
	}

	// �ߋ��s���̍X�V
	m_OldAction = m_Action;

	// �ǂ̓����蔻��
	CollisionWall();

	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// �e��s�����N����
	switch (m_Action)
	{
	case ACTION_MOVE: // ���s

		// �ړ��s�����̍X�V (�ҋ@���[�V�����Ȃ�j���ς�)
		currentMotion = UpdateMove(fDeltaTime);
		if (currentMotion == MOTION_IDOL)
		{ // �j������Ă�ꍇ

			// �֐��𔲂���
			return;
		}

		// ���ݍ��W�ƑO����W����v�����ꍇ���ł��Ċ֐��𔲂���
		if (m_oldPos == GetVec3Position())
		{
			Delete(this); return;
		}

		break;

	case ACTION_FALL_TO_WAIT: // ���Ƃ����x��

		// �M�~�b�N�ҋ@��Ԃ̍X�V
		currentMotion = UpdateFallToWait(fDeltaTime);
		break;

	case ACTION_FALL: // ���Ƃ�������

		// �M�~�b�N�ҋ@��Ԃ̍X�V
		currentMotion = UpdateFall(fDeltaTime);

		// ���g�̍폜
		if (m_fFallStart - FALL_DELETE >= GetVec3Position().y)
		{
			Uninit();
			return;
		}
		break;

	case ACTION_JUMPTABLE: // �W�����v����

		// �M�~�b�N���Ԃ̍X�V
		currentMotion = UpdateJumpTable(fDeltaTime);
		break;

	case ACTION_HEAVYDOOR: // �d����

		// �d������Ԃ̍X�V
		currentMotion = UpdateHeavyDoor(fDeltaTime);
		break;

	case ACTION_STEP: // ��q

		// ��q��Ԃ̍X�V
		currentMotion = UpdateStep(fDeltaTime);
		break;

	case ACTION_BRIDGE: // ��

		// ����Ԃ̍X�V
		currentMotion = UpdateBridge(fDeltaTime);
		break;

	case ACTION_BUTTON: // �{�^��

		// �{�^����Ԃ̍X�V
		currentMotion = UpdateButton(fDeltaTime);
		break;

	case ACTION_VANISH: // ����

		// ���g�̏���
		Vanish();
		return;

	default:
		assert(false);
		break;
	}

	// �A�N�e�B�u��Ԃ̍X�V
	if (UpdateActive(fDeltaTime))
	{
		Delete(m_Action);
		return;
	}

	// �O�Ղ̍X�V
	if (m_pOrbit != nullptr) { m_pOrbit->Update(fDeltaTime); }

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	UpdateMotion(currentMotion, fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CPlayerClone::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw(pShader);
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CPlayerClone::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);		// ���g
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPlayerClone::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);		// ���g
}

//============================================================
//	�m�b�N�o�b�N�q�b�g����
//============================================================
bool CPlayerClone::HitKnockBack(const int /*nDamage*/, const D3DXVECTOR3& /*rVecKnock*/)
{
	if (IsDeath()) { return false; }	// ���S�ς�

	return true;
}

//============================================================
//	�q�b�g����
//============================================================
bool CPlayerClone::Hit(const int nDamage)
{
	if (IsDeath()) { return false; }	// ���S�ς�

	// �m�C�͂���������
	CTension::Vanish();

	// �I������
	Uninit();

	return true;
}

//==========================================
//  �M�~�b�N�̃|�C���^���擾����
//==========================================
void CPlayerClone::SetGimmick(CGimmickAction* gimmick)
{
	// �󂯎�����M�~�b�N��null�̏ꍇ�֐��𔲂���
	if (gimmick == nullptr) { return; }

	// �M�~�b�N�ŏ�������ő吔�𒴂��Ă����ꍇ�֐��𔲂���
	if (gimmick->GetNumActive() <= gimmick->GetNumClone()) { return; }

	// �������|�C���^�ɐݒ肷��
	m_pGimmick = gimmick;

	// �M�~�b�N�̑ҋ@�ʒu��ݒ肷��
	m_pGimmick->SetActionPoint(GetVec3Position());

	// �M�~�b�N�̐l�������Z
	m_pGimmick->AddNumClone();

	// ���g�Z�b�g����炷
	PLAY_SOUND(CSound::LABEL_SE_GIMMICKSET_000);

	// �M�~�b�N���ł̊Ǘ��ԍ����擾����
	m_nIdxGimmick = m_pGimmick->GetNumClone() - 1;

	// �M�~�b�N�ɑΉ������X�e�[�^�X��K�p����
	switch (m_pGimmick->GetType())
	{
	case CGimmick::TYPE_JUMPTABLE: // �W�����v��

		// �W�����v���ԂɕύX
		m_Action = ACTION_JUMPTABLE;
		break;

	case CGimmick::TYPE_HEAVYDOOR:	// �d����

		// �d������ԂɕύX
		m_Action = ACTION_HEAVYDOOR;
		break;

	case CGimmick::TYPE_STEP:		// ���ݑ�

		// ��q��ԂɕύX
		m_Action = ACTION_STEP;
		break;

	case CGimmick::TYPE_BRIDGE:		// ��

		// ����ԂɕύX
		m_Action = ACTION_BRIDGE;
		break;

	case CGimmick::TYPE_POST:		// �ݒu

		// �{�^����ԂɕύX
		m_Action = ACTION_BUTTON;
		break;

	case CGimmick::TYPE_JUMPOFF:	// ��э~��

		// TODO�F��э~��֐��̍쐬
		break;

	default: // ��O����
		assert(false);
		break;
	}

	// �M�~�b�N�^�C�}�[�������l�ɐݒ肷��
	m_fGimmickTimer = GIMMICK_TIMER;

	// �ړ��ʂ�0�ɂ���
	m_move = VEC3_ZERO;
}

//===========================================
//  �M�~�b�N�̍폜
//===========================================
void CPlayerClone::DeleteGimmick()
{
	// �M�~�b�N�����Xnull�̏ꍇ�֐��𔲂���
	if (m_pGimmick == nullptr) { return; }

	// �M�~�b�N�̑��������炷
	m_pGimmick->SabNumClone();

	// �M�~�b�N�̃|�C���^��null�ɂ���
	m_pGimmick = nullptr;
}

//===========================================
//  �t�B�[���h�̃|�C���^���擾����
//===========================================
void CPlayerClone::SetField(CField* field)
{
	// ���ɕʂ̃t�B�[���h���������Ă���ꍇ�֐��𔲂���
	if (m_pField != nullptr) { return; }

	// �������|�C���^�ɐݒ肷��
	m_pField = field;

	// �����n�t���O�̏ꍇ�x����ԂɕύX
	const char flag = m_pField->GetFlag();
	if (
		flag == CField::GetFlag(CField::TYPE_FALL) ||
		flag == CField::GetFlag(CField::TYPE_DECAYED)
		)
	{
		m_move *= FALL_SPEED;
		m_Action = ACTION_FALL_TO_WAIT;
	}
}

//===========================================
//  �t�B�[���h�̍폜
//===========================================
void CPlayerClone::DeleteField(CField* field)
{
	// �����ƌ��ݏ������Ă���t�B�[���h����v�����ꍇ�̂ݍ폜����
	if (m_pField == field) { m_pField = nullptr; }
}

//===========================================
//  ������(�t���O)�̒ǉ�
//===========================================
void CPlayerClone::AddFrags(const char cFrag)
{
	// ��������������ɓ������������݂�����֐��𔲂���
	if (m_sFrags.find(cFrag) != std::string::npos) { return; }

	// ������Ɏ󂯎�����t���O��ǉ�����
	m_sFrags += cFrag;
}

//=========================================
//  ������(�t���O)�̍폜
//===========================================
void CPlayerClone::SabFrags(const char cFrag)
{
	// ����������������ԍ����擾����
	size_t nIdx = m_sFrags.find(cFrag);

	// ��������Ƀt���O�����݂��Ȃ������ꍇ�֐��𔲂���
	if (nIdx == std::string::npos) { return; }

	// �����񂩂�t���O���폜����
	m_sFrags.erase(nIdx);
}

//===========================================
//  ������(�t���O)�̎擾
//===========================================
bool CPlayerClone::GetFrags(const char cFrag)
{
	// ��������������������ʂ�Ԃ�
	return m_sFrags.find(cFrag) != std::string::npos;
}

//==========================================
//  ��������(���s)
//==========================================
CPlayerClone* CPlayerClone::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move)
{
	// �|�C���^��錾
	CPlayerClone* pPlayer = new CPlayerClone;	// �v���C���[���

	// �����Ɏ��s�����ꍇnull��Ԃ�
	if (pPlayer == nullptr) { return nullptr; }

	// �v���C���[�̏�����
	if (FAILED(pPlayer->Init()))
	{ // �������Ɏ��s�����ꍇ

		// �v���C���[�̔j��
		SAFE_DELETE(pPlayer);
		return nullptr;
	}

	// �ʒu��ݒ�
	pPlayer->SetVec3Position(pos);

	// ������ݒ�
	D3DXVECTOR3 rot = VEC3_ZERO;		// ����
	rot.y = atan2f(-move.x, -move.z);	// �������ړ��ʂ��狁�߂�
	pPlayer->SetVec3Rotation(rot);		// �����ݒ�

	// �ړ��ʂ�ݒ�
	pPlayer->m_move = move;

	// �s����ݒ�
	pPlayer->m_Action = ACTION_MOVE;

	// �������Ń^�C�}�[��ݒ�
	pPlayer->m_fDeleteTimer = WALK_TIMER;

	// �R���g���[���̃o�C�u���[�V����
	GET_INPUTPAD->SetVibration(CInputPad::TYPE_CLONE);

	// �m�ۂ����A�h���X��Ԃ�
	return pPlayer->Block();
}

//===========================================
//  ����(���ڃM�~�b�N)
//=========================================
CPlayerClone* CPlayerClone::Create(CGimmickAction* gimmick)
{
	// �M�~�b�N��null�̏ꍇnull��Ԃ�
	if (gimmick == nullptr) { return nullptr; }

	// �|�C���^��錾
	CPlayerClone* pPlayer = new CPlayerClone;	// �v���C���[���

	// �����Ɏ��s�����ꍇnull��Ԃ�
	if (pPlayer == nullptr) { return nullptr; }

	// �v���C���[�̏�����
	if (FAILED(pPlayer->Init()))
	{ // �������Ɏ��s�����ꍇ

		// �v���C���[�̔j��
		SAFE_DELETE(pPlayer);
		return nullptr;
	}

	// �������W���v���C���[�̍��W�ɐݒ肷��
	pPlayer->SetVec3Position(GET_PLAYER->GetVec3Position());
	pPlayer->m_oldPos = GET_PLAYER->GetVec3Position();

	// �󂯎�����M�~�b�N�����蓖�Ă�
	pPlayer->SetGimmick(gimmick);

	// �m�ۂ����A�h���X��Ԃ�
	return pPlayer;
}

//===========================================
//  �S��������
//===========================================
void CPlayerClone::Delete()
{
	// ���X�g��񂪂Ȃ��ꍇ��~����
	if (m_pList == nullptr) { return; }

	// ���g�̑������擾
	int nAll = m_pList->GetNumAll();

	// ���g��S�ďI������
	for (int i = nAll - 1; i >= 0; --i)
	{
		// ���g���擾
		CPlayerClone* pClone = *m_pList->GetIndex(i);

		// ���g���������Ă���M�~�b�N���폜
		pClone->DeleteGimmick();

		// ���S���W���擾
		D3DXVECTOR3 pos = useful::GetMatrixPosition(pClone->GetParts(MODEL_WAIST)->GetMtxWorld());

		// �����̃G�t�F�N�g�𐶐�����
		GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", pos, pClone->GetVec3Rotation(), VEC3_ZERO, 25.0f);

		// ���g�̏I��
		pClone->Uninit();
	}
}

//============================================================
// ��������
//============================================================
void CPlayerClone::Delete(const int nNum)
{
	// ���X�g��񂪂Ȃ��ꍇ��~����
	if (m_pList == nullptr) { assert(false); return; }

	// ���݂̑���������̐������w�肳��Ă����ꍇ��~����
	if (m_pList->GetNumAll() <= nNum) { assert(false); return; }

	// �O�����̏ꍇ��~����
	if (0 > nNum) { assert(false); return; }

	// ���g���擾
	CPlayerClone* pClone = *m_pList->GetIndex(nNum);

	// ���g���������Ă���M�~�b�N���폜
	pClone->DeleteGimmick();

	// ���S���W���擾
	D3DXVECTOR3 pos = useful::GetMatrixPosition(pClone->GetParts(MODEL_WAIST)->GetMtxWorld());

	// �����̃G�t�F�N�g�𐶐�����
	GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", pos, pClone->GetVec3Rotation(), VEC3_ZERO, 25.0f);

	// ���g�̏I��
	SAFE_UNINIT(pClone)
}

//============================================================
//  �I���������� (����ǉ�)
//============================================================
void CPlayerClone::Delete(const EAction act)
{
	// ���X�g��񂪂Ȃ��ꍇ��~����
	if (m_pList == nullptr) { return; }

	// �������擾
	int nNum = m_pList->GetNumAll();

	// �폜���镪�g�̔ԍ���ۑ�����ϐ�
	bool* bDelete = new bool[nNum];

	// �폜�t���O�𗧂Ă�
	for (int i = 0; i < nNum; ++i)
	{
		// �폜�t���O���I�t
		bDelete[i] = false;

		// ���g���擾
		CPlayerClone* pClone = *m_pList->GetIndex(i);

		// ���g�̍폜�t���O���I��
		if (pClone->GetAction() == act)
		{
			bDelete[i] = true;
		}
	}

	// �t���O�̗����Ă��镪�g���폜����
	for (int i = nNum - 1; i >= 0; --i)
	{
		// �폜
		if (bDelete[i]) { Delete(i); }
	}

	// �폜�t���O���폜
	delete[] bDelete;
}

//===========================================
//  �I����������
//===========================================
void CPlayerClone::Delete(CPlayerClone* pClone)
{
	// ���X�g��񂪂Ȃ��ꍇ��~����
	if (pClone == nullptr) { assert(false); return; }

	// ���g�������Ă���M�~�b�N���폜
	pClone->DeleteGimmick();

	// ���S���W���擾
	D3DXVECTOR3 pos = useful::GetMatrixPosition(pClone->GetParts(MODEL_WAIST)->GetMtxWorld());

	// �����̃G�t�F�N�g�𐶐�����
	GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", pos, pClone->GetVec3Rotation(), VEC3_ZERO, 25.0f);

	// ���g�̏I��
	pClone->Uninit();
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CPlayerClone>* CPlayerClone::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//===========================================
// ���a�̎擾
//===========================================
float CPlayerClone::GetRadius()
{
	return RADIUS;
}

//===========================================
//  �g���̎擾
//===========================================
float CPlayerClone::GetHeight()
{
	return HEIGHT;
}

//============================================================
//	�S���ŏ���
//============================================================
void CPlayerClone::VanishAll(void)
{
	// ���g���X�g���Ȃ��ꍇ������
	if (m_pList == nullptr) { return; }

	std::list<CPlayerClone*> list = m_pList->GetList();	// �������X�g
	for (auto& rList : list)
	{ // �v�f�����J��Ԃ�

		// ���g�̏��ōs���ɂ���
		rList->m_Action = ACTION_VANISH;
	}
}

//============================================================
//	���g�̏��ŏ���
//============================================================
void CPlayerClone::Vanish(void)
{
	// ���S���W���擾
	D3DXVECTOR3 pos = useful::GetMatrixPosition(GetParts(MODEL_WAIST)->GetMtxWorld());

	// �����̃G�t�F�N�g�𐶐�����
	GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", pos, GetVec3Rotation(), VEC3_ZERO, 25.0f);

	// ���g�̏I��
	Uninit();
}

//============================================================
//	�ړ��s�����̍X�V����
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateMove(const float fDeltaTime)
{
	D3DXVECTOR3 posClone = GetVec3Position();	// �N���[���ʒu
	EMotion currentMotion = MOTION_DASH;		// ���݂̃��[�V����

	// �d�͂̍X�V
	UpdateGravity();

	// �ړ�
	posClone += m_move * fDeltaTime;

	// ���n����
	UpdateLanding(posClone, &currentMotion);

	// ����
	m_fDeleteTimer -= fDeltaTime;
	if (m_fDeleteTimer <= 0.0f)
	{
		// �����̃G�t�F�N�g�𐶐�����
		GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", GetVec3Position(), GetVec3Rotation(), VEC3_ZERO, 25.0f);
		Uninit();
		return MOTION_IDOL;
	}

	// �M�~�b�N�̐ڐG����
	CheckGimmick();

	// �ʒu�𔽉f
	SetVec3Position(posClone);

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//===========================================
//  ���Ƃ����x����Ԃ̍X�V����
//===========================================
CPlayerClone::EMotion CPlayerClone::UpdateFallToWait(const float fDeltaTime)
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetVec3Position();
	
	// �d��
	UpdateGravity();

	// �ړ�
	pos += m_move * fDeltaTime;

	// ���n����
	UpdateLanding(pos);

	// �ʒu�̓K�p
	SetVec3Position(pos);

	// �M�~�b�N�������Ă��Ȃ������ꍇ�֐��𔲂���
	if (m_pField == nullptr)
	{
		m_move.x *= FALL_RETURN_SPEED;
		m_move.z *= FALL_RETURN_SPEED;
		m_Action = ACTION_MOVE;
		return MOTION_DASH;
	}

	// �A�N�e�B�u��ԂɂȂ����痎�����Ċ֐��𔲂���
	if (m_pField->IsFall())
	{
		// ���Ƃ��������ɕύX
		m_Action = ACTION_FALL;

		// ����
		return MOTION_FALL;
	}

	// ������O�̍�����ۑ�
	m_fFallStart = pos.y;

	// �E�ё�
	return MOTION_STEALTHWALK;
}

//===========================================
//  ���Ƃ��������̍X�V����
//===========================================
CPlayerClone::EMotion CPlayerClone::UpdateFall(const float fDeltaTime)
{
	// �����̈ړ��ʂ����Z
	m_move.y -= FALL;

	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �ړ�
	pos += m_move * fDeltaTime;

	// �ʒu�̓K�p
	SetVec3Position(pos);

	// �E�ё�
	return MOTION_FALL;
}

//============================================================
//	�W�����v��s�����̍X�V����
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateJumpTable(const float fDeltaTime)
{
	// �ړ�
	Approach();

	// �d��
	UpdateGravity();

	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetVec3Position();

	// ���n����
	UpdateLanding(pos);

	// �쓮�����u�Ԃ̏ꍇ�W�����v��ł��グ���[�V������Ԃ�
	if (m_pGimmick->GetMoment()) { return MOTION_CATAPULT; }

	// �W�����v��ҋ@���[�V������Ԃ�
	return MOTION_JUMP_IDOL;
}

//============================================================
//	�d�����s�����̍X�V����
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateHeavyDoor(const float fDeltaTime)
{
	// �ړ�
	Approach();

	// �d��
	UpdateGravity();

	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetVec3Position();

	// ���n����
	UpdateLanding(pos);

	// �M�~�b�N�쓮���̔��グ���[�V������Ԃ�
	if (m_pGimmick->IsActive()) { return MOTION_OPEN; }

	// �W�����v��ҋ@���[�V������Ԃ�
	return MOTION_JUMP_IDOL;
}

//============================================================
//	��q�s�����̍X�V����
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateStep(const float fDeltaTime)
{
	// �ړ�
	Approach();

	// ��q���[�V������Ԃ�
	return MOTION_LADDER;
}

//============================================================
//	���s�����̍X�V����
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateBridge(const float fDeltaTime)
{
	// �ړ�
	Approach();

	// �M�~�b�N�쓮���̒�q���[�V������Ԃ�
	if (m_pGimmick->IsActive())
	{
		// �����̎��̃M�~�b�N�����g���擾
		CPlayerClone *pNextClone = GetGimmickNextClone();
		if (pNextClone != nullptr)
		{ // ���̕��g�������ꍇ

			// ���̐e���f����������
			pNextClone->GetParts(MODEL_WAIST)->SetParentModel(GetParts(MODEL_WAIST));
		}

		// �����L�����N�^�[��傫������
		SetVec3Scaling(BRIDGE_SCALE);

		// ���ɂȂ�ׂ̃��[�V������Ԃ�
		if (m_nIdxGimmick == 0)	{ SetMotion(MOTION_LADDER); return MOTION_LADDER; }	// �擪�͒�q���[�V����
		else					{ SetMotion(MOTION_BRIDGE); return MOTION_BRIDGE; }	// ����ȍ~�͋����[�V����
	}

	// �d��
	UpdateGravity();

	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetVec3Position();

	// ���n����
	UpdateLanding(pos);

	// �ҋ@���[�V������Ԃ�
	return MOTION_IDOL;
}

//============================================================
//	�{�^���s�����̍X�V����
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateButton(const float fDeltaTime)
{
	// �ړ�
	Approach();

	// �ҋ@���[�V������Ԃ�
	return MOTION_IDOL;
}

//============================================================
//	�ߋ��ʒu�̍X�V����
//============================================================
void CPlayerClone::UpdateOldPosition(void)
{
	// �ߋ��ʒu���X�V
	m_oldPos = GetVec3Position();
}

//============================================================
//	�d�͂̍X�V����
//============================================================
void CPlayerClone::UpdateGravity(void)
{
	// �d�͂����Z
	m_move.y -= GRAVITY;
}

//============================================================
//	�����̍X�V����
//============================================================
void CPlayerClone::UpdateRotation(D3DXVECTOR3& rRot)
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
//	���n�󋵂̍X�V����
//============================================================
void CPlayerClone::UpdateLanding(D3DXVECTOR3& rPos, EMotion* pCurMotion)
{
	CStage *pStage = GET_STAGE;	// �X�e�[�W���

	// TODO
	DebugProc::Print(DebugProc::POINT_RIGHT, "%d\n", m_pOldField == m_pCurField);

	// �A�N�^�[�̓����蔻��
	(void)CollisionActor(rPos);

	// �O��̒��n�n�ʂ�ۑ�
	m_pOldField = m_pCurField;

	// �n�ʁE�����ʒu�̒��n����
	if (pStage->LandFieldPosition(rPos, m_oldPos, m_move, &m_pCurField)
	||  pStage->LandLimitPosition(rPos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

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

		// �������̏ꍇ�E��
		if (m_pCurField != nullptr && m_pCurField->GetFlag() == m_pCurField->GetFlag(CField::TYPE_WATER))
		{
			// ����
			Delete(this);

			// �������̍Đ�
			PLAY_SOUND(CSound::LABEL_SE_WATERDEATH_000);

			return;
		}
	}

	if (!m_bJump)
	{ // �󒆂ɂ��Ȃ��ꍇ

		if (GetMotionType() == MOTION_FALL)
		{ // ���[�V�������������̏ꍇ

			// ���n���̍Đ�
			PLAY_SOUND(CSound::LABEL_SE_LAND_S);
		}
	}
	else
	{ // �󒆂ɂ���ꍇ

		// �������[�V�������w��
		if (pCurMotion == nullptr) { return; }
		*pCurMotion = MOTION_FALL;
	}
}

//============================================================
//	���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V����
//============================================================
void CPlayerClone::UpdateMotion(int nMotion, const float fDeltaTime)
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

	switch (GetMotionType())
	{ // ���[�V�����̎�ނ��Ƃ̏���
	case MOTION_IDOL:	// �ҋ@���[�V����
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

	case MOTION_JUMP_IDOL:	// �W�����v��ҋ@���[�V����
		break;

	case MOTION_JUMP_WALK:	// �W�����v��ړ����[�V����
		break;

	case MOTION_CATAPULT:	// �W�����v��ł��グ���[�V����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// �폜����
			Delete(this);
		}
		break;

	case MOTION_LADDER:	// ��q/�����[�V����
		break;

	case MOTION_OPEN:	// ���グ���[�V����

		if (IsMotionFinish() && GetGimmick() == nullptr)
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
bool CPlayerClone::UpdateFadeOut(const float fAdd)
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
bool CPlayerClone::UpdateFadeIn(const float fSub)
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

//===========================================
//  �A�N�e�B�u��Ԃł̏���
//===========================================
bool CPlayerClone::UpdateActive(const float fDeltaTime)
{
	// �M�~�b�N��null�̏ꍇfalse��Ԃ�
	if (m_pGimmick == nullptr) { return false; }

	// �A�N�e�B�u��ԂłȂ��ꍇfalse��Ԃ�
	if (!m_pGimmick->IsActive()) { return false; }

	// �M�~�b�N�^�C�}�[������
	m_fGimmickTimer -= fDeltaTime;

	// �M�~�b�N�^�C�}�[��0��������Ă�ꍇtrue��Ԃ�
	if (m_fGimmickTimer <= 0.0f) { return true; }

	return false;
}

//===========================================
//  �M�~�b�N�̐ڐG����
//===========================================
void CPlayerClone::CheckGimmick()
{
	// �M�~�b�N�̃��X�g�\����������Δ�����
	if (CGimmickAction::GetList() == nullptr) { return; }

	std::list<CGimmickAction*> list = CGimmickAction::GetList()->GetList();	// ���X�g���擾
	D3DXVECTOR3 pos = GetVec3Position();		// �ʒu
	D3DXVECTOR3 size = m_size * 0.5f;			// �T�C�Y
	D3DXVECTOR3 posGimmick = VEC3_ZERO;			// �M�~�b�N�̈ʒu
	D3DXVECTOR3 sizeGimmick = VEC3_ZERO;		// �M�~�b�N�̃T�C�Y

	// �S�ẴM�~�b�N���m�F����
	for (CGimmickAction* gimmick : list)
	{
		// �M�~�b�N���g�����ԂȂ玟�ɐi��
		if (gimmick->IsActive()) { continue; }

		// �ʒu���擾
		posGimmick = gimmick->GetVec3Position();

		// y���̋���������Ă����ꍇ���ɐi��
		if (fabsf(posGimmick.y - pos.y) >= GIMMICK_HEIGHT) { continue; }

		// �T�C�Y���擾
		sizeGimmick = gimmick->GetVec3Sizing() * 0.5f;

		// ��`�̊O�̏ꍇ���ɐi��
		if (!collision::Box3D
		(
			pos,			// ����ʒu
			posGimmick,		// ����ڕW�ʒu
			size,			// ����T�C�Y(�E�E��E��)
			size,			// ����T�C�Y(���E���E�O)
			sizeGimmick,	// ����ڕW�T�C�Y(�E�E��E��)
			sizeGimmick		// ����ڕW�T�C�Y(���E���E�O)
		))
		{
			continue;
		}

		// �M�~�b�N��ݒ�
		SetGimmick(gimmick);

		// ���[�v�𔲂���
		break;
	}
}

//===========================================
//  �ڕW�ʒu�Ɍ���������
//===========================================
void CPlayerClone::Approach(void)
{
#if 0
	// ���g�̈ʒu���擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �ڕW�����Ƃ̍��������߂�
	D3DXVECTOR3 vecTarget = posTarget - pos;

	// �ڕW�ւ̃x�N�g���ɔ{�����������ݒn�ɉ��Z����
	pos += vecTarget * 0.1f;
#endif
	
	// �ړ��ʂ�����������
	m_move = VEC3_ZERO;

	// �M�~�b�N�ҋ@�ʒu��K�p����
	SetVec3Position(m_pGimmick->CalcWaitPoint(m_nIdxGimmick, this));

	// �M�~�b�N�ҋ@������K�p����
	SetVec3Rotation(m_pGimmick->CalcWaitRotation(m_nIdxGimmick, this));

#if 0
	// �ړ��ʂ̃X�J���[�l���Z�o
	float fScalar = vecTarget.x * vecTarget.x + vecTarget.z * vecTarget.z;

	// �ړ��ʂ��ɒ[�ɏ������ꍇtrue��Ԃ�
	if (fScalar < DASH_SPEED * DASH_SPEED)
	{
		return true;
	}

	return false;
#endif
}

//===========================================
//  �������u���b�N���鏈��
//===========================================
CPlayerClone* CPlayerClone::Block()
{
	// ���g�̍��W���擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �O����W���v���C���[���W�ɐݒ肷��
	m_oldPos = GET_PLAYER->GetVec3Position();

	// �����ɏՓ˂����ꍇ�����������̂��폜����
	if (CollisionActor(pos) || CollisionWall())
	{
		GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", pos, GetVec3Rotation(), VEC3_ZERO, 25.0f);
		Uninit();
		return nullptr;
	}

	// �G�t�F�N�g�𐶐�
	GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_zitu_2.efkefc", pos, GetVec3Rotation(), VEC3_ZERO, 15.0f);

	// TODO�F�m�C�͂̌���ʍl���āI
#if 0
#ifndef _DEBUG
	// �m�C�͂�����
	GET_PLAYER->GetTensionGauge()->AddNum(-500);
#endif
#endif

	// �q�b�g���Ă��Ȃ���ΐ����ł���
	return this;
}

//==========================================
// �A�N�^�[�̓����蔻��
//==========================================
bool CPlayerClone::CollisionActor(D3DXVECTOR3& pos)
{
	// ����p�ϐ�
	bool bJump = true;
	bool bHit = false;

	// �A�N�^�[�̃��X�g�\����������Δ�����
	if (CActor::GetList() == nullptr) { return bHit; }

	std::list<CActor*> list = CActor::GetList()->GetList();	// ���X�g���擾

	for (CActor* actor : list)
	{
		// �����蔻�菈��
		actor->Collision
		(
			pos,		// �ʒu
			m_oldPos,	// �O��̈ʒu
			RADIUS,		// ���a
			HEIGHT,		// ����
			m_move,		// �ړ���
			bJump,		// �W�����v��
			bHit
		);
	}

	// �ʒu��K�p
	SetVec3Position(pos);

	if (bJump == false)
	{ // ���n�󋵂� false ���A�W�����v�󋵂� false �̏ꍇ

		// �W�����v�󋵂� false �ɂ���
		m_bJump = false;
	}

	// �Փ˔����Ԃ�
	return bHit;
}

//===========================================
//  �ǂƂ̓����蔻��
//===========================================
bool CPlayerClone::CollisionWall()
{
	// ���W���擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �ǂ̔���
	bool bHit = GET_STAGE->CollisionWall(pos, m_oldPos, RADIUS, HEIGHT, m_move, nullptr);

	// ��l�̕ǂ̔���
	GET_STAGE->LimitPosition(pos, RADIUS);

	// �ʒu��K�p
	SetVec3Position(pos);

	// �Փ˔����Ԃ�
	return bHit;
}

//===========================================
//	�M�~�b�N�̎��̕��g�擾
//===========================================
CPlayerClone *CPlayerClone::GetGimmickNextClone()
{
	// ���g���X�g���Ȃ��ꍇ������
	if (m_pList == nullptr) { return nullptr; }

	// �M�~�b�N���Ȃ��ꍇ������
	if (m_pGimmick == nullptr) { return nullptr; }

	// �����l���ȏ�̎��̕��g�����߂��Ă���ꍇ
	if (m_nIdxGimmick + 1 >= m_pGimmick->GetNumActive()) { return nullptr; }

	std::list<CPlayerClone*> list = m_pList->GetList();	// �������X�g
	for (auto& rList : list)
	{ // �v�f�����J��Ԃ�

		// �Ⴄ�M�~�b�N�������Ă���ꍇ������
		if (rList->m_pGimmick != m_pGimmick) { continue; }

		if (rList->m_nIdxGimmick == m_nIdxGimmick + 1)
		{ // ���̕��g���������ꍇ

			// ���g��Ԃ�
			return rList;
		}
	}

	// �����ɂ����炨������
	assert(false);
	return nullptr;
}
