//=========================================
//
//  ���ݑ�M�~�b�N (gimmick_step.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_step.h"
#include "manager.h"
#include "player.h"
#include "player_clone.h"
#include "collision.h"

//===========================================
//  �萔��`
//===========================================
namespace
{
	const float CLIMB_SPEED	= 600.0f;	// 1�b�Ԃɓo�鑬�x
	const float RADIUS_ROT	= 180.0f;	// ������ύX���Ȃ��v���C���[�Ƃ̋���
}

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickStep::CGimmickStep() : CGimmickAction(),
m_bClimb(false) // �o���Ă���t���O
{

}

//=========================================
//  �f�X�g���N�^
//=========================================
CGimmickStep::~CGimmickStep()
{

}

//=========================================
//  ����������
//=========================================
HRESULT CGimmickStep::Init(void)
{
	// �e�N���X�̏�����
	if (FAILED(CGimmickAction::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ������Ԃ�
	return S_OK;
}

//=========================================
//  �I������
//=========================================
void CGimmickStep::Uninit(void)
{
	// �e�N���X�̏I��
	CGimmickAction::Uninit();
}

//=========================================
//  �X�V����
//=========================================
void CGimmickStep::Update(const float fDeltaTime)
{
	// �o��
	if (IsActive())
	{
		// �v���C���[�����擾
		CPlayer* player = GET_PLAYER;

		// �t���O���ꎞ�ۑ�
		bool bTemp = m_bClimb;

		// �㏸����
		if (!Climb(player) && bTemp)
		{
			// �v���C���[�ړ��ʂ�ݒ�
			D3DXVECTOR3 move = player->GetMove();
			move.y = 0.0f;
			player->SetMove(move);

			// �W�����v
			player->GimmickLowJump();

			// �t���O���X�V
			m_bClimb = false;
		}
	}

	// �e�N���X�̍X�V
	CGimmickAction::Update(fDeltaTime);
}

//=========================================
//  �`�揈��
//=========================================
void CGimmickStep::Draw(CShader* pShader)
{
	// �e�N���X�̕`��
	CGimmickAction::Draw(pShader);
}

//===========================================
//  �e���g���̑ҋ@�ʒu���Z�o
//===========================================
D3DXVECTOR3 CGimmickStep::CalcWaitPoint(const int Idx)
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }

	// �ҋ@�ʒu���擾
	D3DXVECTOR3 pos = GetActionPoint();

	// �����̔ԍ��ɍ��킹�č��������Z����
	pos.y += Idx * CPlayerClone::GetHeight();

	return pos;
}

//===========================================
//  �e���g���̑ҋ@�������Z�o
//===========================================
D3DXVECTOR3 CGimmickStep::CalcWaitRotation(const int Idx, const CPlayerClone* pClone)
{
	// �󂯎�����C���f�b�N�X���ő�l�𒴂��Ă���ꍇ�x��
	if (Idx > GetNumActive()) { assert(false); }

	// �ҋ@������ݒ�
	D3DXVECTOR3 rotWait = VEC3_ZERO;

	// �������擾
	EAngle angle = GetAngle();

	// y����ύX
	switch (angle)
	{
	case ANGLE_0:
		rotWait.y = D3DXToRadian(0.0f);
		break;
	case ANGLE_90:
		rotWait.y = D3DXToRadian(90.0f);
		break;
	case ANGLE_180:
		rotWait.y = D3DXToRadian(180.0f);
		break;
	case ANGLE_270:
		rotWait.y = D3DXToRadian(270.0f);
		break;
	default:
		break;
	}

	// �Z�o����������Ԃ�
	return rotWait;
}

//===========================================
//  �o��
//===========================================
bool CGimmickStep::Climb(CPlayer* player)
{
	// xz���ʂŐڐG���Ă��Ȃ��ꍇ�֐��𔲂���
	if (!DistancePlayer()) { return false; }

	// �v���C���[��y���W���擾
	float fPlayer = player->GetVec3Position().y;

	// ����̍��W���Z�o
	float fHeight = GetVec3Position().y + (CPlayerClone::GetHeight() * GetNumActive());

	// �v���C���[���������ɂ���ꍇ�֐��𔲂���
	if (fPlayer > fHeight) { return false; }

	// �v���C���[�̈ړ��ʂ����Z����
	D3DXVECTOR3 move = player->GetMove();
	move.y = CLIMB_SPEED;
	player->SetMove(move);

	// �t���O�𗧂Ă�
	m_bClimb = true;
	return m_bClimb;
}
