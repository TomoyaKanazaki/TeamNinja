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
#include "stage.h"
#include "sound.h"

#include "multiModel.h"
#include "enemyChaseRange.h"
#include "enemy_item.h"
#include "camera.h"

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
	const float SPEED = -490.0f;			// ���x
	const float ROT_REV = 9.0f;				// �����̕␳�W��
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

	// ���Ǘ��֌W
	namespace sound
	{
		const int FOUND_COUNT = 37;			// ��������炷�J�E���g��
		const int UPSET_COUNT = 200;		// ���h����炷�J�E���g��
	}
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

	// ���̐ݒ菈��
	CEnemyAttack::SetData();
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

		// ����������ݒ�
		pEnemy->SetRotInit(rRot);

		// �ǐՔ͈͂𐶐�
		pEnemy->SetChaseRange(CEnemyChaseRange::Create(rPos, fChaseWidth, fChaseDepth));

		// ���̐ݒ菈��
		pEnemy->SetData();

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

	// ���̈ʒu�ɖ߂鏈���� true �̏ꍇ�A������
	if (!BackOriginPos(pPos, pRot, HEIGHT))
	{
		switch (m_state)
		{
		case CEnemyAmbush::STATE_AMBUSH:

			// �҂���������
			nCurMotion = Ambush(pPos, fDeltaTime);

			break;

		case CEnemyAmbush::STATE_WARNING:

			// �x������
			nCurMotion = Warning(pPos, pRot, fDeltaTime);

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

		case CEnemyAmbush::STATE_STANCE:

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

	// �����蔻�菈��
	Collision(*pPos);

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

		// �u�����h���̏ꍇ������
		if (GetMotionBlendFrame() != 0) { break; }

		if (GetMotionKey() % 2 == 0 && GetMotionKeyCounter() == 0)
		{ // ���������^�C�~���O�̏ꍇ

			// ���s����炷
			PLAY_SOUND(CSound::LABEL_SE_STALKWALK_000);
		}

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

			// TODO�F�x�����[�V�����̐ݒ�
			SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);

			// �҂�������Ԃɂ���
			SetState(STATE_AMBUSH);
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

	case CEnemyAmbush::MOTION_STANDBY:	// �\��

		if (IsMotionFinish())
		{ // ���[�V�������Đ��I�������ꍇ

			// ���݂̃��[�V�����̐ݒ�
			SetMotion(nMotion, BLEND_FRAME_OTHER);
		}

		break;

	case CEnemyAmbush::MOTION_BATTLE:	// �Њd

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

		// �\�����[�V������Ԃ�
		return MOTION_STANDBY;
	}

	// ���Ώۂɂ���
	SetTarget(TARGET_NONE);

	// �ҋ@���[�V������Ԃ�
	return MOTION_IDOL;
}

//============================================================
// �x������
//============================================================
CEnemyAmbush::EMotion CEnemyAmbush::Warning(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
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
CEnemyAmbush::EMotion CEnemyAmbush::Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
{
	// ���s�J�E���g�����Z����
	m_nStateCount++;

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

		// �\����Ԃɂ���
		SetState(STATE_STANCE);
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

		if (HitPlayer(rPos))
		{ // �v���C���[�ɓ��������ꍇ

			// �U������炷
			PLAY_SOUND(CSound::LABEL_SE_STALKATTACK_000);
		}

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

			// �҂�������Ԃɂ���
			SetState(STATE_AMBUSH);
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

	// ���g�ɑ΂���U�����[�V�����ɂ���
	return MOTION_BATTLE;
}

//============================================================
// ���h����
//============================================================
CEnemyAmbush::EMotion CEnemyAmbush::Upset(void)
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
CEnemyAmbush::EMotion CEnemyAmbush::Stance(void)
{
	// ���g�̔�������
	JudgeClone();

	if(PlayerIngress())
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
// ���̈ʒu�ɖ߂鏈��
//============================================================
bool CEnemyAmbush::BackOriginPos(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fHeight)
{
	// ���̏�Ԃ̏ꍇ�Afalse ��Ԃ�
	if (m_state == STATE_BLANKATTACK || m_state == STATE_UPSET) { SetRegressionCount(0); return false; }

	// �����ʒu��A�����Ɏ��s�����ꍇ�Afalse ��Ԃ�
	if (!CEnemyAttack::BackOriginPos(pPos, pRot, fHeight)) { return false; }

	// �҂�������Ԃɂ���
	SetState(STATE_AMBUSH);

	// true ��Ԃ�
	return true;
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
