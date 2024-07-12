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

#include "enemyNavigation.h"
#include "enemyChaseRange.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\wolf.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const int	BLEND_FRAME_OTHER	= 5;	// ���[�V�����̊�{�I�ȃu�����h�t���[��
	const int	BLEND_FRAME_TURN	= 20;	// ���[�V�������h�̃u�����h�t���[��
	const int	BLEND_FRAME_LAND	= 15;	// ���[�V�������n�̃u�����h�t���[��
	const float	RADIUS = 70.0f;				// ���a
	const float HEIGHT = 20.0f;				// �g��

	const float SPEED = -500.0f;			// �ړ���
	const float	REV_ROTA		= 4.5f;		// �����ύX�̕␳�W��
	const float	REV_ROTA_LOOK	= 9.0f;		// �K�E�K�E���Ă鎞�̌����ύX�̕␳�W��
	const float ATTACK_DISTANCE	= 50.0f;	// �U������ɓ��鋗��
	const float FADE_ALPHA_TRANS = 0.02f;	// �t�F�[�h���̓����x�̑J�ڒ萔

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
// ���a�̎擾����
//============================================================
float CEnemyWolf::GetRadius(void) const
{
	// ���a��Ԃ�
	return RADIUS;
}

//============================================================
// �����̎擾����
//============================================================
float CEnemyWolf::GetHeight(void) const
{
	// �g����Ԃ�
	return HEIGHT;
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

	case STATE_FADEOUT:	// �t�F�[�h�A�E�g���

		// �t�F�[�h�A�E�g���̍X�V
		nCurMotion = UpdateFadeOut(pPos, pRot, fDeltaTime);

		break;

	case STATE_FADEIN:	// �t�F�[�h�C�����

		// �t�F�[�h�C�����̍X�V
		nCurMotion = UpdateFadeIn(pPos, pRot, fDeltaTime);

		break;

	default:	// ��O����
		assert(false);
		break;
	}

	// �����蔻�菈��
	Collision(*pPos);

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
// �i�r�Q�[�V�����ɂ�郂�[�V�����̐ݒ菈��
//============================================================
void CEnemyWolf::NavMoitonSet(int* pMotion)
{
	switch (GetNavigation()->GetState())
	{
	case CEnemyNav::STATE_MOVE:

		// �ړ����[�V������ݒ�
		*pMotion = MOTION_RUN;

		break;

	default:

		break;
	}
}

//============================================================
//	�����Ԏ��̍X�V����
//============================================================
int CEnemyWolf::UpdateCrawl(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int motion = MOTION_IDOL;	// ���[�V����

	// �d�͂̍X�V
	UpdateGravity();

	// �G�𗎉�������
	pPos->y += GetMovePosition().y * fDeltaTime;

	// �����X�V
	UpdateRotation(*pRot, fDeltaTime);

	if (GetNavigation() != nullptr)
	{ // �i�r�Q�[�V������ NULL ����Ȃ��ꍇ

		// �i�r�̍X�V����
		GetNavigation()->Update
		(
			pPos,		// �ʒu
			pRot,		// ����
			this,		// �G�̏��
			SPEED,		// ���x
			fDeltaTime	// �f���^�^�C��
		);

		switch (GetNavigation()->GetState())
		{
		case CEnemyNav::STATE_MOVE:

			// �ړ����[�V������ݒ�
			motion = MOTION_RUN;

			break;

		default:

			break;
		}

		// �i�r�Q�[�V�����ɂ�郂�[�V�����̐ݒ菈��
		NavMoitonSet(&motion);
	}

	// ���n����
	UpdateLanding(pPos);

	if (JudgeClone() || 
		JudgePlayer())
	{ // ���g���v���C���[���ڂɓ������ꍇ

		// �i�r�Q�[�V�������Z�b�g����
		GetNavigation()->NavReset();

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
	return motion;
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
	UpdateRotation(*pRot, fDeltaTime);

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
	if (!ShakeOffClone() &&
		!ShakeOffPlayer())
	{ // ���g���ڂɓ����Ă��Ȃ��ꍇ

		// ���h��Ԃɂ���
		m_state = STATE_UPSET;

		// �W�I�𖢐ݒ�ɂ���
		SetTarget(TARGET_NONE);

		// ���h���[�V�����ɂ���
		return MOTION_TURN;
	}

	// �ڕW������ڕW�ʒu�����ɂ���
	LookTarget(*pPos);

	// �U������� false �ɂ���
	SetEnableAttack(false);

	// �ړ�����
	Move(pPos, *pRot, SPEED, fDeltaTime);

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

	if (GetChaseRange() != nullptr &&
		GetChaseRange()->ChaseRange(pPos))
	{ // �ǐՔ͈͂���o���ꍇ

		// �t�F�[�h�A�E�g��Ԃɂ���
		m_state = STATE_FADEOUT;

		// �i�r�Q�[�V�������Z�b�g����
		GetNavigation()->NavReset();

		// �ړ��ʂ����Z�b�g����
		SetMovePosition(VEC3_ZERO);

		// �^�[�Q�b�g�𖳑Ώۂɂ���
		SetTarget(TARGET_NONE);
	}

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
// �t�F�[�h�A�E�g��Ԏ��̍X�V
//============================================================
int CEnemyWolf::UpdateFadeOut(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// �����x���擾
	float fAlpha = GetAlpha();

	// �����x�����Z����
	fAlpha -= FADE_ALPHA_TRANS;

	if (fAlpha <= 0.0f)
	{ // �����x��0�ȉ��ɂȂ����ꍇ

		// �t�F�[�h�C����Ԃɂ���
		m_state = STATE_FADEIN;

		// �ʒu��ݒ肷��
		*pPos = GetPosInit();

		// �ߋ��̈ʒu��K�p����(�������Ȃ��Ɠ����蔻��Ɉ����������Ă��܂�)
		SetOldPosition(*pPos);

		// ������ݒ肷��
		*pRot = VEC3_ZERO;

		// �����x��␳����
		fAlpha = 0.0f;
	}

	// �����x��K�p
	CObjectChara::SetAlpha(fAlpha);
	CEnemyAttack::SetAlpha(fAlpha);

	// �ҋ@���[�V�����ɂ���
	return MOTION_IDOL;
}

//============================================================
// �t�F�[�h�C����Ԏ��̍X�V
//============================================================
int CEnemyWolf::UpdateFadeIn(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// �����x���擾
	float fAlpha = GetAlpha();

	// �����x�����Z����
	fAlpha += FADE_ALPHA_TRANS;

	if (fAlpha >= 1.0f)
	{ // �����x����萔�ȏ�ɂȂ����ꍇ

		// �����Ԃɂ���
		m_state = STATE_CRAWL;

		// �����x��␳����
		fAlpha = 1.0f;
	}

	// �����x��K�p
	CObjectChara::SetAlpha(fAlpha);
	CEnemyAttack::SetAlpha(fAlpha);

	// �ҋ@���[�V�����ɂ���
	return MOTION_IDOL;
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
	RotMove(rRot, REV_ROTA, fDeltaTime);
}
