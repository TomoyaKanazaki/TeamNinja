//============================================================
//
//	�҂������G���� [enemyAmbush.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "enemyAmbush.h"
#include "renderer.h"
#include "deltaTime.h"

#include "multiModel.h"
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
	const float SPEED = -370.0f;			// ���x
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
}

//************************************************************
//	�q�N���X [CEnemyAmbush] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyAmbush::CEnemyAmbush() : CEnemyAttack(),
m_state(STATE_AMBUSH),
m_nStateCount(0)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CEnemyAmbush::~CEnemyAmbush()
{

}

//============================================================
//	����������
//============================================================
HRESULT CEnemyAmbush::Init(void)
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
void CEnemyAmbush::Uninit(void)
{
	// �G�̏I��
	CEnemyAttack::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CEnemyAmbush::Update(const float fDeltaTime)
{
	// �G�̍X�V
	CEnemyAttack::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CEnemyAmbush::Draw(CShader* pShader)
{
	// �G�̕`��
	CEnemyAttack::Draw(pShader);
}

//============================================================
// ���̐ݒ菈��
//============================================================
void CEnemyAmbush::SetData(void)
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
float CEnemyAmbush::GetRadius(void) const
{
	// ���a��Ԃ�
	return RADIUS;
}

//============================================================
// �����̎擾����
//============================================================
float CEnemyAmbush::GetHeight(void) const
{
	// �g����Ԃ�
	return HEIGHT;
}

//============================================================
//	��������(���[�g�҂������ړ��G)
//============================================================
CEnemyAmbush* CEnemyAmbush::Create
(
	const D3DXVECTOR3& rPos,				// �ʒu
	const D3DXVECTOR3& rRot,				// ����
	const float fChaseWidth,				// �ǐՕ�
	const float fChaseDepth					// �ǐՉ��s
)
{
	// �G�𐶐�
	CEnemyAmbush* pEnemy = new CEnemyAmbush;

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
		pEnemy->SetType(TYPE_AMBUSH);

		// �����ʒu��ݒ�
		pEnemy->SetPosInit(rPos);

		// ���̐ݒ菈��
		pEnemy->SetData();

		// �ǐՔ͈͂𐶐�
		pEnemy->SetChaseRange(CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth));

		// �m�ۂ����A�h���X��Ԃ�
		return pEnemy;
	}
}

//============================================================
// ��Ԃ̍X�V����
//============================================================
int CEnemyAmbush::UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	int nCurMotion = MOTION_IDOL;	// ���݂̃��[�V����
	switch (m_state)
	{
	case CEnemyAmbush::STATE_AMBUSH:

		// �҂���������
		nCurMotion = Ambush(pPos, fDeltaTime);

		break;

	case CEnemyAmbush::STATE_WARNING:

		// �x������
		nCurMotion = Warning(pPos, fDeltaTime);

		break;

	case CEnemyAmbush::STATE_STALK:

		// �ǐՏ���
		nCurMotion = Stalk(pPos, pRot, fDeltaTime);

		break;

	case CEnemyAmbush::STATE_ATTACK:

		// �U������
		nCurMotion = Attack(*pPos);

		break;

	case CEnemyAmbush::STATE_BLANKATTACK:

		// �󔒍U������
		nCurMotion = BlankAttack(pRot, fDeltaTime);

		break;

	case CEnemyAmbush::STATE_UPSET:

		// ���h����
		nCurMotion = Upset();

		break;

	case CEnemyAmbush::STATE_CAUTION:

		// �x������
		nCurMotion = Caution();

		break;

	case CEnemyAmbush::STATE_FADEOUT:

		// �t�F�[�h�A�E�g����
		nCurMotion = FadeOut(pPos, pRot);

		break;

	case CEnemyAmbush::STATE_FADEIN:

		// �t�F�[�h�C������
		nCurMotion = FadeIn();

		break;

	case CEnemyAmbush::STATE_STANCE:

		// �\������
		nCurMotion = Stance();

		break;

	case CEnemyAmbush::STATE_THREAT:

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

	// �����蔻�菈��
	Collision(*pPos);

	// ���n���菈��
	UpdateLanding(pPos);

	// ���݂̃��[�V������Ԃ�
	return nCurMotion;
}

