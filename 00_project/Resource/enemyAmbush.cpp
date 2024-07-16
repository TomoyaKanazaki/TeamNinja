////============================================================
////
////	�ǐՓG���� [enemyStalk.cpp]
////	Author�F��������
////
////============================================================
////************************************************************
////	�C���N���[�h�t�@�C��
////************************************************************
//#include "manager.h"
//#include "enemyAmbush.h"
//#include "renderer.h"
//#include "deltaTime.h"
//
//#include "multiModel.h"
//#include "enemy_item.h"
//#include "enemyChaseRange.h"
//
////************************************************************
////	�萔�錾
////************************************************************
//namespace
//{
//	const char* SETUP_TXT = "data\\CHARACTER\\enemy.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
//	const int	BLEND_FRAME_OTHER = 5;		// ���[�V�����̊�{�I�ȃu�����h�t���[��
//	const int	BLEND_FRAME_UPSET = 8;		// ���[�V�������h�̃u�����h�t���[��
//	const int	BLEND_FRAME_LAND = 15;		// ���[�V�������n�̃u�����h�t���[��
//	const int	CAUTIOUS_TRANS_LOOP = 7;	// �x�����[�V�����ɑJ�ڂ���ҋ@���[�v��
//	const float	RADIUS = 20.0f;				// ���a
//	const float HEIGHT = 80.0f;				// �g��
//	const float SPEED = -290.0f;			// ���x
//	const float ROT_REV = 4.0f;				// �����̕␳�W��
//	const int UPSET_STATE_COUNT = 3;		// ���h��Ԃ̃J�E���g��
//	const int CAUTION_STATE_COUNT = 180;	// �x����Ԃ̃J�E���g��
//	const float FADE_ALPHA_TRANS = 0.02f;	// �t�F�[�h�̓����x�̑J�ڒ萔
//
//	const int ITEM_PART_NUMBER = 8;			// �A�C�e�������p�[�c�̔ԍ�
//	const D3DXVECTOR3 ITEM_OFFSET = D3DXVECTOR3(-3.0f, -1.0f, 10.0f);		// �A�C�e���̃I�t�Z�b�g���W
//	const D3DXVECTOR3 ITEM_ROT = D3DXVECTOR3(-D3DX_PI * 0.5f, 0.0f, 0.0f);	// �A�C�e���̌���
//}
//
////************************************************************
////	�q�N���X [CEnemyAmbush] �̃����o�֐�
////************************************************************
////============================================================
////	�R���X�g���N�^
////============================================================
//CEnemyAmbush::CEnemyAmbush() : CEnemyAttack(),
//m_state(STATE_WAIT),
//m_nStateCount(0),
//m_nNumUpsetLoop(0)
//{
//
//}
//
////============================================================
////	�f�X�g���N�^
////============================================================
//CEnemyAmbush::~CEnemyAmbush()
//{
//
//}
//
////============================================================
////	����������
////============================================================
//HRESULT CEnemyAmbush::Init(void)
//{
//	// �G�̏�����
//	if (FAILED(CEnemyAttack::Init()))
//	{ // �������Ɏ��s�����ꍇ
//
//		// ���s��Ԃ�
//		assert(false);
//		return E_FAIL;
//	}
//
//	// �L�����N�^�[���̊���
//	BindCharaData(SETUP_TXT);
//
//	// ������Ԃ�
//	return S_OK;
//}
//
////============================================================
////	�I������
////============================================================
//void CEnemyAmbush::Uninit(void)
//{
//	// �G�̏I��
//	CEnemyAttack::Uninit();
//}
//
////============================================================
////	�X�V����
////============================================================
//void CEnemyAmbush::Update(const float fDeltaTime)
//{
//	// �G�̍X�V
//	CEnemyAttack::Update(fDeltaTime);
//}
//
////============================================================
////	�`�揈��
////============================================================
//void CEnemyAmbush::Draw(CShader* pShader)
//{
//	// �G�̕`��
//	CEnemyAttack::Draw(pShader);
//}
//
////============================================================
//// ���̐ݒ菈��
////============================================================
//void CEnemyAmbush::SetData(void)
//{
//	// �A�C�e����ݒ肷��
//	SetItem(CEnemyItem::Create
//	(
//		CEnemyItem::TYPE_KATANA,
//		ITEM_OFFSET,
//		ITEM_ROT
//	));
//
//	// �e�I�u�W�F�N�g (������) �̐ݒ�
//	GetItem()->SetParentObject(GetParts(ITEM_PART_NUMBER));
//}
//
////============================================================
//// ���a�̎擾����
////============================================================
//float CEnemyAmbush::GetRadius(void) const
//{
//	// ���a��Ԃ�
//	return RADIUS;
//}
//
////============================================================
//// �����̎擾����
////============================================================
//float CEnemyAmbush::GetHeight(void) const
//{
//	// �g����Ԃ�
//	return HEIGHT;
//}
//
////============================================================
//// ��Ԃ̍X�V����
////============================================================
//int CEnemyAmbush::UpdateState(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
//{
//	int nCurMotion = MOTION_IDOL;	// ���݂̃��[�V����
//	switch (m_state)
//	{
//	case CEnemyAmbush::STATE_WAIT:
//
//		// �ҋ@����
//		nCurMotion = Wait(pPos, pRot, fDeltaTime);
//
//		break;
//
//	case CEnemyAmbush::STATE_WARNING:
//
//		// �x������
//		nCurMotion = Warning(pPos);
//
//		break;
//
//	case CEnemyAmbush::STATE_STALK:
//
//		// �ǐՏ���
//		nCurMotion = Stalk(pPos, pRot, fDeltaTime);
//
//		break;
//
//	case CEnemyAmbush::STATE_ATTACK:
//
//		// �U������
//		nCurMotion = Attack(*pPos);
//
//		break;
//
//	case CEnemyAmbush::STATE_UPSET:
//
//		// ���h����
//		nCurMotion = Upset();
//
//		break;
//
//	case CEnemyAmbush::STATE_CAUTION:
//
//		// �x������
//		nCurMotion = Caution();
//
//		break;
//
//	case CEnemyAmbush::STATE_FADEOUT:
//
//		// �t�F�[�h�A�E�g����
//		nCurMotion = FadeOut(pPos, pRot);
//
//		break;
//
//	case CEnemyAmbush::STATE_FADEIN:
//
//		// �t�F�[�h�C������
//		nCurMotion = FadeIn();
//
//		break;
//
//	default:
//
//		// ��~
//		assert(false);
//
//		break;
//	}
//
//	// �����蔻�菈��
//	Collision(*pPos);
//
//	// ���݂̃��[�V������Ԃ�
//	return nCurMotion;
//}
//
////============================================================
//// ���[�V�����̍X�V����
////============================================================
//void CEnemyAmbush::UpdateMotion(int nMotion, const float fDeltaTime)
//{
//	// ����ł��甲����
//	if (IsDeath()) { return; }
//
//	int nAnimMotion = GetMotionType();	// ���ݍĐ����̃��[�V����
//	if (nMotion != NONE_IDX)
//	{ // ���[�V�������ݒ肳��Ă���ꍇ
//
//		if (IsMotionLoop())
//		{ // ���[�v���郂�[�V�������̏ꍇ
//
//			if (nAnimMotion != nMotion)
//			{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ
//
//				// ���݂̃��[�V�����̐ݒ�
//				SetMotion(nMotion, BLEND_FRAME_OTHER);
//			}
//		}
//	}
//
//	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
//	CObjectChara::Update(fDeltaTime);
//
//	// ���݂̃��[�V�����̍X�V
//	switch (GetMotionType())
//	{
//	case CEnemyAmbush::MOTION_IDOL:		// �ҋ@
//		break;
//
//	case CEnemyAmbush::MOTION_WALK:		// ���s
//		break;
//
//	case CEnemyAmbush::MOTION_FOUND:		// ����
//
//		if (IsMotionFinish())
//		{ // ���[�V�������Đ��I�������ꍇ
//
//			// ���݂̃��[�V�����̐ݒ�
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case CEnemyAmbush::MOTION_ATTACK:	// �U��
//
//		if (IsMotionFinish())
//		{ // ���[�V�������Đ��I�������ꍇ
//
//			// ���݂̃��[�V�����̐ݒ�
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case CEnemyAmbush::MOTION_UPSET:		// ���h
//
//		if (IsMotionFinish())
//		{ // ���[�V�������Đ��I�������ꍇ
//
//			// ���h���[�V�����̃��[�v�������Z����
//			m_nNumUpsetLoop++;
//
//			if (m_nNumUpsetLoop < UPSET_STATE_COUNT)
//			{ // ���h���[�V�����̃��[�v�񐔂���萔�����̏ꍇ
//
//				// �ҋ@���[�V�����̐ݒ�
//				SetMotion(MOTION_UPSET, BLEND_FRAME_UPSET);
//			}
//			else
//			{ // ��L�ȊO
//
//				// �ҋ@���[�V�����̐ݒ�
//				SetMotion(MOTION_IDOL, BLEND_FRAME_OTHER);
//			}
//		}
//
//		break;
//
//	case CEnemyAmbush::MOTION_FALL:		// ����
//
//		if (IsMotionFinish())
//		{ // ���[�V�������Đ��I�������ꍇ
//
//			// ���݂̃��[�V�����̐ݒ�
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case CEnemyAmbush::MOTION_LANDING:	// ���n
//
//		if (IsMotionFinish())
//		{ // ���[�V�������Đ��I�������ꍇ
//
//			// ���݂̃��[�V�����̐ݒ�
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	default:
//
//		// ��~
//		assert(false);
//
//		break;
//	}
//}
//
////============================================================
//// ���n�̍X�V����
////============================================================
//void CEnemyAmbush::UpdateLanding(D3DXVECTOR3* pPos)
//{
//	// �e�N���X�̒��n�X�V
//	CEnemy::UpdateLanding(pPos);
//
//	// ���݂̃��[�V������ނ��擾
//	int nCurMotion = GetMotionType();
//
//	//// �������[�V�����̃t���O��ݒ�
//	//bool bTypeFall = nCurMotion == MOTION_FALL;
//
//	//if (!IsJump())
//	//{ // �󒆂ɂ��Ȃ��ꍇ
//
//	//	if (bTypeFall)
//	//	{ // ���[�V�������������̏ꍇ
//
//	//		// ���n���[�V�������w��
//	//		SetMotion(MOTION_LANDING);
//	//	}
//	//}
//	//else
//	//{ // �󒆂ɂ���ꍇ
//
//	//	// �������[�V�������w��
//	//	SetMotion(MOTION_FALL);
//	//}
//}
//
////============================================================
//// �ҋ@����
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::Wait(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
//{
//	EMotion motion = MOTION_IDOL;				// ���[�V����
//
//	// �d�͂̍X�V
//	UpdateGravity();
//
//	// �G�𗎉�������
//	pPos->y += GetMovePosition().y * fDeltaTime;
//
//	// �����̈ړ�����
//	RotMove(*pRot, ROT_REV, fDeltaTime);
//
//	// ���n����
//	UpdateLanding(pPos);
//
//	if (JudgeClone() ||
//		JudgePlayer())
//	{ // ���g���v���C���[���ڂɓ������ꍇ
//
//		// �x����Ԃɂ���
//		m_state = STATE_WARNING;
//
//		// �������[�V������Ԃ�
//		return MOTION_FOUND;
//	}
//
//	// ���Ώۂɂ���
//	SetTarget(TARGET_NONE);
//
//	// �ҋ@���[�V������Ԃ�
//	return motion;
//}
//
////============================================================
//// �x������
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::Warning(D3DXVECTOR3* pPos)
//{
//	// �d�͂̍X�V
//	UpdateGravity();
//
//	// ���n����
//	UpdateLanding(pPos);
//
//	if (GetMotionType() != MOTION_FOUND)
//	{ // �������[�V��������Ȃ������ꍇ
//
//		// �ǐՏ�Ԃɂ���
//		m_state = STATE_STALK;
//	}
//
//	// ���s��Ԃ�Ԃ�
//	return MOTION_WALK;
//}
//
////============================================================
//// �ǐՏ���
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::Stalk(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, const float fDeltaTime)
//{
//	if (ShakeOffClone() ||
//		ShakeOffPlayer())
//	{ // ���g���v���C���[���ڂɓ������ꍇ
//
//		// �ڕW�ʒu�̎��F����
//		LookTarget(*pPos);
//
//		// �U������� false �ɂ���
//		SetEnableAttack(false);
//	}
//	else
//	{ // ��L�ȊO
//
//		// �ҋ@��Ԃɂ���
//		m_state = STATE_WAIT;
//
//		// �ҋ@���[�V������Ԃ�
//		return MOTION_IDOL;
//	}
//
//	// �����̈ړ�����
//	RotMove(*pRot, ROT_REV, fDeltaTime);
//
//	// �d�͂̍X�V
//	UpdateGravity();
//
//	// �ړ�����
//	Move(pPos, *pRot, SPEED, fDeltaTime);
//
//	// ���n����
//	UpdateLanding(pPos);
//
//	if (Approach(*pPos))
//	{ // �ڋ߂����ꍇ
//
//		// �U����Ԃɂ���
//		m_state = STATE_ATTACK;
//
//		// �U�����[�V������Ԃ�
//		return MOTION_ATTACK;
//	}
//
//	if (GetChaseRange() != nullptr &&
//		GetChaseRange()->ChaseRange(pPos))
//	{ // �ǐՔ͈͂���o���ꍇ
//
//		// �t�F�[�h�A�E�g��Ԃɂ���
//		m_state = STATE_FADEOUT;
//
//		// �ړ��ʂ����Z�b�g����
//		SetMovePosition(VEC3_ZERO);
//
//		// �^�[�Q�b�g�𖳑Ώۂɂ���
//		SetTarget(TARGET_NONE);
//	}
//
//	// ���s���[�V������Ԃ�
//	return MOTION_WALK;
//}
//
////============================================================
//// �U������
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::Attack(const D3DXVECTOR3& rPos)
//{
//	switch (GetTarget())
//	{
//	case CEnemyAttack::TARGET_PLAYER:
//
//		// �v���C���[�̓����蔻�菈��
//		HitPlayer(rPos);
//
//		if (GetMotionType() != MOTION_ATTACK)
//		{ // �U�����[�V��������Ȃ��ꍇ
//
//			// �ҋ@��Ԃɂ���
//			m_state = STATE_WAIT;
//		}
//
//		// �ҋ@���[�V�����ɂ���
//		return MOTION_IDOL;
//
//		break;
//
//	case CEnemyAttack::TARGET_CLONE:
//
//		// ���g�̓����蔻�菈��
//		HitClone(rPos);
//
//		// ���h��Ԃɂ���
//		m_state = STATE_UPSET;
//
//		// ���h���[�V�����ɂ���
//		return MOTION_UPSET;
//
//		break;
//
//	default:		// ��O����
//		assert(false);
//		break;
//	}
//
//	// �ҋ@���[�V������Ԃ�
//	return MOTION_IDOL;
//}
//
////============================================================
//// ���h����
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::Upset(void)
//{
//	if (m_nNumUpsetLoop >= UPSET_STATE_COUNT)
//	{ // ��莞�Ԍo�߂����ꍇ
//
//		// �x����Ԃɂ���
//		m_state = STATE_CAUTION;
//
//		// ���h���[�V�����̃��[�v����0�ɂ���
//		m_nNumUpsetLoop = 0;
//
//		// �ҋ@���[�V�����ɂ���
//		return MOTION_IDOL;
//	}
//
//	// ���h���[�V�����ɂ���
//	return MOTION_UPSET;
//}
//
////============================================================
//// �x������
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::Caution(void)
//{
//	// ��ԃJ�E���g�����Z����
//	m_nStateCount++;
//
//	if (m_nStateCount % CAUTION_STATE_COUNT == 0)
//	{ // ��ԃJ�E���g����萔�ɂȂ����ꍇ
//
//		// ��ԃJ�E���g��0�ɂ���
//		m_nStateCount = 0;
//
//		// �t�F�[�h�A�E�g��Ԃɂ���
//		m_state = STATE_FADEOUT;
//	}
//
//	if (JudgeClone() ||
//		JudgePlayer())
//	{ // ���g���v���C���[���ڂɓ������ꍇ
//
//		// ��ԃJ�E���g��0�ɂ���
//		m_nStateCount = 0;
//
//		// �x����Ԃɂ���
//		m_state = STATE_WARNING;
//
//		// �������[�V������Ԃ�
//		return MOTION_FOUND;
//	}
//	else
//	{ // ��L�ȊO
//
//		// ���Ώۂɂ���
//		SetTarget(TARGET_NONE);
//	}
//
//	// TODO�F�ҋ@���[�V������Ԃ�
//	return MOTION_IDOL;
//}
//
////============================================================
//// �t�F�[�h�A�E�g����
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::FadeOut(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot)
//{
//	// �����x���擾
//	float fAlpha = GetAlpha();
//
//	// �����x�����Z����
//	fAlpha -= FADE_ALPHA_TRANS;
//
//	if (fAlpha <= 0.0f)
//	{ // �����x��0�ȉ��ɂȂ����ꍇ
//
//		// �t�F�[�h�C����Ԃɂ���
//		m_state = STATE_FADEIN;
//
//		// �ʒu��ݒ肷��
//		*pPos = GetPosInit();
//
//		// �ߋ��̈ʒu��K�p����(�������Ȃ��Ɠ����蔻��Ɉ����������Ă��܂�)
//		SetOldPosition(*pPos);
//
//		// ������ݒ肷��
//		*pRot = VEC3_ZERO;
//
//		// �����x��␳����
//		fAlpha = 0.0f;
//	}
//
//	// �����x��K�p
//	CObjectChara::SetAlpha(fAlpha);
//	CEnemyAttack::SetAlpha(fAlpha);
//
//	// �ҋ@���[�V�����ɂ���
//	return MOTION_IDOL;
//}
//
////============================================================
//// �t�F�[�h�C������
////============================================================
//CEnemyAmbush::EMotion CEnemyAmbush::FadeIn(void)
//{
//	// �����x���擾
//	float fAlpha = GetAlpha();
//
//	// �����x�����Z����
//	fAlpha += FADE_ALPHA_TRANS;
//
//	if (fAlpha >= 1.0f)
//	{ // �����x����萔�ȏ�ɂȂ����ꍇ
//
//		// �ҋ@��Ԃɂ���
//		m_state = STATE_WAIT;
//
//		// �����x��␳����
//		fAlpha = 1.0f;
//	}
//
//	// �����x��K�p
//	CObjectChara::SetAlpha(fAlpha);
//	CEnemyAttack::SetAlpha(fAlpha);
//
//	// �ҋ@���[�V�����ɂ���
//	return MOTION_IDOL;
//}
