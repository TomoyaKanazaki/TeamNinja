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
#include "stage.h"
#include "sound.h"

#include "multiModel.h"
#include "enemyNavigation.h"
#include "enemyNavStreet.h"
#include "enemyNavRandom.h"
#include "enemyChaseRange.h"
#include "enemy_item.h"
#include "camera.h"
#include "actor.h"
#include "wall.h"
#include "collision.h"
#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* SETUP_TXT = "data\\CHARACTER\\enemy.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const int	BLEND_FRAME_OTHER = 5;		// ���[�V�����̊�{�I�ȃu�����h�t���[��
	const int	BLEND_FRAME_UPSET = 8;		// ���[�V�������h�̃u�����h�t���[��
	const int	BLEND_FRAME_LAND = 15;		// ���[�V�������n�̃u�����h�t���[��
	const int	CAUTIOUS_TRANS_LOOP = 7;	// �x�����[�V�����ɑJ�ڂ���ҋ@���[�v��
	const float	RADIUS = 20.0f;				// ���a
	const float HEIGHT = 80.0f;				// �g��
	const float SPEED = -290.0f;			// ���x
	const float ROT_REV = 4.0f;				// �����̕␳�W��
	const float FADE_ALPHA_TRANS = 0.02f;	// �t�F�[�h�̓����x�̑J�ڒ萔

	const int ITEM_PART_NUMBER = 8;			// �A�C�e�������p�[�c�̔ԍ�
	const D3DXVECTOR3 ITEM_OFFSET = D3DXVECTOR3(-3.0f, -1.0f, 10.0f);		// �A�C�e���̃I�t�Z�b�g���W
	const D3DXVECTOR3 ITEM_ROT = D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f);	// �A�C�e���̌���

	// ��ԊǗ��֌W
	const int FOUND_STATE_COUNT = 59;			// ������Ԃ̃J�E���g��
	const int ATTACK_STATE_COUNT = 44;			// �U����Ԃ̃J�E���g��
	const int BLANKATTACK_STATE_COUNT = 340;	// �󔒍U����Ԃ̃J�E���g��
	const int BLANKATTACK_CYCLE_COUNT = 18;		// �󔒍U����Ԃ̉�]�J�E���g
	const int CAUTION_STATE_COUNT = 180;		// �x����Ԃ̃J�E���g��
	const int THREAT_STATE_COUNT = 50;			// �Њd��Ԃ̃J�E���g��
}

// ���Ǘ��֌W
namespace sound
{
	const int WALK_COUNT = 32;			// ���s����炷�J�E���g��
	const int FOUND_COUNT = 37;			// ��������炷�J�E���g��
	const int UPSET_COUNT = 200;		// ���h����炷�J�E���g��
}

