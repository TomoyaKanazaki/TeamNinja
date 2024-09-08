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
}

//************************************************************
//	�q�N���X [CEnemyWolf] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyWolf::CEnemyWolf() : CEnemyAttack(),
m_pNav(nullptr)		// �i�r�Q�[�V�����̏��
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
	// ���̐ݒ菈��
	CEnemyAttack::SetData();
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
// ���x�̎擾����
//============================================================
float CEnemyWolf::GetSpeed(void) const
{
	// ���x��Ԃ�
	return SPEED;
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

		// �i�r�Q�[�V�����𐶐�
		pEnemy->m_pNav = CEnemyNavRandom::Create(rPos, fMoveWidth, fMoveDepth);

		// �ǐՔ͈͂𐶐�
		pEnemy->SetChaseRange(CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth));

		// ���̐ݒ菈��
		pEnemy->SetData();

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

		// �i�r�Q�[�V�����𐶐�
		pEnemy->m_pNav = CEnemyNavStreet::Create(route);

		// �ǐՔ͈͂𐶐�
		pEnemy->SetChaseRange(CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth));

		// ���̐ݒ菈��
		pEnemy->SetData();

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
		switch (GetState())
		{ // ��Ԃ��Ƃ̏���
		case STATE_ORIGIN:	// �Ǝ����

			// �Ǝ���Ԏ��̍X�V
			nCurMotion = Original(pPos, pRot, fDeltaTime);
			break;

		case STATE_WARNING:	// �x�����

			// �x����Ԏ��̍X�V
			nCurMotion = Warning(pPos, pRot, fDeltaTime, REV_ROTA);
			break;

		case STATE_STALK:	// �ǐՏ��

			// �ǐՏ�Ԏ��̍X�V
			nCurMotion = Stalk(pPos, pRot, fDeltaTime, REV_ROTA);
			break;

		case STATE_ATTACK:	// �U�����

			// �U����Ԏ��̍X�V
			nCurMotion = Attack(*pPos);
			break;

		case STATE_BLANKATTACK:

			// �󔒍U����Ԃ̍X�V
			nCurMotion = BlankAttack(pRot, fDeltaTime, REV_ROTA_LOOK);
			break;

		case STATE_UPSET:	// ���h���

			// ���h��Ԏ��̍X�V
			nCurMotion = Upset();
			break;

		case STATE_STANCE:	// �\�����

			// �\�����̍X�V
			nCurMotion = Stance();
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

			// �Ǝ���Ԃɂ���
			SetState(STATE_ORIGIN);
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
// ���ꂼ��̓Ǝ�����
//============================================================
 int CEnemyWolf::Original(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
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

			// �Ǝ���Ԃɂ���
			SetState(STATE_ORIGIN);

			// �������[�V������Ԃ�
			return MOTION_FOUND;
		}

		// �\����Ԃɂ���
		SetState(STATE_STANCE);

		// �\��������炷
		PLAY_SOUND(CSound::LABEL_SE_WOLFSTANCE_000);

		// TODO�F�\�����[�V������Ԃ�
		return MOTION_FOUND;
	}

	// �W�I�𖢐ݒ�ɂ���
	SetTarget(TARGET_NONE);

	// �ҋ@���[�V������Ԃ�
	return motion;
}

//============================================================
//	�x����Ԏ��̍X�V����
//============================================================
int CEnemyWolf::Warning				// �x������
(
	D3DXVECTOR3* pPos,		// �ʒu
	D3DXVECTOR3* pRot,		// ����
	const float fDeltaTime,	// �f���^�^�C��
	const float fRotRev		// �����̕␳��
)
{
	// �x������
	CEnemyAttack::Warning(pPos, pRot, fDeltaTime, REV_ROTA);

	// ���s���[�V������Ԃ�
	return MOTION_RUN;
}

//============================================================
// �ǐՏ���
//============================================================
int CEnemyWolf::Stalk
(
	D3DXVECTOR3* pPos,		// �ʒu
	D3DXVECTOR3* pRot, 		// ����
	const float fDeltaTime,	// �f���^�^�C��
	const float fRotRev		// �����̕␳��
)
{
	// �ǐՏ���
	switch (CEnemyAttack::Stalk(pPos, pRot, fDeltaTime, REV_ROTA))
	{
	case STATE_ORIGIN:

		// �ҋ@���[�V������Ԃ�
		return MOTION_IDOL;

		break;

	case STATE_ATTACK:

		// ���݂����[�V������Ԃ�
		return MOTION_BITE;

		break;

	case STATE_STANCE:

		// �i�r�Q�[�V�������Z�b�g����
		m_pNav->NavReset();

		// ���s���[�V������Ԃ�
		return MOTION_RUN;

		break;

	default:

		// ���s���[�V������Ԃ�
		return MOTION_RUN;

		break;
	}
}

//============================================================
// �U������
//============================================================
int CEnemyWolf::Attack(const D3DXVECTOR3& rPos)
{
	switch (CEnemyAttack::Attack(rPos))
	{
	case STATE_ORIGIN:

		// �ҋ@���[�V������Ԃ�
		return MOTION_IDOL;

	case STATE_BLANKATTACK:

		// �U�����[�V������Ԃ�
		return MOTION_BITE;

	default:

		// �ҋ@���[�V�����ɂ���
		return MOTION_IDOL;
	}
}

//============================================================
// �󔒍U������
//============================================================
int CEnemyWolf::BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime, const float fRotRev)
{
	switch (CEnemyAttack::BlankAttack(pRot, fDeltaTime, fRotRev))
	{
	case STATE_UPSET:

		// �U��������[�V�����ɂ���
		return MOTION_TURN;

	default:

		// �U�����[�V�����ɂ���
		return MOTION_BITE;
	}
}

//============================================================
// ���h����
//============================================================
int CEnemyWolf::Upset(void)
{
	// ���h����
	CEnemyAttack::Upset();

	// �U��������[�V������Ԃ�
	return MOTION_TURN;
}

//============================================================
// �\������
//============================================================
int CEnemyWolf::Stance(void)
{
	switch (CEnemyAttack::Stance())
	{
	case STATE_WARNING:

		// �������[�V������Ԃ�
		return MOTION_FOUND;

	default:

		// �\�����[�V������Ԃ�
		return MOTION_FOUND;
	}
}

//============================================================
// ���̈ʒu�ɖ߂鏈��
//============================================================
bool CEnemyWolf::BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight)
{
	// ���̏�Ԃ̏ꍇ�Afalse ��Ԃ�
	if (GetState() == STATE_BLANKATTACK || GetState() == STATE_UPSET) { SetRegressionCount(0); return false; }

	// �����ʒu��A�����Ɏ��s�����ꍇ�Afalse ��Ԃ�
	if (!CEnemyAttack::BackOriginPos(pPos, pRot, fHeight)) { return false; }

	// �Ǝ���Ԃɂ���
	SetState(STATE_ORIGIN);

	// �i�r�Q�[�V�������Z�b�g����
	m_pNav->NavReset();

	// true ��Ԃ�
	return true;
}
