//============================================================
//
//	�T�G���� [enemyWolf.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "enemyWolf.h"
#include "renderer.h"
#include "deltaTime.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\wolf.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const int	BLEND_FRAME_OTHER	= 5;	// ���[�V�����̊�{�I�ȃu�����h�t���[��
	const int	BLEND_FRAME_TURN	= 20;	// ���[�V�������h�̃u�����h�t���[��
	const int	BLEND_FRAME_LAND	= 15;	// ���[�V�������n�̃u�����h�t���[��

	const float MOVE = 500.0f;	// �ړ���
	const float	REV_ROTA		= 4.5f;		// �����ύX�̕␳�W��
	const float	REV_ROTA_LOOK	= 9.0f;		// �K�E�K�E���Ă鎞�̌����ύX�̕␳�W��
	const float ATTACK_DISTANCE	= 50.0f;	// �U������ɓ��鋗��

	const float	JUMP_REV = 0.16f;	// �ʏ��Ԏ��̋󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV = 0.16f;	// �ʏ��Ԏ��̒n��̈ړ��ʂ̌����W��
}

//************************************************************
//	�q�N���X [CEnemyWolf] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyWolf::CEnemyWolf() : CEnemyAttack(),
m_state(STATE_CRAWL)	// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyWolf::~CEnemyWolf()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyWolf::Init(void)
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
void CEnemyWolf::Uninit(void)
{
	// �G�̏I��
	CEnemyAttack::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyWolf::Update(const float fDeltaTime)
{
	// �G�̍X�V
	CEnemyAttack::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyWolf::Draw(CShader* pShader)
{
	// �G�̕`��
	CEnemyAttack::Draw(pShader);
}

//============================================================
//	���̐ݒ菈��
//============================================================
void CEnemyWolf::SetData(void)
{

}

//============================================================
//	��Ԃ̍X�V����
//============================================================
int CEnemyWolf::UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// ���݂̃��[�V����
	switch (m_state)
	{ // ��Ԃ��Ƃ̏���
	case STATE_CRAWL:	// ������

		// �����Ԏ��̍X�V
		nCurMotion = UpdateCrawl(pPos, pRot, fDeltaTime);
		break;

	case STATE_CAVEAT:	// �x�����

		// �x����Ԏ��̍X�V
		nCurMotion = UpdateCaveat(pPos, pRot, fDeltaTime);
		break;

	case STATE_FOUND:	// �ǐՏ��

		// �ǐՏ�Ԏ��̍X�V
		nCurMotion = UpdateFound(pPos, pRot, fDeltaTime);
		break;

	case STATE_ATTACK:	// �U�����

		// �U����Ԏ��̍X�V
		nCurMotion = UpdateAttack(pPos, pRot, fDeltaTime);
		break;

	case STATE_UPSET:	// ���h���

		// ���h��Ԏ��̍X�V
		nCurMotion = UpdateUpset(pPos, pRot, fDeltaTime);
		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// ���݂̃��[�V������Ԃ�
	return nCurMotion;
}

//============================================================
//	���[�V�����E�L�����N�^�[�̍X�V����
//============================================================
void CEnemyWolf::UpdateMotion(int nMotion, const float fDeltaTime)
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

	switch (GetMotionType())
	{ // ���[�V�����̎�ނ��Ƃ̏���
	case MOTION_IDOL:	// �ҋ@���[�V����
		break;

	case MOTION_RUN:	// �x�����[�V����

#if 0
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
#endif
		break;

	case MOTION_FOUND:	// �������[�V����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_BITE:	// ���݂����[�V����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case MOTION_TURN:	// �U��������[�V����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(MOTION_IDOL, BLEND_FRAME_TURN);

			// �����Ԃɂ���
			m_state = STATE_CRAWL;
		}

		break;

	case MOTION_FALL:	// �������[�V����

		if (!IsJump())
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
//	���n�̍X�V����
//============================================================
void CEnemyWolf::UpdateLanding(D3DXVECTOR3* pPos)
{
	// �e�N���X�̒��n�X�V
	CEnemyAttack::UpdateLanding(pPos);

	if (!IsJump())
	{ // �󒆂ɂ��Ȃ��ꍇ

		//  TODO�F�Ȃ��������őҋ@���[�V�����ɂȂ��Ă遨���[�v���[�V���������炾
		if (GetMotionType() == MOTION_FALL)
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
//	�����Ԏ��̍X�V����
//============================================================
int CEnemyWolf::UpdateCrawl(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(*pPos, fDeltaTime);

	// ���n����
	UpdateLanding(pPos);

	// �����X�V
	UpdateRotation(*pRot, fDeltaTime);

	if (JudgeClone() || 
		JudgePlayer())
	{ // ���g���v���C���[���ڂɓ������ꍇ

		// �x����Ԃɂ���
		m_state = STATE_CAVEAT;

		// �������[�V������Ԃ�
		return MOTION_FOUND;
	}

	// �����Ԃɂ���
	m_state = STATE_CRAWL;

	// �W�I�𖢐ݒ�ɂ���
	SetTarget(TARGET_NONE);

	// �ҋ@���[�V������Ԃ�
	return MOTION_IDOL;
}

//============================================================
//	�x����Ԏ��̍X�V����
//============================================================
int CEnemyWolf::UpdateCaveat(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(*pPos, fDeltaTime);

	// ���n����
	UpdateLanding(pPos);

	// �ڕW�ʒu�̍X�V
	if (!JudgeClone() &&
		!JudgePlayer()) 
	{ // ���g���v���C���[���������Ȃ������ꍇ

		// ���h��Ԃɂ���
		m_state = STATE_UPSET;

		// �W�I�𖢐ݒ�ɂ���
		SetTarget(TARGET_NONE);

		// ���h���[�V�����ɂ���
		return MOTION_TURN;
	}

	// �ڕW�ʒu�̎��F
	LookTarget(*pPos);

	// �����X�V
	UpdateRotation(*pRot, REV_ROTA_LOOK, fDeltaTime);

	if (GetMotionType() != MOTION_FOUND)
	{ // �������[�V��������Ȃ������ꍇ

		// �ǐՏ�Ԃɂ���
		m_state = STATE_FOUND;
	}

	// ���s��Ԃ�Ԃ�
	return MOTION_RUN;
}

//============================================================
//	�ǐՏ�Ԏ��̍X�V����
//============================================================
int CEnemyWolf::UpdateFound(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	if (!JudgeClone() &&
		!JudgePlayer())
	{ // ���g���ڂɓ������ꍇ

		// ���h��Ԃɂ���
		m_state = STATE_UPSET;

		// �W�I�𖢐ݒ�ɂ���
		SetTarget(TARGET_NONE);

		// ���h���[�V�����ɂ���
		return MOTION_TURN;
	}

	// �f�o�b�O
	DebugProc::Print(DebugProc::POINT_RIGHT, "����!!�ړI�n�F%f %f %f", GetTargetPos().x, GetTargetPos().y, GetTargetPos().z);

	// �ړ�����
	UpdateMove(*pPos, *pRot, fDeltaTime);

	if (Approach(*pPos))
	{ // �ڋ߂����ꍇ

		// �U����Ԃɂ���
		m_state = STATE_ATTACK;

		// ���݂����[�V������Ԃ�
		return MOTION_BITE;
	}

	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(*pPos, fDeltaTime);

	// ���n����
	UpdateLanding(pPos);

	// �����X�V
	UpdateRotation(*pRot, fDeltaTime);

	// ���s���[�V������Ԃ�
	return MOTION_RUN;
}

//============================================================
//	�U����Ԏ��̍X�V����
//============================================================
int CEnemyWolf::UpdateAttack(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(*pPos, fDeltaTime);

	// ���n����
	UpdateLanding(pPos);

	// �����X�V
	UpdateRotation(*pRot, fDeltaTime);

	switch (GetTarget())
	{ // �^�[�Q�b�g���Ƃ̏���
	case CEnemyAttack::TARGET_PLAYER:

		// �v���C���[�̓����蔻�菈��
		HitPlayer(*pPos);

		if (GetMotionType() != MOTION_BITE)
		{ // �U�����[�V��������Ȃ��ꍇ

			// �����Ԃɂ���
			m_state = STATE_CRAWL;
		}

		// �ҋ@���[�V�����ɂ���
		return MOTION_IDOL;

	case CEnemyWolf::TARGET_CLONE:

		// ���g�̓����蔻�菈��
		HitClone(*pPos);

		// ���h��Ԃɂ���
		m_state = STATE_UPSET;

		// ���h���[�V�����ɂ���
		return MOTION_TURN;

	default:	// ��O����
		assert(false);
		break;
	}

	// �ҋ@���[�V������Ԃ�
	return MOTION_IDOL;
}

//============================================================
//	���h��Ԏ��̍X�V����
//============================================================
int CEnemyWolf::UpdateUpset(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(*pPos, fDeltaTime);

	// ���n����
	UpdateLanding(pPos);

	// �����X�V
	UpdateRotation(*pRot, fDeltaTime);

	// ���h���[�V�����ɂ���
	return MOTION_TURN;
}

//============================================================
//	�ړ��ʁE�ڕW�����̍X�V����
//============================================================
void CEnemyWolf::UpdateMove(D3DXVECTOR3& rPos, D3DXVECTOR3& rRot, const float fDeltaTime)
{
	D3DXVECTOR3 move = GetMovePosition();		// �ړ���

	// �ڕW������ڕW�ʒu�����ɂ���
	LookTarget(rPos);

	// �ړ��ʂ�ݒ肷��
	move.x = sinf(rRot.y - D3DX_PI) * MOVE * fDeltaTime;
	move.z = cosf(rRot.y - D3DX_PI) * MOVE * fDeltaTime;

	// �ʒu���ړ�����
	rPos += move;

	SetMovePosition(move);		// �ړ��ʂ𔽉f
}

//============================================================
//	�ʒu�̍X�V����
//============================================================
void CEnemyWolf::UpdatePosition(D3DXVECTOR3& rPos, const float fDeltaTime)
{
	D3DXVECTOR3 move = GetMovePosition();	// �ړ���

	// �ړ��ʂ����Z
	rPos += move * fDeltaTime;

	// �ړ��ʂ�����
	if (IsJump())
	{ // �󒆂̏ꍇ

		move.x += (0.0f - move.x) * JUMP_REV;
		move.z += (0.0f - move.z) * JUMP_REV;
	}
	else
	{ // �n��̏ꍇ

		move.x += (0.0f - move.x) * LAND_REV;
		move.z += (0.0f - move.z) * LAND_REV;
	}

	SetMovePosition(move);	// �ړ��ʂ𔽉f
}

//============================================================
//	�����̍X�V����
//============================================================
void CEnemyWolf::UpdateRotation(D3DXVECTOR3& rRot, const float fDeltaTime)
{
	// �����̍X�V
	UpdateRotation(rRot, REV_ROTA, fDeltaTime);
}

//============================================================
//	�����̍X�V���� (�␳�ʐݒ�)
//============================================================
void CEnemyWolf::UpdateRotation(D3DXVECTOR3& rRot, const float fRevRota, const float fDeltaTime)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// �ڕW����
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����̐��K��
	useful::NormalizeRot(destRot.y);

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = destRot.y - rRot.y;
	useful::NormalizeRot(fDiffRot);	// ���������̐��K��

	// �����̍X�V
	rRot.y += fDiffRot * fDeltaTime * fRevRota;
	useful::NormalizeRot(rRot.y);	// �����̐��K��

	SetDestRotation(destRot);	// �ڕW�����𔽉f
}

//============================================================
//	�ڕW�ʒu�̎��F����
//============================================================
void CEnemyWolf::LookTarget(const D3DXVECTOR3& rPos)
{
	D3DXVECTOR3 destRot = GetDestRotation();	// �ڕW����

	// �ڕW���������߂�
	destRot.y = atan2f(rPos.x - GetTargetPos().x, rPos.z - GetTargetPos().z);

	SetDestRotation(destRot);	// �ڕW�����𔽉f
}
