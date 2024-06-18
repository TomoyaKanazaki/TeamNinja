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
	const int	BLEND_FRAME_OTHER = 5;		// ���[�V�����̊�{�I�ȃu�����h�t���[��
	const int	BLEND_FRAME_LAND = 15;		// ���[�V�������n�̃u�����h�t���[��
	const int	CAUTIOUS_TRANS_LOOP = 7;	// �x�����[�V�����ɑJ�ڂ���ҋ@���[�v��
}

//************************************************************
//	�q�N���X [CEnemyWolf] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CEnemyWolf::CEnemyWolf() : CEnemy()
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
	// �ߋ��ʒu�X�V
	UpdateOldPosition();

	// �G�̍X�V
	CEnemy::Update(fDeltaTime);

	// ���[�V�����̍X�V����
	UpdateMotion(MOTION_IDOL, fDeltaTime);
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
// ���̐ݒ菈��
//============================================================
void CEnemyWolf::SetData(void)
{

}

//============================================================
//	���[�V�����E�L�����N�^�[�̍X�V����
//============================================================
void CEnemyWolf::UpdateMotion(int nMotion, const float fDeltaTime)
{
//	// ����ł��甲����
//	if (IsDeath()) { return; }
//
//	int nAnimMotion = GetMotionType();	// ���ݍĐ����̃��[�V����
//
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
//		else
//		{ // ���[�v���Ȃ����[�V�������̏ꍇ
//
//			switch (GetMotionType())
//			{
//			case CEnemyWolf::MOTION_FOUND:		// �������[�V����
//			case CEnemyWolf::MOTION_TURN:		// �U��������[�V����
//			case CEnemyWolf::MOTION_BITE:
//
//
//
//				break;
//			}
//
//			//switch (GetMotionType())
//			//{ // ���[�V�������Ƃ̏���
//			//case MOTION_CAUTIOUS:	// �x�����[�V����
//			//case MOTION_LANDING:	// ���n���[�V����
//
//			//	if (nMotion != MOTION_IDOL)
//			//	{ // �ҋ@���[�V�����ȊO�̏ꍇ
//
//			//		// ���݂̃��[�V�����̐ݒ�
//			//		SetMotion(nMotion, BLEND_FRAME_OTHER);
//			//	}
//
//			//	break;
//			//}
//		}
//	}
//
//	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
//	CObjectChara::Update(fDeltaTime);
//
//	switch (GetMotionType())
//	{ // ���[�V�����̎�ނ��Ƃ̏���
//	case MOTION_IDOL:	// �ҋ@���[�V����
//
//		if (GetMotionNumLoop() >= CAUTIOUS_TRANS_LOOP)
//		{ // �ҋ@���[�V�����ł��΂炭�����ꍇ
//
//			// �x�����[�V�����̐ݒ�
//			SetMotion(MOTION_CAUTIOUS, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case MOTION_CAUTIOUS:	// �x�����[�V����
//
//		if (IsMotionFinish())
//		{ // ���[�V�������Đ��I�������ꍇ
//
//			// ���݂̃��[�V�����̐ݒ�
//			SetMotion(nMotion, BLEND_FRAME_LAND);
//		}
//
//		break;
//
//#if 0
//	case MOTION_DASH:	// ���s���[�V����
//
//		if (GetMotionPose() % 4 == 0 && GetMotionCounter() == 0)
//		{ // ���������^�C�~���O�̏ꍇ
//
//			switch (m_land)
//			{ // ���n�����Ƃ̏���
//			case LAND_OBSTACLE:
//
//				// �T�E���h�̍Đ�
//				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_WALK_OBS);	// ���s���i��Q���j
//
//				break;
//
//			default:
//
//				// �T�E���h�̍Đ�
//				CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_WALK_BUILD);	// ���s���i�r���j
//
//				break;
//			}
//		}
//
//		break;
//
//	case MOTION_STEALTHWALK:	// �E�ё����[�V����
//
//		break;
//#endif
//
//	case MOTION_JUMP_MINI:	// ���W�����v���[�V����
//
//		if (!m_bJump)
//		{ // �W�����v���ł͂Ȃ��ꍇ
//
//			// ���݂̃��[�V�����̐ݒ�
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case MOTION_JUMP_HIGH:	// ��W�����v���[�V����
//
//		if (!m_bJump)
//		{ // �W�����v���ł͂Ȃ��ꍇ
//
//			// ���݂̃��[�V�����̐ݒ�
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case MOTION_FALL:	// �������[�V����
//
//		if (!m_bJump)
//		{ // �W�����v���ł͂Ȃ��ꍇ
//
//			// ���݂̃��[�V�����̐ݒ�
//			SetMotion(nMotion, BLEND_FRAME_OTHER);
//		}
//
//		break;
//
//	case MOTION_LANDING:	// ���n���[�V����
//
//		if (IsMotionFinish())
//		{ // ���[�V�������Đ��I�������ꍇ
//
//			// ���݂̃��[�V�����̐ݒ�
//			SetMotion(nMotion, BLEND_FRAME_LAND);
//		}
//
//		break;
//	}
}