//=========================================
//
//  ���ݑ�M�~�b�N (gimmick_step.cpp)
//  Author : Tomoya kanazaki
//
//=========================================
#include "gimmick_step.h"
#include "manager.h"
#include "player.h"

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

	//�o������ʒu��ݒ�
	m_fSummit = GetVec3Position().y + CLIMB_MAX;

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
		if (!m_bSummit) // �o���ς݂łȂ��ꍇ
		{
			// �o��
			Climb(player);

			// �v���C���[�̍������r
			if (player->GetVec3Position().y > m_fSummit)
			{
				player->GimmickLowJump();
				m_bSummit = true;
			}
		}
		else
		{
			// �v���C���[�̍������r
			if (player->GetVec3Position().y < m_fSummit)
			{
				m_bSummit = false;
			}
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
//  �o��
//===========================================
void CGimmickStep::Climb(CPlayer* player)
{
	// �v���C���[�̈ړ��ʂ��擾
	D3DXVECTOR3 movePlasyer = player->GetMove();

	// y�̈ړ��ʂ����Z
	movePlasyer.y = CLIMB_SPEED;

	// �ړ��ʂ�K�p����
	player->SetMove(movePlasyer);
}
