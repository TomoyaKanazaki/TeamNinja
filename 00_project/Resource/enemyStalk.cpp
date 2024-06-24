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

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\enemy.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const float MOVE = -300.0f;				// �ړ���
	const float ROT_REV = 0.5f;				// �����̕␳�W��
	const float ATTACK_DISTANCE = 50.0f;	// �U������ɓ��鋗��
	const int	BLEND_FRAME_OTHER = 5;		// ���[�V�����̊�{�I�ȃu�����h�t���[��
	const int	BLEND_FRAME_LAND = 15;		// ���[�V�������n�̃u�����h�t���[��
	const int	CAUTIOUS_TRANS_LOOP = 7;	// �x�����[�V�����ɑJ�ڂ���ҋ@���[�v��

	const D3DXVECTOR3 OFFSET = D3DXVECTOR3(-3.0f, -15.0f, 0.0f);	// �I�t�Z�b�g���W
}

//************************************************************
//	�q�N���X [CEnemyStalk] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyStalk::CEnemyStalk() : CEnemy(),
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
		GetItem()->Offset(GetParts(8)->GetMtxWorld());
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
		GetParts(8)->GetMtxWorld()
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
		Crawl();

		break;

	case CEnemyStalk::STATE_STALK:

		// �ǐՏ���
		Stalk();

		break;

	case CEnemyStalk::STATE_ATTACK:

		// �U������
		nCurMotion = Attack();

		break;

	case CEnemyStalk::STATE_UPSET:
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

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
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

		// �ǐՏ�Ԃɂ���
		m_state = STATE_STALK;

		// �W�I�𕪐g�ɂ���
		m_target = TARGET_CLONE;

		// �������[�V������Ԃ�
		return MOTION_FOUND;
	}

	if (SearchPlayer(&m_posTarget))
	{ // ���g���ڂɓ������ꍇ

		// �ǐՏ�Ԃɂ���
		m_state = STATE_STALK;

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
// �ǐՏ���
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Stalk(void)
{
	// ���񏈗�
	Crawl();

	// �ړ�����
	Move();

	if (Approach())
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
// �ړ�����
//============================================================
void CEnemyStalk::Move(void)
{
	D3DXVECTOR3 pos = GetVec3Position();		// �ʒu
	D3DXVECTOR3 destRot = GetDestRotation();	// �ړI�̌���
	D3DXVECTOR3 rot = GetVec3Rotation();		// ����
	D3DXVECTOR3 move = GetMovePosition();		// �ړ���
	float fDiff;

	// �ړI�̌������擾
	destRot.y = atan2f(pos.x - m_posTarget.x, pos.z - m_posTarget.z);

	// �����̍���
	fDiff = destRot.y - rot.y;

	// �����̐��K��
	useful::NormalizeRot(fDiff);

	// ������␳
	rot.y += fDiff * ROT_REV;

	// �����̐��K��
	useful::NormalizeRot(rot.y);

	// �ړ��ʂ�ݒ肷��
	move.x = sinf(rot.y) * MOVE * GET_MANAGER->GetDeltaTime()->GetTime();
	move.z = cosf(rot.y) * MOVE * GET_MANAGER->GetDeltaTime()->GetTime();

	// �ʒu���ړ�����
	pos += move;

	// ����K�p
	SetVec3Position(pos);
	SetDestRotation(destRot);
	SetVec3Rotation(rot);
	SetMovePosition(move);
}

//============================================================
// �ڋߏ���
//============================================================
bool CEnemyStalk::Approach(void)
{
	float fDistance = 0.0f;					// ����
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu

	// �����𑪂�
	fDistance = sqrtf((pos.x - m_posTarget.x) * (pos.x - m_posTarget.x) + (pos.z - m_posTarget.z) * (pos.z - m_posTarget.z));

	if (fDistance <= ATTACK_DISTANCE)
	{ // ���̋����ɓ������ꍇ

		// �ڋ߂���
		return true;
	}

	// �ڋ߂��ĂȂ�
	return false;
}

//============================================================
// �U������
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Attack(void)
{
	switch (m_target)
	{
	case CEnemyStalk::TARGET_PLAYER:

		// �q�b�g����
		CScene::GetPlayer()->Hit(1);

		// �����Ԃɂ���
		m_state = STATE_CRAWL;

		// �ҋ@���[�V�����ɂ���
		return MOTION_IDOL;

		break;

	case CEnemyStalk::TARGET_CLONE:

		// �q�b�g����
		(*CPlayerClone::GetList()->GetBegin())->Hit(1);

		// �����Ԃɂ���
		m_state = STATE_CRAWL;

		// ���h���[�V�����ɂ���
		return MOTION_UPSET;

		break;

	default:		// ��O����
		assert(false);
		break;
	}

	// �ҋ@���[�V�����ɂ���
	return MOTION_IDOL;
}
