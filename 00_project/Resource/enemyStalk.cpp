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
#include "enemyChaseRange.h"
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
	const float ROT_REV = 4.0f;				// �����̕␳�W��
	const int CAUTION_STATE_COUNT = 180;	// �x����Ԃ̃J�E���g��
	const float FADE_ALPHA_TRANS = 0.02f;	// �t�F�[�h�̓����x�̑J�ڒ萔

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
		nCurMotion = Warning(pPos);

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

	default:

		// ��~
		assert(false);

		break;
	}

	if (Collision(*pPos))
	{ // �����蔻�肪 true �������ꍇ

		// �i�r�Q�[�V�����̃��Z�b�g����
		GetNavigation()->NavReset();
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

	//// �������[�V�����̃t���O��ݒ�
	//bool bTypeFall = nCurMotion == MOTION_FALL;

	//if (!IsJump())
	//{ // �󒆂ɂ��Ȃ��ꍇ

	//	if (bTypeFall)
	//	{ // ���[�V�������������̏ꍇ

	//		// ���n���[�V�������w��
	//		SetMotion(MOTION_LANDING);
	//	}
	//}
	//else
	//{ // �󒆂ɂ���ꍇ

	//	// �������[�V�������w��
	//	SetMotion(MOTION_FALL);
	//}
}

//============================================================
// �i�r�ɂ�郂�[�V�����̐ݒ菈��
//============================================================
void CEnemyStalk::NavMotionSet(EMotion* pMotion)
{
	switch (GetNavigation()->GetState())
	{
	case CEnemyNav::STATE_MOVE:

		// �ړ����[�V������ݒ�
		*pMotion = MOTION_WALK;

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

	// �d�͂̍X�V
	UpdateGravity();

	// �G�𗎉�������
	pPos->y += GetMovePosition().y * fDeltaTime;

	// �����̈ړ�����
	RotMove(*pRot, ROT_REV, fDeltaTime);

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

		// �i�r�ɂ�郂�[�V�����ݒ菈��
		NavMotionSet(&motion);
	}

	// ���n����
	UpdateLanding(pPos);

	if (JudgeClone() ||
		JudgePlayer())
	{ // ���g���v���C���[���ڂɓ������ꍇ

		// �i�r�Q�[�V�������Z�b�g����
		GetNavigation()->NavReset();

		// �x����Ԃɂ���
		m_state = STATE_WARNING;

		// �������[�V������Ԃ�
		return MOTION_FOUND;
	}

	// ���Ώۂɂ���
	SetTarget(TARGET_NONE);

	// �ҋ@���[�V������Ԃ�
	return motion;
}

//============================================================
// �x������
//============================================================
CEnemyStalk::EMotion CEnemyStalk::Warning(D3DXVECTOR3* pPos)
{
	// �d�͂̍X�V
	UpdateGravity();

	// ���n����
	UpdateLanding(pPos);

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
	if (ShakeOffClone() ||
		ShakeOffPlayer())
	{ // ���g���v���C���[���ڂɓ������ꍇ

		// �ڕW�ʒu�̎��F����
		LookTarget(*pPos);

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

	// �����̈ړ�����
	RotMove(*pRot, ROT_REV, fDeltaTime);

	// �d�͂̍X�V
	UpdateGravity();

	// �ړ�����
	Move(pPos, *pRot, SPEED, fDeltaTime);

	// ���n����
	UpdateLanding(pPos);

	if (Approach(*pPos))
	{ // �ڋ߂����ꍇ

		// �U����Ԃɂ���
		m_state = STATE_ATTACK;

		// �U�����[�V������Ԃ�
		return MOTION_ATTACK;
	}

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
	if (GetMotionType() != MOTION_ATTACK &&
		GetMotionType() != MOTION_UPSET)
	{ // ���h���[�V��������Ȃ������ꍇ

		// �x����Ԃɂ���
		m_state = STATE_CAUTION;

		// �ҋ@���[�V�����ɂ���
		return MOTION_IDOL;
	}

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

		// ��ԃJ�E���g��0�ɂ���
		m_nStateCount = 0;

		// �t�F�[�h�A�E�g��Ԃɂ���
		m_state = STATE_FADEOUT;
	}

	if (JudgeClone() ||
		JudgePlayer())
	{ // ���g���v���C���[���ڂɓ������ꍇ

		// ��ԃJ�E���g��0�ɂ���
		m_nStateCount = 0;

		// �i�r�Q�[�V�������Z�b�g����
		GetNavigation()->NavReset();

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

	// TODO�F�U�����[�V������Ԃ�
	return MOTION_ATTACK;
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
