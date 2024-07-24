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
#include "sound.h"

#include "multiModel.h"
#include "enemyNavigation.h"
#include "enemyNavStreet.h"
#include "enemyNavRandom.h"
#include "enemyChaseRange.h"
#include "enemy_item.h"

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

	// ���Ǘ��֌W
	const int WALK_SOUND_COUNT = 32;			// ���s����炷�J�E���g��
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
m_nStateCount(0)
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
	switch (m_state)
	{
	case CEnemyStalk::STATE_CRAWL:

		// ���񏈗�
		nCurMotion = Crawl(pPos, pRot, fDeltaTime);

		break;

	case CEnemyStalk::STATE_WARNING:

		// �x������
		nCurMotion = Warning(pPos, fDeltaTime);

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

	case CEnemyStalk::STATE_CAUTION:

		// �x������
		nCurMotion = Caution();

		break;

	case CEnemyStalk::STATE_FADEOUT:

		// �t�F�[�h�A�E�g����
		nCurMotion = FadeOut(pPos, pRot);

		break;

	case CEnemyStalk::STATE_FADEIN:

		// �t�F�[�h�C������
		nCurMotion = FadeIn();

		break;

	case CEnemyStalk::STATE_STANCE:

		// �\������
		nCurMotion = Stance();

		break;

	case CEnemyStalk::STATE_THREAT:

		// �Њd����
		nCurMotion = Threat();

		break;

	default:

		// ��~
		assert(false);

		break;
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

			// �x����Ԃɂ���
			SetState(STATE_CAUTION);
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

		// TODO�F�\�����[�V������Ԃ�
		return MOTION_WALK;
	}

	// ���Ώۂɂ���
	SetTarget(TARGET_NONE);

	// �ҋ@���[�V������Ԃ�
	return motion;
}

//============================================================
// �x������
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Warning(D3DXVECTOR3* pPos, const float fDeltaTime)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

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

		// �t�F�[�h�A�E�g��Ԃɂ���
		SetState(STATE_FADEOUT);

		// �i�r�Q�[�V�������Z�b�g����
		m_pNav->NavReset();

		// �ړ��ʂ����Z�b�g����
		SetMovePosition(VEC3_ZERO);

		// �^�[�Q�b�g�𖳑Ώۂɂ���
		SetTarget(TARGET_NONE);
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

			// ���g�U������炷
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

			// �U�����[�V�����ɂ���
			return MOTION_ATTACK;
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

	// ���h���[�V�����ɂ���
	return MOTION_ATTACK;
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
// �x������
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Caution(void)
{
	// ��ԃJ�E���g�����Z����
	m_nStateCount++;

	if (m_nStateCount % CAUTION_STATE_COUNT == 0)
	{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ

		// �t�F�[�h�A�E�g��Ԃɂ���
		SetState(STATE_FADEOUT);
	}

	if (JudgeClone() ||
		JudgePlayer())
	{ // ���g���v���C���[���ڂɓ������ꍇ

		// �i�r�Q�[�V�������Z�b�g����
		m_pNav->NavReset();

		// �x����Ԃɂ���
		SetState(STATE_WARNING);

		// �������[�V������Ԃ�
		return MOTION_FOUND;
	}
	else
	{ // ��L�ȊO

		// ���Ώۂɂ���
		SetTarget(TARGET_NONE);
	}

	// TODO�F�ҋ@���[�V������Ԃ�
	return MOTION_IDOL;
}

//============================================================
// �t�F�[�h�A�E�g����
//============================================================
CEnemyStalk::EMotion CEnemyStalk::FadeOut(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot)
{
	// �����x���擾
	float fAlpha = GetAlpha();

	// �����x�����Z����
	fAlpha -= FADE_ALPHA_TRANS;

	if (fAlpha <= 0.0f)
	{ // �����x��0�ȉ��ɂȂ����ꍇ

		// �t�F�[�h�C����Ԃɂ���
		SetState(STATE_FADEIN);

		// �ʒu��ݒ肷��
		*pPos = GetPosInit();

		// �ߋ��̈ʒu��K�p����(�������Ȃ��Ɠ����蔻��Ɉ����������Ă��܂�)
		SetOldPosition(*pPos);

		// ������ݒ肷��
		*pRot = VEC3_ZERO;

		// �ړI�̌�����ݒ肷��(������ɖ��Ӗ��Ɍ����Ă��܂�����)
		SetDestRotation(*pRot);

		// �����x��␳����
		fAlpha = 0.0f;
	}

	// �����x��K�p
	CObjectChara::SetAlpha(fAlpha);
	CEnemy::SetAlpha(fAlpha);

	// �ҋ@���[�V�����ɂ���
	return MOTION_IDOL;
}

//============================================================
// �t�F�[�h�C������
//============================================================
CEnemyStalk::EMotion CEnemyStalk::FadeIn(void)
{
	// �����x���擾
	float fAlpha = GetAlpha();

	// �����x�����Z����
	fAlpha += FADE_ALPHA_TRANS;

	if (fAlpha >= 1.0f)
	{ // �����x����萔�ȏ�ɂȂ����ꍇ

		// �����Ԃɂ���
		SetState(STATE_CRAWL);

		// �����x��␳����
		fAlpha = 1.0f;
	}

	// �����x��K�p
	CObjectChara::SetAlpha(fAlpha);
	CEnemy::SetAlpha(fAlpha);

	// �ҋ@���[�V�����ɂ���
	return MOTION_IDOL;
}

//============================================================
// �\������
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Stance(void)
{
	if (!JudgeClone() &&
		!JudgePlayer())
	{ // ���g���v���C���[�����E���狏�Ȃ��Ȃ����ꍇ

		// �Њd��Ԃɂ���
		SetState(STATE_THREAT);

		// TODO�F�Њd���[�V������Ԃ�
		return MOTION_FALL;
	}

	if (GetChaseRange()->InsideTargetPos(GetPosInit(), GetTargetPos()))
	{ // �͈͓��ɓ������ꍇ

		// �x����Ԃɂ���
		SetState(STATE_WARNING);

		// �������[�V������Ԃ�
		return MOTION_FOUND;
	}

	// TODO�F�\�����[�V������Ԃ�
	return MOTION_WALK;
}

//============================================================
// �Њd����
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Threat(void)
{
	// �t�F�[�h�A�E�g��Ԃɂ���
	SetState(STATE_FADEOUT);

	// TODO�F���s��Ԃ�Ԃ�
	return MOTION_WALK;
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
	if (m_nStateCount % WALK_SOUND_COUNT == 0)
	{ // ���J�E���g���Ƃ�

		// ���s����炷
		PLAY_SOUND(CSound::LABEL_SE_STALKWALK_000);
	}
}
