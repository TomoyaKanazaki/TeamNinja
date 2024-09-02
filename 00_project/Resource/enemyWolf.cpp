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
#include "stage.h"
#include "sound.h"

#include "enemyNavigation.h"
#include "enemyNavStreet.h"
#include "enemyNavRandom.h"
#include "enemyChaseRange.h"
#include "camera.h"

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

	// ��ԊǗ��֌W
	const int CAVEAT_STATE_COUNT = 36;		// ������Ԃ̃J�E���g��
	const int ATTACK_STATE_COUNT = 34;		// �U����Ԃ̃J�E���g��
	const int BLANKBITE_STATE_COUNT = 340;	// ���h��Ԃ̃J�E���g��
	const int BLANKBITE_CYCLE_COUNT = 18;	// ���h��Ԃ̉�]�J�E���g
	const int CAUTION_STATE_COUNT = 180;	// �x����Ԃ̃J�E���g��
	const int REGRESSION_COUNT = 120;		// ��A����J�E���g��
}

// ���֘A
namespace sound
{
	const int WALK_COUNT = 32;		// ���s����炷�J�E���g��
	const int FOUND_COUNT = 2;		// ��������炷�J�E���g��
	const int UPSET_COUNT = 80;		// ���h����炷�J�E���g��
}

//************************************************************
//	�q�N���X [CEnemyWolf] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyWolf::CEnemyWolf() : CEnemyAttack(),
m_pNav(nullptr),		// �i�r�Q�[�V�����̏��
m_state(STATE_CRAWL),	// ���
m_nStateCount(0),		// ��ԃJ�E���g
m_nRegressionCount(0)	// ��A�J�E���g
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
	// �i�r�Q�[�V�����̏I������
	SAFE_UNINIT(m_pNav);

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
//	��������(���͈͈ړ��G)
//============================================================
CEnemyWolf* CEnemyWolf::Create
(
	const D3DXVECTOR3& rPos,	// �ʒu
	const D3DXVECTOR3& rRot,	// ����
	const float fMoveWidth,		// �ړ���
	const float fMoveDepth,		// �ړ����s
	const float fChaseWidth,	// �ǐՕ�
	const float fChaseDepth		// �ǐՉ��s
)
{
	// �G�𐶐�
	CEnemyWolf* pEnemy = new CEnemyWolf;

	if (pEnemy == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�̏�����
		if (FAILED(pEnemy->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�̔j��
			SAFE_DELETE(pEnemy);
			return nullptr;
		}

		// �ʒu��ݒ�
		pEnemy->SetVec3Position(rPos);

		// ������ݒ�
		pEnemy->SetVec3Rotation(rRot);

		// ��ނ�ݒ�
		pEnemy->SetType(TYPE_WOLF);

		// �����ʒu��ݒ�
		pEnemy->SetPosInit(rPos);

		// ����������ݒ�
		pEnemy->SetRotInit(rRot);

		// ���̐ݒ菈��
		pEnemy->SetData();

		// �i�r�Q�[�V�����𐶐�
		pEnemy->m_pNav = CEnemyNavRandom::Create(rPos, fMoveWidth, fMoveDepth);

		// �ǐՔ͈͂𐶐�
		pEnemy->SetChaseRange(CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth));

		// �m�ۂ����A�h���X��Ԃ�
		return pEnemy;
	}
}