//************************************************************
//	�q�N���X [CEnemyStalk] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyStalk::CEnemyStalk() : CEnemyAttack(),
m_pNav(nullptr),
m_state(STATE_CRAWL),
m_nStateCount(0),
m_nRegressionCount(0)
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
	// �i�r�Q�[�V�����̏I������
	SAFE_UNINIT(m_pNav);

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
//	��������(���͈͈ړ��G)
//============================================================
CEnemyStalk* CEnemyStalk::Create
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
	CEnemyStalk* pEnemy = new CEnemyStalk;

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
		pEnemy->SetType(TYPE_STALK);

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
CEnemyStalk* CEnemyStalk::Create
(
	const D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rRot,				// ����
	const std::vector<D3DXVECTOR3> route,	// ���[�g�̔z��
	const float fChaseWidth,				// �ǐՕ�
	const float fChaseDepth					// �ǐՉ��s
)
{
	// �G�𐶐�
	CEnemyStalk* pEnemy = new CEnemyStalk;

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
		pEnemy->SetType(TYPE_STALK);

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
// ��Ԃ̍X�V����
//============================================================
int CEnemyStalk::UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// ���݂̃��[�V����

	// ���̈ʒu�ɖ߂鏈���� true �̏ꍇ�A������
	if (!BackOriginPos(pPos, pRot, HEIGHT))
	{
		switch (m_state)
		{
		case CEnemyStalk::STATE_CRAWL:

			// ���񏈗�
			nCurMotion = Crawl(pPos, pRot, fDeltaTime);

			break;

		case CEnemyStalk::STATE_WARNING:

			// �x������
			nCurMotion = Warning(pPos, pRot, fDeltaTime);

			break;

		case CEnemyStalk::STATE_STALK:

			// �ǐՏ���
			nCurMotion = Stalk(pPos, pRot, fDeltaTime);

			break;

		case CEnemyStalk::STATE_ATTACK:

			// �U������
			nCurMotion = Attack(*pPos);

			break;

		case CEnemyStalk::STATE_BLANKATTACK:

			// �󔒍U������
			nCurMotion = BlankAttack(pRot, fDeltaTime);

			break;

		case CEnemyStalk::STATE_UPSET:

			// ���h����
			nCurMotion = Upset();

			break;

		case CEnemyStalk::STATE_STANCE:

			// �\������
			nCurMotion = Stance();

			break;

		default:

			// ��~
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

			// TODO�F�x�����[�V�����̐ݒ�
			SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);

			// �����Ԃɂ���
			SetState(STATE_CRAWL);
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

	case CEnemyStalk::MOTION_STANDBY:	// �\��

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyStalk::MOTION_BATTLE:	// �Њd

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
// �i�r�ɂ�郂�[�V�����̐ݒ菈��
//============================================================
void CEnemyStalk::NavMotionSet(EMotion* pMotion)
{
	switch (m_pNav->GetState())
	{
	case CEnemyNav::STATE_MOVE:

		// �ړ����[�V������ݒ�
		*pMotion = MOTION_WALK;

		// ���s������
		WalkSound();

		break;

	default:

		break;
	}
}

//============================================================
// ���񏈗�
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Crawl(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	EMotion motion = MOTION_IDOL;				// ���[�V����

	// �����̈ړ�����
	RotMove(*pRot, ROT_REV, fDeltaTime);

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

		// �i�r�ɂ�郂�[�V�����ݒ菈��
		NavMotionSet(&motion);

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;
	}

	if (JudgeClone() ||
		JudgePlayer())
	{ // ���g���v���C���[���ڂɓ������ꍇ

		// �i�r�Q�[�V�������Z�b�g����
		m_pNav->NavReset();

		if (GetChaseRange()->InsideTargetPos(GetPosInit(), GetTargetPos()))
		{ // �͈͓��ɓ������ꍇ

			// �x����Ԃɂ���
			SetState(STATE_WARNING);

			// �������[�V������Ԃ�
			return MOTION_FOUND;
		}

		// �\����Ԃɂ���
		SetState(STATE_STANCE);

		// �\��������炷
		PLAY_SOUND(CSound::LABEL_SE_STALKSTANCE_000);

		// �\�����[�V������Ԃ�
		return MOTION_STANDBY;
	}

	// ���Ώۂɂ���
	SetTarget(TARGET_NONE);

	// �ҋ@���[�V������Ԃ�
	return motion;
}

//============================================================
// �x������
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Warning(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	if (m_nStateCount == sound::FOUND_COUNT)
	{ // ��莞�Ԍo�߂����ꍇ

		// ��������炷
		PLAY_SOUND(CSound::LABEL_SE_STALKFOUND_000);
	}

	// �ڕW�ʒu�̎��F����
	LookTarget(*pPos);

	// �����̈ړ�����
	RotMove(*pRot, ROT_REV, fDeltaTime);

	if (m_nStateCount % FOUND_STATE_COUNT == 0)
	{ // ��莞�Ԍo�߂����ꍇ

		// �ǐՏ�Ԃɂ���
		SetState(STATE_STALK);
	}

	// ���s��Ԃ�Ԃ�
	return MOTION_WALK;
}

//============================================================
// �ǐՏ���
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// ���s�J�E���g�����Z����
	m_nStateCount++;

	// ���s������
	WalkSound();

	if (!ShakeOffClone() &&
		!ShakeOffPlayer())
	{ // ���g���v���C���[�����E���ɂ��Ȃ��ꍇ

		// �����Ԃɂ���
		SetState(STATE_CRAWL);

		// �W�I�𖢐ݒ�ɂ���
		SetTarget(TARGET_NONE);

		// �ҋ@���[�V������Ԃ�
		return MOTION_IDOL;
	}

	// �ڕW�ʒu�̎��F����
	LookTarget(*pPos);

	// �����t�t���O�� false �ɂ���
	SetEnableDodge(false);

	// �U���J�E���g�����Z�b�g����
	SetAttackCount(0);

	// �����̈ړ�����
	RotMove(*pRot, ROT_REV, fDeltaTime);

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

		// �U�����[�V������Ԃ�
		return MOTION_ATTACK;
	}

	if (GetChaseRange() != nullptr &&
		GetChaseRange()->ChaseRange(GetPosInit(), pPos))
	{ // �ǐՔ͈͂���o���ꍇ

		// �\����Ԃɂ���
		SetState(STATE_STANCE);

		// �i�r�Q�[�V�������Z�b�g����
		m_pNav->NavReset();
	}

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

		if (HitPlayer(rPos))
		{ // �v���C���[�ɓ��������ꍇ

			// �U������炷
			PLAY_SOUND(CSound::LABEL_SE_STALKATTACK_000);
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

		break;

	case CEnemyAttack::TARGET_CLONE:

		if (HitClone(rPos))
		{ // ���g�ɓ��������ꍇ

			// �󔒍U����Ԃɂ���
			SetState(STATE_BLANKATTACK);

			// ���g�U������炷
			PLAY_SOUND(CSound::LABEL_SE_STALKATTACK_001);

			// ���g�ɑ΂���U�����[�V�����ɂ���
			return MOTION_BATTLE;
		}
		else
		{ // ��L�ȊO

			// �����Ԃɂ���
			SetState(STATE_CRAWL);
		}

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
// �󔒍U������
//============================================================
CEnemyStalk::EMotion CEnemyStalk::BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	// �����̈ړ�����
	RotMove(*pRot, ROT_REV, fDeltaTime);

	if (m_nStateCount <= BLANKATTACK_STATE_COUNT)
	{ // ���J�E���g�ȉ��̏ꍇ

		if (m_nStateCount % BLANKATTACK_CYCLE_COUNT == 0)
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

	// ���g�ɑ΂���U�����[�V�����ɂ���
	return MOTION_BATTLE;
}

//============================================================
// ���h����
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Upset(void)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	if (m_nStateCount == sound::UPSET_COUNT)
	{ // ��ԃJ�E���g����萔�ɂȂ����Ƃ�

		// ���h����炷
		PLAY_SOUND(CSound::LABEL_SE_STALKUPSET_000);
	}

	// ���h���[�V�����ɂ���
	return MOTION_UPSET;
}

