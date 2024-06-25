//============================================================
//
//	�ǐՓG���� [enemyStalk.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "enemyStalk.h"
#include "renderer.h"
#include "deltaTime.h"

#include "player.h"
#include "player_clone.h"
#include "multiModel.h"
#include "enemy_item.h"

#include "collision.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\enemy.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const float MOVE = -250.0f;				// �ړ���
	const float ROT_REV = 0.5f;				// �����̕␳�W��
	const float ATTACK_DISTANCE = 50.0f;	// �U������ɓ��鋗��
	const int	BLEND_FRAME_OTHER = 5;		// ���[�V�����̊�{�I�ȃu�����h�t���[��
	const int	BLEND_FRAME_LAND = 15;		// ���[�V�������n�̃u�����h�t���[��
	const int	CAUTIOUS_TRANS_LOOP = 7;	// �x�����[�V�����ɑJ�ڂ���ҋ@���[�v��

	const D3DXVECTOR3 OFFSET = D3DXVECTOR3(-12.0f, -9.5f, 0.0f);		// �I�t�Z�b�g���W
}

//************************************************************
//	�q�N���X [CEnemyStalk] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyStalk::CEnemyStalk() : CEnemy(),
m_pClone(nullptr),			// ���g�̏��
m_posTarget(VEC3_ZERO),		// �ڕW�̈ʒu
m_target(TARGET_PLAYER),	// �W�I
m_state(STATE_CRAWL),		// ���
m_fSpeed(0.0f)				// ���x
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyStalk::~CEnemyStalk()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyStalk::Init(void)
{
	// �G�̏�����
	if (FAILED(CEnemy::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �L�����N�^�[���̊���
	BindCharaData(SETUP_TXT);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CEnemyStalk::Uninit(void)
{
	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyStalk::Update(const float fDeltaTime)
{
	// �G�̍X�V
	CEnemy::Update(fDeltaTime);

	if (GetItem() != nullptr)
	{ // �A�C�e���������Ă���ꍇ

		// �A�C�e���̃I�t�Z�b�g����
		GetItem()->Offset
		(
			GetParts(8)->GetMtxWorld(),
			GetParts(8)->GetVec3Rotation()
		);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyStalk::Draw(CShader* pShader)
{
	// �G�̕`��
	CEnemy::Draw(pShader);
}

//============================================================
// ���̐ݒ菈��
//============================================================
void CEnemyStalk::SetData(void)
{
	// �A�C�e����ݒ肷��
	SetItem(CEnemyItem::Create
	(
		CEnemyItem::TYPE_KATANA,
		OFFSET,
		GetParts(8)->GetMtxWorld(),
		GetParts(8)->GetVec3Rotation()
	));
}

//============================================================
// ��Ԃ̍X�V����
//============================================================
int CEnemyStalk::UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// ���݂̃��[�V����
	switch (m_state)
	{
	case CEnemyStalk::STATE_CRAWL:

		// ���񏈗�
		nCurMotion = Crawl();

		break;

	case CEnemyStalk::STATE_WARNING:

		// �x������
		nCurMotion = Warning();

		break;

	case CEnemyStalk::STATE_STALK:

		// �ǐՏ���
		nCurMotion = Stalk(pPos, pRot);

		break;

	case CEnemyStalk::STATE_ATTACK:

		// �U������
		nCurMotion = Attack(*pPos);

		break;

	case CEnemyStalk::STATE_UPSET:

		// ���h����
		nCurMotion = Upset();

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	// ���݂̃��[�V������Ԃ�
	return nCurMotion;
}

//============================================================
// ���[�V�����̍X�V����
//============================================================
void CEnemyStalk::UpdateMotion(int nMotion, const float fDeltaTime)
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
	}

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update(fDeltaTime);

	// ���݂̃��[�V�����̍X�V
	switch (GetMotionType())
	{
	case CEnemyStalk::MOTION_IDOL:		// �ҋ@
		break;

	case CEnemyStalk::MOTION_WALK:		// ���s
		break;

	case CEnemyStalk::MOTION_FOUND:		// ����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyStalk::MOTION_ATTACK:	// �U��

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyStalk::MOTION_UPSET:		// ���h

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// �ҋ@���[�V�����̐ݒ�
			SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);

			// �����Ԃɂ���
			m_state = STATE_CRAWL;
		}

		break;

	case CEnemyStalk::MOTION_FALL:		// ����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyStalk::MOTION_LANDING:	// ���n

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	default:

		// ��~
		assert(false);

		break;
	}
}

//============================================================
// ���n�̍X�V����
//============================================================
void CEnemyStalk::UpdateLanding(D3DXVECTOR3* pPos)
{
	// �e�N���X�̒��n�X�V
	CEnemy::UpdateLanding(pPos);

	// ���݂̃��[�V������ނ��擾
	int nCurMotion = GetMotionType();

	// �������[�V�����̃t���O��ݒ�
	bool bTypeFall = nCurMotion == MOTION_FALL;

	if (!IsJump())
	{ // �󒆂ɂ��Ȃ��ꍇ

		if (bTypeFall)
		{ // ���[�V�������������̏ꍇ

			// ���n���[�V�������w��
			SetMotion(MOTION_LANDING);
		}
	}
	else
	{ // �󒆂ɂ���ꍇ

		// �������[�V�������w��
		SetMotion(MOTION_FALL);
	}
}

//============================================================
// ���񏈗�
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Crawl(void)
{
	if (SearchClone(&m_posTarget))
	{ // ���g���ڂɓ������ꍇ

		// �x����Ԃɂ���
		m_state = STATE_WARNING;

		// �W�I�𕪐g�ɂ���
		m_target = TARGET_CLONE;

		// �������[�V������Ԃ�
		return MOTION_FOUND;
	}

	if (SearchPlayer(&m_posTarget))
	{ // ���g���ڂɓ������ꍇ

		// �x����Ԃɂ���
		m_state = STATE_WARNING;

		// �W�I���v���C���[�ɂ���
		m_target = TARGET_PLAYER;

		// �������[�V������Ԃ�
		return MOTION_FOUND;
	}

	// �����Ԃɂ���
	m_state = STATE_CRAWL;

	// �ҋ@���[�V������Ԃ�
	return MOTION_IDOL;
}

//============================================================
// �x������
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Warning(void)
{
	if (GetMotionType() != MOTION_FOUND)
	{ // �������[�V��������Ȃ������ꍇ

		// �ǐՏ�Ԃɂ���
		m_state = STATE_STALK;
	}

	// ���s��Ԃ�Ԃ�
	return MOTION_WALK;
}

//============================================================
// �ǐՏ���
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot)
{
	if (SearchClone(&m_posTarget))
	{ // ���g���ڂɓ������ꍇ

		// �W�I�𕪐g�ɂ���
		m_target = TARGET_CLONE;
	}
	else if (SearchPlayer(&m_posTarget))
	{ // ���g���ڂɓ������ꍇ

		// �W�I���v���C���[�ɂ���
		m_target = TARGET_PLAYER;
	}

	// �ړ�����
	Move(pPos, pRot);

	if (Approach(*pPos))
	{ // �ڋ߂����ꍇ

		// �U����Ԃɂ���
		m_state = STATE_ATTACK;

		// �U�����[�V������Ԃ�
		return MOTION_ATTACK;
	}

	// �f�o�b�O
	DebugProc::Print(DebugProc::POINT_RIGHT, "����!!�ړI�n�F%f %f %f", m_posTarget.x, m_posTarget.y, m_posTarget.z);

	// ���s���[�V������Ԃ�
	return MOTION_WALK;
}

//============================================================
// �U������
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Attack(const D3DXVECTOR3& rPos)
{
	switch (m_target)
	{
	case CEnemyStalk::TARGET_PLAYER:

		// �v���C���[�̓����蔻�菈��
		HitPlayer(rPos);

		if (GetMotionType() != MOTION_ATTACK)
		{ // �U�����[�V��������Ȃ��ꍇ

			// �����Ԃɂ���
			m_state = STATE_CRAWL;
		}

		// �ҋ@���[�V�����ɂ���
		return MOTION_IDOL;

		break;

	case CEnemyStalk::TARGET_CLONE:

		if (CPlayerClone::GetList() != nullptr &&
			*CPlayerClone::GetList()->GetBegin() != nullptr)
		{ // �v���C���[�̕��g�����݂��Ă���ꍇ

			// �q�b�g����
			(*CPlayerClone::GetList()->GetBegin())->Hit(1);
		}

		// ���h��Ԃɂ���
		m_state = STATE_UPSET;

		// ���h���[�V�����ɂ���
		return MOTION_UPSET;

		break;

	default:		// ��O����
		assert(false);
		break;
	}

	// �ҋ@���[�V������Ԃ�
	return MOTION_IDOL;
}

//============================================================
// ���h����
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Upset(void)
{
	// ���h���[�V�����ɂ���
	return MOTION_UPSET;
}

//============================================================
// �ړ�����
//============================================================
void CEnemyStalk::Move(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// �ړI�̌���
	D3DXVECTOR3 move = GetMovePosition();		// �ړ���
	float fDiff;

	// �ړI�̌������擾
	destRot.y = atan2f(pPos->x - m_posTarget.x, pPos->z - m_posTarget.z);

	// �����̍���
	fDiff = destRot.y - pRot->y;

	// �����̐��K��
	useful::NormalizeRot(fDiff);

	// ������␳
	pRot->y += fDiff * ROT_REV;

	// �����̐��K��
	useful::NormalizeRot(pRot->y);

	// �ړ��ʂ�ݒ肷��
	move.x = sinf(pRot->y) * MOVE * GET_MANAGER->GetDeltaTime()->GetTime();
	move.z = cosf(pRot->y) * MOVE * GET_MANAGER->GetDeltaTime()->GetTime();

	// �ʒu���ړ�����
	*pPos += move;

	// ����K�p
	SetDestRotation(destRot);
	SetMovePosition(move);
}

//============================================================
// �ڋߏ���
//============================================================
bool CEnemyStalk::Approach(const D3DXVECTOR3& rPos)
{
	float fDistance = 0.0f;					// ����

	// �����𑪂�
	fDistance = sqrtf((rPos.x - m_posTarget.x) * (rPos.x - m_posTarget.x) + (rPos.z - m_posTarget.z) * (rPos.z - m_posTarget.z));

	if (fDistance <= ATTACK_DISTANCE)
	{ // ���̋����ɓ������ꍇ

		// �ڋ߂���
		return true;
	}

	// �ڋ߂��ĂȂ�
	return false;
}

//====================================================================================================================================================================================
// TODO�F�������牺�͂��Z���J�X���肾�����ŏC��
//====================================================================================================================================================================================

//============================================================
// �v���C���[�̃q�b�g����
//============================================================
void CEnemyStalk::HitPlayer(const D3DXVECTOR3& rPos)
{
	// �q�b�g����
	D3DXVECTOR3 posPlayer = CScene::GetPlayer()->GetVec3Position();
	D3DXVECTOR3 sizeUpPlayer =
	{
		CScene::GetPlayer()->GetRadius(),
		CScene::GetPlayer()->GetHeight(),
		CScene::GetPlayer()->GetRadius()
	};
	D3DXVECTOR3 sizeDownPlayer =
	{
		CScene::GetPlayer()->GetRadius(),
		0.0f,
		CScene::GetPlayer()->GetRadius()
	};

	// �{�b�N�X�̓����蔻��
	if (collision::Box3D
	(
		rPos,			// ����ʒu
		posPlayer,		// ����ڕW�ʒu
		D3DXVECTOR3(30.0f,100.0f,30.0f),	// ����T�C�Y(�E�E��E��)
		D3DXVECTOR3(30.0f, 0.0f, 30.0f),	// ����T�C�Y(���E���E�O)
		sizeUpPlayer,	// ����ڕW�T�C�Y(�E�E��E��)
		sizeDownPlayer	// ����ڕW�T�C�Y(���E���E�O)
	))
	{ // ������ɓ������ꍇ

		// �q�b�g����
		CScene::GetPlayer()->Hit(20);
	}
}

//============================================================
// ���g�̃q�b�g����
//============================================================
void CEnemyStalk::HitClone(const D3DXVECTOR3& rPos)
{

}