//============================================================
//	��������(���[�g����ړ��G)
//============================================================
CEnemyWolf* CEnemyWolf::Create
(
	const D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rRot,				// ����
	const std::vector<D3DXVECTOR3> route,	// ���[�g�̔z��
	const float fChaseWidth,				// �ǐՕ�
	const float fChaseDepth					// �ǐՉ��s
)
{
	// �G�𐶐�
	CEnemyWolf* pEnemy = new CEnemyWolf;

	if (pEnemy == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �G�̏�����
		if (FAILED(pEnemy->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �G�̔j��
			SAFE_DELETE(pEnemy);
			return nullptr;
		}

		// �ʒu��ݒ�
		pEnemy->SetVec3Position(rPos);

		// ������ݒ�
		pEnemy->SetVec3Rotation(rRot);

		// ��ނ�ݒ�
		pEnemy->SetType(TYPE_WOLF);

		// �����ʒu��ݒ�
		pEnemy->SetPosInit(rPos);

		// ����������ݒ�
		pEnemy->SetRotInit(rRot);

		// ���̐ݒ菈��
		pEnemy->SetData();

		// �i�r�Q�[�V�����𐶐�
		pEnemy->m_pNav = CEnemyNavStreet::Create(route);

		// �ǐՔ͈͂𐶐�
		pEnemy->SetChaseRange(CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth));

		// �m�ۂ����A�h���X��Ԃ�
		return pEnemy;
	}
}

//============================================================
//	��Ԃ̍X�V����
//============================================================
int CEnemyWolf::UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// ���̈ʒu�ɖ߂鏈���� true �̏ꍇ�A������
	if (!BackOriginPos(pPos, pRot, HEIGHT))
	{
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
			nCurMotion = UpdateAttack(*pPos);
			break;

		case STATE_BLANKATTACK:

			// �󔒍U����Ԃ̍X�V
			nCurMotion = UpdateBlankAttack(pRot, fDeltaTime);
			break;

		case STATE_UPSET:	// ���h���

			// ���h��Ԏ��̍X�V
			nCurMotion = UpdateUpset();
			break;

		case STATE_STANCE:	// �\�����

			// �\�����̍X�V
			nCurMotion = UpdateStance();
			break;

		default:	// ��O����
			assert(false);
			break;
		}
	}

	// �d�͂̍X�V
	UpdateGravity();

	// �G�𗎉�������
	pPos->y += GetMovePosition().y * fDeltaTime;

	if (Collision(*pPos))
	{ // �����蔻�肪 true ���A�ړ���Ԃ̏ꍇ

		// �i�r�Q�[�V�����̃��Z�b�g����
		m_pNav->NavReset();
	}

	// ���n���菈��
	UpdateLanding(pPos);

	// ��l�̕ǂ̔���
	GET_STAGE->LimitPosition(*pPos, RADIUS);

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

		// �u�����h���̏ꍇ������
		if (GetMotionBlendFrame() != 0) { break; }

		if (GetMotionKey() % 2 == 0 && GetMotionKeyCounter() == 0)
		{ // ���������^�C�~���O�̏ꍇ

			// ���s����炷
			PLAY_SOUND(CSound::LABEL_SE_STALKWALK_000);
		}

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

			// TODO�F�x�����[�V�����̐ݒ�
			SetMotion(MOTION_IDOL, BLEND_FRAME_TURN);

			// �����Ԃɂ���
			SetState(STATE_CRAWL);
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
	switch (m_pNav->GetState())
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
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	int motion = MOTION_IDOL;	// ���[�V����

	// �����X�V
	RotMove(*pRot, REV_ROTA, fDeltaTime);

	if (m_pNav != nullptr)
	{ // �i�r�Q�[�V������ NULL ����Ȃ��ꍇ

		// �i�r�̍X�V����
		m_pNav->Update
		(
			pPos,		// �ʒu
			pRot,		// ����
			this,		// �G�̏��
			SPEED,		// ���x
			fDeltaTime	// �f���^�^�C��
		);

		// �i�r�Q�[�V�����ɂ�郂�[�V�����̐ݒ菈��
		NavMoitonSet(&motion);
	}

	if (JudgeClone() ||
		JudgePlayer())
	{ // ���g���v���C���[���ڂɓ������ꍇ

		// �i�r�Q�[�V�������Z�b�g����
		m_pNav->NavReset();

		if (GetChaseRange()->InsideTargetPos(GetPosInit(), GetTargetPos()))
		{ // �͈͓��ɓ������ꍇ

			// �x����Ԃɂ���
			SetState(STATE_CAVEAT);

			// �������[�V������Ԃ�
			return MOTION_FOUND;
		}

		// �\����Ԃɂ���
		SetState(STATE_STANCE);

		// �\��������炷
		PLAY_SOUND(CSound::LABEL_SE_WOLFSTANCE_000);

		// TODO�F�\�����[�V������Ԃ�
		return MOTION_LANDING;
	}

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
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	if (m_nStateCount == sound::FOUND_COUNT)
	{ // ��莞�Ԍo�߂����ꍇ

		// ��������炷
		PLAY_SOUND(CSound::LABEL_SE_WOLFFOUND_000);
	}

	// �ڕW�ʒu�̎��F����
	LookTarget(*pPos);

	// �����̈ړ�����
	RotMove(*pRot, REV_ROTA, fDeltaTime);

	if (m_nStateCount % CAVEAT_STATE_COUNT == 0)
	{ // ��莞�Ԍo�߂����ꍇ

		// �ǐՏ�Ԃɂ���
		SetState(STATE_FOUND);
	}

	// ���s���[�V������Ԃ�
	return MOTION_RUN;
}

