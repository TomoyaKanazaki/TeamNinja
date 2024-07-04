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

#include "multiModel.h"
#include "enemyNavigation.h"
#include "enemy_item.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\enemy.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const int	BLEND_FRAME_OTHER = 5;		// ���[�V�����̊�{�I�ȃu�����h�t���[��
	const int	BLEND_FRAME_LAND = 15;		// ���[�V�������n�̃u�����h�t���[��
	const int	CAUTIOUS_TRANS_LOOP = 7;	// �x�����[�V�����ɑJ�ڂ���ҋ@���[�v��
	const float	RADIUS = 20.0f;				// ���a
	const float HEIGHT = 80.0f;				// �g��
	const float SPEED = -290.0f;			// ���x
	const float ROT_REV = 0.5f;				// �����̕␳�W��

	const int ITEM_PART_NUMBER = 8;			// �A�C�e�������p�[�c�̔ԍ�
	const D3DXVECTOR3 ITEM_OFFSET = D3DXVECTOR3(-3.0f, -1.0f, 10.0f);		// �A�C�e���̃I�t�Z�b�g���W
	const D3DXVECTOR3 ITEM_ROT = D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f);	// �A�C�e���̌���
}

//************************************************************
//	�q�N���X [CEnemyStalk] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyStalk::CEnemyStalk() : CEnemyAttack(),
m_pNav(nullptr),
m_state(STATE_CRAWL)
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
	if (FAILED(CEnemyAttack::Init()))
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
	if (m_pNav != nullptr)
	{ // �i�r�� NULL ����Ȃ��ꍇ

		// �i�r�̏I������
		m_pNav->Uninit();
		m_pNav = nullptr;
	}

	// �G�̏I��
	CEnemyAttack::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyStalk::Update(const float fDeltaTime)
{
	// �G�̍X�V
	CEnemyAttack::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyStalk::Draw(CShader* pShader)
{
	// �G�̕`��
	CEnemyAttack::Draw(pShader);
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
		ITEM_OFFSET,
		ITEM_ROT
	));

	// �e�I�u�W�F�N�g (������) �̐ݒ�
	GetItem()->SetParentObject(GetParts(ITEM_PART_NUMBER));

	// �i�r�Q�[�V�����𐶐�
	m_pNav = CEnemyNav::Create(GetVec3Position(), 700.0f, 700.0f);
}

//============================================================
// ���a�̎擾����
//============================================================
float CEnemyStalk::GetRadius(void) const
{
	// ���a��Ԃ�
	return RADIUS;
}

//============================================================
// �����̎擾����
//============================================================
float CEnemyStalk::GetHeight(void) const
{
	// �g����Ԃ�
	return HEIGHT;
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
		nCurMotion = Crawl(pPos, pRot, fDeltaTime);

		break;

	case CEnemyStalk::STATE_WARNING:

		// �x������
		nCurMotion = Warning();

		break;

	case CEnemyStalk::STATE_STALK:

		// �ǐՏ���
		nCurMotion = Stalk(pPos, pRot, fDeltaTime);

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

	if (Collision(*pPos))
	{ // �����蔻�肪 true �������ꍇ

		// ��~��Ԃɂ���
		m_pNav->SetState(CEnemyNav::STATE_STOP);
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
CEnemyStalk::EMotion CEnemyStalk::Crawl(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	D3DXVECTOR3 rotDest = GetDestRotation();	// �ړI�̌���
	D3DXVECTOR3 Move = GetMovePosition();

	if (m_pNav != nullptr)
	{ // �i�r�Q�[�V������ NULL ����Ȃ��ꍇ

		// �����̈ړ�����
		RotMove(*pRot, ROT_REV, fDeltaTime);

		// �i�r�̍X�V����
		m_pNav->Update
		(
			pPos,		// �ʒu
			pRot,		// ����
			&Move,		// �ړ���
			&rotDest,	// �ړI�̌���
			RADIUS,		// ���a
			HEIGHT,		// ����
			SPEED,		// ���x
			fDeltaTime	// �f���^�^�C��
		);
	}

	if (JudgeClone() ||
		JudgePlayer())
	{ // ���g���v���C���[���ڂɓ������ꍇ

		// �x����Ԃɂ���
		m_state = STATE_WARNING;

		// �������[�V������Ԃ�
		return MOTION_FOUND;
	}
	else
	{ // ��L�ȊO

		// ���Ώۂɂ���
		SetTarget(TARGET_NONE);
	}

	SetDestRotation(rotDest);
	SetMovePosition(Move);

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
CEnemyStalk::EMotion CEnemyStalk::Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	if (JudgeClone() ||
		JudgePlayer())
	{ // ���g���v���C���[���ڂɓ������ꍇ

		// �U������� false �ɂ���
		SetEnableAttack(false);
	}
	else
	{ // ��L�ȊO

		// �����Ԃɂ���
		m_state = STATE_CRAWL;

		// �ҋ@���[�V������Ԃ�
		return MOTION_IDOL;
	}

	// �ړ�����
	Move(pPos, *pRot, SPEED, fDeltaTime);

	if (Approach(*pPos))
	{ // �ڋ߂����ꍇ

		// �U����Ԃɂ���
		m_state = STATE_ATTACK;

		// �U�����[�V������Ԃ�
		return MOTION_ATTACK;
	}

	// �f�o�b�O
	DebugProc::Print(DebugProc::POINT_RIGHT, "����!!�ړI�n�F%f %f %f", GetTargetPos().x, GetTargetPos().y, GetTargetPos().z);

	// ���s���[�V������Ԃ�
	return MOTION_WALK;
}

//============================================================
// �U������
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Attack(const D3DXVECTOR3& rPos)
{
	switch (GetTarget())
	{
	case CEnemyAttack::TARGET_PLAYER:

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

	case CEnemyAttack::TARGET_CLONE:

		// ���g�̓����蔻�菈��
		HitClone(rPos);

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