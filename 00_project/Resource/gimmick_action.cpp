//============================================================
//
//	�A�N�V�����M�~�b�N���� [gimmick_action.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "manager.h"
#include "gimmick_action.h"

#include "player_clone.h"
#include "player.h"
#include "collision.h"

// �萔��`
namespace
{
	const D3DXVECTOR3 CLONE_RADIUS = D3DXVECTOR3(20.0f, 0.0f, 20.0f);	// ���a
	const int INIT_NUM_ACTIVE = 2;			// �����\�l���̏����l(TODO�F��Ńf�[�^�����o��������\��)
}

//************************************************************
//	�q�N���X [CGimmickAction] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGimmickAction::CGimmickAction() : CGimmick(),
m_nNumClone(0),					// �͈͂ɓ����Ă��镪�g�̐�
m_nNumActive(INIT_NUM_ACTIVE),	// �����\�ȕ��g�̐�
m_bActive(false)				// ������
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGimmickAction::~CGimmickAction()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGimmickAction::Init(void)
{
	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CGimmick::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGimmickAction::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I��
	CGimmick::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CGimmickAction::Update(const float fDeltaTime)
{
	if (m_nNumActive <= m_nNumClone)
	{ // ���g�̑����������\�l���ȏ�̏ꍇ

		// �����\������ true �ɂ���
		m_bActive = true;
	}
	else
	{ // ��L�ȊO

		// �����\������ false �ɂ���
		m_bActive = false;
	}

	if (m_bActive)
	{ // �����ҋ@���̏ꍇ

		// �v���C���[�Ƃ̓����蔻��
		CollisionPlayer();
	}
	else
	{ // ��L�ȊO

		// ���g�Ƃ̓����蔻�菈��
		CollisionClone();
	}

	// �I�u�W�F�N�g3D�̍X�V
	CGimmick::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CGimmickAction::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CGimmick::Draw(pShader);
}

//============================================================
// �N���[���Ƃ̓����蔻��
//============================================================
void CGimmickAction::CollisionClone(void)
{
	// ���g�̃��X�g�\����������Δ�����
	if (CPlayerClone::GetList() == nullptr) { m_nNumClone = 0; return; }

	std::list<CPlayerClone*> list = CPlayerClone::GetList()->GetList();	// ���X�g���擾
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu
	D3DXVECTOR3 size = GetVec3Sizing() / 2;	// �T�C�Y
	D3DXVECTOR3 posClone = VEC3_ZERO;		// ���g�̈ʒu
	D3DXVECTOR3 sizeClone = CLONE_RADIUS;	// ���g�̃T�C�Y
	int nNumClone = 0;						// �͈͓��̕��g�̑���

	for (auto clone : list)
	{
		// �ǐՂ��镪�g�������ꍇ���̕��g�ɂ���
		if (clone->GetAction() == CPlayerClone::ACTION_CHASE) { continue; }

		// �ʒu���擾
		posClone = clone->GetVec3Position();

		if (collision::Box2D
		(
			pos,		// ����ʒu
			posClone,	// ����ڕW�ʒu
			size,		// ����T�C�Y(�E�E��E��)
			size,		// ����T�C�Y(���E���E�O)
			sizeClone,	// ����ڕW�T�C�Y(�E�E��E��)
			sizeClone	// ����ڕW�T�C�Y(���E���E�O)
		))
		{ // �l�p�̒��ɓ������ꍇ
			clone->SetGimmick(this);

			// ���g�̑��������Z����
			nNumClone++;
		}
	}

	// ���g�̑�����ݒ肷��
	m_nNumClone = nNumClone;
}

//============================================================
// �v���C���[�Ƃ̓����蔻��
//============================================================
void CGimmickAction::CollisionPlayer(void)
{
	// �v���C���[�̃��X�g�\����������Δ�����
	if (CPlayer::GetList() == nullptr) { return; }

	std::list<CPlayer*> list = CPlayer::GetList()->GetList();	// ���X�g���擾
	D3DXVECTOR3 pos = GetVec3Position();	// �ʒu
	D3DXVECTOR3 size = GetVec3Sizing() / 2;	// �T�C�Y
	D3DXVECTOR3 posPlayer = VEC3_ZERO;		// �v���C���[�̈ʒu
	D3DXVECTOR3 sizePlayer = CLONE_RADIUS;	// �v���C���[�̃T�C�Y

	for (auto player : list)
	{
		// �v���C���[���W�����v���Ă��玟�̃v���C���[��
		if (player->IsJump()) { continue; }

		// �ʒu���擾
		posPlayer = player->GetVec3Position();

		if (collision::Box2D
		(
			pos,		// ����ʒu
			posPlayer,	// ����ڕW�ʒu
			size,		// ����T�C�Y(�E�E��E��)
			size,		// ����T�C�Y(���E���E�O)
			sizePlayer,	// ����ڕW�T�C�Y(�E�E��E��)
			sizePlayer	// ����ڕW�T�C�Y(���E���E�O)
		))
		{ // �l�p�̒��ɓ������ꍇ

			// �ړ��ʂ��擾
			D3DXVECTOR3 move = player->GetMove();

			move.y = 1260.0f;

			// �ړ��ʂ�ݒ�
			player->SetMove(move);

			// �W�����v�󋵂� true �ɂ���
			player->SetEnableJump(true);
		}
	}
}
