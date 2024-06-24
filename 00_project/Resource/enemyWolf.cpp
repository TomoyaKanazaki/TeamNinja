//============================================================
//
//	���G���� [enemyDog.cpp]
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
	const int	BLEND_FRAME_LAND	= 15;	// ���[�V�������n�̃u�����h�t���[��

	const float	REV_ROTA = 9.0f;	// �����ύX�̕␳�W��
	const float	JUMP_REV = 0.16f;	// �ʏ��Ԏ��̋󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV = 0.16f;	// �ʏ��Ԏ��̒n��̈ړ��ʂ̌����W��
}

//************************************************************
//	�q�N���X [CEnemyWolf] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyWolf::CEnemyWolf() : CEnemy(),
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
void CEnemyWolf::Uninit(void)
{
	// �G�̏I��
	CEnemy::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyWolf::Update(const float fDeltaTime)
{
	// �G�̍X�V
	CEnemy::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyWolf::Draw(CShader* pShader)
{
	// �G�̕`��
	CEnemy::Draw(pShader);
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
			SetMotion(nMotion, BLEND_FRAME_OTHER);
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
	CEnemy::UpdateLanding(pPos);

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
	int nCurMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// �ړ�����
	nCurMotion = UpdateMove();

	// �d�͂̍X�V
	UpdateGravity();

	// �ʒu�X�V
	UpdatePosition(*pPos, fDeltaTime);

	// ���n����
	UpdateLanding(pPos);

	// �����X�V
	UpdateRotation(*pRot, fDeltaTime);

	// ���݂̃��[�V������Ԃ�
	return nCurMotion;
}

//============================================================
//	�x����Ԏ��̍X�V����
//============================================================
int CEnemyWolf::UpdateCaveat(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// ���݂̃��[�V������Ԃ�
	return nCurMotion;
}

//============================================================
//	�ǐՏ�Ԏ��̍X�V����
//============================================================
int CEnemyWolf::UpdateFound(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// ���݂̃��[�V������Ԃ�
	return nCurMotion;
}

//============================================================
//	�U����Ԏ��̍X�V����
//============================================================
int CEnemyWolf::UpdateAttack(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// ���݂̃��[�V������Ԃ�
	return nCurMotion;
}

//============================================================
//	���h��Ԏ��̍X�V����
//============================================================
int CEnemyWolf::UpdateUpset(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// ���݂̃��[�V������Ԃ�
	return nCurMotion;
}

//============================================================
//	�ړ��ʁE�ڕW�����̍X�V����
//============================================================
int CEnemyWolf::UpdateMove(void)
{
	D3DXVECTOR3 move = GetMovePosition();		// �ړ���
	D3DXVECTOR3 destRot = GetDestRotation();	// �ڕW����
	int nCurMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// �X�e�B�b�N�̌X������ړ��ʂ�ݒ�
	CInputPad* pPad = GET_INPUTPAD;	// �p�b�h���
	float fSpeed = pPad->GetPressLStickTilt();	// �X�e�B�b�N�̌X����
	if (pad::DEAD_ZONE < fSpeed)
	{ // �f�b�h�]�[���ȏ�̏ꍇ

		// �X�e�B�b�N�������擾
		float fStickRot = pPad->GetPressLStickRot() - (D3DX_PI * 0.5f);

		// �ڕW������ݒ�
		destRot.y = fStickRot;
		useful::NormalizeRot(destRot.y);	// �����̐��K��

		// �ړ��ʂ�ݒ肷��
		D3DXVECTOR3 fRate = pPad->GetStickRateL(pad::DEAD_RATE);
		move.x = sinf(fStickRot + D3DX_PI) * (600.0f * fabsf(fRate.x));
		move.z = cosf(fStickRot + D3DX_PI) * (600.0f * fabsf(fRate.z));

		// ���s���[�V�����ɂ���
		nCurMotion = MOTION_RUN;
	}

	SetMovePosition(move);		// �ړ��ʂ𔽉f
	SetDestRotation(destRot);	// �ڕW�����𔽉f

	// ���݂̃��[�V������Ԃ�
	return nCurMotion;
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
	D3DXVECTOR3 destRot = GetDestRotation();	// �ڕW����
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����̐��K��
	useful::NormalizeRot(destRot.y);

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = destRot.y - rRot.y;

	// ���������̐��K��
	useful::NormalizeRot(fDiffRot);

	// �����̍X�V
	rRot.y += fDiffRot * fDeltaTime * REV_ROTA;

	// �����̐��K��
	useful::NormalizeRot(rRot.y);

	SetDestRotation(destRot);	// �ڕW�����𔽉f
}
