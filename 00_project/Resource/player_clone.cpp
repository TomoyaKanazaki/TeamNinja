//============================================================
//
//	�v���C���[�̕��g���� [player_clone.cpp]
//	Author�F���c�E��
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

#include "collision.h"
#include "gimmick_action.h"
#include "effekseerControl.h"
#include "effekseerManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *SETUP_TXT = "data\\CHARACTER\\player_clone.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X

	const int	PRIORITY	= 3;		// �v���C���[�̗D�揇��
	const int	BLEND_FRAME	= 5;		// ���[�V�����̃u�����h�t���[��
	const float	MOVE		= 150.0f;	// �ړ���
	const float	GRAVITY		= 60.0f;	// �d��
	const float	REV_ROTA	= 0.15f;	// �����ύX�̕␳�W��
	const float	ADD_MOVE	= 0.08f;	// ��A�N�V�������̑��x���Z��
	const float	JUMP_REV	= 0.16f;	// �ʏ��Ԏ��̋󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV	= 0.16f;	// �ʏ��Ԏ��̒n��̈ړ��ʂ̌����W��
	const int	BLEND_FRAME_OTHER	= 5;		// ���[�V�����̊�{�I�ȃu�����h�t���[��
	const int	BLEND_FRAME_LAND	= 15;		// ���[�V�������n�̃u�����h�t���[��
	const int	CAUTIOUS_TRANS_LOOP	= 7;		// �x�����[�V�����ɑJ�ڂ���ҋ@���[�v��
	const float	SPAWN_ADD_ALPHA		= 0.03f;	// �X�|�[����Ԏ��̓����x�̉��Z��

	const D3DXVECTOR3 DMG_ADDROT	= D3DXVECTOR3(0.04f, 0.0f, -0.02f);	// �_���[�W��Ԏ��̃v���C���[��]��
	const D3DXVECTOR3 SHADOW_SIZE	= D3DXVECTOR3(80.0f, 0.0f, 80.0f);	// �e�̑傫��

	const COrbit::SOffset ORBIT_OFFSET = COrbit::SOffset(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), XCOL_GREEN);	// �I�t�Z�b�g���
	const int ORBIT_PART = 10;	// ������

	const float DISTANCE = 45.0f; // �v���C���[�Ƃ̋���
	const float TIMER = 10.0f; // �������Ń^�C�}�[

	const float DASH_SPEED = 30.0f; // �_�b�V�����[�V�����ɂȂ鑬�x
	const float STEALTH_SPEED = 1.0f; // �E�ё����[�V�����ɂȂ鑬�x
	const float FALL_SPEED = 0.2f; // ���Ƃ����ҋ@���̈ړ����x�{��
	const float FALL = 150.0f; // ���Ƃ����ɂ�闎��
	const float FALL_DELETE = 250.0f; // ���Ƃ����ɗ����ď�����܂ł̋���
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
CPlayerClone::CPlayerClone() : CObjectChara(CObject::LABEL_AVATAR, CObject::DIM_3D, PRIORITY),
	m_pShadow		(nullptr),		// �e�̏��
	m_pOrbit		(nullptr),		// �O�Ղ̏��
	m_move			(VEC3_ZERO),	// �ړ���
	m_Action		(ACTION_CHASE),	// �s��
	m_fDeleteTimer	(0.0f),			// �������Ń^�C�}�[
	m_fChargeTimer	(0.0f),			// ���ߎ��ԃ^�C�}�[
	m_pGimmick		(nullptr),		// �M�~�b�N�̃|�C���^
	m_sFrags		({}),			// �M�~�b�N�t���O�̕�����
	m_nIdxGimmick	(-1),			// �M�~�b�N���̊Ǘ��ԍ�
	m_oldPos		(VEC3_ZERO),	// �ߋ��ʒu
	m_destRot		(VEC3_ZERO),	// �ڕW����
	m_bJump			(false),		// �W�����v��
	m_fFallStart	(0.0f)			// ���Ƃ����̗�����O�̍���
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
	m_pShadow		= nullptr;		// �e�̏��
	m_pOrbit		= nullptr;		// �O�Ղ̏��
	m_move			= VEC3_ZERO;	// �ړ���
	m_Action		= ACTION_CHASE;	// �s��
	m_fDeleteTimer	= 0.0f;			// �������Ń^�C�}�[
	m_fChargeTimer	= 0.0f;			// ���ߎ��ԃ^�C�}�[
	m_pGimmick		= nullptr;		// �M�~�b�N�̃|�C���^
	m_sFrags		= {};			// �M�~�b�N�t���O�̕�����
	m_nIdxGimmick	= -1;			// �M�~�b�N���̊Ǘ��ԍ�
	m_oldPos		= VEC3_ZERO;	// �ߋ��ʒu
	m_destRot		= VEC3_ZERO;	// �ڕW����
	m_bJump			= true;			// �W�����v��

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

	// �}�e���A����ύX
	SetAllMaterial(material::Green());

	// �T�C�Y�𒲐�
	SetVec3Scaling(D3DXVECTOR3(0.8f, 0.8f, 0.8f));

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
void CPlayerClone::Update(const float fDeltaTime)
{
	EMotion currentMotion = MOTION_IDOL;	// ���݂̃��[�V����

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

		break;

	case ACTION_CHASE: // �Ǐ]

		// �Ǐ]�s�����̍X�V
		currentMotion = UpdateChase(fDeltaTime);

		break;

	case ACTION_MOVE_TO_WAIT: // �ҋ@�ʒu�ւ̈ړ�

		// �ҋ@�ʒu�ւ̈ړ���Ԃ̍X�V
		currentMotion = UpdateMoveToWait(fDeltaTime);

		break;

	case ACTION_WAIT: // �M�~�b�N�ҋ@

		// �M�~�b�N�ҋ@��Ԃ̍X�V
		currentMotion = UpdateWait(fDeltaTime);

		break;

	case ACTION_FALL: // ���Ƃ����x��

		// �M�~�b�N�ҋ@��Ԃ̍X�V
		currentMotion = UpdateFall(fDeltaTime);

		break;

	case ACTION_JUMPTABLE: // �W�����v����

		// �M�~�b�N���Ԃ̍X�V
		currentMotion = UpdateJumpTable(fDeltaTime);

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
}