//============================================================
// ���[�V�����̍X�V����
//============================================================
void CEnemyAmbush::UpdateMotion(int nMotion, const float fDeltaTime)
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
	case CEnemyAmbush::MOTION_IDOL:		// �ҋ@
		break;

	case CEnemyAmbush::MOTION_WALK:		// ���s
		break;

	case CEnemyAmbush::MOTION_FOUND:		// ����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyAmbush::MOTION_ATTACK:	// �U��

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyAmbush::MOTION_UPSET:		// ���h

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// �ҋ@���[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyAmbush::MOTION_FALL:		// ����

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyAmbush::MOTION_LANDING:	// ���n

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
void CEnemyAmbush::UpdateLanding(D3DXVECTOR3* pPos)
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
// �҂���������
//============================================================
CEnemyAmbush::EMotion CEnemyAmbush::Ambush(D3DXVECTOR3* pPos, const float fDeltaTime)
{
	if (JudgeClone() ||
		JudgePlayer())
	{ // ���g���v���C���[���ڂɓ������ꍇ

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
	return MOTION_IDOL;
}

//============================================================
// �x������
//============================================================
CEnemyAmbush::EMotion CEnemyAmbush::Warning(D3DXVECTOR3* pPos, const float fDeltaTime)
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
CEnemyAmbush::EMotion CEnemyAmbush::Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	if (!ShakeOffClone() &&
		!ShakeOffPlayer())
	{ // ���g���v���C���[�����E���ɂ��Ȃ��ꍇ

		// �҂�������Ԃɂ���
		SetState(STATE_AMBUSH);

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
CEnemyAmbush::EMotion CEnemyAmbush::Attack(const D3DXVECTOR3& rPos)
{
	switch (GetTarget())
	{
	case CEnemyAttack::TARGET_PLAYER:

		// �v���C���[�̓����蔻�菈��
		HitPlayer(rPos);

		// ��ԃJ�E���g�����Z����
		m_nStateCount++;

		if (m_nStateCount % ATTACK_STATE_COUNT == 0)
		{ // ���J�E���g�o�߂����ꍇ

			// �҂�������Ԃɂ���
			SetState(STATE_AMBUSH);
		}

		// �ҋ@���[�V�����ɂ���
		return MOTION_IDOL;

		break;

	case CEnemyAttack::TARGET_CLONE:

		// ���g�̓����蔻�菈��
		HitClone(rPos);

		if (HitClone(rPos))
		{ // ���g�ɓ��������ꍇ

			// �󔒍U����Ԃɂ���
			SetState(STATE_BLANKATTACK);

			// �U�����[�V�����ɂ���
			return MOTION_ATTACK;
		}
		else
		{ // ��L�ȊO

			// �҂�������Ԃɂ���
			SetState(STATE_AMBUSH);
		}

		// ���h��Ԃɂ���
		SetState(STATE_UPSET);

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
CEnemyAmbush::EMotion CEnemyAmbush::BlankAttack(D3DXVECTOR3* pRot, const float fDeltaTime)
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
CEnemyAmbush::EMotion CEnemyAmbush::Upset(void)
{
	// ���h���[�V�����ɂ���
	return MOTION_UPSET;
}

//============================================================
// �x������
//============================================================
CEnemyAmbush::EMotion CEnemyAmbush::Caution(void)
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
CEnemyAmbush::EMotion CEnemyAmbush::FadeOut(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot)
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
CEnemyAmbush::EMotion CEnemyAmbush::FadeIn(void)
{
	// �����x���擾
	float fAlpha = GetAlpha();

	// �����x�����Z����
	fAlpha += FADE_ALPHA_TRANS;

	if (fAlpha >= 1.0f)
	{ // �����x����萔�ȏ�ɂȂ����ꍇ

		// �҂�������Ԃɂ���
		SetState(STATE_AMBUSH);

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
CEnemyAmbush::EMotion CEnemyAmbush::Stance(void)
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
CEnemyAmbush::EMotion CEnemyAmbush::Threat(void)
{
	// �t�F�[�h�A�E�g��Ԃɂ���
	SetState(STATE_FADEOUT);

	// TODO�F���s��Ԃ�Ԃ�
	return MOTION_WALK;
}

//============================================================
// ��Ԃ̐ݒ菈��
//============================================================
void CEnemyAmbush::SetState(const EState state)
{
	// ��Ԃ�ݒ肷��
	m_state = state;

	// ��ԃJ�E���g��0�ɂ���
	m_nStateCount = 0;
}