//============================================================
//	�ǐՏ�Ԏ��̍X�V����
//============================================================
int CEnemyWolf::UpdateFound(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// ���s�J�E���g�����Z����
	m_nStateCount++;

	if (!ShakeOffClone() &&
		!ShakeOffPlayer())
	{ // ���g���ڂɓ����Ă��Ȃ��ꍇ

		// �����Ԃɂ���
		SetState(STATE_CRAWL);

		// �W�I�𖢐ݒ�ɂ���
		SetTarget(TARGET_NONE);

		// ���h���[�V�����ɂ���
		return MOTION_IDOL;
	}

	// �ڕW������ڕW�ʒu�����ɂ���
	LookTarget(*pPos);

	// �����t�t���O�� false �ɂ���
	SetEnableDodge(false);

	// �U���J�E���g�����Z�b�g����
	SetAttackCount(0);

	// �����̈ړ�����
	RotMove(*pRot, REV_ROTA, fDeltaTime);

	// �ړ�����
	Move(pPos, *pRot, SPEED, fDeltaTime);

	if (Approach(*pPos))
	{ // �ڋ߂����ꍇ

		if (GetTarget() == CEnemyAttack::TARGET_PLAYER)
		{ // �ڕW���v���C���[�̏ꍇ

			// �����t�t���O�� true �ɂ���
			SetEnableDodge(true);

			// �U���J�E���g�����Z�b�g����
			SetAttackCount(0);
		}

		// �U����Ԃɂ���
		SetState(STATE_ATTACK);

		// ���݂����[�V������Ԃ�
		return MOTION_BITE;
	}

	// �ʒu�X�V
	UpdatePosition(*pPos, fDeltaTime);

	if (GetChaseRange() != nullptr &&
		GetChaseRange()->ChaseRange(GetPosInit(), pPos))
	{ // �ǐՔ͈͂���o���ꍇ

		// �\����Ԃɂ���
		SetState(STATE_STANCE);

		// �i�r�Q�[�V�������Z�b�g����
		m_pNav->NavReset();
	}

	// ���s���[�V������Ԃ�
	return MOTION_RUN;
}