//============================================================
//	�`�揈��
//============================================================
void CPlayerClone::Draw(CShader* pShader)
{
	CToonShader* pToonShader = CToonShader::GetInstance();	// �g�D�[���V�F�[�_�[���
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
void CPlayerClone::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);		// ���g
	m_pShadow->SetEnableUpdate(bUpdate);	// �e
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPlayerClone::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);		// ���g
	m_pShadow->SetEnableDraw(bDraw);	// �e
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

	return true;
}

//==========================================
//  �M�~�b�N�̃|�C���^���擾����
//==========================================
void CPlayerClone::SetGimmick(CGimmickAction* gimmick)
{
	// �������|�C���^�ɐݒ肷��
	m_pGimmick = gimmick;

	// �M�~�b�N���ł̊Ǘ��ԍ����擾����
	m_nIdxGimmick = m_pGimmick->GetNumClone() - 1;

	// �M�~�b�N�ҋ@��ԂɂȂ�
	m_Action = ACTION_MOVE_TO_WAIT;
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
//  ������(�t���O)�̒ǉ�
//===========================================
bool CPlayerClone::GetFrags(const char cFrag)
{
	// ��������������������ʂ�Ԃ�
	return m_sFrags.find(cFrag) != std::string::npos;
}

//============================================================
//	��������
//============================================================
CPlayerClone* CPlayerClone::Create(void)
{
	// �|�C���^��錾
	CPlayerClone* pPlayer = new CPlayerClone;	// �v���C���[���
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

//==========================================
//  ��������(���s)
//==========================================
CPlayerClone* CPlayerClone::Create(const D3DXVECTOR3& move)
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

	// ������ݒ�
	D3DXVECTOR3 rot = VEC3_ZERO;		// ����
	rot.y = atan2f(-move.x, -move.z);	// �������ړ��ʂ��狁�߂�
	pPlayer->SetVec3Rotation(rot);		// �����ݒ�

	// �ړ��ʂ�ݒ�
	pPlayer->m_move = move;

	// �s����ݒ�
	pPlayer->m_Action = ACTION_MOVE;

	// �������Ń^�C�}�[��ݒ�
	pPlayer->m_fDeleteTimer = TIMER;

	// �m�ۂ����A�h���X��Ԃ�
	return pPlayer;
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
	pPlayer->m_fDeleteTimer = TIMER;

	// �m�ۂ����A�h���X��Ԃ�
	return pPlayer;
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

	// ���g���擾
	CPlayerClone* pAvatar = *m_pList->GetIndex(nNum);

	// �����̃G�t�F�N�g�𐶐�����
	GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", pAvatar->GetVec3Position(), pAvatar->GetVec3Rotation(), VEC3_ZERO, 25.0f);

	// ���g�̏I��
	pAvatar->Uninit();
}

//============================================================
//  �S�������� (����ǉ�)
//============================================================
void CPlayerClone::Delete(const EAction act)
{
	// ���X�g��񂪂Ȃ��ꍇ��~����
	if (m_pList == nullptr) { assert(false); return; }

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
		CPlayerClone* pAvatar = *m_pList->GetIndex(i);

		// ���g�̍폜�t���O���I��
		if (pAvatar->GetAction() == act)
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

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CPlayerClone>* CPlayerClone::GetList(void)
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//==========================================
//  ���g��Ǐ]�ɕύX
//==========================================
void CPlayerClone::CallBack()
{
	// ���X�g��񂪂Ȃ��ꍇ��~����
	if (m_pList == nullptr) { return; }

	// �������擾
	int nNum = m_pList->GetNumAll();

	// �Ǐ]�ƕ��s�ȊO�̍s����Ǐ]�ɕύX
	for (int i = 0; i < nNum; ++i)
	{
		// ���g���擾
		CPlayerClone* pClone = *m_pList->GetIndex(i);

		// �Ǐ]���������Ȃ��ꍇ�͎��ɐi��
		if (pClone->GetAction() == ACTION_MOVE) { continue; }
		if (pClone->GetAction() == ACTION_CHASE) { continue; }
		if (pClone->GetAction() == ACTION_FALL) { continue; }

		// �M�~�b�N�ۗ̕L���g�������炷
		pClone->m_pGimmick->SetNumClone(pClone->m_pGimmick->GetNumClone() - 1);

		// �ۑ����Ă���M�~�b�N������������
		pClone->m_pGimmick = nullptr;

		// �M�~�b�N���Ǘ��ԍ������Z�b�g����
		pClone->m_nIdxGimmick = -1;

		// �ړ��ʂ����Z�b�g����
		pClone->m_move = VEC3_ZERO;

#ifdef _DEBUG
		// �}�e���A����ύX
		pClone->SetAllMaterial(material::Green());
#endif

		// �Ǐ]��Ԃɂ���
		pClone->SetAction(ACTION_CHASE);
	}
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

	// ���n����
	UpdateLanding(posClone, &currentMotion);

	// �ړ�
	posClone += m_move * fDeltaTime;

	// ����
	m_fDeleteTimer -= fDeltaTime;
	if (m_fDeleteTimer <= 0.0f)
	{
		// �����̃G�t�F�N�g�𐶐�����
		GET_EFFECT->Create("data\\EFFEKSEER\\bunsin_del.efkefc", GetVec3Position(), GetVec3Rotation(), VEC3_ZERO, 25.0f);
		Uninit();
		return MOTION_IDOL;
	}

	// �ʒu�𔽉f
	SetVec3Position(posClone);

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	�Ǐ]�s�����̍X�V����
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateChase(const float fDeltaTime)
{
	D3DXVECTOR3 posClone = GetVec3Position();	// �N���[���ʒu
	D3DXVECTOR3 rotClone = GetVec3Rotation();	// �N���[������
	EMotion currentMotion = MOTION_DASH;		// ���݂̃��[�V����

	// �d�͂̍X�V
	UpdateGravity();

	// ��O��ǂ������āA���̍ۂ̃��[�V������Ԃ�
	currentMotion = ChasePrev(&posClone, &rotClone);

	// ���n����
	UpdateLanding(posClone, &currentMotion);

	// �ړ�
	posClone += m_move * fDeltaTime;

	// �����̍X�V
	UpdateRotation(rotClone);

	// �ʒu�𔽉f
	SetVec3Position(posClone);

	// �����𔽉f
	SetVec3Rotation(rotClone);

	// ���݂̃��[�V������Ԃ�
	return currentMotion;
}

//============================================================
//	�ҋ@�ʒu�ւ̈ړ����̍X�V����
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateMoveToWait(const float fDeltaTime)
{
	// �M�~�b�N��null�̏ꍇ�֐��𔲂���
	if (m_pGimmick == nullptr) { assert(false); return MOTION_IDOL; }

	// �M�~�b�N�����Ƃ����������ꍇ�֐��𔲂���
	if (m_pGimmick->GetType() == CGimmick::TYPE_FALL)
	{
		// �ړ��ʂ�����������
		m_move.x *= FALL_SPEED;
		m_move.z *= FALL_SPEED;

		// ���Ƃ����x����Ԃɂ���
		m_Action = ACTION_FALL;
		return MOTION_STEALTHWALK; 
	}

	// TODO�FGimmick�ړ��ǂ����悩��
#if 1
	// �M�~�b�N�̈ʒu�Ɉړ�����
	SetVec3Position(m_pGimmick->GetVec3Position());

	// �M�~�b�N�ҋ@��Ԃɂ���
	m_Action = ACTION_WAIT;
#else
	int nNumAct = m_pGimmick->GetNumActive();
	float fRate = (D3DX_PI * 2.0f) / nNumAct;

	D3DXVECTOR3 posOrigin = m_pGimmick->GetVec3Position();
	posOrigin.x += sinf(fRate * 1 + HALF_PI) * 80.0f;
	posOrigin.z += cosf(fRate * 1 + HALF_PI) * 80.0f;

	// �M�~�b�N�̈ʒu�Ɉړ�����
	SetVec3Position(posOrigin);

	// �M�~�b�N�ҋ@��Ԃɂ���
	m_Action = ACTION_WAIT;
#endif

	// �ړ����[�V������Ԃ�
	return MOTION_DASH;
}

//==========================================
//  �M�~�b�N�ҋ@
//==========================================
CPlayerClone::EMotion CPlayerClone::UpdateWait(const float fDeltaTime)
{
#ifdef _DEBUG
	// �}�e���A���J���[��ς��Ă킩��₷������
	SetAllMaterial(material::Yellow());
#endif

	// �M�~�b�N��null�̏ꍇ�֐��𔲂���
	if (m_pGimmick == nullptr) { assert(false); return MOTION_IDOL; }

	// �M�~�b�N���A�N�e�B�u��ԂȂ�
	if (!m_pGimmick->IsActive()) { return MOTION_IDOL; }

	// �M�~�b�N�ɑΉ������X�e�[�^�X��K�p����
	switch (m_pGimmick->GetType())
	{
	case CGimmick::TYPE_JUMPTABLE: // �W�����v��

		// �W�����v���ԂɕύX
		m_Action = ACTION_JUMPTABLE;

		break;

	default: // ���̑�
		break;
	}

	return MOTION_IDOL;
}

//===========================================
//  ���Ƃ����x�����̍X�V����
//===========================================
CPlayerClone::EMotion CPlayerClone::UpdateFall(const float fDeltaTime)
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos = GetVec3Position();

	// �ړ�
	pos += m_move * fDeltaTime;

	// �ʒu�̓K�p
	SetVec3Position(pos);

	// �A�N�e�B�u��ԂɂȂ����痎�����Ċ֐��𔲂���
	if (m_pGimmick->IsActive())
	{
		// �ړ��ʂ�����
		m_move.y -= FALL;

		// ����
		return MOTION_FALL;
	}

	// ���n����
	UpdateLanding(pos);

	// ������O�̍�����ۑ�
	m_fFallStart = pos.y;

	// �E�ё�
	return MOTION_STEALTHWALK;
}

//============================================================
//	�W�����v��s�����̍X�V����
//============================================================
CPlayerClone::EMotion CPlayerClone::UpdateJumpTable(const float fDeltaTime)
{
	if (m_pGimmick->GetMoment()) { return MOTION_CATAPULT; }

	return MOTION_JUMP_IDOL;
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
	CStage *pStage = CScene::GetStage();	// �X�e�[�W���

	// �n�ʁE�����ʒu�̒��n����
	if (pStage->LandFieldPosition(rPos, m_move)
	||  pStage->LandLimitPosition(rPos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJump = false;
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

	case MOTION_JUMP_IDOL:	// �W�����v�䃂�[�V����
		break;

	case MOTION_CATAPULT:	// �ł��グ���[�V����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// �W�����v�䃂�[�V�����ɑJ��
			SetMotion(MOTION_JUMP_IDOL, BLEND_FRAME_OTHER);
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

//==========================================
//  �O�ɂ��Ă�������
//==========================================
CPlayerClone::EMotion CPlayerClone::ChasePrev(D3DXVECTOR3* pPosThis, D3DXVECTOR3* pRotThis)
{
	// ���X�g���擾����
	std::list<CPlayerClone*> list = m_pList->GetList();
	auto itrBegin = list.begin();
	auto itrEnd = list.end();

	// �v���C���[�ʒu�E�������擾����
	CPlayer *pPlayer = GET_PLAYER;	// �v���C���[���
	D3DXVECTOR3 posPlayer = pPlayer->GetVec3Position();	// �v���C���[�ʒu
	D3DXVECTOR3 rotPlayer = pPlayer->GetVec3Rotation();	// �v���C���[����

	// ��O�̃|�C���^��ۑ�����ϐ�
	CPlayerClone* prev = *itrBegin;

	// ���g�̃|�C���^�𑖍�����
	for (auto itr = itrBegin; itr != itrEnd; ++itr)
	{
		// ���g�ł͂Ȃ��ꍇ��O��ۑ����Ď��ɐi��
		if (*itr != this) { prev = *itr; continue; }

		// ���g���擪�������ꍇ�v���C���[�ɒǏ]���֐��𔲂���
		if (this == *itrBegin) { return Chase(pPosThis, pRotThis, posPlayer, rotPlayer); }

		// ���g�̒Ǐ]���鑊���I������
		while (1)
		{
			// ��O�̃|�C���^��ۑ�����
			--itr;
			prev = *itr;

			// �O���Ǐ]���Ă��Ȃ��ꍇ
			if (prev->GetAction() != ACTION_CHASE)
			{
				// ��O���擪�łȂ��ꍇ���ɐi��
				if (prev != *itrBegin) { continue; }

				// �v���C���[�ɒǏ]���֐��𔲂���
				return Chase(pPosThis, pRotThis, posPlayer, rotPlayer);
			}

			// ��O�ɒǏ]���֐��𔲂���
			return Chase(pPosThis, pRotThis, prev->GetVec3Position(), prev->GetVec3Rotation());
		}
	}

	// �����ɗ�����o�O��������������낽���
	assert(false);
	return MOTION_IDOL;
}

//==========================================
//  ���Ă�������
//==========================================
CPlayerClone::EMotion CPlayerClone::Chase
(
	D3DXVECTOR3* pPosThis,			// ���g�̈ʒu
	D3DXVECTOR3* pRotThis,			// ���g�̌���
	const D3DXVECTOR3& rPosPrev,	// ���Ă�����̈ʒu
	const D3DXVECTOR3& rRotPrev		// ���Ă�����̌���
)
{
	// ��O�ɑ΂��Č��ړ�
	D3DXVECTOR3 posTarget = rPosPrev + D3DXVECTOR3
	(
		sinf(rRotPrev.y) * DISTANCE,
		0.0f,
		cosf(rRotPrev.y) * DISTANCE
	);

	// �ڕW�n�_�ւ̃x�N�g�������߂�
	D3DXVECTOR3 vecTarget = posTarget - *pPosThis;

	// �ڕW�ւ̃x�N�g���ɔ{�����������ݒn�ɉ��Z����
	vecTarget.y = 0.0f;
	*pPosThis += vecTarget * 0.1f;

	// �ڕW�̕�������������
	ViewTarget(*pPosThis, rPosPrev);

	// �ړ��ʂ̃X�J���[�l���Y�o
	float fScalar = sqrtf(vecTarget.x * vecTarget.x + vecTarget.z * vecTarget.z);
	if (fScalar > DASH_SPEED)
	{
		return MOTION_DASH;
	}
	else if (fScalar > STEALTH_SPEED)
	{
		return MOTION_STEALTHWALK;
	}
	else
	{
		return MOTION_IDOL;
	}
}

//==========================================
//  �ڕW�̕�������������
//==========================================
void CPlayerClone::ViewTarget(const D3DXVECTOR3& rPosThis, const D3DXVECTOR3& rPosPrev)
{
	// �ڕW�����Ƃ̍��������߂�
	D3DXVECTOR3 vecTarget = rPosPrev - rPosThis;

	// �����x�N�g���̌��������߂�
	float fRot = -atan2f(vecTarget.x, -vecTarget.z);

	// �ڕW������ݒ�
	m_destRot.y = fRot;
}