//============================================================
// �\������
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Stance(void)
{
	// ���g�̔�������
	JudgeClone();

	// �v���C���[�̔�������
	JudgePlayer();

	if (GetChaseRange()->InsideTargetPos(GetPosInit(), GetTargetPos()))
	{ // �͈͓��ɓ������ꍇ

		// �x����Ԃɂ���
		SetState(STATE_WARNING);

		// �������[�V������Ԃ�
		return MOTION_FOUND;
	}

	// �\�����[�V������Ԃ�
	return MOTION_STANDBY;
}

//============================================================
// �����蔻�菈��
//============================================================
void CEnemyStalk::CollisionActor(D3DXVECTOR3& rPos, bool& bHit)
{
	// �A�N�^�[�̃��X�g�\����������Δ�����
	if (CActor::GetList() == nullptr) { return; }

	// ���X�g���擾
	std::list<CActor*> list = CActor::GetList()->GetList();
	D3DXVECTOR3 pos = GetPosInit();
	D3DXVECTOR3 vtxChase = D3DXVECTOR3(GetChaseRange()->GetWidth(), 0.0f, GetChaseRange()->GetDepth());
	D3DXVECTOR3 move = GetMovePosition();		// �ړ���
	bool bJump = IsJump();						// �W�����v��

	for (auto actor : list)
	{
		assert(actor != nullptr);

		// ���f�����ǐՔ͈͓��ɂȂ��ꍇ�A���ɐi��
		if (!collision::Box2D(pos, actor->GetVec3Position(), vtxChase, vtxChase, actor->GetModelData().vtxMax, -actor->GetModelData().vtxMin)) { continue; }

		// �����蔻�菈��
		actor->Collision
		(
			rPos,				// �ʒu
			GetOldPosition(),	// �O��̈ʒu
			GetRadius(),		// ���a
			GetHeight(),		// ����
			move,				// �ړ���
			bJump,				// �W�����v��
			bHit				// �q�b�g��
		);
	}

	// �ړ��ʂƃW�����v�󋵂�ݒ肷��
	SetMovePosition(move);
	SetEnableJump(bJump);
}

//============================================================
// ���̈ʒu�ɖ߂鏈��
//============================================================
bool CEnemyStalk::BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight)
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
void CEnemyStalk::SetState(const EState state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;

	// ��ԃJ�E���g��0�ɂ���
	m_nStateCount = 0;
}

//============================================================
// ���s������
//============================================================
void CEnemyStalk::WalkSound(void)
{
	if (m_nStateCount % sound::WALK_COUNT == 0)
	{ // ���J�E���g���Ƃ�

		// ���s����炷
		PLAY_SOUND(CSound::LABEL_SE_STALKWALK_000);
	}
}
