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

//===========================================
//  �萔��`
//===========================================
namespace
{
	const float CLIMB_MAX = 300.0f; // �o�����E�̍���
	const float CLIMB_SPEED = 200.0f; // 1�b�Ԃɓo�鑬�x
}

//=========================================
//  �R���X�g���N�^
//=========================================
CGimmickStep::CGimmickStep() : CGimmickAction(),
m_fSummit(0.0f), //�o���ʒu
m_bSummit(false) // �o���t���O
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
	// �v���C���[�̍��W���擾
	CPlayer* player = GET_PLAYER;

	// �v���C���[�Ƃ̓����蔻��
	if (CollisionPlayer())
	{
		// �o��
		Climb(player, fDeltaTime);
	}
	else
	{
		// ���g�̑�����\�ɂ���
		player->SetClone(true);
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
D3DXVECTOR3 CGimmickStep::CalcWaitPoint(const int Idx) const
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
//  �o��
//===========================================
void CGimmickStep::Climb(CPlayer* player, const float fDeltaTime)
{
	// �o������̍������Z�o
	float fSummit = GetVec3Position().y + (CPlayerClone::GetHeight() * GetNumActive());

	// �o�����Ă��鍂���Ȃ�֐��𔲂���
	float fHeight = player->GetVec3Position().y;
	if (fHeight > fSummit) { return; }

	// �v���C���[�̈ړ��ʂ��擾
	D3DXVECTOR3 movePlasyer = player->GetMove();

	// y�̈ړ��ʂ����Z
	movePlasyer.y = CLIMB_SPEED;

	// �ړ��ʂ�K�p����
	player->SetMove(movePlasyer);

	// �o���ł���ړ��ʂȂ�W�����v���Ċ֐��𔲂���
	if (fHeight + (movePlasyer.y * fDeltaTime) > fSummit)
	{
		// ���g�̑�����\�ɂ���
		player->SetClone(true);

		// �W�����v
		player->GimmickLowJump();
		return;
	}

	// ���g�̑����s�\�ɂ���
	player->SetClone(false);

	// ���g���폜����
	CPlayerClone::Delete();
}