//============================================================
//	�U����Ԏ��̍X�V����
//============================================================
int CEnemyWolf::UpdateAttack(const D3DXVECTOR3& rPos)
{
	switch (GetTarget())
	{ // �^�[�Q�b�g���Ƃ̏���
	case CEnemyAttack::TARGET_PLAYER:

		// �v���C���[�̓����蔻�菈��
		if (HitPlayer(rPos))
		{
			// �U������炷
			PLAY_SOUND(CSound::LABEL_SE_WOLFATTACK_000);
		}

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;

		if (m_nStateCount % ATTACK_STATE_COUNT == 0)
		{ // ���J�E���g�o�߂����ꍇ

			// �����Ԃɂ���
			SetState(STATE_CRAWL);
		}

		// �ҋ@���[�V�����ɂ���
		return MOTION_IDOL;

	case CEnemyWolf::TARGET_CLONE:

		if (HitClone(rPos))
		{ // ���g�ɓ��������ꍇ

			// �󔒍U����Ԃɂ���
			SetState(STATE_BLANKATTACK);

			// ���g�U������炷
			PLAY_SOUND(CSound::LABEL_SE_WOLFATTACK_001);

			// ���݂����[�V�����ɂ���
			return MOTION_BITE;
		}
		else
		{ // ��L�ȊO

			// �����Ԃɂ���
			SetState(STATE_CRAWL);
		}

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
// �󔒍U������
//============================================================
int CEnemyWolf::UpdateBlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	// �����̈ړ�����
	RotMove(*pRot, REV_ROTA_LOOK, fDeltaTime);

	if (m_nStateCount <= BLANKBITE_STATE_COUNT)
	{ // ���J�E���g�ȉ��̏ꍇ

		if (m_nStateCount % BLANKBITE_CYCLE_COUNT == 0)
		{ // ���J�E���g���Ƃ�

			// �ړI�̌������擾
			D3DXVECTOR3 rotDest = GetDestRotation();

			// �ړI�̌�����ݒ肷��
			rotDest.y = useful::RandomRot();

			// �ړI�̌�����K�p
			SetDestRotation(rotDest);
		}
	}
	else
	{ // ��L�ȊO

		// ���h��Ԃɂ���
		SetState(STATE_UPSET);
	}

	// �U�����[�V�����ɂ���
	return MOTION_BITE;
}

//============================================================
//	���h��Ԏ��̍X�V����
//============================================================
int CEnemyWolf::UpdateUpset(void)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	if (m_nStateCount == sound::UPSET_COUNT)
	{ // ��ԃJ�E���g����萔�ɂȂ����Ƃ�

		// ���h����炷
		PLAY_SOUND(CSound::LABEL_SE_WOLFUPSET_000);
	}

	// ���h���[�V�����ɂ���
	return MOTION_TURN;
}

//============================================================
// �\������
//============================================================
int CEnemyWolf::UpdateStance(void)
{
	// ���g�̔�������
	JudgeClone();

	// �v���C���[�̔�������
	JudgePlayer();

	if (GetChaseRange()->InsideTargetPos(GetPosInit(), GetTargetPos()))
	{ // �͈͓��ɓ������ꍇ

		// �x����Ԃɂ���
		SetState(STATE_CAVEAT);

		// �������[�V������Ԃ�
		return MOTION_FOUND;
	}

	// TODO�F�\�����[�V������Ԃ�
	return MOTION_FALL;
}

//============================================================
// ���̈ʒu�ɖ߂鏈��
//============================================================
bool CEnemyWolf::BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight)
{
	// ���̏�Ԃ̏ꍇ�Afalse ��Ԃ�
	if (m_state == STATE_BLANKATTACK || m_state == STATE_UPSET) { SetRegressionCount(0); return false; }

	// �����ʒu��A�����Ɏ��s�����ꍇ�Afalse ��Ԃ�
	if (!CEnemyAttack::BackOriginPos(pPos, pRot, fHeight)) { return false; }

	// �҂�������Ԃɂ���
	SetState(STATE_CRAWL);

	// �i�r�Q�[�V�������Z�b�g����
	m_pNav->NavReset();

	// true ��Ԃ�
	return true;
}

//============================================================
// ��Ԃ̐ݒ菈��
//============================================================
void CEnemyWolf::SetState(const EState state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;

	// ��ԃJ�E���g��0�ɂ���
	m_nStateCount = 0;
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
